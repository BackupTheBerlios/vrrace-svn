#pragma once
#ifndef MYVERTEX_H
#define MYVERTEX_H

#include "includes.h"

class MyVertex
{
public:
	MyVertex(void);
	~MyVertex(void);

	void	setValues(float givenX, float givenY, float givenZ);		//zum Setzen der Werte
	void	setPValues(float* givenX, float* givenY, float* givenZ);	//zum Setzen auf Zeigern
	
	void	setX(float givenX);											//X auf uebergebenen Wert setzen
	void	setY(float givenY);											//Y auf uebergebenen Wert setzen
	void	setZ(float givenZ);											//Z auf uebergebenen Wert setzen

	void	setPX(float* givenX);										//X deleten und neuen Pointer zuweisen
	void	setPY(float* givenY);										//Y deleten und neuen Pointer zuweisen
	void	setPZ(float* givenZ);										//Z deleten und neuen Pointer zuweisen

	float	getX();														//Inhalt von X zurueckgeben
	float	getY();														//Inhalt von Y zurueckgeben
	float	getZ();														//Inhalt von Z zurueckgeben

	float*	getPX();													//Zeiger auf X zurueckgeben
	float*	getPY();													//Zeiger auf Y zurueckgeben
	float*	getPZ();													//Zeiger auf Z zurueckgeben

	void	addX(float givenX);											//uebergebenen Wert auf inhalt von X addieren
	void	addY(float givenY);											//uebergebenen Wert auf inhalt von Y addieren
	void	addZ(float givenZ);											//uebergebenen Wert auf inhalt von Z addieren

	void	initX();													//X neu anlegen
	void	initY();													//Y neu anlegen
	void	initZ();													//Z neu anlegen

private:
	float*	x;
	float*	y;
	float*	z;
};
#endif