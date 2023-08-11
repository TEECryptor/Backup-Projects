//--------------------------------------------------------------------
//	Name:ClientSocket.cpp
//	Introduce:implementation file
//	Creator:Youfang Yao
//	Date:11/21/05
//	Version:1.0v
//--------------------------------------------------------------------
#include "stdafx.h"
#include "ClientSocket.h"
//--------------------------------------------------------------------
//	Name:CClientSocket
//	Introduce:Constructor
//--------------------------------------------------------------------
CClientSocket::CClientSocket(HWND hMsgWnd)
 : m_uClientID(0),
   m_hMsgWnd(hMsgWnd)
{
	ASSERT(NULL != m_hMsgWnd);
	memset(m_nMsgDataBuff, 0, MAX_SOCKETDATA_LEN);
}
//--------------------------------------------------------------------
//	Name:~CClientSocket
//	Introduce:Distructor
//--------------------------------------------------------------------
CClientSocket::~CClientSocket()
{
}
//--------------------------------------------------------------------
// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0
//--------------------------------------------------------------------


//--------------------------------------------------------------------
/* CClientSocket member functions */
//--------------------------------------------------------------------
//	Name:Create
//	Introduce:To create a socket as client
//--------------------------------------------------------------------
BOOL CClientSocket::Create(UINT uClientID, UINT nSocketPort, int nSocketType, LPCTSTR lpszSocketAddress)
{
	if(!AfxSocketInit()) 
		return FALSE;

	m_uClientID = uClientID;

	if(CSocket::Create(nSocketPort, nSocketType, lpszSocketAddress))
		return TRUE;

	return FALSE;
}
//--------------------------------------------------------------------
//	Name:OnReceive
//	Introduce:To receive the data from server socket
//--------------------------------------------------------------------
void CClientSocket::OnReceive(int nErrorCode) 
{
	memset(m_nMsgDataBuff, 0, MAX_SOCKETDATA_LEN);

	int iReadLen = CSocket::Receive(m_nMsgDataBuff, MAX_SOCKETDATA_LEN);
	
	//Receive data from server error
	if(iReadLen <= 0 || iReadLen > MAX_SOCKETDATA_LEN)
	{
		TRACE("iReadLen = %d is error!", iReadLen);
		return;
	}

	//Send the message data to handle window to deteal with
	PostMessage(m_hMsgWnd, SKT_MSG_RECEIVEDMSG, (WPARAM)m_nMsgDataBuff, (LPARAM)iReadLen);
	
	CSocket::OnReceive(nErrorCode);
}
//--------------------------------------------------------------------
//	Name:SendData	
//	Introduce:To send a message to the connected server socket
//--------------------------------------------------------------------	
BOOL CClientSocket::SendData(LPVOID lpBuff, int iBuffLen, int iFlags)
{
	if(NULL == lpBuff || iBuffLen <= 0)
		return FALSE;

	int iRes = Send(lpBuff, iBuffLen);
	if(iRes <= 0)
		return FALSE;

	return TRUE;
}
//--------------------------------------------------------------------	
