//----------------------------------------------------------------------------------
//VGSCore.cpp VGS渲染核心的实现文件
//描述：
//作者: fuxb
//日期: 2008 -3 -1
//-----------------------------------------------------------------------------------

#include "VGSHeader.h"

//#include "CWindow.h"
#include "VGSCore.h"

// #include "VGSRender.h"
#include "CScene.h" 

#include "CCamera.h"
#include "root.h"

//#include "lua/LuaEngine.h"
//#include "lua/luaplus/LuaState.h"
//using namespace LuaPlus;

// LensFlare的资源
// #include "Resource\\LS_SunImage.h"
// #include "Resource\\LS_Tex1Image.h"
// #include "Resource\\LS_Tex2Image.h"
// #include "Resource\\LS_Tex3Image.h"

#include "Viewport.h"

#include <algorithm>
#include <set> 
#include <string>
using namespace std; 

#include "VGSHeader.h"
#include "SwapChain.h"
#include ".\GUI\CGUIObject.h"

#include "Root.h"
#include ".\Event\VGSMainFrameListener.h"

#include "VGSErrCode.h"
#include "Animation/AnimationManager.h"


#include "MaterialManager.h"
#include "TextureManager.h"
#include "SwfTexture.H"
#include ".\GUI\ATLFlashCtrlUnit.h"

#include <sstream>

#include "VGSSupport.h"

#include "Plane.h"
#include "Ray.h"

# if defined _VGS_LIB{
#include "FileIO/N3DLoader.h"}
# else 
#   if defined _VGS_DLL{
#include "FileIO/SaveN3D.h"
#include "FileIO/IMportS3D.h"}
#	else
#		if defined _VGS_ATL{ 
#include "FileIO/N3DLoader.h"}
#		else
#       endif
#   endif
# endif

#pragma   warning(disable:4786)  // 暂时屏蔽掉warning

CVGSCore::CVGSCore() 
	: g_bSceneDirty(FALSE)
	,mInitialized(false)
{
	m_pRoot = new CRoot();
	m_pRoot->SetVGSCore(this);
	m_pLuaEngine = new LuaEngine(this);
	assert(m_pLuaEngine != NULL);
}

CVGSCore::~CVGSCore()
{
	SAFE_DELETE(m_pLuaEngine);
	SAFE_DELETE(m_pRoot);
}
//-----------------------------------------------------------------------------
// 方法名: CreateWindow()
// 参  数: 
// 用  途: 创建Windows标准窗体
//-----------------------------------------------------------------------------
//HWND CVGSCore::CreateVGSWindow(RECT* rect, HWND parentWnd)
//{
//	//----------------------------------------
//	//创建Windows标准窗体
//	//----------------------------------------
//	CVGSTestWindow* pWindow = new CVGSTestWindow();
//	HWND hWnd = pWindow->Create(rect, parentWnd);
//	g_vWindow.push_back(pWindow);
//	return hWnd;
//}

//-----------------------------------------------------------------------------
// 方法名: InitVGS()
// 参  数: 
// 用  途: 初始化VGS引擎，必须先调用该方法，才能调用CreateScene
// 创建成功返回TUR, 否则返回FALSE
//-----------------------------------------------------------------------------
BOOL CVGSCore::InitVGS(HWND hWnd, INT browserType/* = 0*/, BOOL bWritelog/* = FALSE*/)
{
	// new CRoot();

	//初始化VGS渲染器，建立direct3d对象，创建direct3d设备
	// g_pVGSRender = new CVGSRender();
	if (FAILED(g_VGSRender.Init(hWnd, (WEBBROWSERTYPE)browserType)))
	{
		g_VGSRender.Release();
		// SAFE_RELEASE(g_pVGSRender);
		// SAFE_DELETE(g_pVGSRender);
		MessageBox(GetActiveWindow(), TEXT("初始化VGS渲染器失败。"), TEXT("初始化VGS渲染器...失败"), MB_OK|MB_ICONERROR);
		return FALSE;
	}

	//初始化Com接口，用于Video和Audio的播放
	CoInitialize(NULL);

	//初始化线程同步对象
	InitializeCriticalSection(&m_render_cs);
	InitializeCriticalSection(&m_ctrl_cs);

	frameCnt = 0;

	// 创建主帧监听器
	if (m_pMainFrameListener)
	{
		m_pRoot->removeFrameListener(m_pMainFrameListener);
	}
	m_pMainFrameListener = new CMainFrameListener();
	m_pMainFrameListener->SetRoot(m_pRoot);
	m_pRoot->addFrameListener(m_pMainFrameListener);

	g_ResourcePath.clear();   // 清空搜索路径

// 	if( m_pRoot->getGfxMgrptr() )
// 	{
// 		m_pRoot->getGfxMgrptr()->initialise();
// 	}

	mInitialized = true;
	return TRUE;
}

//-----------------------------------------------------------------------------
// 方法名: InitVGS()
// 参  数: 
// 用  途: 退出VGS引擎
//-----------------------------------------------------------------------------
VOID CVGSCore::ExitVGS()
{
	EnterCriticalSection(&m_ctrl_cs);
	EnterCriticalSection(&m_render_cs);

	//执行lua脚本	2010-5-26 linxq
	m_pLuaEngine->OnExit();

	//清除所有的场景
	DeleteAllScene();

	g_ResourcePath.clear();   // 清空搜索路径

	// m_pRoot->Release();
	m_pRoot->Release();

	SAFE_DELETE(m_pMainFrameListener);

	//释放渲染器
	g_VGSRender.Release();

	LeaveCriticalSection(&m_render_cs);
	LeaveCriticalSection(&m_ctrl_cs);

	//释放Com接口
	CoUninitialize();
}

/** 设置浏览器类型
@param type INT : 0 IE, 1 ff, 2 google, 3 opera, 4 safari
*/
VOID CVGSCore::SetWebBrowserType(INT type)
{
	g_VGSRender.SetWebBrowserType((WEBBROWSERTYPE)type);
}

/** 获取浏览器类型
@return : 0 IE, 1 ff, 2 google, 3 opera, 4 safari
*/
INT  CVGSCore::GetWebBrowserType()
{
	return (INT)g_VGSRender.GetWebBrowserType();
}

/** 设置IE浏览器的子类型
@param type INT : 0 IE, 1 maxthon, 2 tencent
*/
VOID CVGSCore::SetWebBowserSubtype(INT subType)
{
	g_VGSRender.SetWebBowserSubtype((WEBBROWSER_SUBTYPE)subType);
}

/** 获取IE浏览器的子类型
@return : 0 IE, 1 maxthon, 2 tencent
*/
INT  CVGSCore::GetWebBowserSubtype()
{
	return (INT)g_VGSRender.GetWebBowserSubtype();
}

//---------------------------------------------------------
//设置抗拒齿的级别，
//当Level等于0，则没有抗拒齿，
//当Level等于-1，则中度抗拒齿（默认情形）
//当Level大于0，则根据不同的系统选择抗拒齿的参数
//---------------------------------------------------------
VOID CVGSCore::SetAntiAliasLevel(INT level)
{
	//选择级别
// 	INT antiAliasLevelCnt =  g_VGSRender.m_DeviceInfo->pAdapters[0].MultiSampleTypeCnt;
// 	if (level > antiAliasLevelCnt -1)
// 	{
// 		level = antiAliasLevelCnt - 1;
// 	}
// 	if (level < 0)
// 	{
// 		level = (antiAliasLevelCnt + 1) / 2 -1;
// 	}
// 
// 	g_VGSRender.m_antiLevel = level;
// 
// 	//AntiAlias type
// 	g_VGSRender.d3dParam.MultiSampleType =  g_VGSRender.m_DeviceInfo->pAdapters[0].pMultiSampleType[level];
// 
// 	UpdateBackBuffer();
// 
// 	INT sceneCnt = g_vScene.size();
// 	CScene** ppScene = g_vScene.begin();
// 	for (INT i = 0; i < sceneCnt; i++)
// 	{
// 		CScene* pScene = *(ppScene + i);
// 		pScene->antiLevel = level;
// 	}
}

//---------------------------------------------------------
//获得抗拒齿的级别，
//当Level等于0，则没有抗拒齿，
//当Level等于-1，则中度抗拒齿（默认情形）
//当Level大于0，则根据不同的系统选择抗拒齿的参数
//---------------------------------------------------------
INT	CVGSCore::GetAntiAliasLevel()
{
	return g_VGSRender.m_antiLevel;
}

//-----------------------------------------------------------------------------
// 方法名: CreateScene()
// 参  数: CHAR* name, HWND hWnd, RECT* rect
// 用  途: 创建VGS场景到指定窗体
//-----------------------------------------------------------------------------
DWORD CVGSCore::CreateScene(CHAR* name)
{
	EnterCriticalSection(&m_render_cs);

	//新建场景，场景创建到指定的窗体，渲染
	CScene* pScene = new CScene(g_VGSRender.m_pd3d, g_VGSRender.m_pd3dDevice);
	pScene->m_sName = std::string(name);
	pScene->SetRoot(m_pRoot);
	
	//加入分场景列表
	g_vScene.push_back(pScene);
	pScene->m_index = g_vScene.size() - 1;

// 	//执行vgs.lua脚本	2010-5-26 linxq
// 	if (m_pLuaEngine->DoFile("e:\\workspace\\webmax\\trunk\\lua\\vgs.lua"))
// 	{
// 		m_pLuaEngine->OnInitVgs();
// 	}
	//for test 2010-6-1
// 	if(m_pRoot->getGfxMgrptr()) 
// 	{
// 		m_pRoot->getGfxMgrptr()->setScene(pScene);
// 		m_pRoot->getGfxMgrptr()->setMediaDir(m_strN3dDir);
// 	}

// 	GfxMoviePtr movie = m_pRoot->getGfxMgrptr()->createMovie("e:\\mytest\\3d\\scaleform\\bin\\HudBottom.swf");
//   	m_ptrSwfView = m_pRoot->getGfxMgrptr()->createMovieView(movie,"HudBottom1");
//  	m_pRoot->getGfxMgrptr()->CreateRenderTexture(2048, 2048, true);
//   	m_pRoot->getGfxMgrptr()->createRTTMovieView("e:\\mytest\\3d\\scaleform\\bin\\3DWindow.swf");//test

	LeaveCriticalSection(&m_render_cs);
	return (DWORD)pScene;
}

//-----------------------------------------------------------------------------
// 方法名: DeleteScene()
// 参  数: DWORD sceneHandle
// 用  途: 清除VGS场景
//-----------------------------------------------------------------------------
VOID CVGSCore::DeleteScene(DWORD sceneHandle)
{
	EnterCriticalSection(&m_render_cs);
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		//从场景队列删除
		ResetSceneIndex();
		g_vScene.erase(g_vScene.begin() + pScene->m_index);

		g_bSceneDirty = TRUE;

		//释放场景资源
		SAFE_RELEASE(pScene);
		SAFE_DELETE(pScene);
	}
	LeaveCriticalSection(&m_render_cs);
}

//-----------------------------------------------------------------------------
// 方法名: ResetScene()
// 参  数: DWORD sceneHandle
// 用  途: Reset VGS场景
//-----------------------------------------------------------------------------
VOID CVGSCore::ResetScene(DWORD sceneHandle)
{
	EnterCriticalSection(&m_ctrl_cs);
	EnterCriticalSection(&m_render_cs);
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pScene->Reset();
	}
	LeaveCriticalSection(&m_render_cs);
	LeaveCriticalSection(&m_ctrl_cs);
}

//删除所有的场景
VOID CVGSCore::DeleteAllScene()
{
	EnterCriticalSection(&m_ctrl_cs);
	EnterCriticalSection(&m_render_cs);
	std::vector<CScene*>::iterator ppScene = g_vScene.begin();
	INT sceneCnt = g_vScene.size();
	for (INT i = 0; i < sceneCnt; i++)
	{
		CScene* pScene = *(ppScene + i);
		//释放场景资源
		SAFE_RELEASE(pScene);
		SAFE_DELETE(pScene);
	}
	g_vScene.clear();
	LeaveCriticalSection(&m_render_cs);
	LeaveCriticalSection(&m_ctrl_cs);
}

//-----------------------------------------------------------------------------
// 方法名: GetSceneCount()
// 参  数: 
// 用  途: 获得场景的数量
//-----------------------------------------------------------------------------
INT	CVGSCore::GetSceneCount()
{
	return g_vScene.size();
}

//-----------------------------------------------------------------------------
// 方法名: ResetSceneIndex()
// 参  数: 
// 用  途: 给场景重新编号, 只有在删除场景后才需要调用
//-----------------------------------------------------------------------------
VOID CVGSCore::ResetSceneIndex()
{
	if (g_bSceneDirty)
	{
		INT sceneCnt = g_vScene.size();
		std::vector<CScene*>::iterator ppScene = g_vScene.begin();
		for (INT i = 0; i< sceneCnt; i++)
		{
			CScene* pScene = *(ppScene + i);
			pScene->m_index = i;
		}
	}
}

//-----------------------------------------------------------------------------
// 方法名: GetSceneHandleByIndex()
// 参  数: INT index
// 用  途: 通过编号获得场景
//-----------------------------------------------------------------------------
DWORD CVGSCore::GetSceneHandleByIndex(INT index)
{
	INT sceneCnt = g_vScene.size();
	if (index < 0 || index >= sceneCnt)
	{
		return 0;
	}

	ResetSceneIndex();
	std::vector<CScene*>::iterator ppScene = g_vScene.begin();
	CScene* pScene = *(ppScene + index);
	return (DWORD)pScene;
}

//-----------------------------------------------------------------------------
// 方法名: GetSceneHandleByName()
// 参  数: CHAR* name
// 用  途: 通过名称获得场景
//-----------------------------------------------------------------------------
DWORD CVGSCore::GetSceneHandleByName(CHAR* name)
{
	INT sceneCnt = g_vScene.size();
	std::vector<CScene*>::iterator ppScene = g_vScene.begin();
	for (INT i = 0; i< sceneCnt; i++)
	{
		CScene* pScene = *(ppScene + i);
		if (strcmp(pScene->m_sName.c_str(), name) == 0)
		{
			return (DWORD)pScene;
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: GetSceneNameByHandle()
// 参  数: DWORD sceneHandle
// 用  途: 通过句柄获得场景的名称
//-----------------------------------------------------------------------------
VOID CVGSCore::GetSceneNameByHandle(DWORD sceneHandle, CHAR* name)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		// strcpy(name, pScene->m_sName.c_str());
		COPY_NAME(name, pScene->m_sName.c_str());
	}
}
/** 通过句柄索引号获取场景名称
@param in index 场景索引号
@param in/out name 场景名称，最长MAX_NAMESTR_LENGTH个字节 
@return 得到场景名称返回TRUE，否则FALSE
*/
BOOL CVGSCore::GetSceneNameByIndex(INT index, CHAR* name)
{
	ResetSceneIndex();
	INT sceneCnt = g_vScene.size();
	if (index >=0 && index < sceneCnt)
	{
		COPY_NAME(name, g_vScene[index]->m_sName.c_str());
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// 方法名: GetSceneIndexByHandle()
// 参  数: DWORD sceneHandle
// 用  途: 通过句柄获得场景的名称
//-----------------------------------------------------------------------------
INT CVGSCore::GetSceneIndexByHandle(DWORD sceneHandle)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		ResetSceneIndex();
		return pScene->m_index;
	}
	return -1;
}

////-----------------------------------------------------------------------------
//// 方法名: GetWndBySceneHandle()
//// 参  数: DWORD sceneHandle
//// 用  途: 通过场景的句柄获得窗体句柄
////-----------------------------------------------------------------------------
//HWND GetWndBySceneHandle(DWORD sceneHandle)
//{
//	CScene* pScene = (CScene*)sceneHandle;
//	if (pScene)
//	{
//		return pScene->m_hWnd;
//	}
//	return 0;
//}

//-----------------------------------------------------------------------------
// 方法名: GetSceneHandleByWnd()
// 参  数: HWND hwnd
// 用  途: 通过窗体句柄获得场景句柄
//-----------------------------------------------------------------------------
//DWORD CVGSCore::GetSceneHandleByWnd(HWND hWnd)
//{
//	INT sceneCnt = g_vScene.size();
//	CScene** ppScene = g_vScene.begin();
//	for (INT i = 0; i< sceneCnt; i++)
//	{
//		CScene* pScene = *(ppScene + i);
//		if (pScene->m_hWnd == hWnd)
//			return (DWORD)pScene;
//	}
//	return 0;
//}

//----------------------------------------------------------------------------
// 方法名: GetSceneEditAxisStatus(DWORD sceneHandle)
// 参  数: DWORD sceneHandle
// 用  途: 获得场景中坐标轴选择的状态
//-----------------------------------------------------------------------------
UINT CVGSCore::GetSceneEditAxisStatus(DWORD sceneHandle)
{
	/*
   	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		switch(pScene->m_ModelCtrlStatus)
		{
		case 0:
			return 0;
			break;
		case 1:
		case 2:
			return pScene->moveAxis->selectAxisStatus;
			break;
        case 3:
		case 4:
			return pScene->rotateAxis->selectAxisStatus;
			break;
		case 5:
			return pScene->scaleAxis->selectAxisStatus;
			break;
		}
	}*/
	return 0;
}

/////////////////////// 渲染窗口 //////////////////////////////////

/** 创建渲染窗体
@param sceneHandle in VGS场景
@param hExternalWnd in 要VGS渲染窗体所嵌入的窗体句柄
@param Name in 创建的渲染窗体名字
@return 如果创建成功，返回渲染窗体指针，否则返回0
@remarks 一个场景可以包含多个渲染窗体
*/
DWORD CVGSCore::CreateRenderWindow(DWORD sceneHandle, HWND hExternalWnd, CHAR* Name)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL)
	{
		return 0;
	}

	EnterCriticalSection(&m_render_cs);

	DWORD re = (DWORD)m_pRoot->CreateRenderWindow(hExternalWnd, Name);

	LeaveCriticalSection(&m_render_cs);
	return (DWORD)re;
}

/**删除指定的VGS场景
@param sceneHandle in VGS场景管理器指针
@param windowHandle in 要删除的VGS窗口句柄
*/
VOID CVGSCore::DeleteRenderWindow(DWORD sceneHandle, DWORD windowHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CSwapChain* sSwapChain = (CSwapChain*)windowHandle;

	EnterCriticalSection(&m_render_cs);
	m_pRoot->RemoveRenderWindow(sSwapChain);
	LeaveCriticalSection(&m_render_cs);

}

/** 创建渲染窗口内容的缩略图
@param sceneHandle  in : 场景指针
@param pRenderWnd   in : 渲染窗体指针
@param sFileName    in : 要保存的缩略图文件名称，包含路径和扩展名
*/
VOID CVGSCore::CreateRenderwindowThumb(DWORD sceneHandle, DWORD pRenderWnd, const CHAR *sFileName)
{
	MessageBox(NULL, TEXT("暂时没有实现"), TEXT("提示"), 0);
}

//设置渲染窗体的投影模式
//DLL_EXPORT VOID CVGSCore::SetRenderWindowProjMode(DWORD sceneHandle, DWORD swapChainHandle, INT projMode)
//{
//}
//获得渲染窗体的投影模式
//DLL_EXPORT INT GetRenderWindowProjMode(DWORD sceneHandle, DWORD swapChainHandle);

/** 设置场景的渲染窗口是否可以响应鼠标、键盘、手柄的控制
@param in sceneHandle : 场景指针
@param in RenderWndHandle : 渲染窗口指针
@param in WndHandle : 响应消息的窗体，通常是主窗体
*/
VOID CVGSCore::CreateWndControl(DWORD WndHandle, DWORD sceneHandle, DWORD RenderWndHandle)
{
	return; // 函数不需要了
}

/** 设置场景的控制状态
@param bEnable in TRUE:场景可通过鼠标键盘等控制，FALSE：不能控制
*/
// DLL_EXPORT VOID CVGSCore::SetWndControlStatus(BOOL bKeyboardEnable, BOOL bMouseEnable);
// DLL_EXPORT VOID CVGSCore::SetWndControlStatus(BOOL bEnable);

/** 设置场景是否可以键盘控制
*/
VOID CVGSCore::SetWndControlKeyboardStatus(BOOL bEnable)
{
	m_pRoot->setKeyboardControlEnable(bEnable);
}

/** 设置场景是否可以鼠标控制
*/
VOID CVGSCore::SetWndControlMouseStatus(BOOL bEnable)
{
	m_pRoot->setMouseControlEnable(bEnable);
}

/** 设置场景的控制状态
@param out: 场景是否可通过鼠标键盘等控制，FALSE：不能控制
*/
// DLL_EXPORT VOID CVGSCore::GetWndControlStatus(BOOL &bKeyboardEnable, BOOL &bMouseEnable);
// DLL_EXPORT BOOL CVGSCore::GetWndControlStatus();

/** 获取场景是否可以键盘控制
*/
BOOL CVGSCore::GetWndControlKeyboardStatus()
{
	return m_pRoot->getKeyboardControlEnable();
}

/** 获取场景是否可以鼠标控制
*/
BOOL CVGSCore::GetWndControlMouseStatus()
{
	return m_pRoot->getMouseControlEnable();
}

////////////////// 资源路径 //////////////////////
/** 添加资源文件搜索路径
*/ 
VOID CVGSCore::AddResourcePath(const CHAR * sFilePath)
{
	g_ResourcePath.insert(std::string(sFilePath));
}

/** 删除资源文件搜索路径
*/ 
VOID CVGSCore::RemoveResourcePath(const CHAR * sFilePath)
{
	g_ResourcePath.erase(std::string(sFilePath));
}

/** 删除所有资源文件搜索路径
*/ 
VOID CVGSCore::ClearResourcePaths()
{
	g_ResourcePath.clear();
}

//-------------------------------------------------------------------------------------------
//***场景渲染相关的方法
//-------------------------------------------------------------------------------------------
VOID CVGSCore::RenderAll()
{
	EnterCriticalSection(&m_render_cs);
	EnterCriticalSection(&m_ctrl_cs);
	
	//m_pLuaEngine->OnStartOneFrame();
	m_pRoot->RednerOneFrame();  // 渲染一帧图像
	//m_pLuaEngine->OnEndOneFrame();

	LeaveCriticalSection(&m_ctrl_cs);		
	LeaveCriticalSection(&m_render_cs);
}

/**渲染指定的场景，可能包含多个渲染窗体
@param in sceneHandle VGS场景管理器指针
*/
//-----------------------------------------------------------------------------
// 方法名: RenderScene()
// 参  数: 
// 用  途: 渲染整个场景
//-----------------------------------------------------------------------------
VOID CVGSCore::RenderScene(DWORD sceneHandle)
{
	EnterCriticalSection(&m_render_cs);
	m_pRoot->RednerOneFrame();  // 渲染一帧图像
	LeaveCriticalSection(&m_render_cs);

	/*
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;
	
	INT spf = (INT)(1000.0f/vgsScene->m_kfrPlayspeed);
	if (GetTickCount() - lastFrameTimer >= spf)
	{
		lastFrameTimer = GetTickCount();

		if (vgsScene->m_bKfrPlaying)
		{
			if (vgsScene->m_CurrentFrame < vgsScene->m_KfrCnt -1)
				vgsScene->m_CurrentFrame++;
		}
	}
		
	EnterCriticalSection(&m_render_cs);

	BOOL bRendered = FALSE;

	//动态模糊
	if(vgsScene->m_pFSMotionBlur && vgsScene->m_bFSMotionBlur)
	{
		//设置渲染到贴图表面
		vgsScene->m_pFSMotionBlur->SetRenderTarget();
		//渲染
		vgsScene->Render();
		//混合效果
		vgsScene->m_pFSMotionBlur->RenderEffect();
		bRendered = TRUE;
	}

	//全屏光晕
	if (vgsScene->m_pFSGlow && vgsScene->m_bFSGlow)
	{
		vgsScene->m_pFSGlow->SetRenderTarget();
		vgsScene->Render();
		vgsScene->m_pFSGlow->RenderEffect();
		bRendered = TRUE;
	}

	if (!bRendered)
		vgsScene->Render();
		

	LeaveCriticalSection(&m_render_cs);*/

}

//更新后缓冲
BOOL CVGSCore::UpdateBackBuffer(DWORD hRenderWnd)
{
	BOOL re = FALSE;
	CSwapChain *pRenderWnd = (CSwapChain*)hRenderWnd;
	if (pRenderWnd)
	{
		re = pRenderWnd->UpdateBackbuffer();
	}
	return re;
}

/*//渲染材质球场景
VOID CVGSCore::RenderMtrlSphereScene(DWORD sceneHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	vgsScene->RenderMtrlSphere();
}*/


/**添加Viewport
@param in windowHandle 渲染窗体句柄
@camHandle in 视口所属的相机
@param in level   视口的层次，层次越高的视口会覆盖在层次低的视口上方
@param in left	  视口的最左角位置，相对值[0-1] left = vp_left/window_width;
@param in top	  视口的最上角位置，相对值[0-1] top = vp_top/window_height;
@param in width	  视口的宽度，相对值[0-1] width = vp_width/window_width;
@param in  height 视口的高度，相对值[0-1] height = vp_height/window_height;
@return 所创建的视口指针，如果创建失败，返回0
*/
DWORD CVGSCore::AddViewport(DWORD windowHandle, DWORD camHandle, INT level, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
	//for test 2010-6-1
// 	if (m_ptrSwfView)
// 	{
// 		RECT rc;
// 		::GetClientRect(g_VGSRender.m_hWnd, &rc);
// 		int w = rc.right - rc.left, h = rc.bottom - rc.top;
// // 		m_ptrSwfView->SetViewport(w, h, 0, 0, w, h);
// 	}
	CRenderWindow* pRenderWnd  = (CRenderWindow*)windowHandle;
	if (pRenderWnd) 
	{
		return (DWORD)pRenderWnd->AddViewport((CCamera*)camHandle, level, left, top, width, height);
	}
	return 0;
}
/** 删除渲染窗口的视口
@param windowHandle in 渲染窗体指针
@param viewportHandle in 视口指针
*/
VOID CVGSCore::DeleteViewport(DWORD windowHandle, DWORD vpHandle)
{
	CRenderWindow* pRenderWnd  = (CRenderWindow*)windowHandle;
	if (pRenderWnd) 
	{
		pRenderWnd->DestroyViewport((CViewport*)vpHandle);
	}
}

/** 设置视口使用的相机
@param viewportHandle in 视口指针
@param camHandle in 相机指针
*/
VOID CVGSCore::SetViewportCamera(DWORD viewportHandle, DWORD camHandle)
{
	CViewport* pViewport  = (CViewport*)viewportHandle;
	if (pViewport)
	{
		pViewport->SetCamera((CCamera*)camHandle);
	}
}

/** 获取视口当前使用的相机
@param viewportHandle 视口指针
@return 相机指针
*/
DWORD CVGSCore::GetViewportCamera(DWORD viewportHandle)
{	
	CViewport* pViewport  = (CViewport*)viewportHandle;
	if (pViewport)
	{
		return (DWORD)pViewport->GetCamera();
	}
	return 0;
}

/**设置视口位置和大小 
@param ViewPort 视口指针
@param left,top,width,height取值均为[0,1],表示在RenderTarget中的位置
*/
VOID CVGSCore::SetViewportRect(DWORD ViewPort, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
	CViewport* pViewport  = (CViewport*)ViewPort;
	if (pViewport)
	{
		RECT rt;
		rt.left = left;
		rt.top  = top;
		rt.right = left + width;
		rt.bottom = top + height;
		pViewport->SetRect(rt);
	}	
}

/** 设置视口使用相机的宽高比
*/
VOID CVGSCore::UpdateViewportAspectRatio(DWORD viewportHandle)
{
	CViewport* pViewport  = (CViewport*)viewportHandle;
	if (pViewport)
	{
		pViewport->UpdateAspectRatio();
	}
}

/**获取视口大小 
@param ViewPort 视口指针
@param left,top,width,height取值均为[0,1],表示在RenderTarget中的位置
*/
VOID CVGSCore::GetViewportRect(DWORD viewportHandle, float &left, float &top, float &width, float &height)
{
	CViewport* pViewport  = (CViewport*)viewportHandle;
	if (pViewport)
	{
		RECT rt;
		pViewport->GetRect(rt);
		left = rt.left;
		top = rt.top;
		width = rt.right - rt.left;
		height = rt.bottom - rt.top;
	}
}

/** 得到视口的真实宽度
@return : 如果视口不存在，返回-1
*/
INT CVGSCore::GetViewportActualWidth(DWORD viewportHandle)
{
	CViewport* pViewport  = (CViewport*)viewportHandle;
	if (pViewport)
	{
		RECT rc;
		pViewport->GetRect(rc);
		return (rc.right - rc.left);
	}
	return 0;
}

/** 得到视口的真实高度
@return : 如果视口不存在，返回-1
*/
INT CVGSCore::GetViewportActualHeight(DWORD viewportHandle)
{
	CViewport* pViewport  = (CViewport*)viewportHandle;
	if (pViewport)
	{
		RECT rc;
		pViewport->GetRect(rc);
		return (rc.bottom - rc.top);
	}
	return 0;
}

/**设置当前场景是否自动渲染
@param sceneHandle 场景指针
@param renderScene 是否应该渲染，如果设为<b>不</b>自动渲染，则只有调用RenderTarget的Update方法时窗体才能被渲染。
*/
VOID CVGSCore::SetSceneRenderStatus(DWORD sceneHandle, BOOL renderScene)
{	
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	vgsScene->m_bRenderScene = renderScene;
}
/**获取当前场景是否自动渲染
@param sceneHandle 场景指针
*/
BOOL CVGSCore::GetSceneRenderStatus(DWORD sceneHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return FALSE;

	return vgsScene->m_bRenderScene;
}

/**设置渲染窗体是否渲染
@param sceneHandle 场景管理器指针
@param RenderTargetHandle 渲染窗体指针
@param bAutoRender 是否自动渲染，如果设为<b>不</b>自动渲染，则只有调用RenderTarget的Update方法时窗体才能被渲染。
*/
VOID CVGSCore::SetRenderTargetStatus(DWORD sceneHandle, DWORD RenderTargetHandle, BOOL bAutoRender)
{
	CSwapChain* pSwapChain = (CSwapChain*)RenderTargetHandle;
	if (pSwapChain)
	{
		pSwapChain->m_bRender = bAutoRender;
	}
}

/**获取渲染窗体是否渲染
@param sceneHandle 场景管理器指针
@param RenderTargetHandle 渲染窗体指针
*/
BOOL CVGSCore::GetRenderTargetStatus(DWORD sceneHandle, DWORD RenderTargetHandle)
{
	CSwapChain* pSwapChain = (CSwapChain*)RenderTargetHandle;
	if (pSwapChain)
	{
		return pSwapChain->m_bRender;
	}

	return FALSE;
}

