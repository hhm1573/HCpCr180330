#include "stdafx.h"
#include "..\Header\SpeedUINumber.h"

CSpeedUINumber::CSpeedUINumber(LPDIRECT3DDEVICE9 pDevice)
	: CSpeedUI(pDevice)
	, mHundred(0)
	, mTen(0)
	, mOne(0)
{
}

CSpeedUINumber::CSpeedUINumber(const CSpeedUINumber & Ref)
	: CSpeedUI(Ref)
	, mHundred(Ref.mHundred)
	, mTen(Ref.mTen)
	, mOne(Ref.mOne)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CSpeedUINumber::Initialize()
{
	if (FAILED(CSpeedUI::Initialize()))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_SpeedUINumber");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_SpeedUINumber", m_pTextureCom)))
		return E_FAIL;

	mFrame = FRAME(0.f, 10.f, 10.f);
	m_pTransformCom->Scaling(40.5f, 49.5f, 0.f);

	return NOERROR;
}

HRESULT CSpeedUINumber::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CSpeedUINumber::Update(const My_Float & fTimeDelta)
{
	CSpeedUI::Update(fTimeDelta);

	int hundred = int(mPlayerSpeed) / 100;
	int ten = int(mPlayerSpeed) / 10;

	mHundred = hundred;

	if (0 == hundred)
		mTen = ten;

	else
		mTen = ten % (hundred * 10);

	if (0 == ten)
		mOne = (int)mPlayerSpeed;

	else
		mOne = int(mPlayerSpeed) % (ten * 10);

	if (!mIsAward)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return My_Int();
}

My_Int CSpeedUINumber::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CSpeedUINumber::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	Set_RenderState();

	m_pTextureCom->SetUp_OnGraphicDev(My_uInt(mHundred));
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX * 0.5f - 34.f, g_iBackCY - 35.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTextureCom->SetUp_OnGraphicDev(My_uInt(mTen));
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX * 0.5f, g_iBackCY - 35.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	m_pTextureCom->SetUp_OnGraphicDev(My_uInt(mOne));
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX * 0.5f + 35.f, g_iBackCY - 35.f, 0.f));
	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render();

	Release_RenderState();
}

CSpeedUINumber * CSpeedUINumber::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSpeedUINumber*	pInstance = new CSpeedUINumber(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CSpeedUINumber Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSpeedUINumber::Clone()
{
	CSpeedUINumber*	pInstance = new CSpeedUINumber(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CSpeedUINumber Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CSpeedUINumber::ReleaseMemory()
{
	CSpeedUI::ReleaseMemory();
	return My_uLong();
}
