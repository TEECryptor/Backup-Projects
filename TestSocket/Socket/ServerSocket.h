//-----------------------------------------------------------
//	Name:ServerSocket.h
//	Introduce:The define of the CServerSocket class
//	Creator:Youfang Yao
//	Date:11/27/06
//	Version:1.1v
//-----------------------------------------------------------
#ifndef _SERVERSOCKET_H_
#define _SERVERSOCKET_H_
//-----------------------------------------------------------
#include "Afxsock.h"
#include <vector>
#include "SocketDefine.h"
#include "ClientSocket.h"
//-----------------------------------------------------------
using namespace std;
//-----------------------------------------------------------
//	Name:CServerSocket
//	Introduce:The class CServerSocket definition
//-----------------------------------------------------------
class CServerSocket : public CSocket
{
	//-------------------------------------------------------
public:
	CServerSocket();
	virtual ~CServerSocket();
	//-------------------------------------------------------
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	//-------------------------------------------------------
public:
	//-------------------------------------------------------
	//To create a socket as server
	BOOL	Create(HWND hMsgWnd, UINT nSocketPort = 0, int nSocketType = SOCK_STREAM, LPCTSTR lpszSocketAddress = NULL);
	//To send a message to specific client socket or to all clients
	BOOL	SendData(LPVOID lpBuff, int iBuffLen, int iClientID = -1, int iFlags = 0);
	//-------------------------------------------------------
private:
	//-------------------------------------------------------
	HWND					m_hMsgWnd;		//The message handle window
	vector<CClientSocket*>	m_lstClient;	//The clients list
	//-------------------------------------------------------
};
//-----------------------------------------------------------
#endif // _SERVERSOCKET_H_
//-----------------------------------------------------------
