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
// �Զ���������Ϣ
//-------------------------------------------------------------------
enum VGS_DOWNLOAD_MSG
{
	VGS_DOWNLOAD_MSG_STATUSCHANGE = WM_USER + 200,     // status change
	VGS_DOWNLOAD_MSG_DOWNLOADPROGRESS,     // download progress

	VGS_DOWNLOAD_EVENT_FILE_DOWNLOADED,        // һ���ļ��������
	VGS_DOWNLOAD_EVENT_N3DONEIMG_DOWNLOADED,   // n3d�ļ��е�һ��ͼ�����ݶ��������
	VGS_DOWNLOAD_EVENT_N3DSCENE_DOWNLOADED,    // n3d�ļ��еĳ��������������
};
//-------------------------------------------------------------------
//enum VGS_DOWNLOAD_EVENT
//{
//	VGS_DOWNLOAD_EVENT_FILE_DOWNLOADED = 0,   // һ���ļ��������
//	VGS_DOWNLOAD_EVENT_N3DONEIMG_DOWNLOADED,  // n3d�ļ��е�һ��ͼ�����ݶ��������
//	VGS_DOWNLOAD_EVENT_N3DSCENE_DOWNLOADED,    // n3d�ļ��еĳ��������������
//	VGS_DOWNLOAD_EVENT_DWONLOADPROGRESS        // ���صĽ���
//};
//-------------------------------------------------------------------
//���ڷ��͸���������Ϣʱ�����Ĳ���
//-------------------------------------------------------------------
//enum 
//{	
//	DOWN_TYPE = 0,					//�������ݵ����ͣ�ǰ̨0�ͺ�̨1����
//	HEADER_ISDOWN,				//Header��Ϣ�����Ƿ����
//	SCENE_ISDOWN,				//������Ϣ�����Ƿ����
//	ONETEX_ISDOWN,				//һ����ͼ��Ϣ�����Ƿ����
//	ALLTEX_ISDOWN,				//������ͼ��Ϣ�����Ƿ����
//	ONEIMAGE_ISDOWN,			//һ��Image��Ϣ�����Ƿ����
//	ALLIMAGE_ISDOWN,			//����Image��Ϣ�����Ƿ����
//	ALL_ISDOWN,					//����N3D��Ϣ�����Ƿ����
//	IS_DOWN,					//�����������ݣ���ʾ���ؽ���
//	TERMINATED					//�����̹߳ر�
//};
//-------------------------------------------------------------------
//// ����״̬
//-------------------------------------------------------------------
//enum VGS_DOWNLOAD_STATUS
//{	
//	VGS_DOWNLOAD_STATUS_NONE = 0,
//	VGS_DOWNLOAD_STATUS_VERSION,            // ���ڻ�ȡ�ļ��汾��
//	VGS_DOWNLOAD_STATUS_HEADER,				// ���������ļ�ͷ
//	VGS_DOWNLOAD_STATUS_SCENE,				// �������س���
//	VGS_DOWNLOAD_STATUS_IMAGE,			    // ��������ͼ��
//	VGS_DOWNLOAD_STATUS_ALLISDOWN,			// ����N3D��Ϣ�������
//	VGS_DOWNLOAD_STATUS_TERMINATED			// �����̹߳ر�
//};
//-------------------------------------------------------------------
// �ļ���������
//-------------------------------------------------------------------
class CFileDwonloadInfo
{
public:
	std::string			sName;
	std::string			sLocalFile;
	VGS_N3DLOAD_TYPE	loadType;			// ֱ�Ӽ��ػ��Ǻ�̨����
	DWORD				dwTotalBytes;       // �ļ��ܴ�С
	DWORD				dwFinishedBytes;    // �Ѿ���������ֽ���
	bool				bDisplayProgress;   // �Ƿ���ʾ���ؽ���
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
// �ļ���ʹ�����ͼ������ָ��
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
// ���������������Ϣ
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
// �ļ����ع�����
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
	// �����������Ļص���������Ӧ������Ϣ
	long OnHttpDownloadEvent(LONG lTaskID, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2);

	// ������������Ļص�����������ÿ���������Ϣ
	long OnVGSDownloadTaskStatusChange(DWORD TaskPtr, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2);
	long OnVGSDownloadTaskProgress(DWORD TaskPtr, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2);
	//---------------------------------------------------------------
private:
	// �����ļ���С�����߳���Ŀ
	// int GetThreadCount(DWORD filesize);
	string GetTempFolder();
	// ���������̵߳�״̬�任
	void OnHttpDownloadStatusChange(LONG lTaskID,  LONG lThreadID, DWORD dwNewStatus);
	// �������ؽ���
	void OnHttpDownloadProgress(LONG lTaskID,  LONG lThreadID, DWORD dwFinishedBytes, DWORD dwCurrentSpeed);
	// ���ؽ�����Ϣ����
	void DownloadPercent(VGSFileDownloadTaskInfo* pInfo, DWORD dwFinishedBytes, DWORD dwCurrentSpeed);
	// ���غ�Ĵ���
	void DownloadFinished(VGSFileDownloadTaskInfo* pInfo);
	void DownloadStopped(VGSFileDownloadTaskInfo* pInfo);
	//---------------------------------------------------------------
private:
	IHttpDownloadIF*    m_pHttpDownloadIF;
	CRITICAL_SECTION	m_pTaskCS;
	string              m_savePath;				// the path for saving the downloaded file
	string              m_sCurrentSourcePath;	// ��ǰ����·��
	HWND                m_hMsgWnd;
	int					m_maxDownloadingCount;  // the maximum of the downloading task in parallel
	vector<VGSFileDownloadTaskInfo*> m_vpDownloadTaskInfo;
	//---------------------------------------------------------------
};
//-------------------------------------------------------------------