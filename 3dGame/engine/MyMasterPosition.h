#pragma once
#ifndef MYMASTERPOSITION_H
#define MYMASTERPOSITION_H

#include "myvertex.h"

class MyMasterPosition
{
public:
	MyMasterPosition(void);
	~MyMasterPosition(void);

	void						move();
	void						draw();
	void						calcMatrix(D3DXMATRIX* givenMatrix);		//Masterobjekt aktiviert die Berechnung
	void						activateScaling();							//Scalierung aktivieren
	void						deactivateScaling();						//Scalierung deaktivieren
	void						transform();								//End-Transformation druchfuehren
	void						calcClients();								//alle Clients die eigene Matrix senden
	void						setMaster(MyMasterPosition* givenMaster);
	void						addClient(MyMasterPosition* givenClient);
	MyVertex*					getScale();

	MyVertex*					m_pPosition;								//aktuelle Position
	MyVertex*					m_pAbsolutePosition;						//absolute Position
	MyVertex*					m_pDirection;								//Bewegungsrichtung
	float*						m_pSpeed;									//Bewegungsgeschwindigkeit
	MyVertex*					m_pRotation;								//aktuelle Rotation/StellWinkel
	MyVertex*					m_pLocalRotation;							//lokale Rotation
	MyVertex*					m_pRotDir;									//Rotationsrichtung
	MyVertex*					m_pScaleFactor;								//Skalierungsfaktor
	LPDIRECT3DDEVICE9			_D3DDevice;
	int							m_pType;
	bool						m_bControl;

	MyVertex*					getAbsolutePosition();
	void						rotate(float givenX, float givenY, float givenZ);
	D3DXMATRIX*					getRotationMatrix();
	D3DXMATRIX*					getPositionMatrix();						//Abfragen der Matrix für die aktuelle Position
	void						setPositionMatrix(D3DXMATRIX* givenMatrix);
	void						setScale(CUSTOMVERTEX givenScale);
	bool						m_bToSend;
	bool						m_bDestroyable;								//kann Objekt durch kollision zerstört werden

private:
	void						calcOwnMatrix();							//eigene Matrix berechnen

	//D3DXMATRIX*					m_pScaleMatrix;								//Skalierung
	D3DXMATRIX*					m_pTranslationMatrix;						//Translierung
	D3DXMATRIX*					m_pRotationMatrix;							//Rotation
	D3DXMATRIX*					m_pMatrix;									//lokale Transformation
	D3DXMATRIX*					m_pFinalMatrix;								//finale Transformation
	D3DXMATRIX*					m_pFinalTransRotMatrix;

	vector<MyMasterPosition*>	m_pClients;
	MyMasterPosition*			m_pMaster;

	bool						m_bScale;	

};
#endif