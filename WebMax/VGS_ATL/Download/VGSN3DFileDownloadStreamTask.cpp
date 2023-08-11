#include "stdafx.h"
#include "VGSN3DFileDownloadStreamTask.h"

CVGSN3DFileDownloadStreamTask::CVGSN3DFileDownloadStreamTask(CVGSFileDownloadManager* pMgr, IHttpDownloadIF * pHttpDownloader) 
: CVGSFileDownloadStreamTask(pMgr,pHttpDownloader)
{
	m_ThreadIdx = 0;  // when use must > 0
	ZeroMemory(&m_N3DHeader, sizeof(m_N3DHeader));

	m_bSceneDownloaded = false;
	m_bSceneCreated    = false;
	m_bAllImageCreated = false;
}

CVGSN3DFileDownloadStreamTask::~CVGSN3DFileDownloadStreamTask()
{

}

void CVGSN3DFileDownloadStreamTask::CreateThread()
{
	ScanFileEx();  // 扫描文件，由子类实现

	// assign thread task
	m_vStreamThreadInfo.clear();
	m_setThreadImageInfo.clear();

	CStreamThreadInfo streamThreadInfo;

	int iThreadIdx = 1;
	// scene data
	if (m_N3DHeader.SceneDataPtr > 0 && m_N3DHeader.SceneZipDataLen > 0)
	{
		int thredCnt = GetThreadCount(m_N3DHeader.SceneZipDataLen); 
		DWORD dwDataLanPreThread = m_N3DHeader.SceneZipDataLen / thredCnt;
		for (int i = 0; i < thredCnt; ++i, ++iThreadIdx)
		{
			DWORD startPos = m_N3DHeader.SceneDataPtr + dwDataLanPreThread * i;
			DWORD dataLen = dwDataLanPreThread;
			if (i == thredCnt - 1)
			{
				dataLen = m_N3DHeader.SceneZipDataLen - (i * dwDataLanPreThread) + sizeof(DWORD);  // 最后还有四个字节的ImgeCount，也一起下载
			}
			streamThreadInfo.lThreadIdx  = iThreadIdx;
			streamThreadInfo.dwStartPos  = startPos;
			streamThreadInfo.dwDataLen   = dataLen;
			streamThreadInfo.dwFinished  = 0;
			streamThreadInfo.dwMoreInfo  = 0;  // scene data
			streamThreadInfo.dwCurSpeed  = 0;
			m_vStreamThreadInfo.insert(streamThreadInfo);
			m_pHttpDownloader->AddStreamTaskThread(m_TaskID, iThreadIdx, startPos, dataLen);
		}
	}

	// image files
	if (m_vImagePosInfo.size() > 0)
	{
		int threadCnt = GetThreadCount(m_N3DHeader.ImageDataLen);
		int iImageCntPerThread = m_vImagePosInfo.size() / threadCnt;
		for (int i = 0; i < threadCnt; ++i, ++iThreadIdx)
		{
			int curIdx = i * iImageCntPerThread;
			int nextIdx = (i + 1) * iImageCntPerThread;
			if (i == threadCnt - 1)
			{
				nextIdx = m_vImagePosInfo.size() - 1;
			}
			DWORD startPos = m_vImagePosInfo[curIdx].startPos;
			DWORD dataLen  = m_vImagePosInfo[nextIdx].startPos - startPos;
			streamThreadInfo.lThreadIdx  = iThreadIdx;
			streamThreadInfo.dwStartPos  = startPos;
			streamThreadInfo.dwDataLen   = dataLen;
			streamThreadInfo.dwFinished  = 0;
			streamThreadInfo.dwCurSpeed  = 0;
			streamThreadInfo.dwMoreInfo  = 1;  // image data
			m_vStreamThreadInfo.insert(streamThreadInfo);
			m_pHttpDownloader->AddStreamTaskThread(m_TaskID, iThreadIdx, startPos, dataLen);

			// 线程和图像文件的对应关系
			CThreadImageInfo threadImgInfo;
			threadImgInfo.lThreadIdx = iThreadIdx;
			for (int imgIdx = curIdx; imgIdx < nextIdx; ++imgIdx)
			{
				threadImgInfo.vImageIndex.push_back(imgIdx);
				threadImgInfo.vbDownloaded.push_back(false);
			}
			m_setThreadImageInfo.insert(threadImgInfo);
		}
	}


	// Get stream task target file name
	m_sSaveTmpFile = m_sSaveFile + string(HTTP_DOWNLOADING_EXT);  // 临时文件
}

