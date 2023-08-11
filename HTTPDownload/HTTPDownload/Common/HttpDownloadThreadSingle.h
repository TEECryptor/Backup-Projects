//--------------------------------------------
//	File Name:HttpDownloadThreadSingle.h
//	Introduce:Class CHttpDownloadThreadSingle
//	Date:2010-10-10
//--------------------------------------------
#ifndef	_HTTPDOWNLOAD_THREADSINGLE_H_
#define	_HTTPDOWNLOAD_THREADSINGLE_H_
//--------------------------------------------
#include "HttpdownloadThread.h"
//--------------------------------------------
class CHttpDownloadThreadSingle : public CHttpDownloadThread
{
public:
	CHttpDownloadThreadSingle(void);
	~CHttpDownloadThreadSingle(void);
	//----------------------------------------
protected:
	LONG	SetDownloadInfo(HINTERNET hNetFile, HANDLE hTargetFile, CHttpLogFile* pLogFile, LPTHREADINFO lpInfo);
	LONG	Start();
	LONG	Stop();
	//
	DWORD	DownloadingFunc();
	//----------------------------------------
private:
	HINTERNET	m_hNetFile;
	//----------------------------------------
};
//--------------------------------------------
#endif	//_HTTPDOWNLOAD_THREADSINGLE_H_
//--------------------------------------------

