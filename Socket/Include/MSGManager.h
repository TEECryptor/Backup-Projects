//----------------------------------------------
//	Name:MSGManager.h
//	Introduce:Define the classes and functions for this dll
//	Creator:Youfang Yao
//	Date:12-27-07
//----------------------------------------------
#ifndef	_MSGMANAGER_H_
#define	_MSGMANAGER_H_
//----------------------------------------------
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MSGMANAGER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MSGMANAGER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//----------------------------------------------
#ifdef MSGMANAGER_EXPORTS
#define MSGMANAGER_API __declspec(dllexport)
#else
#define MSGMANAGER_API __declspec(dllimport)
#endif
//----------------------------------------------
#define	MAX_DATA_LEN			1024	
//----------------------------------------------

enum
{
	MSG_CODE_BASE				= 0,
	MSG_CODE_CLIENTCONNECTED	= 1,
	MSG_CODE_RECIEVEDATA		= 2,
	MSG_CODE_CNT
};

typedef DWORD (*MSGManagerCallback)(DWORD dwCode, DWORD dwParam, LPVOID lpParam, LPVOID lpUser);


//----------------------------------------------
//	Name:CServer
//	Introduce:Define the server for MSG manager
//----------------------------------------------
class CServer
{
public:
	CServer(){};
	virtual ~CServer(){};
	//------------------------------------------
public:
	virtual DWORD Create(DWORD dwPort, MSGManagerCallback pCallback, LPVOID lpUser) = 0;
	virtual DWORD Listen() = 0;
	virtual DWORD SendData(BYTE* lpBuff, DWORD dwSize, int iConnectID = -1) = 0;
	//------------------------------------------
};
//----------------------------------------------


//----------------------------------------------
//	Name:CClient
//	Introduce:Define the client for MSG manager
//----------------------------------------------
class CClient
{
public:
	CClient(){};
	virtual ~CClient(){};
	//------------------------------------------
public:
	virtual DWORD Create(MSGManagerCallback pCallback, LPVOID lpUser) = 0;
	virtual DWORD Connect(LPCTSTR lpszAddress, DWORD dwPort) = 0;
	virtual DWORD SendData(BYTE* lpBuff, DWORD dwSize) = 0;
	//------------------------------------------
};
//----------------------------------------------

/*Exported*/
MSGMANAGER_API CServer* MSGManager_CreateServer();
MSGMANAGER_API CClient* MSGManager_CreateClient();
MSGMANAGER_API void MSGManager_DestroyServer(CServer* pServer);
MSGMANAGER_API void MSGManager_DestroyClient(CClient* pClient);
//----------------------------------------------
#endif	//_MSGMANAGER_H_
//----------------------------------------------

