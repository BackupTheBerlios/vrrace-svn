#pragma once
#ifndef MYUSERINPUT_H
#define MYUSERINPUT_H

#include "includes.h"

class MyUserInput
{
public:
	MyUserInput(void);
	~MyUserInput(void);

	bool	init(HINSTANCE* hInst, HWND* hWnd);			//alles Initialisieren
	void	inputKB();									//Tastatureingaben verarbeiten

private:
	bool	initDinput();								//Initialisierung des Input-Objektes
	bool	initKeyboard();								//Initialisierung der Tastatur
	bool	initJoystick();								//Initialisierung des Joysticks

	HINSTANCE*				m_hInst;
	HWND*					m_hWnd;
	HRESULT					m_hr;
	LPDIRECTINPUT8			m_lpDI;
	LPDIRECTINPUTDEVICE8	m_lpDIDevice;


};
#endif