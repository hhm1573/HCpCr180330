#include "stdafx.h"
#include "..\Header\Tetris.h"

CTetris::CTetris(LPDIRECT3DDEVICE9 pDevice)
	: CModel(pDevice), 
	mIsMoving(false), mState(WATING), mStandingTime(5.f),
	mCount(0), mDownCount(0), mDownHeight(0.f), mSpeed(50.f), mStandingHeight(0.f), mNumber(0), mCollisioner(nullptr), mReceive(0)
{
}

CTetris::CTetris(const CTetris & Ref)
	: CModel(Ref), 
	mIsMoving(Ref.mIsMoving), mState(Ref.mState), mStandingTime(Ref.mStandingTime),
	mCount(Ref.mCount), mDownCount(Ref.mDownCount), mDownHeight(Ref.mDownHeight), mSpeed(Ref.mSpeed), mStandingHeight(Ref.mStandingHeight), mNumber(Ref.mNumber),
	mCollisioner(Ref.mCollisioner), mReceive(0)
{
}

HRESULT CTetris::Initialize()
{
	// Collision
	mCollisioner = (CCollisioner*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (mCollisioner == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Collisioner", mCollisioner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTetris::Initialize_Prototype()
{
	return S_OK;
}

My_Int CTetris::Update(const My_Float & fTimeDelta)
{
	if (WATING == mState)
	{
		m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &mRespawnPos);
		mState = READY;
	}

	else if (GO_DOWN == mState)
	{
		m_pComponent_Transform->Move_Free(&My_Vec3(0.f, -1.f, 0.f), mSpeed * fTimeDelta);
		
		const My_Vec3* pos = m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
		
		if (pos->y < mStandingHeight)
		{
			mState = STANDING;
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(pos->x, mStandingHeight, pos->z));
		}
	}

	else if (STANDING == mState)
	{
		mStandingTime += fTimeDelta;
	}

	else if (DISAPPEAR == mState)
	{
		m_pComponent_Transform->Move_Free(&My_Vec3(0.f, -1.f, 0.f), mSpeed * fTimeDelta);
		mDownHeight += mSpeed * fTimeDelta;

		if (mDownHeight > 9.f)
		{
			++mCount;

			const My_Vec3* pos = m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(pos->x, mStandingHeight - 9.f * mCount, pos->z));

			if (mCount == mDownCount)
			{
				mState = WATING;
				// mStandingTime = 0.f;
				mCount = 0;
			}

			else
				mState = STANDING;

			mDownHeight = 0.f;
		}
	}

	return My_Int();
}

My_Int CTetris::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CTetris::Render()
{
}

void CTetris::SetAngle(My_Float y, My_Float z)
{
	if (0.f != y)
		m_pComponent_Transform->Rotation_Y(D3DXToRadian(y));
	
	if (0.f != z)
		m_pComponent_Transform->Rotation_Z(D3DXToRadian(z));
}

void CTetris::SetScale()
{
	m_pComponent_Transform->Scaling(0.36f, 0.36f, 0.36f);
}

My_uLong CTetris::ReleaseMemory(void)
{
	Safe_Release(mCollisioner);

	CModel::ReleaseMemory();
	return My_uLong();
}
