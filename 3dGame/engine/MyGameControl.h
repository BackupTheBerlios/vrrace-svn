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
	int init(void);

	MyView*			m_pView;
	MyMesh*			m_pShip;

	bool			m_bShowStatus;
};
#endif