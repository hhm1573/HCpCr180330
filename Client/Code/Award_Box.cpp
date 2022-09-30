#include "stdafx.h"
#include "..\Header\Award_Box.h"
#include "GameObjectMgr.h"



CAward_Box::CAward_Box(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
{
}

CAward_Box::CAward_Box(const CAward_Box & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
{
}

HRESULT CAward_Box::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(600.f, 200.f, 0.f);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX / 2.f, g_iBackCY / 2.f + 100.f, 0.f));

	return NOERROR;
}

HRESULT CAward_Box::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Award_Box");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Award_Box", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CAward_Box::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CAward_Box::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_CLOUD, this);

	return My_Int();
}

My_Int CAward_Box::LastUpdate(const My_Float & fTimeDelta)
{

	return My_Int();
}

void CAward_Box::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CAward_Box::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CAward_Box::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CAward_Box * CAward_Box::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CAward_Box*	pInstance = new CAward_Box(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CAward_Box Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAward_Box::Clone()
{
	CAward_Box*	pInstance = new CAward_Box(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CAward_Box Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CAward_Box::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
