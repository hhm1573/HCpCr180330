#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev final
	:public CBase
{
	DECLARE_SINGLETON(CInputDev)

public:
	enum MOUSESTATE { DIM_LBUTTON, DIM_RBUTTON, DIM_MBUTTON, DIM_XBUTTON };
	enum MOUSEMOVESTATE { DIMM_X, DIMM_Y, DIMM_WHEEL };

private:
	explicit CInputDev();
	virtual ~CInputDev() = default;

private:
	LPDIRECTINPUT8				m_pSDK;
private:
	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	My_Byte						m_KeyState[2][256];
	My_Byte*					m_CurKeyState = nullptr;
	My_Byte*					m_PreKeyState = nullptr;
private:
	LPDIRECTINPUTDEVICE8		m_pMouse;
	DIMOUSESTATE				m_MouseState;

public:
	HRESULT Ready_Input_Device(HINSTANCE hInstance, HWND hWnd);
	void SetUp_Input_Device_State();
private:
	HRESULT Ready_Device_KeyBoard(HWND hWnd);
	HRESULT Ready_Device_Mouse(HWND hWnd);

public:
	My_Byte Get_DIKState(My_Byte byKeyID) const {
		return m_CurKeyState[byKeyID];
	}
	My_Byte Get_DIMState(MOUSESTATE eKeyID) const {
		return m_MouseState.rgbButtons[eKeyID];
	}
	My_Long Get_DIMMoveState(MOUSEMOVESTATE eMouseMove) const {
		return *((My_uLong*)&m_MouseState + eMouseMove);
	}
public:
	My_Bool	Get_DIK_IsPressed(My_Byte byKeyID) const {
		return (m_CurKeyState[byKeyID] & 0x80 && !(m_PreKeyState[byKeyID] & 0x80)) ? true : false;
	}

public:
	virtual My_uLong ReleaseMemory(void);
};

END