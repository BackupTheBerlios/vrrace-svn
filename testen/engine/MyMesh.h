#pragma once
#ifndef MYMESH_H
#define MYMESH_H
#include "../globals.h"
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <dinput.h>
#include "../directx/d3dutil.h"
#include "../directx/d3dfont.h"
#include "../directx/dxutil.h"

//#include "scene.h"

class MyMesh
{
public:
	MyMesh(void);
	~MyMesh(void);

	void	init(LPDIRECT3DDEVICE9 givenDevice);
	HRESULT	InitGeometry();
	void	draw();

//private:
       
    LPD3DXMESH         m_pMesh;          

    D3DMATERIAL9*       m_pMeshMaterials;
    LPDIRECT3DTEXTURE9* m_pMeshTextures;        // Array of textures, entries are NULL if no texture specified
    DWORD               m_dwNumMaterials;       // Number of materials
	BOOL                m_bShowOptimized;

//	LPDIRECT3DDEVICE9	m_pd3dDevice;
};
#endif