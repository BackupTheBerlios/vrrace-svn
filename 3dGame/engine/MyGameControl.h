#pragma once
#ifndef MYGAMECONTROL_H
#define MYGAMECONTROL_H

#include "includes.h"
#include "MyView.h"
#include "MyMesh.h"

class MyGameControl
{
public:
	MyGameControl(void);
	~MyGameControl(void);

	bool				init(LPDIRECT3DDEVICE9 givenDevice);
	bool				loadObjects();
	bool				drawObjects(D3DXMATRIX* givenMatWorld);
	bool				moveObjects();
	//bool	setObjectPosition(D3DXMATRIX* givenMatWorld);

	MyView*				m_pView;
	MyMesh*				m_pShip;

	bool				m_bShowStatus;

private:
	LPDIRECT3DDEVICE9	_D3DDevice;		//Handle auf das D3D-Device, global soll ja nich
	int					m_initCount;	//soll sicherstellen, dass die noetigen Funktionen vollstaendig und in richtiger Reihenfolge aufgerufen werden
};
#endif