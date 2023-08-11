//--------------------------------------------
//	File Name:HttpDownloadThread.cpp
//	Introduce:The implement file of class CHttpDownloadThread
//	Date:2010-05-10
//--------------------------------------------
#include "HttpDownloadThread.h"
//--------------------------------------------
//	Name:DownloadThreadFunc
//	Introduce:Public download thread function
//--------------------------------------------
DWORD WINAPI DownloadThreadFunc(LPVOID lpParam) 
{ 
	CHttpDownloadThread* pDownloadThread = (CHttpDownloadThread*)lpParam;
	if (NULL != pDownloadThread)
		return pDownloadThread->DownloadingFunc();

    return 0; 
} 
//--------------------------------------------


//--------------------------------------------
//	Name:CHttpDownloadThread
//	Introduce:Construction
//--------------------------------------------
CHttpDownloadThread::CHttpDownloadThread(void)
 : m_dwID(0)
 , m_hConnect(NULL)
 , m_hNetFile(NULL)
 , m_hTargetFile(NULL)
 , m_dwSegmentsCnt(0)
 , m_lpSegments(NULL)
 , m_lpCriticalSection(NULL)
 , m_hThread(NULL)
 , m_bCompeleted(FALSE)
 , m_bRequireExit(FALSE)
{
	for (int i = 0; i < 3; i++)
		m_hEvent[0] = NULL;
	memset(&m_stuDownInfo, 0, sizeof(ThreadInfo));
}
//--------------------------------------------
//	Name:~CHttpDownloadThread
//	Introduce:Distruction
//--------------------------------------------
CHttpDownloadThread::~CHttpDownloadThread(void)
{
	Stop();
	CloseNetFile();

	if (NULL != m_lpSegments)
	{
		delete []m_lpSegments;
		m_lpSegments = NULL;
	}
}
//--------------------------------------------


