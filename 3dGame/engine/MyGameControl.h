#pragma once
#ifndef MYGAMECONTROL_H
#define MYGAMECONTROL_H

#include "includes.h"
#include "MyView.h"
#include "MyMesh.h"
#include "MyLight.h"
#include "MyVertex.h"
#include "MyPlayer.h"
//#include "MyStarsField.h"

class MyGameControl
{
public:
	MyGameControl(void);
	~MyGameControl(void);

	bool				init(LPDIRECT3DDEVICE9 givenDevice, D3DXMATRIX* givenMatWorld);
	bool				initObjects();
	bool				initStarsField();
	bool				loadObjects();
	bool				drawObjects();
	bool				drawStarsField();
	bool				drawLights();
	bool				moveObjects();
	bool				buildGame();
	bool				addLight();
	bool				addPlayer(string* givenName);
	MyPlayer*			getPlayer();
	int					getNumLights();
	int					getNumMeshes();
	//bool	setObjectPosition(D3DXMATRIX* givenMatWorld);

	MyView*				m_pView;
	bool				m_bShowStatus;

private:
	LPDIRECT3DDEVICE9	_D3DDevice;		//Handle auf das D3D-Device, global soll ja nich
	int					m_initCount;	//soll sicherstellen, dass die noetigen Funktionen vollstaendig und in richtiger Reihenfolge aufgerufen werden
	MyMesh*				m_pShip;
//	MyStarsField*		m_pStarsField;
//	vector<MyMesh*>		m_pMeshes;
//	vector<MyLight*>	m_pLights;
	D3DXMATRIX*			_matWorld;
	MyPlayer*			m_pLocalPlayer;		//eigener Spieler
	vector<MyPlayer*>	m_pNetworkPlayers;	//andere Teilnehmer
	vector<MyMesh*>		m_pAllMeshes;		//Alle Meshes
	vector<MyMesh*>		m_pMasterMeshes;	//Alle ohne Master
	vector<MyLight*>	m_pAllLights;		//Alle Lichter
	vector<MyMasterPosition*>	m_pMasterObjects;	//Alle Objekte die kein Master haben
	vector<MyMasterPosition*>	m_pDrawObjects;		//Alle zu zeichnenden Objekte
};
#endif