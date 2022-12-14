#include "stdafx.h"
#include "..\Header\Dao_Ani.h"
#include "GameObjectMgr.h"



CDao_Ani::CDao_Ani(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
{
}

CDao_Ani::CDao_Ani(const CDao_Ani & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
{
}

void CDao_Ani::SetRanking(My_Int rank)
{
	if (0 == rank)
		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX / 2.f - 5.f, g_iBackCY / 2.f - 50.f, 0.f));

	else if (1 == rank)
		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX / 2.f - 200.f, g_iBackCY / 2.f - 10.f, 0.f));

	else if (2 == rank)
		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX / 2.f + 195.f, g_iBackCY / 2.f + 25.f, 0.f));
}

HRESULT CDao_Ani::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_tFrame = FRAME(0, 16, 16);
	m_pTransformCom->Scaling(180.f, 180.f, 0.f);
	// 2?? ?ڸ?
	// m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX / 2.f - 200.f, g_iBackCY / 2.f - 10.f, 0.f));

	return NOERROR;
}

HRESULT CDao_Ani::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Dao_Ani");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Dao_Ani", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CDao_Ani::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CDao_Ani::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return My_Int();
}

My_Int CDao_Ani::LastUpdate(const My_Float & fTimeDelta)
{
	m_tFrame.fFrame += m_tFrame.fCount * fTimeDelta;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0;
	}

	return My_Int();
}

void CDao_Ani::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev((My_uInt)m_tFrame.fFrame);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CDao_Ani::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CDao_Ani::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CDao_Ani * CDao_Ani::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDao_Ani*	pInstance = new CDao_Ani(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CDao_Ani Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDao_Ani::Clone()
{
	CDao_Ani*	pInstance = new CDao_Ani(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CDao_Ani Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CDao_Ani::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
