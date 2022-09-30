#include "..\Header\InputDev.h"

IMPLEMENT_SINGLETON(CInputDev)

CInputDev::CInputDev()
{
}

HRESULT CInputDev::Ready_Input_Device(HINSTANCE hInstance, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pSDK, nullptr)))
		return E_FAIL;

	if (FAILED(Ready_Device_KeyBoard(hWnd)))
		return E_FAIL;

	if (FAILED(Ready_Device_Mouse(hWnd)))
		return E_FAIL;


	return NOERROR;
}

void CInputDev::SetUp_Input_Device_State()
{
	if (nullptr == m_pKeyBoard)
		return;
	if (nullptr == m_pMouse)
		return;

	My_Byte*	pTemp = m_PreKeyState;
	m_PreKeyState = m_CurKeyState;
	m_CurKeyState = pTemp;

	m_pKeyBoard->GetDeviceState(256, m_CurKeyState);
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
}

HRESULT CInputDev::Ready_Device_KeyBoard(HWND hWnd)
{
	if (nullptr == m_pSDK)
		return E_FAIL;

	if (FAILED(m_pSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->Acquire()))
		return E_FAIL;

	memset(m_KeyState, 0, sizeof(My_Byte) * 256 * 2);
	m_CurKeyState = m_KeyState[0];
	m_PreKeyState = m_KeyState[1];

	return NOERROR;
}

HRESULT CInputDev::Ready_Device_Mouse(HWND hWnd)
{
	if (nullptr == m_pSDK)
		return E_FAIL;

	if (FAILED(m_pSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	return NOERROR;
}

My_uLong CInputDev::ReleaseMemory(void)
{
	return My_uLong();
}
