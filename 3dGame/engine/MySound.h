#pragma once
#ifndef MYSOUND_H
#define MYSOUND_H

#include "includes.h"
#include "MyDSound.h"

class MySound
{
public:
	MySound(void);
	~MySound(void);
	bool init(LPDIRECTSOUND8 givenPDirectSound,char* filename,DWORD soundFlags);
	bool play(bool givenLoop,DWORD givenPosition);
	bool stop();
	bool set3DSoundDistance(float givenMinDistance,float givenMaxDistance);
	bool set3DSoundPosition();
	bool set3DSoundVelocity();
private:	
	//Pointer auf sekundaeren SoundBuffer
	LPDIRECTSOUNDBUFFER8	m_pSecondarySoundBuffer;
	//Pointer auf 3D-Soundbuffer
	LPDIRECTSOUND3DBUFFER8	m_p3DSoundBuffer;
	//Format des Wavefiles
	WAVEFORMATEX*		m_psWaveFormat;

	void*					m_pData;
	
	DWORD					m_dwFrequency;
	DWORD					m_dwBlockAlign;
	DWORD*					m_pdwDataSize;

	// Methode zum Laden eines WAV-Files
	bool loadWAVFile(char* filename);
	//Methode zur Initialisierung des Sekundaerbuffers
	bool initSecondarySoundBuffer(LPDIRECTSOUND8 givenPDirectSound,DWORD soundFlags);
};

#endif