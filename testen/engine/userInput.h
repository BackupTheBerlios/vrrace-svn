#pragma once
#ifndef USERINPUT_H
#define USERINPUT_H

#include "../globals.h"
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <d3dx9.h>
#include <dinput.h>

class userInput
{
public:
	userInput(HINSTANCE* hInst, HWND* hWnd);
	~userInput(void);

	bool	init();										//alles Initialisieren
	void	inputKB();									//Tastatureingaben verarbeiten

private:
	bool	initDinput();								//Initialisierung des Input-Objektes
	bool	initKeyboard();								//Initialisierung der Tastatur
	bool	initJoystick();								//Initialisierung des Joysticks

private:
	HINSTANCE*				m_hInst;
	HWND*					m_hWnd;
	HRESULT					m_hr;
	LPDIRECTINPUT8			m_lpDI;
	LPDIRECTINPUTDEVICE8	m_lpDIDevice;


};
#endif