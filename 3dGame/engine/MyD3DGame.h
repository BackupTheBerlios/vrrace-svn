#pragma once
#ifndef MYD3DGAME_H
#define MYD3DGAME_H

#include "includes.h"

class MyD3DGame
{
public:
	MyD3DGame(void);
	~MyD3DGame(void);

	bool	init3D(HINSTANCE* givenHInst, HWND* givenHWnd);	//Initialisierung von D3D
	void	prepareScene();									//zeichnen der Objekte
	void	presentScene();

	LPDIRECT3DDEVICE9	getDevice();

private:
	LPDIRECT3D9			m_d3d;			//3D-Device
	LPDIRECT3DDEVICE9	m_d3dDevice;	//Render-Device
	HINSTANCE*			m_hInst;
	HWND*				m_hWnd;

	D3DXMATRIX			m_matWorld, m_matView, m_matProj;

	BYTE				m_red, m_green, m_blue, m_FontX;

	CD3DFont*			m_pFont;
	TCHAR				m_strFont[100];
	DWORD				m_dwFontSize;

	MyGameControl*		m_pGameControl;	//Spielzustand

public:
	int initGame(void);
};
#endif