/**设置渲染窗体的填充模式, 2为线框，3为实体
@param sceneHandle 场景管理器指针
@param CameraHandle 视口指针
@param fillMode 渲染模式，1 : Only points are rendered， 2 : Wireframe models are rendered. 3: Solid polygons are rendered.
*/
// VOID CVGSCore::SetSwapRenderWndFillMode(DWORD sceneHandle, DWORD CameraHandle, INT fillMode)
VOID CVGSCore::SetViewportFillMode(DWORD sceneHandle, DWORD viewport, INT fillMode)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;
	
	CViewport *pViewport = (CViewport*)viewport;
	if (pViewport)
	{
		pViewport->SetFillMode(fillMode);
	}
}

/**获得渲染窗体的填充模式, 1 : Only points are rendered， 2 : Wireframe models are rendered. 3: Solid polygons are rendered.
@param sceneHandle 场景管理器指针
@param viewport 视口指针
*/
// INT GetSwapRenderWndFillMode(DWORD sceneHandle, DWORD CameraHandle)
INT CVGSCore::GetViewportFillMode(DWORD sceneHandle, DWORD viewport)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return 0;
	
	CViewport *pViewport = (CViewport*)viewport;
	if (pViewport)
	{
		return pViewport->GetFillMode();
	}
	return 0;
}


/** 强制渲染当前帧
@param sceneHandle 场景指针
*/
VOID CVGSCore::RenderCurrentFrame(DWORD sceneHandle)
{
	RenderScene(sceneHandle);
}

/**设置视口是否渲染二维界面
@param sceneHandle in 场景管理器指针
@param vpHandle in : 视口指针
*/
VOID CVGSCore::SetViewportRenderGUI(DWORD sceneHandle, DWORD vpHandle, BOOL renderGUI)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	vgsScene->m_bRenderGUI = renderGUI;
}

/**获取视口是否渲染二维界面
@param sceneHandle in ：场景管理器指针
@param vpHandle in : 视口指针
@renderGUI 是否渲染二维元素
*/
BOOL CVGSCore::IsViewportRenderGUI(DWORD sceneHandle, DWORD vpHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return FALSE;

	return vgsScene->m_bRenderGUI;
}

/**设置场景的背景
@param sceneHandle 场景管理器指针
@param ViewportHandle 视口的指针，当前版本必须设置NULL，将设置场景中的所有视口
@param color 颜色
*/
VOID CVGSCore::SetSceneBackColor(DWORD sceneHandle, DWORD ViewportHandle, DWORD color)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pScene->m_backgroundColor = color;
	}
}

/**获得场景的背景
@param sceneHandle 场景管理器指针
@param ViewportHandle 视口的指针,当前版本必须为NULL
*/
DWORD CVGSCore::GetSceneBackColor(DWORD sceneHandle, DWORD ViewportHandle)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		return pScene->m_backgroundColor;
	}
	return 0;
}

/*
//设置场景的VGS窗体初始尺寸
VOID CVGSCore::SetSceneRect(DWORD sceneHandle, RECT* pRect)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pScene->m_oriWidth  = pRect->right - pRect->left;
		pScene->m_oriHeight = pRect->bottom - pRect->top;
	}
}

//获得场景的VGS窗体初始尺寸
VOID CVGSCore::GetSceneRect(DWORD sceneHandle, RECT* pRect)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pRect->left   = 0;
		pRect->top    = 0;
		pRect->right  = pScene->m_oriWidth;
		pRect->bottom = pScene->m_oriHeight;
	}
}
*/

//计算三维坐标的屏幕坐标
VOID CVGSCore::CalcScreenCoords(DWORD sceneHandle, DWORD viewportHandle, VECTOR3* pVec3, INT* X, INT* Y)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;
	
	CViewport* pViewport  = (CViewport*)viewportHandle;
	if (pViewport == NULL) return;

	D3DXVECTOR3 pos;
	pos.x = pVec3->x;
	pos.y = pVec3->y;
	pos.z = pVec3->z;

	vgsScene->CalcScreenCoords(pViewport, &pos, X, Y);
}

/*
//缩放轴侧窗口
VOID CVGSCore::ZoomOrthoView(DWORD sceneHandle, DWORD swapChainHandle, FLOAT zoomVal)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;
	
	CSwapChain* pSwapChain  = (CSwapChain*)swapChainHandle;
	if (pSwapChain == NULL) return;
	
	EnterCriticalSection(&m_render_cs);
	
	pSwapChain->m_rect.left = 0;
	pSwapChain->m_rect.top = 0;
	pSwapChain->m_rect.right = pSwapChain->m_rect.right / zoomVal;
	pSwapChain->m_rect.bottom = pSwapChain->m_rect.bottom / zoomVal;

	if (pSwapChain->m_rect.right < 10)
		pSwapChain->m_rect.right = 10;
	if (pSwapChain->m_rect.right > 1000000)
		pSwapChain->m_rect.right = 1000000;

	pSwapChain->m_rect.bottom = pSwapChain->m_rect.right / pSwapChain->m_ratio; 

	LeaveCriticalSection(&m_render_cs);
}
*/
/*
//当前窗体是否为轴侧窗口
BOOL IsOrthoView(DWORD sceneHandle, DWORD swapChainHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return FALSE;
	
	CSwapChain* pSwapChain  = (CSwapChain*)swapChainHandle;
	if (pSwapChain == NULL) return FALSE;

	if (pSwapChain->GetViewport(0)->GetCamera()->m_Type == VTC_ORTHO)
		return TRUE;
	else
		return FALSE;
}
*/

//将当前窗口中的三维场景设置到合适的大小
// VOID CVGSCore::MaximizeSceneToView(DWORD sceneHandle, DWORD vpHandel, FLOAT fillRatio)
// {
// 	CScene* vgsScene = (CScene*)sceneHandle;
// 	if (vgsScene == NULL) return;
// 	
// 	CSwapChain* pSwapChain  = (CSwapChain*)swapChainHandle;
// 	if (pSwapChain == NULL) return;
// 	
// 	EnterCriticalSection(&m_render_cs);
// 
// 	FLOAT MinX = 1000000000;
// 	FLOAT MinY = 1000000000;
// 
// 	FLOAT MaxX = -1000000000;
// 	FLOAT MaxY = -1000000000;
// 
// 	if (pSwapChain->m_projMode)  //轴侧模式
// 	{
// 		INT modelCnt = vgsScene->model.size();
// 		CModel** ppModel = vgsScene->model.begin();
// 		for (INT i = 0; i< modelCnt; i++)
// 		{
// 			CModel* pModel = *(ppModel + i);
// 
// 			//最小值
// 			FLOAT x1 = pModel->boundingSphere.Center.x - 
// 				pModel->boundingSphere.Radius;
// 			FLOAT y1 = pModel->boundingSphere.Center.y - 
// 				pModel->boundingSphere.Radius;
// 			FLOAT z1 = pModel->boundingSphere.Center.z - 
// 				pModel->boundingSphere.Radius;
// 
// 			D3DXVECTOR3 vMin = D3DXVECTOR3(x1, y1, z1);
// 			INT sx1, sy1;
// 			vgsScene->CalcScreenCoords(pSwapChain, &vMin, &sx1, &sy1);
// 
// 			if (sx1 < MinX) MinX = sx1;
// 			if (sy1 < MinY) MinY = sy1;
// 			if (sx1 > MaxX) MaxX = sx1;
// 			if (sy1 > MaxY) MaxY = sy1;
// 				
// 			//最大值
// 			FLOAT x2 = pModel->boundingSphere.Center.x + 
// 				pModel->boundingSphere.Radius;
// 			FLOAT y2 = pModel->boundingSphere.Center.y + 
// 				pModel->boundingSphere.Radius;
// 			FLOAT z2 = pModel->boundingSphere.Center.z + 
// 				pModel->boundingSphere.Radius;
// 
// 			D3DXVECTOR3 vMax = D3DXVECTOR3(x2, y2, z2);
// 			INT sx2, sy2;
// 			vgsScene->CalcScreenCoords(pSwapChain, &vMax, &sx2, &sy2);
// 
// 			if (sx2 < MinX) MinX = sx2;
// 			if (sy2 < MinY) MinY = sy2;
// 			if (sx2 > MaxX) MaxX = sx2;
// 			if (sy2 > MaxY) MaxY = sy2;
// 		}
// 		
// 		INT width = MaxX - MinX;
// 		INT height = MaxY - MinY;
// 		
// 		float widthRatio = width * 1.0f / (pSwapChain->m_rect.right - pSwapChain->m_rect.left);
// 		float heightRatio = height * 1.0f / (pSwapChain->m_rect.bottom - pSwapChain->m_rect.top);
// 		
// 		float scRatio = 1.0f/fillRatio;
// 
// 		RECT rect = {0, 0, 0, 0};
// 		if (widthRatio > heightRatio)
// 		{
// 			rect.bottom = scRatio * (pSwapChain->m_rect.bottom - pSwapChain->m_rect.top) * widthRatio;
// 			rect.right = rect.bottom * pSwapChain->m_ratio;
// 			pSwapChain->m_rect = rect;
// 		}
// 		else
// 		{
// 			rect.right = scRatio * (pSwapChain->m_rect.right - pSwapChain->m_rect.left) * heightRatio;
// 			rect.bottom = rect.right / pSwapChain->m_ratio;
// 			pSwapChain->m_rect = rect;
// 		}
// 	}
// 	
// 	LeaveCriticalSection(&m_render_cs);
// }


//平移轴侧窗口
VOID CVGSCore::PanOrthoView(DWORD sceneHandle, DWORD swapChainHandle, INT X, INT Y, INT offsetX, INT offsetY)
{/*
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;
	
	CSwapChain* pSwapChain  = (CSwapChain*)swapChainHandle;
	if (pSwapChain == NULL) return;
	
	EnterCriticalSection(&m_render_cs);

	D3DXVECTOR3 vPointer1, vPointer2;

	INT X2 = X + offsetX;
	INT Y2 = Y + offsetY;

	if (X2 < 0 || Y2 < 0) return;

	//前视图和后视图
	if (pSwapChain->CameraType == 1 || pSwapChain->CameraType == 2)
	{
		vgsScene->GetPointerOnPlaneZ(pSwapChain, X, Y, &vPointer1);
		vgsScene->GetPointerOnPlaneZ(pSwapChain, X2, Y2, &vPointer2);
	}

	//左视图和右视图
	if (pSwapChain->CameraType == 3 || pSwapChain->CameraType == 4)
	{
		vgsScene->GetPointerOnPlaneX(pSwapChain, X, Y, &vPointer1);
		vgsScene->GetPointerOnPlaneX(pSwapChain, X2, Y2, &vPointer2);
	}

	//上视图和底视图
	if (pSwapChain->CameraType == 5 || pSwapChain->CameraType == 6)
	{
		vgsScene->GetPointerOnPlaneY(pSwapChain, X, Y, &vPointer1);
		vgsScene->GetPointerOnPlaneY(pSwapChain, X2, Y2, &vPointer2);
	}

	D3DXVECTOR3 offsetV = -(vPointer2 - vPointer1);

	pSwapChain->m_pCamera->Translate(&offsetV, 0);

	LeaveCriticalSection(&m_render_cs);
	*/
}

//-------------------------------------------------------------------------------------------
///文件相关的方法
//-------------------------------------------------------------------------------------------
/** 导入S3D文件
@param sceneHandle		: 场景管理器句柄
@param strS3DFileName	: s3D文件的完整路径
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return 成功返回S3D加载器的句柄，否则返回0
@remarks 通过返回的S3D加载器句柄，还可以获取跟本S3D相关的一些信息。切记，不用时要及时调用DeleteS3DLoader()方法删除加载器
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
// #if defined(_VGS_DLL)
DWORD CVGSCore::ImportS3DFile(DWORD sceneHandle, const CHAR* strS3DFileName, HWND hWnd)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		return vgsScene->ImportS3DFile(std::string(strS3DFileName), hWnd);
	}
	
	return 0;
}

/** 删除S3D加载对象
*/
VOID CVGSCore::DeleteS3DLoader(DWORD sceneHandle, DWORD S3DLoaderHandle)
{
	CImportS3D *pImportS3D = (CImportS3D*)S3DLoaderHandle;
	if (pImportS3D)
	{
		pImportS3D->Release();
		delete pImportS3D;
	}
}

/** 指定的贴图文件是否被S3D使用
@param S3DLoaderHandle in  : S3D加载器指针，由ImportS3DFile()返回
@param ImageFileName   in  : 图片文件名称，不包括路径
*/
BOOL CVGSCore::IsTextureImageUsedbyS3D(DWORD S3DLoaderHandle, const CHAR* ImageFileName)
{
	return FALSE;
}

/** 得到S3D中使用的贴图的数量
@param loaderHandle in : S3D加载器指针，由ImportS3DFile()返回
*/
INT CVGSCore::GetS3DTextureImageCnt(DWORD S3DLoaderHandle)
{
	return 0;
}

/** 得到S3D中使用的第index个贴图文件名称，包含路径
@param loaderHandle in  : S3D加载器指针，由ImportS3DFile()返回
@param texIndex     in  : 索引
@param strTexName   out : 贴图文件名称
*/
BOOL CVGSCore::GetS3DTextureImageFile(DWORD S3DLoaderHandle, INT texIndex, CHAR* strTexName)
{
	return FALSE;
}

/** 保存场景到指定的V3D文件
@param in sceneHandle    ：VGS场景管理器指针
@param in strV3DFileName ：要保存的V3D文件名
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return错误码。0表示成功，其他值表示失败。通过GetErrorStr可以得到详细描述
*/
DWORD CVGSCore::SaveSceneAsV3D(DWORD sceneHandle, CHAR* strV3DFileName, HWND hWnd)
{
	EnterCriticalSection(&m_ctrl_cs);
	EnterCriticalSection(&m_render_cs);

	DWORD re = ERR_SCENE_NOSCENE;
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		re = vgsScene->SaveV3DFile(std::string(strV3DFileName), hWnd);
	}

	LeaveCriticalSection(&m_render_cs);
	LeaveCriticalSection(&m_ctrl_cs);
	return re;
}

/**打开V3D文件, 自动识别不同版本的V3D
@param in sceneHandle VGS场景管理器指针
@param in strV3DFileName 要打开的V3D文件名
@param in OT 打开V3D的方法，例如替换方式还是合并方式，目前仅支持替换方式
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return 返回结果，0表示成功，其他值表示失败。可通过GetErrorStr可以得到原因描述
*/
DWORD CVGSCore::OpenV3DFile(DWORD sceneHandle, const CHAR* strV3DFileName, V3D_OPENTYPE OT, HWND hWnd)
{
	EnterCriticalSection(&m_ctrl_cs);
	EnterCriticalSection(&m_render_cs);

	DWORD re = ERR_SCENE_NOSCENE;
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		re = vgsScene->OpenV3DFile(std::string(strV3DFileName), hWnd);
	}

	LeaveCriticalSection(&m_render_cs);
	LeaveCriticalSection(&m_ctrl_cs);
	return re;
}

/**保存当前场景为N3D格式文件
@param in sceneHandle ：要保存的场景
@param in strN3DFileName ： N3D文件名，包含目录
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return 返回结果，0表示成功，其他值表示失败。可通过GetErrorStr可以得到原因描述
*/
DWORD CVGSCore::SaveSceneAsN3D(DWORD sceneHandle, const CHAR* strN3DFileName, HWND hWnd)
{
	EnterCriticalSection(&m_ctrl_cs);
	EnterCriticalSection(&m_render_cs);
	
	// 暂停动画播放
	//m_pRoot->GetAnimManager().SetAllAnimStatus(AnimStatus(VAS_PAUSED, 0));

	CSaveN3D saveN3D;
	saveN3D.SetRoot(m_pRoot);
	saveN3D.SaveSceneAsN3D((CScene*)sceneHandle, strN3DFileName);

	PostMessage(hWnd, WM_WEBMAX_PROGRESS, 100, 100);  // 开始进度

	// 恢复动画播放状态
	//m_pRoot->GetAnimManager().SetAllAnimStatus(AnimStatus(VAS_PLAYING, 0));

	LeaveCriticalSection(&m_render_cs);
	LeaveCriticalSection(&m_ctrl_cs);

	return 0;
}

/** 打开N3D场景文件, 自动识别不同版本的N3D
@param in sceneHandle ：要保存的场景
@param in strN3DFileName ： N3D文件名，包含目录
@return 返回结果，可通过GetErrorStr可以得到原因描述
*/
// DLL_EXPORT DWORD ImportN3DFile(DWORD sceneHandle, const CHAR* strN3DFileName);

/**保存当前场景为E3D格式文件
@param in sceneHandle    ：要保存的场景
@param in strE3DFileName ：E3D文件名，包含目录
@param in bLinkRes       : 所需的资源是以外部链接方式还是打包到文件内部。bLinkRes为true，资源将以外部链接的方式，不打包到E3D内部
@param in bPackData      : 是否压缩文件，TRUE：压缩，FALSE：不压缩
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return 返回结果，0表示成功，其他值表示失败。可通过GetErrorStr可以得到原因描述
*/
DWORD CVGSCore::SaveSceneAsE3D(DWORD sceneHandle, const CHAR* strE3DFileName, BOOL bLinkRes, BOOL bPackData, HWND hWnd)
{
	MessageBox(NULL, TEXT("暂时没有实现"), TEXT("提示"), 0);
	return ERR_SCENE_NOSCENE;
}

/** 读入N3D文件
@param strE3DFileName in : 要读入的文件名称
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return错误码。0表示成功，其他值表示失败。通过GetErrorStr可以得到详细描述
*/
DWORD CVGSCore::LoadE3DFile(DWORD sceneHandle, const CHAR* strE3DFileName, HWND hWnd)
{
	MessageBox(NULL, TEXT("暂时没有实现"), TEXT("提示"), 0);
	return ERR_SCENE_NOSCENE;
}
#endif

/** 从buffer中读取N3D场景信息
@param in sceneHandle    ：要加载数据的场景
@param in version        : 版本号
@param in pBuffer        ：N3D数据buffer
@param hWnd           in ：传入的窗体句柄，用于接收进度信息
@return 返回结果，可通过GetErrorStr可以得到原因描述
*/
#if(defined(_VGS_LIB)||defined(_VGS_ATL))
DWORD CVGSCore::LoadSceneFromBuffer(DWORD sceneHandle, DWORD version, const BYTE* pBuffer, HWND hWnd)
{
	EnterCriticalSection(&m_ctrl_cs);
	EnterCriticalSection(&m_render_cs);

	DWORD re = ERR_COMMON_OK;
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		CN3DLoader loader;
		re = loader.LoadSceneFromBuffer(vgsScene, version, pBuffer, hWnd);
	}

	LeaveCriticalSection(&m_render_cs);
	LeaveCriticalSection(&m_ctrl_cs);

	return re;
}

/** 从buffer中读取图像信息
@param in sceneHandle    ：要加载图像的场景
@param in version        : 版本号
@param in pBuffer        ：N3D数据buffer
@param hWnd           in ：传入的窗体句柄，用于接收进度信息
@return 返回结果，可通过GetErrorStr可以得到原因描述
*/
DWORD CVGSCore::LoadImageDataFromBuffer(DWORD sceneHandle, DWORD version, const BYTE* pBuffer, HWND hWnd)
{
	EnterCriticalSection(&m_ctrl_cs);
	EnterCriticalSection(&m_render_cs);

	DWORD re = ERR_SCENE_NOSCENE;
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		CN3DLoader N3DLoader;
		N3DLoader.LoadImageDataFromBuffer(vgsScene, version, pBuffer, hWnd);
	}

	LeaveCriticalSection(&m_render_cs);
	LeaveCriticalSection(&m_ctrl_cs);
	return re;
}
#endif

// 由一张图片的buffer创建纹理
DWORD CVGSCore::CreateImageTextureFromBuffer(DWORD sceneHandle, const BYTE* pBuffer, DWORD len, int imgIdx, HWND hWnd)
{
	if (!pBuffer || len <= 0)
	{
		return ERR_FILE_N3D_IMAGELOAD;
	}

	// 遍历所有的texture
	UINT textureCount = m_pRoot->GetTextureManager().getResourceCount();
	for (UINT iTexture = 0; iTexture < textureCount; ++iTexture)
	{
		CTexture *pTexture = (CTexture*)m_pRoot->GetTextureManager().getResource(iTexture);
		if (pTexture && !pTexture->bCreated && pTexture->imageIdx == imgIdx)
		{
			m_pRoot->GetTextureManager().createTextureFromBuffer(pTexture, (char*)pBuffer, len, pTexture->isOpacity);
		}
	}

	return ERR_COMMON_OK;

	//BYTE *pOffset = m_pImageData;

	//DWORD count;
	//CPY_FROM_STREAM(count, pOffset);

	//std::vector<N3D_ImageInfo> vImageInfo;
	//for (DWORD i = 0; i < count; ++i)
	//{
	//	N3D_ImageInfo ImageInfo;

	//	CPY_FROM_STREAM(ImageInfo.nameLen, pOffset);
	//	//ImageInfo.name = new char[ImageInfo.nameLen + 1];
	//	//ZeroMemory(ImageInfo.name, ImageInfo.nameLen + 1);
	//	//CPY_FROM_STREAM(ImageInfo.name, pOffset);
	//	ImageInfo.name = (char*)pOffset;
	//	pOffset += ImageInfo.nameLen;

	//	CPY_FROM_STREAM(ImageInfo.DataLen, pOffset);
	//	ImageInfo.Data = (char*)pOffset;
	//	pOffset += ImageInfo.DataLen;
	//	//ImageInfo.Data = new char[ImageInfo.DataLen + 1];
	//	//ZeroMemory(ImageInfo.Data, ImageInfo.DataLen + 1);
	//	//CPY_FROM_STREAM(m_pImageData, ImageInfo.Data);

	//	vImageInfo.push_back(ImageInfo);
	//}

	//// 创建texture 
	//for (UINT iImage = 0; iImage < vImageInfo.size(); ++iImage)
	//{
	//	// 遍历所有的texture
	//	UINT textureCount = m_pRoot->GetTextureManager().getResourceCount();
	//	for (UINT iTexture = 0; iTexture < textureCount; ++iTexture)
	//	{
	//		CTexture *pTexture = (CTexture*)m_pRoot->GetTextureManager().getResource(iTexture);
	//		if (pTexture && !pTexture->bCreated && pTexture->imageIdx == iImage)
	//		{
	//			N3D_ImageInfo &imageInfo = vImageInfo[iImage];
	//			m_pRoot->GetTextureManager().createTextureFromBuffer(pTexture, imageInfo.Data, imageInfo.DataLen, pTexture->isOpacity);
	//		}
	//	}
	//}
}
//-------------------------------------------------------------------------------------------
///模型相关的方法
//-------------------------------------------------------------------------------------------

/**得到场景中模型的数量
@param in sceneHandle 场景管理器指针
@return 场景中模型的数量
*/
INT CVGSCore::GetModelCount(DWORD sceneHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return 0;

	return vgsScene->model.size();
}

/*删除场景中的模型
@param sceneHandle 场景管理器指针
@param modelHandle 模型(entity)指针
@return 成功删除返回TRUE，否则FALSE
*/
BOOL CVGSCore::DeleteModelbyHandle(DWORD sceneHandle, DWORD modelHandle)
{
	BOOL re = FALSE;
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		EnterCriticalSection(&m_render_cs);
		CModel *pModel = (CModel*)modelHandle;
		if (pModel)
		{
			vgsScene->DeleteModel(pModel);
			re = TRUE;
		}
		LeaveCriticalSection(&m_render_cs);
	}	
	return re;
}


/** 删除场景中的模型
@param sceneHandle 场景管理器指针
@param strModelName 模型(entity)名称
@return 成功删除返回TRUE，否则FALSE
*/
BOOL CVGSCore::DeleteModel(DWORD sceneHandle, const CHAR* strModelName)
{
	BOOL re = FALSE;
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		EnterCriticalSection(&m_render_cs);
		CModel *pModel = vgsScene->GetModelByName(std::string(strModelName));
		if (pModel)
		{
			vgsScene->DeleteModel(pModel);
			re = TRUE;
		}
		LeaveCriticalSection(&m_render_cs);
	}	
	return re;	

		/*
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return FALSE;

	DWORD modelHandle = GetModelHandleByName(sceneHandle, strModelName);

	CModel* pModel = (CModel*)modelHandle;
	if (pModel == NULL) return FALSE;

	EnterCriticalSection(&m_render_cs);

	//删除模型需要进行以下动作：
	//1. 如果是透明模型，将模型从透明模型列表中删除。
	//2. 将模型从模型总列表中删除。
	//3. 把模型从其材质的模型列表中删除。
	//4. 找到模型所用到的材质，如果该材质没有被其它模型所用，则需考虑将该材质从材质队列删除，
	//并释放该材质所占用的资源。
	//5. 考虑该材质用到的贴图，如果该贴图没有被其它模型所用，则先将该贴图从贴图队列删除，并
	//释放该贴图所占用的资源。
	//6. 释放该模型所占用的资源。	

	//如果是透明模型，把模型从透明模型列表删除
	if (pModel->shader->bAlphaBlend ||
		pModel->opacity < 100)
	{
		INT modelCnt = vgsScene->transModel.size();
		CModel** ppModel = vgsScene->transModel.begin();
		for (INT i = 0; i< modelCnt; i++)
		{
			CModel* pTModel = *(ppModel + i);
			if (pTModel == pModel)
			{
				//把模型从透明模型队列中删除
				vgsScene->transModel.erase(ppModel + i);
				break;
			}
		}
	}
	
	//把从总模型队列中删除
	INT modelCnt = vgsScene->model.size();
	CModel** ppModel = vgsScene->model.begin();
	for (INT i = 0; i< modelCnt; i++)
	{
		CModel* pTModel = *(ppModel + i);
		if (pTModel == pModel)
		{
			//从模型队列删除
			vgsScene->model.erase(ppModel + i);
			break;
		}
	}	

	//把模型从其材质列表删除
	INT mtrlModelCnt = pModel->shader->modelList.size();
	CModel** ppMtrlModel = (CModel**)pModel->shader->modelList.begin();
	for (INT j = 0; j< mtrlModelCnt; j++)
	{
		CModel* pMtrlModel = *(ppMtrlModel + j);
		if (pMtrlModel == pModel)
		{
			pModel->shader->modelList.erase((DWORD*)(ppMtrlModel + j));
			break;
		}
	}

	//释放模型使用的材质和贴图
	if (pModel->shader->modelList.size() == 0)
	{
		//把材质从材质列表中删除
		INT shaderCnt = vgsScene->shader.size();
		CMaterial** ppMtrl = vgsScene->shader.begin();
		for (INT i = 0; i< shaderCnt; i++)
		{
			CMaterial* pMtrl = *(ppMtrl + i);
			if (pMtrl == pModel->shader)
				vgsScene->shader.erase(ppMtrl + i);
		}		

		//把材质从材质列表删除
		SAFE_RELEASE(pModel->shader);
		SAFE_DELETE(pModel->shader);
	}


	//释放模型的资源
	SAFE_RELEASE(pModel);
	SAFE_DELETE(pModel);

	LeaveCriticalSection(&m_render_cs);
	
	return TRUE;*/
	return FALSE;
}

//根据模型的名称得到模型的句柄
DWORD CVGSCore::GetModelHandleByName(DWORD sceneHandle, const CHAR* strModelName)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return 0;

	INT modelCnt = vgsScene->model.size();
	std::vector<CModel*>::iterator ppModel = vgsScene->model.begin();
	for (INT i = 0; i< modelCnt; i++)
	{
		CModel* pModel = *(ppModel + i);
		if (strcmp( pModel->getName().c_str(), strModelName) == 0)
			return (DWORD)pModel;
	}
	return 0;
}
//根据模型的编号得到模型的句柄
DWORD CVGSCore::GetModelHandleByIndex(DWORD sceneHandle, INT index)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return 0;

	INT modelCnt = vgsScene->model.size();
	if (index < 0 || index >= modelCnt) return 0;

	std::vector<CModel*>::iterator ppModel = vgsScene->model.begin();
	CModel* pModel = *(ppModel + index);
	return (DWORD)pModel;
}

/**根据模型的句柄得到模型的名称
@param in sceneHandle 场景指针
@param in modelHandle 模型的句柄
@param in & out strModelName 模型名称
@return 得到名字返回TRUE，否则FALSE
*/
BOOL CVGSCore::GetModelNameByHandle(DWORD sceneHandle, DWORD modelHandle, CHAR* strModelName)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return FALSE;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		COPY_NAME(strModelName, pModel->getName().c_str());
	}
	return TRUE;
}
/**根据模型的索引号得到模型的名称
@param in sceneHandle 场景指针
@param in index 模型的索引号
@param in & out strModelName 模型名称
@return 得到名字返回TRUE，否则FALSE
*/
BOOL CVGSCore::GetModelNameByIndex(DWORD sceneHandle, INT index, CHAR* strModelName)
{
	DWORD hModel = GetModelHandleByIndex(sceneHandle, index);
	if (hModel)
	{
		GetModelNameByHandle(sceneHandle, hModel, strModelName);
		return TRUE;
	}
	return FALSE;
}


//设置模型的名称
VOID CVGSCore::SetModelName(DWORD sceneHandle, DWORD modelHandle, CHAR* strModelName)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		pModel->setName(std::string(strModelName));
	}
}

/** 移动模型的位置
@param in sceneHandle	: 场景句柄
@param in camHandle     : 相机句柄
@param in pVec          ：移动的相对向量
@param in coordEnum     ：移动的参考坐标系
			               0 - 在世界坐标系平移
						   1 - 在自身坐标系平移
*/
VOID CVGSCore::MoveModel(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec, UINT flag)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		if (pVec->x == 0.0f && pVec->y == 0.0f && pVec->z == 0.0f) return;
		D3DXVECTOR3 moveVec = D3DXVECTOR3(pVec->x, pVec->y, pVec->z);
		pModel->m_pNode->translate(&moveVec, (VGS_TRANSFORMSPACE)flag);
	}
}

/** 得到模型的位置,即包围盒中心的世界坐标
@param out pVec : 模型包围盒中心的世界坐标位置
*/
VOID CVGSCore::GetModelPosition(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel && pVec)
	{
		D3DXVECTOR3 v3Pos = pModel->m_pNode->getPosition(VTS_WORLD);
		pVec->x = v3Pos.x;
		pVec->y = v3Pos.y;
		pVec->z = v3Pos.z;
	}
}
//设置模型的位置
VOID CVGSCore::SetModelPosition(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
		pModel->setPosition(&(D3DXVECTOR3(pVec->x, pVec->y, pVec->z)), VTS_WORLD);
}

/** 获取模型的包围盒
@param in sceneHandle : 场景管理器指针
@param in modelHandle : 模型指针
@param out vMinCorner : 包围盒坐标最小的角的世界坐标
@param out vMinCorner : 包围盒坐标最大的角的世界坐标
@return 包围盒有效，返回TRUE, 否则FALSE
*/ 
BOOL CVGSCore::GetModelBoundsBox(DWORD sceneHandle, DWORD modelHandle, VECTOR3 &vMinCorner, VECTOR3 &vMaxCorner)
{	
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return FALSE;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		VGS_BoundBox bb = pModel->boundingBox;
		vMinCorner.x = bb.minCorner.x;
		vMinCorner.y = bb.minCorner.y;
		vMinCorner.z = bb.minCorner.z;
		vMaxCorner.x = bb.maxCorner.x;
		vMaxCorner.y = bb.maxCorner.y;
		vMaxCorner.z = bb.maxCorner.z;
		return TRUE;
	}
	return FALSE;
}

