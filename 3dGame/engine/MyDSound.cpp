#include "MyDSound.h"

MyDSound::MyDSound(void)
{
	ZeroMemory(this,sizeof(MyDSound));
	m_pDirectSound			= NULL;
	m_pPrimarySoundBuffer	= NULL;
	m_p3DListener			= NULL;
}

MyDSound::~MyDSound(void)
{
	MyTools::releaseInterface(m_p3DListener);
	MyTools::releaseInterface(m_pPrimarySoundBuffer);
	MyTools::releaseInterface(m_pDirectSound);
	ZeroMemory(this,sizeof(MyDSound));
}

bool MyDSound::init(HWND* givenHWND)
{
	DSBUFFERDESC bufferDesc;
	
	//Konfiguration der Struktur fuer primaeren SoundBuffer
	bufferDesc.dwSize			= sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags			= DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes	= 0;
	bufferDesc.dwReserved		= 0;
	bufferDesc.lpwfxFormat		= NULL;
	bufferDesc.guid3DAlgorithm	= GUID_NULL; //???Algorithmus setzen

	//DirectSound-Schnittstelle erzeugen - es wird Standardgeraet verwendet
	if(FAILED(DirectSoundCreate8(&DSDEVID_DefaultPlayback,&m_pDirectSound,NULL))) {
		return false;
	}
	//Setzen der Kooperationsebene
	m_pDirectSound->SetCooperativeLevel(*givenHWND,DSSCL_PRIORITY);

	//Erzeugung des Primaeren SoundBuffers
	if(FAILED(m_pDirectSound->CreateSoundBuffer(&bufferDesc,&m_pPrimarySoundBuffer,NULL))) {
		return false;
	}

	//Abfrage des 3D-Listener-Interfaces
	if(FAILED(m_pPrimarySoundBuffer->QueryInterface(IID_IDirectSound3DListener8,(void**)&m_p3DListener)))
	{
		return false;
	}

	return true;
}

LPDIRECTSOUND8 MyDSound::getDSound(void)
{
	return m_pDirectSound;
}
