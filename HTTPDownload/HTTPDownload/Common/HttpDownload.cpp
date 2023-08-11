//--------------------------------------------
//	File Name:HTTPDownload.cpp
//	Introduce:The implement file of class CHttpDownload
//	Date:2010-05-10
//--------------------------------------------
#include "HttpDownload.h"
#include <time.h>
#include <TCHAR.h>
//--------------------------------------------
//	Name:TaskCallbackFunc
//	Introduce:Tasks callback function, downloading tasks
//			  will call this function to update status
//--------------------------------------------
long TaskCallbackFunc(LONG lTaskID,
					  LONG lThreadID, 
					  DWORD dwEvent, 
					  DWORD dwParam1, 
					  DWORD dwParam2, 
					  LPVOID lpUser)
{
	CHttpDownload* pHttpDownload = (CHttpDownload*)lpUser;
    if (NULL != pHttpDownload)
	{
		return pHttpDownload->OnUpdateTaskProgress(	lTaskID, 
													lThreadID,
													dwEvent, 
													dwParam1, 
													dwParam2);
	}
	return 0;
}
//--------------------------------------------



//--------------------------------------------
//	Name:CHttpDownload
//	Introduce:Construction
//--------------------------------------------
CHttpDownload::CHttpDownload(void)
 : m_hSession(NULL)
 , m_bInitialized(FALSE)
 , m_bUseLogFile(FALSE)
 , m_pLogFile(NULL)
{
	memset(&m_stuDownloadCfg, 0, sizeof(HttpDownloadCfg));
}
//--------------------------------------------
//	Name:~CHttpDownload
//	Introduce:Distruction
//--------------------------------------------
CHttpDownload::~CHttpDownload(void)
{
	CHttpDownloadTask* pTask = NULL;
	for (	TaskPtrArray::iterator it = m_arTask.begin();
			it != m_arTask.end();
			it++)
	{
		pTask = *it;
		if (NULL != pTask)
		{
			pTask->Stop();
			delete pTask;
			pTask = NULL;
		}
	}

	CHttpDownloadStreamTask* pStreamTask = NULL;	
	for (	StreamTaskPtrArray::iterator it = m_arStreamTask.begin();
			it != m_arStreamTask.end();
			it++)
	{
		pStreamTask = *it;
		if (NULL != pStreamTask)
		{
			pStreamTask->Stop();
			delete pStreamTask;
			pStreamTask = NULL;
		}
	}

	CloseLogFile();
	CloseInternet();
}
//--------------------------------------------



