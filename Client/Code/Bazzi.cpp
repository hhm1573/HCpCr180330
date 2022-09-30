#include "stdafx.h"
#include "..\Header\Bazzi.h"
#include "GameObjectMgr.h"
#include "Terrain.h"

#include "InputDev.h"
#include "Vehicle.h"
#include "GraphicDev.h"

#include "Item_Banana.h"
#include "Item_Cloud.h"
#include "Item_Cloud_Small.h"
#include "Item_Water_Bomb.h"
#include "Item_First.h"
#include "Item_UFO.h"
#include "Item_Booster.h"
#include "Item_Icon.h"
#include "Item_Devil.h"
#include "Item_Magnet.h"
#include "Item_Missile.h"
#include "Item_Water_Fly.h"

#include "Smoke_Effect.h"

#include "Dao.h"
#include "Dizni.h"

#include "Camera_Target.h"
#include "Camera_MiniMap.h"

#include "RecordingManager.h"
#include "RankingManager.h"

#include "SkidMark.h"
#include "Crocodile.h"
#include "SoundManager.h"

CBazzi::CBazzi(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
	, m_pComponent_Vehicle(nullptr)
	, m_pCollisioner(nullptr)
	, m_iState(0)
	, m_pMagnet(nullptr)
{
	srand(unsigned(time(NULL)));
}

CBazzi::CBazzi(const CBazzi & Ref)
	:CModel(Ref)
	, m_pComponent_Vehicle(Ref.m_pComponent_Vehicle)
	, m_pCollisioner(Ref.m_pCollisioner)
	, m_bIsRotate(Ref.m_bIsRotate)
	, m_iState(Ref.m_iState)
	, m_bIsColBooster(Ref.m_bIsColBooster)
	, m_fBoosterColTime(Ref.m_fBoosterColTime)
	, mRecordingTime(Ref.mRecordingTime)
{
	if (nullptr != m_pComponent_Vehicle)
		m_pComponent_Vehicle->AddRef();

	m_iState = 0;
	m_iRand = 0;
	m_IconVec;
	m_iItem;
	m_fSpeed = 0.f;
	m_fAccSpeed = 0.f;
	m_fAccTime = 0.f;
	m_fFirstTime = 0.f;
	m_bIsFirst = true;
	m_bIsWater = false;
	m_vPosition = { 0.f, 0.f, 0.f };
	m_bIsDevil = false;
	m_bIsMagnet = false;
	m_bIsMissile = false;
	m_bIsRotate = false;
	m_iReceive = 0;
	mKeyState = 0x00; // 0000 0000
	m_fAccAngle = 0.f;
	m_fAccAngle2 = 0.f;
	m_bIsKeyUp = false;
	m_bIsFirst2 = true;
	m_bIsKeyUp2 = false;
	m_bIsKeyUp3 = false;
	m_bIsColBooster = false;
	m_fBoosterColTime = 0.f;
	m_bIsBooster = false;
	mItemState = NONE;
	mIsCounting = false;
	mIsGoal = false;
	mGoalTime = 0.f;
}

void CBazzi::SetStartPos()
{
	My_Matrix	matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pComponent_Transform->Set_Information(&matWorld);

	// Transform
	m_pComponent_Transform->Scaling(0.125f, 0.125f, 0.125f);
	m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(422.f, 6.5f, 286.5f));
	// m_pComponent_Vehicle->Set_Inform(50.0f, 0.f, 1.0f);
}

