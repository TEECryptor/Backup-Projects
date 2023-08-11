//--------------------------------------------
//	File Name:HTTPDownloadLib.h
//	Introduce:The export file of HTTPDownloadLib.lib
//	Date:2010-05-12
//--------------------------------------------
#ifndef	_HTTPDOWNLOAD_LIB_H_
#define	_HTTPDOWNLOAD_LIB_H_
//--------------------------------------------
#include <windows.h>
#include "HTTPDownloadIF.h"
//--------------------------------------------
/***********The export functions************/
//--------------------------------------------
extern "C"
{
	/*	Name:Create_HttpDownload
		Parameter:None
		Return:Return a IHttpDownloadIF pointer if successfully, other wise return NULL
	*/
	IHttpDownloadIF* Create_HttpDownload(void);

	/*	Name:pHttpDownload
		Parameter:pHttpDownload:i:A IHttpDownloadIF pointer which need to be deleted
		Return:None
	*/
	void Destroy_HttpDownload(IHttpDownloadIF* pHttpDownload);
}
//--------------------------------------------
#endif	//_HTTPDOWNLOAD_LIB_H_
//--------------------------------------------