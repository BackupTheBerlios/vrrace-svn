#include "MyD3DGame.h"

MyD3DGame::MyD3DGame(void)
{
	m_d3d			= NULL;
	m_pD3dDevice	= NULL;
	m_hInst			= NULL;
	m_hWnd			= NULL;

	lstrcpy( m_strFont, _T("Arial" ) );
	m_dwFontSize	= 12;

	m_pFont			= new CD3DFont( m_strFont, m_dwFontSize);
	m_pGameControl	= new MyGameControl();
	m_pUserInput	= new MyUserInput();
	m_pKoordSys		= new MyTest();
	m_pDirectPlay	= NULL;
	m_iDPchoice		= -1;

	m_pfFramesPerSecond	= new float(0.0);
	m_dwTmpTime	= timeGetTime();

	m_dSumTime		= 0.0;
	m_iTimeCounter	= 0;
}

MyD3DGame::~MyD3DGame(void)
{
	SAFE_DELETE(m_pUserInput);
	SAFE_DELETE(m_pKoordSys);
	SAFE_DELETE(m_pFont);	
	SAFE_DELETE(m_pGameControl);
	
	//delete	m_pfFramesPerSecond;
	SAFE_DELETE(m_pfFramesPerSecond);
	SAFE_RELEASE(m_pD3dDevice);
	SAFE_RELEASE(m_d3d);
}

bool	MyD3DGame::init3D()
{
	D3DDISPLAYMODE			d3ddm;
	D3DPRESENT_PARAMETERS	d3dpp;

	if ((m_d3d	= Direct3DCreate9(D3D_SDK_VERSION))	== NULL)
	{
		return false;
	}
	
	if (FAILED(m_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.Windowed						= TRUE;
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat				= d3ddm.Format;
	d3dpp.hDeviceWindow					= *m_hWnd;
	d3dpp.EnableAutoDepthStencil		= TRUE;
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr = m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, *m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3dDevice);	//3d-Device erstellen
	char* test =new char[100];
	sprintf(test, "%d", hr);
	if (FAILED(hr))
	{
		if (hr == D3DERR_INVALIDCALL)
		MessageBox(*m_hWnd, test, "error", 1);
		return false;
	}

	m_pD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pD3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pD3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pD3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pD3dDevice->SetRenderState(D3DRS_AMBIENT, 0xff010101);

	m_pD3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	m_pD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	return true;
}

bool	MyD3DGame::init(HINSTANCE* givenHInst, HWND* givenHWnd, MyDPlay* givenDPlay, int choice, int shipChoice)
{
	m_hInst			= givenHInst;
	m_hWnd			= givenHWnd;
	m_pDirectPlay	= givenDPlay;
	m_iDPchoice		= choice;
	m_iShipChoice	= shipChoice;

	if (FAILED(this->init3D()))
	{
		return false;
	}

	if (FAILED(this->initFont()))
	{
		return false;
	}

	if (FAILED(this->initGame()))
	{
		return false;
	}

	if (FAILED(this->initInput()))
	{
		return false;
	}

	m_pGameControl->startSound();

	return true;
}

bool	MyD3DGame::initInput()
{
	return m_pUserInput->init(m_hInst, m_hWnd, m_pGameControl);
}
	
bool	MyD3DGame::initFont()
{
	m_pFont->InitDeviceObjects(m_pD3dDevice);
	m_pFont->RestoreDeviceObjects();

	return true;
}

void	MyD3DGame::prepareScene()
{
	//world matrix
	D3DXMatrixIdentity(&m_matWorld);
	m_pD3dDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	//view matrix
	D3DXMatrixLookAtLH(
		&m_matView,
		&D3DXVECTOR3(
			m_pGameControl->m_pMainCam->getPos()->getX(),
			m_pGameControl->m_pMainCam->getPos()->getY(),
			m_pGameControl->m_pMainCam->getPos()->getZ()
			),
		&D3DXVECTOR3(
			m_pGameControl->m_pMainCam->getVP()->getX(),
			m_pGameControl->m_pMainCam->getVP()->getY(),
			m_pGameControl->m_pMainCam->getVP()->getZ()
			),
		&D3DXVECTOR3(
			m_pGameControl->m_pMainCam->getUV()->getX(),
			m_pGameControl->m_pMainCam->getUV()->getY(),
			m_pGameControl->m_pMainCam->getUV()->getZ()
			)
		);
		
	m_pD3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	//projection
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI/4, 1.0f, 1.0f, 20000000.0f);
	m_pD3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pD3dDevice->Clear(
		0,
		NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(
			MM_BGCOLOR_RED,
			MM_BGCOLOR_GREEN,
			MM_BGCOLOR_BLUE),
		1.0f,
		0L);

	m_pD3dDevice->BeginScene();
}

