#include "stdafx.h"
#include "..\Header\Dao.h"

#include "Terrain.h"
#include "GameObjectMgr.h"

#include "Item_Booster.h"
#include "Item_Water_Bomb.h"

#include "Ray_Tri.h"
#include "Ray_Tri2.h"

#include "RankingManager.h"
#include "Item_Banana.h"
#include "Item_Cloud.h"
#include "Item_Water_Bomb.h"
#include "Item_Devil.h"
#include "Item_First.h"
#include "Item_Missile.h"
#include "Item_Water_Fly.h"
#include "Item_UFO.h"

#include "Bazzi.h"
#include "SoundManager.h"

CDao::CDao(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
	, mComponentVehicle(nullptr)
	, mComponentNavMesh(nullptr)
	, mCollisioner(nullptr)
	, mCurrentData(nullptr)
	, mState(0)
	, mAccSpeed(0.f)
	, mAccTime(0.f)
	, mFirstTime(0.f)
	, mIsFirst(true)
	, mSpeed(0.f)
	, mIsWater(false)
	, mIsHit(false)
	, mMissileY1(0.f)
	, mMissileY2(0.f)
	, mReceive(0)
	, mIsTargeted(false)
	, mItemUseTime(0.f)
	, mIsMissileTarget(false)
{
	D3DXMatrixIdentity(&mPreWorld);
	srand(unsigned(time(NULL)));
}

CDao::CDao(const CDao & Ref)
	: CModel(Ref)
	, mComponentVehicle(Ref.mComponentVehicle)
	, mCollisioner(Ref.mCollisioner)
	, mCurrentData(Ref.mCurrentData)
	, mState(Ref.mState)
	, mAccSpeed(Ref.mAccSpeed)
	, mAccTime(Ref.mAccTime)
	, mFirstTime(Ref.mFirstTime)
	, mIsFirst(Ref.mIsFirst)
	, mSpeed(Ref.mSpeed)
	, mIsWater(Ref.mIsWater)
	, mIsHit(Ref.mIsHit)
	, mMissileY1(Ref.mMissileY1)
	, mMissileY2(Ref.mMissileY2)
	, mReceive(0)
	, mIsTargeted(false)
	, mPreWorld(Ref.mPreWorld)
	, mComponentNavMesh(Ref.mComponentNavMesh)
	, mIsMissileTarget(Ref.mIsMissileTarget)
	, mItemState(NONE)
	, mIsStart(Ref.mIsStart)
	, mIsGoal(Ref.mIsGoal)
	, mGoalTime(Ref.mGoalTime)
{
}

HRESULT CDao::Initialize()
{
	if (FAILED(CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_")))
		return E_FAIL;

	// Vehicle
	mComponentVehicle = (CVehicle*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Vehicle");
	if (mComponentVehicle == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Vehicle", mComponentVehicle)))
		return E_FAIL;
	mComponentVehicle->Set_Inform(50.0f, 2500.0f, 1.0f);

	mComponentNavMesh = (CNavMesh*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_NavMesh");
	if (mComponentNavMesh == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_NavMesh", mComponentNavMesh)))
		return E_FAIL;

	// Collision
	mCollisioner = (CCollisioner*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (mCollisioner == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Collisioner", mCollisioner)))
		return E_FAIL;

	// Transform
	m_pComponent_Transform->Scaling(0.125f, 0.125f, 0.125f);
	m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(420.f, 6.5f, 286.5f));

	mItemVec.reserve(2);

	Load(0);
	mRecordList.pop_front();

	return S_OK;
}

HRESULT CDao::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Character/Dao_Cart_Client.dat");
}

My_Int CDao::Update(const My_Float & fTimeDelta)
{
	if (mIsStop)
		return My_Int();

	if (mIsGoal)
		mGoalTime += fTimeDelta;

	if (GetAsyncKeyState('Z') || (mIsStart && (!mIsGoal || (mIsGoal && mGoalTime < 1.f))))
	{
		const My_Vec3*	vPos = m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
		const My_Vec3*	vLook = m_pComponent_Transform->Get_Information(CTransform::INFO_LOOK);
		const My_Vec3*	vRight = m_pComponent_Transform->Get_Information(CTransform::INFO_RIGHT);

		if (3 != mState)
		{
			if (nullptr == mCurrentData)
			{
				mCurrentData = mRecordList.front();
				mRecordList.pop_front();
			}

			mComponentVehicle->Accelerate();

			My_Vec3 vecDir = mCurrentData->pos - *vPos;
			My_Vec3 vecLook = *vLook;
			My_Vec3 vecRight = *vRight;

			D3DXVec3Normalize(&vecDir, &vecDir);
			D3DXVec3Normalize(&vecLook, &vecLook);
			D3DXVec3Normalize(&vecRight, &vecRight);

			My_Float angle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecLook)));

			/*if (angle > 120.f)
			Safe_Delete(mCurrentData);*/

			if (angle > 5.f)
			{
				My_Float rightAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecRight)));

				if (rightAngle > 90.f)
					mComponentVehicle->Turn_Left();

				else
					mComponentVehicle->Turn_Right();
			}



			//else
			/*{
			if (mCurrentData->keyState & 0x01)
			mComponentVehicle->Accelerate();

			if (mCurrentData->keyState & 0x02)
			mComponentVehicle->Accelerate_Reverse();

			if (mCurrentData->keyState & 0x04)
			mComponentVehicle->Turn_Left();

			if (mCurrentData->keyState & 0x08)
			mComponentVehicle->Turn_Right();
			}
			*/
			if (nullptr != mCurrentData && mRecordList.size() != 0 && D3DXVec3Length(&(*vPos - mCurrentData->pos)) < 8.f)
				Safe_Delete(mCurrentData);

			mComponentVehicle->Update(fTimeDelta, vLook, m_bLanded);

			const CTerrain*		pTerrain = (CTerrain*)CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Terrain", 0);

			// 물폭탄에 맞으면 NavMesh 타지 말기
			if (false == mIsWater)
			{
				if (m_pComponent_Transform->Move_On_NavMesh(&(mComponentVehicle->Get_Velocity() * fTimeDelta), pTerrain->Get_Buffer(), fTimeDelta, mComponentVehicle, &m_bLanded) == false) {
					m_pComponent_Transform->Set_NavMesh(mComponentNavMesh->Get_FromPos(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION)));
				}
				m_pComponent_Transform->Rotation_Y(mComponentVehicle->Get_AngularVelocity());
			}
			m_pComponent_Transform->Rotation_Y(mComponentVehicle->Get_AngularVelocity());
		}
	}

	if (mIsTargeted)
	{
		// 마그넷을 먹었다면!
		// 삼각형 메쉬 생성하기
		CGameObject* pRayTri2 = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Ray_Tri2");
		if (pRayTri2 == nullptr)
			return 0;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Ray_Tri2", pRayTri2)))
			return 0;

		dynamic_cast<CRay_Tri2*>(pRayTri2)->Set_Target_Transform(m_pComponent_Transform);
		dynamic_cast<CRay_Tri2*>(pRayTri2)->Set_Target_Num(1);

		CGameObject* pRayTri = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Ray_Tri");
		if (pRayTri == nullptr)
			return 0;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Ray_Tri", pRayTri)))
			return 0;

		dynamic_cast<CRay_Tri*>(pRayTri)->Set_Target_Transform(m_pComponent_Transform);
		dynamic_cast<CRay_Tri*>(pRayTri)->Set_Target_Num(1);


		mIsTargeted = false;
	}

	return My_Int();
}

