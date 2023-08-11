//-------------------------------------------------------------------------
// ServerSocket.cpp : implementation file
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ServerSocket.h"
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
// CServerSocket
//-------------------------------------------------------------------------
CServerSocket::CServerSocket()
: m_hMsgWnd(NULL)
{
}
//-------------------------------------------------------------------------
CServerSocket::~CServerSocket()
{
	for (PtrSocketArrary::iterator it = m_lstClientSocket.begin();
		it != m_lstClientSocket.end();
		it++)
	{
		CMySocket* pSocket = *it;
		delete pSocket;
		pSocket = NULL;
	}
	m_lstClientSocket.clear();
}
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
// CServerSocket member functions
//-------------------------------------------------------------------------
void CServerSocket::Init(HWND hWnd)
{
	m_hMsgWnd = hWnd;
}
//-------------------------------------------------------------------------
void CServerSocket::OnAccept(int nErrorCode)
{
	CMySocket* pNewClientSocket = new CMySocket();
	pNewClientSocket->SetMsgWnd(m_hMsgWnd);
	if (TRUE == Accept(*pNewClientSocket))
	{
		pNewClientSocket->Send("OK", 2);
		m_lstClientSocket.push_back(pNewClientSocket);
	}
	
	CAsyncSocket::OnAccept(nErrorCode);
}
//-------------------------------------------------------------------------