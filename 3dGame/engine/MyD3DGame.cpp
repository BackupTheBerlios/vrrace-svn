#include "MyD3DGame.h"

MyD3DGame::MyD3DGame(void)
{
	m_d3d			= NULL;
	m_pD3dDevice	= NULL;
	m_hInst			= NULL;
	m_hWnd			= NULL;

	m_red			= 0;
	m_green			= 0;
	m_blue			= 0;

	m_FontX			= 0;

	lstrcpy( m_strFont, _T("Arial" ) );
	m_dwFontSize	= 12;

	m_pFont			= new CD3DFont( m_strFont, m_dwFontSize);
	m_pGameControl	= new MyGameControl();
	m_pUserInput	= new MyUserInput();
	m_pKoordSys		= new MyTest();
}

MyD3DGame::~MyD3DGame(void)
{
	delete	m_pGameControl;
	delete	m_pUserInput;
	delete	m_pKoordSys;
	delete	m_pFont;
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
	m_pD3dDevice->SetRenderState(D3DRS_AMBIENT, 0xff0a0a0a);

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
			m_pGameControl->m_pView->m_Position.x,
			m_pGameControl->m_pView->m_Position.y,
			m_pGameControl->m_pView->m_Position.z
			),
		&D3DXVECTOR3(
			m_pGameControl->m_pView->m_ViewPoint.x,
			m_pGameControl->m_pView->m_ViewPoint.y,
			m_pGameControl->m_pView->m_ViewPoint.z
			),
		&D3DXVECTOR3(
			m_pGameControl->m_pView->m_UpVector.x,
			m_pGameControl->m_pView->m_UpVector.y,
			m_pGameControl->m_pView->m_UpVector.z
			)
		);
		
	m_pD3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	//projection
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI/4, 1.0f, 1.0f, 1000.0f);
	m_pD3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(m_red,m_green,m_blue), 1.0f, 0L);
	m_pD3dDevice->BeginScene();
}

void	MyD3DGame::showStatus()
{
	TCHAR* temp = new TCHAR[100];

	//Zeige aktuelle Betrachterposition
	sprintf(
		temp,
		"Position: %2.2f %2.2f %2.2f",
		m_pGameControl->m_pView->m_Position.x,
		m_pGameControl->m_pView->m_Position.y,
		m_pGameControl->m_pView->m_Position.z);

	m_pFont->DrawText(5, 5, D3DCOLOR_ARGB(255, 255, 255, 255), temp);

	
	//Zeige den Fluchtpunkt
	sprintf(
		temp,
		"Fluchtpunkt: %2.2f %2.2f %2.2f",
		m_pGameControl->m_pView->m_ViewPoint.x,
		m_pGameControl->m_pView->m_ViewPoint.y,
		m_pGameControl->m_pView->m_ViewPoint.z);

	m_pFont->DrawText(5, 20, D3DCOLOR_ARGB(255, 255, 255, 255), temp);

	//Zeige Menge der Lichter und Meshes
	sprintf(
		temp,
		"Lichter: %d | Meshes: %d",
		m_pGameControl->getNumLights(),
		m_pGameControl->getNumMeshes());

	m_pFont->DrawText(5, 35, D3DCOLOR_ARGB(255, 255, 255, 255), temp);

	delete temp;
}

void	MyD3DGame::doScene()
{
	m_pGameControl->drawLights();
	//m_pKoordSys->drawKS(m_pD3dDevice);
	m_pGameControl->drawObjects(&m_matWorld);
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
	m_pGameControl->init(m_pD3dDevice);
	m_pGameControl->loadObjects();
	return true;
}

bool	MyD3DGame::moveScene()
{
	m_pGameControl->moveObjects();

	return true;
}

void	MyD3DGame::runGame()
{
	m_pUserInput->inputKB();

	this->moveScene();
	this->prepareScene();
	this->doScene();
	this->presentScene();
}