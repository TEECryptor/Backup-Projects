//---------------------------------------------------------------------
//	FileName:	LibImage.cpp
//	Introduce:	The .cpp file of the images library, defines the entry 
//				point for the DLL application.
//	Creator:	Youfang Yao
//	Date:		05/24/06
//	Version:	1.0v 
//---------------------------------------------------------------------
#include "stdafx.h"
#include "LibImage.h"
#include "LibImageImpl.h"
//---------------------------------------------------------------------
//	Name:DllMain
//	Introduce:The entry point of the Dll
//---------------------------------------------------------------------
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
//---------------------------------------------------------------------
//To create an image interface pointer, user apps do some image operations
//by this pointer
//---------------------------------------------------------------------
LIBIMAGE_API CLibImage* LibImage_Create()
{
	CLibImageImpl* pLibImageImpl = new CLibImageImpl();

	return pLibImageImpl;
}
//---------------------------------------------------------------------
//To destroy an image interface pointer
//---------------------------------------------------------------------
LIBIMAGE_API bool LibImage_Destroy(CLibImage* pLibImage)
{
	if(pLibImage)
	{
		delete pLibImage;
		pLibImage = NULL;
	}

	return true;
}
//---------------------------------------------------------------------

