//--------------------------------------------
//	File Name:HTTPDownloadStreamTask.h
//	Introduce:Class CHttpDownloadStreamTask
//	Date:2010-06-05
//--------------------------------------------
#ifndef	_HTTPDOWNLOAD_STREAMTASK_H_
#define	_HTTPDOWNLOAD_STREAMTASK_H_
//--------------------------------------------
#include "HttpDownloadTask.h"
#include <vector>
#include <map>
//--------------------------------------------
typedef vector<CHttpDownloadThread*>	DownloadThreadPtrArray;
typedef map<LONG, DWORD>	ThreadFinishedBytesMap;
typedef map<LONG, DWORD>	ThreadSegmentIndexMap;
typedef map<LONG, BOOL>		ThreadCompletedMap;
//--------------------------------------------
//	Name:CHttpDownloadStreamTask
//	Introduce:Class CHttpDownloadSteamTask
//--------------------------------------------
class CHttpDownloadStreamTask :
	public CHttpDownloadTask
{
public:
	CHttpDownloadStreamTask(void);
	~CHttpDownloadStreamTask(void);
	//----------------------------------------
public:	
	LONG	SetTaskInfo(HINTERNET hSession, TaskInfo Info, BOOL bForceDownload = FALSE);
	LONG	Start();
	LONG	Stop();
	void	SetAsWaiting();
	void	GetCurProgress(DWORD &dwFinishedBytes, DWORD &dwSpeed);
    //
	LONG	AddDownloadThread(LONG lThreadID, DWORD dwStartPos, DWORD dwDataLength);
	LONG	GetStreamTaskThreadInfo(LONG lThreadID, DWORD &dwStartPos, DWORD &dwDataLength, DWORD &dwFinished);
	LONG	DelDownloadThread(LONG lThreadID);
	LONG	StartDownloadThread(LONG lThreadID);
	LONG	StopDownloadThread(LONG lThreadID);
	//----------------------------------------
protected:
	LONG	WriteCommonTailInfo();
	LONG	WriteInitSegmentsInfo();
	LONG	WriteTotalTailSize();
	LONG	CheckSegmentIsLegal(DataSegmentInfo segInfo);
	LONG	AddSegmentsInfo(DataSegmentInfo &segInfo, DWORD &dwIndex);
	LONG	WriteNewSegmentInfo(DWORD dwSegIndex, DataSegmentInfo segInfo);
	LONG	WriteFreeSegmentsInfo();

	/*LONG	OnStart(LONG lThreadID = -1);
	LONG	OnCompleted(LONG lThreadID = -1);
	LONG	OnRestart(LONG lThreadID = -1);*/
	LONG	OnStop();
	LONG	OnProgress();
	//----------------------------------------
private:
	CHttpDownloadThread*	GetDownloadThreadByID(LONG lThreadID);
	//----------------------------------------
private:
	DownloadThreadPtrArray	m_arDownloadThread;
	ThreadFinishedBytesMap	m_mapThreadFinishedBytes;
	ThreadSegmentIndexMap	m_mapThreadSegmentIndex;
	ThreadCompletedMap		m_mapThreadCompleted;
	//----------------------------------------
};
//--------------------------------------------
#endif	//_HTTPDOWNLOAD_STREAMTASK_H_
//--------------------------------------------
