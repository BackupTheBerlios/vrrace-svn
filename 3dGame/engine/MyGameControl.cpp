#include "MyGameControl.h"

MyGameControl::MyGameControl(void)
{
	m_initCount		= 0;
	m_pView			= new MyView();
	m_bShowStatus	= false;
	//m_pMeshes		= new vector(new MyMesh());
}

MyGameControl::~MyGameControl(void)
{
	delete m_pView;
}

bool	MyGameControl::loadObjects()
{
	if (m_initCount > 0)
	{
		m_initCount++;
		for (int count = 0; count < m_pMeshes.capacity(); count++)
		{
			m_pMeshes[count]->load();
		}
	} else {
		return false;
	}
	return true;
}

bool	MyGameControl::drawObjects(D3DXMATRIX* givenMatWorld)
{
	for (int count = 0; count < m_pMeshes.capacity(); count++)
	{
           D3DXMatrixTranslation(
			givenMatWorld,
			m_pMeshes[count]->getPosition()->x,
			m_pMeshes[count]->getPosition()->y,
			m_pMeshes[count]->getPosition()->z
			);
			_D3DDevice->SetTransform(D3DTS_WORLD, givenMatWorld);
			m_pMeshes[count]->draw();
	}

	return true;
}

bool	MyGameControl::drawLights()
{
	for (int count = 0; count < m_pLights.capacity(); count++)
	{
		m_pLights[count]->show();
	}
	return true;
}

bool	MyGameControl::moveObjects()
{
	for (int count = 0; count < m_pMeshes.capacity(); count++)
	{
		m_pMeshes[count]->move();
	}
	
	return true;
}

bool	MyGameControl::init(LPDIRECT3DDEVICE9 givenDevice)
{
	if (m_initCount == 0)
	{
		m_initCount++;
	} else {
		return false;
	}

	_D3DDevice				= givenDevice;

	m_pView->m_Position.x	= 3.0f;
	m_pView->m_Position.y	= 1.0f;
	m_pView->m_Position.z	= 3.0f;

	m_pView->m_UpVector.x	= 0.0f;
	m_pView->m_UpVector.y	= 1.0f;
	m_pView->m_UpVector.z	= 0.0f;

	m_pView->m_ViewPoint.x	= 0.0f;
	m_pView->m_ViewPoint.y	= 0.0f;
	m_pView->m_ViewPoint.z	= 0.0f;

	this->initObjects();

	return true;
}

bool	MyGameControl::initObjects()
{
		/*MyMesh*	temp	= new MyMesh();

		temp->init(_D3DDevice, "resources/shusui.x", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.01f);
*/
/*		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/shusui.x", 0.0f, 0.0f, 0.0f, 0.01f, 0.0f, -0.05f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/star sail.x", 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, -0.051f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 2.x", 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, -0.01f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 5.x", 0.0f, 0.0f, 1.0f, 0.0f, 0.01f, -0.01f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 8.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 13.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 8.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 13.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 8.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 13.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 8.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 13.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 8.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 13.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		
		/*
		MyMesh*	temp2	= new MyMesh();
		temp2->init(_D3DDevice, "resources/shusui.x", 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, -0.01f);

		m_pMeshes.push_back(temp2);
*/

	return true;
}

bool	MyGameControl::addObject()
{
	MyMesh*	tempObj	= new MyMesh();
	if (tempObj == NULL)
	{
		return false;
	} else {
		if (tempObj->init(_D3DDevice, "resources/shusui.x", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.001f))
		{
			tempObj->load();
			m_pMeshes.push_back(tempObj);
		} else {
			return false;
		}
	}
		
	return true;
}

bool	MyGameControl::addLight()
{
	if (m_pLights.capacity() == 0) {
	MyLight*	tempLight	= new MyLight();
	if (tempLight == NULL)
	{
		MessageBox(NULL, "konnte Licht nicht erstellen", "Achtung", MB_OK);
		return false;
	} else {
		if (tempLight->init(_D3DDevice))
		{
			m_pLights.push_back(tempLight);
		} else {
			MessageBox(NULL, "konnte Licht nicht initialisieren", "Achtung", MB_OK);
			return false;
		}
	}
	}
	return true;
}

int	MyGameControl::getNumLights()
{
	return m_pLights.capacity();
}

int MyGameControl::getNumMeshes()
{
	return m_pMeshes.capacity();
}