HRESULT CBazzi::Initialize(void)
{
	if (FAILED(CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_")))
		return E_FAIL;

	m_pComponent_Vehicle = (CVehicle*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Vehicle");
	if (m_pComponent_Vehicle == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Vehicle", m_pComponent_Vehicle)))
		return E_FAIL;
	m_pComponent_Vehicle->Set_Inform(50.0f, 2500.0f, 1.0f);

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

	const My_Vec3 vecPos = { 422.f, 0.f, 286.5f };
	m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &vecPos);
	m_pComponent_Transform->Scaling(0.125f, 0.125f, 0.125f);

	return S_OK;
}

HRESULT CBazzi::Initialize_Prototype(void)
{
	if (FAILED(CModel::Initialize_Prototype(L"../Bin/Data/Character/Bazzi_Cart_Client.dat")))
		return E_FAIL;



	if (FAILED(CModel::Move_Down(L"Bazzi_Ear_Right", 7.5f)))
		return E_FAIL;
	if (FAILED(CModel::Move_Right(L"Bazzi_Ear_Right", 5.0f)))
		return E_FAIL;
	if (FAILED(CModel::Move_Forward(L"Bazzi_Ear_Right", 2.0f)))
		return E_FAIL;
	if (FAILED(CModel::Scaling_X(L"Bazzi_Ear_Right", -1.55f)))
		return E_FAIL;
	if (FAILED(CModel::Scaling_Y(L"Bazzi_Ear_Right", -1.15f)))
		return E_FAIL;
	if (FAILED(CModel::Scaling_Z(L"Bazzi_Ear_Right", -1.05f)))
		return E_FAIL;



	if (FAILED(CModel::Move_Down(L"Bazzi_Ear_Left", 8.8f)))
		return E_FAIL;
	if (FAILED(CModel::Move_Left(L"Bazzi_Ear_Left", 3.49f)))
		return E_FAIL;
	if (FAILED(CModel::Move_Forward(L"Bazzi_Ear_Left", 2.0f)))
		return E_FAIL;
	if (FAILED(CModel::Scaling_X(L"Bazzi_Ear_Left", -1.55f)))
		return E_FAIL;
	if (FAILED(CModel::Scaling_Y(L"Bazzi_Ear_Left", -1.15f)))
		return E_FAIL;
	if (FAILED(CModel::Scaling_Z(L"Bazzi_Ear_Left", -1.05f)))
		return E_FAIL;

	return S_OK;
}

My_Int CBazzi::Update(const My_Float & fTimeDelta)
{
	if (mIsStop)
		return My_Int();

	if (mIsGoal)
		mGoalTime += fTimeDelta;

	if (mIsBoss || (!mIsCounting && (!mIsGoal || (mIsGoal && mGoalTime < 1.f))))
	{
		const My_Vec3*	pos = m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
		const My_Vec3*	vLook = m_pComponent_Transform->Get_Information(CTransform::INFO_LOOK);

		Recording(fTimeDelta);
		MovingKeyCheck();
		ItemKeyCheck();
		ItemCheatKeyCheck();
		Effect();
		Update_Animation(fTimeDelta);
		Update_Collision_Tetris(fTimeDelta);

		if (mIsBoss)
		{
			mMissileTime += fTimeDelta;
			if (mMissileTime > 8.f)
			{
				mMissileTime = 0.f;

				const int iIconSize = CGameObjectMgr::GetInstance()->Get_LayerSize(SCENE_STAGE, L"Object_UI");
				const CGameObject*      pGameObject = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Icon", 1);

				if (nullptr == pGameObject)
				{
					// Item_Icon         // 새로운 아이템 아이콘 오브젝트를 Clone 하자
					CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Icon");
					if (pObject == nullptr)
						return E_FAIL;
					if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Icon", pObject)))
						return E_FAIL;

					My_Int randItem = rand() % 3;

					if (0 == randItem)
						randItem = 8;

					else if (1 == randItem)
						randItem = 3;

					else if (2 == randItem)
						randItem = 2;

					dynamic_cast<CItem_Icon*>(pObject)->Set_Number(randItem);
					m_IconVec.push_back(pObject);		// 현재 아이템 아이콘 오브젝트를 벡터에 담기
					m_iItem.push_back(randItem);			// 현재 아이템 아이콘 넘버를 벡터에 담기
				}
			}
		}

		m_pComponent_Vehicle->Update(fTimeDelta, vLook, m_bLanded);

		const CTerrain*		pTerrain = (CTerrain*)CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Terrain", 0);

		// 물폭탄에 맞으면 NavMesh 타지 말기
		if (false == m_bIsWater)
		{
			////My_Matrix matWorld = *m_pComponent_Transform->Get_WorldMatrix();
			//My_Vec3		vLook = *m_pComponent_Transform->Get_Information(CTransform::INFO_LOOK);
			//vLook.x = 0.0f;
			//D3DXVec3Normalize(&vLook, &vLook);
			//My_Vec3		vAxisZ = My_Vec3(0.0f, 0.0f, 1.0f);
			//My_Float	fCosf = D3DXVec3Dot(&vLook, &vAxisZ);
			//My_Float	fRad = acosf(fCosf);
			//My_Float	fDeg = D3DXToDegree(fRad);

			//if (vLook.y < 0.0f)
			//	fRad *= -1.0f;

			//m_pComponent_Transform->Rotation_X(fRad);



			if (m_pComponent_Transform->Move_On_NavMesh(&(m_pComponent_Vehicle->Get_Velocity() * fTimeDelta), pTerrain->Get_Buffer(), fTimeDelta, m_pComponent_Vehicle, &m_bLanded) == false) {
				m_pComponent_Transform->Set_NavMesh(m_pComponent_NavMesh->Get_FromPos(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION)));
			}
			m_pComponent_Transform->Rotation_Y(m_pComponent_Vehicle->Get_AngularVelocity());
		}
		else {
			if (FAILED(Update_WaterTrap(fTimeDelta)))
				return -1;
		}

		if (m_iReceive == 2)
			int i = 0;
	}

	return My_Int();
}

My_Int CBazzi::LastUpdate(const My_Float & fTimeDelta)
{
	if (!mIsGoal)
		Collision(fTimeDelta);

	//const CTerrain*		pTerrain = (CTerrain*)CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Terrain", 0);
	//
	//if (false == m_bIsWater)		// 물폭탄에 갇혔을 때는 지형 타지 않게 하기
	//{
	//	//m_pComponent_Transform->Set_HeightOnBuffer(pTerrain->Get_Buffer());
	//}

	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	m_pCollisioner->Add_Collision(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION), 1.0f, 1, &m_iState);

	My_Vec3	vPos = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
	m_pCollisioner->Add_CollisionAABB(vPos - My_Vec3(0.25f, 0.25f, 0.25f), vPos + My_Vec3(0.25f, 0.25f, 0.25f), 1, &m_iReceive, &m_vCol);

	return My_Int();
}

void CBazzi::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

My_Float CBazzi::GetSpeed() const
{
	return D3DXVec3Length(&m_pComponent_Vehicle->Get_Velocity());
}

HRESULT CBazzi::Update_Collision_Tetris(const My_Float & fTimeDelta)
{
	if (m_fFlatTime != 0.0f) {

		if (false == m_bIsSmall)
		{
			CSoundMgr::GetInstance()->PlaySound(L"SmallPlayer.ogg", CSoundMgr::Channel_Player);
			m_bIsSmall = true;
		}

		m_fFlatTime += fTimeDelta;
		My_Float	fHeight = fabs(cosf(m_fFlatTime * 2.0f)) * 0.025f;
		m_pComponent_Transform->Scaling(0.125f, 0.02f + fHeight, 0.125f);
	}

	if (m_fFlatTime >= 5.0f) {
		m_fFlatTime = 0.0f;
		m_pComponent_Transform->Scaling(0.125f, 0.125f, 0.125f);
	}

	if (m_iReceive < 5 || m_iReceive > 24)
		return S_OK;

	My_Float	fAbs_X = fabs(m_vCol.x);
	My_Float	fAbs_Y = fabs(m_vCol.y);
	My_Float	fAbs_Z = fabs(m_vCol.z);



	if (fAbs_Y < fAbs_X && fAbs_Y < fAbs_Z) {
		m_fFlatTime += fTimeDelta;
		//m_pComponent_Transform->Scaling(0.125f, 0.02f, 0.125f);
		m_iReceive = 0;
	}

	if (m_fFlatTime != 0.0f) {
		m_iReceive = 0;
		return S_OK;
	}

	m_vCol.y = 0.0f;
	if (fAbs_X >= fAbs_Z)
		m_vCol.x = 0.0f;
	else
		m_vCol.z = 0.0f;
	m_pComponent_Transform->Move_Free(&m_vCol, 1.0f);

	My_Vec3		vVel = m_pComponent_Vehicle->Get_Velocity();
	if (m_vCol.x == 0) {
		m_pComponent_Vehicle->Apply_Velocity(My_Vec3(0.0f, 5.0f, 0.0f));
		m_pComponent_Vehicle->Set_Velocity_Z(vVel.z * -0.5f);
	}
	else {
		m_pComponent_Vehicle->Apply_Velocity(My_Vec3(0.0f, 5.0f, 0.0f));
		m_pComponent_Vehicle->Set_Velocity_X(vVel.x * -0.5f);
	}

	m_iReceive = 0;

	return S_OK;
}

HRESULT CBazzi::Update_WaterTrap(const My_Float & fTimeDelta)
{
	if (m_fSpeed == 0) {
		m_pComponent_Vehicle->Set_Velocity_X(0.0f);
		m_pComponent_Vehicle->Set_Velocity_Z(0.0f);
		m_pComponent_Vehicle->Apply_Velocity(My_Vec3(0.0f, 10.0f, 0.0f));
	}

	m_fSpeed += fTimeDelta;

	My_Int	iSpd = (My_Int)m_fSpeed;
	if (iSpd % 2 == 0) {
		m_pComponent_Vehicle->Apply_Velocity(My_Vec3(0.0f, 0.5f, 0.0f));
	}
	else {
		m_pComponent_Vehicle->Apply_Velocity(My_Vec3(0.0f, 0.9f, 0.0f));
	}

	const CTerrain*		pTerrain = (CTerrain*)CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Terrain", 0);
	m_pComponent_Transform->Move_On_NavMesh(&(m_pComponent_Vehicle->Get_Velocity() * fTimeDelta), pTerrain->Get_Buffer(), fTimeDelta, m_pComponent_Vehicle, &m_bLanded);

	My_Float	fRad = m_fSpeed;
	if (m_fSpeed >= 2.0f) {
		fRad = m_fSpeed - 2.0f;
		fRad *= -1.0f;
	}
	fRad = -fRad * 0.2f * fTimeDelta;

	m_pComponent_Transform->Rotation_X(fRad);

	return S_OK;
}


void CBazzi::Recording(const My_Float& fTimeDelta)
{
	mRecordingTime += fTimeDelta;
	const My_Vec3*	pos = m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);

	if (mRecordingTime > 0.5f)
	{
		CRecordingManager::GetInstance()->SetRecordData(RECORD_DATA(*pos, 0x01));
		mRecordingTime = 0.f;
	}
}

