#pragma once
#ifndef MYGAMECONTROL_H
#define MYGAMECONTROL_H

#include "includes.h"

class MyGameControl
{
public:
	MyGameControl(void);
	~MyGameControl(void);
	int init(void);
//private:
	MyView* m_pView;
};
#endif