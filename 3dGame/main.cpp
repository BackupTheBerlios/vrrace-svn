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

bool GenerateGameWindow(MyDPlay* givenDPlay, int choice)
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

	//ShowWindow(hWnd, SW_NORMAL);
	//UpdateWindow(hWnd);

	MSG	msg;
	ZeroMemory(&msg, sizeof(msg));

	g_pD3DGame	= new MyD3DGame();
	
	if (g_pD3DGame->init(&g_hInst, &hWnd, givenDPlay, choice))
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
void GameManagement(HWND hWnd, bool isCheck)
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
		if(mydplay->init(&hWnd,"test",NULL,&dwPort,true))
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
					EndDialog(hWnd, 0);
					//Spielfenster erzeugen
					GenerateGameWindow(mydplay, 1);
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
		mydplay->closeConnection();
	}
	else if(IsDlgButtonChecked(hWnd, IDC_CLIENT) == BST_CHECKED)
	{
		TCHAR* tcIPAddr = new TCHAR[18];
		GetDlgItemText(hWnd, IDC_IPADDRESS, tcIPAddr, 18);
		//MessageBox(NULL,tcIPAddr,"Message",MB_OK | MB_ICONINFORMATION);
		if(mydplay->init(&hWnd,"test",tcIPAddr,&dwPort,false))
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
						EndDialog(hWnd, 0);
						//Spielfenster erzeugen
						GenerateGameWindow(mydplay, 2);
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
		//MessageBox(NULL,"close()","Message",MB_OK | MB_ICONINFORMATION);
		mydplay->closeConnection();
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
	static HBITMAP hBitmapBigship;
	static BITMAP bitmapBigship;
	static HBITMAP hBitmapShusui;
	static BITMAP bitmapShusui;
	static HBITMAP hBitmapSpaceship2;
	static BITMAP bitmapSpaceship2;
	static HBITMAP hBitmapSpaceship5;
	static BITMAP bitmapSpaceship5;
	static HBITMAP hBitmapSpaceship13;
	static BITMAP bitmapSpaceship13;
	static HBITMAP hBitmapStarsail;
	static BITMAP bitmapStarsail;
	HDC hdc, hdcMem;
	PAINTSTRUCT ps;
	
	// Nachricht verarbeiten
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			hBitmapBigship			= (HBITMAP) LoadImage(g_hInst,"resources\\ships\\bigship1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmapBigship,sizeof(BITMAP),(LPVOID) &bitmapBigship);
			hBitmapShusui			= (HBITMAP) LoadImage(g_hInst,"resources\\ships\\shusui.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmapShusui,sizeof(BITMAP),(LPVOID) &bitmapShusui);
			hBitmapSpaceship2		= (HBITMAP) LoadImage(g_hInst,"resources\\ships\\spaceship 2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmapSpaceship2,sizeof(BITMAP),(LPVOID) &bitmapSpaceship2);
			hBitmapSpaceship5		= (HBITMAP) LoadImage(g_hInst,"resources\\ships\\spaceship 5.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmapSpaceship5,sizeof(BITMAP),(LPVOID) &bitmapSpaceship5);
			hBitmapSpaceship13		= (HBITMAP) LoadImage(g_hInst,"resources\\ships\\spaceship 13.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmapSpaceship13,sizeof(BITMAP),(LPVOID) &bitmapSpaceship13);
			hBitmapStarsail			= (HBITMAP) LoadImage(g_hInst,"resources\\ships\\star sail.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmapStarsail,sizeof(BITMAP),(LPVOID) &bitmapStarsail);
			if((hBitmapBigship == NULL) || (hBitmapShusui == NULL) || (hBitmapSpaceship2 == NULL) ||
				(hBitmapSpaceship5 == NULL) || (hBitmapSpaceship13 == NULL) || (hBitmapStarsail == NULL))
			{
				MessageBox(NULL, "Fehler beim Laden des Bildes", "ERROR", MB_OK | MB_ICONSTOP);
			}
			// Timer installieren
			//SetTimer(hWnd, 0, 25, NULL);
			CheckDlgButton(hWnd, IDC_PLAYERMODE, BST_CHECKED);
			//CheckDlgButton(hWnd, IDC_RADIO3, BST_CHECKED);
			EnableWindow(GetDlgItem(hWnd, IDC_SERVER), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_CLIENT), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_IPADDRESS), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_PORT), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_CHECKCON), FALSE);
						
			break;
		}

	case WM_PAINT:
		{
			HBITMAP hOldBitmap;
			RECT rect;
			if(hBitmapBigship != NULL)
			{
				GetClientRect(GetDlgItem(hWnd,IDC_BMPBIGSHIP),&rect);
				hdc = BeginPaint(GetDlgItem(hWnd,IDC_BMPBIGSHIP), &ps);
				hdcMem = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmapBigship);
				StretchBlt(hdc, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top), hdcMem, 0, 0, bitmapBigship.bmWidth, bitmapBigship.bmHeight, SRCCOPY);
				EndPaint(hWnd, &ps);
				SelectObject(hdcMem, hOldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hOldBitmap);
			}
			if(hBitmapShusui != NULL)
			{
				GetClientRect(GetDlgItem(hWnd,IDC_BMPSHUSUI),&rect);
				hdc = BeginPaint(GetDlgItem(hWnd,IDC_BMPSHUSUI), &ps);
				hdcMem = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmapShusui);
				StretchBlt(hdc, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top), hdcMem, 0, 0, bitmapShusui.bmWidth, bitmapShusui.bmHeight, SRCCOPY);
				EndPaint(hWnd, &ps);
				SelectObject(hdcMem, hOldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hOldBitmap);
			}
			if(hBitmapSpaceship2 != NULL)
			{
				GetClientRect(GetDlgItem(hWnd,IDC_BMPSPACESHIP2),&rect);
				hdc = BeginPaint(GetDlgItem(hWnd,IDC_BMPSPACESHIP2), &ps);
				hdcMem = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmapSpaceship2);
				StretchBlt(hdc, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top), hdcMem, 0, 0, bitmapSpaceship2.bmWidth, bitmapSpaceship2.bmHeight, SRCCOPY);
				EndPaint(hWnd, &ps);
				SelectObject(hdcMem, hOldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hOldBitmap);
			}
			if(hBitmapSpaceship5 != NULL)
			{
				GetClientRect(GetDlgItem(hWnd,IDC_BMPSPACESHIP5),&rect);
				hdc = BeginPaint(GetDlgItem(hWnd,IDC_BMPSPACESHIP5), &ps);
				hdcMem = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmapSpaceship5);
				StretchBlt(hdc, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top), hdcMem, 0, 0, bitmapSpaceship5.bmWidth, bitmapSpaceship5.bmHeight, SRCCOPY);
				EndPaint(hWnd, &ps);
				SelectObject(hdcMem, hOldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hOldBitmap);
			}
			if(hBitmapSpaceship13 != NULL)
			{
				GetClientRect(GetDlgItem(hWnd,IDC_BMPSPACESHIP13),&rect);
				hdc = BeginPaint(GetDlgItem(hWnd,IDC_BMPSPACESHIP13), &ps);
				hdcMem = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmapSpaceship13);
				StretchBlt(hdc, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top), hdcMem, 0, 0, bitmapSpaceship13.bmWidth, bitmapSpaceship13.bmHeight, SRCCOPY);
				EndPaint(hWnd, &ps);
				SelectObject(hdcMem, hOldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hOldBitmap);
			}
			if(hBitmapStarsail != NULL)
			{
				GetClientRect(GetDlgItem(hWnd,IDC_BMPSTARSAIL),&rect);
				hdc = BeginPaint(GetDlgItem(hWnd,IDC_BMPSTARSAIL), &ps);
				hdcMem = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmapStarsail);
				StretchBlt(hdc, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top), hdcMem, 0, 0, bitmapStarsail.bmWidth, bitmapStarsail.bmHeight, SRCCOPY);
				EndPaint(hWnd, &ps);
				SelectObject(hdcMem, hOldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hOldBitmap);
			}
			break;
		}

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
			{
				TCHAR*	tcUsername = new TCHAR[100];
				GetDlgItemText(hWnd, IDC_USERNAME, tcUsername, 100);

				InitializeCriticalSection(&MyDPlay::m_csDP);

				if(IsDlgButtonChecked(hWnd, IDC_PLAYERMODE) == BST_CHECKED)
					//SinglePlayer
				{
					EndDialog(hWnd, 0);
					//Spielfenster erzeugen
					GenerateGameWindow(NULL, 0);
				}
				else if((IsDlgButtonChecked(hWnd, IDC_SERVER) == BST_CHECKED) || (IsDlgButtonChecked(hWnd, IDC_CLIENT) == BST_CHECKED)) {
					//MultiPlayer
					GameManagement(hWnd, false);
					/*TCHAR*		tcPort	= new TCHAR[10];
					DWORD		dwPort	= 0;
					MyDPlay*	mydplay = new MyDPlay();
					if((IsDlgButtonChecked(hWnd, IDC_SERVER) == BST_CHECKED) || (IsDlgButtonChecked(hWnd, IDC_CLIENT) == BST_CHECKED))
					{
						GetDlgItemText(hWnd, IDC_PORT, tcPort, 10);
						dwPort = _ttoi(tcPort);
					}
					if(IsDlgButtonChecked(hWnd, IDC_SERVER) == BST_CHECKED)
					{
						if(mydplay->init(&hWnd,"test",NULL,&dwPort,true))
						{
							if(mydplay->createSession())
							{
								EndDialog(hWnd, 0);
								//Spielfenster erzeugen
								GenerateGameWindow();
							}
							else {
								MessageBox(NULL,"createSession","Message",MB_OK | MB_ICONSTOP);
							}
						}
						else {
							MessageBox(NULL,"Init","Message",MB_OK | MB_ICONSTOP);
						}
						MessageBox(NULL,"close()","Message",MB_OK | MB_ICONINFORMATION);
						mydplay->closeConnection();
					}
					else if(IsDlgButtonChecked(hWnd, IDC_CLIENT) == BST_CHECKED)
					{
						TCHAR* tcIPAddr = new TCHAR[18];
						GetDlgItemText(hWnd, IDC_IPADDRESS, tcIPAddr, 18);
						//MessageBox(NULL,tcIPAddr,"Message",MB_OK | MB_ICONINFORMATION);
						if(mydplay->init(&hWnd,"test",tcIPAddr,&dwPort,false))
						{
							if(mydplay->enumAvailServer())
							{
								if(mydplay->connectSession())
								{
									EndDialog(hWnd, 0);
									//Spielfenster erzeugen
									GenerateGameWindow();
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
						MessageBox(NULL,"close()","Message",MB_OK | MB_ICONINFORMATION);
						mydplay->closeConnection();
						MyTools::deleteArray(tcIPAddr);
					}*/
					//MessageBox(NULL,tcPort,"Message",MB_OK | MB_ICONINFORMATION);

					/*MyDPlay* mydplay = new MyDPlay();
					if(mydplay->init(&hWnd,"test","vrrace","127.0.0.1",true))
					{
						if(mydplay->enumAvailServer())
						{
							if(mydplay->connectSession())
							{
							}
							else {
								MessageBox(NULL,"ConnectSession","Message",MB_OK | MB_ICONSTOP);
							}
						}
						else {
							MessageBox(NULL,"EnumHosts","Message",MB_OK | MB_ICONSTOP);
						}
						if(mydplay->createSession())
						{
						}
						else {
							MessageBox(NULL,"createSession","Message",MB_OK | MB_ICONSTOP);
						}
					}
					else {
						MessageBox(NULL,"Init","Message",MB_OK | MB_ICONSTOP);
					}*/
					
					/*mydplay->closeConnection();
					delete mydplay;*/
						/*MyTools::deleteArray(tcPort);
						MyTools::deleteObject(mydplay);*/
				}
				else {
					MessageBox(hWnd, "Korrigieren Sie Ihre Eingaben", "Error", MB_OK | MB_ICONSTOP);
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
			GameManagement(hWnd, true);
			break;
		case IDC_PLAYERMODE:
			{
				if(IsDlgButtonChecked(hWnd, IDC_PLAYERMODE) == BST_CHECKED)
				{
					EnableWindow(GetDlgItem(hWnd, IDC_SERVER), FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_CLIENT), FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_IPADDRESS), FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_PORT), FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_CHECKCON), FALSE);
				}
				else {
					EnableWindow(GetDlgItem(hWnd, IDC_SERVER), TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_CLIENT), TRUE);
					if(IsDlgButtonChecked(hWnd, IDC_SERVER) == BST_CHECKED)
					{
						EnableWindow(GetDlgItem(hWnd, IDC_PORT), TRUE);
						EnableWindow(GetDlgItem(hWnd, IDC_CHECKCON), TRUE);
					}
					else if(IsDlgButtonChecked(hWnd, IDC_CLIENT) == BST_CHECKED)
					{
						EnableWindow(GetDlgItem(hWnd, IDC_IPADDRESS), TRUE);
						EnableWindow(GetDlgItem(hWnd, IDC_PORT), TRUE);
						EnableWindow(GetDlgItem(hWnd, IDC_CHECKCON), TRUE);
					}
				}
				break;
			}
		case IDC_SERVER:
			{
				if(IsDlgButtonChecked(hWnd, IDC_SERVER) == BST_CHECKED)
				{
					EnableWindow(GetDlgItem(hWnd, IDC_IPADDRESS), FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_PORT), TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_CHECKCON), TRUE);
				}
				break;
			}
		case IDC_CLIENT:
			{
				if(IsDlgButtonChecked(hWnd, IDC_CLIENT) == BST_CHECKED)
				{
					EnableWindow(GetDlgItem(hWnd, IDC_IPADDRESS), TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_PORT), TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_CHECKCON), TRUE);
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