void	MyD3DGame::showStatus()
{
	EnterCriticalSection(&MyDPlay::m_csDP);

	TCHAR* temp = new TCHAR[100];

	//Zeige aktuelle Betrachterposition
	sprintf(
		temp,
		"Position: %2.2f %2.2f %2.2f",
		m_pGameControl->m_pMainCam->getPos()->getX(),
		m_pGameControl->m_pMainCam->getPos()->getY(),
		m_pGameControl->m_pMainCam->getPos()->getZ());

	m_pFont->DrawText(
		5,
		5,
		D3DCOLOR_ARGB(
			MM_TEXTCOLOR_ALPHA,
			MM_TEXTCOLOR_RED,
			MM_TEXTCOLOR_GREEN,
			MM_TEXTCOLOR_BLUE),
		temp);

	
	//Zeige den Fluchtpunkt
	sprintf(
		temp,
		"Fluchtpunkt: %2.2f %2.2f %2.2f",
		m_pGameControl->m_pMainCam->getVP()->getX(),
		m_pGameControl->m_pMainCam->getVP()->getY(),
		m_pGameControl->m_pMainCam->getVP()->getZ());

	m_pFont->DrawText(
		5,
		20,
		D3DCOLOR_ARGB(
			MM_TEXTCOLOR_ALPHA,
			MM_TEXTCOLOR_RED,
			MM_TEXTCOLOR_GREEN,
			MM_TEXTCOLOR_BLUE),
		temp);

	//Zeige den UpVector
	sprintf(
		temp,
		"UpVector: %2.2f %2.2f %2.2f",
		m_pGameControl->m_pMainCam->getUV()->getX(),
		m_pGameControl->m_pMainCam->getUV()->getY(),
		m_pGameControl->m_pMainCam->getUV()->getZ());

	m_pFont->DrawText(
		5,
		35,
		D3DCOLOR_ARGB(
			MM_TEXTCOLOR_ALPHA,
			MM_TEXTCOLOR_RED,
			MM_TEXTCOLOR_GREEN,
			MM_TEXTCOLOR_BLUE),
		temp);

	//Zeige den Speed
	sprintf(
		temp,
		"Speed: %2.2f",
		*m_pGameControl->getPlayer()->getMesh()->m_pSpeed);

	m_pFont->DrawText(
		5,
		50,
		D3DCOLOR_ARGB(
			MM_TEXTCOLOR_ALPHA,
			MM_TEXTCOLOR_RED,
			MM_TEXTCOLOR_GREEN,
			MM_TEXTCOLOR_BLUE),
		temp);

	//Zeige Menge der Lichter und Meshes
	sprintf(
		temp,
		"Lichter: %d | Meshes: %d",
		m_pGameControl->getNumLights(),
		m_pGameControl->getNumMeshes());

	//Zeige Anzahl der Frames per Second
	sprintf(temp,"FPS: %d",/**m_pfFramesPerSecond*/m_pDirectPlay->m_iFrameRate);

	m_pFont->DrawText(
		5,
		65,
		D3DCOLOR_ARGB(
			MM_TEXTCOLOR_ALPHA,
			MM_TEXTCOLOR_RED,
			MM_TEXTCOLOR_GREEN,
			MM_TEXTCOLOR_BLUE),
		temp);

	//Zeige Anzahl der Punkte
	sprintf(temp,"Punkte: %d",/**m_pfFramesPerSecond*/m_pGameControl->getPlayer()->m_points);

	m_pFont->DrawText(
		5,
		80,
		D3DCOLOR_ARGB(
			MM_TEXTCOLOR_ALPHA,
			MM_TEXTCOLOR_RED,
			MM_TEXTCOLOR_GREEN,
			MM_TEXTCOLOR_BLUE),
		temp);
	delete temp;

	LeaveCriticalSection(&MyDPlay::m_csDP);
}

void	MyD3DGame::doScene()
{
	m_pGameControl->drawStarsField();
	m_pGameControl->drawLights();
	//m_pKoordSys->drawKS(m_pD3dDevice);
	m_pGameControl->drawObjects();
	m_pGameControl->presentMusic();
	if (m_pGameControl->m_bShowStatus) this->showStatus();
}

