#include "MyUserInput.h"

LPDIRECTINPUT8			MyUserInput::m_lpDI			= NULL;
HRESULT					MyUserInput::m_hr			= NULL;
LPDIRECTINPUTDEVICE8	MyUserInput::m_pJoystick	= NULL;

MyUserInput::MyUserInput()
{
	m_lpDIDevice		= NULL;
	_m_pGameControl		= NULL;
	m_JoystickAvailable	= false;
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

	if (SUCCEEDED(initJoystick())) {m_JoystickAvailable = true;}

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

bool	MyUserInput::initJoystick()
{
	m_lpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);

	if (m_pJoystick == NULL)	{return false;}

	m_hr	= m_pJoystick->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(m_hr))	{return false;}

	m_hr	= m_pJoystick->SetCooperativeLevel(*m_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(m_hr))	{return false;}

	m_diDevCaps.dwSize	= sizeof(DIDEVCAPS);
	m_hr	= m_pJoystick->GetCapabilities(&m_diDevCaps);
	if (FAILED(m_hr))	{return false;}

	m_hr	= m_pJoystick->EnumObjects(EnumObjectsCallback, (VOID*)*m_hWnd, DIDFT_AXIS);
	if (FAILED(m_hr))	{return false;}

	m_hr	= m_pJoystick->Acquire();
	if (FAILED(m_hr))
	{
		return false;
	}

	return true;
}

BOOL CALLBACK MyUserInput::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	if (FAILED(m_hr = m_lpDI->CreateDevice(pdidInstance->guidInstance, &m_pJoystick, NULL)))
	{
		return DIENUM_CONTINUE;
	} else {
		return DIENUM_STOP;
	}
}

BOOL CALLBACK MyUserInput::EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext)
{
	if (pdidoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize		= sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
		diprg.diph.dwHow		= DIPH_BYID;
		diprg.diph.dwObj		= pdidoi->dwType;
		diprg.lMin				= -MAXVAL;
		diprg.lMax				= +MAXVAL;
/*
		DWORD*	pdwNumForceFeedbackAxis = (DWORD*)pContext;
		if ((pdidoi->dwFlags & DIDOI_FFACTUATOR) != 0)
		{
			(*pdwNumForceFeedbackAxis)++;
		}*/
		if (FAILED(m_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}

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

float MyUserInput::inputFactor()
{
	return 1.0f / (float)_m_pGameControl->getDirectPlay()->m_iFrameRate * 0.8f;
}

void	MyUserInput::inputKB()
{
	#define KEYDOWN(name, key) (name[key] & 0x80)

	char	buffer[256];

	if (m_lpDIDevice && _m_pGameControl->getPlayer())
	{
		m_hr	= m_lpDIDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);

		if SUCCEEDED(m_hr)
		{
			//muss hier durch switch-block ersetzt werden
			if (KEYDOWN(buffer, DIK_ESCAPE ))
			{
				exit(1);
			} else 
			if (KEYDOWN(buffer, DIK_F1 ))
			{
				if (_m_pGameControl->m_bShowStatus)
					_m_pGameControl->m_bShowStatus	= false;
				else
					_m_pGameControl->m_bShowStatus	= true;
			} else
			if (KEYDOWN(buffer, DIK_F2))
			{
//				_m_pGameControl->buildGame();
			} else
			if (KEYDOWN(buffer, DIK_F3))
			{
				_m_pGameControl->addLight();
			} else
			if (KEYDOWN(buffer, DIK_LEFT))
			{
				_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f, 0.0f, -this->inputFactor());
				_m_pGameControl->m_pMainCam->rotate();
				_m_pGameControl->sendPlayer(0.0f, 0.0f, -this->inputFactor());
				//_m_pGameControl->m_pMainCam->calcPosition();
			} else
			if (KEYDOWN(buffer, DIK_RIGHT))
			{
				_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f, 0.0f, this->inputFactor());
				_m_pGameControl->m_pMainCam->rotate();
				_m_pGameControl->sendPlayer(0.0f, 0.0f, this->inputFactor());
				//_m_pGameControl->m_pMainCam->calcPosition();
			}
			if (KEYDOWN(buffer, DIK_UP))
			{
				_m_pGameControl->getPlayer()->getMesh()->rotate(-this->inputFactor(),0.0f,0.0f);
				_m_pGameControl->m_pMainCam->rotate();
				_m_pGameControl->sendPlayer(-this->inputFactor(),0.0f,0.0f);
				//_m_pGameControl->m_pMainCam->calcPosition();
			} else
			if (KEYDOWN(buffer, DIK_DOWN))
			{
				_m_pGameControl->getPlayer()->getMesh()->rotate(this->inputFactor(),0.0f,0.0f);
				_m_pGameControl->m_pMainCam->rotate();
				_m_pGameControl->sendPlayer(this->inputFactor(),0.0f,0.0f);
				//_m_pGameControl->m_pMainCam->calcPosition();
			}
			if (KEYDOWN(buffer, DIK_A))
			{
				_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f,-this->inputFactor(),0.0f);
				_m_pGameControl->m_pMainCam->rotate();
				_m_pGameControl->sendPlayer(0.0f, -this->inputFactor(),0.0f);
				//_m_pGameControl->m_pMainCam->calcPosition();
			} else
			if (KEYDOWN(buffer, DIK_D))
			{
				_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f,this->inputFactor(),0.0f);
				_m_pGameControl->m_pMainCam->rotate();
				_m_pGameControl->sendPlayer(0.0f, this->inputFactor(),0.0f);
				//_m_pGameControl->m_pMainCam->calcPosition();
			}
			if (KEYDOWN(buffer, DIK_W))
			{
				//TODO andere Funktion aufrufen, der Betrag von Direction muss verändert werden!
				*_m_pGameControl->getPlayer()->getMesh()->m_pSpeed -= this->inputFactor();
				_m_pGameControl->getPlayer()->getMesh()->move();
				_m_pGameControl->sendPlayer(0.0f,0.0f,0.0f);
				//_m_pGameControl->m_pMainCam->calcPosition();
			} else
			if (KEYDOWN(buffer, DIK_S))
			{
				*_m_pGameControl->getPlayer()->getMesh()->m_pSpeed += this->inputFactor();
				_m_pGameControl->getPlayer()->getMesh()->move();
				_m_pGameControl->sendPlayer(0.0f,0.0f,0.0f);
				//_m_pGameControl->m_pMainCam->calcPosition();
			}
		}
		else
		{
			//if (m_hr == DIERR_INPUTLOST)
			//{
				m_hr = m_lpDIDevice->Acquire();			
			//}
		}
	}
	return;
}