/** 获取模型的包围球
@param in sceneHandle : 场景管理器指针
@param in modelHandle : 模型指针
@param out vCenter    : 包围球中心点的世界坐标
@param out Radius     : 包围球半径
@return 包围盒有效，返回TRUE, 否则FALSE
*/ 
BOOL CVGSCore::GetModelBoundsSphere(DWORD sceneHandle, DWORD modelHandle, VECTOR3 &vCenter, FLOAT &Radius)
{	
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return FALSE;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		vCenter.x = pModel->boundingSphere.Center.x;
		vCenter.y = pModel->boundingSphere.Center.y;
		vCenter.z = pModel->boundingSphere.Center.z;
		Radius    = pModel->boundingSphere.Radius;
		return TRUE;
	}
	return FALSE;
}

/** 设置模型是否显示包围盒
@param in sceneHandle : 场景管理器指针
@param in modelHandle : 模型指针
@param in bvisible    : 是否显示包围盒
*/
VOID CVGSCore::SetModelBoundsBoxVisibility(DWORD sceneHandle, DWORD modelHandle, BOOL bVisible)
{	
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		pModel->showRBBox = bVisible;
	}
}

/** 旋转模型
@param in pVec : 在各个方向上旋转的角度增量,弧度
@param in coordEnum :
	//0 - 绕穿过自身中心，与世界坐标系平行的坐标系旋转
	//1 - 绕自身坐标系旋转模型
	//2 - 绕父坐标系旋转
*/
VOID CVGSCore::RotateModel(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec, INT coordEnum)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	if (pVec->x == 0.0f && pVec->y == 0.0f && pVec->z == 0.0f) return;
	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		D3DXVECTOR3 rosVec = D3DXVECTOR3(pVec->x, pVec->y, pVec->z);
		pModel->rotate(&rosVec, (VGS_TRANSFORMSPACE)coordEnum);
	}
}

//模型绕指定的轴旋转
//DLL_EXPORT VOID CVGSCore::RotateModelByAxis(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pPos, VECTOR3* pVec, FLOAT fAngle);

/** 得到模型的转角
@param out pRot ：模型当前相对于原始状态在三个方向上的转角，单位是弧度
*/
BOOL CVGSCore::GetModelRotation(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pRot)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return FALSE; 

	CModel* pModel = (CModel*)modelHandle;
	if (pModel && pRot)
	{
		D3DXMATRIX mat = pModel->m_pNode->getMatrix(VTS_WORLD);
		D3DXVECTOR3 trans, rota, scal;
		_decomposeTransfromMatrix(mat, trans, rota, scal);
		pRot->x = rota.x; pRot->y = rota.y; pRot->z = rota.z;
	}
	return TRUE;
}

/** 设置模型的转角
@param out pRot ：模型当前相对于原始状态在三个方向上的转角，单位是弧度
*/
VOID CVGSCore::SetModelRotation(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pRot)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		pModel->setRotationByAngle(&D3DXVECTOR3(pRot->x, pRot->y, pRot->z), VTS_WORLD);
	}
}

/** 以自身中心为中心，在当前基础上缩放模型
@param in pScale : 各个方向上的缩放值
@reamarks: 例如：pScale等于(1, 2, 0.5)：就表示模型将会在现在的基础上，在X方向不缩放，y方向放大一倍，z方向缩小一倍
*/
VOID CVGSCore::ScaleModel(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pScale, INT flag /*= 0*//*世界坐标系*/)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	if (pScale->x == 1.0f && pScale->y == 1.0f && pScale->z == 1.0f) return;
	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		D3DXVECTOR3 scaleVec = D3DXVECTOR3(pScale->x, pScale->y, pScale->z);
		pModel->scale(&scaleVec, (VGS_TRANSFORMSPACE)flag);
	}
}

//-----------------------------------------------------------------------------
// 方法名: ScaleModelByPoint()
// 参  数: DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale
// 用  途: 以固定点为中心缩放模型
//-----------------------------------------------------------------------------
VOID CVGSCore::ScaleModelByPoint(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	if (pScale->x == 1.0f && pScale->y == 1.0f && pScale->z == 1.0f) return;
	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		D3DXVECTOR3 vPos = D3DXVECTOR3(pPos->x, pPos->y, pPos->z);
		D3DXVECTOR3 vScale = D3DXVECTOR3(pScale->x, pScale->y, pScale->z);
		pModel->scaleByPoint(&vScale, &vPos, VTS_WORLD);	
	}
}

/** 以自身中心为中心，在模型原始尺寸基础上缩放模型
@reamarks: 例如：pScale等于(1, 2, 0.5)：就表示模型相对于原始尺寸，将在X方向不缩放，y方向放大一倍，z方向缩小一倍
*/
VOID CVGSCore::SetModelScale(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pScale)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
		pModel->setScale(&D3DXVECTOR3(pScale->x, pScale->y, pScale->z), VTS_PARENT/*VTS_LOCAL*/);
}

/** 得到模型的缩放
@param out pScale : 模型绝对缩放值，相对于原始大小，不是相对于上一次的。
@reamarks: 例如：pScale等于(1, 2, 0.5)：就表示模型现在相对于原始尺寸，在X方向没有缩放，y方向放大了一倍，z方向缩小了一倍
*/
BOOL CVGSCore::GetModelScale(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pScale)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel && pScale)
	{
		D3DXVECTOR3 v3Scale = pModel->m_pNode->getScale(VTS_PARENT);
		pScale->x = v3Scale.x;
		pScale->y = v3Scale.y;
		pScale->z = v3Scale.z;
		return TRUE;
	}
	return FALSE;
}
//以固定点为中心缩放模型
// DLL_EXPORT VOID CVGSCore::ScaleModelByPoint(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale);

//Add By Baojiahui 修正移动相机以及移动物体时位置不准确的方法----------------------------------------------------------------------------------------------------------
/** 得到垂直于屏幕坐标下对应的3d场景中某个平面上的交点坐标
@param in sceneHandle      :场景管理器指针
@param in vpHandle         :视口指针
@param in ScreenX, ScreenY :相对于RenderWindow区域的屏幕坐标
@param in planeNormal*      :特定平面的法线向量指针
@param in planePoint*       :过特定平面的点坐标指针
@param out vec3WorldPoint*  :返回的平面交点坐标指针
*/
VOID CVGSCore::GetWorldPointUnderLoc(DWORD sceneHandle,DWORD vpHandle,UINT ScreenX, UINT ScreenY,VECTOR3* planeNormal,VECTOR3* planePoint,VECTOR3* vec3WorldPoint)
{
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

/** 得到屏幕坐标下对应的3d场景中的模型，如有多个模型，得到最近的一个。隐藏或被冻结的model不会被选中
@param in sceneHandle      : 场景管理器指针
@param in vpHandle         : 视口指针
@param in ScreenX, ScreenY : 相对于RenderWindow区域的屏幕坐标
@param in modeltype        : 0 : 仅选中模型， 1：选中所有，包括灯光和相机
@return : 模型指针
*/
DWORD CVGSCore::GetModelUnderLoc(DWORD sceneHandle, DWORD vpHandle, UINT ScreenX, UINT ScreenY, int modeltype/* = 0*/)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return 0;

	CViewport* pViewport = (CViewport*)vpHandle;
	if (pViewport == NULL) return 0;

	EnterCriticalSection(&m_render_cs);
	CModel* pModel = NULL;
	pModel = vgsScene->GetModelUnderCursor(pViewport, ScreenX, ScreenY);
	LeaveCriticalSection(&m_render_cs);
	return (DWORD)pModel;
}

/** 获取模型属性信息
*/
BOOL CVGSCore::GetModelInfo(DWORD sceneHandle, DWORD modelHandle, VGS_ModelInfo &Info)
{
	ZeroMemory(&Info, sizeof(Info));
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		Info.isVisible = pModel->visible;
		Info.isGround  = pModel->isGround;
		Info.isCollision = pModel->isCollision;
		Info.isClickable = pModel->clickable;
		Info.isBillboard = pModel->isBillboard;

		if (fabs(pModel->pointAtOrient.PointAtUp.x - 1) < VGS_ZERO_VALUE_5)
		{
			Info.dirctionAxis = VGS_WAXIS_X; // 世界坐标系X轴
		}
		else if (fabs(pModel->pointAtOrient.PointAtUp.z - 1) < VGS_ZERO_VALUE_5)
		{
			Info.dirctionAxis = VGS_WAXIS_Z; // 世界坐标系Z轴
		}
		else
		{
			Info.dirctionAxis = VGS_WAXIS_Y; // 世界坐标系Y轴
		}
				
		return TRUE;
	}
	return FALSE;
}

/** 设置模型属性信息
*/
VOID CVGSCore::SetModelInfo(DWORD sceneHandle, DWORD modelHandle, const VGS_ModelInfo &Info)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		pModel->visible = Info.isVisible;
		pModel->isGround = Info.isGround;
		pModel->isCollision = Info.isCollision;
		pModel->clickable = Info.isClickable;
		pModel->isBillboard = Info.isBillboard;
		VECTOR3 UpAxis = _GetAxisDirection(Info.dirctionAxis);
		SetBillboardUp(sceneHandle, modelHandle, &UpAxis);
	}
}

/** 设置模型的鼠标事件函数名称
@param scenHandle   in : 场景指针
@param modelHandle  in : 模型指针
@param type         in  : 事件类型
@param sEventName   in : 函数名称
*/
VOID CVGSCore::SetModelMouseEvent(DWORD sceneHandle, DWORD modelHandle, VGS_MOUSEEVENT_TYPE type, const CHAR* sEventName)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		pModel->setEventFuncName(type, std::string(sEventName));
	}
}

/** 获取模型的鼠标事件函数名称
@param scenHandle   in  : 场景指针
@param modelHandle  in  : 模型指针
@param type         in  : 事件类型
@param sEventName   out : 函数名称
@return 如果模型已经制定了对应的事件函数，返回TRUE，否则FALSE
*/
BOOL CVGSCore::GetModelMouseEvent(DWORD sceneHandle, DWORD modelHandle, VGS_MOUSEEVENT_TYPE type, CHAR* sEventName)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		std::string str = pModel->getEventFuncName(type);
		if (!str.empty())
		{
			COPY_NAME(sEventName, str.c_str());
			return TRUE;
		}
	}
	return FALSE;
}

/** 把相机设置为billboard相机
*/
VOID CVGSCore::SetBillboardCamera(DWORD sceneHandle, DWORD camHanlde)
{

}

/** 获取billboard相机
*/
DWORD CVGSCore::GetBillboardCamera(DWORD sceneHandle)
{
	return 0;
}

/** 设置模型正对Billboard状态
@param axis in : 设置模型的哪个坐标轴（自身）指向相机
*/
VOID CVGSCore::SetModelBillboardStatus(DWORD sceneHandle, DWORD modelHandle, BOOL status, VGS_AXIS_TYPE UpAxis/* = VGS_LAXIS_NEGATIVEZ*/)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		pModel->isBillboard = status;
		VECTOR3 v3Up = _GetAxisDirection(UpAxis);
		SetBillboardUp(sceneHandle, modelHandle, &v3Up);
	}
}

/** 判断模型是否为Billboard模型
@param axis in : 模型的哪个坐标轴（自身）指向相机
*/
BOOL CVGSCore::GetModelBillboardStatus(DWORD sceneHandle, DWORD modelHandle, VGS_AXIS_TYPE &axis)
{

	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return FALSE;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		return pModel->isBillboard;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// 方法名: SetBillboardUp()
// 参  数: DWORD modelHandle, VECTOR3* pModelRay, VECTOR3* pAt, VECTOR3* pUp
//modelHandle ->模型的指针
//pUp ->模型的上方是什么方向，譬如Tree，要求Z方向正对目标点，但up方向始终为(0,1,0), 否则就倒了
// 用  途: 设置模型正对Billboard
//-----------------------------------------------------------------------------
VOID CVGSCore::SetBillboardUp(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pUp)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		pModel->pointAtOrient.PointAtUp.x = pUp->x;
		pModel->pointAtOrient.PointAtUp.y = pUp->y;
		pModel->pointAtOrient.PointAtUp.z = pUp->z;
	}
}

//设置模型正对Billboard的Up向量
//DLL_EXPORT VOID CVGSCore::SetBillboardUp(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pUp);

//设置场景碰撞的有效状态
/*VOID CVGSCore::SetSceneCollisionStatus(DWORD sceneHandle, BOOL status)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		vgsScene->m_bIsDetectCollision = status;
	}
}
*/
////设置碰撞发生的有效距离, 默认为1.0
//DLL_EXPORT VOID CVGSCore::SetSceneCollisionSta(DWORD sceneHandle, FLOAT distance);

/** 设置模型为水平碰撞模型
*/
VOID CVGSCore::SetModelCollisionStatus(DWORD sceneHandle, DWORD modelHandle, BOOL status)
{	
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CModel* pModel = (CModel*)modelHandle;
	if (pModel)
	{
		pModel->isCollision = status;
	}
}

/** 判断模型是否为水平碰撞模型
*/
BOOL CVGSCore::GetModelCollisionStatus(DWORD sceneHandle, DWORD modelHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		return pModel->isCollision;
	}
	return FALSE;
}

/** 设置模型为地面模型
*/
VOID CVGSCore::SetModelAsGround(DWORD sceneHandle, DWORD modelHandle, BOOL status)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		pModel->isGround = status;
	}
}

/** 判断模型是否为地面模型
*/
BOOL CVGSCore::GetModelAsGround(DWORD sceneHandle, DWORD modelHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		return pModel->isGround;
	}
	return FALSE;
}

////移动模型Diffuse贴图的UV
//DLL_EXPORT VOID CVGSCore::MoveModelUV(DWORD sceneHandle, DWORD modelHandle, FLOAT x, FLOAT y);
//
////设置模型动态UV的状态
//DLL_EXPORT VOID CVGSCore::SetModelDynamicUVStatus(DWORD sceneHandle, DWORD modelHandle, BOOL status);
//
////获得模型动态UV的状态
//DLL_EXPORT BOOL CVGSCore::GetModelDynamicUVStatus(DWORD sceneHandle, DWORD modelHandle);
//
////设置模型的动态UV
//DLL_EXPORT VOID CVGSCore::SetModelDynamicUV(DWORD sceneHandle, DWORD modelHandle, FLOAT u, FLOAT v);
////获得模型的动态UV
//DLL_EXPORT VOID CVGSCore::GetModelDynamicUV(DWORD sceneHandle, DWORD modelHandle, FLOAT* u, FLOAT* v);


/** 设置模型是否可见
*/
VOID CVGSCore::SetModelVisibility(DWORD sceneHandle, DWORD modelHandle, BOOL status)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		pModel->visible = status;
	}
}

/** 获取模型是否可见
*/
BOOL CVGSCore::GetModelVisibility(DWORD sceneHandle, DWORD modelHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		return pModel->visible;
	}
	return FALSE;
}

/** 设置模型在播放状态下是否可以点击
*/
VOID CVGSCore::SetModelClickable(DWORD sceneHandle, DWORD modelHandle, BOOL status)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		pModel->clickable = status;
	}
}

/** 获取模型在播放状态下是否可以点击
*/
BOOL CVGSCore::GetModelClickabel(DWORD sceneHandle, DWORD modelHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		return pModel->clickable;
	}
	return FALSE;
}

//设置模型为SkyBox模型
VOID CVGSCore::SetSkyBox(DWORD sceneHandle, DWORD modelHandle, BOOL status)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		pModel->isSkyBox = status;
	}
}

//判断模型是否为SkyBox
BOOL CVGSCore::IsSkyBox(DWORD sceneHandle, DWORD modelHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		return pModel->isSkyBox;
	}
	return FALSE;
}

/** 设置模型的颜色
*/
VOID CVGSCore::SetModelColor(DWORD sceneHandle, DWORD modelHandle, BYTE red, BYTE green, BYTE blue, BYTE alpha)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		pModel->SetColor(red, green, blue, alpha);
	}
}

/** 设置模型透明图
*/
VOID CVGSCore::SetModelOpacity(DWORD sceneHandle, DWORD modelHandle, INT opacity)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		pModel->SetOpacity(opacity);	
	}
}

/** 模型是否包含关键帧动画
*/
BOOL CVGSCore::IsModelHasKeyframeAnimation(DWORD sceneHandle, DWORD modelHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (vgsScene && pModel)
	{
		return pModel->clickable;
	}
	return FALSE;
}

/** 模型是否包含骨骼动画
*/
BOOL CVGSCore::IsModelHasSkeletonAnimation(DWORD sceneHandle, DWORD modelHandle)
{
	return 0;
}

////设置模型的颜色
//DLL_EXPORT VOID CVGSCore::SetModelColor(DWORD sceneHandle, DWORD modelHandle, INT red, INT green, INT blue, INT alpha);
//
////得到模型的透明度
//DLL_EXPORT INT GetModelOpacity(DWORD sceneHandle, DWORD modelHandle);
//
////设置模型的透明度
//DLL_EXPORT VOID CVGSCore::SetModelOpacity(DWORD sceneHandle, DWORD modelHandle, INT opacity);

// 子模型方法
/** 获取模型子模型使用的材质名称，每个子模型只能有一个材质
@param in sceneHandle ： 场景管理器
@param in modelHandle ：模型名指针
@return 子模型数目
**/
INT CVGSCore::GetSubModelCount(DWORD sceneHandle, DWORD modelHandle)
{
	CModel *pModel = (CModel*)modelHandle;
	if(pModel)
	{
		return pModel->getSubModelCount();
	}
	return 0;
}

// 获取模型子模型使用的材质名称，每个子模型只能有一个材质
BOOL CVGSCore::GetSubModelMaterialName(DWORD modelHandle, int index, CHAR *sName)
{
	CModel *pModel = (CModel*)modelHandle;
	if(pModel)
	{
		CMaterial *pMaterial = pModel->getSubModelMaterial(index);
		if (pMaterial)
		{
			COPY_NAME(sName, pMaterial->getName().c_str());
			return TRUE;
		}
	}
	return 0;
}

// 获取模型子模型使用的材质指针，每个子模型只能有一个材质
DWORD CVGSCore::GetSubModelMaterialHandle(DWORD modelHandle, int index)
{
	CModel *pModel = (CModel*)modelHandle;
	if(pModel)
	{
		return (DWORD)pModel->getSubModelMaterial(index);
	}
	return 0;
}

/** 为模型中的第index个子模型指定材质
@param in modelHandle : 场景管理器指针
@param in index : 子模型索引
@param in pMtrName : 材质名称
*/
VOID CVGSCore::SetSubModelMaterial(DWORD sceneHandle, DWORD modelHandle, int index, const CHAR* pMtrName)
{
	CScene *pScene = (CScene*)sceneHandle;
	CModel *pModel = (CModel*)modelHandle;
	if(pModel && pScene)
	{
		CMaterial *pMaterial = pScene->GetMaterialByName(pMtrName);
		if (pMaterial)
		{
			pModel->setSubModelMaterial(index, pMaterial);
		}
	}
}

/**为模型制定材质，如果模型包含多个子模型，那么所有子模型都会使用这个材质
@param in modelHandle : 场景管理器指针
@param in pMtrName : 材质名称
*/
VOID CVGSCore::SetModelMaterial(DWORD sceneHandle, DWORD modelHandle, const CHAR* pMtrName)
{
	CModel *pModel = (CModel*)modelHandle;
	if(pModel)
	{
		UINT count = pModel->getSubModelCount();
		for (UINT i = 0; i < count; ++i)
		{
			SetSubModelMaterial(sceneHandle, modelHandle, i, pMtrName);
		}
	}
}


//-------------------------------------------------------------------------------------------
///材质相关的方法
//-------------------------------------------------------------------------------------------
/** 创建材质
@param strMtrlName in : 要创建的材质名称
*/
DWORD CVGSCore::CreateMaterial(DWORD sceneHandle, const CHAR* strMtrlName)
{
	CScene* pScn = (CScene*)sceneHandle;
	if (pScn == NULL) return 0;

	EnterCriticalSection(&m_render_cs);

	CMaterial* pMtrl = 0;
	pMtrl = (CMaterial* )m_pRoot->GetMaterialManager().createMaterial(strMtrlName);

	pScn->m_vShader.push_back(pMtrl);

	LeaveCriticalSection(&m_render_cs);

	return (DWORD)pMtrl;
}

/** 删除材质
@param mtrlHandle in : 要删除的材质指针
*/
VOID CVGSCore::DeleteMaterialByHandle(DWORD sceneHandle, DWORD mtrlHandle)
{
	CScene* pScn = (CScene*)sceneHandle;
	if (pScn == NULL) return;

	CMaterial* pMtrl = (CMaterial* )mtrlHandle;
	m_pRoot->GetMaterialManager().removeResource(pMtrl);

	INT shaderCnt = pScn->m_vShader.size();
	std::vector<CMaterial*>::iterator ppMtrl = pScn->m_vShader.begin();
	for (INT i = 0; i< shaderCnt; i++)
	{
		/*CMaterial**/ pMtrl = *(ppMtrl + i);
		if ((DWORD)pMtrl == mtrlHandle)
		{
			EnterCriticalSection(&m_render_cs);

			// 检查材质是否正被使用，如果不被使用，就删除
/*
			//如果材质的贴图没有其他材质使用，则删除
			DWORD diffuseTex = (DWORD)pMtrl->diffuseTex;
			DWORD opacityTex = (DWORD)pMtrl->opacityTex;
			DWORD compTex = (DWORD)pMtrl->compTex;
			DWORD lightTex = (DWORD)pMtrl->lightTex;
			DWORD reflectionTex = (DWORD)pMtrl->reflectionTex;
			DWORD bumpTex = (DWORD)pMtrl->bumpTex;
			DWORD pBumpWater = (DWORD)pMtrl->pBumpWater;
			DWORD pCubeMap = (DWORD)pMtrl->pCubeMap;
			DWORD pSpecularCubeMap = (DWORD)pMtrl->pSpecularCubeMap;
			DWORD pMovieTexture = (DWORD)pMtrl->pMovieTexture;
			DWORD specularTex = (DWORD)pMtrl->specularTex;

			SAFE_RELEASE(pMtrl);

			if (IsTextureUsingByMaterial(sceneHandle, diffuseTex))
				DeleteTexture(sceneHandle, diffuseTex);
			if (IsTextureUsingByMaterial(sceneHandle, opacityTex))
				DeleteTexture(sceneHandle, opacityTex);
			if (IsTextureUsingByMaterial(sceneHandle, compTex))
				DeleteTexture(sceneHandle, compTex);
			if (IsTextureUsingByMaterial(sceneHandle, lightTex))
				DeleteTexture(sceneHandle, lightTex);
			if (IsTextureUsingByMaterial(sceneHandle, reflectionTex))
				DeleteTexture(sceneHandle, reflectionTex);

			if (IsTextureUsingByMaterial(sceneHandle, bumpTex))
				DeleteTexture(sceneHandle, bumpTex);
			if (IsTextureUsingByMaterial(sceneHandle, pBumpWater))
				DeleteBumpWater(sceneHandle, pBumpWater);
			if (IsTextureUsingByMaterial(sceneHandle, pCubeMap))
				DeleteCubeMap(sceneHandle, pCubeMap);
			if (IsTextureUsingByMaterial(sceneHandle, pSpecularCubeMap))
				DeleteCubeMap(sceneHandle, pSpecularCubeMap);
			if (IsTextureUsingByMaterial(sceneHandle, pMovieTexture))
				DeleteMovieTexture(sceneHandle, pMovieTexture);

			if (IsTextureUsingByMaterial(sceneHandle, specularTex))
				DeleteTexture(sceneHandle, specularTex);
*/
			// 			SAFE_DELETE(pMtrl);	

			pScn->m_vShader.erase(ppMtrl + i);
			LeaveCriticalSection(&m_render_cs);
			return;
		}
	}
	LeaveCriticalSection(&m_render_cs);
}

/** 删除材质
@param pName in : 要删除的材质名称
*/
VOID CVGSCore::DeleteMaterialByName(DWORD sceneHandle, const CHAR* pName)
{
	DWORD hMaterial = GetMaterialHandleByName(sceneHandle, pName);
	DeleteMaterialByHandle(sceneHandle, hMaterial);
}

/** 得到场景中材质的数量
*/
INT CVGSCore::GetMaterialCount(DWORD sceneHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return 0;

	return vgsScene->m_vShader.size();
}

/** 根据材质的名称得到材质的句柄
*/
DWORD CVGSCore::GetMaterialHandleByName(DWORD sceneHandle, const CHAR* strMtrlName)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return 0;

	return (DWORD)(m_pRoot->GetMaterialManager().getResource(strMtrlName));
}

/** 根据材质的编号得到材质的句柄
*/
DWORD CVGSCore::GetMaterialHandleByIndex(DWORD sceneHandle, INT index)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return 0;

	INT mtrlCnt = vgsScene->m_vShader.size();
	if (index >= mtrlCnt || index < 0)
		return 0;

	std::vector<CMaterial*>::iterator ppMtrl = vgsScene->m_vShader.begin();
	CMaterial* pMtrl = *(ppMtrl + index);
	return (DWORD)pMtrl;
}

/** 根据材质的句柄得到材质的名称
*/
VOID CVGSCore::GetMaterialNameByHandle(DWORD sceneHandle, DWORD mtrlHandle, CHAR* strMtrlName)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	if (pMtrl)
		COPY_NAME(strMtrlName, pMtrl->getName().c_str());
}
//获得材质信息
VOID CVGSCore::GetMaterialInfo(DWORD sceneHandle, DWORD mtrlHandle, MtrlInfo* pMtrlInfo)
{
	CScene *pScene = (CScene*)sceneHandle;
	CMaterial *pMaterial = (CMaterial*)mtrlHandle;
	if (pMtrlInfo && pMaterial)
	{
		ZeroMemory(pMtrlInfo, sizeof(MtrlInfo));

		COPY_NAME(pMtrlInfo->Name, pMaterial->getName().c_str());

		pMtrlInfo->bUseDiffuse = pMaterial->m_bUseDiffuse;
		pMtrlInfo->Diffuse.red = pMaterial->d3dmtrl.Diffuse.r * 255;
		pMtrlInfo->Diffuse.green = pMaterial->d3dmtrl.Diffuse.g * 255;
		pMtrlInfo->Diffuse.blue = pMaterial->d3dmtrl.Diffuse.b * 255;

		pMtrlInfo->Ambient.red   = pMaterial->d3dmtrl.Ambient.r * 255;
		pMtrlInfo->Ambient.green = pMaterial->d3dmtrl.Ambient.g * 255;
		pMtrlInfo->Ambient.blue  = pMaterial->d3dmtrl.Ambient.b * 255;

		pMtrlInfo->Specular.red   = pMaterial->specular.r * 255;
		pMtrlInfo->Specular.green = pMaterial->specular.g * 255;
		pMtrlInfo->Specular.blue  = pMaterial->specular.b * 255;
		pMtrlInfo->SpecularLevel  = pMaterial->specularLevel;
		pMtrlInfo->shininess      = pMaterial->d3dmtrl.Power;

		pMtrlInfo->Emissive = pMaterial->selfIllumination;

		pMtrlInfo->bAlphaTest    = pMaterial->bAlphaTest;
		pMtrlInfo->AlphaTestValue = pMaterial->AlpahaTestValue;
		pMtrlInfo->bTwoSide = pMaterial->twoSide;
		pMtrlInfo->FillMode = (INT)pMaterial->fillMode;
		pMtrlInfo->Opacity = pMaterial->getOpacity();
	}
	return;
}
//设置材质信息
VOID CVGSCore::SetMaterialInfo(DWORD sceneHandle, DWORD mtrlHandle, MtrlInfo* pMtrlInfo)
{
	CScene *pScene = (CScene*)sceneHandle;
	CMaterial *pMaterial = (CMaterial*)mtrlHandle;
	if (pScene && pMtrlInfo && pMaterial)
	{		
		BOOL bTransparent = pMaterial->IsTransparent();

		pMaterial->setName(std::string(pMtrlInfo->Name));
		pMaterial->m_bUseDiffuse = pMtrlInfo->bUseDiffuse;

		pMaterial->setOpacity(pMtrlInfo->Opacity); 

		float opacity = pMtrlInfo->Opacity / 100.0;
		pMaterial->d3dmtrl.Diffuse = D3DXCOLOR(pMtrlInfo->Diffuse.red / 255.0, pMtrlInfo->Diffuse.green / 255.0, pMtrlInfo->Diffuse.blue / 255.0, opacity);
		pMaterial->d3dmtrl.Ambient = D3DXCOLOR(pMtrlInfo->Ambient.red / 255.0, pMtrlInfo->Ambient.green / 255.0, pMtrlInfo->Ambient.blue / 255.0, 1.0f);
		pMaterial->specular = D3DXCOLOR(pMtrlInfo->Specular.red / 255.0, pMtrlInfo->Specular.green / 255.0, pMtrlInfo->Specular.blue / 255.0, opacity);

		float factor = pMtrlInfo->SpecularLevel / 100.0;		
		pMaterial->d3dmtrl.Specular = D3DXCOLOR(pMtrlInfo->Specular.red * factor / 255.0, pMtrlInfo->Specular.green * factor / 255.0, pMtrlInfo->Specular.blue * factor / 255.0, 1.0f);
		pMaterial->d3dmtrl.Power    = pMtrlInfo->shininess;

		pMaterial->selfIllumination = pMtrlInfo->Emissive;
		factor = pMtrlInfo->Emissive / 100.0;
		pMaterial->d3dmtrl.Emissive = D3DXCOLOR(pMtrlInfo->Diffuse.red * factor / 255.0, pMtrlInfo->Diffuse.green * factor / 255.0, pMtrlInfo->Diffuse.blue * factor / 255.0, opacity);

		pMaterial->bAlphaTest = pMtrlInfo->bAlphaTest;
		pMaterial->AlpahaTestValue = pMtrlInfo->AlphaTestValue;
		pMaterial->twoSide = pMtrlInfo->bTwoSide;
		pMaterial->fillMode = (_D3DFILLMODE)pMtrlInfo->FillMode;

		// 如果变为透明模型，需要添加到透明模型队列
		if (!bTransparent && pMaterial->IsTransparent())
		{
			vector<VGS_RenderSubmesh>::iterator it = pMaterial->subMeshList.begin();
			for (; it != pMaterial->subMeshList.end(); ++it)
			{
				pScene->AddModelToTransparentList((CModel*)it->modelPointer);
			}
		}
		else if (bTransparent && !pMaterial->IsTransparent()) // 由透明变为不透明
		{
			vector<VGS_RenderSubmesh>::iterator it = pMaterial->subMeshList.begin();
			for (; it != pMaterial->subMeshList.end(); ++it)
			{
				pScene->RemoveModelFormTransparentList((CModel*)it->modelPointer);
			}
		}
	}
	return;
}

