//-------------------------------------------------------------------------
#pragma once
//-------------------------------------------------------------------------
#include <vector>
using namespace std;
//-------------------------------------------------------------------------
#define	WM_RECEIVED_DATA WM_USER + 1
//-------------------------------------------------------------------------
class CMySocket;
typedef vector<CMySocket*>	PtrSocketArrary;
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
class CMySocket : public CAsyncSocket
{
public:
	CMySocket(){memset(m_csMsgBuf, 0, 1024);};
	virtual ~CMySocket(){};
	//---------------------------------------------------------------------
public:
	void SetMsgWnd(HWND hMsgWnd)
	{
		m_hMsgWnd = hMsgWnd;
	}
	//---------------------------------------------------------------------
	virtual void OnReceive(int nErrorCode)
	{
		int iSize = Receive(m_csMsgBuf, 1024);
		SendMessage(m_hMsgWnd, WM_RECEIVED_DATA, (WPARAM)m_csMsgBuf, iSize);
		memset(m_csMsgBuf, 0, 1024);

		CAsyncSocket::OnReceive(nErrorCode);
	}
	//---------------------------------------------------------------------
private:
	HWND			m_hMsgWnd;
	char			m_csMsgBuf[1024];
	//---------------------------------------------------------------------
};
//-------------------------------------------------------------------------



//-------------------------------------------------------------------------
// CServerSocket command target
//-------------------------------------------------------------------------
class CServerSocket : public CAsyncSocket
{
public:
	CServerSocket();
	virtual ~CServerSocket();
	//---------------------------------------------------------------------
public:
	virtual void OnAccept(int nErrorCode);
	//---------------------------------------------------------------------
public:
	void		Init(HWND hWnd);
	//---------------------------------------------------------------------
private:
	HWND			m_hMsgWnd;
	PtrSocketArrary	m_lstClientSocket;
	//---------------------------------------------------------------------
};
//-------------------------------------------------------------------------


