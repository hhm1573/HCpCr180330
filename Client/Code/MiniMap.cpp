#include "stdafx.h"
#include "..\Header\MiniMap.h"

#include "GameObjectMgr.h"

#include "Camera_Target.h"
#include "Camera_MiniMap.h"

CMiniMap::CMiniMap(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, mCamTarget(nullptr)
	, mCamMiniMap(nullptr)
{
	D3DXMatrixIdentity(&mTargetViewMat);
	D3DXMatrixIdentity(&mTargetProjMat);
	D3DXMatrixIdentity(&mMiniMapViewMat);
	D3DXMatrixIdentity(&mMiniMapProjMat);
}

CMiniMap::CMiniMap(const CMiniMap & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, mCamTarget(Ref.mCamTarget)
	, mCamMiniMap(Ref.mCamMiniMap)
	, mOriBackBufferSurface(Ref.mOriBackBufferSurface)
	, mMiniMapSurface(Ref.mMiniMapSurface)
	, mMiniMapTexture(Ref.mMiniMapTexture)
	, mTargetViewMat(Ref.mTargetViewMat)
	, mTargetProjMat(Ref.mTargetProjMat)
	, mMiniMapViewMat(Ref.mMiniMapViewMat)
	, mMiniMapProjMat(Ref.mMiniMapProjMat)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();
}

HRESULT CMiniMap::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 180.f, g_iBackCY * 0.5f, 0.f));
	m_pTransformCom->Scaling(280.f, 280.f, 0.f);

	m_pDevice->GetRenderTarget(0, &mOriBackBufferSurface);

	D3DSURFACE_DESC tempDesc;
	mOriBackBufferSurface->GetDesc(&tempDesc);
	D3DXCreateTexture(m_pDevice, tempDesc.Width, tempDesc.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &mMiniMapTexture);
	mMiniMapTexture->GetSurfaceLevel(0, &mMiniMapSurface);

	mCamTarget = static_cast<const CCamera*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_Target", 0));
	mCamMiniMap = static_cast<const CCamera*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_MiniMap", 0));

	return NOERROR;
}

HRESULT CMiniMap::Initialize_Component()
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

	return NOERROR;
}

void CMiniMap::SetRenderTargetMiniMap()
{
	Safe_Release(mOriBackBufferSurface);

	m_pDevice->GetRenderTarget(0, &mOriBackBufferSurface);
	m_pDevice->SetRenderTarget(0, mMiniMapSurface);

	mMiniMapViewMat = mCamMiniMap->GetViewMat();
	mMiniMapProjMat = mCamMiniMap->GetProjMat();

	m_pDevice->SetTransform(D3DTS_VIEW, &mMiniMapViewMat);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &mMiniMapProjMat);
}

void CMiniMap::ReleaseRenderTargetMiniMap()
{
	Safe_Release(mMiniMapSurface);

	m_pDevice->GetRenderTarget(0, &mMiniMapSurface);
	m_pDevice->SetRenderTarget(0, mOriBackBufferSurface);

	mTargetViewMat = mCamTarget->GetViewMat();
	mTargetProjMat = mCamTarget->GetProjMat();

	m_pDevice->SetTransform(D3DTS_VIEW, &mTargetViewMat);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &mTargetProjMat);
}

HRESULT CMiniMap::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CMiniMap::Update(const My_Float & fTimeDelta)
{
	if (!mIsAward)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return My_Int();
}

My_Int CMiniMap::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CMiniMap::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pDevice->SetTexture(0, mMiniMapTexture);

	Set_RenderState();
	m_pBufferCom->Render();
	Release_RenderState();

	SetRenderTargetMiniMap();
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 0.f, 0.5f), 1.f, 0);
	ReleaseRenderTargetMiniMap();
}

void CMiniMap::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CMiniMap::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CMiniMap * CMiniMap::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMiniMap*	pInstance = new CMiniMap(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CMiniMap Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMiniMap::Clone()
{
	CMiniMap*	pInstance = new CMiniMap(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CMiniMap Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CMiniMap::ReleaseMemory()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	Safe_Release(mMiniMapTexture);
	Safe_Release(mOriBackBufferSurface);
	Safe_Release(mMiniMapSurface);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
