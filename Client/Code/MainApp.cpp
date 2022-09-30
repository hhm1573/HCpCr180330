#include "stdafx.h"
#include "..\Header\MainApp.h"

#include "GraphicDev.h"
#include "InputDev.h"

#include "SceneMgr.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"

#include "Debugger.h"

#include "Loading.h"

#include "Camera_Free.h"
#include "SoundManager.h"

CMainApp::CMainApp()
{
	m_pGraphic_Interface = CGraphicDev::GetInstance();
	m_pGraphic_Interface->AddRef();

	m_pSceneMgr = CSceneMgr::GetInstance();
	m_pSceneMgr->AddRef();

	m_pComponentMgr = CComponentMgr::GetInstance();
	m_pComponentMgr->AddRef();

	m_pGameObjectMgr = CGameObjectMgr::GetInstance();
	m_pGameObjectMgr->AddRef();

	m_pDebugger = CDebugger::GetInstance();
	m_pDebugger->AddRef();
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Initialize()
{
	CSoundMgr::GetInstance()->Initialize();

	if (FAILED(Initialize_DefaultSetting(CGraphicDev::MODE_WIN, g_iBackCX, g_iBackCY)))
		return E_FAIL;


	if (FAILED(Initialize_Component_Prototype()))
		return E_FAIL;

	if (FAILED(Initialize_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Initialize_GameObject()))
		return E_FAIL;


	if (FAILED(Initialize_StartScene(SCENE_LOADING)))
		return E_FAIL;

	return NOERROR;
}

My_Int CMainApp::Update(const My_Float & fTimeDelta)
{
	if (nullptr == m_pSceneMgr)
		return -1;

	CSoundMgr::GetInstance()->Update();

	CInputDev::GetInstance()->SetUp_Input_Device_State();
	m_pDebugger->Update();

	return m_pSceneMgr->Update(fTimeDelta);
}

void CMainApp::Render()
{
	if (nullptr == m_pGraphicDev
		|| nullptr == m_pSceneMgr)
		return;

	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	m_pGraphicDev->BeginScene();

	Render_GameObject(SCENE_STATIC, L"Component_Renderer");

	m_pSceneMgr->Render();
	m_pDebugger->Render();

	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(nullptr, nullptr, nullptr, nullptr);
}

HRESULT CMainApp::Initialize_DefaultSetting(CGraphicDev::WIN_MODE eMode, const My_uShort & wBackCX, const My_uShort & wBackCY)
{
	if (FAILED(CInputDev::GetInstance()->Ready_Input_Device(g_hInst, g_hWnd)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Interface->Initialize_Graphic_Device(eMode, g_hWnd, wBackCX, wBackCY)))
		return E_FAIL;
	m_pGraphicDev = m_pGraphic_Interface->Get_Graphic_Device();
	m_pGraphicDev->AddRef();

	if (FAILED(Initialize_DefaultRenderState()))
		return E_FAIL;


	if (FAILED(CComponentMgr::GetInstance()->Reserve(SCENE_END)))
		return E_FAIL;

	if (FAILED(CGameObjectMgr::GetInstance()->Reserve(SCENE_END)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainApp::Initialize_DefaultRenderState()
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DMATERIAL9	Material;
	memset(&Material, 0, sizeof(D3DMATERIAL9));
	Material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	Material.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	Material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pGraphicDev->SetMaterial(&Material);

	return NOERROR;
}

HRESULT CMainApp::Initialize_StartScene(SCENEID eID)
{
	CScene*			pScene = nullptr;

	switch (eID)
	{
	case SCENE_LOADING:
		pScene = CLoading::Create(m_pGraphicDev);
		break;
	case SCENE_STAGE:
		break;
	}

	if (nullptr == pScene)
		return E_FAIL;

	if (nullptr == m_pSceneMgr)
		return E_FAIL;

	if (FAILED(m_pSceneMgr->SetUp_CurrentScene(pScene)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainApp::Initialize_Component_Prototype()
{
	CComponent*			pComponent = nullptr;

	// For.Component_Renderer
	pComponent = Engine::CRenderer::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_STATIC, L"Component_Renderer", pComponent)))
		return E_FAIL;

	//Transform
	pComponent = Engine::CTransform::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_STATIC, L"Component_Transform", pComponent)))
		return E_FAIL;



	{	//Buffer

		//TriCol
		pComponent = Engine::CTriCol::Create(m_pGraphicDev);
		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(m_pComponentMgr->Add_Component(SCENE_STATIC, L"Component_Buffer_TriCol", pComponent)))
			return E_FAIL;

		//RectCol
		pComponent = Engine::CRectCol::Create(m_pGraphicDev);
		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(m_pComponentMgr->Add_Component(SCENE_STATIC, L"Component_Buffer_RectCol", pComponent)))
			return E_FAIL;

		//RectTex
		pComponent = Engine::CRectTex::Create(m_pGraphicDev);
		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(m_pComponentMgr->Add_Component(SCENE_STATIC, L"Component_Buffer_RectTex", pComponent)))
			return E_FAIL;

		//CubeCol
		pComponent = Engine::CCubeCol::Create(m_pGraphicDev);
		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(m_pComponentMgr->Add_Component(SCENE_STATIC, L"Component_Buffer_CubeCol", pComponent)))
			return E_FAIL;

		//CubeTex
		pComponent = Engine::CCubeTex::Create(m_pGraphicDev);
		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(m_pComponentMgr->Add_Component(SCENE_STATIC, L"Component_Buffer_CubeTex", pComponent)))
			return E_FAIL;

		//Terrain
		/*pComponent = Engine::CTerrain_Buffer::Create(m_pGraphicDev,L"../Bin/Resources/Textures/Terrain/Height.bmp",1.0f);
		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(m_pComponentMgr->Add_Component(SCENE_STATIC, L"Component_Buffer_Terrain", pComponent)))
			return E_FAIL;*/
	}

	return NOERROR;
}