//判断当前材质是否被模型关联
DWORD CVGSCore::IsMtrlUsingByModel(DWORD sceneHandle, DWORD mtrlHandle)
{
	return 0;
}

/** 获取指定的材质是否使用了贴图
*/
BOOL CVGSCore::IsMaterialHasTexture(DWORD sceneHandle, DWORD mtrlHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	CMaterial *pMaterial = (CMaterial *)mtrlHandle;
	if (pScene && pMaterial)
	{
		return pMaterial->getTexureLayerCount() > 0 ? TRUE:FALSE;
	}
	return FALSE;
}


///////////// texture layer ///////////////////////////////////
/** 获取材质的texture数目
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@return texture的数目 
*/
DWORD CVGSCore::GetMaterialTextureCount(DWORD sceneHandle, const CHAR* pMaterailName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
		if (pMaterial)
		{
			return pMaterial->getTexureLayerCount();
		}
	}
	return 0;
}

/** 获取材质的第textureIndex个texture句柄
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureIndex  in : texture索引
@return texture的句柄
*/
DWORD CVGSCore::GetMaterialTextureHandle(DWORD sceneHandle, const CHAR* pMaterailName, int textureIndex)
{
	if (sceneHandle)
	{
		CScene *pScene = (CScene*)sceneHandle;
		if (pScene)
		{
			CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
			if (pMaterial)
			{
				return (DWORD)pMaterial->getTextureLayerByIndex(textureIndex);
			}
		}
	}
	return 0;
}

/** 清空材质的一个texture内容, 清空后texture的指针依然有效
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureHandle in : texture句柄
*/
VOID CVGSCore::ClearMaterialTexture(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
		if (pMaterial)
		{
			pMaterial->clearTextureLayerByHandle((CTextureLayer *)textureHandle);
		}
	}
}

/** 删除材质的一层texture，删除后，该层texture之后的所有texture索引将会改变, <b>使用时一定要注意</b>。
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureHandle in : texture句柄
*/
VOID CVGSCore::RemoveMaterialTexture(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle)
{
	//注:当前不实现-linxq
	return;
}

/** 获取textureIndex的类型
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureHandle  in : texture句柄
@return texture的类型
*/
VGS_TextureType CVGSCore::GetTextureType(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
		CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
		if (pMaterial && pTxt)
		{
			return pTxt->type;
		}
	}
	return (VGS_TextureType)0;
}


/** 设置材质的动态UV速度
*/
// VOID CVGSCore::SetMaterialUVSpeed(DWORD sceneHandle, DWORD mtrlHandle, float USpeed, float VSpeed)
VOID CVGSCore::SetTextureUVSpeed(DWORD sceneHandle, DWORD textureHandle, float USpeed, float VSpeed)
{
	CTextureLayer *pLayer = (CTextureLayer*)textureHandle;
	if (pLayer)
	{
		pLayer->USpeed = USpeed;
		pLayer->VSpeed = VSpeed;
	}
}

/** 获取材质的动态UV速度
*/
// VOID CVGSCore::GetMaterialUVSpeed(DWORD sceneHandle, DWORD mtrlHandle, float &USpeed, float &VSpeed)
VOID CVGSCore::GetTextureUVSpeed(DWORD sceneHandle, DWORD textureHandle, float &USpeed, float &VSpeed)
{
	CTextureLayer *pLayer = (CTextureLayer*)textureHandle;
	if (pLayer)
	{
		USpeed = pLayer->USpeed;
		VSpeed = pLayer->VSpeed;
	}
	else
	{
		USpeed = VSpeed = 0;
	}
}


/** 获取textureIndex使用的UV坐标编号
@param sceneHandle   in : VGS场景指针
@param textureHandle  in : texture句柄
@return texture的UV坐标编号
*/
INT  CVGSCore::GetTextureUVIndex(DWORD sceneHandle, DWORD textureHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
		if (pTxt)
		{
			return pTxt->UVIndex;
		}
	}
	return 0;
}

/** 设置textureIndex使用的UV坐标编号
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param UVIndex		 in : texture使用的UV坐标编号
*/
VOID CVGSCore::SetTextureUVIndex(DWORD sceneHandle, DWORD textureHandle, INT UVIndex)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
		if (pTxt)
		{
			pTxt->UVIndex = UVIndex;
		}
	}
}

/** 获取贴图的Mipmaps数目
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture指针
@return 贴图的Mipmaps数目
*/
INT CVGSCore::GetTextureMipmaps(DWORD sceneHandle, DWORD textureHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		return pTxt->mipMap;
	}
	return 0;
}

/** 设置贴图的Mipmaps数目
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture指针
@param Mipmaps       in : 贴图的Mipmaps数目
*/
VOID CVGSCore::SetTextureMipmaps(DWORD sceneHandle, DWORD textureHandle, INT Mipmaps)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		pTxt->mipMap = Mipmaps;
	}
}


/** 设置texture的混色信息
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param BlenderInfo	 in : texture的混色参数
*/
VOID CVGSCore::SetTextureBlenderInfo(DWORD sceneHandle, DWORD textureHandle, VGS_BlenderType type, const VGS_BlenderInfo &BlenderInfo)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		if (VBT_COLOR == type) 
		{
			pTxt->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(BlenderInfo.option);
			pTxt->Blenderinfo.ColorBlendParam = BlenderInfo.param.BlenderOP_alpha;
		}
		else if (VBT_ALPHA == type)
		{
			pTxt->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(BlenderInfo.option);
			pTxt->Blenderinfo.AlphaBlendParam = BlenderInfo.param.BlenderOP_alpha;
		}
	}
}

/** 获取texture的混色信息
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param BlenderInfo	 out : texture的混色参数
*/
VOID CVGSCore::GetTextureBlenderInfo(DWORD sceneHandle, DWORD textureHandle, VGS_BlenderType type, VGS_BlenderInfo &BlenderInfo)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		if (VBT_COLOR == type)
		{
			BlenderInfo.option = _getVGSBlenderOperation(pTxt->Blenderinfo.ColorBlendOption);
			BlenderInfo.param.BlenderOP_alpha  = pTxt->Blenderinfo.ColorBlendParam;
		}
		else if (VBT_ALPHA == type)
		{
			BlenderInfo.option = _getVGSBlenderOperation(pTxt->Blenderinfo.AlphaBlendOption);
			BlenderInfo.param.BlenderOP_alpha  = pTxt->Blenderinfo.AlphaBlendParam;
		}
	}
}


///////////// texture ///////////////////////////////////


// 从Image buffer更新使用该Image的所有texture
BOOL CVGSCore::FillTexturesFromBuffer(UINT len, CHAR* pBuffer, INT imgIndex)
{
	if (len == 0 || !pBuffer)
	{
		return false;
	}

	// 遍历所有的texture
	UINT textureCount = m_pRoot->GetTextureManager().getResourceCount();
	for (UINT iTexture = 0; iTexture < textureCount; ++iTexture)
	{
		CTexture *pTexture = (CTexture*)m_pRoot->GetTextureManager().getResource(iTexture);
		if (pTexture && !pTexture->bCreated && pTexture->imageIdx == imgIndex)
		{
			m_pRoot->GetTextureManager().createTextureFromBuffer(pTexture, pBuffer, len, pTexture->isOpacity);
		}
	}
	return true;
}

//------------基本、透明、反射贴图-----------start

/** 为材质增加一层基本贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片名称，包括完成路径
@param pTextureName  in : texture层的名称，传入NULL表示自动命名
@param UVIndex		 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@return 新创建的texture句柄
*/
DWORD CVGSCore::AddMaterialBasalTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* pTextureFile, const CHAR* pTextureName/* = NULL*/, int UVIndex/* = 0*/)
{
	if (sceneHandle && pMaterailName && pTextureFile)
	{
		CScene *pScene = (CScene*)sceneHandle;
		if (pScene)
		{
			CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
			if (pMaterial)
			{
				std::string sTexName = "";
				if (pTextureName)
				{
					sTexName = std::string(pTextureName);
				}
				return (DWORD)pMaterial->createTextureLayer(std::string(pTextureFile), sTexName, UVIndex);
			}
		}
	}
	return 0;
}
/** 设置基本贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 in : texture使用的图像文件，包含完整路径
*/
VOID CVGSCore::SetBasalTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		CTexture* ptr = (CTexture *)(pTxt->pTexture);
		if (ptr)
		{
			ptr->ChangeImage(pTextureFile);
		}
	}
}
/** 获取基本贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
@return 得到贴图，返回TRUE,否则FALSE
*/
BOOL CVGSCore::GetBasalTextureImage(DWORD sceneHandle, DWORD textureHandle, CHAR* pTextureFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		CTexture* ptr = (CTexture *)(pTxt->pTexture);
		if (ptr)
		{
			std::string strFileName = ptr->getTextrueFileName();
			COPY_NAME(pTextureFile, strFileName.c_str());
			return TRUE;
		}
	}

	return FALSE;
}

/** 为材质增加一层透明贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片名称，包括完整路径
@param pTextureName  in : texture层的名称，传入NULL表示自动命名
@param UVIndex		 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@return 新创建的texture句柄
*/
DWORD CVGSCore::AddOpacityTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* pTextureFile, const CHAR* pTextureName/* = NULL*/, int UVIndex/* = 0*/)
{
	if (sceneHandle && pMaterailName && pTextureFile)
	{
		CScene *pScene = (CScene*)sceneHandle;
		if (pScene)
		{
			CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
			if (pMaterial)
			{
				std::string sTexName = "";
				if (pTextureName)
				{
					sTexName = std::string(pTextureName);
				}
				return (DWORD)pMaterial->createOpacityTextureLayer(std::string(pTextureFile), sTexName, UVIndex);
			}
		}
	}
	return 0;
}
VOID CVGSCore::SetTransparentTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		CTexture* ptr = (CTexture *)(pTxt->pTexture);
		if (ptr)
		{
			ptr->ChangeTransparentImage(pTextureFile);
		}
	}
}

/** 为材质增加一层反射贴图
@param hScene   in : VGS场景指针
@param szMtrl in : 材质名称
@param szTextureFile  in : texture使用的图片名称，包括完整路径
@param szTexture  in : texture层的名称，传入NULL表示自动命名
@param nUVindex	 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@param vrt	 in : 反射类型
@return 新创建的texture句柄
*/
DWORD CVGSCore::AddReflectTexture(DWORD hScene, const char* szMtrl, const char* szTextureFile, const char* szTexture, int nUVindex, VGS_REFLECT_TYPE vrt)
{
	if (hScene && szMtrl && szTextureFile)
	{
		CScene *pScene = (CScene*)hScene;
		if (pScene)
		{
			CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(szMtrl);
			if (pMaterial)
			{
				std::string sTexName = "";
				if (szTexture)
				{
					sTexName = std::string(szTexture);
				}
				CTextureLayer* pTxtrLayer = pMaterial->createTextureLayer(std::string(szTextureFile), sTexName, 0);
				if (pTxtrLayer)
				{
					switch (vrt)
					{
					case VRT_CURVED://曲面反射
						if (g_VGSRender.d3dParam.IsSupportTexGenSphereMap)
							pTxtrLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
						else
							pTxtrLayer->UVIndex = D3DTSS_TCI_CAMERASPACENORMAL;
						pTxtrLayer->transformFlag = D3DTTFF_COUNT3;
						break;
					case VRT_PLANAR://平面反射
						if (g_VGSRender.d3dParam.IsSupportTexGenSphereMap)
							pTxtrLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
						else
							pTxtrLayer->UVIndex = D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR;	
						pTxtrLayer->transformFlag = _D3DTEXTURETRANSFORMFLAGS(D3DTTFF_COUNT3 | D3DTTFF_PROJECTED);
						break;
					case VRT_INVALID:
					default:
						pTxtrLayer->transformFlag = D3DTTFF_DISABLE;
						pTxtrLayer->UVIndex = D3DTSS_TCI_PASSTHRU;
						break;
					}
					pTxtrLayer->type = TLT_REFLECT;
				}
				return (DWORD)pTxtrLayer;
			}
		}
	}
	return 0;
}
BOOL CVGSCore::SetReflectTextureType(DWORD hScene, DWORD hTexture, VGS_REFLECT_TYPE vrt)
{
	CScene *pScene = (CScene*)hScene;
	CTextureLayer* pTxtrLayer = (CTextureLayer *)hTexture;
	if (hScene && pTxtrLayer)
	{
		if (TLT_REFLECT != pTxtrLayer->type)
		{
			return false;
		}

		pTxtrLayer->m_refletionType = vrt;
		switch (vrt)
		{
		case VRT_CURVED://曲面反射
			if (g_VGSRender.d3dParam.IsSupportTexGenSphereMap)
				pTxtrLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
			else
				pTxtrLayer->UVIndex = D3DTSS_TCI_CAMERASPACENORMAL;
			pTxtrLayer->transformFlag = D3DTTFF_COUNT3;
			break;
		case VRT_PLANAR://平面反射
			if (g_VGSRender.d3dParam.IsSupportTexGenSphereMap)
				pTxtrLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
			else
				pTxtrLayer->UVIndex = D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR;	
			pTxtrLayer->transformFlag = _D3DTEXTURETRANSFORMFLAGS(D3DTTFF_COUNT3 | D3DTTFF_PROJECTED);
			break;
		case VRT_INVALID:
		default:
			pTxtrLayer->transformFlag = D3DTTFF_DISABLE;
			pTxtrLayer->UVIndex = D3DTSS_TCI_PASSTHRU;
			break;
		}
		return true;
	}
	return false;
}
VGS_REFLECT_TYPE CVGSCore::GetReflectTextureType(DWORD hScene, DWORD hTexture)
{
	CScene *pScene = (CScene*)hScene;
	CTextureLayer* pTxtrLayer = (CTextureLayer *)hTexture;
	if (hScene && pTxtrLayer)
	{
		if (TLT_REFLECT != pTxtrLayer->type)
			return VRT_INVALID;

		return pTxtrLayer->m_refletionType;
		/*if (g_VGSRender.d3dParam.IsSupportTexGenSphereMap)
		{
			if (D3DTSS_TCI_SPHEREMAP == pTxtrLayer->UVIndex && D3DTTFF_COUNT3 == pTxtrLayer->transformFlag)
				return VRT_CURVED;
			else if (D3DTSS_TCI_SPHEREMAP == pTxtrLayer->UVIndex && (D3DTTFF_COUNT3 | D3DTTFF_PROJECTED) == pTxtrLayer->transformFlag)
				return VRT_PLANAR;
			else
				return VRT_INVALID;
		}
		else
		{
			if (D3DTSS_TCI_CAMERASPACENORMAL == pTxtrLayer->UVIndex && D3DTTFF_COUNT3 == pTxtrLayer->transformFlag)
				return VRT_CURVED;
			else if (D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR == pTxtrLayer->UVIndex && (D3DTTFF_COUNT3 | D3DTTFF_PROJECTED) == pTxtrLayer->transformFlag)
				return VRT_PLANAR;
			else
				return VRT_INVALID;

		}*/
	}
	return VRT_INVALID;
}

//------------基本、透明、反射贴图-----------end

//------------动态贴图-----------start

/** 为材质增加一层动态贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param FrameCount  in : texture使用的图片序列的图像数目
@param pTextureFile  in : texture使用的图片序列，包括完成路径
@param Duration      in : 播放所有图片的时间长度，单位秒。例如，如果一共有5张图片，Duration = 1，那么平均200毫秒播放一张图片
@param UVIndex		 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@return 新创建的texture句柄
*/
DWORD CVGSCore::AddMaterialDynamicTexture(DWORD sceneHandle, const CHAR* pMaterailName, int FrameCount, DWORD* pTextureFile, int Duration, int UVIndex/* = 0*/)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene && pMaterailName && pTextureFile)
	{
		CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
		if (pMaterial)
		{
			//texture name
			std::string strName = "";
			int i = 0;
			while(true)
			{
				std::stringstream ss;
				ss<<pMaterailName<<"_"<<i;
				strName = ss.str();
				if (!m_pRoot->GetTextureManager().IsResourceExist(strName))
					break;
				i++;
			}
			//texture file name
			std::vector<std::string> vecFileName(FrameCount);
			for (i = 0; i < FrameCount; i++)
			{
				vecFileName[i] = (char *)(pTextureFile[i]);
			}
			return (DWORD)pMaterial->CreateDynamicTextureLayer(strName.c_str(), vecFileName,Duration,UVIndex);
		}
	}

	return 0;
}
/** 获取动态贴图所用图片的数量
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
*/
INT CVGSCore::GetDynamicTextureImageCount(DWORD sceneHandle, DWORD textureHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		CDynamicTex* ptr = (CDynamicTex *)(pTxt->pTexture);
		if (ptr)
			return ptr->GetImageCount();
	}
	return 0;
}
/** 获取动态贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param index		 in : 动态贴图序列中的图片索引
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
@return 得到图片，返回TRUE，否则FALSE
*/
BOOL CVGSCore::GetDynamicTextureImageByIndex(DWORD sceneHandle, DWORD textureHandle, INT index, CHAR* pTextureFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		CDynamicTex* ptr = (CDynamicTex *)(pTxt->pTexture);
		if (ptr)
		{
			std::string strFileName= ptr->GetFileName(index);
			strFileName = _GetPureFileNameStr(strFileName);
			COPY_NAME(pTextureFile, strFileName.c_str());
			return TRUE;
		}
	}
	return FALSE;
}

/** 设置动态贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param index		 in : 动态贴图序列中的图片索引
@param pTextureFile	 in : texture使用的图像文件， 包含完整路径
*/
VOID CVGSCore::SetDynamicTextureImageByIndex(DWORD sceneHandle, DWORD textureHandle, INT index, const CHAR* pTextureFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		CDynamicTex* ptr = (CDynamicTex *)(pTxt->pTexture);
		if (ptr)
		{
			ptr->ChangeImage(index, pTextureFile);
		}
	}
}
/** 为动态贴图重新设置所有使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param FrameCount    in : texture使用的图片数目
@param pTextureFile  in : texture使用的图片列表
*/
VOID CVGSCore::SetDynamicTextureImages(DWORD sceneHandle, DWORD textureHandle, int FrameCount, DWORD* pTextureFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		CDynamicTex* ptr = (CDynamicTex *)(pTxt->pTexture);
		if (ptr)
		{
			ptr->ClearImages();
			for (int i = 0; i < FrameCount; i++)
			{
				std::string strFileName = (char *)(pTextureFile[i]);
				ptr->AddImage(strFileName);
			}
		}
	}
}

/** 添加一张动态贴图图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 in : 要添加的texture图像文件， 包含完整路径
*/
VOID CVGSCore::AddDynamicTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		CDynamicTex* ptr = (CDynamicTex *)(pTxt->pTexture);
		if (ptr)
		{
			ptr->AddImage(pTextureFile);
		}
	}
}

/** 删除动态贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param index		 in : 动态贴图序列中的图片索引
@return 删除成功返回TRUE，否则FALSE
*/
BOOL CVGSCore::DeleteDynamicTextureImageByIndex(DWORD sceneHandle, DWORD textureHandle, INT FrameIdx)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		CDynamicTex* ptr = (CDynamicTex *)(pTxt->pTexture);
		if (ptr)
		{
			return ptr->DeleteImage(FrameIdx);
		}
	}
	return FALSE;
}

/** 设置动态贴图的播放周期长度，单位秒。 注意：如果图片序列为空，调用该方法是无效的。
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param Duration      in : 动态贴图序列播放一个周期所需的时间长度,单位秒
*/
VOID CVGSCore::SetDynamicTextureDuration(DWORD sceneHandle, DWORD textureHandle, INT Duration)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		CDynamicTex* ptr = (CDynamicTex *)(pTxt->pTexture);
		if (ptr)
			ptr->SetDuration(Duration);
	}
}
/** 获取动态贴图序列播放时间长度
@param sceneHandle	   in  : 场景管理器
@param textureHandle   in  : texture指针  
@return 动态贴图序列播放时间长度
*/
INT CVGSCore::GetDynamicDuration(DWORD sceneHandle, DWORD textureHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTxt = (CTextureLayer *)textureHandle;
	if (pScene && pTxt)
	{
		CDynamicTex* ptr = (CDynamicTex *)(pTxt->pTexture);
		if (ptr)
			return ptr->GetDuration();
	}
	return 0;
}
//------------动态贴图-----------end


//------------立方贴图-----------start
VGS_CubemapType CVGSCore::GetCubeTextureType(DWORD hScene, DWORD hTexture)
{
	CScene *pScene = (CScene*)hScene;
	CTextureLayer *pTxt = (CTextureLayer *)hTexture;
	if (pScene && pTxt)
	{
		if (TLT_CUBEMAP != pTxt->type)
		{
			return VCT_INVALID;
		}
		else
		{
			CCubeMap* ptr = (CCubeMap *)pTxt->pTexture;
			if (ptr)
			{
				return (VGS_CubemapType)(ptr->m_Mode);
			}
		}
	}
	return VCT_INVALID;
}

/** 为材质增加一层立方贴图
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片序列，包括完成路径。cubic必须包含6张图片，图片需要按照前后左右上下的顺序提供
@param pMaterailName in : 材质名称，如果为空，将自动命名
@return 新创建的texture句柄
*/
DWORD CVGSCore::AddMaterialCubicStaticTexture(DWORD sceneHandle, const CHAR* pMaterailName, DWORD* pTextureFile, const CHAR* pTextureName)
{
	if (NULL == sceneHandle || NULL == pMaterailName || NULL == pTextureFile)
		return 0;
	CScene *pScene = (CScene*)sceneHandle;
	CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
	if (pScene && pMaterial)
	{
		vector<std::string> vsCubeFiles;
		for (UINT i = 0; i < 6; ++i)
		{
			std::string sFile = std::string((char*)(pTextureFile[i]));
			vsCubeFiles.push_back(std::string((char*)(pTextureFile[i])));  // cube贴图文件列表
		}	
		return (DWORD)pMaterial->createStaticCubemapTextureLayer(pTextureName, vsCubeFiles, 128); // 创建cubemap
	}
	return 0;
}
/** 设置cubic贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 in : texture使用的图像文件序列，包含完整路径。必须是6张图片，而且按照前后左右上下的顺序提供
*/
VOID CVGSCore::SetCubicTextureImages(DWORD sceneHandle, DWORD textureHandle, DWORD* pTextureFile)
{
	if (NULL == sceneHandle || NULL == textureHandle || NULL == pTextureFile)
		return;
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		CCubeMap* ptr = (CCubeMap *)pTextureLayer->pTexture;
		if (ptr)
		{
			vector<std::string> vsCubeFiles;// cube贴图文件列表
			for (UINT i = 0; i < 6; ++i)
			{
				std::string sFile = std::string((char*)(pTextureFile[i]));
				vsCubeFiles.push_back(std::string((char*)(pTextureFile[i])));  
			}	
			ptr->FillImagesToTexture(vsCubeFiles);
		}
	}
}
/** 获取立方贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param faceType      in : 指定一个面
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
@return 得到贴图，返回TRUE,否则FALSE
*/
BOOL CVGSCore::GetCubicTextureImages(DWORD sceneHandle, DWORD textureHandle, VGS_CUBIC_FACE faceType, CHAR* pTextureFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		CCubeMap* ptr = (CCubeMap *)pTextureLayer->pTexture;
		if (ptr)
		{
			std::string strFileName= ptr->GetImageByIndex((int)faceType);
			strFileName = _GetPureFileNameStr(strFileName);
			COPY_NAME(pTextureFile, strFileName.c_str());
			return TRUE;
		}
	}
	return FALSE;
}

/** 为材质增加一层动态立方贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称，如果为空，将自动命名
@param pTextureName  in : 创建的texture名称
@return 新创建的texture句柄
*/
DWORD CVGSCore::AddMaterialCubicDynamicTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* pTextureName)
{
	CScene *pScene = (CScene*)sceneHandle;
	CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
	if (pScene && pMaterial)
	{
		CTextureLayer *pTextureLayer = pMaterial->createDynamicCubemapTextureLayer(pTextureName, 128); // 创建cubemap
				pScene->addDynamicCubemap((CCubeMap*)pTextureLayer->pTexture);
				return (DWORD)pTextureLayer;
	}
	return 0;
}

// 为动态cubmap添加反射模型
VOID CVGSCore::AddDynamicReflectedModel(DWORD sceneHandle, DWORD DynamicCubeHandle, DWORD modelHandle)
{
	if (sceneHandle && DynamicCubeHandle && modelHandle)
	{
		CTextureLayer *pLayer = (CTextureLayer*)DynamicCubeHandle;
		if (pLayer && pLayer->pTexture)
		{
			CCubeMap *pCubemap = (CCubeMap*)pLayer->pTexture;
			{
				if (pCubemap)
				{
					pCubemap->AddEnvModel(modelHandle);
				}
			}
		}
	}
}
// 移除动态cubemap的反射模型
VOID CVGSCore::RemoveDynamicReflectedModel(DWORD sceneHandle, DWORD DynamicCubeHandle, DWORD modelHandle)
{
	if (sceneHandle && DynamicCubeHandle && modelHandle)
	{
		CTextureLayer *pLayer = (CTextureLayer*)DynamicCubeHandle;
		if (pLayer && pLayer->pTexture)
		{
			CCubeMap *pCubemap = (CCubeMap*)pLayer->pTexture;
			{
				if (pCubemap)
				{
					pCubemap->removeEnvModel(modelHandle);
				}
			}
		}
	}
}
VOID CVGSCore::RemoveDynamicReflectedAllModel(DWORD hScene, DWORD hDynamicCubeMap)
{
	if (hScene && hDynamicCubeMap)
	{
		CTextureLayer *pLayer = (CTextureLayer*)hDynamicCubeMap;
		if (pLayer && pLayer->pTexture)
		{
			CCubeMap *pCubemap = (CCubeMap*)pLayer->pTexture;
			{
				if (pCubemap)
				{
					pCubemap->RemoveAllEnvModels();
				}
			}
		}
	}
}

int CVGSCore::GetDynamicReflectedModelCount(DWORD hScene, DWORD hDynamicCubeMap)
{
	CTextureLayer *pLayer = (CTextureLayer*)hDynamicCubeMap;
	if (hScene > 0 && pLayer && pLayer->pTexture)
	{
		CCubeMap *pCubemap = (CCubeMap*)pLayer->pTexture;
		{
			if (pCubemap)
			{
				return pCubemap->GetEnvModelCount();
			}
		}
	}
	return 0;
}
DWORD CVGSCore::GetDynamicReflectedModelHandle(DWORD hScene, DWORD hDynamicCubeMap, int nIdx)
{
	CTextureLayer *pLayer = (CTextureLayer*)hDynamicCubeMap;
	if (hScene > 0 && pLayer && pLayer->pTexture)
	{
		CCubeMap *pCubemap = (CCubeMap*)pLayer->pTexture;
		{
			if (pCubemap)
			{
				return pCubemap->GetEnvModelHandle(nIdx);
			}
		}
	}
	return 0;
}

//------------立方贴图-----------end

//------------bump贴图-----------start
/** 为材质增加一层bump贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片，包括完整路径。如果不指定图片，传入NULL或一个空字符串。
@param UVIndex		 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@return 新创建的texture句柄
*/
DWORD CVGSCore::AddMaterialBumpTexture(DWORD sceneHandle, const char* pMaterailName, const CHAR* pTextureFile, VGS_BumpType type, int UVIndex/* = 0*/)
{
	CScene *pScene = (CScene*)sceneHandle;
	CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
	if (pScene && pMaterial)
	{
		CTextureLayer *pTextureLayer = pMaterial->createBumpTextureLayer(NULL); 
		pScene->addBumpmap((CBumpMap*)pTextureLayer->pTexture);
		return (DWORD)pTextureLayer;
	}
	return 0;
}

DWORD CVGSCore::AddMaterialBumpEnviromentTexture(DWORD sceneHandle, const char* pMaterailName, const char* pTextureFile, float noise1, float noise2)
{
	CScene *pScene = (CScene*)sceneHandle;
	CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
	if (pScene && pMaterial)
	{
		CTextureLayer *pTextureLayer = pMaterial->createBumpTextureLayer(NULL); 
		pScene->addBumpmap((CBumpMap*)pTextureLayer->pTexture);
		((CBumpMap*)pTextureLayer->pTexture)->SetBumpAsEnviromentmap(pTextureFile, noise1, noise2);
		return (DWORD)pTextureLayer;
	}
	return 0;
}
DWORD CVGSCore::AddMaterialBumpNormalTexture(DWORD sceneHandle, const char* pMaterailName, const char* pTextureFile, DWORD lightHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
	if (pScene && pMaterial)
	{
		CTextureLayer *pTextureLayer = pMaterial->createBumpTextureLayer(NULL);
		pScene->addBumpmap((CBumpMap*)pTextureLayer->pTexture);
		((CBumpMap*)pTextureLayer->pTexture)->SetBumpAsNormalmap(pTextureFile, (CLight*)lightHandle);
		return (DWORD)pTextureLayer;
	}
	return 0;
}

DWORD CVGSCore::AddMaterialBumpwaveTexture(DWORD sceneHandle, const char* pMaterailName, INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY)
{
	CScene *pScene = (CScene*)sceneHandle;
	CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
	if (pScene && pMaterial)
	{
		CTextureLayer *pTextureLayer = pMaterial->createBumpTextureLayer(NULL); 
		pScene->addBumpmap((CBumpMap*)pTextureLayer->pTexture);
		((CBumpMap*)pTextureLayer->pTexture)->SetBumpAsWavemap(dwWidth, dwHeight, xSize, ySize, radius, speed, centerX, centerY);
		return (DWORD)pTextureLayer;
	}
	return 0;
}

VGS_BumpType CVGSCore::GetBumpTextureType(DWORD hScene, DWORD hBumpTexture)
{
	CScene *pScene = (CScene*)hScene;
	CTextureLayer *pTextureLayer = (CTextureLayer *)hBumpTexture;
	if (pScene && pTextureLayer)
	{
		CBumpMap* ptr = (CBumpMap *)(pTextureLayer->pTexture);
		return ptr->m_type;
	}
	return VBT_BUMPINVALID;
}
/** 获取bump贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
*/
BOOL CVGSCore::GetBumpTextureImage(DWORD sceneHandle, DWORD textureHandle, char* pTextureFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		if (pTextureLayer->pTexture)
		{
			std::string strFileName= pTextureLayer->pTexture->m_sFilePathandName;
			strFileName = _GetPureFileNameStr(strFileName);
			COPY_NAME(pTextureFile, strFileName.c_str());
			return TRUE;
		}
	}
	return FALSE;
}

