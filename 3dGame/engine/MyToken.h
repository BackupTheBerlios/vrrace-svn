#pragma once
#ifndef MYTOKEN_H
#define MYTOKEN_H

#include "includes.h"
#include "MyVertex.h"

class MyToken
{
public:
	char*		name;
	DPNID		dpnid;
	int			oid;
	int			status;
	DWORD		vectorId;
	D3DXMATRIX positionMatrix;
	//MyVertex	scaleFactor;
	CUSTOMVERTEX scaleFactor;
};
#endif