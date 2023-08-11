//--------------------------------------------
//	File Name:HttpDownloadStreamTask.cpp
//	Introduce:The implement file of class CHttpDownloadStreamTask
//	Date:2010-06-05
//--------------------------------------------
#include "HttpDownloadStreamTask.h"
//--------------------------------------------
//	Name:CHttpDownloadStreamTask
//	Introduce:Construction
//--------------------------------------------
CHttpDownloadStreamTask::CHttpDownloadStreamTask(void)
 : CHttpDownloadTask()
{
}
//--------------------------------------------
//	Name:~CHttpDownloadStreamTask
//	Introduce:Distruction
//--------------------------------------------
CHttpDownloadStreamTask::~CHttpDownloadStreamTask(void)
{
	// Clear all download threads
	CHttpDownloadThread* pThread = NULL;
	for (DownloadThreadPtrArray::iterator it = m_arDownloadThread.begin();
		 it != m_arDownloadThread.end();
		 it++)
	{
		pThread = *it;
		if (NULL != pThread)
		{
			pThread->Stop();
			delete pThread;
			pThread = NULL;
		}
	}
	m_arDownloadThread.clear();

	// Change downloaded file name here. Because for stream task, 
	// the downloading file is used by caller untill task deleting.
	if (m_dwCurFinishedBytes >= m_dwNetFileSize)
	{
		CompletedTargetFile();
	}
	DeleteTaskThread();
	CloseTargetFile();
}
//--------------------------------------------



//--------------------------------------------
//	Name:SetTaskInfo
//	Introduce:Set the download information for this task.
//--------------------------------------------
LONG CHttpDownloadStreamTask::SetTaskInfo(HINTERNET hSession, 
										  TaskInfo Info, 
										  BOOL bForceDownload)
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

	// Create task working thread
	GIF (CreateTaskThread())

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:Start
//	Introduce:Start current task.
//--------------------------------------------
LONG CHttpDownloadStreamTask::Start()
{
	for (DownloadThreadPtrArray::iterator it = m_arDownloadThread.begin();
		it != m_arDownloadThread.end();
		it++)
	{
		StartDownloadThread((*it)->GetThreadID());
	}
	return HTTP_E_NOERROR;
}
/*
//--------------------------------------------
//	Name:Stop
//	Introduce:Stop current task.
//--------------------------------------------
LONG CHttpDownloadStreamTask::Stop()
{
	return HTTP_E_NOERROR;
}*/
//--------------------------------------------
//	Name:SetAsWaiting
//	Introduce:Set task current status to waiting
//--------------------------------------------
void CHttpDownloadStreamTask::SetAsWaiting()
{
	//Do nothing, stream task has no waiting status
}
//--------------------------------------------
//	Name:GetCurProgress
//	Introduce:Return current progress information of this task
//--------------------------------------------
void CHttpDownloadStreamTask::GetCurProgress(DWORD &dwFinishedBytes, 
											 DWORD &dwSpeed)
{
	dwFinishedBytes = 0;
	dwSpeed = 0;
}
//--------------------------------------------