// 单线程扫描文件
void CVGSN3DFileDownloadStreamTask::ScanFileEx()
{
	m_vImagePosInfo.clear();

	if (m_pHttpDownloader)
	{
		HINTERNET hInternetSession = InternetOpen("Microsoft Internet Explorer",
			INTERNET_OPEN_TYPE_PRECONFIG,
			NULL, NULL, 0);
		if (hInternetSession)
		{
			HINTERNET hURL = InternetOpenUrl(hInternetSession, 
				m_sURL.c_str(), 
				NULL, 
				0, 
				INTERNET_FLAG_RESYNCHRONIZE|INTERNET_FLAG_NEED_FILE, 
				0);


			DWORD dwBytesRead = 0;
			DWORD dwCurPos = 0;  // 读文件的当前指针位置

			// read n3d head
			// dwCurPos = InternetSetFilePointer(hURL, 0, 0, FILE_BEGIN, 0);
			DWORD bRead = InternetReadFile(hURL, (LPSTR)(&m_N3DHeader), sizeof(m_N3DHeader), &dwBytesRead);
			if (bRead && dwBytesRead > 0)
			{
				if (m_N3DHeader.Version >= 52001)  // 52001之后增加了图像文件索引
				{
					//
				}

			} // if m_N3DHeader

			InternetCloseHandle(hInternetSession);

		} // if  hInternetSession

	} // if m_pHttpDownloader
	
}

// 单线程扫描文件
void CVGSN3DFileDownloadStreamTask::ScanFile()
{
	m_vImagePosInfo.clear();

	if (m_pHttpDownloader)
	{
		HINTERNET hInternetSession = InternetOpen("Microsoft Internet Explorer",
													INTERNET_OPEN_TYPE_PRECONFIG,
													NULL, NULL, 0);
		if (hInternetSession)
		{
			HINTERNET hURL = InternetOpenUrl(hInternetSession, 
											m_sURL.c_str(), 
											NULL, 
											0, 
											INTERNET_FLAG_RESYNCHRONIZE|INTERNET_FLAG_NEED_FILE, 
											0);


			DWORD dwBytesRead = 0;
			DWORD dwCurPos = 0;  // 读文件的当前指针位置

			// read n3d head
			dwCurPos = InternetSetFilePointer(hURL, 0, 0, FILE_BEGIN, 0);
			DWORD bRead = InternetReadFile(hURL, (LPSTR)(&m_N3DHeader), sizeof(m_N3DHeader), &dwBytesRead);
			if (bRead && dwBytesRead > 0)
			{
				dwCurPos = InternetSetFilePointer(hURL, m_N3DHeader.ImageDataPtr, 0, FILE_BEGIN, 0);  // goto the beginning of the image data block

				DWORD imageCnt = 0;
				bool bRead = InternetReadFile(hURL, (LPSTR)(&imageCnt), sizeof(DWORD), &dwBytesRead);  // read image total count
				if (bRead && sizeof(DWORD) == dwBytesRead && imageCnt > 0)
				{
					dwCurPos += sizeof(DWORD);  // 跳过ImageCount的四个字节
					m_vImagePosInfo.reserve(imageCnt);
					CImagePosInfo imagePosInfo;
					imagePosInfo.bDownloaded = false;
					while (true)
					{
						imagePosInfo.startPos = dwCurPos;  // start position

						DWORD dataValue   = 0;
						dwBytesRead = 0;
						bRead = InternetReadFile(hURL, (LPSTR)(&dataValue), sizeof(DWORD), &dwBytesRead);  // read name length
						if (bRead && sizeof(dataValue) == dwBytesRead)
						{
							dwCurPos = InternetSetFilePointer(hURL, dataValue, 0, FILE_CURRENT, 0);  // name string
							bRead = InternetReadFile(hURL, (LPSTR)(&dataValue), sizeof(DWORD), &dwBytesRead);  // image content length
							if (bRead && sizeof(dataValue) == dwBytesRead)
							{
								dwCurPos = InternetSetFilePointer(hURL, dataValue, 0, FILE_CURRENT, 0);  // image content
								imagePosInfo.datalen = dwCurPos - imagePosInfo.startPos;  // data length
								m_vImagePosInfo.push_back(imagePosInfo);
							}
							else
							{
								break;
							}
						}
						else
						{
							break;
						} // if ... else

						//bReadEnd = ReadOneImageInfo(imagePosInfo, hURL);
					} // while
				} // if imagecnt
				
			} // if m_N3DHeader

			InternetCloseHandle(hInternetSession);

		} // if  hInternetSession

	} // if m_pHttpDownloader
}

