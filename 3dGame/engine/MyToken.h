#pragma once
#ifndef MYTOKEN_H
#define MYTOKEN_H

#include "includes.h"
#include "MyVertex.h"

typedef struct GAMEOBJECTS
{
	DWORD		vectorId;
	MYPOS		posinfo;
	bool		scale;
}MYGAMEOBJECTS;

typedef struct PLAYEROBJECTS
{
	DPNID		dpnid;
	int			status;
	GAMEOBJECTS	position;
	D3DXMATRIX	matrix;
}PLAYEROBJECTS;

typedef struct INFO
{
	int			framerate;
}INFO;

#endif