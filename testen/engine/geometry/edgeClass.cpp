#define STRICT
#include "edgeClass.h"

/*edgeClass::edgeClass()
{
}*/

bool edgeClass::init(pointClass* givenStart, pointClass* givenEnd)
{
	m_Start	= givenStart;
	m_End	= givenEnd;
	return true;
}

edgeClass::~edgeClass()
{
	delete	m_Start;
	delete	m_End;

	m_Start	= NULL;
	m_End	= NULL;
}