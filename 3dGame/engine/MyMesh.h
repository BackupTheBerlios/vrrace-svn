#pragma once
#ifndef MYMESH_H
#define MYMESH_H

#include "includes.h"
#include "MyVertex.h"
#include "MyMasterPosition.h"

class MyMesh : public MyMasterPosition
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
	void				setReference(MyMasterPosition* givenReference);
	
private:
	void				matrixOperations();
	LPCTSTR				m_FileName;
	DWORD				m_dwNumMaterials;
	LPD3DXMESH			m_pMesh;
	D3DMATERIAL9*		m_pMaterials;	//Materialbuffer
	LPDIRECT3DTEXTURE9*	m_pTextures;	//Texturbuffer
	D3DXMATRIX*			_matWorld;
	MyMesh*				m_pMasterObject;
};
#endif