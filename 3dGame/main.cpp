#include "main.h"
#include "resource.h"

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			return 0;
		break;
	case WM_CLOSE:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			return 0;
		break;
	}
	return DefWindowProc(hWnd, msg,	wParam,	lParam);
}

bool GenerateGameWindow()
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

	if (g_pD3DGame->init(&g_hInst, &hWnd))
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
	return true;
}

/*Dialogfunktion*/
INT_PTR CALLBACK DialogProc(HWND hWnd,
							unsigned int uiMsg,
							WPARAM WParam,
							LPARAM LParam)
{
	/*HDC			hDC;
	LOGBRUSH	LogBrush;
	HBRUSH		hBrush;
	RECT		Rect;
	CHOOSECOLOR	ChooseColorStruct;
	char		acText[256];
	*/


	// Nachricht verarbeiten
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		
		// Timer installieren
		//SetTimer(hWnd, 0, 25, NULL);
		break;

	case WM_CLOSE:
		// Code 0 zurückliefern; das bedeutet: Dialog abgebrochen
		EndDialog(hWnd, 0);
		break;

	case WM_DESTROY:
		// Timer löschen
		//KillTimer(hWnd, 0);
		break;

	case WM_TIMER:
		break;

	case WM_COMMAND:
		//Befehlsverarbeitung
		switch(LOWORD(WParam))
		{
		case IDC_OK:
			//falls OK-Button gedrueckt
				//MessageBox(NULL,"OK-Button pressed","Message",MB_OK | MB_ICONINFORMATION);
				EndDialog(hWnd, 0);

				//Spielfenster erzeugen
				GenerateGameWindow();
			break;

		case IDC_CANCEL:
			//falls Cancel-Button gedrueckt
				MessageBox(NULL,"CANCEL-Button pressed","Message",MB_OK | MB_ICONINFORMATION);
			break;
		case IDC_QUIT:
			//falls Quit-Button gedrueckt
				MessageBox(NULL,"QUIT-Button pressed","Message",MB_OK | MB_ICONINFORMATION);
				EndDialog(hWnd,0);
			break;
		}
		break;

	case WM_NOTIFY:
		break;

	default:
		// Standardverarbeitung der Nachricht
		return FALSE;
		break;
	}

	// Nachricht wurde verarbeitet.
	return TRUE;
}

INT WINAPI WinMain(HINSTANCE hInst,
				   HINSTANCE,
				   LPSTR,
				   INT)
{
	g_hInst = hInst;

	InitCommonControls();

	if(DialogBox(hInst,MAKEINTRESOURCE(IDD_VRRACE),NULL,DialogProc))
	{
		//Fehler aufgetreten
		MessageBox(NULL,"Fehler bei der Initialisierung der Applikation","ERROR",MB_OK | MB_ICONSTOP);
		return 1;
	}

	return 0;
}