#include "MyMesh.h"

MyMesh::MyMesh(void)
{
	_D3DDevice		= NULL;
	m_FileName		= NULL;
	m_pPosition		= new MyVertex();
	m_pDirection	= new MyVertex();

	m_pPosition->setValues(0.0f, 0.0f, 0.0f);
	m_pDirection->setValues(0.0f, 0.0f, 0.0f);
}

bool	MyMesh::init(LPDIRECT3DDEVICE9 givenDevice,
					 LPCTSTR givenFileName,
					 float posX,
					 float posY,
					 float posZ,
					 float dirX,
					 float dirY,
					 float dirZ)
{
	if (givenDevice != NULL)
	{
		_D3DDevice		= givenDevice;
	} else {
		return false;
	}

	if (givenFileName != NULL)
	{
		m_FileName		= givenFileName;
	} else {
		return false;
	}

	m_pPosition->setValues(posX, posY, posZ);
	m_pDirection->setValues(dirX, dirY, dirZ);

	return true;
}

void	MyMesh::move()
{
	m_pPosition->addX(m_pDirection->getX());
	m_pPosition->addY(m_pDirection->getY());
	m_pPosition->addZ(m_pDirection->getZ());
}

MyVertex*	MyMesh::getPosition()
{
	return m_pPosition;
}

MyVertex*	MyMesh::getDirection()
{
	return m_pDirection;
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

	D3DXMATERIAL*	d3dxMaterials	= (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m_pMaterials					= new D3DMATERIAL9[m_dwNumMaterials];
	m_pTextures						= new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	for (DWORD count = 0; count < m_dwNumMaterials; count++)
	{
		m_pMaterials[count]				= d3dxMaterials[count].MatD3D;
		
		m_pMaterials[count].Ambient		= m_pMaterials[count].Diffuse;
		m_pMaterials[count].Diffuse.r	= 1.0f;
		m_pMaterials[count].Diffuse.g	= 1.0f;
		m_pMaterials[count].Diffuse.b	= 1.0f;


		m_pTextures[count]			= NULL;

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
	for (DWORD count = 0; count < m_dwNumMaterials; count++)
	{
		m_pMaterials[count].Diffuse.r	= 1.0f;
		m_pMaterials[count].Diffuse.g	= 1.0f;
		m_pMaterials[count].Diffuse.b	= 1.0f;
		_D3DDevice->SetMaterial(&m_pMaterials[count]);
		_D3DDevice->SetTexture(0, m_pTextures[count]);
		m_pMesh->DrawSubset(count);
	}
}