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
// ��ʼ��
//-------------------------------------------------------------------
void CFileLoadManager::Init()
{
	ClearAllRequests();

	// �����߳�
	CreateLoadThread();
}
//-------------------------------------------------------------------
// ��������/ж���߳�
//-------------------------------------------------------------------
bool CFileLoadManager::CreateLoadThread()
{
	m_bTreadTerminated = false;	

	// ����һ�������߳�
	DWORD dwThreadId = 0;
	m_ThreadHandle = CreateThread( NULL, 0, ::ThreadFunc, LPVOID(this), 0, &dwThreadId);
	if (!m_ThreadHandle)
	{
		return false;
	}	

	return true;
}
//-------------------------------------------------------------------
// ֹͣ�߳�
//-------------------------------------------------------------------
void CFileLoadManager::StopThread()
{
	m_bTreadTerminated = true;
}
//-------------------------------------------------------------------
// ����/ж���̹߳���
//-------------------------------------------------------------------
void CFileLoadManager::ThreadFunc() 
{
	while (!m_bTreadTerminated)
	{
		//��ֹ�߳�̫����Դ
		Sleep(10);
		if (m_vRequestQueue.empty())
			continue;
		else
			ProcessNextRequest();
	} 
}
//-------------------------------------------------------------------
// ����һ������
//-------------------------------------------------------------------
void CFileLoadManager::ProcessNextRequest()
{
	EnterCriticalSection(&m_ResQue);

	FileLoadRequest &req = m_vRequestQueue.front(); // �����һ������
	int size1 = m_vRequestQueue.size();

	switch (req.type)
	{
	case VGS_FILELOADREQUEST_TYPE_LOADN3D: // ���س���s3d
		{
			CVGS2Window* pVGSWnd = (CVGS2Window*)(req.pProcessor);
			if (pVGSWnd)
			{
				pVGSWnd->ReadLocalN3DFile((CFileDwonloadInfo*)req.pDataInfo);
			}
			break;
		}
	case VGS_FILELOADREQUEST_TYPE_LOADA3D: // ���س���s3d
		{
			CVGS2Window* pVGSWnd = (CVGS2Window*)(req.pProcessor);
			if (pVGSWnd)
			{
				pVGSWnd->ReadLocalA3DFile((CFileDwonloadInfo*)req.pDataInfo);
			}
			break;
		}
	//	/*case VGSRT_LOAD_TEXTURE: // ������ͼ
	//	{
	//	SetMtrlTexture(req.sMtrlName, req.sTextureName, req.mapChannel);
	//	::PostMessage(req.hwnd, WM_NOTIFY_SCENELOADER, WPARAM(SLM_LOADTEXTURE), LPARAM(req.ParamHandle));
	//	break;
	//	}*/
	//case VGSRT_CREATE_MYPLAYER: // �����������
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
	//case VGSRT_CREATE_OTHERPLAYER: // �����������
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
	//	case VGSRT_UNLOAD_RESOURCE: // ж����Դ
	//	{
	//	DestroyS3DScene((char*)req.sResourceGroup.c_str());
	//	::PostMessage(req.hwnd, WM_NOTIFY_SCENELOADER, WPARAM(SLM_UNLOADRESOURCE), LPARAM(req.ParamHandle));
	//	break;
	//	}
	//	*/
	//case VGSRT_SHUTDOWN: // �ر��߳�
	//	{
	//		StopThread();
	//		::PostMessage(req.hwnd, WM_NOTIFY_SCENELOADER, WPARAM(SLM_SHUTDOWN), LPARAM(req.ParamHandle));
	//		break;
	//	}
	//default:
	//	break;
	}

	m_vRequestQueue.pop_front(); // ɾ����һ������

	LeaveCriticalSection(&m_ResQue);
}
//-------------------------------------------------------------------
// ���һ������
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
// ɾ����������
//-------------------------------------------------------------------
void CFileLoadManager::ClearAllRequests()
{
	m_vRequestQueue.clear();
}
//-------------------------------------------------------------------