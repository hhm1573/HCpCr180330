#include "stdafx.h"
#include "..\Header\Terrain.h"
#include "Debugger.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pRendererCom(rhs.m_pRendererCom)
{
	/*if (nullptr != m_pTransformCom)
	m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
	m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
	m_pTextureCom->AddRef();*/
}

HRESULT CTerrain::Initialize_Prototype(void)
{
	return NOERROR;
}

HRESULT CTerrain::Initialize(void)
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return NOERROR;
}

My_Int CTerrain::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CTerrain::LastUpdate(const My_Float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (CDebugger::GetInstance()->Get_IsRender_Terrain() == true)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return My_Int();
}

void CTerrain::Render(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0);

	m_pBufferCom->Render();
}

HRESULT CTerrain::Initialize_Component(void)
{
	if (nullptr == m_pComponentMgr)
		return E_FAIL;

	// For.Com_Renderer
	m_pRendererCom = (CRenderer*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == m_pRendererCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	// For.Com_Transform
	m_pTransformCom = (CTransform*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == m_pTransformCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Buffer
	m_pBufferCom = (CTerrain_Buffer*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Buffer_Terrain");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Map");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain*	pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Component()))
	{
		MSG_BOX("CTerrain Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTerrain::Clone(void)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CTerrain Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

My_uLong CTerrain::ReleaseMemory(void)
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);


	CGameObject::ReleaseMemory();

	return My_uLong();
}
