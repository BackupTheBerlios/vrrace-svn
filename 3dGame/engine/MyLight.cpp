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

	m_d3dLight.Diffuse.r	= 1.0f;
	m_d3dLight.Diffuse.g	= 1.0f;
	m_d3dLight.Diffuse.b	= 1.0f;

	m_d3dLight.Ambient.r	= 1.0f;
	m_d3dLight.Ambient.g	= 1.0f;
	m_d3dLight.Ambient.b	= 1.0f;

	m_d3dLight.Specular.r	= 1.0f;
	m_d3dLight.Specular.g	= 1.0f;
	m_d3dLight.Specular.b	= 1.0f;

	m_d3dLight.Position.x	= 0.0f;
	m_d3dLight.Position.y	= 0.0f;
	m_d3dLight.Position.z	= 0.0f;

	m_d3dLight.Direction.x	= 1.0f;
	m_d3dLight.Direction.y	= 1.0f;
	m_d3dLight.Direction.z	= 1.0f;

	m_d3dLight.Attenuation0	= 1.0f;
	m_d3dLight.Attenuation1	= 0.0f;
	m_d3dLight.Attenuation2	= 0.0f;

//	m_d3dLight.Falloff		= 0.0f;
//	m_d3dLight.Theta		= 0.5f;
//	m_d3dLight.Phi			= 1.0f;
	m_d3dLight.Range		= 5000.0f;

	return true;
}

void	MyLight::show()
{
//	FLOAT x = cosf(timeGetTime()/100.0f)*20;
    
//  FLOAT z = sinf(timeGetTime()/100.0f)*20;

//	m_d3dLight.Position.x	+= 0.01f;
//	m_d3dLight.Position.y	+= 0.01f;
//	m_d3dLight.Position.z	+= 0.01f;

	//m_d3dLight.Position.z	+= 0.01f;
	_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	_D3DDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	_D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	_D3DDevice->SetRenderState( D3DRS_DITHERENABLE,   TRUE );
	_D3DDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
	_D3DDevice->SetLight(0, &m_d3dLight);
	_D3DDevice->LightEnable(0, TRUE);
}