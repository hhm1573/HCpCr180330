#include "stdafx.h"
#include "..\Header\SpeedUIOutLine.h"

CSpeedUIOutLine::CSpeedUIOutLine(LPDIRECT3DDEVICE9 pDevice)
	: CSpeedUI(pDevice)
{
}

CSpeedUIOutLine::CSpeedUIOutLine(const CSpeedUIOutLine & Ref)
	: CSpeedUI(Ref)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CSpeedUIOutLine::Initialize()
{
	if (FAILED(CSpeedUI::Initialize()))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_SpeedUIOutLine");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_SpeedUIOutLine", m_pTextureCom)))
		return E_FAIL;

	mFrame = FRAME(0.f, 5.f, 5.f);

	return NOERROR;
}

HRESULT CSpeedUIOutLine::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CSpeedUIOutLine::Update(const My_Float & fTimeDelta)
{
	CSpeedUI::Update(fTimeDelta);

	if (mPlayerSpeed < 100.f)
		mFrame.fFrame = -1.f;

	else if (mPlayerSpeed < 102.f)
		mFrame.fFrame = 0.f;

	else if (mPlayerSpeed < 104.f)
		mFrame.fFrame = 1.f;

	else if (mPlayerSpeed < 106.f)
		mFrame.fFrame = 2.f;

	else if (mPlayerSpeed < 108.f)
		mFrame.fFrame = 3.f;

	else 
		mFrame.fFrame = 4.f;

	if (mFrame.fFrame >= 0.f && !mIsAward)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return My_Int();
}

My_Int CSpeedUIOutLine::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CSpeedUIOutLine::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev(My_uInt(mFrame.fFrame));

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

CSpeedUIOutLine * CSpeedUIOutLine::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSpeedUIOutLine*	pInstance = new CSpeedUIOutLine(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CSpeedUIOutLine Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSpeedUIOutLine::Clone()
{
	CSpeedUIOutLine*	pInstance = new CSpeedUIOutLine(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CSpeedUIOutLine Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CSpeedUIOutLine::ReleaseMemory()
{
	CSpeedUI::ReleaseMemory();
	return My_uLong();
}
