#include "MyLight.h"

bool	MyLight::init(LPDIRECT3DDEVICE9 givenDevice,
								D3DXMATRIX* givenMatWorld,
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
								float givenRange)
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

	m_fRange = givenRange;

	m_pPosition->setValues(posX, posY, posZ);
	m_pDirection->setValues(dirX, dirY, dirZ);
	m_pRotation->setValues(rotX, rotY, rotZ);
	m_pRotDir->setValues(rotDirX, rotDirY, rotDirZ);

	ZeroMemory(&m_d3dLight, sizeof(m_d3dLight));

	m_d3dLight.Type			= D3DLIGHT_POINT;

	m_d3dLight.Attenuation0	= 1.0f;
	m_d3dLight.Attenuation1	= 0.0f;
	m_d3dLight.Attenuation2	= 0.0f;

//	m_d3dLight.Falloff		= 0.0f;
//	m_d3dLight.Theta		= 0.5f;
//	m_d3dLight.Phi			= 1.0f;
	m_d3dLight.Range		= m_fRange;

	return true;
}

void	MyLight::setMaterialValues(float rDiffuse, float gDiffuse, float bDiffuse,
									float rAmbient, float gAmbient, float bAmbient,
									float rSpecular, float gSpecular, float bSpecular)
{
	m_d3dLight.Diffuse.r	= rDiffuse;//1.0f;
	m_d3dLight.Diffuse.g	= gDiffuse;//1.0f;
	m_d3dLight.Diffuse.b	= bDiffuse;//1.0f;

	m_d3dLight.Ambient.r	= rAmbient;//1.0f;
	m_d3dLight.Ambient.g	= gAmbient;//1.0f;
	m_d3dLight.Ambient.b	= bAmbient;//1.0f;

	m_d3dLight.Specular.r	= rSpecular;//1.0f;
	m_d3dLight.Specular.g	= gSpecular;//1.0f;
	m_d3dLight.Specular.b	= bSpecular;//1.0f;
}

void	MyLight::show()
{
//	FLOAT x = cosf(timeGetTime()/100.0f)*20;
    
//  FLOAT z = sinf(timeGetTime()/100.0f)*20;

//	m_d3dLight.Position.x	+= 0.01f;
//	m_d3dLight.Position.y	+= 0.01f;
//	m_d3dLight.Position.z	+= 0.01f;

	//m_d3dLight.Position.z	+= 0.01f;
	
	//this->transform();

	m_d3dLight.Position.x	= this->getAbsolutePosition()->getX();//0.0f;
	m_d3dLight.Position.y	= this->getAbsolutePosition()->getY();//0.0f;
	m_d3dLight.Position.z	= this->getAbsolutePosition()->getZ();//0.0f;

	m_d3dLight.Direction.x	= m_pDirection->getX();//1.0f;
	m_d3dLight.Direction.y	= m_pDirection->getY();//1.0f;
	m_d3dLight.Direction.z	= m_pDirection->getZ();//1.0f;

	_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	_D3DDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	_D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	_D3DDevice->SetRenderState( D3DRS_DITHERENABLE,   TRUE );
	_D3DDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
	_D3DDevice->SetLight(0, &m_d3dLight);
	_D3DDevice->LightEnable(0, TRUE);
}