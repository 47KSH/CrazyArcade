#include "windefine.h"
#include "MainGame.h"

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, INT)
{
	LPCTSTR szClassName = _T("MyMainWindowClass");

	WNDCLASSEX wcex = {0};

    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_VREDRAW | CS_HREDRAW;
    wcex.lpfnWndProc = &WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = szClassName;
	wcex.hIconSm = wcex.hIcon;

	if (!::RegisterClassEx(&wcex))
	{
		return 1;
	}

	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	int width = 800;
	int height = 600;

	//int x = (cx - width)/2;
	//int y = (cy - height)/2;
	//해상도가 좌측상단(0, 0) 이 아니면 짤려서 나옴.. !!

	//화면 해상도를 설정을 하기위한 구조체..
	DEVMODE dev;
	dev.dmSize = sizeof(DEVMODE);
	dev.dmPelsWidth = width;
	dev.dmPelsHeight = height;
	dev.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

	//해상도를 바꾸기 위해서 이 함수를 써야하는데 그래서 위 구조체를 정의해줬다. (크아가 이런식으로 하는거군..)
	//::ChangeDisplaySettings(&dev, CDS_FULLSCREEN);

	DWORD dwStyle = WS_POPUP; // 미니 전체화면..
	//dwStyle = dwStyle & ~WS_THICKFRAME;
	//dwStyle = dwStyle & ~WS_MINIMIZEBOX;
	//dwStyle = dwStyle & ~WS_MAXIMIZEBOX;

	HWND hWnd = ::CreateWindowEx(0, szClassName, _T("Win32 Mouse"), dwStyle,
		0, 0, width, height,
		NULL, NULL, hInst, NULL);

	if (hWnd == NULL)
	{
		return 2;
	}



	::ShowWindow(hWnd, SW_NORMAL);
	::UpdateWindow(hWnd);


	RECT rc;
	::GetClientRect(hWnd, &rc);
	POINT lt = {rc.left, rc.top};
	POINT rb = {rc.right, rc.bottom};

	::ClientToScreen(hWnd, &lt);
	::ClientToScreen(hWnd, &rb);

	RECT rcScreen = {lt.x, lt.y, rb.x, rb.y};

	::ClipCursor(&rcScreen);


	MSG msg;

	MainGame mg;
	mg.Attach(hWnd);
	mg.Load();

	DWORD dt = 0;
	DWORD st = ::GetTickCount();

	while (true)
	{
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;

		mg.Input(dt);
		mg.Update(dt, hWnd);
		mg.Draw();

		dt = ::GetTickCount() - st;
		st = ::GetTickCount();
	}
	return msg.wParam;
}