void CBazzi::MovingKeyCheck()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		m_pComponent_Vehicle->Accelerate();
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		if (!(mKeyState & 0x01) && !(mKeyState & 0x02) && (m_bIsBooster == false))
		{
			mKeyState |= 0x04;
			m_bIsBack = true;

			if (2.0f > m_fAccAngle)
			{
				m_fAccAngle += D3DXToRadian(10.f);

				// 머리 돌기
				CModel::Rotation_Y(L"Bazzi_Head", D3DXToRadian(10.f));
			}

			m_pComponent_Vehicle->Accelerate_Reverse();
			CSoundMgr::GetInstance()->PlaySound(L"Motor.ogg", CSoundMgr::Channel_Motor);
		}
	}

	if ((mKeyState & 0x04) && !(GetAsyncKeyState(VK_DOWN) & 0x8000)) // Up
	{
		if (!(mKeyState & 0x01) && !(mKeyState & 0x02) && (m_bIsBooster == false))
		{
			m_bIsKeyUp3 = true;
		}
	}

	if (true == m_bIsKeyUp3)
	{
		m_fAccAngle -= D3DXToRadian(10.f);

		if (m_fAccAngle <= 0.f)
		{
			m_fAccAngle = 0.f;
			m_bIsKeyUp3 = false;
			m_bIsBack = false;
			mKeyState ^= 0x04;
		}
		else
		{
			CModel::Rotation_Y(L"Bazzi_Head", -D3DXToRadian(10.f));
		}
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		if (!(mKeyState & 0x04) && !(mKeyState & 0x02) && (m_bIsBooster == false))
		{
			mKeyState |= 0x01;

			if (0.25f > m_fAccAngle)
			{
				m_fAccAngle += D3DXToRadian(5.f);

				// 머리 돌기
				CModel::Rotation_Z(L"Bazzi_Head", D3DXToRadian(5.f));

				// 왼쪽 귀 돌기
				/*	My_Vec3		vPos1, vPos2;
				CModel::Get_Model_Pos(L"Bazzi_Ear_Left", &vPos1);
				CModel::Get_Model_Pos(L"Bazzi_Head", &vPos2);

				D3DXVec3TransformCoord(&vPos1, &vPos1, m_pComponent_Transform->Get_WorldMatrix());
				D3DXVec3TransformCoord(&vPos2, &vPos2, m_pComponent_Transform->Get_WorldMatrix());

				My_Vec3		vDir = vPos1 - vPos2;
				My_Float		fDis = D3DXVec3Length(&vDir);
				D3DXVec3Normalize(&vDir, &vDir);

				My_Matrix	matRot;
				D3DXMatrixRotationZ(&matRot, D3DXToRadian(1.f) * fTimeDelta);
				D3DXVec3TransformNormal(&vDir, &vDir, &matRot);

				My_Vec3		vPos3 = vPos2 + vDir * fDis;

				My_Matrix matWorld = *m_pComponent_Transform->Get_WorldMatrix();
				D3DXMatrixInverse(&matWorld, nullptr, &matWorld);

				D3DXVec3TransformCoord(&vPos3, &vPos3, &matWorld);

				CModel::Set_Model_Pos(L"Bazzi_Ear_Left", &vPos3);*/
			}

		}

		if (false == m_bIsDevil)
		{
			m_pComponent_Vehicle->Turn_Left();
		}
		else
			m_pComponent_Vehicle->Turn_Right();
	}

	if ((mKeyState & 0x01) && !(GetAsyncKeyState(VK_LEFT) & 0x8000) && (m_bIsBooster == false)) // Up
	{
		if (!(mKeyState & 0x04))
		{
			m_bIsKeyUp = true;
		}
	}

	if (true == m_bIsKeyUp)
	{
		m_fAccAngle -= D3DXToRadian(5.f);

		if (m_fAccAngle <= 0.f)
		{
			m_fAccAngle = 0.f;
			m_bIsKeyUp = false;
			mKeyState ^= 0x01;
		}
		else
		{
			CModel::Rotation_Z(L"Bazzi_Head", -D3DXToRadian(5.f));
		}
	}
	//else
	//{
	//	m_fAccAngle = 0.f;
	//	m_bIsKeyUp = false;
	//}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if (!(mKeyState & 0x04) && !(mKeyState & 0x01) && (m_bIsBooster == false))
		{
			mKeyState |= 0x02;

			if (-0.25f < m_fAccAngle)
			{
				m_fAccAngle -= D3DXToRadian(5.f);

				CModel::Rotation_Z(L"Bazzi_Head", -D3DXToRadian(5.f));
			}
		}

		if (false == m_bIsDevil)
		{
			m_pComponent_Vehicle->Turn_Right();
		}
		else
			m_pComponent_Vehicle->Turn_Left();
	}

	if ((mKeyState & 0x02) && !(GetAsyncKeyState(VK_RIGHT) & 0x8000) && (m_bIsBooster == false)) // Up
	{
		if (!(mKeyState & 0x04))
		{
			m_bIsKeyUp2 = true;
		}
	}

	if (true == m_bIsKeyUp2)
	{
		m_fAccAngle += D3DXToRadian(5.f);

		if (m_fAccAngle >= 0.f)
		{
			m_fAccAngle = 0.f;
			m_bIsKeyUp2 = false;
			mKeyState ^= 0x02;
		}

		else
		{
			CModel::Rotation_Z(L"Bazzi_Head", D3DXToRadian(5.f));
		}
	}

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
		m_pComponent_Vehicle->Drift();

	}

	if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) {
		m_pComponent_Vehicle->Apply_Velocity(My_Vec3(0.0f, 1.0f, 0.0f));
	}

	if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000) {
		m_pComponent_Vehicle->Braking();
	}
}

