#include "stdafx.h"
#include "..\Header\Boss_HP.h"


CBoss_HP::CBoss_HP(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_vPosition(0.f, 0.f, 0.f)
{
}

CBoss_HP::CBoss_HP(const CBoss_HP & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, m_vPosition(Ref.m_vPosition)
	, m_bIsCol(Ref.m_bIsCol)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CBoss_HP::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(640.f, 50.f, 0.f));
	m_pTransformCom->Scaling(50.f, 50.f, 0.f);

	return NOERROR;
}

HRESULT CBoss_HP::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Heart");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Heart", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CBoss_HP::Initialize_Prototype()
{
	return NOERROR;

}

My_Int CBoss_HP::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_CLOUD, this);

	return My_Int();
}

My_Int CBoss_HP::LastUpdate(const My_Float & fTimeDelta)
{
	if (true == m_bIsCol)
		return 1;

	return My_Int();
}

void CBoss_HP::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CBoss_HP::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CBoss_HP::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

CBoss_HP * CBoss_HP::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoss_HP*	pInstance = new CBoss_HP(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CBoss_HP Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBoss_HP::Clone()
{
	CBoss_HP*	pInstance = new CBoss_HP(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CBoss_HP Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CBoss_HP::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
