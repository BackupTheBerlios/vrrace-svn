#include "MyMesh.h"

MyMesh::MyMesh(void)
{
	_D3DDevice		= NULL;
	m_FileName		= NULL;
	m_Position.x	= m_Position.y	= m_Position.z	= 0.0f;
	m_Direction.x	= m_Direction.y	= m_Direction.z	= 0.0f;

}

bool	MyMesh::init(LPDIRECT3DDEVICE9 givenDevice,
					 LPCSTR givenFileName,
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
		m_FileName		= givenFileName; //"resources/shusui.x";
	} else {
		return false;
	}

	m_Position.x	+= posX;
	m_Position.y	+= posY;
	m_Position.z	+= posZ;

	m_Direction.x	+= dirX;
	m_Direction.y	+= dirY;
	m_Direction.z	+= dirZ;

/*	TCHAR* temp = new TCHAR[100];

	//Zeige aktuelle Betrachterposition
	sprintf(
		temp,
		"Position: %2.2f %2.2f %2.2f\nDirection: %2.2f %2.2f %2.2f",
		posX,
		posY,
		posZ,
		dirX,
		dirY,
		dirZ);

	MessageBox(NULL, temp, "letztes Objekt", MB_OK);
*/
	return true;
}

void	MyMesh::move()
{
	m_Position.x	+= m_Direction.x;
	m_Position.y	+= m_Direction.y;
	m_Position.z	+= m_Direction.z;
}

CUSTOMVERTEX*	MyMesh::getPosition()
{
	return &m_Position;
}

CUSTOMVERTEX*	MyMesh::getDirection()
{
	return &m_Direction;
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
		m_pMaterials[count]			= d3dxMaterials[count].MatD3D;
		m_pMaterials[count].Ambient	= m_pMaterials[count].Diffuse;

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
		_D3DDevice->SetMaterial(&m_pMaterials[count]);
		_D3DDevice->SetTexture(0, m_pTextures[count]);
		m_pMesh->DrawSubset(count);
	}
}