#ifndef POINTCLASS_H
#define POINTCLASS_H

#include <math.h>
//using namespace std;



class pointClass
{
public:
	pointClass();	//Konstruktor zur initialisierung
	~pointClass();	//Destruktor

	void	set(double givenX, double givenY, double givenZ);

	void	rotate(double givenAngle, pointClass* givenRotPoint);
	void	translate(pointClass* givenTransPoint);

	float	x;		//Position auf der X-Achse
	float	y;		//Position auf der Y-Achse
	float	z;		//Position auf der Z-Achse

};

#endif