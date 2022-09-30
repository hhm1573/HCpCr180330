#include "stdafx.h"
#include "..\Header\Booster_Effect.h"
#include "GameObjectMgr.h"
#include "Camera_Target.h"
#include "Engine_Effect.h"


CBooster_Effect::CBooster_Effect(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
{
	m_pCamrea = nullptr;
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CBooster_Effect::CBooster_Effect(const CBooster_Effect & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, m_pCamrea(Ref.m_pCamrea)
	, m_tFrame(Ref.m_tFrame)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CBooster_Effect::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_tFrame = FRAME(0, 14, 7);

	m_pTransformCom->Scaling(g_iBackCX, g_iBackCY, 0.f);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX / 2.f, g_iBackCY / 2.f, 0.f));

	// 카메라 얻어오기
	m_pCamrea = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_Target", 0));

	return NOERROR;
}

HRESULT CBooster_Effect::Initialize_Component()
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
	m_pBufferCom = (CViewPort_Buffer*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Buffer_ViewPort");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Buffer_ViewPort", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Booster_Effect");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Booster_Effect", m_pTextureCom)))
		return E_FAIL;

	// Engine_Effect
	CGameObject*  pGameObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Engine_Effect");
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Engine_Effect", pGameObject)))
		return E_FAIL;
	dynamic_cast<CEngine_Effect*>(pGameObject)->Set_Name(L"CartBox_Engine_Left");

	pGameObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Engine_Effect");
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Engine_Effect", pGameObject)))
		return E_FAIL;
	dynamic_cast<CEngine_Effect*>(pGameObject)->Set_Name(L"CartBox_Engine_Right");

	return NOERROR;
}

HRESULT CBooster_Effect::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CBooster_Effect::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOOSTER, this);

	return My_Int();
}

My_Int CBooster_Effect::LastUpdate(const My_Float & fTimeDelta)
{
	m_tFrame.fFrame += m_tFrame.fCount * fTimeDelta;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0;
	}

	if (3 == dynamic_cast<CCamera_Target*>(m_pCamrea)->Get_Booster())
	{
		return 1;
	}

	return My_Int();
}

void CBooster_Effect::Render()
{
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev((My_uInt)m_tFrame.fFrame);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CBooster_Effect::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CBooster_Effect::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CBooster_Effect * CBooster_Effect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBooster_Effect*	pInstance = new CBooster_Effect(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CBooster_Effect Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBooster_Effect::Clone()
{
	CBooster_Effect*		pInstance = new CBooster_Effect(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CBooster_Effect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CBooster_Effect::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);


	CGameObject::ReleaseMemory();

	return My_uLong();
}
