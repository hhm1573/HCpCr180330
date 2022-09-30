#pragma once

#include "Include.h"
#include "Model.h"

namespace Engine 
{
	class CVehicle;
	class CCollisioner;
	class CNavMesh;
}

class CDizni final
	:public CModel
{
	enum ItemState { BANANA, MISSILE, WATERBOMB, WATERFLY, BOOSTER, NONE };

private:
	explicit CDizni(LPDIRECT3DDEVICE9 pDevice);
	explicit CDizni(const CDizni& Ref);
	virtual ~CDizni() = default;

public:
	CTransform*	Get_Component() { return m_pComponent_Transform; }

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	const My_Matrix*	GetWorld() const { return m_pComponent_Transform->Get_WorldMatrix(); }

public:
	void SetIsTargeted(My_Bool flag) { mIsTargeted = flag; }
	void SetSpeed(My_Float fX, My_Float fY, My_Float fZ) { mComponentVehicle->Set_Inform(fX, fY, fZ); }
	void SetMissileTarget() { mIsMissileTarget = true; }
	void SetWaterFlyTarget() { mIsWaterFlyTarget = true; }
	void SetStartPos();
	void SetIsStart() { mIsStart = true; }
	void SetIsGoal() { mIsGoal = true; }
	void SetStop(My_Bool flag)
	{
		mIsStop = flag;
	}

private:
	CVehicle*			mComponentVehicle;
	CNavMesh*			mComponentNavMesh;
	CCollisioner*		mCollisioner;

private:
	list<RECORD_DATA*>	mRecordList;
	RECORD_DATA*		mCurrentData;

private:
	My_Int				mState;
	My_Matrix			mPreWorld;
	My_Bool				m_bLanded = false;

	
	// Booster
	/*My_Int				mIsColBooster;
	My_Float			mBoosterColTime;*/

	// Banana
	My_Float			mAccSpeed;

	// WaterBomb
	My_Float			mAccTime;
	My_Bool				mIsFirst;
	My_Float			mFirstTime;
	My_Float			mSpeed;
	My_Bool				mIsWater;

	// Magnet && Missile
	My_Bool				mIsTargeted;
	My_Bool				mIsHit;
	My_Float			mMissileY1;
	My_Float			mMissileY2;
	My_Bool				mIsMissileTarget;
	My_Bool				mIsWaterFlyTarget;

	My_Int				mReceive;

	// ItemSlot
	vector<My_Int>		mItemVec;
	My_Float			mItemUseTime;

	ItemState			mItemState = NONE;
	My_Bool				mIsStart = false;

	My_Bool				mIsGoal = false;
	My_Float			mGoalTime = 0.f;
	My_Bool						mIsStop = false;

private:
	My_Vec3						m_vCol;
	My_Float					m_fFlatTime = 0.0f;
	My_Vec3						m_vPrevPos[4];

private:
	void Load(int recordNum);
	void Collision(const My_Float& fTimeDelta);

private:
	void UseItem(const My_Float& fTimeDelta);

private:
	void				ItemBanana();
	void				ItemCloud();
	void				ItemWaterBomb();
	void				ItemFirst();
	void				ItemBooster();
	void				ItemDevil();
	void				ItemMissile();
	void				ItemUFO();
	void				ItemWaterFly();
	HRESULT				Update_Collision_Tetris(const My_Float& fTimeDelta);

public:
	static CDizni*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*	Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

