#include "stdafx.h"
#include "..\Header\MiniMapIcon.h"

#include "GameObjectMgr.h"
#include "MiniMap.h"

CMiniMapIcon::CMiniMapIcon(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, mTarget(nullptr)
	, mMiniMap(nullptr)
{
}

CMiniMapIcon::CMiniMapIcon(const CMiniMapIcon & rhs)
	: CGameObject(rhs)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pRendererCom(rhs.m_pRendererCom)
	, mTarget(rhs.mTarget)
	, mMiniMap(rhs.mMiniMap)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CMiniMapIcon::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	mMiniMap = static_cast<const CMiniMap*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_MiniMap", 0));

	return NOERROR;
}

HRESULT CMiniMapIcon::Initialize_Component()
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

	//// For.Com_Texture
	//m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_MiniMap_Player");
	//if (nullptr == m_pTextureCom)
	//	return E_FAIL;
	//if (FAILED(CGameObject::Add_Component(L"Component_Texture_MiniMap_Player", m_pTextureCom)))
	//	return E_FAIL;


	return NOERROR;
}

HRESULT CMiniMapIcon::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CMiniMapIcon::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CMiniMapIcon::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CMiniMapIcon::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	const_cast<CMiniMap*>(mMiniMap)->SetRenderTargetMiniMap();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	const_cast<CMiniMap*>(mMiniMap)->ReleaseRenderTargetMiniMap();

}

void CMiniMapIcon::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
}

void CMiniMapIcon::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

My_uLong CMiniMapIcon::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
