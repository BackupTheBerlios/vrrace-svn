#include "MyGameControl.h"

MyGameControl::MyGameControl(void)
{
	m_initCount			= 0;
	m_pView				= new MyView();
	m_bShowStatus		= true;
	m_pLocalPlayer		= new MyPlayer();

//	m_pStarsField		= new MyStarsField();
}

MyGameControl::~MyGameControl(void)
{
	delete m_pView;
//	delete m_pStarsField;
}

bool	MyGameControl::loadObjects()
{
	if (m_initCount > 0)
	{
		m_initCount++;
		for (DWORD count = 0; count < m_pAllMeshes.size(); count++)
		{
			if (m_pAllMeshes[count]->m_pType	== 1)
			{
				m_pAllMeshes[count]->load();
			}
		}
	} else {
		return false;
	}
	return true;
}

bool	MyGameControl::drawObjects()
{
	for (DWORD count = 0; count < m_pMasterMeshes.size(); count++)
	{
		m_pMasterMeshes[count]->calcClients();
	}
	
	for (count = 0; count < m_pAllMeshes.size(); count++)
	{
		m_pAllMeshes[count]->draw();
	}
	
	return true;
}

bool	MyGameControl::drawLights()
{
	for (DWORD count = 0; count < m_pAllLights.size(); count++)
	{
		m_pAllLights[count]->show();
	}
	return true;
}

bool	MyGameControl::moveObjects()
{
	for (DWORD count = 0; count < m_pAllMeshes.size(); count++)
	{
		m_pAllMeshes[count]->move();
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

bool	MyGameControl::addPlayer(string* givenName)
{
	if (m_pLocalPlayer == NULL)
	{
		return false;
	} else {
		if (m_pLocalPlayer->getMesh()->init(_D3DDevice,
											_matWorld,
											"resources/x_files/shusui.x",
											NULL,
											1000.0f, 0.0f, 1000.0f,
											0.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 0.0f,
											0.0f, 0.01f, 0.01f,
											false, true))
		{
			m_pLocalPlayer->getMesh()->load();

			m_pAllMeshes.push_back(m_pLocalPlayer->getMesh());
			m_pMasterMeshes.push_back(m_pLocalPlayer->getMesh());
			m_pView->getPos()->setValues(1010.0f, 0.0f, 1010.0f);
			m_pView->getVP()->setPValues(
				m_pLocalPlayer->getMesh()->getAbsolutePosition()->getPX(),				
				m_pLocalPlayer->getMesh()->getAbsolutePosition()->getPY(),
				m_pLocalPlayer->getMesh()->getAbsolutePosition()->getPZ());
		} else {
			return false;
		}
	}

	return true;
}

MyPlayer*	MyGameControl::getPlayer()
{
	return m_pLocalPlayer;
}

bool	MyGameControl::buildGame()
{
	MyMesh*	tempObjd	= new MyMesh();
	if (tempObjd == NULL)
	{
		return false;
	} else {
		if (tempObjd->init(_D3DDevice,
							_matWorld,
							"resources/x_files/sun.x",
							"resources/x_files/Planet0.dds",
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.01f, 0.0f,
							true, false))
		{
			tempObjd->load();
			tempObjd->activateScaling();
			tempObjd->getScale()->setValues(100.0f, 100.0f, 100.0f);
			tempObjd->initMaterialValues(0.0f, 0.0f, 0.0f, 1.1f,
											0.0f, 0.0f, 0.0f,
											1.0f, 1.0f, 1.0f);
			m_pAllMeshes.push_back(tempObjd);
			m_pMasterMeshes.push_back(tempObjd);
		} else {
			return false;
		}
	}

	MyMesh*	tempObjb	= new MyMesh();
	if (tempObjb == NULL)
	{
		return false;
	} else {
		if (tempObjb->init(_D3DDevice,
							_matWorld,
							"resources/x_files/sphere.x",
							NULL,
							500.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 3.0f, 0.0f,
							0.0f, -0.05f, 0.0f,
							true, false))
		{
			tempObjb->load();
			tempObjb->activateScaling();
			tempObjb->getScale()->setValues(10.0f, 10.0f, 10.0f);
			tempObjb->initMaterialValues(0.0f, 0.0f, 0.0f, 1.1f,
											0.0f, 0.0f, 0.0f,
											1.0f, 1.0f, 1.0f);
			tempObjb->setMaster(m_pMasterMeshes[0]);
			m_pAllMeshes.push_back(tempObjb);
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
							"resources/x_files/sphere0.x",
							NULL,
							50.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							false, false))
		{
			tempObj->setMaster(m_pAllMeshes[1]);
			tempObj->load();
			tempObj->activateScaling();
			tempObj->getScale()->setValues(2.0f, 2.0f, 2.0f);
			m_pAllMeshes.push_back(tempObj);
			
			
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
							NULL,
							60.0f, 0.0f, -500.0f,
							0.0f, 0.0f, 0.3f,
							0.0f, 3.142f, 0.0f,
							0.0f, 0.0f, 0.01f,
							false, false))
		{
			tempObjc->load();
			
			m_pAllMeshes.push_back(tempObjc);
			m_pMasterMeshes.push_back(tempObjc);
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
							NULL,
							90.0f, -30.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							false, false))
		{
			tempObje->load();
			tempObje->setMaster(m_pMasterMeshes[1]);
			m_pAllMeshes.push_back(tempObje);
		} else {
			return false;
		}
	}

	MyMesh*	sunLayer1	= new MyMesh();
	if (sunLayer1 == NULL)
	{
		return false;
	} else {
		if (sunLayer1->init(_D3DDevice,
							_matWorld,
							"resources/x_files/sun.x",
							"resources/x_files/Planet0.dds",
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.05f,
							true, false))
		{
			sunLayer1->load();
			sunLayer1->activateScaling();
			sunLayer1->getScale()->setValues(99.9f, 99.9f, 99.9f);
			sunLayer1->initMaterialValues(0.0f, 0.0f, 0.0f, 1.1f,
											0.0f, 0.0f, 0.0f,
											1.0f, 1.0f, 1.0f);
			m_pAllMeshes.push_back(sunLayer1);
			sunLayer1->setMaster(m_pMasterMeshes[0]);
		} else {
			return false;
		}
	}

	//this->addPlayer(NULL);

	return true;
}

bool	MyGameControl::addLight()
{
	if (m_pAllLights.size() == 0) {
	MyLight*	tempLight	= new MyLight();
	if (tempLight == NULL)
	{
		MessageBox(NULL, "konnte Licht nicht erstellen", "Achtung", MB_OK);
		return false;
	} else {
		if (tempLight->init(_D3DDevice))
		{
			m_pAllLights.push_back(tempLight);
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
	return m_pAllLights.size();
}

int MyGameControl::getNumMeshes()
{
	return m_pAllMeshes.size();
}

bool MyGameControl::initStarsField()
{
//	if(!m_pStarsField->init(_D3DDevice, 1024))
//	{
//		return false;
//	}
	return true;
}

bool MyGameControl::drawStarsField()
{
//	if(!m_pStarsField->draw())
//	{
//		return false;
//	}
	return true;
}