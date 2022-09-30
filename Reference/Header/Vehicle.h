#pragma once

#include "Component.h"

namespace Engine {
	class ENGINE_DLL CVehicle final
		:public CComponent
	{
	private:
		enum DriveState { STATE_IDLE, STATE_ACCELERATE, STATE_BRAKING, STATE_REVERSE };
		enum WheelState { WH_STATE_IDLE, WH_STATE_RIGHT, WH_STATE_LEFT };
	private:
		explicit CVehicle(LPDIRECT3DDEVICE9 pDevice);
		explicit CVehicle(const CVehicle& Ref);
		virtual ~CVehicle() = default;

	private:
		DriveState			m_eState = STATE_IDLE;
		My_Vec3				m_vV;
		My_Vec3				m_vFt;
		My_Vec3				m_vFd;
		My_Vec3				m_vFrr;
		My_Float			m_fM = 0.0f;
		My_Float			m_fFhp = 0.0f;
	private:
		WheelState			m_eWhState = WheelState::WH_STATE_IDLE;
		My_Float			m_fVa = 0.0f;
		My_Float			m_fDistW = 0.0f;
		My_Float			m_fWheelrad = 0.0f;
		My_Float			m_fDriftMult = 0.0f;
	private:
		My_Bool				m_bDrift = false;

	public:
		HRESULT				Initialize();
		HRESULT				Set_Inform(const My_Float& fM, const My_Float& fFhp,const My_Float& fDistW);
	public:
		HRESULT				Accelerate();
		HRESULT				Braking();
		HRESULT				Accelerate_Reverse();
		HRESULT				Turn_Right();
		HRESULT				Turn_Left();
		HRESULT				Update(const My_Float& fTimeDelta, const My_Vec3* vLook, const My_Bool& bIsLanded);
	public:
		HRESULT				Drift();
	private:
		HRESULT				Update_Direction(const My_Float& fTimeDelta, const My_Vec3* vLook);
		HRESULT				Update_Angular(const My_Float& fTimeDelta, const My_Vec3 * vLook);
		HRESULT				Update_Angular_Adv(const My_Float& fTimeDelta, const My_Vec3 * vLook);

	public:
		My_Vec3				Get_Velocity() const {
			return m_vV;
		}
		My_Float			Get_AngularVelocity() const {
			return m_fVa;
		}
		My_Bool				Get_IsDrifting() const {
			return m_bDrift;
		}
		My_Float			Get_DriftMult() const {
			return m_fDriftMult;
		}

	public:
		void				Apply_Velocity(const My_Vec3 vVelocity) {
			m_vV += vVelocity;
		}
		void				Set_Velocity_X(const My_Float& fSpd) {
			m_vV.x = fSpd;
		}
		void				Set_Velocity_Y(const My_Float& fSpd) {
			m_vV.y = fSpd;
		}
		void				Set_Velocity_Z(const My_Float& fSpd) {
			m_vV.z = fSpd;
		}

	public:
		static CVehicle*				Create(LPDIRECT3DDEVICE9 pDevice);
		virtual CComponent*				Clone(void) override;
	protected:
		virtual My_uLong				ReleaseMemory() override;
	};
}