// Root.cpp: implementation of the CRoot class.
//
//////////////////////////////////////////////////////////////////////

#include "Root.h"
#include "VGSRender.h"
#include "SwapChain.h"
#include ".\Animation\AnimationManager.h"
#include "MaterialManager.h"
#include "TextureManager.h"
#include "VGSCore.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//template <> CRoot* Singleton<CRoot>::ms_Singleton = 0;

//CRoot* CRoot::getSingletonPtr()
//{
//	return ms_Singleton;
//}
//CRoot& CRoot::getSingleton()
//{
//	assert(ms_Singleton);  
//	return (*ms_Singleton);
//}

CRoot::CRoot()
{

	m_pVGSCore = 0;

	//new CAnimationManager();       // 动画管理器
	//new CMaterialManager();
	//new CTextureManager();

	m_LastFrameTime = GetTickCount();  // 上一帧的时间

	m_bMouseControlEnable = TRUE;
	m_bKeyboardControlEnable = TRUE;

	m_bReleased = false;

	//m_AnimMgr.SetRoot(this);
	m_MaterialMgr.SetRoot(this);
	m_TextureMgr.SetRoot(this);

// 	m_ptrGfxMgr = NULL;

// 	if( 0 )
// 	{
// 		m_ptrGfxMgr = new GfxManager();
// 		m_ptrGfxMgr->SetRoot(this);
// 	}
}

CRoot::~CRoot()
{
	Release();
	//
	//CAnimationManager *pAnimMgr    = CAnimationManager::getSingletonPtr();
	//SAFE_DELETE(pAnimMgr);	
	//CTextureManager *pTextureMgr   = CTextureManager::getSingletonPtr();
	//SAFE_DELETE(pTextureMgr);	
	//CMaterialManager *pMaterialMgr = CMaterialManager::getSingletonPtr();
	//SAFE_DELETE(pMaterialMgr);	

	//for (INT i = 0; i < m_vRenderWnd.size(); ++i)
	//{
	//	SAFE_DELETE(m_vRenderWnd[i]);
	//}

	//m_vRenderWnd.clear();
}
VOID CRoot::SetVGSCore(CVGSCore* pCore)
{
	m_pVGSCore = pCore;
	assert(pCore);
	m_TextureMgr.setVGSRender(&getVGSRender());
	
// 	if( m_ptrGfxMgr )
// 	{
// 		m_ptrGfxMgr->setVGSRender(&getVGSRender());
// 	}

}

CVGSRender& CRoot::getVGSRender() 
{ 
	return m_pVGSCore->g_VGSRender;
}

// release
VOID CRoot::Release()
{
// 	SAFE_DELETE(m_ptrGfxMgr);
	m_bReleased = true;

	m_pVGSCore = 0; 

	m_AnimMgr.Release();
	m_MaterialMgr.Release();
	m_TextureMgr.Release();	

	for (UINT iWnd = 0; iWnd < m_vRenderWnd.size(); ++iWnd)
	{
		if (m_vRenderWnd[iWnd])
		{
			m_vRenderWnd[iWnd]->DestroyAllViewport();
			m_vRenderWnd[iWnd]->Release();
		}
	}

}
// 渲染一帧图像
VOID CRoot::RednerOneFrame()  
{
	if (m_bReleased)
	{
		return;
	}

	DWORD curTime = GetTickCount();  // 当前时间

	DWORD span = curTime - m_LastFrameTime;  // 两帧之间的时间差，单位是毫秒
	m_LastFrameTime = curTime;

	// FrameEvent evt;
	m_Event.timeSinceLastFrame = span / 1000.0;  // 转换成秒

	vector <CFrameListener*>::iterator it = m_vFrameListener.begin();
	for ( ; it != m_vFrameListener.end(); ++it) 
	{
		//FrameEvent evt;
		//evt.timeSinceLastFrame = span / 1000.0;  // 转换成秒
		(*it)->frameStarted(m_Event);
	}

	for (UINT i = 0; i < m_vRenderWnd.size(); ++i)
	{
		CRenderWindow *pRenderWnd = m_vRenderWnd[i];
		if (pRenderWnd)
		{
			pRenderWnd->Update();
		}
	}

	it = m_vFrameListener.begin();
	for ( ; it != m_vFrameListener.end(); ++it)
	{
		//FrameEvent evt;
		//evt.timeSinceLastFrame = span / 1000.0;  // 转换成秒
		(*it)->frameEnded(m_Event);
	}
}

