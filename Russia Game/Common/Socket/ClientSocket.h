//-----------------------------------------------------------
//	Name:ClientSocket.h
//	Introduce:The define of the CClientSocket class
//	Creator:Youfang Yao
//	Date:11/27/06
//	Version:1.1v
//-----------------------------------------------------------
#ifndef _CLIENTSOCKET_H_
#define _CLIENTSOCKET_H_
//-----------------------------------------------------------
#include "Afxsock.h"
#include "SocketDefine.h"
//-----------------------------------------------------------
//	Name:CClientSocket
//	Introduce:The class CClientSocket definition
//-----------------------------------------------------------
class CClientSocket : public CSocket
{
	//-------------------------------------------------------
public:
	CClientSocket(HWND hMsgWnd);
	virtual ~CClientSocket();
	//-------------------------------------------------------
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	//-------------------------------------------------------
public:
	//-------------------------------------------------------
	//To create a socket as server
	BOOL	Create(UINT uClientID, UINT nSocketPort = 0, int nSocketType = SOCK_STREAM, LPCTSTR lpszSocketAddress = NULL);
	//To send a message to the connected server socket
	BOOL	SendData(LPVOID lpBuff, int iBuffLen, int iFlags = 0);
	//-------------------------------------------------------
private:
	UINT	m_uClientID;
	HWND	m_hMsgWnd;		//The message handle window
	BYTE	m_nMsgDataBuff[MAX_SOCKETDATA_LEN];
	//-------------------------------------------------------
};
//-----------------------------------------------------------
#endif // _CLIENTSOCKET_H_
//-----------------------------------------------------------