// 读一个贴图文件信息
bool CVGSN3DFileDownloadStreamTask::ReadOneImageInfo(CImagePosInfo& imagePos, HINTERNET hURL)
{
	DWORD dataValue   = 0;
	DWORD dwBytesRead = 0;
	bool bRead = InternetReadFile(hURL, (LPSTR)(&dataValue), sizeof(dataValue), &dwBytesRead);  // name length
	if (bRead && sizeof(dataValue) == dwBytesRead)
	{
		bool dwPos = InternetSetFilePointer(hURL, dataValue, 0, FILE_CURRENT, 0);  // name string
		bRead = InternetReadFile(hURL, (LPSTR)(&dataValue), sizeof(dataValue), &dwBytesRead);  // image content length
		if (bRead && sizeof(dataValue) == dwBytesRead)
		{
			dwPos = InternetSetFilePointer(hURL, dataValue, 0, FILE_CURRENT, 0);  // image content
		}
		return true;  // successful read a image data
	}
	return false;  // if error or at the end of the file
}

// 处理每个线程的下载状态消息
void  CVGSN3DFileDownloadStreamTask::OnPrecessDownloadStatusChange(LONG lThreadID, VGS_DOWNLOADTASK_STATUS status)
{
	// threadID非正，表示上报的是整个任务的信息
	if (lThreadID <= 0)
	{
		return;
	}
	
	// 处理线程进度
	CStreamThreadInfo info;
	info.lThreadIdx = lThreadID;
	set<CStreamThreadInfo>::iterator it = m_vStreamThreadInfo.find(info);
	if (it != m_vStreamThreadInfo.end())
	{
		if (it->dwMoreInfo == 0 && status == VGS_DOWNLOADTASK_STATUS_COMPLETED)  // scene data downloaded
		{
			m_bSceneDownloaded = IsSceneDwonloaded();
			if (m_bSceneDownloaded)
			{
				m_pVGSDownloadManager->OnVGSDownloadTaskProgress(DWORD(this), lThreadID, VGS_DOWNLOAD_EVENT_N3DSCENE_DOWNLOADED, 0, 0);
			}
		}
	}
}

// 场景是否下载完成
bool CVGSN3DFileDownloadStreamTask::IsSceneDwonloaded()
{
	set<CStreamThreadInfo>::iterator it = m_vStreamThreadInfo.begin();
	for (; it != m_vStreamThreadInfo.end(); ++it)
	{
		if (it->dwMoreInfo == 0 && it->dwFinished < it->dwDataLen)
		{
			return false;
		}
	}
	return true;
}

// 处理每个线程的下载进度消息
void CVGSN3DFileDownloadStreamTask::OnPrecessDownloadProgress(LONG lThreadID, DWORD finishedBytes, DWORD currentSpeed)
{
	// threadID非正，表示上报的是整个任务的信息
	if (lThreadID <= 0)
	{
		return;
	}

	DWORD dwFinishedBytes = finishedBytes;
	DWORD dwCurrentSpeed = currentSpeed;

	// 计算整个任务的下载进度
	//m_pHttpDownloader->GetTaskProgress(m_TaskID, dwFinishedBytes, dwCurrentSpeed);	
	// report progress
	//m_pVGSDownloadManager->OnVGSDownloadTaskProgress(DWORD(this), lThreadID, VGS_DOWNLOAD_MSG_DOWNLOADPROGRESS, dwFinishedBytes, dwCurrentSpeed);

	// 处理线程进度
	CStreamThreadInfo info;
	info.lThreadIdx = lThreadID;
	set<CStreamThreadInfo>::iterator it = m_vStreamThreadInfo.find(info);
	if (it != m_vStreamThreadInfo.end())
	{
		// 获得当前任务的下载进度
		it->dwFinished = dwFinishedBytes;

		if (it->dwMoreInfo == 1) // image data
		{
			CThreadImageInfo threadImgInfo;
			threadImgInfo.lThreadIdx = lThreadID;
			set<CThreadImageInfo>::const_iterator it2 = m_setThreadImageInfo.find(threadImgInfo);
			if (it2 != m_setThreadImageInfo.end())
			{
				vector<int> vDownloadedImg;
				CheckDownloaded(*it2, dwFinishedBytes, vDownloadedImg);  // 检查下载完的文件

				vector<int>::iterator it3 = vDownloadedImg.begin();
				for (; it3 != vDownloadedImg.end(); ++it3)
				{
					// 一个图像数据段下载完成
					m_pVGSDownloadManager->OnVGSDownloadTaskProgress(DWORD(this), lThreadID, VGS_DOWNLOAD_EVENT_N3DONEIMG_DOWNLOADED, LONG(*it3), 0);
					// Sleep(0);
				}
			}
		}
		else if (it->dwMoreInfo == 0)  // scene data
		{
		  // should do nothing
		}			
	}

}

