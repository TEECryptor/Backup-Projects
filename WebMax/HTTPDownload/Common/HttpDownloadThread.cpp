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

	LONG lRes = HTTP_E_NOERROR;

	// Open a request at first
	GIF (OpenNetFile())

	return lRes;
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
		::WaitForSingleObject(m_hThread, 5000);

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

	DWORD dwCurSegmentIndex = 0;
	while (	FALSE == m_bCompeleted &&
			FALSE == m_bRequireExit)
	{
		dwCurSegmentIndex = m_stuDownInfo.dwCurSegmentIndex;

		// Download data and write to file
		lDownloadSize = DownloadData(btBuffer, HTTP_MAX_BUFF_LEN);
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

		Sleep(10);
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
	
	try
	{		
		DWORD dwRes = 0;

		// Try to open
		const TCHAR* acceptTYPE[] = {_T("*/*"), 0}; 
		m_hNetFile = HttpOpenRequest(m_hConnect,
									 _T("GET"),
									 m_stuDownInfo.tcNetFile,
									 _T("HTTP/1.1"),
									 NULL,
									 acceptTYPE,
									 INTERNET_FLAG_RELOAD,
									 0);
		
		if (NULL == m_hNetFile)
			lRes = HTTP_E_OPENNETFILE_ERROR;

		TCHAR tcRangeHeader[256] = {_T("\0")};
		DWORD dwIndex = m_stuDownInfo.dwCurSegmentIndex;
		_stprintf(	tcRangeHeader, 
					_T("Range: bytes=%d-%d\r\n"), 
					m_lpSegments[dwIndex].dwStartPos + m_lpSegments[dwIndex].dwFinished, 
					m_lpSegments[dwIndex].dwStartPos + m_lpSegments[dwIndex].dwDataLength);
		dwRes = HttpAddRequestHeaders(	m_hNetFile, 
										tcRangeHeader, 
										(DWORD)_tcslen(tcRangeHeader), 
										HTTP_ADDREQ_FLAG_ADD_IF_NEW);
		if (0 == dwRes)
		{
			DWORD dwError = GetLastError();
			CloseNetFile();
			return HTTP_E_NOMULTI_THREAD;
		}

		HttpSendRequest(m_hNetFile, NULL, 0, 0, 0);
	}
	catch(...)
	{
		lRes = HTTP_E_OPENNETFILE_ERROR;
	}

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
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);
	m_hThread = NULL;

	return HTTP_E_NOERROR;
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
