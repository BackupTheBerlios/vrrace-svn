#pragma once
#ifndef SCENE_H
#define SCENE_H
#include "../globals.h"
#include <Windows.h>
#include <mmsystem.h>
#include <io.h>
#include <stdio.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <d3d9caps.h>
#include <dinput.h>
#include "../directx/d3dutil.h"
#include "../directx/d3dfont.h"
#include "../directx/dxutil.h"
//#include "../directx/d3denumeration.h"


#include "light.h"
#include "MyMesh.h"
#include "geometry/masterClass.h"

class scene
{
public:
	scene(HINSTANCE* hInst, HWND* hWnd);
	~scene(void);

	bool	init3D();						//Initialisierung von d3d
	void	sdraw();
	void	edraw();
	void	draw(masterClass* givenObject);							//Zeichnen der Objekte

	pointClass				m_EyePosition;
	pointClass				m_LookAt;

	int	r, g, b;
	//pointClass
	LPDIRECT3DDEVICE9		m_d3dDevice;	//Device zum Rendern

	void					setupMatrices();
	void					drawMesh(MyMesh* givenMesh);
	void					initMesh(MyMesh* givenMesh);
private:
	LPDIRECT3D9				m_d3d;			//zum Erstellen des 3d-Devices gedacht
	LPDIRECT3DVERTEXBUFFER9	m_pVB;			//vertexbuffer
	HINSTANCE*				m_hInst;
	HWND*					m_hWnd;

	D3DXMATRIX				m_matWorld;
	D3DXMATRIX				m_matView;
	D3DXMATRIX				m_matProj;
	D3DXVECTOR3				m_vecDir;
	D3DLIGHT9				m_light;
	D3DMATERIAL9			m_mtrl;
//	CD3DFont*				m_pFont;
};
#endif