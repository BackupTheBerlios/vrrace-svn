#ifndef POLYCLASS_H
#define POLYCLASS_H

/*
position ist immer abhängig vom hauptobjekt
und der koordinatenursprung der fläche ist
der mittelpunkt des objektes
*/

#include <io.h>
#include <stdio.h>
#include "pointClass.h"
#include "edgeClass.h"
#include <vector>
using namespace std;

class polyClass
{
public:
//	polyClass();			//Konstruktor zur initialisierung
//	~polyClass();		//Destruktor

	bool init(pointClass* givenOne, pointClass* givenTwo, pointClass* givenThree);
	bool init(pointClass* givenOne, pointClass* givenTwo, pointClass* givenThree, pointClass* givenFour);	//definiert die Flaeche durch gegebene Punkte
//	CUSTOMVERTEX getVertex();


	pointClass*	p1;
	pointClass* p2;
	pointClass* p3;
//	vector<pointClass*>	m_points;		//unbekanntwe Anzahl Punkte
};

#endif