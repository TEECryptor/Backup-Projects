//----------------------------------------------
//	Name:ClientImpl.h
//	Introduce:Interface for the CClientImpl class.
//	Creator:Youfang Yao
//	Date:12-27-07
//----------------------------------------------
#ifndef	_CLIENTIMPL_H_
#define	_CLIENTIMPL_H_
//----------------------------------------------
#include "MSGManager.h"
#include "winsock2.h"
//----------------------------------------------
#define	WM_SENDDATA			WM_USER + 2
//----------------------------------------------
//	Name:CClientImpl
//	Introduce:The Implementation for CClient interface
//----------------------------------------------
class CClientImpl : public CClient  
{
public:
	CClientImpl();
	virtual ~CClientImpl();
	//------------------------------------------
public:
	//The recieve data thread function
	DWORD	RecieveThreadFunc();
	//The send data thread function
	DWORD	SendThreadFunc();
	//------------------------------------------
public:
	//Create the server
	DWORD	Create(MSGManagerCallback pCallback, LPVOID lpUser);
	//Connect to the server
	DWORD	Connect(LPCTSTR lpszAddress, DWORD dwPort);
	//Send a data message from buffer to server
	DWORD	SendData(BYTE* lpBuff, DWORD dwSize);
	//------------------------------------------
public:
	DWORD		m_dwConnectID;			//The connected ID of this client in server
	SOCKET		m_ClientSocket;			//The client socket
	HANDLE		m_hRecieveThread;		//The thread for revieving data from server
	HANDLE		m_hSendThread;			//The thread for sending data to server
	DWORD		m_dwRecieveThreadID;	//The thead ID for revieving data from server
	DWORD		m_dwSendThreadID;		//The thead ID for sending data to server
	HANDLE		m_hSendStartEvent;		//The start event for sending thread
	//
	MSGManagerCallback	m_pCallback;	//The callback function pointer
	LPVOID				m_pCallUser;	//The user defined pointer for callback function
	BYTE		m_SendBuff[MAX_DATA_LEN];
	BYTE		m_RecvBuff[MAX_DATA_LEN];
	//------------------------------------------
};
//----------------------------------------------
#endif //_CLIENTIMPL_H_
//----------------------------------------------
