//-----------------------------------------------
//	Name:BurnEngine.cpp
//	Introduce:Defines the entry point for the DLL application.
//-----------------------------------------------
#include "stdafx.h"
#include "BurnEngine.h"
#include "ASBurnEngine.h"
//-----------------------------------------------
//	Name:DllMain
//	Introduce:The entry of this DLL
//-----------------------------------------------
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
//-----------------------------------------------



//-----------------------------------------------
//	Name:BurnEngine_Create
//	Introduce:To create a CBurnEngine object pointer
//-----------------------------------------------
BURNENGINE_API CBurnEngine* BurnEngine_Create()
{
	CASBurnEngine* pEngine = new CASBurnEngine();

	return pEngine;
}
//-----------------------------------------------
//	Name:BurnEngine_Destroy
//	Introduce:To destroy a CBurnEngine object pointer
//-----------------------------------------------
BURNENGINE_API void BurnEngine_Destroy(CBurnEngine* pBurnEngine)
{
	if(NULL != pBurnEngine)
	{
		pBurnEngine->Release();
		delete pBurnEngine;
		pBurnEngine = NULL;
	}
}
//-----------------------------------------------
