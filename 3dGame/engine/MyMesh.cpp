#include "MyMesh.h"

bool	MyMesh::init()
{
	m_FileName		= "resources/orbiter.x";
	m_Position.x	= 5.0f;
	m_Position.y	= 2.0f;
	m_Position.z	= 1.0f;

	return true;
}

HRESULT	MyMesh::load(LPDIRECT3DDEVICE9 givenDevice)
{
	LPD3DXBUFFER	pD3DXMtrlBuffer;

	if (FAILED(D3DXLoadMeshFromX(
							m_FileName,
							D3DXMESH_SYSTEMMEM,
							givenDevice,
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
							givenDevice,
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

void	MyMesh::draw(LPDIRECT3DDEVICE9 givenDevice)
{
	for (DWORD count = 0; count < m_dwNumMaterials; count++)
	{
		givenDevice->SetMaterial(&m_pMaterials[count]);
		givenDevice->SetTexture(0, m_pTextures[count]);
		m_pMesh->DrawSubset(count);
	}
}