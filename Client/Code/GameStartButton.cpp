#include "stdafx.h"
#include "..\Header\GameStartButton.h"
#include "SoundManager.h"

CGameStartButton::CGameStartButton(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, mNum(0), mIsStart(false)
{
}

CGameStartButton::CGameStartButton(const CGameStartButton & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, mNum(Ref.mNum), mIsStart(Ref.mIsStart)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CGameStartButton::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(300.f, 165.f, 0.f);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX * 0.5f + 280.f, g_iBackCY * 0.5f + 235.f, 0.f));

	return NOERROR;
}

HRESULT CGameStartButton::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_GameStartButton");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_GameStartButton", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CGameStartButton::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CGameStartButton::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_hWnd, &point);
	RECT rc = { g_iBackCX * 0.5f + 280.f - 150.f, g_iBackCY * 0.5f + 235.f - 82.5f, g_iBackCX * 0.5f + 280.f + 150.f, g_iBackCY * 0.5f + 235.f + 82.5f };

	if (PtInRect(&rc, point))
	{
		mNum = 1;

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			mIsStart = true;
			CSoundMgr::GetInstance()->PlaySound(L"StartButton.MP3", CSoundMgr::Channel_Button);
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_BGM);
			CSoundMgr::GetInstance()->PlayBGM(L"StageBGM.MP3");
		}
	}

	else
		mNum = 0;

	return My_Int();
}

My_Int CGameStartButton::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CGameStartButton::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->SetUp_OnGraphicDev(mNum);
	Set_RenderState();
	m_pBufferCom->Render();
	Release_RenderState();
}

void CGameStartButton::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CGameStartButton::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CGameStartButton * CGameStartButton::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CGameStartButton*	pInstance = new CGameStartButton(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CGameStartButton Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGameStartButton::Clone()
{
	CGameStartButton*	pInstance = new CGameStartButton(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CGameStartButton Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CGameStartButton::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
