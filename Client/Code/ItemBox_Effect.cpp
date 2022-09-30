#include "stdafx.h"
#include "..\Header\ItemBox_Effect.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"


CItemBox_Effect::CItemBox_Effect(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_vPosition(0.f, 0.f, 0.f)
	, m_vRight(0.f, 0.f, 0.f)
	, m_vUp(0.f, 0.f, 0.f)
	, m_vLook(0.f, 0.f, 0.f)
{
	srand(unsigned(time(NULL)));
}

CItemBox_Effect::CItemBox_Effect(const CItemBox_Effect & rhs)
	: CGameObject(rhs)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pRendererCom(rhs.m_pRendererCom)
	, m_vPosition(rhs.m_vPosition)
	, m_vRight(rhs.m_vRight)
	, m_vUp(rhs.m_vUp)
	, m_vLook(rhs.m_vLook)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CItemBox_Effect::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_iRand = rand() % 4;

	m_vPosition = { g_iBackCX / 2.f, g_iBackCY / 2.f, 0.f };
	m_pTransformCom->Scaling(10.f, 10.f, 0.f);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX / 2.f, g_iBackCY / 2.f, 0.f));
	return NOERROR;
}

HRESULT CItemBox_Effect::Initialize_Component()
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
	m_pBufferCom = (CViewPort_Buffer*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Buffer_ViewPort");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Buffer_ViewPort", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Color_Paper");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Color_Paper", m_pTextureCom)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CItemBox_Effect::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CItemBox_Effect::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return My_Int();
}

My_Int CItemBox_Effect::LastUpdate(const My_Float & fTimeDelta)
{
	if (true == m_bIsFirst)
	{
		m_bIsFirst = false;
		m_fFirstTime = fTimeDelta;
	}

	m_fAccTime += fTimeDelta;

	if (3.f < (m_fAccTime - m_fFirstTime))
		return 1;

	m_vPosition.y -= m_fJumpPow * m_fJumpAcc - 9.8f * m_fJumpAcc * m_fJumpAcc * 0.5f;

	m_fJumpAcc += 0.2f;

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &m_vPosition);

	return My_Int();
}

void CItemBox_Effect::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev(m_iRand);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();

}

void CItemBox_Effect::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CItemBox_Effect::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CItemBox_Effect * CItemBox_Effect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	// 원형객체를 생성하는 작업.
	CItemBox_Effect*	pInstance = new CItemBox_Effect(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItemBox_Effect  Prototype Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CItemBox_Effect::Clone()
{
	CItemBox_Effect* pInstance = new CItemBox_Effect(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItemBox_Effect Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

My_uLong CItemBox_Effect::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