// 插件哪些文件下载完成
void CVGSN3DFileDownloadStreamTask::CheckDownloaded(const CThreadImageInfo& info, DWORD downloadedBytes, vector<int> &vDownloadedImgIdx)
{
	vDownloadedImgIdx.clear();
	CStreamThreadInfo threadInfo;
	threadInfo.lThreadIdx = info.lThreadIdx;
	set<CStreamThreadInfo>::iterator threadIter = m_vStreamThreadInfo.find(threadInfo);
	vector<int>::const_iterator it = info.vImageIndex.begin();
	DWORD dwBytes = 0;  // 累计图片文件的字节数目
	for (; it != info.vImageIndex.end(); ++it)
	{
		int idx = (*it);
		if (idx > 0 && idx < m_vImagePosInfo.size())
		{
			dwBytes += m_vImagePosInfo[idx].datalen;  // 累计图片文件的字节数目
			if (downloadedBytes >= dwBytes)  // idx文件已经下载完成
			{
				if (!m_vImagePosInfo[idx].bDownloaded)
				{
					m_vImagePosInfo[idx].bDownloaded = true;
					vDownloadedImgIdx.push_back(idx);
				}
				continue;
			}
			else
			{
				break;
			}
		}
		
	}
}

// 读取N3D场景数据
// pData : in 接收数据的buffer指针
// len   : in 接收数据的buffer长度，必须大于等于GetN3DSceneZippedLength
// return: 是否接收到数据。如果正常接收到数据，返回true。
bool CVGSN3DFileDownloadStreamTask::ReadN3DSceneData(BYTE* pData, DWORD len)
{
	bool re = false;
	if (m_bSceneDownloaded && pData && len >= m_N3DHeader.SceneZipDataLen)
	{
		re = ReadDataFromLocalFile(pData, m_N3DHeader.SceneDataPtr, m_N3DHeader.SceneZipDataLen);

		//HANDLE hFile = CreateFile(m_sSaveFile.c_str(),
		//	GENERIC_READ,
		//	FILE_SHARE_READ | FILE_SHARE_WRITE,
		//	NULL,
		//	OPEN_ALWAYS,
		//	FILE_ATTRIBUTE_NORMAL,
		//	NULL);
		//if (INVALID_HANDLE_VALUE == hFile)
		//{
		//	DWORD dwError = 0;
		//	dwError = GetLastError();
		//	return re;
		//}

		//// Try to read some data to buffer
		//SetFilePointer(hFile, m_N3DHeader.SceneDataPtr, 0, FILE_BEGIN);
		//DWORD dwReadBytes = 0;
		//ReadFile(hFile, pData, m_N3DHeader.SceneZipDataLen, &dwReadBytes, NULL);
		//re = (dwReadBytes == m_N3DHeader.SceneZipDataLen);

		//// Here, add your code to check the data and use it.

		//// After read file data to buffer, close it
		//CloseHandle(hFile);
		//hFile = NULL;

		return re;
	}
	return re;
}

// get n3d header
bool CVGSN3DFileDownloadStreamTask::GetN3DHeader(N3D_Header & header)
{
	memcpy(&header, &m_N3DHeader, sizeof(N3D_Header));
	return true;
}

// 获取一张图片的数据长度
DWORD CVGSN3DFileDownloadStreamTask::GetImageDataLength(int imgIdx)
{
	if (imgIdx >= 0 && imgIdx < m_vImagePosInfo.size())
	{
		return m_vImagePosInfo[imgIdx].datalen;
	}
	return 0;
}

// 读取一张图片的数据
// pData : in 接收数据的buffer指针
// len   : in 接收数据的buffer长度，必须大于等于GetImageDataLength
// return: 是否接收到数据。如果正常接收到数据，返回true。
bool CVGSN3DFileDownloadStreamTask::ReadnN3DOneImage(BYTE *pData, DWORD len, int imgIdx)
{
	bool re = false;
	if (imgIdx >= 0 && imgIdx < m_vImagePosInfo.size())
	{
		return ReadDataFromLocalFile(pData, m_vImagePosInfo[imgIdx].startPos, m_vImagePosInfo[imgIdx].datalen);
	}
	return re;
}

// 从下载到的文件中读取一段数据
// pOutBuffer的大小应该不小于len
bool CVGSN3DFileDownloadStreamTask::ReadDataFromLocalFile(BYTE* pOutBuffer, DWORD startPos, DWORD len)
{
	if (!pOutBuffer || len <= 0)
	{
		return false;
	}
	
	HANDLE hFile = CreateFile(m_sSaveTmpFile.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		DWORD dwError = 0;
		dwError = GetLastError();
		return false;
	}

	// Try to read some data to buffer
	SetFilePointer(hFile, startPos, 0, FILE_BEGIN);
	DWORD dwReadBytes = 0;
	ReadFile(hFile, pOutBuffer, len, &dwReadBytes, NULL);
	bool re = (dwReadBytes == len);

	// Here, add your code to check the data and use it.

	// After read file data to buffer, close it
	CloseHandle(hFile);
	hFile = NULL;

	return re;
}