//--------------------------------------------
//	Name:Initialize
//	Introduce:Initialize the Http download, this
//			  function MUST be called at first
//--------------------------------------------
LONG CHttpDownload::Initialize()
{   
	LONG lRes = HTTP_E_NOERROR;

	// Try to open internet
	GIF(OpenInternet());

	// For CreateTaskID()
	srand((unsigned)time(NULL));

	// Save flag
	m_bInitialized = TRUE;

	return lRes;
}
//--------------------------------------------
//	Name:Configure
//	Introduce:Configure download parameters 
//--------------------------------------------
LONG CHttpDownload::Configure(HttpDownloadCfg &cfg)
{
	if (FALSE == m_bInitialized)
		return HTTP_E_NOTCALL_INITIALIZE;

	memcpy(&m_stuDownloadCfg, &cfg, sizeof(HttpDownloadCfg));

	if (0 == m_stuDownloadCfg.dwMaxTaskCnt)
		return HTTP_E_MAXTASKCNT_ERROR;

	if (0 == m_stuDownloadCfg.dwMaxThreadCnt)
		return HTTP_E_MAXTHREADCNT_ERROR;

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:SetLogFile
//	Introduce:Set enable or disable log file function, and set the base log file name
//--------------------------------------------
LONG CHttpDownload::SetLogFile(BOOL bEnable, 
							   LPCTSTR lpLogFileBase)
{
	m_bUseLogFile = bEnable;
	if (TRUE == m_bUseLogFile)
	{
		if (NULL != lpLogFileBase && _tcslen(lpLogFileBase) > 0)
			_tcscpy(m_tcBaseLogFileName, lpLogFileBase);
		else
			_tcscpy(m_tcBaseLogFileName, _T("C:\\HttpLog.txt"));

		InitLogFile();
	}
	else
	{
		_tcscpy(m_tcBaseLogFileName, _T("\0"));
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:CreateTask
//	Introduce:Create a new task, return the task ID if successfully,
//			  otherwise return FALSE;
//--------------------------------------------
LONG CHttpDownload::CreateTask(LPCTSTR lpURL, 
							   LPCTSTR lpTargetFile,
							   DWORD dwThreadCnt, 
							   BOOL bForceDownload, 
							   DWORD dwPort, 
							   LPCTSTR lpUserName,
							   LPCTSTR lpPassword)
{
	HTTPLOG1(m_pLogFile, "DONE --> CreateTask(): Create task for %s", lpURL)

	if (FALSE == m_bInitialized)
	{
		HTTPLOG(m_pLogFile, "ERROR --> CreateTask(): Create task failed! Error: Don't call function Initialize().")
		return HTTP_E_NOTCALL_INITIALIZE;
	}

	if (NULL == lpURL || _tcslen(lpURL) == 0)
	{
		HTTPLOG(m_pLogFile, "ERROR --> CreateTask(): Create task failed! Error: lpURL is NULL.")
		return HTTP_E_URL_ERROR;
	}

	if (NULL == lpTargetFile || _tcslen(lpTargetFile) == 0)
	{
		HTTPLOG(m_pLogFile, "ERROR --> CreateTask(): Create task failed! Error: lpTargetFile is NULL.")
		return HTTP_E_TARGET_ERROR;
	}

	if (TRUE == m_stuDownloadCfg.bForceUseSingleThread)
		dwThreadCnt = 1;

	if (dwThreadCnt <= 0 || dwThreadCnt > m_stuDownloadCfg.dwMaxThreadCnt)
		dwThreadCnt = m_stuDownloadCfg.dwMaxThreadCnt;

	LONG lRes = HTTP_E_NOERROR;
	TCHAR tcHostName[HTTP_MAX_BUFF_LEN] = {_T("\0")};
	TCHAR tcPortNum[128] = {_T("\0")};
	TCHAR tcNetFile[HTTP_MAX_BUFF_LEN] = {_T("\0")};
	DWORD dwPortInURL = 0;

	HTTPLOG(m_pLogFile, "DONE --> CreateTask(): Try to parse URL stirng...")
	GIF (ParseURL(lpURL, tcHostName, tcPortNum, tcNetFile))
	HTTPLOG(m_pLogFile, "DONE --> CreateTask(): Parse URL stirng successfully!")

	HTTPLOG(m_pLogFile, "DONE --> CreateTask(): Try to convert port number to string...")
	GIF (ConvertPortNum(tcPortNum, dwPortInURL))
	HTTPLOG(m_pLogFile, "DONE --> CreateTask(): Convert port number to string successfully!")

	DWORD dwNewTaskID = CreateTaskID();
	CHttpDownloadTask* pNewTask = new CHttpDownloadTask();
	pNewTask->SetTaskID(dwNewTaskID);
	
	HTTPLOG1(m_pLogFile, "DONE --> CreateTask(): Task is newed, task ID = %d", dwNewTaskID)

	TaskInfo stuInfo;
	memset(&stuInfo, 0, sizeof(TaskInfo));
	stuInfo.dwPort = dwPortInURL > 0 ? dwPortInURL : dwPort;
	stuInfo.dwThreadCnt = dwThreadCnt > 0 ? dwThreadCnt : m_stuDownloadCfg.dwMaxThreadCnt;
	stuInfo.bForceUseSingleThread = m_stuDownloadCfg.bForceUseSingleThread;
	_tcscpy(stuInfo.tcHostName, tcHostName);
	_tcscpy(stuInfo.tcNetFile, tcNetFile);
	_tcscpy(stuInfo.tcTargetFile, lpTargetFile);
	if (NULL == lpUserName)
		_tcscpy(stuInfo.tcUserName, _T(""));
	else
		_tcscpy(stuInfo.tcUserName, lpUserName);
	if (NULL == lpPassword)
		_tcscpy(stuInfo.tcPassword,  _T(""));
	else
		_tcscpy(stuInfo.tcPassword, lpPassword);
	
	stuInfo.bUseLogFile = m_bUseLogFile;
	if (TRUE == stuInfo.bUseLogFile)
		_tcscpy(stuInfo.tcLogFileBaseName, m_tcBaseLogFileName);

	HTTPLOG(m_pLogFile, "DONE --> CreateTask(): Try to call SetTaskInfo() to set task...")

	lRes = pNewTask->SetTaskInfo(m_hSession, stuInfo, bForceDownload);	

	// If file was downloaded, don't return error
	if (HTTP_E_FILE_ISDOWNLOADED == lRes)
		lRes = HTTP_E_NOERROR;

	// Create task error!
	if (HTTP_E_NOERROR != lRes)
	{
		delete pNewTask;
		pNewTask = NULL;
		HTTPLOG1(m_pLogFile, "ERROR --> CreateTask(): Function SetTaskInfo() return error! Error Code: %d", lRes)
		return lRes;
	}

	pNewTask->SetCallbackFunc(TaskCallbackFunc, this);

	m_arTask.push_back(pNewTask);

	HTTPLOG1(m_pLogFile, "DONE --> CreateTask(): Create new task successfully! Task ID = %d", dwNewTaskID)

	return dwNewTaskID;
}
//--------------------------------------------
//	Name:DeleteTask
//	Introduce:Delete a task based on task ID
//--------------------------------------------
LONG CHttpDownload::DeleteTask(LONG lTaskID, 
							   BOOL bDeleteFile)
{
	HTTPLOG1(m_pLogFile, "DONE --> DeleteTask(): Enter! Try to delete task: %d", lTaskID)

	CHttpDownloadTask* pFoundTask = NULL;
	for (	TaskPtrArray::iterator it = m_arTask.begin();
			it != m_arTask.end();
			it++)
	{
		pFoundTask = *it;
		if (pFoundTask->GetTaskID() == lTaskID)
		{
			pFoundTask->Stop();
			if (TRUE == bDeleteFile)
			{
				pFoundTask->DeleteDownloadedFile();
			}
			m_arTask.erase(it);
			break;
		}
	}

	if (NULL == pFoundTask)
	{
		HTTPLOG1(m_pLogFile, "ERROR --> DeleteTask(): Task: %d is not found in list!", lTaskID)
		return HTTP_E_TASKID_NOTFOUND;
	}

	delete pFoundTask;
	pFoundTask = NULL;
	HTTPLOG1(m_pLogFile, "DONE --> DeleteTask(): Task: %d has been deleted successfully!", lTaskID)

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:GetTaskCount
//	Introduce:Return current task count in list
//--------------------------------------------
LONG CHttpDownload::GetTaskCount()
{
	DWORD dwTaskCnt = (DWORD)m_arTask.size();

	return dwTaskCnt;
}
//--------------------------------------------
//	Name:GetTaskByIndex
//	Introduce:Return current task ID based on index
//--------------------------------------------
LONG CHttpDownload::GetTaskByIndex(int iIndex)
{
	DWORD dwTaskID = 0;
	DWORD dwItIndex = 0;

	for (	TaskPtrArray::iterator it = m_arTask.begin();
			it != m_arTask.end();
			it++, dwItIndex++)
	{
		if (dwItIndex == iIndex)
		{
			dwTaskID = (*it)->GetTaskID();
			break;
		}
	}

	return dwTaskID;
}
//--------------------------------------------
//	Name:StartAllTasks
//	Introduce:Try to start all tasks
//--------------------------------------------
LONG CHttpDownload::StartAllTasks()
{
	for (	TaskPtrArray::iterator it = m_arTask.begin();
			it != m_arTask.end();
			it++)
	{
		StartTask((*it)->GetTaskID());
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:StopAllTasks
//	Introduce:Try to stop all tasks
//--------------------------------------------
LONG CHttpDownload::StopAllTasks()
{
	for (	TaskPtrArray::iterator it = m_arTask.begin();
			it != m_arTask.end();
			it++)
	{
		if (eHttpTaskStatus_Running == (*it)->GetCurrentStatus())
		{
			(*it)->Stop();
		}
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:StartTask
//	Introduce:Try to start a task by task ID
//--------------------------------------------
LONG CHttpDownload::StartTask(LONG lTaskID)
{
	HTTPLOG1(m_pLogFile, "DONE --> StartTask(): Enter! Try to start task: %d", lTaskID)

	CHttpDownloadTask* pFoundTask = NULL;
	pFoundTask = GetTaskByID(lTaskID);
	if (NULL == pFoundTask)
	{
		HTTPLOG1(m_pLogFile, "ERROR --> StartTask(): Task: %d is not found in list!", lTaskID)
		return HTTP_E_TASKID_NOTFOUND;
	}

	LONG lRes = HTTP_E_NOERROR;
	LONG lStatus = pFoundTask->GetCurrentStatus();
	if (eHttpTaskStatus_Running == lStatus)
	{		
		HTTPLOG1(m_pLogFile, "DONE --> StartTask(): Task: %d is in running!", lTaskID)
		return lRes;
	}

	DWORD dwRunningTaskCnt = GetTaskCntByStatus(eHttpTaskStatus_Running);
	if (dwRunningTaskCnt >= m_stuDownloadCfg.dwMaxTaskCnt)
	{
		pFoundTask->SetAsWaiting();
		lRes = HTTP_E_RUNNINGCNT_MEETMAX;
		HTTPLOG1(m_pLogFile, "WARNING --> StartTask(): There are max number running tasks, Task: %d is added in waiting list!", lTaskID)
	}
	else
	{
		lRes = pFoundTask->Start();
		HTTPLOG1(m_pLogFile, "DONE --> StartTask(): Task: %d is started!", lTaskID)
    }

	return lRes;
}
//--------------------------------------------
//	Name:StopTask
//	Introduce:Try to stop a task by task ID
//--------------------------------------------
LONG CHttpDownload::StopTask(LONG lTaskID)
{
	HTTPLOG1(m_pLogFile, "DONE --> StopTask(): Enter! Try to stop task: %d", lTaskID)

	CHttpDownloadTask* pFoundTask = NULL;
	pFoundTask = GetTaskByID(lTaskID);
	if (NULL == pFoundTask)
	{
		HTTPLOG1(m_pLogFile, "ERROR --> StopTask(): Task: %d is not found in list!", lTaskID)
		return HTTP_E_TASKID_NOTFOUND;
	}

	LONG lRes = pFoundTask->Stop();
	HTTPLOG1(m_pLogFile, "DONE --> StopTask(): Task: %d has been stopped successfully!", lTaskID)

	return lRes;
}
//--------------------------------------------
//	Name:GetTaskNetFileSize
//	Introduce:Return the net file size of a task, in bytes
//--------------------------------------------
LONG CHttpDownload::GetTaskNetFileSize(LONG lTaskID)
{
	CHttpDownloadTask* pFoundTask = NULL;
	pFoundTask = GetTaskByID(lTaskID);
	if (NULL == pFoundTask)
		return HTTP_E_TASKID_NOTFOUND;

	LONG lRes = pFoundTask->GetNetFileSize();

	return lRes;
}
//--------------------------------------------
//	Name:GetTaskStatus
//	Introduce:Return current status of a task
//--------------------------------------------
LONG CHttpDownload::GetTaskStatus(LONG lTaskID)
{
	CHttpDownloadTask* pFoundTask = NULL;
	pFoundTask = GetTaskByID(lTaskID);
	if (NULL == pFoundTask)
		return HTTP_E_TASKID_NOTFOUND;

	LONG lStatus = pFoundTask->GetCurrentStatus();

	return lStatus;
}
//--------------------------------------------
//	Name:SetTaskThreadCnt
//	Introduce:Set thread count of a task
//--------------------------------------------
LONG CHttpDownload::SetTaskThreadCnt(LONG lTaskID, DWORD dwThreadCnt)
{
	CHttpDownloadTask* pFoundTask = NULL;
	pFoundTask = GetTaskByID(lTaskID);
	if (NULL == pFoundTask)
		return HTTP_E_TASKID_NOTFOUND;

	if (dwThreadCnt <= 0 || dwThreadCnt > m_stuDownloadCfg.dwMaxThreadCnt)
		dwThreadCnt = m_stuDownloadCfg.dwMaxThreadCnt;

	return pFoundTask->SetThreadCnt(dwThreadCnt); 
}
//--------------------------------------------
//	Name:GetTaskInfo
//	Introduce:Return download information of a task
//--------------------------------------------
LONG CHttpDownload::GetTaskInfo(LONG lTaskID, 
								LPDWORD 
								lpThreadCnt, 
								LPTSTR lpURL, 
								LPTSTR lpTargetFile)
{
	if (NULL == lpThreadCnt || 
		NULL == lpURL ||
		NULL == lpTargetFile)
		return HTTP_E_INVALIDARG;

	CHttpDownloadTask* pFoundTask = NULL;
	pFoundTask = GetTaskByID(lTaskID);
	if (NULL == pFoundTask)
		return HTTP_E_TASKID_NOTFOUND;

	TaskInfo info;
	memset(&info, 0, sizeof(TaskInfo));
	pFoundTask->GetTaskInfo(&info);
	DWORD dwThreadCnt = pFoundTask->GetThreadCnt();

	TCHAR tcURL[HTTP_MAX_BUFF_LEN] = {_T("\0")};
	_stprintf(tcURL, _T("http://%s%s"), info.tcHostName, info.tcNetFile);

	*lpThreadCnt = dwThreadCnt;
	_tcscpy(lpURL, tcURL);
	_tcscpy(lpTargetFile, info.tcTargetFile);

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:GetTaskProgress
//	Introduce:Return current progress information of a task
//--------------------------------------------
LONG CHttpDownload::GetTaskProgress(LONG lTaskID, 
									DWORD &dwFinishedBytes, 
									DWORD &dwCurrentSpeed)
{
	CHttpDownloadTask* pFoundTask = NULL;
	pFoundTask = GetTaskByID(lTaskID);
	if (NULL == pFoundTask)
		return HTTP_E_TASKID_NOTFOUND;

	pFoundTask->GetCurProgress(dwFinishedBytes, dwCurrentSpeed);

	return HTTP_E_NOERROR;
}	
//--------------------------------------------


//--------------------------------------------
//	Name:CreateStreamTask
//	Introduce:Create a stream task, caller can add/start/stop/delete threads of stream task
//--------------------------------------------
LONG CHttpDownload::CreateStreamTask(LPCTSTR lpURL, 
									 LPCTSTR lpTargetFile, 
									 BOOL bForceDownload, 
									 DWORD dwPort, 
									 LPCTSTR lpUserName, 
									 LPCTSTR lpPassword)
{
	if (FALSE == m_bInitialized)
		return HTTP_E_NOTCALL_INITIALIZE;

	if (NULL == lpURL || _tcslen(lpURL) == 0)
		return HTTP_E_URL_ERROR;

	if (NULL == lpTargetFile || _tcslen(lpTargetFile) == 0)
		return HTTP_E_TARGET_ERROR;

	LONG lRes = HTTP_E_NOERROR;
	TCHAR tcHostName[HTTP_MAX_BUFF_LEN] = {_T("\0")};
	TCHAR tcPortNum[128] = {_T("\0")};
	TCHAR tcNetFile[HTTP_MAX_BUFF_LEN] = {_T("\0")};
	DWORD dwPortInURL = 0;

	GIF (ParseURL(lpURL, tcHostName, tcPortNum, tcNetFile))

	GIF (ConvertPortNum(tcPortNum, dwPortInURL))

	DWORD dwNewTaskID = CreateTaskID(TRUE);
	CHttpDownloadStreamTask* pNewStreamTask = new CHttpDownloadStreamTask();
	pNewStreamTask->SetTaskID(dwNewTaskID);
	
	TaskInfo stuInfo;
	stuInfo.dwPort = dwPortInURL > 0 ? dwPortInURL : dwPort;
	stuInfo.dwThreadCnt = 0;	// Don't create thead for stream task
	_tcscpy(stuInfo.tcHostName, tcHostName);
	_tcscpy(stuInfo.tcNetFile, tcNetFile);
	_tcscpy(stuInfo.tcTargetFile, lpTargetFile);
	if (NULL == lpUserName)
		_tcscpy(stuInfo.tcUserName, _T(""));
	else
		_tcscpy(stuInfo.tcUserName, lpUserName);
	if (NULL == lpPassword)
		_tcscpy(stuInfo.tcPassword,  _T(""));
	else
		_tcscpy(stuInfo.tcPassword, lpPassword);
	
	lRes = pNewStreamTask->SetTaskInfo(m_hSession, stuInfo, bForceDownload);
	if (HTTP_E_NOERROR != lRes)
	{
		delete pNewStreamTask;
		pNewStreamTask = NULL;
		return lRes;
	}

	pNewStreamTask->SetCallbackFunc(TaskCallbackFunc, this);

	m_arStreamTask.push_back(pNewStreamTask);

	return dwNewTaskID;
}
//--------------------------------------------
//	Name:DeleteStreamTask
//	Introduce:Delete a stream task
//--------------------------------------------
LONG CHttpDownload::DeleteStreamTask(LONG lStreamTaskID, 
									 BOOL bDeleteFile)
{
	CHttpDownloadStreamTask* pFoundTask = NULL;
	for (	StreamTaskPtrArray::iterator it = m_arStreamTask.begin();
			it != m_arStreamTask.end();
			it++)
	{
		pFoundTask = *it;
		if (pFoundTask->GetTaskID() == lStreamTaskID)
		{
			pFoundTask->Stop();
			if (TRUE == bDeleteFile)
			{
				pFoundTask->DeleteDownloadedFile();
			}
			m_arStreamTask.erase(it);
			break;
		}
	}

	if (NULL == pFoundTask)
		return HTTP_E_TASKID_NOTFOUND;

	delete pFoundTask;
	pFoundTask = NULL;

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:AddStreamTaskThread
//	Introduce:Add a downloading thread for a stream task
//--------------------------------------------
LONG CHttpDownload::AddStreamTaskThread(LONG lStreamTaskID, 
										LONG lThreadID, 
										DWORD dwStartPos, 
										DWORD dwDataLength)
{
	CHttpDownloadStreamTask* pStreamTask = GetStreamTaskByID(lStreamTaskID);
	if (NULL == pStreamTask)
		return HTTP_E_TASKID_NOTFOUND;

	LONG lRes = HTTP_E_NOERROR;
	lRes = pStreamTask->AddDownloadThread(lThreadID, dwStartPos, dwDataLength);

	return lRes;
}
//--------------------------------------------
//	Name:GetStreamTaskThreadInfo
//	Introduce:Get a downloading thread information of a stream task
//--------------------------------------------
LONG CHttpDownload::GetStreamTaskThreadInfo(LONG lStreamTaskID, 
											LONG lThreadID, 
											DWORD &dwStartPos, 
											DWORD &dwDataLength, 
											DWORD &dwFinished)
{
	CHttpDownloadStreamTask* pStreamTask = GetStreamTaskByID(lStreamTaskID);
	if (NULL == pStreamTask)
		return HTTP_E_TASKID_NOTFOUND;

	LONG lRes = HTTP_E_NOERROR;
	lRes = pStreamTask->GetStreamTaskThreadInfo(lThreadID, dwStartPos, dwDataLength, dwFinished);

	return lRes;
}
//--------------------------------------------
//	Name:DelStreamTaskThread
//	Introduce:Delete a downloading thread for a stream task
//--------------------------------------------
LONG CHttpDownload::DelStreamTaskThread(LONG lStreamTaskID, 
										LONG lThreadID)
{
	CHttpDownloadStreamTask* pStreamTask = GetStreamTaskByID(lStreamTaskID);
	if (NULL == pStreamTask)
		return HTTP_E_TASKID_NOTFOUND;

	LONG lRes = HTTP_E_NOERROR;
	lRes = pStreamTask->DelDownloadThread(lThreadID);

	return lRes;
}
//--------------------------------------------
//	Name:StartStreamTaskThread
//	Introduce:Start a downloading thread in a stream task
//--------------------------------------------
LONG CHttpDownload::StartStreamTaskThread(LONG lStreamTaskID, 
										  LONG lThreadID)
{
	CHttpDownloadStreamTask* pStreamTask = GetStreamTaskByID(lStreamTaskID);
	if (NULL == pStreamTask)
		return HTTP_E_TASKID_NOTFOUND;

	LONG lRes = HTTP_E_NOERROR;
	lRes = pStreamTask->StartDownloadThread(lThreadID);

	return lRes;
}
//--------------------------------------------
//	Name:StopStreamTaskThread
//	Introduce:Stop a downloading thread in a stream task
//--------------------------------------------
LONG CHttpDownload::StopStreamTaskThread(LONG lStreamTaskID, 
										 LONG lThreadID)
{
	CHttpDownloadStreamTask* pStreamTask = GetStreamTaskByID(lStreamTaskID);
	if (NULL == pStreamTask)
		return HTTP_E_TASKID_NOTFOUND;

	LONG lRes = HTTP_E_NOERROR;
	lRes = pStreamTask->StopDownloadThread(lThreadID);

	return lRes;
}
//--------------------------------------------



//--------------------------------------------
//	Name:OnUpdateTaskProgress
//	Introduce:Task update status events handle
//--------------------------------------------
LONG CHttpDownload::OnUpdateTaskProgress(LONG lTaskID, 
										 LONG lThreadID, 
										 DWORD dwEvent, 
										 DWORD dwParam1, 
										 DWORD dwParam2)
{
	// Send the event to applicaiton
	if (NULL != m_stuDownloadCfg.lpHttpCallbackFunc)
	{
		((HttpDownload_CallbackFunc)m_stuDownloadCfg.lpHttpCallbackFunc)(lTaskID, 
																		 lThreadID,
																		 dwEvent, 
																		 dwParam1, 
																		 dwParam2, 
																		 m_stuDownloadCfg.lpUser);
	}

	// If a running task is stopped or completed, pick a waiting task to run
	if (eHTTPCBEvent_StatusChanged == dwEvent)
	{
		if (eHttpTaskStatus_Stopped == dwParam1 ||
			eHttpTaskStatus_Completed == dwParam1)
		{
			PickWaitingTaskToRun();
		}
	}

	return 0;
}
//--------------------------------------------



//--------------------------------------------
//	Name:CreateTaskID
//	Introduce:Return a random number as the new task ID.
//			  The number range is [100000, 499999] for normal task,
//			  and range is [500000, 899999] for stream task,
//--------------------------------------------
DWORD CHttpDownload::CreateTaskID(BOOL bIsStreamTask)
{
	DWORD dwMinTaskID = bIsStreamTask ? HTTP_STREAMTASKID_BASE : HTTP_TASKID_BASE;
	DWORD dwIDRagne = HTTP_STREAMTASKID_BASE - HTTP_TASKID_BASE;

	DWORD dwTaskID = dwMinTaskID;
	dwTaskID = DWORD(dwMinTaskID + (dwIDRagne * rand() / (RAND_MAX + 1.0f)));

	return dwTaskID;
}
//--------------------------------------------
//	Name:ParseURL
//	Introduce:Parse input URL to host name and net file name.
//--------------------------------------------
LONG CHttpDownload::ParseURL(LPCTSTR lpURL, 
							 LPTSTR lpHost,  
							 LPTSTR lpPort, 
							 LPTSTR lpNetFile)
{
	if (NULL == lpURL || _tcslen(lpURL) == 0)
		return HTTP_E_URL_ERROR;

	if (NULL == lpHost || NULL == lpPort || NULL == lpNetFile)
		return HTTP_E_INVALIDARG;

	// To check URL string has protocol or not
	LPCTSTR lpStart = lpURL;
	if (_tcsnicmp(_T("Http://"), lpStart, 7) == 0)
		lpStart += 7;
	
	// Get host name string
	LPCTSTR lp = lpStart;
	BOOL bHasPort = FALSE;
	while (*lp != _T('/'))
	{
		if (*lp == _T(':'))
			bHasPort = TRUE;
		lp++;
	}
	_tcsncpy(lpHost, lpStart, lp-lpStart);
	if (_tcslen(lpHost) == 0)
		return HTTP_E_HOSTNAME_ERROR;

	// Get port number string
	if (TRUE == bHasPort)
	{
		LPCTSTR lpPortPos = lp;
		while (*lpPortPos != _T(':') && lpPortPos > lpStart)
			lpPortPos--;
		if (lpPortPos > lpStart && *lpPortPos == _T(':'))
		{
			// Remove port string from host name string
			memset(lpHost, 0, _tcslen(lpHost));
			_tcsncpy(lpHost, lpStart, lpPortPos-lpStart);
	
			// Copy port string
			lpPortPos++;
			_tcsncpy(lpPort, lpPortPos, lp-lpPortPos);
		}
	}
	
	// Get net file name string
	_tcscpy(lpNetFile, lp);
	if (_tcslen(lpNetFile) == 0)
		return HTTP_E_NETFILE_ERROR;

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:ConvertPortNum
//	Introduce:Convert port string to number
//--------------------------------------------
LONG CHttpDownload::ConvertPortNum(LPCTSTR lpPortNum, DWORD &dwPortInURL)
{
	dwPortInURL = 0;
	LONG lRes = HTTP_E_NOERROR;

	if (NULL !=  lpPortNum &&
		_tcslen(lpPortNum) > 0)
	{
		try
		{
			dwPortInURL = _ttol(lpPortNum);
		}
		catch(...)
		{
			lRes = HTTP_E_URL_ERROR;
		}
	}

	return lRes;
}
//--------------------------------------------
//	Name:GetTaskCntByStatus
//	Introduce:Return tasks count which have the status specificed
//--------------------------------------------
DWORD CHttpDownload::GetTaskCntByStatus(eHTTPTaskStatus eStatus)
{
	DWORD dwCount = 0;
	for (	TaskPtrArray::iterator it = m_arTask.begin();
			it != m_arTask.end();
			it++)
	{
		if ((*it)->GetCurrentStatus() == eStatus)
		{
			dwCount++;
		}
	}

	return dwCount;
}
//--------------------------------------------
//	Name:PickWaitingTaskToRun
//	Introduce:Select a waiting task and try to run it
//--------------------------------------------
void CHttpDownload::PickWaitingTaskToRun()
{
	for (	TaskPtrArray::iterator it = m_arTask.begin();
			it != m_arTask.end();
			it++)
	{
		if (eHttpTaskStatus_Waitting == (*it)->GetCurrentStatus())
		{
			(*it)->Start();
			break;
		}
	}
}
//--------------------------------------------
//	Name:GetTaskByID
//	Introduce:Return task pointer based on task ID
//--------------------------------------------
CHttpDownloadTask* CHttpDownload::GetTaskByID(DWORD dwID)
{
	CHttpDownloadTask* pTask = NULL;

	for (TaskPtrArray::iterator it = m_arTask.begin();
		 it != m_arTask.end();
		 it++)
	{
		if ((*it)->GetTaskID() == dwID)
		{
			pTask = *it;
			break;
		}
	}

	// Try to find this task in stream task list
	if (NULL == pTask)
	{
		pTask = GetStreamTaskByID(dwID);
	}

	return pTask;
}
//--------------------------------------------
//	Name:GetStreamTaskByID
//	Introduce:Return stream task pointer based on task ID
//--------------------------------------------
CHttpDownloadStreamTask* CHttpDownload::GetStreamTaskByID(DWORD dwSteamTaskID)
{
	CHttpDownloadStreamTask* pStreamTask = NULL;

	for (	StreamTaskPtrArray::iterator it = m_arStreamTask.begin();
			it != m_arStreamTask.end();
			it++)
	{
		if ((*it)->GetTaskID() == dwSteamTaskID)
		{
			pStreamTask = *it;
			break;
		}
	}

	return pStreamTask;
}
//--------------------------------------------
//	Name:OpenInternet
//	Introduce:Open internet handle.
//--------------------------------------------
LONG CHttpDownload::OpenInternet()
{ 
	if (NULL != m_hSession)
		return HTTP_E_NOERROR;

	try
	{
		m_hSession = InternetOpen(	_T("Microsoft Internet Explorer"),
									INTERNET_OPEN_TYPE_PRECONFIG,
									NULL,
									NULL,
									0);
		if (NULL == m_hSession)
			return HTTP_E_OPENNET_ERROR;

		DWORD dwMaxConnectPerServer = 100;
		InternetSetOption(NULL, INTERNET_OPTION_MAX_CONNS_PER_SERVER, &dwMaxConnectPerServer, sizeof(DWORD));
		InternetSetOption(NULL, INTERNET_OPTION_MAX_CONNS_PER_1_0_SERVER, &dwMaxConnectPerServer, sizeof(DWORD));

		DWORD dwTimeOut = 20000;
		InternetSetOption(m_hSession, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeOut, sizeof(DWORD));
	}
	catch(...)
	{
		return HTTP_E_OPENNET_ERROR;
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:CloseInternet
//	Introduce:Close internet handle.
//--------------------------------------------
LONG CHttpDownload::CloseInternet()
{
	try
	{
		if (NULL != m_hSession)
		{
			InternetCloseHandle(m_hSession);
			m_hSession = NULL;
		}
	}
	catch(...)
	{
		return HTTP_E_CLOSENET_ERROR;
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:InitLogFile
//	Introduce:Close a openned net file
//--------------------------------------------
void CHttpDownload::InitLogFile()
{
	// Close current log file
	CloseLogFile();

	// Create new log file
	if (TRUE == m_bUseLogFile)
	{
		char csBaseName[MAX_PATH] = {0};
#ifdef	UNICODE
		WideCharToMultiByte(CP_ACP, m_tcBaseLogFileName, -1, csBaseName, MAX_PATH, NULL, FALSE);
#else
		strcpy(csBaseName, m_tcBaseLogFileName);
#endif	//UNICODE
		
		char csLogFile[MAX_PATH] = {0};
		char csLogFileExt[64] = {0};
		const char* lpPos = csBaseName;
		while (*lpPos != '\0') lpPos++;
		while (*lpPos != '.' && *lpPos != '\\') lpPos--;

		int iNum = 0;
		BOOL bIsExist = TRUE;
		do
		{
			sprintf(csLogFileExt, "_%d", iNum++);
			if (*lpPos == '.')
				strcat(csLogFileExt, lpPos);
			else
				strcat(csLogFileExt, ".txt");

			memset(csLogFile, 0, MAX_PATH);
			strncpy(csLogFile, csBaseName, lpPos-csBaseName);
			strcat(csLogFile, csLogFileExt);	

			FILE* f = fopen(csLogFile, "r");
			if (NULL != f)
			{
				fclose(f);
				bIsExist = TRUE;
			}
			else
			{
				bIsExist = FALSE;
			}
		}while (TRUE == bIsExist);

		m_pLogFile = new CHttpLogFile();
		m_pLogFile->Open(csLogFile);
	}
}
//--------------------------------------------
//	Name:CloseLogFile
//	Introduce:Close a openned net file
//--------------------------------------------
void CHttpDownload::CloseLogFile()
{
	if (NULL != m_pLogFile)
	{
		m_pLogFile->Close();
		delete m_pLogFile;
		m_pLogFile = NULL;
	}
}
//--------------------------------------------
