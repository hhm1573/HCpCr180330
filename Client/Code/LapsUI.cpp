#include "stdafx.h"
#include "..\Header\LapsUI.h"

#include "RankingManager.h"

CLapsUI::CLapsUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
{
}

CLapsUI::CLapsUI(const CLapsUI & Ref)
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
}

HRESULT CLapsUI::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLapsUI::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Laps");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Laps", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLapsUI::Initialize_Prototype()
{
	return NOERROR;

}

My_Int CLapsUI::Update(const My_Float & fTimeDelta)
{
	if (!mIsAward)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return My_Int();
}

My_Int CLapsUI::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CLapsUI::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	Set_RenderState();

	My_Int laps = CRankingManager::GetInstance()->GetLaps();

	if (laps >= 3)
		laps = 3;

	m_pTextureCom->SetUp_OnGraphicDev(laps - 1);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 117.f, 82.f, 0.f));
	m_pTransformCom->Scaling(45.f, 54.f, 0.f);
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTextureCom->SetUp_OnGraphicDev(3);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 85.f, 90.f, 0.f));
	m_pTransformCom->Scaling(25.f, 34.f, 0.f);
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTextureCom->SetUp_OnGraphicDev(2);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 60.f, 90.f, 0.f));
	m_pTransformCom->Scaling(25.f, 34.f, 0.f);
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	Release_RenderState();
}

void CLapsUI::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CLapsUI::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CLapsUI * CLapsUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLapsUI*	pInstance = new CLapsUI(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CLapsUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLapsUI::Clone()
{
	CLapsUI*	pInstance = new CLapsUI(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CLapsUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CLapsUI::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
