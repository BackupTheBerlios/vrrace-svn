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

	m_pfFramesPerSecond	= new float(0.0);
}

MyD3DGame::~MyD3DGame(void)
{
	delete	m_pGameControl;
	delete	m_pUserInput;
	delete	m_pKoordSys;
	delete	m_pFont;
	delete	m_pfFramesPerSecond;
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

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed					= TRUE;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat			= d3ddm.Format;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;

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

bool	MyD3DGame::init(HINSTANCE* givenHInst, HWND* givenHWnd)
{
	m_hInst	= givenHInst;
	m_hWnd	= givenHWnd;

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
			m_pGameControl->m_pView->getPos()->getX(),
			m_pGameControl->m_pView->getPos()->getY(),
			m_pGameControl->m_pView->getPos()->getZ()
			),
		&D3DXVECTOR3(
			m_pGameControl->m_pView->getVP()->getX(),
			m_pGameControl->m_pView->getVP()->getY(),
			m_pGameControl->m_pView->getVP()->getZ()
			),
		&D3DXVECTOR3(
			m_pGameControl->m_pView->getUV()->getX(),
			m_pGameControl->m_pView->getUV()->getY(),
			m_pGameControl->m_pView->getUV()->getZ()
			)
		);
		
	m_pD3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	//projection
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI/4, 1.0f, 1.0f, 20000.0f);
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
	TCHAR* temp = new TCHAR[100];

	//Zeige aktuelle Betrachterposition
	sprintf(
		temp,
		"Position: %2.2f %2.2f %2.2f",
		m_pGameControl->m_pView->getPos()->getX(),
		m_pGameControl->m_pView->getPos()->getY(),
		m_pGameControl->m_pView->getPos()->getZ());

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
		m_pGameControl->m_pView->getVP()->getX(),
		m_pGameControl->m_pView->getVP()->getY(),
		m_pGameControl->m_pView->getVP()->getZ());

	m_pFont->DrawText(
		5,
		20,
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
	sprintf(temp,"FPS: %2.2f",*m_pfFramesPerSecond);

	m_pFont->DrawText(
		5,
		35,
		D3DCOLOR_ARGB(
			MM_TEXTCOLOR_ALPHA,
			MM_TEXTCOLOR_RED,
			MM_TEXTCOLOR_GREEN,
			MM_TEXTCOLOR_BLUE),
		temp);

	delete temp;
}

void	MyD3DGame::doScene()
{
	m_pGameControl->drawLights();
	//m_pKoordSys->drawKS(m_pD3dDevice);
	m_pGameControl->drawObjects();
	m_pGameControl->drawStarsField();
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
	m_pGameControl->init(m_pD3dDevice, &m_matWorld);
	m_pGameControl->buildGame();
	m_pGameControl->addPlayer(NULL);
	m_pGameControl->addLight();
	m_pGameControl->loadObjects();
	m_pGameControl->initStarsField();
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

	this->moveScene();
	this->prepareScene();
	this->doScene();
	this->presentScene();
	
	DWORD dwTime1 = timeGetTime();

	*m_pfFramesPerSecond = 1.0f / ((float)(dwTime1-dwTime0)/1000.0f);
}