#pragma once
#ifndef MYDPLAY_H
#define MYDPLAY_H

#include "includes.h"
#include "MyMesh.h"
#include "MyToken.h"
#include "MyPlayer.h"
#include "MySound.h"

// {BF551F1B-956D-4b81-B639-D1105A254F32}
//DEFINE_GUID(MyDPlayGUID, 0xbf551f1b, 0x956d, 0x4b81, 0xb6, 0x39, 0xd1, 0x10, 0x5a, 0x25, 0x4f, 0x32);
// {336C8C64-01A5-489a-9998-1319EFCFC4B1}
static const GUID MyDPlayGUID = 
	{ 0x336c8c64, 0x1a5, 0x489a, { 0x99, 0x98, 0x13, 0x19, 0xef, 0xcf, 0xc4, 0xb1 } };

static char*			PLANETSOUND	= "resources/wav_files/Sound1.wav";
static char*			SHIPSOUND		= "resources/wav_files/Sound2.wav";

typedef struct HOST_NODE
{
    DPN_APPLICATION_DESC*   pAppDesc;
    IDirectPlay8Address*    pHostAddress;
    TCHAR*                  strSessionName;

    //HOST_NODE*             pNext;
} HOST_NODE;

class MyDPlay
{
public:
	MyDPlay(void);
	~MyDPlay(void);
	bool init(char* givenTCPAddress, DWORD* givenPort, bool givenServer);
	bool createSession(void);
	bool connectSession(void);
	bool openConnection(void);
	bool enumAvailServer(void);
	bool sendMessage(void* givenToken, int choice);
	bool closeConnection(void);
	void set3DInstance(LPDIRECT3DDEVICE9 givenDevice, D3DXMATRIX* givenWorld);
	void setSoundInstance(LPDIRECTSOUND8 givenDevice);
	static HRESULT WINAPI DPMessageProc(PVOID pvUserContext, DWORD dwMessageId, PVOID pvMsgBuffer);
	static bool*					m_pbHostingApp;
	static bool*					m_pbConnected;
	static DPNID					m_pid;
	static vector<MyMesh*>			m_pAllMeshes;		//Alle Meshes
	static vector<MyMesh*>			m_pMasterMeshes;	//Alle ohne Master
	static MyPlayer*				m_pLocalPlayer;		//eigener Spieler
	static vector<MyPlayer*>		m_pNetworkPlayers;	//andere Teilnehmer
	static vector<MyMesh*>			m_pNetworkMeshes;   //Meshes vom Server aktualisiert
	static vector<MyMesh*>			m_pLocalMeshes;		//verantwortliche Meshes
	static vector<MySound*>			m_pMeshSounds;		//Sounds der Meshes
	static vector<LPCTSTR>			m_pMeshPaths;		//Path der Meshes
	static TCHAR*					m_pUsername;
	static CRITICAL_SECTION			m_csDP;
	static int						m_iFrameRate;
	static HWND*					m_hWnd;
	static bool						m_bInitialized;
private:
	static LPDIRECT3DDEVICE9	_D3DDevice;
	static D3DXMATRIX*			_matWorld;
	static LPDIRECTSOUND8		_DSoundDevice;
	bool checkServiceProvider(const GUID* givenSPGUID);
	bool createOwnAddress(void);
	bool createServerAddress(void);
	static IDirectPlay8Peer*		m_pDP;
	IDirectPlay8Address*			m_pOwnAddress;
	IDirectPlay8Address*			m_pServerAddress;
	static HOST_NODE*				m_pServerNode;
	DWORD*							m_pdwPort;
	//LPDIRECTPLAYLOBBY2A	m_pDPLobby;
	//HWND*							m_hWnd;
	bool*							m_pbServer;
	//char*							m_pcPlayerName;
	//char*							m_pcSessionName;
	char*							m_pcTCPAddress;
};

#endif