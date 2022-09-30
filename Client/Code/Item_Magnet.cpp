#include "stdafx.h"
#include "..\Header\Item_Magnet.h"
#include "GameObjectMgr.h"
#include "Ray_Tri.h"
#include "Ray_Tri2.h"
#include "Item_Booster.h"
#include "Item_Missile.h"
#include "Bazzi.h"


CItem_Magnet::CItem_Magnet(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_vPosition(0.f, 0.f, 0.f)
{
}

CItem_Magnet::CItem_Magnet(const CItem_Magnet & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, m_pPlayer_Transform(Ref.m_pPlayer_Transform)
	, m_vPosition(Ref.m_vPosition)
	, m_iNum(Ref.m_iNum)
	, m_fAccSpeed(Ref.m_fAccSpeed)
	, m_bIsDestroy(Ref.m_bIsDestroy)
	, m_bIsBooster(Ref.m_bIsBooster)
	, m_bIsMagnet(Ref.m_bIsMagnet)
	, m_bIsMissile(Ref.m_bIsMissile)
	, m_iTargetNum(Ref.m_iTargetNum)
	, mTime(Ref.mTime)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

HRESULT CItem_Magnet::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	//m_pTransformCom->Scaling(100.f, 100.f, 0.f);

	return NOERROR;
}

HRESULT CItem_Magnet::Initialize_Component()
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

	// For.Com_Buffer_RectTex
	m_pBufferCom = (CRectTex*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Buffer_RectTex");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Buffer_RectTex", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Item_Magnet");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Item_Magnet", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CItem_Magnet::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CItem_Magnet::Update(const My_Float & fTimeDelta)
{
	/*mTime += fTimeDelta;

	if (mTime > 5.f)
	m_bIsDestroy = true;
	*/


	if (true == m_bIsDestroy)
		return 1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	m_vPosition = *m_pPlayer_Transform->Get_Information(CTransform::INFO_POSITION);

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(m_vPosition.x, m_vPosition.y + 1.f, m_vPosition.z));

	//if(2 != m_iNum)
	//	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(m_vPosition.x, m_vPosition.y + 1.f, m_vPosition.z));
	//else
	//{
	//	CGameObject* pObject2 = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Ray_Tri2", 0));
	//	My_Vec3 pPosition = dynamic_cast<CRay_Tri2*>(pObject2)->Get_Position();
	//	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(pPosition.x, pPosition.y, pPosition.z));
	//}

	return My_Int();
}

My_Int CItem_Magnet::LastUpdate(const My_Float & fTimeDelta)
{
	if (true == m_bIsFirst)
	{
		m_fFirstTime = fTimeDelta;
		m_bIsFirst = false;
	}

	m_fAccTime += fTimeDelta;

	if (4.f < (m_fAccTime - m_fFirstTime))
	{
		CGameObject* pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0));
		dynamic_cast<CBazzi*>(pGameObject)->SetIsMagnet(false);
		return 1;
	}

	m_fAccSpeed += fTimeDelta;

	/*빌보드*/
	My_Matrix	matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, nullptr, &matView);

	My_Vec3 vec3[3];

	vec3[0] = *(My_Vec3*)&matView.m[CTransform::INFO_RIGHT][0];
	vec3[1] = *(My_Vec3*)&matView.m[CTransform::INFO_UP][0];
	vec3[2] = *(My_Vec3*)&matView.m[CTransform::INFO_LOOK][0];

	D3DXVec3Normalize(&vec3[0], &vec3[0]);
	D3DXVec3Normalize(&vec3[1], &vec3[1]);
	D3DXVec3Normalize(&vec3[2], &vec3[2]);

	m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(vec3[0]));
	m_pTransformCom->Set_Information(CTransform::INFO_UP, &(vec3[1]));
	m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &(vec3[2]));

	switch (m_iNum)
	{
	case 0:
		m_pTransformCom->Scaling(3.f, 3.f, 3.f);
		break;
	case 1:
	{
		m_pTransformCom->Scaling(2.f, 2.f, 2.f);
	}
	break;
	case 2:
	{
		m_pTransformCom->Scaling(1.f, 1.f, 1.f);

		if (false == m_bIsBooster && true == m_bIsMagnet)
		{
			m_bIsBooster = true;
			m_bIsMagnet = false;

			// Item_Booster
			CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Booster");
			if (pObject == nullptr)
				return E_FAIL;
			if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Booster", pObject)))
				return E_FAIL;

			// 배찌의 스피드를 조절하기 위해 컴포넌트 전달
			dynamic_cast<CItem_Booster*>(pObject)->SetIsPlayer(true);
			dynamic_cast<CItem_Booster*>(pObject)->Set_IsMagnet(true);

			// Booster_Effect
			pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Booster_Effect");
			if (pObject == nullptr)
				return E_FAIL;
			if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Booster_Effect", pObject)))
				return E_FAIL;
		}

		if (true == m_bIsMissile)
		{
			m_bIsMissile = false;

			// Item_Missile
			CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Missile");
			if (pObject == nullptr)
				return E_FAIL;
			if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Missile", pObject)))
				return E_FAIL;
			dynamic_cast<CItem_Missile*>(pObject)->Set_Player_Tramsform(m_pPlayer_Transform);

			if (true == m_bIsBoss)
				dynamic_cast<CItem_Missile*>(pObject)->Set_IsBoss(true);
			else
				dynamic_cast<CItem_Missile*>(pObject)->Set_Target_Num(m_iTargetNum);

			return 1;
		}

	}
	break;
	}

	m_pTransformCom->Rotation_Z(-2.f * m_fAccSpeed);

	return My_Int();
}

void CItem_Magnet::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(m_iNum);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CItem_Magnet::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x0f);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CItem_Magnet::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CItem_Magnet * CItem_Magnet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem_Magnet*		pInstance = new CItem_Magnet(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItem_Magnet Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_Magnet::Clone()
{
	CItem_Magnet*		pInstance = new CItem_Magnet(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItem_Magnet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CItem_Magnet::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
