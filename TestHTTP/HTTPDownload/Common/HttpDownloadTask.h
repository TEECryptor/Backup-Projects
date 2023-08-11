//--------------------------------------------
//	File Name:HTTPDownloadTask.h
//	Introduce:Class CHttpDownloadTask
//	Date:2010-05-10
//--------------------------------------------
#ifndef	_HTTPDOWNLOAD_TASK_H_
#define	_HTTPDOWNLOAD_TASK_H_
//--------------------------------------------
#include "HttpDownloadThreadSingle.h"
#include <list>
//--------------------------------------------
using namespace std;
typedef list<DataSegmentInfo>	SegmentInfoList;
//--------------------------------------------
//	Name:CHttpDownloadTask
//	Introduce:Class CHttpDownloadTask
//--------------------------------------------
class CHttpDownloadTask
{
public:
	CHttpDownloadTask(void);
	virtual ~CHttpDownloadTask(void);
	//----------------------------------------
public:
	virtual void	SetTaskID(DWORD dwID);
	virtual DWORD	GetTaskID();
	virtual LONG	SetTaskInfo(HINTERNET hSession, TaskInfo Info, BOOL bForceDownload = FALSE);
	virtual void	GetTaskInfo(LPTASKINFO lpInfo);
	virtual LONG	SetThreadCnt(DWORD dwThreadCnt);
	virtual DWORD	GetThreadCnt();
	virtual void	SetCallbackFunc(LPVOID lpCallbackFunc, LPVOID lpUser);
	//
	virtual LONG	Start();
	virtual LONG	Stop();
	virtual void	SetAsWaiting();
	virtual LONG	GetNetFileSize();
	virtual LONG	GetCurrentStatus();
	virtual void	GetCurProgress(DWORD &dwFinishedBytes, DWORD &dwSpeed);
	virtual void	DeleteDownloadedFile();
	//----------------------------------------
public:
	DWORD	WorkingProgressFunc();
	//----------------------------------------
protected:
	/*Helper functions*/
	LONG	ConnectInternet();
	LONG	DisconnectInternet();
	LONG	OpenNetFile();
	LONG	CloseNetFile();
	BOOL	CanUseMultiThread();
	void	InitLogFile();
	void	CloseLogFile();
	void	LogTaskInformation();
	//
	virtual LONG CalcNetFileSize();
	virtual LONG CheckTargetSpace(DWORD dwNeedSize);
	virtual LONG OpenTargetFile(DWORD dwFileSize, BOOL bOverWrite);
	virtual LONG ParseTargetFile();
	virtual LONG GetSegmentInfoForThread(DWORD dwThreadIndex, LPDATASEGMENTINFO lpSegments);
	virtual LONG WriteCommonTailInfo();
	virtual LONG WriteInitSegmentsInfo();
	virtual LONG WriteTotalTailSize();
	virtual LONG CloseTargetFile();
	virtual LONG CompletedTargetFile();
	virtual LONG CreateDownloadThreads();
	virtual LONG DeleteDownloadThreads();
	virtual LONG CreateTaskThread();
	virtual LONG DeleteTaskThread();
	//
	virtual BOOL TargetFileIsDownloaded();
	virtual BOOL CanDoContinueDownload(LPCTSTR lpszFileName);
	virtual LONG SendEventToParent(LONG lTaskID, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2);
	//
	virtual LONG OnStart();
	virtual LONG OnStop();
	virtual LONG OnCompleted();
	virtual LONG OnRestart();
	virtual LONG OnProgress();
	//----------------------------------------
protected:
	DWORD					m_dwTaskID;
	TaskInfo				m_stuDownloadInfo;
	DWORD					m_dwNetFileSize;
	DWORD					m_dwLastDownloadSize;
	eHTTPTaskStatus			m_eCurStatus;
	HANDLE					m_hProcessThread;
	HANDLE					m_hProcessEvent;
	DWORD					m_dwProcessThreadID;
	CHttpDownloadThread*	m_pDownloadThreads;
	LPVOID					m_pCallbackFunc;
	LPVOID					m_pUserData;
	SegmentInfoList			m_lstSegments;
	//
	HINTERNET				m_hSession;
	HINTERNET 				m_hConnect;
	HINTERNET 				m_hNetFile;
	HANDLE					m_hTargetFile;
	CRITICAL_SECTION		m_csTargetWriting;
	//
	DWORD					m_dwLastTime;
	DWORD					m_dwZeroBytesTime;
	DWORD					m_dwCurFinishedBytes;
	DWORD					m_dwCurrentSpeed;
	BOOL					m_bRequireExit;
	BOOL					m_bIsCompeleted;
	BOOL					m_bDontNeedDownload;
	//
	CHttpLogFile*			m_pLogFile;
	//----------------------------------------
};
//--------------------------------------------
#endif	//_HTTPDOWNLOAD_TASK_H_
//--------------------------------------------