/** 设置Bump贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 in : texture使用的图像文件，包含完整路径. 如果不需要图片，设置为NULL或空字符串。
*/
VOID CVGSCore::SetBumpTextureImage(DWORD sceneHandle, DWORD textureHandle, const char* pTextureFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer && pTextureFile)
	{
		CBumpMap* ptr = (CBumpMap *)(pTextureLayer->pTexture);
		if (ptr)
		{
			ptr->ChangeEnvironmentMapImageFile(pTextureFile, ptr->m_Noise1, ptr->m_Noise2);
		}
	}
}

/** 获取bump贴图的参数
@param type  out : bump的类型
@param param out : 参数
*/
BOOL CVGSCore::GetBumpTextureParam(DWORD sceneHandle, DWORD textureHandle, VGS_BumpParam &param)
{
	ZeroMemory(&param, sizeof(param));
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		CBumpMap* ptr = (CBumpMap *)(pTextureLayer->pTexture);
		switch (ptr->m_type)
		{
		case VBT_BUMPENVIRONMENT:
			param.envParam.width = ptr->width;
			param.envParam.height = ptr->height;
			param.envParam.noise1 = ptr->m_Noise1;
			param.envParam.noise2 = ptr->m_Noise2;
			return TRUE;
		case VBT_BUMPNORMARLMAP:
			param.normalParam.LightHandle = (DWORD)(ptr->m_Light);
			return TRUE;
		case VBT_BUMPWATER:
			param.waterParam.width =  ptr->m_MapWidth;
			param.waterParam.height = ptr->m_MapHeight;
			param.waterParam.XScale = ptr->m_xSize;
			param.waterParam.YScale = ptr->m_ySize;
			param.waterParam.Radius = ptr->m_radius;
			param.waterParam.Speed = ptr->m_speed;
			param.waterParam.CenterX = ptr->m_CenterX;
			param.waterParam.CenterY = ptr->m_CenterY;
			return TRUE;
		}
	}
	return FALSE;
}
/** 设置bump贴图的参数
*/
VOID CVGSCore::SetBumpTextureParam(DWORD sceneHandle, DWORD textureHandle, const VGS_BumpParam &param)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		CBumpMap* ptr = (CBumpMap *)(pTextureLayer->pTexture);
		switch (ptr->m_type)
		{
		case VBT_BUMPENVIRONMENT:
			ptr->SetEnvironmentMapNoise(param.envParam.noise1, param.envParam.noise2);
			break;
		case VBT_BUMPNORMARLMAP:
			ptr->SetBumpMapLight((CLight *)(param.normalParam.LightHandle));
			break;
		case VBT_BUMPWATER:
			ptr->SetBumpwaveParam(param.waterParam.width, param.waterParam.height,
				param.waterParam.XScale,param.waterParam.YScale,
				param.waterParam.Radius,param.waterParam.Speed,
				param.waterParam.CenterX,param.waterParam.CenterY);
			break;
		}
	}
}

/** 设置bump water的参数
@param sceneHandle      in : VGS场景指针
@param pTexture         in : texture句柄
@param dwWidth/dwHeight in : 贴图的尺寸
@param xSize/ySize      in :
@param radius:
@param speed:
@param centerX/centerY
*/
VOID CVGSCore::SetBumpwaveTextureParam(DWORD sceneHandle, DWORD pTexture, 
							  INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY)
{
	if (sceneHandle)
	{
		CScene *pScene = (CScene*)sceneHandle;
		if (pScene)
		{
			CTextureLayer *pLayer = (CTextureLayer*)pTexture;
			if (pLayer)
			{
				CBumpMap *pBumpwave = (CBumpMap*)(pLayer->pTexture);
				if(pBumpwave)
				{
					pBumpwave->SetBumpwaveParam(dwWidth, dwHeight, xSize, ySize, radius, speed, centerX, centerY);
				}
			}
		}
	}
}

/** 判断一层texture是否被用作bump
@param type out : 如果是bump，type返回bump的类型
@return 如果是bump，返回TRUE，否则FALSE
*/
BOOL CVGSCore::GetTextureAsBump(DWORD sceneHandle, DWORD textureHandle, VGS_BumpType &type)
{

	return FALSE;
}
/** 设置材质为bump
*/
VOID CVGSCore::SetTextureAsBump(DWORD sceneHandle, DWORD textureHandle, VGS_BumpType type)
{
}

//------------bump贴图-----------end

//------------实时反射/折射-----------start

// 增加一层实时反射层
DWORD CVGSCore::AddMaterialRTReflectionTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* sTextureLayerName, INT width/* = 256*/, INT height/* = 256*/)
{
	CScene *pScene = (CScene*)sceneHandle;
	CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMaterailName);
	if (pScene && pMaterial)
	{
		CTextureLayer *pLayer = pMaterial->CreateRTreflectTextureLayer(sTextureLayerName,width,height);
		if (pLayer)
		{
			CRTFMap* ptr = (CRTFMap*)pLayer->pTexture;
			pScene->m_vRtfTexture.push_back(ptr);
		}
		return (DWORD)pLayer;
	}
	return 0;
}
// 设置反射模型以获取反射平面
VOID CVGSCore::setRTReflectionModel(DWORD sceneHandle, DWORD textureHandle, DWORD hReflectionModel)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer && hReflectionModel != NULL)
	{
		CRTFMap* ptr = (CRTFMap *)(pTextureLayer->pTexture);
		if (ptr)
		{
			return ptr->SetRefPlaneByModel(hReflectionModel);
		}
	}
}

// 获取反射模型
DWORD CVGSCore::getRTReflectionModel(DWORD sceneHandle, DWORD textureHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		CRTFMap* ptr = (CRTFMap *)(pTextureLayer->pTexture);
		if (ptr)
		{
			return (DWORD)(ptr->m_pRefPlaneModel);
		}
	}
	return 0;
}
// 设置反射的模型列表
VOID CVGSCore::setRTReflectedModel(DWORD sceneHandle, DWORD textureHandle, DWORD count, DWORD* pReflectedModels)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		CRTFMap* ptr = (CRTFMap *)(pTextureLayer->pTexture);
		if (ptr)
		{
			ptr->m_refModelList.clear();

			for (int i = 0; i < count; i++)
			{
				ptr->AddRefModel(pReflectedModels[i]);
			}
		}
	}
}

// 获取被反射的模型数目
DWORD CVGSCore::getRTReflectedModelCount(DWORD sceneHandle, DWORD textureHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		CRTFMap* ptr = (CRTFMap *)(pTextureLayer->pTexture);
		if (ptr)
		{
			return ptr->GetReflectedModelCount();
		}
	}
	return 0;
}

// 获取被反射的模型
DWORD CVGSCore::getRTReflectedModelByIndex(DWORD sceneHandle, DWORD textureHandle, INT index)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		CRTFMap* ptr = (CRTFMap *)(pTextureLayer->pTexture);
		if (ptr)
		{
			return ptr->GetReflectedModelByIndex(index);
		}
	}
	return 0;
}
// 获取反射贴图的尺寸
BOOL CVGSCore::getRTReflectionDimension(DWORD sceneHandle, DWORD textureHandle, INT &width, INT &height)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		CRTFMap* ptr = (CRTFMap *)(pTextureLayer->pTexture);
		if (ptr)
		{
			width = ptr->m_width;
			height = ptr->m_height;
			return TRUE;
		}
	}

	return FALSE;
}

/** 设置反射贴图的尺寸
*/
VOID CVGSCore::setRTReflectionDimension(DWORD sceneHandle, DWORD textureHandle, INT width, INT height)
{
	CScene *pScene = (CScene*)sceneHandle;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		CRTFMap* ptr = (CRTFMap *)(pTextureLayer->pTexture);
		if (ptr)
		{
			ptr->SetSize(width, height);
		}
	}
}

// 增加一层实时折射层
DWORD CVGSCore::AddMaterialRTRefractionTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* sTextureLayerName, INT width/* = 256*/, INT height/* = 256*/)
{
	return 0;
}


// 设置图层为实时折射
VOID CVGSCore::setTextureAsRTRefraction(DWORD sceneHandle, DWORD textureHandle)
{
	
}
// 设置折射模型以获取折射平面
VOID CVGSCore::setRTRefractionModel(DWORD sceneHandle, DWORD textureHandle, DWORD hRefractionModel)
{
}

// 获取折射模型
DWORD CVGSCore::getRTRefractionModel(DWORD sceneHandle, DWORD textureHandle)
{
	DWORD re = 0;
	

	return re;
}
// 设置折射的模型列表
VOID CVGSCore::setRTRefractedModel(DWORD sceneHandle, DWORD textureHandle, DWORD count, DWORD* phRefractedModels)
{
}
// 获取被折射的模型数目
DWORD CVGSCore::getRTRefractedModelCount(DWORD sceneHandle, DWORD textureHandle)
{
	DWORD re = 0;
	return re;
}

// 获取被反射的模型
DWORD CVGSCore::getRTRefractedModelByIndex(DWORD sceneHandle, DWORD textureHandle, INT index)
{
	BOOL re = FALSE;
	return re;
}

// 获取折射贴图的尺寸
BOOL CVGSCore::getRTRefractionDimension(DWORD sceneHandle, DWORD textureHandle, INT &width, INT &height)
{
	BOOL re = FALSE;

	return re;
}
/** 设置折射贴图的尺寸
*/
VOID CVGSCore::setRTRefractionDimension(DWORD sceneHandle, DWORD textureHandle, INT width, INT height)
{

}
// 设置贴图层为实时反射
VOID CVGSCore::setTextureAsRTReflection(DWORD sceneHandle, DWORD textureHandle)
{
}

//------------实时反射/折射-----------end
//---------Flash Texture Start----linxq 2009-12-15
DWORD CVGSCore::AddMaterialFlashTexture(DWORD hScene, const char* pMtrlName, const char* pTxtrName, const char* pFlashPath,
							  int flashWidth, int flashHeight, int nWidth, int nHeight)
{
	if (NULL == hScene || NULL == pMtrlName || NULL == pFlashPath)
		return 0;
	CScene *pScene = (CScene*)hScene;
	CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(pMtrlName);
	if (pScene && pMaterial)
	{
		return (DWORD)pMaterial->CreateFlashTextureLayer(pTxtrName, pFlashPath, flashWidth, flashHeight, nWidth, nHeight); 
	}
	return NULL;
}
VOID CVGSCore::SetFlashTexturePlayStatus(DWORD hScene, DWORD textureHandle, int nStatus)
{
	CScene *pScene = (CScene*)hScene;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		SwfTexture* ptr = (SwfTexture *)(pTextureLayer->pTexture);
		if (ptr)
		{
			switch (nStatus)
			{				
			case 0: //暂停
				ptr->Pause();
				break;				
			case 1: //播放
				ptr->Play(ptr->m_bLoop);
				break;				
			case 2: //停止
				ptr->Stop();
				break;
			}
		}
	}
}
VOID CVGSCore::SetFlashTextureGoToFrame(DWORD hScene, DWORD textureHandle, int nFrameIndex)
{
	CScene *pScene = (CScene*)hScene;
	CTextureLayer *pTextureLayer = (CTextureLayer *)textureHandle;
	if (pScene && pTextureLayer)
	{
		SwfTexture* ptr = (SwfTexture *)(pTextureLayer->pTexture);
		if (ptr)
		{
			ptr->GoToFrame(nFrameIndex);
		}
	}
}

//---------Flash Texture end----



/** 设置纹理的大小
@param sceneHandle  in  : 场景管理器
@param mtrlHandle   in  : 材质指针  
@param mapChanel    in  : 贴图通道 
@param width/height out : 图片的大小
*/
//DLL_EXPORT VIOD SetTextureSize(DWORD sceneHandle, DWORD textureHandle, INT width, INT height);

/** 获取纹理的大小
@param sceneHandle  in  : 场景管理器
@param mtrlHandle   in  : 材质指针  
@param mapChanel    in  : 贴图通道 
@param width/height out : 图片的大小
@return TRUE：返回的width/heigh有效，FALSE:返回的width/heigh无效
*/
BOOL CVGSCore::GetTextureSize(DWORD sceneHandle, DWORD textureHandle, INT &width, INT &height, INT iFrmIndex/* = 0*/)
{
	return FALSE;
}


///** 获取贴图图像的大小
//@param sceneHandle  in  : 场景管理器
//@param mtrlHandle   in  : 材质指针  
//@param mapChanel    in  : 贴图通道 
//@param width/height out : 图片的大小
//@return TRUE：返回的width/heigh有效，FALSE:返回的width/heigh无效
//*/
//DLL_EXPORT BOOL CVGSCore::GetTextureSize(DWORD sceneHandle, DWORD mtrlHandle, INT mapChanel, INT &width, INT &height);

//设置材质的颜色
VOID CVGSCore::SetMaterialColor(DWORD sceneHandle, DWORD mtrlHandle, INT red, INT green, INT blue, INT alpha)
{
	return ;
}

//设置是否显示材质的贴图
VOID CVGSCore::SetShowMtrlTexture(DWORD sceneHandle, DWORD mtrlHandle, BOOL showTexture)
{
}
//获得是否显示贴图的状态
BOOL CVGSCore::GetShowMtrlTexture(DWORD sceneHandle, DWORD mtrlHandle)
{
	return FALSE;
}

//-------------------------------------------------------------------------------------------
///材质与纹理----------------end--
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
///其他与图像相关的接口----------------start--
//-------------------------------------------------------------------------------------------

/** 得到场景中贴图的数量, 包括2D、3D使用的贴图
*/ 
// DLL_EXPORT UINT GetTextureCount(DWORD sceneHandle);
UINT CVGSCore::GetTextureImageCount(DWORD sceneHandle)
{
	//TODO:当前未实现，需要读取文件后实现，考虑实现一个单独记录图像文件的管理器. linxq 2009-12-14
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		g_setImage.clear();
		UINT nCount = m_pRoot->GetMaterialManager().getResourceCount();
		for(int i = 0; i < nCount; i++)
		{
			CMaterial *pMaterial = (CMaterial *)m_pRoot->GetMaterialManager().getResource(i);
			if (pMaterial)
			{
				int nTotal = pMaterial->m_vpTexLayers.size();
				for(int j = 0; j < nTotal; j++)
				{
					CTextureLayer *ptr = pMaterial->m_vpTexLayers[j];
					if(ptr && ptr->pTexture)
					{
						switch (ptr->type)
						{
						case TLT_BASAL:
						case TLT_OPACITY:
						case TLT_REFLECT:
							if (!ptr->pTexture->m_sName.empty())
								g_setImage.insert(ptr->pTexture->m_sName);
							break;
						case TLT_CUBEMAP:
							{
								CCubeMap* pTexture = (CCubeMap*)(ptr->pTexture);
								std::vector<std::string> strVec(pTexture->m_vecFileName.size());
								std::transform(pTexture->m_vecFileName.begin(), pTexture->m_vecFileName.end(), strVec.begin(), _GetPureFileNameStr);
								g_setImage.insert(strVec.begin(), strVec.end());
							}
							break;
						case TLT_DYNAMIC:
							{
								CDynamicTex* pTexture = (CDynamicTex*)(ptr->pTexture);
								std::vector<std::string> strVec(pTexture->m_vecFileName.size());
								std::transform(pTexture->m_vecFileName.begin(), pTexture->m_vecFileName.end(), strVec.begin(), _GetPureFileNameStr);
								g_setImage.insert(strVec.begin(), strVec.end());
							}
							break;
						case TLT_BUMPMAP:
							{
								CBumpMap* pTexture = (CBumpMap*)(ptr->pTexture);
								switch(pTexture->m_type)
								{
								case VBT_BUMPENVIRONMENT:
								case VBT_BUMPNORMARLMAP:
									{
										string str = _GetPureFileNameStr(pTexture->m_sFilePathandName);
										g_setImage.insert(str);
									}
									break;
								}
							}
							break;
						default:
							break;
						}
					}
				}
			}
		}
		//gui
		CGUIManager& guiMgr = pScene->GetGuiManagerRef();
		BackGround* pBg = guiMgr.GetBackDropPtr();
		if (pBg)
		{
			string strImg;
			pBg->GetBaseImage(strImg);
			if (!strImg.empty())
				g_setImage.insert(strImg);
		}
		vector<CGUIObject*> vecGuis = guiMgr.GetAllObjectsList();
		vector<CGUIObject*>::iterator it = vecGuis.begin(), end = vecGuis.end();
		for(; it != end; ++it)
		{
			switch((*it)->GetType())
			{
			case VGS_GUIOBJ_TYPE_OVERLAY:
				{
					string str1, str2;
					((COverlay*)(*it))->GetOverlayImageFile(str1, str2);
					if (!str1.empty())
						g_setImage.insert(str1);
					if (!str2.empty())
						g_setImage.insert(str2);					
				}
				break;
			case VGS_GUIOBJ_TYPE_BUTTON:
				{
					CButton* btn = (CButton*)(*it);
					string str;
					btn->GetMouseUpImageFileName(str);
					if (!str.empty())
						g_setImage.insert(str);
					btn->GetMouseDownImageFileName(str);
					if (!str.empty())
						g_setImage.insert(str);
					btn->GetMouseOverImageFileName(str);
					if (!str.empty())
						g_setImage.insert(str);
				}
				break;
			case VGS_GUIOBJ_TYPE_NAVIGATION:
				{
					CNavigator* ptr = (CNavigator*)(*it);
					string str1, str2;
					ptr->GetOverlayImageFile(str1, str2);
					if (!str1.empty())
						g_setImage.insert(str1);
					//pointer
					for (int i = 0; i < ptr->GetPointerCount(); i++)
					{
						ptr->GetPointerByIndex(i)->GetOverlayImageFile(str1, str2);
						if (!str1.empty())
							g_setImage.insert(str1);
					}
				}
				break;
			case VGS_GUIOBJ_TYPE_NAVIPOINTOR:
				break;
			}
		}
	return g_setImage.size();
	}

	return 0;
}

/** 根据索引得到纹理的文件名称（不包含路径）
@param in sceneHandle : 场景指针
@param in index : 纹理在队列中的索引
@param out TextureName : 纹理名称
*/
// DLL_EXPORT BOOL CVGSCore::GetTextureNamebyIndex(DWORD sceneHandle, INT index, CHAR* TextureName);
BOOL CVGSCore::GetTextureImageNamebyIndex(DWORD sceneHandle, INT index, CHAR* TextureName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene && index >= 0 && index < g_setImage.size())
	{
		string str("");
		int i  = 0;
		set<string>::iterator it = g_setImage.begin(), end = g_setImage.end();
		for (; it != end; ++it)
		{
			if (i++ == index)
			{
				str = *it;
				break;
			}
		}
		if (!str.empty())
		{
			COPY_NAME(TextureName, str.c_str());
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

/** 图像是否正被使用
@param ImagePureName in : 图像文件名称，不包含路径
*/
BOOL CVGSCore::IsImageInused(DWORD sceneHandle, const CHAR* ImagePureName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		if (0 == ImagePureName)
			return FALSE;
		string str(ImagePureName);
		if (str.empty())
			return FALSE;
		set<string>::const_iterator it = g_setImage.find(str);
		if (it != g_setImage.end())
			return TRUE;
	}
	return FALSE;
}
//-------------------------------------------------------------------------------------------
///其他与图像相关的接口----------------end--
//-------------------------------------------------------------------------------------------


/** 创建天空盒
*/
VOID CVGSCore::CreateSkyBox(DWORD sceneHandle, const CHAR* sTextureName)
{
}

/** 设置天空盒使用的贴图
*/
VOID CVGSCore::ChangeSetSkyBoxTexture(DWORD sceneHandle, const CHAR* sTextureName)
{
}

/** 设置天空盒的距离
*/
VOID CVGSCore::SetSkyBoxDistance(DWORD sceneHandle, FLOAT dist)
{
}

/** 设置天空盒是否有效
*/
VOID CVGSCore::SetSkyBoxEnabled(DWORD sceneHandle, BOOL bEnabled)
{
}

/** 使用指定图片创建背景图,每个场景只能有一张背景图。重复设置背景图将清除之前已经创建的背景图。
*/
DWORD CVGSCore::SetBackground(DWORD sceneHandle, const CHAR* sImageFile)
{	
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().CreateBackGroudLayer(sImageFile);
		return (DWORD)ptr;
	}
	return 0;
}

// 获取背景图句柄，每个场景只能有一张背景图
DWORD CVGSCore::GetBackgroundHandle(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		return (DWORD)(pScene->GetGuiManagerRef().GetBackDropPtr());
	}
	return 0;
}

/** 删除场景背景图
*/
VOID CVGSCore::RemoveBackground(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pScene->GetGuiManagerRef().RemoveBackDrop();
	}
}

/** 获取背景图使用的图像名称，不包括路径
*/
BOOL CVGSCore::GetBackgroundImage(DWORD sceneHandle, CHAR *pImageFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		BackGround* ptr = pScene->GetGuiManagerRef().GetBackDropPtr();
		if (ptr)
		{
			std::string str;
			ptr->GetBaseImage(str);
			COPY_NAME(pImageFile, str.c_str());
		}
	}
	return FALSE;
}  

/** 设置场景的背景图是否可见
*/
VOID CVGSCore::SetBackgroundVisibility(DWORD sceneHandle, BOOL bVisible)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		BackGround* ptr = pScene->GetGuiManagerRef().GetBackDropPtr();
		if (ptr)
		{
			ptr->SetVisibility(bVisible);
		}
	}
}

/** 获取场景的背景图是否可见
*/
BOOL CVGSCore::IsBackgroundVisible(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		BackGround* ptr = pScene->GetGuiManagerRef().GetBackDropPtr();
		if (ptr)
		{
			return ptr->GetVisibility();
		}
	}
	return FALSE;
}

/** 设置背景图的相对与视口的相对大小和相对位置，left/top/width/height取值范围[0, 1]，坐标原点在视口的左上角
建议使用FRect结构传递参数.
*/
VOID CVGSCore::SetBackgroundRect(DWORD sceneHandle, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		BackGround* ptr = pScene->GetGuiManagerRef().GetBackDropPtr();
		if (ptr)
		{
			ptr->SetRect(left, top, width, height);
		}
	}
} 

/** 获取背景图的相对与视口的相对大小和相对位置，left/top/width/height取值范围[0, 1]，坐标原点在视口的左上角
建议使用FRect结构传递参数.
*/
VOID CVGSCore::GetBackgroundRect(DWORD sceneHandle, FLOAT &left, FLOAT &top, FLOAT &width, FLOAT &height)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		BackGround* ptr = pScene->GetGuiManagerRef().GetBackDropPtr();
		if (ptr)
		{
			ptr->GetRect(left, top, width, height);
		}
	}
}

/** 背景图全屏显示
*/
VOID CVGSCore::FullScreenBackground(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		BackGround* ptr = pScene->GetGuiManagerRef().GetBackDropPtr();
		if (ptr)
		{
			ptr->FullScreen();
		}
	}
}

//-------------------------------------------------------------------------------------------
///摄像机相关的方法
//-------------------------------------------------------------------------------------------
/** 使用缺省参数创建一个相机
*/
DWORD CVGSCore::CreateCamera(DWORD sceneHandle, const CHAR* strCameraName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CCamera *pCamera = pScene->CreateCamera(std::string(strCameraName));
		return (DWORD)pCamera;
	}
	return 0;
}

/** 创建各种预定义相机
*/
DWORD CVGSCore::CreateFrontCamera(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CCamera *pCamera = pScene->CreateCamera(std::string(CAMERA_NAME_FRONT));
		
		D3DXVECTOR3 vFrom(0, 0, -100);
		D3DXVECTOR3 vAt(0, 0, 0);
		pCamera->SetPosition(&vFrom, VTS_WORLD);
		pCamera->Lookat(&vAt, VTS_WORLD);
		pCamera->SetRoll(0);
		pCamera->m_camType = VCT_ORTHO; // 轴侧相机

		return (DWORD)pCamera;
	}
	return 0;
}
DWORD CVGSCore::CreateBackCamera(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CCamera *pCamera = pScene->CreateCamera(std::string(CAMERA_NAME_BACK));
		
		D3DXVECTOR3 vFrom(0, 0, 100);
		D3DXVECTOR3 vAt(0, 0, 0);
		pCamera->SetPosition(&vFrom, VTS_WORLD);
		pCamera->Lookat(&vAt, VTS_WORLD);
		pCamera->SetRoll(0);
		pCamera->m_camType = VCT_ORTHO; // 轴侧相机

		return (DWORD)pCamera;
	}
	return 0;
}
DWORD CVGSCore::CreateTopCamera(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CCamera *pCamera = pScene->CreateCamera(std::string(CAMERA_NAME_TOP));
		
		D3DXVECTOR3 vFrom(0, 100, 0);
		D3DXVECTOR3 vAt(0, 0, 0);
		pCamera->SetPosition(&vFrom, VTS_WORLD);
		pCamera->Lookat(&vAt, VTS_WORLD);
		pCamera->SetRoll(0);
		pCamera->m_camType = VCT_ORTHO; // 轴侧相机

		return (DWORD)pCamera;
	}
	return 0;
}
DWORD CVGSCore::CreateBottomCamera(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CCamera *pCamera = pScene->CreateCamera(std::string(CAMERA_NAME_BOTTOM));
		
		D3DXVECTOR3 vFrom(0, -100, 0);
		D3DXVECTOR3 vAt(0, 0, 0);
		pCamera->SetPosition(&vFrom, VTS_WORLD);
		pCamera->Lookat(&vAt, VTS_WORLD);
		pCamera->SetRoll(0);
		pCamera->m_camType = VCT_ORTHO; // 轴侧相机

		return (DWORD)pCamera;
	}
	return 0;

} 
DWORD CVGSCore::CreateLeftCamera(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CCamera *pCamera = pScene->CreateCamera(std::string(CAMERA_NAME_LEFT));
		
		D3DXVECTOR3 vFrom(-100, 0, 0);
		D3DXVECTOR3 vAt(0, 0, 0);
		pCamera->SetPosition(&vFrom, VTS_WORLD);
		pCamera->Lookat(&vAt, VTS_WORLD);
		pCamera->SetRoll(0);
		pCamera->m_camType = VCT_ORTHO; // 轴侧相机

		return (DWORD)pCamera;
	}
	return 0;
}
DWORD CVGSCore::CreateRightCamera(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CCamera *pCamera = pScene->CreateCamera(std::string(CAMERA_NAME_RIGHT));
		
		D3DXVECTOR3 vFrom(100, 0, 0);
		D3DXVECTOR3 vAt(0, 0, 0);
		pCamera->SetPosition(&vFrom, VTS_WORLD);
		pCamera->Lookat(&vAt, VTS_WORLD);
		pCamera->SetRoll(0);
		pCamera->m_camType = VCT_ORTHO; // 轴侧相机

		return (DWORD)pCamera;
	}
	return 0;
}
DWORD CVGSCore::CreatePerspectiveCamera(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CCamera *pCamera = pScene->CreateCamera(std::string(CAMERA_NAME_PERSPECTIVE));
		return DWORD(pCamera);
	}
	return 0;
}

// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD CVGSCore::GetFrontCamera(DWORD sceneHandle)
{
	return GetCameraHandleByName(sceneHandle, (CHAR*)CAMERA_NAME_FRONT);
}
// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD CVGSCore::GetBackCamera(DWORD sceneHandle)
{
	return GetCameraHandleByName(sceneHandle, (CHAR*)CAMERA_NAME_BACK);
}
// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD CVGSCore::GetTopCamera(DWORD sceneHandle)
{
	return GetCameraHandleByName(sceneHandle, (CHAR*)CAMERA_NAME_TOP);
}
// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD CVGSCore::GetBottomCamera(DWORD sceneHandle)
{
	return GetCameraHandleByName(sceneHandle, (CHAR*)CAMERA_NAME_BOTTOM);
}
// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD CVGSCore::GetLeftCamera(DWORD sceneHandle)
{
	return GetCameraHandleByName(sceneHandle, (CHAR*)CAMERA_NAME_LEFT);
}
// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD CVGSCore::GetRightCamera(DWORD sceneHandle)
{
	return GetCameraHandleByName(sceneHandle, (CHAR*)CAMERA_NAME_RIGHT);
}
// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD CVGSCore::GetPerspectiveCamera(DWORD sceneHandle)
{
	return GetCameraHandleByName(sceneHandle, (CHAR*)CAMERA_NAME_PERSPECTIVE);
}

//得到场景中摄像机的数量
INT CVGSCore::GetCameraCount(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		return pScene->m_vCamera.size();
	}
	return 0;
}

//根据像机的名称得到像机的句柄
DWORD CVGSCore::GetCameraHandleByName(DWORD sceneHandle, CHAR* strCameraName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		return (DWORD)pScene->GetCameraByName(std::string(strCameraName));
	}
	return 0;
}
//根据摄像机的编号得到像机的句柄
DWORD CVGSCore::GetCameraHandleByIndex(DWORD sceneHandle, INT index)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		return (DWORD)pScene->GetCameraByIndex(index);
	}
	return 0;
}

//根据像机的句柄得到像机的名称
VOID CVGSCore::GetCameraNameByHandle(DWORD sceneHandle, DWORD camHandle, CHAR* strCameraName)
{
	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		COPY_NAME(strCameraName, pCamera->getName().c_str());
	}
}
//设置相机的名称
VOID CVGSCore::SetCameraNamebyHandle(DWORD sceneHandle, DWORD camHandle, CHAR* strCameraName)
{
	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		pCamera->setName(std::string(strCameraName));
	}
}
VOID CVGSCore::SetCameraNamebyName(DWORD sceneHandle, CHAR* strOldCameraName, CHAR* strNewCameraName)
{
	DWORD camHandle = GetCameraHandleByName(sceneHandle, strOldCameraName);
	SetCameraNamebyHandle(sceneHandle, camHandle, strNewCameraName);
}


/** 根据相机的索引得到相机的名字
*/
BOOL CVGSCore::GetCameraNameByIndex(DWORD sceneHandle, INT index, CHAR* pName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		std::string str = pScene->GetCameraNameByIndex(index);
		strcpy(pName, str.c_str());
		return TRUE;
	}
	return FALSE;
}


/**删除相机
*/
VOID CVGSCore::DeleteCamerabyHandle(DWORD sceneHandle, DWORD camHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pScene->DeleteCamera((CCamera*)camHandle);
	}
}
/**删除相机
*/
VOID CVGSCore::DeleteCamerabyName(DWORD sceneHandle, CHAR* camName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		DWORD camHandle = GetCameraHandleByName(sceneHandle, camName);
		pScene->DeleteCamera(camHandle);
	}
}

//设置当前摄像机
VOID CVGSCore::SetCurrentCamera(DWORD sceneHandle, DWORD swapChainHandle, DWORD camHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CSwapChain* pSwapChain = (CSwapChain*)swapChainHandle;
		if (pSwapChain)
		{
			pSwapChain->GetViewport(0)->SetCamera((CCamera*)camHandle);
		}
	}
}
//得到当前摄像机句柄
DWORD CVGSCore::GetCurrentCamera(DWORD sceneHandle, DWORD swapChainHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CSwapChain* pSwapChain = (CSwapChain*)swapChainHandle;
		if (pSwapChain)
		{
			return (DWORD)pSwapChain->GetViewport(0)->GetCamera();
		}
	}
	return 0;
}

