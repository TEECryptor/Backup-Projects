//-----------------------------------------------------------
//	Name:ServerSocket.cpp
//	Introduce:implementation file
//	Creator:Youfang Yao
//	Date:11/21/05
//	Version:1.0v
//-----------------------------------------------------------
#include "stdafx.h"
#include "ServerSocket.h"
//-----------------------------------------------------------
//	Name:CServerSocket
//	Introduce:Constructor
//-----------------------------------------------------------
CServerSocket::CServerSocket()
 : m_hMsgWnd(NULL)
{
}
//-----------------------------------------------------------
//	Name:~CServerSocket
//	Introduce:Distructor
//-----------------------------------------------------------
CServerSocket::~CServerSocket()
{
	CSocket* pClient = NULL;
	for(int iCnt = 0; iCnt < m_lstClient.size(); iCnt++)
	{
		pClient = m_lstClient[iCnt];
		if(NULL != pClient)
		{
			delete pClient;
			pClient = NULL;
		}
	}
	m_lstClient.clear();
}
//-----------------------------------------------------------
// Do not edit the following lines, which are needed by ClassWizard.
//-----------------------------------------------------------
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0
//-----------------------------------------------------------



//-----------------------------------------------------------
//*	CServerSocket member functions */
//-----------------------------------------------------------
//	Name:Create
//	Introduce:To create a socket as server
//-----------------------------------------------------------
BOOL CServerSocket::Create(HWND hMsgWnd, UINT nSocketPort, int nSocketType, LPCTSTR lpszSocketAddress)
{
	if(NULL == hMsgWnd)
		return FALSE;
	m_hMsgWnd = hMsgWnd;

	if(!AfxSocketInit()) 
		return FALSE;

	if(CSocket::Create(nSocketPort, nSocketType, lpszSocketAddress))
	{
		if(CSocket::Listen()) 
			return TRUE;
	}

	return FALSE;
}
//-----------------------------------------------------------
//	Name:OnAccept
//	Intrudce:The function is called when a new client socket have connected
//-----------------------------------------------------------
void CServerSocket::OnAccept(int nErrorCode) 
{
	CClientSocket* pNewClient = new CClientSocket(m_hMsgWnd);
	if(CSocket::Accept(*pNewClient))
	{
		//Add the new client to list
		m_lstClient.push_back(pNewClient);

		//Send the message to message handle window
		int iIndex = m_lstClient.size() - 1;
		PostMessage(m_hMsgWnd, SKT_MSG_CLIENTCONNECTED, (WPARAM)iIndex, (LPARAM)0);
	}
	else
	{
		delete pNewClient;
		pNewClient = NULL;
	}
	
	CSocket::OnAccept(nErrorCode);
}
//-----------------------------------------------------------
//	Name:SendData
//	Introduce:To send a message to specific client socket or to all clients
//-----------------------------------------------------------
BOOL CServerSocket::SendData(LPVOID lpBuff, int iBuffLen, int iClientID, int iFlags)
{
	if(NULL == lpBuff || iBuffLen <= 0)
		return FALSE;

	int iCnt = m_lstClient.size();
	if(	((iClientID < 0) && (-1 != iClientID)) || 
		(iClientID >= (int)m_lstClient.size()) )
		return FALSE;

	//Send the data to all clients
	if(-1 == iClientID)
	{
		for(int iCnt = 0; iCnt < m_lstClient.size(); iCnt++)
		{
			m_lstClient[iCnt]->Send(lpBuff, iBuffLen, iFlags);
		}
	}
	//Send the data only to iClientID
	else
	{
		m_lstClient[iClientID]->Send(lpBuff, iBuffLen, iFlags);
	}

	return TRUE;
}
//-----------------------------------------------------------