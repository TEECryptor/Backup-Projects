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
#include "FlashPlayerImpl.h"
#include "SurfaceSceneImpl.h"
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
//	Name:MediaLib_CreateScene
//	Introduce:To create and return a CSurfaceScene object
//------------------------------------------------------
MEDIALIB_API CSurfaceScene* MediaLib_CreateScene()
{
	CSurfaceSceneImpl* pScene = new CSurfaceSceneImpl();

	return pScene;
}
//------------------------------------------------------
//	Name:MediaLib_DestroyScene
//	Introduce:To destroy a CSurfaceScene object
//------------------------------------------------------
MEDIALIB_API void MediaLib_DestroyScene(CSurfaceScene *pScene)
{
	if(pScene)
	{
		delete pScene;
		pScene = NULL;
	}
}
//------------------------------------------------------
//	Name:MediaLib_CreateFlash
//	Introduce:To create and return a CFlashPlayer object pointer
//------------------------------------------------------
MEDIALIB_API CFlashPlayer* MediaLib_CreateFlash()
{
	CFlashPlayerImpl* pFlasPlayer = new CFlashPlayerImpl();

	return pFlasPlayer;
}
//------------------------------------------------------
//	Name:MediaLib_DestroyFlash
//	Introduce:To destroy the specific CFlashPlayer object pointer
//------------------------------------------------------
MEDIALIB_API void MediaLib_DestroyFlash(CFlashPlayer* pFlashPlayer)
{
	if(pFlashPlayer)
	{
		if(pFlashPlayer->IsPlaying())
			pFlashPlayer->Stop();

		delete pFlashPlayer;
		pFlashPlayer = NULL;
	}
}
//------------------------------------------------------
