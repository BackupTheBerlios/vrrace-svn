#include "main.h"

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		break;
	}
	return DefWindowProc(hWnd, msg,	wParam,	lParam);
}

INT WINAPI WinMain(HINSTANCE hInst,
				   HINSTANCE,
				   LPSTR,
				   INT)
{
	WNDCLASSEX	wc	= {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		MsgProc,
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		MM_WINDOW_CLASS_NAME,
		NULL
	};

	RegisterClassEx(&wc);

	HWND	hWnd	= CreateWindow(
						MM_WINDOW_CLASS_NAME,
						MM_WINDOW_DESCRIPTION,
						WS_OVERLAPPEDWINDOW,
						1,
						1,
						800,
						800,
						GetDesktopWindow(),
						NULL,
						wc.hInstance,
						NULL
					);

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	MSG	msg;
	ZeroMemory(&msg, sizeof(msg));

	g_pD3DGame	= new MyD3DGame();

	if (g_pD3DGame->init(&hInst, &hWnd))
	{
	}

	while (msg.message != WM_QUIT) 
	{
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
		//else {
			g_pD3DGame->runGame();
		//}
	}

	delete g_pD3DGame;
	
	UnregisterClass(MM_WINDOW_CLASS_NAME, wc.hInstance);
	return 0;
}