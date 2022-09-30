// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"

#include "MainApp.h"

#include "TimerMgr.h"
#include "FrameMgr.h"

#include "SceneMgr.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"
#include "Debugger.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
HWND		g_hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
My_uLong Release_Singleton();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
	CMainApp*		pMainApp = nullptr;

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	pMainApp = CMainApp::Create();
	if (pMainApp == nullptr)
		return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	CTimerMgr*		pTimerMgr = CTimerMgr::GetInstance();
	pTimerMgr->AddRef();

	if (FAILED(pTimerMgr->Add_Timer(L"Timer_Default")))
		return FALSE;
	if (FAILED(pTimerMgr->Add_Timer(L"Timer_FPS60")))
		return FALSE;

	CFrameMgr*		pFrameMgr = CFrameMgr::GetInstance();
	pFrameMgr->AddRef();

	if (FAILED(pFrameMgr->Add_Frame(L"Frame_60", 60.0f)))
		return FALSE;
	if (FAILED(pFrameMgr->Add_Frame(L"Frame_100", 100.0f)))
		return FALSE;

    MSG msg;

    // 기본 메시지 루프입니다.
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			My_Float fTimeDelta_Default = pTimerMgr->Compute_Time(L"Timer_Default");

			if (nullptr == pMainApp)
				break;


			if (true == pFrameMgr->Permit_Call(L"Frame_60", fTimeDelta_Default)) // 일초에 내가 지정한 횟수만큼만 트루를 리턴.
			{
				My_Float fTimeDelta = pTimerMgr->Compute_Time(L"Timer_FPS60");

				if (pMainApp->Update(fTimeDelta) & 0x80000000)
					break;

				pMainApp->Render();
			}

			//if (true == pFrame_Manager->Permit_Call(L"Frame_100", fTimeDelta_Default)) // 일초에 내가 지정한 횟수만큼만 트루를 리턴.
			//{
			//	
			//}
		}
	}

	Safe_Release(pTimerMgr);
	Safe_Release(pFrameMgr);

	My_uLong dwRefCnt = 0;

	if (dwRefCnt = Safe_Release(pMainApp))
		MSG_BOX("CMainApp Release Failed");

	if (dwRefCnt = Release_Singleton())
		MSG_BOX("Release_Singleton Failed");

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT rc = { 0, 0, g_iBackCX, g_iBackCY };

   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      200, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

My_uLong Release_Singleton() {

	My_uLong		dwRefCnt = 0;
	if (dwRefCnt = CSceneMgr::GetInstance()->DestroyInstance())
	{
		MSG_BOX("CManagement Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CGameObjectMgr::GetInstance()->DestroyInstance())
	{
		MSG_BOX("CGameObjectMgr Release Failed");
		return dwRefCnt;
	}


	if (dwRefCnt = CDebugger::GetInstance()->DestroyInstance()) {
		MSG_BOX("Debugger Release Failed");
		return dwRefCnt;
	}


	if (dwRefCnt = CComponentMgr::GetInstance()->DestroyInstance())
	{
		MSG_BOX("CComponent_Manager Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CGraphicDev::GetInstance()->DestroyInstance())
	{
		MSG_BOX("CGraphic_Device Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CFrameMgr::GetInstance()->DestroyInstance())
	{
		MSG_BOX("CFrame_Manager Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CTimerMgr::GetInstance()->DestroyInstance())
	{
		MSG_BOX("CTimer_Manager Release Failed");
		return dwRefCnt;
	}

	return My_uLong();
}