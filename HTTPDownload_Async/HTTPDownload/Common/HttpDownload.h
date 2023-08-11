//--------------------------------------------
//	File Name:HTTPDownload.h
//	Introduce:Class CHttpDownload, it implements IHttpDownloadIF interface
//	Date:2010-05-10
//--------------------------------------------
#ifndef	_HTTPDOWNLOAD_H_
#define	_HTTPDOWNLOAD_H_
//--------------------------------------------
#include "HTTPDownloadIF.h"
#include "HttpDownloadTask.h"
#include "HttpDownloadStreamTask.h"
#include <vector>
//--------------------------------------------
using namespace std;
typedef vector<CHttpDownloadTask*>			TaskPtrArray;
typedef vector<CHttpDownloadStreamTask*>	StreamTaskPtrArray;
//--------------------------------------------
class CHttpDownload : public IHttpDownloadIF
{
public:
	CHttpDownload(void);
	~CHttpDownload(void);
	//----------------------------------------
public:
	LONG Initialize();
	LONG Configure(HttpDownloadCfg &cfg);
	LONG CreateTask(LPCTSTR lpURL, LPCTSTR lpTargetFile, DWORD dwThreadCnt = 0, BOOL bForceDownload = FALSE, DWORD dwPort = 80, LPCTSTR lpUserName = NULL, LPCTSTR lpPassword = NULL);
	LONG DeleteTask(LONG lTaskID, BOOL bDeleteFile = FALSE);
	LONG GetTaskCount();
	LONG GetTaskByIndex(int iIndex);
	LONG StartAllTasks();
	LONG StopAllTasks();
	//
	LONG StartTask(LONG lTaskID);
	LONG StopTask(LONG lTaskID);
	LONG GetTaskNetFileSize(LONG lTaskID);
	LONG GetTaskStatus(LONG lTaskID);
	LONG SetTaskThreadCnt(LONG lTaskID, DWORD dwThreadCnt);
	LONG GetTaskInfo(LONG lTaskID, LPDWORD lpThreadCnt, LPTSTR lpURL, LPTSTR lpTargetFile);
	LONG GetTaskProgress(LONG lTaskID, DWORD &dwFinishedBytes, DWORD &dwCurrentSpeed);
	//	
	LONG CreateStreamTask(LPCTSTR lpURL, LPCTSTR lpTargetFile, BOOL bForceDownload = FALSE, DWORD dwPort = 80, LPCTSTR lpUserName = NULL, LPCTSTR lpPassword = NULL);
	LONG DeleteStreamTask(LONG lStreamTaskID, BOOL bDeleteFile = FALSE);
	LONG AddStreamTaskThread(LONG lStreamTaskID, LONG lThreadID, DWORD dwStartPos, DWORD dwDataLength);
	LONG GetStreamTaskThreadInfo(LONG lStreamTaskID, LONG lThreadID, DWORD &dwStartPos, DWORD &dwDataLength, DWORD &dwFinished);
	LONG DelStreamTaskThread(LONG lStreamTaskID, LONG lThreadID);
	LONG StartStreamTaskThread(LONG lStreamTaskID, LONG lThreadID);
	LONG StopStreamTaskThread(LONG lStreamTaskID, LONG lThreadID);
	//
	LONG OnUpdateTaskProgress(LONG lTaskID, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2);
	//----------------------------------------
private:
	/*Helper functions*/
	DWORD	CreateTaskID(BOOL bIsStreamTask = FALSE);
	LONG	ParseURL(LPCTSTR lpURL, LPTSTR lpHost, LPTSTR lpPort, LPTSTR lpNetFile);
	LONG	ConvertPortNum(LPCTSTR lpPortNum, DWORD &dwPortInURL);
	DWORD	GetTaskCntByStatus(eHTTPTaskStatus eStatus);
	void	PickWaitingTaskToRun();
	CHttpDownloadTask* GetTaskByID(DWORD dwID);
	CHttpDownloadStreamTask* GetStreamTaskByID(DWORD dwSteamTaskID);
	//
	LONG	OpenInternet();
	LONG	CloseInternet();
	//----------------------------------------
private:
	BOOL				m_bInitialized;
	HttpDownloadCfg		m_stuDownloadCfg;
	TaskPtrArray		m_arTask;
	StreamTaskPtrArray	m_arStreamTask;
	//
	HINTERNET			m_hSession;
	//----------------------------------------
};
//--------------------------------------------
#endif	//_HTTPDOWNLOAD_H_
//--------------------------------------------
