#include "main.h"


void writeText(int posx, int posy, string givenText, int r, int g, int b)
{
	SetBkMode(hDc, TRANSPARENT);
	SetTextColor(hDc, RGB(r, g, b));
	TextOut(hDc, posx, posy*12, givenText.c_str(), givenText.length());
	ReleaseDC(hWnd, hDc);
}

/*
Windows message Handler, Wird vom OS aufgerufen
*/
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	

	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
/*	case WM_PAINT:
		LPPAINTSTRUCT	lpPaint;
		HDC				phDc;
		RECT			lpRect;
		//MessageBox(NULL, TEXT("paint"), TEXT("STATUS"), MB_ICONERROR | MB_OK);
		//LPPAINTSTRUCT lpPaint;
		phDc = BeginPaint(hWnd, lpPaint);
		GetClientRect(hWnd, &lpRect);
		FillRect(hDc, &lpRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		EndPaint(hWnd, lpPaint);
		break;*/
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
DoFrame, endlosschleife?
*/
void	doFrame(HWND* ghWnd, HDC* tmp)
{
	static	int numticks	= 0;
	float		newTime		= timeGetTime() / 1000.0f;
	float		dt			= newTime - oldTime;

	

	oldTime	= newTime;

	inputKbJ->inputKB();

	d3dscene->m_EyePosition.y += 0.1f;
	d3dscene->m_EyePosition.x = cosf(timeGetTime()/700.0f)*10;
	d3dscene->m_EyePosition.z = sinf(timeGetTime()/700.0f)*10;

/*
	if (d3dscene->r < 255)
	{
		d3dscene->r	+= 1;
	} else {
		if (d3dscene->g < 255)
		{
			d3dscene->g += 1;
		} else {
			if (d3dscene->b < 255)
			{
				d3dscene->b += 1;
			} else {
				d3dscene->r = 0;
				d3dscene->g = 0;
				d3dscene->b = 0;
			}
		}
	}
*/
	d3dscene->sdraw();
	d3dscene->drawMesh(tiger);
	//tiger->draw();
/*	
	for (int count = 0; count < m_Objects.size();count++)
	{
		d3dscene->draw(m_Objects.at(count));
	}

*/	d3dscene->edraw();
	//winDC = GetDC(m_hWnd);


//	writeText(10, "fuck");
	
}

/*
Windows Main-Funktion
*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	cursorY = 1;
    // Register the window class.
    WNDCLASSEX wc =
	{
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
			"fenster",
			NULL
	};
	wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClassEx(&wc);

    // Create the application's window.
//	g_hInst		= hInst;
    hWnd	= CreateWindow("fenster", "scheiss DIRECTX fuck M$", 
                              WS_OVERLAPPEDWINDOW, 10, 10, 1000, 1000,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL);

	hDc		= GetDC(hWnd);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	writeText(80, 1, "build MainObjects", 255, 255, 255);

	d3dscene				= new scene(&hInst, &hWnd);
	inputKbJ				= new userInput(&hInst, &hWnd);
	MyFileLoader*	datei	= new MyFileLoader();
	
	writeText(20, 1, "OK", 0, 255, 0);
	

	masterClass* tempObj = new masterClass();

	tempObj->addPoint(0.0f, 0.0f, 0.0f);
	tempObj->addPoint(3.0f, 0.0f, 0.0f);
	tempObj->addPoint(3.0f, 10.0f, 0.0f);
	tempObj->addPoint(0.0f, 10.0f, 0.0f);
	tempObj->addPoint(0.0f, 0.0f, 3.0f);
	tempObj->addPoint(3.0f, 0.0f, 3.0f);
	tempObj->addPoint(3.0f, 10.0f, 3.0f);
	tempObj->addPoint(0.0f, 10.0f, 3.0f);


	char buffer [33];

	tempObj->addEdge(0, 1);
	tempObj->addEdge(1, 2);
	tempObj->addEdge(2, 3);
	tempObj->addEdge(3, 1);
	tempObj->addEdge(0, 4);
	tempObj->addEdge(0, 5);
	tempObj->addEdge(0, 6);
	tempObj->addEdge(0, 7);
	tempObj->addEdge(4, 5);
	tempObj->addEdge(5, 6);
	tempObj->addEdge(6, 7);
	tempObj->addEdge(7, 4);

	tempObj->addPoly(0, 1, 2);
	tempObj->addPoly(2, 3, 0);

	tempObj->addPoly(1, 5, 6);
	tempObj->addPoly(6, 2, 1);

	tempObj->addPoly(4, 0, 3);
	tempObj->addPoly(3, 7, 4);

	tempObj->addPoly(5, 4, 7);
	tempObj->addPoly(7, 6, 5);
/*
	for (int count = 0; count < 1000; count++)
	{
		tempObj->addPoint(1.0f, 1.0f, count);
//		tempObj->addEdge(0, 1);
	}
*/
	
	m_Objects.push_back(tempObj);

	masterClass* tempObjb = new masterClass();

	tempObjb->addPoint(-60.0f, 0.0f, -60.0f);
	tempObjb->addPoint(60.0f, 0.0f, -60.0f);
	tempObjb->addPoint(60.0f, 0.0f, 60.0f);
	tempObjb->addPoint(-60.0f, 0.0f, 60.0f);

	tempObjb->addPoint(-60.0f, 15.0f, -60.0f);
	tempObjb->addPoint(60.0f, 15.0f, -60.0f);
	tempObjb->addPoint(60.0f, 15.0f, 60.0f);
	tempObjb->addPoint(-60.0f, 15.0f, 60.0f);

	tempObjb->addPoly(0, 1, 2);
	tempObjb->addPoly(2, 3, 0);

	tempObjb->addPoly(6, 5, 4);
	tempObjb->addPoly(4, 7, 6);

	m_Objects.push_back(tempObjb);
	

	itoa((int)tempObj->getPoly(0)->p3->y, buffer, 10);

	//MessageBox(NULL, TEXT(buffer), TEXT("STATUS"), MB_ICONERROR | MB_OK);
	
	writeText(80, 2, "init 3D-Scene", 255, 255, 255);
	if (d3dscene->init3D())
	{
		writeText(20, 2, "OK", 0, 255, 0);
		oldTime	= timeGetTime() / 1000.0f;

		writeText(80, 3, "init Keyboard", 255, 255, 255);
		if (FAILED(inputKbJ->init()))
        { 
            MessageBox(NULL, TEXT("Fehler bei DirectInput8Create"), TEXT("VR Tutorial"), MB_ICONERROR | MB_OK);
            exit(1); 
        }
		writeText(20, 3, "OK", 0, 255, 0);
		
		string world = "game/maps/level1.txt";
		writeText(80, 4, "load World '"+world+"'", 255, 255, 255);
		
		if (datei->getMap(world) == 0)
		{
			writeText(10, 4, "FAILED", 255, 0, 0);
		} else {
			writeText(20, 4, "OK", 0, 255, 0);
		}

		MSG	msg;
		ZeroMemory(&msg, sizeof(msg));


		writeText(10, 5, "start Gameloop", 0, 0, 255);

		/*MyMesh*/	tiger	= new MyMesh();
		//tiger->init(d3dscene->m_d3dDevice);
		//tiger->InitGeometry();

		d3dscene->initMesh(tiger);
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				doFrame(&hWnd, &hDc);
			}
		}
	}
	UnregisterClass("D3D Tutorial", wc.hInstance);
	return 0;
}

