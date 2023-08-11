#pragma once
//-------------------------------------------------------------------
#include <vector>
#include "..\\VGSTypedef.h"
#include "..\\VGSATLHeader.h"
#include "..\\VGSGetCore.h"
#include "VGSFileDownloadTaskIF.h"
#include "VGSFileDownloadTask.h"
#include "HTTPDownloadIF.h"
#include "HTTPDownloadLib.h"
//-------------------------------------------------------------------
using namespace std;
//-------------------------------------------------------------------
// 自定义下载消息
//-------------------------------------------------------------------
enum VGS_DOWNLOAD_MSG
{
	VGS_DOWNLOAD_MSG_STATUSCHANGE = WM_USER + 200,     // status change
	VGS_DOWNLOAD_MSG_DOWNLOADPROGRESS,     // download progress

	VGS_DOWNLOAD_EVENT_FILE_DOWNLOADED,        // 一个文件下载完成
	VGS_DOWNLOAD_EVENT_N3DONEIMG_DOWNLOADED,   // n3d文件中的一个图像数据段下载完成
	VGS_DOWNLOAD_EVENT_N3DSCENE_DOWNLOADED,    // n3d文件中的场景数据下载完成
};
//-------------------------------------------------------------------
//enum VGS_DOWNLOAD_EVENT
//{
//	VGS_DOWNLOAD_EVENT_FILE_DOWNLOADED = 0,   // 一个文件下载完成
//	VGS_DOWNLOAD_EVENT_N3DONEIMG_DOWNLOADED,  // n3d文件中的一个图像数据段下载完成
//	VGS_DOWNLOAD_EVENT_N3DSCENE_DOWNLOADED,    // n3d文件中的场景数据下载完成
//	VGS_DOWNLOAD_EVENT_DWONLOADPROGRESS        // 下载的进度
//};
//-------------------------------------------------------------------
//用于发送给主窗口消息时附带的参数
//-------------------------------------------------------------------
//enum 
//{	
//	DOWN_TYPE = 0,					//下载数据的类型，前台0和后台1下载
//	HEADER_ISDOWN,				//Header信息下载是否完成
//	SCENE_ISDOWN,				//场景信息下载是否完成
//	ONETEX_ISDOWN,				//一个贴图信息下载是否完成
//	ALLTEX_ISDOWN,				//所有贴图信息下载是否完成
//	ONEIMAGE_ISDOWN,			//一个Image信息下载是否完成
//	ALLIMAGE_ISDOWN,			//所有Image信息下载是否完成
//	ALL_ISDOWN,					//所有N3D信息下载是否完成
//	IS_DOWN,					//正在下载数据，显示下载进度
//	TERMINATED					//下载线程关闭
//};
//-------------------------------------------------------------------
//// 下载状态
//-------------------------------------------------------------------
//enum VGS_DOWNLOAD_STATUS
//{	
//	VGS_DOWNLOAD_STATUS_NONE = 0,
//	VGS_DOWNLOAD_STATUS_VERSION,            // 正在获取文件版本号
//	VGS_DOWNLOAD_STATUS_HEADER,				// 正在下载文件头
//	VGS_DOWNLOAD_STATUS_SCENE,				// 正在下载场景
//	VGS_DOWNLOAD_STATUS_IMAGE,			    // 正在下载图像
//	VGS_DOWNLOAD_STATUS_ALLISDOWN,			// 所有N3D信息下载完成
//	VGS_DOWNLOAD_STATUS_TERMINATED			// 下载线程关闭
//};
//-------------------------------------------------------------------
// 文件下载属性
//-------------------------------------------------------------------
class CFileDwonloadInfo
{
public:
	std::string			sName;
	std::string			sLocalFile;
	VGS_N3DLOAD_TYPE	loadType;			// 直接加载还是后台加载
	DWORD				dwTotalBytes;       // 文件总大小
	DWORD				dwFinishedBytes;    // 已经下载完的字节数
	bool				bDisplayProgress;   // 是否显示下载进度
	//
	CFileDwonloadInfo()
	{
		sName = "";
		sLocalFile = "";
		loadType = VGS_N3DLOAD_TYPE_LOAD;
		dwTotalBytes = 0;
		dwFinishedBytes = 0;
		bDisplayProgress = false;
	}
	virtual ~CFileDwonloadInfo()
	{
	}
};

