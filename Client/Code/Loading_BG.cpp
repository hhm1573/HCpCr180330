#include "stdafx.h"
#include "..\Header\Loading_BG.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"
#include "SceneMgr.h"
#include "Stage.h"


CLoading_BG::CLoading_BG(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CLoading_BG::CLoading_BG(const CLoading_BG & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, m_pTexture(Ref.m_pTexture)
	, m_tFrame(Ref.m_tFrame)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CLoading_BG::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_tFrame = FRAME(0, 12, 119);

	return NOERROR;
}

HRESULT CLoading_BG::Initialize_Component()
{
	if (nullptr == m_pComponentMgr)
		return E_FAIL;

	// For.Com_Renderer
	m_pRendererCom = (CRenderer*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == m_pRendererCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Renderer", m_pRendererCom)))
		return E_FAIL;

	// For.Com_Transform
	m_pTransformCom = (CTransform*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == m_pTransformCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Buffer_ViewPort
	m_pBufferCom = (CViewPort_Buffer*)m_pComponentMgr->Clone_Component(SCENE_LOADING, L"Component_Buffer_ViewPort");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Buffer_ViewPort", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_LOADING, L"Component_Texture_Loading");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Loading", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLoading_BG::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CLoading_BG::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return My_Int();
}

My_Int CLoading_BG::LastUpdate(const My_Float & fTimeDelta)
{
	if (m_tFrame.fFrame < 118)
		m_tFrame.fFrame += m_tFrame.fCount * fTimeDelta;
	else
		m_tFrame.fFrame = 118;

	if (118 <= m_tFrame.fFrame && m_bIsLoading == false)
	{
		CGameObject*		pGameObject = nullptr;

		// For.Object_Loading_BG
		pGameObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_LOADING, L"Loading_Img");
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_LOADING, L"Loading_Img", pGameObject)))
			return E_FAIL;

		m_bIsLoading = true;
	}
	return My_Int();
}

void CLoading_BG::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pTransformCom->Scaling(1280.f, 960.f, 0.f);

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(640.f, 480.f, 0.f));

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev((My_uInt)m_tFrame.fFrame);

	m_pBufferCom->Render();
}

CLoading_BG * CLoading_BG::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLoading_BG*	pInstance = new CLoading_BG(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CLoading_BG Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLoading_BG::Clone()
{
	CLoading_BG*	pInstance = new CLoading_BG(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CLoading_BG Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CLoading_BG::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
