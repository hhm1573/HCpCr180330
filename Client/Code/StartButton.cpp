#include "stdafx.h"
#include "..\Header\StartButton.h"
#include "SoundManager.h"

CStartButton::CStartButton(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, mNum(0), mIsStart(false)
{
}

CStartButton::CStartButton(const CStartButton & Ref)
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

HRESULT CStartButton::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(136.f, 67.f, 0.f);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX * 0.5f - 90.f, g_iBackCY * 0.5f + 200.f, 0.f));

	return NOERROR;
}

HRESULT CStartButton::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_StartButton");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_StartButton", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CStartButton::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CStartButton::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_hWnd, &point);
	RECT rc = { g_iBackCX * 0.5f - 90.f - 68.f, g_iBackCY * 0.5f + 200.f - 33.5, g_iBackCX * 0.5f - 90.f + 68.f, g_iBackCY * 0.5f + 200.f + 33.5 };

	if (PtInRect(&rc, point))
	{
		mNum = 1;

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			mIsStart = true;
			CSoundMgr::GetInstance()->PlaySound(L"ButtonClick.MP3", CSoundMgr::Channel_Button);
		}
	}

	else
		mNum = 0;

	return My_Int();
}

My_Int CStartButton::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CStartButton::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->SetUp_OnGraphicDev(mNum);
	Set_RenderState();
	m_pBufferCom->Render();
	Release_RenderState();
}

void CStartButton::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CStartButton::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CStartButton * CStartButton::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStartButton*	pInstance = new CStartButton(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CStartButton Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStartButton::Clone()
{
	CStartButton*	pInstance = new CStartButton(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CStartButton Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CStartButton::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