HRESULT CBazzi::ItemKeyCheck()
{
	if (GetAsyncKeyState(VK_CONTROL) & 0x0001)
	{
		if (0 < m_iItem.size())			// 아이템 아이콘 넘버를 담은 벡터의 크기가 0보다 크면
		{
			switch (m_iItem[0])		// 아이템 아이콘 넘버 벡터의 첫번째 값으로 switch문 돌리기
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
				ItemDevil();
			}
			break;
			case 6:		// UFO
			{
				ItemUFO();
			}
			break;
			case 7:
			{
				ItemMagnet();
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

			if (0 < m_IconVec.size())		// 아이템 아이콘 오브젝트를 담은 벡터의 사이즈가 0보다 크면
			{
				dynamic_cast<CItem_Icon*>(m_IconVec[0])->Set_IsUsed(true);		// 첫번째 오브젝트에 아이템이 사용됐다는 상태 전달

				if (2 == m_IconVec.size())		// 아이템 아이콘 오브젝트를 담은 벡터의 사이즈가 2일 때,
					m_IconVec[0] = m_IconVec[1];		// 두번째 벡터의 값을 첫번째 벡터에 값에 담고

				m_IconVec.pop_back();		// 벡터를 pop_back 해준다.
			}

			if (0 < m_iItem.size())		// 아이템 아이콘 텍스쳐 넘버를 담은 벡터의 사이즈가 0보다 크면
			{
				if (2 == m_iItem.size())	// 아이템 아이콘 텍스쳐 넘버를 담은 벡터의 사이즈가 2라면
					m_iItem[0] = m_iItem[1];		// 두번째 값을 첫번째 값으로 옮기고

				m_iItem.pop_back();		// 벡터를 pop_back 해준다.
			}
		}
	}

	return S_OK;
}

HRESULT CBazzi::ItemCheatKeyCheck()
{
	// 스페이스바 누르면 바나나 생성
	if (GetAsyncKeyState(VK_SPACE) & 0x0001)
		ItemBanana();

	// I 버튼 누르면 구름 생성
	if (GetAsyncKeyState('I') & 0x0001)
		ItemCloud();

	// O 버튼 누르면 물폭탄 생성
	if (GetAsyncKeyState('O') & 0x0001)
		ItemWaterBomb();

	// P 버튼 누르면 1등 양보 생성
	if (GetAsyncKeyState('P') & 0x0001)
		ItemFirst();

	// K 버튼 누르면 부스터
	if (GetAsyncKeyState('K') & 0x0001)
		ItemBooster();

	// L 버튼 누르면 악마
	if (GetAsyncKeyState('L') & 0x0001)
		ItemDevil();

	// J 누르면 자석
	if (GetAsyncKeyState('J') & 0x0001)
		ItemMagnet();

	// B누르면 미사일
	if (GetAsyncKeyState('B') & 0x0001)
		ItemMissile();

	// N누르면 UFO
	if (GetAsyncKeyState('N') & 0x0001)
		ItemUFO();

	// M누르면 물파리
	if (GetAsyncKeyState('M') & 0x0001)
		ItemWaterFly();

	return S_OK;
}

HRESULT CBazzi::Effect()
{
	/* Effect */
	// Engine Effect
	const CGameObject* pBooster = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Booster_Effect", 0);

	if (nullptr == pBooster)
	{

		// Smoke_Effect
		CGameObject*  pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Smoke_Effect");
		if (pObject == nullptr)
			return E_FAIL;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Smoke_Effect", pObject)))
			return E_FAIL;
		dynamic_cast<CSmoke_Effect*>(pObject)->Set_Name(L"CartBox_Engine_Left");

		pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Smoke_Effect");
		if (pObject == nullptr)
			return E_FAIL;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Smoke_Effect", pObject)))
			return E_FAIL;
		dynamic_cast<CSmoke_Effect*>(pObject)->Set_Name(L"CartBox_Engine_Right");
	}
	else
	{
		m_bIsBooster = true;

		if (true == m_bIsBooster && false == m_bIsBack)
		{
			m_fAccAngle += D3DXToRadian(-1.f);

			if (m_fAccAngle > D3DXToRadian(-30.f))
				CModel::Rotation_X2(L"Bazzi_Head", D3DXToRadian(-1.f));
			else
			{
				CGameObject* m_pCamrea = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_Target", 0));

				if (3 == dynamic_cast<CCamera_Target*>(m_pCamrea)->Get_Booster())
				{
					CModel::Rotation_X2(L"Bazzi_Head", D3DXToRadian(30.f));
					m_fAccAngle = 0.f;
					m_bIsBooster = false;
				}
			}
		}
	}

}

void CBazzi::ItemBanana()
{
	if (mIsBoss)
	{
		// Item_Banana
		CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Banana");
		if (pObject == nullptr)
			return;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Banana", pObject)))
			return;

		// 보스용
		dynamic_cast<CItem_Banana*>(pObject)->Set_IsBoss(true);
		// Player의 Transform 전달
		dynamic_cast<CItem_Banana*>(pObject)->Set_Player_Transform(m_pComponent_Transform);
	}

	CSoundMgr::GetInstance()->PlaySound(L"ItemShoot.ogg", CSoundMgr::Channel_Item);

	// Item_Banana
	CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Banana");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Banana", pObject)))
		return;

	// Player의 Transform 전달
	dynamic_cast<CItem_Banana*>(pObject)->Set_Player_Transform(m_pComponent_Transform);
}

