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
	static	BOOL	CALLBACK	EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
	static	BOOL	CALLBACK	EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);

	HINSTANCE*					m_hInst;
	HWND*						m_hWnd;
	static HRESULT				m_hr;
	static LPDIRECTINPUT8		m_lpDI;
	DIDEVCAPS					m_diDevCaps;
	LPDIRECTINPUTDEVICE8		m_lpDIDevice;
	LPDIRECTINPUTDEVICE8		m_pMouse;
	static LPDIRECTINPUTDEVICE8	m_pJoystick;
	MyGameControl*				_m_pGameControl;
};
#endif