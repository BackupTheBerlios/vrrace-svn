#pragma once
#ifndef MYMASTERPOSITION_H
#define MYMASTERPOSITION_H

#include "myvertex.h"

class MyMasterPosition
{
public:
	MyMasterPosition(void);
	~MyMasterPosition(void);

	MyVertex*			m_pPosition;	//aktuelle Position
	MyVertex*			m_pDirection;	//Bewegungsrichtung
	MyVertex*			m_pRotation;	//aktuelle Rotation/StellWinkel
	MyVertex*			m_pRotDir;		//Rotationsrichtung
};
#endif