#include "MyDPlay.h"

IDirectPlay8Peer*	MyDPlay::m_pDP			= NULL;
CRITICAL_SECTION	MyDPlay::m_csDP;
HOST_NODE*			MyDPlay::m_pServerNode	= NULL;
bool*				MyDPlay::m_pbHostingApp	= NULL;
bool*				MyDPlay::m_pbConnected	= NULL;

/*Callback-Funktion fuer DirectPlay*/
HRESULT WINAPI MyDPlay::DPMessageProc(PVOID pvUserContext,
									DWORD dwMessageId,
									PVOID pvMsgBuffer)
{
	HRESULT hResult;

	switch(dwMessageId)
	{
	case DPN_MSGID_ENUM_HOSTS_RESPONSE:
		{
			PDPNMSG_ENUM_HOSTS_RESPONSE		pEnumHostsResponseMsg;
			const DPN_APPLICATION_DESC*		pAppDesc;
			HOST_NODE*						pServerNode;
			TCHAR*							pSessionName = NULL;

			pEnumHostsResponseMsg = (PDPNMSG_ENUM_HOSTS_RESPONSE) pvMsgBuffer;
			pAppDesc = pEnumHostsResponseMsg->pApplicationDescription;
			EnterCriticalSection(&m_csDP);
			pServerNode = m_pServerNode;
			if(pAppDesc->pwszSessionName)
			{
				if(pSessionName)
					SAFE_DELETE(pSessionName);
				DWORD dwChars = (DWORD) wcslen(pAppDesc->pwszSessionName) + 1;
				pSessionName = new TCHAR[dwChars];
				if(pSessionName)
				{
					DXUtil_ConvertWideStringToGenericCch(pSessionName, pAppDesc->pwszSessionName, dwChars);
				}
			}
			if(_stricmp(pSessionName,MM_SESSION_NAME) == 0)
			{
				if((pServerNode==NULL) || (pServerNode->pAppDesc->guidInstance != pAppDesc->guidInstance))
				{
					if(pServerNode)
						SAFE_DELETE(pServerNode);
					pServerNode = new HOST_NODE;
					ZeroMemory(pServerNode, sizeof(HOST_NODE));
					if(FAILED(pEnumHostsResponseMsg->pAddressSender->Duplicate(&pServerNode->pHostAddress)))
					{
						MessageBox(NULL,"Fehler beim Duplizieren der Server-Adresse beim Enummerieren der Hosts","Message",MB_OK | MB_ICONSTOP);
						SAFE_RELEASE(pServerNode->pHostAddress);
						SAFE_DELETE(pServerNode->pAppDesc);
						delete pServerNode;
					}
					pServerNode->pAppDesc = new DPN_APPLICATION_DESC;
					ZeroMemory(pServerNode->pAppDesc, sizeof(DPN_APPLICATION_DESC));
					memcpy(pServerNode->pAppDesc, pAppDesc, sizeof(DPN_APPLICATION_DESC));

					pServerNode->pAppDesc->pwszSessionName					= NULL;
					pServerNode->pAppDesc->pwszPassword						= NULL;
					pServerNode->pAppDesc->pvReservedData					= NULL;
					pServerNode->pAppDesc->dwReservedDataSize				= 0;
					pServerNode->pAppDesc->pvApplicationReservedData		= NULL;
					pServerNode->pAppDesc->dwApplicationReservedDataSize	= 0;
					MessageBox(NULL, pSessionName, "Message", MB_OK | MB_ICONINFORMATION);
				}
				/*else
				{
					MessageBox(NULL,"bereits in Liste","Message",MB_OK | MB_ICONSTOP);
				}*/
			}
			//MessageBox(NULL, pSessionName, "Message", MB_OK | MB_ICONINFORMATION);
			pServerNode->strSessionName = pSessionName;
			m_pServerNode = pServerNode;
			pServerNode = NULL;
			LeaveCriticalSection(&m_csDP);
			break;
		}
		

	case DPN_MSGID_CREATE_PLAYER:
		{
			PDPNMSG_CREATE_PLAYER	pCreatePlayerMsg;
			DWORD					dwSize				= 0;
			DPN_PLAYER_INFO*		pdpPlayerInfo		= NULL;

			pCreatePlayerMsg = (PDPNMSG_CREATE_PLAYER)pvMsgBuffer;

			hResult = m_pDP->GetPeerInfo(pCreatePlayerMsg->dpnidPlayer, pdpPlayerInfo, &dwSize, 0);

			if(FAILED(hResult) && (hResult != DPNERR_BUFFERTOOSMALL))
			{
				MessageBox(NULL,"Fehler beim Abfragen der Informationen des erstellten Spielers","Message",MB_OK | MB_ICONSTOP);
				return hResult;
			}
			pdpPlayerInfo = (DPN_PLAYER_INFO*) new BYTE[dwSize];
			ZeroMemory(pdpPlayerInfo, dwSize);
			pdpPlayerInfo->dwSize = sizeof(DPN_PLAYER_INFO);

			if(FAILED(m_pDP->GetPeerInfo(pCreatePlayerMsg->dpnidPlayer, pdpPlayerInfo, &dwSize, 0)))
			{
				MessageBox(NULL,"Fehler beim Holen der Informationen des erstellten Spielers","Message",MB_OK | MB_ICONSTOP);
				SAFE_DELETE_ARRAY(pdpPlayerInfo);
			}
			else {
				if(pdpPlayerInfo->dwInfoFlags & DPNPLAYER_LOCAL)
				{
					MessageBox(NULL,"erstellter Spieler","Message",MB_OK | MB_ICONINFORMATION);
				}
			}
            SAFE_DELETE_ARRAY(pdpPlayerInfo);
			break;
		}

	case DPN_MSGID_TERMINATE_SESSION:
		{
			*m_pbHostingApp = *m_pbConnected = false;
			break;
		}

		
	}
	return S_OK;
}