My_Int CDao::LastUpdate(const My_Float & fTimeDelta)
{
	if (!mIsGoal)
	{
		Update_Collision_Tetris(fTimeDelta);
		UseItem(fTimeDelta);
		Collision(fTimeDelta);

	}
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return My_Int();
}

void CDao::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CDao::SetStartPos()
{
	My_Matrix	matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pComponent_Transform->Set_Information(&matWorld);

	// Transform
	m_pComponent_Transform->Scaling(0.125f, 0.125f, 0.125f);
	m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(420.f, 6.5f, 286.5f));
	// mComponentVehicle->Set_Inform(50.0f, 0.f, 1.0f);
}

void CDao::Load(int recordNum)
{
	HANDLE hFile = CreateFile(L"../Bin/Data/Record8.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Record Load Failed!!", L"파일 불러오기 오류", MB_OK);
		return;
	}

	RECORD_DATA record;
	DWORD readByte = 0;

	while (true)
	{
		ReadFile(hFile, &record, sizeof(RECORD_DATA), &readByte, nullptr);

		if (0 == readByte)
			break;

		mRecordList.push_back(new RECORD_DATA(record));
	}

	CloseHandle(hFile); // 개방한 파일 핸들 닫기
}

void CDao::Collision(const My_Float& fTimeDelta)
{
	if (2 == mState) // 아이템 박스와 충돌
	{
		if (mItemVec.size() < 2)
		{
			My_Int randItem;

			if (CRankingManager::DAO != CRankingManager::GetInstance()->GetFrist())
			{
				if (CRankingManager::BAZZI == CRankingManager::GetInstance()->GetFrontRacer(CRankingManager::DAO))
				{
					randItem = rand() % 2;

					if (0 == randItem)
						randItem = 2;
					else if (1 == randItem)
						randItem = 9;
				}
				else
				{
					randItem = rand() % 5;

					if (2 == randItem)
						randItem = 5;

					else if (4 == randItem)
						randItem = 6;
				}
			}

			else
			{
				randItem = rand() % 3;

				if (2 == randItem)
					randItem = 5;
			}

			mItemVec.push_back(randItem);
		}

		mState = 0;
	}

	if (3 == mState || mItemState == BANANA)	// 바나나와 충돌했을 때
	{
		if (0.f == mAccSpeed)
		{
			mItemState = BANANA;
			mPreWorld = *m_pComponent_Transform->Get_WorldMatrix();
		}

		mAccSpeed += 0.05f;
		m_pComponent_Transform->Rotation_Y(((10.6462f - (mAccSpeed)) * fTimeDelta));

		// 속도가 다 줄었으면 원래 값으로 초기화
		if (10.7f < mAccSpeed)
		{
			mAccSpeed = 0.f;
			mState = 0;

			// mIsBreakaway = true;
			m_pComponent_Transform->Set_Information(&mPreWorld);
			mItemState = NONE;
		}
	}

	if (4 == mState || mItemState == WATERBOMB)		// 물폭탄과 충돌했을 때
	{
		if (true == mIsFirst)
		{
			mItemState = WATERBOMB;
			mFirstTime = fTimeDelta;
			mIsFirst = false;

			mPreWorld = *m_pComponent_Transform->Get_WorldMatrix();

			// Item_Water_Bomb
			CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Water_Bomb");
			if (pObject == nullptr)
				return;
			if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Water_Bomb", pObject)))
				return;

			dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Number(2);
			dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Player_Transform(m_pComponent_Transform);
		}

		mAccTime += fTimeDelta;

		if (2.f > (mAccTime - mFirstTime))
		{
			mIsWater = true;
			mSpeed += 0.001f;
			My_Vec3 vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(mPreWorld._41, vPosition.y + mSpeed, mPreWorld._43));
		}
		else if (2.f < (mAccTime - mFirstTime) && 4.f > (mAccTime - mFirstTime))
		{
			mAccSpeed += 0.001f;
			My_Vec3 vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
			if (mPreWorld._42 <= vPosition.y)
				m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(mPreWorld._41, vPosition.y - mAccSpeed, mPreWorld._43));
		}
		else
		{
			mSpeed = 0.f;
			mAccSpeed = 0.f;
			mAccTime = 0.f;
			mFirstTime = 0.f;
			mIsFirst = true;
			mIsWater = false;
			mItemState = NONE;

			m_pComponent_Transform->Set_Information(&mPreWorld);

			mState = 0;
		}
	}

	if ((6 == mState || mItemState == MISSILE) && mIsMissileTarget) // 미사일과 충돌
	{
		if (true == mIsFirst)
		{
			mFirstTime = fTimeDelta;
			mPreWorld = *m_pComponent_Transform->Get_WorldMatrix();
			mIsFirst = false;
			mItemState = MISSILE;
		}

		mAccTime += fTimeDelta;

		if (2.f > (mAccTime - mFirstTime))
		{
			mIsHit = true;
			mMissileY1 += 0.1f;
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(mPreWorld._41, mPreWorld._42 + mMissileY1, mPreWorld._43));
			m_pComponent_Transform->Rotation_X(20.f * fTimeDelta);

		}
		else if (2.f < (mAccTime - mFirstTime) && 3.9f > (mAccTime - mFirstTime))
		{
			mMissileY2 += 0.1f;
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(mPreWorld._41, mPreWorld._42 + mMissileY1 - mMissileY2, mPreWorld._43));
			m_pComponent_Transform->Rotation_X(20.f * fTimeDelta);
		}
		else
		{
			mAccTime = 0.f;
			mMissileY1 = 0.f;
			mMissileY2 = 0.f;
			mIsFirst = true;
			mIsHit = false;
			m_pComponent_Transform->Set_Information(&mPreWorld);

			mState = 0;
			mIsMissileTarget = false;
			mItemState = NONE;
		}
	}

	if (7 == mState) // 부스터박스와 충돌
	{
		// Item_Booster
		CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Booster");
		if (pObject == nullptr)
			return;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Booster", pObject)))
			return;

		static_cast<CItem_Booster*>(pObject)->SetIsPlayer(false);
		static_cast<CItem_Booster*>(pObject)->Set_Player_Vehicle(mComponentVehicle);
	}

	if ((9 == mState || mItemState == WATERFLY) && mIsWaterFlyTarget)		// 물파리와 충돌했을 때
	{
		if (true == mIsFirst)
		{
			mFirstTime = fTimeDelta;
			mIsFirst = false;
			mItemState = WATERFLY;

			mPreWorld = *m_pComponent_Transform->Get_WorldMatrix();

			// Item_Water_Bomb
			CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Water_Bomb");
			if (pObject == nullptr)
				return;
			if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Water_Bomb", pObject)))
				return;

			dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Number(2);
			dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Player_Transform(m_pComponent_Transform);
		}

		mAccTime += fTimeDelta;

		if (2.f > (mAccTime - mFirstTime))
		{
			mIsWater = true;
			mSpeed += 0.001f;
			My_Vec3 vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(mPreWorld._41, vPosition.y + mSpeed, mPreWorld._43));
		}
		else if (2.f < (mAccTime - mFirstTime) && 4.f > (mAccTime - mFirstTime))
		{
			mAccSpeed += 0.001f;
			My_Vec3 vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
			if (mPreWorld._42 <= vPosition.y)
				m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(mPreWorld._41, vPosition.y - mAccSpeed, mPreWorld._43));
		}
		else
		{
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(mPreWorld._41, mPreWorld._42, mPreWorld._43));

			mSpeed = 0.f;
			mAccSpeed = 0.f;
			mAccTime = 0.f;
			mFirstTime = 0.f;
			mIsFirst = true;
			mIsWater = false;
			mItemState = NONE;

			mState = 0;
		}
	}

	if (mState == 11) // 1등양보
	{
		mComponentVehicle->Set_Velocity_X(0.0f);
		mComponentVehicle->Set_Velocity_Y(0.0f);
		mComponentVehicle->Set_Velocity_Z(0.0f);
		mState = 0;
	}

	My_Vec3	vPos = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);

	mCollisioner->Add_Collision(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION), 1.0f, 5, &mState);
	mCollisioner->Add_CollisionAABB(vPos - My_Vec3(0.25f, 0.25f, 0.25f), vPos + My_Vec3(0.25f, 0.25f, 0.25f), 2, &mReceive, &m_vCol);
}

