#include "MyVertex.h"

MyVertex::MyVertex(void)
{
	this->initX();
	this->initY();
	this->initZ();
}

void	MyVertex::setValues(float givenX, float givenY, float givenZ)
{
	this->setX(givenX);
	this->setY(givenY);
	this->setZ(givenZ);
}

void	MyVertex::setPValues(float* givenX, float* givenY, float* givenZ)
{
	this->setPX(givenX);
	this->setPY(givenY);
	this->setPZ(givenZ);
}

void	MyVertex::setX(float givenX)	{*x	= givenX;}
void	MyVertex::setY(float givenY)	{*y	= givenY;}
void	MyVertex::setZ(float givenZ)	{*z	= givenZ;}

void	MyVertex::setPX(float* givenX)	{delete x;	x	= givenX;}
void	MyVertex::setPY(float* givenY)	{delete y;	y	= givenY;}
void	MyVertex::setPZ(float* givenZ)	{delete z;	z	= givenZ;}

float	MyVertex::getX()				{return *x;}
float	MyVertex::getY()				{return *y;}
float	MyVertex::getZ()				{return *z;}

float*	MyVertex::getPX()				{return x;}
float*	MyVertex::getPY()				{return y;}
float*	MyVertex::getPZ()				{return z;}

void	MyVertex::addX(float givenX)	{*x	+= givenX;}
void	MyVertex::addY(float givenY)	{*y	+= givenY;}
void	MyVertex::addZ(float givenZ)	{*z	+= givenZ;}

void	MyVertex::initX()				{x	= new float(0.0);}
void	MyVertex::initY()				{y	= new float(0.0);}
void	MyVertex::initZ()				{z	= new float(0.0);}

MyVertex::~MyVertex(void)
{
	delete x;
	delete y;
	delete z;
}