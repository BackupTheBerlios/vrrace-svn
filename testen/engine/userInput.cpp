#include ".\userInput.h"

userInput::userInput(HINSTANCE* hInst, HWND* hWnd)
{
	m_lpDI			= NULL;
	m_lpDIDevice	= NULL;
	m_hInst			= hInst;
	m_hWnd			= hWnd;
}

userInput::~userInput(void)
{
}

bool	userInput::init()
{
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

bool	userInput::initDinput()
{
	m_hr	= DirectInput8Create(*m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_lpDI, NULL);

	if FAILED(m_hr)
	{
		return false;
	}
	return true;
}

bool	userInput::initKeyboard()
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

void	userInput::inputKB()
{
	#define KEYDOWN(name, key) (name[key] & 0x80)

	char	buffer[256];

	if (m_lpDIDevice)
	{
		m_hr	= m_lpDIDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);

		if SUCCEEDED(m_hr)
		{
			if (KEYDOWN(buffer, DIK_ESCAPE ))
			{
				exit(1);
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

//	if (KEYDOWN(buffer, DIK_RIGHT));
//	else if (KEYDOWN(buffer, DIK_LEFT));
}