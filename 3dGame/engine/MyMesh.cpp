#include "MyMesh.h"

MyMesh::MyMesh(void)
{
	_D3DDevice				= NULL;
	m_FileName				= NULL;
	m_TextureFileName		= NULL;
	m_pbAlphaBlending		= new bool(false);
	
	m_pPosition->setValues(0.0f, 0.0f, 0.0f);
	m_pDirection->setValues(0.0f, 0.0f, 0.0f);
	m_pRotation->setValues(0.0f, 0.0f, 0.0f);
	m_pRotDir->setValues(0.0f, 0.0f, 0.0f);
	m_pScaleFactor->setValues(1.0f, 1.0f, 1.0f);
}

MyMesh::~MyMesh()
{
	delete		m_TextureFileName;
	delete		m_pbAlphaBlending;

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
					 float speed,
					 float rotX,
					 float rotY,
					 float rotZ,
					 float rotDirX,
					 float rotDirY,
					 float rotDirZ,
					 bool givenAlphaBlending,
					 bool givenControl
					 )
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

	*m_pbAlphaBlending		= givenAlphaBlending;

	m_bControl				= givenControl;

	m_pPosition->setValues(posX, posY, posZ);
	m_pDirection->setValues(dirX, dirY, dirZ);
	*m_pSpeed = speed;
	m_pRotation->setValues(rotX, rotY, rotZ);
	m_pRotDir->setValues(rotDirX, rotDirY, rotDirZ);

	return true;
}

void MyMesh::initMaterialValues(float rDiffuse, float gDiffuse, float bDiffuse, float aDiffuse,
											float rAmbient, float gAmbient, float bAmbient,
											float rEmissive, float gEmissive, float bEmissive)
{
	for (DWORD count = 0; count < m_dwNumMaterials; count++)
	{

		if(*m_pbAlphaBlending)
		{
			m_pMaterials[count].Diffuse.r = rDiffuse;
			m_pMaterials[count].Diffuse.g = gDiffuse;
			m_pMaterials[count].Diffuse.b = bDiffuse;
			m_pMaterials[count].Diffuse.a = aDiffuse;//-(float)(pcount)/8.0f;

			m_pMaterials[count].Ambient.r = rAmbient;
			m_pMaterials[count].Ambient.g = gAmbient;
			m_pMaterials[count].Ambient.b = bAmbient;

			m_pMaterials[count].Emissive.r = rEmissive;
			m_pMaterials[count].Emissive.g = gEmissive;
			m_pMaterials[count].Emissive.b = bEmissive;
		}
	}
/*
	m_afDiffuse[0]	= rDiffuse;
	m_afDiffuse[1]	= gDiffuse;
	m_afDiffuse[2]	= bDiffuse;
	m_afDiffuse[3]	= aDiffuse;

	m_afAmbient[0]	= rAmbient;
	m_afAmbient[1]	= gAmbient;
	m_afAmbient[2]	= bAmbient;

	m_afEmissive[0]	= rEmissive;
	m_afEmissive[1]	= gEmissive;
	m_afEmissive[2]	= bEmissive;*/
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
		if (!*m_pbAlphaBlending)
		{
			m_pMaterials[count].Ambient		= m_pMaterials[count].Diffuse;
		
			m_pMaterials[count].Diffuse.r	= 1.0f;
			m_pMaterials[count].Diffuse.g	= 1.0f;
			m_pMaterials[count].Diffuse.b	= 1.0f;
			//m_pMaterials[count].Diffuse.a	= 1.0f;
		}
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
	/*if(!*m_pbIsDrawn)
	{
		m_InitialScaleValues.x = this->getScale()->getX();
		m_InitialScaleValues.y = this->getScale()->getY();
		m_InitialScaleValues.z = this->getScale()->getZ();
		*m_pbIsDrawn = true;
	}*/

	//this->getScale()->setValues(m_InitialScaleValues.x,m_InitialScaleValues.y,m_InitialScaleValues.z);

	//Alpha-Blending
	if(*m_pbAlphaBlending)
	{
		_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	/*char temp[118];
	sprintf(temp,"%s %d | %f - %f - %f",m_FileName,*m_piLayers,tmpScaleX,tmpScaleY,tmpScaleZ);

	MessageBox(NULL,temp,"Message",MB_OK);
	*/

	//	this->getScale()->setValues((this->getScale()->getX())+1,(this->getScale()->getY())+1,(this->getScale()->getZ())+1);
		this->transform();
		for (DWORD count = 0; count < m_dwNumMaterials; count++)
		{
			/*if(*m_pbAlphaBlending)
			{
				m_pMaterials[count].Diffuse.r = m_afDiffuse[0];
				m_pMaterials[count].Diffuse.g = m_afDiffuse[1];
				m_pMaterials[count].Diffuse.b = m_afDiffuse[2];
				m_pMaterials[count].Diffuse.a = m_afDiffuse[3]-(float)(pcount)/8.0f;

				m_pMaterials[count].Ambient.r = m_afAmbient[0];
				m_pMaterials[count].Ambient.g = m_afAmbient[1];
				m_pMaterials[count].Ambient.b = m_afAmbient[2];

				m_pMaterials[count].Emissive.r = m_afEmissive[0];
				m_pMaterials[count].Emissive.g = m_afEmissive[1];
				m_pMaterials[count].Emissive.b = m_afEmissive[2];
			}
*/
			_D3DDevice->SetMaterial(&m_pMaterials[count]);
			_D3DDevice->SetTexture(0, m_pTextures[count]);
			
			if(*m_pbAlphaBlending)
			{
				_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
				m_pMesh->DrawSubset(count);
				_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
			m_pMesh->DrawSubset(count);
		
	}
	//this->getScale()->setValues(tmpScaleX,tmpScaleY,tmpScaleZ);
}