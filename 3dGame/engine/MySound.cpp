#include "MySound.h"

MySound::MySound(void)
{
	ZeroMemory(this,sizeof(MySound));
	m_pSecondarySoundBuffer	= NULL;
	m_dwFrequency			= 44100;
	m_dwBlockAlign			= 4;
	m_pdwDataSize			= new DWORD(0);
	m_psWaveFormat			= new WAVEFORMATEX();
}

MySound::~MySound(void)
{
	MyTools::deleteObject(m_pdwDataSize);
	MyTools::deleteObject(m_psWaveFormat);
	MyTools::releaseInterface(m_pSecondarySoundBuffer);
	ZeroMemory(this,sizeof(MySound));
}

/*Funktion zur Initialisierung von MySound*/
bool MySound::init(LPDIRECTSOUND8 givenPDirectSound,char* filename,DWORD soundFlags)
{
	if(!this->loadWAVFile(filename))
	{
		return false;
	}
	if(!this->initSecondarySoundBuffer(givenPDirectSound,soundFlags))
	{
		return false;
	}
	return true;
}

/*Methode zum Laden eines WAV-Files*/
bool MySound::loadWAVFile(char* filename)
{
	FILE*				pFile			= NULL;
	SRiffHeader			riffHeader;
	SWaveChunkHeader	chunkHeader;
		
	//Datei zum Lesen oeffnen
	pFile = fopen(filename,"rb");
	if(pFile == NULL) {
		MessageBox(NULL,"Fehler beim Oeffnen des WAVE-Files","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	//Lesen des RIFF-Headers und Ueberpruefung auf Korrektheit
	fread(&riffHeader,sizeof(SRiffHeader),1,pFile);
	if(strnicmp(riffHeader.acRiff,"RIFF",4)) {
		MessageBox(NULL,"Ungueltiger Riff-Header innerhalb des WAVE-Files","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}
	else if(strnicmp(riffHeader.acFormat,"WAVE",4)) {
		MessageBox(NULL,"Ungueltiges Format des WAVE-Files","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	//Chunks einlesen
	while(TRUE) {
		//zunaechst Chunk-Header lesen
		//falls Lesefehler
		if(fread(&chunkHeader,sizeof(SWaveChunkHeader),1,pFile)!=1) {
			break;
		}
		else {
			//Differenzierung zwischen verschiedenen Typen von Chunks
			if(!strnicmp(chunkHeader.acType,"FMT ",4)) {
				//falls Format-Chunk, Groesse pruefen
				if(chunkHeader.dwDataSize > sizeof(WAVEFORMATEX)) {
					fclose(pFile);
					MessageBox(NULL,"Fehler bei der Groessenangabe innerhalb des WAVE-Files","ERROR",MB_OK | MB_ICONSTOP);
					return false;
				}
				//Lesen der Daten
				ZeroMemory(m_psWaveFormat,sizeof(WAVEFORMATEX));
				fread(m_psWaveFormat,1,chunkHeader.dwDataSize,pFile);
				
				//Lesen der Frequenz und Anzahl Bytes pro Block
				m_dwFrequency = m_psWaveFormat->nSamplesPerSec;
				m_dwBlockAlign = m_psWaveFormat->nBlockAlign;
			}
			else if(!strnicmp(chunkHeader.acType,"DATA",4)) {
				//DataChunk: Speicher anfordern und lesen Daten
				m_pData = calloc(chunkHeader.dwDataSize,1);
				if(m_pData == NULL) {
					MessageBox(NULL,"Fehler beim Lesen des WAVE-Files","ERROR",MB_OK | MB_ICONSTOP);
					return false;
				}

				//lesen
				fread(m_pData,1,chunkHeader.dwDataSize,pFile);
				//zuweisen Groesse
				*m_pdwDataSize = chunkHeader.dwDataSize;
			}
			else {
				//unknown Chunk
				fseek(pFile,chunkHeader.dwDataSize,SEEK_CUR);
			}
		}
	}

	//Datei schliessen
	fclose(pFile);

	return true;
}

/*Methode zur Initialisierung des Sekundaerbuffers*/
bool MySound::initSecondarySoundBuffer(LPDIRECTSOUND8 givenPDirectSound,DWORD soundFlags)
{
	DSBUFFERDESC		bufferDesc;
	LPDIRECTSOUNDBUFFER	pTemp;
	void*				pSoundBuffer	= NULL;
	DWORD				dwNumBytes		= 0;

	//Konfiguration der Struktur fuer SoundBuffer
	bufferDesc.dwSize			= sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags			= soundFlags;
	bufferDesc.dwBufferBytes	= *m_pdwDataSize;
	bufferDesc.dwReserved		= 0;
	bufferDesc.lpwfxFormat		= m_psWaveFormat;
	bufferDesc.guid3DAlgorithm	= GUID_NULL;

	//erstellen des SoundBuffers
	if(FAILED(givenPDirectSound->CreateSoundBuffer(&bufferDesc,&pTemp,NULL))) {
		MessageBox(NULL,"Fehler beim Erzeugen des sekundaeren Soundbuffers","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	//Schnittstelle abfragen
	pTemp->QueryInterface(IID_IDirectSoundBuffer8,(void**)(&m_pSecondarySoundBuffer));
	//Freigeben der Schnittstelle
	MyTools::releaseInterface(pTemp);
	//3D-Soundschnittstelle abfragen
	m_pSecondarySoundBuffer->QueryInterface(IID_IDirectSound3DBuffer8,(void**)(&m_p3DSoundBuffer));
	
	//Soundpuffer sperren
	if(FAILED(m_pSecondarySoundBuffer->Lock(0,0,&pSoundBuffer,&dwNumBytes,NULL,NULL,DSBLOCK_ENTIREBUFFER))) {
		free(m_pData);
		MessageBox(NULL,"Fehler beim Sperren des Soundbuffers","ERROR",MB_OK | MB_ICONSTOP);
		return false;
	}

	//Kopieren der Daten
	memcpy(pSoundBuffer,m_pData,*m_pdwDataSize);
	free(m_pData);

	//Freigabe des Soundpuffer
	m_pSecondarySoundBuffer->Unlock(pSoundBuffer,dwNumBytes,NULL,NULL);
	return true;
}

/*Methode zum Zugriff auf SoundBuffer*/
LPDIRECTSOUNDBUFFER8* MySound::getSecondarySoundBuffer(void)
{
	return &m_pSecondarySoundBuffer;
}
