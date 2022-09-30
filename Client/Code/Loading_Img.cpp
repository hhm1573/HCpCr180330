#include "stdafx.h"
#include "..\Header\Loading_Img.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"
#include "SceneMgr.h"


CLoading_Img::CLoading_Img(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CLoading_Img::CLoading_Img(const CLoading_Img & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, m_pTexture(Ref.m_pTexture)
	, m_tFrame(Ref.m_tFrame)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CLoading_Img::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_tFrame = FRAME(0, 15, 30);

	m_pTransformCom->Scaling(500.f, 300.f, 0.f);

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(640.f, 680.f, 0.f));

	return NOERROR;
}

HRESULT CLoading_Img::Initialize_Component()
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
	m_pBufferCom = (CViewPort_Buffer*)m_pComponentMgr->Clone_Component(SCENE_LOADING, L"Component_Buffer_ViewPort");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Buffer_ViewPort", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_LOADING, L"Component_Texture_Loading_Img");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Loading_Img", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLoading_Img::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CLoading_Img::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return My_Int();
}

My_Int CLoading_Img::LastUpdate(const My_Float & fTimeDelta)
{
	m_tFrame.fFrame += m_tFrame.fCount * fTimeDelta;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0;
	}

	return My_Int();
}

void CLoading_Img::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev((My_uInt)m_tFrame.fFrame);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CLoading_Img::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CLoading_Img::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

CLoading_Img * CLoading_Img::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLoading_Img*	pInstance = new CLoading_Img(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CLoading_Img Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLoading_Img::Clone()
{
	CLoading_Img*	pInstance = new CLoading_Img(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CLoading_Img Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CLoading_Img::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
