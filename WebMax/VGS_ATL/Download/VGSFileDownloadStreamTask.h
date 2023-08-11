#pragma once

#include "..\VGSATLHeader.h"
#include "VGSFileDownloadTaskIF.h"
#include "VGSFileDownloadManager.h"
#include <set>

using namespace std;


// 流式下载线程信息
typedef class CStreamThreadInfo
{
public:
	long  lThreadIdx;   // 线程索引
	DWORD dwStartPos;   // 下载起始位置
	DWORD dwDataLen;    // 要下载的数据长度
	DWORD dwFinished;   // 已经完成的字节数目
	DWORD dwCurSpeed;   // 当前的下载速度
	DWORD dwMoreInfo;   // 留作保存其他信息

	CStreamThreadInfo()
	{
		lThreadIdx  = 0;
		dwStartPos  = 0;
		dwDataLen   = 0;
		dwFinished  = 0;
		dwCurSpeed  = 0;
		dwMoreInfo  = 0;
	}

	friend bool operator < (const CStreamThreadInfo& lv, const CStreamThreadInfo &rv)
	{
		return lv.lThreadIdx < rv.lThreadIdx;
	}


}CStreamThreadInfo;

class CVGSFileDownloadStreamTask : public IVGSFileDownloadTask
{
public:
	DWORD Init(const string& sName, const string &sURL, const string &sSaveFile, const string &sUsrName, const string &passward);
	DWORD GetFileSize();
	void  Download(bool bForceDownload = false);
	void  EnableReportStatus(bool bEnable);
	DWORD Stop();
	DWORD Release();
	string GetName(){return m_sName;}

	// 需要由子类根据文件类型分配线程任务
	virtual void  CreateThread() = 0;          
	virtual void  OnPrecessDownloadProgress(LONG lThreadID, DWORD dwFinishedBytes, DWORD dwCurrentSpeed) = 0;
	virtual void  OnPrecessDownloadStatusChange(LONG lThreadID, VGS_DOWNLOADTASK_STATUS status) = 0;

	// bool    m_bScanOK;            // 文件是否已经扫描完毕

public:
	CVGSFileDownloadStreamTask(CVGSFileDownloadManager* pMgr, IHttpDownloadIF * pHttpDownloader);
	~CVGSFileDownloadStreamTask();

protected:

	int  m_port; // 端口号

	CVGSFileDownloadManager* m_pVGSDownloadManager;
	virtual void ScanFile() = 0;  // 根据文件格式扫描文件

	bool ParseURL();  //  解析URL

	set<CStreamThreadInfo> m_vStreamThreadInfo;  // 下载线程信息 
private:
};