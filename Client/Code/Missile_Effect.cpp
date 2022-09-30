#include "stdafx.h"
#include "..\Header\Missile_Effect.h"
#include "GameObjectMgr.h"
#include "Item_Missile.h"


CMissile_Effect::CMissile_Effect(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
{
	m_vPos = { 0.f, 0.f, 0.f };
	m_vLook = { 0.f, 0.f, 0.f };
	m_szName = nullptr;
	m_fAccTime = 0.f;
	m_fFirstTime = 0.f;
	m_bIsFirst = true;
}

CMissile_Effect::CMissile_Effect(const CMissile_Effect & Ref)
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

	m_vPos = { 0.f, 0.f, 0.f };
	m_vLook = { 0.f, 0.f, 0.f };
	m_szName = nullptr;
	m_fAccTime = 0.f;
	m_fFirstTime = 0.f;
	m_bIsFirst = true;
}

HRESULT CMissile_Effect::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.5f, 0.5f, 0.5f);

	return NOERROR;
}

HRESULT CMissile_Effect::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Missile_Effect");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Missile_Effect", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMissile_Effect::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CMissile_Effect::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return My_Int();
}

My_Int CMissile_Effect::LastUpdate(const My_Float & fTimeDelta)
{
	const CGameObject* pMissile = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Item_Missile", 0);

	if (nullptr == pMissile)
		return 1;

	My_Vec3 vPosition = m_vPos - m_vLook * 10.f * fTimeDelta;
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(vPosition.x, vPosition.y + 0.1f, vPosition.z));

	Compute_ViewZ(m_pTransformCom->Get_Information(CTransform::INFO_POSITION));

	return My_Int();
}

void CMissile_Effect::Render()
{
	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CMissile_Effect::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CMissile_Effect::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CMissile_Effect * CMissile_Effect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMissile_Effect*	pInstance = new CMissile_Effect(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CMissile_Effect Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMissile_Effect::Clone()
{
	CMissile_Effect*		pInstance = new CMissile_Effect(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CMissile_Effect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CMissile_Effect::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);


	CGameObject::ReleaseMemory();
	return My_uLong();
}
