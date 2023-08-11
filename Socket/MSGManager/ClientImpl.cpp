//----------------------------------------------
//	Name:ClientImpl.cpp
//	Introduce:Implementation of the CClientImpl class.
//	Creator:Youfang Yao
//	Date:12-27-07
//----------------------------------------------
#include "stdafx.h"
#include "ClientImpl.h"
//----------------------------------------------
//	Name:RecieveThreadProc
//	Introduce:The recieve data thread function
//----------------------------------------------
DWORD WINAPI RecieveThreadProc(LPVOID lpParameter)
{
	if(NULL != lpParameter)
		return ((CClientImpl*)lpParameter)->RecieveThreadFunc();
	return 0;
}
//----------------------------------------------
//	Name:SendThreadProc
//	Introduce:The send data thread function
//----------------------------------------------
DWORD WINAPI SendThreadProc(LPVOID lpParameter)
{
	if(NULL != lpParameter)
		return ((CClientImpl*)lpParameter)->SendThreadFunc();
	return 0;
}
//----------------------------------------------


//----------------------------------------------
//	Name:CClientImpl
//	Introduce:Construction
//----------------------------------------------
CClientImpl::CClientImpl()
 : CClient()
 , m_dwConnectID(0)
 , m_hRecieveThread(NULL)
 , m_hSendThread(NULL)		
 , m_dwRecieveThreadID(0)	
 , m_dwSendThreadID(0)
 , m_hSendStartEvent(NULL)
 , m_pCallback(NULL)
 , m_pCallUser(NULL)
{
}
//----------------------------------------------
//	Name:~CClientImpl
//	Introduce:Distruction
//----------------------------------------------
CClientImpl::~CClientImpl()
{

}
//----------------------------------------------



//----------------------------------------------
//	Name:Create
//	Introduce:Create the server
//----------------------------------------------
DWORD CClientImpl::Create(MSGManagerCallback pCallback, LPVOID lpUser)
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

	//Create the client SOCKET for connecting to server
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(INVALID_SOCKET == m_ClientSocket) 
	{
		iResult = WSAGetLastError();
		WSACleanup();
		return iResult;
	}

	return NO_ERROR;
}
//----------------------------------------------
//	Name:Connect
//	Introduce:Connect to the server
//----------------------------------------------
DWORD CClientImpl::Connect(LPCTSTR lpszAddress, DWORD dwPort)
{
	int iResult = NO_ERROR;

	//Create the recieving thread
	m_hRecieveThread = CreateThread(NULL, 0, RecieveThreadProc, this, 0, &m_dwRecieveThreadID);
	if(NULL == m_hRecieveThread)
	{
		iResult = GetLastError();
		return iResult;
	}

	//Create the sending thread
	m_hSendStartEvent = CreateEvent(0, FALSE, FALSE, 0);
	m_hSendThread = CreateThread(NULL, 0, SendThreadProc, this, 0, &m_dwSendThreadID);
	if(NULL == m_hSendThread)
	{
		iResult = GetLastError();
		CloseHandle(m_hRecieveThread);
		return iResult;
	}

	//Waiting for sending thread is started
	iResult = WaitForSingleObject(m_hSendStartEvent, 5000);	
	if(WAIT_TIMEOUT == iResult)
	{
		CloseHandle(m_hRecieveThread);
		CloseHandle(m_hSendStartEvent);
		CloseHandle(m_hSendThread);
		return WAIT_TIMEOUT;
	}

	//Server address
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(lpszAddress);
	clientService.sin_port = htons((USHORT)dwPort);

	//Connect to server
	if(connect(m_ClientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) 
	{
		CloseHandle(m_hRecieveThread);
		CloseHandle(m_hSendStartEvent);
		CloseHandle(m_hSendThread);
		iResult = WSAGetLastError();
		WSACleanup();
		return iResult;
	}

	return NO_ERROR;
}
//----------------------------------------------
//	Name:SendData
//	Introduce:end a data message from buffer to server
//----------------------------------------------
DWORD CClientImpl::SendData(BYTE* lpBuff, DWORD dwSize)
{
	//The input parameters are incorrect
	if(NULL == lpBuff || dwSize > MAX_DATA_LEN)
		return E_POINTER;

	//Save the data for sending
	memset(m_SendBuff, 0, sizeof(BYTE)*MAX_DATA_LEN);
	memcpy(m_SendBuff, lpBuff, dwSize);

	//Post WM_SENDDATA message to send thread
	PostThreadMessage(m_dwSendThreadID, WM_SENDDATA, dwSize, 0);

	return NO_ERROR;
}
//----------------------------------------------


//----------------------------------------------
//	Name:RecieveThreadFunc
//	Introduce:The recieve data thread function
//----------------------------------------------
DWORD CClientImpl::RecieveThreadFunc()
{
	while(true)
	{  
		int iRecvLen = 0;

		iRecvLen = recv(m_ClientSocket, (char*)m_RecvBuff, MAX_DATA_LEN, 0);
		if(iRecvLen > 0 && iRecvLen != WSAECONNRESET)
		{
			if(m_pCallback) 
				m_pCallback(MSG_CODE_RECIEVEDATA, iRecvLen, m_RecvBuff, m_pCallUser);
		}
		else if(iRecvLen <= 0)
			Sleep(100);
		else 
			break;
	}

	return 0;
}
//----------------------------------------------
//	Name:SendThreadFunc
//	Introduce:The send data thread function
//----------------------------------------------
DWORD CClientImpl::SendThreadFunc()
{
	MSG msg;
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	//Now, the thread has had a message queue and we can start it
	if(0 == SetEvent(m_hSendStartEvent))
		return 1;

	//Message loop
	while(true)
    {
		//Get msg from message queue
        if(GetMessage(&msg, 0, 0, 0)) 
        {
			switch(msg.message)
			{
			case WM_SENDDATA:			//A data should be sent now
				{
					int iByteSent = 0;
					iByteSent = send(m_ClientSocket, (char*)m_SendBuff, msg.wParam, 0);
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
