#include "main.h"
#include "resource.h"

#include "engine\MyDPlay.h"

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT Paint;

	switch(msg)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
		// Das Fenster mit Schwarz füllen
		BeginPaint(hWnd, &Paint);
		FillRect(Paint.hdc, &Paint.rcPaint, (HBRUSH)(GetStockObject(BLACK_BRUSH)));
		EndPaint(hWnd, &Paint);
		break;
	/*case WM_DESTROY:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			exit(0);
		break;*/
	case WM_CLOSE:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			//exit(0);
		break;
	}
	return DefWindowProc(hWnd, msg,	wParam,	lParam);
}

bool GenerateGameWindow(MyDPlay* givenDPlay, int choice, int shipChoice)
{
	int iWidth;
	int iHeight;

	WNDCLASSEX	wc	= {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		MsgProc,
		0,
		0,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		MM_WINDOW_CLASS_NAME,
		NULL
	};

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Fehler bei der Registrierung des Fensters", "ERROR", MB_OK | MB_ICONSTOP);
	}

	RECT Rect;
	SetRect(&Rect, 0, 0, 800, 800);
	AdjustWindowRect(&Rect, WS_VISIBLE | WS_OVERLAPPEDWINDOW, FALSE);
	iWidth = Rect.right - Rect.left;
	iHeight = Rect.bottom - Rect.top;

	HWND	hWnd	= CreateWindow(
						MM_WINDOW_CLASS_NAME,
						MM_WINDOW_DESCRIPTION,
						WS_VISIBLE | WS_OVERLAPPEDWINDOW,
						GetSystemMetrics(SM_CXSCREEN) / 2 - iWidth / 2,//1,
						GetSystemMetrics(SM_CYSCREEN) / 2 - iHeight / 2,//1,
						iWidth,//800,
						iHeight,//800,
						NULL,//GetDesktopWindow(),
						NULL,
						GetModuleHandle(NULL),//wc.hInstance,
						NULL
					);

	if(givenDPlay != NULL)
		givenDPlay->m_hWnd = &hWnd;

	//ShowWindow(hWnd, SW_NORMAL);
	//UpdateWindow(hWnd);

	MSG	msg;
	ZeroMemory(&msg, sizeof(msg));

	g_pD3DGame	= new MyD3DGame();
	
	if (g_pD3DGame->init(&g_hInst, &hWnd, givenDPlay, choice, shipChoice))
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

/*Methode zur Verarbeitung*/
void GameManagement(HWND hWnd, bool isCheck, int shipChoice)
{
	//MultiPlayer
	TCHAR*		tcPort		= new TCHAR[10];
	TCHAR*		tcUsername	= new TCHAR[48];
	DWORD		dwPort		= 0;
	MyDPlay*	mydplay		= new MyDPlay();
	if((IsDlgButtonChecked(hWnd, IDC_SERVER) == BST_CHECKED) || (IsDlgButtonChecked(hWnd, IDC_CLIENT) == BST_CHECKED))
	{
		GetDlgItemText(hWnd, IDC_PORT, tcPort, 10);
		dwPort = _ttoi(tcPort);
	}
	if(IsDlgButtonChecked(hWnd, IDC_SERVER) == BST_CHECKED)
	{
		if(mydplay->init(NULL,&dwPort,true))
		{
			if(mydplay->createSession())
			{
				if(isCheck)
				{
					MessageBox(NULL, "Hosten der Session erfolgreich", "Message", MB_OK | MB_ICONINFORMATION);
				}
				else {
					GetDlgItemText(hWnd, IDC_USERNAME, tcUsername, 48);
					mydplay->m_pUsername = tcUsername;
					//MessageBox(NULL, mydplay->m_pUsername, "Message", MB_OK);
					//Timer löschen
					KillTimer(hWnd, 0);
					EndDialog(hWnd, 0);
					//Spielfenster erzeugen
					GenerateGameWindow(mydplay, 1, shipChoice);
				}
			}
			else {
				MessageBox(NULL,"createSession","Message",MB_OK | MB_ICONSTOP);
			}
		}
		else {
			MessageBox(NULL,"Init","Message",MB_OK | MB_ICONSTOP);
		}
		//MessageBox(NULL,"close()","Message",MB_OK | MB_ICONINFORMATION);

		//EnterCriticalSection(&mydplay->m_csDP);
		//mydplay->closeConnection();
		//LeaveCriticalSection(&mydplay->m_csDP);
	}
	else if(IsDlgButtonChecked(hWnd, IDC_CLIENT) == BST_CHECKED)
	{
		TCHAR* tcIPAddr = new TCHAR[18];
		GetDlgItemText(hWnd, IDC_IPADDRESS, tcIPAddr, 18);
		//MessageBox(NULL,tcIPAddr,"Message",MB_OK | MB_ICONINFORMATION);
		if(mydplay->init(tcIPAddr,&dwPort,false))
		{
			if(mydplay->enumAvailServer())
			{
				if(mydplay->connectSession())
				{
					if(isCheck)
					{
						MessageBox(NULL, "Verbindungstest erfolgreich", "Message", MB_OK | MB_ICONINFORMATION);
					}
					else {
						GetDlgItemText(hWnd, IDC_USERNAME, tcUsername, 48);
						mydplay->m_pUsername = tcUsername;
						//MessageBox(NULL, mydplay->m_pUsername, "Message", MB_OK);
						//Timer löschen
						KillTimer(hWnd, 0);
						EndDialog(hWnd, 0);
						//Spielfenster erzeugen
						GenerateGameWindow(mydplay, 2, shipChoice);
					}
				}
				else {
					MessageBox(NULL,"ConnectSession","Message",MB_OK | MB_ICONSTOP);
				}
			}
			else {
				MessageBox(NULL,"EnumHosts","Message",MB_OK | MB_ICONSTOP);
			}
		}
		else {
			MessageBox(NULL,"Init","Message",MB_OK | MB_ICONSTOP);
		}
		//EnterCriticalSection(&mydplay->m_csDP);
		//mydplay->closeConnection();
		//LeaveCriticalSection(&mydplay->m_csDP);
		
		MyTools::deleteArray(tcIPAddr);
	}
	MyTools::deleteArray(tcPort);
	MyTools::deleteArray(tcUsername);
	MyTools::deleteObject(mydplay);	
}