void CBazzi::ItemCloud()
{
	CSoundMgr::GetInstance()->PlaySound(L"ItemShoot.ogg", CSoundMgr::Channel_Item);

	// Item_Cloud
	CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Cloud");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Cloud", pObject)))
		return;

	// Player의 WorldMatrix 전달
	dynamic_cast<CItem_Cloud*>(pObject)->Set_Player_Transform(m_pComponent_Transform->Get_WorldMatrix());
}

void CBazzi::ItemWaterBomb()
{
	CSoundMgr::GetInstance()->PlaySound(L"WaterShoot.ogg", CSoundMgr::Channel_Item);

	// Item_Water_Bomb
	CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Water_Bomb");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Water_Bomb", pObject)))
		return;

	dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Number(1);
	dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Player_Matrix(m_pComponent_Transform->Get_WorldMatrix());
}

void CBazzi::ItemFirst()
{
	CSoundMgr::GetInstance()->PlaySound(L"ItemShoot.ogg", CSoundMgr::Channel_Item);

	const My_Vec3* vCenterPos = nullptr;

	if (CRankingManager::BAZZI != CRankingManager::GetInstance()->GetFrist())
	{
		const CGameObject* object = nullptr;

		if (CRankingManager::DAO == CRankingManager::GetInstance()->GetFrist())
			object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0);

		else if (CRankingManager::DIZNI == CRankingManager::GetInstance()->GetFrist())
			object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0);

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

void CBazzi::ItemBooster()
{
	CSoundMgr::GetInstance()->PlaySound(L"Booster.ogg", CSoundMgr::Channel_Item);

	CGameObject* pObject = nullptr;

	// Booster_Effect
	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Booster_Effect");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Booster_Effect", pObject)))
		return;

	// Item_Booster
	pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Booster");
	if (pObject == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Booster", pObject)))
		return;

	// 배찌의 스피드를 조절하기 위해 컴포넌트 전달
	dynamic_cast<CItem_Booster*>(pObject)->SetIsPlayer(true);
	dynamic_cast<CItem_Booster*>(pObject)->Set_Player_Vehicle(m_pComponent_Vehicle);
}

void CBazzi::ItemDevil()
{
	CSoundMgr::GetInstance()->PlaySound(L"DevilShoot.ogg", CSoundMgr::Channel_Item);

	const CGameObject* object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0);
	CTransform* daoTransform = const_cast<CTransform*>(static_cast<const CTransform*>(object->Get_ComponentPointer(L"Component_Transform")));

	object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0);
	CTransform* dizniTransform = const_cast<CTransform*>(static_cast<const CTransform*>(object->Get_ComponentPointer(L"Component_Transform")));

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

void CBazzi::ItemMagnet()
{
	CSoundMgr::GetInstance()->PlaySound(L"MagnetTarget.ogg", CSoundMgr::Channel_Item);

	m_bIsMagnet = true;

	CGameObject*  	pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0));
	dynamic_cast<CDao*>(pGameObject)->SetIsTargeted(true);

	pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0));
	dynamic_cast<CDizni*>(pGameObject)->SetIsTargeted(true);

	m_pMagnet = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Magnet");
	if (m_pMagnet == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Magnet", m_pMagnet)))
		return;

	dynamic_cast<CItem_Magnet*>(m_pMagnet)->Set_Transform(m_pComponent_Transform);
	dynamic_cast<CItem_Magnet*>(m_pMagnet)->Set_IsMagnet(true);
}

void CBazzi::ItemMissile()
{
	CSoundMgr::GetInstance()->PlaySound(L"MagnetTarget.ogg", CSoundMgr::Channel_Item);

	if (mIsBoss)
	{
		m_bIsMissile = true;

		// 보스용
		CGameObject* pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Crocodile", 0));
		dynamic_cast<CCrocodile*>(pGameObject)->Set_IsMissileTarget(true);

		/*	CGameObject* pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0));
		dynamic_cast<CDao*>(pGameObject)->Set_IsMagnet(true);

		pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0));
		static_cast<CDizni*>(pGameObject)->SetIsTargeted(true);*/

		m_pMagnet = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Magnet");
		if (m_pMagnet == nullptr)
			return;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Magnet", m_pMagnet)))
			return;

		dynamic_cast<CItem_Magnet*>(m_pMagnet)->Set_Transform(m_pComponent_Transform);
		dynamic_cast<CItem_Magnet*>(m_pMagnet)->Set_IsMissile(true);
		// 보스용
		dynamic_cast<CItem_Magnet*>(m_pMagnet)->Set_IsBoss(true);

		return;
	}

	m_bIsMissile = true;

	CGameObject* pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0));
	dynamic_cast<CDao*>(pGameObject)->SetIsTargeted(true);

	pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0));
	static_cast<CDizni*>(pGameObject)->SetIsTargeted(true);

	m_pMagnet = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Magnet");
	if (m_pMagnet == nullptr)
		return;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Magnet", m_pMagnet)))
		return;

	dynamic_cast<CItem_Magnet*>(m_pMagnet)->Set_Transform(m_pComponent_Transform);
	dynamic_cast<CItem_Magnet*>(m_pMagnet)->Set_IsMissile(true);
}

void CBazzi::ItemUFO()
{
	CSoundMgr::GetInstance()->PlaySound(L"UFO0.ogg", CSoundMgr::Channel_Item);

	if (mIsBoss)
	{
		CGameObject*  pGameObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_UFO");
		if (pGameObject == nullptr)
			return;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_UFO", pGameObject)))
			return;

		// 보스용
		dynamic_cast<CItem_UFO*>(pGameObject)->Set_IsBoss(true);
		dynamic_cast<CItem_UFO*>(pGameObject)->Set_Transform(m_pComponent_Transform);

		return;
	}

	CRankingManager::Racer firstRacer = CRankingManager::GetInstance()->GetFrist();

	if (CRankingManager::BAZZI != firstRacer)
	{
		CGameObject*  pGameObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_UFO");
		if (pGameObject == nullptr)
			return;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_UFO", pGameObject)))
			return;

		dynamic_cast<CItem_UFO*>(pGameObject)->Set_Transform(m_pComponent_Transform);

		const CGameObject* object = nullptr;
		if (CRankingManager::DAO == firstRacer)
		{
			object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0);
			static_cast<CItem_UFO*>(pGameObject)->SetTargetNum(1);
		}

		else if (CRankingManager::DIZNI == firstRacer)
		{
			object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0);
			static_cast<CItem_UFO*>(pGameObject)->SetTargetNum(2);
		}
		static_cast<CItem_UFO*>(pGameObject)->SetTarget(const_cast<CGameObject*>(object));
	}
}

