//--------------------------------------------
//	File Name:HTTPDownloadDll.h
//	Introduce:The export file of HTTPDownloadDll.dll
//	Date:2010-05-12
//--------------------------------------------
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HTTPDOWNLOADDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HTTPDOWNLOADDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//--------------------------------------------
#ifndef	_HTTPDOWNLOAD_DLL_H_
#define	_HTTPDOWNLOAD_DLL_H_
//--------------------------------------------
#ifdef HTTPDOWNLOADDLL_EXPORTS
#define HTTPDOWNLOADDLL_API __declspec(dllexport)
#else
#define HTTPDOWNLOADDLL_API __declspec(dllimport)
#endif
//--------------------------------------------
#include "HTTPDownloadIF.h"
//--------------------------------------------
/**********The export functions**********/
//--------------------------------------------
extern "C"
{
	/*	Name:Create_HttpDownload
		Parameter:None
		Return:Return a IHttpDownloadIF pointer if successfully, other wise return NULL
	*/
	HTTPDOWNLOADDLL_API IHttpDownloadIF* Create_HttpDownload(void);

	/*	Name:pHttpDownload
		Parameter:pHttpDownload:i:A IHttpDownloadIF pointer which need to be deleted
		Return:None
	*/
	HTTPDOWNLOADDLL_API void Destroy_HttpDownload(IHttpDownloadIF* pHttpDownload);
}
//--------------------------------------------
#endif	//_HTTPDOWNLOAD_DLL_H_
//--------------------------------------------
