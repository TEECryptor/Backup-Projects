//-------------------------------------------------------------------
#include "stdafx.h"
#include "VGSFileDownloadManager.h"
#include "VGSFileDownloadStreamTask.h"
#include "VGSFileDownloadTask.h"
#include "..\\csysFunc.h"
#include "VGSN3DFileDownloadStreamTask.h"
//-------------------------------------------------------------------
#ifdef _DEBUG
#pragma comment(lib, "VGSDownloadlib_d.lib")
#else
#pragma comment(lib, "VGSDownloadlib.lib")
#endif 
//-------------------------------------------------------------------
long HttpCallbackFunc(LONG lTaskID, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2, LPVOID lpUser)
{
	if (NULL != lpUser)
	{
		return ((CVGSFileDownloadManager*)lpUser)->OnHttpDownloadEvent(lTaskID, lThreadID, dwEvent, dwParam1, dwParam2);
	}

	return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
CVGSFileDownloadManager::CVGSFileDownloadManager() 
{
	InitializeCriticalSection(&m_pTaskCS);
	m_maxDownloadingCount = 16;

	m_savePath = GetTempFolder();
	m_sCurrentSourcePath = "";

	m_pHttpDownloadIF = Create_HttpDownload();  // create http download object
	m_pHttpDownloadIF->Initialize();

	// Set configure data to downloader
	HttpDownloadCfg cfg;
	memset(&cfg, 0, sizeof(cfg));
	cfg.dwMaxTaskCnt   = m_maxDownloadingCount;
	cfg.dwMaxThreadCnt = 3;
	cfg.lpHttpCallbackFunc = HttpCallbackFunc;
	cfg.lpUser = this;
	m_pHttpDownloadIF->Configure(cfg); 
}
//-------------------------------------------------------------------
CVGSFileDownloadManager::~CVGSFileDownloadManager()
{
	ClearAllTask();

	delete m_pHttpDownloadIF;
	m_pHttpDownloadIF = 0;

	DeleteCriticalSection(&m_pTaskCS);
	Destroy_HttpDownload(m_pHttpDownloadIF);
}
//-------------------------------------------------------------------
/** create a download task with specific mode
@param bStreamMode   in  : is stream mode or not
@param sURL          in  : url of the file to be downloaded
@param sSaveFile 	 out : save path of the downloaded file, if "", save to the temp folder of this program
@return  task pointer
*/
//-------------------------------------------------------------------
VGSFileDownloadTaskInfo* CVGSFileDownloadManager::CreateVGSFileDownloadTask(bool bStreamMode, 
																			const string& sName, 
																			const string &sURL, 
																			VGS_DOWNLOADFILE_TYPE type, 
																			VGS_FILE_USAGE usage, 
																			CFileDwonloadInfo* pObject)
{
	// 检查是否已经有相同的下载任务
	VGSFileDownloadTaskInfo *pDownloadTaskInfo = FindTask(sURL);
	if (NULL != pDownloadTaskInfo)
	{
		pObject->sLocalFile = pDownloadTaskInfo->sLocalFile;

		// 登记usage信息，等待下载完成
		CUsageAndObject usageObj(usage, pObject);
		pDownloadTaskInfo->vUsageAndPtr.push_back(usageObj);

		// 发送下载完成消息
		DownloadFinished(pDownloadTaskInfo);
		
		return pDownloadTaskInfo;
	}

	// 创建新的下载任务
	pDownloadTaskInfo = new VGSFileDownloadTaskInfo();
	pDownloadTaskInfo->pTask = NULL;
	pDownloadTaskInfo->sURL  = sURL;
	pDownloadTaskInfo->sName = sName;
	pDownloadTaskInfo->bDownloaded = false;
	pDownloadTaskInfo->vUsageAndPtr.push_back(CUsageAndObject(usage, pObject));

	// 当前的下载路径
	std::string sFullURL = _getFullPath(m_sCurrentSourcePath, sURL);
	if (_isLocalPath(sFullURL))
	{
		// 本地路径和下载路径相同
		pDownloadTaskInfo->sLocalFile = sFullURL;  
		pDownloadTaskInfo->pTask = NULL;
		pDownloadTaskInfo->bDownloaded = true;
		m_vpDownloadTaskInfo.push_back(pDownloadTaskInfo);
		pObject->sLocalFile = sFullURL;
		pObject->dwTotalBytes = 100;
		pObject->dwFinishedBytes = 100;

		// 发送下载完成消息
		DownloadFinished(pDownloadTaskInfo);
	}
	else
	{ 
		// Create a non-stream task
		if (false == bStreamMode)
			pDownloadTaskInfo->pTask = (IVGSFileDownloadTask*)new CVGSFileDownloadTask(m_pHttpDownloadIF);
		// Create a stream task
		else
			pDownloadTaskInfo->pTask = (IVGSFileDownloadTask*)new CVGSN3DFileDownloadStreamTask(this, m_pHttpDownloadIF);
		if (pDownloadTaskInfo->pTask) 
		{		
			std::string sFileToSave = "";
			sFileToSave = m_savePath + _GetPureFileNameStr(sURL);
			pDownloadTaskInfo->sLocalFile = sFileToSave;
			pObject->sLocalFile = sFileToSave;

			EnterCriticalSection(&m_pTaskCS);
			m_vpDownloadTaskInfo.push_back(pDownloadTaskInfo);
			LeaveCriticalSection(&m_pTaskCS);
			
			pDownloadTaskInfo->pTask->Init(sName, sFullURL, sFileToSave);
			pObject->dwTotalBytes = pDownloadTaskInfo->pTask->GetFileSize();

			pDownloadTaskInfo->pTask->CreateThread();
			pDownloadTaskInfo->pTask->Download();
		}
	}

	return pDownloadTaskInfo;
}	
//-------------------------------------------------------------------
//	Name:FindTask
//	Introduce:Find a download task from list
//-------------------------------------------------------------------
VGSFileDownloadTaskInfo* CVGSFileDownloadManager::FindTask(const string &sURL)
{
	VGSFileDownloadTaskInfo* pFoundTaskInfo = NULL;
	for (vector<VGSFileDownloadTaskInfo*>::iterator it = m_vpDownloadTaskInfo.begin();
		 it != m_vpDownloadTaskInfo.end();
		 it++)
	{
		if ((*it)->pTask && (*it)->pTask->GetURL() == sURL)
		{
			pFoundTaskInfo = *it;
			break;
		}
	}
	return pFoundTaskInfo;
}
//-------------------------------------------------------------------
//	Name:RemoveTask
//	Introduce:Remove a download task from list
//-------------------------------------------------------------------
long CVGSFileDownloadManager::RemoveTask(IVGSFileDownloadTask *pTask, bool bEraseFile)
{
	EnterCriticalSection(&m_pTaskCS);
	VGSFileDownloadTaskInfo* pFoundTaskInfo = NULL;
	for (vector<VGSFileDownloadTaskInfo*>::iterator it = m_vpDownloadTaskInfo.begin();
		 it != m_vpDownloadTaskInfo.end();
		 it++)
	{
		if ((*it)->pTask == pTask)
		{
			pTask->Stop();
			pTask->Release();
			m_vpDownloadTaskInfo.erase(it);
			break;
		}
	}
	LeaveCriticalSection(&m_pTaskCS); 

	return 0;
}
//-------------------------------------------------------------------
// set the maximum of the downloading task in parallel
//-------------------------------------------------------------------
void CVGSFileDownloadManager::SetMaxDownloadingTask(int maxCount)
{
	m_maxDownloadingCount = maxCount;
}
//-------------------------------------------------------------------
// remove all the download task
//-------------------------------------------------------------------
void CVGSFileDownloadManager::ClearAllTask()
{
	EnterCriticalSection(&m_pTaskCS);

	m_pHttpDownloadIF->StopAllTasks();

	vector<VGSFileDownloadTaskInfo*>::iterator it = m_vpDownloadTaskInfo.begin();
	for (; it != m_vpDownloadTaskInfo.end(); ++it)
	{
		if (NULL != (*it)->pTask)
		{
			(*it)->pTask->Release();
		}
	}
	m_vpDownloadTaskInfo.clear();

	LeaveCriticalSection(&m_pTaskCS);
}


// 传给下载器的回调函数，处理下载器消息
long CVGSFileDownloadManager::OnHttpDownloadEvent(LONG lTaskID, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2)
{
	
	switch(dwEvent)
	{
	case eHTTPCBEvent_StatusChanged:		// 下载状态改变
		OnHttpDownloadStatusChange(lTaskID, lThreadID, dwParam1);
		break;
	case eHTTPCBEvent_ProgressChanged:      // 下载进度
		OnHttpDownloadProgress(lTaskID, lThreadID, dwParam1, dwParam2);
		break;
	case eHTTPCBEvent_Error:
		{
			//TCHAR tcErrorMsg[256] = {_T("\0")};
			//_stprintf(tcErrorMsg, _T("Task ID: %d occurred an error!\nError Code: %d"), lTaskID, dwParam1);
			//MessageBox(tcErrorMsg);
		}
		break;
	}

	return 0;
}
//-------------------------------------------------------------------
//	Name:OnHttpDownloadStatusChange
//	Introduce:处理下载器下载任务的状态变化
//-------------------------------------------------------------------
void CVGSFileDownloadManager::OnHttpDownloadStatusChange(LONG lTaskID,  LONG lThreadID, DWORD dwNewStatus)
{
	EnterCriticalSection(&m_pTaskCS);

	vector<VGSFileDownloadTaskInfo*>::iterator it = m_vpDownloadTaskInfo.begin();
	for (; it != m_vpDownloadTaskInfo.end(); ++it)
	{
		IVGSFileDownloadTask *pTask = (*it)->pTask;
		if (pTask && pTask->GetTaskID() == lTaskID)
		{
			// 整个任务的状态变化
			if (lThreadID == -1)
			{
				if (dwNewStatus == eHttpTaskStatus_Completed)
				{
					DownloadFinished(*it);
				}
				else if (dwNewStatus == eHttpTaskStatus_Stopped)
				{
					DownloadStopped(*it);
				}
			}
			// 按线程上报的状态
			else
			{
				/*
				 *  Don't support stream task yet
				*/
				/*
				if ((*it)->GetDownloadMode() == VGS_DOWNLOAD_MODE_STREAM) // stream task
				{
					CVGSFileDownloadStreamTask *pTask = (CVGSFileDownloadStreamTask*)(*it);
					if (pTask)
					{
						if (dwNewStatus == eHttpTaskStatus_Completed)
						{
							pTask->OnPrecessDownloadStatusChange(lThreadID, VGS_DOWNLOADTASK_STATUS_COMPLETED);  // 转交给下载任务处理，计算整个任务总的下载进度
						}
						else if (dwNewStatus == eHttpTaskStatus_Stopped)
						{
							pTask->OnPrecessDownloadStatusChange(lThreadID, VGS_DOWNLOADTASK_STATUS_TERMINATED);  // 转交给下载任务处理，计算整个任务总的下载进度
						}
					}
				}*/
			}

			break;
		}
	}

	LeaveCriticalSection(&m_pTaskCS);
}
//-------------------------------------------------------------------
//	Name:OnHttpDownloadProgress
//	Introduce:处理下载器下载任务的进度变化
//-------------------------------------------------------------------
void CVGSFileDownloadManager::OnHttpDownloadProgress(LONG lTaskID,  LONG lThreadID, DWORD dwFinishedBytes, DWORD dwCurrentSpeed)
{
	EnterCriticalSection(&m_pTaskCS);

	vector<VGSFileDownloadTaskInfo*>::iterator it = m_vpDownloadTaskInfo.begin();
	for (; it != m_vpDownloadTaskInfo.end(); ++it)
	{
		IVGSFileDownloadTask *pTask = (*it)->pTask;
		if (pTask && pTask->GetTaskID() == lTaskID)
		{
			// 上报的是整个任务，而不是单个线程，的统计信息
			if (lThreadID <= 0) 
			{				
				DownloadPercent(*it, dwFinishedBytes, dwCurrentSpeed);
			}
			// 上报的是单个线程的统计信息
			else 
			{
				/*
				 *  Don't support stream task yet
				*/
				/*
				if ((*it)->GetDownloadMode() == VGS_DOWNLOAD_MODE_STREAM)  // stream task, 以线程为单位上报进度
				{
					CVGSFileDownloadStreamTask *pTask = (CVGSFileDownloadStreamTask*)(*it);
					if (pTask)
					{
						pTask->OnPrecessDownloadProgress(lThreadID, dwFinishedBytes, dwCurrentSpeed);  // 转交给下载任务处理，计算整个任务总的下载进度
					}
				} 
				*/
			}
			break;
		}
	}
	LeaveCriticalSection(&m_pTaskCS);
}

string CVGSFileDownloadManager::GetTempFolder()
{
	return "d:\\";
}

// 传给下载任务的回调函数，处理每个任务的状态变化消息
long CVGSFileDownloadManager::OnVGSDownloadTaskStatusChange(DWORD TaskPtr, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2)
{

	return 0;
}

// 传给下载任务的回调函数，处理每个任务的下载进度消息
long CVGSFileDownloadManager::OnVGSDownloadTaskProgress(DWORD TaskPtr, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2)
{	
	switch (dwEvent)
	{
	case VGS_DOWNLOAD_MSG_DOWNLOADPROGRESS:  // report the download progress
		PostMessage(m_hMsgWnd, VGS_DOWNLOAD_MSG_DOWNLOADPROGRESS, TaskPtr, dwParam1);
		break;
	case VGS_DOWNLOAD_EVENT_N3DONEIMG_DOWNLOADED:
		// int imgIdx = (int)dwParam1; // image index
		PostMessage(m_hMsgWnd, VGS_DOWNLOAD_EVENT_N3DONEIMG_DOWNLOADED, TaskPtr, dwParam1);
		break;
	case VGS_DOWNLOAD_EVENT_N3DSCENE_DOWNLOADED:
		PostMessage(m_hMsgWnd, VGS_DOWNLOAD_EVENT_N3DSCENE_DOWNLOADED, TaskPtr, 0);
		break;
	}
	return 0;
}
//-------------------------------------------------------------------
//	Name:DownloadPercent
//	Introduce:下载进度消息处理
//-------------------------------------------------------------------
void CVGSFileDownloadManager::DownloadPercent(VGSFileDownloadTaskInfo* pInfo, 
											  DWORD dwFinishedBytes, 
											  DWORD dwCurrentSpeed)
{
	if (NULL != pInfo)
	{
		size_t count = pInfo->vUsageAndPtr.size();
		for (size_t i = 0; i < count; ++i)
		{
			if (pInfo->vUsageAndPtr[i].ptr)
			{
				pInfo->vUsageAndPtr[i].ptr->dwFinishedBytes = dwFinishedBytes;
				PostMessage(m_hMsgWnd, 
							WM_DOWNLOAD_MSG_DOWNLOADPROGRESS, 
							(LONG)pInfo->vUsageAndPtr[i].usage, 
							(LONG)pInfo->vUsageAndPtr[i].ptr);
			}
		}
	}
}
//-------------------------------------------------------------------
//	Name:DownloadFinished
//	Introduce:处理下载完成消息
//-------------------------------------------------------------------
void CVGSFileDownloadManager::DownloadFinished(VGSFileDownloadTaskInfo* pInfo)
{
	if (NULL != pInfo)
	{
		pInfo->bDownloaded = true;
		if (pInfo->pTask)
		{
			pInfo->pTask->SetStatus(VGS_DOWNLOADTASK_STATUS_COMPLETED);
		}
	}

	size_t count = pInfo->vUsageAndPtr.size();
	for (size_t i = 0; i < count; ++i)
	{
		pInfo->vUsageAndPtr[i].ptr->dwFinishedBytes = pInfo->vUsageAndPtr[i].ptr->dwTotalBytes;
		PostMessage(m_hMsgWnd, WM_DOWNLOAD_MSG_DOWNLOADPROGRESS, (LONG)pInfo->vUsageAndPtr[i].usage, (LONG)pInfo->vUsageAndPtr[i].ptr);
		PostMessage(m_hMsgWnd, WM_DOWNLOAD_MSG_FILE_DOWNLOADED, (LONG)pInfo->vUsageAndPtr[i].usage, (DWORD)pInfo->vUsageAndPtr[i].ptr);
	}
}
//-------------------------------------------------------------------
//	Name:DownloadStopped
//	Introduce:处理下载停止消息
//-------------------------------------------------------------------
void CVGSFileDownloadManager::DownloadStopped(VGSFileDownloadTaskInfo* pInfo)
{
	if (NULL == pInfo)
		return;

	size_t count = pInfo->vUsageAndPtr.size();
	for (size_t i = 0; i < count; ++i)
	{
		//TODO
	}
}
//---------------------------------------------------------------------
