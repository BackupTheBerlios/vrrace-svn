#pragma once
#ifndef MYMESH_H
#define MYMESH_H

#include "includes.h"

class MyMesh
{
public:
	void				init(LPDIRECT3DDEVICE9 givenDevice, LPCSTR givenFileName, float posX, float posY, float posZ, float dirX, float dirY, float dirZ);
	HRESULT				load();
	void				draw();
	void				move();
	CUSTOMVERTEX*		getPosition();

private:
	LPCTSTR				m_FileName;
	DWORD				m_dwNumMaterials;
	LPD3DXMESH			m_pMesh;
	D3DMATERIAL9*		m_pMaterials;
	LPDIRECT3DTEXTURE9*	m_pTextures;
	CUSTOMVERTEX		m_Position;
	CUSTOMVERTEX		m_Direction;
	LPDIRECT3DDEVICE9	_D3DDevice;
};
#endif