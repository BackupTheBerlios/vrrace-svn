#pragma once
#ifndef MYDSOUND_H
#define MYDSOUND_H

#include "includes.h"

class MyDSound
{
public:
	MyDSound(void);
	~MyDSound(void);
	// Funktion zur Initialisierung von DirectSound
	bool init(HWND* givenHWND);
	// Funktion zum Zugriff auf IDirect8-Schnittstelle
	LPDIRECTSOUND8 getDSound(void);
	// Funktion zum Setzen der Position des Hoerers
	bool setListenerPosition(float x, float y, float z);
	// Funktion zum Setzen der Geschwindigkeit des Hoerers
	bool setListenerDirection(float x, float y, float z);
	// Funktion zum Setzen der Orientierung des Hoerers
	bool setListenerOrientation(float vpX, float vpY, float vpZ, float uvX, float uvY, float uvZ);
private:
	// Pointer auf IDirect8-Schnittstelle
	LPDIRECTSOUND8				m_pDirectSound;
	// Pointer auf primaeren SoundBuffer
	LPDIRECTSOUNDBUFFER			m_pPrimarySoundBuffer;
	// Pointer auf Hoererschnittstelle
	LPDIRECTSOUND3DLISTENER8	m_p3DListener;
};

//Struktur fuer Header einer RIFF-Datei
typedef struct SRiffHeader {
	char	acRiff[4];
	DWORD	dwSize;
	char	acFormat[4];
} SRiffHeader;

//WAVE-Chunk-Header
typedef struct SWaveChunkHeader {
	char	acType[4];
	DWORD	dwDataSize;
} SWaveChunkHeader;

#endif