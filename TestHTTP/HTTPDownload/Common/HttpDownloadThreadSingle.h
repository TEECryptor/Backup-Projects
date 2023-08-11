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
	//void	SetThreadID(DWORD dwID);
	//DWORD	GetThreadID();
	//LONG	SetDownloadInfo(HINTERNET hConnect, HANDLE hTargetFile, CHttpLogFile* pLogFile, LPTHREADINFO lpInfo);
	//void	GetDownloadInfo(LPTHREADINFO lpInfo);
	//void	SetDataSegments(DWORD dwSegmentCnt, LPDATASEGMENTINFO lpDataSegments);
	//DWORD	GetDataSegmentCnt();
	//void	GetDataSegments(LPDATASEGMENTINFO lpDataSegments);
	//void	SetCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
	//DWORD	GetFinishedBytes();
	//
	LONG	Start();
	LONG	Stop();
	//BOOL	IsStop();
	//BOOL	IsCompeleted();
	//
	DWORD	DownloadingFunc();
	//----------------------------------------
private:
	LONG	ConnectInternet();
	LONG	DisconnectInternet();
private:
	HINTERNET	m_hInternetSession;
	//----------------------------------------
};
//--------------------------------------------
#endif	//_HTTPDOWNLOAD_THREADSINGLE_H_
//--------------------------------------------

