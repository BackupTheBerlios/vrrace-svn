#include "includes.h"

MyD3DGame::MyD3DGame(void)
{
	m_d3d			= NULL;
	m_d3dDevice		= NULL;
	m_hInst			= NULL;
	m_hWnd			= NULL;

	m_red			= 0;
	m_green			= 0;
	m_blue			= 0;

	m_FontX			= 0;

	lstrcpy( m_strFont, _T("Arial" ) );
	m_dwFontSize	= 12;

	m_pFont	= new CD3DFont( m_strFont, m_dwFontSize);

	m_pGameControl	= new MyGameControl();
}

MyD3DGame::~MyD3DGame(void)
{
	delete m_pGameControl;
}

bool	MyD3DGame::init3D(HINSTANCE* givenHInst, HWND* givenHWnd)
{
	D3DDISPLAYMODE			d3ddm;
	D3DPRESENT_PARAMETERS	d3dpp;

	m_hInst	= givenHInst;
	m_hWnd	= givenHWnd;

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

	HRESULT hr = m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, *m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_d3dDevice);	//3d-Device erstellen
	char* test =new char[100];
	sprintf(test, "%d", hr);
	if (FAILED(hr))
	{
		if (hr == D3DERR_INVALIDCALL)
		MessageBox(*m_hWnd, test, "error", 1);
		return false;
	}

	m_d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	m_pFont->InitDeviceObjects(m_d3dDevice);
	m_pFont->RestoreDeviceObjects();

	return true;
}

void	MyD3DGame::prepareScene()
{
	m_red++;

	if ((m_red % 2)	== 0)
	m_FontX++;
	//world matrix
	D3DXMatrixIdentity(&m_matWorld);
	m_d3dDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	//view matrix
	D3DXMatrixLookAtLH(&m_matView,
		&D3DXVECTOR3(m_pGameControl->m_pView->m_Position.x,
			m_pGameControl->m_pView->m_Position.y,
			m_pGameControl->m_pView->m_Position.z),
		&D3DXVECTOR3(m_pGameControl->m_pView->m_ViewPoint.x,
			m_pGameControl->m_pView->m_ViewPoint.y,
			m_pGameControl->m_pView->m_ViewPoint.z),
		&D3DXVECTOR3(m_pGameControl->m_pView->m_UpVector.x,
			m_pGameControl->m_pView->m_UpVector.y,
			m_pGameControl->m_pView->m_UpVector.z));
		
	m_d3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	//projection
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f);
	m_d3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(m_red,m_green,m_blue), 1.0f, 0L);
	m_d3dDevice->BeginScene();

	m_pFont->DrawText(2, m_FontX, D3DCOLOR_ARGB(255,255,255,0), "Hello Welt");
	
}

void	MyD3DGame::presentScene()
{
	m_d3dDevice->EndScene();
	m_d3dDevice->Present(NULL, NULL, NULL, NULL);
}

LPDIRECT3DDEVICE9	MyD3DGame::getDevice()
{
	return m_d3dDevice;
}

int MyD3DGame::initGame(void)
{
	m_pGameControl->init();
	return 0;
}
