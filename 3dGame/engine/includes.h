#pragma once
#ifndef INCLUDES_H
#define INCLUDES_H

//Systemincludes
#include <windows.h>
#include <d3dx9.h>
#include <stdio.h>
#include <string>
using namespace std;

//Directx-Includes
#include "../directx/d3dutil.h"
#include "../directx/dxutil.h"
#include "../directx/d3dfont.h"

//eigene Includes
#include "MyView.h"
#include "MyGameControl.h"
#include "MyD3DGame.h"

//Bibliotheken
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr9.lib")
#pragma comment(lib, "d3dx9dt.lib")
#pragma comment(lib, "winmm.lib")

//Konstanten
const LPCSTR MM_WINDOW_CLASS_NAME	= "vrrace_main";	//Name des Programmfensters
const LPCSTR MM_WINDOW_DESCRIPTION	= "VRRACE";			//Beschriftung des Fensters

#endif