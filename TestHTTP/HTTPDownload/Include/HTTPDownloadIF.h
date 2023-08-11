//--------------------------------------------
//	File Name:HTTPDownloadIF.h
//	Introduce:The interface funcitons definition
//	Date:2010-05-10
//--------------------------------------------
#ifndef	_HTTPDOWNLOAD_IF_H
#define _HTTPDOWNLOAD_IF_H
//--------------------------------------------
#include "HTTPDownloadDefine.h"
//--------------------------------------------
//	Name:IHttpDownloadIF
//	Introduce:Http download interface definitions
//--------------------------------------------
class IHttpDownloadIF
{
public:
	IHttpDownloadIF() {};
	virtual ~IHttpDownloadIF() {};

	/*	Name:Initialize
		Introduce:Initialize http downloader, this MUST be called at first
		Param:None
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
	*/
	virtual LONG Initialize() = 0;
	
	/*	Name:Configure
		Introduce:Configure downloading common parameters for tasks
		Param:cfg:[In]:Configure parameter data
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
	*/
	virtual	LONG Configure(HttpDownloadCfg &cfg) = 0;

	/*	Name:SetLogFile
		Introduce:Set enable or disable log file function, and set the base log file name
		bEnable:[In]:Log file is enabled or not
		lpLogFileBase:[In]:Base log file name, if bEnable = TRUE.
		Return:Return HTTP_E_NOERROR always
	*/
	virtual LONG SetLogFile(BOOL bEnable = FALSE, LPCTSTR lpLogFileBase = NULL) = 0;

	/*	Name:CreateTask
		Introduce:Create a downloading task
		Param:lpURL:[In]:Net file URL string, which we want to download
			  lpTargetFile:[In]:The save as file name string in hard disc
			  dwThreadCnt:[In]:How many threads will be created for this task. If dwThreadCnt is NOT in [1, cfg.dwMaxThreadCnt], this calling will return HTTP_E_MAXTHREADCNT_ERROR
			  bForceDownload:[In]:If set bForceDownload = TRUE, task will download file from 0, otherwise try to do a continue downloading
			  dwPort:[In]:Host port, please use the default value
			  lpUserName,lpPassword:[In]:Use to connect host, can use the default value for almost cases
		Return:If return value > 0, it is the new task ID. Otherwise it is an error code.
			   This is a synchronous function, it doesn't return until creating is compeleted. 
			   This will take a few seconds, it bases on thread counts and internet.
	*/
	virtual LONG CreateTask(LPCTSTR lpURL, LPCTSTR lpTargetFile, DWORD dwThreadCnt = 0, BOOL bForceDownload = FALSE, DWORD dwPort = 80, LPCTSTR lpUserName = NULL, LPCTSTR lpPassword = NULL) = 0;
	
	/*	Name:DeleteTask
		Introduce:Delete a task object
		Param:lTaskID:[In]:Task ID, which was return by calling CreateTask()
			  bDeleteFile:[In]:If bDeleteFile = TRUE, the un-completed downloaded file will be deleted too 
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
	*/
	virtual LONG DeleteTask(LONG lTaskID, BOOL bDeleteFile = FALSE) = 0;

	/*	Name:GetTaskCount
		Introduce:Return current total tasks count
		Param:None
		Return:Tasks count
	*/
	virtual LONG GetTaskCount() = 0;

	/*	Name:GetTaskByIndex
		Introduce:Return task ID based on index
		Param:iIndex:[In]:The index of task
		Return:If return value > 0, it isTasks ID. Otherwise it is an error code
	*/
	virtual LONG GetTaskByIndex(int iIndex) = 0;
	
	/*	Name:StartAllTasks
		Introduce:Try to start all tasks
		Param:iIndex:None
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
		       This is an asynchronous function, it returns immediately and will call callback funtion when tasks are started. 
	*/
	virtual LONG StartAllTasks() = 0;

	/*	Name:StopAllTasks
		Introduce:Try to stop all tasks
		Param:iIndex:None
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
		       This is a synchronous function, it doesn't return until stopping is compeleted. 
			   This will take a few seconds, it bases on task count, thread counts and internet.
	*/
	virtual LONG StopAllTasks() = 0;

	/*	Name:StartTask
		Introduce:Try to start a task
		Param:lTaskID:[In]:Task ID
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
		       This is an asynchronous function, it returns immediately and will call callback funtion when task is started. 
	*/
	virtual LONG StartTask(LONG lTaskID) = 0;

	/*	Name:StopTask
		Introduce:Try to stop a task
		Param:lTaskID:[In]:Task ID
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
		       This is a synchronous function, it doesn't return until stopping is compeleted. 
			   This will take a few seconds, it bases on thread counts and internet.
	*/
	virtual LONG StopTask(LONG lTaskID) = 0;	
	
	/*	Name:GetTaskNetFileSize
		Introduce:Return the total file size in web, in bytes
		Param:lTaskID:[In]:Task ID
		Return:If return value > 0, it is the file size. Otherwise it is an error code
	*/
	virtual LONG GetTaskNetFileSize(LONG lTaskID) = 0;	
	
	/*	Name:GetTaskStatus
		Introduce:Return current task status, application can use this API to get task status in without callback function case
		Param:lTaskID:[In]:Task ID
		Return:If return value > 0, it is current task status. Otherwise it is an error code
	*/
	virtual LONG GetTaskStatus(LONG lTaskID) = 0;	