//--------------------------------------------
//	Name:AddDownloadThread
//	Introduce:Add a download thread to this stream task
//--------------------------------------------
LONG CHttpDownloadStreamTask::AddDownloadThread(LONG lThreadID, 
												DWORD dwStartPos, 
												DWORD dwDataLength)
{
	if (NULL != GetDownloadThreadByID(lThreadID))
		return HTTP_E_THREADID_ISEXIST;

	if (dwStartPos >= m_dwNetFileSize)
		return HTTP_E_THREADRANGE_ERROR;

	if (dwStartPos + dwDataLength > m_dwNetFileSize)
		dwDataLength = m_dwNetFileSize - dwStartPos;

	DWORD dwSegIndex = 0;
	DataSegmentInfo segment;
	memset(&segment, 0, sizeof(DataSegmentInfo));
	segment.dwStartPos = dwStartPos;
	segment.dwDataLength = dwDataLength;
	segment.dwFinished = 0;

	LONG lRes = HTTP_E_NOERROR;

	GIF (CheckSegmentIsLegal(segment))

	GIF (AddSegmentsInfo(segment, dwSegIndex))

	ThreadInfo info;
	memset(&info, 0, sizeof(ThreadInfo));
	_tcscpy(info.tcNetFile, m_stuDownloadInfo.tcNetFile);
	info.dwFinishedLenght = segment.dwFinished;
	info.dwTailInfoOffset = m_dwNetFileSize + 
							sizeof(TempFileTailer) + 
							dwSegIndex * (sizeof(DWORD) + sizeof(DataSegmentInfo));

	CHttpDownloadThread* pDownloadThread = new CHttpDownloadThread();
	pDownloadThread->SetThreadID(lThreadID);
	pDownloadThread->SetDataSegments(1, &segment);
	pDownloadThread->SetDownloadInfo(m_hConnect, m_hTargetFile, &info);
	pDownloadThread->SetCriticalSection(&m_csTargetWriting);
	m_arDownloadThread.push_back(pDownloadThread);
	m_mapThreadFinishedBytes[lThreadID] = segment.dwFinished;
	m_mapThreadSegmentIndex[lThreadID] = dwSegIndex;
	m_mapThreadCompleted[lThreadID] = FALSE;

	GIF (WriteCommonTailInfo())

	GIF (WriteNewSegmentInfo(dwSegIndex, segment))

	GIF (WriteFreeSegmentsInfo())

	GIF (WriteTotalTailSize())

	m_dwCurFinishedBytes += segment.dwFinished;
	SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_ProgressChanged, m_dwCurFinishedBytes, 0);

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:GetStreamTaskThreadInfo
//	Introduce:Get a download thread information in this stream task
//--------------------------------------------
LONG CHttpDownloadStreamTask::GetStreamTaskThreadInfo(LONG lThreadID, 
													  DWORD &dwStartPos, 
													  DWORD &dwDataLength, 
													  DWORD &dwFinished)
{
	CHttpDownloadThread* pThread = GetDownloadThreadByID(lThreadID);
	if (NULL == pThread)
		return HTTP_E_THREADID_NOTFOUND;

	DataSegmentInfo info;
	pThread->GetDataSegments(&info);
	dwStartPos = info.dwStartPos;
	dwDataLength = info.dwDataLength;
	dwFinished = info.dwFinished;

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:DelDownloadThread
//	Introduce:Delete a download thread in this stream task
//--------------------------------------------
LONG CHttpDownloadStreamTask::DelDownloadThread(LONG lThreadID)
{
	BOOL bFound = FALSE;
	CHttpDownloadThread* pThread = NULL;
	for (DownloadThreadPtrArray::iterator it = m_arDownloadThread.begin();
		 it != m_arDownloadThread.end();
		 it++)
	{
		pThread = *it;
		if (NULL != pThread && pThread->GetThreadID() == lThreadID)
		{
			pThread->Stop();
			delete pThread;
			pThread = NULL;
			bFound = TRUE;
			ThreadSegmentIndexMap::iterator itSegIndex = m_mapThreadSegmentIndex.find(lThreadID);
			m_mapThreadSegmentIndex.erase(itSegIndex);
			m_mapThreadFinishedBytes[lThreadID] = 0;
			m_mapThreadCompleted[lThreadID] = FALSE;
			m_arDownloadThread.erase(it);
			break;
		}
	}
	
	return bFound ? HTTP_E_NOERROR : HTTP_E_TASKID_NOTFOUND;
}

//--------------------------------------------
//	Name:StartDownloadThread
//	Introduce:Start a download thread in this stream task
//--------------------------------------------
LONG CHttpDownloadStreamTask::StartDownloadThread(LONG lThreadID)
{
	CHttpDownloadThread* pThread = GetDownloadThreadByID(lThreadID);
	if (NULL == pThread)
		return HTTP_E_THREADID_NOTFOUND;

	LONG lRes = pThread->Start();
	if (HTTP_E_NOERROR != lRes)
		return lRes;

	SendEventToParent(m_dwTaskID, lThreadID, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Running, 0);

	m_eCurStatus = eHttpTaskStatus_Running;
	SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_StatusChanged, m_eCurStatus, 0);

	return lRes;
}
LONG CHttpDownloadStreamTask::Stop()
{
	for (DownloadThreadPtrArray::iterator it = m_arDownloadThread.begin();
		it != m_arDownloadThread.end();
		it++)
	{
		StopDownloadThread((*it)->GetThreadID());
	}

	return HTTP_E_NOERROR;
}

