#include "..\Header\GraphicDev.h"



USING(Engine)
IMPLEMENT_SINGLETON(CGraphicDev)



Engine::CGraphicDev::CGraphicDev()
{
	m_pSDK = nullptr;
	m_pDevice = nullptr;
}

HRESULT Engine::CGraphicDev::Initialize_Graphic_Device(WIN_MODE eMode, HWND hWnd, const Engine::My_uInt & wSizeX, const Engine::My_uInt & wSizeY)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);		//SDK 생성

	D3DCAPS9		deviceCaps;
	memset(&deviceCaps, 0, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &deviceCaps))) {
		MSG_BOX("GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD		vp;		//VertexProcessing

	if (deviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)		//Hardware Trasform and light
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;					//장치에서 이동과 빛연산이 가능한지 확인함
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	vp |= D3DCREATE_MULTITHREADED;		//멀티스레드 기능을 설정하는 구문.

	D3DPRESENT_PARAMETERS		d3dpp;
	memset(&d3dpp, 0, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp, eMode, hWnd, wSizeX, wSizeY);

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_pDevice))) {
		MSG_BOX("CreateDevice Failed");
		return E_FAIL;
	}

	m_wSizeX = wSizeX;
	m_wSizeY = wSizeY;

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
		return E_FAIL;

	if (FAILED(D3DXCreateFont(m_pDevice, 12, 8, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("굴림"), &m_pFont)))
		return E_FAIL;

	return S_OK;
}

HRESULT Engine::CGraphicDev::SetParameters(D3DPRESENT_PARAMETERS & d3dpp, WIN_MODE eMode, HWND hWnd, const Engine::My_uInt & wSizeX, const Engine::My_uInt & wSizeY)
{
	d3dpp.BackBufferWidth = wSizeX;				// 백버퍼의 가로길이
	d3dpp.BackBufferHeight = wSizeY;				// 백버퍼의 세로길이
	d3dpp.BackBufferCount = 1;				// 백버퍼의 갯수
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;				// 백버퍼 픽셀 하나당 크기

	d3dpp.MultiSampleQuality = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;				// 백버퍼와 전면버퍼의 교체방식(그리는 방식)설정
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;				// 모니터에 그리는 주기

	return S_OK;
}

Engine::My_uLong Engine::CGraphicDev::ReleaseMemory(void)
{
	Engine::My_uLong		dwRefCnt = 0;

	dwRefCnt = Engine::Safe_Release(m_pSprite);
	if (dwRefCnt != 0) {
		MSG_BOX("m_pSprite Release Failed");
		return dwRefCnt;
	}

	dwRefCnt = Engine::Safe_Release(m_pFont);
	if (dwRefCnt != 0) {
		MSG_BOX("m_pFont Release Failed");
		return dwRefCnt;
	}

	dwRefCnt = Engine::Safe_Release(m_pDevice);
	if (dwRefCnt != 0) {
		MSG_BOX("Device Release Failed");
		return dwRefCnt;
	}

	dwRefCnt = Safe_Release(m_pSDK);
	if (dwRefCnt != 0) {
		MSG_BOX("SDK Release Failed");
		return dwRefCnt;
	}


	return dwRefCnt;
}