void	MyD3DGame::presentScene()
{
	m_pD3dDevice->EndScene();
	m_pD3dDevice->Present(NULL, NULL, NULL, NULL);
}

LPDIRECT3DDEVICE9	MyD3DGame::getDevice()
{
	return m_pD3dDevice;
}

bool	MyD3DGame::initGame(void)
{
	m_pGameControl->init(m_pD3dDevice, &m_matWorld, m_hWnd, m_pDirectPlay, m_iDPchoice, m_iShipChoice);
	m_pGameControl->initMusic();
	m_pGameControl->initDirectSound();
	m_pGameControl->buildGame();
	m_pGameControl->addPlayer(NULL);
	m_pGameControl->addLight();
	m_pGameControl->loadObjects();
	m_pGameControl->initStarsField();
	m_pDirectPlay->set3DInstance(m_pD3dDevice, &m_matWorld);
	m_pDirectPlay->m_bInitialized = true;
	return true;
}

bool	MyD3DGame::moveScene()
{
	m_pGameControl->moveObjects();

	return true;
}

void	MyD3DGame::runGame()
{
	DWORD dwTime0 = timeGetTime();

	m_pUserInput->inputKB();
	m_pUserInput->inputJS();

	this->moveScene();

	int counter = 0;
	if ((((float)(dwTime0 - m_dwTmpTime)/1000.0f) > 0.15f) && (m_iDPchoice != 0))
	{
		//counter++;
		m_pGameControl->sendData();
		//if (counter == 10)
		//{
			m_pGameControl->sendPlayer(0.0f, 0.0f, 0.0f, 1);
			//counter = 0;
		//}
		m_dwTmpTime = dwTime0;
	}

	this->prepareScene();
	this->doScene();
	this->presentScene();
	

	DWORD dwTime1 = timeGetTime();

	*m_pfFramesPerSecond = ((float)(dwTime1-dwTime0)/1000.0f);

	if(m_iDPchoice != 2)
	{
		EnterCriticalSection(&MyDPlay::m_csDP);

		if(m_iTimeCounter == 10)
		{
			g_dFrameTime	= m_dSumTime / m_iTimeCounter;
			//if(g_dFrameTime > 0.0)
			//	g_dFrameRate	= 1000.0 / g_dFrameTime;
			if(g_dFrameTime > (1000.0 / (double)m_pDirectPlay->m_iFrameRate))
			{
				if(m_pDirectPlay->m_iFrameRate > 10)
					m_pDirectPlay->m_iFrameRate -= 10;
				/*if(m_iDPchoice == 1)
				{
					INFO sendingToken;
					sendingToken.framerate = m_pDirectPlay->m_iFrameRate;
					m_pDirectPlay->sendMessage(&sendingToken, 2);
				}*/
			} else if(g_dFrameTime < ((1000.0 / (double)m_pDirectPlay->m_iFrameRate) / 1.1))
			{
				if((m_pDirectPlay->m_iFrameRate += 10) > 150)
					m_pDirectPlay->m_iFrameRate = 150;
				/*if(m_iDPchoice == 1)
				{
					INFO sendingToken;
					sendingToken.framerate = m_pDirectPlay->m_iFrameRate;
					m_pDirectPlay->sendMessage(&sendingToken, 2);
				}*/
			}
			m_dSumTime		= 0.0;
			m_iTimeCounter	= 0;
		}
		else {
			m_dSumTime		+= (double)(dwTime1-dwTime0);
			m_iTimeCounter++;
		}

		LeaveCriticalSection(&MyDPlay::m_csDP);

		if(m_iDPchoice == 1)
		{
			INFO sendingToken;
			sendingToken.framerate = m_pDirectPlay->m_iFrameRate;
			m_pDirectPlay->sendMessage(&sendingToken, 2);
		}
	}

	EnterCriticalSection(&MyDPlay::m_csDP);

	double sleeptime = (1000.0 / (double)MyDPlay::m_iFrameRate) - (double)(dwTime1-dwTime0);
	
	LeaveCriticalSection(&MyDPlay::m_csDP);

	if(sleeptime > 0.0)
	{
		Sleep((int)sleeptime);
	}
	
	
	
	/*char temp[100];
	sprintf(temp,"%f",(1000.0f / 60.0f - (float)(dwTime1-dwTime0)));
	MessageBox(NULL,temp,"Time",MB_OK);*/
}