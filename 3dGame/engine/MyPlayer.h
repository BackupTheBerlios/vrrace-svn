#pragma once
#ifndef MYPLAYER_H
#define MYPLAYER_H

#include "includes.h"
#include "MyMesh.h"
#include "MyView.h"
#include "MySound.h"

class MyPlayer
{
public:
	MyPlayer(void);
	~MyPlayer(void);
	void		setMesh(MyMesh* givenMesh);
	MyMesh*		getMesh();
	MySound*	getSound();
	DPNID*		getID();
	void		setCamera(MyView* givenCamera);
	MyView*		getCamera();

	string*		m_Name;	//Spielername
	MyMesh*		m_pPlayerMesh;
	MySound*	m_pPlayerSound;
	DPNID		m_pPlayerID;
	int			m_pShipChoice;
/*	int*		m_pEnergy;
	int*		m_pObjectID;	//Objektidentifikator -> welche Textur/Mesh/Objekttyp
	int*		m_pStatus;		//zerstört, unsichbar, initialisiert, ....
	int*		m_pVectorID;*/
	MyView*		m_pCamera;
};
#endif