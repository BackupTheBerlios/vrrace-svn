#pragma once
#ifndef TOOLS_H
#define TOOLS_H

#include "includes.h"

class MyTools
{
public:
	MyTools(void);
	~MyTools(void);
	static bool deleteObject(void* givenObject);
	static bool deleteArray(void* givenObject);
	static bool releaseInterface(IUnknown* givenInterface);
};
#endif