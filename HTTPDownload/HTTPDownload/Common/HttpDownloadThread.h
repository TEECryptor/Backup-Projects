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
#include "HTTPLogFile.h"
#include <Winsock.h>
//--------------------------------------------
//	Name:CHttpDownloadThread
//	Introduce:Class CHttpDownloadThread
//--------------------------------------------
class CHttpDownloadThread
{
public:
	CHttpDownloadThread(void);
	virtual ~CHttpDownloadThread(void);
	//----------------------------------------
public:
	virtual void	SetThreadID(DWORD dwID);
	virtual DWORD	GetThreadID();
	virtual LONG	SetDownloadInfo(HINTERNET hConnect, HANDLE hTargetFile, CHttpLogFile* pLogFile, LPTHREADINFO lpInfo);
	virtual void	GetDownloadInfo(LPTHREADINFO lpInfo);
	virtual void	SetDataSegments(DWORD dwSegmentCnt, LPDATASEGMENTINFO lpDataSegments);
	virtual DWORD	GetDataSegmentCnt();
	virtual void	GetDataSegments(LPDATASEGMENTINFO lpDataSegments);
	virtual void	SetCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
	virtual DWORD	GetFinishedBytes();
	//
	virtual LONG	Start();
	virtual LONG	Stop();
	virtual BOOL	IsStop();
	virtual BOOL	IsCompeleted();
	//
	virtual DWORD	DownloadingFunc();
	//----------------------------------------
protected:	
	virtual LONG	WriteDataToFile(LPBYTE lpBuffer, DWORD dwLength);
	virtual LONG	WriteTailInfoToFile();
	//----------------------------------------
private:
	LONG	OpenNetFile();
	LONG	CloseNetFile();
	LONG	CreateDownloadThread();
	LONG	DeleteDownloadThread();
	LONG	CheckAndTrimHeader(LPBYTE lpBuffer, DWORD dwLength);
	LONG	DownloadData(LPBYTE lpBuffer, DWORD dwLength);
	//----------------------------------------
protected:
	DWORD				m_dwID;
	HINTERNET 			m_hConnect;
	HINTERNET			m_hNetFile;
	HANDLE				m_hTargetFile;
	DWORD				m_dwSegmentsCnt;
	ThreadInfo			m_stuDownInfo;
	LPDATASEGMENTINFO	m_lpSegments;
	CHttpLogFile*		m_pLogFile;
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