MyDPlay::MyDPlay(void)
{
	ZeroMemory(this, sizeof(MyDPlay));
	m_pDP				= NULL;
	m_pOwnAddress		= NULL;
	m_pServerAddress	= NULL;
	//m_pServerNode		= NULL;
	//m_pDPLobby			= NULL;
	m_pcPlayerName		= NULL;
	m_pcSessionName		= NULL;
	m_pcTCPAddress		= NULL;
	m_pbServer			= new bool(false);
	m_pbHostingApp		= new bool(false);
	m_pbConnected		= new bool(false);
	m_pdwPort			= new DWORD(8888);
	InitializeCriticalSection(&m_csDP);
}

MyDPlay::~MyDPlay(void)
{
	MyTools::releaseInterface(m_pDP);
	MyTools::releaseInterface(m_pOwnAddress);
	MyTools::releaseInterface(m_pServerAddress);
	
	MyTools::deleteObject(m_pServerNode);
	MyTools::deleteObject(m_pbServer);
	MyTools::deleteObject(m_pbHostingApp);
	MyTools::deleteObject(m_pbConnected);
	MyTools::deleteObject(m_pdwPort);
	//MyTools::deleteArray(m_pcPlayerName);
	//MyTools::deleteArray(m_pcSessionName);
	//MyTools::deleteArray(m_pcTCPAddress);
	//MyTools::releaseInterface(m_pDPLobby);

	//CoUninitialize();

	ZeroMemory(this, sizeof(MyDPlay));
}

/*Methode zur Initialisierung von DirectPlay*/
bool MyDPlay::init(HWND* givenHWnd, char* givenPlayerName, char* givenSessionName, char* givenTCPAddress, bool givenServer)
{
	HRESULT						hResult;
	//LPDIRECTPLAYLOBBYA			pDPLobby;	

	if(givenHWnd != NULL)
	{
		m_hWnd			= givenHWnd;
	}
	if(givenPlayerName != NULL)
	{
		m_pcPlayerName	= givenPlayerName;
	}
	if(givenSessionName != NULL)
	{
		m_pcSessionName	= givenSessionName;
	}
	if(givenTCPAddress != NULL)
	{
		m_pcTCPAddress	= givenTCPAddress;
	}

	*m_pbServer	= givenServer;

	//CoInitialize(NULL);
	
	hResult = CoCreateInstance(CLSID_DirectPlay8Peer, NULL,
								CLSCTX_INPROC_SERVER,
								IID_IDirectPlay8Peer,
								(LPVOID*) &m_pDP);
	if(FAILED(hResult))
	{
		//CoUninitialize();
		MessageBox(NULL,"Fehler bei der Erstellung der DirectPlay-Schnittstelle","ERROR",MB_OK | MB_ICONSTOP);
		SAFE_RELEASE(m_pDP);
		return false;
	}

	if(FAILED(m_pDP->Initialize(NULL, DPMessageProc, 0)))
	{
		MessageBox(NULL,"Fehler bei der Initialisierung von DirectPlay","ERROR",MB_OK | MB_ICONSTOP);
		SAFE_RELEASE(m_pDP);
		return false;
	}

	if(!this->checkServiceProvider(&CLSID_DP8SP_TCPIP))
	{
		return false;
	}

	/*//Erzeugung des LobbyObjects
	if(FAILED(DirectPlayLobbyCreate(NULL, &pDPLobby, NULL, NULL, 0)))
	{
		MessageBox(NULL,"Fehler bei der Erzeugung des LobbyObjects","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	if(FAILED(pDPLobby->QueryInterface(IID_IDirectPlayLobby2A, (LPVOID*) &m_pDPLobby)))
	{
		MessageBox(NULL,"Fehler bei der Anforderung der Lobby-Schnittstelle","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	MyTools::releaseInterface(pDPLobby);

	if(!this->openConnection())
	{
		MessageBox(NULL,"Fehler beim Aufbau der Connection","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}*/

	return true;
}

