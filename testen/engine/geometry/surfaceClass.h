#ifndef SURFACECLASS_H
#define SURFACECLASS_H

/*
position ist immer abhängig vom hauptobjekt
und der koordinatenursprung der fläche ist
der mittelpunkt des objektes
*/

#include <io.h>
#include <stdio.h>
//#include <iostream>
//#include <fstream>
//#include <string>
#include "pointClass.h"
#include "edgeClass.h"
#include "polyClass.h"
#include <vector>
using namespace std;

class surfaceClass
{
public:
	surfaceClass();			//Konstruktor zur initialisierung
	~surfaceClass();		//Destruktor

	short	addPolygon(
		double givenX1, double givenY1, double givenZ1,
		double givenX2, double givenY2, double givenZ2,
		double givenX3, double givenY3, double givenZ3);		//Fügt dem Objekt ein Polygon3 hinzu

	short	addPolygon(
		double givenX1, double givenY1, double givenZ1,
		double givenX2, double givenY2, double givenZ2,
		double givenX3, double givenY3, double givenZ3,
		double givenX4, double givenY4, double givenZ4);		//Fügt dem Objekt ein Polygon4 hinzu

	bool	init(pointClass* givenOne, pointClass* givenTwo, pointClass* givenThree);
	bool	init(pointClass* givenOne, pointClass* givenTwo, pointClass* givenThree, pointClass* givenFour);

private:

	vector<pointClass*>	m_points;		//unbekannte Anzahl Punkte
	vector<edgeClass*>	m_edges;		//unbekannte Anzahl Kanten
	vector<polyClass>	m_polys;		//unbekannte Anzahl Polygone
};

#endif