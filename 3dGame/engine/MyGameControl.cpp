#include "MyGameControl.h"

MyGameControl::MyGameControl(void)
{
	m_initCount			= 0;
	m_pMainCam			= new MyView();
	m_bShowStatus		= true;
	
	m_pStarsField		= new MyStarsField();

	_DirectPlay		= NULL;
	m_iDPchoice			= -1;
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

		EnterCriticalSection(&_DirectPlay->m_csDP);

		for (DWORD count = 0; count < _DirectPlay->m_pAllMeshes.size(); count++)
		{
			if (_DirectPlay->m_pAllMeshes[count]->m_pType	== 1)
			{
				_DirectPlay->m_pAllMeshes[count]->load();
			}
		}

		LeaveCriticalSection(&_DirectPlay->m_csDP);

	} else {
		return false;
	}
	return true;
}

bool	MyGameControl::drawObjects()
{

	EnterCriticalSection(&_DirectPlay->m_csDP);

	for (DWORD count = 0; count < _DirectPlay->m_pMasterMeshes.size(); count++)
	{
		//if(m_pMasterMeshes[count]->m_bToSend || (m_iDPchoice == 0))
			_DirectPlay->m_pMasterMeshes[count]->calcClients();
	}
	
	for (count = 0; count < _DirectPlay->m_pAllMeshes.size(); count++)
	{
		_DirectPlay->m_pAllMeshes[count]->draw();
	}

	LeaveCriticalSection(&_DirectPlay->m_csDP);
	
	return true;
}

bool	MyGameControl::drawLights()
{
	/*for (DWORD count = 0; count < m_pAllLights.size(); count++)
	{
		m_pAllLights[count]->show();
	}*/
	return true;
}

bool	MyGameControl::moveObjects()
{
	m_pStarsField->move();
	for (DWORD count = 0; count < _DirectPlay->m_pAllMeshes.size(); count++)
	{
		//if(_DirectPlay->m_pAllMeshes[count]->m_bToSend || (m_iDPchoice == 0))
			_DirectPlay->m_pAllMeshes[count]->move();
	}
	m_pMainCam->move();
			
	return true;
}

bool	MyGameControl::init(LPDIRECT3DDEVICE9 givenDevice,
							D3DXMATRIX* givenMatWorld,
							MyDPlay* givenDPlay,
							int choice)
{
	if (m_initCount == 0)
	{
		m_initCount++;
	} else {
		return false;
	}

	_D3DDevice				= givenDevice;
	_matWorld				= givenMatWorld;
	_DirectPlay				= givenDPlay;
	m_iDPchoice				= choice;

	_DirectPlay->m_pLocalPlayer		= new MyPlayer();

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
	if (_DirectPlay->m_pLocalPlayer == NULL)
	{
		return false;
	} else {
		if (_DirectPlay->m_pLocalPlayer->getMesh()->init(_D3DDevice,
											_matWorld,
											"resources/x_files/star sail.x",
											NULL,
											100.0f, 0.0f, 1000.0f,
											0.0f, 0.0f, 0.0f,
											0.0f,
											0.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 0.0f,
											false, true))
		{

			EnterCriticalSection(&_DirectPlay->m_csDP);

			_DirectPlay->m_pLocalPlayer->m_pPlayerID = _DirectPlay->m_pid;
			_DirectPlay->m_pLocalPlayer->getMesh()->load();
			_DirectPlay->m_pAllMeshes.push_back(_DirectPlay->m_pLocalPlayer->getMesh());
			_DirectPlay->m_pMasterMeshes.push_back(_DirectPlay->m_pLocalPlayer->getMesh());

			LeaveCriticalSection(&_DirectPlay->m_csDP);

			m_pMainCam->setMaster(_DirectPlay->m_pLocalPlayer->getMesh());
			m_pMainCam->getLP()->setValues(0.0f, 0.0f, 100.0f);
			m_pMainCam->move();
		} else {
			return false;
		}
	}

	return true;
}

MyPlayer*	MyGameControl::getPlayer()
{
	return _DirectPlay->m_pLocalPlayer;
}