void CBazzi::ItemWaterFly()
{
	CSoundMgr::GetInstance()->PlaySound(L"WaterFly.ogg", CSoundMgr::Channel_Item);

	CRankingManager::Racer frontRacer = CRankingManager::GetInstance()->GetFrontRacer(CRankingManager::BAZZI);

	if (CRankingManager::BAZZI != frontRacer && CRankingManager::RACER_END != frontRacer)
	{
		CGameObject*  pGameObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Water_Fly");
		if (pGameObject == nullptr)
			return;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Water_Fly", pGameObject)))
			return;

		static_cast<CItem_Water_Fly*>(pGameObject)->Set_Transform(m_pComponent_Transform);
		const CGameObject* object = nullptr;

		if (CRankingManager::DAO == frontRacer)
		{
			object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0);
			static_cast<CItem_Water_Fly*>(pGameObject)->SetTarget(1);
		}

		else if (CRankingManager::DIZNI == frontRacer)
		{
			object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0);
			static_cast<CItem_Water_Fly*>(pGameObject)->SetTarget(2);
		}

		static_cast<CItem_Water_Fly*>(pGameObject)->SetTargetTrans(const_cast<CTransform*>(static_cast<const CTransform*>(object->Get_ComponentPointer(L"Component_Transform"))));
	}
}


HRESULT CBazzi::Update_Animation(const My_Float & fTimeDelta)
{
	//Head Animation
	My_Matrix	matBazzi_Head;
	if (FAILED(CModel::Get_LocalMat(L"Bazzi_Head", &matBazzi_Head)))
		return E_FAIL;
	if (FAILED(CModel::Set_ParentMat(L"Bazzi_Ear_Right", matBazzi_Head)))
		return E_FAIL;
	if (FAILED(CModel::Set_ParentMat(L"Bazzi_Ear_Left", matBazzi_Head)))
		return E_FAIL;

	//Tyre Animation
	/*CModel::Rotation_Z2(L"CartBox_Tire_Right_1", D3DXToRadian(10.f));
	CModel::Rotation_Z2(L"CartBox_Tire_Right_2", D3DXToRadian(10.f));
	CModel::Rotation_Z2(L"CartBox_Tire_Left_1", D3DXToRadian(-10.f));
	CModel::Rotation_Z2(L"CartBox_Tire_Left_2", D3DXToRadian(-10.f));*/
	{
		My_Vec3		vVel = m_pComponent_Vehicle->Get_Velocity();
		My_Vec3		vLook = *m_pComponent_Transform->Get_Information(CTransform::INFO_LOOK);
		D3DXVec3Normalize(&vLook, &vLook);

		My_Float	fProjLength = D3DXVec3Dot(&vVel, &vLook);
		My_Vec3		vLookVel = vLook * fProjLength;

		CModel::Rotation_Z2(L"CartBox_Tire_Right_1", fProjLength * fTimeDelta);
		CModel::Rotation_Z2(L"CartBox_Tire_Right_2", fProjLength * fTimeDelta);
		CModel::Rotation_Z2(L"CartBox_Tire_Left_1", -fProjLength * fTimeDelta);
		CModel::Rotation_Z2(L"CartBox_Tire_Left_2", -fProjLength * fTimeDelta);
	}

	//SkidMark

	if (NONE == mItemState)
	{
		My_Vec3		vWheelOffset = My_Vec3(0.0f, 0.0f, 0.6f);
		My_Vec3		vPosOffset = My_Vec3(-0.3f, 0.0f, 0.3f);

		My_Bool		bIsDrifting = m_pComponent_Vehicle->Get_IsDrifting();
		My_Vec3		vCurPos = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
		My_Vec3		vCurWheel[4];

		My_Vec3		vLook = *m_pComponent_Transform->Get_Information(CTransform::INFO_LOOK);
		My_Vec3		vLookNor = *D3DXVec3Normalize(&vLookNor, &vLook);
		My_Vec3		vAxisZ = My_Vec3(0.0f, 0.0f, 1.0f);
		My_Float	fCosf = D3DXVec3Dot(&vLookNor, &vAxisZ);
		My_Float	fRad = acosf(fCosf);

		if (vLookNor.x > vAxisZ.x)
			fRad *= -1.0f;

		My_Vec3		vUp = *m_pComponent_Transform->Get_Information(CTransform::INFO_UP);
		My_Matrix	matRot;
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationAxis(&matRot, &vUp, -(fRad + D3DXToRadian(45.0f)));
		D3DXVec3TransformNormal(&vPosOffset, &vPosOffset, &matRot);
		D3DXVec3TransformNormal(&vCurWheel[0], &vWheelOffset, &matRot);
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(90.0f));
		D3DXVec3TransformNormal(&vCurWheel[1], &vCurWheel[0], &matRot);
		D3DXVec3TransformNormal(&vCurWheel[2], &vCurWheel[1], &matRot);
		D3DXVec3TransformNormal(&vCurWheel[3], &vCurWheel[2], &matRot);

		for (int i = 2; i < 4; ++i)
			vCurWheel[i] += vCurPos/* + vPosOffset*/;

		if (m_bLanded == true) {
			My_Float fDriftMult = m_pComponent_Vehicle->Get_DriftMult();
			My_Vec3		vVel = m_pComponent_Vehicle->Get_Velocity();
			D3DXVec3Normalize(&vLook, &vLook);
			My_Float	fProjLength = D3DXVec3Dot(&vVel, &vLook);
			My_Vec3		vProj = vLook * fProjLength;
			My_Vec3		vVertVel = vVel - vProj;
			My_Float	fVertSpd = fabs(D3DXVec3Length(&vVertVel));

			if (/*bIsDrifting == true || */fDriftMult >= 0.5f || fVertSpd >= 15.0f) {
				CGameObjectMgr* pGameObjectMgr = CGameObjectMgr::GetInstance();
				CSkidMark*	pSkidMark = (CSkidMark*)pGameObjectMgr->Get_GameObject(SCENE_STAGE, L"Object_SkidMark", 0);
				if (pSkidMark == nullptr) {
					MSG_BOX("No SkidMark Object");
					return E_FAIL;
				}
				for (int i = 0; i < 4; ++i)
					pSkidMark->Add_SkidMark(m_vPrevPos[i], vCurWheel[i]);
				CSoundMgr::GetInstance()->PlaySound(L"Drift.ogg", CSoundMgr::Channel_Item);
			}
		}
		for (int i = 0; i < 4; ++i)
			m_vPrevPos[i] = vCurWheel[i];
	}

	return S_OK;
}

