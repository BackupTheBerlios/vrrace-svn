#include "MyMesh.h"

MyMesh::MyMesh(void)
{
	_D3DDevice				= NULL;
	m_FileName				= NULL;
	m_TextureFileName		= NULL;

	m_pPosition->setValues(0.0f, 0.0f, 0.0f);
	m_pDirection->setValues(0.0f, 0.0f, 0.0f);
	m_pRotation->setValues(0.0f, 0.0f, 0.0f);
	m_pRotDir->setValues(0.0f, 0.0f, 0.0f);

/*	m_pReferencePoint->m_pPosition->setValues(0.0f, 0.0f, 0.0f);
	m_pReferencePoint->m_pDirection->setValues(0.0f, 0.0f, 0.0f);
	m_pReferencePoint->m_pRotation->setValues(0.0f, 0.0f, 0.0f);
	m_pReferencePoint->m_pRotDir->setValues(0.0f, 0.0f, 0.0f);
*/
	m_pScaleFactor->setValues(1.0f, 2.0f, 1.0f);
}

MyMesh::~MyMesh()
{
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
					 float rotDirZ)
{
	if (givenDevice != NULL)
	{
		_D3DDevice			= givenDevice;
	} else {
		return false;
	}

	if (givenMatWorld != NULL)
	{
		_matWorld			= givenMatWorld;
	} else {
		return false;
	}

	if (givenFileName != NULL)
	{
		m_FileName			= givenFileName;
	} else {
		return false;
	}

	if(givenTextureFileName != NULL)
	{
		m_TextureFileName	= givenTextureFileName;
	}

	m_pPosition->setValues(posX, posY, posZ);
	m_pDirection->setValues(dirX, dirY, dirZ);
	m_pRotation->setValues(rotX, rotY, rotZ);
	m_pRotDir->setValues(rotDirX, rotDirY, rotDirZ);

	return true;
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

		if(m_TextureFileName != NULL)
		{
			d3dxMaterials[count].pTextureFilename = m_TextureFileName;
		}
		
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

void	MyMesh::draw()
{
	this->transform();
	for (DWORD count = 0; count < m_dwNumMaterials; count++)
	{
		_D3DDevice->SetMaterial(&m_pMaterials[count]);
		_D3DDevice->SetTexture(0, m_pTextures[count]);
		m_pMesh->DrawSubset(count);
	}
}