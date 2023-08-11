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
 , m_pLogFile(NULL)
 , m_lpCriticalSection(NULL)
 , m_hThread(NULL)
 , m_Socket(0)
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
										  CHttpLogFile* pLogFile,
										  LPTHREADINFO lpInfo)
{
	m_hConnect = hConnect;
	m_hTargetFile = hTargetFile;
	m_pLogFile = pLogFile;

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
	{
		DWORD dwError = GetLastError();
		HTTPLOG2(m_pLogFile, "Thread ID %d: ERROR --> Start(): Try to start download thread failed! Error code: %d", m_dwID, dwError);
		return HTTP_E_CREATETHREAD_ERROR;								
	}

	HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> Start(): Download thread is started!", m_dwID);

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

		CloseNetFile();
		HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> Stop(): Download thread is stopped!", m_dwID);
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

		// Check and trim header at first for each segment
		if (0 == m_lpSegments[dwCurSegmentIndex].dwFinished)
		{
			HTTPLOG2(m_pLogFile, "Thread ID %d: DONE --> DownloadingFunc(): The first time for segment %d, call CheckAndTrimHeader()!", m_dwID, dwCurSegmentIndex);

			lDownloadSize = CheckAndTrimHeader(btBuffer, HTTP_MAX_BUFF_LEN);
			
			// Error, can't download file
			if (lDownloadSize <= 0)
			{
				lRes = lDownloadSize;
				break;
			}
		}
		// Download data and write to file
		else
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
			HTTPLOG2(m_pLogFile, "Thread ID %d: DONE --> DownloadingFunc(): Current segment %d has finished, open net file with the next segment!", m_dwID, m_stuDownInfo.dwCurSegmentIndex);

			// Download the next segment
			m_stuDownInfo.dwCurSegmentIndex++;

			// All segment have been downloaded, compeleted!
			if (m_stuDownInfo.dwCurSegmentIndex >= m_dwSegmentsCnt)
			{
				HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> DownloadingFunc(): All segments have finished!", m_dwID);

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
	HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> OpenNetFile(): Enter", m_dwID);

	// Close the old request
	GIF(CloseNetFile());
/*	
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD( 2, 2 ); 
	WSAStartup( wVersionRequested, &wsaData );
*/
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET  == m_Socket)
	{
		DWORD dwError = GetLastError();
		HTTPLOG2(m_pLogFile, "Thread ID %d: ERROR --> OpenNetFile(): Create socket failed! Error code: %d", m_dwID, dwError);
		return HTTP_E_OPENNETFILE_ERROR;
	}

	hostent *he;
	he = gethostbyname(m_stuDownInfo.tcHost);

	sockaddr_in cl_addr; 
    cl_addr.sin_family = AF_INET; 
    cl_addr.sin_port = htons(80); 
	cl_addr.sin_addr = *((in_addr *)he->h_addr);

	lRes = connect(m_Socket, (struct sockaddr*)(&cl_addr), sizeof(cl_addr));
	if (SOCKET_ERROR == lRes)
    { 
		lRes = WSAGetLastError();
		HTTPLOG2(m_pLogFile, "Thread ID %d: ERROR --> OpenNetFile(): Try to connect server failed! Error Code = %d", m_dwID, lRes);
		return HTTP_E_OPENNETFILE_ERROR;
    } 

	char sendBuf[512] = {0};
	strcpy(sendBuf, "GET ");
	strcat(sendBuf, m_stuDownInfo.tcNetFile);
	strcat(sendBuf, " HTTP/1.1");
	strcat(sendBuf, "\r\n");
	//
	strcat(sendBuf, "Host:");
	strcat(sendBuf, m_stuDownInfo.tcHost);
	strcat(sendBuf, "\r\n");
	//
	strcat(sendBuf, "Accept:*/*");
	strcat(sendBuf, "\r\n");
	//
	char cRange[128] = {0};
	DWORD dwIndex = m_stuDownInfo.dwCurSegmentIndex;
	sprintf(cRange, "Range: bytes=%d-%d", m_lpSegments[dwIndex].dwStartPos + m_lpSegments[dwIndex].dwFinished, m_lpSegments[dwIndex].dwStartPos + m_lpSegments[dwIndex].dwDataLength);
	strcat(sendBuf, cRange);
	strcat(sendBuf, "\r\n");
	//
	strcat(sendBuf, "Conection:Keep-Alive");
	strcat(sendBuf, "\r\n");
	//
	strcat(sendBuf, "\r\n");

	char csLogText[128] = {0};
	sprintf(csLogText, "Thread ID %d: DONE --> OpenNetFile(): Try to open net file with data range: %s", m_dwID, cRange);
	HTTPLOG(m_pLogFile, csLogText);

	lRes = send(m_Socket, sendBuf, strlen(sendBuf), 0);
	if (SOCKET_ERROR == lRes)
    { 
		lRes = WSAGetLastError();
		HTTPLOG2(m_pLogFile, "Thread ID %d: ERROR --> OpenNetFile(): Try to send request to server failed! Error code: %d", m_dwID, lRes);
		return HTTP_E_OPENNETFILE_ERROR;
    } 

	HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> OpenNetFile(): Open net file successfully!", m_dwID);

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
	if (0 != m_Socket)
	{
		closesocket(m_Socket); 
		m_Socket = 0;
		HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> CloseNetFile(): Net file has been closed, set m_Socket = NULL.", m_dwID);
	}

/*
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
*/
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
	{
		DWORD dwError = GetLastError();
		HTTPLOG2(m_pLogFile, "Thread ID %d: DONE --> CreateDownloadThread(): Create thread object failed! Error Code = %d", m_dwID, dwError);
		return HTTP_E_CREATETHREAD_ERROR;		
	}
	
	HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> CreateDownloadThread(): Create thread object successfully!", m_dwID);

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
	HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> DeleteDownloadThread(): Thread object has deleted!", m_dwID);

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

	HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> CheckAndTrimHeader(): Enter!", m_dwID);

	DWORD dwRequestSize = dwLength;
	DWORD dwRetainSize = m_lpSegments[m_stuDownInfo.dwCurSegmentIndex].dwDataLength - 
						 m_lpSegments[m_stuDownInfo.dwCurSegmentIndex].dwFinished;
	LONG lRes;
	char* lpTmpBuf = new char[dwRequestSize];

	// Set socket working in non-bocking mode
	unsigned long ul = 1000;  
	lRes = ioctlsocket(m_Socket, FIONBIO, &ul);
	if (0 != lRes)
	{
		int iError = WSAGetLastError();
		HTTPLOG2(m_pLogFile, "Thread ID %d: DONE --> CheckAndTrimHeader(): Call ioctlsocket() to set non-bocking failed! Error Code: %d", m_dwID, iError);
	}
	else
	{
		HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> CheckAndTrimHeader(): Call ioctlsocket() to set non-bocking successfully!", m_dwID);
	}

	// Receive data from server
	lRes = recv(m_Socket, lpTmpBuf, dwRequestSize, 0); 
	if (lRes > 0)
	{
		HTTPLOG2(m_pLogFile, "Thread ID %d: DONE --> CheckAndTrimHeader(): Receive data from server successfully! Data length = %d", m_dwID, lRes);

		float fHttpVsr;
		int iStatus;
		long lDataLen;
		char cTmep[1024] = {0};
		sscanf(strstr((const char*)lpTmpBuf, "HTTP/"), "HTTP/%f %d", &fHttpVsr, &iStatus);
		//sscanf(strstr((const char*)lpTmpBuf, "Content-Length"), "Content-Length: %d", &lDataLen);
		if (iStatus >= 300)
		{
			HTTPLOG2(m_pLogFile, "Thread ID %d: ERROR --> CheckAndTrimHeader(): Check status from server failed! Server Status = %d", m_dwID, iStatus);
			return HTTP_E_OPENNETFILE_ERROR;
		}

		char* pStart = strstr(lpTmpBuf, "\r\n\r\n");
		if (NULL == pStart)
		{
			HTTPLOG1(m_pLogFile, "Thread ID %d: ERROR --> CheckAndTrimHeader(): Received data content has an uncompleted header!", m_dwID);
			return HTTP_E_OPENNETFILE_ERROR;
		}

		DWORD dwHeaderLen = (pStart - lpTmpBuf) + sizeof("\r\n\r\n") - 1;
		DWORD dwRealDataLen = lRes - dwHeaderLen;
		if (dwRealDataLen <= dwRetainSize)
		{
			memcpy(lpBuffer, lpTmpBuf+dwHeaderLen, dwRealDataLen);
			lRes = dwRealDataLen;
		}
		else
		{
			memcpy(lpBuffer, lpTmpBuf+dwHeaderLen, dwRetainSize);
			lRes = dwRetainSize;
		}

		HTTPLOG2(m_pLogFile, "Thread ID %d: DONE --> CheckAndTrimHeader(): Get the file content data!  Content length = %d", m_dwID, lRes);
	}
	else
	{
		HTTPLOG2(m_pLogFile, "Thread ID %d: ERROR --> CheckAndTrimHeader(): Receive data from server failed! Error code: %d", m_dwID, lRes);
	}
	delete []lpTmpBuf;
	lpTmpBuf = NULL;
	HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> CheckAndTrimHeader(): Exit!", m_dwID);

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

	dwDownloaddedSize = recv(m_Socket, (char*)lpBuffer, dwRequestSize, 0); 
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
