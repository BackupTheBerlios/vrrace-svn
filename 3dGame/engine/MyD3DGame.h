#pragma once
#ifndef MYD3DGAME_H
#define MYD3DGAME_H

#include "includes.h"
#include "MyGameControl.h"
#include "MyTest.h"
#include "MyUserInput.h"
#include "MyMesh.h"
#include "MyDSound.h"

class MyD3DGame
{
public:
	MyD3DGame(void);
	~MyD3DGame(void);

	bool	init(HINSTANCE* givenHInst, HWND* givenHWnd, MyDPlay* givenDPlay, int choice);	//Initialisierung von D3D
	void	runGame();										//mainloop fuer das spiel

	LPDIRECT3DDEVICE9	getDevice();						//zurueckgeben des 3d-devices ?

private:
	void	prepareScene();									//zeichnen der Objekte, Vorbereitung
	void	presentScene();									//abschliessen der Szene
	bool	moveScene();									//alle Bewegungen fuer Objekte, Licht, Kamera
	bool	init3D();										//Device und so erstellen
	bool	initFont();										//Initialisierung des Fontobjektes
	bool	initInput();									//Benutzereingaben initialisieren
	bool	initGame();										//Initialisierung des Spiels
	void	doScene();										//Aufbau der Szene
	void	showStatus();									//Statusfenster anzeigen
	
	LPDIRECT3D9			m_d3d;								//3D-Device
	LPDIRECT3DDEVICE9	m_pD3dDevice;						//Render-Device
	HINSTANCE*			m_hInst;							//Instance auf Programmhandle ?
	HWND*				m_hWnd;								//Handler auf das aktuelle Fenster
	D3DXMATRIX			m_matWorld, m_matView, m_matProj;	//Projektion, Sicht
	CD3DFont*			m_pFont;							//Font-Objekt zum schreiben auf Bildschirm
	TCHAR				m_strFont[100];						//Zeichenkette fuer die Ausgabe
	DWORD				m_dwFontSize;						//Schriftgroesse
	MyGameControl*		m_pGameControl;						//Spielzustand
	MyTest*				m_pKoordSys;						//Koordinatensystem
	MyUserInput*		m_pUserInput;						//Benutzereingaben
	MyDPlay*			m_pDirectPlay;						//DirectPlay
	int					m_iDPchoice;						//Verbindungsart
	float*				m_pfFramesPerSecond;				//Anzahl Frames
	DWORD				m_dwTmpTime;						//TempZeit für Senden
	double				m_dSumTime;							//Summe Time
	int					m_iTimeCounter;
};
#endif