#pragma once
#ifndef MYDPLAY_H
#define MYDPLAY_H

#include "includes.h"

// {BF551F1B-956D-4b81-B639-D1105A254F32}
//DEFINE_GUID(MyDPlayGUID, 0xbf551f1b, 0x956d, 0x4b81, 0xb6, 0x39, 0xd1, 0x10, 0x5a, 0x25, 0x4f, 0x32);
// {336C8C64-01A5-489a-9998-1319EFCFC4B1}
static const GUID MyDPlayGUID = 
	{ 0x336c8c64, 0x1a5, 0x489a, { 0x99, 0x98, 0x13, 0x19, 0xef, 0xcf, 0xc4, 0xb1 } };

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
	bool init(HWND* givenHWnd, char* givenPlayerName, char* givenSessionName, char* givenTCPAddress, bool givenServer);
	bool createSession(void);
	bool connectSession(void);
	bool openConnection(void);
	bool enumAvailServer(void);
	bool closeConnection(void);
	static HRESULT WINAPI DPMessageProc(PVOID pvUserContext, DWORD dwMessageId, PVOID pvMsgBuffer);
private:
	bool checkServiceProvider(const GUID* givenSPGUID);
	bool createOwnAddress(void);
	bool createServerAddress(void);
	static IDirectPlay8Peer*		m_pDP;
	IDirectPlay8Address*			m_pOwnAddress;
	IDirectPlay8Address*			m_pServerAddress;
	static CRITICAL_SECTION			m_csDP;
	static HOST_NODE*				m_pServerNode;
	DWORD*							m_pdwPort;
	//LPDIRECTPLAYLOBBY2A	m_pDPLobby;
	HWND*							m_hWnd;
	bool*							m_pbServer;
	static bool*					m_pbHostingApp;
	static bool*					m_pbConnected;
	char*							m_pcPlayerName;
	char*							m_pcSessionName;
	char*							m_pcTCPAddress;
};

#endif