// 创建渲染窗口
CRenderWindow* CRoot::CreateRenderWindow(HWND hExternalWnd, CHAR* Name/* = NULL*/)  
{
	CSwapChain *pRenderWnd = new CSwapChain(/*g_VGSRender*/getVGSRender().m_pd3d, /*g_VGSRender*/getVGSRender().m_pd3dDevice, hExternalWnd);
	pRenderWnd->m_MultiSampleType = /*g_VGSRender*/getVGSRender().d3dParam.MultiSampleType;
	pRenderWnd->Create();
	pRenderWnd->setName(std::string(Name));
	m_vRenderWnd.push_back(pRenderWnd);

	return pRenderWnd;
}

// 销毁一个renderwindow
VOID CRoot::RemoveRenderWindow(CRenderWindow* pRenderWnd)
{
	if (pRenderWnd)
	{
		for (UINT i = 0; i < m_vRenderWnd.size(); ++i)
		{
			if (pRenderWnd == m_vRenderWnd[i])
			{
				SAFE_DELETE(pRenderWnd);
				m_vRenderWnd.erase(m_vRenderWnd.begin() + i);
				break;
			}
		}
	}
}

VOID CRoot::RemoveAllRenderWindow()
{
	for (UINT i = 0; i < m_vRenderWnd.size(); ++i)
	{
		if (m_vRenderWnd[i])
		{
			SAFE_RELEASE(m_vRenderWnd[i]);
			SAFE_DELETE(m_vRenderWnd[i]);
		}
	}
	m_vRenderWnd.clear();
}

UINT CRoot::GetRenderWindowCount()
{
	return m_vRenderWnd.size();
}

CRenderWindow* CRoot::GetRenderWindow(int index)
{
	if (index >= 0 && index < m_vRenderWnd.size())
	{
		return m_vRenderWnd[index];
	}
	return 0;
}

// 添加一个帧监听器
VOID CRoot::addFrameListener(CFrameListener* pListener)
{
	m_vFrameListener.push_back(pListener);
}

// 删除一个帧监听器
VOID CRoot::removeFrameListener(CFrameListener* pListener)
{
	for (UINT i = 0; i < m_vFrameListener.size(); ++i)
	{
		if (m_vFrameListener[i] == pListener)
		{
			m_vFrameListener.erase(m_vFrameListener.begin() + i);
			break;
		}
	}
}

VOID CRoot::setMouseControlEnable(BOOL bEnable)
{
	m_bMouseControlEnable = bEnable;
}
BOOL CRoot::getMouseControlEnable()
{
	return m_bMouseControlEnable;
}
VOID CRoot::setKeyboardControlEnable(BOOL bEnable)
{
	m_bKeyboardControlEnable = bEnable;
}
BOOL CRoot::getKeyboardControlEnable()
{
	return m_bKeyboardControlEnable;	
}

/** 消息处理
@param sceneHandle in : 场景句柄
@param DWORD in : 渲染窗口句柄。设为NULL，表示对所有渲染窗口。
@param uMsg in : 消息ID，可参考windows标准消息说明
@param wParam/lParam  in : 消息参数，可参考windows标准消息说明
*/
void CRoot::ProcessMessage(DWORD sceneHandle, DWORD hRenderWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_bReleased)
	{
		return;
	}

	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		{
			CScene *pScene = (CScene*)sceneHandle;
			if (pScene && m_bMouseControlEnable)
			{
				IController *pController = pScene->GetCurrentController();
				if (pController)
				{
					VGSMouseState mouseInfo;
					mouseInfo.x = LOWORD(lParam);
					mouseInfo.y = HIWORD(lParam);
					mouseInfo.extentedKey = getExtenedKeyType(wParam);
					mouseInfo.mouseType   = getMouseType(wParam);
					mouseInfo.hRenderWnd = hRenderWnd;
					pController->SetMouseState(mouseInfo);
					pController->SetFrameState(m_Event);
					pController->Update();
				}
			}
			break;
		}
	case WM_KEYDOWN:
		{
			CScene *pScene = (CScene*)sceneHandle;
			if (pScene && m_bKeyboardControlEnable)
			{
				IController *pController = pScene->GetCurrentController();
				if (pController)
				{
					VGSKeyState keyInfo;
					keyInfo.key = (DWORD)wParam;
					keyInfo.hRenderWnd = hRenderWnd;
					pController->SetKeyboardState(keyInfo);
					pController->SetFrameState(m_Event);
					pController->Update();
				}
			}
			break;
		}
	case WM_SIZE:
		CScene *pScene = (CScene*)sceneHandle;
		if (pScene)
		{
			if (hRenderWnd == 0)
			{
				for (UINT i = 0; i < m_vRenderWnd.size(); ++i)
				{
					if (m_vRenderWnd[i]) 
					{
						((CSwapChain*)m_vRenderWnd[i])->UpdateBackbuffer();
					}
				}
			}
			else
			{
				CSwapChain *pRenderWnd = (CSwapChain*)hRenderWnd;
				if (pRenderWnd)
				{
					pRenderWnd->UpdateBackbuffer();
				}
			}
		}
		break;
	}
}