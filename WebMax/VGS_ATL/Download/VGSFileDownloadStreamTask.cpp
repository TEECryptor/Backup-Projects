#include "stdafx.h"
#include "VGSFileDownloadStreamTask.h"

CVGSFileDownloadStreamTask::CVGSFileDownloadStreamTask(CVGSFileDownloadManager* pMgr, IHttpDownloadIF * pHttpDownloader) 
                            : IVGSFileDownloadTask(pHttpDownloader)
{
	m_pVGSDownloadManager = pMgr;
	m_Mode = VGS_DOWNLOAD_MODE_STREAM;
	// m_bScanOK = false;

	m_port = 80;
}
CVGSFileDownloadStreamTask::~CVGSFileDownloadStreamTask()
{

}
DWORD CVGSFileDownloadStreamTask::Init(const string& sName, const string &sURL, const string &sSaveFile, const string &sUsrName, const string &sPassward)
{
	m_sName     = sName;
	m_sURL      = sURL;
	m_sSaveFile = sSaveFile; 
	m_sUserName = sUsrName;
	m_sPassward = sPassward;

	if (m_pHttpDownloader)
	{
		m_TaskID = m_pHttpDownloader->CreateStreamTask(sURL.c_str(), sSaveFile.c_str());

		ParseURL();  // Ω‚ŒˆURL
	}

	return m_TaskID;
}
DWORD CVGSFileDownloadStreamTask::GetFileSize()
{
	if (m_pHttpDownloader)
	{
		return m_pHttpDownloader->GetTaskNetFileSize(m_TaskID);
	}
	return 0;
}
void  CVGSFileDownloadStreamTask::Download(bool bForceDownload/* = false*/)
{
	if (m_pHttpDownloader)
	{
		m_pHttpDownloader->StartTask(m_TaskID);
	}
}

DWORD CVGSFileDownloadStreamTask::Stop()
{
	if (m_pHttpDownloader)
	{
		m_pHttpDownloader->StopTask(m_TaskID);
	}
	return 0;
}
DWORD CVGSFileDownloadStreamTask::Release()
{
	if (m_pHttpDownloader && m_TaskID > 0)
	{
		m_pHttpDownloader->DeleteTask(m_TaskID);
		m_pHttpDownloader = 0;
	}
	return 0;
}
void CVGSFileDownloadStreamTask::EnableReportStatus(bool bEnable)
{

}
//void CVGSFileDownloadStreamTask::CreateThread()
//{
//	
//}
//DWORD CVGSFileDownloadStreamTask::AddStreamTaskThread(DWORD threadID, DWORD dwStart, DWORD dwEnd)
//{
//	return 0;
//}

//  Ω‚ŒˆURL

bool CVGSFileDownloadStreamTask::ParseURL()
{
	// m_sURL = "http://192.168.1.111:8080/downloadtest/downloadtest.htm";
	// To check URL string has protocol or not
	LPCTSTR lpStart = m_sURL.c_str();
	if (_tcsnicmp(_T("Http://"), lpStart, 7) == 0)
	{
		lpStart += 7;
	}

	// Get host name string
	LPCTSTR lp = lpStart;
	LPCTSTR lpPortPos = lpStart;
	BOOL bHasPort = FALSE;
	while (*lp != _T('/'))
	{
		if (*lp == _T(':'))
		{
			bHasPort = TRUE;
			lpPortPos = lp + 1;  // start pos of port
		}
		lp++;
	}

	// Get port number string
	if (TRUE == bHasPort)
	{
		string strPort = m_sURL.substr(lpPortPos - m_sURL.c_str(), lp - lpPortPos);
		try
		{
			m_port = _ttol(strPort.c_str());
		}
		catch (...)
		{
			m_port = 80;
		}
	}

	return true;
	
}