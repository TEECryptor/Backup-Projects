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
 , m_hNetFile(NULL)
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
//	Name:SetDownloadInfo
//	Introduce:Set download information for this thread
//--------------------------------------------
LONG CHttpDownloadThreadSingle::SetDownloadInfo(HINTERNET hNetFile, 
												HANDLE hTargetFile, 
												CHttpLogFile* pLogFile, 
												LPTHREADINFO lpInfo)
{
	m_hNetFile = hNetFile;
	return CHttpDownloadThread::SetDownloadInfo(NULL, hTargetFile, pLogFile, lpInfo);
}
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
	return CHttpDownloadThread::Stop();
}
//--------------------------------------------
//	Name:DownloadingFunc
//	Introduce:Downloading function for this thread
//--------------------------------------------
DWORD CHttpDownloadThreadSingle::DownloadingFunc()
{	
	if (NULL == m_hNetFile)
	{
		HTTPLOG1(m_pLogFile, "Thread ID %d: ERROR --> DownloadingFunc(): Internet file handle m_hNetFile is NULL!", m_dwID);
		goto ThreadEnd;
	}
		
	HTTPLOG1(m_pLogFile, "Thread ID %d: DONE --> DownloadingFunc(): Try to call API InternetReadFile() to read net file......", m_dwID);

    BOOL bResult = TRUE;
	LONG lDownloadSize = 0;
	LONG lWrittenSize = 0;
	BYTE btBuffer[HTTP_MAX_BUFF_LEN] = {0};

	do
	{
		bResult = InternetReadFile(m_hNetFile,
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