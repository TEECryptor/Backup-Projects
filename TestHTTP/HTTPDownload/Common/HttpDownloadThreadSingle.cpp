//--------------------------------------------
//	File Name:HttpDownloadThreadSingle.cpp
//	Introduce:The implement file of class CHttpDownloadThreadSingle
//	Date:2010-10-10
//--------------------------------------------
#include ".\httpdownloadthreadsingle.h"
//--------------------------------------------
//	Name:CHttpDownloadThreadSingle
//	Introduce:Construction
//--------------------------------------------
CHttpDownloadThreadSingle::CHttpDownloadThreadSingle(void)
 : CHttpDownloadThread()
 , m_hInternetSession(NULL)
{
}
//--------------------------------------------
//	Name:~CHttpDownloadThreadSingle
//	Introduce:Distruction
//--------------------------------------------
CHttpDownloadThreadSingle::~CHttpDownloadThreadSingle(void)
{
}
//--------------------------------------------



//--------------------------------------------
//	Name:Start
//	Introduce:Start this downloading thread
//--------------------------------------------
LONG CHttpDownloadThreadSingle::Start()
{
	return CHttpDownloadThread::Start();
}
//--------------------------------------------
//	Name:Stop
//	Introduce:Stop and delete the thread object
//--------------------------------------------
LONG CHttpDownloadThreadSingle::Stop()
{	
	CHttpDownloadThread::Stop();

	DisconnectInternet();

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:DownloadingFunc
//	Introduce:Downloading function for this thread
//--------------------------------------------
DWORD CHttpDownloadThreadSingle::DownloadingFunc()
{
	LONG lRes = 0;
	TCHAR tcNetFileURL[1024] = {0};
	_tcscpy(tcNetFileURL, _T("http://"));
	_tcscat(tcNetFileURL, m_stuDownInfo.tcHost);
	_tcscat(tcNetFileURL, m_stuDownInfo.tcNetFile);
	
	lRes = ConnectInternet();
	if (HTTP_E_NOERROR != lRes)
	{
		HTTPLOG2(m_pLogFile, "Thread ID %d: ERROR --> DownloadingFunc(): Try to connect internet failed! Error Code: %d", m_dwID, lRes);
		goto ThreadEnd;
	}

	HINTERNET hURL = InternetOpenUrl(m_hInternetSession, tcNetFileURL, NULL, 0, 0, 0);
	if (NULL == hURL) 
	{
		DWORD dwError = GetLastError();
		HTTPLOG2(m_pLogFile, "Thread ID %d: ERROR --> DownloadingFunc(): Call API InternetOpenUrl() failed! Error Code: %d", m_dwID, dwError);
		goto ThreadEnd;
	}

    BOOL bResult = TRUE;
	LONG lDownloadSize = 0;
	LONG lWrittenSize = 0;
	BYTE btBuffer[HTTP_MAX_BUFF_LEN] = {0};

	do
	{
		bResult = InternetReadFile(hURL,
                                   btBuffer,
                                   HTTP_MAX_BUFF_LEN,
                                   (LPDWORD)&lDownloadSize);
		if (FALSE == bResult)
		{
			DWORD dwError = GetLastError();
			HTTPLOG2(m_pLogFile, "Thread ID %d: ERROR --> DownloadingFunc(): Call API InternetReadFile() failed! Error Code: %d", m_dwID, dwError);
			break;
		}

		if(lDownloadSize > 0)
		{
			lWrittenSize = WriteDataToFile(btBuffer, lDownloadSize);
            
			m_lpSegments[m_stuDownInfo.dwCurSegmentIndex].dwFinished += lWrittenSize;
			m_stuDownInfo.dwFinishedLenght += lWrittenSize;	
			
			WriteTailInfoToFile();
		}
		else
		{			
			HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> DownloadingFunc(): Thead downloading is completed!", m_dwID);
			m_bCompeleted = TRUE;
		}

	}while(!m_bRequireExit && !m_bCompeleted);

ThreadEnd:
	Stop();

	return 0;
}
//--------------------------------------------
//	Name:ConnectInternet
//	Introduce:Connect to internet
//--------------------------------------------
LONG CHttpDownloadThreadSingle::ConnectInternet()
{
	// Disconnect current at first
	DisconnectInternet();

	try
	{
		m_hInternetSession = InternetOpen(	_T("Microsoft Internet Explorer"),
											INTERNET_OPEN_TYPE_PRECONFIG,
											NULL, 
											NULL, 
											0);
		if (NULL == m_hInternetSession) 
		{		
			DWORD dwError = GetLastError();
			HTTPLOG2(m_pLogFile, "Thread ID %d: ERROR --> ConnectInternet(): Connect internet failed! Error Code: %d", m_dwID, dwError);
			return HTTP_E_OPENNET_ERROR;
		}	
	}
	catch(...)
	{
		return HTTP_E_OPENNET_ERROR;
	}
			
	HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> ConnectInternet(): Connect internet successfully!", m_dwID);

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:DisconnectInternet
//	Introduce:Disconnect from internet
//--------------------------------------------
LONG CHttpDownloadThreadSingle::DisconnectInternet()
{
	try
	{
		if (NULL != m_hInternetSession)
		{
			InternetCloseHandle(m_hInternetSession);
			m_hInternetSession = NULL;
			HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> DisconnectInternet(): Disconnect internet successfully!", m_dwID);
		}
	}
	catch(...)
	{
		return HTTP_E_CLOSENET_ERROR;
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------
