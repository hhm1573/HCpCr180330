#pragma once

#include "Model.h"

namespace Engine {
	class CVehicle;
	class CNavMesh;
	class CCollisioner;
}

class CBazzi final
	:public CModel
{
	enum ItemState { BANANA, MISSILE, WATERBOMB, WATERFLY, NONE };

private:
	explicit CBazzi(LPDIRECT3DDEVICE9 pDevice);
	explicit CBazzi(const CBazzi& Ref);
	virtual ~CBazzi() = default;

public:
	void		Set_IsDevil(bool isDevil) {
		m_bIsDevil = isDevil;
	}

	void SetSpeed(My_Float fX, My_Float fY, My_Float fZ) { m_pComponent_Vehicle->Set_Inform(fX, fY, fZ); }
	void SetMissileTarget() { mIsMissileTarget = true; }
	void SetWaterFlyTarget() { mIsWaterFlyTarget = true; }
	void SetStartPos();
	void SetIsCounting(My_Bool flag) { mIsCounting = flag; }
	void SetIsGoal() { mIsGoal = true; }
	void SetStop(My_Bool flag)
	{
		mIsStop = flag;
	}
	void SetIsMagnet(My_Bool bIsMagnet) { m_bIsMagnet = bIsMagnet; }
	void SetIsBoss(My_Bool flag) { mIsBoss = flag; }
	void SetPos(My_Vec3* vec3) { m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, vec3); }

public:
	CTransform*		Get_Transform() {
		return m_pComponent_Transform;
	}
	My_Vec3			Get_ModelPos(My_TChar* name)
	{
		My_Vec3 pPos;
		CModel::Get_Model_Pos(name, &pPos);
		D3DXVec3TransformCoord(&pPos, &pPos, m_pComponent_Transform->Get_WorldMatrix());
		return pPos;
	}

private:
	CVehicle*				m_pComponent_Vehicle = nullptr;
	CNavMesh*				m_pComponent_NavMesh = nullptr;
	CCollisioner*			m_pCollisioner = nullptr;
	CGameObject*			m_pMagnet = nullptr;
	My_Bool					m_bLanded = false;


private:
	My_Int						m_iState = 0;
	int							m_iRand = 0;
	vector<CGameObject*>		m_IconVec;
	vector<int>					m_iItem;
	My_Float					m_fSpeed = 0.f;
	My_Float					m_fAccSpeed = 0.f;
	My_Float					m_fAccTime = 0.f;
	My_Float					m_fFirstTime = 0.f;
	My_Bool						m_bIsFirst = true;
	My_Bool						m_bIsWater = false;
	My_Vec3						m_vPosition;
	My_Bool						m_bIsDevil = false;
	My_Bool						m_bIsMagnet = false;
	My_Bool						m_bIsMissile = false;
	My_Bool						m_bIsRotate = false;
	My_Int						m_iReceive = 0;
	BYTE						mKeyState = 0x00; // 0000 0000
	My_Float					m_fAccAngle = 0.f;
	My_Float					m_fAccAngle2 = 0.f;
	My_Bool						m_bIsKeyUp = false;
	My_Bool						m_bIsFirst2 = true;
	My_Bool						m_bIsKeyUp2 = false;
	My_Bool						m_bIsKeyUp3 = false;
	My_Bool						m_bIsColBooster = false;
	My_Float					m_fBoosterColTime = 0.f;
	My_Bool						m_bIsBooster = false;
	My_Float					mRecordingTime = 0.f;
	My_Float					m_fY = 0.f;
	My_Float					m_fY2 = 0.f;
	My_Bool						m_bIsHit = false;
	My_Matrix					m_matWorld;
	My_Bool						mIsMissileTarget;
	My_Bool						mIsWaterFlyTarget;
	ItemState					mItemState = NONE;
	My_Bool						mIsCounting = false;
	My_Bool						mIsGoal = false;
	My_Float					mGoalTime = 0.f;
	My_Bool						m_bIsBack = false;
	My_Bool						mIsBoss = false;
	My_Bool						mIsStop = false;
	My_Float					mMissileTime = 0.f;
	My_Bool						m_bIsSmall = false;

private:
	My_Vec3						m_vCol;
	My_Float					m_fFlatTime = 0.0f;
	My_Vec3						m_vPrevPos[4];

public:
	HRESULT				Initialize(void);
	HRESULT				Initialize_Prototype(void);
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render(void);

public:
	const My_Matrix*	GetWorld() const { return m_pComponent_Transform->Get_WorldMatrix(); }

public:
	My_Float			GetSpeed() const;

private:
	void				Recording(const My_Float& fTimeDelta);
	void				MovingKeyCheck();
	HRESULT				ItemKeyCheck();
	HRESULT				ItemCheatKeyCheck();
	HRESULT				Effect();
	HRESULT				Update_Animation(const My_Float& fTimeDelta);
	HRESULT				Update_Collision_Tetris(const My_Float& fTimeDelta);
	HRESULT				Update_WaterTrap(const My_Float& fTimeDelta);

private:
	void				ItemBanana();
	void				ItemCloud();
	void				ItemWaterBomb();
	void				ItemFirst();
	void				ItemBooster();
	void				ItemDevil();
	void				ItemMagnet();
	void				ItemMissile();
	void				ItemUFO();
	void				ItemWaterFly();

private:
	HRESULT				Collision(const My_Float& fTimeDelta);

public:
	static CBazzi*				Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone() override ;
protected:
	virtual My_uLong			ReleaseMemory() override;
};

