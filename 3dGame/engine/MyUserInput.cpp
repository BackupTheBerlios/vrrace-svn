#include "MyUserInput.h"

LPDIRECTINPUT8			MyUserInput::m_lpDI			= NULL;
HRESULT					MyUserInput::m_hr			= NULL;
LPDIRECTINPUTDEVICE8	MyUserInput::m_pJoystick	= NULL;
BOOL					MyUserInput::EffectFound	= FALSE;
LPDIRECTINPUTEFFECT		MyUserInput::pEff[6];


MyUserInput::MyUserInput()
{
	m_lpDIDevice		= NULL;
	_m_pGameControl		= NULL;
	m_JoystickAvailable	= false;
}

MyUserInput::~MyUserInput(void)
{
	m_lpDIDevice		= NULL;
	_m_pGameControl		= NULL;
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
	m_lpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_FORCEFEEDBACK | DIEDFL_ATTACHEDONLY);

	if (m_pJoystick == NULL)	{return false;}

	m_hr	= m_pJoystick->SetCooperativeLevel(*m_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(m_hr))	{return false;}

	m_hr	= m_pJoystick->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(m_hr))	{return false;}

	m_diDevCaps.dwSize	= sizeof(DIDEVCAPS);
	m_hr	= m_pJoystick->GetCapabilities(&m_diDevCaps);
	if (FAILED(m_hr))	{return false;}

	m_hr	= m_pJoystick->EnumObjects(EnumObjectsCallback, (VOID*)*m_hWnd, DIDFT_AXIS);
	if (FAILED(m_hr))	{return false;}

	m_hr	= m_pJoystick->EnumEffectsInFile("resources/ffeffects/alleffects.ffe", EnumEffectsInFileProc, NULL, DIFEF_MODIFYIFNEEDED);
	if (FAILED(m_hr))	{return false;}
	//m_hr	= m_pJoystick->EnumEffectsInFile("resources/ffeffects/explode.ffe", EnumEffectsInFileProc, NULL, DIFEF_MODIFYIFNEEDED);
	//if (FAILED(m_hr))	{return false;}

	m_hr	= m_pJoystick->Acquire();
	if (FAILED(m_hr))
	{
		return false;
	}

	return true;
}

