#include "MyUserInput.h"

MyUserInput::MyUserInput()
{
	m_lpDI			= NULL;
	m_lpDIDevice	= NULL;
	_m_pGameControl	= NULL;
}

MyUserInput::~MyUserInput(void)
{
}

bool	MyUserInput::init(HINSTANCE* hInst, HWND* hWnd, MyGameControl* givenGC)
{
	m_hInst			= hInst;
	m_hWnd			= hWnd;
	_m_pGameControl	= givenGC;

	if FAILED(initDinput())
	{
		return false;
	}

	if FAILED(initKeyboard())
	{
		return false;
	}

	return true;
}

bool	MyUserInput::initDinput()
{
	m_hr	= DirectInput8Create(*m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_lpDI, NULL);

	if FAILED(m_hr)
	{
		return false;
	}
	return true;
}

bool	MyUserInput::initKeyboard()
{
	//Device erstellen
	m_hr	= m_lpDI->CreateDevice(GUID_SysKeyboard, &m_lpDIDevice, NULL);

	if FAILED(m_hr)
	{
		return false;
	}

	//DataFormat setzen
	m_hr	= m_lpDIDevice->SetDataFormat(&c_dfDIKeyboard);

	if FAILED(m_hr)
	{
		return false;
	}

	//Verhalten definieren
	m_hr	= m_lpDIDevice->SetCooperativeLevel(*m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if FAILED(m_hr)
	{
		return false;
	}

	//Zugriff
	m_hr	= m_lpDIDevice->Acquire();

	if FAILED(m_hr)
	{
		return false;
	}

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
				_m_pGameControl->addObject(1);
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