void CDao::UseItem(const My_Float& fTimeDelta)
{
	if (mItemVec.size() > 0)
		mItemUseTime += fTimeDelta;

	if (0 == mState && mItemVec.size() > 0 && mItemUseTime > 3.f)
	{
		switch (mItemVec[0])
		{
		case 0:		// 구름
		{
			ItemCloud();
		}
		break;
		case 1:		// 바나나
		{
			ItemBanana();
		}
		break;
		case 2:		// 물폭탄
		{
			ItemWaterBomb();
		}
		break;
		case 3:		// 1등 양보
		{
			ItemFirst();
		}
		break;
		case 4:		// 부스터
		{
			ItemBooster();
		}
		break;
		case 5:	// 악마
		{
			CSoundMgr::GetInstance()->PlaySound(L"Devil.ogg", CSoundMgr::Channel_Item);

			ItemDevil();
		}
		break;
		case 6:		// UFO
		{
			ItemUFO();
		}
		break;
		case 8:
		{
			ItemMissile();
		}
		break;
		case 9:
		{
			ItemWaterFly();
		}
		break;
		}

		mItemUseTime = 0.f;

		if (mItemVec.size() > 1)
			mItemVec[0] = mItemVec[1];

		mItemVec.pop_back();
	}
}

void CDao::ItemBanana()
{
	// Item_Banana
	CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Banana");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Banana", pObject)))
		return;

	// Player의 Transform 전달
	dynamic_cast<CItem_Banana*>(pObject)->Set_Player_Transform(m_pComponent_Transform);
}

