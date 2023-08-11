//----------------------------------------------
//	Name:ServerImpl.h
//	Introduce:Interface for the CServerImpl class.
//	Creator:Youfang Yao
//	Date:12-27-07
//----------------------------------------------
#ifndef	_SERVERIMPL_H_
#define	_SERVERIMPL_H_
//----------------------------------------------
#include "MSGManager.h"
#include "ClientImpl.h"
#include "winsock2.h"
#include <list>
//----------------------------------------------
#define	WM_CLIENTCONNECT	WM_USER + 1
#define	WM_SENDDATA			WM_USER + 2
//----------------------------------------------
typedef std::list<CClientImpl*> ClientPtrArray;
//----------------------------------------------
//	Name:CServerImpl
//	Introduce:The Implementation for CServer interface
//----------------------------------------------
class CServerImpl : public CServer  
{
public:
	CServerImpl();
	virtual ~CServerImpl();
	//------------------------------------------
public:
	//The listen thread function
	DWORD	ListenThreadFunc();
	//The work thread function
	DWORD	WorkThreadFunc();
	//------------------------------------------
public:
	//Create the server
	DWORD	Create(DWORD dwPort, MSGManagerCallback pCallback, LPVOID lpUser);
	//Start the listen thread, and start to recive clients connecting
	DWORD	Listen();
	//Send a data message to clients
	DWORD	SendData(BYTE* lpBuff, DWORD dwSize, int iConnectID = -1);
	//------------------------------------------
private:
	SOCKET		m_ListenSocket;			//The listen socket for server	
	HANDLE		m_hListenThread;		//The thread for listenning
	HANDLE		m_hWorkThread;			//The thread for working(receive and send data)
	DWORD		m_dwListenThreadID;		//The thead ID for listenning
	DWORD		m_dwWorkThreadID;		//The thead ID for working(receive and send data)
	HANDLE		m_hWorkStartEvent;		//The start event for working thread
	//
	ClientPtrArray		m_lstClientPtr;	//The pointer array for clients
	MSGManagerCallback	m_pCallback;	//The callback function pointer
	LPVOID				m_pCallUser;	//The user defined pointer for callback function
	BYTE		m_SendBuff[MAX_DATA_LEN];
	BYTE		m_RecvBuff[MAX_DATA_LEN];
	//------------------------------------------
};
//----------------------------------------------
#endif //_SERVERIMPL_H_
//----------------------------------------------
