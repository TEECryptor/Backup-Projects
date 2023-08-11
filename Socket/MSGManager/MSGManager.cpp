//----------------------------------------------
//	Name:MSGManager.cpp
//	Introduce:Defines the entry point for the DLL application.
//	Creator:Youfang Yao
//	Date:12-27-07
//----------------------------------------------
#include "stdafx.h"
#include "MSGManager.h"
#include "ServerImpl.h"
#include "ClientImpl.h"
//----------------------------------------------
//	Name:DllMain
//	Introduce:The entry point for the DLL
//----------------------------------------------
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
//----------------------------------------------


//----------------------------------------------
//	Name:MSGManager_CreateServer
//	Introduce:To create and return a CServer object pointer
//----------------------------------------------
MSGMANAGER_API CServer* MSGManager_CreateServer()
{
	CServerImpl* pServer = new CServerImpl();
	return pServer;
}
//----------------------------------------------
//	Name:MSGManager_CreateClient
//	Introduce:To create and return a CClient object pointer
//----------------------------------------------
MSGMANAGER_API CClient* MSGManager_CreateClient()
{
	CClientImpl* pClient = new CClientImpl();
	return pClient;
}
//----------------------------------------------
//	Name:MSGManager_DestroyServer
//	Introduce:To destroy a CServer object
//----------------------------------------------
MSGMANAGER_API void MSGManager_DestroyServer(CServer* pServer)
{
	if(NULL != pServer)
	{
		delete pServer;
		pServer = NULL;
	}
}
//----------------------------------------------
//	Name:MSGManager_DestroyClient
//	Introduce:To destroy a CClient object
//----------------------------------------------
MSGMANAGER_API void MSGManager_DestroyClient(CClient* pClient)
{
	if(NULL != pClient)
	{
		delete pClient;
		pClient = NULL;
	}
}
//----------------------------------------------
