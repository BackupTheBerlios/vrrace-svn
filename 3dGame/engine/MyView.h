#pragma once
#ifndef MYVIEW_H
#define MYVIEW_H

#include "includes.h"


class MyView
	/*
	Klasse fuer die Cameraposition
	*/
{
public:
	MyView(void);
	~MyView(void);
	
	CUSTOMVERTEX	m_Position;		//Betrachter-Position
	CUSTOMVERTEX	m_ViewPoint;	//Fluchtpunkt
	CUSTOMVERTEX	m_UpVector;		//zeigt nach oben
};
#endif