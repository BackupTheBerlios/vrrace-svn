#pragma once
#ifndef MYVIEW_H
#define MYVIEW_H

#include "includes.h"
#include "MyVertex.h"


class MyView
{
public:
	MyView(void);
	~MyView(void);

	MyVertex*	getPos();
	MyVertex*	getVP();
	MyVertex*	getUV();

private:

	MyVertex*	m_pPosition;
	MyVertex*	m_pViewPoint;
	MyVertex*	m_pUpVector;		//zeigt nach oben
};
#endif