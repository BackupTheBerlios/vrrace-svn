#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H
#define DIRECTINPUT_VERSION 0x800
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE)
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <dinput.h>

struct CUSTOMVERTEX
{
	float	x, y, z;
	DWORD	color;
};


//HINSTANCE	g_hInst;
//HWND		g_hWnd;

#endif