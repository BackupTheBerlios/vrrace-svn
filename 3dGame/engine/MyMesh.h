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
								LPSTR givenTextureFileName,
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
								float rotDirZ,
								bool givenAlphaBlending,
								bool givenControl
								);
	void				initMaterialValues(float rDiffuse, float gDiffuse, float bDiffuse, float aDiffuse,
											float rAmbient, float gAmbient, float bAmbient,
											float rEmissive, float gEmissive, float bEmissive);
	HRESULT				load();
	void				draw();
	void				setReference(MyMasterPosition* givenReference);
	
private:
	void				matrixOperations();
	LPCTSTR				m_FileName;
	LPSTR				m_TextureFileName;
	DWORD				m_dwNumMaterials;
	LPD3DXMESH			m_pMesh;
	D3DMATERIAL9*		m_pMaterials;	//Materialbuffer
	LPDIRECT3DTEXTURE9*	m_pTextures;	//Texturbuffer
	D3DXMATRIX*			_matWorld;
	MyMesh*				m_pMasterObject;
	bool*				m_pbAlphaBlending;
	
	CUSTOMVERTEX		m_InitialScaleValues;
};
#endif