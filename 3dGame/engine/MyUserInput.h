#pragma once
#ifndef MYUSERINPUT_H
#define MYUSERINPUT_H

#include "includes.h"
#include "MyGameControl.h"

class MyUserInput
{
public:
	MyUserInput(void);
	~MyUserInput(void);

	bool	init(HINSTANCE* hInst, HWND* hWnd, MyGameControl* givenGC);	//alles Initialisieren
	void	inputKB();													//Tastatureingaben verarbeiten

private:
	bool	initDinput();												//Initialisierung des Input-Objektes
	bool	initKeyboard();												//Initialisierung der Tastatur
	bool	initJoystick();												//Initialisierung des Joysticks
	bool	initMouse();												//Initialisierung der Maus
	BOOL	CALLBACK	EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
//	BOOL	CALLBACK	EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);

	HINSTANCE*				m_hInst;
	HWND*					m_hWnd;
	HRESULT					m_hr;
	LPDIRECTINPUT8			m_lpDI;
	LPDIRECTINPUTDEVICE8	m_lpDIDevice;
	LPDIRECTINPUTDEVICE8	m_pMouse;
	LPDIRECTINPUTDEVICE8	m_pJoystick;
	MyGameControl*			_m_pGameControl;
};
#endif