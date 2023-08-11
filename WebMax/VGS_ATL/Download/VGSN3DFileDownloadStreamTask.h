#pragma  once

#include "..\\VGSATLHeader.h"
#include "VGSFileDownloadStreamTask.h"
#include "..\\n3d\\N3D_Def.h"

#include <vector>
#include <wininet.h>

using namespace std;
using namespace N3D;

// 文件位置信息
struct CImagePosInfo
{
	DWORD startPos;  // 开始位置
	DWORD datalen;   // 数据长度
	bool  bDownloaded;
	CImagePosInfo()
	{
		startPos = 0;
		datalen  = 0;
		bDownloaded = false;
	}
};

// 线程和图像数据的对应关系
class CThreadImageInfo
{
public:
	long lThreadIdx;
	vector<int>  vImageIndex;
	vector<bool> vbDownloaded;
	CThreadImageInfo()
	{
		lThreadIdx = 0;
	}

	friend bool operator < (const CThreadImageInfo& lv, const CThreadImageInfo &rv)
	{
		return lv.lThreadIdx < rv.lThreadIdx;
	}
};

class CVGSN3DFileDownloadStreamTask : public CVGSFileDownloadStreamTask
{
public:
	void  CreateThread(); 
	void  OnPrecessDownloadProgress(LONG lThreadID, DWORD finishedBytes, DWORD currentSpeed);
	void  OnPrecessDownloadStatusChange(LONG lThreadID, VGS_DOWNLOADTASK_STATUS status);

	
public:
	CVGSN3DFileDownloadStreamTask(CVGSFileDownloadManager* pMgr, IHttpDownloadIF * pHttpDownloader);
	~CVGSN3DFileDownloadStreamTask();

	// 获得N3D场景数据压缩后的长度
	DWORD GetN3DSceneZippedLength(){return m_N3DHeader.SceneZipDataLen;}

	// 获得N3D场景数据解压后的长度
	DWORD GetN3DSceneUnZippedLength(){return m_N3DHeader.OriginSceneDataLen;}

	// 读取N3D场景数据
	// pData : in 接收数据的buffer指针
	// len   : in 接收数据的buffer长度，必须大于等于GetN3DSceneZippedLength
	// return: 是否接收到数据。如果正常接收到数据，返回true。
	bool ReadN3DSceneData(BYTE* pData, DWORD len);

	// 读取一张图片的数据
	// pData : in 接收数据的buffer指针
	// len   : in 接收数据的buffer长度，必须大于等于GetImageDataLength
	// return: 是否接收到数据。如果正常接收到数据，返回true。
	bool ReadnN3DOneImage(BYTE *pData, DWORD len, int imgIdx);

	// 获取一张图片的数据长度
	DWORD GetImageDataLength(int imgIdx);

	// get n3d header
	bool GetN3DHeader(N3D_Header & header);

	// n3d中图片文件的数目
	int GetImageCount(){return m_vImagePosInfo.size();}

protected:
	void ScanFile();
	void ScanFileEx();
	bool ParseURL(LPCTSTR lpURL, 
		LPTSTR lpHost,  
		LPTSTR lpPort, 
		LPTSTR lpNetFile);

	// 从下载到的文件中读取一段数据
	// pOutBuffer的大小应该不小于len
	bool ReadDataFromLocalFile(BYTE* pOutBuffer, DWORD startPos, DWORD len);

	bool m_bSceneDownloaded;  // whether scene data is downloaed

private:
	N3D_Header    m_N3DHeader;  // n3d header
	long          m_ThreadIdx;
	
	bool          m_bSceneCreated;         // is scene data created to the vgs Scene
	bool          m_bAllImageCreated;      // is all image files created to the vgs texture 

	vector<CImagePosInfo> m_vImagePosInfo;       // n3d中图片文件的列表
	set<CThreadImageInfo> m_setThreadImageInfo;  // 线程和图像的对应关系

	// 读一个贴图文件信息
	bool ReadOneImageInfo(CImagePosInfo& imagePos, HINTERNET hURL);


	// 场景是否下载完成
	bool IsSceneDwonloaded();

	// 插件哪些文件下载完成
	void CheckDownloaded(const CThreadImageInfo& info, DWORD downloadedBytes, vector<int> &vDownloadedImgIdx);
};