bool	MyGameControl::buildGame()
{
	MyMesh*	sonne	= new MyMesh();
	if (sonne == NULL)
	{
		return false;
	} else {

		EnterCriticalSection(&_DirectPlay->m_csDP);

		if (sonne->init(_D3DDevice,
							_matWorld,
							"resources/x_files/sun.x",
							"resources/x_files/Planet0.dds",
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.01f, 0.0f,
							true, false))
		{
			sonne->load();
			sonne->activateScaling();
			sonne->getScale()->setValues(100.0f, 100.0f, 100.0f);
			sonne->initMaterialValues(0.0f, 0.0f, 0.0f, 1.1f,
											0.0f, 0.0f, 0.0f,
											1.0f, 1.0f, 1.0f);
			if(m_iDPchoice != 0)
			{
				if(*_DirectPlay->m_pbHostingApp)
				{
					_DirectPlay->m_pLocalMeshes.push_back(sonne);
				} else {
					_DirectPlay->m_pNetworkMeshes.push_back(sonne);
				}
			}
			_DirectPlay->m_pAllMeshes.push_back(sonne);
			_DirectPlay->m_pMasterMeshes.push_back(sonne);
		} else {
			LeaveCriticalSection(&_DirectPlay->m_csDP);
			return false;
		}

		LeaveCriticalSection(&_DirectPlay->m_csDP);
	}

	MyMesh*	erde	= new MyMesh();
	if (erde == NULL)
	{
		return false;
	} else {

		EnterCriticalSection(&_DirectPlay->m_csDP);

		if (erde->init(_D3DDevice,
							_matWorld,
							"resources/x_files/sphere.x",
							NULL,
							500.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f,
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
			if(m_iDPchoice != 0)
			{
				if(*_DirectPlay->m_pbHostingApp)
				{
					_DirectPlay->m_pLocalMeshes.push_back(erde);
				} else {
					_DirectPlay->m_pNetworkMeshes.push_back(erde);
				}
			}
			_DirectPlay->m_pAllMeshes.push_back(erde);
		} else {
			LeaveCriticalSection(&_DirectPlay->m_csDP);
			return false;
		}

		LeaveCriticalSection(&_DirectPlay->m_csDP);
	}

	
	MyMesh*	mond	= new MyMesh();
	if (mond == NULL)
	{
		return false;
	} else {

		EnterCriticalSection(&_DirectPlay->m_csDP);

		if (mond->init(_D3DDevice,
							_matWorld,
							"resources/x_files/sphere0.x",
							NULL,
							50.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							false, false))
		{
			mond->setMaster(erde);
			mond->load();
			mond->activateScaling();
			mond->getScale()->setValues(2.0f, 2.0f, 2.0f);
			if(m_iDPchoice != 0)
			{
				if(*_DirectPlay->m_pbHostingApp)
				{
					_DirectPlay->m_pLocalMeshes.push_back(mond);
				} else {
					_DirectPlay->m_pNetworkMeshes.push_back(mond);
				}
			}
			_DirectPlay->m_pAllMeshes.push_back(mond);
			
			
		} else {
			LeaveCriticalSection(&_DirectPlay->m_csDP);
			return false;
		}

		LeaveCriticalSection(&_DirectPlay->m_csDP);
	}
	
	MyMesh*	kreuzer1	= new MyMesh();
	if (kreuzer1 == NULL)
	{
		return false;
	} else {

		EnterCriticalSection(&_DirectPlay->m_csDP);

		if (kreuzer1->init(_D3DDevice,
							_matWorld,
							"resources/x_files/space station 5.x",
							NULL,
							60.0f, 0.0f, -500.0f,
							0.0f, 0.0f, 0.3f,
							-10.0f,
							0.0f, 3.142f, 0.0f,
							0.0f, 0.0f, 0.0f,
							false, false))
		{
			kreuzer1->load();
			if(m_iDPchoice != 0)
			{
				if(*_DirectPlay->m_pbHostingApp)
				{
					_DirectPlay->m_pLocalMeshes.push_back(kreuzer1);
				} else {
					_DirectPlay->m_pNetworkMeshes.push_back(kreuzer1);
				}
			}
			_DirectPlay->m_pAllMeshes.push_back(kreuzer1);
			_DirectPlay->m_pMasterMeshes.push_back(kreuzer1);
		} else {
			LeaveCriticalSection(&_DirectPlay->m_csDP);
			return false;
		}

		LeaveCriticalSection(&_DirectPlay->m_csDP);
	}

	

	MyMesh*	jaeger1	= new MyMesh();
	if (jaeger1 == NULL)
	{
		return false;
	} else {

		EnterCriticalSection(&_DirectPlay->m_csDP);

		if (jaeger1->init(_D3DDevice,
							_matWorld,
							"resources/x_files/bigship1.x",
							NULL,
							90.0f, -30.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							false, false))
		{
			jaeger1->load();
			jaeger1->setMaster(kreuzer1);
			if(m_iDPchoice != 0)
			{
				if(*_DirectPlay->m_pbHostingApp)
				{
					_DirectPlay->m_pLocalMeshes.push_back(jaeger1);
				} else {
					_DirectPlay->m_pNetworkMeshes.push_back(jaeger1);
				}
			}
			_DirectPlay->m_pAllMeshes.push_back(jaeger1);
		} else {
			LeaveCriticalSection(&_DirectPlay->m_csDP);
			return false;
		}

		LeaveCriticalSection(&_DirectPlay->m_csDP);
	}
	
	MyMesh*	sunLayer1	= NULL;
	for(int count = 0; count < 8; count++)
	{
		sunLayer1 = new MyMesh();
		if (sunLayer1 == NULL)
		{
			return false;
		} else {

			EnterCriticalSection(&_DirectPlay->m_csDP);

			if (sunLayer1->init(_D3DDevice,
								_matWorld,
								"resources/x_files/sun.x",
								"resources/x_files/Planet0.dds",
								0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f,
								0.0f,
								0.0f, 0.0f, 0.0f,
								//0.0f, 0.0f, 0.05f,
								(float)pow((double)-1.0f,(double)count)*(float)count/1000.0f, (float)-pow((double)-1.0f,(double)count)*(float)count/1000.0f, (float)pow((double)-1.0f,(double)count)*(float)count/1000.0f,
								true, false))
			{
				sunLayer1->load();
				sunLayer1->activateScaling();
				sunLayer1->getScale()->setValues(99.9f-((float)count/1000.0f), 99.9f-((float)count/1000.0f), 99.9f-((float)count/1000.0f));
				sunLayer1->initMaterialValues(0.0f, 0.0f, 0.0f, 1.1f,
												0.0f, 0.0f, 0.0f,
												1.0f, 1.0f, 1.0f);
				if(m_iDPchoice != 0)
				{
					if(*_DirectPlay->m_pbHostingApp)
					{
						_DirectPlay->m_pLocalMeshes.push_back(sunLayer1);
					} else {
						_DirectPlay->m_pNetworkMeshes.push_back(sunLayer1);
					}
				}
				_DirectPlay->m_pAllMeshes.push_back(sunLayer1);
				sunLayer1->setMaster(sonne);
				sunLayer1 = NULL;
			} else {
				LeaveCriticalSection(&_DirectPlay->m_csDP);
				return false;
			}

			LeaveCriticalSection(&_DirectPlay->m_csDP);
		}
	}
	return true;
}

bool	MyGameControl::sendData()
{
	for(DWORD count = 0; count < _DirectPlay->m_pLocalMeshes.size(); count++)
	{
		GAMEOBJECTS sendingToken;
		sendingToken.vectorId			= count;

		EnterCriticalSection(&_DirectPlay->m_csDP);

		sendingToken.posinfo.position.x	= _DirectPlay->m_pLocalMeshes[count]->m_pPosition->getX();
		sendingToken.posinfo.position.y	= _DirectPlay->m_pLocalMeshes[count]->m_pPosition->getY();
		sendingToken.posinfo.position.z	= _DirectPlay->m_pLocalMeshes[count]->m_pPosition->getZ();
		
		sendingToken.posinfo.direction.x	= _DirectPlay->m_pLocalMeshes[count]->m_pDirection->getX();
		sendingToken.posinfo.direction.y	= _DirectPlay->m_pLocalMeshes[count]->m_pDirection->getY();
		sendingToken.posinfo.direction.z	= _DirectPlay->m_pLocalMeshes[count]->m_pDirection->getZ();
		
		sendingToken.posinfo.rotation.x	= _DirectPlay->m_pLocalMeshes[count]->m_pRotation->getX();
		sendingToken.posinfo.rotation.y	= _DirectPlay->m_pLocalMeshes[count]->m_pRotation->getY();
		sendingToken.posinfo.rotation.z	= _DirectPlay->m_pLocalMeshes[count]->m_pRotation->getZ();
		
		sendingToken.posinfo.rotdir.x	= _DirectPlay->m_pLocalMeshes[count]->m_pRotDir->getX();
		sendingToken.posinfo.rotdir.y	= _DirectPlay->m_pLocalMeshes[count]->m_pRotDir->getY();
		sendingToken.posinfo.rotdir.z	= _DirectPlay->m_pLocalMeshes[count]->m_pRotDir->getZ();

		LeaveCriticalSection(&_DirectPlay->m_csDP);

		_DirectPlay->sendMessage(&sendingToken, 0);
	}
	return true;
}

void	MyGameControl::sendPlayer(float givenX, float givenY, float givenZ)
{
	if(m_iDPchoice != 0)
	{
		_DirectPlay->m_pLocalPlayer->getMesh()->move();

		PLAYEROBJECTS	sendingToken;
		sendingToken.dpnid	= _DirectPlay->m_pLocalPlayer->m_pPlayerID;

		EnterCriticalSection(&_DirectPlay->m_csDP);

		sendingToken.position.posinfo.position.x	= _DirectPlay->m_pLocalPlayer->getMesh()->m_pPosition->getX();
		sendingToken.position.posinfo.position.y	= _DirectPlay->m_pLocalPlayer->getMesh()->m_pPosition->getY();
		sendingToken.position.posinfo.position.z	= _DirectPlay->m_pLocalPlayer->getMesh()->m_pPosition->getZ();
		
		sendingToken.position.posinfo.direction.x	= _DirectPlay->m_pLocalPlayer->getMesh()->m_pDirection->getX();
		sendingToken.position.posinfo.direction.y	= _DirectPlay->m_pLocalPlayer->getMesh()->m_pDirection->getY();
		sendingToken.position.posinfo.direction.z	= _DirectPlay->m_pLocalPlayer->getMesh()->m_pDirection->getZ();
		
		sendingToken.position.posinfo.rotation.x	= _DirectPlay->m_pLocalPlayer->getMesh()->m_pRotation->getX();
		sendingToken.position.posinfo.rotation.y	= _DirectPlay->m_pLocalPlayer->getMesh()->m_pRotation->getY();
		sendingToken.position.posinfo.rotation.z	= _DirectPlay->m_pLocalPlayer->getMesh()->m_pRotation->getZ();
		
		sendingToken.position.posinfo.rotdir.x	= _DirectPlay->m_pLocalPlayer->getMesh()->m_pRotDir->getX();
		sendingToken.position.posinfo.rotdir.y	= _DirectPlay->m_pLocalPlayer->getMesh()->m_pRotDir->getY();
		sendingToken.position.posinfo.rotdir.z	= _DirectPlay->m_pLocalPlayer->getMesh()->m_pRotDir->getZ();

		sendingToken.position.posinfo.rotate.x	= givenX;
		sendingToken.position.posinfo.rotate.y	= givenY;
		sendingToken.position.posinfo.rotate.z	= givenZ;

		sendingToken.matrix							= *_DirectPlay->m_pLocalPlayer->getMesh()->getPositionMatrix();

		LeaveCriticalSection(&_DirectPlay->m_csDP);

		_DirectPlay->sendMessage(&sendingToken, 1);
	}

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
		if (tempLight->init(_D3DDevice,
								_matWorld,
								0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f,
								0.0f, 0.01f, 0.0f,
								5000.0f))
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
	return (int)_DirectPlay->m_pAllMeshes.size();
}

bool MyGameControl::initStarsField()
{
	if(!m_pStarsField->init(_D3DDevice, _matWorld, 
								0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f,
								0.01f, 0.01f, 0.01f,
								1024))
	{
		return false;
	}
	return true;
}

bool MyGameControl::drawStarsField()
{
	if(!m_pStarsField->draw())
	{
		return false;
	}
	return true;
}