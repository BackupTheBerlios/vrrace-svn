#pragma once
#ifndef MYMESH_H
#define MYMESH_H

#include "includes.h"
#include "MyVertex.h"

class MyMesh
{
public:
	MyMesh(void);
	bool				init(LPDIRECT3DDEVICE9 givenDevice, LPCTSTR givenFileName, float posX, float posY, float posZ, float dirX, float dirY, float dirZ);
	HRESULT				load();
	void				draw();
	void				move();
	MyVertex*			getPosition();
	MyVertex*			getDirection();

private:
	LPCTSTR				m_FileName;
	DWORD				m_dwNumMaterials;
	LPD3DXMESH			m_pMesh;
	D3DMATERIAL9*		m_pMaterials;
	LPDIRECT3DTEXTURE9*	m_pTextures;
	MyVertex*			m_pPosition;
	MyVertex*			m_pDirection;
	LPDIRECT3DDEVICE9	_D3DDevice;
};
#endif