//-------------------------------------------------------------------------
// SunMsgSender.cpp : Defines the exported functions for the DLL application.
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "SunMsgSender.h"
#include  <Winsock2.h> 
#include "SunSocketClient.h"
//-------------------------------------------------------------------------
#pragma comment(lib,"ws2_32.lib")   
//-------------------------------------------------------------------------
CSunSocketClient*	g_pClientSocket = NULL;
//-------------------------------------------------------------------------



//-------------------------------------------------------------------------
SUNMSGSENDER_API int SMS_Init(void)
{
	WORD wVersionRequested; 
	WSADATA wsaData; 
	int iError; 

	wVersionRequested = MAKEWORD(1, 1); 
	iError = WSAStartup(wVersionRequested, &wsaData); 
	if (0 != iError)                                      
		return iError; 

	if (1 != LOBYTE(wsaData.wVersion) || 1 != HIBYTE(wsaData.wVersion))   
	{ 
		WSACleanup(); 
		return wsaData.wVersion;   
	} 

	g_pClientSocket = new CSunSocketClient();

	return 0;
}
//-------------------------------------------------------------------------
SUNMSGSENDER_API int SMS_ConnectServer(const char* lpszHost, int iPort)
{
	if (NULL != g_pClientSocket)
	{
		return g_pClientSocket->ConnectServer(lpszHost, iPort);
	}

	return -1;
}
//-------------------------------------------------------------------------
SUNMSGSENDER_API int SMS_SendMsg(const char* lpszMsg, int iMsgLen)
{
	if (NULL != g_pClientSocket)
	{
		return g_pClientSocket->SendMsg(lpszMsg, iMsgLen);
	}

	return -1;
}
//-------------------------------------------------------------------------
SUNMSGSENDER_API int SMS_Disconnect(void)
{
	if (NULL != g_pClientSocket)
	{
		return g_pClientSocket->Disconnect();
	}

	return -1;
}
//-------------------------------------------------------------------------
SUNMSGSENDER_API int SMS_Exit(void)
{
	WSACleanup(); 
	return 0;
}
//-------------------------------------------------------------------------
