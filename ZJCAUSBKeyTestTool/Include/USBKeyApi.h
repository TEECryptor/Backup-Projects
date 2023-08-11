// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ZJCAUSBKEYTESTAPI_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ZJCAUSBKEYTESTAPI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef _USBKEY_API_H_
#define _USBKEY_API_H_

#include "IUSBKeyApi.h"

#ifdef USBKEYAPI_EXPORTS
#define USBKEYAPI_API __declspec(dllexport)
#else
#define USBKEYAPI_API __declspec(dllimport)
#endif

USBKEYAPI_API IUSBKeyApi* USBKey_CreateCSP();
USBKEYAPI_API IUSBKeyApi* USBKey_CreateP11();
USBKEYAPI_API IUSBKeyApi* USBKey_CreateSKF();
USBKEYAPI_API void USBKey_Destroy(IUSBKeyApi* pApi);
USBKEYAPI_API ULONG USBKey_GetErrorLog(LPSTR lpLogText, ULONG *pulLen);
USBKEYAPI_API ULONG USBKey_LogEnabled(BOOL bEnable, LPSTR lpLogFile);

#endif	//_USBKEY_API_H_