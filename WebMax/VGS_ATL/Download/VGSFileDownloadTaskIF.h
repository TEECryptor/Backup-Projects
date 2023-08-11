//-------------------------------------------------------------------
#pragma  once
//-------------------------------------------------------------------
#include "..\VGSATLHeader.h"
#include <string>
#include "HTTPDownloadIF.h"
//-------------------------------------------------------------------
using namespace std;
//-------------------------------------------------------------------
// download status
//-------------------------------------------------------------------
enum VGS_DOWNLOADTASK_STATUS
{
	VGS_DOWNLOADTASK_STATUS_NONE = 0,
	VGS_DOWNLOADTASK_STATUS_READY,
	VGS_DOWNLOADTASK_STATUS_WAITTING,
	VGS_DOWNLOADTASK_STATUS_DOWNLOADING,
	VGS_DOWNLOADTASK_STATUS_TERMINATED,
	VGS_DOWNLOADTASK_STATUS_COMPLETED
};
//-------------------------------------------------------------------
enum VGS_DOWNLOAD_MODE
{
	VGS_DOWNLOAD_MODE_NORMAL = 0, //  non-stream mode
	VGS_DOWNLOAD_MODE_STREAM      // stream mode
};
//-------------------------------------------------------------------
//  base class of download task class
//-------------------------------------------------------------------
class IVGSFileDownloadTask
{
public:
	virtual DWORD Init(const string& sName, const string &sURL, const string &sSaveFile = "", const string &sUserName = "", const string &passward = "") = 0;
	virtual DWORD GetFileSize() = 0;
	virtual void  Download(bool bForceDownload = false) = 0;
	virtual void  EnableReportStatus(bool bEnable) = 0;
	virtual DWORD Stop() = 0;
	virtual DWORD Release() = 0;
	virtual void  CreateThread() = 0;           // create thread info
	//---------------------------------------------------------------
public:
	IVGSFileDownloadTask(IHttpDownloadIF * pHttpDownloader)
	{
		m_sName = "";
		m_sURL = "";
		m_sSaveFile = "";
		m_sUserName = "";
		m_sPassward = "";
		m_sSaveTmpFile = "";

		m_Status = VGS_DOWNLOADTASK_STATUS_NONE;
		m_pHttpDownloader = pHttpDownloader;
	}
	//---------------------------------------------------------------
	virtual ~IVGSFileDownloadTask()
	{

	}
	//---------------------------------------------------------------
public:
	virtual VGS_DOWNLOAD_MODE GetDownloadMode()
	{
		return m_Mode;
	}
	//---------------------------------------------------------------
	virtual long GetTaskID()
	{
		return m_TaskID;
	}
	//---------------------------------------------------------------
	virtual long GetThreadCount()
	{
		return m_defaultThreadCnt;
	}
	//---------------------------------------------------------------
	virtual string GetDownloadedFile()
	{
		return m_sSaveFile;
	}
	//---------------------------------------------------------------
	virtual string GetName()
	{
		return m_sName;
	}
	//---------------------------------------------------------------
	virtual string GetURL()
	{
		return m_sURL;
	}
	//---------------------------------------------------------------
	virtual VGS_DOWNLOADTASK_STATUS GetStatus()
	{
		return m_Status;
	}
	//---------------------------------------------------------------
	virtual void SetStatus(VGS_DOWNLOADTASK_STATUS status)
	{
		m_Status = status;
	}
	//---------------------------------------------------------------
protected:	
	// 根据文件大小计算线程数目
	int GetThreadCount(DWORD filesize)
	{
		if (filesize > 5 * 1024 * 1024) // create 3 thread if file size great than 5M bytes.
		{
			return 3;
		}
		else if (filesize > 2 * 1024 * 1024) // create 2 thread if file size great than 2M bytes.
		{
			return 2;
		}
		else 
		{
			return 1;
		}
	}
	//---------------------------------------------------------------
protected:	
	VGS_DOWNLOAD_MODE m_Mode;			// stream or non-stream
	VGS_DOWNLOADTASK_STATUS m_Status;	// 当前的下载状态

	string m_sName;
	string m_sURL;
	string m_sSaveFile;
	string m_sUserName;
	string m_sPassward;

	string m_sSaveTmpFile;				// 临时保存的文件

	long  m_TaskID;						// 下载器为其分配的ID，大于0时有效
	long  m_defaultThreadCnt;

	IHttpDownloadIF * m_pHttpDownloader;
	//---------------------------------------------------------------
};
//-------------------------------------------------------------------