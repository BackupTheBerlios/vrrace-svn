#pragma once
#ifndef MYMESH_H
#define MYMESH_H

#include "includes.h"
#include "MyVertex.h"
#include "MyMasterPosition.h"

class MyMesh
{
public:
	MyMesh(void);
	~MyMesh(void);
	bool				init(LPDIRECT3DDEVICE9 givenDevice,
								D3DXMATRIX* givenMatWorld,
								LPCTSTR givenFileName,
								float posX,
								float posY,
								float posZ,
								float dirX,
								float dirY,
								float dirZ,
								float rotX,
								float rotY,
								float rotZ,
								float rotDirX,
								float rotDirY,
								float rotDirZ);
	HRESULT				load();
	void				draw();
	void				move();
	MyMasterPosition*	getPosition();
	MyMasterPosition*	getReference();
	void				setReference(MyMasterPosition* givenReference);
	
private:
	LPCTSTR				m_FileName;
	DWORD				m_dwNumMaterials;
	LPD3DXMESH			m_pMesh;
	D3DMATERIAL9*		m_pMaterials;	//Materialbuffer
	LPDIRECT3DTEXTURE9*	m_pTextures;	//Texturbuffer
	MyMasterPosition*	m_pPosition;
	MyMasterPosition*	m_pReferencePoint;
	LPDIRECT3DDEVICE9	_D3DDevice;
	D3DXMATRIX*			_matWorld;
};
#endif