	/*	Name:SetTaskThreadCnt
		Introduce:Set a task thread count, DON'T call this API for a running task.
		Param:lTaskID:[In]:Task ID
			  dwThreadCnt:[In]:Thew new thread count
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
	*/	
	virtual LONG SetTaskThreadCnt(LONG lTaskID, DWORD dwThreadCnt) = 0;

	/*	Name:GetTaskInfo
		Introduce:Return task information
		Param:lTaskID:[In]:Task ID
		      lpThreadCnt:[Out]:Thread count of this task
			  lpURL:[Out]:Web file URL string, make sure lpURL has enough size to reserve data.
			  lpTargetFile:[Out]:Saved file name string, make sure lpTargetFile has enough size to reserve data.
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
	*/
	virtual LONG GetTaskInfo(LONG lTaskID, LPDWORD lpThreadCnt, LPTSTR lpURL, LPTSTR lpTargetFile) = 0;

	/*	Name:GetTaskProgress
		Introduce:Return current task progress information, application can use this API to get task progress in without callback function case
		Param:lTaskID:[In]:Task ID
		      dwFinishedBytes:[Out]:Finished data size, in bytes
		      dwCurrentSpeed:[Out]:Current downloading speed, in bytes/sec
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
	*/
	virtual LONG GetTaskProgress(LONG lTaskID, DWORD &dwFinishedBytes, DWORD &dwCurrentSpeed) = 0;

	/*	Name:CreateStreamTask
		Introduce:Create a stream downloading task
		Param:lpURL:[In]:Net file URL string, which we want to download
			  lpTargetFile:[In]:The save as file name string in hard disc
			  bForceDownload:[In]:If set bForceDownload = TRUE, task will download file from 0, otherwise try to do a continue downloading
			  dwPort:[In]:Host port, please use the default value
			  lpUserName,lpPassword:[In]:Use to connect host, can use the default value for almost cases
		Return:If return value > 0, it is the new task ID. Otherwise it is an error code.
			   This is a synchronous function, it doesn't return until creating is compeleted. 
			   This will take a few seconds, it bases on thread counts and internet.
	*/
	virtual LONG CreateStreamTask(LPCTSTR lpURL, LPCTSTR lpTargetFile, BOOL bForceDownload = FALSE, DWORD dwPort = 80, LPCTSTR lpUserName = NULL, LPCTSTR lpPassword = NULL) = 0;

	/*	Name:DeleteStreamTask
		Introduce:Delete a stream task object
		Param:lStreamTaskID:[In]:Stream task ID, which was return by calling CreateStreamTask()
			  bDeleteFile:[In]:If bDeleteFile = TRUE, the un-completed downloaded file will be deleted too 
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
	*/
	virtual LONG DeleteStreamTask(LONG lStreamTaskID, BOOL bDeleteFile = FALSE) = 0;

	/*	Name:AddStreamTaskThread
		Introduce:Add a download thread to stream task
		Param:lStreamTaskID:[In]:Stream task ID, which was return by calling CreateStreamTask()
			  lThreadID:[In]:Thread ID, MUST > 0, should be set by caller.
			  dwStartPos:[In]:Data range start position, in bytes.
			  dwDataLength:[In]:Data range length, in bytes.
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
	*/
	virtual LONG AddStreamTaskThread(LONG lStreamTaskID, LONG lThreadID, DWORD dwStartPos, DWORD dwDataLength) = 0;
	
	/*	Name:GetStreamTaskThreadInfo
		Introduce:Get a download thread task information
		Param:lStreamTaskID:[In]:Stream task ID, which was return by calling CreateStreamTask()
			  lThreadID:[In]:Thread ID, should be set by caller.
			  dwStartPos:[Out]:Data range start position, in bytes.
			  dwDataLength:[Out]:Data range length, in bytes.
			  dwFinished:[Out]:Current finished data size, in bytes.
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
	*/
	virtual LONG GetStreamTaskThreadInfo(LONG lStreamTaskID, LONG lThreadID, DWORD &dwStartPos, DWORD &dwDataLength, DWORD &dwFinished) = 0;;
		
	/*	Name:DelStreamTaskThread
		Introduce:Delete a download thread in a stream task
		Param:lStreamTaskID:[In]:Stream task ID, which was return by calling CreateStreamTask()
			  lThreadID:[In]:Thread ID, should be set by caller.
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
	*/	
	virtual LONG DelStreamTaskThread(LONG lStreamTaskID, LONG lThreadID) = 0;
			
	/*	Name:StartStreamTaskThread
		Introduce:Start a download thread task in a stream task
		Param:lStreamTaskID:[In]:Stream task ID, which was return by calling CreateStreamTask()
			  lThreadID:[In]:Thread ID, should be set by caller.
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
	*/	
	virtual LONG StartStreamTaskThread(LONG lStreamTaskID, LONG lThreadID) = 0;
	
	/*	Name:StopStreamTaskThread
		Introduce:Stop a download thread task in a stream task
		Param:lStreamTaskID:[In]:Stream task ID, which was return by calling CreateStreamTask()
			  lThreadID:[In]:Thread ID, should be set by caller.
		Return:Return HTTP_E_NOERROR if successfully, otherwise return an error code
	*/
	virtual LONG StopStreamTaskThread(LONG lStreamTaskID, LONG lThreadID) = 0;
};
//--------------------------------------------
#endif	//_HTTPDOWNLOAD_IF_H
//--------------------------------------------