#pragma once
#ifndef MYMUSIC_H
#define MYMUSIC_H

//#include <dshow.h>
//#include "DShowBaseClasses\\Streams.h"
#include "includes.h"

class MyMusic
{
public:
	MyMusic(void);
	~MyMusic(void);
	bool init(LPCTSTR givenFilename,bool givenWantLooping);
	bool play();
	bool pause();
	bool stop();
	LONGLONG getActualPosition();
	bool setActualPosition(LONGLONG llPosition);
	bool presentMusic();
	bool setVolume(long givenVolume);
private:
	// Variable zur Speicherung Pointer auf Graph von DirectShow
	IGraphBuilder*	m_pDSGraph;
	// Variable zur Speicherung Pointer auf MediaControl-Schnittstelle
	IMediaControl*	m_pDSMediaControl;
	// Variable zur Speicherung Pointer auf MediaSeeking-Schnittstelle
	IMediaSeeking*	m_pDSMediaSeeking;
	// Variable zur Speicherung Pointer auf BasicAudio-Schnittstelle
	IBasicAudio*	m_pDSBasicAudio;
	// Variable zur Speicherung des Dateinamens
	LPCTSTR			m_Filename;
	// Variable zur Speicherung des Status
	bool*			m_pbIsPlaying;
	// Variable zur Speicherung Looping-Status
	bool*			m_pbWantLooping;
	// Variable zur Speicherung der Dauer des Musikstuecks
	LONGLONG*		m_pllTime;
};
#endif