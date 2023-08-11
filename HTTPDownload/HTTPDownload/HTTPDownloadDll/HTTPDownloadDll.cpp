//--------------------------------------------
//	File Name:HTTPDownloadDll.cpp
//	Introduce:Defines the entry point for the DLL application.
//	Date:2010-05-12
//--------------------------------------------
#include "stdafx.h"
#include "HTTPDownloadDll.h"
#include "HTTPDownload.h"
//--------------------------------------------
extern "C"
{
//--------------------------------------------
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
//--------------------------------------------


//--------------------------------------------
HTTPDOWNLOADDLL_API IHttpDownloadIF* Create_HttpDownload(void)
{
	CHttpDownload* pHttpDownload = new CHttpDownload();

	return pHttpDownload;
}
//--------------------------------------------
HTTPDOWNLOADDLL_API void Destroy_HttpDownload(IHttpDownloadIF* pHttpDownload)
{
	if (NULL != pHttpDownload)
	{
		pHttpDownload->StopAllTasks();
		delete pHttpDownload;
		pHttpDownload = NULL;
	}
}
//--------------------------------------------
}	//extern "C"
//--------------------------------------------