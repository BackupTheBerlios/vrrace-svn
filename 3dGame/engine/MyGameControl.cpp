#include "MyGameControl.h"

MyGameControl::MyGameControl(void)
{
	m_initCount		= 0;
	m_pView			= new MyView();
	m_bShowStatus	= true;
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
		for (int count = 0; count < m_pMeshes.size(); count++)
		{
			m_pMeshes[count]->load();
		}
	} else {
		return false;
	}
	return true;
}

bool	MyGameControl::drawObjects()
{
	for (int count = 0; count < m_pMeshes.size(); count++)
	{
		m_pMeshes[count]->draw();
	}

	return true;
}

bool	MyGameControl::drawLights()
{
	for (int count = 0; count < m_pLights.size(); count++)
	{
		m_pLights[count]->show();
	}
	return true;
}

bool	MyGameControl::moveObjects()
{
	for (int count = 0; count < m_pMeshes.size(); count++)
	{
		m_pMeshes[count]->move();
	}
	
	return true;
}

bool	MyGameControl::init(LPDIRECT3DDEVICE9 givenDevice,
							D3DXMATRIX* givenMatWorld)
{
	if (m_initCount == 0)
	{
		m_initCount++;
	} else {
		return false;
	}

	_D3DDevice				= givenDevice;
	_matWorld				= givenMatWorld;

	m_pView->getPos()->setValues(100.0f, 20.0f, 100.0f);
	m_pView->getVP()->setValues(0.0f, 0.0f, 0.0f);
	m_pView->getUV()->setValues(0.0f, 1.0f, 0.0f);

	this->initObjects();

	return true;
}

bool	MyGameControl::initObjects()
{
		

	return true;
}

bool	MyGameControl::addObject()
{
	MyMesh*	tempObjb	= new MyMesh();
	if (tempObjb == NULL)
	{
		return false;
	} else {
		if (tempObjb->init(_D3DDevice,
							_matWorld,
							"resources/x_files/sphere4.x",
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.01f, 0.0f))
		{
			tempObjb->load();
			m_pMeshes.push_back(tempObjb);
		} else {
			return false;
		}
	}

	MyMesh*	tempObj	= new MyMesh();
	if (tempObj == NULL)
	{
		return false;
	} else {
		if (tempObj->init(_D3DDevice,
							_matWorld,
							"resources/x_files/spaceship 8.x",
							50.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f))
		{
			tempObj->setReference(tempObjb->getPosition());
			tempObj->load();
			m_pMeshes.push_back(tempObj);
		} else {
			return false;
		}
	}
	
	MyMesh*	tempObjc	= new MyMesh();
	if (tempObjc == NULL)
	{
		return false;
	} else {
		if (tempObjc->init(_D3DDevice,
							_matWorld,
							"resources/x_files/space station 5.x",
							50.0f, 0.0f, 50.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.001f, 0.01f))
		{
			tempObjc->load();
			m_pMeshes.push_back(tempObjc);
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
	return m_pLights.size();
}

int MyGameControl::getNumMeshes()
{
	return m_pMeshes.size();
}