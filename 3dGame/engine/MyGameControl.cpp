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

	m_pView->getPos()->setValues(200.0f, 50.0f, 200.0f);
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
							"resources/x_files/sphere.x",
							-1000.0f, -50.0f, -1000.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 3.0f, 0.0f,
							0.0f, 0.005f, 0.0f))
		{
			tempObjb->load();
			tempObjb->activateScaling();
			tempObjb->getScale()->setValues(300.0f, 300.0f, 300.0f);
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
							"resources/x_files/sphere.x",
							500.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f))
		{
			tempObj->setReference(tempObjb->getPosition());
			tempObj->load();
			tempObj->activateScaling();
			tempObj->getScale()->setValues(20.0f, 20.0f, 20.0f);
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
							60.0f, 0.0f, -500.0f,
							0.0f, 0.0f, 0.3f,
							0.0f, 3.142f, 0.0f,
							0.0f, 0.0f, 0.01f))
		{
			tempObjc->load();
			m_pView->getVP()->setPValues(
				tempObjc->getPosition()->m_pPosition->getPX(),
				tempObjc->getPosition()->m_pPosition->getPY(),
				tempObjc->getPosition()->m_pPosition->getPZ());

			m_pMeshes.push_back(tempObjc);
		} else {
			return false;
		}
	}

	MyMesh*	tempObjd	= new MyMesh();
	if (tempObjd == NULL)
	{
		return false;
	} else {
		if (tempObjd->init(_D3DDevice,
							_matWorld,
							"resources/x_files/bigship1.x",
							30.0f, -20.0f, -500.0f,
							0.0f, 0.0f, 0.3f,
							0.0f, 3.142f, 0.0f,
							0.0f, 0.0f, 0.0f))
		{
			tempObjd->load();
			m_pMeshes.push_back(tempObjd);
		} else {
			return false;
		}
	}

	MyMesh*	tempObje	= new MyMesh();
	if (tempObje == NULL)
	{
		return false;
	} else {
		if (tempObje->init(_D3DDevice,
							_matWorld,
							"resources/x_files/bigship1.x",
							90.0f, -30.0f, -510.0f,
							0.0f, 0.0f, 0.3f,
							0.0f, 3.142f, 0.0f,
							0.0f, 0.0f, 0.0f))
		{
			tempObje->load();
			m_pMeshes.push_back(tempObje);
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