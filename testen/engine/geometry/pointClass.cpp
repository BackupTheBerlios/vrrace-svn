#define STRICT
#include "pointClass.h"
//#include "..\globals.h"


pointClass::pointClass()
{
	x	= 0.0f;
	y	= 0.0f;
	z	= 0.0f;
}

void	pointClass::rotate(double givenAngle, pointClass* givenRotPoint)
{

}

void	pointClass::translate(pointClass* givenPoint)
{
	x += givenPoint->x;
	y += givenPoint->y;
	z += givenPoint->z;
}

pointClass::~pointClass()
{
}

void	pointClass::set(double givenX, double givenY, double givenZ)
{
	x	= givenX;
	y	= givenY;
	z	= givenZ;
}