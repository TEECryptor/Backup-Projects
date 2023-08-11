//------------------------------------------------------
//	FileName:MediaLib.cpp
//	Introduce:Defines the entry point for the DLL application.
//	Creator:YOufang yao
//	Date:10-26-06
//	Version:1.0v
//------------------------------------------------------
#include "stdafx.h"
#include "MediaLib.h"
#include "MusicImpl.h"
//------------------------------------------------------
//	Name:DllMain
//	Introduce:The entry point for the DLL application.
//------------------------------------------------------
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
//------------------------------------------------------



//------------------------------------------------------
//	Name:MediaLib_CreateMusic
//	Introduce:To create and return a music object
//------------------------------------------------------
MEDIALIB_API CMusic* MediaLib_CreateMusic()
{
	CMusicImpl* pMusicImpl = new CMusicImpl();
	
	return pMusicImpl;
}
//------------------------------------------------------
//	Name:MediaLib_DestroyMusic
//	Introduce:To destroy a music object
//------------------------------------------------------
MEDIALIB_API void MediaLib_DestroyMusic(CMusic *pMusic)
{
	if(pMusic)
	{
		delete pMusic;
		pMusic = NULL;
	}
}
//------------------------------------------------------
//	Name:MediaLib_CreateImage
//	Introduce:To create and return a image object
//------------------------------------------------------
MEDIALIB_API CImage* MediaLib_CreateImage()
{
	return NULL;
}
//------------------------------------------------------
//	Name:MediaLib_DestroyImage
//	Introduce:To destroy a image object
//------------------------------------------------------
MEDIALIB_API void MediaLib_DestroyImage(CImage *pImage)
{
}
//------------------------------------------------------
