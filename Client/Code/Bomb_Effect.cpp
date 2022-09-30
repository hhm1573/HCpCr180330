#include "stdafx.h"
#include "..\Header\Bomb_Effect.h"
#include "GameObjectMgr.h"
#include "Item_Missile.h"


CBomb_Effect::CBomb_Effect(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
{
	m_vPos = { 0.f, 0.f, 0.f };
	m_vLook = { 0.f, 0.f, 0.f };
	m_vScale = { 0.f, 0.f, 0.f };
	m_szName = nullptr;
	m_fAccTime = 0.f;
	m_fFirstTime = 0.f;
	m_bIsFirst = true;
	m_fY = 0.f;
}

CBomb_Effect::CBomb_Effect(const CBomb_Effect & Ref)
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
	m_vScale = { 0.f, 0.f, 0.f };
	m_szName = nullptr;
	m_fAccTime = 0.f;
	m_fFirstTime = 0.f;
	m_bIsFirst = true;
	m_fY = 0.f;
}

HRESULT CBomb_Effect::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_vScale = { 20.f, 10.f, 10.f };

	return NOERROR;
}

HRESULT CBomb_Effect::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Bomb_Effect");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Bomb_Effect", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CBomb_Effect::Initialize_Prototype()
{
	return NOERROR;

}

My_Int CBomb_Effect::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	m_vScale.x -= 0.4f;
	m_vScale.y -= 0.2f;
	m_vScale.z -= 0.2f;

	m_fY += 0.1f;

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(m_vPos.x, m_vPos.y - m_fY, m_vPos.z));

	return My_Int();
}

My_Int CBomb_Effect::LastUpdate(const My_Float & fTimeDelta)
{
	if (m_vScale.x < 0.f)
		return 1;

	/*ºôº¸µå*/
	My_Matrix	matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, nullptr, &matView);

	My_Vec3 vec3[3];

	vec3[0] = *(My_Vec3*)&matView.m[CTransform::INFO_RIGHT][0];
	vec3[1] = *(My_Vec3*)&matView.m[CTransform::INFO_UP][0];
	vec3[2] = *(My_Vec3*)&matView.m[CTransform::INFO_LOOK][0];

	D3DXVec3Normalize(&vec3[0], &vec3[0]);
	D3DXVec3Normalize(&vec3[1], &vec3[1]);
	D3DXVec3Normalize(&vec3[2], &vec3[2]);

	m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(vec3[0]/* * 1.4f*/));
	m_pTransformCom->Set_Information(CTransform::INFO_UP, &(vec3[1] /** 1.4f*/));
	m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &(vec3[2] /** 1.4f*/));

	m_pTransformCom->Scaling(m_vScale.x, m_vScale.y, m_vScale.z);

	Compute_ViewZ(m_pTransformCom->Get_Information(CTransform::INFO_POSITION));

	return My_Int();
}

void CBomb_Effect::Render()
{
	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CBomb_Effect::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CBomb_Effect::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CBomb_Effect * CBomb_Effect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBomb_Effect*	pInstance = new CBomb_Effect(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CBomb_Effect Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBomb_Effect::Clone()
{
	CBomb_Effect*		pInstance = new CBomb_Effect(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CBomb_Effect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CBomb_Effect::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);


	CGameObject::ReleaseMemory();
	return My_uLong();
}
