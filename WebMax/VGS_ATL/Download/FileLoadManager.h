#pragma once

#include "..\\..\\VGSCoreV3\\VGSHeader.h"
#include <list>

using namespace std;

// 请求类型
enum VGS_FILELOADREQUEST_TYPE
{
	VGS_FILELOADREQUEST_TYPE_LOADN3D = 0,
	VGS_FILELOADREQUEST_TYPE_LOADA3D,
	VGS_FILELOADREQUEST_TYPE_LOADFLASH2D,
	VGS_FILELOADREQUEST_TYPE_LOADFLASHTEXTURE,
	VGS_FILELOADREQUEST_TYPE_LOADIMAGETEXTURE,
	VGS_FILELOADREQUEST_TYPE_NONE
};

// 请求数据结构
struct FileLoadRequest
{
	UINT ReqID;   
	VGS_FILELOADREQUEST_TYPE type;
	//string sFileName;
	//HWND hwnd;        // 接收消息的窗口
	void* pProcessor;   // 处理器指针
	void* pDataInfo;    // 要处理的数据指针
};

class CFileLoadManager
{
private:	

	std::list <FileLoadRequest> m_vRequestQueue; // 任务队列

	bool m_bTreadTerminated;
	UINT m_RequestID;
	HANDLE m_ThreadHandle; // 线程对象

	bool CreateLoadThread();  // 创建加载/卸载线程
	void ProcessNextRequest();

	CRITICAL_SECTION			m_ResQue;	 // 线程同步
public:

	CFileLoadManager();
	~CFileLoadManager();
	void Release();	
	void Init();   // 初始化
	UINT AddRequest(FileLoadRequest &req); // 添加一个请求
	void StopThread(); 
	void ThreadFunc(); // 加载/卸载线程过程// 删除所有请求
	void ClearAllRequests();
};