void CDao::ItemCloud()
{
	// Item_Cloud
	CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Cloud");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Cloud", pObject)))
		return;

	// Player의 WorldMatrix 전달
	dynamic_cast<CItem_Cloud*>(pObject)->Set_Player_Transform(m_pComponent_Transform->Get_WorldMatrix());
}

void CDao::ItemWaterBomb()
{
	// Item_Water_Bomb
	CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Water_Bomb");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Water_Bomb", pObject)))
		return;

	dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Number(1);
	dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Player_Matrix(m_pComponent_Transform->Get_WorldMatrix());
}

void CDao::ItemFirst()
{
	const My_Vec3* vCenterPos = nullptr;

	if (CRankingManager::DAO != CRankingManager::GetInstance()->GetFrist())
	{
		const CGameObject* object = nullptr;

		if (CRankingManager::DIZNI == CRankingManager::GetInstance()->GetFrist())
			object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0);

		else if (CRankingManager::BAZZI == CRankingManager::GetInstance()->GetFrist())
			object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0);

		const My_Matrix* matWorld = const_cast<CTransform*>(static_cast<const CTransform*>(object->Get_ComponentPointer(L"Component_Transform")))->Get_WorldMatrix();

		for (int i = 0; i < 3; ++i)
		{
			// Item_Water_First
			CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_First");
			if (pObject == nullptr)
				return;
			if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_First", pObject)))
				return;

			// 플레이어 월드 행렬 보내기
			dynamic_cast<CItem_First*>(pObject)->Set_Player_Transform(matWorld);

			switch (i)
			{
			case 0:
				// 첫번째 객체의 위치를 저장하고
				vCenterPos = dynamic_cast<CItem_First*>(pObject)->Get_Position();
				break;
			case 1:
				// 두번째 세번째 객체는 첫번째 객체의 위치를 기반으로 위치를 잡는다.
				dynamic_cast<CItem_First*>(pObject)->Set_CenterPos(vCenterPos);
				break;
			case 2:
				dynamic_cast<CItem_First*>(pObject)->Set_CenterPos(vCenterPos);
				break;
			}
			dynamic_cast<CItem_First*>(pObject)->Set_Num(i);
		}
	}
}

