#include "stdafx.h"
#include "..\Header\RankingUI.h"

CRankingUI::CRankingUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, mRanking(nullptr)
	, mPlayerRank(0)
{
}

CRankingUI::CRankingUI(const CRankingUI & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, mRanking(Ref.mRanking)
	, mPlayerRank(Ref.mPlayerRank)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CRankingUI::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	mRanking = CRankingManager::GetInstance()->GetRankingVector();

	return NOERROR;
}

HRESULT CRankingUI::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Ranking");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Ranking", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CRankingUI::Initialize_Prototype()
{
	return NOERROR;

}

My_Int CRankingUI::Update(const My_Float & fTimeDelta)
{
	if (!mIsAward)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	for (int i = 0; i < 3; ++i)
	{
		if ((*mRanking)[i] == CRankingManager::BAZZI)
		{
			mPlayerRank = i;
			break;
		}
	}
	return My_Int();
}

My_Int CRankingUI::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CRankingUI::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	Set_RenderState();

	// Ranking
	m_pTransformCom->Scaling(64.f, 64.f, 0.f);

	m_pTextureCom->SetUp_OnGraphicDev(mPlayerRank + 3);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(55.f, g_iBackCY * 0.5f - 155.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTransformCom->Scaling(32.f, 32.f, 0.f);
	m_pTextureCom->SetUp_OnGraphicDev(6);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(100.f, g_iBackCY * 0.5f - 140.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	// Render Ranking Name
	m_pTransformCom->Scaling(100.f, 25.f, 0.f);

	m_pTextureCom->SetUp_OnGraphicDev((*mRanking)[0]);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(70.f, g_iBackCY * 0.5f - 105.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTextureCom->SetUp_OnGraphicDev((*mRanking)[1]);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(70.f, g_iBackCY * 0.5f - 70, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTextureCom->SetUp_OnGraphicDev((*mRanking)[2]);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(70.f, g_iBackCY * 0.5f - 35.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	Release_RenderState();
}

void CRankingUI::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CRankingUI::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CRankingUI * CRankingUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRankingUI*	pInstance = new CRankingUI(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CRankingUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRankingUI::Clone()
{
	CRankingUI*	pInstance = new CRankingUI(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CRankingUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CRankingUI::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