HRESULT CBazzi::Collision(const My_Float& fTimeDelta)
{
	if (2 == m_iState)		// 아이템 박스와 충돌했을 때
	{
		CSoundMgr::GetInstance()->PlaySound(L"ItemEat.ogg", CSoundMgr::Channel_Item);

		m_iState = 0;
		// 아이템 아이콘을 띄우기 위해 Object_UI 레이어의 사이즈를 받아온다.
		const int iIconSize = CGameObjectMgr::GetInstance()->Get_LayerSize(SCENE_STAGE, L"Object_UI");

		const CGameObject*      pGameObject = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Icon", 1);

		if (nullptr == pGameObject)
		{
			// Item_Icon         // 새로운 아이템 아이콘 오브젝트를 Clone 하자
			CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Icon");
			if (pObject == nullptr)
				return E_FAIL;
			if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Icon", pObject)))
				return E_FAIL;

			My_Int randItem;

			if (CRankingManager::BAZZI != CRankingManager::GetInstance()->GetFrist())
			{
				randItem = rand() % 10;

				if (0 == randItem)
					randItem = 2;
			}

			else
			{
				randItem = rand() % 4;

				if (2 == randItem)
					randItem = 5;

				else if (3 == randItem)
					randItem = 1;
			}

			dynamic_cast<CItem_Icon*>(pObject)->Set_Number(randItem);

			m_IconVec.push_back(pObject);		// 현재 아이템 아이콘 오브젝트를 벡터에 담기
			m_iItem.push_back(randItem);			// 현재 아이템 아이콘 넘버를 벡터에 담기
		}
	}

	if ((3 == m_iState && mItemState == NONE) || mItemState == BANANA)		// 바나나와 충돌했을 때
	{
		CSoundMgr::GetInstance()->PlaySound(L"Banana.ogg", CSoundMgr::Channel_Item);

		// 카메라 얻어오기
		m_bIsRotate = true; 
		mItemState = BANANA;

		CGameObject* pCamrea = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_Target", 0));
		static_cast<CCamera_Target*>(pCamrea)->SetRotate(m_bIsRotate);		// 플레이어가 회전중이라는 값을 카메라에 전달

		CGameObject* pMiniMapCamrea = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_MiniMap", 0));
		static_cast<CCamera_MiniMap*>(pMiniMapCamrea)->SetRotate(m_bIsRotate);
		// 점점 속도를 줄이기 위한 변수
		m_fAccSpeed += 0.05f;
		m_pComponent_Transform->Rotation_Y(((10.6462f - (m_fAccSpeed)) * fTimeDelta));

		// 속도가 다 줄었으면 원래 값으로 초기화
		if (10.7f < m_fAccSpeed)
		{
			m_bIsRotate = false;
			m_fAccSpeed = 0.f;
			m_iState = 0;

			dynamic_cast<CCamera_Target*>(pCamrea)->SetRotate(m_bIsRotate);
			static_cast<CCamera_MiniMap*>(pMiniMapCamrea)->SetRotate(m_bIsRotate);

			My_Vec3 matCameraLook = static_cast<CCamera_Target*>(pCamrea)->GetLookVec();
			const My_Matrix* matWorld = m_pComponent_Transform->Get_WorldMatrix();
			My_Vec3 myLook = *(My_Vec3*)&matWorld->m[2][0];

			My_Matrix	matRot;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixRotationAxis(&matRot, (My_Vec3*)&matWorld->m[0][0], D3DXToRadian(-32.0f));

			matCameraLook.y = 0.f;
			myLook.y = 0.f;

			D3DXVec3Normalize(&matCameraLook, &matCameraLook);
			D3DXVec3Normalize(&myLook, &myLook);

			My_Float angle = acosf(D3DXVec3Dot(&matCameraLook, &myLook));

			m_pComponent_Transform->Rotation_Y(angle);

			mItemState = NONE;
		}
	}

	if ((4 == m_iState && mItemState == NONE) || (13 == m_iState && mItemState == NONE) || mItemState == WATERBOMB)		// 물폭탄과 충돌했을 때
	{
		if (true == m_bIsFirst)
		{
			mItemState = WATERBOMB;
			m_fFirstTime = fTimeDelta;
			m_bIsFirst = false;

			CSoundMgr::GetInstance()->PlaySound(L"WaterHit.ogg", CSoundMgr::Channel_Item);

			m_vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);

			// Item_Water_Bomb
			CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Water_Bomb");
			if (pObject == nullptr)
				return E_FAIL;
			if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Water_Bomb", pObject)))
				return E_FAIL;

			dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Number(2);
			dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Player_Transform(m_pComponent_Transform);
		}

		m_fAccTime += fTimeDelta;

		if (2.f > (m_fAccTime - m_fFirstTime))
		{
			m_bIsWater = true;
		}
		else if (2.f < (m_fAccTime - m_fFirstTime) && 4.f > (m_fAccTime - m_fFirstTime))
		{
			m_fAccSpeed += 0.001f;
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

			m_pComponent_Vehicle->Apply_Velocity(My_Vec3(0.0f, -10.0f, 0.0f));

			m_iState = 0;
			mItemState = NONE;
		}
	}

	// 미사일과 충돌했을 때
	if (((6 == m_iState && mItemState == NONE) || mItemState == MISSILE) && mIsMissileTarget)
	{
		if (true == m_bIsFirst)
		{
			mItemState = MISSILE;
			m_fFirstTime = fTimeDelta;
			m_bIsFirst = false;
			m_matWorld = *m_pComponent_Transform->Get_WorldMatrix();
			m_vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);

			CSoundMgr::GetInstance()->PlaySound(L"Bomb.ogg", CSoundMgr::Channel_Item);

			m_bIsRotate = true;

			CGameObject* pCamrea = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_Target", 0));
			static_cast<CCamera_Target*>(pCamrea)->SetRotate(m_bIsRotate);		// 플레이어가 회전중이라는 값을 카메라에 전달

			CGameObject* pMiniMapCamrea = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_MiniMap", 0));
			static_cast<CCamera_MiniMap*>(pMiniMapCamrea)->SetRotate(m_bIsRotate);
		}

		m_fAccTime += fTimeDelta;


		if (2.f > (m_fAccTime - m_fFirstTime))
		{
			m_bIsHit = true;
			m_fY += 0.1f;
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(m_vPosition.x, m_vPosition.y + m_fY, m_vPosition.z));
			m_pComponent_Transform->Rotation_X(20.f * fTimeDelta);

		}
		else if (2.f < (m_fAccTime - m_fFirstTime) && 3.9f > (m_fAccTime - m_fFirstTime))
		{
			m_fY2 += 0.1f;
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(m_vPosition.x, m_vPosition.y + m_fY - m_fY2, m_vPosition.z));
			m_pComponent_Transform->Rotation_X(20.f * fTimeDelta);
		}
		else
		{
			mItemState = NONE;
			m_fAccTime = 0.f;
			m_fY = 0.f;
			m_fY2 = 0.f;
			m_bIsFirst = true;
			m_bIsHit = false;
			m_pComponent_Transform->Set_Information(&m_matWorld);

			m_iState = 0;
			mIsMissileTarget = false;
			m_bIsRotate = false;

			CGameObject* pCamrea = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_Target", 0));
			static_cast<CCamera_Target*>(pCamrea)->SetRotate(m_bIsRotate);		// 플레이어가 회전중이라는 값을 카메라에 전달

			CGameObject* pMiniMapCamrea = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_MiniMap", 0));
			static_cast<CCamera_MiniMap*>(pMiniMapCamrea)->SetRotate(m_bIsRotate);
		}
	}

	if (!m_bIsColBooster && 7 == m_iState) // 부스터 박스와 충돌
	{
		CSoundMgr::GetInstance()->PlaySound(L"BoosterStart.ogg", CSoundMgr::Channel_Item);

		CGameObject* pObject = nullptr;

		// Booster_Effect
		pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Booster_Effect");
		if (pObject == nullptr)
			return E_FAIL;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Booster_Effect", pObject)))
			return E_FAIL;

		// Item_Booster
		pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Booster");
		if (pObject == nullptr)
			return E_FAIL;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Booster", pObject)))
			return E_FAIL;

		// 배찌의 스피드를 조절하기 위해 컴포넌트 전달
		dynamic_cast<CItem_Booster*>(pObject)->SetIsPlayer(true);
		dynamic_cast<CItem_Booster*>(pObject)->Set_Player_Vehicle(m_pComponent_Vehicle);

		m_iState = 0;
		m_bIsColBooster = true;
	}

	if (m_bIsColBooster)
	{
		m_fBoosterColTime += fTimeDelta;

		if (m_fBoosterColTime > 1.5f)
		{
			m_bIsColBooster = false;
			m_fBoosterColTime = 0.f;
		}
	}

	if (((9 == m_iState && mItemState == NONE) || mItemState == WATERFLY) && mIsWaterFlyTarget)		// 물파리와 충돌했을 때
	{
		if (true == m_bIsFirst)
		{
			CSoundMgr::GetInstance()->PlaySound(L"WaterFlyWater.ogg", CSoundMgr::Channel_Item);

			mItemState = WATERFLY;
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
			dynamic_cast<CItem_Water_Bomb*>(pObject)->Set_Player_Transform(m_pComponent_Transform);
		}

		m_fAccTime += fTimeDelta;

		if (2.f > (m_fAccTime - m_fFirstTime))
		{
			m_bIsWater = true;
		}
		else if (2.f < (m_fAccTime - m_fFirstTime) && 4.f > (m_fAccTime - m_fFirstTime))
		{
			m_fAccSpeed += 0.001f;
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
			mIsWaterFlyTarget = false;

			m_pComponent_Vehicle->Apply_Velocity(My_Vec3(0.0f, -10.0f, 0.0f));

			m_iState = 0;
			mItemState = NONE;
		}
	}

	if (m_iState == 11) // 1등양보
	{
		CSoundMgr::GetInstance()->PlaySound(L"Collide.ogg", CSoundMgr::Channel_Item);

		m_pComponent_Vehicle->Set_Velocity_X(0.0f);
		m_pComponent_Vehicle->Set_Velocity_Y(0.0f);
		m_pComponent_Vehicle->Set_Velocity_Z(0.0f);
		m_iState = 0;
	}

	// 자석 아이템의 화살표가 빨간색이면 배찌가 다오에게 가기
	if (m_pMagnet != nullptr && m_bIsMagnet == true)
	{
		if (dynamic_cast<CItem_Magnet*>(m_pMagnet)->Get_Num() == 2)
		{
			CSoundMgr::GetInstance()->PlaySound(L"Magnet.ogg", CSoundMgr::Channel_Item);

			My_Int	iTargetNum = dynamic_cast<CItem_Magnet*>(m_pMagnet)->Get_TargetNum();
			CGameObject* pTarget = nullptr;
			My_Vec3 vTargetPos;

			switch (iTargetNum)
			{
			case 1:
			{
				pTarget = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0));
				CTransform* pTransform = dynamic_cast<CDao*>(pTarget)->Get_Component();
				vTargetPos = *pTransform->Get_Information(CTransform::INFO_POSITION);

			}
			break;
			case 2:
			{
				pTarget = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0));
				CTransform* pTransform = dynamic_cast<CDizni*>(pTarget)->Get_Component();
				vTargetPos = *pTransform->Get_Information(CTransform::INFO_POSITION);
			}
			break;
			}
			/*	CGameObject* pDao = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0));*/
			My_Vec3 vDir = vTargetPos - *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
			My_Float fDis = D3DXVec3Length(&vDir);
			D3DXVec3Normalize(&vDir, &vDir);

			My_Vec3 vPosition = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
			My_Vec3 vPosition2 = { 0.f, 0.f, 0.f };

			if (fDis < 5.f)
			{
				m_bIsMagnet = false;
				dynamic_cast<CItem_Magnet*>(m_pMagnet)->Set_Destroy(true);
			}
			else
			{
				vPosition2 = vPosition + vDir * 50.f * fTimeDelta;
				m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &vPosition2);
			}
		}
	}

	if (m_iState == 14) {
		m_pComponent_Vehicle->Apply_Velocity(My_Vec3(-5.0f, 10.0f, 0.0f));
		m_iState = 0;
	}

	if (m_iState == 15) {
		m_fFlatTime = 0.0f;
		m_fFlatTime += fTimeDelta;
		m_iState = 0;
	}

}

CBazzi * CBazzi::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBazzi*		pInstance = new CBazzi(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CBazzi::Clone()
{
	CBazzi*		p = new CBazzi(*this);

	if (FAILED(p->Initialize())) {
		delete p;
		p = nullptr;
	}

	return p;
}

My_uLong CBazzi::ReleaseMemory()
{
	Safe_Release(m_pComponent_Vehicle);
	Safe_Release(m_pCollisioner);
	Safe_Release(m_pComponent_NavMesh);
	
	return CModel::ReleaseMemory();
	return My_uLong();
}
