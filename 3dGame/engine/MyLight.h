#pragma once
#ifndef MYLIGHT_H
#define MYLIGHT_H

#include "includes.h"
#include "MyVertex.h"
#include "MyMasterPosition.h"

class	MyLight : public MyMasterPosition
{
public:
	bool	init(LPDIRECT3DDEVICE9 givenDevice,
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
								float givenRange);
	void	setMaterialValues(float rDiffuse, float gDiffuse, float bDiffuse, float aDiffuse,
									float rAmbient, float gAmbient, float bAmbient,
									float rSpecular, float gSpecular, float bSpecular);
	void	show();

private:
	D3DXMATRIX*			_matWorld;
	D3DLIGHT9			m_d3dLight;
	LPDIRECT3DDEVICE9	_D3DDevice;
	float				m_fRange;
};
#endif