//设置当前相机为系统预设相机
// DLL_EXPORT VOID CVGSCore::SetCurrentCameraToPreConfig(DWORD sceneHandle, DWORD swapChainHandle, CAMERATYPE cameraType);

//获得当前相机的是否为预设相机，如果不是返回NONE
// DLL_EXPORT CAMERATYPE GetCurrentPreConfigCamera(DWORD sceneHandle, DWORD swapChainHandle);


/** 平移摄像机
@param in sceneHandle 场景管理器
@param in camHandle 相机句柄
@param in pVec 相机移动的相对向量
@param in coordEnum 参考坐标系
	//0 - 在世界坐标系平移摄像机
	//1 - 在自身坐标系平移
	//2 - walk，不能飞到天上去
*/
VOID CVGSCore::MoveCamera(DWORD sceneHandle, DWORD camHandle, VECTOR3* pVec, UINT coordEnum)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CCamera* pCamera = (CCamera*)camHandle;
	D3DXVECTOR3 d3dVec(pVec->x, pVec->y, pVec->z);
	pCamera->Move(d3dVec, (VGS_TRANSFORMSPACE)coordEnum);  // 平移相对一段距离
	// pCamera->m_pNode->translate(&D3DXVECTOR3(pVec->x, pVec->y, pVec->z), coordEnum);
}

/** 旋转摄像机
@param in pVec : 各个方向上的旋转角度
@param in coordEnum : 参考坐标系
//0 - 绕世界坐标系旋转摄像机
//1 - 绕自身坐标系旋转像机
//2 - 绕穿过from中心，与世界坐标系平行的坐标系旋转
*/
VOID CVGSCore::RotateCamera(DWORD sceneHandle, DWORD camHandle, VECTOR3* pVec, UINT coordEnum)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera->m_pNode)
	{
		pCamera->m_pNode->rotate(&D3DXVECTOR3(pVec->x*ATOS, pVec->y*ATOS, pVec->z*ATOS), (VGS_TRANSFORMSPACE)coordEnum);
	}
}

/** 以pTargetPos点为中心,旋转相机fAngle弧度
@param in sceneHandle   : 场景管理器
@param in camHandle     : 相机句柄
@param in pTargetPos    : 围绕旋转的中心点(世界坐标系)
@param in axistype      : 围绕旋转的轴(世界坐标系)， 0：world X， 1：world y，2：world z, 3: local X, 4: local Y, 5: local Z
@param in fAngle        : 旋转的角度，以 弧度 为单位
*/ 
VOID CVGSCore::RoundCamera(DWORD sceneHandle, DWORD camHandle, VECTOR3 *pTargetPos, INT axistype, FLOAT fAngle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	CCamera* pCamera = (CCamera*)camHandle;
	if (pTargetPos && vgsScene && pCamera)
	{
		D3DXVECTOR3 v3Target(pTargetPos->x, pTargetPos->y, pTargetPos->z);
		D3DXVECTOR3 v3Axis;
		if (axistype == 0) // x
		{
			v3Axis = D3DXVECTOR3_X;
		}
		else if (axistype == 1) // y
		{
			v3Axis = D3DXVECTOR3_Y;
		}
		else if (axistype == 2)
		{
			v3Axis = D3DXVECTOR3_Z;
		}
		else if (axistype == 3) // local x
		{
			v3Axis = pCamera->GetRight(VTS_WORLD); 
		}
		else if (axistype == 4) // y
		{
			v3Axis = pCamera->GetUp(VTS_WORLD); //
		}
		else if (axistype == 5)
		{
			v3Axis = pCamera->GetDirection(VTS_WORLD);
		}
		else
		{
			return;
		}

		pCamera->RoundEx(&v3Target, &v3Axis, fAngle);
	}
}
/** 以pTargetPos点为中心,旋转相机fAngle弧度
@param in sceneHandle   : 场景管理器
@param in camHandle     : 相机句柄
@param in pTargetPos    : 围绕旋转的中心点
@param in axistype      : 围绕旋转的轴， 0：x， 1：y，2：z
@param in fAngle        : 旋转的角度，以 弧度 为单位
*/ 
// VOID CVGSCore::RoundCamera1(DWORD sceneHandle, DWORD camHandle, VECTOR3* pAxisCenter, VECTOR3* pAxisVec, FLOAT fAngle)
// {
// 	CScene* vgsScene = (CScene*)sceneHandle;
// 	if (vgsScene == NULL) return;
// 
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	pCamera->Round1(&D3DXVECTOR3(pAxisCenter->x, pAxisCenter->y, pAxisCenter->z), 
// 					&D3DXVECTOR3(pAxisVec->x, pAxisVec->y, pAxisVec->z), 
// 					fAngle*ATOS);
// }

/** 设置相机的宽高比

VOID CVGSCore::SetCameraAspect(DWORD camHandle, const FLOAT fAspectRatio)
{

}*/

/**以定点为轴心, 在相机坐标系中旋转相机
@param pAxisCenter in : 相机围绕旋转的目标点
@param pXYZAangle  in : 相机旋转的角度，弧度 
*/
VOID CVGSCore::RoundInCameraSpace(DWORD sceneHandle, DWORD camHandle, VECTOR3* pAxisCenter, VECTOR3* pXYZAngle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CCamera* pCamera = (CCamera*)camHandle;
	pCamera->RoundInCameraSpaceEx(&D3DXVECTOR3(pAxisCenter->x, pAxisCenter->y, pAxisCenter->z), 
								pXYZAngle->x, 
								pXYZAngle->y, 
								pXYZAngle->z);
}

/** 相机目标点不动，沿相机方向前后推拉相机。
@param in sceneHandle : 场景指针
@param in camHandle : 相机指针
@param dollyValue  in   : 相机推拉的距离，> 0 向前， < 1向后
*/
VOID CVGSCore::DollyCamera(DWORD sceneHandle, DWORD camHandle, FLOAT dollyValue)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;
	
	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		pCamera->Dolly(dollyValue);
	}
}

/*设置摄像机的世界坐标系位置
@param in sceneHandle : 场景指针
@param in camHandle   : 相机指针
@param in pPos        : 世界坐标系中的新位置
*/
VOID CVGSCore::SetCameraPosition(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		D3DXVECTOR3 vFrom(pPos->x, pPos->y, pPos->z);
		pCamera->SetPosition(&vFrom, VTS_WORLD);
	}
}

/*获取摄像机的世界坐标系位置
@param in sceneHandle : 场景指针
@param in camHandle   : 相机指针
@param in pPos        : 世界坐标系中的新位置
*/
VOID CVGSCore::GetCameraPosition(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		D3DXVECTOR3 vFrom = pCamera->GetPosition(VTS_WORLD);
		*pPos = VECTOR3(vFrom.x, vFrom.y, vFrom.z);
	}
}

//设置摄像机的目标点
VOID CVGSCore::SetCameraAt(DWORD sceneHandle, DWORD camHandle, const VECTOR3* pPos)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		pCamera->Lookat(&D3DXVECTOR3(pPos->x, pPos->y, pPos->z), VTS_WORLD);
	}
}
VOID CVGSCore::GetCameraAt(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;
	
	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		D3DXVECTOR3 at = pCamera->GetLookat(VTS_WORLD);
		*pPos = VECTOR3(at.x, at.y, at.z);
	}

}
/** 摄像机正对模型
@param in sceneHandle : 场景指针
@param in camHandle : 相机指针
@param in modelHandle : 模型指针
*/
VOID CVGSCore::SetCameraLookAtModel(DWORD sceneHandle, DWORD camHandle, DWORD modelHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		CModel* pModel = (CModel*)modelHandle;
		pCamera->SetLookAtModel(pModel);
	}
}
// DLL_EXPORT VOID CVGSCore::GetCameraAt(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos);

/** 设置摄像机的FOV
@param in fov : 相机的fov，即y方向的视角
*/
VOID CVGSCore::SetCameraFov(DWORD sceneHandle, DWORD camHandle, FLOAT fov)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		pCamera->SetFov(fov);
	}

}

/** 设置摄像机的nearClip，与相机距离小于nearClip的物体将不会被看到
*/
VOID CVGSCore::SetCameraNearClip(DWORD sceneHandle, DWORD camHandle, FLOAT nearClip)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		pCamera->setNearClip(nearClip);
	}
}

/** 设置摄像机的farClip，与相机距离大于farClip的物体将不会被看到
*/
VOID CVGSCore::SetCameraFarClip(DWORD sceneHandle, DWORD camHandle, FLOAT farClip)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		pCamera->setFarClip(farClip);
	}

}

//设置摄像机的Roll
// DLL_EXPORT VOID CVGSCore::SetCameraRoll(DWORD sceneHandle, DWORD camHandle, FLOAT rollAngle);

/** 一次性获得相机的属性
*/
BOOL CVGSCore::GetCameraInfo(DWORD sceneHandle, DWORD camHandle, CameraInfoEx* pCamInfo)
{
	ZeroMemory(pCamInfo, sizeof(CameraInfo));

	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return FALSE;

	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		D3DXVECTOR3 vFrom  = pCamera->GetPosition(VTS_WORLD);
		pCamInfo->from     = VECTOR3(vFrom.x, vFrom.y, vFrom.z);
		D3DXVECTOR3 vAt    = pCamera->GetLookat(VTS_WORLD);
		pCamInfo->at       = VECTOR3(vAt.x, vAt.y, vAt.z);
		pCamInfo->farClip  = pCamera->getFarClip();
		pCamInfo->nearClip = pCamera->getNearClip();
		pCamInfo->fov      = pCamera->GetFov();
		D3DXVECTOR3 vUp    = pCamera->GetUp(VTS_WORLD);
		pCamInfo->Up       = VECTOR3(vUp.x, vUp.y, vUp.z);
		pCamInfo->ProjectType = pCamera->m_camType;
		//FLOAT roll = pCamera->GetRoll();
		//pCamInfo->roll = roll;		
		return TRUE;
	}
	return FALSE;
}

/** 一次性设置相机的属性
*/
VOID CVGSCore::SetCameraInfo(DWORD sceneHandle, DWORD camHandle, CameraInfoEx* pCamInfo)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	EnterCriticalSection(&m_render_cs);
	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		pCamera->SetPosition(&D3DXVECTOR3(pCamInfo->from.x, pCamInfo->from.y, pCamInfo->from.z), VTS_WORLD);
		pCamera->Lookat((&D3DXVECTOR3(pCamInfo->at.x, pCamInfo->at.y, pCamInfo->at.z)), VTS_WORLD);
		pCamera->SetRoll(pCamInfo->roll);
		pCamera->SetFov(pCamInfo->fov);
		pCamera->setNearClip(pCamInfo->nearClip);
		pCamera->setFarClip(pCamInfo->farClip);
		pCamera->m_camType = pCamInfo->ProjectType;
	}
	LeaveCriticalSection(&m_render_cs);
}

/** 获取相机的方向
@param vDirection out : 相机在世界坐标系中的方向，即lookat的方向。如果获取方向失败，vDirection = (0, 0, 0)
*/
VOID CVGSCore::GetCameraDirection(DWORD sceneHandle, DWORD camHandle, VECTOR3 &vDirection)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	EnterCriticalSection(&m_render_cs);
	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		/*D3DXVECTOR3 vAt = pCamera->GetLookat(VTS_WORLD);
		D3DXVECTOR3 vFrom = pCamera->GetPosition(VTS_WORLD);
		D3DXVECTOR3 vDir = vAt - vFrom;*/
		D3DXVECTOR3 vDir = pCamera->GetDirection(VTS_WORLD);
		vDirection = VECTOR3(vDir.x, vDir.y, vDir.z);
	}
	LeaveCriticalSection(&m_render_cs);
}

/** 设置相机的方向
@param vDirection in : 相机在世界坐标系中的方向，即lookat方向
*/
VOID CVGSCore::SetCameraDirection(DWORD sceneHandle, DWORD camHandle, const VECTOR3 &vDirection)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	EnterCriticalSection(&m_render_cs);
	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		D3DXVECTOR3 vFrom = pCamera->GetPosition(VTS_WORLD);
		D3DXVECTOR3 vAt = pCamera->GetLookat(VTS_WORLD);
		float len = D3DXVec3Length(&(vAt - vFrom));  // from 到 at 的距离
		D3DXVec3Normalize(&vAt, &D3DXVECTOR3(vDirection.x, vDirection.y, vDirection.z));
		vAt = vFrom + vAt * len;
		pCamera->Lookat(&vAt, VTS_WORLD);
	}
	LeaveCriticalSection(&m_render_cs);
}

// 获取相机的up方向
VOID CVGSCore::GetCameraUp(DWORD sceneHandle, DWORD camHandle, VECTOR3 &vUp)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	EnterCriticalSection(&m_render_cs);
	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		D3DXVECTOR3 vCamUp = pCamera->GetUp(VTS_WORLD);
		vUp = VECTOR3(vCamUp.x, vCamUp.y, vCamUp.z);
	}
	LeaveCriticalSection(&m_render_cs);
}

// 获取相机的right方向
VOID CVGSCore::GetCameraRight(DWORD sceneHandle, DWORD camHandle, VECTOR3 &vRight)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	EnterCriticalSection(&m_render_cs);
	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		D3DXVECTOR3 vCamUp = pCamera->GetRight(VTS_WORLD);
		vRight = VECTOR3(vCamUp.x, vCamUp.y, vCamUp.z);
	}
	LeaveCriticalSection(&m_render_cs);
}

// 获取相机的near/far clip
VOID CVGSCore::GetCameraClip(DWORD sceneHandle, DWORD camHandle, float &nearClip, float &farClip)
{
	nearClip = 1;
	farClip  = 10000;
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	EnterCriticalSection(&m_render_cs);
	CCamera* pCamera = (CCamera*)camHandle;
	if (pCamera)
	{
		nearClip = pCamera->getNearClip();
		farClip  = pCamera->getFarClip();
	}
	LeaveCriticalSection(&m_render_cs);
}

//-------------------------------------------------------------------------------------------
///灯光相关的方法
//-------------------------------------------------------------------------------------------
/** 得到场景中灯光的数量
@return 场景中的所有灯光数目
*/
INT CVGSCore::GetLightCount(DWORD sceneHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		return vgsScene->m_vLight.size();
	}
	return 0;
}

/** 删除灯光
*/
VOID CVGSCore::DeleteLight(DWORD sceneHandle, DWORD lightHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		vgsScene->DeleteLight((CLight*)lightHandle);
	}
}

/** 根据灯光的名称得到灯光的句柄
*/
DWORD CVGSCore::GetLightHandleByName(DWORD sceneHandle, CHAR* strLightName)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		return (DWORD)vgsScene->GetLightByName(std::string(strLightName));
	}
	return 0;
}

/** 根据灯光的编号得到灯光的句柄
*/
DWORD CVGSCore::GetLightHandleByIndex(DWORD sceneHandle, INT index)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		return (DWORD)vgsScene->GetLightByIndex(index);
	}
	return 0;
}

/** 根据灯光的句柄得到灯光的名称
*/
VOID CVGSCore::GetLightNameByHandle(DWORD sceneHandle, DWORD lightHandle, CHAR* strLightName)
{
	CLight* pCamera = (CLight*)lightHandle;
	if (pCamera)
	{
		COPY_NAME(strLightName, pCamera->getName().c_str());
	}
}

/*设置灯光的名称
@return 设置成功返回TRUE，否则FALSE，失败可能是由于命名冲突导致的。
*/
BOOL CVGSCore::SetLightName(DWORD sceneHandle, DWORD lightHandle, CHAR* strLightName)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		CLight *pLight = (CLight*)lightHandle;
		if (pLight)
		{
			pLight->setName(std::string(strLightName));
			return TRUE;
		}
	}	
	return FALSE;
}

/** 根据灯光的编号得到灯光的名称
*/
BOOL CVGSCore::GetLightNameByIndex(DWORD sceneHandle, INT index, CHAR* strLightName)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		std::string str = vgsScene->GetLightNameByIndex(index);
		COPY_NAME(strLightName, str.c_str());
		return TRUE;
	}
	return FALSE;
}

/** 在世界坐标系中移动灯光
@param in pVec : 移动的目标点世界坐标，绝对值
*/
VOID CVGSCore::MoveLight(DWORD sceneHandle, DWORD lightHandle, VECTOR3* pVec)
{
	CLight* pLight = (CLight*)lightHandle;
	if (pLight)
	{
		pLight->m_pNode->translate(&D3DXVECTOR3(pVec->x, pVec->y, pVec->z), VTS_WORLD);
	}
}

//旋转灯光
// DLL_EXPORT VOID CVGSCore::RotateLight(DWORD sceneHandle, DWORD lightHandle, VECTOR3* pRos);

/** 设置灯光的位置
@param in vPos : 灯光要移动到的新位置，参考坐标系是世界坐标系
*/
VOID CVGSCore::SetLightPosition(DWORD sceneHandle, DWORD lightHandle, VECTOR3* vPos)
{
	CLight* pLight = (CLight*)lightHandle;
	if (pLight && vPos)
	{
		pLight->m_pNode->setPosition(&D3DXVECTOR3(vPos->x, vPos->y, vPos->z), VTS_WORLD);
	}
}

//设置灯光的方向
// DLL_EXPORT VOID CVGSCore::SetLightDirection(DWORD sceneHandle, DWORD lightHandle, VECTOR3* pDir);

//-----------------------------------------------------------------------------
// 方法名: EnableLight()
// 参  数: DWORD lightHandle, BOOL isEnable
// 用  途: 设置灯光变是否有效
//-----------------------------------------------------------------------------
VOID CVGSCore::EnableLight(DWORD sceneHandle, DWORD lightHandle, BOOL isEnable)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	CLight *pLight = (CLight*)lightHandle;
	if (pLight)
	{
		EnterCriticalSection(&m_render_cs);

		int index = vgsScene->GetLightIndex(pLight);
		vgsScene->m_pd3dDevice->LightEnable(index, isEnable);
		pLight->enable = isEnable;

		LeaveCriticalSection(&m_render_cs);
	}
	
}

/** 得到灯光的属性
*/
VOID CVGSCore::GetLightInfo(DWORD sceneHandle, DWORD lightHandle, LightInfo* pLightInfo)
{
	if (!pLightInfo)
	{
		return;
	}

	ZeroMemory(pLightInfo, sizeof(LightInfo));

	CLight* pLight = (CLight*)lightHandle;
	if (pLight)
	{
		pLightInfo->Ambient   = _GetRGBColor(pLight->ambient);
		pLightInfo->Diffuse   = _GetRGBColor(pLight->diffuse);

		pLightInfo->Direction = _GetVECTOR3(pLight->getDirection());
		pLightInfo->Position  = _GetVECTOR3(pLight->getPosition());

		pLightInfo->Multiply  = pLight->multiply;
	}
}

/** 设置灯光的属性
*/
VOID CVGSCore::SetLightInfo(DWORD sceneHandle, DWORD lihgtHandle, LightInfo* pLightInfo)
{
	CLight* pLight = (CLight*)lihgtHandle;
	if (pLight && pLightInfo)
	{
		pLight->ambient = _GetD3DColor(pLightInfo->Ambient);
		pLight->diffuse = _GetD3DColor(pLightInfo->Diffuse);
		pLight->setDirection(&_GetD3DVECTOR3(pLightInfo->Direction));
		pLight->setPosition(&_GetD3DVECTOR3(pLightInfo->Position));
		pLight->param.Diffuse  = pLightInfo->Multiply * pLight->diffuse;
		pLight->param.Ambient  = pLightInfo->Multiply * pLight->ambient;
		pLight->param.Specular = pLightInfo->Multiply * pLight->diffuse;
		
	}
}


/** 设置灯光是否有效
*/
VOID CVGSCore::SetLightEnableStatus(DWORD sceneHandle, DWORD lightHandle, BOOL bEnable)
{

}


/////////////////////// 二维元素创建方法 /////////////////////////////////////////

/** 初始化GUI状态，新创建的场景如果要使用GUI，必须首先调用该方法
// 如果已经初始化，将不做任何操作，直接返回
// 如果清空或销毁场景后，可重现初始化GUI状态
*/
VOID CVGSCore::InitGUIStatus(DWORD sceneHandle, DWORD renderWnd, DWORD viewport)
{
#if _VGS_DLL
#else
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CViewport* vp = (CViewport *)viewport;
		RECT rc;
		vp->GetRect(rc);
		pScene->GetGuiManagerRef().InitGUIStatus(rc.right - rc.left, rc.bottom - rc.top);
	}
#endif
}


/** 得到光标下方的GUI对象句柄及类型,如果有多个对象，返回最上层（ZOrder最大）的一个GUI对象
@param sceneHandle in : 场景指针
@param x/y  : 视口中的坐标
@param type out ： GUI对象的类型，参考VGS_GUIOBJ_TYPE定义
@return GUI对象的指针
*/
DWORD CVGSCore::GetGUIObjectUnderLoc(DWORD sceneHandle, INT x, INT y, VGS_GUIOBJ_TYPE &type)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetGUIObjectUnderLoc(x, y, type);
		return (DWORD)ptr;
	}
	return 0;
}

/** 获取GUI对象的类型
@param GUIObjectHandle in : GUI对象的句柄
@return GUI对象的类型，参考VGS_GUIOBJ_TYPE定义
*/
VGS_GUIOBJ_TYPE CVGSCore::GetGUIObjectType(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(GUIObjectHandle);
		if (ptr)
			return ptr->GetType();
	}
	return VGS_GUIOBJ_TYPE_NONE;
}


/** 返回GUI对象的ZOrder，越大表示GUI对象处于越上层
*/
INT CVGSCore::GetGUIObjectZOrder(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		return pScene->GetGuiManagerRef().GetObjectZOrder((CGUIObject*)GUIObjectHandle);
	}
	return 0;
}


/** 设置GUI对象的ZOrder，越大表示GUI对象处于越上层
*/
//DLL_EXPORT VOID CVGSCore::SetObjectZOrder(DWORD GUIObjectHandle, int offset);

// GUI对象上移一层
VOID CVGSCore::MoveGUIObjectUp(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pScene->GetGuiManagerRef().MoveObjectUp((CGUIObject*)GUIObjectHandle);
	}
}

/** GUI对象下移一层
*/
VOID CVGSCore::MoveGUIObjectDown(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pScene->GetGuiManagerRef().MoveObjectDown((CGUIObject*)GUIObjectHandle);
	}
}

/** GUI对象移动到最顶层
*/
VOID CVGSCore::GotoGUIObjectsTop(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pScene->GetGuiManagerRef().GotoObjectsTop((CGUIObject*)GUIObjectHandle);
	}
}

/** GUI对象移动到最底层
*/
VOID CVGSCore::GotoGUIObjectsBottom(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pScene->GetGuiManagerRef().GotoObjectsBottom((CGUIObject*)GUIObjectHandle);
	}
}

/** 获取所有二维元素的句柄列表
// vHandle必须要预先分配足够的空间
*/ 
VOID CVGSCore::GetAllGUIObjectsList(DWORD* vHandle)
{
	//考虑去除该接口...
}

/** 获取所有GUI(不包括背景图)的数目
*/
UINT CVGSCore::GetAllGUIObjectsCount(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		return pScene->GetGuiManagerRef().GetAllObjectsCount();
	}
	return 0;
}


/**得到某种二维元素(不包括背景图)的数量
*/
INT CVGSCore::GetGUIObjectCount(DWORD sceneHandle, VGS_GUIOBJ_TYPE guitype)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		return pScene->GetGuiManagerRef().GetObjectsCount(guitype);
	}
	return 0;
}


/** 获取指定类型GUI对象的句柄列表
*/
VOID CVGSCore::GetGUIObjectList(VGS_GUIOBJ_TYPE guitype, DWORD* vHandle)
{
	//考虑去除该接口...
}

/** 通过索引得到某种gui对象的句柄
*/
DWORD CVGSCore::GetGUIObjetHandleByIndex(DWORD sceneHandle, VGS_GUIOBJ_TYPE guitype, INT index)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		return (DWORD)(pScene->GetGuiManagerRef().GetObjectHandlebyIndex(guitype, index));
	}
	return 0;
}


/** 设置GUI对象定位的参考原点
*/
VOID CVGSCore::SetGUIObjOffsetOrigin(DWORD sceneHandle, DWORD overlayHandle, VGSOFFSETORIGIN offsetOrigin)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(overlayHandle);
		if (ptr)
			ptr->SetOffsetOrigin(offsetOrigin);
	}
}

/** 获得GUI对象定位的参考原点
*/
VGSOFFSETORIGIN CVGSCore::GetGUIObjOffsetOrigin(DWORD sceneHandle, DWORD overlayHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(overlayHandle);
		if (ptr)
			return ptr->GetOffsetOrigin();
	}
	return (VGSOFFSETORIGIN)0;
}


/**设置GUI对象是否使用相对坐标(%)保存位置
*/
VOID CVGSCore::SetGUIObjUseRelativePos(DWORD sceneHandle, DWORD guiHandle, BOOL bRelativePos)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (0 != ptr)
			ptr->SetUsedRelativePos(bRelativePos);
	}
}

/**获取GUI对象是否使用相对坐标(%)保存位置
*/
BOOL CVGSCore::GetGUIObjUseRelativePos(DWORD sceneHandle, DWORD guiHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (ptr)
			return ptr->GetUsedRelativePos();
	}
	return 0;
}


/** 设置GUI对象是否使用相对尺寸(%)保存尺寸
*/
VOID CVGSCore::SetGUIObjUseRelativeSize(DWORD sceneHandle, DWORD guiHandle, BOOL bRelativeSize)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (0 != ptr)
			ptr->SetUsedRelativeSize(bRelativeSize);
	}
}

/** 获取GUI对象是否使用相对尺寸(%)保存尺寸
*/
BOOL CVGSCore::GetGUIObjUseRelativeSize(DWORD sceneHandle, DWORD guiHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (ptr)
			return ptr->GetUsedRelativeSize();
	}
	return 0;
}


/** 删除 GUI
@param sceneHandle   in : 场景管理器
@param GUIHandle in : GUI句柄
*/
VOID CVGSCore::DeleteGUIObject(DWORD sceneHandle, DWORD GUIHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pScene->GetGuiManagerRef().RemoveObject((CGUIObject*)GUIHandle);
	}
}


/** 得到 GUI对象 屏幕坐标的绝对Rect
@param rect out : rect的表示的位置是GUI对象相对于视口offsetOrigin的位置,单位为像素
*/
VOID CVGSCore::GetGUIObjectRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, BOOL &bRelativeSize, BOOL &bRelativePos)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(GUIHandle);
		if (ptr)
		{
			ptr->GetRect(*rect);
			bRelativePos = ptr->GetUsedRelativePos();
			bRelativeSize = ptr->GetUsedRelativeSize();
		}
	}
}

/** 得到 GUI对象 屏幕坐标的绝对Rect
@param rect out : rect的表示的位置是GUI对象相对于视口左上角的位置,也就是GUI对象在屏幕的实际位置。单位为像素
*/
VOID CVGSCore::GetGUIObjectAbsoluteRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, BOOL &bRelativeSize, BOOL &bRelativePos)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(GUIHandle);
		if (ptr)
		{
			ptr->GetAbsoluteRect(*rect);
			bRelativePos = ptr->GetUsedRelativePos();
			bRelativeSize = ptr->GetUsedRelativeSize();
		}
	}
}

/** 设置 GUI对象 的相对屏幕坐标的绝对Rect
@param rect in : rect的表示的位置是GUI对象相对于视口offsetOrigin的位置,单位为像素
*/
VOID CVGSCore::SetGUIObjectRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, BOOL bRelativeSize/* = TRUE*/, BOOL bRelativePos/* = TRUE*/)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(GUIHandle);
		if (0 != ptr)
		{
			ptr->SetRect(*rect);
			ptr->SetUsedRelativePos(bRelativePos);
			ptr->SetUsedRelativeSize(bRelativeSize);
		}
	}
}

/** 设置 GUI对象 的相对屏幕坐标的绝对Rect
@param rect in : rect的表示的位置是GUI对象相对于视口左上角的位置,也就是GUI对象的实际位置。单位为像素
*/
VOID CVGSCore::SetGUIObjectAbsoluteRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, BOOL bRelativeSize/* = TRUE*/, BOOL bRelativePos/* = TRUE*/)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(GUIHandle);
		if (0 != ptr)
		{
			ptr->SetAbsoluteRect(*rect);
			ptr->SetUsedRelativePos(bRelativePos);
			ptr->SetUsedRelativeSize(bRelativeSize);
		}
	}
}

/** 设置GUI对象的位置
@param sceneHandle   in : 场景管理器
@param guiHandle in : gui句柄
@param x, y : 相对于视口offsetOrigin的坐标,单位为像素
@param bRelative : true采用相对位置，false采用绝对位置。如果采用相对位置，实际上记录了overlay左上角位置相对于目前视口大小的百分比，
视口缩放时，overlay会保持这个比例不变
*/
VOID CVGSCore::SetGUIObjectPosition(DWORD sceneHandle, DWORD guiHandle, INT x, INT y, BOOL bRelative/* = true*/)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (0 != ptr)
		{
			ptr->SetPosition(x, y);
			ptr->SetUsedRelativePos(bRelative);
		}
	}
}

/** 设置gui对象的尺寸
@param sceneHandle   in : 场景管理器
@param guiHandle in : overlay句柄
@param width, height    : Overlay的宽高
@param bRelative : true采用相对大小，false采用绝对大小。如果采用相对大小，实际上记录了overlay尺寸相对于目前视口尺寸的百分比，
视口缩放时，gui对象会保持这个比例不变
*/
VOID CVGSCore::SetGUIObjectSize(DWORD sceneHandle, DWORD guiHandle, INT width, INT height, BOOL bRelative/* = true*/)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (0 != ptr)
		{
			ptr->SetSize(width, height);
			ptr->SetUsedRelativeSize(bRelative);
		}
	}
}
/**得到 gui对象 的透明度
@return gui对象的不透明度 [0, 100]
*/
INT CVGSCore::GetGUIObjectOpacity(DWORD sceneHandle, DWORD guiHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (ptr)
			return ptr->GetOpacity();
	}
	return 0;
}
 

/**设置 gui对象 的透明度
@opacity in : gui对象的不透明度 [0, 100]
*/
VOID CVGSCore::SetGUIObjectOpacity(DWORD sceneHandle, DWORD guiHandle, INT opacity)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (ptr)
			ptr->SetOpacity(opacity);
	}
} 

/** 设置gui对象是否可见
*/
VOID CVGSCore::SetGUIObjectVisibility(DWORD sceneHandle, DWORD guiHandle, BOOL visible)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (0 != ptr)
			ptr->SetVisibility(visible);
	}
}

/** 获取gui对象是否可见
*/
BOOL CVGSCore::GetGUIObjectVisibility(DWORD sceneHandle, DWORD guiHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (ptr)
			return ptr->GetVisibility();
	}
	return 0;
}


