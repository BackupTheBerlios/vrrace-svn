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
	int		initGame(void);									//Initialisierung des Spiels
	void	runGame();										//mainloop fuer das spiel

	LPDIRECT3DDEVICE9	getDevice();						//zurueckgeben des 3d-devices ?

private:
	void	prepareScene();									//zeichnen der Objekte, Vorbereitung
	void	presentScene();									//abschliessen der Szene
	void	initFont();										//Initialisierung des Fontobjektes
	void	doScene();										//Aufbau der Szene

	LPDIRECT3D9			m_d3d;								//3D-Device
	LPDIRECT3DDEVICE9	m_pD3dDevice;						//Render-Device
	HINSTANCE*			m_hInst;							//Instance auf Programmhandle ?
	HWND*				m_hWnd;								//Handler auf das aktuelle Fenster
	D3DXMATRIX			m_matWorld, m_matView, m_matProj;	//Projektion, Sicht
	BYTE				m_red, m_green, m_blue, m_FontX;	//Hintergrundfarben und Fontposx ??
	CD3DFont*			m_pFont;							//Font-Objekt zum schreiben auf Bildschirm
	TCHAR				m_strFont[100];						//Zeichenkette fuer die Ausgabe
	DWORD				m_dwFontSize;						//Schriftgroesse
	MyGameControl*		m_pGameControl;						//Spielzustand
	MyTest*				m_pKoordSys;						//Koordinatensystem
};
#endif