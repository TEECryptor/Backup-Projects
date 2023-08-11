//----------------------------------------------
//	Name:ServerImpl.cpp
//	Introduce:Implementation of the CServerImpl class.
//	Creator:Youfang Yao
//	Date:12-27-07
//----------------------------------------------
#include "stdafx.h"
#include "ServerImpl.h"
//----------------------------------------------
//	Name:ListenThreadProc
//	Introduce:The listen thread function
//----------------------------------------------
DWORD WINAPI ListenThreadProc(LPVOID lpParameter)
{
	if(NULL != lpParameter)
		return ((CServerImpl*)lpParameter)->ListenThreadFunc();
	return 0;
}
//----------------------------------------------
//	Name:WorkThreadProc
//	Introduce:The work thread function
//----------------------------------------------
DWORD WINAPI WorkThreadProc(LPVOID lpParameter)
{
	if(NULL != lpParameter)
		return ((CServerImpl*)lpParameter)->WorkThreadFunc();
	return 0;
}
//----------------------------------------------


//----------------------------------------------
//	Name:CServerImpl
//	Introduce:Construction
//----------------------------------------------
CServerImpl::CServerImpl()
 : CServer()
 , m_pCallback(NULL)
 , m_pCallUser(NULL)
 , m_hListenThread(NULL)
 , m_hWorkThread(NULL)	
 , m_dwListenThreadID(0)
 , m_dwWorkThreadID(0)
 , m_hWorkStartEvent(NULL)
{

}
//----------------------------------------------
//	Name:~CServerImpl
//	Introduce:Destruction
//----------------------------------------------
CServerImpl::~CServerImpl()
{

}
//----------------------------------------------



//----------------------------------------------
//	Name:Create
//	Introduce:Create the server
//----------------------------------------------
DWORD CServerImpl::Create(DWORD dwPort, MSGManagerCallback pCallback, LPVOID lpUser)
{
	//The call back function should not be NULL!
	if(NULL == pCallback)
		return E_POINTER;
	m_pCallback = pCallback;
	m_pCallUser = lpUser;

	//Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(NO_ERROR != iResult)
		return iResult;

	//Create a SOCKET for listening for incoming connection requests.
	m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(INVALID_SOCKET == m_ListenSocket) 
	{
		iResult = WSAGetLastError();
		WSACleanup();
		return iResult;
	}

	//The sockaddr_in structure specifies the address family,
	//IP address, and port for the socket that is being bound.
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons((USHORT)dwPort);
	if(bind(m_ListenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		iResult = WSAGetLastError();
		closesocket(m_ListenSocket);
		WSACleanup();
		return iResult;
	}

	return NO_ERROR;
}
//----------------------------------------------
//	Name:Listen
//	Introduce:Start the listen thread, and start to recive clients connecting
//----------------------------------------------
DWORD CServerImpl::Listen()
{	
	int iResult = NO_ERROR;

	//Listen for incoming connection requests on the created socket
	if(listen(m_ListenSocket, 1) == SOCKET_ERROR)
	{
		iResult = WSAGetLastError();
		WSACleanup();
		return iResult;
	}

	//Create the listen thread
	m_hListenThread = CreateThread(NULL, 0, ListenThreadProc, this, 0, &m_dwListenThreadID);
	if(NULL == m_hListenThread)
	{
		WSACleanup();
		iResult = GetLastError();
		return iResult;
	}

	//Create the working thread
	m_hWorkStartEvent = CreateEvent(0, FALSE, FALSE, 0);
	m_hWorkThread = CreateThread(NULL, 0, WorkThreadProc, this, 0, &m_dwWorkThreadID);
	if(NULL == m_hWorkThread)
	{
		WSACleanup();
		iResult = GetLastError();
		return iResult;
	}

	//Waiting for working thread is started
	iResult = WaitForSingleObject(m_hWorkStartEvent, 5000);	
	if(WAIT_TIMEOUT == iResult)
	{
		WSACleanup();
		CloseHandle(m_hWorkStartEvent);
		CloseHandle(m_hListenThread);
		CloseHandle(m_hWorkThread);
		return WAIT_TIMEOUT;
	}

	return NO_ERROR;
}
//----------------------------------------------
//	Name:ListenThreadFunc
//	Introduce:Send a data message to clients
//----------------------------------------------
DWORD CServerImpl::SendData(BYTE* lpBuff, DWORD dwSize, int iConnectID)
{
	//The input parameters are incorrect
	if(NULL == lpBuff || dwSize > MAX_DATA_LEN)
		return E_POINTER;

	//Save the data for sending
	memset(m_SendBuff, 0, sizeof(BYTE)*MAX_DATA_LEN);
	memcpy(m_SendBuff, lpBuff, dwSize);

	//Post WM_SENDDATA message to working thread
	PostThreadMessage(m_dwWorkThreadID, WM_SENDDATA, iConnectID, dwSize);

	return NO_ERROR;
}
//----------------------------------------------



//----------------------------------------------
//	Name:ListenThreadFunc
//	Introduce:The listen thread function
//----------------------------------------------
DWORD CServerImpl::ListenThreadFunc()
{
	//Loop for clients connecting
	while(true) 
	{
		CClientImpl* pClientSocket = new CClientImpl();
		pClientSocket->m_ClientSocket = SOCKET_ERROR;
		while(SOCKET_ERROR == pClientSocket->m_ClientSocket)
		{
			pClientSocket->m_ClientSocket = accept(m_ListenSocket, NULL, NULL);
			Sleep(100);
		}

		//A client is connected, save the pointer to array
		UINT uCnt = m_lstClientPtr.size();
		pClientSocket->m_dwConnectID = uCnt;
		m_lstClientPtr.push_back(pClientSocket);

		//Post WM_CLIENTCONNECT message to working thread
		PostThreadMessage(m_dwWorkThreadID, WM_CLIENTCONNECT, pClientSocket->m_dwConnectID, 0);
    }

	return 0;
}
//----------------------------------------------
//	Name:WorkThreadFunc
//	Introduce:The work thread function
//----------------------------------------------
DWORD CServerImpl::WorkThreadFunc()
{
	MSG msg;
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	//Now, the thread has had a message queue and we can start it
	if(0 == SetEvent(m_hWorkStartEvent))
		return 1;

	//Message loop
	while(true)
    {
		//Get msg from message queue
        if(GetMessage(&msg, 0, 0, 0)) 
        {
			switch(msg.message)
			{
			case WM_CLIENTCONNECT:		//A client is connected
				{					
					//To tell host application by call back function
					if(NULL != m_pCallback) 
						m_pCallback(MSG_CODE_CLIENTCONNECTED, msg.wParam, NULL, m_pCallUser);
				}
				break;
			case WM_SENDDATA:			//A data should be sent now
				{
					//Send the data to clients
					for(ClientPtrArray::iterator it = m_lstClientPtr.begin(); it != m_lstClientPtr.end(); it++)
					{
						if(-1 == msg.wParam || (*it)->m_dwConnectID == msg.wParam)
						{
							int iByteSent = 0;
							iByteSent = send((*it)->m_ClientSocket, (char*)m_SendBuff, msg.lParam, 0 );
						}
					}
				}
				break;
			case WM_QUIT:				//Exit this thread
				return 0;
			}
		}
	}

	return 0;
}
//----------------------------------------------
