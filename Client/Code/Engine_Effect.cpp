#include "stdafx.h"
#include "..\Header\Engine_Effect.h"
#include "GameObjectMgr.h"
#include "Camera_Target.h"
#include "Bazzi.h"


CEngine_Effect::CEngine_Effect(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	m_vPos = { 0.f, 0.f, 0.f };
	m_vLook = { 0.f, 0.f, 0.f };
	m_szName = nullptr;
	m_fAccTime = 0.f;
	m_fFirstTime = 0.f;
	m_bIsFirst = true;
}

CEngine_Effect::CEngine_Effect(const CEngine_Effect & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();

	ZeroMemory(&m_tFrame, sizeof(FRAME));
	m_vPos = { 0.f, 0.f, 0.f };
	m_vLook = { 0.f, 0.f, 0.f };
	m_szName = nullptr;
	m_fAccTime = 0.f;
	m_fFirstTime = 0.f;
	m_bIsFirst = true;
}

HRESULT CEngine_Effect::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_tFrame = FRAME(0, 4, 2);

	m_pTransformCom->Scaling(0.5f, 0.5f, 0.5f);

	return NOERROR;
}

HRESULT CEngine_Effect::Initialize_Component()
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
	m_pBufferCom = (CRectTex*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Buffer_RectTex");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Buffer_RectTex", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Engine_Effect");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Engine_Effect", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CEngine_Effect::Initialize_Prototype()
{
	return NOERROR;

}

My_Int CEngine_Effect::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	m_tFrame.fFrame += m_tFrame.fCount * fTimeDelta;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0;
	}

	return My_Int();
}

My_Int CEngine_Effect::LastUpdate(const My_Float & fTimeDelta)
{
	const CGameObject* pBooster = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Booster_Effect", 0);

	if (nullptr == pBooster)
		return 1;

	CGameObject* pBazzi = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0));
	CTransform*	pTransform = dynamic_cast<CBazzi*>(pBazzi)->Get_Transform();
	My_Vec3		vRight = *pTransform->Get_Information(CTransform::INFO_RIGHT);
	My_Vec3		vPos = dynamic_cast<CBazzi*>(pBazzi)->Get_ModelPos(m_szName);
	My_Vec3		vUp = *pTransform->Get_Information(CTransform::INFO_UP);
	My_Vec3		vLook = *pTransform->Get_Information(CTransform::INFO_LOOK);

	m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(vRight * 4.f));
	m_pTransformCom->Set_Information(CTransform::INFO_UP, &(vUp * 4.f));
	m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &(vLook * 4.f));

	My_Vec3 vPosition = vPos - vLook * 1.f;
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(vPosition.x, vPosition.y + 0.1f, vPosition.z));

	Compute_ViewZ(m_pTransformCom->Get_Information(CTransform::INFO_POSITION));

	return My_Int();
}

void CEngine_Effect::Render()
{
	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev((My_uInt)m_tFrame.fFrame);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CEngine_Effect::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CEngine_Effect::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CEngine_Effect * CEngine_Effect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEngine_Effect*	pInstance = new CEngine_Effect(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CEngine_Effect Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEngine_Effect::Clone()
{
	CEngine_Effect*		pInstance = new CEngine_Effect(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CEngine_Effect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CEngine_Effect::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);


	CGameObject::ReleaseMemory();

	return My_uLong();
}
