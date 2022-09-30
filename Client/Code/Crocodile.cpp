#include "stdafx.h"
#include "..\Header\Crocodile.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"
#include "Terrain.h"
#include "Ray_Tri.h"
#include "Ray_Tri2.h"

#include "Item_Water_Bomb.h"
#include "Boss_Skill.h"
#include	"Boss_HP.h"
#include "Devil_Skill.h"
#include "Item_Banana.h"

#include "BossMoney.h"
#include "SoundManager.h"


CCrocodile::CCrocodile(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
	, m_pComponent_Vehicle(nullptr)
	, m_pCollisioner(nullptr)
{
	srand(unsigned(time(NULL)));
}

CCrocodile::CCrocodile(const CCrocodile & Ref)
	: CModel(Ref)
	, m_pComponent_Vehicle(Ref.m_pComponent_Vehicle)
	, m_pCollisioner(Ref.m_pCollisioner)
{
	if (nullptr != m_pComponent_Vehicle)
		m_pComponent_Vehicle->AddRef();
}

HRESULT CCrocodile::Initialize()
{
	m_pComponent_Vehicle = (CVehicle*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Vehicle");
	if (m_pComponent_Vehicle == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Vehicle", m_pComponent_Vehicle)))
		return E_FAIL;
	m_pComponent_Vehicle->Set_Inform(50.0f, 700.0f, 1.0f);

	m_pComponent_NavMesh = (CNavMesh*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_NavMesh");
	if (m_pComponent_NavMesh == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_NavMesh", m_pComponent_NavMesh)))
		return E_FAIL;

	m_pCollisioner = (CCollisioner*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (m_pCollisioner == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Collisioner", m_pCollisioner)))
		return E_FAIL;




	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CCrocodile::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Crocodile.dat");
}

My_Int CCrocodile::Update(const My_Float & fTimeDelta)
{
	if (mIsAwardBoss)
	{
		m_pComponent_Vehicle->Accelerate();

		const My_Vec3*	vLook = m_pComponent_Transform->Get_Information(CTransform::INFO_LOOK);
		m_pComponent_Vehicle->Update(fTimeDelta, vLook, m_bLanded);

		const CTerrain*		pTerrain = (CTerrain*)CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Terrain", 0);

		//// 물폭탄에 맞으면 NavMesh 타지 말기
		//if (false == m_bIsWater)
		//{
		if (false == m_bIsWater)
		{
			if (m_pComponent_Transform->Move_On_NavMesh(&(m_pComponent_Vehicle->Get_Velocity() * fTimeDelta), pTerrain->Get_Buffer(), fTimeDelta, m_pComponent_Vehicle, &m_bLanded) == false) {
				m_pComponent_Transform->Set_NavMesh(m_pComponent_NavMesh->Get_FromPos(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION)));
			}
			m_pComponent_Transform->Rotation_Y(m_pComponent_Vehicle->Get_AngularVelocity());

		}

		m_pComponent_Transform->Rotation_Y(m_pComponent_Vehicle->Get_AngularVelocity());
	}

	if (true == m_bIsBoss)
	{
		if (mIsDead)
			return My_Int();

		if (0 == mHP)
		{
			CSoundMgr::GetInstance()->PlaySound(L"CoinFalling.mp3", CSoundMgr::Channel_Effect);

			for (int i = 0; i < 15; ++i)
			{
				CGameObject* 	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_BossMoney");
				if (pObject == nullptr)
					return E_FAIL;
				if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_BossMoney", pObject)))
					return E_FAIL;

				My_Int		iRandX = rand() % 40 - 20;
				My_Int		iRandY = rand() % 50 - 20;
				My_Int		iRandZ = rand() % 40 - 20;

				My_Vec3		vPos = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
				vPos = { vPos.x + iRandX, vPos.y + 7.f, vPos.z + iRandZ };

				My_Vec3 pos = vPos + *m_pComponent_Transform->Get_Information(CTransform::INFO_LOOK) * 20.f;

				dynamic_cast<CBossMoney*>(pObject)->Set_Position(pos);
			}

			for (int i = 0; i < 15; ++i)
			{
				CGameObject* 	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_BossMoney");
				if (pObject == nullptr)
					return E_FAIL;
				if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_BossMoney", pObject)))
					return E_FAIL;

				My_Int		iRandX = rand() % 40 - 20;
				My_Int		iRandY = rand() % 50 - 20;
				My_Int		iRandZ = rand() % 40 - 20;

				My_Vec3		vPos = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
				vPos = { vPos.x + iRandX, vPos.y + 7.f, vPos.z + iRandZ };

				My_Vec3 pos = vPos - *m_pComponent_Transform->Get_Information(CTransform::INFO_LOOK) * 20.f;

				dynamic_cast<CBossMoney*>(pObject)->Set_Position(pos);
			}

			mIsDead = true;
			return My_Int();
		}

		m_fAccTime2 += fTimeDelta;

		if (m_vPosition.z > 110.f)
		{
			m_vPosition = m_vPosition + m_vLook * 3.f * fTimeDelta;
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &m_vPosition);
		}
		

	/*	if (GetAsyncKeyState('W') & 0x8000)
		{
			m_pComponent_Vehicle->Accelerate();
		}

		if (GetAsyncKeyState('S') & 0x8000) {
			m_pComponent_Vehicle->Accelerate_Reverse();
		}

		if (GetAsyncKeyState('A') & 0x8000) {
			m_pComponent_Vehicle->Turn_Left();
		}

		if (GetAsyncKeyState('D') & 0x8000) {
			m_pComponent_Vehicle->Turn_Right();
		}*/

		if (true == m_bIsFirst2)
		{
			m_bIsFirst2 = false;
			m_fFirstTime2 = fTimeDelta;
		}

		//m_fAccTime2 += fTimeDelta;

		if (1.2f < (m_fAccTime2 - m_fFirstTime2))
		{
			if (m_bIsWater == false)
			{
				CGameObject* 	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Boss_Skill");
				if (pObject == nullptr)
					return E_FAIL;
				if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Boss_Skill", pObject)))
					return E_FAIL;

				dynamic_cast<CBoss_Skill*>(pObject)->Set_Boss_Matrix(m_pComponent_Transform->Get_WorldMatrix());
				
				CSoundMgr::GetInstance()->PlaySound(L"WaterItem.ogg", CSoundMgr::Channel_Effect);
			}

			m_fFirstTime2 = m_fAccTime2;
		}

		// 보스 피가 7이면 바나나 깔기
		const CGameObject* 	pObject = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Boss_HP", 8);

		if (nullptr == pObject)
		{
			if (true == m_bIsFirst3)
			{
				m_bIsFirst3 = false;
				m_fFirstTime3 = fTimeDelta;
			}

			m_fAccTime3 += fTimeDelta;

			if (5.0f < (m_fAccTime3 - m_fFirstTime3))
			{
				CSoundMgr::GetInstance()->PlaySound(L"SmallPlayer.ogg", CSoundMgr::Channel_Effect);

				for (int i = 0; i < 15; ++i)
				{
					CGameObject* 	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Banana");
					if (pObject == nullptr)
						return E_FAIL;
					if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Banana", pObject)))
						return E_FAIL;

					My_Int		iRandX = rand() % 40 - 20;
					My_Int		iRandY = rand() % 50 - 20;
					My_Int		iRandZ = rand() % 60 - 30;

					My_Vec3		vPos = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
					vPos = { vPos.x + iRandX, vPos.y + 7.f, vPos.z + iRandZ };

					My_Vec3 pos = vPos + *m_pComponent_Transform->Get_Information(CTransform::INFO_LOOK) * 20.f;

					dynamic_cast<CItem_Banana*>(pObject)->Set_IsBoss(true);
					dynamic_cast<CItem_Banana*>(pObject)->Set_Position(pos);
				}

				m_fFirstTime3 = m_fAccTime3;
			}
		}

		pObject = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Boss_HP", 6);

		if (nullptr == pObject)
		{
			if (true == m_bIsFirst4)
			{
				m_bIsFirst4 = false;
				m_fFirstTime4 = fTimeDelta;
			}

			m_fAccTime4 += fTimeDelta;

			if (5.0f < (m_fAccTime4 - m_fFirstTime4))
			{
				CSoundMgr::GetInstance()->PlaySound(L"DevilShoot.ogg", CSoundMgr::Channel_Effect);

				for (int i = 0; i < 15; ++i)
				{
					CGameObject* 	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Devil_Skill");
					if (pObject == nullptr)
						return E_FAIL;
					if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Devil_Skill", pObject)))
						return E_FAIL;

					My_Int		iRandX = rand() % 40 - 20;
					My_Int		iRandY = rand() % 50 - 20;
					My_Int		iRandZ = rand() % 60 - 30;

					My_Vec3		vPos = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
					vPos = { vPos.x + iRandX, vPos.y + 7.f, vPos.z + iRandZ };

					My_Vec3 pos = vPos + *m_pComponent_Transform->Get_Information(CTransform::INFO_LOOK) * 20.f;

					dynamic_cast<CDevil_Skill*>(pObject)->Set_Position(pos);
				}

				m_fFirstTime4 = m_fAccTime4;
			}
		}

		const My_Vec3*	pos = m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
		const My_Vec3*	vLook = m_pComponent_Transform->Get_Information(CTransform::INFO_LOOK);

		m_pComponent_Vehicle->Update(fTimeDelta, vLook, m_bLanded);

		const CTerrain*		pTerrain = (CTerrain*)CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Terrain", 0);

		//// 물폭탄에 맞으면 NavMesh 타지 말기
		//if (false == m_bIsWater)
		//{
		if (false == m_bIsWater)
		{
			if (m_pComponent_Transform->Move_On_NavMesh(&(m_pComponent_Vehicle->Get_Velocity() * fTimeDelta), pTerrain->Get_Buffer(), fTimeDelta, m_pComponent_Vehicle, &m_bLanded) == false) {
				m_pComponent_Transform->Set_NavMesh(m_pComponent_NavMesh->Get_FromPos(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION)));
			}
			m_pComponent_Transform->Rotation_Y(m_pComponent_Vehicle->Get_AngularVelocity());

		}

		m_pComponent_Transform->Rotation_Y(m_pComponent_Vehicle->Get_AngularVelocity());
	}

	if (true == m_bIsMissileTarget)
	{
		// 마그넷을 먹었다면!
		// 삼각형 메쉬 생성하기
		CGameObject* pRayTri2 = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Ray_Tri2");
		if (pRayTri2 == nullptr)
			return E_FAIL;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Ray_Tri2", pRayTri2)))
			return E_FAIL;

		dynamic_cast<CRay_Tri2*>(pRayTri2)->Set_Target_Transform(m_pComponent_Transform);
		dynamic_cast<CRay_Tri2*>(pRayTri2)->Set_IsBoss(true);

		CGameObject* pRayTri = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Ray_Tri");
		if (pRayTri == nullptr)
			return E_FAIL;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Ray_Tri", pRayTri)))
			return E_FAIL;

		dynamic_cast<CRay_Tri*>(pRayTri)->Set_Target_Transform(m_pComponent_Transform);
		dynamic_cast<CRay_Tri*>(pRayTri)->Set_IsBoss(true);

		m_bIsMissileTarget = false;
	}

	return My_Int();
}

My_Int CCrocodile::LastUpdate(const My_Float & fTimeDelta)
{
	if (!mIsDead)
	{
		m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

		if ((4 == m_iState && NONE == mItemState) || WATERBOMB == mItemState)			// 물폭탄과 충돌했을 때
		{
			if (true == m_bIsFirst)
			{
				mItemState = WATERBOMB;
				m_fFirstTime = fTimeDelta;
				m_bIsFirst = false;

				m_vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);

				// Item_Water_Bomb
				CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Water_Bomb");
				if (pObject == nullptr)
					return E_FAIL;
				if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Water_Bomb", pObject)))
					return E_FAIL;

				dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Number(2);
				dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_IsBoss(true);
				dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Player_Transform(m_pComponent_Transform);
			}

			m_fAccTime += fTimeDelta;

			if (2.f > (m_fAccTime - m_fFirstTime))
			{
				m_bIsWater = true;
				m_fSpeed += 0.002f;
				My_Vec3 vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
				m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(m_vPosition.x, vPosition.y + m_fSpeed, m_vPosition.z));
			}
			else if (2.f < (m_fAccTime - m_fFirstTime) && 4.f > (m_fAccTime - m_fFirstTime))
			{
				m_fAccSpeed += 0.002f;
				My_Vec3 vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
				if (m_vPosition.y <= vPosition.y)
					m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(m_vPosition.x, vPosition.y - m_fAccSpeed, m_vPosition.z));
			}
			else
			{
				m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(m_vPosition.x, m_vPosition.y, m_vPosition.z));

				m_fSpeed = 0.f;
				m_fAccSpeed = 0.f;
				m_fAccTime = 0.f;
				m_fFirstTime = 0.f;
				m_bIsFirst = true;
				m_bIsWater = false;

				m_iState = 0;
				mItemState = NONE;
			}
		}

		if ((6 == m_iState && NONE == mItemState) || MISSILE == mItemState)		// 미사일과 충돌했을 때
		{
			if (true == m_bIsFirst)
			{
				m_fFirstTime = fTimeDelta;
				m_bIsFirst = false;
				m_vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);

				--mHP;

				CGameObject* pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Boss_HP", mHP));
				if (nullptr != pGameObject)
					dynamic_cast<CBoss_HP*>(pGameObject)->Set_IsCol(true);
				mItemState = MISSILE;

				CSoundMgr::GetInstance()->PlaySound(L"Bomb.ogg", CSoundMgr::Channel_Item);
			}

			m_fAccTime += fTimeDelta;

			if (0.5f > (m_fAccTime - m_fFirstTime))
			{
				m_bIsWater = true;
				m_fSpeed += 0.01f;
				My_Vec3 vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
				m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(m_vPosition.x, vPosition.y + m_fSpeed, m_vPosition.z));
			}
			else if (0.5f < (m_fAccTime - m_fFirstTime) && 1.f > (m_fAccTime - m_fFirstTime))
			{
				m_fAccSpeed += 0.01f;
				My_Vec3 vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
				if (m_vPosition.y <= vPosition.y)
					m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(m_vPosition.x, vPosition.y - m_fAccSpeed, m_vPosition.z));
			}
			else
			{
				m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(m_vPosition.x, m_vPosition.y, m_vPosition.z));

				m_fSpeed = 0.f;
				m_fAccSpeed = 0.f;
				m_fAccTime = 0.f;
				m_fFirstTime = 0.f;
				m_bIsFirst = true;
				m_bIsWater = false;

				m_iState = 0;
				mItemState = NONE;
			}
		}

		m_pCollisioner->Add_Collision(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION), 10.0f, 12, &m_iState);
	}

	return My_Int();
}

void CCrocodile::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CCrocodile * CCrocodile::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCrocodile*		pInstance = new CCrocodile(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CCrocodile::Clone(void)
{
	CCrocodile*		pInstance = new CCrocodile(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CCrocodile::ReleaseMemory(void)
{
	Safe_Release(m_pComponent_Vehicle);
	Safe_Release(m_pCollisioner);
	Safe_Release(m_pComponent_NavMesh);

	CModel::ReleaseMemory();
	return My_uLong();
}