/** 通过名称得到gui对象
*/
DWORD CVGSCore::GetGUIObjectHandleByName(DWORD sceneHandle, CHAR* name)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		return (DWORD)(pScene->GetGuiManagerRef().GetObjectHandleByName(name));
	}
	return 0;
}


/** 得到gui对象的名称
*/
VOID CVGSCore::GetGUIObjectNameByHandle(DWORD sceneHandle, DWORD guiHandle, CHAR* name)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (ptr)
		{
			COPY_NAME(name, ptr->GetName());
		}
	}
}

/** 设置gui对象的名称
*/
BOOL CVGSCore::SetGUIObjectName(DWORD sceneHandle, DWORD guiHandle, CHAR* NewName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (ptr)
		{
			ptr->SetName(NewName);
		}
	}
	return 0;
}


/** 设置gui对象选择框是否可见
*/
VOID CVGSCore::ShowGUIObjectSelcetionBox(DWORD sceneHandle, DWORD guiHandle, BOOL bVisible)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (0 != ptr)
			ptr->SetShowSelBox(bVisible);
	}
}

/** 获取二维对象选择框是否显示
*/
BOOL CVGSCore::GetGUISelectionBoxVisible(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(GUIObjectHandle);
		if (ptr)
			return ptr->GetShowSelBox();
	}
	return 0;
}

/** 设置GUI对象的鼠标事件函数名称
*/
VOID CVGSCore::SetGuiMouseEvent(DWORD sceneHandle, DWORD guiHandle, VGS_MOUSEEVENT_TYPE type, const CHAR* sEventName)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (ptr)
		{
			ptr->SetEventFuncName(type, std::string(sEventName));
		}
	}
}

/** 获取GUI对象的鼠标事件函数名称
*/
BOOL CVGSCore::GetGuiMouseEvent(DWORD sceneHandle, DWORD guiHandle, VGS_MOUSEEVENT_TYPE type, CHAR* sEventName)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().GetObjectByHandle(guiHandle);
		if (ptr)
		{
			std::string str = ptr->GetEventFuncName(type);
			if (!str.empty())
			{
				COPY_NAME(sEventName, str.c_str());
				return TRUE;
			}
		}
	}
	return FALSE;
}

//------------------Flash GUI Start-------------------------------------------------------------------------
DWORD CVGSCore::CreateFlashGUIFromFile(DWORD sceneHandle, HWND hParent, const char* sName, const char* sFile, const RECT &rect)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene && sName != NULL && sFile != NULL)
	{
		if (NULL == hParent)
			hParent = g_VGSRender.m_hWnd;
		CGUIObject* ptr = pScene->GetGuiManagerRef().CreateFlashCtrl(hParent, sName, sFile, rect);
		return (DWORD)ptr;
	}
	return 0;
}
VOID CVGSCore::MoveFlashGUI(DWORD sceneHandle, DWORD guiHandle,INT x, INT y)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene && guiHandle != NULL)
	{
		CATLFlashCtrl* ptr = (CATLFlashCtrl *)(guiHandle);

		RECT rc;
		ptr->GetRect(rc);
		rc.left += x;
		rc.top += y;

		RECT rcParent;
		ptr->GetParentRect(rcParent);
		
		if (rc.right - rc.left < 0 )
		{
			ptr->m_bFullWidth = true;
			rc.right = rc.left + (rcParent.right - rcParent.left);
		}
		if (rc.bottom - rc.top < 0 )
		{
			ptr->m_bFullHeight = true;
			rc.bottom = rc.top + (rcParent.bottom - rcParent.top);
		}

		ptr->SetRect(rc.left, rc.right, rc.top, rc.bottom);

		//这里方法如下：可以先把场景暂停渲染，然后把后缓冲区的像素在当前调用中刷新，
		//这样可以不用实时渲染场景，开销会小很多
// 		m_pVGS2Wnd->m_VGS2.SunSetRenderStatus(FALSE);
// 		m_pVGS2Wnd->m_VGS2.vgsScene->m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
// 		m_pVGS2Wnd->m_VGS2.SunSetRenderStatus(TRUE);
	}
}
VOID CVGSCore::SetFlashGUIPlayStatus(DWORD sceneHandle, DWORD guiHandle,INT status)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene && guiHandle != NULL)
	{
		CATLFlashCtrl* ptr = (CATLFlashCtrl *)(guiHandle);
		switch(status)
		{
		case 0:
			ptr->Pause();
			break;
		case 1:
			ptr->Play(TRUE);
			break;
		case 2:
			ptr->Stop();
			break;
		default:
			break;
		}
	}
}
VOID CVGSCore::AddFlashGUISemaphore(DWORD sceneHandle, DWORD guiHandle,const char*  SemaphoreName, const char*  SemaphoreFunc)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene && guiHandle != NULL)
	{
		CATLFlashCtrl* ptr = (CATLFlashCtrl *)(guiHandle);

		INT SemaphoreCnt = ptr->SemaphoreList.size();
		for(UINT n = 0; n<SemaphoreCnt; n++)
		{
			if(ptr->SemaphoreList[n].SemaphoreName == SemaphoreName)
				break;   
		}

		std::string strSFunc =std::string((char *)_bstr_t(SemaphoreFunc));

		Semaphore semap;
		semap.SemaphoreName = SemaphoreName;
		semap.SemaphoreFunc = strSFunc;
		ptr->SemaphoreList.push_back(semap);
	}
}
//------------------Flash GUI End-------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
///前景图相关的方法
//-------------------------------------------------------------------------------------------
/**
由图片文件创建Overlay
@param sceneHandle in : 场景管理器
@param name        in : Overlay名称
@param sFile       in : 图片文件名称，包含路径
@param sOpcatityImageFile in : 透明图片文件名称，包含路径, 如果没有透明贴图，sOpcatityImageFile设置空即可
@param rect        in : Overlay四个角的坐标
@return 返回overlay的指针
*/
DWORD CVGSCore::CreateOverlayFromFile(DWORD sceneHandle, const CHAR* name, const CHAR* sFile, const CHAR* sOpcatityImageFile, RECT &rect)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().CreateOverLayer(name, sFile, sOpcatityImageFile);
		if (ptr)
		{
			ptr->SetRect(rect);
		}
		return (DWORD)ptr;
	}
	return 0;
}

/** 获取overlay的属性
@param sceneHandle   in  : 场景管理器
@param overlayHandle in  : overlay句柄
@param info          out : Overlay的属性
*/
// DLL_EXPORT BOOL CVGSCore::GetOverlayInfo(DWORD sceneHandle, DWORD overlayHandle, OverlayInfo &info);

/**得到Overlay的数量
*/
INT CVGSCore::GetOverlayCount(DWORD sceneHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		return pScene->GetGuiManagerRef().GetObjectsCount(VGS_GUIOBJ_TYPE_OVERLAY);
	}
	return 0;
}

// 获取所有Overlay的句柄列表
VOID CVGSCore::GetOverlayList(DWORD sceneHandle,DWORD* vHandle)
{
	//考虑去除该接口...
// 	CScene *pScene = (CScene*)sceneHandle;
// 	if (pScene)
// 	{
// 		return pScene->GetGuiManagerRef().GetObjectsList(VGS_GUIOBJ_TYPE_OVERLAY, );
// 	}
}

/**获得Overlay使用的图像文件
@param sBaseFile in : 基本贴图文件
@param sOpacFile in : 透明贴图文件
*/
VOID CVGSCore::GetOverlayImageFile(DWORD sceneHandle, DWORD overlayHandle, CHAR* sBaseFile, CHAR* sOpacFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		COverlay* ptr = (COverlay* )(pScene->GetGuiManagerRef().GetObjectByHandle(overlayHandle));
		if (ptr)
		{
			std::string str1, str2;
			ptr->GetOverlayImageFile(str1, str2);
			COPY_NAME(sBaseFile, str1.c_str());
			COPY_NAME(sOpacFile, str2.c_str());
		}
	}
}

/**设置Overlay使用的图像文件
@param sBaseFile in : 基本贴图文件
@param sOpacFile in : 透明贴图文件
*/
VOID CVGSCore::SetOverlayImageFile(DWORD sceneHandle, DWORD overlayHandle, const CHAR* sBaseFile, const CHAR* sOpacFile/* = NULL*/)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		COverlay* ptr = (COverlay* )(pScene->GetGuiManagerRef().GetObjectByHandle(overlayHandle));
		if (ptr)
		{
			ptr->SetOverlayImageFile(std::string(sBaseFile), std::string(sOpacFile));
		}
	}
}

//-------------------------------------------------------------------------------------------
///Button相关的方法
//-------------------------------------------------------------------------------------------
/** 创建Button
@param  sName             in ：按钮的名称
@param rect               in ：按钮所在区域的四个坐标，控制按钮的位置和大小
@param mouseUpImageFile   in ：鼠标松开的源图像文件
@param mouseOverImageFile in ：鼠标移上的源图像文件
@param mouseDownImageFile in ：鼠标按下的源图像文件
@return 如果成功创建，返回按钮指针，否则0
*/
DWORD CVGSCore::CreateButtonFromFile(DWORD sceneHandle, const CHAR* sName, const RECT &rect, 
						              const CHAR* mouseUpImageFile,
							          const CHAR* mouseOverImageFile/* = NULL*/,
							          const CHAR* mouseDownImageFile/* = NULL*/)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CGUIObject* ptr = pScene->GetGuiManagerRef().CreateButton(sName, rect, mouseUpImageFile, mouseOverImageFile, mouseDownImageFile);
		if (ptr)
		{
			ptr->SetRect(rect);
		}
		return (DWORD)ptr;
	}
	return 0;
}

/** 获得按钮的回调函数名称, 长度不超过MAX_NAMESTR_LENGTH
*/
VOID CVGSCore::GetButtonCallBack(DWORD sceneHandle, DWORD buttonHandle, CHAR* callBackFunc)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CButton* ptr = (CButton* )(pScene->GetGuiManagerRef().GetObjectByHandle(buttonHandle));
		if (ptr)
		{
			std::string str = "";
			str = ptr->GetJsCallbackFunc();
			COPY_NAME(callBackFunc, str.c_str());
		}
	}
}

/** 设置按钮的回调函数名称[按钮按下，回调函数], 长度不超过MAX_NAMESTR_LENGTH
*/
VOID CVGSCore::SetButtonCallBack(DWORD sceneHandle, DWORD buttonHandle, const CHAR* callBackFunc)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CButton* ptr = (CButton* )(pScene->GetGuiManagerRef().GetObjectByHandle(buttonHandle));
		if (ptr)
		{
			ptr->SetJsCallbackFunc(callBackFunc);
		}
	}
}
/** 获得按钮使用的MouseUp图像文件,也就是正常状态下显示的图像文件
*/
VOID CVGSCore::GetButtonMouseUpImageFile(DWORD sceneHandle, DWORD buttonHandle, CHAR* strMouseUpImageFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CButton* ptr = (CButton* )(pScene->GetGuiManagerRef().GetObjectByHandle(buttonHandle));
		if (ptr)
		{
			std::string str = "";
			if( ptr->GetMouseUpImageFileName(str))
			{
				COPY_NAME(strMouseUpImageFile, str.c_str());
			}
		}
	}
}

/** 设置按钮使用的MouseUp图像文件
*/
VOID CVGSCore::SetButtonMouseUpImageFile(DWORD sceneHandle, DWORD buttonHandle, const CHAR* strMouseUpImageFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CButton* ptr = (CButton* )(pScene->GetGuiManagerRef().GetObjectByHandle(buttonHandle));
		if (ptr)
		{
			ptr->SetMouseUpImageFileName(strMouseUpImageFile);
		}
	}
}

/** 获得按钮使用的Mouse over(鼠标悬停在按钮上面时)图像文件
*/
VOID CVGSCore::GetButtonMouseOverImageFile(DWORD sceneHandle, DWORD buttonHandle, CHAR* strMouseOverImageFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CButton* ptr = (CButton* )(pScene->GetGuiManagerRef().GetObjectByHandle(buttonHandle));
		if (ptr)
		{
			std::string str = "";
			if( ptr->GetMouseOverImageFileName(str))
			{
				COPY_NAME(strMouseOverImageFile, str.c_str());
			}
		}
	}
}

/** 设置按钮使用的Mouse over(鼠标悬停在按钮上面时)图像文件
*/
VOID CVGSCore::SetButtonMouseOverImageFile(DWORD sceneHandle, DWORD buttonHandle, const CHAR* strMouseOverImageFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CButton* ptr = (CButton* )(pScene->GetGuiManagerRef().GetObjectByHandle(buttonHandle));
		if (ptr)
		{
			ptr->SetMouseOverImageFileName(strMouseOverImageFile);
		}
	}
}

/** 获得按钮使用的MouseDown(按钮被按下)图像文件
*/
VOID CVGSCore::GetButtonMouseDownImageFile(DWORD sceneHandle, DWORD buttonHandle, CHAR* strMouseDownImageFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CButton* ptr = (CButton* )(pScene->GetGuiManagerRef().GetObjectByHandle(buttonHandle));
		if (ptr)
		{
			std::string str = "";
			if( ptr->GetMouseDownImageFileName(str))
			{
				COPY_NAME(strMouseDownImageFile, str.c_str());
			}
		}
	}
}

/** 设置按钮使用的MouseDown(按钮被按下)图像文件
*/
VOID CVGSCore::SetButtonMouseDownImageFile(DWORD sceneHandle, DWORD buttonHandle, const CHAR* strMouseDownImageFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CButton* ptr = (CButton* )(pScene->GetGuiManagerRef().GetObjectByHandle(buttonHandle));
		if (ptr)
		{
			ptr->SetMouseDownImageFileName(strMouseDownImageFile);
		}
	}
}

/** 设置Button是否激活
*/
VOID CVGSCore::SetButtonEnable(DWORD sceneHandle, DWORD buttonHandle, BOOL enable)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CButton* ptr = (CButton* )(pScene->GetGuiManagerRef().GetObjectByHandle(buttonHandle));
		if (0 != ptr)
		{
			ptr->SetEnable(enable);
		}
	}
}

/** 判断Button是否激活
*/
BOOL CVGSCore::IsButtonEnable(DWORD sceneHandle, DWORD buttonHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CButton* ptr = (CButton* )(pScene->GetGuiManagerRef().GetObjectByHandle(buttonHandle));
		if (ptr)
		{
			return ptr->GetEnable();
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------
/** 导航图相关的方法 */
//-------------------------------------------------------------------------------------------
/**创建导航图, 
@param name        in : 导航图使用的名称
@param strMapFile  in : 导航图使用的地图文件名称，包含完整路径
@param pRect       in : 导航范围，场景顶视图中看到的场景区域，left < right(x方向), top < bottom(z方向)
@return 返回导航图对象的句柄或0
*/
DWORD CVGSCore::CreateNavigater(DWORD sceneHandle, const CHAR* name, const CHAR* strMapFile, const FRect &pRect)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().CreateNavigator(name,strMapFile));
		if (ptr)
		{
			ptr->SetNavRange(pRect);
		}
		return (DWORD)ptr;
	}
	return 0;
}

/** 获得导航图底图
@param strMapFile out : 导航图背景图使用的图片文件，不包含路径
*/
VOID CVGSCore::GetNavigaterMap(DWORD sceneHandle, DWORD navHandle, CHAR* strMapFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navHandle));
		if (ptr)
		{
			std::string str = ptr->GetNavMap();
			COPY_NAME(strMapFile, str.c_str());
		}
	}
}

/** 设置导航图底图
@param strMapFile in : 导航图背景图使用的图片文件，需要包含完整路径
*/
VOID CVGSCore::SetNavigaterMap(DWORD sceneHandle, DWORD navHandle, const CHAR* strMapFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navHandle));
		if (ptr)
		{
			ptr->SetNavMap(std::string(strMapFile));
		}
	}
}

/** 获得导航图的导航范围坐标
@param pRect out : 场景顶视图中看到的场景区域，left < right(x方向), top < bottom(z方向)
*/
VOID CVGSCore::GetNavigaterSceneRect(DWORD sceneHandle, DWORD navHandle, FRect &pRect)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navHandle));
		if (ptr)
		{
			ptr->GetNavRange(pRect);
		}
	}
}

/** 设置导航图的导航范围坐标
@param pRect in : 场景顶视图中看到的场景区域，left < right(x方向), top < bottom(z方向)
*/
VOID CVGSCore::SetNavigaterSceneRect(DWORD sceneHandle, DWORD navHandle, const FRect &pRect)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navHandle));
		if (ptr)
		{
			ptr->SetNavRange(pRect);
		}
	}
}

/**增加导航点对象
@param  navigaterHandle - 导航图对象的句柄
@param  navPointerName - 导航图点的名称
@param  pointerType - 导航图点的类型，0为相机导航，1为模型导航
@param  pointerHandle - 导航点所导航的对象的句柄，如果导航图点类型为相机，则为相机句柄，否则为模型句柄
@return 成功创建导航点返回TRUE，否则FALSE
*/
BOOL CVGSCore::AddNavigaterPointer(DWORD sceneHandle, DWORD navigaterHandle,
									const CHAR* navPointerName,
									const CHAR* strPointerFile, 
									INT	  pointerType/*   = 0*/,	// 0 - camera or 1 - model 
									DWORD pointerHandle/* = 0*/     //camera or model handle
									)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			return (NULL != ptr->AddNavPointer(navPointerName, strPointerFile, pointerType, pointerHandle));
		}
	}
	return FALSE;
}

/** 使用已有的导航图点创建一个新导航点
@param  navPointerName   in : 要创建的导航点名称
@param  sSrcPointerName   in : 已存在的导航点名称，新创建的点会使用其图片，并且具有相同的导航类型
@return 成功创建导航点返回TRUE，否则FALSE
*/
BOOL CVGSCore::CloneNavigaterPointer(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* navPointerName, 
							          const CHAR* sSrcPointerName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			return (NULL != ptr->CloneNavPointer(navPointerName, sSrcPointerName));
		}
	}
	return FALSE;
}

/** 删除导航点
@param navigaterHandle in : 导航图
@param sName           in : 导航点名称
*/
VOID CVGSCore::DeleteNavigaterPointer(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			ptr->deleteNavPointer(sName);
		}
	}
}

/** 得到Navigater的导航点数量
@param navigaterHandle in : 导航图对象指针
@return 导航图中的导航点数目
*/
INT CVGSCore::GetNavPointerCount(DWORD sceneHandle, DWORD navigaterHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			return ptr->GetPointerCount();
		}
	}
	return 0;
}

/** 根据索引，获取导航图的导航点名称
*/
VOID CVGSCore::GetNavPointerNameByIndex(DWORD sceneHandle, DWORD navigaterHandle, INT index, CHAR* sName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			std::string str = ptr->GetPointerName(index);
			COPY_NAME(sName, str.c_str());
		}
	}
}

/** 获得导航图点所使用的图片名称，不带路径
@param navigaterHandle in : 导航图对象指针
@param sName           in : 导航点名称
@param strPointerFile  out : 导航点使用的图片名称，不包含路径
*/
VOID CVGSCore::GetNavPointerImage(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, CHAR* strPointerFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sName);
			if (pnter)
			{
				std::string str1, str2;
				pnter->GetOverlayImageFile(str1, str2);
				COPY_NAME(strPointerFile, str1.c_str());
			}
		}
	}
}
/** 设置导航图点所使用的图片名称
@param navigaterHandle in : 导航图对象指针
@param sName           in : 导航点名称
@param strPointerFile  in : 导航点使用的图片名称，需要包含完整路径
*/
VOID CVGSCore::SetNavPointerImage(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, const CHAR* strPointerFile)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sName);
			if (pnter)
			{
				pnter->SetOverlayImageFile(strPointerFile, "");
			}
		}
	}
}

/** 获得导航点的导航类型和导航对象
*/
BOOL CVGSCore::GetNavPointerObj(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT &pointerType, DWORD &pointerHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sName);
			if (pnter)
			{
				pointerType = pnter->GetNavType();
				pointerHandle = pnter->GetPointerHandle();
				return TRUE;
			}
		}
	}
	return FALSE;
}

/** 设置导航点的导航类型和导航对象
*/
VOID CVGSCore::SetNavPointerObj(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT pointerType, DWORD pointerHandle)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sName);
			if (pnter)
			{
				pnter->SetNavType(pointerType);
				pnter->SetPointerHandle(pointerHandle);
			}
		}
	}
}

/** 设置导航点是否可见
*/
VOID CVGSCore::SetNavPointerVisiblilty(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, BOOL bVisible)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sName);
			if (0 != pnter)
			{
				pnter->SetVisibility(bVisible);
			}
		}
	}
}

/** 获取导航点是否可见
*/
BOOL CVGSCore::GetNavPointerVisiblilty(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sName);
			if (pnter)
			{
				return pnter->GetVisibility();
			}
		}
	}
	return 0;
}

/** 设置导航点的不透明度 [0, 100]
*/
VOID CVGSCore::SetNavPointerOpacity(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT opacity)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sName);
			if (pnter)
			{
				pnter->SetOpacity(opacity);
			}
		}
	}
}

/** 获取导航点的不透明度 [0, 100]
*/
INT  CVGSCore::GetNavPointerOpacity(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sName);
			if (pnter)
			{
				return pnter->GetOpacity();
			}
		}
	}
	return 0;
}

/** 设置导航点的宽度和高度，像素数目
*/
VOID CVGSCore::SetNavPointerSize(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT width, INT height)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sName);
			if (pnter)
			{
				pnter->SetSize(width, height);
			}
		}
	}
}

/** 获取导航点的宽度和高度，像素数目
*/
BOOL CVGSCore::GetNavPointerSize(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT &width, INT &height)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sName);
			if (pnter)
			{
				pnter->GetSize(width, height);
				return true;
			}
		}
	}
	return 0;
}

/** 设置导航点的位置，像素数目
*/
VOID CVGSCore::SetPointerPosition(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT left, INT top)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sName);
			if (pnter)
			{
				int l = 0, t = 0;
				ptr->GetPosition(l,t);
				pnter->SetPosition(left + l, top + t);
				pnter->SetRelativePosition(left, top);
			}
		}
	}
}

/** 获取导航点的位置，像素数目
*/
BOOL CVGSCore::GetPointerPosition(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT &left, INT &top)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sName);
			if (pnter)
			{
				pnter->GetRelativePosition(left, top);
				return true;
			}
		}
	}
	return 0;
}

/** 导航点更名
*/
VOID CVGSCore::RenamePointer(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sOldName, const CHAR* sNewName)
{
	CScene *pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		CNavigator* ptr = (CNavigator* )(pScene->GetGuiManagerRef().GetObjectByHandle(navigaterHandle));
		if (ptr)
		{
			CNavPointer* pnter = ptr->GetPointerByName(sOldName);
			if (pnter)
			{
				pnter->SetName(sNewName);
			}
		}
	}
}

////////////////// 控制对象 ////////////////////////////


///// 控制对象方法 ////////////////////////////////
// 设置当前控制器
VOID CVGSCore::SetCurrentController(DWORD sceneHandle, DWORD ControllerHandle)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		pVGSSceneMgr->SetCurrentController((IController*)ControllerHandle);
	}
}

// 获取当前控制器
DWORD CVGSCore::GetCurrentController(DWORD sceneHandle)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		return DWORD(pVGSSceneMgr->GetCurrentController());
	}
	return 0;
}

// 获取控制对象的类型
CONTROLTYPE CVGSCore::GetControllerType(DWORD ControllerHandle)
{
	if (!ControllerHandle)
	{
		return CT_NONE;
	}
	return ((IController*)ControllerHandle)->GetControlType();
}

// 获取当前控制器使用的相机
DWORD CVGSCore::GetCurrentControllerCamera(DWORD sceneHandle)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		IController* pController = pVGSSceneMgr->GetCurrentController();
		if (pController)
		{
			return (DWORD)pController->GetCamera();
		}
	}
	return 0;
}

// 设置控制器的相机
VOID CVGSCore::SetControllerCamera(DWORD sceneHandle, DWORD ControllerHandle, DWORD CameraHandle)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		((IController*)ControllerHandle)->SetCamera((CCamera*)CameraHandle);
	}
}

// 获取当前控制器的相机
DWORD CVGSCore::GetControllerCamera(DWORD sceneHandle, DWORD ControllerHandle)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		return (DWORD) (((IController*)ControllerHandle)->GetCamera());
	}

	return 0;
}

//-------------------------------------------------------------------------------------------
//***Walker漫游对象相关的方法
//-------------------------------------------------------------------------------------------

/** 创建Walker漫游对象
param in scneneHandle : 场景指针
param in wInfo : walker的参数
return 如果成功返回walker的指针，否则返回NULL
*/
DWORD CVGSCore::CreateWalker(DWORD sceneHandle, WalkerInfo* wInfo)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (!pVGSSceneMgr)
	{
		return 0;
	}

	EnterCriticalSection(&m_ctrl_cs);
	EnterCriticalSection(&m_render_cs);
	DWORD re = (DWORD)(pVGSSceneMgr->CreateWalker(*wInfo));      // 创建walker对象	
	LeaveCriticalSection(&m_render_cs);
	LeaveCriticalSection(&m_ctrl_cs);
	return re;
}

/** 删除Walker漫游对象,如果删除了当前控制前，引擎会自动设置一个当前控制器
*/
VOID CVGSCore::DeleteWalkerByHandle(DWORD sceneHandle, DWORD walkerHandle)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		//线程同步 
		EnterCriticalSection(&m_ctrl_cs);
		EnterCriticalSection(&m_render_cs);
		pVGSSceneMgr->DeleteWalker(walkerHandle);
		LeaveCriticalSection(&m_render_cs);
		LeaveCriticalSection(&m_ctrl_cs);
	}
}
VOID CVGSCore::DeleteWalkerByName(DWORD sceneHandle, const CHAR* pName)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		//线程同步 
		EnterCriticalSection(&m_ctrl_cs);
		EnterCriticalSection(&m_render_cs);
		pVGSSceneMgr->DeleteWalker(string(pName));
		LeaveCriticalSection(&m_render_cs);
		LeaveCriticalSection(&m_ctrl_cs);
	}
}


//得到Walker的数量
INT CVGSCore::GetWalkerCount(DWORD sceneHandle)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		return (INT)(pVGSSceneMgr->GetWalkerCount());
	}
	return 0;
}

//通过编号获得Walker
DWORD CVGSCore::GetWalkerHandleByIndex(DWORD sceneHandle, INT index)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		return (DWORD)(pVGSSceneMgr->GetWalker(index));
	}	
	return 0;
}

//通过name获得Walker
DWORD CVGSCore::GetWalkerHandleByName(DWORD sceneHandle, const CHAR* sName)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		return (DWORD)(pVGSSceneMgr->GetWalker(string(sName)));
	}	
	return 0;
}

//通过句柄获得名称
VOID CVGSCore::GetWalkerNameByHandle(DWORD sceneHandle, DWORD walkerHandle, CHAR* name)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		string sName = ((CWalker*)walkerHandle)->GetName();
		// strncpy(name, sName.c_str(), min((int)sName.length(), MAX_NAMESTR_LENGTH));
		COPY_NAME(name, sName.c_str());
	}	
}

//设置预览状态时行走的walker的属性
VOID CVGSCore::SetWalkerProp(DWORD sceneHandle, DWORD walkerHandle, WalkerInfo* wInfo)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		//线程同步 
		EnterCriticalSection(&m_ctrl_cs);
		((CWalker*)(walkerHandle))->SetWalkerInfo(*wInfo);
		//线程同步 
		EnterCriticalSection(&m_ctrl_cs);
	}
}
//从摄像机得到Walker的属性
VOID CVGSCore::GetWalkerProp(DWORD sceneHandle, DWORD walkerHandle, WalkerInfo* wInfo)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{

		WalkerInfo info;
		((CWalker*)(walkerHandle))->GetWalkerInfo(info);
		*wInfo = info;
	}
}


//-------------------------------------------------------------------------------------------
//***Editor控制对象相关的方法
//-------------------------------------------------------------------------------------------
//创建Editor对象
DWORD CVGSCore::CreateEditor(DWORD sceneHandle, EditorInfo* esInfo)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (!pVGSSceneMgr)
	{
		return 0;
	}

	//线程同步 
	EnterCriticalSection(&m_ctrl_cs);
	EnterCriticalSection(&m_render_cs);
	DWORD re = (DWORD)(pVGSSceneMgr->CreateEditor(*esInfo));      // 创建Editor对象	
	LeaveCriticalSection(&m_render_cs);
	LeaveCriticalSection(&m_ctrl_cs);
	return re;
}
//删除Editor对象
VOID CVGSCore::DeleteEditorbyHandle(DWORD sceneHandle, DWORD editorHandle)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		//线程同步 
		EnterCriticalSection(&m_ctrl_cs);
		EnterCriticalSection(&m_render_cs);
		pVGSSceneMgr->DeleteEditor(editorHandle);
		LeaveCriticalSection(&m_render_cs);
		LeaveCriticalSection(&m_ctrl_cs);
	}
}

VOID CVGSCore::DeleteEditorbyName(DWORD sceneHandle, const CHAR* pName)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		//线程同步 
		EnterCriticalSection(&m_ctrl_cs);
		EnterCriticalSection(&m_render_cs);
		pVGSSceneMgr->DeleteEditor(string(pName));
		LeaveCriticalSection(&m_render_cs);
		LeaveCriticalSection(&m_ctrl_cs);
	}
}

//得到Editor的数量
INT CVGSCore::GetEditorCount(DWORD sceneHandle)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		return (INT)(pVGSSceneMgr->GetEditorCount());
	}
	return 0;
}
//通过编号获得Walker
DWORD CVGSCore::GetEditorHandleByIndex(DWORD sceneHandle, INT index)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		return (DWORD)(pVGSSceneMgr->GetEditor(index));
	}	
	return 0;
}

//通过名称获得Editor
DWORD CVGSCore::GetEditorHandleByName(DWORD sceneHandle, CHAR* name)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		return (DWORD)(pVGSSceneMgr->GetEditor(string(name)));
	}	
	return 0;
}

//通过句柄获得名称
VOID CVGSCore::GetEditorNameByHandle(DWORD sceneHandle, DWORD editorHandle, CHAR* name)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		string sName = ((CEditor*)editorHandle)->GetName();
		// strncpy(name, sName.c_str(), min((int)sName.length(), MAX_NAMESTR_LENGTH));
		COPY_NAME(name, sName.c_str());
	}	
}

//设置预览状态时行走的Editor的属性
VOID CVGSCore::SetEditorProp(DWORD sceneHandle, DWORD editorHandle, EditorInfo* eInfo)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		((CEditor*)(editorHandle))->SetEditorInfo(*eInfo);
	}
}
//从摄像机得到Editor的属性
VOID CVGSCore::GetEditorProp(DWORD sceneHandle, DWORD editorHandle, EditorInfo* eInfo)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		EditorInfo info;
		((CEditor*)(editorHandle))->GetEditorInfo(info);
		*eInfo = info;
	}
}
/** 设置editor所对的目标点，实际上要围绕该点旋转
@param lookatType in : looat的类型，0：场景中心 1：场景中的一点
*/
VOID CVGSCore::SetEditorLookAt(DWORD sceneHandle, DWORD editorHandle,	const VECTOR3 &v3, INT lookatType/* = 0*/)
{
	if (sceneHandle && editorHandle)
	{
		EnterCriticalSection(&m_ctrl_cs);
		VECTOR3 At(v3.x, v3.y, v3.z);
		((CEditor*)editorHandle)->LookAt(At, lookatType);
		LeaveCriticalSection(&m_ctrl_cs);
	}
}

