#pragma once
#ifndef MYMESH_H
#define MYMESH_H

#include "includes.h"

class MyMesh
{
public:
	bool	init();
	HRESULT	load(LPDIRECT3DDEVICE9 givenDevice);
	void	draw(LPDIRECT3DDEVICE9 givenDevice);

private:
	LPCTSTR				m_FileName;
	DWORD				m_dwNumMaterials;
	LPD3DXMESH			m_pMesh;
	D3DMATERIAL9*		m_pMaterials;
	LPDIRECT3DTEXTURE9*	m_pTextures;
	CUSTOMVERTEX		m_Position;
};
#endif