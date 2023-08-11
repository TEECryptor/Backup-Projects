//-------------------------------------------------------------------------
#include "StdAfx.h"
#include "SunSocketClient.h"
//-------------------------------------------------------------------------
CSunSocketClient::CSunSocketClient(void)
 : m_ClientSocket(NULL)
{
}
//-------------------------------------------------------------------------
CSunSocketClient::~CSunSocketClient(void)
{
	Disconnect();
}
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
int CSunSocketClient::ConnectServer(const char* lpszHost, int iPort)
{
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 

	SOCKADDR_IN addrSrv; 
	memset(&addrSrv, 0, sizeof(addrSrv));
	addrSrv.sin_addr.S_un.S_addr = inet_addr(lpszHost); 
	addrSrv.sin_family = AF_INET; 
	addrSrv.sin_port = htons(iPort); 

	int iError = connect(m_ClientSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (SOCKET_ERROR == iError)
	{
		closesocket(m_ClientSocket); 
		m_ClientSocket = NULL;
		return WSAGetLastError();
	}
/*
	char csBuf[32] = {0};
	recv(m_ClientSocket, csBuf, 32, 0);
	if (stricmp(csBuf, "OK") != 0)
	{
		closesocket(m_ClientSocket); 
		m_ClientSocket = NULL;
		return WSAGetLastError();
	}
*/
	return 0;
}
//-------------------------------------------------------------------------
int	CSunSocketClient::SendMsg(const char* lpszMsg, int iMsgLen)
{
	if (SOCKET_ERROR == send(m_ClientSocket, lpszMsg, iMsgLen, 0))
	{
		return WSAGetLastError();
	}

	return 0;
}
//-------------------------------------------------------------------------
int CSunSocketClient::Disconnect(void)
{
	if (NULL != m_ClientSocket)
	{
		closesocket(m_ClientSocket); 
		m_ClientSocket = NULL;
	}
	return 0;
}
//-------------------------------------------------------------------------