/** 设置editor所对的模型，实际上要围绕模型包围盒的中心旋转
*/
VOID CVGSCore::SetEditorLookAtModel(DWORD sceneHandle, DWORD editorHandle, DWORD modelHandle)
{
	if (sceneHandle && editorHandle)
	{
		EnterCriticalSection(&m_ctrl_cs);
		/*VECTOR3 vModel;
		GetModelPosition(sceneHandle, modelHandle, &vModel);
		Ogre::Vector3 At(vModel.x, vModel.y, vModel.z);*/
		((CEditor*)editorHandle)->LookAtModel((CModel*)modelHandle);
		LeaveCriticalSection(&m_ctrl_cs);
	}
}

// 获取editor控制对象正对的模型名称，如果没有正对模型，返回false
BOOL CVGSCore::GetEditorLookAtModelName(DWORD sceneHandle, DWORD editorHandle, CHAR* sName)
{
	BOOL re = FALSE;
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr && editorHandle)
	{
		EnterCriticalSection(&m_ctrl_cs);
		CModel* pEntity = ((CEditor*)editorHandle)->GetTargetModel();
		if (pEntity)
		{
			COPY_NAME(sName, pEntity->getName().c_str());
			re = TRUE; 
		}
		LeaveCriticalSection(&m_ctrl_cs);
	}
	return re;
}

//-------------------------------------------------------------------------------------------
//***Flyer控制对象相关的方法
//-------------------------------------------------------------------------------------------
//创建Flyer对象
DWORD CVGSCore::CreateFlyer(DWORD sceneHandle, FlyerInfo* fInfo)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		EnterCriticalSection(&m_ctrl_cs);
		EnterCriticalSection(&m_render_cs);
		DWORD re = (DWORD)(pVGSSceneMgr->CreateFlyer(*fInfo));      // 创建flyer对象
		LeaveCriticalSection(&m_render_cs);
		LeaveCriticalSection(&m_ctrl_cs);
		return re;
	}

	return 0;
}
//删除Flyer对象
VOID CVGSCore::DeleteFlyerbyHandle(DWORD sceneHandle, DWORD flyerHandle)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		EnterCriticalSection(&m_ctrl_cs);
		EnterCriticalSection(&m_render_cs);
		pVGSSceneMgr->DeleteFlyer(flyerHandle);
		LeaveCriticalSection(&m_render_cs);
		LeaveCriticalSection(&m_ctrl_cs);
	}
}
VOID CVGSCore::DeleteFlyerbyName(DWORD sceneHandle, const CHAR* pName)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		EnterCriticalSection(&m_ctrl_cs);
		EnterCriticalSection(&m_render_cs);
		pVGSSceneMgr->DeleteFlyer(string(pName));
		LeaveCriticalSection(&m_render_cs);
		LeaveCriticalSection(&m_ctrl_cs);
	}
}

//得到Flyer的数量
INT CVGSCore::GetFlyerCount(DWORD sceneHandle)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{	
		return (pVGSSceneMgr->GetFlyerCount());
	}
	return 0;
}

//通过编号获得Flyer
DWORD CVGSCore::GetFlyerHandleByIndex(DWORD sceneHandle, INT index)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		return (DWORD)(pVGSSceneMgr->GetFlyer(index));
	}	
	return 0;
}
//通过名称获得Flyer
DWORD CVGSCore::GetFlyerHandleByName(DWORD sceneHandle, CHAR* name)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		return (DWORD)(pVGSSceneMgr->GetFlyer(string(name)));
	}	
	return 0;
}

//通过句柄获得名称
VOID CVGSCore::GetFlyerNameByHandle(DWORD sceneHandle, DWORD flyerHandle, CHAR* name)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		string sName = ((CFlyer*)flyerHandle)->GetName();
		// strncpy(name, sName.c_str(), min((int)sName.length(), MAX_NAMESTR_LENGTH));
		COPY_NAME(name, sName.c_str());
	}	
}

//设置预览状态时行走的Flyer的属性
VOID CVGSCore::SetFlyerProp(DWORD sceneHandle, DWORD flyerHandle, FlyerInfo* fInfo)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		EnterCriticalSection(&m_ctrl_cs);
		((CFlyer*)(flyerHandle))->SetFlyerInfo(*fInfo);
		LeaveCriticalSection(&m_ctrl_cs);
	}
}
//从摄像机得到Editor的属性
VOID CVGSCore::GetFlyerProp(DWORD sceneHandle, DWORD flyerHandle, FlyerInfo* fInfo)
{
	CScene *pVGSSceneMgr = (CScene*)sceneHandle;
	if (pVGSSceneMgr)
	{
		FlyerInfo info;
		((CFlyer*)(flyerHandle))->GetFlyerInfo(info);
		*fInfo = info;
	}
}
///////////////////////////////// 特效 /////////////////////////////////////////

//-------------------------------------------------------------------------------------------
///后期特效处理的相关方法
//-------------------------------------------------------------------------------------------
VOID CVGSCore::CreateBloomEffect(DWORD viewportHandle)
{
}
VOID CVGSCore::EnableBloomEffect(DWORD viewportHandle, BOOL enabled)
{
	
}
VOID CVGSCore::DestroyBloomEffect(DWORD viewportHandle)
{
	
}

VOID CVGSCore::CreateHDREffect(DWORD viewportHandle)
{
	
}
VOID CVGSCore::EnableHDREffect(DWORD viewportHandle, BOOL enabled)
{
	
}
VOID CVGSCore::DestroyHDREffect(DWORD viewportHandle)
{
	
}

//创建动态模糊特效对象
VOID CVGSCore::CreateFSMotionBlur(DWORD sceneHandle)
{
	
}
//打开动态模糊特效
VOID CVGSCore::OpenFSMotionBlur(DWORD sceneHandle)
{
	
}
//关闭动态模糊特效
VOID CVGSCore::CloseFSMotionBlur(DWORD sceneHandle)
{
	
}
//设置动态模糊的强度, 0-100
VOID CVGSCore::SetFSMotionBlurPower(DWORD sceneHandle, INT value)
{
	
}

//---------------------------------------------------------------------------------
//创建全屏光晕对象
VOID CVGSCore::CreateFSGlow(DWORD sceneHandle)
{
	
}
//打开全屏光晕特效
VOID CVGSCore::OpenFSGlow(DWORD sceneHandle)
{
	
}
//关闭全屏光晕特效
VOID CVGSCore::CloseFSGlow(DWORD sceneHandle)
{
	
}
//设置全屏光晕强度 0-100
VOID CVGSCore::SetFSGlowPower(DWORD sceneHandle, INT value)
{
	
}

//----------------------------------------------------------------------------------
//创建LensFlare镜头光斑对象
VOID CVGSCore::CreateLensFlare(DWORD sceneHandle, VECTOR3* sunPos)
{
	
}
//删除镜头光斑对象
VOID CVGSCore::DeleteLensFlare(DWORD sceneHandle)
{

}

/** 创建雾效
@param   fogMode         in : 雾的类型
@param   red/green/blue  in : 雾的颜色
@param   density         in : 雾的密度
@param   start/end       in : 雾的开始结束距离（到相机的距离）
*/
 VOID CVGSCore::CreateFog(DWORD sceneHandle, INT fogMode, INT red, INT green, INT blue, FLOAT density, FLOAT start, FLOAT end)
 {

 }

////////////////////////////////// 特效结束 ////////////////////////////////////////

//-------------------------------------------------------------------------------------------
///关键帧动画相关的方法
//-------------------------------------------------------------------------------------------
/** 获取指定类型动画的数目
@param in type : 目前有四种
	VAT_KEYFRM = 0,       // 关键帧动画
	VAT_CAMERA,           // 相机动画
	VAT_LIGHT,            // 灯光动画
	VAT_SKELETON          // 骨骼动画
*/
DWORD CVGSCore::GetAnimationCount(DWORD sceneHandle, VGS_ANIMATIONTYPE type)
{
	return m_pRoot->GetAnimManager().GetAnimationCount(type);
}

/** 根据索引得到动画的名称
@param in type  : VAT_KEYFRM：关键帧动画，VAT_CAMERA：相机动画，VAT_LIGHT：灯光动画, VAT_SKELETON:骨骼动画
@param in index : 动画索引
@param out sName : 名称
@return 得到名称，返回true，否则false
*/
BOOL CVGSCore::GetAnimationNameByIndex(DWORD sceneHandle, VGS_ANIMATIONTYPE type, int index, CHAR* sName)
{
	return m_pRoot->GetAnimManager().GetAnimationNameByIndex(type, index, sName);
}

/** 播放指定名称的动画
*/
VOID CVGSCore::PlayAnimation(DWORD sceneHandle, const CHAR* sAnimName)
{
	m_pRoot->GetAnimManager().SetAnimStatus(std::string(sAnimName), AnimStatus(VAS_PLAYING, VGS_ANIMSPEED_NONE));
}

/** 播放所有动画
*/
VOID CVGSCore::PlayAllAnimation(DWORD sceneHandle)
{
	m_pRoot->GetAnimManager().SetAllAnimStatus(AnimStatus(VAS_PLAYING, VGS_ANIMSPEED_NONE));
}

/** 暂停指定名称的动画
*/
VOID CVGSCore::PauseAnimation(DWORD sceneHandle, const CHAR* sAnimName)
{		
	m_pRoot->GetAnimManager().SetAnimStatus(std::string(sAnimName), AnimStatus(VAS_PAUSED, VGS_ANIMSPEED_NONE));
}
/** 暂停所有动画
*/
VOID CVGSCore::PauseAllAnimation(DWORD sceneHandle)
{
	m_pRoot->GetAnimManager().SetAllAnimStatus(AnimStatus(VAS_PAUSED, VGS_ANIMSPEED_NONE));
}

/** 停止指定名称的动画
*/
VOID CVGSCore::StopAnimation(DWORD sceneHandle, const CHAR* sAnimName)
{	
	m_pRoot->GetAnimManager().SetAnimStatus(std::string(sAnimName), AnimStatus(VAS_STOP, VGS_ANIMSPEED_NONE));
}
/** 停止所有动画
*/
VOID CVGSCore::StopAllAnimation(DWORD sceneHandle)
{
	m_pRoot->GetAnimManager().SetAllAnimStatus(AnimStatus(VAS_STOP, VGS_ANIMSPEED_NONE));
}

/** 得到关键帧动画的帧数
*/
INT CVGSCore::GetKeyframeCount(DWORD sceneHandle, const CHAR* sAnimName)
{
	return m_pRoot->GetAnimManager().GetAnimFramCount(std::string(sAnimName));
}

/** 跳到某一帧
*/
VOID CVGSCore::GotoFrame(DWORD sceneHandle, const CHAR* sAnimName, INT FrameIndex)
{
	m_pRoot->GetAnimManager().SetCurrentFrame(std::string(sAnimName), FrameIndex);
}

/** 得到当前正在播放的帧
*/
INT CVGSCore::GetCurrentFrame(DWORD sceneHandle, const CHAR* sAnimName)
{
	return m_pRoot->GetAnimManager().GetCurrentFrame(std::string(sAnimName));
}

/** 设置所有关键帧动画播放的帧率
*/
VOID CVGSCore::SetAllKfrPlaySpeed(DWORD sceneHandle, INT FPS)
{
	float factor = FPS / gkfrPlaySpeed;  // 换算成标准播放速率的倍数
	m_pRoot->GetAnimManager().SetAllAnimStatus(AnimStatus(VAS_NONE, factor));  // 只设置速率，不设置状态
}

/** 设置指定关键帧动画播放的帧率
*/
VOID CVGSCore::SetKfrPlaySpeed(DWORD sceneHandle, const CHAR* sAnimName, INT FPS)
{
	float factor = FPS / gkfrPlaySpeed;  // 换算成标准播放速率的倍数
	m_pRoot->GetAnimManager().SetAnimStatus(std::string(sAnimName), AnimStatus(VAS_NONE, factor));  // 只设置速率，不设置状态
}

/** 获得关键帧动画播放的帧率
*/ 
INT CVGSCore::GetKfrPlaySpeed(DWORD sceneHandle, const CHAR* sAnimName)
{
	AnimStatus status;
	m_pRoot->GetAnimManager().GetAnimStatus(sAnimName, status);  // 
	if (status.velocity != VGS_ANIMSPEED_NONE)
	{
		return status.velocity * gkfrPlaySpeed;  // 换算FPS
	}
	return gkfrPlaySpeed;
}

/** 获取动画FPS的缺省值
*/
INT CVGSCore::GetDefaultFPS()
{
	return gkfrPlaySpeed;
}

/** 获取动画播是否循环播放
*/
BOOL CVGSCore::GetAnimationLoop(DWORD sceneHandle, const CHAR* sAnimName)
{
	return m_pRoot->GetAnimManager().GetAnimationLoop(std::string(sAnimName));
} 

/** 设置动画播是否循环播放
*/
VOID CVGSCore::SetAnimationLoop(DWORD sceneHandle, const CHAR* sAnimName, BOOL bLoop)
{
	m_pRoot->GetAnimManager().SetAnimationLoop(std::string(sAnimName),(BOOL)bLoop);
}

///////////////////////// math functions /////////////////////////////////
/** 根据法线和点创建一个平面
@param vNormal in ：屏幕的法线
@param point   in : 平面上任意一点
@return : 平面的指针
*/
DWORD CVGSCore::CreatePlane(const VECTOR3 &vNormal, const VECTOR3 &point)
{
	MessageBox(NULL, TEXT("暂时没有实现"), TEXT("提示"), 0);
	return 0;
}

/** 销毁一个平面
*/ 
VOID CVGSCore::DestroyPlane(const DWORD hPlane)
{
	MessageBox(NULL, TEXT("暂时没有实现"), TEXT("提示"), 0);
}

/** 计算一点到一个平面的距离
@param hPlane  in ：平面对象指针
@param point   in : 空间中的任意一点
@param dist    out：点point到平面hPlane的距离，如果point位于hPlane的法线正方向，返回值>0,如果在负方向上，返回值<0,如果点在平面上，返回值=0
@return : 正确得到结果，返回true，否则false
*/
BOOL CVGSCore::GetDistance(const DWORD hPlane, const VECTOR3 &point, FLOAT &dist)
{
	MessageBox(NULL, TEXT("暂时没有实现"),TEXT( "提示"), 0);
	dist = 0;
	return false;
}

/** 计算一个轴方向，经过一个四元数作用后的新的方向
@param quat     in  : 旋转四元数
@param vOrigDir in  : 初始的方向
@param vNewDir  out : vOrigDir经过quat作用后的新的方向
*/
VOID CVGSCore::RotationAxis(const VGS_QUATERNION &quat, const VECTOR3 &vOrigDir, VECTOR3 &vNewDir)
{
	//MessageBox(NULL, "暂时没有实现", "提示", 0);
}
////////////////// 消息响应 /////////////////////////

/** 消息响应
@param sceneHandle in : 场景句柄
@param DWORD in : 渲染窗口句柄。设为NULL，表示对所有渲染窗口。
@param uMsg in : 消息ID，可参考windows标准消息说明
@param wParam/lParam  in : 消息参数，可参考windows标准消息说明
*/
VOID CVGSCore::VGSMsgProc(DWORD sceneHandle, DWORD hRenderWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (sceneHandle)
	{
		// 由root分发消息
		m_pRoot->ProcessMessage(sceneHandle, hRenderWnd, uMsg, wParam, lParam);
	}
}

//////////////////// 其他 ////////////////////////////////
/// 获取最近一次错误码
DWORD CVGSCore::GetVGSLastErrCode()
{
	return CVGSErrCode::GetLastErrorCode();
}

/// 获取最近一次错误描述
VOID CVGSCore::GetVGSLastErrStr(char *sErrStr)
{
	string str = CVGSErrCode::GetLastErrorStr();
	int len = min((INT)strlen(str.c_str()), MAX_ERRORSTR_LENGTH - 1);
	strncpy(sErrStr, str.c_str(), len); 
	sErrStr[len] = '\0';
	
}

/// 得到错误码对应的详细描述
VOID CVGSCore::GetVGSErrStr(DWORD Code, char *sErrStr)
{
	string str = CVGSErrCode::GetErrorStr(Code);
	int len = min((INT)strlen(str.c_str()), MAX_ERRORSTR_LENGTH - 1);
	strncpy(sErrStr, str.c_str(), len); 
	sErrStr[len] = '\0';
}


///////////////////坐标轴相关——Start/////////////////////////////////////////////////////////
VOID CVGSCore::SelectModelAxis(DWORD sceneHandle, DWORD modelHandle, UINT selectStatus)
{
	CScene* pScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (pScene && pModel)
	{
		Axis* ptr = pModel->GetCurAxis();
		if (NULL != ptr)
		{
			ptr->SelectAxis(selectStatus);
		}
	}
}
VOID CVGSCore::SetShowModelAxis(DWORD sceneHandle, DWORD modelHandle, BOOL bShow)
{
	CScene* pScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (pScene && pModel)
	{
		pModel->SetShowAxis(bShow);
	}

}
VOID CVGSCore::SetModelAxisStatus(DWORD sceneHandle, DWORD modelHandle, int nStatus)
{
	CScene* pScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	if (pScene && pModel)
	{
		pModel->SetAxisStatus(nStatus);
	}

}
int CVGSCore::GetModelAxisStatusUnderCursor(DWORD sceneHandle, DWORD modelHandle, DWORD hViewport, UINT nScreenX, UINT nScreenY)
{
	CScene* pScene = (CScene*)sceneHandle;
	CModel* pModel = (CModel*)modelHandle;
	CViewport* vp = (CViewport*)hViewport;
	if (pScene && pModel && vp)
	{
		// 		D3DXVECTOR3 pos, dir;
		// 		pScene->GetRayUnderPoint(vp, nScreenX, nScreenY, &pos, &dir);

		Ray ray;
		ray.BuildWithSceenPos(*(vp->GetD3DViewport()), pScene->m_MatProj, nScreenX, nScreenY);
		ray.Transfrom(pScene->m_MatViewInverse);


		Axis* pAxis = pModel->GetCurAxis();
		if (pAxis)
		{
			return pAxis->GetModelAxisStatusWithRay(ray.GetOrigin(), ray.GetDirection());
		}		
	}

	return 0;
}

///////////////////坐标轴相关——End/////////////////////////////////////////////////////////

int CVGSCore::Lua_GetViewPortRect(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulViewport =  args[1].GetInteger();
		if (0 == ulViewport)
		{
			return 0;
		}
		float left = 0.0f, top = 0.0f, width = 0.0f, height = 0.0f;
		GetViewportRect(ulViewport, left, top, width, height);
		state->PushNumber(left);
		state->PushNumber(top);
		state->PushNumber(width);
		state->PushNumber(height);
		return 4;
	}
	return 0;
}
int CVGSCore::Lua_GetModelNameByIndex(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long hScene = args[1].GetInteger();
		int nIndex = args[2].GetInteger();
		char buf[MAX_NAMESTR_LENGTH];
		ZeroMemory(buf, MAX_NAMESTR_LENGTH);
		bool bFlag = GetModelNameByIndex(hScene, nIndex, buf);
		if (!bFlag)
			return 0;
		state->PushString(buf);
		return 1;
	}
	return 0;
}
int CVGSCore::Lua_GetModelNameByHandle(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long hScene = args[1].GetInteger();
		unsigned long ulModel = args[2].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		char buf[MAX_NAMESTR_LENGTH];
		ZeroMemory(buf, MAX_NAMESTR_LENGTH);
		bool bFlag = GetModelNameByHandle(hScene, ulModel, buf);
		if (!bFlag)
			return 0;
		state->PushString(buf);
		return 1;
	}
	return 0;
}
int CVGSCore::Lua_GetModelPosition(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long hScene = args[1].GetInteger();
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		VECTOR3 pos;
		GetModelPosition(hScene, ulModel, &pos);
		state->PushNumber(pos.x);
		state->PushNumber(pos.y);
		state->PushNumber(pos.z);
		return 3;
	}
	return 0;
}
int CVGSCore::Lua_SetModelPosition(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger() && args[3].IsTable())
	{
		unsigned long hScene = args[1].GetInteger();
		unsigned long ulModel = args[2].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[3];
		VECTOR3 pos;
		pos.x = (float)table[1].GetNumber();
		pos.y = (float)table[2].GetNumber();
		pos.z = (float)table[3].GetNumber();

		SetModelPosition(hScene, ulModel, &pos);

		return 0;
	}
	return 0;
}
int CVGSCore::Lua_MoveModel(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger() && args[3].IsTable() && args[4].IsInteger())
	{
		unsigned long hScene = args[1].GetInteger();
		unsigned long ulModel = args[2].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[3];
		VECTOR3 pos;
		pos.x = (float)table[1].GetNumber();
		pos.y = (float)table[2].GetNumber();
		pos.z = (float)table[3].GetNumber();

		unsigned int nFlag = args[4].GetInteger();

		MoveModel(hScene, ulModel, &pos, nFlag);

		return 0;
	}
	return 0;
}

int CVGSCore::Lua_GetModelRotation(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long hScene = args[1].GetInteger();
		unsigned long ulModel = args[2].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		VECTOR3 pos;
		if (GetModelRotation(hScene, ulModel, &pos))
		{
			state->PushNumber(pos.x);
			state->PushNumber(pos.y);
			state->PushNumber(pos.z);
			return 3;
		}
	}
	return 0;
}
int CVGSCore::Lua_SetModelRotation(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger() && args[3].IsTable())
	{
		unsigned long hScene = args[1].GetInteger();
		unsigned long ulModel = args[2].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		LuaObject table = args[3];
		VECTOR3 v3;
		v3.x = table[1].GetNumber();
		v3.y = table[2].GetNumber();
		v3.z = table[3].GetNumber();
		SetModelRotation(hScene, ulModel, &v3);
	}
	return 0;
}
int CVGSCore::Lua_RotateModel(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger() && args[3].IsTable() && args[4].IsInteger())
	{
		unsigned long hScene = args[1].GetInteger();
		unsigned long ulModel = args[2].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		LuaObject table = args[3];
		VECTOR3 v3;
		v3.x = table[1].GetNumber();
		v3.y = table[2].GetNumber();
		v3.z = table[3].GetNumber();

		VGS_TRANSFORMSPACE ts = (VGS_TRANSFORMSPACE)(args[4].GetInteger());

		RotateModel(hScene, ulModel, &v3, ts);
	}
	return 0;
}
int CVGSCore::Lua_GetModelScale(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long hScene = args[1].GetInteger();
		unsigned long ulModel = args[2].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		VECTOR3 pos;
		if (GetModelScale(hScene, ulModel, &pos))
		{
			state->PushNumber(pos.x);
			state->PushNumber(pos.y);
			state->PushNumber(pos.z);
			return 3;
		}
	}
	return 0;
}
int CVGSCore::Lua_SetModelScale(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger() && args[3].IsTable())
	{
		unsigned long hScene = args[1].GetInteger();
		unsigned long ulModel = args[2].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		LuaObject table = args[3];
		VECTOR3 v3;
		v3.x = table[1].GetNumber();
		v3.y = table[2].GetNumber();
		v3.z = table[3].GetNumber();
 		SetModelScale(hScene, ulModel, &v3);
	}
	return 0;
}
int CVGSCore::Lua_ScaleModel(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger() && args[3].IsTable())
	{
		unsigned long hScene = args[1].GetInteger();
		unsigned long ulModel = args[2].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		LuaObject table = args[3];
		VECTOR3 v3;
		v3.x = table[1].GetNumber();
		v3.y = table[2].GetNumber();
		v3.z = table[3].GetNumber();
		ScaleModel(hScene, ulModel, &v3);
	}
	return 0;
}
int CVGSCore::Lua_createFlashOverlay(LuaState* state)
{
	//Lua: createFlashOverlay(path, name)
	LuaStack args(state);
	if (args[1].IsString() && args[2].IsString())
	{
// 		std::string filename = args[1].GetString();
// 		std::string name = args[2].GetString();
// 		GfxMoviePtr movie = m_pRoot->getGfxMgrptr()->createMovie(filename);
// 		m_ptrSwfView = m_pRoot->getGfxMgrptr()->createMovieView(movie,name);
	}
	return 0;
}
int CVGSCore::Lua_createRenderTexture(LuaState* state)
{
	//Lua: createRenderTexture(width, height)
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger() )
	{
		unsigned long w = args[1].GetInteger();
		unsigned long h = args[2].GetInteger();

// 		m_pRoot->getGfxMgrptr()->CreateRenderTexture(w, h, true);
	}
	return 0;
}
int CVGSCore::Lua_createFlashRTT(LuaState* state)
{
	//Lua: createFlashRTT(path)
	LuaStack args(state);
	if (args[1].IsString())
	{
		std::string filename = args[1].GetString();

// 		GfxMoviePtr movie = m_pRoot->getGfxMgrptr()->createMovie(filename);
// 		m_pRoot->getGfxMgrptr()->createRTTMovieView(movie);//test
	}
	return 0;
}
int CVGSCore::Lua_setFlashRenderState(LuaState* state)
{
	//Lua: setFlashRenderStat(true/false)
	LuaStack args(state);
	if (args[1].IsBoolean())
	{
		bool flag = args[1].GetBoolean();

// 		flag ? m_pRoot->getGfxMgrptr()->startRender() : m_pRoot->getGfxMgrptr()->stopRender();
	}
	return 0;
}

static int GetKeyModifiers()
{
	if (::GetKeyState(VK_CONTROL) & 0x8000)
		return 1;
	if (::GetKeyState(VK_MENU) & 0x8000)//alt
		return 2;
	if (::GetKeyState(VK_SHIFT) & 0x8000)
		return 3;
	return -1;
}

void CVGSCore::MessageWndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!mInitialized)
		return;
// 	if (m_pRoot->getGfxMgrptr())
// 	{
// 		m_pRoot->getGfxMgrptr()->MessageWndProc( message, wParam, lParam);
// 	}

	int screenX = LOWORD(lParam);
	int screenY = HIWORD(lParam);
	switch(message)
	{
	case WM_SIZE:
		m_pLuaEngine->OnResize();
		break;
	case WM_ACTIVATE:
		m_pLuaEngine->OnActive();
		break;
	case WM_CHAR:
		m_pLuaEngine->OnKeyPressEvent(wParam, GetKeyModifiers());
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			m_pLuaEngine->OnKeyDownEvent(wParam, GetKeyModifiers());
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		m_pLuaEngine->OnKeyUpEvent(wParam, GetKeyModifiers());
		break;
	case WM_LBUTTONDOWN:
		m_pLuaEngine->AllMouseDown(0, GetKeyModifiers(),screenX, screenY);			
		//TODO:需要优化，主要是应当获取场景裁减等操作后的模型列表.	林秀全.
		for (vector<CScene*>::const_iterator it = g_vScene.begin(); it != g_vScene.end(); ++it)
		{
			vector<CModel*>::const_iterator it2 = (*it)->model.begin(), end2 = (*it)->model.end();
			for (it2; it2 != end2; ++it2)
			{
				if ((*it2)->getMouseLDwonFuncName().empty() || !(*it2)->m_isClickable)
					continue;
				m_pLuaEngine->OnMouseDown((*it2)->getMouseLDwonFuncName().c_str(), 0, GetKeyModifiers(),screenX, screenY);
			}
		}
		break;
	case WM_LBUTTONUP:
		m_pLuaEngine->AllMouseUp(0, GetKeyModifiers(),screenX, screenY);			
		break;
	case WM_LBUTTONDBLCLK:
		m_pLuaEngine->AllMouseDoubleClick(0, GetKeyModifiers(),screenX, screenY);			
		for (vector<CScene*>::const_iterator it = g_vScene.begin(); it != g_vScene.end(); ++it)
		{
			vector<CModel*>::const_iterator it2 = (*it)->model.begin(), end2 = (*it)->model.end();
			for (it2; it2 != end2; ++it2)
			{
				if ((*it2)->getMouseDBClickFuncName().empty() || !(*it2)->m_isClickable)
					continue;
				m_pLuaEngine->OnMouseDoubleClick((*it2)->getMouseDBClickFuncName().c_str(), 0, GetKeyModifiers(),screenX, screenY);
			}
		}
		break;
	case WM_RBUTTONDOWN:
		m_pLuaEngine->AllMouseDown(1, GetKeyModifiers(),screenX, screenY);			
		break;
	case WM_RBUTTONUP:
		m_pLuaEngine->AllMouseUp(1, GetKeyModifiers(),screenX, screenY);			
		break;
	case WM_RBUTTONDBLCLK:
		m_pLuaEngine->AllMouseDoubleClick(1, GetKeyModifiers(),screenX, screenY);			
		for (vector<CScene*>::const_iterator it = g_vScene.begin(); it != g_vScene.end(); ++it)
		{
			vector<CModel*>::const_iterator it2 = (*it)->model.begin(), end2 = (*it)->model.end();
			for (it2; it2 != end2; ++it2)
			{
				if ((*it2)->getMouseDBClickFuncName().empty() || !(*it2)->m_isClickable)
					continue;
				m_pLuaEngine->OnMouseDoubleClick((*it2)->getMouseDBClickFuncName().c_str(), 0, GetKeyModifiers(),screenX, screenY);
			}
		}
		break;
	case WM_MBUTTONDBLCLK:
		m_pLuaEngine->AllMouseDoubleClick(2, GetKeyModifiers(),screenX, screenY);			
		break;
	case WM_MOUSEHOVER:
		m_pLuaEngine->AllMouseEnter(GetKeyModifiers(),screenX, screenY);			
		break;
	case WM_MOUSELEAVE:
		m_pLuaEngine->AllMouseLeave(GetKeyModifiers(),screenX, screenY);			
		break;
	case WM_MOUSEMOVE:
		m_pLuaEngine->AllMouseMove(GetKeyModifiers(),screenX, screenY);			
		break;
	case WM_MOUSEWHEEL:
		{
			int zDelta = HIWORD(wParam);    // wheel rotation
			m_pLuaEngine->AllMouseWheel(GetKeyModifiers(),screenX, screenY, zDelta);
		}
		break;
	default:
		break;
	}
}
void CVGSCore::setN3dDir(const char* n3dDir)
{
	m_strN3dDir = n3dDir;

// 	if(  m_pRoot->getGfxMgrptr() )
// 	{
// 		m_pRoot->getGfxMgrptr()->setMediaDir(m_strN3dDir);
// 	}


	//执行vgs.lua脚本	2010-5-26 linxq
	if (m_pLuaEngine->DoFile((m_strN3dDir + "vgs.lua").c_str()))
	{
		m_pLuaEngine->OnInitVgs();
	}
}
