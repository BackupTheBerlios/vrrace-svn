#include "MyGameControl.h"

MyGameControl::MyGameControl(void)
{
	m_initCount			= 0;
	m_pMainCam			= new MyView();
	m_bShowStatus		= true;
	m_pLocalPlayer		= new MyPlayer();

//	m_pStarsField		= new MyStarsField();
}

MyGameControl::~MyGameControl(void)
{
//	delete m_pView;
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
	m_pMainCam->move();
			
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

	m_pMainCam->getPos()->setValues(200.0f, 50.0f, 200.0f);
	m_pMainCam->getVP()->setValues(0.0f, 0.0f, 0.0f);
	m_pMainCam->getUV()->setValues(0.0f, 1.0f, 0.0f);

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
											"resources/x_files/star sail.x",
											NULL,
											100.0f, 0.0f, 1000.0f,
											0.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 0.01f,
											false, true))
		{
			m_pLocalPlayer->getMesh()->load();

			m_pAllMeshes.push_back(m_pLocalPlayer->getMesh());
			m_pMasterMeshes.push_back(m_pLocalPlayer->getMesh());
			m_pMainCam->setMaster(m_pLocalPlayer->getMesh());
			m_pMainCam->getLP()->setValues(0.0f, 0.0f, 100.0f);
//			m_pMainCam->calcPosition();
			m_pMainCam->move();
			/*m_pView->getVP()->setPValues(
				m_pLocalPlayer->getMesh()->getAbsolutePosition()->getPX(),				
				m_pLocalPlayer->getMesh()->getAbsolutePosition()->getPY(),
				m_pLocalPlayer->getMesh()->getAbsolutePosition()->getPZ());*/
//			m_pMainCam = m_pLocalPlayer->getCamera();
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
	MyMesh*	sonne	= new MyMesh();
	if (sonne == NULL)
	{
		return false;
	} else {
		if (sonne->init(_D3DDevice,
							_matWorld,
							"resources/x_files/sun.x",
							"resources/x_files/Planet0.dds",
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.01f, 0.01f, 0.01f,
							true, false))
		{
			sonne->load();
			sonne->activateScaling();
			sonne->getScale()->setValues(100.0f, 100.0f, 100.0f);
			sonne->initMaterialValues(0.0f, 0.0f, 0.0f, 1.1f,
											0.0f, 0.0f, 0.0f,
											1.0f, 1.0f, 1.0f);
			m_pAllMeshes.push_back(sonne);
			m_pMasterMeshes.push_back(sonne);
		} else {
			return false;
		}
	}

	MyMesh*	erde	= new MyMesh();
	if (erde == NULL)
	{
		return false;
	} else {
		if (erde->init(_D3DDevice,
							_matWorld,
							"resources/x_files/sphere.x",
							NULL,
							500.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 3.0f, 0.0f,
							0.0f, -0.05f, 0.0f,
							true, false))
		{
			erde->load();
			erde->activateScaling();
			erde->getScale()->setValues(10.0f, 10.0f, 10.0f);
			erde->initMaterialValues(0.0f, 0.0f, 0.0f, 1.1f,
											0.0f, 0.0f, 0.0f,
											1.0f, 1.0f, 1.0f);
			erde->setMaster(sonne);
			m_pAllMeshes.push_back(erde);
		} else {
			return false;
		}
	}

	
	MyMesh*	mond	= new MyMesh();
	if (mond == NULL)
	{
		return false;
	} else {
		if (mond->init(_D3DDevice,
							_matWorld,
							"resources/x_files/sphere0.x",
							NULL,
							50.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							false, false))
		{
			mond->setMaster(erde);
			mond->load();
			mond->activateScaling();
			mond->getScale()->setValues(2.0f, 2.0f, 2.0f);
			m_pAllMeshes.push_back(mond);
			
			
		} else {
			return false;
		}
	}
	
	MyMesh*	kreuzer1	= new MyMesh();
	if (kreuzer1 == NULL)
	{
		return false;
	} else {
		if (kreuzer1->init(_D3DDevice,
							_matWorld,
							"resources/x_files/space station 5.x",
							NULL,
							60.0f, 0.0f, -500.0f,
							0.0f, 0.0f, 0.3f,
							0.0f, 3.142f, 0.0f,
							0.0f, 0.0f, 0.0f,
							false, false))
		{
			kreuzer1->load();
			
			m_pAllMeshes.push_back(kreuzer1);
			m_pMasterMeshes.push_back(kreuzer1);
		} else {
			return false;
		}
	}

	

	MyMesh*	jaeger1	= new MyMesh();
	if (jaeger1 == NULL)
	{
		return false;
	} else {
		if (jaeger1->init(_D3DDevice,
							_matWorld,
							"resources/x_files/bigship1.x",
							NULL,
							90.0f, -30.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							false, false))
		{
			jaeger1->load();
			jaeger1->setMaster(kreuzer1);
			m_pAllMeshes.push_back(jaeger1);
		} else {
			return false;
		}
	}
	
	MyMesh*	sunLayer1	= NULL;
	for(int count = 0; count < 8; count++)
	{
		sunLayer1 = new MyMesh();
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
								/*0.0f, 0.0f, 0.05f,*/
								(float)pow((double)-1.0f,(double)count)*(float)count/1000.0f, (float)-pow((double)-1.0f,(double)count)*(float)count/1000.0f, (float)pow((double)-1.0f,(double)count)*(float)count/1000.0f,
								true, false))
			{
				sunLayer1->load();
				sunLayer1->activateScaling();
				sunLayer1->getScale()->setValues(99.9f-((float)count/1000.0f), 99.9f-((float)count/1000.0f), 99.9f-((float)count/1000.0f));
				sunLayer1->initMaterialValues(0.0f, 0.0f, 0.0f, 1.1f,
												0.0f, 0.0f, 0.0f,
												1.0f, 1.0f, 1.0f);
				m_pAllMeshes.push_back(sunLayer1);
				sunLayer1->setMaster(sonne);
				sunLayer1 = NULL;
			} else {
				return false;
			}
		}
	}
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
	return (int)m_pAllLights.size();
}

int MyGameControl::getNumMeshes()
{
	return (int)m_pAllMeshes.size();
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