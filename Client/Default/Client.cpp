// Client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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

// ���� ����:
HINSTANCE g_hInst;                                // ���� �ν��Ͻ��Դϴ�.
HWND		g_hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.
	CMainApp*		pMainApp = nullptr;

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
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

    // �⺻ �޽��� �����Դϴ�.
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


			if (true == pFrameMgr->Permit_Call(L"Frame_60", fTimeDelta_Default)) // ���ʿ� ���� ������ Ƚ����ŭ�� Ʈ�縦 ����.
			{
				My_Float fTimeDelta = pTimerMgr->Compute_Time(L"Timer_FPS60");

				if (pMainApp->Update(fTimeDelta) & 0x80000000)
					break;

				pMainApp->Render();
			}

			//if (true == pFrame_Manager->Permit_Call(L"Frame_100", fTimeDelta_Default)) // ���ʿ� ���� ������ Ƚ����ŭ�� Ʈ�縦 ����.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
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