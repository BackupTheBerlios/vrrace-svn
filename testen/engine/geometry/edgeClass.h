#ifndef EDGECLASS_H
#define EDGECLASS_H

/*
position ist immer abh�ngig vom hauptobjekt
und der koordinatenursprung der fl�che ist
der mittelpunkt des objektes
*/

#include <io.h>
#include <stdio.h>
//#include <iostream>
//#include <fstream>
//#include <string>
#include "pointClass.h"
//using namespace std;

class edgeClass
{
public:
//	edgeClass();			//Konstruktor zur initialisierung
	~edgeClass();		//Destruktor

	bool init(pointClass* givenStart, pointClass* givenEnd);
	bool move(pointClass* givenStartMove, pointClass* givenEndMove);

//public:
//	protected:
//private:
	pointClass* m_Start;	//erster Punkt
	pointClass* m_End;		//letzter Punkt
};

#endif