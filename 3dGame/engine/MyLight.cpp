#include "MyLight.h"

bool	MyLight::init(LPDIRECT3DDEVICE9 givenDevice)
{
	if (givenDevice == NULL)
	{
		return false;
	} else {
		_D3DDevice		= givenDevice;
	}

	ZeroMemory(&m_d3dLight, sizeof(m_d3dLight));

	m_d3dLight.Type			= D3DLIGHT_SPOT;

	m_d3dLight.Diffuse.r	= 1.0f;
	m_d3dLight.Diffuse.g	= 1.0f;
	m_d3dLight.Diffuse.b	= 1.0f;

	m_d3dLight.Ambient.r	= 1.0f;
	m_d3dLight.Ambient.g	= 1.0f;
	m_d3dLight.Ambient.b	= 1.0f;

	m_d3dLight.Specular.r	= 1.0f;
	m_d3dLight.Specular.g	= 1.0f;
	m_d3dLight.Specular.b	= 1.0f;

	m_d3dLight.Position.x	= 10.0f;
	m_d3dLight.Position.y	= 10.0f;
	m_d3dLight.Position.z	= 10.0f;

	m_d3dLight.Direction.x	= -10.0f;
	m_d3dLight.Direction.y	= -10.0f;
	m_d3dLight.Direction.z	= -10.0f;

	m_d3dLight.Attenuation0	= 1.0f;
	m_d3dLight.Attenuation1	= 0.025f;
	m_d3dLight.Attenuation2	= 0.02f;

	m_d3dLight.Falloff		= 1.0f;
	m_d3dLight.Theta		= 0.5f;
	m_d3dLight.Phi			= 1.0f;
	m_d3dLight.Range		= 50.0f;

	return true;
}

void	MyLight::show()
{
//	FLOAT x = cosf(timeGetTime()/100.0f)*20;
    
//  FLOAT z = sinf(timeGetTime()/100.0f)*20;

//	m_d3dLight.Direction.x	= x;
//	m_d3dLight.Direction.z	= z;

	//m_d3dLight.Position.z	+= 0.01f;
	_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//_D3DDevice->SetRenderState(D3DRS_COLORVERTEX, FALSE);
	//_D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	_D3DDevice->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
	_D3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
	_D3DDevice->SetLight(0, &m_d3dLight);
	_D3DDevice->LightEnable(0, TRUE);
}