#include "MyMesh.h"

MyMesh::MyMesh(void)
{
	_D3DDevice			= NULL;
	m_FileName			= NULL;
	m_pPosition			= new MyMasterPosition();
	m_pReferencePoint	= new MyMasterPosition();
	m_pScaleFactor		= new MyVertex();
	m_bScale			= false;

	m_pPosition->m_pPosition->setValues(0.0f, 0.0f, 0.0f);
	m_pPosition->m_pDirection->setValues(0.0f, 0.0f, 0.0f);
	m_pPosition->m_pRotation->setValues(0.0f, 0.0f, 0.0f);
	m_pPosition->m_pRotDir->setValues(0.0f, 0.0f, 0.0f);

	m_pReferencePoint->m_pPosition->setValues(0.0f, 0.0f, 0.0f);
	m_pReferencePoint->m_pDirection->setValues(0.0f, 0.0f, 0.0f);
	m_pReferencePoint->m_pRotation->setValues(0.0f, 0.0f, 0.0f);
	m_pReferencePoint->m_pRotDir->setValues(0.0f, 0.0f, 0.0f);

	m_pScaleFactor->setValues(1.0f, 2.0f, 1.0f);
}

MyMesh::~MyMesh()
{
	delete m_pPosition;
	delete m_pScaleFactor;
	m_pReferencePoint	= NULL;

	if (m_pMaterials != NULL)
		delete[] m_pMaterials;
	
	if (m_pTextures)
	{
		for (DWORD count = 0; count < m_dwNumMaterials; count++)
		{
			if (m_pTextures[count])
				m_pTextures[count]->Release();
		}
		delete[] m_pTextures;
	}
	
	if (m_pMesh != NULL)
		m_pMesh->Release();
}

bool	MyMesh::init(LPDIRECT3DDEVICE9 givenDevice,
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
					 float rotDirZ)
{
	if (givenDevice != NULL)
	{
		_D3DDevice		= givenDevice;
	} else {
		return false;
	}

	if (givenMatWorld != NULL)
	{
		_matWorld		= givenMatWorld;
	} else {
		return false;
	}

	if (givenFileName != NULL)
	{
		m_FileName		= givenFileName;
	} else {
		return false;
	}

	m_pPosition->m_pPosition->setValues(posX, posY, posZ);
	m_pPosition->m_pDirection->setValues(dirX, dirY, dirZ);
	m_pPosition->m_pRotation->setValues(rotX, rotY, rotZ);
	m_pPosition->m_pRotDir->setValues(rotDirX, rotDirY, rotDirZ);

	return true;
}

void	MyMesh::move()
{
	m_pPosition->m_pPosition->addX(m_pPosition->m_pDirection->getX());
	m_pPosition->m_pPosition->addY(m_pPosition->m_pDirection->getY());
	m_pPosition->m_pPosition->addZ(m_pPosition->m_pDirection->getZ());

	m_pPosition->m_pRotation->addX(m_pPosition->m_pRotDir->getX());
	m_pPosition->m_pRotation->addY(m_pPosition->m_pRotDir->getY());
	m_pPosition->m_pRotation->addZ(m_pPosition->m_pRotDir->getZ());
}

MyMasterPosition*	MyMesh::getPosition()
{
	return m_pPosition;
}

MyMasterPosition*	MyMesh::getReference()
{
	return m_pReferencePoint;
}

MyVertex*	MyMesh::getScale()
{
	return m_pScaleFactor;
}

void	MyMesh::activateScaling()
{
	m_bScale	= true;
}

void	MyMesh::deactivateScaling()
{
	m_bScale	= false;
}

void	MyMesh::setReference(MyMasterPosition* givenReference)
{
	m_pReferencePoint->m_pPosition->setPValues(
		givenReference->m_pPosition->getPX(),
		givenReference->m_pPosition->getPY(),
		givenReference->m_pPosition->getPZ());
	m_pReferencePoint->m_pDirection->setPValues(
		givenReference->m_pDirection->getPX(),
		givenReference->m_pDirection->getPY(),
		givenReference->m_pDirection->getPZ());
	m_pReferencePoint->m_pRotation->setPValues(
		givenReference->m_pRotation->getPX(),
		givenReference->m_pRotation->getPY(),
		givenReference->m_pRotation->getPZ());
	m_pReferencePoint->m_pRotDir->setPValues(
		givenReference->m_pRotDir->getPX(),
		givenReference->m_pRotDir->getPY(),
		givenReference->m_pRotDir->getPZ());
}

