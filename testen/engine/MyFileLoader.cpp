#include ".\myfileloader.h"


MyFileLoader::MyFileLoader(void)
{
}

MyFileLoader::~MyFileLoader(void)
{
}

int	MyFileLoader::getMap(string givenMapName)
{
	ifstream mapFile;
	mapFile.open(givenMapName.c_str());
	if (mapFile)
	{
		return 1;
	} else {
		return 0;
	}
}