HRESULT CMainApp::Initialize_GameObject_Prototype()
{
	CAMERA_DESC tCameraDesc;
	tCameraDesc.vEye = D3DXVECTOR3(0.0f, 10.0f, -20.0f);
	tCameraDesc.vAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	tCameraDesc.vAxisY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	PROJ_DESC tProjDesc;
	tProjDesc.fFovy = D3DXToRadian(60.0f);
	tProjDesc.fAspect = My_Float(g_iBackCX) / g_iBackCY;
	tProjDesc.fFar = 1000.0f;
	tProjDesc.fNear = 0.1f;
	CGameObject* pObject = (CGameObject*)CCamera_Free::Create(m_pGraphicDev, &tCameraDesc, &tProjDesc);
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pGameObjectMgr->Add_Object_Prototype(SCENE_STATIC, L"Camera_Free", pObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainApp::Initialize_GameObject()
{
	CGameObject* pObject = (CGameObject*)m_pGameObjectMgr->Clone_GameObject(SCENE_STATIC, L"Camera_Free");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pGameObjectMgr->Add_Object(SCENE_STATIC, L"Camera_Free", pObject)))
		return E_FAIL;

	return NOERROR;
}

void CMainApp::Render_GameObject(SCENEID eID, const My_TChar * RendererTag)
{
	if (nullptr == m_pRenderer)
		m_pRenderer = (CRenderer*)m_pComponentMgr->Clone_Component(eID, RendererTag);

	if (nullptr != m_pRenderer)
		m_pRenderer->Render_Objects();
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CMainApp Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

My_uLong CMainApp::ReleaseMemory()
{
	Safe_Release(m_pDebugger);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pComponentMgr);
	Safe_Release(m_pGameObjectMgr);
	Safe_Release(m_pSceneMgr);
	Safe_Release(m_pGraphic_Interface);
	Safe_Release(m_pGraphicDev);
	return My_uLong();
}
