#include "MyTools.h"
#include "includes.h"

MyTools::MyTools(void)
{
}

MyTools::~MyTools(void)
{
}

/*Methode zum sicheren Loeschen eines Objects*/
bool MyTools::deleteObject(void* givenObject)
{
	if(givenObject)
	{
		delete givenObject;
		givenObject = NULL;
	}
	return true;
}

/*Methode zum sicheren Loeschen eines Arrays*/
bool MyTools::deleteArray(void* givenObject)
{
	if(givenObject)
	{
		delete[] givenObject;
		givenObject = NULL;
	}
	return true;
}

/*Methode zum sicheren Freigeben einer angeforderten Schnittstelle*/
bool MyTools::releaseInterface(IUnknown* givenInterface)
{
	if(givenInterface)
	{
		givenInterface->Release();
		givenInterface = NULL;
	}
	return true;
}