#include "stdafx.h"
#include "..\Header\Item_Banana.h"
#include "GameObjectMgr.h"
#include "Camera_Target.h"


CItem_Banana::CItem_Banana(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pPlayerTransform(nullptr)
	, m_pCollisioner(nullptr)
	, m_fAccSpeed(0.f)
	, m_iState(0)
{
}

CItem_Banana::CItem_Banana(const CItem_Banana & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, m_pPlayerTransform(Ref.m_pPlayerTransform)
	, m_fAccSpeed(Ref.m_fAccSpeed)
	, m_pCollisioner(Ref.m_pCollisioner)
	, m_iState(Ref.m_iState)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

void CItem_Banana::Set_Player_Transform(CTransform * pPlayerTransform)
{
	m_pPlayerTransform = pPlayerTransform;

	My_Vec3 vPlayerPosition = *m_pPlayerTransform->Get_Information(CTransform::INFO_POSITION);
	My_Vec3 vPlayerLook = *m_pPlayerTransform->Get_Information(CTransform::INFO_LOOK);

	vPlayerPosition = My_Vec3(vPlayerPosition.x, vPlayerPosition.y + 0.5f, vPlayerPosition.z);

	if (true == m_bIsBoss)
	{

	}
	else
	{
		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(vPlayerPosition + vPlayerLook * (-18.f)));
	}
}

HRESULT CItem_Banana::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pCollisioner = (CCollisioner*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (m_pCollisioner == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Collisioner", m_pCollisioner)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CItem_Banana::Initialize_Component()
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

	// For.Com_Buffer_RectTex
	m_pBufferCom = (CRectTex*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Buffer_RectTex");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Buffer_RectTex", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Item_Banana");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Item_Banana", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CItem_Banana::Initialize_Prototype()
{

	return NOERROR;

}

My_Int CItem_Banana::Update(const My_Float & fTimeDelta)
{
	if (1 == m_iState || 8 == m_iState || 5 == m_iState)
		return 1;

	if (true == m_bIsBoss)
	{
		m_fAccTime += fTimeDelta;

		if (5.f < m_fAccTime)
			return 1;

		if (m_vPosition.y >= 7.5f)
			m_vPosition.y -= 0.1f * m_fAccTime;

		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &m_vPosition);
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return My_Int();
}

My_Int CItem_Banana::LastUpdate(const My_Float & fTimeDelta)
{
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

	m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(vec3[0] * 1.4f) );
	m_pTransformCom->Set_Information(CTransform::INFO_UP, &(vec3[1] * 1.4f));
	m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &(vec3[2] * 1.4f));

	m_pCollisioner->Add_Collision(*m_pTransformCom->Get_Information(CTransform::INFO_POSITION), 1.0f, 3, &m_iState);


	return My_Int();
}

void CItem_Banana::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CItem_Banana::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x0f);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CItem_Banana::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CItem_Banana * CItem_Banana::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem_Banana*	pInstance = new CItem_Banana(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItem_Banana Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_Banana::Clone()
{
	CItem_Banana*	pInstance = new CItem_Banana(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItem_Banana Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CItem_Banana::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pCollisioner);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
