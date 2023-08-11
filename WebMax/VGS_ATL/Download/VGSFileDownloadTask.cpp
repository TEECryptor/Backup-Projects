#include "stdafx.h"
#include "VGSFileDownloadTask.h"

CVGSFileDownloadTask::CVGSFileDownloadTask(IHttpDownloadIF * pHttpDownloader) 
                      : IVGSFileDownloadTask(pHttpDownloader)
{
	m_Mode = VGS_DOWNLOAD_MODE_NORMAL;

	m_TaskID = -1;
	m_defaultThreadCnt = 2;
}

CVGSFileDownloadTask::~CVGSFileDownloadTask()
{
	Release();
}

DWORD CVGSFileDownloadTask::Init(const string& sName, const string &sURL, const string &sSaveFile, const string &sUsrName, const string &passward)
{
	m_sName = sName;
	m_sURL = sURL;
	m_sSaveFile = sSaveFile;
	if (m_pHttpDownloader)
	{
		m_TaskID = m_pHttpDownloader->CreateTask(sURL.c_str(), sSaveFile.c_str(), m_defaultThreadCnt);
	}

	return m_TaskID;
}
DWORD CVGSFileDownloadTask::GetFileSize()
{
	if (m_pHttpDownloader)
	{
		return m_pHttpDownloader->GetTaskNetFileSize(m_TaskID);
	}
	return 0;
}
void CVGSFileDownloadTask::Download(bool bForceDownload/* = false*/)
{
	if (m_pHttpDownloader)
	{
		m_pHttpDownloader->StartTask(m_TaskID);
	}
}

DWORD CVGSFileDownloadTask::Stop()
{
	if (m_pHttpDownloader)
	{
		return m_pHttpDownloader->StopTask(m_TaskID);
	}
	return 0;
}
DWORD CVGSFileDownloadTask::Release()
{
	if (m_pHttpDownloader && m_TaskID > 0)
	{
		m_pHttpDownloader->DeleteTask(m_TaskID);
		m_pHttpDownloader = 0;
	}
	return 0;
}

void CVGSFileDownloadTask::EnableReportStatus(bool bEnable)
{

}

void  CVGSFileDownloadTask::CreateThread()
{
	return;
}
//void  CVGSFileDownloadTask::SetThreadCount(DWORD dwCount)
//{
//
//}