#pragma once
#ifndef MYFILELOADER_H
#define MYFILELOADER_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

class MyFileLoader
{
public:
	MyFileLoader(void);
	~MyFileLoader(void);

	int	getMap(string givenMapName);		//lädt die angegebene Map
	int	getObjects(char* givenMapName);	//lädt die angegebenen MapObjekte
	int getPlayer(char* givenMapName);	//lädt den angegebenen Player
};
#endif