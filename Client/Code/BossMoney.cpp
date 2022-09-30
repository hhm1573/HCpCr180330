#include "stdafx.h"
#include "..\Header\BossMoney.h"
#include "GameObjectMgr.h"
#include "Camera_Target.h"

#include "SoundManager.h"

CBossMoney::CBossMoney(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pCollisioner(nullptr)
	, m_fAccSpeed(0.f)
	, m_iState(0)
{
}

CBossMoney::CBossMoney(const CBossMoney & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
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

HRESULT CBossMoney::Initialize()
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

HRESULT CBossMoney::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_BossMoney");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_BossMoney", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CBossMoney::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CBossMoney::Update(const My_Float & fTimeDelta)
{
	if (1 == m_iState)
	{
		CSoundMgr::GetInstance()->PlaySound(L"EatCoin.mp3", CSoundMgr::Channel_Effect);
		return 1;
	}

	m_fAccTime += fTimeDelta * 3.f;

	if (m_vPosition.y >= 7.5f)
		m_vPosition.y -= 0.1f * m_fAccTime;

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &m_vPosition);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return My_Int();
}

My_Int CBossMoney::LastUpdate(const My_Float & fTimeDelta)
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

	m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(vec3[0] * 1.4f));
	m_pTransformCom->Set_Information(CTransform::INFO_UP, &(vec3[1] * 1.4f));
	m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &(vec3[2] * 1.4f));

	m_pCollisioner->Add_Collision(*m_pTransformCom->Get_Information(CTransform::INFO_POSITION), 1.0f, 20, &m_iState);


	return My_Int();
}

void CBossMoney::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CBossMoney::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x0f);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CBossMoney::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CBossMoney * CBossMoney::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBossMoney*	pInstance = new CBossMoney(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItem_Banana Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBossMoney::Clone()
{
	CBossMoney*	pInstance = new CBossMoney(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItem_Banana Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CBossMoney::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pCollisioner);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