/*Methode zum Pruefen des ServiceProvider*/
bool MyDPlay::checkServiceProvider(const GUID* givenSPGUID)
{
	HRESULT hResult;
	DPN_SERVICE_PROVIDER_INFO* pSPInformation	= NULL;
	DWORD dwItems								= 0;
	DWORD dwSize								= 0;

	hResult = m_pDP->EnumServiceProviders(givenSPGUID, NULL, NULL, &dwSize, &dwItems, 0);

	if(FAILED(hResult) && hResult != DPNERR_BUFFERTOOSMALL)
	{
		MessageBox(NULL,"Fehler bei der Ueberpruefung des ServiceProvider","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	pSPInformation = (DPN_SERVICE_PROVIDER_INFO*) new BYTE[dwSize];

	if(FAILED(m_pDP->EnumServiceProviders(givenSPGUID, NULL, pSPInformation, &dwSize, &dwItems, 0)))
	{
		SAFE_DELETE_ARRAY(pSPInformation);
		MessageBox(NULL,"Fehler bei der Ueberpruefung des ServiceProvider","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	if(!dwItems)
	{
		MessageBox(NULL,"Es sind keine ServiceProvider auffindbar","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

    SAFE_DELETE_ARRAY(pSPInformation);
	return true;
}

/*Methode zum Erstellen der Adresse*/
bool MyDPlay::createOwnAddress(void)
{
	SAFE_RELEASE(m_pOwnAddress);

	if(FAILED(CoCreateInstance(CLSID_DirectPlay8Address,
								NULL,
								CLSCTX_INPROC_SERVER,
								IID_IDirectPlay8Address,
								(LPVOID*) &m_pOwnAddress)))
	{
		MessageBox(NULL,"Fehler beim Erstellen der Adresse","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	if(FAILED(m_pOwnAddress->SetSP(&CLSID_DP8SP_TCPIP)))
	{
		MessageBox(NULL,"Fehler beim Festlegen des Service Provider","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	if(*m_pbServer)
	{
		if(FAILED(m_pOwnAddress->AddComponent(DPNA_KEY_PORT, m_pdwPort, sizeof(DWORD), DPNA_DATATYPE_DWORD)))
		{
			MessageBox(NULL,"Fehler beim Festlegen des Ports","ERROR",MB_OK | MB_ICONSTOP);
			return false;
		}
	}

	return true;
}

/*Methode zum Erstellen der Adresse*/
bool MyDPlay::createServerAddress(void)
{
	WCHAR wcHostName[118] = {0};

	SAFE_RELEASE(m_pServerAddress);

	if(FAILED(CoCreateInstance(CLSID_DirectPlay8Address,
								NULL,
								CLSCTX_INPROC_SERVER,
								IID_IDirectPlay8Address,
								(LPVOID*) &m_pServerAddress)))
	{
		MessageBox(NULL,"Fehler beim Erstellen der Serveradresse","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	if(FAILED(m_pServerAddress->SetSP(&CLSID_DP8SP_TCPIP)))
	{
		MessageBox(NULL,"Fehler beim Festlegen des Serviceproviders","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}
//Hostname setzen
	DXUtil_ConvertGenericStringToWideCch(wcHostName, "localhost", 118);

	if(wcslen(wcHostName) > 0)
	{
		if(FAILED(m_pServerAddress->AddComponent(DPNA_KEY_HOSTNAME, wcHostName, 2 * ((UINT)wcslen(wcHostName) + 1), DPNA_DATATYPE_STRING)))
		{
			MessageBox(NULL,"Fehler beim Setzen des Hostnames","ERROR",MB_OK | MB_ICONSTOP);
			return false;
		}
	}

	if(*m_pdwPort != 0)
	{
		if(FAILED(m_pServerAddress->AddComponent(DPNA_KEY_PORT, m_pdwPort, sizeof(DWORD), DPNA_DATATYPE_DWORD)))
		{
			MessageBox(NULL,"Fehler beim Festlegen des Ports","ERROR",MB_OK | MB_ICONSTOP);
			return false;
		}
	}
	
	return true;
}

/*Methode zum Anlegen einer Session*/
bool MyDPlay::createSession(void)
{
	WCHAR wcSessionName[118];
	DPN_APPLICATION_DESC pAppDesc;

	DXUtil_ConvertGenericStringToWideCch(wcSessionName, m_pcSessionName, 118);

	if(!this->createOwnAddress())
	{
		return false;
	}

	ZeroMemory(&pAppDesc, sizeof(DPN_APPLICATION_DESC));
	pAppDesc.dwSize				= sizeof(DPN_APPLICATION_DESC);
	pAppDesc.guidApplication	= MyDPlayGUID;
	pAppDesc.pwszSessionName	= wcSessionName;
	pAppDesc.dwFlags			= DPNSESSION_NODPNSVR | DPNSESSION_MIGRATE_HOST;

	if(FAILED(m_pDP->Host(&pAppDesc, &m_pOwnAddress, 1, NULL, NULL, NULL, 0)))
	{
		MessageBox(NULL,"Fehler beim Hosten der Application","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	} else	{
		*m_pbHostingApp = true;
	}

	return true;
}

/*Methode zum Pruefen der verfuegbaren Server*/
bool MyDPlay::enumAvailServer(void)
{
	//TCHAR*					tcHost = new TCHAR[118];
	DPN_APPLICATION_DESC	pAppDesc;

	if(!this->createOwnAddress())
	{
		return false;
	}
	if(!this->createServerAddress())
	{
		return false;
	}

	ZeroMemory(&pAppDesc, sizeof(DPN_APPLICATION_DESC));
	pAppDesc.dwSize				= sizeof(DPN_APPLICATION_DESC);
	pAppDesc.guidApplication	= MyDPlayGUID;

	if(FAILED(m_pDP->EnumHosts(&pAppDesc, m_pServerAddress, m_pOwnAddress, NULL, 0, 4, 0, 0, NULL, NULL, DPNENUMHOSTS_SYNC)))
	{
		MessageBox(NULL,"Fehler beim Enummerieren der Hosts","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	return true;
}

/*Methode zum Verbinden zu einer Session*/
bool MyDPlay::connectSession(void)
{
	DPN_APPLICATION_DESC	pAppDesc = {0};
	HOST_NODE*				pServerNode;
	IDirectPlay8Address*	pServerAddress;

	if(!this->createOwnAddress())
	{
		return false;
	}

	ZeroMemory(&pAppDesc, sizeof(DPN_APPLICATION_DESC));
	pAppDesc.dwSize				= sizeof(DPN_APPLICATION_DESC);
	pAppDesc.guidApplication	= MyDPlayGUID;

	//ServerNode verarbeiten
	pServerNode = m_pServerNode;

	if(pServerNode)
	{
		if(SUCCEEDED(pServerNode->pHostAddress->Duplicate(&pServerAddress)))
		{
			if(SUCCEEDED(m_pDP->Connect(&pAppDesc, pServerAddress, m_pOwnAddress, NULL, NULL, NULL, 0, NULL, NULL, NULL, DPNCONNECT_SYNC)))
			{
				*m_pbConnected = true;
				//SessionName;
			}
			else
			{
				MessageBox(NULL,"Fehler beim Connect zur Session","ERROR",MB_OK | MB_ICONSTOP);
				return false;
			}
		}
		else
		{
			MessageBox(NULL,"Fehler beim Duplizieren der Hostadresse","ERROR",MB_OK | MB_ICONSTOP);
			return false;
		}
	}

	SAFE_RELEASE(pServerAddress);
	
	return true;
}

/*Methode zum Oeffnen der Verbindung*/
bool MyDPlay::openConnection(void)
{
	/*DPCOMPOUNDADDRESSELEMENT	aAddress[2];
	DWORD						dwAddressSize = 0;
	LPVOID						lpConnection;

	aAddress[0].guidDataType	= DPAID_ServiceProvider;
	aAddress[0].dwDataSize		= sizeof(GUID);
	aAddress[0].lpData			= (LPVOID) &DPSPGUID_TCPIP;

	aAddress[1].guidDataType	= DPAID_INet;
	aAddress[1].dwDataSize		= lstrlen(m_pcTCPAddress) + 1;
	aAddress[1].lpData			= m_pcTCPAddress;

	m_pDPLobby->CreateCompoundAddress(aAddress, 2, NULL, &dwAddressSize);

	lpConnection = GlobalAllocPtr(GHND, dwAddressSize);

	if(FAILED(m_pDP->InitializeConnection(lpConnection, 0)))
	{
		//MessageBox(NULL,"Fehler beim Aufbau der Verbindung","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	GlobalFreePtr(lpConnection);
	*/
	return true;
}

/*Methode zum Schliessen der Verbindung*/
bool MyDPlay::closeConnection(void)
{
	if(FAILED(m_pDP->Close(0)))
	{
		MessageBox(NULL,"Fehler beim Schliessen der Verbindung","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}
	return true;
}
