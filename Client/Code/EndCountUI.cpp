#include "stdafx.h"
#include "..\Header\EndCountUI.h"

#include "GameObjectMgr.h"
#include "Bazzi.h"
#include "Dao.h"
#include "Dizni.h"
#include "TimeUI.h"
#include "RankingManager.h"
#include "Camera_Target.h"

#include "SoundManager.h"

CEndCountingUI::CEndCountingUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, mCounting(4), mTime(0.f), mIsStart(false)
{
}

CEndCountingUI::CEndCountingUI(const CEndCountingUI & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, mCounting(Ref.mCounting), mTime(Ref.mTime), mIsStart(Ref.mIsStart)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CEndCountingUI::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX * 0.5f, g_iBackCY * 0.5f - 180.f, 0.f));

	return NOERROR;
}

HRESULT CEndCountingUI::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_EndCounting");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_EndCounting", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CEndCountingUI::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CEndCountingUI::Update(const My_Float & fTimeDelta)
{
	if (mIsStart)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		mTime += fTimeDelta;

		if (mCounting > -1 && mTime > 1.f && !mIsEnd)
		{
			--mCounting;
			mTime = 0.f;

			if (!mIsPlayerGoal)
				CSoundMgr::GetInstance()->PlaySound(L"CountDown.ogg", CSoundMgr::Channel_Item);

			if (mCounting == -1) // 카운팅 끝
			{
				if (!mIsPlayerGoal)
					mCounting = 6;

				mTime = 0.f;
				mIsEnd = true;
				const_cast<CCamera_Target*>(static_cast<const CCamera_Target*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_Target", 0)))->SetState(CCamera_Target::GOAL);

				// 캐릭터들 전부 멈추게
				// const_cast<CBazzi*>(static_cast<const CBazzi*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0)))->SetStop(false);
				const_cast<CDao*>(static_cast<const CDao*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0)))->SetStop(true);
				const_cast<CDizni*>(static_cast<const CDizni*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0)))->SetStop(true);
			}
		}

		else if (mIsEnd && mTime > 3.f)
			return 1;
	}

	return My_Int();
}

My_Int CEndCountingUI::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CEndCountingUI::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	if (mCounting > 4 || mIsPlayerGoal)
		m_pTransformCom->Scaling(540.f, 272.f, 0.f);

	else
		m_pTransformCom->Scaling(62.f, 76.f, 0.f);

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	if (mIsPlayerGoal)
		m_pTextureCom->SetUp_OnGraphicDev(5);

	else
		m_pTextureCom->SetUp_OnGraphicDev(mCounting);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CEndCountingUI::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CEndCountingUI::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CEndCountingUI * CEndCountingUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEndCountingUI*	pInstance = new CEndCountingUI(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CEndCountingUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEndCountingUI::Clone()
{
	CEndCountingUI*	pInstance = new CEndCountingUI(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CEndCountingUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CEndCountingUI::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
