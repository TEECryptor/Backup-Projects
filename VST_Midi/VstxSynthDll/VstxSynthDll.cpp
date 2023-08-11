// VstxSynthDll.cpp : Defines the entry point for the DLL application.
//
/*
#include "stdafx.h"
#include "VstxSynthDll.h"
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

// This is an example of an exported variable
VSTXSYNTHDLL_API int nVstxSynthDll=0;

// This is an example of an exported function.
VSTXSYNTHDLL_API int fnVstxSynthDll(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see VstxSynthDll.h for the class definition
CVstxSynthDll::CVstxSynthDll()
{ 
	return; 
}
*/
#include <windows.h>
#include <stdio.h>
#include "audioeffect.h"

extern AudioEffect* createEffectInstance (audioMasterCallback audioMaster);

#if defined (__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
    #define VST_EXPORT	__attribute__ ((visibility ("default")))
#else
    #define VST_EXPORT
#endif

extern "C" {
__declspec(dllexport)
VST_EXPORT void * main (audioMasterCallback audioMaster) 
{
	// Get VST Version of the Host
	if (!audioMaster (0, audioMasterVersion, 0, 0, 0, 0))
		return 0;  // old version

	// Create the AudioEffect
	AudioEffect* effect = createEffectInstance (audioMaster);
	if (!effect)
		return 0;

	// Return the VST AEffect structur
	return effect->getAeffect ();
}
} // extern "C"


#if WIN32
void* hInstance;

extern "C" {
BOOL WINAPI DllMain (HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
{
	hInstance = hInst;
	return 1;
}
} // extern "C"
#endif

