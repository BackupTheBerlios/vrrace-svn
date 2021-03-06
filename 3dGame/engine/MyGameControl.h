#pragma once
#ifndef MYGAMECONTROL_H
#define MYGAMECONTROL_H

#include "includes.h"
#include "MyView.h"
#include "MyMesh.h"
#include "MyLight.h"
#include "MyVertex.h"
#include "MyPlayer.h"
#include "MyStarsField.h"
#include "MyDPlay.h"
#include "MyMusic.h"
#include "MyDSound.h"

static const LPCTSTR	MUSICFILENAME	= "resources/wav_files/Music.mp3";

class MyGameControl
{
public:
	MyGameControl(void);
	~MyGameControl(void);

	bool				init(LPDIRECT3DDEVICE9 givenDevice, D3DXMATRIX* givenMatWorld, HWND* givenHWnd, MyDPlay* givenDPlay, int choice, int shipChoice);
	bool				initObjects();
	bool				initStarsField();
	bool				initMusic();
	bool				initDirectSound();
	bool				startSound();
	bool				loadObjects();
	bool				drawObjects();
	bool				drawStarsField();
	bool				drawLights();
	bool				presentMusic();
	void				moveObjects();
	bool				buildGame();
	bool				addLight();
	bool				addPlayer(string* givenName);
	MyPlayer*			getPlayer();
	int					getNumLights();
	int					getNumMeshes();
	bool				sendData();
	MyDPlay*			getDirectPlay();
	//bool	setObjectPosition(D3DXMATRIX* givenMatWorld);

	MyView*				m_pMainCam;
	bool				m_bShowStatus;
	void				sendPlayer(float givenX, float givenY, float givenZ, int status);
	bool				collision(MyMesh* givenObject1, MyMesh* givenObject2);
	int					ff_g;//gravi
	bool				ff_f;//vorn
	bool				ff_r;//rechts
	bool				ff_b;//hinten
	bool				ff_l;//links
	bool				ff_e;//explosion

	MyDPlay*			_DirectPlay;	//DirectPlay

private:
	LPDIRECT3DDEVICE9	_D3DDevice;			//Handle auf das D3D-Device, global soll ja nich
	MyDSound*			m_pDirectSound;		//PrimaryBuffer DirectSound
	MySound*			m_pCollectSound;	//Sammelsound
	MySound*			m_pCollisionSound;	//Kollisionssound
	MyMusic*			m_pMusic;			//Music
	HWND*				m_hWnd;
	int					m_iDPchoice;		//Verbindungsart
	int					m_iShipChoice;		//Schiffwahl
	int					m_initCount;		//soll sicherstellen, dass die noetigen Funktionen vollstaendig und in richtiger Reihenfolge aufgerufen werden
	MyMesh*				m_pShip;
	MyStarsField*		m_pStarsField;
//	vector<MyMesh*>		m_pMeshes;
//	vector<MyLight*>	m_pLights;
	D3DXMATRIX*			_matWorld;
//	vector<MyMesh*>		m_pAllMeshes;		//Alle Meshes
	vector<MyLight*>	m_pAllLights;		//Alle Lichter
	vector<MyMasterPosition*>	m_pMasterObjects;	//Alle Objekte die kein Master haben
	vector<MyMasterPosition*>	m_pDrawObjects;		//Alle zu zeichnenden Objekte
};
#endif