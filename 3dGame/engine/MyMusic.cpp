#include "MyMusic.h"

MyMusic::MyMusic(void)
{
	ZeroMemory(this,sizeof(MyMusic));
	m_pDSGraph			= NULL;
	m_pDSMediaControl	= NULL;
	m_pDSMediaSeeking	= NULL;
	m_pDSBasicAudio		= NULL;
	m_pbIsPlaying		= new bool(false);
	m_pbWantLooping		= new bool(false);
	m_pllTime			= new LONGLONG(0);
}

MyMusic::~MyMusic(void)
{
	if(*m_pbIsPlaying)
	{
		this->stop();
	}
	//Freigabe der Schnittstellen
	MyTools::releaseInterface(m_pDSBasicAudio);
	MyTools::releaseInterface(m_pDSMediaSeeking);
	MyTools::releaseInterface(m_pDSMediaControl);
	MyTools::releaseInterface(m_pDSGraph);	

	//Freigeben des Speichers der lokalen Variablen
	MyTools::deleteObject(m_pbIsPlaying);
	MyTools::deleteObject(m_pbWantLooping);
	MyTools::deleteObject(m_pllTime);
	MyTools::deleteObject(&m_Filename);

	//CoUninitialize();

	ZeroMemory(this,sizeof(MyMusic));
}

/*Methode zur Initialisierung von DirectShow zum Abspielen von Musik*/
bool MyMusic::init(LPCTSTR givenFilename,bool givenWantLooping)
{
	HRESULT		hResult;
	WCHAR		awcFilename[1024];	

	if(givenFilename != NULL)
	{
		m_Filename = givenFilename;
	}
	
	*m_pbWantLooping = givenWantLooping;

	//CoInitialize(NULL);
	//Generierung des DirectShow-Graphen
	hResult = CoCreateInstance(	CLSID_FilterGraph,
								NULL,
								CLSCTX_INPROC_SERVER,
								IID_IGraphBuilder,
								(void**)&m_pDSGraph);
	if(FAILED(hResult))
	{
		//CoUninitialize();
		MessageBox(NULL,"Fehler bei der Erstellung des Filtergraphen","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}
	//Umwandlung des Dateinamen in Unicode
	MultiByteToWideChar(CP_ACP,0,m_Filename,-1,awcFilename,1024);
	//File rendern
	if(FAILED(m_pDSGraph->RenderFile(awcFilename,NULL)))
	{
		MessageBox(NULL,"Fehler beim Rendern des Musicfiles","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}
	//Schnittstellen zur Kontrolle der Music erstellen
	if(FAILED(m_pDSGraph->QueryInterface(IID_IMediaControl,(void**)(&m_pDSMediaControl))))
	{
		MessageBox(NULL,"Fehler beim Erstellen der MediaControl-Schnittstelle","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}
	if(FAILED(m_pDSGraph->QueryInterface(IID_IMediaSeeking,(void**)(&m_pDSMediaSeeking))))
	{
		MessageBox(NULL,"Fehler beim Erstellen der MediaSeeking-Schnittstelle","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}
	if(FAILED(m_pDSGraph->QueryInterface(IID_IBasicAudio,(void**)(&m_pDSBasicAudio))))
	{
		MessageBox(NULL,"Fehler beim Erstellen der BasicAudio-Schnittstelle","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	//Timeformat setzen
	m_pDSMediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);

	//Dauer bestimmen
	m_pDSMediaSeeking->GetDuration(m_pllTime);
	return true;
}

/*Methode zum Starten eines Musikstuecks*/
bool MyMusic::play()
{
	if(this->setActualPosition(0))
	{
		if(FAILED(m_pDSMediaControl->Run()))
		{
			MessageBox(NULL,"Fehler beim Starten der Music","ERROR",MB_OK | MB_ICONSTOP);
			return false;
		}
		*m_pbIsPlaying = true;
	}
	return true;
}

/*Methode zum Pausieren eines Musikstuecks*/
bool MyMusic::pause()
{
	if(FAILED(m_pDSMediaControl->Pause()))
	{
		MessageBox(NULL,"Fehler beim Pausieren der Music","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}
	return true;
}

/*Methode zum Stoppen eines Musikstuecks*/
bool MyMusic::stop()
{
	if(FAILED(m_pDSMediaControl->Stop())) {
		MessageBox(NULL,"Fehler beim Stoppen der Music","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}
	*m_pbIsPlaying = false;
	return true;
}

/**Methode zur Ermittlung der aktuellen Position des Musikstuecks*/
LONGLONG MyMusic::getActualPosition()
{
	LONGLONG llPosition;
	if(FAILED(m_pDSMediaSeeking->GetCurrentPosition(&llPosition)))
	{
		MessageBox(NULL,"Fehler beim Abfragen der Position der Music","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}
	return llPosition;
}

/**Methode zum Setzen der aktuellen Position des Musikstuecks*/
bool MyMusic::setActualPosition(LONGLONG llPosition)
{
	if(FAILED(m_pDSMediaSeeking->SetPositions(&llPosition,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning)))
	{
		MessageBox(NULL,"Fehler beim Setzen der Position der Music","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}
	return true;
}

/**Methode zur Presentation der Music*/
bool MyMusic::presentMusic()
{
	if(*m_pbIsPlaying)
	{
		if(this->getActualPosition() >= *m_pllTime)
		{
			if(*m_pbWantLooping)
			{
				return this->play();
			}
		}
		else
		{
			*m_pbIsPlaying=false;
		}
	}
	return true;
}