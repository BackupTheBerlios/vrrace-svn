#pragma once
#ifndef MYVIEW_H
#define MYVIEW_H

#include "includes.h"
#include "MyVertex.h"
#include "MyMasterPosition.h"


class MyView
{
public:
	MyView(void);
	~MyView(void);

	MyVertex*			getPos();
	MyVertex*			getLP();
	MyVertex*			getVP();
	MyVertex*			getUV();
	void				setMaster(MyMasterPosition* givenMaster);
	MyMasterPosition*	getMaster();
	void				move();
	void				rotate();

private:

	MyVertex*			m_pPosition;		//absolute Position
	MyVertex*			m_pViewPoint;		//zeiger auf Fluchtpunkt
	MyVertex*			m_pUpVector;		//zeigt nach oben
	MyVertex*			m_pLocalPos;		//abstand vom Master
	MyMasterPosition*	m_pMaster;
};
#endif