HRESULT	MyUserInput::inputJS()
{
	if (m_JoystickAvailable)
	{
		DIJOYSTATE2	js;

		if (NULL == m_pJoystick)	{return S_OK;}

		m_hr	= m_pJoystick->Poll();

		if (FAILED(m_hr))
		{
			m_hr	= m_pJoystick->Acquire();
			while (m_hr == DIERR_INPUTLOST)
			{
				m_hr	= m_pJoystick->Acquire();
			}
			//MessageBox(NULL, "mmm", "mmm", MB_OK);
			return S_OK;
		}

		m_hr	= m_pJoystick->GetDeviceState(sizeof(DIJOYSTATE2), &js);
		if (FAILED(m_hr))	{return m_hr;}

		if (js.lY < 0)
		{
			_m_pGameControl->getPlayer()->getMesh()->rotate(-0.01f, 0.0f, 0.0f);
			_m_pGameControl->m_pMainCam->rotate();
			_m_pGameControl->sendPlayer(-0.01f, 0.0f, 0.0f);
		}
		if (js.lY > 0)
		{
			_m_pGameControl->getPlayer()->getMesh()->rotate(0.01f, 0.0f, 0.0f);
			_m_pGameControl->m_pMainCam->rotate();
			_m_pGameControl->sendPlayer(0.01f, 0.0f, 0.0f);
		}
		if (js.lX < 0)
		{
			_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f, 0.0f, -0.02f);
			_m_pGameControl->m_pMainCam->rotate();
			_m_pGameControl->sendPlayer(0.0f, 0.0f, -0.02f);
		}
		if (js.lX > 0)
		{
			_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f, 0.0f, 0.02f);
			_m_pGameControl->m_pMainCam->rotate();
			_m_pGameControl->sendPlayer(0.0f, 0.0f, 0.02f);
		}
		if (js.lRz < 0)
		{
			_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f, -0.01f, 0.0f);
			_m_pGameControl->m_pMainCam->rotate();
			_m_pGameControl->sendPlayer(0.0f, -0.01f, 0.0f);
		}
		if (js.lRz > 0)
		{
			_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f, 0.01f, 0.0f);
			_m_pGameControl->m_pMainCam->rotate();
			_m_pGameControl->sendPlayer(0.0f, 0.01f, 0.0f);
		}
		if (js.rglSlider[0] < 0)	{_m_pGameControl->getPlayer()->getMesh()->m_pDirection->addZ(-0.01f);_m_pGameControl->sendPlayer(0.0f, 0.0f, 0.0f);}
		if (js.rglSlider[0] > 0)	{_m_pGameControl->getPlayer()->getMesh()->m_pDirection->addZ(0.01f);_m_pGameControl->sendPlayer(0.0f, 0.0f, 0.0f);}
	}

	return S_OK;
}