void CDao::ItemBooster()
{
	// Item_Booster
	CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Booster");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Booster", pObject)))
		return;

	static_cast<CItem_Booster*>(pObject)->SetIsPlayer(false);
	static_cast<CItem_Booster*>(pObject)->Set_Player_Vehicle(mComponentVehicle);
}

void CDao::ItemDevil()
{
	const CGameObject* object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0);
	CTransform* daoTransform = const_cast<CTransform*>(static_cast<const CTransform*>(object->Get_ComponentPointer(L"Component_Transform")));

	object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0);
	CTransform* dizniTransform = const_cast<CTransform*>(static_cast<const CTransform*>(object->Get_ComponentPointer(L"Component_Transform")));
	const_cast<CBazzi*>(static_cast<const CBazzi*>(object))->Set_IsDevil(true);

	// Dao
	// Item_Devil
	CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Devil");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Devil", pObject)))
		return;

	dynamic_cast<CItem_Devil*>(pObject)->Set_Player_Tramsform(daoTransform);
	dynamic_cast<CItem_Devil*>(pObject)->Set_Num(1);

	// Item_Devil
	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Devil");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Devil", pObject)))
		return;

	// 배찌 머리 위에 악마를 생성하기 위해 전달
	dynamic_cast<CItem_Devil*>(pObject)->Set_Player_Tramsform(daoTransform);
	dynamic_cast<CItem_Devil*>(pObject)->Set_Num(2);

	// Item_Devil
	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Devil");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Devil", pObject)))
		return;

	// 배찌 머리 위에 악마를 생성하기 위해 전달
	dynamic_cast<CItem_Devil*>(pObject)->Set_Player_Tramsform(daoTransform);
	dynamic_cast<CItem_Devil*>(pObject)->Set_Num(3);

	// Item_Devil
	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Devil");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Devil", pObject)))
		return;

	// 배찌 머리 위에 악마를 생성하기 위해 전달
	dynamic_cast<CItem_Devil*>(pObject)->Set_Player_Tramsform(daoTransform);
	dynamic_cast<CItem_Devil*>(pObject)->Set_Num(4);



	// Dizni
	// Item_Devil
	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Devil");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Devil", pObject)))
		return;

	dynamic_cast<CItem_Devil*>(pObject)->Set_Player_Tramsform(dizniTransform);
	dynamic_cast<CItem_Devil*>(pObject)->Set_Num(1);

	// Item_Devil
	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Devil");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Devil", pObject)))
		return;

	// 배찌 머리 위에 악마를 생성하기 위해 전달
	dynamic_cast<CItem_Devil*>(pObject)->Set_Player_Tramsform(dizniTransform);
	dynamic_cast<CItem_Devil*>(pObject)->Set_Num(2);

	// Item_Devil
	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Devil");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Devil", pObject)))
		return;

	// 배찌 머리 위에 악마를 생성하기 위해 전달
	dynamic_cast<CItem_Devil*>(pObject)->Set_Player_Tramsform(dizniTransform);
	dynamic_cast<CItem_Devil*>(pObject)->Set_Num(3);

	// Item_Devil
	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Devil");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Devil", pObject)))
		return;

	// 배찌 머리 위에 악마를 생성하기 위해 전달
	dynamic_cast<CItem_Devil*>(pObject)->Set_Player_Tramsform(dizniTransform);
	dynamic_cast<CItem_Devil*>(pObject)->Set_Num(4);
}

