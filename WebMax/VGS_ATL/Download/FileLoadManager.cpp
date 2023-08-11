//-------------------------------------------------------------------
#include "stdafx.h"
#include "FileLoadManager.h"
#include "..\\VGS2Window.h"
//-------------------------------------------------------------------
DWORD WINAPI ThreadFunc(LPVOID lpParameter)
{

	CFileLoadManager *pIVGS3DLL = (CFileLoadManager*)lpParameter;
	if (pIVGS3DLL)
	{
		pIVGS3DLL->ThreadFunc();
	}
	return true;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
CFileLoadManager::CFileLoadManager()
{
	m_RequestID = 0;

	InitializeCriticalSection(&m_ResQue);
}
//-------------------------------------------------------------------
CFileLoadManager::~CFileLoadManager()
{
	m_RequestID = 0;

	DeleteCriticalSection(&m_ResQue);
}
//-------------------------------------------------------------------
void CFileLoadManager::Release()
{
	StopThread();
	ClearAllRequests();
}
//-------------------------------------------------------------------
// 初始化
//-------------------------------------------------------------------
void CFileLoadManager::Init()
{
	ClearAllRequests();

	// 创建线程
	CreateLoadThread();
}
//-------------------------------------------------------------------
// 创建加载/卸载线程
//-------------------------------------------------------------------
bool CFileLoadManager::CreateLoadThread()
{
	m_bTreadTerminated = false;	

	// 创建一个加载线程
	DWORD dwThreadId = 0;
	m_ThreadHandle = CreateThread( NULL, 0, ::ThreadFunc, LPVOID(this), 0, &dwThreadId);
	if (!m_ThreadHandle)
	{
		return false;
	}	

	return true;
}
//-------------------------------------------------------------------
// 停止线程
//-------------------------------------------------------------------
void CFileLoadManager::StopThread()
{
	m_bTreadTerminated = true;
}
//-------------------------------------------------------------------
// 加载/卸载线程过程
//-------------------------------------------------------------------
void CFileLoadManager::ThreadFunc() 
{
	while (!m_bTreadTerminated)
	{
		//防止线程太耗资源
		Sleep(10);
		if (m_vRequestQueue.empty())
			continue;
		else
			ProcessNextRequest();
	} 
}
//-------------------------------------------------------------------
// 处理一条请求
//-------------------------------------------------------------------
void CFileLoadManager::ProcessNextRequest()
{
	EnterCriticalSection(&m_ResQue);

	FileLoadRequest &req = m_vRequestQueue.front(); // 处理第一个请求
	int size1 = m_vRequestQueue.size();

	switch (req.type)
	{
	case VGS_FILELOADREQUEST_TYPE_LOADN3D: // 加载场景s3d
		{
			CVGS2Window* pVGSWnd = (CVGS2Window*)(req.pProcessor);
			if (pVGSWnd)
			{
				pVGSWnd->ReadLocalN3DFile((CFileDwonloadInfo*)req.pDataInfo);
			}
			break;
		}
	case VGS_FILELOADREQUEST_TYPE_LOADA3D: // 加载场景s3d
		{
			CVGS2Window* pVGSWnd = (CVGS2Window*)(req.pProcessor);
			if (pVGSWnd)
			{
				pVGSWnd->ReadLocalA3DFile((CFileDwonloadInfo*)req.pDataInfo);
			}
			break;
		}
	//	/*case VGSRT_LOAD_TEXTURE: // 加载贴图
	//	{
	//	SetMtrlTexture(req.sMtrlName, req.sTextureName, req.mapChannel);
	//	::PostMessage(req.hwnd, WM_NOTIFY_SCENELOADER, WPARAM(SLM_LOADTEXTURE), LPARAM(req.ParamHandle));
	//	break;
	//	}*/
	//case VGSRT_CREATE_MYPLAYER: // 创建自身玩家
	//	{
	//		if (true == CreateMyPlayer((PlayerDownloader*)req.ParamHandle))
	//		{
	//			::PostMessage(req.hwnd, WM_NOTIFY_SCENELOADER, WPARAM(SLM_CREATE_MYPLAYER_SUCCESS), LPARAM(req.ParamHandle));
	//			break;
	//		}
	//		else
	//		{
	//			::PostMessage(req.hwnd, WM_NOTIFY_SCENELOADER, WPARAM(SLM_CREATE_MYPLAYER_FAILED), LPARAM(req.ParamHandle));
	//			break;
	//		}
	//	}
	//case VGSRT_CREATE_OTHERPLAYER: // 创建其他玩家
	//	{
	//		if (true == CreateOtherPlayer((PlayerDownloader*)(req.ParamHandle), 300))
	//		{
	//			::PostMessage(req.hwnd, WM_NOTIFY_SCENELOADER, WPARAM(SLM_CREATE_OTHERPLAYER_SUCCESS), LPARAM(req.ParamHandle));
	//			break;
	//		}
	//		else
	//		{
	//			::PostMessage(req.hwnd, WM_NOTIFY_SCENELOADER, WPARAM(SLM_CREATE_OTHERPLAYER_FAILED), LPARAM(req.ParamHandle));
	//			break;
	//		}
	//	}
	//	/*
	//	case VGSRT_UNLOAD_RESOURCE: // 卸载资源
	//	{
	//	DestroyS3DScene((char*)req.sResourceGroup.c_str());
	//	::PostMessage(req.hwnd, WM_NOTIFY_SCENELOADER, WPARAM(SLM_UNLOADRESOURCE), LPARAM(req.ParamHandle));
	//	break;
	//	}
	//	*/
	//case VGSRT_SHUTDOWN: // 关闭线程
	//	{
	//		StopThread();
	//		::PostMessage(req.hwnd, WM_NOTIFY_SCENELOADER, WPARAM(SLM_SHUTDOWN), LPARAM(req.ParamHandle));
	//		break;
	//	}
	//default:
	//	break;
	}

	m_vRequestQueue.pop_front(); // 删除第一个请求

	LeaveCriticalSection(&m_ResQue);
}
//-------------------------------------------------------------------
// 添加一个请求
//-------------------------------------------------------------------
UINT CFileLoadManager::AddRequest(FileLoadRequest &req) 
{
	EnterCriticalSection(&m_ResQue);

	req.ReqID = m_RequestID++;
	m_vRequestQueue.push_back(req);

	LeaveCriticalSection(&m_ResQue);
	return req.ReqID;
}
//-------------------------------------------------------------------
// 删除所有请求
//-------------------------------------------------------------------
void CFileLoadManager::ClearAllRequests()
{
	m_vRequestQueue.clear();
}
//-------------------------------------------------------------------