BOOL CALLBACK MyUserInput::EnumEffectsInFileProc(LPCDIFILEEFFECT lpdife, LPVOID pvRef)
{
    HRESULT     hr;
    static int  i;
 

    hr = m_pJoystick->CreateEffect(lpdife->GuidEffect, 
                               lpdife->lpDiEffect,
                               &pEff[i], 
                               NULL);
    if (FAILED(hr))
    {
        // Error handling
    }
    if (++i > 5) return DIENUM_STOP;
    else return DIENUM_CONTINUE;
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

BOOL CALLBACK MyUserInput::DIEnumEffectsProc(LPCDIEFFECTINFO pei, LPVOID pv)
{
	*((GUID *)pv) = pei->guid;
	EffectFound	= TRUE;
	return DIENUM_STOP;
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

void	MyUserInput::doFF()
{
	if (m_JoystickAvailable)
	{
	if (_m_pGameControl->ff_g)
	{
		pEff[0]->Start(1,0);
	} else {
		pEff[0]->Stop();
	}
	if (_m_pGameControl->ff_f)
	{
		pEff[1]->Start(1,0);
		_m_pGameControl->ff_f = false;
	}/* else {
		pEff[1]->Stop();
	}*/
	if (_m_pGameControl->ff_r)
	{
		pEff[2]->Start(1,0);
		_m_pGameControl->ff_r = false;
	}/* else {
		pEff[2]->Stop();
	}*/
	if (_m_pGameControl->ff_b)
	{
		pEff[3]->Start(1,0);
		_m_pGameControl->ff_b = false;
	}/* else {
		pEff[3]->Stop();
	}*/
	if (_m_pGameControl->ff_l)
	{
		pEff[4]->Start(1,0);
		_m_pGameControl->ff_l = false;
	}/* else {
		pEff[4]->Stop();
	}*/
	if (_m_pGameControl->ff_e)
	{
		pEff[5]->Start(1,0);
		_m_pGameControl->ff_e = false;
	}/* else {
		pEff[5]->Stop();
	}*/
	}
}

float MyUserInput::inputFactor(float givenValue)
{
	return 1.0f / (float)_m_pGameControl->getDirectPlay()->m_iFrameRate * givenValue;
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
				if(_m_pGameControl->_DirectPlay->m_bInitialized)
				{
					EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
					SendMessage(*m_hWnd,WM_CLOSE,NULL,NULL);
					LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
					//exit(1);
				}
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
				EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f, 0.0f, -this->inputFactor(0.8));
				_m_pGameControl->m_pMainCam->rotate();
				LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->sendPlayer(0.0f, 0.0f, -this->inputFactor(0.8), 1);
				//_m_pGameControl->m_pMainCam->calcPosition();
			} else
			if (KEYDOWN(buffer, DIK_RIGHT))
			{
				EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f, 0.0f, this->inputFactor(0.8));
				_m_pGameControl->m_pMainCam->rotate();
				LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->sendPlayer(0.0f, 0.0f, this->inputFactor(0.8), 1);
				//_m_pGameControl->m_pMainCam->calcPosition();
			}
			if (KEYDOWN(buffer, DIK_UP))
			{
				EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->getPlayer()->getMesh()->rotate(-this->inputFactor(0.8),0.0f,0.0f);
				_m_pGameControl->m_pMainCam->rotate();
				LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->sendPlayer(-this->inputFactor(0.8),0.0f,0.0f, 1);
				//_m_pGameControl->m_pMainCam->calcPosition();
			} else
			if (KEYDOWN(buffer, DIK_DOWN))
			{
				EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->getPlayer()->getMesh()->rotate(this->inputFactor(0.8),0.0f,0.0f);
				_m_pGameControl->m_pMainCam->rotate();
				LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->sendPlayer(this->inputFactor(0.8),0.0f,0.0f, 1);
				//_m_pGameControl->m_pMainCam->calcPosition();
			}
			if (KEYDOWN(buffer, DIK_A))
			{
				EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f,-this->inputFactor(0.8),0.0f);
				_m_pGameControl->m_pMainCam->rotate();
				LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->sendPlayer(0.0f, -this->inputFactor(0.8),0.0f, 1);
				//_m_pGameControl->m_pMainCam->calcPosition();
			} else
			if (KEYDOWN(buffer, DIK_D))
			{
				EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f,this->inputFactor(0.8),0.0f);
				_m_pGameControl->m_pMainCam->rotate();
				LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->sendPlayer(0.0f, this->inputFactor(0.8),0.0f, 1);
				//_m_pGameControl->m_pMainCam->calcPosition();
			}
			if (KEYDOWN(buffer, DIK_W))
			{
				EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				//TODO andere Funktion aufrufen, der Betrag von Direction muss verändert werden!
				*_m_pGameControl->getPlayer()->getMesh()->m_pSpeed -= this->inputFactor(0.8);
				//_m_pGameControl->getPlayer()->getMesh()->move();
				LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->sendPlayer(0.0f,0.0f,0.0f, 1);
				//_m_pGameControl->m_pMainCam->calcPosition();
//				pEff[0]->Start(1,0);
			} else
			if (KEYDOWN(buffer, DIK_S))
			{
				EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				*_m_pGameControl->getPlayer()->getMesh()->m_pSpeed += this->inputFactor(0.8);
				//_m_pGameControl->getPlayer()->getMesh()->move();
				LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				_m_pGameControl->sendPlayer(0.0f,0.0f,0.0f, 1);
				//_m_pGameControl->m_pMainCam->calcPosition();
//				pEff[1]->Start(1,0);
			}
			if (KEYDOWN(buffer, DIK_N))
			{
				EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
				if (_m_pGameControl->getPlayer()->getMesh()->m_mustSleep)
				_m_pGameControl->getPlayer()->getMesh()->newInit();
				LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
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
			EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->getPlayer()->getMesh()->rotate(this->inputFactor(js.lY/(float)MAXVAL), 0.0f, 0.0f);
			_m_pGameControl->m_pMainCam->rotate();
			LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->sendPlayer(this->inputFactor(js.lY/(float)MAXVAL), 0.0f, 0.0f, 1);
		}
		if (js.lY > 0)
		{
			EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->getPlayer()->getMesh()->rotate(this->inputFactor(js.lY/(float)MAXVAL), 0.0f, 0.0f);
			_m_pGameControl->m_pMainCam->rotate();
			LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->sendPlayer(this->inputFactor(js.lY/(float)MAXVAL), 0.0f, 0.0f, 1);
		}
		if (js.lX < 0)
		{
			EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f, 0.0f, this->inputFactor(js.lX/(float)MAXVAL));
			_m_pGameControl->m_pMainCam->rotate();
			LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->sendPlayer(0.0f, 0.0f, this->inputFactor(js.lX/(float)MAXVAL), 1);
		}
		if (js.lX > 0)
		{
			EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f, 0.0f, this->inputFactor(js.lX/(float)MAXVAL));
			_m_pGameControl->m_pMainCam->rotate();
			LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->sendPlayer(0.0f, 0.0f, this->inputFactor(js.lX/(float)MAXVAL), 1);
		}
		if (js.lRz < 0)
		{
			EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f, this->inputFactor(js.lRz/(float)MAXVAL), 0.0f);
			_m_pGameControl->m_pMainCam->rotate();
			LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->sendPlayer(0.0f, this->inputFactor(js.lRz/(float)MAXVAL), 0.0f, 1);
		}
		if (js.lRz > 0)
		{
			EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->getPlayer()->getMesh()->rotate(0.0f, this->inputFactor(js.lRz/(float)MAXVAL), 0.0f);
			_m_pGameControl->m_pMainCam->rotate();
			LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->sendPlayer(0.0f, this->inputFactor(js.lRz/(float)MAXVAL), 0.0f, 1);
		}
		if (js.rglSlider[0] < 0)
		{
			EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			*_m_pGameControl->getPlayer()->getMesh()->m_pSpeed += this->inputFactor(js.rglSlider[0]/(float)MAXVAL);
			//_m_pGameControl->getPlayer()->getMesh()->move();
			LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->sendPlayer(0.0f,0.0f,0.0f, 1);
		}
		if (js.rglSlider[0] > 0)
		{
			EnterCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			*_m_pGameControl->getPlayer()->getMesh()->m_pSpeed += this->inputFactor(js.rglSlider[0]/(float)MAXVAL);
			//_m_pGameControl->getPlayer()->getMesh()->move();
			LeaveCriticalSection(&_m_pGameControl->_DirectPlay->m_csDP);
			_m_pGameControl->sendPlayer(0.0f,0.0f,0.0f, 1);
		}
	}

	return S_OK;
}