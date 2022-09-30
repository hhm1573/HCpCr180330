#include "stdafx.h"
#include "..\Header\Item_Icon.h"
#include "GameObjectMgr.h"


CItem_Icon::CItem_Icon(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_iNumber(0)
	, m_fFirstTime(0.f)
	, m_fAccTime(0.f)
	, m_bIsFirst(false)
	, m_bIsUsed(false)
{
}

CItem_Icon::CItem_Icon(const CItem_Icon & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, m_iNumber(Ref.m_iNumber)
	, m_fFirstTime(Ref.m_fFirstTime)
	, m_fAccTime(Ref.m_fAccTime)
	, m_bIsFirst(Ref.m_bIsFirst)
	, m_bIsUsed(Ref.m_bIsUsed)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CItem_Icon::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX / 2.f - 20.f, g_iBackCY / 2.f - 220.f, 0.f));
	m_pTransformCom->Scaling(100.f, 100.f, 0.f);

	return NOERROR;
}

HRESULT CItem_Icon::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CItem_Icon::Update(const My_Float & fTimeDelta)
{
	if (true == m_bIsUsed)
		return 1;

	if (!mIsAward)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return My_Int();
}

My_Int CItem_Icon::LastUpdate(const My_Float & fTimeDelta)
{
	if (false == m_bIsFirst)
	{
		m_fFirstTime = fTimeDelta;
		m_bIsFirst = true;
	}

	m_fAccTime += fTimeDelta;

	if (1.f < (m_fAccTime - m_fFirstTime))
	{
		const CGameObject* pGameObject = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Icon", 0);
		
		if (this == pGameObject)
			m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(160.f, 80.f, 0.f));
		else
		{
			m_pTransformCom->Scaling(70.f, 70.f, 70.f);
			m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(65.f, 70.f, 0.f));
		}
	}


	return My_Int();
}

void CItem_Icon::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev(m_iNumber);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

HRESULT CItem_Icon::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Item_Icon");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Item_Icon", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

void CItem_Icon::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CItem_Icon::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CItem_Icon * CItem_Icon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem_Icon*	pInstance = new CItem_Icon(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItemSlot Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_Icon::Clone()
{
	CItem_Icon*		pInstance = new CItem_Icon(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItemSlot Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CItem_Icon::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
