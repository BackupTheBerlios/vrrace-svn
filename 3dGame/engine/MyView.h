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
{
public:
	MyView(void);
	~MyView(void);


	// Sicht-Position
	PositionPoint m_Position;
	// Sicht-Richtung
	PositionPoint m_ViewPoint;
	// Oben
	PositionPoint m_UpVector;


};
#endif