//-------------------------------------------------------------------
// 文件的使用类型及其对象指针
//-------------------------------------------------------------------
class CUsageAndObject
{
public:
	VGS_FILE_USAGE              usage;
	CFileDwonloadInfo*          ptr;
	CUsageAndObject()
	{
		usage = VGS_FILE_USAGE_N3D;
		ptr = 0;
	}
	CUsageAndObject(VGS_FILE_USAGE Ausage, CFileDwonloadInfo* Aptr)
	{
		usage = Ausage;
		ptr = Aptr;
	}
};
//-------------------------------------------------------------------
// 下载任务包含的信息
//-------------------------------------------------------------------
typedef struct VGSFileDownloadTaskInfo
{
	IVGSFileDownloadTask* pTask;
	std::string sURL;
	std::string sName;
	std::string sLocalFile;
	bool        bDownloaded;
	vector<CUsageAndObject> vUsageAndPtr;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// 文件下载管理器
//-------------------------------------------------------------------
class CVGSFileDownloadManager  : public CVGSGetCore 
{
public:
	CVGSFileDownloadManager();
	~CVGSFileDownloadManager();
	//---------------------------------------------------------------
public:
	void SetMsgWndHandle(HWND hWnd) {m_hMsgWnd = hWnd;}
	void SetSourcePath(const std::string &sPath) {m_sCurrentSourcePath = sPath;}
	VGSFileDownloadTaskInfo* CreateVGSFileDownloadTask(bool bStreamMode, const string &sName, const string &sURL, VGS_DOWNLOADFILE_TYPE type, VGS_FILE_USAGE usage, CFileDwonloadInfo* pObject);  // create a download task with specific mode
    VGSFileDownloadTaskInfo* FindTask(const string &sURL);
	long RemoveTask(IVGSFileDownloadTask *pTask, bool bEraseFile);
	void SetMaxDownloadingTask(int maxCount);  // set the maximum of the downloading task in parallel
	void ClearAllTask();  // remove all the download task
	//---------------------------------------------------------------
	// 传给下载器的回调函数，响应下载消息
	long OnHttpDownloadEvent(LONG lTaskID, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2);

	// 传给下载任务的回调函数，处理每个任务的消息
	long OnVGSDownloadTaskStatusChange(DWORD TaskPtr, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2);
	long OnVGSDownloadTaskProgress(DWORD TaskPtr, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2);
	//---------------------------------------------------------------
private:
	// 根据文件大小计算线程数目
	// int GetThreadCount(DWORD filesize);
	string GetTempFolder();
	// 处理下载线程的状态变换
	void OnHttpDownloadStatusChange(LONG lTaskID,  LONG lThreadID, DWORD dwNewStatus);
	// 处理下载进度
	void OnHttpDownloadProgress(LONG lTaskID,  LONG lThreadID, DWORD dwFinishedBytes, DWORD dwCurrentSpeed);
	// 下载进度消息处理
	void DownloadPercent(VGSFileDownloadTaskInfo* pInfo, DWORD dwFinishedBytes, DWORD dwCurrentSpeed);
	// 下载后的处理
	void DownloadFinished(VGSFileDownloadTaskInfo* pInfo);
	void DownloadStopped(VGSFileDownloadTaskInfo* pInfo);
	//---------------------------------------------------------------
private:
	IHttpDownloadIF*    m_pHttpDownloadIF;
	CRITICAL_SECTION	m_pTaskCS;
	string              m_savePath;				// the path for saving the downloaded file
	string              m_sCurrentSourcePath;	// 当前下载路径
	HWND                m_hMsgWnd;
	int					m_maxDownloadingCount;  // the maximum of the downloading task in parallel
	vector<VGSFileDownloadTaskInfo*> m_vpDownloadTaskInfo;
	//---------------------------------------------------------------
};
//-------------------------------------------------------------------