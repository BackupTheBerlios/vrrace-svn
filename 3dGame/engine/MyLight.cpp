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

	m_d3dLight.Position.x	= 1.0f;
	m_d3dLight.Position.y	= 1.0f;
	m_d3dLight.Position.z	= 0.0f;

	m_d3dLight.Attenuation0	= 1.0f;
	m_d3dLight.Range		= 2.0f;

	return true;
}

void	MyLight::show()
{
	_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	_D3DDevice->SetLight(0, &m_d3dLight);
	_D3DDevice->LightEnable(0, TRUE);
}