//--------------------------------------------
//	Name:SetThreadID
//	Introduce:Set download thread ID
//--------------------------------------------
void CHttpDownloadThread::SetThreadID(DWORD dwID)
{
	m_dwID = dwID;
}
//--------------------------------------------
//	Name:GetThreadID
//	Introduce:Get download thread ID
//--------------------------------------------
DWORD CHttpDownloadThread::GetThreadID()
{
	return m_dwID;
}
//--------------------------------------------
//	Name:SetDownloadInfo
//	Introduce:Set download information for this thread
//--------------------------------------------
LONG CHttpDownloadThread::SetDownloadInfo(HINTERNET hConnect, 
										  HANDLE hTargetFile,
										  LPTHREADINFO lpInfo)
{
	m_hConnect = hConnect;
	m_hTargetFile = hTargetFile;

	if (NULL != lpInfo)
	{
		memcpy(&m_stuDownInfo, lpInfo, sizeof(ThreadInfo));
	}
/*
	LONG lRes = HTTP_E_NOERROR;

	// Open a request at first
	GIF (OpenNetFile())
*/
	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:GetDownloadInfo
//	Introduce:Return current downloading information of this thread
//--------------------------------------------
void CHttpDownloadThread::GetDownloadInfo(LPTHREADINFO lpInfo)
{
	if (NULL != lpInfo)
	{
		memcpy(lpInfo, &m_stuDownInfo, sizeof(ThreadInfo));
	}
}
//--------------------------------------------
//	Name:SetDataSegments
//	Introduce:Set data segments information for this thread
//--------------------------------------------
void CHttpDownloadThread::SetDataSegments(DWORD dwSegmentCnt, 
										  LPDATASEGMENTINFO lpDataSegments)
{
	if (dwSegmentCnt > 0 &&
		NULL != lpDataSegments)
	{
		if (NULL != m_lpSegments)
			delete []m_lpSegments;

		m_dwSegmentsCnt = dwSegmentCnt;
		m_lpSegments = new DataSegmentInfo[m_dwSegmentsCnt];
		memcpy(m_lpSegments, lpDataSegments, m_dwSegmentsCnt*sizeof(DataSegmentInfo));
	}
}
//--------------------------------------------
//	Name:GetDataSegmentCnt
//	Introduce:Return current data segments count
//--------------------------------------------
DWORD CHttpDownloadThread::GetDataSegmentCnt()
{
	return m_dwSegmentsCnt;
}
//--------------------------------------------
//	Name:GetDataSegments
//	Introduce:Return current thread's data segments information
//--------------------------------------------
void CHttpDownloadThread::GetDataSegments(LPDATASEGMENTINFO lpDataSegments)
{
	if (m_dwSegmentsCnt > 0 &&
		NULL != m_lpSegments &&
		NULL != lpDataSegments)
	{
		memcpy(lpDataSegments, m_lpSegments, m_dwSegmentsCnt*sizeof(DataSegmentInfo));
	}
}
//--------------------------------------------
//	Name:SetCriticalSection
//	Introduce:Set target file writing critical section handle
//--------------------------------------------
void CHttpDownloadThread::SetCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	if (NULL != lpCriticalSection)
	{
		m_lpCriticalSection = lpCriticalSection;
	}
}
//--------------------------------------------
//	Name:GetFinishedBytes
//	Introduce:Return how bytes have been downloaded
//--------------------------------------------
DWORD CHttpDownloadThread::GetFinishedBytes()
{
	return m_stuDownInfo.dwFinishedLenght;
}
//--------------------------------------------
//	Name:Start
//	Introduce:Start this downloading thread
//--------------------------------------------
LONG CHttpDownloadThread::Start()
{
	LONG lRes = HTTP_E_NOERROR;

	// Make sure the old instance is stopped
	Stop();
	m_bRequireExit = FALSE;

	// Create the thread object
	m_hThread = CreateThread(	NULL,
								0,
								DownloadThreadFunc,
								(LPVOID)this,
								0,
								NULL);
	if (NULL == m_hThread)
		return HTTP_E_CREATETHREAD_ERROR;	

	m_hEvent[0] = CreateEvent(NULL, FALSE, FALSE, _T("Async00"));
	m_hEvent[1] = CreateEvent(NULL, FALSE, FALSE, _T("Async01"));
	m_hEvent[2] = CreateEvent(NULL, FALSE, FALSE, _T("Async02"));
	if (NULL == m_hEvent[0] || NULL == m_hEvent[1] || NULL == m_hEvent[2])
		return HTTP_E_CREATETHREAD_ERROR;	

	return lRes;
}
//--------------------------------------------
//	Name:Stop
//	Introduce:Stop and delete the thread object
//--------------------------------------------
LONG CHttpDownloadThread::Stop()
{
	if (NULL != m_hThread)
	{
		m_bRequireExit = TRUE;
		::TerminateThread(m_hThread, -1);

		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:IsStop
//	Introduce:Return TRUE if this thread is stopped
//--------------------------------------------
BOOL CHttpDownloadThread::IsStop()
{
	BOOL bIsStopped = (NULL == m_hThread) ? TRUE : FALSE;

	return bIsStopped;
}
//--------------------------------------------
//	Name:IsCompeleted
//	Introduce:Return TRUE if this thread is finished
//--------------------------------------------
BOOL CHttpDownloadThread::IsCompeleted()
{
	return m_bCompeleted;
}
//--------------------------------------------


//--------------------------------------------
//	Name:DownloadingFunc
//	Introduce:Downloading function for this thread
//--------------------------------------------
DWORD CHttpDownloadThread::DownloadingFunc()
{
	LONG lDownloadSize = 0;
	LONG lWrittenSize = 0;
	BYTE btBuffer[HTTP_MAX_BUFF_LEN] = {0};

	// Open the net file
	LONG lRes = HTTP_E_NOERROR;
	GIF (OpenNetFile());

	DWORD dwCurSegmentIndex = 0;
	while (	FALSE == m_bCompeleted &&
			FALSE == m_bRequireExit)
	{
		dwCurSegmentIndex = m_stuDownInfo.dwCurSegmentIndex;
/*
		// Check and trim header at first for each segment
		if (0 == m_lpSegments[dwCurSegmentIndex].dwFinished)
		{
			lDownloadSize = CheckAndTrimHeader(btBuffer, HTTP_MAX_BUFF_LEN);
			
			// Error, can't download file
			if (lDownloadSize <= 0)
			{
				lRes = lDownloadSize;
				break;
			}
		}
		else*/
		// Download data and write to file
		{
			lDownloadSize = DownloadData(btBuffer, HTTP_MAX_BUFF_LEN);
		}
		if (lDownloadSize > 0)
		{
			lWrittenSize = WriteDataToFile(btBuffer, lDownloadSize);
            
			m_lpSegments[dwCurSegmentIndex].dwFinished += lWrittenSize;
			m_stuDownInfo.dwFinishedLenght += lWrittenSize;	
			
			WriteTailInfoToFile();
		}

		// Current segment has been finished
		if (m_lpSegments[dwCurSegmentIndex].dwFinished >=
			m_lpSegments[dwCurSegmentIndex].dwDataLength)
		{
			// Download the next segment
			m_stuDownInfo.dwCurSegmentIndex++;

			// All segment have been downloaded, compeleted!
			if (m_stuDownInfo.dwCurSegmentIndex >= m_dwSegmentsCnt)
			{
				CloseNetFile();
				m_bCompeleted = TRUE;
			}
			// Reopen net file and send the new request to it
			else
			{
				CloseNetFile();

				OpenNetFile();
			}
		}

		//Sleep(10);
	}

	return 0;
}
//--------------------------------------------



//--------------------------------------------
//	Name:OpenNetFile
//	Introduce:Open the net file for downloading
//--------------------------------------------
LONG CHttpDownloadThread::OpenNetFile()
{
	LONG lRes = HTTP_E_NOERROR;

	// Close the old request
	GIF(CloseNetFile());

	InternetSetStatusCallback(m_hConnect, CHttpDownloadThread::AsyncInternetCallback);

	TCHAR tcRange[128] = {0};
	DWORD dwIndex = m_stuDownInfo.dwCurSegmentIndex;
	_stprintf(tcRange, _T("Range: bytes=%d-%d"), m_lpSegments[dwIndex].dwStartPos + m_lpSegments[dwIndex].dwFinished, m_lpSegments[dwIndex].dwStartPos + m_lpSegments[dwIndex].dwDataLength);

	TCHAR tcURL[512] = {0};
	_tcscpy(tcURL, _T("http://"));
	_tcscat(tcURL, m_stuDownInfo.tcHost);
	_tcscat(tcURL, m_stuDownInfo.tcNetFile);
	m_hNetFile = InternetOpenUrl(m_hConnect, tcURL, tcRange, _tcslen(tcRange), INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD, (DWORD)this);
	while (NULL == m_hNetFile)
	{
		DWORD dwError = ::GetLastError();
		if (ERROR_IO_PENDING == dwError || ERROR_SUCCESS == dwError)
		{
			if (WaitExitEvent()) 
				break;
		}
		else 
			return HTTP_E_OPENNETFILE_ERROR;
	}


	//try
	//{		
	//	DWORD dwRes = 0;

	//	// Try to open
	//	const TCHAR* acceptTYPE[] = {_T("*/*"), 0}; 
	//	m_hNetFile = HttpOpenRequest(m_hConnect,
	//								 _T("GET"),
	//								 m_stuDownInfo.tcNetFile,
	//								 _T("HTTP/1.1"),
	//								 NULL,
	//								 acceptTYPE,
	//								 INTERNET_FLAG_RELOAD,
	//								 0);
	//	
	//	if (NULL == m_hNetFile)
	//		lRes = HTTP_E_OPENNETFILE_ERROR;

	//	TCHAR tcRangeHeader[256] = {_T("\0")};
	//	DWORD dwIndex = m_stuDownInfo.dwCurSegmentIndex;
	//	_stprintf(	tcRangeHeader, 
	//				_T("Range: bytes=%d-%d\r\n"), 
	//				m_lpSegments[dwIndex].dwStartPos + m_lpSegments[dwIndex].dwFinished, 
	//				m_lpSegments[dwIndex].dwStartPos + m_lpSegments[dwIndex].dwDataLength);
	//	dwRes = HttpAddRequestHeaders(	m_hNetFile, 
	//									tcRangeHeader, 
	//									(DWORD)_tcslen(tcRangeHeader), 
	//									HTTP_ADDREQ_FLAG_ADD_IF_NEW);
	//	if (0 == dwRes)
	//	{
	//		DWORD dwError = GetLastError();
	//		CloseNetFile();
	//		return HTTP_E_NOMULTI_THREAD;
	//	}

	//	if (FALSE == HttpSendRequest(m_hNetFile, tcRangeHeader, (DWORD)_tcslen(tcRangeHeader), 0, 0))
	//	{
	//		lRes = HTTP_E_OPENNETFILE_ERROR;
	//	}
	//}
	//catch(...)
	//{
	//	lRes = HTTP_E_OPENNETFILE_ERROR;
	//}

	return lRes;
}
//--------------------------------------------
//	Name:CloseNetFile
//	Introduce:Close the net file
//--------------------------------------------
LONG CHttpDownloadThread::CloseNetFile()
{	
	try
	{		
		if (NULL != m_hNetFile)
		{
			InternetCloseHandle(m_hNetFile);
			m_hNetFile = NULL;
		}
	}
	catch(...)
	{
		return HTTP_E_CLOSENETFILE_ERROR;
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:CreateDownloadThread
//	Introduce:Download dwLength data to lpBuffer
//--------------------------------------------
LONG CHttpDownloadThread::CreateDownloadThread()
{
	DeleteDownloadThread();

	// Create the thread object
	m_hThread = CreateThread(	NULL,
								0,
								DownloadThreadFunc,
								(LPVOID)this,
								CREATE_SUSPENDED,
								NULL);
	if (NULL == m_hThread)
		return HTTP_E_CREATETHREAD_ERROR;	

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:DeleteDownloadThread
//	Introduce:Download dwLength data to lpBuffer
//--------------------------------------------
LONG CHttpDownloadThread::DeleteDownloadThread()
{
	m_bRequireExit = TRUE;
	::TerminateThread(m_hThread, -1);

	CloseHandle(m_hThread);
	m_hThread = NULL;

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:CheckAndTrimHeader
//	Introduce:Check and trim SOCKET header data
//--------------------------------------------
LONG CHttpDownloadThread::CheckAndTrimHeader(LPBYTE lpBuffer, DWORD dwLength)
{
	if (NULL == lpBuffer || 0 == dwLength)
		return 0;

	DWORD dwRequestSize = 0;
	DWORD dwRetainSize = m_lpSegments[m_stuDownInfo.dwCurSegmentIndex].dwDataLength - 
						 m_lpSegments[m_stuDownInfo.dwCurSegmentIndex].dwFinished;

	// Check the request size
	dwRequestSize = (dwLength < dwRetainSize) ? dwLength : dwRetainSize;

	LONG lRes;
	char* lpTmpBuf = new char[dwRequestSize];
	lRes = recv(m_Socket, lpTmpBuf, dwRequestSize, 0); 
	if (lRes > 0)
	{
		float fHttpVsr;
		int iStatus;
		long lDataLen;
		char cTmep[1024] = {0};
		sscanf(strstr((const char*)lpTmpBuf, "HTTP/"), "HTTP/%f %d", &fHttpVsr, &iStatus);
		sscanf(strstr((const char*)lpTmpBuf, "Content-Length"), "Content-Length: %d", &lDataLen);
		if (iStatus >= 300)
			return HTTP_E_OPENNETFILE_ERROR;

		char* pStart = strstr(lpTmpBuf, "\r\n\r\n");
		DWORD dwHeaderLen = (pStart - lpTmpBuf) + sizeof("\r\n\r\n") - 1;
		DWORD dwRealDataLen = lRes - dwHeaderLen;
		memcpy(lpBuffer, lpTmpBuf+dwHeaderLen, dwRealDataLen);
		lRes = dwRealDataLen;
	}
	delete []lpTmpBuf;
	lpTmpBuf = NULL;

	return lRes;
}
//--------------------------------------------
//	Name:DownloadData
//	Introduce:Download dwLength data to lpBuffer
//--------------------------------------------
LONG CHttpDownloadThread::DownloadData(LPBYTE lpBuffer, DWORD dwLength)
{
	DWORD dwRequestSize = 0;
	DWORD dwDownloaddedSize = 0;
	DWORD dwRetainSize = m_lpSegments[m_stuDownInfo.dwCurSegmentIndex].dwDataLength - 
						 m_lpSegments[m_stuDownInfo.dwCurSegmentIndex].dwFinished;

	// Check the request size
	dwRequestSize = (dwLength < dwRetainSize) ? dwLength : dwRetainSize;
	
	INTERNET_BUFFERS InetBuf = {0};
	InetBuf.dwStructSize = sizeof(INTERNET_BUFFERS);
	InetBuf.lpvBuffer = lpBuffer;
	InetBuf.dwBufferLength = dwLength;
	try
	{
		BOOL bRes = TRUE;
		bRes = InternetReadFileEx(m_hNetFile,
								  &InetBuf,
								  IRF_ASYNC,
								  (DWORD)this);
		if (FALSE == bRes)
		{
			DWORD dwError;
			dwError = GetLastError();
			if (ERROR_IO_PENDING == dwError)
			{
				if (WaitExitEvent()) 
				{
					dwDownloaddedSize = InetBuf.dwBufferLength;
				}
			}
			else
			{
				return HTTP_E_DOWNLOAD_FAILED;
			}
		}
		else
		{
			if (WAIT_OBJECT_0 == WaitForSingleObject(m_hEvent[2], 0))
				ResetEvent(m_hEvent[2]);
			dwDownloaddedSize = InetBuf.dwBufferLength;
		}
	}
	catch(...)
	{
		return HTTP_E_DOWNLOAD_FAILED;
	}

/*
	try
	{
		BOOL bRes = TRUE;
		bRes = InternetReadFile(m_hNetFile,
								lpBuffer,
								dwRequestSize,
								&dwDownloaddedSize);
		if (FALSE == bRes)
		{
			DWORD dwError;
			dwError = GetLastError();
			return HTTP_E_DOWNLOAD_FAILED;
		}
	}
	catch(...)
	{
		return HTTP_E_DOWNLOAD_FAILED;
	}
*/
	return dwDownloaddedSize;
}
//--------------------------------------------
//	Name:WriteDataToFile
//	Introduce:Write dwLength data from lpBuffer to target file
//--------------------------------------------
LONG CHttpDownloadThread::WriteDataToFile(LPBYTE lpBuffer, DWORD dwLength)
{
	EnterCriticalSection(m_lpCriticalSection);
	
	DWORD dwWrittenBytes = 0;
	DWORD dwRes = SetFilePointer(	m_hTargetFile, 
									m_lpSegments[m_stuDownInfo.dwCurSegmentIndex].dwStartPos + m_lpSegments[m_stuDownInfo.dwCurSegmentIndex].dwFinished, 
									NULL, 
									FILE_BEGIN);
	if (0xFFFFFFFF != dwRes)
	{
		WriteFile(	m_hTargetFile,
					lpBuffer,
					dwLength,
					&dwWrittenBytes,
					NULL);					
	}

	LeaveCriticalSection(m_lpCriticalSection);

	return dwWrittenBytes;
}
//--------------------------------------------
//	Name:WriteTailInfoToFile
//	Introduce:Write dwLength data from lpBuffer to target file
//--------------------------------------------
LONG CHttpDownloadThread::WriteTailInfoToFile()
{
	EnterCriticalSection(m_lpCriticalSection);
	
	DWORD dwWrittenBytes = 0;
	DWORD dwRes = SetFilePointer(	m_hTargetFile, 
									m_stuDownInfo.dwTailInfoOffset, 
									NULL, 
									FILE_BEGIN);
	if (0xFFFFFFFF != dwRes)
	{
		WriteFile(m_hTargetFile, &m_dwSegmentsCnt, sizeof(DWORD), &dwWrittenBytes, NULL);
		for (DWORD dwIndex = 0; dwIndex < m_dwSegmentsCnt; dwIndex++)
		{
			WriteFile(m_hTargetFile, &m_lpSegments[dwIndex], sizeof(DataSegmentInfo), &dwWrittenBytes, NULL);
		}	
	}

	LeaveCriticalSection(m_lpCriticalSection);

	return HTTP_E_NOERROR;
}
//--------------------------------------------




//--------------------------------------------
VOID CALLBACK CHttpDownloadThread::AsyncInternetCallback(HINTERNET hInternet,
    DWORD dwContext,
    DWORD dwInternetStatus,
    LPVOID lpvStatusInformation,
    DWORD dwStatusInformationLength)
{
   CHttpDownloadThread* p = (CHttpDownloadThread*)dwContext;
  
   //在我们的应用中，我们只关心下面三个状态
   switch(dwInternetStatus)
   {
   //句柄被创建
   case INTERNET_STATUS_HANDLE_CREATED:
	   p->m_hNetFile = (HINTERNET)(((LPINTERNET_ASYNC_RESULT)(lpvStatusInformation))->dwResult);
    break;
  
   //句柄被关闭
   case INTERNET_STATUS_HANDLE_CLOSING:
    SetEvent(p->m_hEvent[1]);
    break;

   //一个请求完成，比如一次句柄创建的请求，或者一次读数据的请求
   case INTERNET_STATUS_REQUEST_COMPLETE:
    if (ERROR_SUCCESS == ((LPINTERNET_ASYNC_RESULT)
     (lpvStatusInformation))->dwError)
    { 
     //设置句柄被创建事件或者读数据成功完成事件
     SetEvent(p->m_hEvent[0]);
    }
    else
    { 
     //如果发生错误，则设置子线程退出事件 这里也是一个陷阱，经常会忽视处理这个错误，
     SetEvent(p->m_hEvent[2]);
    }
    break;

   case INTERNET_STATUS_CONNECTION_CLOSED:
    SetEvent(p->m_hEvent[2]);
    break;

   }
}
//--------------------------------------------
BOOL CHttpDownloadThread::WaitExitEvent()
{
   DWORD dwRet = WaitForMultipleObjects(3, m_hEvent, FALSE, INFINITE);
  
   switch (dwRet)
   {
   case WAIT_OBJECT_0:		//句柄被创建事件或者读数据请求成功完成事件
   case WAIT_OBJECT_0+1:	//句柄被关闭事件
   case WAIT_OBJECT_0+2:	//用户要求终止子线程事件或者发生错误事件
    break;
   }

   return WAIT_OBJECT_0 != dwRet;
}
//--------------------------------------------
