#include "MyGameControl.h"

MyGameControl::MyGameControl(void)
{
	m_initCount			= 0;
	m_pMainCam			= new MyView();
	m_bShowStatus		= true;
	
	m_pMusic			= new MyMusic();
	m_pDirectSound		= new MyDSound();
	m_pStarsField		= new MyStarsField();

	_DirectPlay		= NULL;
	m_iDPchoice			= -1;
}

MyGameControl::~MyGameControl(void)
{
//	delete m_pView;
//	delete m_pStarsField;
	delete m_pMusic;
	delete	m_pDirectSound;
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

MyDPlay* MyGameControl::getDirectPlay()
{
	return _DirectPlay;
}

bool	MyGameControl::drawObjects()
{

	EnterCriticalSection(&_DirectPlay->m_csDP);

	for (DWORD count = 0; count < _DirectPlay->m_pMasterMeshes.size(); count++)
	{
		if (_DirectPlay->m_pAllMeshes[count] != NULL)
			_DirectPlay->m_pMasterMeshes[count]->calcClients();
	}
	
	for (count = 0; count < _DirectPlay->m_pAllMeshes.size(); count++)
	{
		if (_DirectPlay->m_pAllMeshes[count] != NULL)
			_DirectPlay->m_pAllMeshes[count]->draw();
	}

	LeaveCriticalSection(&_DirectPlay->m_csDP);
	
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
	m_pStarsField->move();
	for (DWORD count = 0; count < _DirectPlay->m_pAllMeshes.size(); count++)
	{
		//if(_DirectPlay->m_pAllMeshes[count]->m_bToSend || (m_iDPchoice == 0))
		if (_DirectPlay->m_pAllMeshes[count] != NULL)
		{
			_DirectPlay->m_pAllMeshes[count]->move();
			if(_DirectPlay->m_pMeshSounds[count] != NULL)
			{
				_DirectPlay->m_pMeshSounds[count]->set3DSoundPosition(_DirectPlay->m_pAllMeshes[count]->getAbsolutePosition()->getX(),
					_DirectPlay->m_pAllMeshes[count]->getAbsolutePosition()->getY(),
					_DirectPlay->m_pAllMeshes[count]->getAbsolutePosition()->getZ());
				_DirectPlay->m_pMeshSounds[count]->set3DSoundDirection(_DirectPlay->m_pAllMeshes[count]->m_pDirection->getX(),
					_DirectPlay->m_pAllMeshes[count]->m_pDirection->getY(),
					_DirectPlay->m_pAllMeshes[count]->m_pDirection->getZ());
			}
		}
	}
	m_pMainCam->move();
	m_pDirectSound->setListenerPosition(m_pMainCam->getPos()->getX(), m_pMainCam->getPos()->getY(), m_pMainCam->getPos()->getZ());
	m_pDirectSound->setListenerDirection(m_pMainCam->getDirection()->getX(), m_pMainCam->getDirection()->getY(), m_pMainCam->getDirection()->getZ());
	m_pDirectSound->setListenerOrientation(m_pMainCam->getVP()->getX(), m_pMainCam->getVP()->getY(), m_pMainCam->getVP()->getZ(),
		m_pMainCam->getUV()->getX(), m_pMainCam->getUV()->getY(), m_pMainCam->getUV()->getZ());
			
	return true;
}

bool	MyGameControl::init(LPDIRECT3DDEVICE9 givenDevice,
							D3DXMATRIX* givenMatWorld,
							HWND*	givenHWnd,
							MyDPlay* givenDPlay,
							int choice,
							int shipChoice)
{
	if (m_initCount == 0)
	{
		m_initCount++;
	} else {
		return false;
	}

	_D3DDevice				= givenDevice;
	_matWorld				= givenMatWorld;
	m_hWnd					= givenHWnd;
	_DirectPlay				= givenDPlay;
	m_iDPchoice				= choice;
	m_iShipChoice			= shipChoice;

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

bool	MyGameControl::initMusic()
{
	if(m_pMusic->init(MUSICFILENAME, true))
	{
		if(m_pMusic->setVolume(-1800))
		{
			//m_pMusic->play();
		}
		return true;
	} else {
		return false;
	}
}

bool	MyGameControl::initDirectSound()
{
	bool result = m_pDirectSound->init(m_hWnd);
	_DirectPlay->setSoundInstance(m_pDirectSound->getDSound());
	return result;
}

bool	MyGameControl::startSound()
{
	EnterCriticalSection(&_DirectPlay->m_csDP);

	for (DWORD count = 0; count < _DirectPlay->m_pMeshSounds.size(); count++)
	{
		if(_DirectPlay->m_pMeshSounds[count])
		{
			_DirectPlay->m_pMeshSounds[count]->play(true, 0);
		}
	}

	LeaveCriticalSection(&_DirectPlay->m_csDP);

	return true;
}

bool	MyGameControl::presentMusic()
{
	return true;// m_pMusic->presentMusic();
}

bool	MyGameControl::addPlayer(string* givenName)
{
	if (_DirectPlay->m_pLocalPlayer == NULL)
	{
		return false;
	} else {
		_DirectPlay->m_pLocalPlayer->m_pShipChoice = m_iShipChoice;
		if (_DirectPlay->m_pLocalPlayer->getMesh()->init(_D3DDevice,
											_matWorld,
											MyDPlay::m_pMeshPaths[_DirectPlay->m_pLocalPlayer->m_pShipChoice],//"resources/x_files/star sail.x",
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
			if(SUCCEEDED(_DirectPlay->m_pLocalPlayer->getMesh()->load()))
			{
				if(_DirectPlay->m_pLocalPlayer->getSound()->init(m_pDirectSound->getDSound(), SHIPSOUND, DSBCAPS_CTRL3D | DSBCAPS_LOCDEFER))
				{
					_DirectPlay->m_pLocalPlayer->getSound()->set3DSoundDistance(100.0f, 1000.0f);
					_DirectPlay->m_pAllMeshes.push_back(_DirectPlay->m_pLocalPlayer->getMesh());
					_DirectPlay->m_pMasterMeshes.push_back(_DirectPlay->m_pLocalPlayer->getMesh());
					_DirectPlay->m_pMeshSounds.push_back(/*_DirectPlay->m_pLocalPlayer->getSound()*/NULL);

					LeaveCriticalSection(&_DirectPlay->m_csDP);

					m_pMainCam->setMaster(_DirectPlay->m_pLocalPlayer->getMesh());
					m_pMainCam->getLP()->setValues(0.0f, 0.0f, 100.0f);
					m_pMainCam->move();
				} else {
					LeaveCriticalSection(&_DirectPlay->m_csDP);
					return false;
				}
			} else {
				SAFE_DELETE(_DirectPlay->m_pLocalPlayer);
				_DirectPlay->m_pLocalPlayer = NULL;
				LeaveCriticalSection(&_DirectPlay->m_csDP);
				return false;
			}
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
	MyMesh*		sonne		= new MyMesh();
	MySound*	sonneSound	= new MySound();
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
			if(SUCCEEDED(sonne->load()))
			{
				sonne->activateScaling();
				sonne->getScale()->setValues(100.0f, 100.0f, 100.0f);
				sonne->initMaterialValues(0.0f, 0.0f, 0.0f, 1.1f,
												0.0f, 0.0f, 0.0f,
												1.0f, 1.0f, 1.0f);
				if(sonneSound->init(m_pDirectSound->getDSound(), PLANETSOUND, DSBCAPS_CTRL3D | DSBCAPS_LOCDEFER))
				{
					sonneSound->set3DSoundDistance(10.0f, 1000.0f);
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
					_DirectPlay->m_pMeshSounds.push_back(sonneSound);
				} else {
					LeaveCriticalSection(&_DirectPlay->m_csDP);
					return false;
				}
			} else {
				LeaveCriticalSection(&_DirectPlay->m_csDP);
				return false;
			}
		} else {
			LeaveCriticalSection(&_DirectPlay->m_csDP);
			return false;
		}

		LeaveCriticalSection(&_DirectPlay->m_csDP);
	}

	MyMesh*		erde		= new MyMesh();
	MySound*	erdeSound	= new MySound();
	if (erde == NULL)
	{
		return false;
	} else {

		EnterCriticalSection(&_DirectPlay->m_csDP);

		if (erde->init(_D3DDevice,
							_matWorld,
							"resources/x_files/sphere.x",
							"resources/x_files/earth.bmp",
							500.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f,
							0.0f, 3.0f, 0.0f,
							0.0f, -0.3f, 0.0f,
							true, false))
		{
			if(SUCCEEDED(erde->load()))
			{
				erde->activateScaling();
				erde->getScale()->setValues(10.0f, 10.0f, 10.0f);
				erde->initMaterialValues(0.0f, 0.0f, 0.0f, 1.1f,
												0.0f, 0.0f, 0.0f,
												1.0f, 1.0f, 1.0f);
				erde->setMaster(sonne);
				if(erdeSound->init(m_pDirectSound->getDSound(), PLANETSOUND, DSBCAPS_CTRL3D | DSBCAPS_LOCDEFER))
				{
					erdeSound->set3DSoundDistance(100.0f, 1000.0f);
					//erdeSound->play(true, 0);
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
					_DirectPlay->m_pMeshSounds.push_back(erdeSound);
				} else {
					LeaveCriticalSection(&_DirectPlay->m_csDP);
					return false;
				}
			} else {
				LeaveCriticalSection(&_DirectPlay->m_csDP);
				return false;
			}
		} else {
			LeaveCriticalSection(&_DirectPlay->m_csDP);
			return false;
		}

		LeaveCriticalSection(&_DirectPlay->m_csDP);
	}

	MyMesh*		erdkern			= new MyMesh();
	MySound*	erdkernSound	= new MySound();
	if (erdkern == NULL)
	{
		return false;
	} else {

		EnterCriticalSection(&_DirectPlay->m_csDP);

		if (erdkern->init(_D3DDevice,
							_matWorld,
							"resources/x_files/sphere.x",
							NULL,
							500.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f,
							0.0f, 3.0f, 0.0f,
							0.0f, 0.02f, 0.0f,
							true, false))
		{
			if(SUCCEEDED(erdkern->load()))
			{
				erdkern->deactivateScaling();
				//erdkern->getScale()->setValues(1.0f, 1.0f, 1.0f);
				erdkern->initMaterialValues(0.0f, 0.0f, 0.0f, 1.1f,
												0.0f, 0.0f, 0.0f,
												1.0f, 1.0f, 1.0f);
				erdkern->setMaster(sonne);
				if(erdkernSound->init(m_pDirectSound->getDSound(), PLANETSOUND, DSBCAPS_CTRL3D | DSBCAPS_LOCDEFER))
				{
					erdkernSound->set3DSoundDistance(10.0f, 1000.0f);
					if(m_iDPchoice != 0)
					{
						if(*_DirectPlay->m_pbHostingApp)
						{
							_DirectPlay->m_pLocalMeshes.push_back(erdkern);
						} else {
							_DirectPlay->m_pNetworkMeshes.push_back(erdkern);
						}
					}
					_DirectPlay->m_pAllMeshes.push_back(erdkern);
					_DirectPlay->m_pMeshSounds.push_back(erdkernSound);
				} else {
					LeaveCriticalSection(&_DirectPlay->m_csDP);
					return false;
				}
			} else {
				LeaveCriticalSection(&_DirectPlay->m_csDP);
				return false;
			}
		} else {
			LeaveCriticalSection(&_DirectPlay->m_csDP);
			return false;
		}

		LeaveCriticalSection(&_DirectPlay->m_csDP);
	}

	MyMesh*		mond		= new MyMesh();
	MySound*	mondSound	= new MySound();
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
			mond->setMaster(erdkern);
			if(SUCCEEDED(mond->load()))
			{
				mond->activateScaling();
				mond->getScale()->setValues(2.0f, 2.0f, 2.0f);
				if(mondSound->init(m_pDirectSound->getDSound(), PLANETSOUND, DSBCAPS_CTRL3D | DSBCAPS_LOCDEFER))
				{
					mondSound->set3DSoundDistance(10.0f, 1000.0f);
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
					_DirectPlay->m_pMeshSounds.push_back(mondSound);
				} else {
					LeaveCriticalSection(&_DirectPlay->m_csDP);
					return false;
				}
			} else {
				LeaveCriticalSection(&_DirectPlay->m_csDP);
				return false;
			}
		} else {
			LeaveCriticalSection(&_DirectPlay->m_csDP);
			return false;
		}

		LeaveCriticalSection(&_DirectPlay->m_csDP);
	}
	
	MyMesh*		kreuzer1		= new MyMesh();
	MySound*	kreuzer1Sound	= new MySound();
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
			if(SUCCEEDED(kreuzer1->load()))
			{
				if(kreuzer1Sound->init(m_pDirectSound->getDSound(), SHIPSOUND, DSBCAPS_CTRL3D | DSBCAPS_LOCDEFER))
				{
					kreuzer1Sound->set3DSoundDistance(10.0f, 1000.0f);
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
					_DirectPlay->m_pMeshSounds.push_back(kreuzer1Sound);
				} else {
					LeaveCriticalSection(&_DirectPlay->m_csDP);
					return false;
				}
			} else {
				LeaveCriticalSection(&_DirectPlay->m_csDP);
				return false;
			}
		} else {
			LeaveCriticalSection(&_DirectPlay->m_csDP);
			return false;
		}

		LeaveCriticalSection(&_DirectPlay->m_csDP);
	}

	

	MyMesh*		jaeger1			= new MyMesh();
	MySound*	jaeger1Sound	= new MySound();
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
			if(SUCCEEDED(jaeger1->load()))
			{
				jaeger1->setMaster(kreuzer1);
				if(jaeger1Sound->init(m_pDirectSound->getDSound(), SHIPSOUND, DSBCAPS_CTRL3D | DSBCAPS_LOCDEFER))
				{
					jaeger1Sound->set3DSoundDistance(10.0f, 1000.0f);
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
					_DirectPlay->m_pMeshSounds.push_back(jaeger1Sound);
				} else {
					LeaveCriticalSection(&_DirectPlay->m_csDP);
					return false;
				}
			} else {
				LeaveCriticalSection(&_DirectPlay->m_csDP);
				return false;
			}
		} else {
			LeaveCriticalSection(&_DirectPlay->m_csDP);
			return false;
		}

		LeaveCriticalSection(&_DirectPlay->m_csDP);
	}
	
	MyMesh*		sunLayer1		= NULL;
	MySound*	sunLayer1Sound	= NULL;
	for(int count = 0; count < 8; count++)
	{
		sunLayer1		= new MyMesh();
		sunLayer1Sound	= new MySound();
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
				if(SUCCEEDED(sunLayer1->load()))
				{
					sunLayer1->activateScaling();
					sunLayer1->getScale()->setValues(99.9f-((float)count/1000.0f), 99.9f-((float)count/1000.0f), 99.9f-((float)count/1000.0f));
					sunLayer1->initMaterialValues(0.0f, 0.0f, 0.0f, 1.1f,
													0.0f, 0.0f, 0.0f,
													1.0f, 1.0f, 1.0f);
					if(sunLayer1Sound->init(m_pDirectSound->getDSound(), PLANETSOUND, DSBCAPS_CTRL3D | DSBCAPS_LOCDEFER))
					{
						sunLayer1Sound->set3DSoundDistance(10.0f, 1000.0f);
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
						_DirectPlay->m_pMeshSounds.push_back(sunLayer1Sound);
						sunLayer1->setMaster(sonne);
						sunLayer1		= NULL;
						sunLayer1Sound	= NULL;
					} else {
						LeaveCriticalSection(&_DirectPlay->m_csDP);
						return false;
					}
				} else {
					LeaveCriticalSection(&_DirectPlay->m_csDP);
					return false;
				}
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
		sendingToken.vectorId				= count;

		EnterCriticalSection(&_DirectPlay->m_csDP);

		sendingToken.posinfo.position.x		= _DirectPlay->m_pLocalMeshes[count]->m_pPosition->getX();
		sendingToken.posinfo.position.y		= _DirectPlay->m_pLocalMeshes[count]->m_pPosition->getY();
		sendingToken.posinfo.position.z		= _DirectPlay->m_pLocalMeshes[count]->m_pPosition->getZ();
		
		sendingToken.posinfo.direction.x	= _DirectPlay->m_pLocalMeshes[count]->m_pDirection->getX();
		sendingToken.posinfo.direction.y	= _DirectPlay->m_pLocalMeshes[count]->m_pDirection->getY();
		sendingToken.posinfo.direction.z	= _DirectPlay->m_pLocalMeshes[count]->m_pDirection->getZ();
		
		sendingToken.posinfo.rotation.x		= _DirectPlay->m_pLocalMeshes[count]->m_pRotation->getX();
		sendingToken.posinfo.rotation.y		= _DirectPlay->m_pLocalMeshes[count]->m_pRotation->getY();
		sendingToken.posinfo.rotation.z		= _DirectPlay->m_pLocalMeshes[count]->m_pRotation->getZ();
		
		sendingToken.posinfo.rotdir.x		= _DirectPlay->m_pLocalMeshes[count]->m_pRotDir->getX();
		sendingToken.posinfo.rotdir.y		= _DirectPlay->m_pLocalMeshes[count]->m_pRotDir->getY();
		sendingToken.posinfo.rotdir.z		= _DirectPlay->m_pLocalMeshes[count]->m_pRotDir->getZ();

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
		sendingToken.ship   = _DirectPlay->m_pLocalPlayer->m_pShipChoice;

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
		
		sendingToken.position.posinfo.rotdir.x		= _DirectPlay->m_pLocalPlayer->getMesh()->m_pRotDir->getX();
		sendingToken.position.posinfo.rotdir.y		= _DirectPlay->m_pLocalPlayer->getMesh()->m_pRotDir->getY();
		sendingToken.position.posinfo.rotdir.z		= _DirectPlay->m_pLocalPlayer->getMesh()->m_pRotDir->getZ();

		sendingToken.position.posinfo.rotate.x		= givenX;
		sendingToken.position.posinfo.rotate.y		= givenY;
		sendingToken.position.posinfo.rotate.z		= givenZ;

		sendingToken.matrix							= *_DirectPlay->m_pLocalPlayer->getMesh()->getPositionMatrix();

		sendingToken.position.speed					= *_DirectPlay->m_pLocalPlayer->getMesh()->m_pSpeed;

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
								10000.0f))
		{
			tempLight->setMaterialValues(1.0f, 1.0f, 1.0f, 0.4f, 0.4f, 0.4f, 1.0f, 1.0f, 1.0f);
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
								20000))
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