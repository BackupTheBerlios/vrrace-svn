//#pragma once
#ifndef INCLUDES_H
#define INCLUDES_H

//Systemincludes
#include <windows.h>
#include <io.h>
#include <dinput.h>
#include <dsound.h>
#include <dshow.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <dplay8.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <commctrl.h>

//Namensraum
using namespace std;

//Directx-Includes
#include "../directx/d3dutil.h"
#include "../directx/dxutil.h"
#include "../directx/d3dfont.h"

//eigene Includes
/*
#include "MyMesh.h"
#include "MyTest.h"
#include "MyView.h"
#include "MyGameControl.h"
#include "MyUserInput.h"
#include "MyD3DGame.h"
#include "MyMesh.h"
*/

#include "MyTools.h"

//Bibliotheken
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr9.lib")
#pragma comment(lib, "d3dx9dt.lib")
#pragma comment(lib, "dplayx.lib")
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")

//Konstanten
const LPCSTR	MM_WINDOW_CLASS_NAME	= "vrrace_main";	//Name des Programmfensters
const LPCSTR	MM_WINDOW_DESCRIPTION	= "VRRACE";			//Beschriftung des Fensters

const BYTE		MM_BGCOLOR_RED			= 2;				//Hintergrund Rot
const BYTE		MM_BGCOLOR_GREEN		= 2;				//Hintergrund Gruen
const BYTE		MM_BGCOLOR_BLUE			= 2;				//Hintergrund Blau

const BYTE		MM_TEXTCOLOR_RED		= 255;				//Textfarbe Rot
const BYTE		MM_TEXTCOLOR_GREEN		= 255;				//Textfarbe Gruen
const BYTE		MM_TEXTCOLOR_BLUE		= 255;				//Textfarbe Blau
const BYTE		MM_TEXTCOLOR_ALPHA		= 255;				//Textfarbe Alpha

//Defines
#define	SAMPLE_BUFFER_SIZE	16

typedef struct CUSTOMVERTEX
{
    float	x;
	float	y;
	float	z;
	DWORD	dwColor;
}CUSTOMVERTEX;

#endif