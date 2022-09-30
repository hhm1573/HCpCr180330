#include "stdafx.h"
#include "..\Header\Smoke_Effect.h"
#include "GameObjectMgr.h"
#include "Camera_Target.h"
#include "Bazzi.h"


CSmoke_Effect::CSmoke_Effect(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
{
	srand(unsigned(time(NULL)));

	ZeroMemory(&m_tFrame, sizeof(FRAME));
	m_vPos = { 0.f, 0.f, 0.f };
	m_vLook = { 0.f, 0.f, 0.f };
	m_szName = nullptr;
	m_fAccTime = 0.f;
	m_fFirstTime = 0.f;
	m_bIsFirst = true;
	m_iRand = 0;
	m_vScale = { 0.f, 0.f, 0.f };
}

CSmoke_Effect::CSmoke_Effect(const CSmoke_Effect & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, m_tFrame(Ref.m_tFrame)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();

	m_vPos = { 0.f, 0.f, 0.f };
	m_vLook = { 0.f, 0.f, 0.f };
	m_szName = nullptr;
	m_fAccTime = 0.f;
	m_fFirstTime = 0.f;
	m_bIsFirst = true;
	m_iRand = 0;
	m_vScale = { 0.f, 0.f, 0.f };
}

HRESULT CSmoke_Effect::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_tFrame = FRAME(0, 4, 2);

	m_vScale = { 0.5f, 0.5f, 0.5f };
	m_pTransformCom->Scaling(m_vScale.x, m_vScale.y, m_vScale.z);

	return NOERROR;
}

HRESULT CSmoke_Effect::Initialize_Component()
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
	m_pBufferCom = (CRectTex*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Buffer_RectTex");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Buffer_RectTex", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Smoke_Effect");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Smoke_Effect", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CSmoke_Effect::Initialize_Prototype()
{
	return NOERROR;

}

My_Int CSmoke_Effect::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOOSTER, this);

	m_tFrame.fFrame += m_tFrame.fCount * fTimeDelta;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0;
	}

	Compute_ViewZ(m_pTransformCom->Get_Information(CTransform::INFO_POSITION));

	return My_Int();
}

My_Int CSmoke_Effect::LastUpdate(const My_Float & fTimeDelta)
{
	if (true == m_bIsFirst)
	{
		m_fFirstTime = fTimeDelta;
		m_bIsFirst = false;
	}

	m_fAccTime += fTimeDelta;

	if (1.f < (m_fAccTime - m_fFirstTime))
	{
		return 1;
	}

	m_iRand = rand() % 180 - 90;
	My_Matrix matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(m_iRand));
	D3DXVec3TransformNormal(&m_vLook, &m_vLook, &matRot);
	My_Vec3 vPosition = m_vPos - m_vLook * 1.f * fTimeDelta;
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(vPosition.x, vPosition.y + 0.05f, vPosition.z));

	m_vScale.x -= 0.01f;
	m_vScale.y -= 0.01f;
	m_vScale.z -= 0.01f;

	m_pTransformCom->Scaling(m_vScale.x, m_vScale.y, m_vScale.z);

	return My_Int();
}

void CSmoke_Effect::Render()
{
	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CSmoke_Effect::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CSmoke_Effect::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CSmoke_Effect * CSmoke_Effect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSmoke_Effect*	pInstance = new CSmoke_Effect(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CSmoke_Effect Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSmoke_Effect::Clone()
{
	CSmoke_Effect*		pInstance = new CSmoke_Effect(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CSmoke_Effect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CSmoke_Effect::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);


	CGameObject::ReleaseMemory();

	return My_uLong();
}
