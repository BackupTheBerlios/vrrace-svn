#pragma once
#ifndef MASTERCLASS_H
#define MASTERCLASS_H

#include <vector>
#include <string>
#include "pointClass.h"
#include "edgeClass.h"
#include "surfaceClass.h"
using namespace std;

class masterClass
{
public:
	masterClass();									//Konstruktor zur initialisierung
	~masterClass();									//Destruktor

	bool	initObject();								//zur Initialisierung

	bool	setPosition();								//Position auf 0,0,0 setzen
	bool	setPosition(pointClass* givenPosition);	//Position auf �bergebenen Punkt setzen
	bool	setPosition(double givenX, double givenY, double givenZ); //Position auf �bergebene Koordinaten setzen

	//short addSurface(surfaceClass givenSurface);	//F�gt dem Objekt eine Fl�che hinzu

	short	addPoint(pointClass* givenPoint);			//F�gt Punkt hinzu, wenn er noch nicht im Objekt ist
	short	addPoint(double givenX, double givenY, double givenZ);

	short	addEdge(pointClass* givenStart, pointClass* givenEnd); //Kante hinzuf�gen
	short	addEdge(int givenStart, int givenEnd);

	short	addPoly(int given1, int given2, int given3);
	polyClass*	getPoly(int givenIndex);			//gibt Polygonkoordinaten zur�ck

	short addSurface();
	short addSecObj(masterClass* givenObject);		//F�gt ein sekund�res Objekt hinzu

	int getNumPoints();								//gibt die Menge der Punkte zur�ck
	int getNumEdges();								//gibt die Menge der Kanten zur�ck
	int getNumPolys();								//gibt die Menge der Polygone zur�ck
	int getNumSurfaces();
	
	double getMeshPointX(int givenIndex);			//gibt einen MeshPointwert zur�ck
	double getMeshPointY(int givenIndex);			//gibt einen MeshPointwert zur�ck
	double getMeshPointZ(int givenIndex);			//gibt einen MeshPointwert zur�ck

	double getMeshEstartX(int givenIndex);			//gibt einen MeshEdgeStartwert zur�ck
	double getMeshEstartY(int givenIndex);			//gibt einen MeshEdgeStartwert zur�ck
	double getMeshEstartZ(int givenIndex);			//gibt einen MeshEdgeStartwert zur�ck

	double getMeshEendX(int givenIndex);			//gibt einen MeshEdgeEndwert zur�ck
	double getMeshEendY(int givenIndex);			//gibt einen MeshEdgeEndwert zur�ck
	double getMeshEendZ(int givenIndex);			//gibt einen MeshEdgeEndwert zur�ck

	double getPosX();								//liefert X-Positon des Objekts zur�ck
	double getPosY();								//liefert Y-Positon des Objekts zur�ck
	double getPosZ();								//liefert Z-Positon des Objekts zur�ck

	short createWorldBall();						//Testfunktion zum Erstellen der Erdkugel

private:

	void	newMeshPoint(pointClass* givenPoint);	//f�gt Punkt in den Vector ein, wenn er noch nicht drin ist
	void	newEdge(edgeClass* givenEdge);//pointClass* givenPoint1, pointClass* givenPoint2);

private:
	string					m_sName;				//Name des Objektes
//	short					m_nType;				//Typ des Objektes
//	long					m_lNumber;				//Objektnummer

	pointClass				m_xPosition;			//Position des Objektes
	pointClass*				m_xViewPoint;			//Blickrichtung des Objektes
	pointClass*				m_xDirection;			//Bewegungsrichtung des Objektes
	pointClass				m_xRotation;			//Rotation des Objektes

	vector<pointClass*>		m_MeshPoints;			//Liste aller Objektpunkte
	vector<edgeClass*>		m_MeshEdges;			//Liste aller Kanten
	vector<polyClass*>		m_Polygons;				//Liste aller Polygone
	vector<surfaceClass>	m_Surfaces;				//Liste aller Fl�chen

	vector<masterClass*>*	m_SecondaryObjects;		//Liste aller Unterobjekte

	vector<surfaceClass*>*	m_VisSurfaces;			//Liste mit sichtbaren Fl�chen
	vector<masterClass*>*	m_VisSecObjs;			//Liste aller sichtbaren Objekte





//private:

};

#endif