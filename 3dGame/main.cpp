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

void everyFrame()
{
	g_pD3DGame->prepareScene();
	//hier alles zeichnen--

	//---------------------
	g_pD3DGame->presentScene();
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
		"vrrace_main",
		NULL
	};

	RegisterClassEx(&wc);

	HWND	hWnd	= CreateWindow(
						"vrrace_main",
						"vrrace",
						WS_OVERLAPPEDWINDOW,
						100,
						100,
						300,
						300,
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

	if (g_pD3DGame->init3D(&hInst, &hWnd))
	{
		g_pD3DGame->initGame();
	}

	while (msg.message != WM_QUIT) 
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			everyFrame();
		}
	}

	delete g_pD3DGame;
	
	UnregisterClass("vrrace_main", wc.hInstance);
	return 0;
}