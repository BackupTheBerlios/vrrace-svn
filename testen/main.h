#ifndef MAIN_H
#define MAIN_H
#define WIN32_LEAN_AND_MEAN

#include "globals.h"

#include <Windows.h>
#include <winuser.h>
#include <io.h>
#include <stdio.h>
#include <d3dx9.h>
#include <dinput.h>


#include "directx/d3dutil.h"
#include "directx/d3dfont.h"
#include "directx/dxutil.h"
#include "directx/dsutil.h"

#include "engine/scene.h"
#include "engine/userInput.h"
#include "engine/MyFileLoader.h"
#include "engine/MyMesh.h"

#include "engine/geometry/masterClass.h"
#include "engine/geometry/edgeClass.h"


#define VERTEXBUFFER	LPDIRECT3DVERTEXBUFFER9
#define INDEXBUFFER		LPDIRECT3DINDEXBUFFER9
#define TEXTURE			LPDIRECT3DTEXTURE9
#define MATERIAL		D3DMATERIAL9
#define LIGHT			D3DLIGHT9

scene*					d3dscene	= NULL;
userInput*				inputKbJ	= NULL;
float					oldTime		= 0.0f;

vector<masterClass*>	m_Objects;

HWND					hWnd;
HDC						hDc;

int						cursorY;
MyMesh*					tiger;


#endif MAIN_H