#pragma once
#ifndef MYSTARSFIELD_H
#define MYSTARSFIELD_H

#include "includes.h"
#include "MyVertex.h"
#include "MyMasterPosition.h"

class MyStarsField : public MyMasterPosition
{
public:
	MyStarsField(void);
	~MyStarsField(void);
	bool init(LPDIRECT3DDEVICE9 givenDevice,
								D3DXMATRIX* givenMatWorld,
								float posX,
								float posY,
								float posZ,
								float dirX,
								float dirY,
								float dirZ,
								float rotX,
								float rotY,
								float rotZ,
								float rotDirX,
								float rotDirY,
								float rotDirZ,
								int numberStars);
	bool draw(void);
private:
	D3DXMATRIX*			_matWorld;
	CUSTOMVERTEX*		m_pStars;
	int					m_iNumberStars;
};

#endif