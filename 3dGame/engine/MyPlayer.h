#pragma once
#ifndef MYPLAYER_H
#define MYPLAYER_H

#include "includes.h"
#include "MyMesh.h"
#include "MyView.h"

class MyPlayer
{
public:
	MyPlayer(void);
	~MyPlayer(void);
	void	setMesh(MyMesh* givenMesh);
	MyMesh*	getMesh();
	DPNID*	getID();
	void	setCamera(MyView* givenCamera);
	MyView*	getCamera();

	string*	m_Name;	//Spielername
	MyMesh*	m_pPlayerMesh;
	DPNID	m_pPlayerID;
/*	int*	m_pEnergy;
	int*	m_pObjectID;	//Objektidentifikator -> welche Textur/Mesh/Objekttyp
	int*	m_pStatus;		//zerstört, unsichbar, initialisiert, ....
	int*	m_pVectorID;*/
	MyView*	m_pCamera;
};
#endif