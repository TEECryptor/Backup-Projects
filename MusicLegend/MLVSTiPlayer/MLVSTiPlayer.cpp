//----------------------------------------------------------------------------
//	File Name:MLVSTiPlayer.cpp
//	Introduce:Defines the entry point for the DLL application
//	Creator:Youfang Yao
//	Date:2010-08-14
//----------------------------------------------------------------------------
#include "stdafx.h"
#include "MLVSTiPlayer.h"
#include "MLVSTiPlayerImpl.h"
//----------------------------------------------------------------------------
//	Name:DllMain
//	Introduce:DLL entery definition
//----------------------------------------------------------------------------
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
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//	Name:MLVSTiPlayer_Create
//	Introduce:Create a VSTi Player interface
//----------------------------------------------------------------------------
MLVSTIPLAYER_API IMLVSTiPlayer* MLVSTiPlayer_Create()
{
	CMLVSTiPlayerImpl* pVSTiPlayer = new CMLVSTiPlayerImpl();

	return pVSTiPlayer;
}
//----------------------------------------------------------------------------
//	Name:MLVSTiPlayer_Destroy
//	Introduce:Destroy a VSTi Player interface
//----------------------------------------------------------------------------
MLVSTIPLAYER_API void MLVSTiPlayer_Destroy(IMLVSTiPlayer* pPlayer)
{
	if (NULL != pPlayer)
	{
		delete pPlayer;
		pPlayer = NULL;
	}
}
//----------------------------------------------------------------------------