//--------------------------------------------
//	Name:StopDownloadThread
//	Introduce:Stop a download thread in this stream task
//--------------------------------------------
LONG CHttpDownloadStreamTask::StopDownloadThread(LONG lThreadID)
{
	CHttpDownloadThread* pThread = GetDownloadThreadByID(lThreadID);
	if (NULL == pThread)
		return HTTP_E_THREADID_NOTFOUND;

	LONG lRes = pThread->Stop();
	if (HTTP_E_NOERROR != lRes)
		return lRes;

	SendEventToParent(m_dwTaskID, lThreadID, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Stopped, 0);

	// If all thread are stopped or completed, set task status to eHttpTaskStatus_Stopped
	BOOL bAllThreadStopped = TRUE;
	for (DownloadThreadPtrArray::iterator it = m_arDownloadThread.begin();
		 it != m_arDownloadThread.end();
		 it++)
	{
		if (!(*it)->IsStop() && !(*it)->IsCompeleted())
		{
			bAllThreadStopped = FALSE;
			break;
		}
	}
	m_eCurStatus = bAllThreadStopped ? eHttpTaskStatus_Stopped : eHttpTaskStatus_Running;
	SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_StatusChanged, m_eCurStatus, 0);

	return lRes;
}
//--------------------------------------------



