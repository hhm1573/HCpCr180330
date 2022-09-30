#include "stdafx.h"
#include "..\Header\Devil_Skill.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"
#include "Crocodile.h"


CDevil_Skill::CDevil_Skill(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CDevil_Skill::CDevil_Skill(const CDevil_Skill & Ref)
	: CModel(Ref)
	, m_fAccTime(Ref.m_fAccTime)
	, m_pBossTransform(Ref.m_pBossTransform)
	, m_fFirstTime(Ref.m_fFirstTime)
	, m_bIsFirst(Ref.m_bIsFirst)
{
}

HRESULT CDevil_Skill::Initialize()
{
	if (FAILED(CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_")))
		return E_FAIL;

	m_pComponent_Transform->Scaling(2.f, 2.f, 2.f);

	m_pCollisioner = (CCollisioner*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (m_pCollisioner == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Collisioner", m_pCollisioner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDevil_Skill::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Item/Item_Devil_Client.dat");
}

My_Int CDevil_Skill::Update(const My_Float & fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	if (5.f < m_fAccTime)
		return 1;

	if (m_vPos.y >= 7.5f)
		m_vPos.y -= 10.f * m_fAccTime;

	m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &m_vPos);

	return My_Int();
}

My_Int CDevil_Skill::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	m_pCollisioner->Add_Collision(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION), 10.0f, 15, &m_iState);

	return My_Int();
}

void CDevil_Skill::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CDevil_Skill * CDevil_Skill::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDevil_Skill*		pInstance = new CDevil_Skill(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CDevil_Skill::Clone(void)
{
	CDevil_Skill*		pInstance = new CDevil_Skill(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CDevil_Skill::ReleaseMemory(void)
{
	Safe_Release(m_pCollisioner);
	CModel::ReleaseMemory();

	return My_uLong();
}
