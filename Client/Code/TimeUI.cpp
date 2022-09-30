#include "stdafx.h"
#include "..\Header\TimeUI.h"


CTimeUI::CTimeUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, mTime(0.f)
	, mMiliSec(0)
	, mSec(0)
	, mMinute(0)
{
}

CTimeUI::CTimeUI(const CTimeUI & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, mTime(Ref.mTime)
	, mMiliSec(Ref.mMiliSec)
	, mSec(Ref.mSec)
	, mMinute(Ref.mMinute)
	, mIsStart(Ref.mIsStart)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CTimeUI::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	for (size_t i = 0; i < 6; ++i)
		mTimeDigit[i] = 0;

	return NOERROR;
}

HRESULT CTimeUI::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Time");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Time", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTimeUI::Initialize_Prototype()
{
	return NOERROR;

}

My_Int CTimeUI::Update(const My_Float & fTimeDelta)
{
	if (mIsStart)
	{
		// 시간 누적
		mTime += fTimeDelta;

		if (mTime > 0.001f)
		{
			mTimeDigit[5] += 2;
			mTime = 0.f;
		}

		if (mTimeDigit[5] >= 10) // 0
		{
			++mTimeDigit[4];
			mTimeDigit[5] = 0;
		}

		if (mTimeDigit[4] >= 10) // :00
		{
			++mTimeDigit[3];
			mTimeDigit[4] = 0;
		}

		if (mTimeDigit[3] >= 10) // 0:00
		{
			++mTimeDigit[2];
			mTimeDigit[3] = 0;
		}

		if (mTimeDigit[2] >= 6) // :00:00
		{
			++mTimeDigit[1];
			mTimeDigit[2] = 0;
		}

		if (mTimeDigit[1] >= 10) // 0:00:00
		{
			++mTimeDigit[0];
			mTimeDigit[1] = 0;
		}

	}
	
	if (!mIsAward)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return My_Int();
}

My_Int CTimeUI::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CTimeUI::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	Set_RenderState();

	// TIME :
	m_pTransformCom->Scaling(64.f, 32.f, 0.f);

	m_pTextureCom->SetUp_OnGraphicDev(11);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 200.f, 130.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();


	// TimeDigit
	m_pTransformCom->Scaling(32.f, 32.f, 0.f);

	m_pTextureCom->SetUp_OnGraphicDev(mTimeDigit[0]);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 145.f, 130.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTextureCom->SetUp_OnGraphicDev(mTimeDigit[1]);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 132.f, 130.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTextureCom->SetUp_OnGraphicDev(mTimeDigit[2]);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 108.f, 130.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTextureCom->SetUp_OnGraphicDev(mTimeDigit[3]);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 93.f, 130.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTextureCom->SetUp_OnGraphicDev(mTimeDigit[4]);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 69.f, 130.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTextureCom->SetUp_OnGraphicDev(mTimeDigit[5]);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 56.f, 130.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();


	// Colon
	m_pTextureCom->SetUp_OnGraphicDev(10);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 120.f, 130.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTextureCom->SetUp_OnGraphicDev(10);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX - 81.f, 130.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	Release_RenderState();
}

void CTimeUI::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CTimeUI::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CTimeUI * CTimeUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTimeUI*	pInstance = new CTimeUI(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CTimeUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTimeUI::Clone()
{
	CTimeUI*	pInstance = new CTimeUI(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CTimeUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CTimeUI::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
