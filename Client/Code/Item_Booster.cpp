#include "stdafx.h"
#include "..\Header\Item_Booster.h"
#include "GameObjectMgr.h"
#include "Camera_Target.h"


CItem_Booster::CItem_Booster(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pPlayer_Vehicle(nullptr)
	, m_pCamrea(nullptr)
	, mBoosterTime(0.f)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CItem_Booster::CItem_Booster(const CItem_Booster & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, m_pPlayer_Vehicle(Ref.m_pPlayer_Vehicle)
	, m_pCamrea(Ref.m_pCamrea)
	, m_tFrame(Ref.m_tFrame)
	, mIsPlayer(Ref.mIsPlayer)
	, mBoosterTime(Ref.mBoosterTime)
	, m_bIsMagnet(Ref.m_bIsMagnet)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

void CItem_Booster::Set_Player_Vehicle(CVehicle * pVehicle)
{
	m_pPlayer_Vehicle = pVehicle;

	m_pPlayer_Vehicle->Set_Inform(50.0f, 5000.f, 1.0f);
}

HRESULT CItem_Booster::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_tFrame = FRAME(0, 10, 2);

	m_pTransformCom->Scaling(500.f, 300.f, 0.f);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(g_iBackCX / 2.f - 10.f, g_iBackCY / 2.f - 50.f, 0.f));

	return NOERROR;
}

void CItem_Booster::SetIsPlayer(My_Bool flag)
{
	mIsPlayer = flag;

	if (mIsPlayer)
	{
		// 카메라 얻어오기
		m_pCamrea = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_Target", 0));
		dynamic_cast<CCamera_Target*>(m_pCamrea)->SetBooster(1);		// 플레이어가 부스터 중이라는 값을 카메라에 전달
	}
}

HRESULT CItem_Booster::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Item_Booster");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Item_Booster", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CItem_Booster::Initialize_Prototype()
{
	return NOERROR;

}

My_Int CItem_Booster::Update(const My_Float & fTimeDelta)
{
	if (mIsPlayer)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOOSTER, this);

	return My_Int();
}

My_Int CItem_Booster::LastUpdate(const My_Float & fTimeDelta)
{
	m_tFrame.fFrame += m_tFrame.fCount * fTimeDelta;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0;
	}

	if (mIsPlayer)
	{
		if (false == m_bIsMagnet)
		{
			if (3 == dynamic_cast<CCamera_Target*>(m_pCamrea)->Get_Booster())
			{
				m_pPlayer_Vehicle->Set_Inform(50.0f, 2500.f, 1.0f);
				return 1;
			}
		}
		else
		{
			const CGameObject* pEffect = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Booster_Effect", 0);
			if (nullptr == pEffect)
				return 1;
		}
	}

	else
	{
		mBoosterTime += fTimeDelta;
		if (mBoosterTime > 4.f)
			return 1;
	}

	return My_Int();
}

void CItem_Booster::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pBufferCom->Set_Transform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->SetUp_OnGraphicDev((My_uInt)m_tFrame.fFrame);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CItem_Booster::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CItem_Booster::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CItem_Booster * CItem_Booster::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem_Booster*	pInstance = new CItem_Booster(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItem_Booster Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_Booster::Clone()
{
	CItem_Booster*		pInstance = new CItem_Booster(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItem_Booster Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CItem_Booster::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);


	CGameObject::ReleaseMemory();

	return My_uLong();
}
