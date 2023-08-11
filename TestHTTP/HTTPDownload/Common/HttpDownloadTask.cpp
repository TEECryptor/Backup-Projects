//--------------------------------------------
//	File Name:HTTPDownloadTask.cpp
//	Introduce:The implement file of class CHttpDownloadTask
//	Date:2010-05-10
//--------------------------------------------
#include "HttpDownloadTask.h"
//--------------------------------------------
//	Name:WorkingThreadFunc
//	Introduce:Public working thread function
//--------------------------------------------
DWORD WINAPI WorkingThreadFunc(LPVOID lpParam) 
{ 
	CHttpDownloadTask* pTask = (CHttpDownloadTask*)lpParam;
	if (NULL != pTask)
		return pTask->WorkingProgressFunc();

    return 0; 
} 
//--------------------------------------------
//	Name:WaitFor
//	Introduce:Our waiting function with dispatch message 
//--------------------------------------------
BOOL WaitFor(HANDLE hEvent)
{
	if (NULL == hEvent)
		return TRUE;

	DWORD nRet = WAIT_TIMEOUT;

	while(WAIT_OBJECT_0 != nRet)
	{
		nRet = WaitForSingleObject(hEvent, 100);

		Sleep(100);

		MSG msg;
		while (PeekMessage (&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage  (&msg);
		}
	}
	return TRUE;
}
//--------------------------------------------



//--------------------------------------------
//	Name:CHttpDownloadTask
//	Introduce:Construction
//--------------------------------------------
CHttpDownloadTask::CHttpDownloadTask(void)
 : m_dwTaskID(0)
 , m_dwNetFileSize(0)
 , m_dwLastDownloadSize(0)
 , m_eCurStatus(eHttpTaskStatus_Ready)
 , m_hProcessThread(NULL)
 , m_hProcessEvent(NULL)
 , m_dwProcessThreadID(0)
 , m_pDownloadThreads(NULL)
 , m_pCallbackFunc(NULL)
 , m_pUserData(NULL)
 , m_hSession(NULL)
 , m_hConnect(NULL)
 , m_hNetFile(NULL)
 , m_hTargetFile(NULL)
 , m_dwLastTime(0)
 , m_dwZeroBytesTime(0)
 , m_dwCurFinishedBytes(0)
 , m_dwCurrentSpeed(0)
 , m_bRequireExit(FALSE)
 , m_bIsCompeleted(FALSE)
 , m_bDontNeedDownload(FALSE)
 , m_pLogFile(NULL)
{
	memset(&m_stuDownloadInfo, 0, sizeof(TaskInfo));
}
//--------------------------------------------
//	Name:~CHttpDownloadTask
//	Introduce:Distruction
//--------------------------------------------
CHttpDownloadTask::~CHttpDownloadTask(void)
{
	DeleteDownloadThreads();
	DeleteTaskThread();
	DisconnectInternet();
	CloseTargetFile();
	//
	CloseLogFile();
}
//--------------------------------------------


//--------------------------------------------
//	Name:SetTaskID
//	Introduce:Set current task ID.
//--------------------------------------------
void CHttpDownloadTask::SetTaskID(DWORD dwID)
{
	m_dwTaskID = dwID;
}
//--------------------------------------------
//	Name:GetTaskID
//	Introduce:Return current task ID.
//--------------------------------------------
DWORD CHttpDownloadTask::GetTaskID()
{
	return m_dwTaskID;
}
//--------------------------------------------
//	Name:SetTaskInfo
//	Introduce:Set the download information for this task.
//--------------------------------------------
LONG CHttpDownloadTask::SetTaskInfo(HINTERNET hSession, TaskInfo Info, BOOL bForceDownload)
{
	LONG lRes = HTTP_E_NOERROR;

	if (NULL == hSession)
		return HTTP_E_OPENNET_ERROR;

	if (_tcslen(Info.tcHostName) == 0)
		return HTTP_E_HOSTNAME_ERROR;

	if (_tcslen(Info.tcNetFile) == 0)
		return HTTP_E_NETFILE_ERROR;

	if (_tcslen(Info.tcTargetFile) == 0)
		return HTTP_E_TARGET_ERROR;

	m_hSession = hSession;
	memcpy(&m_stuDownloadInfo, &Info, sizeof(TaskInfo));

	// Initliaze log file
	InitLogFile();
	LogTaskInformation();

	// Connet to the server
	GIF (ConnectInternet())

	// Calc the net file size in bytes
	lRes = CalcNetFileSize();
	if (lRes <= 0)
		return lRes;
	m_dwNetFileSize = lRes;

	// Initialize critical section for writing target file
	InitializeCriticalSection(&m_csTargetWriting);

	// Check the target hard disc have enough space or not
	GIF (CheckTargetSpace(m_dwNetFileSize))

	// Try to open the target file with NULL data
	GIF (OpenTargetFile(m_dwNetFileSize, bForceDownload))

	// Try to parse the target file, for continue downloading
	GIF (ParseTargetFile())

	// Try to add the tail information at the end of file
	GIF (WriteCommonTailInfo())

	// Create downloading threads
	GIF (CreateDownloadThreads())

	// Write download threads segments for begin
	GIF (WriteInitSegmentsInfo())

	// Write total data size at end of target file
	GIF (WriteTotalTailSize())

	// Create task working thread
	GIF (CreateTaskThread())

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:GetTaskInfo
//	Introduce:Return the download information for this task.
//--------------------------------------------
void CHttpDownloadTask::GetTaskInfo(LPTASKINFO lpInfo)
{
	if (NULL != lpInfo)
	{
		memcpy(lpInfo, &m_stuDownloadInfo, sizeof(TaskInfo));
	}
}
//--------------------------------------------
//	Name:SetThreadCnt
//	Introduce:Set working thread count for this task.
//--------------------------------------------
LONG CHttpDownloadTask::SetThreadCnt(DWORD dwThreadCnt)
{
	HTTPLOG(m_pLogFile, "DONE --> SetThreadCnt(): Enter!");

	LONG lRes = HTTP_E_NOERROR;
	if (eHttpTaskStatus_Running == m_eCurStatus)
	{			
		HTTPLOG(m_pLogFile, "ERROR --> SetThreadCnt(): Can't call this function() for a running task!");
		return HTTP_E_CANTSET_FORRUNNING;
	}

	m_stuDownloadInfo.dwThreadCnt = dwThreadCnt;
	m_dwCurFinishedBytes = 0;

	if (eHttpTaskStatus_Stopped == m_eCurStatus)
	{
		GIF (ParseTargetFile())
	}

	GIF (WriteCommonTailInfo())
	GIF (CreateDownloadThreads())
	GIF (WriteInitSegmentsInfo())
	GIF (WriteTotalTailSize())

	HTTPLOG(m_pLogFile, "DONE --> SetThreadCnt(): Exit!");

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:GetThreadCnt
//	Introduce:Return the thread count of this task.
//--------------------------------------------
DWORD CHttpDownloadTask::GetThreadCnt()
{
	return m_stuDownloadInfo.dwThreadCnt;
}
//--------------------------------------------
//	Name:SetCallbackFunc
//	Introduce:Set the callback function pointer.
//--------------------------------------------
void CHttpDownloadTask::SetCallbackFunc(LPVOID lpCallbackFunc, LPVOID lpUser)
{
	m_pCallbackFunc = lpCallbackFunc;
	m_pUserData = lpUser;
}
//--------------------------------------------
//	Name:Start
//	Introduce:Start current task.
//--------------------------------------------
LONG CHttpDownloadTask::Start()
{	
	// If file was downloaded, send event to parent directly
	// Because we didn't create the working thread in this case.
	if (TRUE == m_bDontNeedDownload)
	{
		m_bIsCompeleted = TRUE;
		m_eCurStatus = eHttpTaskStatus_Completed;
		SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_ProgressChanged, m_dwNetFileSize, 0);
		SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Completed, 0);
		HTTPLOG(m_pLogFile, "DONE --> Start(): This file has been downloaded already, return directly!");
		return HTTP_E_NOERROR;
	}

	// Post the start message to the working thread
	if (eHttpTaskStatus_Ready == m_eCurStatus ||
		eHttpTaskStatus_Waitting == m_eCurStatus ||
		eHttpTaskStatus_Stopped == m_eCurStatus)
	{
		PostThreadMessage(m_dwProcessThreadID, PROC_MSG_START, -1, 0);
		HTTPLOG(m_pLogFile, "DONE --> Start(): PROC_MSG_START message has been posted!");
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:Stop
//	Introduce:Stop current task.
//--------------------------------------------
LONG CHttpDownloadTask::Stop()
{
	if (eHttpTaskStatus_Running != m_eCurStatus)
		return HTTP_E_NOERROR;

	DWORD dwEnterTime = GetTickCount();
	HTTPLOG(m_pLogFile, "DONE --> Stop(): Enter!");

	PostThreadMessage(m_dwProcessThreadID, PROC_MSG_STOP, -1, 0);

	DWORD dwRetryCnt = 0;
	while (	eHttpTaskStatus_Stopped != m_eCurStatus &&
			dwRetryCnt < 50)
	{
		dwRetryCnt++;
		Sleep(100);
	}
	HTTPLOG1(m_pLogFile, "DONE --> Stop(): Exit! Used Time: %ds", (GetTickCount()-dwEnterTime)/1000);
	  
	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:SetAsWaiting
//	Introduce:Set task current status to waiting
//--------------------------------------------
void CHttpDownloadTask::SetAsWaiting()
{
	m_eCurStatus = eHttpTaskStatus_Waitting;
	SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Waitting, 0);
}
//--------------------------------------------
//	Name:GetNetFileSize
//	Introduce:Return the size of net file, in bytes.
//--------------------------------------------
LONG CHttpDownloadTask::GetNetFileSize()
{
	return m_dwNetFileSize;
}
//--------------------------------------------
//	Name:GetCurrentStatus
//	Introduce:Return current status of this task
//--------------------------------------------
LONG CHttpDownloadTask::GetCurrentStatus()
{
	return m_eCurStatus;
}
//--------------------------------------------
//	Name:GetCurProgress
//	Introduce:Return current progress information of this task
//--------------------------------------------
void CHttpDownloadTask::GetCurProgress(DWORD &dwFinishedBytes, DWORD &dwSpeed)
{
	dwFinishedBytes = m_dwLastDownloadSize+m_dwCurFinishedBytes;
	dwSpeed = m_dwCurrentSpeed;
}
//--------------------------------------------
//	Name:DeleteDownloadedFile
//	Introduce:Delete downloaded file
//--------------------------------------------
void CHttpDownloadTask::DeleteDownloadedFile()
{
	CloseTargetFile();

	TCHAR tcDownloadingFile[MAX_PATH] = {_T("\0")};
	_tcscpy(tcDownloadingFile, m_stuDownloadInfo.tcTargetFile);
	_tcscat(tcDownloadingFile, HTTP_DOWNLOADING_EXT);
	DeleteFile(tcDownloadingFile);
}
//--------------------------------------------
//	Name:WorkingProgressFunc
//	Introduce:Working thread function, to check current download progress
//--------------------------------------------
DWORD CHttpDownloadTask::WorkingProgressFunc()
{
	SetEvent(m_hProcessEvent);

	MSG msg;
	LONG lRes = 0;
	do
	{
		while(PeekMessage(&msg, 0, 0, 0,PM_REMOVE|PM_QS_POSTMESSAGE))
		{
			switch(msg.message)
			{
			case PROC_MSG_START:
			/*	if (TRUE == m_bDontNeedDownload)
					PostThreadMessage(m_dwProcessThreadID, PROC_MSG_STOP, -1, 1);
				else*/
					lRes = OnStart();
				break;
			case PROC_MSG_STOP:
				if (1 == msg.lParam)
				{					
					lRes = OnCompleted();
				}
				else
				{
					lRes = OnStop();
				}
				break;
			case PROC_MSG_RESTART:
				lRes = OnRestart();
				if (HTTP_E_FILE_ISDOWNLOADED == lRes)
				{
					SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_ProgressChanged, m_dwNetFileSize, 0);
					SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Completed, 0);
				}
				else if (lRes <= 0)				
				{
					SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_Error, lRes, 0);
				}
				break;
			default:
				SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				break;
			}
		}

		if (eHttpTaskStatus_Running == m_eCurStatus)
		{
			OnProgress();
		}

		Sleep(1000);

	}while (!m_bRequireExit && !m_bIsCompeleted);

	return 0;
}
//--------------------------------------------



