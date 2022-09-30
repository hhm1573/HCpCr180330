#pragma once

#include "Model.h"

namespace Engine {
	class CVehicle;
	class CNavMesh;
	class CCollisioner;
}

class CCrocodile final
	:public CModel
{
	enum ItemState { MISSILE, WATERBOMB, NONE };

private:
	explicit CCrocodile(LPDIRECT3DDEVICE9 pDevice);
	explicit CCrocodile(const CCrocodile& Ref);
	virtual ~CCrocodile() = default;

public:
	void	Set_IsBoss(My_Bool bIsBoss) 
	{
		m_bIsBoss = true;
		m_vPosition = { 120.f, 6.5f, 390.f };
		m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &m_vPosition);

		m_vLook = { 0.f, 0.f, -1.f };
		My_Vec3		vRight = { -1.f, 0.f, 0.f };
		My_Vec3		vUp = { 0.f, 1.f, 0.f };

		D3DXVec3Normalize(&m_vLook, &m_vLook);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		m_pComponent_Transform->Set_Information(CTransform::INFO_LOOK, &m_vLook);
		m_pComponent_Transform->Set_Information(CTransform::INFO_RIGHT, &vRight);
		m_pComponent_Transform->Set_Information(CTransform::INFO_UP, &vUp);

		m_pComponent_Transform->Scaling(1.5f, 1.5f, 1.5f);
	}

	void			Set_Speed(My_Float fX, My_Float fY, My_Float fZ) {
		m_pComponent_Vehicle->Set_Inform(fX, fY, fZ);
	}
	void			Set_IsMissileTarget(My_Bool bIsTarget) {
		m_bIsMissileTarget = bIsTarget;
	}
	void			SetIsAward(My_Bool flag) { mIsAwardBoss = flag;  m_bIsBoss = false;
		//m_vPosition = { 422.f, 0.f, 285.f };
		m_vPosition = { 401.237976f, 6.55142593f, 115.639153f };
		//m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(422.f, 0.f, 285.f));
		m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &m_vPosition);

		m_vLook = { -1.f, 0.f, 0.f };
		My_Vec3		vRight = { 0.f, 0.f, 1.f };
		My_Vec3		vUp = { 0.f, 1.f, 0.f };

		D3DXVec3Normalize(&m_vLook, &m_vLook);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		m_pComponent_Transform->Set_Information(CTransform::INFO_LOOK, &m_vLook);
		m_pComponent_Transform->Set_Information(CTransform::INFO_RIGHT, &vRight);
		m_pComponent_Transform->Set_Information(CTransform::INFO_UP, &vUp);

		m_pComponent_Transform->Scaling(1.5f, 1.5f, 1.5f);}

public:
	CTransform*	Get_Component() {
		return m_pComponent_Transform;
	}
	My_Bool		GetIsDead() 
	{
		My_Vec3 vec = { 130.f, 15.f, 116.f };
		
		if (D3DXVec3Length(&(vec - *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION))) < 30.f)
			return true;

		else
			return false;
	}
	My_Bool		GetDead() { return mIsDead; }

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

private:
	CVehicle*				m_pComponent_Vehicle = nullptr;
	CNavMesh*				m_pComponent_NavMesh = nullptr;
	CCollisioner*			m_pCollisioner = nullptr;

private:
	My_Bool		m_bIsBoss = false;
	My_Int		m_iState = 0;
	My_Bool		m_bIsMissileTarget = false;
	My_Float		m_fFirstTime = 0.f;
	My_Float		m_fAccTime = 0.f;
	My_Bool		m_bIsWater = false;
	My_Vec3		m_vPosition = { 0.f, 0.f, 0.f };
	My_Float		m_fSpeed = 0.f;
	My_Float		m_fAccSpeed = 0.f;
	My_Bool		m_bIsFirst = true;
	My_Float		m_fFirstTime2 = 0.f;
	My_Float		m_fAccTime2 = 0.f;
	My_Bool		m_bIsFirst2 = true;
	My_Bool		m_bIsMissile = false;
	My_Int		m_iHP = 100;
	My_Int		m_iMaxHP = 100;
	My_Int		m_iHPNum = 9;
	My_Float		m_fFirstTime3 = 0.f;
	My_Float		m_fAccTime3 = 0.f;
	My_Bool		m_bIsFirst3 = true;
	My_Float		m_fFirstTime4 = 0.f;
	My_Float		m_fAccTime4 = 0.f;
	My_Bool		m_bIsFirst4 = true;
	My_Vec3		m_vLook = { 0.f, 0.f, 0.f };
	My_Bool				m_bLanded = false;
	My_Bool		mIsAwardBoss = false;
	ItemState	mItemState = NONE;
	My_Int		mHP = 10;
	My_Bool		mIsDead = false;

public:
	static CCrocodile*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