HRESULT	MyMesh::load()
{
	LPD3DXBUFFER	pD3DXMtrlBuffer;

	if (FAILED(D3DXLoadMeshFromX(
							m_FileName,
							D3DXMESH_SYSTEMMEM,
							_D3DDevice,
							NULL,
							&pD3DXMtrlBuffer,
							NULL,
							&m_dwNumMaterials,
							&m_pMesh
							)))
	{
		MessageBox(NULL, "Could not find file", "3dgame.exe", MB_OK);
		return E_FAIL;
	}

	D3DXMATERIAL*	d3dxMaterials		= (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m_pMaterials						= new D3DMATERIAL9[m_dwNumMaterials];
	m_pTextures							= new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	for (DWORD count = 0; count < m_dwNumMaterials; count++)
	{
		m_pMaterials[count]				= d3dxMaterials[count].MatD3D;
		
		m_pMaterials[count].Ambient		= m_pMaterials[count].Diffuse;
		
		m_pMaterials[count].Diffuse.r	= 1.0f;
		m_pMaterials[count].Diffuse.g	= 1.0f;
		m_pMaterials[count].Diffuse.b	= 1.0f;
		//m_pMaterials[count].Diffuse.a	= 1.0f;
		
		m_pTextures[count]				= NULL;

		if (d3dxMaterials[count].pTextureFilename != NULL
			&& lstrlen(d3dxMaterials[count].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(
							_D3DDevice,
							d3dxMaterials[count].pTextureFilename,
							&m_pTextures[count]
							)))
			{
				MessageBox(NULL, "Could not find Texture map", "3dGame.exe", MB_OK);
			}
		}
	}

	pD3DXMtrlBuffer->Release();

	return S_OK;				
}

void	MyMesh::matrixOperations()
{
	D3DXMATRIX	tmpMatRot;			//Rotationsmatrix
	D3DXMATRIX	tmpMatTrans;		//Translationsmatrix
	D3DXMATRIX	tmpMatOwn;			//Transformation lokal

	D3DXMATRIX	tmpMatRefRot;		//Rotationsmatrix der Referenz
	D3DXMATRIX	tmpMatRefTrans;		//Translationsmatrix der Referenz
	D3DXMATRIX	tmpMatRef;			//TransformationsMatrix der Referenz

	D3DXMATRIX	tmpMatRotTrans;		//end-Transformationsmatrix
	D3DXMATRIX	tmpMatScale;		//Skalierungsmatrix

	//rotiere lokal
	D3DXMatrixRotationYawPitchRoll(
			&tmpMatRot,
			m_pPosition->m_pRotation->getY(),
			m_pPosition->m_pRotation->getX(),
			m_pPosition->m_pRotation->getZ());

	//transliere lokal
	D3DXMatrixTranslation(
			&tmpMatTrans,
			m_pPosition->m_pPosition->getX(),
			m_pPosition->m_pPosition->getY(),
			m_pPosition->m_pPosition->getZ()
			);

	//rotiere nach Referenz
	D3DXMatrixRotationYawPitchRoll(
			&tmpMatRefRot,
			m_pReferencePoint->m_pRotation->getY(),
			m_pReferencePoint->m_pRotation->getX(),
			m_pReferencePoint->m_pRotation->getZ());

	//transliere nach Referenz
	D3DXMatrixTranslation(
			&tmpMatRefTrans,
			m_pReferencePoint->m_pPosition->getX(),
			m_pReferencePoint->m_pPosition->getY(),
			m_pReferencePoint->m_pPosition->getZ());

	//fasse lokale Matrizen zusammen
	D3DXMatrixMultiply(
			&tmpMatOwn,
			&tmpMatRot,
			&tmpMatTrans);

	//fasse Referenz-Matrizen zusammen
	D3DXMatrixMultiply(
			&tmpMatRef,
			&tmpMatRefRot,
			&tmpMatRefTrans);

	//wenn Skalierung verlangt
	if (m_bScale) {
		//Skaliere
		D3DXMatrixScaling(
			&tmpMatScale,
			m_pScaleFactor->getX(),
			m_pScaleFactor->getY(),
			m_pScaleFactor->getZ());

		//fasse lokal und Referenzmatrix zusammen
		D3DXMatrixMultiply(
			&tmpMatRotTrans,
			&tmpMatOwn,
			&tmpMatRef);

		//fasse Transformation und Skalierung zusammen
		D3DXMatrixMultiply(
			_matWorld,
			&tmpMatScale,
			&tmpMatRotTrans);

	} else {
		//fassee nur lokale und referentielle Transformation zusammen
		D3DXMatrixMultiply(
			_matWorld,
			&tmpMatOwn,
			&tmpMatRef);
	}

	//fuehre Transformationen durch
	_D3DDevice->SetTransform(D3DTS_WORLD, _matWorld);
}

void	MyMesh::draw()
{
	this->matrixOperations();

	for (DWORD count = 0; count < m_dwNumMaterials; count++)
	{
		_D3DDevice->SetMaterial(&m_pMaterials[count]);
		_D3DDevice->SetTexture(0, m_pTextures[count]);
		m_pMesh->DrawSubset(count);
	}
}