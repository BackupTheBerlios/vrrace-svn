#pragma once
#ifndef MYVIEW_H
#define MYVIEW_H

#include "includes.h"

typedef struct PositionPoint
{
	float	x;
	float	y;
	float	z;
}PositionPoint;

class MyView
	/*
	Klasse fuer die Cameraposition
	*/
{
public:
	MyView(void);
	~MyView(void);
	
	PositionPoint m_Position;	//Betrachter-Position
	PositionPoint m_ViewPoint;	//Fluchtpunkt
	PositionPoint m_UpVector;	//zeigt nach oben
};
#endif