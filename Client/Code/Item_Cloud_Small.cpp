#include "stdafx.h"
#include "..\Header\Item_Cloud_Small.h"


CItem_Cloud_Small::CItem_Cloud_Small(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_vPosition(0.f, 0.f, 0.f)
	, m_iCloudNum(0)
	, m_fTime(0.f)
{
}

CItem_Cloud_Small::CItem_Cloud_Small(const CItem_Cloud_Small & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, m_vPosition(Ref.m_vPosition)
	, m_iCloudNum(Ref.m_iCloudNum)
	, m_fTime(Ref.m_fTime)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

void CItem_Cloud_Small::Set_CloudNum(const int & iCloudNum)
{
	m_iCloudNum = iCloudNum;

	switch (m_iCloudNum)
	{
	case 1:
		m_vPosition = {g_iBackCX >> 2, g_iBackCY >> 1, 0.f};
		break;
	case 2:
		m_vPosition = { g_iBackCX >> 1, (g_iBackCY >> 1) + 50.f, 0.f };
		break;
	case 3:
		m_vPosition = { g_iBackCX * (3.f / 4.f), g_iBackCY >> 1, 0.f };
		break;
	case 4:
		m_vPosition = { (g_iBackCX >> 2) + 200.f, (g_iBackCY >> 1) - 150.f, 0.f };
		break;
	case 5:
		m_vPosition = { (g_iBackCX >> 1) + 150.f, (g_iBackCY >> 1) - 150.f, 0.f };
		break;
	case 6:
		m_vPosition = { (g_iBackCX >> 1), (g_iBackCY >> 1) - 300.f, 0.f };
		break;
	}
}

HRESULT CItem_Cloud_Small::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(600.f, 512.f, 0.f);

	return NOERROR;
}

HRESULT CItem_Cloud_Small::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Cloud_Small");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Cloud_Small", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CItem_Cloud_Small::Initialize_Prototype()
{
	return NOERROR;

}

My_Int CItem_Cloud_Small::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_CLOUD, this);

	return My_Int();
}

My_Int CItem_Cloud_Small::LastUpdate(const My_Float & fTimeDelta)
{
	m_fTime += fTimeDelta;

	// 구름이 화면 밖으로 사라지게 하기
	if (3.f < m_fTime)
	{
		switch (m_iCloudNum)
		{
		case 1:
			m_vPosition.x -= 200.f * (fTimeDelta);
			m_vPosition.y += 200.f * (fTimeDelta);
			break;
		case 2:
			m_vPosition.y += 200.f * (fTimeDelta);
			break;
		case 3:
			m_vPosition.x += 200.f * (fTimeDelta);
			m_vPosition.y += 200.f * (fTimeDelta);
			break;
		case 4:
			m_vPosition.x -= 200.f * (fTimeDelta);
			m_vPosition.y -= 200.f * (fTimeDelta);
			break;
		case 5:
			m_vPosition.x += 200.f * (fTimeDelta);
			m_vPosition.y -= 200.f * (fTimeDelta);
			break;
		case 6:
			m_vPosition.y -= 200.f * (fTimeDelta);
			break;
		}
		
		if (6.f < m_fTime)
			return 1;
	}


	return My_Int();
}

void CItem_Cloud_Small::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &m_vPosition);

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CItem_Cloud_Small::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CItem_Cloud_Small::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

CItem_Cloud_Small * CItem_Cloud_Small::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem_Cloud_Small*	pInstance = new CItem_Cloud_Small(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CViewPort_Buffer Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_Cloud_Small::Clone()
{
	CItem_Cloud_Small*	pInstance = new CItem_Cloud_Small(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItem_Cloud_Small Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CItem_Cloud_Small::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