/*Dialogfunktion*/
INT_PTR CALLBACK DialogProc(HWND hWnd,
							unsigned int uiMsg,
							WPARAM WParam,
							LPARAM LParam)
{
	static HBITMAP	hBitmapBigship;
	static BITMAP	bitmapBigship;
	static HBITMAP	hBitmapShusui;
	static BITMAP	bitmapShusui;
	static HBITMAP	hBitmapSpaceship2;
	static BITMAP	bitmapSpaceship2;
	static HBITMAP	hBitmapSpaceship5;
	static BITMAP	bitmapSpaceship5;
	static HBITMAP	hBitmapSpaceship13;
	static BITMAP	bitmapSpaceship13;
	static HBITMAP	hBitmapStarsail;
	static BITMAP	bitmapStarsail;
	
	static BOOL		mode			= TRUE;
	static BOOL		server			= FALSE;
	static BOOL		client			= FALSE;
	static BOOL		ip				= FALSE;
	static BOOL		port			= FALSE;
	static BOOL		check			= FALSE;

	HDC				hdc;
	HDC				hdcMem;
	//PAINTSTRUCT		ps;
	
	// Nachricht verarbeiten
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			hBitmapBigship			= (HBITMAP) LoadImage(g_hInst,"resources/ships/bigship1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmapBigship,sizeof(BITMAP),(LPVOID) &bitmapBigship);
			hBitmapShusui			= (HBITMAP) LoadImage(g_hInst,"resources/ships/shusui.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmapShusui,sizeof(BITMAP),(LPVOID) &bitmapShusui);
			hBitmapSpaceship2		= (HBITMAP) LoadImage(g_hInst,"resources/ships/spaceship 2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmapSpaceship2,sizeof(BITMAP),(LPVOID) &bitmapSpaceship2);
			hBitmapSpaceship5		= (HBITMAP) LoadImage(g_hInst,"resources/ships/spaceship 5.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmapSpaceship5,sizeof(BITMAP),(LPVOID) &bitmapSpaceship5);
			hBitmapSpaceship13		= (HBITMAP) LoadImage(g_hInst,"resources/ships/spaceship 13.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmapSpaceship13,sizeof(BITMAP),(LPVOID) &bitmapSpaceship13);
			hBitmapStarsail			= (HBITMAP) LoadImage(g_hInst,"resources/ships/star sail.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmapStarsail,sizeof(BITMAP),(LPVOID) &bitmapStarsail);
			if((hBitmapBigship == NULL) || (hBitmapShusui == NULL) || (hBitmapSpaceship2 == NULL) ||
				(hBitmapSpaceship5 == NULL) || (hBitmapSpaceship13 == NULL) || (hBitmapStarsail == NULL))
			{
				MessageBox(NULL, "Fehler beim Laden des Bildes", "ERROR", MB_OK | MB_ICONSTOP);
			}
			CheckDlgButton(hWnd, IDC_PLAYERMODE, mode);
			EnableWindow(GetDlgItem(hWnd, IDC_SERVER), server);
			EnableWindow(GetDlgItem(hWnd, IDC_CLIENT), client);
			EnableWindow(GetDlgItem(hWnd, IDC_IPADDRESS), ip);
			EnableWindow(GetDlgItem(hWnd, IDC_PORT), port);
			EnableWindow(GetDlgItem(hWnd, IDC_CHECKCON), check);

			// Timer installieren
			SetTimer(hWnd, 0, 25, NULL);
						
			break;
		}

	case WM_TIMER:
		{
			HBITMAP hOldBitmap;
			RECT rect;
			if(hBitmapBigship != NULL)
			{
				//hdc = BeginPaint(GetDlgItem(hWnd,IDC_BMPBIGSHIP), &ps);
				hdc = GetDC(GetDlgItem(hWnd,IDC_BMPBIGSHIP1));
				hdcMem = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmapBigship);
				GetClientRect(GetDlgItem(hWnd,IDC_BMPBIGSHIP1),&rect);
				StretchBlt(hdc, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top), hdcMem, 0, 0, bitmapBigship.bmWidth, bitmapBigship.bmHeight, SRCCOPY);
				//EndPaint(hWnd, &ps);
				SelectObject(hdcMem, hOldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hOldBitmap);
				ReleaseDC(GetDlgItem(hWnd,IDC_BMPBIGSHIP1), hdc);
			}
			if(hBitmapShusui != NULL)
			{
				//hdc = BeginPaint(GetDlgItem(hWnd,IDC_BMPSHUSUI), &ps);
				hdc = GetDC(GetDlgItem(hWnd,IDC_BMPSHUSUI));
				hdcMem = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmapShusui);
				GetClientRect(GetDlgItem(hWnd,IDC_BMPSHUSUI),&rect);
				StretchBlt(hdc, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top), hdcMem, 0, 0, bitmapShusui.bmWidth, bitmapShusui.bmHeight, SRCCOPY);
				//EndPaint(hWnd, &ps);
				SelectObject(hdcMem, hOldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hOldBitmap);
				ReleaseDC(GetDlgItem(hWnd,IDC_BMPSHUSUI), hdc);
			}
			if(hBitmapSpaceship2 != NULL)
			{	
				//hdc = BeginPaint(GetDlgItem(hWnd,IDC_BMPSPACESHIP2), &ps);
				hdc = GetDC(GetDlgItem(hWnd,IDC_BMPSPACESHIP2));
				hdcMem = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmapSpaceship2);
				GetClientRect(GetDlgItem(hWnd,IDC_BMPSPACESHIP2),&rect);
				StretchBlt(hdc, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top), hdcMem, 0, 0, bitmapSpaceship2.bmWidth, bitmapSpaceship2.bmHeight, SRCCOPY);
				//EndPaint(hWnd, &ps);
				SelectObject(hdcMem, hOldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hOldBitmap);
				ReleaseDC(GetDlgItem(hWnd,IDC_BMPSPACESHIP2), hdc);
			}
			if(hBitmapSpaceship5 != NULL)
			{
				//hdc = BeginPaint(GetDlgItem(hWnd,IDC_BMPSPACESHIP5), &ps);
				hdc = GetDC(GetDlgItem(hWnd,IDC_BMPSPACESHIP5));
				hdcMem = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmapSpaceship5);
				GetClientRect(GetDlgItem(hWnd,IDC_BMPSPACESHIP5),&rect);
				StretchBlt(hdc, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top), hdcMem, 0, 0, bitmapSpaceship5.bmWidth, bitmapSpaceship5.bmHeight, SRCCOPY);
				//EndPaint(hWnd, &ps);
				SelectObject(hdcMem, hOldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hOldBitmap);
				ReleaseDC(GetDlgItem(hWnd,IDC_BMPSPACESHIP5), hdc);
			}
			if(hBitmapSpaceship13 != NULL)
			{
				//hdc = BeginPaint(GetDlgItem(hWnd,IDC_BMPSPACESHIP13), &ps);
				hdc = GetDC(GetDlgItem(hWnd,IDC_BMPSPACESHIP13));
				hdcMem = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmapSpaceship13);
				GetClientRect(GetDlgItem(hWnd,IDC_BMPSPACESHIP13),&rect);
				StretchBlt(hdc, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top), hdcMem, 0, 0, bitmapSpaceship13.bmWidth, bitmapSpaceship13.bmHeight, SRCCOPY);
				//EndPaint(hWnd, &ps);
				SelectObject(hdcMem, hOldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hOldBitmap);
				ReleaseDC(GetDlgItem(hWnd,IDC_BMPSPACESHIP13), hdc);
			}
			if(hBitmapStarsail != NULL)
			{
				//hdc = BeginPaint(GetDlgItem(hWnd,IDC_BMPSTARSAIL), &ps);
				hdc = GetDC(GetDlgItem(hWnd,IDC_BMPSTARSAIL));
				hdcMem = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmapStarsail);
				GetClientRect(GetDlgItem(hWnd,IDC_BMPSTARSAIL),&rect);
				StretchBlt(hdc, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top), hdcMem, 0, 0, bitmapStarsail.bmWidth, bitmapStarsail.bmHeight, SRCCOPY);
				//EndPaint(hWnd, &ps);
				SelectObject(hdcMem, hOldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hOldBitmap);
				ReleaseDC(GetDlgItem(hWnd,IDC_BMPSTARSAIL), hdc);
			}
			break;
		}

	case WM_CLOSE:
		// Code 0 zurückliefern; das bedeutet: Dialog abgebrochen
		EndDialog(hWnd, 0);
		break;

	case WM_DESTROY:
		//Timer löschen
		KillTimer(hWnd, 0);
		break;

	case WM_COMMAND:
		//Befehlsverarbeitung
		switch(LOWORD(WParam))
		{
		case IDC_OK:
			//falls OK-Button gedrueckt
			{
				TCHAR*	tcUsername = new TCHAR[100];
				int		shipChoice = -1;
				GetDlgItemText(hWnd, IDC_USERNAME, tcUsername, 100);

				if(IsDlgButtonChecked(hWnd, IDC_BIGSHIP1) == BST_CHECKED)
				{
					shipChoice = 0;
				}
				else if(IsDlgButtonChecked(hWnd, IDC_SHUSUI) == BST_CHECKED)
				{
					shipChoice = 1;
				}
				else if(IsDlgButtonChecked(hWnd, IDC_SPACESHIP2) == BST_CHECKED)
				{
					shipChoice = 2;
				}
				else if(IsDlgButtonChecked(hWnd, IDC_SPACESHIP5) == BST_CHECKED)
				{
					shipChoice = 3;
				}
				else if(IsDlgButtonChecked(hWnd, IDC_SPACESHIP13) == BST_CHECKED)
				{
					shipChoice = 4;
				}
				else if(IsDlgButtonChecked(hWnd, IDC_STARSAIL) == BST_CHECKED)
				{
					shipChoice = 5;
				}

				InitializeCriticalSection(&MyDPlay::m_csDP);

				if(!(IsDlgButtonChecked(hWnd, IDC_BIGSHIP1) == BST_CHECKED) && 
					!(IsDlgButtonChecked(hWnd, IDC_SHUSUI) == BST_CHECKED) &&
					!(IsDlgButtonChecked(hWnd, IDC_SPACESHIP2) == BST_CHECKED) &&
					!(IsDlgButtonChecked(hWnd, IDC_SPACESHIP5) == BST_CHECKED) &&
					!(IsDlgButtonChecked(hWnd, IDC_SPACESHIP13) == BST_CHECKED) &&
					!(IsDlgButtonChecked(hWnd, IDC_STARSAIL) == BST_CHECKED))
				{
					MessageBox(NULL, "Waehlen Sie ein Schiff aus", "ERROR", MB_OK | MB_ICONSTOP);
				}
				else if(IsDlgButtonChecked(hWnd, IDC_PLAYERMODE) == BST_CHECKED)
					//SinglePlayer
				{
					MyDPlay::m_pUsername = tcUsername;
					//MessageBox(NULL, MyDPlay::m_pUsername, "Message", MB_OK);
					//Timer löschen
					KillTimer(hWnd, 0);
					EndDialog(hWnd, 0);
					//Spielfenster erzeugen
					GenerateGameWindow(NULL, 0, shipChoice);
				}
				else if((IsDlgButtonChecked(hWnd, IDC_SERVER) == BST_CHECKED) || (IsDlgButtonChecked(hWnd, IDC_CLIENT) == BST_CHECKED)) {
					//MultiPlayer
					GameManagement(hWnd, false, shipChoice);
				}
				else {
					MessageBox(hWnd, "Korrigieren Sie Ihre Eingaben", "ERROR", MB_OK | MB_ICONSTOP);
				}
				MyTools::deleteArray(tcUsername);
				
			break;
			}

		case IDC_CANCEL:
			//falls Cancel-Button gedrueckt
				MessageBox(NULL,"CANCEL-Button pressed","Message",MB_OK | MB_ICONINFORMATION);
			break;
		case IDC_QUIT:
			//falls Quit-Button gedrueckt
				//MessageBox(NULL,"QUIT-Button pressed","Message",MB_OK | MB_ICONINFORMATION);
				EndDialog(hWnd,0);
			break;
		case IDC_CHECKCON:
			GameManagement(hWnd, true, -1);
			break;
		case IDC_PLAYERMODE:
			{
				mode = IsDlgButtonChecked(hWnd, IDC_PLAYERMODE);
				if(mode == BST_CHECKED)
				{
					server = client = ip = port = check = FALSE;
					EnableWindow(GetDlgItem(hWnd, IDC_SERVER), server);
					EnableWindow(GetDlgItem(hWnd, IDC_CLIENT), client);
					EnableWindow(GetDlgItem(hWnd, IDC_IPADDRESS), ip);
					EnableWindow(GetDlgItem(hWnd, IDC_PORT), port);
					EnableWindow(GetDlgItem(hWnd, IDC_CHECKCON), check);
				}
				else {
					server = client = TRUE;
					EnableWindow(GetDlgItem(hWnd, IDC_SERVER), server);
					EnableWindow(GetDlgItem(hWnd, IDC_CLIENT), client);
					server = IsDlgButtonChecked(hWnd, IDC_SERVER);
					client = IsDlgButtonChecked(hWnd, IDC_CLIENT);
					if(server == BST_CHECKED)
					{
						port = check = TRUE;
						EnableWindow(GetDlgItem(hWnd, IDC_PORT), port);
						EnableWindow(GetDlgItem(hWnd, IDC_CHECKCON), check);
					}
					else if(client == BST_CHECKED)
					{
						ip = port = check = TRUE;
						EnableWindow(GetDlgItem(hWnd, IDC_IPADDRESS), ip);
						EnableWindow(GetDlgItem(hWnd, IDC_PORT), port);
						EnableWindow(GetDlgItem(hWnd, IDC_CHECKCON), check);
					}
				}
				break;
			}
		case IDC_SERVER:
			{
				server = IsDlgButtonChecked(hWnd, IDC_SERVER);
				if(server == BST_CHECKED)
				{
					ip = FALSE;
					port = check = TRUE;
					EnableWindow(GetDlgItem(hWnd, IDC_IPADDRESS), ip);
					EnableWindow(GetDlgItem(hWnd, IDC_PORT), port);
					EnableWindow(GetDlgItem(hWnd, IDC_CHECKCON), check);
				}
				break;
			}
		case IDC_CLIENT:
			{
				client = IsDlgButtonChecked(hWnd, IDC_CLIENT);
				if(client == BST_CHECKED)
				{
					ip = port = check = TRUE;
					EnableWindow(GetDlgItem(hWnd, IDC_IPADDRESS), ip);
					EnableWindow(GetDlgItem(hWnd, IDC_PORT), port);
					EnableWindow(GetDlgItem(hWnd, IDC_CHECKCON), check);
				}
				break;
			}
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
	g_hInst			= hInst;

	MyDPlay::m_pMeshPaths.push_back("resources/x_files/bigship1.x");
	MyDPlay::m_pMeshPaths.push_back("resources/x_files/shusui.x");
	MyDPlay::m_pMeshPaths.push_back("resources/x_files/spaceship 2.x");
	MyDPlay::m_pMeshPaths.push_back("resources/x_files/spaceship 5.x");
	MyDPlay::m_pMeshPaths.push_back("resources/x_files/spaceship 13.x");
	MyDPlay::m_pMeshPaths.push_back("resources/x_files/star sail.x");

	InitCommonControls();

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if(DialogBox(hInst,MAKEINTRESOURCE(IDD_VRRACE),NULL,DialogProc))
	{
		//Fehler aufgetreten
		MessageBox(NULL,"Fehler bei der Initialisierung der Applikation","ERROR",MB_OK | MB_ICONSTOP);
		return 1;
	}

	CoUninitialize();

	return 0;
}