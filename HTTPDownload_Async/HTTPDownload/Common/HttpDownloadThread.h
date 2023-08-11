//--------------------------------------------
//	File Name:HTTPDownloadThread.h
//	Introduce:Class CHttpDownloadThread
//	Date:2010-05-10
//--------------------------------------------
#ifndef	_HTTPDOWNLOAD_THREAD_H_
#define	_HTTPDOWNLOAD_THREAD_H_
//--------------------------------------------
#include "HTTPDownloadDefine.h"
#include "InternalCommon.h"
#include <Winsock.h>
//--------------------------------------------
//	Name:CHttpDownloadThread
//	Introduce:Class CHttpDownloadThread
//--------------------------------------------
class CHttpDownloadThread
{
public:
	CHttpDownloadThread(void);
	~CHttpDownloadThread(void);
	//----------------------------------------
public:
	void	SetThreadID(DWORD dwID);
	DWORD	GetThreadID();
	LONG	SetDownloadInfo(HINTERNET hConnect, HANDLE hTargetFile, LPTHREADINFO lpInfo);
	void	GetDownloadInfo(LPTHREADINFO lpInfo);
	void	SetDataSegments(DWORD dwSegmentCnt, LPDATASEGMENTINFO lpDataSegments);
	DWORD	GetDataSegmentCnt();
	void	GetDataSegments(LPDATASEGMENTINFO lpDataSegments);
	void	SetCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
	DWORD	GetFinishedBytes();
	//
	LONG	Start();
	LONG	Stop();
	BOOL	IsStop();
	BOOL	IsCompeleted();
	//
	DWORD	DownloadingFunc();
	//----------------------------------------
    HANDLE	m_hEvent[3];   
	static	VOID CALLBACK AsyncInternetCallback(HINTERNET hInternet, DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	BOOL	WaitExitEvent();
	//----------------------------------------
private:
	LONG	OpenNetFile();
	LONG	CloseNetFile();
	LONG	CreateDownloadThread();
	LONG	DeleteDownloadThread();
	LONG	CheckAndTrimHeader(LPBYTE lpBuffer, DWORD dwLength);
	LONG	DownloadData(LPBYTE lpBuffer, DWORD dwLength);
	LONG	WriteDataToFile(LPBYTE lpBuffer, DWORD dwLength);
	LONG	WriteTailInfoToFile();
	//----------------------------------------
private:
	DWORD				m_dwID;
	HINTERNET 			m_hConnect;
	HINTERNET			m_hNetFile;
	HANDLE				m_hTargetFile;
	DWORD				m_dwSegmentsCnt;
	ThreadInfo			m_stuDownInfo;
	LPDATASEGMENTINFO	m_lpSegments;
	LPCRITICAL_SECTION	m_lpCriticalSection;
	//
	HANDLE				m_hThread;
	BOOL				m_bCompeleted;
	BOOL				m_bRequireExit;
	//----------------------------------------
	SOCKET				m_Socket;
	//----------------------------------------
};
//--------------------------------------------
#endif	//_HTTPDOWNLOAD_THREAD_H_
//--------------------------------------------
