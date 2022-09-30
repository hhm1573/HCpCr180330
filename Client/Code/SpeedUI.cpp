#include "stdafx.h"
#include "..\Header\SpeedUI.h"

#include "GameObjectMgr.h"
#include "Bazzi.h"

CSpeedUI::CSpeedUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
{
	ZeroMemory(&mFrame, sizeof(FRAME));
	mPlayerSpeed = 0.f;
	mPlayer = nullptr;
}

CSpeedUI::CSpeedUI(const CSpeedUI & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, mFrame(Ref.mFrame)
	, mPlayerSpeed(Ref.mPlayerSpeed)
	, mPlayer(Ref.mPlayer)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CSpeedUI::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	mPlayer = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX * 0.5f, g_iBackCY - 55.f, 0.f));
	m_pTransformCom->Scaling(300.f, 111.f, 0.f);

	return NOERROR;
}

HRESULT CSpeedUI::Initialize_Component()
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

	//// For.Com_Texture
	//m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_ItemSlot");
	//if (nullptr == m_pTextureCom)
	//	return E_FAIL;
	//if (FAILED(CGameObject::Add_Component(L"Component_Texture_ItemSlot", m_pTextureCom)))
	//	return E_FAIL;

	return NOERROR;
}

HRESULT CSpeedUI::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CSpeedUI::Update(const My_Float & fTimeDelta)
{
	mPlayerSpeed = static_cast<const CBazzi*>(mPlayer)->GetSpeed();
	mPlayerSpeed /= 0.36f;

	return My_Int();
}

My_Int CSpeedUI::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CSpeedUI::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CSpeedUI::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CSpeedUI::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

My_uLong CSpeedUI::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
