#pragma once
#ifndef MYLIGHT_H
#define MYLIGHT_H

#include "includes.h"

class	MyLight
{
public:
	bool	init(LPDIRECT3DDEVICE9 givenDevice);
	bool	show();

private:
	D3DLIGHT9			m_d3dLight;
	LPDIRECT3DDEVICE9	_D3DDevice;
};
#endif