#include "stdafx.h"
#include "..\Header\SpeedUIBackGround.h"

CSpeedUIBackGround::CSpeedUIBackGround(LPDIRECT3DDEVICE9 pDevice)
	: CSpeedUI(pDevice)
{
}

CSpeedUIBackGround::CSpeedUIBackGround(const CSpeedUIBackGround & Ref)
	: CSpeedUI(Ref)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CSpeedUIBackGround::Initialize()
{
	if (FAILED(CSpeedUI::Initialize()))
		return E_FAIL;
	
	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_SpeedUIBackGround");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_SpeedUIBackGround", m_pTextureCom)))
		return E_FAIL;

	mFrame = FRAME(0.f, 2.f, 2.f);

	return NOERROR;
}

HRESULT CSpeedUIBackGround::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CSpeedUIBackGround::Update(const My_Float & fTimeDelta)
{
	CSpeedUI::Update(fTimeDelta);

	if (mPlayerSpeed < 100.f)
		mFrame.fFrame = 0.f;

	else
		mFrame.fFrame = 1.f;

	if (!mIsAward)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return My_Int();
}

My_Int CSpeedUIBackGround::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CSpeedUIBackGround::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev(My_uInt(mFrame.fFrame));

	Set_RenderState();

	if (1 == My_uInt(mFrame.fFrame))
	{
		m_pTextureCom->SetUp_OnGraphicDev(0);
		m_pBufferCom->Render();
		m_pTextureCom->SetUp_OnGraphicDev(1);
	}

	m_pBufferCom->Render();

	Release_RenderState();
}

CSpeedUIBackGround * CSpeedUIBackGround::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSpeedUIBackGround*	pInstance = new CSpeedUIBackGround(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CSpeedUIBackGround Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSpeedUIBackGround::Clone()
{
	CSpeedUIBackGround*	pInstance = new CSpeedUIBackGround(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CSpeedUIBackGround Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CSpeedUIBackGround::ReleaseMemory()
{
	CSpeedUI::ReleaseMemory();
	return My_uLong();
}