//--------------------------------------------
//	Name:ConnectInternet
//	Introduce:Connect to internet.
//--------------------------------------------
LONG CHttpDownloadTask::ConnectInternet()
{
	LONG lRes = HTTP_E_NOERROR;

	// Disconnect the old connect
	GIF(DisconnectInternet());

	try
	{
		m_hConnect = InternetConnect(	m_hSession,
										m_stuDownloadInfo.tcHostName,
										m_stuDownloadInfo.dwPort > 0 ? (INTERNET_PORT)m_stuDownloadInfo.dwPort : INTERNET_DEFAULT_HTTP_PORT,
										_tcslen(m_stuDownloadInfo.tcUserName) > 0 ? m_stuDownloadInfo.tcUserName : NULL,
										_tcslen(m_stuDownloadInfo.tcPassword) > 0 ? m_stuDownloadInfo.tcPassword : NULL,
										INTERNET_SERVICE_HTTP, 
										0, 
										0);
        if (NULL != m_hConnect)
		{
			HTTPLOG(m_pLogFile, "DONE --> ConnectInternet(): Call API InternetConnect() to connect server successfully!");
		}
		else
		{
			DWORD dwError = 0;
			dwError = GetLastError();
			switch (dwError)
			{
			case ERROR_INTERNET_TIMEOUT:
				lRes = HTTP_E_HOSTNAME_ERROR;
				break;
			default:
				lRes = HTTP_E_SENDREQUEST_ERROR;
				break;
			}	
			HTTPLOG1(m_pLogFile, "ERROR --> ConnectInternet(): Call API InternetConnect() failed! Error code: %d", dwError);
		}
	}
	catch(...)
	{
		lRes = HTTP_E_CONNECTNET_ERROR;
	}

	return lRes;
}
//--------------------------------------------
//	Name:DisconnectInternet
//	Introduce:Disconnect to internet.
//--------------------------------------------
LONG CHttpDownloadTask::DisconnectInternet()
{
	try
	{
		if (NULL != m_hConnect)
		{
			InternetCloseHandle(m_hConnect);
			m_hConnect = NULL;
			HTTPLOG(m_pLogFile, "DONE --> DisconnectInternet(): Call API InternetCloseHandle() to disconnect server!");
		}
	}
	catch(...)
	{
		return HTTP_E_DISCONNECTNET_ERROR;
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:OpenNetFile
//	Introduce:Open a the net file URL.
//--------------------------------------------
LONG CHttpDownloadTask::OpenNetFile()
{
	LONG lRes = HTTP_E_NOERROR;

	// Close the openned net file
	GIF(CloseNetFile());
	
	try
	{
		const TCHAR* acceptTYPE[] = {_T("*/*"), 0}; 
		m_hNetFile = HttpOpenRequest(m_hConnect,
									 _T("GET"),
									 m_stuDownloadInfo.tcNetFile,
									 _T("HTTP/1.1"),
									 NULL,
									 acceptTYPE,
									 INTERNET_FLAG_RELOAD,
									 0);
		if (NULL != m_hNetFile)
		{
			if (TRUE == HttpSendRequest(m_hNetFile, NULL, 0, NULL, 0))
			{
				HTTPLOG(m_pLogFile, "DONE --> OpenNetFile(): Call APIs HttpOpenRequest() and HttpSendRequest() to open the net file successfully!");
			}
			else
			{
				DWORD dwError;
				dwError = GetLastError();
				switch (dwError)
				{
				case ERROR_FILE_NOT_FOUND:
					lRes = HTTP_E_NETFILE_ERROR;
					break;
				case ERROR_INTERNET_TIMEOUT:
					lRes = HTTP_E_HOSTNAME_ERROR;
					break;
				default:
					lRes = HTTP_E_SENDREQUEST_ERROR;
					break;
				}			
				HTTPLOG1(m_pLogFile, "ERROR --> OpenNetFile(): Call API HttpSendRequest() to open the net file failed! Error Code: %d", dwError);
			}
		}
		else			
		{
			DWORD dwError;
			dwError = GetLastError();
			lRes = HTTP_E_OPENNETFILE_ERROR;
			HTTPLOG1(m_pLogFile, "ERROR --> OpenNetFile(): Call API HttpOpenRequest() to open the net file failed! Error Code:%d", dwError);
		}
	}
	catch(...)
	{
		lRes = HTTP_E_OPENNETFILE_ERROR;
	}

	return lRes;
}
//--------------------------------------------
//	Name:CloseNetFile
//	Introduce:Close a openned net file
//--------------------------------------------
LONG CHttpDownloadTask::CloseNetFile()
{	
	try
	{		
		if (NULL != m_hNetFile)
		{
			InternetCloseHandle(m_hNetFile);
			m_hNetFile = NULL;
			HTTPLOG(m_pLogFile, "DONE --> CloseNetFile(): Call API InternetCloseHandle() to close the net file!");
		}
	}
	catch(...)
	{
		return HTTP_E_CLOSENETFILE_ERROR;
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:CanUseMultiThread
//	Introduce:To check can use multi-thread downloading or not
//--------------------------------------------
BOOL CHttpDownloadTask::CanUseMultiThread()
{
	BOOL bMultiThreadSupported = FALSE;

	if (m_stuDownloadInfo.dwThreadCnt > 1)
		bMultiThreadSupported = TRUE;

	HTTPLOG1(m_pLogFile, "DONE --> CanUseMultiThread(): Multi-Thread downloading supported: %s", bMultiThreadSupported ? _T("YES") : _T("NO"));

	return bMultiThreadSupported;
}
//--------------------------------------------
//	Name:InitLogFile
//	Introduce:Close a openned net file
//--------------------------------------------
void CHttpDownloadTask::InitLogFile()
{
	// Close current log file
	CloseLogFile();

	// Create new log file
	if (TRUE == m_stuDownloadInfo.bUseLogFile)
	{
		char csBaseName[MAX_PATH] = {0};
#ifdef	UNICODE
		WideCharToMultiByte(CP_ACP, m_stuDownloadInfo.tcLogFileBaseName, -1, csBaseName, MAX_PATH, NULL, FALSE);
#else
		strcpy(csBaseName, m_stuDownloadInfo.tcLogFileBaseName);
#endif	//UNICODE
		
		char csLogFile[MAX_PATH] = {0};
		char csLogFileExt[64] = {0};
		const char* lpPos = csBaseName;
		while (*lpPos != '\0') lpPos++;
		while (*lpPos != '.' && *lpPos != '\\') lpPos--;

		sprintf(csLogFileExt, "_%d", m_dwTaskID);
		if (*lpPos == '.')
			strcat(csLogFileExt, lpPos);
		else
			strcat(csLogFileExt, ".txt");
		strncpy(csLogFile, csBaseName, lpPos-csBaseName);
		strcat(csLogFile, csLogFileExt);		

		m_pLogFile = new CHttpLogFile();
		m_pLogFile->Open(csLogFile);
	}
}
//--------------------------------------------
//	Name:CloseLogFile
//	Introduce:Close a openned net file
//--------------------------------------------
void CHttpDownloadTask::CloseLogFile()
{
	if (NULL != m_pLogFile)
	{
		m_pLogFile->Close();
		delete m_pLogFile;
		m_pLogFile = NULL;
	}
}
//--------------------------------------------
//	Name:LogTaskInformation
//	Introduce:Write this task base information to log file
//--------------------------------------------
void CHttpDownloadTask::LogTaskInformation()
{
	char csHostName[1024] = {0};
	char csNetFileName[1024] = {0};
	char csTargetFile[MAX_PATH] = {0};
#ifdef	UNICODE
	WideCharToMultiByte(CP_ACP, m_stuDownloadInfo.tcHostName, -1, csHostName, MAX_PATH, NULL, FALSE);
	WideCharToMultiByte(CP_ACP, m_stuDownloadInfo.tcNetFile, -1, csNetFileName, MAX_PATH, NULL, FALSE);
	WideCharToMultiByte(CP_ACP, m_stuDownloadInfo.tcTargetFile, -1, csTargetFile, MAX_PATH, NULL, FALSE);
#else
	strcpy(csHostName, m_stuDownloadInfo.tcHostName);
	strcpy(csNetFileName, m_stuDownloadInfo.tcNetFile);
	strcpy(csTargetFile, m_stuDownloadInfo.tcTargetFile);
#endif	//UNICODE
	
	char csURL[1024] = {0};
	strcpy(csURL, "Http://");
	strcat(csURL, csHostName);
	strcat(csURL, csNetFileName);
	HTTPLOG(m_pLogFile, "-----------------------------Task Base Information----------------------------------");
	HTTPLOG1(m_pLogFile, "Task ID: %d", m_dwTaskID);
	HTTPLOG1(m_pLogFile, "Net File URL: %s", csURL);
	HTTPLOG1(m_pLogFile, "Target File Name: %s", csTargetFile);
	HTTPLOG1(m_pLogFile, "Initliazed Thread Count: %d", m_stuDownloadInfo.dwThreadCnt);
	HTTPLOG(m_pLogFile, "------------------------------------------------------------------------------------");
}
//--------------------------------------------
//	Name:CalcNetFileSize
//	Introduce:Return Return the size of net file, in bytes if succesfully.
//			  Otherwise return an error code.
//--------------------------------------------
LONG CHttpDownloadTask::CalcNetFileSize()
{
	DWORD dwNetFileSize = 0;
	LONG lRes = HTTP_E_NOERROR;

	try
	{
		GIF(OpenNetFile()); 

		BOOL bRes = TRUE;
		DWORD dwIndex = 0;
		DWORD dwBufLen = HTTP_MAX_BUFF_LEN;
		TCHAR tcBuffer[HTTP_MAX_BUFF_LEN] = {_T("\0")};
		
		// Check status
		bRes = HttpQueryInfo(	m_hNetFile,  
								HTTP_QUERY_STATUS_CODE, 
								tcBuffer, 
								&dwBufLen, 
								&dwIndex);
		long lStatusCode = _tstol(tcBuffer);
		if (HTTP_STATUS_OK != lStatusCode)
		{
			DWORD dwError;
			dwError = GetLastError();
			HTTPLOG2(m_pLogFile, "ERROR --> CalcNetFileSize(): Call API HttpQueryInfo() to query status failed! Error Code: %d, Status: %d", dwError, lStatusCode); 

			CloseNetFile();
			return -1;
		}

		HTTPLOG1(m_pLogFile, "DONE --> CalcNetFileSize(): Call API HttpQueryInfo() to query status successfully! Status: %d", lStatusCode); 

		// Get file size from head information
		dwBufLen = HTTP_MAX_BUFF_LEN;
		memset(tcBuffer, 0, HTTP_MAX_BUFF_LEN);
		bRes = HttpQueryInfo(	m_hNetFile,
								HTTP_QUERY_CONTENT_LENGTH/*HTTP_QUERY_CONTENT_RANGE*/,
								tcBuffer, 
								&dwBufLen,
								&dwIndex);

		// Failed by get header, try to get size again by seeking file pointer
		if (FALSE == bRes || _tcslen(tcBuffer) == 0) 
		{
			DWORD dwError;
			dwError = GetLastError();
			HTTPLOG1(m_pLogFile, "ERROR --> CalcNetFileSize(): Call API HttpQueryInfo() to query file size failed! Error Code: %d", dwError); 

			CloseNetFile();
			return -1;
		}
		else
		{
			dwNetFileSize = _ttol(tcBuffer);
			HTTPLOG1(m_pLogFile, "DONE --> CalcNetFileSize(): Call API HttpQueryInfo() to query file size successfully! File size: %d Bytes", dwNetFileSize); 
		}

		lRes = CloseNetFile();
	}
	catch(...)
	{
		dwNetFileSize = 0;;
	}

	return dwNetFileSize;
}
//--------------------------------------------
//	Name:CheckTargetSpace
//	Introduce:Return TRUE if the target disc free space is enough.
//			  Otherwise return an error code.
//--------------------------------------------
LONG CHttpDownloadTask::CheckTargetSpace(DWORD dwNeedSize)
{
	LONG lRes = HTTP_E_DISCFREESPACE_ERROR;

	// Get target folder string based on target file name
	TCHAR tcTargetFolder[MAX_PATH] = {_T("\0")};
	LPCTSTR lp = m_stuDownloadInfo.tcTargetFile;
	while(*lp != _T('\0'))
	{
		lp++;
	}
	while(*lp != _T('\\') && *lp != _T('/'))
	{
		lp--;
	}
	_tcsncpy(tcTargetFolder, m_stuDownloadInfo.tcTargetFile, lp-m_stuDownloadInfo.tcTargetFile);

	// Get free space size in bytes
	ULARGE_INTEGER  liDiskFreeSpace; 
	memset(&liDiskFreeSpace, 0, sizeof(ULARGE_INTEGER));
	GetDiskFreeSpaceEx(	tcTargetFolder,
						&liDiskFreeSpace,
						NULL,
						NULL);

	// Leave 50K for addition
	if (dwNeedSize < (liDiskFreeSpace.QuadPart + 50*1024))
	{
		lRes = HTTP_E_NOERROR;
	}
	
	HTTPLOG1(m_pLogFile, "DONE --> CheckTargetSpace(): Check target disc free space finished! Result:%s", lRes==HTTP_E_NOERROR ? "YES" : "NO"); 

	return lRes;
}
//--------------------------------------------
//	Name:OpenTargetFile
//	Introduce:Return TRUE if the target file opened successfully.
//			  Otherwise return an error code.
//--------------------------------------------
LONG CHttpDownloadTask::OpenTargetFile(DWORD dwFileSize, BOOL bOverWrite)
{
	LONG lRes = HTTP_E_OPENTARGET_ERROR;

	GIF(CloseTargetFile());

	// If server can't use multi-thread, we need to download it from file begin by single thread
	if (!CanUseMultiThread())
		bOverWrite = TRUE;

	try
	{
		if (TRUE == bOverWrite)
		{
			// Delete the file same name as completed target file
			DeleteFile(m_stuDownloadInfo.tcTargetFile);
		}
		else
		{
			if (!TargetFileIsDownloaded())
				DeleteFile(m_stuDownloadInfo.tcTargetFile);
			else
				m_bDontNeedDownload = TRUE;
		}

		// If file was downloaded, return directly
		if (TRUE == m_bDontNeedDownload)
		{
			DisconnectInternet();			
			HTTPLOG(m_pLogFile, "DONE --> OpenTargetFile(): File has been downloaded, return directly!"); 
			return HTTP_E_FILE_ISDOWNLOADED;
		}

		// Downloading project file name
		TCHAR tcDownloadingFile[MAX_PATH] = {_T("\0")};
		_tcscpy(tcDownloadingFile, m_stuDownloadInfo.tcTargetFile);
		_tcscat(tcDownloadingFile, HTTP_DOWNLOADING_EXT);

		// If there is a same project file, over write it 
		// if it is not a WebMax downloading project file.
		if (!CanDoContinueDownload(tcDownloadingFile))
			bOverWrite = TRUE;

		// Open the downloading project file
		m_hTargetFile = CreateFile( tcDownloadingFile,
									GENERIC_READ | GENERIC_WRITE,
									FILE_SHARE_READ | FILE_SHARE_WRITE,
									NULL,
									bOverWrite ? CREATE_ALWAYS : OPEN_ALWAYS,
									FILE_ATTRIBUTE_NORMAL,
									NULL);
		if (INVALID_HANDLE_VALUE == m_hTargetFile)
		{
			m_hTargetFile = NULL;
			DWORD dwError = GetLastError();
			switch(dwError)
			{
			case INVALID_HANDLE_VALUE:	// This file is using by other application
				lRes = HTTP_E_OPENTARGET_ERROR;
				break;
			default:					// The target disc is read-only
				lRes = HTTP_E_DISCWRITE_ERROR;
			}
			HTTPLOG1(m_pLogFile, "ERROR --> OpenTargetFile(): Call API CreateFile() to create target file failed! Error Code: %d", dwError); 
		}		
		else
		{
			lRes = HTTP_E_NOERROR;
			HTTPLOG(m_pLogFile, "DONE --> OpenTargetFile(): Target file has been openned sucessfully!"); 
		}
	}
	catch(...)
	{
		lRes = HTTP_E_OPENTARGET_ERROR;
	}

	return lRes;
}
//--------------------------------------------
//	Name:ParseTargetFile
//	Introduce:Parse the openned target file, to check
//			  which data segments have not downloaded
//--------------------------------------------
LONG CHttpDownloadTask::ParseTargetFile()
{
	// At first, there is a segment for whole file
	DataSegmentInfo segDefault;
	segDefault.dwStartPos = 0;
	segDefault.dwFinished = 0;
	segDefault.dwDataLength = m_dwNetFileSize;
	m_lstSegments.push_back(segDefault);

	// Check file size at first
	DWORD dwFileSize = 0;
	SetFilePointer(m_hTargetFile, 0, NULL, FILE_BEGIN);
	dwFileSize = SetFilePointer(m_hTargetFile, 0, NULL, FILE_END);
	if (dwFileSize <= sizeof(TempFileTailer) + sizeof(DWORD))
	{		
		HTTPLOG(m_pLogFile, "DONE --> ParseTargetFile(): Target file is a new file!"); 
		return HTTP_E_NOERROR;
	}

	DWORD dwReadBytes = 0;

	// Get tail data size
	DWORD dwTailDataSize = 0;
	SetFilePointer(m_hTargetFile, -1*(int)sizeof(DWORD), NULL, FILE_END);
	ReadFile(m_hTargetFile, &dwTailDataSize, sizeof(DWORD), &dwReadBytes, NULL);

	// Set file pointer at begin of tail
	SetFilePointer(m_hTargetFile, -1*(sizeof(DWORD)+dwTailDataSize), NULL, FILE_END);

	// Read common tail
	TempFileTailer tailer;
	ReadFile(m_hTargetFile, &tailer, sizeof(tailer), &dwReadBytes, NULL);

	// Check our flag
	if (_tcscmp(tailer.tcTailFlg, HTTP_DOWNLOADING_FLAG) != 0)
	{
		HTTPLOG(m_pLogFile, "ERROR --> ParseTargetFile(): Target file size is unsupported type!"); 
		return HTTP_E_NOERROR;
	}
	
	// Read segment information for all threads
	m_lstSegments.clear();
	for (DWORD dwIndex = 0; dwIndex < tailer.stuTaskInfo.dwThreadCnt; dwIndex++)
	{
		DWORD dwSegmentCnt = 0;
		ReadFile(m_hTargetFile, &dwSegmentCnt, sizeof(DWORD), &dwReadBytes, NULL);
		for (DWORD dwSegCnt = 0; dwSegCnt < dwSegmentCnt; dwSegCnt++)
		{
			DataSegmentInfo segment;
			ReadFile(m_hTargetFile, &segment, sizeof(DataSegmentInfo), &dwReadBytes, NULL);
			m_lstSegments.push_back(segment);
			//
			char csSegmentLogText[128] = {0};
			sprintf(csSegmentLogText, "Undownloaded data segment: Thread Index:%d Segment Index:%d, Start Pos:%d Lenght:%d Finished:%d",
					dwIndex, dwSegCnt, segment.dwStartPos, segment.dwDataLength, segment.dwFinished);
			HTTPLOG1(m_pLogFile, "DONE --> ParseTargetFile(): %s", csSegmentLogText); 
		}
	}

	// Add the default segment if no segments in project file
	if (m_lstSegments.size() == 0)
	{
		m_lstSegments.push_back(segDefault);
		//
		char csSegmentLogText[128] = {0};
		sprintf(csSegmentLogText, "No any data has been downloaded, use the total data range: Start Pos:%d Lenght:%d Finished:%d",
				segDefault.dwStartPos, segDefault.dwDataLength, segDefault.dwFinished);
		HTTPLOG1(m_pLogFile, "DONE --> ParseTargetFile(): %s", csSegmentLogText); 
	}

	// Get the finished bytes size
	m_dwLastDownloadSize = tailer.dwFinishedSize;
	for (SegmentInfoList::iterator it = m_lstSegments.begin();
		 it != m_lstSegments.end();
		 it++)
	{
		m_dwLastDownloadSize += (*it).dwFinished;
	}

	HTTPLOG1(m_pLogFile, "DONE --> ParseTargetFile(): Target file parsed successfully, total finished size is:%d Bytes", m_dwLastDownloadSize); 

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:GetSegmentInfoForThread
//	Introduce:Get segment information data for thread object
//--------------------------------------------
LONG CHttpDownloadTask::GetSegmentInfoForThread(DWORD dwThreadIndex, 
												LPDATASEGMENTINFO lpSegments)
{
	DWORD dwGotSegmentCnt = 0;
	DWORD dwTotalSegmentCnt = (DWORD)m_lstSegments.size();
	DWORD dwSegmentNum = dwTotalSegmentCnt / m_stuDownloadInfo.dwThreadCnt;

	DWORD dwIndex = 0;
	for (SegmentInfoList::iterator it = m_lstSegments.begin();
		 it != m_lstSegments.end();
		 it++, dwIndex++)
	{
		DataSegmentInfo segInfo = *it;
		DWORD dwUnfinishDataSize = (*it).dwDataLength - (*it).dwFinished;
		DWORD dwUnfinishDataStartPos = (*it).dwStartPos + (*it).dwFinished;

		if (dwIndex < dwSegmentNum*m_stuDownloadInfo.dwThreadCnt)
		{
			if ((dwIndex%m_stuDownloadInfo.dwThreadCnt) == dwThreadIndex)
			{
				lpSegments[dwGotSegmentCnt].dwStartPos = dwUnfinishDataStartPos;
				lpSegments[dwGotSegmentCnt].dwDataLength = dwUnfinishDataSize;
				lpSegments[dwGotSegmentCnt].dwFinished = 0;
				dwGotSegmentCnt++;
			}
		}
		else
		{
			DWORD dwBytesPerThread = dwUnfinishDataSize / m_stuDownloadInfo.dwThreadCnt;
			lpSegments[dwGotSegmentCnt].dwStartPos = dwUnfinishDataStartPos + dwThreadIndex*dwBytesPerThread;
			lpSegments[dwGotSegmentCnt].dwDataLength = dwBytesPerThread;
			lpSegments[dwGotSegmentCnt].dwFinished = 0;
			if (dwThreadIndex == m_stuDownloadInfo.dwThreadCnt-1)
				lpSegments[dwGotSegmentCnt].dwDataLength = dwUnfinishDataSize - (m_stuDownloadInfo.dwThreadCnt-1)*dwBytesPerThread;
			dwGotSegmentCnt++;
		}
	}

	return dwGotSegmentCnt;
}
//--------------------------------------------
//	Name:WriteCommonTailInfo
//	Introduce:Write the tail data structure at the end of target file
//--------------------------------------------
LONG CHttpDownloadTask::WriteCommonTailInfo()
{
	TempFileTailer tailer;
	memset(&tailer, 0, sizeof(TempFileTailer));

	_tcscpy(tailer.tcTailFlg, HTTP_DOWNLOADING_FLAG);
	tailer.dwNetFileSize = m_dwNetFileSize;
	tailer.dwFinishedSize = m_dwLastDownloadSize;
	memcpy(&tailer.stuTaskInfo, &m_stuDownloadInfo, sizeof(TaskInfo));

	DWORD dwRes = SetFilePointer(	m_hTargetFile, 
									m_dwNetFileSize, 
									NULL, 
									FILE_BEGIN);
	if (0xFFFFFFFF == dwRes)
		return HTTP_E_SETFILEPOINTER_ERROR;
    
	DWORD dwWrittenBytes = 0;
	WriteFile(m_hTargetFile, &tailer, sizeof(TempFileTailer), &dwWrittenBytes, NULL);	

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:WriteInitSegmentsInfo
//	Introduce:Write initliaze segments information for all
//			  download threads
//--------------------------------------------
LONG CHttpDownloadTask::WriteInitSegmentsInfo()
{
	DWORD dwRes = SetFilePointer(	m_hTargetFile, 
									m_dwNetFileSize + sizeof(TempFileTailer), 
									NULL, 
									FILE_BEGIN);
	if (0xFFFFFFFF == dwRes)
		return HTTP_E_SETFILEPOINTER_ERROR;

	DWORD dwWrittenBytes = 0;
	DWORD dwSegmentCnt = 0;
	for (DWORD dwIndex = 0; dwIndex < m_stuDownloadInfo.dwThreadCnt; dwIndex++)
	{
		dwSegmentCnt = m_pDownloadThreads[dwIndex].GetDataSegmentCnt();
		WriteFile(m_hTargetFile, &dwSegmentCnt, sizeof(DWORD), &dwWrittenBytes, NULL);	

		if (dwSegmentCnt > 0)
		{
			LPDATASEGMENTINFO lpSegments = new DataSegmentInfo[dwSegmentCnt];
			memset(lpSegments, 0, dwSegmentCnt*sizeof(DataSegmentInfo));
			m_pDownloadThreads[dwIndex].GetDataSegments(lpSegments);
			for (DWORD dwSeg = 0; dwSeg < dwSegmentCnt; dwSeg++)
			{
				WriteFile(m_hTargetFile, &lpSegments[dwSeg], sizeof(DataSegmentInfo), &dwWrittenBytes, NULL);	
			}
			delete []lpSegments;
			lpSegments = NULL;
		}
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:WriteTotalTailSize
//	Introduce:Write total tail data size at the end of target file
//--------------------------------------------
LONG CHttpDownloadTask::WriteTotalTailSize()
{
	if (m_stuDownloadInfo.dwThreadCnt <= 0 ||
		NULL == m_pDownloadThreads)
		return HTTP_E_CREATTHREAD_FAIL;

	DWORD dwTotalTailDataSize = sizeof(TempFileTailer);
	for (DWORD dwIndex = 0; dwIndex < m_stuDownloadInfo.dwThreadCnt; dwIndex++)
	{
		DWORD dwSegmentCnt = m_pDownloadThreads[dwIndex].GetDataSegmentCnt();
		dwTotalTailDataSize += (sizeof(DWORD) + dwSegmentCnt * sizeof(DataSegmentInfo));
	}

	DWORD dwRes = SetFilePointer(	m_hTargetFile, 
									m_dwNetFileSize + dwTotalTailDataSize, 
									NULL, 
									FILE_BEGIN);
	if (0xFFFFFFFF == dwRes)
		return HTTP_E_SETFILEPOINTER_ERROR;
    
	DWORD dwWrittenBytes = 0;
	WriteFile(m_hTargetFile, &dwTotalTailDataSize, sizeof(DWORD), &dwWrittenBytes, NULL);			

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:CloseTargetFile
//	Introduce:Return TRUE if the target file closed successfully.
//			  Otherwise return an error code.
//--------------------------------------------
LONG CHttpDownloadTask::CloseTargetFile()
{
	BOOL bClosed = FALSE;
	if (NULL == m_hTargetFile)
		return TRUE;

	bClosed = CloseHandle(m_hTargetFile);
	m_hTargetFile = NULL;

	HTTPLOG(m_pLogFile, "DONE --> CloseTargetFile(): Close target file sucessfully!"); 

	return bClosed ? HTTP_E_NOERROR : HTTP_E_CLOSETARGET_ERROR;
}
//--------------------------------------------
//	Name:CompletedTargetFile
//	Introduce:Copy downloaded data from temp file to target file,
//			  when task is completed
//--------------------------------------------
LONG CHttpDownloadTask::CompletedTargetFile()
{	
	// Downloading project file
	TCHAR tcDownloadingFile[MAX_PATH] = {_T("\0")};
	_tcscpy(tcDownloadingFile, m_stuDownloadInfo.tcTargetFile);
	_tcscat(tcDownloadingFile, HTTP_DOWNLOADING_EXT);

	// Truncate the tail information
	SetFilePointer(m_hTargetFile, m_dwNetFileSize, NULL, FILE_BEGIN);
	SetEndOfFile(m_hTargetFile);

	// Close it
	CloseTargetFile();
/*
	// If don't do any downloading, delete the project file dircetly
	if (TRUE == m_bDontNeedDownload)
	{
		DeleteFile(tcDownloadingFile);
		SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_ProgressChanged, m_dwNetFileSize, 0);
		return HTTP_E_NOERROR;
	}
*/
	// Remove the HTTP_DOWNLOADING_EXT extion of target file name
	char cNowName[MAX_PATH] = {_T("\0")};
	char cNewName[MAX_PATH] = {_T("\0")};
#ifdef	UNICODE
	WideCharToMultiByte(CP_ACP, tcDownloadingFile, -1, cNowName, MAX_PATH, NULL, FALSE);
	WideCharToMultiByte(CP_ACP, m_stuDownloadInfo.tcTargetFile, -1, cNewName, MAX_PATH, NULL, FALSE);
#else
	strcpy(cNowName, tcDownloadingFile);
	strcpy(cNewName, m_stuDownloadInfo.tcTargetFile);
#endif	//UNICODE
	rename(cNowName, cNewName);

	HTTPLOG1(m_pLogFile, "DONE --> CompletedTargetFile(): File %s has been downloaded sucessfully!", cNewName); 

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:CreateDownloadThreads
//	Introduce:Create all download threads
//--------------------------------------------
LONG CHttpDownloadTask::CreateDownloadThreads()
{
	DeleteDownloadThreads();

	if (m_stuDownloadInfo.dwThreadCnt <= 0)
		return HTTP_E_THREADCNT_ERROR;

	if (m_dwNetFileSize <= 0)
		return HTTP_E_NETFILESIZE_ERROR;

	// Use singe thread downloading
	if (!CanUseMultiThread())
	{
		HTTPLOG(m_pLogFile, "DONE --> CreateDownloadThreads(): Use single thread for downloading!"); 

		DataSegmentInfo segment;
		memset(&segment, 0, sizeof(segment));
		segment.dwDataLength = m_dwNetFileSize;
		//
		ThreadInfo info;
		memset(&info, 0, sizeof(ThreadInfo));
		_tcscpy(info.tcHost, m_stuDownloadInfo.tcHostName);
		_tcscpy(info.tcNetFile, m_stuDownloadInfo.tcNetFile);
		info.dwTailInfoOffset = m_dwNetFileSize + sizeof(TempFileTailer);;
		//
		m_stuDownloadInfo.dwThreadCnt = 1;
		m_pDownloadThreads = new CHttpDownloadThreadSingle[m_stuDownloadInfo.dwThreadCnt];		
		m_pDownloadThreads[0].SetThreadID(1);
		m_pDownloadThreads[0].SetDataSegments(1, &segment);
		m_pDownloadThreads[0].SetDownloadInfo(m_hConnect, m_hTargetFile, m_pLogFile, &info);
		m_pDownloadThreads[0].SetCriticalSection(&m_csTargetWriting);
		//
		char csSegmentInfo[128] = {0};
		sprintf(csSegmentInfo, "Segment Index:%d Start Pos:%d, Length:%d", 0, segment.dwStartPos, segment.dwDataLength);
		HTTPLOG2(m_pLogFile, "DONE --> CreateDownloadThreads(): Thread ID:%d, Segment counts:%d", 1, 1); 
		HTTPLOG1(m_pLogFile, "                               %s", csSegmentInfo); 
	}
	// Use multi-threads downloading
	else
	{
		HTTPLOG(m_pLogFile, "DONE --> CreateDownloadThreads(): Use multi-threads for downloading!"); 

		DWORD dwTotalSegmentCnt = (DWORD)m_lstSegments.size();
		DWORD dwSegmentCntPerThread = dwTotalSegmentCnt/m_stuDownloadInfo.dwThreadCnt + dwTotalSegmentCnt%m_stuDownloadInfo.dwThreadCnt;
		LPDATASEGMENTINFO lpSegment = new DataSegmentInfo[dwSegmentCntPerThread];
		HTTPLOG2(m_pLogFile, "DONE --> CreateDownloadThreads(): Total segment counts:%d, Segment counts of a thread:%d", dwTotalSegmentCnt, dwSegmentCntPerThread); 

		DWORD dwCurrentStartPos = 0;
		DWORD dwCurrentTailInfoPos = m_dwNetFileSize + sizeof(TempFileTailer);
		m_pDownloadThreads = new CHttpDownloadThread[m_stuDownloadInfo.dwThreadCnt];
		for (DWORD dwIndex = 0; dwIndex < m_stuDownloadInfo.dwThreadCnt; dwIndex++)
		{
			ThreadInfo info;
			memset(&info, 0, sizeof(ThreadInfo));
			_tcscpy(info.tcHost, m_stuDownloadInfo.tcHostName);
			_tcscpy(info.tcNetFile, m_stuDownloadInfo.tcNetFile);
			info.dwTailInfoOffset = dwCurrentTailInfoPos;

			memset(lpSegment, 0, dwSegmentCntPerThread*sizeof(DataSegmentInfo));
			DWORD dwRealSegmentCnt = GetSegmentInfoForThread(dwIndex, lpSegment);

			m_pDownloadThreads[dwIndex].SetThreadID(dwIndex+1);
			m_pDownloadThreads[dwIndex].SetDataSegments(dwRealSegmentCnt, lpSegment);
			m_pDownloadThreads[dwIndex].SetDownloadInfo(m_hConnect, m_hTargetFile, m_pLogFile, &info);
			m_pDownloadThreads[dwIndex].SetCriticalSection(&m_csTargetWriting);

			dwCurrentTailInfoPos += (sizeof(DWORD) + dwRealSegmentCnt*sizeof(DataSegmentInfo));

			HTTPLOG2(m_pLogFile, "DONE --> CreateDownloadThreads(): Thread ID:%d, Segment counts:%d", dwIndex+1, dwRealSegmentCnt); 
			for (DWORD dwSegment = 0; dwSegment < dwRealSegmentCnt; dwSegment++)
			{
				char csSegmentInfo[128] = {0};
				sprintf(csSegmentInfo, "Segment Index:%d Start Pos:%d, Length:%d", dwSegment, lpSegment[dwSegment].dwStartPos, lpSegment[dwSegment].dwDataLength);
				HTTPLOG1(m_pLogFile, "                               %s", csSegmentInfo); 
			}
		}

		delete []lpSegment;
		lpSegment = NULL;
	}

	HTTPLOG(m_pLogFile, "DONE --> CreateDownloadThreads(): Create download threads successfully!"); 

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:DeleteDownloadThreads
//	Introduce:Delete all download threads
//--------------------------------------------
LONG CHttpDownloadTask::DeleteDownloadThreads()
{
	if (NULL != m_pDownloadThreads)
	{
		Stop();

		delete []m_pDownloadThreads;
		m_pDownloadThreads = NULL;
		HTTPLOG(m_pLogFile, "DONE --> DeleteDownloadThreads(): Download threads have been deleted!"); 
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:CreateTaskThread
//	Introduce:Create task working threads
//--------------------------------------------
LONG CHttpDownloadTask::CreateTaskThread()
{
	DeleteTaskThread();

	m_hProcessEvent = CreateEvent(	NULL,
									FALSE,
									FALSE,
									_T("ProgressEvent"));
	m_hProcessThread = CreateThread(NULL,
									0,
									WorkingThreadFunc,
									(LPVOID)this,
									CREATE_SUSPENDED,
									&m_dwProcessThreadID);  
	if (NULL == m_hProcessEvent || NULL == m_hProcessThread)
	{		
		HTTPLOG2(m_pLogFile, "ERROR --> CreateTaskThread(): Create thread or event object failed! m_hProcessEvent = %d, m_hProcessThread = %d", (LONG)m_hProcessEvent, (LONG)m_hProcessThread); 
		return HTTP_E_CREATTHREAD_FAIL;
	}

	m_bRequireExit = FALSE;
	m_bIsCompeleted = FALSE;
	m_eCurStatus = eHttpTaskStatus_Ready;
	ResumeThread(m_hProcessThread);
	WaitFor(m_hProcessEvent);
	HTTPLOG(m_pLogFile, "DONE --> CreateTaskThread(): Create task working thread successfully!"); 

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:DeleteTaskThread
//	Introduce:Delete task working threads
//--------------------------------------------
LONG CHttpDownloadTask::DeleteTaskThread()
{
	if (NULL != m_hProcessEvent)
	{
		// Stop at first
		Stop();

		// Waiting for task thread exited
		m_bRequireExit = TRUE;
		WaitFor(m_hProcessThread);

		// Release handles
		CloseHandle(m_hProcessEvent);	
		CloseHandle(m_hProcessThread);	
		m_hProcessEvent = NULL;
		m_hProcessThread = NULL;
		HTTPLOG(m_pLogFile, "DONE --> DeleteTaskThread(): Task working thread has been deleted!"); 
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:TargetFileIsDownloaded
//	Introduce:To check the file is downloaded or not
//--------------------------------------------
BOOL CHttpDownloadTask::TargetFileIsDownloaded()
{
	HANDLE hFile = CreateFile( m_stuDownloadInfo.tcTargetFile,
								GENERIC_READ,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return FALSE;

	BOOL bRes = FALSE;

	DWORD dwFileSize = 0;	
	dwFileSize = SetFilePointer(hFile, 0, NULL, FILE_END);
	if (dwFileSize == m_dwNetFileSize)
		bRes = TRUE;

	CloseHandle(hFile);

	return bRes;
}
//--------------------------------------------
//	Name:CanDoContinueDownload
//	Introduce:To check a file is WebMax downloading project file or not
//--------------------------------------------
BOOL CHttpDownloadTask::CanDoContinueDownload(LPCTSTR lpszFileName)
{
	if (NULL == lpszFileName ||
		_tcslen(lpszFileName) == 0)
		return FALSE;

	HANDLE hFile = CreateFile( lpszFileName,
								GENERIC_READ,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return FALSE;

	// Check file size at first
	DWORD dwFileSize = 0;	
	dwFileSize = SetFilePointer(hFile, 0, NULL, FILE_END);
	if (dwFileSize <= sizeof(TempFileTailer) + sizeof(DWORD))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwReadBytes = 0;

	// Get tail data size
	DWORD dwTailDataSize = 0;
	SetFilePointer(hFile, -1*(int)sizeof(DWORD), NULL, FILE_END);
	ReadFile(hFile, &dwTailDataSize, sizeof(DWORD), &dwReadBytes, NULL);

	// Set file pointer at begin of tail
	SetFilePointer(hFile, -1*(sizeof(DWORD)+dwTailDataSize), NULL, FILE_END);

	// Read common tail
	TempFileTailer tailer;
	ReadFile(hFile, &tailer, sizeof(tailer), &dwReadBytes, NULL);

	CloseHandle(hFile);

	// Check our flag
	if (_tcscmp(tailer.tcTailFlg, HTTP_DOWNLOADING_FLAG) != 0)
		return FALSE;

	// Check URL strings
	if (_tcsicmp(tailer.stuTaskInfo.tcHostName, m_stuDownloadInfo.tcHostName) != 0 ||
		_tcsicmp(tailer.stuTaskInfo.tcNetFile, m_stuDownloadInfo.tcNetFile) != 0)
		return FALSE;

	return TRUE;
}
//--------------------------------------------
//	Name:SendEventToParent
//	Introduce:Send event information to parent, 
//			  by calling callback function
//--------------------------------------------
LONG CHttpDownloadTask::SendEventToParent(LONG lTaskID, 
										  LONG lThreadID,
										  DWORD dwEvent, 
										  DWORD dwParam1, 
										  DWORD dwParam2)
{
	if (NULL != m_pCallbackFunc)
		return ((HttpDownload_CallbackFunc)m_pCallbackFunc)(lTaskID, lThreadID, dwEvent, dwParam1, dwParam2, m_pUserData);

	return 0;
}
//--------------------------------------------



//--------------------------------------------
//	Name:OnStart
//	Introduce:Try to start all downloading threads
//--------------------------------------------
LONG CHttpDownloadTask::OnStart()
{
	if (0 == m_stuDownloadInfo.dwThreadCnt ||
		NULL == m_pDownloadThreads)
		return HTTP_E_CREATTHREAD_FAIL;

	LONG lRes = HTTP_E_NOERROR;
	for (DWORD dwIndex = 0; dwIndex < m_stuDownloadInfo.dwThreadCnt; dwIndex++)
	{
		GIF (m_pDownloadThreads[dwIndex].Start())
	}

	m_dwZeroBytesTime = 0;
	m_eCurStatus = eHttpTaskStatus_Running;
	SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_StatusChanged, m_eCurStatus, 0);
	HTTPLOG(m_pLogFile, "DONE --> OnStart(): Task has started!"); 

	return lRes;
}
//--------------------------------------------
//	Name:OnStop
//	Introduce:Try to stop all downloading threads
//--------------------------------------------
LONG CHttpDownloadTask::OnStop()
{
	if (0 == m_stuDownloadInfo.dwThreadCnt ||
		NULL == m_pDownloadThreads)
		return HTTP_E_CREATTHREAD_FAIL;

	LONG lRes = HTTP_E_NOERROR;
	for (DWORD dwIndex = 0; dwIndex < m_stuDownloadInfo.dwThreadCnt; dwIndex++)
	{
		GIF (m_pDownloadThreads[dwIndex].Stop())
	}

/*  Don't send this event in this case, because it will cause long time for applicaiton exist!!!
	// Update the end progress to application
	m_dwCurrentSpeed = 0;
	DWORD dwFinishedBytes = 0;
	for (DWORD dwIndex = 0; dwIndex < m_stuDownloadInfo.dwThreadCnt; dwIndex++)
	{
		dwFinishedBytes += m_pDownloadThreads[dwIndex].GetFinishedBytes();
	}
	SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_ProgressChanged, m_dwLastDownloadSize+dwFinishedBytes, m_dwCurrentSpeed);
*/
	m_eCurStatus = eHttpTaskStatus_Stopped;
	SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Stopped, 0);
	HTTPLOG(m_pLogFile, "DONE --> OnStop(): Task has stopped!"); 

	return lRes;
}
//--------------------------------------------
//	Name:OnCompleted
//	Introduce:This download task is completed
//--------------------------------------------
LONG CHttpDownloadTask::OnCompleted()
{
	CompletedTargetFile();
	DisconnectInternet();
	m_dwCurrentSpeed = 0;
	m_bIsCompeleted = TRUE;
	m_eCurStatus = eHttpTaskStatus_Completed;
	SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Completed, 0);
	HTTPLOG(m_pLogFile, "DONE --> OnCompleted(): Task has completed!"); 

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:OnRestart
//	Introduce:Try to restart all downloading threads
//--------------------------------------------
LONG CHttpDownloadTask::OnRestart()
{
	HTTPLOG(m_pLogFile, "DONE --> OnRestart(): Task is tring to restart!"); 

	OnStop();
	CloseTargetFile();
	DeleteDownloadThreads();

	LONG lRes = HTTP_E_NOERROR;

	// Try to open the target file with NULL data
	GIF (OpenTargetFile(m_dwNetFileSize, FALSE))

	// Try to parse the target file, for continue downloading
	GIF (ParseTargetFile())

	// Try to add the tail information at the end of file
	GIF (WriteCommonTailInfo())

	// Create downloading threads
	GIF (CreateDownloadThreads())

	// Write download threads segments for begin
	GIF (WriteInitSegmentsInfo())

	// Write total data size at end of target file
	GIF (WriteTotalTailSize())

	// Try to start again
	GIF (OnStart())

	HTTPLOG(m_pLogFile, "DONE --> OnRestart(): Task restart successfully!"); 

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:OnProgress
//	Introduce:Update downloading progress
//--------------------------------------------
LONG CHttpDownloadTask::OnProgress()
{
	if (0 == m_stuDownloadInfo.dwThreadCnt ||
		NULL == m_pDownloadThreads)
		return HTTP_E_CREATTHREAD_FAIL;

	DWORD dwCompeletedCnt = 0;
	DWORD dwFinishedBytes = 0;
	for (DWORD dwIndex = 0; dwIndex < m_stuDownloadInfo.dwThreadCnt; dwIndex++)
	{
		dwFinishedBytes += m_pDownloadThreads[dwIndex].GetFinishedBytes();
		if (m_pDownloadThreads[dwIndex].IsCompeleted())
			dwCompeletedCnt++;
	}

	DWORD dwCurrentTime = GetTickCount();
	DWORD dwDeltaTime = dwCurrentTime - m_dwLastTime;
	DWORD dwDeltaBytes = dwFinishedBytes - m_dwCurFinishedBytes;
	m_dwCurrentSpeed = 1000 * dwDeltaBytes / dwDeltaTime;
	SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_ProgressChanged, m_dwLastDownloadSize+dwFinishedBytes, m_dwCurrentSpeed);

	m_dwLastTime = dwCurrentTime;
	m_dwCurFinishedBytes = dwFinishedBytes;
	HTTPLOG1(m_pLogFile, "DONE --> OnProgress(): Total downloaded size: %d Bytes",  m_dwLastDownloadSize+dwFinishedBytes); 

	if (dwCompeletedCnt == m_stuDownloadInfo.dwThreadCnt)
	{
		m_eCurStatus = eHttpTaskStatus_Completed;
		PostThreadMessage(m_dwProcessThreadID, PROC_MSG_STOP, -1, 1);
		HTTPLOG(m_pLogFile, "DONE --> OnProgress(): All threads have finished, post PROC_MSG_STOP message for end!"); 
	}

	// To check how long time of speed is 0
	if (0 == m_dwCurrentSpeed)
		m_dwZeroBytesTime += 1;
	else 
		m_dwZeroBytesTime = 0;

	// If speed has been 0 for 20 seconds, try to auto-restart 
	if (m_dwZeroBytesTime >= 20)
	{
		PostThreadMessage(m_dwProcessThreadID, PROC_MSG_RESTART, -1, 0);
		HTTPLOG(m_pLogFile, "ERROR --> OnProgress(): Have not download any data for 20 seconds, try to retart task!"); 
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