void CDao::ItemMissile()
{
	My_Int targetNum;
	CRankingManager::Racer frontRacer = CRankingManager::GetInstance()->GetFrontRacer(CRankingManager::DAO);

	if (CRankingManager::BAZZI == frontRacer)
		targetNum = 0;
	else if (CRankingManager::DIZNI == frontRacer)
		targetNum = 2;
	else
		return;
	
	CSoundMgr::GetInstance()->PlaySound(L"Bomb.ogg", CSoundMgr::Channel_Item);


	// Item_Missile
	CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Missile");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Missile", pObject)))
		return;
	dynamic_cast<CItem_Missile*>(pObject)->Set_Player_Tramsform(m_pComponent_Transform);
	dynamic_cast<CItem_Missile*>(pObject)->Set_Target_Num(targetNum);
}

void CDao::ItemUFO()
{
	CRankingManager::Racer firstRacer = CRankingManager::GetInstance()->GetFrist();

	if (CRankingManager::DAO != firstRacer)
	{
		CGameObject*  pGameObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_UFO");
		if (pGameObject == nullptr)
			return;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_UFO", pGameObject)))
			return;

		dynamic_cast<CItem_UFO*>(pGameObject)->Set_Transform(m_pComponent_Transform);

		const CGameObject* object = nullptr;
		if (CRankingManager::DIZNI == firstRacer)
		{
			object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0);
			static_cast<CItem_UFO*>(pGameObject)->SetTargetNum(2);
		}

		else if (CRankingManager::BAZZI == firstRacer)
		{
			CSoundMgr::GetInstance()->PlaySound(L"UFO1.ogg", CSoundMgr::Channel_Item);

			object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0);
			static_cast<CItem_UFO*>(pGameObject)->SetTargetNum(0);
		}
		static_cast<CItem_UFO*>(pGameObject)->SetTarget(const_cast<CGameObject*>(object));
	}
}

