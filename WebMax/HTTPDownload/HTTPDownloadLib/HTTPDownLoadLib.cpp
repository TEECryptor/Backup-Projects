//--------------------------------------------
//	File Name:HTTPDownloadLib.cpp
//	Introduce:The implement file of HTTPDownloadLib.lib
//	Date:2010-05-12
//--------------------------------------------
#include "HTTPDownloadLib.h"
#include "HTTPDownload.h"
//--------------------------------------------
extern "C"
{
//--------------------------------------------
IHttpDownloadIF* Create_HttpDownload(void)
{
	IHttpDownloadIF* pHttpDownload = new CHttpDownload();

	return pHttpDownload;
}
//--------------------------------------------
void Destroy_HttpDownload(IHttpDownloadIF* pHttpDownload)
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