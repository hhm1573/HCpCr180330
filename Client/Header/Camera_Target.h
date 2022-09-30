#pragma once

#include "Camera.h"

namespace Engine {
	class CTransform;
}

class CCamera_Target final
	:public CCamera
{
public:
	enum CameraTargetState { NORMAL, GOAL, AWARD };

private:
	explicit CCamera_Target(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera_Target(const CCamera_Target& Ref);
	virtual ~CCamera_Target() = default;

private:
	CTransform*			m_pTarget = nullptr;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype(const CAMERA_DESC& tCameraDesc, const PROJ_DESC& tProjDesc);
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta)override;
	virtual void		Render() override;

public:
	My_Vec3		GetLookVec() const { return *(My_Vec3*)&m_matWorld.m[2][0]; }
	const int&			Get_Booster() {
		return m_iBooster;
	}

public:
	HRESULT				SetTarget(CTransform* pTarget);
	void					SetRotate(bool bIsRotate) {
		m_bIsRotate = bIsRotate;
	}
	void					SetBooster(int iBooster) {
		m_iBooster = iBooster;
	}

	void				SetState(CameraTargetState state) { mState = state; }

private:
	My_Vec3				m_vPrePos;
	My_Bool				m_bIsRotate;
	My_Int				m_iBooster;
	My_Float			m_fDistance;
	My_Float			m_fAccTime;
	My_Float			m_fFirstTime;
	My_Bool				m_bIsFirst;
	My_Float			m_fSpeed;
	My_Vec3				m_vMyPos;
	CameraTargetState	mState;
	My_Bool				mIsFirst = true;
	My_Float			mAngle = 0.f;

public:
	static CCamera_Target*		Create(LPDIRECT3DDEVICE9 pDevice, const CAMERA_DESC& tCameraDesc, const PROJ_DESC& tProjDesc);
	virtual CGameObject*		Clone() override;
protected:
	virtual My_uLong			ReleaseMemory() override;
};