void CDao::ItemWaterFly()
{
	CRankingManager::Racer frontRacer = CRankingManager::GetInstance()->GetFrontRacer(CRankingManager::DAO);

	if (CRankingManager::DAO != frontRacer && CRankingManager::RACER_END != frontRacer)
	{
		CGameObject*  pGameObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Water_Fly");
		if (pGameObject == nullptr)
			return;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Water_Fly", pGameObject)))
			return;

		static_cast<CItem_Water_Fly*>(pGameObject)->Set_Transform(m_pComponent_Transform);
		const CGameObject* object = nullptr;

		if (CRankingManager::DIZNI == frontRacer)
		{
			object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0);
			static_cast<CItem_Water_Fly*>(pGameObject)->SetTarget(2);
		}

		else if (CRankingManager::BAZZI == frontRacer)
		{
			CSoundMgr::GetInstance()->PlaySound(L"WaterFlyWater.ogg", CSoundMgr::Channel_Item);

			object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0);
			static_cast<CItem_Water_Fly*>(pGameObject)->SetTarget(0);
		}

		static_cast<CItem_Water_Fly*>(pGameObject)->SetTargetTrans(const_cast<CTransform*>(static_cast<const CTransform*>(object->Get_ComponentPointer(L"Component_Transform"))));
	}
}

HRESULT CDao::Update_Collision_Tetris(const My_Float & fTimeDelta)
{
	if (m_fFlatTime != 0.0f) {
		m_fFlatTime += fTimeDelta;
		My_Float	fHeight = fabs(cosf(m_fFlatTime * 2.0f)) * 0.025f;
		m_pComponent_Transform->Scaling(0.125f, 0.02f + fHeight, 0.125f);
	}

	if (m_fFlatTime >= 5.0f) {
		m_fFlatTime = 0.0f;
		m_pComponent_Transform->Scaling(0.125f, 0.125f, 0.125f);
	}

	if (mReceive < 5 || mReceive > 24)
		return S_OK;

	My_Float	fAbs_X = fabs(m_vCol.x);
	My_Float	fAbs_Y = fabs(m_vCol.y);
	My_Float	fAbs_Z = fabs(m_vCol.z);



	if (fAbs_Y < fAbs_X && fAbs_Y < fAbs_Z) {
		m_fFlatTime += fTimeDelta;
		//m_pComponent_Transform->Scaling(0.125f, 0.02f, 0.125f);
		mReceive = 0;
	}

	if (m_fFlatTime != 0.0f) {
		mReceive = 0;
		return S_OK;
	}

	m_vCol.y = 0.0f;
	if (fAbs_X >= fAbs_Z)
		m_vCol.x = 0.0f;
	else
		m_vCol.z = 0.0f;
	m_pComponent_Transform->Move_Free(&m_vCol, 1.0f);

	My_Vec3		vVel = mComponentVehicle->Get_Velocity();
	if (m_vCol.x == 0) {
		mComponentVehicle->Apply_Velocity(My_Vec3(0.0f, 5.0f, 0.0f));
		mComponentVehicle->Set_Velocity_Z(vVel.z * -1.0f);
	}
	else {
		mComponentVehicle->Apply_Velocity(My_Vec3(0.0f, 5.0f, 0.0f));
		mComponentVehicle->Set_Velocity_X(vVel.x * -1.0f);
	}

	mReceive = 0;

	return S_OK;
}

CDao * CDao::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDao*		pInstance = new CDao(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CDao::Clone(void)
{
	CDao*		pInstance = new CDao(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CDao::ReleaseMemory(void)
{
	Safe_Release(mComponentVehicle);
	Safe_Release(mCollisioner);
	Safe_Release(mComponentNavMesh);

	CModel::ReleaseMemory();
	return My_uLong();
}