//--------------------------------------------
//	Name:WriteCommonTailInfo
//	Introduce:Write the tail data structure at the end of target file
//--------------------------------------------
LONG CHttpDownloadStreamTask::WriteCommonTailInfo()
{
	EnterCriticalSection(&m_csTargetWriting);

	TempFileTailer tailer;
	memset(&tailer, 0, sizeof(TempFileTailer));

	_tcscpy(tailer.tcTailFlg, HTTP_DOWNLOADING_FLAG);
	tailer.dwNetFileSize = m_dwNetFileSize;
	tailer.dwFinishedSize = 0;		//In stream task, current finished bytes in each thread
	memcpy(&tailer.stuTaskInfo, &m_stuDownloadInfo, sizeof(TaskInfo));

	// A thread only has a segment in stream task
	tailer.stuTaskInfo.dwThreadCnt = (DWORD)m_lstSegments.size();

	DWORD dwRes = SetFilePointer(	m_hTargetFile, 
									m_dwNetFileSize, 
									NULL, 
									FILE_BEGIN);
	if (0xFFFFFFFF == dwRes)
	{
		LeaveCriticalSection(&m_csTargetWriting);
		return HTTP_E_SETFILEPOINTER_ERROR;
	}
    
	DWORD dwWrittenBytes = 0;
	WriteFile(m_hTargetFile, &tailer, sizeof(TempFileTailer), &dwWrittenBytes, NULL);	

	LeaveCriticalSection(&m_csTargetWriting);

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:WriteInitSegmentsInfo
//	Introduce:Write initliaze segments information for all
//			  download threads
//--------------------------------------------
LONG CHttpDownloadStreamTask::WriteInitSegmentsInfo()
{
	EnterCriticalSection(&m_csTargetWriting);

	DWORD dwRes = SetFilePointer(	m_hTargetFile, 
									m_dwNetFileSize + sizeof(TempFileTailer), 
									NULL, 
									FILE_BEGIN);
	if (0xFFFFFFFF == dwRes)
	{		
		LeaveCriticalSection(&m_csTargetWriting);
		return HTTP_E_SETFILEPOINTER_ERROR;
	}

	DWORD dwSegmentCnt = 0;
	DWORD dwWrittenBytes = 0;

	for (SegmentInfoList::iterator it = m_lstSegments.begin();
		 it != m_lstSegments.end();
		 it++)
	{		
		DWORD dwSegCntPerThread = 1;
		WriteFile(m_hTargetFile, &dwSegCntPerThread, sizeof(DWORD), &dwWrittenBytes, NULL);	

		DataSegmentInfo segInfo = *it;
		WriteFile(m_hTargetFile, &segInfo, sizeof(DataSegmentInfo), &dwWrittenBytes, NULL);	
	}

	LeaveCriticalSection(&m_csTargetWriting);

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:WriteTotalTailSize
//	Introduce:Write total tail data size at the end of target file
//--------------------------------------------
LONG CHttpDownloadStreamTask::WriteTotalTailSize()
{
	EnterCriticalSection(&m_csTargetWriting);

	DWORD dwTotalTailDataSize = 0;
	
	// Add the file tailer information size
	dwTotalTailDataSize += sizeof(TempFileTailer);

	// Add all segment data size
	DWORD dwSegmentCnt = (DWORD)m_lstSegments.size();
	dwTotalTailDataSize += dwSegmentCnt * (sizeof(DWORD) + sizeof(DataSegmentInfo));

	DWORD dwRes = SetFilePointer(	m_hTargetFile, 
									m_dwNetFileSize + dwTotalTailDataSize, 
									NULL, 
									FILE_BEGIN);
	if (0xFFFFFFFF == dwRes)
	{		
		LeaveCriticalSection(&m_csTargetWriting);
		return HTTP_E_SETFILEPOINTER_ERROR;
	}
    
	DWORD dwWrittenBytes = 0;
	WriteFile(m_hTargetFile, &dwTotalTailDataSize, sizeof(DWORD), &dwWrittenBytes, NULL);	
	
	LeaveCriticalSection(&m_csTargetWriting);

	return HTTP_E_NOERROR;		
}
//--------------------------------------------
//	Name:CheckSegmentIsLegal
//	Introduce:To check a new thread segment is legal or not
//--------------------------------------------
LONG CHttpDownloadStreamTask::CheckSegmentIsLegal(DataSegmentInfo segInfo)
{
	BOOL bIsLegal = TRUE;
	for (DownloadThreadPtrArray::iterator it  = m_arDownloadThread.begin();
		it != m_arDownloadThread.end();
		it++)
	{
		DataSegmentInfo UsingSegment;
		(*it)->GetDataSegments(&UsingSegment);
	/*
		if (segInfo.dwStartPos >= UsingSegment.dwStartPos &&
			segInfo.dwStartPos < (UsingSegment.dwStartPos + UsingSegment.dwDataLength))
	*/
		// New thread data range MUST > all created threads data range!!!!
		if (segInfo.dwStartPos < (UsingSegment.dwStartPos + UsingSegment.dwDataLength))
		{
			bIsLegal = FALSE;
			break;
		}
	}

	return bIsLegal ? HTTP_E_NOERROR : HTTP_E_THREADRANGE_ERROR;
}
//--------------------------------------------
//	Name:AddeSegmentsInfo
//	Introduce:Add a segments information for this task,
//            when a thread has been added
//--------------------------------------------
LONG CHttpDownloadStreamTask::AddSegmentsInfo(DataSegmentInfo &segInfo, DWORD &dwIndex)
{
	// Save original list
	SegmentInfoList tempSegmentList;
	for (SegmentInfoList::iterator it = m_lstSegments.begin();
		 it != m_lstSegments.end();
		 it++)
	{
		DataSegmentInfo curSeg = *it;
		tempSegmentList.push_back(curSeg);
	}
	m_lstSegments.clear();

	// Sort all segments and save back to m_lstSegments
	DWORD dwCurIndex = 0;
	DWORD dwLastSegEndPos = 0;
	for (SegmentInfoList::iterator it = tempSegmentList.begin();
		 it != tempSegmentList.end();
		 it++)
	{
		DataSegmentInfo curSeg = *it;

		// Find next segment need to divide
		if (dwLastSegEndPos > (curSeg.dwStartPos + curSeg.dwDataLength))
			continue;

		// Check the last end position
		if (dwLastSegEndPos > curSeg.dwStartPos)
		{
			curSeg.dwDataLength = curSeg.dwDataLength - (dwLastSegEndPos-curSeg.dwStartPos);
			curSeg.dwFinished = max(0, curSeg.dwFinished-(dwLastSegEndPos-curSeg.dwStartPos));
			curSeg.dwStartPos = dwLastSegEndPos;
		}

		// The adding segment and current segment have some same range
		if (segInfo.dwStartPos < (curSeg.dwStartPos+curSeg.dwDataLength) && 
			(segInfo.dwStartPos + segInfo.dwDataLength) > curSeg.dwStartPos)
		{
			// There is an old segment same as to the adding one
			if (segInfo.dwStartPos == curSeg.dwStartPos &&
				segInfo.dwDataLength == curSeg.dwDataLength)
			{
				segInfo.dwFinished = curSeg.dwFinished;
				m_lstSegments.push_back(curSeg);
				dwIndex = dwCurIndex++;
			}
			// The original segment and adding one have the same start position
			else if (segInfo.dwStartPos == curSeg.dwStartPos)
			{
				// Divide the original segment to two segments
				if (segInfo.dwDataLength < curSeg.dwDataLength)
				{					
					segInfo.dwFinished = min(curSeg.dwFinished, segInfo.dwDataLength);
					m_lstSegments.push_back(segInfo);
					dwIndex = dwCurIndex++;
					//
					curSeg.dwStartPos = curSeg.dwStartPos + segInfo.dwDataLength;
					curSeg.dwDataLength = curSeg.dwDataLength - segInfo.dwDataLength;
					curSeg.dwFinished = max(0, LONG(curSeg.dwFinished-segInfo.dwDataLength));
					m_lstSegments.push_back(curSeg);
					dwCurIndex++;
				}
				// The original segment is shortter then adding one
				// Need to adjust the next one start position
				else
				{
					dwLastSegEndPos = segInfo.dwStartPos + segInfo.dwDataLength;
					segInfo.dwFinished = curSeg.dwFinished;
					m_lstSegments.push_back(segInfo);
					dwIndex = dwCurIndex++;
				}
			}
			// The start position of adding one is in (seg.dwStartPos, seg.dwStartPos+seg.dwDataLength)
			else if (segInfo.dwStartPos > curSeg.dwStartPos)
			{
				// The first part of original segment
				DataSegmentInfo segNew;
				segNew.dwStartPos = curSeg.dwStartPos;
				segNew.dwDataLength = segInfo.dwStartPos - curSeg.dwStartPos;
				segNew.dwFinished = min(curSeg.dwFinished, segNew.dwDataLength);
				m_lstSegments.push_back(segNew);
				dwCurIndex++;

				// The middle part of original segment, it's same to adding one
				segInfo.dwFinished = max(0, LONG(curSeg.dwFinished-segNew.dwDataLength));
				m_lstSegments.push_back(segInfo);
				dwIndex = dwCurIndex++;

				// If the end position of adding < the end position of original,
				// there is a end part segment.
				if ((segInfo.dwStartPos + segInfo.dwDataLength) < 
					(curSeg.dwStartPos + curSeg.dwDataLength))
				{
					DataSegmentInfo segNew2;
					segNew2.dwStartPos = segInfo.dwStartPos + segInfo.dwDataLength;
					segNew2.dwDataLength = curSeg.dwDataLength - (segNew.dwDataLength+segInfo.dwDataLength);
					segNew2.dwFinished = max(0, LONG(curSeg.dwFinished-(segNew.dwDataLength+segInfo.dwDataLength)));
					m_lstSegments.push_back(segNew2);
					dwCurIndex++;
				}
				// If the end position of adding >= the end position of original,
				// Need to adjust the next one start position in this case
				else
				{
					dwLastSegEndPos = segInfo.dwStartPos + segInfo.dwDataLength;
				}
			}
		}
		else
		{
			m_lstSegments.push_back(curSeg);
			dwCurIndex++;
		}
	}

	return HTTP_E_NOERROR;		
}
//--------------------------------------------
//	Name:WriteNewSegmentInfo
//	Introduce:Write the new segment information to project file
//            Should be called when new thread added
//--------------------------------------------
LONG CHttpDownloadStreamTask::WriteNewSegmentInfo(DWORD dwSegIndex, 
												  DataSegmentInfo segInfo)
{
	EnterCriticalSection(&m_csTargetWriting);

	DWORD dwWrittenBytes = 0;
	DWORD dwRes = SetFilePointer(	m_hTargetFile, 
									m_dwNetFileSize + sizeof(TempFileTailer) + dwSegIndex * (sizeof(DWORD) + sizeof(DataSegmentInfo)), 
									NULL, 
									FILE_BEGIN);
	if (0xFFFFFFFF != dwRes)
	{
		DWORD dwSegCnt = 1;
		WriteFile(m_hTargetFile, &dwSegCnt, sizeof(DWORD), &dwWrittenBytes, NULL);	
		WriteFile(m_hTargetFile, &segInfo, sizeof(DataSegmentInfo), &dwWrittenBytes, NULL);	
	}

	LeaveCriticalSection(&m_csTargetWriting);

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:WriteFreeSegmentsInfo
//	Introduce:Write free segments information to project file
//            Should be called when new thread added
//--------------------------------------------
LONG CHttpDownloadStreamTask::WriteFreeSegmentsInfo()
{
	EnterCriticalSection(&m_csTargetWriting);

	DWORD dwIndex = 0;
	for (SegmentInfoList::iterator it = m_lstSegments.begin();
		 it != m_lstSegments.end();
		 it++, dwIndex++)
	{		
		BOOL bIsFreeSegment = TRUE;
		for (DownloadThreadPtrArray::iterator itThread = m_arDownloadThread.begin();
			itThread != m_arDownloadThread.end();
			itThread++)
		{
			LONG lThreadID = (*itThread)->GetThreadID();
			if (m_mapThreadSegmentIndex[lThreadID] == dwIndex)
			{
				bIsFreeSegment = FALSE;
				break;
			}
		}

		if (FALSE == bIsFreeSegment)
			continue;
		
		DataSegmentInfo segInfo = *it;

		DWORD dwWrittenBytes = 0;
		DWORD dwRes = SetFilePointer(	m_hTargetFile, 
										m_dwNetFileSize + sizeof(TempFileTailer) + dwIndex * (sizeof(DWORD) + sizeof(DataSegmentInfo)), 
										NULL, 
										FILE_BEGIN);
		if (0xFFFFFFFF != dwRes)
		{
			DWORD dwSegCnt = 1;
			WriteFile(m_hTargetFile, &dwSegCnt, sizeof(DWORD), &dwWrittenBytes, NULL);	
			WriteFile(m_hTargetFile, &segInfo, sizeof(DataSegmentInfo), &dwWrittenBytes, NULL);	
		}
	}

	LeaveCriticalSection(&m_csTargetWriting);

	return HTTP_E_NOERROR;
}
//--------------------------------------------
/*
//--------------------------------------------
//	Name:OnStart
//	Introduce:Try to start downloading thread based on lThreadID
//--------------------------------------------
LONG CHttpDownloadStreamTask::OnStart(LONG lThreadID)
{
	LONG lRes = HTTP_E_NOERROR;
	CHttpDownloadThread* pThread = GetDownloadThreadByID(lThreadID);
	if (NULL == pThread)
		return HTTP_E_THREADID_NOTFOUND;

	GIF (pThread->Start())

	SendEventToParent(m_dwTaskID, lThreadID, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Running, 0);

	return lRes;
}
//--------------------------------------------
//	Name:OnCompleted
//	Introduce:Thread lThreadID has completed
//--------------------------------------------
LONG CHttpDownloadStreamTask::OnCompleted(LONG lThreadID)
{
	SendEventToParent(m_dwTaskID, lThreadID, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Completed, 0);
	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:OnRestart
//	Introduce:Return re-start download thread based on lThreadID
//--------------------------------------------
LONG CHttpDownloadStreamTask::OnRestart(LONG lThreadID)
{
	OnStop(lThreadID);

	LONG lRes = HTTP_E_NOERROR;

	// Try to start again
	GIF (OnStart(lThreadID))

	return HTTP_E_NOERROR;
}*/
//--------------------------------------------
//	Name:OnStop
//	Introduce:Try to stop all downloading threads
//--------------------------------------------
LONG CHttpDownloadStreamTask::OnStop()
{
	DWORD dwFinishedBytes = 0;
	DWORD dwTotalFinishedByte = 0;

	// Stop all downloading threads
	CHttpDownloadThread* pThread = NULL;
	for (DownloadThreadPtrArray::iterator it = m_arDownloadThread.begin();
		 it != m_arDownloadThread.end();
		 it++)
	{
		pThread = *it;
		LONG lThreadID = pThread->GetThreadID();
		if (NULL != pThread)
		{
			pThread->Stop();
			dwFinishedBytes = pThread->GetFinishedBytes();
			dwTotalFinishedByte += dwFinishedBytes;
			SendEventToParent(m_dwTaskID, lThreadID, eHTTPCBEvent_ProgressChanged, dwFinishedBytes, 0);
			if (FALSE == m_mapThreadCompleted[lThreadID])
			{
				SendEventToParent(m_dwTaskID, lThreadID, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Stopped, 0);
			}
		}
	}

	m_dwCurrentSpeed = 0;
	m_eCurStatus = eHttpTaskStatus_Stopped;
	SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_ProgressChanged, dwTotalFinishedByte, 0);
	SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Stopped, 0);

	return HTTP_E_NOERROR;
}
//--------------------------------------------
//	Name:OnProgress
//	Introduce:Update downloading threads progress
//--------------------------------------------
LONG CHttpDownloadStreamTask::OnProgress()
{
	BOOL bAllThreadStopped = TRUE;
	DWORD dwTotalFinishedByte = 0;
	DWORD dwCurrentTime = GetTickCount();
	DWORD dwDeltaTime = dwCurrentTime - m_dwLastTime;
	m_dwLastTime = dwCurrentTime;

	// Update each thread progress
	for (DownloadThreadPtrArray::iterator it = m_arDownloadThread.begin();
		 it != m_arDownloadThread.end();
		 it ++)
	{
		DWORD dwFinishedBytes = (*it)->GetFinishedBytes();
		dwTotalFinishedByte += dwFinishedBytes;

		LONG lThreadID = (*it)->GetThreadID();
		if (FALSE == m_mapThreadCompleted[lThreadID])
		{
			DWORD dwSpeed = 1000 * (dwFinishedBytes - m_mapThreadFinishedBytes[lThreadID]) / dwDeltaTime;
			m_mapThreadFinishedBytes[lThreadID] = dwFinishedBytes;
			SendEventToParent(m_dwTaskID, lThreadID, eHTTPCBEvent_ProgressChanged, dwFinishedBytes, dwSpeed);

			if (TRUE == (*it)->IsCompeleted())
			{
				m_mapThreadCompleted[lThreadID] = TRUE;
				SendEventToParent(m_dwTaskID, lThreadID, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Completed, 0);
			}
			else if (FALSE == (*it)->IsStop())
			{
				bAllThreadStopped = FALSE;
			}
		}
	}

	// Update total progress for this stream task
	m_dwCurrentSpeed = 1000 * (dwTotalFinishedByte - m_dwCurFinishedBytes) / dwDeltaTime;
	SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_ProgressChanged, dwTotalFinishedByte, m_dwCurrentSpeed);
	m_dwCurFinishedBytes = dwTotalFinishedByte;

	// To check all threads are stopped, or all data are downloaded or not
	if (TRUE == bAllThreadStopped)
	{
		if (m_dwCurFinishedBytes >= m_dwNetFileSize)
		{
			DisconnectInternet();
			m_dwCurrentSpeed = 0;
			m_eCurStatus = eHttpTaskStatus_Completed;
			SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Completed, 0);
		}
		else
		{		
			m_eCurStatus = eHttpTaskStatus_Stopped;
			SendEventToParent(m_dwTaskID, -1, eHTTPCBEvent_StatusChanged, eHttpTaskStatus_Stopped, 0);
		}
	}

	return HTTP_E_NOERROR;
}
//--------------------------------------------


//--------------------------------------------
//	Name:GetDownloadThreadByID
//	Introduce:Return download thread object pointer by ID
//--------------------------------------------
CHttpDownloadThread* CHttpDownloadStreamTask::GetDownloadThreadByID(LONG lThreadID)
{
	CHttpDownloadThread* pFoundThread = NULL;
	for (DownloadThreadPtrArray::iterator it = m_arDownloadThread.begin();
		 it != m_arDownloadThread.end();
		 it++)
	{
		if ((*it)->GetThreadID() == lThreadID)
		{
			pFoundThread = *it;
			break;
		}
	}
	
	return pFoundThread;
}
//--------------------------------------------