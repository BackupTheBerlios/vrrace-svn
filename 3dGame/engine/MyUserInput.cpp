#include "MyUserInput.h"

MyUserInput::MyUserInput()
{
	m_lpDI			= NULL;
	m_lpDIDevice	= NULL;
	_m_pGameControl	= NULL;
	m_pJoystick		= NULL;
}

MyUserInput::~MyUserInput(void)
{
}

bool	MyUserInput::init(HINSTANCE* hInst, HWND* hWnd, MyGameControl* givenGC)
{
	m_hInst			= hInst;
	m_hWnd			= hWnd;
	_m_pGameControl	= givenGC;

	if (FAILED(initDinput()))	{return false;}
	if (FAILED(initKeyboard()))	{return false;}
	if (FAILED(initMouse()))	{return false;}

	return true;
}

bool	MyUserInput::initDinput()
{
	m_hr	= DirectInput8Create(*m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_lpDI, NULL);

	if (FAILED(m_hr))	{return false;}

	return true;
}

bool	MyUserInput::initMouse()
{
	//Device erstellen
	m_hr	= m_lpDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if (FAILED(m_hr))	{return false;}

	//DataFormat setzen
	m_hr	= m_pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(m_hr))	{return false;}

	//Verhalten definieren
	m_hr	= m_pMouse->SetCooperativeLevel(*m_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(m_hr))	{return false;}

	//Vorbereitung von gepufferten Input
	DIPROPDWORD	dipdw;
	
	dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize	= sizeof(DIPROPDWORD);
	dipdw.diph.dwObj		= 0;
	dipdw.diph.dwHow		= DIPH_DEVICE;
	dipdw.dwData			= SAMPLE_BUFFER_SIZE;

	m_hr	= m_pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (FAILED(m_hr))	{return false;}

	m_hr	= m_pMouse->Acquire();
	if (FAILED(m_hr))	{return false;}

	return true;
}
/*
bool	MyUserInput::initJoystick()
{
	m_lpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
	return true;
}
/*
BOOL static CALLBACK MyUserInput::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	if (FAILED(m_hr = m_lpDI->CreateDevice(pdidInstance->guidInstance, &m_pJoystick, NULL)))
	{
		return DIENUM_CONTINUE;
	} else {
		return DIENUM_STOP;
	}
}
*/
bool	MyUserInput::initKeyboard()
{
	//Device erstellen
	m_hr	= m_lpDI->CreateDevice(GUID_SysKeyboard, &m_lpDIDevice, NULL);
	if (FAILED(m_hr))	{return false;}

	//DataFormat setzen
	m_hr	= m_lpDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(m_hr))	{return false;}

	//Verhalten definieren
	m_hr	= m_lpDIDevice->SetCooperativeLevel(*m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(m_hr))	{return false;}

	//Zugriff
	m_hr	= m_lpDIDevice->Acquire();
	if (FAILED(m_hr))	{return false;}

	return true;
}

void	MyUserInput::inputKB()
{
	#define KEYDOWN(name, key) (name[key] & 0x80)

	char	buffer[256];

	if (m_lpDIDevice)
	{
		m_hr	= m_lpDIDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);

		if SUCCEEDED(m_hr)
		{
			//muss hier durch switch-block ersetzt werden
			if (KEYDOWN(buffer, DIK_ESCAPE ))
			{
				exit(1);
			}
			if (KEYDOWN(buffer, DIK_F1 ))
			{
				if (_m_pGameControl->m_bShowStatus)
					_m_pGameControl->m_bShowStatus	= false;
				else
					_m_pGameControl->m_bShowStatus	= true;
			}
			if (KEYDOWN(buffer, DIK_F2))
			{
//				_m_pGameControl->buildGame();
			}
			if (KEYDOWN(buffer, DIK_F3))
			{
				_m_pGameControl->addLight();
			}
			
		}
		else
		{
			if (m_hr == DIERR_INPUTLOST)
			{
				while (FAILED(m_hr = m_lpDIDevice->Acquire()))
				{
				}
			}
		}
	}
	return;
}