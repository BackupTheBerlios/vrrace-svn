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
	bool	setPosition(pointClass* givenPosition);	//Position auf übergebenen Punkt setzen
	bool	setPosition(double givenX, double givenY, double givenZ); //Position auf übergebene Koordinaten setzen

	//short addSurface(surfaceClass givenSurface);	//Fügt dem Objekt eine Fläche hinzu

	short	addPoint(pointClass* givenPoint);			//Fügt Punkt hinzu, wenn er noch nicht im Objekt ist
	short	addPoint(double givenX, double givenY, double givenZ);

	short	addEdge(pointClass* givenStart, pointClass* givenEnd); //Kante hinzufügen
	short	addEdge(int givenStart, int givenEnd);

	short	addPoly(int given1, int given2, int given3);
	polyClass*	getPoly(int givenIndex);			//gibt Polygonkoordinaten zurück

	short addSurface();
	short addSecObj(masterClass* givenObject);		//Fügt ein sekundäres Objekt hinzu

	int getNumPoints();								//gibt die Menge der Punkte zurück
	int getNumEdges();								//gibt die Menge der Kanten zurück
	int getNumPolys();								//gibt die Menge der Polygone zurück
	int getNumSurfaces();
	
	double getMeshPointX(int givenIndex);			//gibt einen MeshPointwert zurück
	double getMeshPointY(int givenIndex);			//gibt einen MeshPointwert zurück
	double getMeshPointZ(int givenIndex);			//gibt einen MeshPointwert zurück

	double getMeshEstartX(int givenIndex);			//gibt einen MeshEdgeStartwert zurück
	double getMeshEstartY(int givenIndex);			//gibt einen MeshEdgeStartwert zurück
	double getMeshEstartZ(int givenIndex);			//gibt einen MeshEdgeStartwert zurück

	double getMeshEendX(int givenIndex);			//gibt einen MeshEdgeEndwert zurück
	double getMeshEendY(int givenIndex);			//gibt einen MeshEdgeEndwert zurück
	double getMeshEendZ(int givenIndex);			//gibt einen MeshEdgeEndwert zurück

	double getPosX();								//liefert X-Positon des Objekts zurück
	double getPosY();								//liefert Y-Positon des Objekts zurück
	double getPosZ();								//liefert Z-Positon des Objekts zurück

	short createWorldBall();						//Testfunktion zum Erstellen der Erdkugel

private:

	void	newMeshPoint(pointClass* givenPoint);	//fügt Punkt in den Vector ein, wenn er noch nicht drin ist
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
	vector<surfaceClass>	m_Surfaces;				//Liste aller Flächen

	vector<masterClass*>*	m_SecondaryObjects;		//Liste aller Unterobjekte

	vector<surfaceClass*>*	m_VisSurfaces;			//Liste mit sichtbaren Flächen
	vector<masterClass*>*	m_VisSecObjs;			//Liste aller sichtbaren Objekte





//private:

};

#endif