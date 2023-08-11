// VGS2.cpp : Implementation of CVGS2
#include "stdafx.h"
//#include "VGS2ATL.h"
#include "VGS2.h" 

#include "InvokeJSFunc.h"
#include <comdef.h>
#include "CSysFunc.h"
#include "VGSCommonFunc.h"

#include ".\\Download\\VGSFileDownloadManager.h"


extern class CVGSATL2Module _AtlModule;


/////////////////////////////////////////////////////////////////////////////
// CVGS2
/////////////////////////////////////////////////////////////////////////////
INT	wndIndex = 0; //窗体编号
DWORD lastTimer = 0; //计时器

WEBBROWSERTYPE  g_BrowserType = WBT_IE;        // 浏览器类型，0 ：ie 1：ff 2：google
WEBBROWSER_SUBTYPE g_BrowserSubType = WBST_IE;

// 将bstr转换成string的宏
// #define _BSTR2STR(bstrStr) (std::string((char*)_bstr_t(bstrStr)))
std::string _BSTR_TO_STR(BSTR bstrStr) 
{
	if (bstrStr)
	{
		return std::string((char*)_bstr_t(bstrStr)); 
	}
	return "";
}

// 检查窗体是否有效
#define _VGS_RETURN {if (!m_pVGS2Wnd) return E_FAIL;}



//构造函数
CVGS2::CVGS2()
{	
	extern __declspec(selectany) CAtlModule* _pAtlModule;
	_pAtlModule = (CAtlModule*)(&_AtlModule);


	m_bWindowOnly = TRUE;
	m_pVGS2Wnd = NULL;	

	m_theTextHandle = 0;
	m_theModelHandle = 0;
	m_theCamHandle = 0; 
	m_theSceneResHandle = 0;
	m_strflashVar = "";

	// default for ie
	m_bRequiresSave = false;
	m_BrowserType   = g_BrowserType;  

	ZeroMemory(m_webServer, sizeof(m_webServer));
	ZeroMemory(m_localPath, sizeof(m_localPath));
}

//析构函数
CVGS2::~CVGS2()
{
	//	Release();
}


//---------------------------------------------------------------------------------------------------
//方法: OnCreate()
//参数: UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/
//描述: ATL创建时调用的方法
//----------------------------------------------------------------------------------------------------
LRESULT CVGS2::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//初始化Com接口
	CoInitialize(NULL);

	//创建VGS2窗体对象
	if (m_pVGS2Wnd != NULL)
	{
		::DestroyWindow(m_pVGS2Wnd->m_hWnd);
		SAFE_DELETE(m_pVGS2Wnd);
	}

	//初始化线程同步对象
	InitializeCriticalSection(&m_render_cs);
	InitializeCriticalSection(&m_ctrl_cs);

	//创建窗体对象
	m_pVGS2Wnd = new CVGS2Window(m_spClientSite, &m_render_cs, &m_ctrl_cs);
	if (m_pVGS2Wnd == NULL)
	{
		return E_FAIL;
	}

	m_pVGS2Wnd->SetVGSCore(&m_VGSEngine);

	g_BrowserType = (WEBBROWSERTYPE)m_BrowserType;//WBT_FF;WBT_IE
// 	m_pVGS2Wnd->m_BrowserType = g_BrowserType;  // 浏览器类型
	m_pVGS2Wnd->SetVGS2Interface(this);

	RECT rc;
	GetClientRect(&rc);
	m_pVGS2Wnd->Create(m_hWnd, rc);
	::SetWindowLong(m_pVGS2Wnd->m_hWnd, GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP);

	//::SetBkColor(::GetDC(m_pVGS2Wnd->m_hWnd),RGB(0,0,0));

	//创建网络会话对象
	CreateInternetSession();

	// 设置下载器
	//m_pVGS2Wnd->GetDownloadN3DFileManager().setInternetSession(m_hInternetSession);
	//m_pVGS2Wnd->GetDownloadN3DFileManager().setCriticalSection(&m_ctrl_cs, &m_render_cs);
	// m_pVGS2Wnd->GetDownloadN3DFileManager().SetMsgWndHandle(m_pVGS2Wnd->m_hWnd);


	// m_pVGS = m_pVGS2Wnd->getVGSObject();   // 获取vgs对象
	m_pVGS2Wnd->EnableStartEngineCall();  // 回调脚本入口函数

	return S_OK;
}

//---------------------------------------------------------------------------------------------------
//方法: OnDestroy()
//参数: UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/
//描述: ATL创建时调用的方法
//----------------------------------------------------------------------------------------------------
LRESULT CVGS2::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	if(m_pVGS2Wnd == NULL) return E_FAIL;

	ReleaseSession();

	/*if (m_pDownloadN3DFileManager)
	{
		m_pDownloadN3DFileManager->ClearAll();
		delete m_pDownloadN3DFileManager;
		m_pDownloadN3DFileManager = 0;
	}*/

	m_VGSEngine.SunRelease();

	::DestroyWindow(m_pVGS2Wnd->m_hWnd);
	SAFE_DELETE(m_pVGS2Wnd);

	//释放线程同步对象
	DeleteCriticalSection(&m_render_cs);
	DeleteCriticalSection(&m_ctrl_cs);

	//释放Com接口
	CoUninitialize();

	return S_OK;
}

//---------------------------------------------------------------------------------------------------
//方法: OnSize()
//参数: UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/
//描述: ATL改变尺寸时调用的方法
//----------------------------------------------------------------------------------------------------
LRESULT CVGS2::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	if(m_pVGS2Wnd == NULL) return E_FAIL;

	RECT rc;
	GetClientRect(&rc);
	::SetWindowPos(	m_pVGS2Wnd->m_hWnd, NULL, rc.left, rc.top, 
		rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
	return S_OK;
}

//-----------------------------------------------------------------------------
// 方法名: CreateInternetSession
// 参  数: 
// 用  途: 创建网络会话对象
//-----------------------------------------------------------------------------
BOOL CVGS2::CreateInternetSession()
{
	m_hInternetSession = 0;

	//创建网络会话对象
	char AppName[256];
	char strModuleName[256];
	GetModuleFileName(NULL, strModuleName, 256);
 	GetAppName(AppName, strModuleName);
 	m_hInternetSession = InternetOpen(AppName /*"iexplore"*/,
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL, NULL, 0);
	if(m_hInternetSession == NULL)
	{
		::MessageBox(GetActiveWindow(), 
			         ("Create Internet session error."), 
					 ("Now creating Internet session..."), 
					 MB_OK|MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------
// 方法名: ReleaseSession
// 参  数: 
// 用  途: 关闭及释放网络会话对象
//----------------------------------------------------------------
VOID CVGS2::ReleaseSession()
{
	if (m_hInternetSession)
	{
		InternetCloseHandle(m_hInternetSession);
	}
}

//---------------------------------------------------------------------------------------------------
//以下是JavaScript接口
//---------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//方法: SetModelVisibility()
//参数: BSTR modelName, BOOL visible
//描述: 设置模型的可见性
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetModelVisibility(BSTR modelName, BOOL visible)
{
	// if (m_pVGS)
	{
		DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
		m_VGSEngine.SunSetModelVisibility(modelHandle, visible);
		return S_OK;
	}

	// return E_FAIL;
}

//------------------------------------------------------------------------------
//方法: SetDynamicUV()
//参数: BSTR modelName, FLOAT u, FLOAT v, FLOAT speed
//描述: 设置模型的卷动UV
//modelName为模型的名称， u,v 为uv方向的移动步长， speed为调整倍率
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetDynamicUV(BSTR modelName, FLOAT u, FLOAT v, FLOAT speed)
{
	// 	if (m_pVGS2Wnd == NULL) return E_FAIL;
	// 
	// 	//转换BSTR
	// 	char pModelName[32];
	// 	strcpy(pModelName, (char *)_bstr_t(modelName));
	// 	//设置模型的卷动UV
	// 	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName(pModelName);
	// 	if (!modelHandle)
	// 		return S_OK;
	// 
	// 	m_VGSEngine.SunSetDynamicUVStatus(modelHandle, TRUE);
	// 	m_VGSEngine.SunSetDynamicUVProp(modelHandle, u, v, speed);
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: AddFlashCtrl()
//参数: BSTR flashName, BSTR flashFile, INT alignMode, INT left, INT top, INT width, INT height, BOOL loop
//描述: 增加Flash控件到场景
//flashName为FlashCtrl的别名， flashFile为Flash文件的地址
//left, top, width, height为FlashCtrl的Rect, alignMode为基准位置，left, top, 为相对位置，width-height为宽和高
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::AddFlashCtrl(BSTR flashName, BSTR flashFile, INT alignMode, INT left, INT top, INT width, INT height, BOOL loop)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	EnterCriticalSection(&m_ctrl_cs);
	//	//转换BSTR
	//	char pFlashName[32];
	//	strcpy(pFlashName, (char *)_bstr_t(flashName));
	//
	//	char pFlashFilePathAndName[512];
	//	char pFlashFile[512];
	//	strcpy(pFlashFile, (char*)_bstr_t(flashFile));
	//	strcpy(pFlashFilePathAndName, m_webServer);
	//	strcat(pFlashFilePathAndName, pFlashFile);
	//	
	//	CATLFlashCtrl* pFlash = new CATLFlashCtrl(m_pVGS2Wnd->m_hWnd, m_spClientSite);
	//	
	//	pFlash->index = m_pVGS2Wnd->m_Flash.size();
	//	strcpy(pFlash->name, pFlashName);
	//	pFlash->alignMode = alignMode;
	//
	//	//保存相对alignMode的位置
	//	pFlash->left = left;
	//	pFlash->top = top;
	//	pFlash->width = width;
	//	pFlash->height = height;
	//
	//	pFlash->oriLeft = left;
	//	pFlash->oriTop = top;
	//
	//	//flash file 
	//	_bstr_t bst(pFlashFilePathAndName);
	//	pFlash->m_strMovieName = bst.copy();
	//
	//	//判断前一个Flash是否下载完
	//	if (pFlash->index > 0)
	//	{
	//		CATLFlashCtrl** ppFlash = m_pVGS2Wnd->m_Flash.begin();
	//		int preIndex = pFlash->index - 1;
	//		CATLFlashCtrl* pPreFlash = *(ppFlash + preIndex);
	//		if(pPreFlash->GetLoadPercent() < 100)
	//		{
	//			m_pVGS2Wnd->m_Flash.push_back(pFlash);
	//			//pFlash->playStatus = 1;
	//			LeaveCriticalSection(&m_ctrl_cs);
	//			return S_OK;
	//		}
	//	}
	//
	//	RECT clientRect;
	//	GetClientRect(&clientRect);
	//	
	//	INT realLeft, realTop;
	//	//对齐模式
	//	switch ((ALIGNMODE)alignMode)
	//	{	
	//	case ALIGN_NONE:  //0
	//		realLeft = clientRect.left;
	//		realTop = clientRect.top;
	//		break;
	//	case ALIGN_LEFT://1
	//		realLeft = clientRect.left;
	//		break;
	//	case ALIGN_RIGHT://2
	//		realLeft = clientRect.right - pFlash->width;
	//		break;
	//	case ALIGN_BOTTOM://3
	//		realTop = clientRect.bottom - pFlash->height;
	//		break;
	//	case ALIGN_TOP://4
	//		realTop = clientRect.top;
	//		break;
	//	case ALIGN_LEFT_TOP://5
	//		realLeft = clientRect.left;
	//		realTop = clientRect.top;
	//		break;
	//	case ALIGN_MIDDLE_TOP://6
	//		realLeft = (clientRect.left + clientRect.right)/2 - pFlash->width/2;
	//		realTop =  clientRect.top;
	//		break;
	//	case ALIGN_RIGHT_TOP://7
	//		realLeft = clientRect.right - pFlash->width;
	//		realTop = clientRect.top;
	//		break;
	//	case ALIGN_LEFT_BOTTOM://8
	//		realLeft = clientRect.left;
	//		realTop = clientRect.bottom - pFlash->height;
	//		break;
	//	case ALIGN_MIDDLE_BOTTOM://9
	//		realLeft = (clientRect.left + clientRect.right)/2 - pFlash->width/2;
	//		realTop = clientRect.bottom - pFlash->height;
	//		break;
	//	case ALIGN_RIGHT_BOTTOM://10
	//		realLeft = clientRect.right - pFlash->width;
	//		realTop = clientRect.bottom - pFlash->height;
	//		break;
	//	case ALIGN_CENTER://11
	//		realLeft =  (clientRect.left + clientRect.right)/2 - pFlash->width/2;
	//		realTop = (clientRect.top + clientRect.bottom)/2 - pFlash->height/2;
	//		break;
	//	}
	//
	//	//增加相对位置
	//	realLeft += pFlash->left;
	//	realTop += pFlash->top;
	//
	//	//宽度占满吗?
	//	if (width < 0)
	//	{
	//		pFlash->isFullWidth = TRUE;
	//		width = clientRect.right - clientRect.left;
	//	}
	//	//高度度占满吗?
	//	if (height < 0)
	//	{
	//		pFlash->isFullHeight = TRUE;
	//		height = clientRect.bottom - clientRect.top;
	//	}
	//
	//	RECT& rect = clientRect;
	//	pFlash->Create(pFlashName, rect, realLeft, realTop, width, height, pFlash->m_strMovieName, 30, TRUE);
	//	pFlash->Play(loop);
	//	m_pVGS2Wnd->m_Flash.push_back(pFlash);
	//
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: DeleteFlashCtrl()
//参数: BSTR flashName
//描述: 删除场景中的Flash控件
//flashName为FlashCtrl的别名
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::DeleteFlashCtrl(BSTR flashName)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	char fName[256];
	//	strcpy(fName,(char *)_bstr_t(flashName));
	//
	//	CATLFlashCtrl** ppFlash = m_pVGS2Wnd->m_Flash.begin();
	//	INT flashCnt = m_pVGS2Wnd->m_Flash.size();
	//	for (UINT i =0; i< flashCnt; i++)
	//	{
	//		CATLFlashCtrl* pFlash = *(ppFlash + i);
	//		if (strcmp(pFlash->name, (char *)_bstr_t(flashName)) == 0)
	//		{
	//			//停止Flash
	//			//pFlash->Stop();
	//			//删除指针//释放IShockwaveFlash接口
	//			pFlash->Delete();
	//			SAFE_RELEASE(pFlash);
	//			SAFE_DELETE(pFlash);  
	//			//从列表中删除指针
	//			m_pVGS2Wnd->m_Flash.erase(ppFlash + i);
	//			break;
	//		}
	//	}
	//		
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}


//------------------------------------------------------------------------------
//方法: AddFlashCtrl()
//参数: BSTR flashName, INT alignMode, INT left, INT top, INT width, INT height
//描述: 设置Flash的Rect, 
//flashName为FlashCtrl的别名，left, top, width, height为Rect,
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetFlashRect(BSTR flashName, INT alignMode, INT left, INT top, INT width, INT height)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	INT flashCnt = m_pVGS2Wnd->m_Flash.size();
	//	CATLFlashCtrl** ppFlash = m_pVGS2Wnd->m_Flash.begin();
	//	for (UINT i =0; i< flashCnt; i++)
	//	{
	//		CATLFlashCtrl* pFlash = *(ppFlash + i);
	//		if (strcmp(pFlash->name, (char *)_bstr_t(flashName)) == 0)
	//		{
	//			//保存相对alignMode的位置
	//			pFlash->left = left;
	//			pFlash->top = top;
	//			pFlash->width = width;
	//			pFlash->height = height;
	//			
	//			RECT clientRect = m_pVGS2Wnd->m_ClientRect;
	//			INT realLeft, realTop;
	//			//对齐模式
	//			switch ((ALIGNMODE)alignMode)
	//			{	
	//			case ALIGN_NONE:
	//				break;
	//			case ALIGN_LEFT:
	//				realLeft = clientRect.left;
	//				realTop = clientRect.top;
	//				break;
	//			case ALIGN_RIGHT:
	//				realLeft = clientRect.right - pFlash->width;
	//				break;
	//			case ALIGN_BOTTOM:
	//				realTop = clientRect.bottom - pFlash->height;
	//				break;
	//			case ALIGN_TOP:
	//				realTop = clientRect.top;
	//				break;
	//			case ALIGN_LEFT_TOP:
	//				realLeft = clientRect.left;
	//				realTop = clientRect.top;
	//				break;
	//			case ALIGN_MIDDLE_TOP:
	//				realLeft = (clientRect.left + clientRect.right)/2 - pFlash->width/2;
	//				realTop =  clientRect.top;
	//				break;
	//			case ALIGN_RIGHT_TOP:
	//				realLeft = clientRect.right - pFlash->width;
	//				realTop = clientRect.top;
	//				break;
	//			case ALIGN_LEFT_BOTTOM:
	//				realLeft = clientRect.left;
	//				realTop = clientRect.bottom - pFlash->height;
	//				break;
	//			case ALIGN_MIDDLE_BOTTOM:
	//				realLeft = (clientRect.left + clientRect.right)/2 - pFlash->width/2;
	//				realTop = clientRect.bottom - pFlash->height;
	//				break;
	//			case ALIGN_RIGHT_BOTTOM:
	//				realLeft = clientRect.right - pFlash->width;
	//				realTop = clientRect.bottom - pFlash->height;
	//				break;
	//			case ALIGN_CENTER:
	//				realLeft =  (clientRect.left + clientRect.right)/2 - pFlash->width/2;
	//				realTop = (clientRect.top + clientRect.bottom)/2 - pFlash->height/2;
	//				break;
	//			}
	//
	//			//增加相对位置
	//			realLeft += pFlash->left;
	//			realTop += pFlash->top;
	//
	//			//宽度占满吗?
	//			if (width < 0)
	//			{
	//				pFlash->isFullWidth = TRUE;
	//				width = clientRect.right - clientRect.left;
	//			}
	//			//高度度占满吗?
	//			if (height < 0)
	//			{
	//				pFlash->isFullHeight = TRUE;
	//				height = clientRect.bottom - clientRect.top;
	//			}
	//
	//			pFlash->SetRect(realLeft, realTop, width, height);
	//
	//			//这里方法如下：可以先把场景暂停渲染，然后把后缓冲区的像素在当前调用中刷新，
	//			//这样可以不用实时渲染场景，开销会小很多
	//			m_VGSEngine.SunSetRenderStatus(FALSE);
	//			m_VGSEngine.vgsScene->m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	//			m_VGSEngine.SunSetRenderStatus(TRUE);
	//			break;
	//		}
	//	}

	return S_OK;
}

//移动Flash控件
STDMETHODIMP CVGS2::MoveFlash(BSTR flashName, INT x, INT y)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	INT flashCnt = m_pVGS2Wnd->m_Flash.size();
	//	CATLFlashCtrl** ppFlash = m_pVGS2Wnd->m_Flash.begin();
	//	for (UINT i =0; i< flashCnt; i++)
	//	{
	//		CATLFlashCtrl* pFlash = *(ppFlash + i);
	//		if (strcmp(pFlash->name, (char *)_bstr_t(flashName)) == 0)
	//		{
	//			//增加相对位置
	//			int realLeft = pFlash->left + x;
	//			int realTop = pFlash->top + y;
	//
	//			int width = pFlash->width;
	//			int height = pFlash->height;
	//
	//			RECT clientRect = m_VGSEngine.vgsScene->rect;
	//
	//			//宽度占满吗?
	//			if (width < 0)
	//			{
	//				pFlash->isFullWidth = TRUE;
	//				width = clientRect.right - clientRect.left;
	//			}
	//			//高度度占满吗?
	//			if (height < 0)
	//			{
	//				pFlash->isFullHeight = TRUE;
	//				height = clientRect.bottom - clientRect.top;
	//			}
	//
	//			pFlash->SetRect(realLeft, realTop, width, height);
	//
	//			//这里方法如下：可以先把场景暂停渲染，然后把后缓冲区的像素在当前调用中刷新，
	//			//这样可以不用实时渲染场景，开销会小很多
	//			m_VGSEngine.SunSetRenderStatus(FALSE);
	//			m_VGSEngine.vgsScene->m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	//			m_VGSEngine.SunSetRenderStatus(TRUE);
	//
	//			break;
	//		}
	//	}

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetFlashCtrlVisibility()
//参数: BSTR flashName, BOOL status
//描述: 设置Flash是否可见
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetFlashCtrlVisibility(BSTR flashName, BOOL status)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//	
	//	INT flashCnt = m_pVGS2Wnd->m_Flash.size();
	//	CATLFlashCtrl** ppFlash = m_pVGS2Wnd->m_Flash.begin();
	//	for (UINT i =0; i< flashCnt; i++)
	//	{
	//		CATLFlashCtrl* pFlash = *(ppFlash + i);
	//		if (strcmp(pFlash->name, (char *)_bstr_t(flashName)) == 0)
	//		{
	//			pFlash->SetVisible(status);
	//			break;
	//		}
	//	}

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetFlashPlayStatus()
//参数: BSTR flashName, INT status
//描述: 设置Flash播放状态，0为暂停播放，1为从暂停点开始播放，2为停止并跳转到第一帧
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetFlashPlayStatus(BSTR flashName, INT status)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	INT flashCnt = m_pVGS2Wnd->m_Flash.size();
	//	CATLFlashCtrl** ppFlash = m_pVGS2Wnd->m_Flash.begin();
	//	for (UINT i =0; i< flashCnt; i++)
	//	{
	//		CATLFlashCtrl* pFlash = *(ppFlash + i);
	//		if (strcmp(pFlash->name, (char *)_bstr_t(flashName)) == 0)
	//		{
	//			EnterCriticalSection(&m_ctrl_cs);
	//			EnterCriticalSection(&m_render_cs);
	//			switch(status)
	//			{
	//			case 0:
	//				pFlash->Pause();
	//				break;
	//			case 1:
	//				pFlash->Play(TRUE);
	//				break;
	//			case 2:
	//				pFlash->Stop();
	//				break;
	//			}
	//			LeaveCriticalSection(&m_render_cs);
	//			LeaveCriticalSection(&m_ctrl_cs);
	//		}
	//	}	
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetRenderStatus()
//参数:  BOOL status
//描述: 设置场景是否渲染
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetRenderStatus(BOOL status)
{
	_VGS_RETURN	

		//RECT clientRect = m_pVGS2Wnd->m_ClientRect;
		//进入临界区
		EnterCriticalSection(&m_ctrl_cs);
	EnterCriticalSection(&m_render_cs);
	m_VGSEngine.SunSetRenderStatus(status);
	if (status && m_pVGS2Wnd->m_bActive)
	{
		//Edit by Bao jiahui 2007 11 22 
		//更新场景模型的顶点缓冲
		/*if (!m_VGSEngine.SunUpdateBackBuffer(&clientRect))
		{
		LeaveCriticalSection(&m_render_cs);//---
		LeaveCriticalSection(&m_ctrl_cs);//---
		return S_OK;
		}*/

		m_VGSEngine.SunRenderScene();

	}

	//进入临界区
	LeaveCriticalSection(&m_render_cs);
	LeaveCriticalSection(&m_ctrl_cs);

	//更新Flash的Rect
	//m_pVGS2Wnd->UpdateFlashRect();
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: MoveCamera()
//参数: BSTR cameraName, FLOAT x, FLOAT y, FLOAT z, INT stepCnt, INT coordEnum
//描述: 移动指定摄像机
// cameraName为摄像机名称
// x, y, z为移动的距离
// stepCnt为移动的步数
// coordEnum为参考坐标系(from和at一起移动) 
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::MoveCamera(BSTR cameraName, FLOAT x, FLOAT y, FLOAT z, INT stepCnt, INT coordEnum)
{
	DWORD cameraHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(cameraName));
	VECTOR3 vec;
	vec.x = x;
	vec.y = y;
	vec.z = z;
	SetRenderStatus(0);
	for (INT i = 0; i< stepCnt; i++)
	{
		m_VGSEngine.SunMoveCamera(cameraHandle, &vec, coordEnum);
		m_VGSEngine.SunRenderScene();
	}
	SetRenderStatus(1);

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: MoveCurrentCamera()
//参数: BSTR cameraName, FLOAT x, FLOAT y, FLOAT z, INT stepCnt, INT coordEnum
//描述: 移动当前摄像机(from和at一起移动) 
//	stepCnt为步长
//0:在世界坐标系平移摄像机
//1:在自身坐标系平移
//2:walk坐标系，不能飞到天上去
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::MoveCurrentCamera(FLOAT x, FLOAT y, FLOAT z, INT stepCnt, INT coordEnum)
{
	DWORD cameraHandle = m_VGSEngine.SunGetCurrentCamera();
	VECTOR3 vec;
	vec.x = x;
	vec.y = y;
	vec.z = z;
	SetRenderStatus(0);
	for (INT i = 0; i< stepCnt; i++)
	{
		m_VGSEngine.SunMoveCamera(cameraHandle, &vec, coordEnum);
		m_VGSEngine.SunRenderScene();
	}
	SetRenderStatus(1);

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: ShowDefaultLogo()
//参数: BOOL status
//描述: 是否显示默认的Logo
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::ShowDefaultLogo(BOOL status)
{
	if (m_pVGS2Wnd == NULL) return E_FAIL;

	m_pVGS2Wnd->m_ShowDefaultLogo = status;
	m_VGSEngine.SunRenderScene();
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetControlStatus()
//参数: INT status
//描述: 设置控制状态
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetControlStatus(INT status)
{
	if (m_pVGS2Wnd == NULL) return E_FAIL;

	//0--WALK模式, 1--Edit模式, 2--FLY模式, 3为None模式
	m_VGSEngine.SunSetControlStatus(_getControlStatus(status));
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetCameraClip()
//参数: BSTR cameraName, FLOAT nearClip, FLOAT farClip
//描述: 设置像机的剪切平面
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetCameraClip(BSTR cameraName, FLOAT nearClip, FLOAT farClip)
{
	if (m_pVGS2Wnd == NULL) return E_FAIL;

	DWORD camHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(cameraName));
	m_VGSEngine.SunSetCameraFarClip(camHandle, farClip);
	m_VGSEngine.SunSetCameraNearClip(camHandle, nearClip);

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetCurrentCameraClip()
//参数: FLOAT nearClip, FLOAT farClip
//描述: 设置当前像机的剪切平面
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetCurrentCameraClip(FLOAT nearClip, FLOAT farClip)
{
	_VGS_RETURN

		DWORD cam =	m_VGSEngine.SunGetCurrentCamera();
	m_VGSEngine.SunSetCameraFarClip(cam, farClip);
	m_VGSEngine.SunSetCameraNearClip(cam, nearClip);

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: DollyCurrentCamera()
//参数: FLOAT value
//描述: 推拉当前像机的镜头
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::DollyCurrentCamera(FLOAT value, INT stepCnt)
{
	_VGS_RETURN

		DWORD cam =	m_VGSEngine.SunGetCurrentCamera();
	for (INT i = 0; i < stepCnt; i++)
	{
		m_VGSEngine.SunDollyCamera(cam, value);
	}

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: DollyCurrentCamera()
//参数: BSTR cameraName, FLOAT value
//描述: 推拉指定像机的镜头
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::DollyCamera(BSTR cameraName, FLOAT value, INT stepCnt)
{	
	_VGS_RETURN

		DWORD cam =	m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(cameraName));
	for (INT i = 0; i < stepCnt; i++)
	{
		m_VGSEngine.SunDollyCamera(cam, value);
	}

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetBackgroundColor()
//参数: INT red, INT green, INT blue
//描述: 设置当前渲染背景色
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetBackgroundColor(INT red, INT green, INT blue)
{
	_VGS_RETURN

		VGSRGB color;
	color.red = red;
	color.green = green; 
	color.blue = blue;
	m_VGSEngine.SunSetBackground(&color);
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: PreloadN3DFile()
//参数: BSTR strFileName, BSTR N3DName
//描述: 把网络上指定的N3D文件先下载到内存，但不加载到场景，
//这样可以在播放上一个场景时在后台下载
//其它场景，strFileName为网络上的N3D文件名，N3DName为其代名词
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::PreloadN3DFile(BSTR strN3DFileName, BSTR N3DName)
{
	/*
	if (m_pVGS2Wnd == NULL) return E_FAIL;
	if (m_VGSEngine.vgsScene == NULL) return E_FAIL;
	if (m_hInternetSession == NULL) return E_FAIL;

	//转换BSTR
	char pN3DFileName[512];
	strcpy(pN3DFileName, m_webServer);
	strcat(pN3DFileName, (char *)_bstr_t(strN3DFileName));

	//将Load的N3D文件放进队列
	CDownLoadN3DFile* pN3D = new CDownLoadN3DFile(m_VGSEngine.vgsScene,
	&m_ctrl_cs,
	&m_render_cs,
	m_hInternetSession);
	strcpy(pN3D->name, (char*)_bstr_t(N3DName));
	strcpy(pN3D->file, pN3DFileName);
	pN3D->type = 1;  //下载并但不加载到场景模式
	pN3D->index = m_pVGS2Wnd->m_LoadN3DObj.size();
	m_pVGS2Wnd->m_LoadN3DObj.push_back(pN3D);


	//----------------------------------------------------------
	//如果是网络文件
	//----------------------------------------------------------
	if (m_webServer[0] == 'h' && m_webServer[1] == 't' && m_webServer[2] == 't' && m_webServer[3] == 'p')
	{
	m_IsWebFile = TRUE;

	//判断前一个N3D是否已经下载完
	if (pN3D->index > 0)
	{
	INT preIndex = pN3D->index - 1;
	CDownLoadN3DFile** ppN3D = m_pVGS2Wnd->m_LoadN3DObj.begin();
	CDownLoadN3DFile* _pN3D = *(ppN3D + preIndex);
	if (!_pN3D->m_bAllDown)
	return S_OK;
	}

	//在后台下载N3D文件
	if (pN3D->DownLoadHttpFile(pN3DFileName, NULL, m_pVGS2Wnd->m_hWnd) == false)
	return E_FAIL;
	}

	//----------------------------------------------------------
	//如果是单机文件
	//----------------------------------------------------------
	else if (m_webServer[0] == 'f' && m_webServer[1] == 'i' && m_webServer[2] == 'l' && m_webServer[3] == 'e')
	{
	m_IsWebFile = FALSE;
	for (INT i = 0; i< 512 -4; i++)
	pN3DFileName[i] = pN3DFileName[i + 8];
	m_pVGS2Wnd->ReadLocalN3DFile(pN3D, pN3DFileName);
	}
	*/

	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: GotoN3D()
//参数: BSTR N3DName
//描述: 跳到后台正在加载或者已经加载的N3D场景，N3DName为其代名词，加载新的场景之前，
//会释放当前场景的内存。
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::GotoN3D(BSTR N3DName)
{	
	/*
	if (m_pVGS2Wnd == NULL) return E_FAIL;

	if (m_pVGS2Wnd->pSceneManager != NULL)
	m_pVGS2Wnd->pSceneManager->bActive = FALSE;

	//先停止渲染，并将背景色清空
	m_VGSEngine.SunSetRenderStatus(FALSE);
	UINT N3DCnt = m_pVGS2Wnd->m_PreloadN3DObj.size();
	CHAR name[32];
	strcpy(name, (char*)_bstr_t(N3DName));
	for (UINT i = 0; i< N3DCnt; i++)
	{
	if (strcmp(name, m_pVGS2Wnd->m_PreloadN3DObj[i]->name) == 0)
	{		
	//可以构造新场景了
	m_pVGS2Wnd->m_bCanConstructScene = TRUE;

	m_VGSEngine.SunResetScene();
	m_pVGS2Wnd->InitVars();
	m_pVGS2Wnd->InitOther();

	//设置当前播放的场景编号--放在InitVariable后面，因为InitVariable初始化了m_CurN3DIndex
	m_pVGS2Wnd->m_CurN3DIndex =  m_pVGS2Wnd->m_PreloadN3DObj[i]->index;

	m_pVGS2Wnd->m_bDownType = 1; 
	m_VGSEngine.SunSetRenderStatus(FALSE);

	m_pVGS2Wnd->WebFileObj = m_pVGS2Wnd->m_PreloadN3DObj[i];
	if (m_pVGS2Wnd->WebFileObj == NULL) 
	return E_FAIL;

	//设置场景管理器的Downloader
	m_pVGS2Wnd->pSceneManager->m_pN3DDownLoader = m_pVGS2Wnd->WebFileObj;

	break;
	}
	}

	//判断当前goto的这个N3D文件的场景数据是否已经全部下载到内存，如果已经下载完，则构造场景
	if (m_pVGS2Wnd->WebFileObj->m_bSceneDown) 
	{
	//判断Goto之前贴图下载了多少个
	m_pVGS2Wnd->m_DownedTexIndex = m_pVGS2Wnd->WebFileObj->m_DownedTexCnt -1;
	//这时候要把当前已经下载的数据大小保存一下，因为接下来的这个创建场景及贴图的循环需要时间，
	//当贴图创建完，有可能下载线程把贴图都下载完了，导致后面的贴图不能创建了。所以可能会产生误差。
	DWORD m_DownedDataSize = m_pVGS2Wnd->WebFileObj->curPos;

	//设置Download文字
	strcpy(m_pVGS2Wnd->m_strPercent, "Now is create scene...");
	m_VGSEngine.SunSetTextString(m_pVGS2Wnd->m_downText, m_pVGS2Wnd->m_strPercent);
	m_VGSEngine.SunRenderScene();

	//构造场景
	m_pVGS2Wnd->ConstructCurScene();

	//执行JavaScript里的"SceneIsDown"函数
	m_pVGS2Wnd->SceneIsDownInvoke();

	//设置Download文字
	strcpy(m_pVGS2Wnd->m_strPercent, "Now is create texture...");
	m_VGSEngine.SunSetTextString(m_pVGS2Wnd->m_downText, m_pVGS2Wnd->m_strPercent);
	m_VGSEngine.SunRenderScene();

	//创建贴图
	for (INT j = 0; j<= m_pVGS2Wnd->m_DownedTexIndex; j++)
	{
	//模型用到的贴图不在这里创建，而在CManagerResource里创建
	//这里创建Overlay用到的贴图
	m_pVGS2Wnd->CreateOverlayTexture(j);
	//这里创建Button用到的贴图
	m_pVGS2Wnd->CreateButtonTexture(j);
	}

	//创建Image
	m_pVGS2Wnd->m_DownedImageIndex = m_pVGS2Wnd->WebFileObj->m_DownedImageCnt -1;
	if (m_pVGS2Wnd->m_DownedImageIndex > 0)
	{
	for (INT j = 0; j<= m_pVGS2Wnd->m_DownedImageIndex; j++)
	{
	if (SUCCEEDED (m_pVGS2Wnd->CreateSceneImage(m_pVGS2Wnd->WebFileObj, j)) )
	m_pVGS2Wnd->WebFileObj->pImageIsCreate[j] = TRUE;
	else
	m_pVGS2Wnd->WebFileObj->pImageIsCreate[j] = FALSE;

	}
	}

	//如果所有的数据都下载完了
	if (m_pVGS2Wnd->WebFileObj->allDataSize != 0 && m_pVGS2Wnd->WebFileObj->allDataSize == m_DownedDataSize)
	{
	m_pVGS2Wnd->m_percent = 100;
	//把相同材质的模型规整到材质的modelIndices中
	//m_VGSEngine.vgsScene->PushSameMtrlModels();
	//把透明材质的模型规整到m_transModel中
	//m_VGSEngine.vgsScene->PopTransModels();

	//删除下载的显示文字
	m_VGSEngine.SunDelText(m_pVGS2Wnd->m_downText);
	m_pVGS2Wnd->m_downText = NULL;

	//执行JavaScript里的"AllIsDown"函数
	m_pVGS2Wnd->AllIsDownInvoke();

	m_pVGS2Wnd->m_bCanConstructScene = TRUE;
	m_VGSEngine.SunSetRenderStatus(TRUE);

	//贴图编号归0
	m_pVGS2Wnd->m_DownedTexIndex = 0;

	m_pVGS2Wnd->m_AllIsOK = TRUE;
	}

	}

	m_VGSEngine.SunSetRenderStatus(TRUE);

	if (m_pVGS2Wnd->pSceneManager != NULL)
	m_pVGS2Wnd->pSceneManager->bActive = TRUE;
	*/

	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: SetCurrentCameraByName()
//参数: BSTR camName
//描述: 设置场景的当前相机，camName为3DSMax中指定的相机名称。
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetCurrentCameraByName(BSTR camName)
{
	_VGS_RETURN

		DWORD ctr = m_VGSEngine.SunGetCurrentController();
	DWORD cam = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(camName));
	m_VGSEngine.SunSetControllerCamera(ctr, cam);
	// 	UINT camCnt = m_VGSEngine.vgsScene->camera.size();
	// 	CCamera** ppCamera = m_VGSEngine.vgsScene->camera.begin();
	// 	for (UINT i = 0; i< camCnt; i++)
	// 	{
	// 		CCamera* pCamera = *(ppCamera + i);
	// 		if (strcmp(pCamera->name.c_str(), (char*)_bstr_t(camName)) == 0)
	// 		{
	// 			switch(m_VGSEngine.controlStatus)
	// 			{
	// 			case WALK:
	// 				m_VGSEngine.walker.m_pCamera = pCamera;
	// 				m_VGSEngine.vgsScene->currentCam = pCamera;
	// 				break;
	// 			case EDIT:
	// 				m_VGSEngine.editor.m_pCamera = pCamera;
	// 				m_VGSEngine.vgsScene->currentCam = pCamera;
	// 				break;
	// 			case FLY:
	// 				m_VGSEngine.flyer.m_pCamera = pCamera;
	// 				m_VGSEngine.vgsScene->currentCam = pCamera;
	// 				break;
	// 			case NONE:
	// 				break;
	// 			}
	// 		}
	// 	}

	return S_OK;
}


//--------------------------------------------------------------------------------
//方法: SetServer()
//参数: BSTR strServer
//描述: 设置VGS服务器
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetServer(BSTR strServer)
{
	if (m_pVGS2Wnd == NULL) return E_FAIL;    

	std::string str = _BSTR_TO_STR(strServer);
	int n = strlen(str.c_str());
	const char * psz = str.c_str();
	if ((str.c_str())[str.length() ] != 0 || str.length() >= 512 -1)
		return S_OK;

	//转换BSTR 
	CHAR strHtmFile[512];
	ZeroMemory(strHtmFile, 512);  
	strncpy(strHtmFile, str.c_str(), sizeof(strHtmFile) - 1);

	ZeroMemory(m_webServer, sizeof(m_webServer));

	if (g_BrowserType == WBT_OPERA)  // opera浏览器
	{
		std::string sURL = string(strHtmFile);
		if (sURL.find("file://localhost/") == 0)  // 本地文件的开始字符串是“file://localhost/”
		{
			std::string sNewURL = "file:///";
			sNewURL.append(strHtmFile + strlen("file://localhost/"));
			sURL = sNewURL;
		}
		GetFilePath(m_webServer, (char*)sURL.c_str());
	}
	else
	{
		GetFilePath(m_webServer, strHtmFile);
	}

	// fuxb 20101-02-21 ==> 去掉本地文件路径的前缀"file///"
	std::string sss = std::string(m_webServer);
	if (std::string(m_webServer).substr(0, 8).compare("file:///") == 0)
	{
		std::string sStr = std::string(m_webServer).substr(8);
		ZeroMemory(m_webServer, sizeof(m_webServer));
		strncpy(m_webServer, sStr.c_str(), sizeof(m_webServer) - 1);
	}
	// <==

	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: SetModelClickable()
//参数: BSTR modelName, BOOL clickable
//描述: 设置VGS服务器
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetModelClickable(BSTR modelName, BOOL clickable)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	_VGS_RETURN
		DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char *)_bstr_t(modelName));
	m_VGSEngine.SunSetModelClickable(modelHandle, clickable);
	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: CreateOverlayFromImage()
//参数: BSTR overlayName, BSTR oriImageName, BSTR alphaImageName, INT left, INT top, INT width, INT height
//描述: 从Image对象创建Overlay
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::CreateOverlayFromImage(BSTR overlayName, BSTR oriImageName, BSTR alphaImageName, INT alignMode, INT left, INT top, INT width, INT height)
{
	_VGS_RETURN

		//	if (m_pVGS2Wnd == NULL) return E_FAIL;
		//
		//	//转换BSTR
		//	CHAR strOverlayName[32];
		//	strcpy(strOverlayName, (char*)_bstr_t(overlayName));
		//
		//	CHAR strOriImageName[32];
		//	strcpy(strOriImageName, (char*)_bstr_t(oriImageName));
		//
		//	CHAR strAlphaImageName[32];
		//	strcpy(strAlphaImageName, (char*)_bstr_t(alphaImageName));
		//
		//	BOOL bFullH = FALSE; 
		//	BOOL bFullV = FALSE;
		//	RECT olRect;
		//	olRect.left = left;
		//	olRect.top = top;
		//	if (width < 0)
		//	{
		//		width = m_VGSEngine.vgsScene->rect.right - m_VGSEngine.vgsScene->rect.left;
		//		bFullH = TRUE;
		//	}
		//	if (height < 0)
		//	{
		//		height = m_VGSEngine.vgsScene->rect.bottom - m_VGSEngine.vgsScene->rect.top;
		//		bFullV = height;
		//	}
		//
		//	olRect.right = olRect.left + width;
		//	olRect.bottom = olRect.top + height;
		//
		//	EnterCriticalSection(&m_ctrl_cs);
		//	EnterCriticalSection(&m_render_cs);
		//	DWORD overlayHandle = m_VGSEngine.SunNewOverlayFromImage(strOverlayName, strOriImageName, strAlphaImageName, (ALIGNMODE)alignMode, &olRect);
		//	LeaveCriticalSection(&m_render_cs);
		//	LeaveCriticalSection(&m_ctrl_cs);
		//
		//	COverlay* pOverlay = (COverlay*)overlayHandle;
		//	if (pOverlay)
		//	{
		//		pOverlay->m_bFullH = bFullH;
		//		pOverlay->m_bFullV = bFullV;
		//	}
		return S_OK;
}

//--------------------------------------------------------------------------------
//方法: DeleteOverlay()
//参数: BSTR overlayName
//描述: 删除Overlay
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::DeleteOverlay(BSTR overlayName)
{
	_VGS_RETURN


		//	if (m_pVGS2Wnd == NULL) return E_FAIL;
		//
		//	EnterCriticalSection(&m_ctrl_cs);
		//	EnterCriticalSection(&m_render_cs);
		//
		//	//转换BSTR
		//	CHAR strOverlayName[32];
		//	strcpy(strOverlayName, (char*)_bstr_t(overlayName));
		//
		//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName(strOverlayName);
		//	if (overlayHandle != 0)
		//		m_VGSEngine.SunDelOverlay(overlayHandle);
		//	
		//	LeaveCriticalSection(&m_render_cs);
		//	LeaveCriticalSection(&m_ctrl_cs);

		return S_OK;
}

//--------------------------------------------------------------------------------
//方法: SetOverlayOpacity()
//参数: BSTR overlayName, INT opacity
//描述: 设置Overlay的透明度
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetOverlayOpacity(BSTR overlayName, INT opacity)
{
	_VGS_RETURN
		DWORD handle = m_VGSEngine.SunGetOverlayHandleByName((char*)_bstr_t(overlayName));
	m_VGSEngine.SunSetOverlayOpacity(handle, opacity);
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	//转换BSTR
	//	CHAR strOverlayName[32];
	//	strcpy(strOverlayName, (char*)_bstr_t(overlayName));
	//
	//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName(strOverlayName);
	//	if (overlayHandle != 0)
	//	{
	//		//进入临界区
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//		m_VGSEngine.SunSetOverlayOpacity(overlayHandle, opacity);
	//		//Quit临界区
	//		LeaveCriticalSection(&m_render_cs);
	//		LeaveCriticalSection(&m_ctrl_cs);
	//	}

	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: CreateButtonFromImage()
//参数: BSTR btnName, BSTR mouseUpImage, BSTR mouseUpImage_alpha, 
//      BSTR mouseOverImage, BSTR mouseOverImage_alpha,
//      BSTR mouseDownImage, BSTR mouseDownImage_alpha,
//      INT alignMode, INT x, INT y, INT width, INT height,
//      BSTR callBackFunc
//描述: 从Image对象创建Button
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::CreateButtonFromImage(BSTR btnName, BSTR mouseUpImage, BSTR mouseUpImage_alpha, 
										  BSTR mouseOverImage, BSTR mouseOverImage_alpha,
										  BSTR mouseDownImage, BSTR mouseDownImage_alpha,
										  INT alignMode, INT x, INT y, INT width, INT height,
										  BSTR callBackFunc)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//	RECT btnRect;
	//	btnRect.left = x;
	//	btnRect.top = y;
	//	btnRect.right = btnRect.left + width;
	//	btnRect.bottom = btnRect.top + height;
	//
	//	DWORD btnHandle = m_VGSEngine.SunNewButtonFromImage((char*)_bstr_t(btnName), 
	//												   (char*)_bstr_t(mouseUpImage), (char*)_bstr_t(mouseUpImage_alpha),
	//												   (char*)_bstr_t(mouseOverImage), (char*)_bstr_t(mouseOverImage_alpha),
	//												   (char*)_bstr_t(mouseDownImage), (char*)_bstr_t(mouseDownImage_alpha),
	//												   (ALIGNMODE)alignMode,
	//												   &btnRect, 
	//												   (char*)_bstr_t(callBackFunc));
	//	//if (btnHandle == 0)
	//	//	return E_FAIL;
	//	
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: DeleteButton()
//参数: BSTR btnName
//描述: 删除Button
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::DeleteButton(BSTR btnName)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	EnterCriticalSection(&m_render_cs);
	//
	//	//转换BSTR
	//	CHAR strbtnName[32];
	//	strcpy(strbtnName, (char*)_bstr_t(btnName));
	//
	//	DWORD btnHandle = m_VGSEngine.SunGetButtonByName(strbtnName);
	//	if (btnHandle != 0)
	//	{
	//		m_VGSEngine.SunDeleteButton(btnHandle);
	//		m_pVGS2Wnd->m_pButton = NULL;
	//	}
	//
	//	LeaveCriticalSection(&m_render_cs);

	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: TranslateModel()
//参数: BSTR modelName, FLOAT x, FLOAT y, FLOAT z, INT coord
//描述: 移动模型
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::TranslateModel(BSTR modelName, FLOAT x, FLOAT y, FLOAT z, INT coord)
{
	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	VECTOR3 vec;
	vec.x = x;
	vec.y = y; 
	vec.z = z;
	m_VGSEngine.SunMoveModel(modelHandle, &vec, coord);

	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: SetEditCtrl_DollySpeed()
//参数: FLOAT dollySpeed
//描述: 推进拉远相机的速度, EDIT模式有效
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetEditCtrl_DollySpeed(FLOAT dollySpeed)
{
	//	m_VGSEngine.editor.dollySpeed = dollySpeed;
	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: SetEditCtrl_RosAxis()
//参数: INT rosHint
//描述: 设置旋转轴, EDIT模式有效, 设置鼠标托拽时的旋转轴
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetEditCtrl_RosAxis(INT rosHint)
{
	//	if (rosHint < 0 || rosHint > 2)
	//	{
	//		m_VGSEngine.editor.rosAxis = 0;
	//		return S_OK;
	//	}
	//
	//	m_VGSEngine.editor.rosAxis = rosHint;
	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: SetEditCtrl_DollyDistance()
//参数: FLOAT nearDis, FLOAT farDis
//描述: 设置鼠标滚轮推拉镜头的最近最远距离, EDIT模式有效
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetEditCtrl_DollyDistance(FLOAT nearDis, FLOAT farDis)
{
	//	m_VGSEngine.editor.dollyNearDis = nearDis;
	//	m_VGSEngine.editor.dollyFarDis = farDis;
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetEditCtrl_AutoParam()
//参数: FLOAT PitchSpeedValue, FLOAT speedValue, FLOAT RollSpeedValue
//描述: 设置当前相机的自动Round速度
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetEditCtrl_AutoParam(FLOAT PitchSpeedValue, FLOAT RoundSpeedValue, FLOAT RollSpeedValue)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	m_VGSEngine.SetAutoPitchRoundRollSpeed(PitchSpeedValue, RoundSpeedValue, RollSpeedValue);
	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: SetEditCtrl_AutoPitchRoundRoll()
//参数: BOOL status
//描述: 设置镜头是否自动旋转和翻滚, EDIT模式有效
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetEditCtrl_AutoPitchRoundRoll(BOOL status)
{
	//	if (status == TRUE)
	//		m_VGSEngine.editor.bIsAutoRoundRoll = true;
	//	else
	//		m_VGSEngine.editor.bIsAutoRoundRoll = false;
	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: SetEditCtrl_ManualParam()
//参数: FLOAT PitchSpeed, FLOAT RoundSpeed
//描述: 设置镜头手动旋转和翻滚的速度, EDIT模式有效
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetEditCtrl_ManualParam(FLOAT PitchSpeed, FLOAT RoundSpeed)
{
	//	m_VGSEngine.editor.manualPitchSpeed = PitchSpeed;
	//	m_VGSEngine.editor.manualRoundSpeed = RoundSpeed;
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: MoveCurrentCameraTo()
//参数: FLOAT from_x, FLOAT from_y, FLOAT from_z, FLOAT at_x, FLOAT at_y, FLOAT at_z, INT frameCnt
//描述: 移动当前摄像机
// x, y, z为世界坐标， frameCnt为帧数
// acc type为加速模式
// 0 - 匀速运动， 1- 匀加速， 2-匀减速
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::MoveCurrentCameraTo(FLOAT from_x, FLOAT from_y, FLOAT from_z, FLOAT at_x, FLOAT at_y, FLOAT at_z, INT frameCnt, INT accType)
{
	//	if (m_VGSEngine.vgsScene == NULL || m_VGSEngine.vgsScene->currentCam == NULL)
	//		return E_FAIL;
	//
	DWORD cameraHandle = m_VGSEngine.SunGetCurrentCamera();
	D3DXVECTOR3 vFrom = D3DXVECTOR3(from_x, from_y, from_z);
	D3DXVECTOR3 vAt = D3DXVECTOR3(at_x, at_y, at_z);
	m_VGSEngine.MoveCurrentCameraTo(&vFrom, &vAt, frameCnt, accType);

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetFBL()
//参数: FLOAT fblValue
//描述: 设置FBL数值，FBL是指当模型的包围球半径除以模型中心到相机的位置得到的参考数值
//如果模型的FBL小于预设的FBL，则不渲染该模型，这有利于优化场景，不渲染远处的小体量模型
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetFBL(FLOAT fblValue)
{
	//	if (m_VGSEngine.vgsScene == NULL || m_VGSEngine.vgsScene->currentCam == NULL)
	//	return E_FAIL;
	//
	//	m_VGSEngine.vgsScene->fbl = fblValue;

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetOverlayWidth()
//参数: BSTR overlayName, INT width
//描述: 设置指定Overlay的宽度
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetOverlayWidth(BSTR overlayName, INT width)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	//转换BSTR
	//	CHAR strOverlayName[32];
	//	strcpy(strOverlayName, (char*)_bstr_t(overlayName));
	//
	//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName(strOverlayName);
	//	if (overlayHandle != 0)
	//	{
	//		RECT rect;
	//		m_VGSEngine.SunGetOverlayRect(overlayHandle, &rect);
	//		INT center = (rect.left + rect.right)/2;
	//		rect.left = center -width/2;
	//		rect.right = center + width/2;
	//		
	//		//进入临界区
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//		m_VGSEngine.SunSetOverlayRect(overlayHandle, &rect);
	//		//进入临界区
	//		LeaveCriticalSection(&m_render_cs);
	//		LeaveCriticalSection(&m_ctrl_cs);
	//		//m_VGSEngine.SunRenderScene();
	//	}
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetOverlayHeight()
//参数: BSTR overlayName, INT height
//描述: 设置指定Overlay的高度
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetOverlayHeight(BSTR overlayName, INT height)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	//转换BSTR
	//	CHAR strOverlayName[32];
	//	strcpy(strOverlayName, (char*)_bstr_t(overlayName));
	//
	//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName(strOverlayName);
	//	if (overlayHandle != 0)
	//	{
	//		RECT rect;
	//		m_VGSEngine.SunGetOverlayRect(overlayHandle, &rect);
	//		INT center = (rect.top + rect.bottom)/2;
	//		rect.top = center - height/2;
	//		rect.bottom = center + height/2;
	//		//进入临界区
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//		m_VGSEngine.SunSetOverlayRect(overlayHandle, &rect);
	//		//进入临界区
	//		LeaveCriticalSection(&m_render_cs);
	//		LeaveCriticalSection(&m_ctrl_cs);
	//		//m_VGSEngine.SunRenderScene();
	//		
	//	}
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetOverlayAlignMode()
//参数: BSTR overlayName, INT alignMode
//描述: 设置指定Overlay的对齐模式
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetOverlayAlignMode(BSTR overlayName, INT alignMode)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	//转换BSTR
	//	CHAR strOverlayName[32];
	//	strcpy(strOverlayName, (char*)_bstr_t(overlayName));
	//
	//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName(strOverlayName);
	//	if (overlayHandle != 0)
	//		m_VGSEngine.SetOverlayAlignMode(overlayHandle, alignMode);
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetOverlayRect()
//参数: BSTR overlayName, INT left, INT top, INT width, INT height
//描述: 设置指定Overlay的矩形
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetOverlayRect(BSTR overlayName, INT left, INT top, INT width, INT height)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	//转换BSTR
	//	CHAR strOverlayName[32];
	//	strcpy(strOverlayName, (char*)_bstr_t(overlayName));
	//
	//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName(strOverlayName);
	//	if (overlayHandle != 0)
	//	{
	//		RECT rect;
	//		rect.left = left;
	//		rect.top = top;
	//		rect.right = rect.left + width;
	//		rect.bottom = rect.top + height;
	//		
	//		//进入临界区
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//		m_VGSEngine.SunSetOverlayRect(overlayHandle, &rect);
	//		//进入临界区
	//		LeaveCriticalSection(&m_render_cs);
	//		LeaveCriticalSection(&m_ctrl_cs);
	//		//m_VGSEngine.SunRenderScene();
	//	}
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: MoveOverlay()
//参数: BSTR overlayName, INT x, INT y
//描述: 移动指定Overlay的位置
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::MoveOverlay(BSTR overlayName, INT x, INT y)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	//转换BSTR
	//	CHAR strOverlayName[32];
	//	strcpy(strOverlayName, (char*)_bstr_t(overlayName));
	//
	//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName(strOverlayName);
	//	if (overlayHandle != 0)
	//	{
	//		RECT rect;
	//		m_VGSEngine.SunGetOverlayRect(overlayHandle, &rect);
	//		rect.left += x;
	//		rect.right += x;
	//		rect.top += y;
	//		rect.bottom += y;
	//		m_VGSEngine.SunSetOverlayRect(overlayHandle, &rect);
	//		m_VGSEngine.SunRenderScene();
	//	}
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: MoveCurrentCameraFromTo()
//参数: FLOAT x, FLOAT y, FLOAT z
//描述: 移动相机的From到D3DXVECTOR(x, y, z)
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::MoveCurrentCameraFromTo(FLOAT x, FLOAT y, FLOAT z)
{
	// TODO: Add your implementation code here
	//	m_VGSEngine.vgsScene->currentCam->m_from = D3DXVECTOR3(x, y, z);
	//	m_VGSEngine.vgsScene->currentCam->m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_VGS_RETURN
		DWORD cam = m_VGSEngine.SunGetCurrentCamera();
	m_VGSEngine.SunSetCameraFrom(cam, &VECTOR3(x, y, z));
	m_VGSEngine.SunSetCameraRoll(cam, 0);
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: MoveCurrentCameraAtTo()
//参数: FLOAT x, FLOAT y, FLOAT z
//描述: 移动相机的At到D3DXVECTOR(x, y, z)
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::MoveCurrentCameraAtTo(FLOAT x, FLOAT y, FLOAT z)
{
	// TODO: Add your implementation code here
	//	m_VGSEngine.vgsScene->currentCam->m_at = D3DXVECTOR3(x, y, z);
	//	m_VGSEngine.vgsScene->currentCam->m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	DWORD cam = m_VGSEngine.SunGetCurrentCamera();
	m_VGSEngine.SunSetCameraAt(cam, &VECTOR3(x, y, z));
	m_VGSEngine.SunSetCameraRoll(cam, 0);
	return S_OK;
}

//------------------------------------------------------------------------------
//方法: MoveCurrentCameraFrom()
//参数: FLOAT x, FLOAT y, FLOAT z
//描述: 移动相机的From +=D3DXVECTOR(x, y, z)
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::MoveCurrentCameraFrom(FLOAT x, FLOAT y, FLOAT z)
{

	DWORD cam = m_VGSEngine.SunGetCurrentCamera();
	m_VGSEngine.SunMoveCamera(cam, &VECTOR3(x, y, z), 0);
	m_VGSEngine.SunSetCameraRoll(cam, 0);
	return S_OK;

	// TODO: Add your implementation code here
	//	m_VGSEngine.vgsScene->currentCam->m_from += D3DXVECTOR3(x, y, z);
	//	m_VGSEngine.vgsScene->currentCam->m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//return S_OK;
}

//------------------------------------------------------------------------------
//方法: MoveCurrentCameraAt()
//参数: FLOAT x, FLOAT y, FLOAT z
//描述: 移动相机的At += D3DXVECTOR(x, y, z)
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::MoveCurrentCameraAt(FLOAT x, FLOAT y, FLOAT z)
{
	VECTOR3 vAt;
	DWORD cam = m_VGSEngine.SunGetCurrentCamera();
	m_VGSEngine.SunGetCameraAt(cam, &vAt);
	vAt.x += x;
	vAt.y += y;
	vAt.z += z;
	m_VGSEngine.SunSetCameraAt(cam, &vAt);
	m_VGSEngine.SunSetCameraRoll(cam, 0);
	return S_OK;
	// TODO: Add your implementation code here
	//	m_VGSEngine.vgsScene->currentCam->m_at += D3DXVECTOR3(x, y, z);
	//	m_VGSEngine.vgsScene->currentCam->m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// return S_OK;
}

//------------------------------------------------------------------------------
//方法: LoadN3DFile()
//参数: BSTR strN3DFile, BSTR N3DName
//描述: LoadN3D文件到场景
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::LoadN3DFile(BSTR strN3DFileName, BSTR N3DName)
{
	_VGS_RETURN
	if (!m_hInternetSession) 
		return E_FAIL;

	std::string sFileName = _getFullPath(m_webServer, _BSTR_TO_STR(strN3DFileName));
	std::string sExtName = _getFileExtension(sFileName);
	std::string sName     = _BSTR_TO_STR(N3DName);

	if (sExtName == ".n3d")
	{
		m_pVGS2Wnd->DownloadAndLoadN3DFile(sFileName, sName);
	}
	else if (sExtName == ".a3d")
	{
		m_pVGS2Wnd->DownloadAndLoadA3DFile(sFileName, sName);
	}

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: ResetScene()
//参数: 
//描述: 释放场景
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::ResetScene()
{
	m_VGSEngine.SunResetScene();
	//释放N3D文件下载队列
	//	INT N3DCnt = m_pVGS2Wnd->m_LoadN3DObj.size();
	//	CDownLoadN3DFile** ppN3D = m_pVGS2Wnd->m_LoadN3DObj.begin();
	//	for (INT i = 0; i< N3DCnt; i++)
	//	{
	//		CDownLoadN3DFile* pN3D = *(ppN3D + i);
	//		if (pN3D->m_bTerminated == FALSE)
	//		{
	//			pN3D->m_bTerminated = TRUE;
	//			WaitForSingleObject(pN3D->m_threadHandle, INFINITE);
	//		}
	//		pN3D->N3DFile.Release();
	//
	//		SAFE_RELEASE(pN3D);
	//		SAFE_DELETE(pN3D);
	//	}
	//	m_pVGS2Wnd->m_LoadN3DObj.clear();
	//
	//	//释放FileDownload文件下载队列
	//	INT downFileCnt = m_pVGS2Wnd->m_LoadFiles.size();
	//	CDownLoadFile** ppDownFile = m_pVGS2Wnd->m_LoadFiles.begin();
	//	for (i = 0; i< downFileCnt; i++)
	//	{
	//		CDownLoadFile* pDownFile = *(ppDownFile + i);
	//		if (pDownFile->m_bTerminated == FALSE)
	//		{
	//			pDownFile->m_bTerminated = TRUE;
	//			WaitForSingleObject(pDownFile->m_threadHandle, INFINITE);
	//		}
	//		
	//		SAFE_RELEASE(pDownFile);
	//		SAFE_DELETE(pDownFile);
	//	}
	//	m_pVGS2Wnd->m_LoadFiles.clear();
	//	//进入保留区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//	//用VGS2.ResetScene()
	//	//m_VGSEngine.DeleteLogo();
	//
	//	m_VGSEngine.SunResetScene();
	//
	//	m_pVGS2Wnd->InitVars();
	//	m_pVGS2Wnd->InitOther();
	//
	//	//离开保留区
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetModelMtrl()
//参数: BSTR modelName, INT subMeshID, SBSTR mtrlName
//描述: 设置模型的材质，如果subMeshID设置成-1则将所有自模型设置成同一材质
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetModelMtrl(BSTR modelName, INT subMeshID, BSTR mtrlName)
{
	//渲染队列
	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));

	//设置渲染队列中的模型的材质
	if(modelHandle && mtrlHandle)
	{
		m_VGSEngine.SunSetModelMtrl(modelHandle, subMeshID, mtrlHandle);

		//鼠标松开后的材质
		//CModel* pModel = (CModel*)modelHandle;
		//pModel->mouseUpShader = (CMaterial*)mtrlHandle;	
		//----------------------------------------------------------------------------------------
	}

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetMtrlOpacity()
//参数: BSTR mtrlName, INT opacity
//描述: 设置材质透明度
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetMtrlOpacity(BSTR mtrlName, INT opacity)
{
	if (opacity > 100) opacity = 100;
	if (opacity < 0) opacity = 0;

	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	//材质不为空
	if(mtrlHandle)
		m_VGSEngine.SetMtrlOpacity(mtrlHandle, opacity);	

	return S_OK;
}

//------------------------------------------------------------------------------
//方法: SetCurrentCameraByIndex()
//参数: INT camIndex
//描述: 根据相机编号设置当前相机
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetCurrentCameraByIndex(INT camIndex)
{
	//	DWORD curCam = m_VGSEngine.SunGetCameraHandleByIndex(camIndex);
	//	if (curCam <= 0) 
	//		return S_OK;
	//
	//	switch(m_VGSEngine.controlStatus)
	//	{
	//	case WALK:
	//		m_VGSEngine.walker.m_pCamera = (CCamera*)curCam;
	//		m_VGSEngine.vgsScene->currentCam = m_VGSEngine.walker.m_pCamera;
	//		break;
	//	case EDIT:
	//		m_VGSEngine.editor.m_pCamera = (CCamera*)curCam;
	//		m_VGSEngine.vgsScene->currentCam = m_VGSEngine.editor.m_pCamera;
	//		break;
	//	case FLY:
	//		m_VGSEngine.flyer.m_pCamera = (CCamera*)curCam;
	//		m_VGSEngine.vgsScene->currentCam = m_VGSEngine.flyer.m_pCamera;
	//		break;
	//	case NONE:
	//		break;
	//	}

	return S_OK;
}

STDMETHODIMP CVGS2::SetWalkCtrl_ManualParam(FLOAT moveSpeed, FLOAT moveAcc, FLOAT rotateSpeed, FLOAT rotateAcc, FLOAT height)
{
	//	m_VGSEngine.walker.walkSpeed = moveSpeed;
	//	m_VGSEngine.walker.walkAcc = moveAcc;
	//	m_VGSEngine.walker.rosSpeed = rotateSpeed;
	//	m_VGSEngine.walker.rosAcc = rotateAcc;
	//	m_VGSEngine.walker.height = height;

	return S_OK;
}


STDMETHODIMP CVGS2::EnableMouseDownCallback()
{
	m_pVGS2Wnd->bEnable_MouseDown_Callback = TRUE;

	return S_OK;
}

STDMETHODIMP CVGS2::DisableMouseDownCallback()
{
	m_pVGS2Wnd->bEnable_MouseDown_Callback = FALSE;

	return S_OK;
}

STDMETHODIMP CVGS2::EnableMouseUpCallback()
{
	m_pVGS2Wnd->bEnable_MouseUp_Callback = TRUE;

	return S_OK;
}

STDMETHODIMP CVGS2::DisableMouseUpCallback()
{
	m_pVGS2Wnd->bEnable_MouseUp_Callback = FALSE;
	return S_OK;
}

STDMETHODIMP CVGS2::EnableMouseDBClickCallback()
{
	m_pVGS2Wnd->bEnable_MouseDBClick_Callback = TRUE;
	return S_OK;
}

STDMETHODIMP CVGS2::DisableMouseDBClickCallback()
{
	m_pVGS2Wnd->bEnable_MouseDBClick_Callback = FALSE;
	return S_OK;
}

STDMETHODIMP CVGS2::EnableMouseMoveCallback()
{
	m_pVGS2Wnd->bEnable_MouseMove_Callback = TRUE;
	return S_OK;
}

//关闭鼠标移动回调函数
STDMETHODIMP CVGS2::DisableMouseMoveCallback()
{
	m_pVGS2Wnd->bEnable_MouseMove_Callback = FALSE;
	return S_OK;
}

//设置当鼠标在模型上移进或移出时变化的材质
STDMETHODIMP CVGS2::SetModelMouseMtrl(BSTR modelName, BSTR mouseOverMtrl, BSTR mouseDownMtrl)
{
	//找到资源队列中对应的模型指针
	//	CModel* pModel = NULL;
	//	CMaterial* pMouseOverMtrl = NULL;
	//	CMaterial* pMouseClickedMtrl = NULL;
	//   
	//----------------------------------------------------------------------------------------------

	return S_OK;
}

//设置模型的透明度
STDMETHODIMP CVGS2::SetModelOpacity(BSTR modelName, INT opacity)
{
	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	if(modelHandle) 
	{
		m_VGSEngine.SunSetModelOpacity(modelHandle, opacity);
	}
	return S_OK;
}

//创建BumpWater对象
STDMETHODIMP CVGS2::CreateBumpWaterTexture(BSTR bumpWaterName, INT width, INT height, 
										   INT xSize, INT ySize, FLOAT radius, FLOAT speed, 
										   FLOAT centerX, FLOAT centerY)
{
	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	DWORD bumpWaterHandle = m_VGSEngine.CreateBumpTexture((char*)_bstr_t(bumpWaterName), BUMPWAVEMAP, width, height, 
	//																		xSize, ySize, radius, speed, centerX, centerY);
	//	//离开临界区
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

STDMETHODIMP CVGS2::RotateCurrentCamera(FLOAT x, FLOAT y, FLOAT z, INT stepCnt, INT coordEnum)
{
	_VGS_RETURN
		DWORD cameraHandle = m_VGSEngine.SunGetCurrentCamera();
	VECTOR3 vAxis(x, y, z);
	for (INT i = 0; i< stepCnt; i++)
	{
		m_VGSEngine.SunRotateCamera(cameraHandle, &vAxis, coordEnum);
	}
	//	VECTOR3 vec;
	//	vec.x = x;
	//	vec.y = y;
	//	vec.z = z;
	//	SetRenderStatus(0);
	//	for (INT i = 0; i< stepCnt; i++)
	//	{
	//		m_VGSEngine.SunRotateCamera(cameraHandle, &vec, coordEnum);
	//		m_VGSEngine.SunRenderScene();
	//	}
	//	SetRenderStatus(1);

	return S_OK;
}

STDMETHODIMP CVGS2::RotateCamera(BSTR cameraName, FLOAT x, FLOAT y, FLOAT z, INT stepCnt, INT coordEnum)
{
	_VGS_RETURN
		DWORD cameraHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(cameraName));
	VECTOR3 vec;
	vec.x = x;
	vec.y = y;
	vec.z = z;
	// SetRenderStatus(0);
	for (INT i = 0; i< stepCnt; i++)
	{
		m_VGSEngine.SunRotateCamera(cameraHandle, &vec, coordEnum);
		// m_VGSEngine.SunRenderScene();
	}
	// SetRenderStatus(1);

	return S_OK;
}

//-------------------------------------------------------------
//将相机对准模型，可以对准一个模型(model2Name==NULL)
//model1为第一个模型
//model2为第二个模型
//x, y, z为对准的方向
//distance为From到At的距离
//stepCnt为花费的帧数
//-------------------------------------------------------------
STDMETHODIMP CVGS2::FocusCurrentCameraTo(BSTR model1Name, BSTR model2Name, 
										 FLOAT x, FLOAT y, FLOAT z, 
										 FLOAT distance, INT stepCnt)
{
	//	DWORD model1Handle = 
	//		m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(model1Name));
	//	DWORD model2Handle = 
	//		m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(model2Name));
	//	
	//	CModel* pModel1 = (CModel*)model1Handle;
	//	CModel* pModel2 = (CModel*)model2Handle;
	//
	//	if (pModel1)
	//	{
	//		D3DXVECTOR3 at = pModel1->boundingSphere.Center;
	//		D3DXVECTOR3 dir = D3DXVECTOR3(x, y, z);
	//		D3DXVec3Normalize(&dir, &dir);
	//		D3DXVECTOR3 from = at - dir * distance;
	//		if (pModel2)
	//		{
	//			at = (at + pModel2->boundingSphere.Center)/2;
	//			from = at - D3DXVECTOR3(x, y, z) * distance;
	//		}
	//		MoveCurrentCameraTo(from.x, from.y, from.z, at.x, at.y, at.z, stepCnt, 2);
	//	}
	return S_OK;
}

//隐藏所有物体
STDMETHODIMP CVGS2::HideAllModels()
{
	//	INT modelCnt = m_VGSEngine.vgsScene->model.size();
	//	CModel** ppModel = m_VGSEngine.vgsScene->model.begin();
	//	for (INT i = 0; i< modelCnt; i++)
	//	{
	//		CModel* pModel = *(ppModel + i);
	//		pModel->visible = false;
	//	}

	return S_OK;
}

//显示所有物体
STDMETHODIMP CVGS2::ShowAllModels()
{
	//	INT modelCnt = m_VGSEngine.vgsScene->model.size();
	//	CModel** ppModel = m_VGSEngine.vgsScene->model.begin();
	//	for (INT i = 0; i< modelCnt; i++)
	//	{
	//		CModel* pModel = *(ppModel + i);
	//		pModel->visible = true;
	//	}

	return S_OK;
}

STDMETHODIMP CVGS2::GetModelHandleByName(BSTR modelName, VARIANT* modelHandle)
{
	DWORD l_modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));

	modelHandle->vt = VT_UINT;
	modelHandle->lVal = l_modelHandle;
	return S_OK;
}

//复位指定模型的材质
STDMETHODIMP CVGS2::RestoreModelMtrl(BSTR modelName)
{
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	//复位模型的材质
	//	if(modelHandle)
	//	{
	//		CModel* pModel = (CModel*)modelHandle;
	//		//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	//		//m_VGSEngine.SunSetModelMtrl(modelHandle, (DWORD)(pModel->oriShader));
	//		//---------------------------------------------------------------------------------------
	//	}
	return S_OK;
}

//复位所有模型的材质
STDMETHODIMP CVGS2::RestoreAllModelsMtrl()
{
	//	INT modelCnt = m_VGSEngine.vgsScene->model.size();
	//	CModel** ppModel = m_VGSEngine.vgsScene->model.begin();
	//	for (INT i = 0; i< modelCnt; i++)
	//	{
	//		CModel* pModel = *(ppModel + i);
	//	    //Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	//		//pModel->oriShader->opacity = pModel->oriShader->oriOpacity;
	//		//m_VGSEngine.SunSetModelMtrl((DWORD)pModel, (DWORD)(pModel->oriShader));
	//		//----------------------------------------------------------------------------------------
	//	}
	return S_OK;
}

//创建材质
STDMETHODIMP CVGS2::CreateMtrl(BSTR mtrlName, INT red, INT green, INT blue, INT alpha, INT selfIllu)
{
	_VGS_RETURN
		DWORD matHandle = m_VGSEngine.SunCreateEmptyMtrl((char*)_bstr_t(mtrlName));
	MtrlInfo info;
	m_VGSEngine.SunGetMtrlInfo(matHandle, &info);
	info.Diffuse = VGSRGB(red, green, blue);
	info.Opacity = INT(alpha / 255.0 * 100);
	info.Emissive = selfIllu;
	m_VGSEngine.SunSetMtrlInfo(matHandle, &info);
	//	CMaterial* pMtrl =  new CMaterial(m_VGSEngine.vgsScene->m_pd3dDevice);
	//	pMtrl->name = std::string((char*)_bstr_t(mtrlName));
	//	
	//	//diffuse
	//	pMtrl->d3dmtrl.Diffuse.r = red/255.0f;
	//	pMtrl->d3dmtrl.Diffuse.g = green/255.0f;
	//	pMtrl->d3dmtrl.Diffuse.b = blue/255.0f;
	//	pMtrl->d3dmtrl.Diffuse.a = alpha/255.0f;
	//
	//	//ambient
	//	pMtrl->d3dmtrl.Ambient = pMtrl->d3dmtrl.Diffuse;
	//
	//	//specular
	//	pMtrl->d3dmtrl.Specular = pMtrl->d3dmtrl.Diffuse;
	//
	//	//自发光数值
	//	pMtrl->selfIllumination = selfIllu;
	//	//mtrl->emi
	//	pMtrl->d3dmtrl.Emissive.a = 1.0f;
	//	pMtrl->d3dmtrl.Emissive.r = selfIllu/100.0f;
	//	pMtrl->d3dmtrl.Emissive.g = selfIllu/100.0f;
	//	pMtrl->d3dmtrl.Emissive.b = selfIllu/100.0f;
	//
	//	//power
	//	pMtrl->d3dmtrl.Power = 10;
	//	pMtrl->type = D3DSHADE_GOURAUD;
	//
	//	//强度的区域
	//	pMtrl->opacity = (INT)(alpha / 255.0f * 100);
	//	pMtrl->oriOpacity = pMtrl->opacity;
	//	//---------------------材质的FVF---------------------------
	//	pMtrl->m_FVF = FVF_T4;
	//	pMtrl->m_FVFSize = sizeof(T4MESHVERTEX);
	//
	//	m_VGSEngine.vgsScene->shader.push_back(pMtrl);

	return S_OK;
}

//设置材质的颜色
STDMETHODIMP CVGS2::SetMtrlColor(BSTR mtrlName, INT red, INT green, INT blue, INT alpha)
{
	_VGS_RETURN
		DWORD matHandle = m_VGSEngine.SunCreateEmptyMtrl((char*)_bstr_t(mtrlName));
	m_VGSEngine.SunSetMtrlColor(matHandle, red, green, blue, alpha);
	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	/*DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	if (pMtrl)
	{
	pMtrl->d3dmtrl.Diffuse.r = red/255.0f;
	pMtrl->d3dmtrl.Diffuse.g = green/255.0f;
	pMtrl->d3dmtrl.Diffuse.b = blue/255.0f;
	pMtrl->d3dmtrl.Diffuse.a = alpha/255.0f;
	//pMtrl->d3dmtrl.Ambient = pMtrl->d3dmtrl.Diffuse;
	//pMtrl->d3dmtrl.Specular = pMtrl->d3dmtrl.Diffuse;

	//如果是不透明材质
	if (pMtrl->opacity == 100)
	{
	CModel** ppModel = (CModel**)(pMtrl->modelList.begin());
	INT modelCnt = pMtrl->modelList.size();
	for (INT i = 0; i< modelCnt; i++)
	{
	pMtrl->opacity = alpha/255.0f * 100;
	CModel* pModel = *(ppModel + i);
	pModel->SetMtrl(pMtrl);
	//如果模型为不透明模型，将材质设置为透明材质，则将模型加入到透明模型队列
	if (pMtrl->opacity < 100)
	{
	if (pModel->opacity == 100)
	m_VGSEngine.vgsScene->transModel.push_back(pModel);
	//从不透明模型队列清空
	pMtrl->modelList.clear();
	}

	}
	}
	//如果是透明材质
	else
	{
	pMtrl->opacity = alpha/255.0f * 100;
	for (INT j = 0; j < m_VGSEngine.vgsScene->transModel.size(); j++)
	{
	CModel** ppModel = m_VGSEngine.vgsScene->transModel.begin();
	CModel* pModel = *(ppModel + j);
	if (pModel->shader == pMtrl)
	{
	pModel->SetMtrl(pMtrl);
	//如果将材质设置为不透明材质，则将模型从透明模型队列删除
	if (pMtrl->opacity == 100)
	{
	m_VGSEngine.vgsScene->transModel.erase(ppModel + j);
	//pModel->shader->modelList.push_back((DWORD)pModel);
	if (j > 0) j--;
	}
	}
	}
	}
	}
	*/
	//--------------------------------------------------------------------------------------------------
	return S_OK;
}

//删除指定材质
STDMETHODIMP CVGS2::DeleteMtrl(BSTR mtrlName)
{

	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	m_VGSEngine.SunDeleteMtrl(mtrlHandle);
	//	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	//
	//	INT shaderCnt = m_VGSEngine.vgsScene->shader.size();
	//	CMaterial** ppMtrl = m_VGSEngine.vgsScene->shader.begin();
	//	for (INT i = 0; i< shaderCnt; i++)
	//	{
	//		CMaterial* tMtrl = *(ppMtrl + i);
	//		if (pMtrl == tMtrl)
	//		{
	//			SAFE_RELEASE(tMtrl);
	//			SAFE_DELETE(tMtrl);
	//			m_VGSEngine.vgsScene->shader.erase(ppMtrl + i);
	//			break;
	//		}
	//	}

	return S_OK;
}

//设置材质的自发光数值
STDMETHODIMP CVGS2::SetMtrlIllumination(BSTR mtrlName, INT illumination)
{
	_VGS_RETURN
		DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	MtrlInfo info;
	m_VGSEngine.SunGetMtrlInfo(mtrlHandle, &info);
	info.Emissive = illumination;
	m_VGSEngine.SunSetMtrlInfo(mtrlHandle, &info);

	//	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	//	pMtrl->d3dmtrl.Emissive.a = 1.0f;
	//	pMtrl->d3dmtrl.Emissive.r = illumination/255.0f;
	//	pMtrl->d3dmtrl.Emissive.g = illumination/255.0f;
	//	pMtrl->d3dmtrl.Emissive.b = illumination/255.0f;
	//
	//	pMtrl->selfIllumination = illumination;
	return S_OK;
}

//移动指定的模型
//coordEnum 0-world coord, 1-self coord
STDMETHODIMP CVGS2::MoveModel(BSTR modelName, FLOAT x, FLOAT y, FLOAT z, INT coordEnum)
{
	_VGS_RETURN
		DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	m_VGSEngine.SunMoveModel(modelHandle, &VECTOR3(x, y, z), coordEnum);
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel)
	//	{
	//		D3DXVECTOR3 moveVec = D3DXVECTOR3(x, y, z);
	//		pModel->pNode->translate(&moveVec, coordEnum);
	//	}
	return S_OK;
}

//移动指定的模型到某个位置
//acctype 0-匀速运动，1-匀加速运动, 2-匀减速运动
STDMETHODIMP CVGS2::MoveModelTo(BSTR modelName, FLOAT x, FLOAT y, FLOAT z, INT frameCnt, INT accType)
{
	_VGS_RETURN
		DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	m_VGSEngine.MoveModelTo(modelHandle, &VECTOR3(x, y, z), frameCnt, accType);

	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	if (frameCnt <= 0 || !modelHandle)
	//	{
	//		return S_OK;
	//	}
	//
	//	EnterCriticalSection(&m_render_cs);
	//
	//	//由于MoveModelTo需要frameCnt个frame运行的过程，每运行一个frame，就要求MoveModel一段距离
	//	//如果在这里用循环语句，则会导致主线程锁死。主线程无法再响应消息。
	//	//所以必须要把MoveModelTo转化为主线程每个帧循环可以调用的方式
	//	//***方法：通过将MoveModelTo转化为用户自定义模型关键帧动画数据的方法来实现。
	//	CModel* pModel = (CModel*)modelHandle;
	//	pModel->user_moveFrameCnt = frameCnt;
	//	pModel->user_moveCurFrame = 0;
	//	pModel->user_moveKFR = new D3DXVECTOR3[frameCnt];
	//
	//	// 当前模型的位置,如果是静态模型则取worldPosition,
	//	// 如果是动态模型，则取起始帧的位置
	//	D3DXVECTOR3 curPos;
	//	// if (pModel->kfrCnt == 0)
	//	// {
	//		//curPos = D3DXVECTOR3(pModel->pNode._41,
	//							//pModel->pNode->curTransform.ori_matrix._42,
	//							//pModel->pNode->curTransform.ori_matrix._43);
	//		curPos  = pModel->pNode->worldPosition;
	//	/*}
	//	else
	//	{
	//		D3DXMATRIX tempMat;
	//		D3DXMatrixMultiply(&tempMat, &(pModel->pNode->kfrtransform[0]), &(pModel->pNode->curTransform.ori_matrix));
	//		curPos = D3DXVECTOR3(tempMat._41, tempMat._42, tempMat._43);
	//	}*/
	//							
	//	//目标点位置
	//	D3DXVECTOR3 objPos = D3DXVECTOR3(x, y, z);
	//	//与目标点的距离
	//	D3DXVECTOR3 vDis = objPos - curPos;
	//
	//	//运动的加速度和速度变量
	//	D3DXVECTOR3 vAcc = D3DXVECTOR3(0, 0, 0);
	//	D3DXVECTOR3 vVel = D3DXVECTOR3(0, 0, 0);
	//	//计算速度&加速度
	//	INT i;
	//	switch (accType)
	//	{
	//	case 0: //匀速运动
	//		vAcc = D3DXVECTOR3(0, 0, 0);	
	//		vVel = vDis/frameCnt;
	//		//循环动作
	//		for (i = 0; i< frameCnt; i++)
	//			pModel->user_moveKFR[i] = vVel;
	//		break;
	//	case 1: //匀加速运动
	//		vAcc = 2*vDis/(frameCnt * (frameCnt-1)); //数学公式很奇妙
	//		//循环动作
	//		for (i = 0; i< frameCnt; i++)
	//		{
	//			vVel = vAcc * (i+1);
	//			pModel->user_moveKFR[i] = vVel;
	//		}	
	//		break;
	//
	//	case 2: //匀减速运动
	//		vAcc = 2*vDis/(frameCnt * (frameCnt +1));  //数学公式很奇妙
	//		//循环动作
	//		for (i = 0; i< frameCnt; i++)
	//		{
	//			vVel = vAcc * (frameCnt - i);
	//			pModel->user_moveKFR[i] = vVel;
	//		}	
	//		break;
	//	}	
	//
	//	LeaveCriticalSection(&m_render_cs);

	//---------------------------------------------------------------------------------------
	return S_OK;
}

//复位指定模型的透明度
STDMETHODIMP CVGS2::RestoreModelOpacity(BSTR modelName)
{
	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel)
	//	{
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//		pModel->SetOpacity(pModel->oriOpacity);
	//		LeaveCriticalSection(&m_render_cs);
	//		LeaveCriticalSection(&m_ctrl_cs);
	//	}

	//--------------------------------------------------------------------------------------------
	return S_OK;
}

//复位所有模型的透明度 
STDMETHODIMP CVGS2::RestoreAllModelsOpacity()
{
	//	INT modelCnt = m_VGSEngine.vgsScene->model.size();
	//	CModel** ppModel = m_VGSEngine.vgsScene->model.begin();
	//
	//	for (INT i = 0; i< modelCnt; i++)
	//	{
	//		CModel* pModel = *(ppModel + i);
	//		_bstr_t bst(pModel->name.c_str());
	//		BSTR bstr = bst.copy();
	//		RestoreModelOpacity(bstr);
	//	}

	return S_OK;
}

//外部Player控制---------------------------------------------------------------
//向前走
STDMETHODIMP CVGS2::PlayerGoForward()
{
	// TODO: Add your implementation code here
	//m_VGSEngine.vgsScene->softKey[0] = TRUE;
	return S_OK;
}

//向后退
STDMETHODIMP CVGS2::PlayerGoBackward()
{
	//	PlayerStop();
	//	m_VGSEngine.vgsScene->softKey[1] = TRUE;

	return S_OK;
}

//向左走
STDMETHODIMP CVGS2::PlayerGoLeftward()
{
	//	PlayerStop();
	//	m_VGSEngine.vgsScene->softKey[2] = TRUE;

	return S_OK;
}

//向右走
STDMETHODIMP CVGS2::PlayerGoRightward()
{
	//	PlayerStop();
	//	m_VGSEngine.vgsScene->softKey[3] = TRUE;

	return S_OK;
}

//停止
STDMETHODIMP CVGS2::PlayerStop()
{
	//	ZeroMemory(m_VGSEngine.vgsScene->softKey, 
	//		sizeof(m_VGSEngine.vgsScene->softKey));
	return S_OK;
}

//左旋转
STDMETHODIMP CVGS2::PlayerTurnLeft()
{
	//	PlayerStop();
	//	m_VGSEngine.vgsScene->softKey[4] = TRUE;
	return S_OK;
}

//右旋转
STDMETHODIMP CVGS2::PlayerTurnRight()
{
	//	PlayerStop();
	//	m_VGSEngine.vgsScene->softKey[5] = TRUE;
	return S_OK;
}

//上旋转
STDMETHODIMP CVGS2::PlayerTurnUp()
{
	//	PlayerStop();
	//	m_VGSEngine.vgsScene->softKey[6] = TRUE;
	return S_OK;
}

//下旋转
STDMETHODIMP CVGS2::PlayerTurnDown()
{
	//	PlayerStop();
	//	m_VGSEngine.vgsScene->softKey[7] = TRUE;
	return S_OK;
}

//上移动
STDMETHODIMP CVGS2::PlayerGoUp()
{
	//	PlayerStop();
	//	m_VGSEngine.vgsScene->softKey[8] = TRUE;
	return S_OK;
}

//下移动
STDMETHODIMP CVGS2::PlayerGoDown()
{
	//	PlayerStop();
	//	m_VGSEngine.vgsScene->softKey[9] = TRUE;
	return S_OK;
}

//镜头推近
STDMETHODIMP CVGS2::PlayerZoomIn()
{
	//	PlayerStop();
	//	m_VGSEngine.vgsScene->softKey[10] = TRUE;
	return S_OK;
}

//镜头拉远
STDMETHODIMP CVGS2::PlayerZoomOut()
{
	//	PlayerStop();
	//	m_VGSEngine.vgsScene->softKey[11] = TRUE;
	return S_OK;
}

//设置相机推拉的速度
STDMETHODIMP CVGS2::SetPlayerZoomSpeed(FLOAT speed)
{
	//	PlayerStop();
	//	m_VGSEngine.fZoomSpeed = speed;
	return S_OK;
}

//设置模型的颜色
STDMETHODIMP CVGS2::SetModelColor(BSTR modelName, INT red, INT green, INT blue, INT alpha)
{
	_VGS_RETURN
		DWORD modelHanle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	m_VGSEngine.SunSetModelColor(modelHanle, red, green, blue, alpha);
	//    if (alpha > 255) alpha =  255;
	//	if (alpha < 0) alpha = 0;
	//
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel)
	//	{	
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//		pModel->SetColor(red, green, blue, alpha);
	//		LeaveCriticalSection(&m_render_cs);
	//		LeaveCriticalSection(&m_ctrl_cs);
	//		
	//		pModel->mouseUpColor = D3DCOLOR_RGBA(red, green, blue, alpha);
	//		return S_OK;
	//	}
	return S_OK;
}

//设置FBL是否对模型有效
STDMETHODIMP CVGS2::SetModelFBLStatus(BSTR modelName, BOOL status)
{
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel)
	//		pModel->bUseFBL = status;
	return S_OK;
}

//设置波纹速度
STDMETHODIMP CVGS2::SetBumpWaterSpeed(BSTR waterName, FLOAT speed)
{
	//	DWORD bumpWaterHandle = m_VGSEngine.GetBumpTextureHandleByName((char*)_bstr_t(waterName));
	//	CBumpMap* pBumpWater = (CBumpMap*)bumpWaterHandle;
	//	if (pBumpWater)
	//		pBumpWater->m_speed = speed;

	return S_OK;
}

//设置波纹半径
STDMETHODIMP CVGS2::SetBumpWaterRadius(BSTR waterName, FLOAT radius)
{
	//	DWORD bumpWaterHandle = m_VGSEngine.GetBumpTextureHandleByName((char*)_bstr_t(waterName));
	//	CBumpMap* pBumpWater = (CBumpMap*)bumpWaterHandle;
	//	if (pBumpWater)
	//		pBumpWater->m_radius = radius;

	return S_OK;
}

//设置天空盒模型
STDMETHODIMP CVGS2::SetSkyBoxModel(BSTR modelName, BOOL status)
{
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel)
	//	{
	//		pModel->isSkyBox = status;
	//		//D3DXVECTOR3 vScale = D3DXVECTOR3(10, 10, 10);
	//		//pModel->Scale(&vScale, 1);
	//	}
	return S_OK;
}

//设置某个相机的From	
STDMETHODIMP CVGS2::SetCameraFrom(BSTR cameraName, FLOAT x, FLOAT y, FLOAT z)
{
	_VGS_RETURN

		DWORD camHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(cameraName));	
	m_VGSEngine.SunSetCameraFrom(camHandle, &VECTOR3(x, y, z));
	m_VGSEngine.SunSetCameraRoll(camHandle, 0);
	return S_OK;

	//	CCamera* pCam = (CCamera*)camHandle;
	//	
	//	if (pCam)
	//	{
	//		D3DXVECTOR3 vec = D3DXVECTOR3(x, y, z);
	//		pCam->SetFrom(&vec);
	//		pCam->ori_from = vec;
	//		pCam->m_up = D3DXVECTOR3(0, 1, 0);
	//	}

	//return S_OK;
}

//设置某个相机的At
STDMETHODIMP CVGS2::SetCameraAt(BSTR cameraName, FLOAT x, FLOAT y, FLOAT z)
{
	_VGS_RETURN
		DWORD camHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(cameraName));
	m_VGSEngine.SunSetCameraAt(camHandle, &VECTOR3(x, y, z));
	m_VGSEngine.SunSetCameraRoll(camHandle, 0);
	return S_OK;

	//	CCamera* pCam = (CCamera*)camHandle;
	//	if (pCam)
	//	{
	//		D3DXVECTOR3 vec = D3DXVECTOR3(x, y, z);
	//		pCam->SetAt(&vec);
	//	}

	// return S_OK;
}

//设置相机的fov
STDMETHODIMP CVGS2::SetCameraFov(BSTR cameraName, FLOAT fov)
{
	_VGS_RETURN
		DWORD camHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(cameraName));
	m_VGSEngine.SunSetCameraFov(camHandle, fov);
	return S_OK;
	//	DWORD camHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(cameraName));
	//	CCamera* pCam = (CCamera*)camHandle;
	//	if (pCam)
	//	{
	//		pCam->SetFov(fov);
	//	}
	return S_OK;
}

//设置当前相机的fov
STDMETHODIMP CVGS2::SetCurrentCameraFov(FLOAT fov)
{
	_VGS_RETURN
		DWORD cam = m_VGSEngine.SunGetCurrentCamera();
	m_VGSEngine.SunSetCameraFov(cam, fov);

	//	if (m_VGSEngine.vgsScene == NULL || m_VGSEngine.vgsScene->currentCam == NULL)
	//		return E_FAIL;
	//	
	//	fov = fov * ATOS;
	//	m_VGSEngine.vgsScene->currentCam->SetFov(fov);

	return S_OK;
}

//创建FullScreen Motion Blur对象
STDMETHODIMP CVGS2::CreateFSMotionBlur(INT blurCount)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	//创建动态模糊特效控制对象
	//	if (m_VGSEngine.CreateFSMotionBlur())
	//	{
	//		m_VGSEngine.SetFSMotionBlurPower(blurCount);
	//	}
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

//设置FSMotionBlur的强度
STDMETHODIMP CVGS2::SetFSMotionBlurPower(INT value)
{
	//	if (m_VGSEngine.vgsScene->m_pFSMotionBlur)
	//		m_VGSEngine.SetFSMotionBlurPower(value);
	return S_OK;
}

//开始FSMotionBlur
STDMETHODIMP CVGS2::StartFSMotionBlur()
{
	//	if (m_VGSEngine.vgsScene->m_pFSMotionBlur)
	//		m_VGSEngine.OpenFSMotionBlur();
	return S_OK;
}

//停止FSMotionBlur
STDMETHODIMP CVGS2::StopFSMotionBlur()
{
	//	if (m_VGSEngine.vgsScene->m_pFSMotionBlur)
	//		m_VGSEngine.CloseFSMotionBlur();
	return S_OK;
}

//设置动画相机动态模糊的帧段
//cameraName为相机的名称
//startFrame为模糊的起始帧
//endFrame为模糊的终止帧
//power为模糊的强度
//status为是否动态模糊
STDMETHODIMP CVGS2::SetCameraKFRFSMotionBlur(BSTR cameraName, INT startFrame, INT endFrame, INT power, BOOL status)
{
	//	DWORD camHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(cameraName));
	//	CCamera* pCam = (CCamera*)camHandle;
	//	if (pCam && pCam->kfrCnt > 1)
	//	{
	//		for (INT i= startFrame; i<= endFrame; i++)
	//		{
	//			pCam->kfrFSMotionBlur[i] = status;	
	//			pCam->kfrFSMotionBlurValue[i] = power;
	//		}
	//	}

	return S_OK;
}

//设置关键帧动画回调函数, 当动画播放到某一帧时，调用指定的JS函数
//frameIndex, 帧编号
//funcName, JS回调函数名称
STDMETHODIMP CVGS2::SetCameraKfrCallBackFunc(BSTR cameraName, INT frameIndex, BSTR funcName)
{
	DWORD camHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(cameraName));
	// m_VGSEngine.sunsetcam
	//	CCamera* pCam = (CCamera*)camHandle;
	//	if (frameIndex >= pCam->kfrCnt) 
	//		return S_OK;
	//
	//	if (pCam && pCam->kfrCnt > 1)
	//	{
	//		INT funcStrLen = _bstr_t(funcName).length();
	//		if (pCam->InvokeFuncName[frameIndex].name)
	//			SAFE_DELETE_ARRAY(pCam->InvokeFuncName[frameIndex].name);
	//
	//		pCam->InvokeFuncName[frameIndex].name = new CHAR[funcStrLen+1];
	//		ZeroMemory(pCam->InvokeFuncName[frameIndex].name, sizeof(CHAR)*(funcStrLen+1));
	//		strcpy(pCam->InvokeFuncName[frameIndex].name, (char*)_bstr_t(funcName));
	//	}
	return S_OK;
}

//设置Flash到指定的帧
STDMETHODIMP CVGS2::SetFlashCurrentFrame(BSTR flashName, INT frameIndex)
{
	//	INT flashCnt = m_pVGS2Wnd->m_Flash.size();
	//	CATLFlashCtrl** ppFlash = m_pVGS2Wnd->m_Flash.begin();
	//	for (UINT i =0; i< flashCnt; i++)
	//	{
	//		CATLFlashCtrl* pFlash = *(ppFlash + i);
	//		if (strcmp(pFlash->name, (char *)_bstr_t(flashName)) == 0)
	//		{
	//			pFlash->SetCurFrm(frameIndex);
	//			break;
	//		}
	//	}
	return S_OK;
}

//创建镜头光斑特效
//x, y, z为太阳的位置
//coverFlag 是否判断遮挡
STDMETHODIMP CVGS2::CreateLensFlare(FLOAT x, FLOAT y, FLOAT z, BOOL coverFlag)
{
	//	D3DXVECTOR3 sunPos = D3DXVECTOR3(x, y, z);
	//	//创建LensFlare
	//	VECTOR3 vector;
	//	vector.x = sunPos.x;
	//	vector.y = sunPos.y;
	//	vector.z = sunPos.z;
	//	m_VGSEngine.vgsScene->m_bLensFlare_CalCover = coverFlag;
	//	
	//	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	if (m_VGSEngine.vgsScene->m_pLensFlare)
	//	{
	//		SAFE_RELEASE(m_VGSEngine.vgsScene->m_pLensFlare);
	//		SAFE_DELETE(m_VGSEngine.vgsScene->m_pLensFlare);
	//	}
	//	m_VGSEngine.CreateLensFlare(&vector);
	//	m_VGSEngine.vgsScene->m_bLensFlare = TRUE;
	//	//离开临界区
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//创建雾对象
//fogMode, 0-LINEAR, 1-EXP, 2-EXP2
STDMETHODIMP CVGS2::CreateFog(INT fogMode, INT red, INT green, INT blue, FLOAT density, FLOAT start, FLOAT end)
{
	_VGS_RETURN
		m_VGSEngine.SunCreateFog(fogMode, red, green, blue, density, start, end);

	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//	m_VGSEngine.vgsScene->m_bFog = TRUE;
	//	//颜色
	//	m_VGSEngine.vgsScene->m_FogColor = D3DCOLOR_RGBA(red, green, blue, 0);
	//	m_VGSEngine.vgsScene->m_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, 
	//						m_VGSEngine.vgsScene->m_FogColor);
	//	//模式
	//	switch(fogMode)
	//	{
	//	case 0:
	//		m_VGSEngine.vgsScene->m_FogMode = D3DFOG_LINEAR;
	//		break;
	//	case 1:
	//		m_VGSEngine.vgsScene->m_FogMode = D3DFOG_EXP;
	//		break;
	//	case 2:
	//		m_VGSEngine.vgsScene->m_FogMode = D3DFOG_EXP2;
	//		break;
	//	}
	//	m_VGSEngine.vgsScene->m_pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, 
	//					m_VGSEngine.vgsScene->m_FogMode);
	//	
	//	m_VGSEngine.vgsScene->m_FogStart = start;
	//	m_VGSEngine.vgsScene->m_FogEnd = end;
	//	m_VGSEngine.vgsScene->m_FogDensity = density;
	//	//如果是线性模式
	//	if (m_VGSEngine.vgsScene->m_FogMode == D3DFOG_LINEAR)
	//	{
	//		m_VGSEngine.vgsScene->m_pd3dDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start));
	//		m_VGSEngine.vgsScene->m_pd3dDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&end));
	//	}
	//	else
	//	{
	//		//浓度
	//		m_VGSEngine.vgsScene->m_pd3dDevice->SetRenderState(D3DRS_FOGDENSITY, *((DWORD*)(&density)));
	//	}
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

STDMETHODIMP CVGS2::SetFogMode(INT mode)
{				
	_VGS_RETURN
		m_VGSEngine.SunSetFogMode(mode);
	//模式
	//	switch(mode)
	//	{
	//	case 0:
	//		m_VGSEngine.vgsScene->m_FogMode = D3DFOG_LINEAR;
	//		break;
	//	case 1:
	//		m_VGSEngine.vgsScene->m_FogMode = D3DFOG_EXP;
	//		break;
	//	case 2:
	//		m_VGSEngine.vgsScene->m_FogMode = D3DFOG_EXP2;
	//		break;
	//	}
	//	m_VGSEngine.vgsScene->m_pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, 
	//					m_VGSEngine.vgsScene->m_FogMode);
	return S_OK;
}

STDMETHODIMP CVGS2::SetFogColor(INT red, INT blue, INT green)
{
	_VGS_RETURN
		m_VGSEngine.SunSetFogColor(red, blue, green);
	//颜色
	//	m_VGSEngine.vgsScene->m_FogColor = D3DCOLOR_RGBA(red, green, blue, 0);
	//	m_VGSEngine.vgsScene->m_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, 
	//						m_VGSEngine.vgsScene->m_FogColor);

	return S_OK;
}

STDMETHODIMP CVGS2::SetFogDensity(FLOAT density)
{
	_VGS_RETURN
		m_VGSEngine.SunSetFogDensity(density);
	//	m_VGSEngine.vgsScene->m_FogDensity = density;
	//	//如果是线性模式
	//	if (m_VGSEngine.vgsScene->m_FogMode != D3DFOG_LINEAR)
	//		//浓度
	//		m_VGSEngine.vgsScene->m_pd3dDevice->SetRenderState(D3DRS_FOGDENSITY, *((DWORD*)(&density)));

	return S_OK;
}

STDMETHODIMP CVGS2::SetFogField(FLOAT start, FLOAT end)
{
	_VGS_RETURN
		m_VGSEngine.SunSetFogFiled(start, end);
	//	m_VGSEngine.vgsScene->m_FogStart = start;
	//	m_VGSEngine.vgsScene->m_FogEnd = end;
	//	//如果是线性模式
	//	if (m_VGSEngine.vgsScene->m_FogMode == D3DFOG_LINEAR)
	//	{
	//		m_VGSEngine.vgsScene->m_pd3dDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start));
	//		m_VGSEngine.vgsScene->m_pd3dDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&end));
	//	}

	return S_OK;
}

STDMETHODIMP CVGS2::SetFogRenderStatus(BOOL status)
{
	_VGS_RETURN
		m_VGSEngine.SunSetFogEnableStatus(status);

	return S_OK;
}

//设置全屏滤镜效果
STDMETHODIMP CVGS2::SetFilter(BSTR overlayName, BOOL status, INT opacity)
{
	//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName((char*)_bstr_t(overlayName));
	//	COverlay* pOverlay = (COverlay*)overlayHandle;
	//	if (pOverlay)
	//	{	
	//		pOverlay->bIsFilter = status;
	//		//进入临界区
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//		pOverlay->SetOpacity(opacity);
	//		//离开临界区
	//		LeaveCriticalSection(&m_render_cs);
	//		LeaveCriticalSection(&m_ctrl_cs);
	//	}
	return S_OK;
}


//设置鼠标移动事件的响应时间间隔, timeStep为毫秒间隔
STDMETHODIMP CVGS2::SetMouseMoveEventTimer(INT timeStep)
{
	//	m_pVGS2Wnd->m_UserEventTimeStep = timeStep;
	return S_OK;
}

//恢复模型的顶点颜色
STDMETHODIMP CVGS2::RestoreModelColor(BSTR modelName)
{
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel)
	//	{
	//		INT alpha  = (pModel->oriColor >> 24) & 0xff;
	//		INT red  = (pModel->oriColor >> 16) & 0xff;
	//		INT green  = (pModel->oriColor >> 8) & 0xff;
	//		INT blue  = (pModel->oriColor) & 0xff;
	//
	//		SetModelColor(modelName, red, green, blue, alpha);
	//	}

	return S_OK;
}

//恢复所有模型的顶点色
STDMETHODIMP CVGS2::RestoreAllModelsColor()
{
	//	INT modelCnt = m_VGSEngine.vgsScene->model.size();
	//	CModel** ppModel = m_VGSEngine.vgsScene->model.begin();
	//
	//	for (INT i = 0; i< modelCnt; i++)
	//	{
	//		CModel* pModel = *(ppModel + i);
	//		_bstr_t bst(pModel->name.c_str());
	//		BSTR bstr = bst.copy();
	//		RestoreModelColor(bstr);
	//	}

	return S_OK;
}

//设置透明材质的Alpha测试模式
//mtrlName为材质名
//status == 0时为AlphaBlend测试，status ==1 时为AlphaTest测试，默认为AlphaBlend模式
STDMETHODIMP CVGS2::SetAlphaTestStatus(BSTR mtrlName, BOOL status)
{
	//	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	//	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	//	if (pMtrl)
	//	{
	//		switch (status)
	//		{
	//		case 0:
	//			pMtrl->bAlphaTest = FALSE;
	//			break;
	//		case 1:
	//			pMtrl->bAlphaTest = TRUE;
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	return S_OK;
}

//设置ZOne模型
STDMETHODIMP CVGS2::SetZOneModel(BSTR modelName, BOOL status)
{
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel)
	//	{
	//		pModel->isZOne = status;
	//	}

	return S_OK;
}

//设置LensFlare是否可见
STDMETHODIMP CVGS2::SetLensFlareVisibility(BOOL status)
{
	//	if (m_VGSEngine.vgsScene)
	//		m_VGSEngine.vgsScene->m_bLensFlare = status;

	return S_OK;
}

//设置碰撞发生的距离
STDMETHODIMP CVGS2::SetCollisionDistance(FLOAT distance)
{
	//	m_VGSEngine.SunSetColDistance(distance);

	return S_OK;
}

//创建静态CubeMap贴图对象
STDMETHODIMP CVGS2::CreateStaticCubeMap(BSTR name, BSTR frontImage, BSTR backImage, BSTR leftImage, BSTR rightImage, BSTR topImage, BSTR bottomImage, INT size)
{
	//	DWORD frontImageHandle = m_VGSEngine.SunGetImageHandleByName((char*)_bstr_t(frontImage));
	//	DWORD backImageHandle = m_VGSEngine.SunGetImageHandleByName((char*)_bstr_t(backImage));
	//	DWORD leftImageHandle = m_VGSEngine.SunGetImageHandleByName((char*)_bstr_t(leftImage));
	//	DWORD rightImageHandle = m_VGSEngine.SunGetImageHandleByName((char*)_bstr_t(rightImage));
	//	DWORD topImageHandle = m_VGSEngine.SunGetImageHandleByName((char*)_bstr_t(topImage));
	//	DWORD bottomImageHandle = m_VGSEngine.SunGetImageHandleByName((char*)_bstr_t(bottomImage));
	//	
	//	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//	DWORD cubeMapHandle = m_VGSEngine.CreateStaticCubeMap((char*)_bstr_t(name),
	//																frontImageHandle, backImageHandle,
	//																leftImageHandle, rightImageHandle,
	//																topImageHandle, bottomImageHandle,
	//																size);
	//	
	//	//离开临界区
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	//
	//	CCubeMap* pCubeMap = (CCubeMap*)cubeMapHandle;
	//	if (pCubeMap)
	//		m_VGSEngine.vgsScene->cubeMap.push_back(pCubeMap);

	return S_OK;
}

//设置材质为CubeMap反射材质
STDMETHODIMP CVGS2::SetMtrlCubeMap(BSTR mtrlName, BSTR cubeMapName, FLOAT power)
{
	//	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	//	DWORD cubeMapHandle = m_VGSEngine.GetCubeMapHandleByName((char*)_bstr_t(cubeMapName));
	//
	//	m_VGSEngine.SunSetMtrlCubeMap(mtrlHandle, cubeMapHandle, power);

	return S_OK;
}


//-----------------------------------------------------------------------------
//灯光相关的方法
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//方法: SetLightColor(BSTR lightName, INT red, INT green, INT blue)
//用途: 设置灯光的光线颜色
//参数: 
//lightName		灯光的名称，全部用大写字母
//red			灯光颜色的红色分量, 取值范围0-255
//green			灯光颜色的绿色分量, 取值范围0-255
//blue			灯光颜色的蓝色分量, 取值范围0-255
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetLightColor(BSTR lightName, INT red, INT green, INT blue)
{
	_VGS_RETURN
		DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	LightInfo info;
	m_VGSEngine.SunGetLightInfo(lightHandle, &info);
	info.Diffuse = VGSRGB(red, green, blue);
	m_VGSEngine.SunSetLightInfo(lightHandle, &info);
	//	CLight* pLight = (CLight*)lightHandle;
	//	if (pLight)
	//	{
	//		D3DCOLORVALUE vColor;
	//		vColor.r = red/255.0f;
	//		vColor.g = green/255.0f;
	//		vColor.b = blue/255.0f;
	//		vColor.a = 1.0f;
	//		pLight->param.Diffuse = vColor;
	//		INT index = m_VGSEngine.SunGetLightIndexByHandle(lightHandle);
	//		m_VGSEngine.vgsScene->m_pd3dDevice->SetLight(index, &pLight->param);
	//	}

	return S_OK;
}

//-----------------------------------------------------------------------------
//方法: SetLightPower(BSTR lightName, FLOAT power)
//用途: 设置灯光的颜色
//参数: 
//lightName		灯光的名称，全部用大写字母
//power			灯光强度, 取值范围0-1
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetLightPower(BSTR lightName, FLOAT power)
{
	_VGS_RETURN
		DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	LightInfo info;
	m_VGSEngine.SunGetLightInfo(lightHandle, &info);
	info.Multiply = power;
	m_VGSEngine.SunSetLightInfo(lightHandle, &info);
	//	DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	//	CLight* pLight = (CLight*)lightHandle;
	//	if (pLight)
	//	{
	//		pLight->param.Diffuse.r *= power;
	//		pLight->param.Diffuse.g *= power;
	//		pLight->param.Diffuse.b *= power;
	//		INT index = m_VGSEngine.SunGetLightIndexByHandle(lightHandle);
	//		m_VGSEngine.vgsScene->m_pd3dDevice->SetLight(index, &pLight->param);
	//	}

	return S_OK;
}

//-----------------------------------------------------------------------------
//方法: SetLightAmbient(BSTR lightName, INT red, INT green, INT blue)
//用途: 设置灯光的环境颜色
//参数: 
//lightName		灯光的名称，全部用大写字母
//red			灯光环境颜色的红色分量, 取值范围0-255
//green			灯光环境颜色的绿色分量, 取值范围0-255
//blue			灯光环境颜色的蓝色分量, 取值范围0-255
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetLightAmbient(BSTR lightName, INT red, INT green, INT blue)
{	
	_VGS_RETURN
		DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	LightInfo info;
	m_VGSEngine.SunGetLightInfo(lightHandle, &info);
	info.Ambient = VGSRGB(red, green, blue);
	m_VGSEngine.SunSetLightInfo(lightHandle, &info);

	//	DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	//	CLight* pLight = (CLight*)lightHandle;
	//	if (pLight)
	//	{
	//		D3DCOLORVALUE vColor;
	//		vColor.r = red/255.0f;
	//		vColor.g = green/255.0f;
	//		vColor.b = blue/255.0f;
	//		vColor.a = 1.0f;
	//		pLight->param.Ambient = vColor;
	//		INT index = m_VGSEngine.SunGetLightIndexByHandle(lightHandle);
	//		m_VGSEngine.vgsScene->m_pd3dDevice->SetLight(index, &pLight->param);
	//	}
	return S_OK;
}

//-----------------------------------------------------------------------------
//方法: SetLightPosition(BSTR lightName, FLOAT x, FLOAT y, FLOAT z)
//用途: 设置灯光的环境颜色
//参数: 
//lightName		灯光的名称，全部用大写字母
//x				灯光位置-X坐标值
//y				灯光位置-Y坐标值
//z				灯光位置-Z坐标值
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetLightPosition(BSTR lightName, FLOAT x, FLOAT y, FLOAT z)
{

	_VGS_RETURN
		DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	LightInfo info;
	m_VGSEngine.SunGetLightInfo(lightHandle, &info);
	info.Position = VECTOR3(x, y, z);
	m_VGSEngine.SunSetLightInfo(lightHandle, &info);

	//	DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	//	CLight* pLight = (CLight*)lightHandle;
	//	if (pLight)
	//	{
	//		pLight->param.Position = D3DXVECTOR3(x, y, z);
	//		INT index = m_VGSEngine.SunGetLightIndexByHandle(lightHandle);
	//		m_VGSEngine.vgsScene->m_pd3dDevice->SetLight(index, &pLight->param);
	//	}

	return S_OK;
}

//-----------------------------------------------------------------------------
//方法: SetLightRange(BSTR lightName, FLOAT range)
//用途: 设置灯光的照射范围
//参数: 
//lightName		灯光的名称，全部用大写字母
//range			灯光照射的范围
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetLightRange(BSTR lightName, FLOAT range)
{

	_VGS_RETURN
		DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	LightInfo info;
	m_VGSEngine.SunGetLightInfo(lightHandle, &info);
	info.Range = range;
	m_VGSEngine.SunSetLightInfo(lightHandle, &info);

	//	DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	//	CLight* pLight = (CLight*)lightHandle;
	//	if (pLight)
	//	{
	//		pLight->param.Range = range;
	//		INT index = m_VGSEngine.SunGetLightIndexByHandle(lightHandle);
	//		m_VGSEngine.vgsScene->m_pd3dDevice->SetLight(index, &pLight->param);
	//	}

	return S_OK;
}

//-----------------------------------------------------------------------------
//方法: SetLightAttenuation(BSTR lightName, FLOAT attenuation0, FLOAT attenuation1, FLOAT attenuation2)
//用途: 设置灯光的衰减值
//参数: 
//lightName				灯光的名称，全部用大写字母
//attenuation0			灯光衰减值
//attenuation1			灯光线性衰减值
//attenuation2			灯光二次衰减值
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetLightAttenuation(BSTR lightName, FLOAT attenuation0, FLOAT attenuation1, FLOAT attenuation2)
{

	_VGS_RETURN
		DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	LightInfo info;
	m_VGSEngine.SunGetLightInfo(lightHandle, &info);
	info.Attenuation0 = attenuation0;
	info.Attenuation1 = attenuation1;
	info.Attenuation2 = attenuation2;
	m_VGSEngine.SunSetLightInfo(lightHandle, &info);

	//	DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	//	CLight* pLight = (CLight*)lightHandle;
	//	if (pLight)
	//	{
	//		pLight->param.Attenuation0 = attenuation0;
	//		pLight->param.Attenuation1 = attenuation1;
	//		pLight->param.Attenuation2 = attenuation2;
	//		INT index = m_VGSEngine.SunGetLightIndexByHandle(lightHandle);
	//		m_VGSEngine.vgsScene->m_pd3dDevice->SetLight(index, &pLight->param);
	//	}

	return S_OK;
}

//-----------------------------------------------------------------------------
//方法: SetLightStatus(BSTR lightName, BOOL status)
//用途: 设置是否激活灯光
//参数: 
//lightName				灯光的名称，全部用大写字母
//status				灯光激活的状态
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetLightStatus(BSTR lightName, BOOL status)
{

	_VGS_RETURN
		DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	m_VGSEngine.SunSetLightEnableStatus(lightHandle, status);

	//	DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	//	CLight* pLight = (CLight*)lightHandle;
	//	if (pLight)
	//	{
	//		INT index = m_VGSEngine.SunGetLightIndexByHandle(lightHandle);
	//		m_VGSEngine.vgsScene->m_pd3dDevice->LightEnable(index, status);
	//	}
	return S_OK;
}

//-----------------------------------------------------------------------------
//方法: CreateMovieTexture(BSTR texName, BSTR moviePath, INT movieType, BOOL withAudio, BSTR mtrlName)
//用途: 创建电影贴图对象
//参数: 
//texName				电影贴图对象的名称
//movieFile				电影文件名
//movieType				电影文件类型
//withAudio				是否包含声音
//repeatCnt				重复播放的次数 
//mtrlName				材质名称，该材质为使用电影贴图的材质
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::CreateMovieTexture(BSTR texName, BSTR movieFile, 
									   INT movieType, BOOL withAudio, 
									   INT repeatCnt, BSTR mtrlName)
{

	//转换BSTR
	//	char pMovieFileName[512];
	//	strcpy(pMovieFileName, m_webServer);
	//	strcat(pMovieFileName, (char *)_bstr_t(movieFile));
	//
	//	//防止创建时点击刷新按钮或关掉IE
	//	EnterCriticalSection(&m_ctrl_cs);
	//	DWORD movieTexHandle = 
	//		m_VGSEngine.CreateMovieTexture((char*)_bstr_t(texName),
	//												pMovieFileName,
	//												movieType, withAudio,
	//												repeatCnt,
	//												(char*)_bstr_t(mtrlName),
	//												&m_render_cs,
	//												&m_ctrl_cs);
	//	
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

//-----------------------------------------------------------------------------
//方法: SetMovieMtrl(BSTR mtrlName, BSTR movieTexName)
//用途: 设置材质为电影贴图材质
//参数: 
//mtrlName				材质的名称
//lightName				电影贴图对象的名称
//-----------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetMovieMtrl(BSTR mtrlName, BSTR movieTexName)
{
	//	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	//	DWORD movieTexHandle = m_VGSEngine.GetMovieTextureHandleByName((char*)_bstr_t(movieTexName));
	//	if (mtrlHandle && movieTexHandle)
	//		m_VGSEngine.SetMovieMtrl(mtrlHandle, movieTexHandle);

	return S_OK;
}

//设定模型是否播放关键帧动画
//name模型名称
//bKfrPlay是否播放关键帧动画
STDMETHODIMP CVGS2::SetModelKfrPlayStatus(BSTR modelName, BOOL bKfrPlay)
{
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel)
	//	{
	//		pModel->kfrPlay = bKfrPlay;
	//	}

	return S_OK;
}

//设置所有模型的关键帧播放状态
STDMETHODIMP CVGS2::SetAllModelKfrPlayStatus(BOOL bKfrPlay)
{
	//	if (m_VGSEngine.vgsScene == NULL)
	//		return S_OK;
	//
	//	INT modelCnt = m_VGSEngine.vgsScene->model.size();
	//	CModel** ppModel = m_VGSEngine.vgsScene->model.begin();
	//	for (INT i = 0; i< modelCnt; i++)
	//	{
	//		CModel* pModel = *(ppModel + i);
	//		pModel->kfrPlay = bKfrPlay;
	//	}
	return S_OK;
}

//设置所有模型的关键帧播放状态
STDMETHODIMP CVGS2::SetAllModelsKfrPlayStatus(BOOL bKfrPlay)
{
	//	if (m_VGSEngine.vgsScene == NULL)
	//		return S_OK;
	//
	//	INT modelCnt = m_VGSEngine.vgsScene->model.size();
	//	CModel** ppModel = m_VGSEngine.vgsScene->model.begin();
	//	for (INT i = 0; i< modelCnt; i++)
	//	{
	//		CModel* pModel = *(ppModel + i);
	//		pModel->kfrPlay = bKfrPlay;
	//	}
	return S_OK;
}

//设置某个模型播放的关键帧动画循环次数
STDMETHODIMP CVGS2::SetModelKfrRepeatCnt(BSTR modelName, INT kfrRepeatCnt)
{
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel && pModel->kfrCnt > 1)
	//	{
	//		pModel->kfrRepeatCnt = kfrRepeatCnt;
	//		pModel->kfrPlay = TRUE;
	//	}
	return S_OK;
}

//设置所有的模型播放的关键帧动画循环次数
STDMETHODIMP CVGS2::SetAllModelKfrRepeatCnt(INT kfrRepeatCnt)
{
	//	CModel** ppModel = m_VGSEngine.vgsScene->model.begin();
	//	INT modelCnt = m_VGSEngine.vgsScene->model.size();
	//	for (INT i = 0; i< modelCnt; i++)
	//	{
	//		CModel* pModel = *(ppModel + i);
	//		if (pModel && pModel->kfrCnt > 1)
	//		{
	//			pModel->kfrRepeatCnt = kfrRepeatCnt;
	//			pModel->kfrPlay = TRUE;
	//		}
	//	}
	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: CreateBackdropFromImage()
//参数: BSTR backdropName, BSTR oriImageName, BSTR alphaImageName, INT alignMode, INT left, INT top, INT width, INT height
//描述: 从Image对象创建Overlay
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::CreateBackdropFromImage(BSTR backdropName, BSTR oriImageName, BSTR alphaImageName, INT alignMode, INT left, INT top, INT width, INT height)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	//转换BSTR
	//	CHAR strBackdropName[32];
	//	strcpy(strBackdropName, (char*)_bstr_t(backdropName));
	//
	//	CHAR strOriImageName[32];
	//	strcpy(strOriImageName, (char*)_bstr_t(oriImageName));
	//
	//	CHAR strAlphaImageName[32];
	//	strcpy(strAlphaImageName, (char*)_bstr_t(alphaImageName));
	//
	//	BOOL bFullH = FALSE; 
	//	BOOL bFullV = FALSE;
	//	RECT olRect;
	//	olRect.left = left;
	//	olRect.top = top;
	//	if (width < 0)
	//	{
	//		width = m_VGSEngine.vgsScene->rect.right - m_VGSEngine.vgsScene->rect.left;
	//		bFullH = TRUE;
	//	}
	//	if (height < 0)
	//	{
	//		height = m_VGSEngine.vgsScene->rect.bottom - m_VGSEngine.vgsScene->rect.top;
	//		bFullV = height;
	//	}
	//
	//	olRect.right = olRect.left + width;
	//	olRect.bottom = olRect.top + height;
	//
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	DWORD overlayHandle = m_VGSEngine.SunNewOverlayFromImage(strBackdropName, strOriImageName, strAlphaImageName, (ALIGNMODE)alignMode, &olRect);
	//	COverlay* pOverlay = (COverlay*)overlayHandle;
	//	if (pOverlay)
	//	{
	//		//pOverlay->SetZValue(0.1f);
	//		pOverlay->isBackdrop = TRUE;
	//	}
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	//
	//	if (pOverlay)
	//	{
	//		pOverlay->m_bFullH = bFullH;
	//		pOverlay->m_bFullV = bFullV;
	//	}

	return S_OK;
}

//从Image 创建高光贴图
STDMETHODIMP CVGS2::CreateSpecularMap(BSTR name, BSTR imageName, INT width, INT height, INT mipLevel)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//	DWORD specularMapHandle = 
	//		m_VGSEngine.SunCreateTextureFromImage((char*)_bstr_t(name),
	//													 (char*)_bstr_t(imageName), 
	//													 RGBA8880,
	//													 mipLevel);
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	//
	//	CTexture* pTex = (CTexture*)specularMapHandle;
	//	if (pTex)
	//	{
	//		m_VGSEngine.vgsScene->texture.push_back(pTex);
	//	}

	return S_OK;
}

//设置材质的高光贴图
STDMETHODIMP CVGS2::SetSpecularMapMtrl(BSTR mtrlName, BSTR specularMapName)
{
	/*	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	DWORD specularMapHandle = m_VGSEngine.SunGetTextureHandleByName((char*)_bstr_t(specularMapName));

	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	CTexture* pTex = (CTexture*)specularMapHandle;

	if (pMtrl && pTex)
	{
	pMtrl->specularTex = pTex;
	}
	*/
	return S_OK;
}

//创建摄像机
STDMETHODIMP CVGS2::CreateCamera(BSTR name, FLOAT fromX, FLOAT fromY, FLOAT fromZ, FLOAT atX, FLOAT atY, FLOAT atZ, FLOAT fov, FLOAT nearClip, FLOAT farClip)
{
	_VGS_RETURN
	DWORD camHandle = m_VGSEngine.SunCreateCamera(_BSTR_TO_STR(name).c_str(), &D3DXVECTOR3(fromX, fromY, fromZ), &D3DXVECTOR3(atX, atY, atZ));
	m_VGSEngine.SunSetCameraFov(camHandle, fov);
	m_VGSEngine.SunSetCameraNearClip(camHandle, nearClip);
	m_VGSEngine.SunSetCameraFarClip(camHandle, farClip);
	//	D3DXVECTOR3 from = D3DXVECTOR3(fromX, fromY, fromZ);
	//	D3DXVECTOR3 at = D3DXVECTOR3(atX, atY, atZ);
	//	DWORD camHandle = m_VGSEngine.CreateCamera((char*)_bstr_t(name), &from, &at);
	//	CCamera* pCamera = (CCamera*)camHandle;
	//	if(pCamera)
	//	{
	//		fov *= ATOS;
	//		pCamera->SetFov(fov);
	//		pCamera->m_nearClip = nearClip;
	//		pCamera->m_farClip = farClip;
	//		
	//		m_VGSEngine.vgsScene->camera.push_back(pCamera);
	//	}									

	return S_OK;
}
STDMETHODIMP CVGS2::PauseCameraKfr(BSTR camName)
{
	//	m_VGSEngine.vgsScene->m_bKfrPlaying = FALSE;
	//
	//	UINT camCnt = m_VGSEngine.vgsScene->camera.size();
	//	CCamera** ppCamera = m_VGSEngine.vgsScene->camera.begin();
	//	for (UINT i = 0; i< camCnt; i++)
	//	{
	//		CCamera* pCamera = *(ppCamera + i);
	//		if (strcmp(pCamera->name.c_str(), (char*)_bstr_t(camName)) == 0)
	//		{
	//			pCamera->Pause();
	//			return S_OK;
	//		}
	//	}

	return S_OK;
}

STDMETHODIMP CVGS2::ResumeCameraKfr(BSTR cameraName)
{
	//	m_VGSEngine.vgsScene->m_bKfrPlaying = TRUE;
	//	// TODO: Add your implementation code here
	//	UINT camCnt = m_VGSEngine.vgsScene->camera.size();
	//	CCamera** ppCamera = m_VGSEngine.vgsScene->camera.begin();
	//	for (UINT i = 0; i< camCnt; i++)
	//	{
	//		CCamera* pCamera = *(ppCamera + i);
	//		if (strcmp(pCamera->name.c_str(), (char*)_bstr_t(cameraName)) == 0)
	//		{
	//			pCamera->Play();
	//			return S_OK;
	//		}
	//	}
	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: PlayCameraKfr()
//参数: BSTR camName, INT repeatCnt
//描述: 播放指定摄像机的关键帧动画
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::PlayCameraKfr(BSTR camName, INT repeatCnt)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	//如果场景关键帧没有播放，则先设置关键帧一定为播放
	//	m_VGSEngine.vgsScene->m_bKfrPlaying = TRUE;
	//
	//	UINT camCnt = m_VGSEngine.vgsScene->camera.size();
	//	CCamera** ppCamera = m_VGSEngine.vgsScene->camera.begin();
	//	for (UINT i = 0; i< camCnt; i++)
	//	{
	//		CCamera* pCamera = *(ppCamera + i);
	//		if (strcmp(pCamera->name.c_str(), (char*)_bstr_t(camName)) == 0)
	//		{
	//			//保留当前动态模糊的状态
	//			m_VGSEngine.vgsScene->m_bLastFSMotionBlurStatus = 
	//				m_VGSEngine.vgsScene->m_bFSMotionBlur;
	//			StopFSMotionBlur();
	//
	//			pCamera->kfrRepeatIndex = 0;
	//			pCamera->Play();
	//			pCamera->kfrRepeatCnt = repeatCnt;
	//
	//			
	//			//同步模型的关键帧动画
	//			CModel** ppModel = m_VGSEngine.vgsScene->model.begin();
	//			INT modelCnt = m_VGSEngine.vgsScene->model.size();
	//			for (INT j = 0; j< modelCnt; j++)
	//			{
	//				CModel* pModel = *(ppModel + j);
	//				if (pModel->kfrCnt > 1)
	//					pModel->kfrCurFrame = 0;
	//			}
	//
	//			return S_OK;
	//		}
	//	}

	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: SetCurrentCameraFrom()
//参数: FLOAT x, FLOAT y, FLOAT z
//描述: 设定当前相机的From点
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetCurrentCameraFrom(FLOAT x, FLOAT y, FLOAT z)
{
	MoveCurrentCameraFromTo(x, y, z);
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	if (m_VGSEngine.vgsScene->currentCam == NULL) return S_OK;
	//
	//	D3DXVECTOR3 vec = D3DXVECTOR3(x, y, z);
	//	m_VGSEngine.vgsScene->currentCam->SetFrom(&vec);
	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: SetCurrentCameraAt()
//参数: FLOAT x, FLOAT y, FLOAT z
//描述: 设定当前相机的At点
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::SetCurrentCameraAt(FLOAT x, FLOAT y, FLOAT z)
{
	MoveCurrentCameraAtTo(x, y, z);
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	if (m_VGSEngine.vgsScene->currentCam == NULL) return S_OK;
	//
	//	D3DXVECTOR3 vec = D3DXVECTOR3(x, y, z);
	//	m_VGSEngine.vgsScene->currentCam->SetAt(&vec);
	return S_OK;
}

//--------------------------------------------------------------------------------
//方法: StopCameraKfr()
//参数: BSTR camName
//描述: 停止播放指定摄像机的关键帧动画
//--------------------------------------------------------------------------------
STDMETHODIMP CVGS2::StopCameraKfr(BSTR camName)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	UINT camCnt = m_VGSEngine.vgsScene->camera.size();
	//	CCamera** ppCamera = m_VGSEngine.vgsScene->camera.begin();
	//	for (UINT i = 0; i< camCnt; i++)
	//	{
	//		CCamera* pCamera = *(ppCamera + i);
	//		if (strcmp(pCamera->name.c_str(), (char*)_bstr_t(camName)) == 0)
	//		{
	//			if (pCamera->kfrCnt > 1)
	//			{
	//				pCamera->Stop();
	//				pCamera->kfrRepeatCnt = -1;
	//			}
	//			return S_OK;
	//		}
	//	}
	return S_OK;
}


//删除模型
STDMETHODIMP CVGS2::DeleteModel(BSTR modelName)
{
	_VGS_RETURN
		DWORD modelHanlde = m_VGSEngine.SunGetModelHandleByName(_BSTR_TO_STR(modelName).c_str());
	m_VGSEngine.SunDeleteModelByHandle(modelHanlde);
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//	m_VGSEngine.SunDeleteModelByHandle(modelHandle);
	//	//清空临时模型变量
	//	if ((DWORD)(m_pVGS2Wnd->m_pCurCursorModel) == modelHandle)
	//		m_pVGS2Wnd->m_pCurCursorModel = NULL;
	//	//清空临时模型变量
	//	if ((DWORD)(m_pVGS2Wnd->m_pLastCursorModel) == modelHandle)
	//		m_pVGS2Wnd->m_pLastCursorModel = NULL;
	//	//清空临时模型变量
	//	if ((DWORD)(m_pVGS2Wnd->m_pLastSelectedModel) == modelHandle)
	//		m_pVGS2Wnd->m_pLastSelectedModel = NULL;
	//	//清空临时模型变量
	//	if ((DWORD)(m_pVGS2Wnd->m_pCurSelectedModel) == modelHandle)
	//		m_pVGS2Wnd->m_pCurSelectedModel = NULL;
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

//设置当前Player是否可以控制移动
STDMETHODIMP CVGS2::SetPlayerCanMove(BOOL status)
{
	//	m_VGSEngine.bCanMove = status;

	return S_OK;
}

//设置当前Player是否可以控制旋转
STDMETHODIMP CVGS2::SetPlayerCanRos(BOOL status)
{
	//	m_VGSEngine.bCanRos = status;

	return S_OK;
}

//设置当前Player是否可以控制推拉镜头
STDMETHODIMP CVGS2::SetPlayerCanZoom(BOOL status)
{
	//m_VGSEngine.bCanZoom = status;

	return S_OK;
}

//设置当前Player是否可以控制镜头升降
STDMETHODIMP CVGS2::SetPlayerCanLift(BOOL status)
{
	//	m_VGSEngine.bCanLift = status;

	return S_OK;
}

//删除电影贴图
STDMETHODIMP CVGS2::DeleteMovieTexture(BSTR mtrlName, BSTR movieTexName)
{
	//	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	//	DWORD movieTexHandle = m_VGSEngine.GetMovieTextureHandleByName((char*)_bstr_t(movieTexName));
	//		
	//	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	//	if (pMtrl && movieTexHandle)
	//	{
	//		
	//		//防止删除MovieTexture时，渲染线程访问CheckMovieStatus();
	//		EnterCriticalSection(&m_render_cs);
	//
	//		//清空贴图资源
	//		UINT texLayerCnt =pMtrl->texLayersList.size();
	//		TextureLayer** ppTexLayer =(TextureLayer**)(pMtrl->texLayersList.begin());
	//		//删除该材质所在的texLayer
	//		for(UINT m=0; m<texLayerCnt; m++)
	//		{
	//			TextureLayer* pTexLayer =*(ppTexLayer + m);
	//			if(pTexLayer->Name ==std::string((char*)_bstr_t(movieTexName))&& pTexLayer->type == TLT_MOVIE)
	//			{
	//			   //ZeroMemory(pTexLayer->Name, sizeof(pTexLayer->Name));
	//			   pTexLayer->pTexture = NULL;
	//			   pMtrl->texLayersList.erase(ppTexLayer + m);
	//			   break;
	//			}			
	//		}
	//		m_VGSEngine.DeleteMovieTexture(movieTexHandle);
	//
	//		LeaveCriticalSection(&m_render_cs);
	//	}

	return S_OK;
}


//播放模型的关键帧动画
//modelName模型名称
//startFrame开始帧
//endFrame结束帧
//repeatCnt重复次数
STDMETHODIMP CVGS2::PlayModelKfr(BSTR modelName, INT startFrame, INT endFrame, INT repeatCnt)
{
	//	m_VGSEngine.vgsScene->m_bKfrPlaying = TRUE;
	//
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel)
	//	{
	//		if (startFrame < 0 || 
	//			endFrame <= 0 ||
	//			startFrame > pModel->kfrCnt ||
	//			endFrame > pModel->kfrCnt)
	//		{
	//			return S_OK;
	//		}
	//		pModel->startFrame = startFrame;
	//		pModel->kfrCurFrame = startFrame;
	//		pModel->endFrame = endFrame;
	//		pModel->kfrRepeatCnt = repeatCnt;
	//		pModel->kfrRepeatIndex = 0;
	//		pModel->kfrPlay = TRUE;
	//	}

	return S_OK;
}


//设置按钮是否可见
STDMETHODIMP CVGS2::SetButtonVisibility(BSTR btnName, BOOL visible)
{
	//	DWORD btnHandle = m_VGSEngine.SunGetButtonByName((char*)_bstr_t(btnName));
	//	CButton* pButton = (CButton*)btnHandle;
	//	if (pButton)
	//		pButton->SetVisible(visible);

	return S_OK;
}

//设置系统的抗拒齿级别
STDMETHODIMP CVGS2::SetAntiAliasLevel(INT level)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//	m_VGSEngine.SetAntiAliasLevel(level);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

//设置材质的高光CubeMap
STDMETHODIMP CVGS2::SetMtrlSpecularCubeMap(BSTR mtrlName, BSTR specularCubeMapName)
{
	//	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	//	DWORD SCubeMapHandle = m_VGSEngine.GetCubeMapHandleByName((char*)_bstr_t(specularCubeMapName));
	//	
	//	if (mtrlHandle)
	//		m_VGSEngine.SetMtrlSpecularCubeMap(mtrlHandle, SCubeMapHandle);

	return S_OK;
}

//设置按钮是否激活
STDMETHODIMP CVGS2::SetButtonActive(BSTR name, BOOL bActive)
{
	//	CButton* pBtn = (CButton*)(m_VGSEngine.SunGetButtonByName((char*)_bstr_t(name)));
	//	if (pBtn)
	//	{
	//		pBtn->enable = bActive;
	//	}

	return S_OK;
}

//创建剖面图对象
//sectionMapName-剖面图对象名称
//width, height-剖面图尺寸
//sliceModelName-切片模型
//leftTopX, leftTopY, leftTopZ 侧面左上角坐标
//rightTopX, rightTopY, rightTopZ 侧面右上角坐标
//leftBottomX, leftBottomY, leftBottom 侧面左下角坐标
//rightBottomX, rightBottomY, rightBottomZ 侧面右下角坐标
//image对象名称
STDMETHODIMP CVGS2::CreateSectionMap(BSTR sectionMapName, 
									 INT width, INT height,
									 BSTR sliceModelName, 
									 FLOAT leftTopX, FLOAT leftTopY, FLOAT leftTopZ, 
									 FLOAT rightTopX, FLOAT rightTopY, FLOAT rightTopZ, 
									 FLOAT leftBottomX, FLOAT leftBottomY, FLOAT leftBottomZ, 
									 FLOAT rightBottomX, FLOAT rightBottomY, FLOAT rightBottomZ, 
									 BSTR imageName)
{
	//	DWORD sliceModelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(sliceModelName));
	//	DWORD imageHandle = m_VGSEngine.SunGetImageHandleByName((char*)_bstr_t(imageName));
	//
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//	m_VGSEngine.CreateSectionMap((char*)_bstr_t(sectionMapName),
	//										width, height,
	//										sliceModelHandle,
	//										leftTopX, leftTopY, leftTopZ,
	//										rightTopX, rightTopY, rightTopZ,
	//										leftBottomX, leftBottomY, leftBottomZ,
	//										rightBottomX, rightBottomY, rightBottomZ,
	//										imageHandle);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//执行切片计算
STDMETHODIMP CVGS2::ExecuteSlice(BSTR sectionMapName)
{
	//	DWORD sectionMapHandle = m_VGSEngine.GetSectionMapHandleByName((char*)_bstr_t(sectionMapName));
	//	if (sectionMapHandle)
	//	{
	//		
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//
	//		m_VGSEngine.ExecuteSlice(sectionMapHandle);
	//
	//		LeaveCriticalSection(&m_render_cs);
	//		LeaveCriticalSection(&m_ctrl_cs);
	//	}
	return S_OK;
}

//设置Overlay是否可见
STDMETHODIMP CVGS2::SetOverlayVisibility(BSTR overlayName, BOOL visible)
{
	// TODO: Add your implementation code here
	//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName((char*)_bstr_t(overlayName));
	//	COverlay* pOverlay = (COverlay*)overlayHandle;
	//	if (pOverlay)
	//		pOverlay->m_bVisible = visible;
	return S_OK;
}

//----------------------------------------------------------------------------
//创建Audio对象
STDMETHODIMP CVGS2::CreateAudio(BSTR name, BSTR audioFile, INT repeatCnt)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	//转换BSTR
	//	char pAudioFileName[512];
	//	strcpy(pAudioFileName, m_webServer);
	//	strcat(pAudioFileName, (char *)_bstr_t(audioFile));
	//
	//	//如果是本地文件
	//	if (m_webServer[0] == 'f' && m_webServer[1] == 'i' && m_webServer[2] == 'l' && m_webServer[3] == 'e')
	//	{
	//		for (INT i = 0; i< 512 -4; i++)
	//			pAudioFileName[i] = pAudioFileName[i + 8];
	//	}
	//	
	//	m_VGSEngine.CreateAudio((char*)_bstr_t(name),
	//									(char*)_bstr_t(pAudioFileName),
	//									repeatCnt);
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::DeleteAudioByName(BSTR name)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	// TODO: Add your implementation code here
	//	DWORD audioHandle = m_VGSEngine.GetAudioHandleByName((char*)_bstr_t(name));
	//	m_VGSEngine.DeleteAudio(audioHandle);
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::DeleteAudioByIndex(INT index)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	// TODO: Add your implementation code here
	//	DWORD audioHandle = m_VGSEngine.GetAudioHandleByIndex(index);
	//	m_VGSEngine.DeleteAudio(audioHandle);
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

STDMETHODIMP CVGS2::DeleteAllAudio()
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	// TODO: Add your implementation code here
	//	m_VGSEngine.DeleteAllAudio();
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::SetAudioFile(BSTR audioName, BSTR audioFile)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	// TODO: Add your implementation code here
	//	DWORD audioHandle = m_VGSEngine.GetAudioHandleByName((char*)_bstr_t(audioName));
	//	//得到服务器地址
	//	//转换BSTR
	//	char pAudioFileName[512];
	//	strcpy(pAudioFileName, m_webServer);
	//	strcat(pAudioFileName, (char *)_bstr_t(audioFile));
	//
	//	//如果是本地文件
	//	if (m_webServer[0] == 'f' && m_webServer[1] == 'i' && m_webServer[2] == 'l' && m_webServer[3] == 'e')
	//	{
	//		for (INT i = 0; i< 512 -4; i++)
	//			pAudioFileName[i] = pAudioFileName[i + 8];
	//	}
	//
	//	m_VGSEngine.SetAudioFilePath(audioHandle, pAudioFileName);
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::SetAudioRepeatCount(BSTR audioName, INT repeatCnt)
{
	// TODO: Add your implementation code here
	//	DWORD audioHandle = m_VGSEngine.GetAudioHandleByName((char*)_bstr_t(audioName));
	//	m_VGSEngine.SetAudioRepeatCount(audioHandle, repeatCnt);
	return S_OK;
}

STDMETHODIMP CVGS2::PlayAudio(BSTR audioName)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	// TODO: Add your implementation code here
	//	DWORD audioHandle = m_VGSEngine.GetAudioHandleByName((char*)_bstr_t(audioName));
	//	m_VGSEngine.PlayAudio(audioHandle);
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::PauseAudio(BSTR audioName)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	// TODO: Add your implementation code here
	//	DWORD audioHandle = m_VGSEngine.GetAudioHandleByName((char*)_bstr_t(audioName));
	//	m_VGSEngine.PauseAudio(audioHandle);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::StopAudio(BSTR audioName)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	// TODO: Add your implementation code here
	//	DWORD audioHandle = m_VGSEngine.GetAudioHandleByName((char*)_bstr_t(audioName));
	//	m_VGSEngine.StopAudio(audioHandle);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::RewindAudio(BSTR audioName)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	// TODO: Add your implementation code here
	//	DWORD audioHandle = m_VGSEngine.GetAudioHandleByName((char*)_bstr_t(audioName));
	//	m_VGSEngine.RewindAudio(audioHandle);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}


//--------------------------------------------------------------------------
//ModelGroup
//--------------------------------------------------------------------------
//创建模型组对象
STDMETHODIMP CVGS2::CreateModelGroup(BSTR modelGroupName)
{
	// TODO: Add your implementation code here
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	m_VGSEngine.CreateModelGroup((char*)_bstr_t(modelGroupName));
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::DeleteModelGroup(BSTR name)
{
	// TODO: Add your implementation code here
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	m_VGSEngine.DeleteModelGroupByName((char*)_bstr_t(name));
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::DeleteModelGroupByIndex(INT index)
{
	// TODO: Add your implementation code here
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	m_VGSEngine.DeleteModelGroupByIndex(index);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

STDMETHODIMP CVGS2::DeleteAllModelGroup()
{
	// TODO: Add your implementation code here
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	m_VGSEngine.DeleteAllModelGroup();
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::AddModelToModelGroup(BSTR modelGroupName, BSTR modelName)
{
	// TODO: Add your implementation code here
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	DWORD modelGroupHandle = m_VGSEngine.GetModelGroupHandleByName((char*)_bstr_t(modelGroupName));
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	if (modelHandle)
	//		m_VGSEngine.AddModelToModelGroup(modelGroupHandle, modelHandle);
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::DeleteModelFromModelGroup(BSTR modelGroupName, BSTR modelName)
{
	// TODO: Add your implementation code here
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	DWORD modelGroupHandle = m_VGSEngine.GetModelGroupHandleByName((char*)_bstr_t(modelGroupName));
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	m_VGSEngine.DeleteModelFromModelGroup(modelGroupHandle, modelHandle);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::ClearModelGroup(BSTR modelGroupName)
{
	// TODO: Add your implementation code here
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	DWORD modelGroupHandle = m_VGSEngine.GetModelGroupHandleByName((char*)_bstr_t(modelGroupName));
	//	m_VGSEngine.ClearModelGroup(modelGroupHandle);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

//在鼠标移移动事件中是否激活ModelUnderLoc检测
STDMETHODIMP CVGS2::SetModelEventOnMouseMove(BOOL status)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	m_pVGS2Wnd->m_bModelEventOnMouseMove = status;
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//设置模型是否响应鼠标移入\移出\按下\松开，这将改变模型的材质
STDMETHODIMP CVGS2::SetModelMouseHint(BSTR modelName, BOOL bFlag)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	CModel* pModel = (CModel*)(m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName)));
	//	if(pModel != NULL)
	//		pModel->mouseInOutable = bFlag;
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//设置模型在鼠标响应事件的顶点颜色
STDMETHODIMP CVGS2::SetModelMouseColor(BSTR modelName, 
									   INT mouseOver_Red, 
									   INT mouseOver_Green, 
									   INT mouseOver_Blue,
									   INT mouseOver_Alpha,
									   INT mouseClick_Red,
									   INT mouseClick_Green,
									   INT mouseClick_Blue,
									   INT mouseClick_Alpha)
{
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel)
	//	{
	//		pModel->bMouseColor = TRUE;
	//		pModel->mouseOverColor = D3DCOLOR_RGBA(mouseOver_Red, mouseOver_Green, mouseOver_Blue, mouseOver_Alpha);
	//		pModel->mouseClickedColor = D3DCOLOR_RGBA(mouseClick_Red, mouseClick_Green, mouseClick_Blue, mouseClick_Alpha);
	//	}
	return S_OK;
}

//设置模型是否响应鼠标事件的方式，有两种方式可供选择，一种是以模型为单元响应鼠标事件；
//一种是以模型所在的模型组为单元响应鼠标事件。譬如设置BOX1当鼠标移上时变为红色，
//选中后变为绿色，当设置响应类型为[模型]响应方式效果正常；但如果设置为[模型组]响应方式时，
//当鼠标移上BOX1时，BOX1所在的ModelGroup中的所有物体均会变成红色，选中后均会变成绿色。
STDMETHODIMP CVGS2::SetModelMouseHintType(BSTR modelName, INT type)
{
	// TODO: Add your implementation code here
	//	CModel* pModel = (CModel*)(m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName)));
	//	if(pModel != NULL)
	//		pModel->bmouseHintByGroup = (BOOL)type;

	return S_OK;
}

STDMETHODIMP CVGS2::SetLightKfrPlayStatus(BSTR lightName, BOOL bFlag)
{
	// TODO: Add your implementation code here
	//	DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	//	m_VGSEngine.SetLightKfrPlayStatus(lightHandle, bFlag);
	return S_OK;
}

STDMETHODIMP CVGS2::SetAllLightKfrPlayStatus(BOOL bFlag)
{
	// TODO: Add your implementation code here
	//	m_VGSEngine.SetAllLightKfrPlayStatus(bFlag);
	return S_OK;
}

STDMETHODIMP CVGS2::SetLightKfrRepeatCnt(BSTR lightName, INT repeatCnt)
{
	// TODO: Add your implementation code here
	//	DWORD lightHandle = m_VGSEngine.SunGetLightHandleByName((char*)_bstr_t(lightName));
	//	m_VGSEngine.SetLightKfrRepeatCnt(lightHandle, repeatCnt);

	return S_OK;
}

STDMETHODIMP CVGS2::SetAllLightKfrRepeatCnt(INT repeatCnt)
{
	// TODO: Add your implementation code here
	//	m_VGSEngine.SetAllLightKfrRepeatCnt(repeatCnt);
	return S_OK;
}

//设置模型的位置
STDMETHODIMP CVGS2::SetModelPosition(BSTR modelName, FLOAT x, FLOAT y, FLOAT z)
{
	_VGS_RETURN
		DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName(_BSTR_TO_STR(modelName).c_str());
	m_VGSEngine.SunSetModelPosition(modelHandle, &VECTOR3(x, y, z));
	// TODO: Add your implementation code here
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel)
	//		pModel->pNode->setPosition(&D3DXVECTOR3(x, y, z));

	return S_OK;
}

//设置模型的转角
STDMETHODIMP CVGS2::SetModelRotation(BSTR modelName, FLOAT x, FLOAT y, FLOAT z)
{
	_VGS_RETURN

		DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName(_BSTR_TO_STR(modelName).c_str());
	m_VGSEngine.SunSetModelRotation(modelHandle, &VECTOR3(x, y, z));
	// TODO: Add your implementation code here
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//	if (pModel)
	//		//pModel->SetRotation(&D3DXVECTOR3(x, y, z));
	//		pModel->pNode->setRotationByAngle(&D3DXVECTOR3(x, y, z));
	return S_OK;
}

//设置导航图是否可见
STDMETHODIMP CVGS2::SetNavigaterVisibility(BSTR navName, BOOL visible)
{
	//	DWORD navHandle = m_VGSEngine.SunGetNavigaterByName((char*)_bstr_t(navName));
	//	CNavigater* pNav = (CNavigater*)navHandle;
	//	if (pNav)
	//		pNav->SetVisibility(visible);
	return S_OK;
}

//设置是否可以通过Pointer改变相机的位置
STDMETHODIMP CVGS2::SetCanNavigateByPointer(BSTR navName, BOOL status)
{	
	//	DWORD navHandle = m_VGSEngine.SunGetNavigaterByName((char*)_bstr_t(navName));
	//	CNavigater* pNav = (CNavigater*)navHandle;
	//	if (pNav)
	//		pNav->m_bCanNavByPointer = status;

	return S_OK;
}

//增加导航指针
STDMETHODIMP CVGS2::AddNavigaterPointer(BSTR navigaterName, BSTR navPointerName, 
										INT	 navPointerType,
										BSTR camOrModelName,
										BSTR navPointerOriImage, 
										BSTR navPointerAlphaImage, 
										INT width, INT height)
{
	// TODO: Add your implementation code here
	//	DWORD navHandle = m_VGSEngine.SunGetNavigaterByName((char*)_bstr_t(navigaterName));
	//	if (navHandle)
	//	{
	//		DWORD pointerObjHandle = 0;
	//		switch (navPointerType)
	//		{
	//		case 0:
	//			pointerObjHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(camOrModelName));
	//			break;
	//		case 1:
	//			pointerObjHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(camOrModelName));
	//			break;
	//		}
	//		
	//		if (pointerObjHandle)
	//		{
	//			m_VGSEngine.AddNavigaterPointer(navHandle, 
	//												   (char*)_bstr_t(navPointerName),
	//												   navPointerType,
	//												   pointerObjHandle,
	//												   (char*)_bstr_t(navPointerOriImage),
	//												   (char*)_bstr_t(navPointerAlphaImage),
	//												   width, height);
	//		}
	//	}
	return S_OK;
}

//设置导航指针是否可见
STDMETHODIMP CVGS2::SetNavigaterPointerVisibility(BSTR navigaterName, BSTR navPointerName, BOOL status)
{
	// TODO: Add your implementation code here
	//	DWORD navHandle = m_VGSEngine.SunGetNavigaterByName((char*)_bstr_t(navigaterName));
	//	CNavigater* pNav = (CNavigater*)navHandle;
	//	if (pNav)
	//	{
	//		INT navPointerCnt = pNav->m_NavPointer.size();
	//		NavPointer** ppNavPointer = pNav->m_NavPointer.begin();
	//		for (INT i = 0; i< navPointerCnt; i++)
	//		{
	//			NavPointer* pNavPointer = *(ppNavPointer + i);
	//			if (strcmp(pNavPointer->Name, (char*)_bstr_t(navPointerName)) == 0)
	//			{
	//				pNavPointer->pPointerOverlay->m_bVisible = status;
	//				break;
	//			}
	//		}
	//	}
	return S_OK;
}

//删除Navigater的Pointer
STDMETHODIMP CVGS2::DeleteNavigaterPointer(BSTR navigaterName, BSTR navPointerName)
{
	// TODO: Add your implementation code here
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	DWORD navHandle = m_VGSEngine.SunGetNavigaterByName((char*)_bstr_t(navigaterName));
	//	CNavigater* pNav = (CNavigater*)navHandle;
	//	if (pNav)
	//	{
	//		INT navPointerCnt = pNav->m_NavPointer.size();
	//		NavPointer** ppNavPointer = pNav->m_NavPointer.begin();
	//		for (INT i = 0; i< navPointerCnt; i++)
	//		{
	//			NavPointer* pNavPointer = *(ppNavPointer + i);
	//			if (strcmp(pNavPointer->Name, (char*)_bstr_t(navPointerName)) == 0)
	//			{
	//				//删除NavPointer的Overlay
	//				INT overlayCnt = m_VGSEngine.vgsScene->overlay.size();
	//				COverlay** ppOverlay = m_VGSEngine.vgsScene->overlay.begin();
	//				
	//				for (INT j = 0; j< overlayCnt; j++)
	//				{
	//					COverlay* pOverlay = *(ppOverlay + j);
	//					if (pOverlay == pNavPointer->pPointerOverlay)
	//					{
	//						SAFE_RELEASE(pOverlay);
	//						SAFE_DELETE(pOverlay);
	//						m_VGSEngine.vgsScene->overlay.erase(ppOverlay + j);
	//						break;
	//					}
	//				}
	//
	//				//Overlay用到的贴图还没有删除?
	//
	//
	//				//删除导航指针
	//				SAFE_DELETE(pNavPointer);
	//				pNav->m_NavPointer.erase(ppNavPointer + i);
	//				break;
	//			}
	//		}
	//	}
	//
	//	LeaveCriticalSection(&m_ctrl_cs);
	//	LeaveCriticalSection(&m_render_cs);
	return S_OK;
}

//设置导航指针对象
//navName - 导航图对象名称
//navPointerName - 导航图指针对象名称
//objType - 导航对象的类型 0表示相机，1表示模型
//objName - 当objType为0时，objName为相机名称；当objType为1时，objName为模型名称
STDMETHODIMP CVGS2::SetNavigaterPointerObject(BSTR navName, BSTR navPointerName, INT objType, BSTR objName)
{
	// TODO: Add your implementation code here
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	DWORD navHandle = m_VGSEngine.SunGetNavigaterByName((char*)_bstr_t(navName));
	//	CNavigater* pNav = (CNavigater*)navHandle;
	//	if (pNav)
	//	{
	//		INT navPointerCnt = pNav->m_NavPointer.size();
	//		NavPointer** ppNavPointer = pNav->m_NavPointer.begin();
	//		for (INT i = 0; i< navPointerCnt; i++)
	//		{
	//			NavPointer* pNavPointer = *(ppNavPointer + i);
	//			if (strcmp(pNavPointer->Name, (char*)_bstr_t(navPointerName)) == 0)
	//			{
	//				DWORD objHandle = 0;
	//				switch (objType)
	//				{
	//				case 0:
	//					objHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(objName));	
	//					pNavPointer->NavPointerHandle = objHandle;
	//					pNavPointer->NavPointerType = objType;
	//					break;
	//				case 1:
	//					objHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(objName));	
	//					pNavPointer->NavPointerHandle = objHandle;
	//					pNavPointer->NavPointerType = objType;
	//					break;
	//				}
	//			}
	//		}
	//	}
	//
	//	LeaveCriticalSection(&m_ctrl_cs);
	//	LeaveCriticalSection(&m_render_cs);
	return S_OK;
}

//设置指定的Overlay全屏
STDMETHODIMP CVGS2::SetOverlayFullScreen(BSTR overlayName, BOOL xFull, BOOL yFull)
{
	//转换BSTR
	//	CHAR strOverlayName[32];
	//	strcpy(strOverlayName, (char*)_bstr_t(overlayName));
	//
	//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName(strOverlayName);
	//	COverlay* pOverlay = (COverlay*)overlayHandle;
	//	if (pOverlay != 0)
	//	{
	//		RECT rect;
	//		rect.left = 0; 
	//		rect.top = 0; 
	//
	//		if (xFull)
	//		{
	//			rect.right = m_VGSEngine.vgsScene->rect.right - m_VGSEngine.vgsScene->rect.left;
	//			pOverlay->m_bFullH = TRUE;
	//			//3.0f
	//			pOverlay->useRelativePos = TRUE;
	//			pOverlay->m_x1Percent = 0;
	//			pOverlay->m_x2Percent = 1;
	//		}
	//		if (yFull)
	//		{
	//			rect.bottom = m_VGSEngine.vgsScene->rect.bottom - m_VGSEngine.vgsScene->rect.top;
	//			pOverlay->m_bFullV = TRUE;
	//			//3.1f
	//			pOverlay->useRelativePos = TRUE;
	//			pOverlay->m_y1Percent = 0;
	//			pOverlay->m_y2Percent = 1;
	//		}
	//		
	//		//进入临界区
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//		m_VGSEngine.SunSetOverlayRect(overlayHandle, &rect);
	//		//进入临界区
	//		LeaveCriticalSection(&m_render_cs);
	//		LeaveCriticalSection(&m_ctrl_cs);
	//		//m_VGSEngine.SunRenderScene();
	//	}
	return S_OK;
}

//把Overaly前移一层
STDMETHODIMP CVGS2::PushOverlayFront(BSTR overlayName)
{
	//转换BSTR
	//	CHAR strOverlayName[32];
	//	strcpy(strOverlayName, (char*)_bstr_t(overlayName));
	//
	//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName(strOverlayName);
	//	if (overlayHandle != 0)
	//	{
	//		//进入临界区
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//		m_VGSEngine.PushOverlayFront(overlayHandle);
	//		//进入临界区
	//		LeaveCriticalSection(&m_render_cs);
	//		LeaveCriticalSection(&m_ctrl_cs);
	//	}

	return S_OK;
}

//把Overaly后移一层
STDMETHODIMP CVGS2::PushOverlayBack(BSTR overlayName)
{
	//转换BSTR
	//	CHAR strOverlayName[32];
	//	strcpy(strOverlayName, (char*)_bstr_t(overlayName));
	//
	//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName(strOverlayName);
	//	if (overlayHandle != 0)
	//	{
	//		//进入临界区
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//		m_VGSEngine.PushOverlayBack(overlayHandle);
	//		//进入临界区
	//		LeaveCriticalSection(&m_render_cs);
	//		LeaveCriticalSection(&m_ctrl_cs);
	//	}

	return S_OK;
}

//删除场景中的所有模型
STDMETHODIMP CVGS2::DeleteAllModel()
{
	//	INT modelCnt = m_VGSEngine.vgsScene->model.size();
	//	if (modelCnt == 0) return S_OK;
	//
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	
	//	CModel** ppModel = m_VGSEngine.vgsScene->model.begin();
	//	for (INT i = 0; i< modelCnt; i++)
	//	{
	//		CModel* pModel = *(ppModel + modelCnt - i - 1);
	//		DWORD modelHandle = (DWORD)pModel;
	//
	//		m_VGSEngine.SunDeleteModelByHandle(modelHandle);
	//		//清空临时模型变量
	//		if ((DWORD)(m_pVGS2Wnd->m_pCurCursorModel) == modelHandle)
	//			m_pVGS2Wnd->m_pCurCursorModel = NULL;
	//		//清空临时模型变量
	//		if ((DWORD)(m_pVGS2Wnd->m_pLastCursorModel) == modelHandle)
	//			m_pVGS2Wnd->m_pLastCursorModel = NULL;
	//		//清空临时模型变量
	//		if ((DWORD)(m_pVGS2Wnd->m_pLastSelectedModel) == modelHandle)
	//			m_pVGS2Wnd->m_pLastSelectedModel = NULL;
	//		//清空临时模型变量
	//		if ((DWORD)(m_pVGS2Wnd->m_pCurSelectedModel) == modelHandle)
	//			m_pVGS2Wnd->m_pCurSelectedModel = NULL;
	//	}
	//	
	//	m_VGSEngine.vgsScene->model.clear();
	//	m_VGSEngine.vgsScene->transModel.clear();
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//将关键帧动画的播放设置为播放状态
STDMETHODIMP CVGS2::PlayKeyFrame()
{
	m_VGSEngine.PlayKeyframe();
	return S_OK;
}

//将关键帧动画的播放设置为暂停状态
STDMETHODIMP CVGS2::PauseKeyFrame()
{
	//	m_VGSEngine.PauseKeyframe();
	return S_OK;
}

STDMETHODIMP CVGS2::StopKeyFrame()
{
	//	m_VGSEngine.StopKeyframe();
	return S_OK;
}

STDMETHODIMP CVGS2::SetKeyFrameRepeatCount(INT repeatCnt)
{
	//	m_VGSEngine.SetKfrRepeatCount(repeatCnt);
	return S_OK;
}


STDMETHODIMP CVGS2::GotoFrame(INT frame)
{	
	//	m_VGSEngine.GotoFrame(frame);
	return S_OK;
}

STDMETHODIMP CVGS2::SetKeyFramePlaySpeed(INT fps)
{
	//	m_VGSEngine.SetKfrPlaySpeed(fps);

	return S_OK;
}

//创建全屏光晕
STDMETHODIMP CVGS2::CreateFSGlow(FLOAT BloomValue, FLOAT BlurValue)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	//创建动态模糊特效控制对象
	//	if (m_VGSEngine.CreateFSGlow())
	//		m_VGSEngine.SetFSGlowValue(BloomValue, BlurValue);
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//设置全屏光晕的强度
STDMETHODIMP CVGS2::SetFSGlowValue(FLOAT BloomValue, FLOAT BlurValue)
{
	//	m_VGSEngine.SetFSGlowValue(BloomValue, BlurValue);
	return S_OK;
}

//启动全屏光晕
STDMETHODIMP CVGS2::StartFSGlow()
{
	//	m_VGSEngine.OpenFSGlow();
	return S_OK;
}

//关闭全屏光晕
STDMETHODIMP CVGS2::EndFSGlow()
{
	//	m_VGSEngine.CloseFSGlow();
	return S_OK;
}

//设置材质的高光色
STDMETHODIMP CVGS2::SetMtrlSpecular(BSTR mtrlName, INT red, INT green, INT blue)
{
	//	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	//	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	//	if (pMtrl)
	//	{
	//		pMtrl->specular.r = red/255.0f;
	//		pMtrl->specular.g = green/255.0f;
	//		pMtrl->specular.b = blue/255.0f;		
	//		pMtrl->specular.a = 1.0f;
	//		pMtrl->d3dmtrl.Specular = pMtrl->specularLevel * pMtrl->specular;
	//		pMtrl->d3dmtrl.Specular.a = 1.0f;
	//	}
	return S_OK;
}

//设置材质的环境色
STDMETHODIMP CVGS2::SetMtrlAmbient(BSTR mtrlName, INT red, INT green, INT blue)
{
	_VGS_RETURN
	
	DWORD mtrHandle = m_VGSEngine.SunGetMaterialHandleByName(_BSTR_TO_STR(mtrlName).c_str());
	MtrlInfo info;
	m_VGSEngine.SunGetMtrlInfo(mtrHandle, &info);
	info.Ambient = VGSRGB(red, green, blue);
	m_VGSEngine.SunSetMtrlInfo(mtrHandle, &info);
	//	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	//	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	//	if (pMtrl)
	//	{
	//		pMtrl->d3dmtrl.Ambient.a = 1.0f;
	//		pMtrl->d3dmtrl.Ambient.r = red/255.0f;
	//		pMtrl->d3dmtrl.Ambient.g = green/255.0f;
	//		pMtrl->d3dmtrl.Ambient.b = blue/255.0f;
	//	}
	return S_OK;
}

//设置高光的强度
STDMETHODIMP CVGS2::SetMtrlSpecularLevel(BSTR mtrlName, INT level)
{
	_VGS_RETURN
		DWORD mtrHandle = m_VGSEngine.SunGetMaterialHandleByName(_BSTR_TO_STR(mtrlName).c_str());
	MtrlInfo info;
	m_VGSEngine.SunGetMtrlInfo(mtrHandle, &info);
	info.SpecularLevel = level;
	m_VGSEngine.SunSetMtrlInfo(mtrHandle, &info);
	//	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	//	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	//	if (pMtrl)
	//	{
	//		pMtrl->specularLevel = level/100.0f;
	//		pMtrl->d3dmtrl.Specular = pMtrl->specularLevel * pMtrl->specular;
	//		pMtrl->d3dmtrl.Specular.a = 1.0f;
	//	}
	return S_OK;
}

//设置高光的锐度
STDMETHODIMP CVGS2::SetMtrlSpecularPower(BSTR mtrlName, INT power)
{
	_VGS_RETURN
		DWORD mtrHandle = m_VGSEngine.SunGetMaterialHandleByName(_BSTR_TO_STR(mtrlName).c_str());
	MtrlInfo info;
	m_VGSEngine.SunGetMtrlInfo(mtrHandle, &info);
	info.shininess = power;
	m_VGSEngine.SunSetMtrlInfo(mtrHandle, &info);
	//	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	//	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	//	if (pMtrl)
	//		pMtrl->d3dmtrl.Power = power;	//高光锐度
	return S_OK;
}

//手动设置关键帧动画的播放，从StartFrame播放到EndFrame
STDMETHODIMP CVGS2::PlayKeyFrame1(INT startFrame, INT endFrame)
{
	//	m_VGSEngine.vgsScene->m_bKfrPlaying = TRUE;
	//
	//	if (startFrame >= 0 && startFrame <= m_VGSEngine.vgsScene->m_KfrCnt -1)
	//	{
	//		m_VGSEngine.vgsScene->m_StartFrame = startFrame;
	//		m_VGSEngine.vgsScene->m_CurrentFrame = startFrame;
	//		m_VGSEngine.vgsScene->m_KfrRepeatIndex = 0;
	//	}
	//	else
	//		m_VGSEngine.vgsScene->m_StartFrame = 0;
	//
	//	
	//	if (startFrame >= 0 && startFrame <= m_VGSEngine.vgsScene->m_KfrCnt -1)
	//		m_VGSEngine.vgsScene->m_EndFrame = endFrame;
	//	else
	//		m_VGSEngine.vgsScene->m_EndFrame = m_VGSEngine.vgsScene->m_KfrCnt -1;

	return S_OK;
}

//设置相机关键帧动画的播放状态
STDMETHODIMP CVGS2::SetCameraKfrPlayStatus(BSTR camName, BOOL status)
{
	//	DWORD camHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(camName));
	//	CCamera* pCamera = (CCamera*)camHandle;
	//	if (pCamera)
	//	{
	//		pCamera->playKfr = status;
	//	}
	return S_OK;
}

//设置所有模型的可见性
STDMETHODIMP CVGS2::SetAllModelVisibility(BOOL status)
{
	//	if (m_VGSEngine.vgsScene == NULL)
	//		return S_OK;
	//
	//	INT modelCnt = m_VGSEngine.vgsScene->model.size();
	//	CModel** ppModel = m_VGSEngine.vgsScene->model.begin();
	//	for (INT i = 0; i< modelCnt; i++)
	//	{
	//		CModel* pModel = *(ppModel + i);
	//		if((INT)status ==0)
	//			pModel->visible =FALSE;
	//		else
	//			pModel->visible =TRUE;
	//	}

	return S_OK;
}

//设置所有相机的关键帧动画播放状态
STDMETHODIMP CVGS2::SetAllCameraKfrPlayStatus(BOOL status)
{
	//	if (m_VGSEngine.vgsScene == NULL)
	//		return S_OK;
	//
	//	INT cameraCnt = m_VGSEngine.vgsScene->camera.size();
	//	CCamera** ppCamera = m_VGSEngine.vgsScene->camera.begin();
	//	for (INT i = 0; i< cameraCnt; i++)
	//	{
	//		CCamera* pCam = *(ppCamera + i);
	//		pCam->playKfr = status;
	//	}

	return S_OK;
}

//设置相机关键帧动画的重复播放次数
STDMETHODIMP CVGS2::SetCameraKfrRepeatCnt(BSTR camName, INT repeatCnt)
{
	//	if (m_VGSEngine.vgsScene == NULL)
	//		return S_OK;
	//	
	//	DWORD camHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(camName));
	//	CCamera* pCamera = (CCamera*)camHandle;
	//
	//	if (pCamera)
	//		pCamera->kfrRepeatCnt = repeatCnt;

	return S_OK;
}

//设置所有相机动画的重复播放次数
STDMETHODIMP CVGS2::SetAllCameraKfrRepeatCnt(INT repeatCnt)
{
	//	if (m_VGSEngine.vgsScene == NULL)
	//		return S_OK;
	//
	//	INT cameraCnt = m_VGSEngine.vgsScene->camera.size();
	//	CCamera** ppCamera = m_VGSEngine.vgsScene->camera.begin();
	//	for (INT i = 0; i< cameraCnt; i++)
	//	{
	//		CCamera* pCam = *(ppCamera + i);
	//		pCam->kfrRepeatCnt = repeatCnt;
	//	}


	return S_OK;
}
//设置动态贴图播放的速度
STDMETHODIMP CVGS2::SetDynamicTexPlaySpeed(BSTR UseDyTexMtrlName,INT texLayerIdx, INT fps)
{

	//	if (m_VGSEngine.vgsScene == NULL)
	//		return S_OK;
	//	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(UseDyTexMtrlName));
	//	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	//	if (pMtrl)
	//	{
	//		TextureLayer* pTexLayer = pMtrl->texLayersList[texLayerIdx];
	//		CDynamicTex* pDyTex = (CDynamicTex*) pTexLayer->pTexture;
	//		if(pDyTex)
	//		{
	//			pDyTex->SetFPS(fps);
	//			return S_OK;
	//		}
	//		
	//	}
	return S_OK;
}
/*//设置动态贴图播放的速度
STDMETHODIMP CVGS2::SetDynamicTexPlaySpeed(BSTR dynamicTexName, INT fps)
{

if (m_VGSEngine.vgsScene == NULL)
return S_OK;

INT dyTexCnt = m_VGSEngine.vgsScene->dynamicTexture.size();
CDynamicTex** ppDyTex = m_VGSEngine.vgsScene->dynamicTexture.begin();
for (INT i = 0; i< dyTexCnt; i++)
{
CDynamicTex* pDyTex = *(ppDyTex + i);
if (strcmp(pDyTex->name, (char*)_bstr_t(dynamicTexName)) == 0)
{
pDyTex->SetFPS(fps);
return S_OK;
}
}
return S_OK;
}*/


//通过模型的Pivot值设置模型的位置
STDMETHODIMP CVGS2::SetModelPositionByPivot(BSTR modelName, FLOAT x, FLOAT y, FLOAT z)
{
	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	/*	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	CModel* pModel = (CModel*)modelHandle;

	if (pModel)
	pModel->SetPositionByPivot(&D3DXVECTOR3(x, y, z));
	*/
	//-------------------------------------------------------------------------------------
	return S_OK;
}

//设置模型的动态UV状态，当设置为False时，自动恢复原始UV设置
STDMETHODIMP CVGS2::SetDynamicUVStatus(BSTR modelName, BOOL status)
{
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	CModel* pModel = (CModel*)modelHandle;
	//
	//	if (pModel)
	//	{
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//
	//		m_VGSEngine.SunSetDynamicUVStatus(modelHandle, status);
	//
	//		LeaveCriticalSection(&m_ctrl_cs);
	//		LeaveCriticalSection(&m_render_cs);
	//	}
	return S_OK;
}
//Add by Baojiahui 2007/9/24-----------------------------------------------------------
//设置overlay的材质，以实现动态更换overlay的图像以及动态哦overlay的效果
STDMETHODIMP CVGS2::SetOverlayMtrl(BSTR overlayName, BSTR mtrlName)
{
	// TODO: Add your implementation code here
	//渲染队列
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//
	//	//转换BSTR
	//	CHAR strOverlayName[32];
	//	strcpy(strOverlayName, (char*)_bstr_t(overlayName));
	//	DWORD overlayHandle = m_VGSEngine.SunGetOverlayHandleByName(strOverlayName);
	//	DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	//	//设置渲染队列中的模型的材质
	//	if(overlayHandle && mtrlHandle)
	//	{   
	//		COverlay* pOverlay = (COverlay*)overlayHandle;
	//	    pOverlay->SetMaterial((CMaterial*)mtrlHandle);	
	//	}

	return S_OK;
}
//--------------------------------------------------------------------------------------

//设置触发地面模型回调函数, 当地面模型被碰撞侦测到时，调用指定的JS函数
//modelName, model的名称
//funcName, JS回调函数名称
STDMETHODIMP CVGS2::SetZoneTriggerModelCallBackFunc(BSTR modelName, BSTR funcName)
{
	//    DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	INT funcStrLen = _bstr_t(funcName).length();
	//	BOOL resetBackFunc = false;
	//	INT triggerModelCnt = m_VGSEngine.vgsScene->triggerModel.size();
	//	TriggerCallBack** pptriggerModel = m_VGSEngine.vgsScene->triggerModel.begin();
	//	for (INT i = 0; i< triggerModelCnt; i++)
	//	{
	//		TriggerCallBack* t_tModel = *(pptriggerModel + i);
	//		if (t_tModel->triggerHandle==modelHandle)
	//		{ 
	//		    SAFE_DELETE_ARRAY(t_tModel->callBackFunc);
	//            t_tModel->callBackFunc= new CHAR[funcStrLen+1];
	//			ZeroMemory(t_tModel->callBackFunc, sizeof(CHAR)*(funcStrLen+1));
	//		    strcpy(t_tModel->callBackFunc, (char*)_bstr_t(funcName));
	//            resetBackFunc = true;
	//			break;
	//		}
	//
	//	}
	//	if(resetBackFunc==false)
	//	{
	//	   TriggerCallBack* trigModel= new TriggerCallBack[1];
	//       trigModel->triggerHandle=modelHandle;
	//       trigModel->callBackFunc = new CHAR[funcStrLen+1];
	//	   ZeroMemory(trigModel->callBackFunc, sizeof(CHAR)*(funcStrLen+1));
	//	   strcpy(trigModel->callBackFunc, (char*)_bstr_t(funcName));
	//
	//	   m_VGSEngine.vgsScene->triggerModel.push_back(trigModel);
	//
	//	}
	return S_OK;
}
//--------------------------------------------------------------------------------------

//设置alphaTest的参数
//INT level级别
STDMETHODIMP CVGS2::SetAlphaTestLevel(INT level)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	m_VGSEngine.vgsScene->alphaTestLevel= (DWORD)level;
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}
//--------------------------------------------------------------------------------------

//设置button的透明度
//buttonName button的名称
//opacity 透明度值
STDMETHODIMP CVGS2::SetButtonOpacity(BSTR buttonName, INT opacity)
{
	// TODO: Add your implementation code here
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//	
	//	DWORD btnHandle = m_VGSEngine.SunGetButtonByName((char*)_bstr_t(buttonName));
	//	CButton* pButton = (CButton*)btnHandle;
	//
	//	if (pButton)
	//	{
	//		if(pButton->btnOverlay)
	//		{
	//			DWORD overlayHandle = (DWORD)pButton->btnOverlay;
	//			if (overlayHandle != 0)
	//			{
	//				//进入临界区
	//				EnterCriticalSection(&m_ctrl_cs);
	//				EnterCriticalSection(&m_render_cs);
	//				m_VGSEngine.SunSetOverlayOpacity(overlayHandle, opacity);
	//				//Quit临界区
	//				LeaveCriticalSection(&m_render_cs);
	//				LeaveCriticalSection(&m_ctrl_cs);
	//			}
	//		}
	//	}
	return S_OK;
}

STDMETHODIMP CVGS2::CurCamGotoFrame(INT frame)
{
	// TODO: Add your implementation code here
	//    m_VGSEngine.vgsScene->currentCam->kfrCurFrame=frame;
	return S_OK;
}

STDMETHODIMP CVGS2::SetSceneEditMode(INT EditMode)
{
	// TODO: Add your implementation code here
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//    //0--None模式, 1--Move模式移动x轴,2--Move模式移动y轴，3--Move模式移动z轴,4--Move模式移动xy平面,5--Move模式移动yz平面,6--Move模式移动zx平面,7--Rotate模式x轴旋转,8--Rotate模式y轴旋转,9--Rotate模式z轴旋转,10--Scale模式x轴缩放,11--Scale模式y轴缩放,12--Scale模式z轴缩放
	//    m_VGSEngine.vgsScene->sceneEditMode = EditMode;
	return S_OK;
}

STDMETHODIMP CVGS2::CloneModel(BSTR oriModelName, BSTR desModelName, INT ID1,INT ID2,FLOAT posX, FLOAT posY, FLOAT posZ)
{
	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	/*// TODO: Add your implementation code here
	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//get the ori Model's handle
	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(oriModelName));
	CModel* pOriModel = (CModel*)modelHandle;

	EnterCriticalSection(&m_ctrl_cs);
	EnterCriticalSection(&m_render_cs);

	INT modelCnt =m_VGSEngine.vgsScene->model.size();

	//Build the new Model ---------------------------------------------------------------------
	CModel* pModel = new CModel();
	pModel->index = modelCnt;

	//name
	strcpy(pModel->name, (char*)_bstr_t(desModelName));

	//transform
	pModel->pNode->curTransform=pOriModel->pNode->curTransform;

	pModel->pNode->worldPosition =pOriModel->pNode->worldPosition;
	//pivot
	//pModel->pivot = pOriModel->pivot;

	//读入模型的ID1
	pModel->pNode->ID1 = ID1;

	//读入模型的ID2
	pModel->pNode->ID2 = ID2;

	//reference
	pModel->refModelIndex = pOriModel->refModelIndex;
	//如果是索引模型，则后面的数据不读了
	if (pModel->refModelIndex == -1)
	{
	//bounding box
	pModel->oriBBox.MinCorner = pOriModel->oriBBox.MinCorner;
	pModel->oriBBox.MaxCorner = pOriModel->oriBBox.MaxCorner;

	//Bounding Sphere
	pModel->oriBSphere.Center = pOriModel->oriBSphere.Center;

	pModel->oriBSphere.Radius = pOriModel->oriBSphere.Radius;


	//update matrix
	pModel->UpdateScaleMatrix();
	pModel->UpdateRotateMatrix();
	pModel->UpdateTranslateMatrix();
	pModel->MultiplyMatrix();
	pModel->transform.matrix = pOriModel->transform.matrix;
	pModel->transform.ori_matrix = pOriModel->transform.ori_matrix;
	//update other
	pModel->UpdateOther();
	pModel->visible = TRUE;



	//model RenderBoundBox
	pModel->pRBBox= new CRenderBoundBox(m_VGSEngine.vgsScene->m_pd3dDevice);
	pModel->pRBBox->SetRBBoxVertex(pModel->oriBBox.MinCorner,pModel->oriBBox.MaxCorner);
	//把模型的位置变换给RBBox，
	pModel->pRBBox->SetRotationMatrix(&pModel->transform.rotation_matrix);
	pModel->pRBBox->SetTranslateMatrix(&pModel->transform.translate_matrix);
	pModel->pRBBox->SetScaleMatrix(&pModel->transform.scale_matrix);
	pModel->pRBBox->MultiplyMatrix();
	//pModel->showRBBox = false;


	//material ID
	pModel->shader = pOriModel->shader;

	//vertex offset

	//vertex count
	pModel->vertexCnt = pOriModel->vertexCnt;

	//如果不是索引模型，则读入顶点，如果是索引模型，就不读入顶点了，这样节省了内存空间
	if (pModel->refModelIndex == -1)
	{
	}

	//模型的顶点颜色变量
	pModel->oriColor = pOriModel->oriColor;

	//pModel->color = pModel->color;
	pModel->mouseOverColor = pModel->oriColor;
	pModel->mouseClickedColor = pModel->oriColor;
	pModel->mouseUpColor = pModel->oriColor;

	//face count


	pModel->faceCnt = pOriModel->faceCnt;	

	//keyframe
	//变换矩阵转化为相对上一帧的变换矩阵//--------------------------
	D3DXMATRIX mat;
	D3DXMatrixInverse(&mat, NULL, &(pModel->transform.matrix));

	pModel->kfrCnt = pOriModel->kfrCnt;



	if (pModel->kfrCnt > 0) 
	{

	pModel->matrix_kfr = new D3DXMATRIX[pModel->kfrCnt];
	//keyframe Data
	for (UINT k = 0; k< pOriModel->kfrCnt; k++)
	{
	//frame data
	pModel->matrix_kfr[k] = pOriModel->matrix_kfr[k];

	//计算差值变换矩阵
	D3DXMatrixMultiply(&(pModel->matrix_CurFrame),	
	&mat,
	&pModel->matrix_CurFrame); 

	}

	}

	//vkeyframe
	pModel ->vkfrCnt = pOriModel ->vkfrCnt;
	if (pModel->vkfrCnt > 0)
	{

	pModel->vertices_kfr = new CVertex*[pModel->vkfrCnt];
	//vkeyframeData
	for (UINT k = 0; k< pOriModel->vkfrCnt; k++)
	{

	pModel->vertices_kfr[k] = new CVertex[pOriModel->vertexCnt];

	for (UINT z=0; z< pOriModel->vertexCnt; z++)
	{
	pModel->vertices_kfr[k][z].position = pOriModel->vertices_kfr[k][z].position;
	}
	}	
	}

	//确定当前模型所使用的材质，确定模型的FVF
	pModel->m_FVF = pModel->shader->m_FVF;
	pModel->m_FVFSize = pModel->shader->m_FVFSize;

	//确定模型是否是Billboard模型
	pModel->isBillboard = pOriModel->isBillboard;

	//确定模型是否是Collision模型
	pModel->isCollision = pOriModel->isCollision;

	//确定模型是否是Ground模型
	pModel->isGround = pOriModel->isGround;

	//确定模型是否是SkyBox
	pModel->isSkyBox = pOriModel->isSkyBox;

	//确定模型是否可见
	pModel->visible = pOriModel->visible;

	//确定模型是否是Clickable
	pModel->clickable = pOriModel->clickable;

	//确定模型是否使用动态UV
	pModel->bDynamicUV = pOriModel->bDynamicUV;

	//U Step
	pModel->UStep = pOriModel->UStep;

	//确定模型是否使用动态UV
	pModel->VStep = pOriModel->VStep;

	//clone vBuffer
	//m_VGSEngine.vgsScene->CreateModelVBuffer(pModel);
	m_VGSEngine.vgsScene->CloneModelVBuffer(pModel,pOriModel);
	//clone IBuffer
	//m_VGSEngine.vgsScene->CreateModelIBuffer(pModel);
	m_VGSEngine.vgsScene->CloneModelIBuffer(pModel,pOriModel);
	}


	//将模型放到对应的材质modelList
	pModel->shader->modelList.push_back((DWORD)pModel);

	//push model
	//m_VGSEngine.vgsScene->model.push_back(pModel);
	m_VGSEngine.vgsScene->AddModel(pModel);

	//push model - transparent model半透明模型
	if (pModel->shader->bAlphaBlend)
	m_VGSEngine.vgsScene->transModel.push_back(pModel);

	D3DXVECTOR3 mPos=D3DXVECTOR3(posX,posY,posZ);
	pModel->SetPosition(&mPos);

	m_pVGS2Wnd->SortTheModelList();
	//-----------------------------------------------------------------------------------------

	LeaveCriticalSection(&m_render_cs);
	LeaveCriticalSection(&m_ctrl_cs);
	*/
	//------------------------------------------------------------------------------------------
	return S_OK;
}




STDMETHODIMP CVGS2::get_theModelPosX(float *pVal)
{
	// TODO: Add your implementation code here
	//    CModel* pModel = (CModel*)m_theModelHandle;
	//    if(pModel)
	//		*pVal=pModel->pNode->worldPosition.x;

	return S_OK;
}

STDMETHODIMP CVGS2::get_theModelPosY(float *pVal)
{
	// TODO: Add your implementation code here
	//    CModel* pModel = (CModel*)m_theModelHandle;
	//    if(pModel)
	//		*pVal=pModel->pNode->worldPosition.y;

	return S_OK;
}

STDMETHODIMP CVGS2::get_theModelPosZ(float *pVal)
{
	// TODO: Add your implementation code here
	//    CModel* pModel = (CModel*)m_theModelHandle;
	//    if(pModel)
	//		*pVal=pModel->pNode->worldPosition.z;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theModelScreenPosX(short *pVal)
{
	// TODO: Add your implementation code here
	//    CModel* pModel = (CModel*)m_theModelHandle;
	//    INT screenX,screenY;
	//	if(pModel)
	//	{
	//	   m_VGSEngine.vgsScene->CalcScreenCoords(&(pModel->pNode->worldPosition),&screenX,&screenY);
	//	
	//	}
	//	*pVal =short(screenX);
	return S_OK;
}

STDMETHODIMP CVGS2::get_theModelScreenPosY(short *pVal)
{
	// TODO: Add your implementation code here
	//    CModel* pModel = (CModel*)m_theModelHandle;
	//    INT screenX,screenY;
	//	if(pModel)
	//	{
	//	   m_VGSEngine.vgsScene->CalcScreenCoords(&(pModel->pNode->worldPosition),&screenX,&screenY);
	//	
	//	}
	//	*pVal =short(screenY);
	return S_OK;
}

STDMETHODIMP CVGS2::GetInfoFromModel(BSTR modelName)
{
	// TODO: Add your implementation code here
	//    DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	if(modelHandle) 
	//	    m_theModelHandle=modelHandle;
	return S_OK;
}

STDMETHODIMP CVGS2::CreateNewText(BSTR textString)
{
	// TODO: Add your implementation code here
	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//		
	//    DWORD texHandle= m_VGSEngine.SunNewText((char*)_bstr_t(textString));
	//	if(texHandle)
	//	    m_theTextHandle=texHandle;
	//	
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

STDMETHODIMP CVGS2::get_theTextHandle(long *pVal)
{
	// TODO: Add your implementation code here
	//    *pVal=(long)m_theTextHandle;
	return S_OK;
}

STDMETHODIMP CVGS2::SetTextPosition(long textHandle, UINT x, UINT y)
{
	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	// TODO: Add your implementation code here
	//    m_VGSEngine.SunSetTextPosition((DWORD)textHandle,x,y);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::SetTextColor(long textHandle, INT red, INT green, INT blue,INT alpha)
{
	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	// TODO: Add your implementation code here
	//    m_VGSEngine.SunSetTextColor((DWORD)textHandle,D3DCOLOR_RGBA(red, green, blue, alpha));
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::DeleteText(long textHandle)
{
	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	// TODO: Add your implementation code here
	//    m_VGSEngine.SunDelText(textHandle);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::GetInfoFromCamera(BSTR cameraName)
{
	// TODO: Add your implementation code here
	//    DWORD cameraHandle = m_VGSEngine.SunGetCameraHandleByName((char*)_bstr_t(cameraName));
	//	if(cameraHandle) 
	//	    m_theCamHandle=cameraHandle;
	return S_OK;
}




STDMETHODIMP CVGS2::get_theCamFromPosX(float *pVal)
{
	// TODO: Add your implementation code here
	//    CCamera* pCamera=(CCamera*)m_theCamHandle;
	//	D3DXVECTOR3 pCamFrom;
	//	pCamFrom= pCamera->GetFrom();
	//	*pVal= pCamFrom.x;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theCamFromPosY(float *pVal)
{
	// TODO: Add your implementation code here
	//     CCamera* pCamera=(CCamera*)m_theCamHandle;
	//	D3DXVECTOR3 pCamFrom;
	//	pCamFrom= pCamera->GetFrom();
	//	*pVal= pCamFrom.y;   
	return S_OK;
}

STDMETHODIMP CVGS2::get_theCamFromPosZ(float *pVal)
{
	// TODO: Add your implementation code here
	//    CCamera* pCamera=(CCamera*)m_theCamHandle;
	//	D3DXVECTOR3 pCamFrom;
	//	pCamFrom= pCamera->GetFrom();
	//	*pVal= pCamFrom.z;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theCamAtPosX(float *pVal)
{
	// TODO: Add your implementation code here
	//    CCamera* pCamera=(CCamera*)m_theCamHandle;
	//	D3DXVECTOR3 pCamAt;
	//	pCamAt= pCamera->GetAt();
	//	*pVal= pCamAt.x;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theCamAtPosY(float *pVal)
{
	// TODO: Add your implementation code here
	//    CCamera* pCamera=(CCamera*)m_theCamHandle;
	//	D3DXVECTOR3 pCamAt;
	//	pCamAt= pCamera->GetAt();
	//	*pVal= pCamAt.y;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theCamAtPosZ(float *pVal)
{
	// TODO: Add your implementation code here
	//    CCamera* pCamera=(CCamera*)m_theCamHandle;
	//	D3DXVECTOR3 pCamAt;
	//	pCamAt= pCamera->GetAt();
	//	*pVal= pCamAt.z;
	return S_OK;
}

STDMETHODIMP CVGS2::SetTextString(long textHandle,BSTR textString)
{
	// TODO: Add your implementation code here
	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	// TODO: Add your implementation code here
	//    m_VGSEngine.SunSetTextString((DWORD)textHandle,(char*)_bstr_t(textString));
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

//加载一个子场景到指定父场景
STDMETHODIMP CVGS2::LoadScene(BSTR SceneName, BSTR ParentSceneName, BSTR N3DFileURL, 
							  FLOAT m11, FLOAT m12, FLOAT m13, FLOAT m14, 
							  FLOAT m21, FLOAT m22, FLOAT m23, FLOAT m24, 
							  FLOAT m31, FLOAT m32, FLOAT m33, FLOAT m34, 
							  FLOAT m41, FLOAT m42, FLOAT m43, FLOAT m44)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//	if (m_VGSEngine.vgsScene == NULL) return E_FAIL;
	//	if (m_hInternetSession == NULL) return E_FAIL;
	//
	//	//转换BSTR
	//	char pN3DFileName[512];
	//	strcpy(pN3DFileName, (char*)_bstr_t(N3DFileURL));
	//
	//	//场景的空间变换(位置，转角，缩放)
	//	D3DXMATRIX transform;
	//	transform._11 = m11;	transform._12 = m12;	transform._13 = m13;	transform._14 = m14;
	//	transform._21 = m21;	transform._22 = m22;	transform._23 = m23;	transform._24 = m24;
	//	transform._31 = m31;	transform._32 = m32;	transform._33 = m33;	transform._34 = m34;
	//	transform._41 = m41;	transform._42 = m42;	transform._43 = m43;	transform._44 = m44;
	//
	//	//创建管理场景资源的对象
	//	CSceneResource* pSceneResource = m_VGSEngine.CreateSceneResource((char*)_bstr_t(SceneName), &transform);
	//
	//	//将Load的N3D文件放进队列
	//	CDownLoadN3DFile* pN3D = new CDownLoadN3DFile(m_VGSEngine.vgsScene,
	//												  &m_ctrl_cs,
	//												  &m_render_cs,
	//												  m_hInternetSession);
	//
	//	strcpy(pN3D->name, (char*)_bstr_t(SceneName));
	//	strcpy(pN3D->file, pN3DFileName);
	//	pN3D->type = 0;  //下载并直接加载到场景模式
	//	pN3D->index = m_pVGS2Wnd->m_LoadN3DObj.size();
	//	pN3D->m_pSceneResource = pSceneResource;
	//	m_pVGS2Wnd->m_LoadN3DObj.push_back(pN3D);
	//
	//	
	//	//----------------------------------------------------------
	//	//是远程文件
	//	//----------------------------------------------------------
	//	if (pN3DFileName[0] == 'h' && pN3DFileName[1] == 't' && pN3DFileName[2] == 't' && pN3DFileName[3] == 'p')
	//	{
	//		m_IsWebFile = TRUE;
	//		//判断前一个N3D是否已经下载完
	//		if (pN3D->index > 0)  //没有下载完
	//		{
	//			INT preIndex = pN3D->index - 1;
	//			CDownLoadN3DFile** ppN3D = m_pVGS2Wnd->m_LoadN3DObj.begin();
	//			CDownLoadN3DFile* _pN3D = *(ppN3D + preIndex);
	//			if (!_pN3D->m_bAllDown)
	//				return S_OK;
	//		}
	//			
	//		//在后台下载N3D文件
	//		if (pN3D->DownLoadHttpFile(pN3DFileName, NULL, m_pVGS2Wnd->m_hWnd, pSceneResource) == false)
	//			m_VGSEngine.DeleteSceneResource(pSceneResource);
	//	}


	return S_OK;
}

//删除指定名称的场景对象
STDMETHODIMP CVGS2::DeleteScene(BSTR SceneName)
{
	_VGS_RETURN
		m_VGSEngine.SunDeleteScene();
	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	//如果当前场景还在下载的过程中，则先关闭下载线程，并释放下载对象
	//	int N3DCnt = m_pVGS2Wnd->m_LoadN3DObj.size();
	//	CDownLoadN3DFile** ppN3D = m_pVGS2Wnd->m_LoadN3DObj.begin();
	//	for (INT i = 0; i< N3DCnt; i++)
	//	{
	//		CDownLoadN3DFile* pN3D = *(ppN3D + i);
	//		if (strcmp(pN3D->name, (char*)_bstr_t(SceneName)) == 0)
	//		{
	//			pN3D->m_bTerminated = TRUE;
	//			WaitForSingleObject(pN3D->m_threadHandle, INFINITE);
	//			SAFE_RELEASE(pN3D);
	//			SAFE_DELETE(pN3D);
	//			break;
	//		}
	//	}
	//	LeaveCriticalSection(&m_ctrl_cs);
	//
	//  	//进入临界区
	//	EnterCriticalSection(&m_render_cs);
	//
	//    CSceneResource* pSceneResource = m_VGSEngine.GetSceneResource(std::string((char*)_bstr_t(SceneName)));
	//	
	//	m_VGSEngine.DeleteSceneResource(pSceneResource);
	//    
	//	//清空临时模型变量
	//	m_pVGS2Wnd->m_pCurCursorModel = NULL;
	//	m_pVGS2Wnd->m_pLastCursorModel = NULL;
	//	m_pVGS2Wnd->m_pLastSelectedModel = NULL;
	//	m_pVGS2Wnd->m_pCurSelectedModel = NULL;
	//
	//	LeaveCriticalSection(&m_render_cs);

	return S_OK;
}

//克隆场景
STDMETHODIMP CVGS2::CloneScene(BSTR destSceneName, BSTR srcSceneName, BSTR ParentSceneName, 
							   FLOAT m11, FLOAT m12, FLOAT m13, FLOAT m14, 
							   FLOAT m21, FLOAT m22, FLOAT m23, FLOAT m24, 
							   FLOAT m31, FLOAT m32, FLOAT m33, FLOAT m34, 
							   FLOAT m41, FLOAT m42, FLOAT m43, FLOAT m44)
{
	//场景的空间变换(位置，转角，缩放)
	//	D3DXMATRIX transform;
	//	transform._11 = m11;	transform._12 = m12;	transform._13 = m13;	transform._14 = m14;
	//	transform._21 = m21;	transform._22 = m22;	transform._23 = m23;	transform._24 = m24;
	//	transform._31 = m31;	transform._32 = m32;	transform._33 = m33;	transform._34 = m34;
	//	transform._41 = m41;	transform._42 = m42;	transform._43 = m43;	transform._44 = m44;
	//
	//	//进入临界区
	//	EnterCriticalSection(&m_render_cs);
	//	m_VGSEngine.CloneScene(std::string((char*)_bstr_t(destSceneName)), 
	//								  std::string((char*)_bstr_t(srcSceneName)), 
	//								  std::string((char*)_bstr_t(ParentSceneName)),
	//							      &transform);
	//	LeaveCriticalSection(&m_render_cs);

	return S_OK;
}

//------------------------------------------------------------------------------------------
//创建实时反射贴图
//------------------------------------------------------------------------------------------
STDMETHODIMP CVGS2::CreateRTFTexture(BSTR TextureName, BSTR refPlaneName, INT height, INT width, FLOAT power)
{			
	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//	//创建实时反射贴图对象
	//	m_VGSEngine.CreateRTFTexture((char*)_bstr_t(TextureName), (char*)_bstr_t(refPlaneName), height, width, power);
	//	
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//为实时反射贴图增加反射模型 AddModelToRTFTexture
STDMETHODIMP CVGS2::AddModelToRTFTexture(BSTR RTFTextureName, BSTR ModelName)
{
	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	
	//	m_VGSEngine.AddModelToRTFTexture((char*)_bstr_t(RTFTextureName), (char*)_bstr_t(ModelName));
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//创建动态的CubeMap
STDMETHODIMP CVGS2::CreateDynamicCubeMap(BSTR cubeMapame, BSTR centerModelName, INT size, INT power)
{
	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//	//创建动态贴图
	//	DWORD dynamicCubeMapHandle = m_VGSEngine.CreateDynamicCubeMap((char*)_bstr_t(cubeMapame), 
	//																		 (char*)_bstr_t(centerModelName),
	//																		 size, power);
	//	
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	//	
	//	CCubeMap* pCubeMap = (CCubeMap*)dynamicCubeMapHandle;
	//	if (pCubeMap)
	//		m_VGSEngine.vgsScene->cubeMap.push_back(pCubeMap);

	return S_OK;
}

//将模型加入到CubeMap的模型反射列表中
STDMETHODIMP CVGS2::AddDynamicCubeMapReflectionModel(BSTR cubeMapName, BSTR modelName)
{
	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	DWORD cubeMapHandle = m_VGSEngine.GetCubeMapHandleByName((char*)_bstr_t(cubeMapName));
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	m_VGSEngine.AddDyCubeMapReflectionModel(cubeMapHandle, modelHandle);
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

//将模型从CubeMap的模型反射列表中删除
STDMETHODIMP CVGS2::DeleteDynamicCubeMapReflectionModel(BSTR cubeMapName, BSTR modelName)
{
	//进入临界区
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	DWORD cubeMapHandle = m_VGSEngine.GetCubeMapHandleByName((char*)_bstr_t(cubeMapName));
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(modelName));
	//	m_VGSEngine.DeleteDyCubeMapReflectionModel(cubeMapHandle, modelHandle);
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

//创建真实的水面
STDMETHODIMP CVGS2::CreateRealWater(BSTR name, FLOAT x, FLOAT y, FLOAT z, INT gridX, INT gridY)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	
	//	m_VGSEngine.CreateRealWater((char*)_bstr_t(name), x, y, z, gridX, gridY);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

//增加水面的折射模型
STDMETHODIMP CVGS2::AddRealWaterRTTModel(BSTR RealWaterName, BSTR ModelName)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(ModelName));
	//	DWORD waterHandle = m_VGSEngine.GetRealWaterHandleByName((char*)_bstr_t(RealWaterName));
	//	m_VGSEngine.AddRealWaterRTTModel(waterHandle, modelHandle);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//增加水面的反射模型
STDMETHODIMP CVGS2::AddRealWaterRTFModel(BSTR RealWaterName, BSTR ModelName)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	
	//	DWORD modelHandle = m_VGSEngine.SunGetModelHandleByName((char*)_bstr_t(ModelName));
	//	DWORD waterHandle = m_VGSEngine.GetRealWaterHandleByName((char*)_bstr_t(RealWaterName));
	//	m_VGSEngine.AddRealWaterRTFModel(waterHandle, modelHandle);
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

//设置水面的CubeMap反射图
STDMETHODIMP CVGS2::SetRealWaterCubeMap(BSTR RealWaterName, BSTR CubeMapName)
{
	//	DWORD waterHandle = m_VGSEngine.GetRealWaterHandleByName((char*)_bstr_t(RealWaterName));
	//	DWORD cubeMapHandle = m_VGSEngine.GetCubeMapHandleByName((char*)_bstr_t(CubeMapName));
	//	m_VGSEngine.SetRealWaterCubeMap(waterHandle, cubeMapHandle);
	return S_OK;
}
//创建后期颜色处理特效
STDMETHODIMP CVGS2::CreateColorEffect()
{
	//	m_VGSEngine.CreateColorEffect();

	return S_OK;
}

//设置颜色，亮度，对比度
STDMETHODIMP CVGS2::SetColorEffect(FLOAT brightness, FLOAT contrast, FLOAT red, FLOAT green, FLOAT blue)
{
	//	m_VGSEngine.SetColorEffect(brightness, contrast, red, green, blue);
	return S_OK;
}

STDMETHODIMP CVGS2::SetTextureImageFromURL(BSTR mtrlName, INT texLayerIdx, BSTR imageURL)
{
	// TODO: Add your implementation code here
	//    EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//    DWORD mtrlHandle = m_VGSEngine.SunGetMaterialHandleByName((char*)_bstr_t(mtrlName));
	//
	//	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	//	if (pMtrl)
	//	{
	//       if(texLayerIdx<=pMtrl->texLayersList.size())
	//	   {
	//	     TextureLayer* pTexLayer =pMtrl->texLayersList[texLayerIdx];
	//		 if(pTexLayer->type !=TLT_CUBEMAP && pTexLayer->type != TLT_DYNAMIC && 
	//			 pTexLayer->type != TLT_BUMPMAP && pTexLayer->type != TLT_MOVIE && 
	//			 pTexLayer->type != TLT_FLASH && pTexLayer->type != TLT_RTFMAP)
	//		 {
	//			 if(pTexLayer->pTexture != NULL)
	//			 {
	//				 //将Load的N3D文件放进队列
	//				 CDownLoadFile* pFile = new CDownLoadFile();			 
	//				 strcpy(pFile->file,(char*)_bstr_t(imageURL) );			 
	//				 pFile->index = m_pVGS2Wnd->m_LoadFiles.size();
	//				 pFile->m_DownType = TYPE_IMAGEFILE;  //下载类型为image
	//				 
	//				 //是否从网络下载文件
	//				 BOOL loadFromWeb =FALSE;
	//
	//				 pFile->m_DownForObj = pTexLayer->pTexture;
	//
	//				 //----------------------------------------------------------
	//				 //如果是网络文件
	//				 //----------------------------------------------------------
	//				 if (pFile->file[0] == 'h' && pFile->file[1] == 't' && pFile->file[2] == 't' && pFile->file[3] == 'p')
	//				 {				 
	//					 loadFromWeb = TRUE;
	//				 }
	//				 else if(m_IsWebFile)
	//				 {
	//					 loadFromWeb = TRUE;
	//					 //转换BSTR
	//					 char pImageFileName[512];
	//					 strcpy(pImageFileName, m_webServer);
	//					 strcat(pImageFileName, pFile->file);
	//					 ZeroMemory(pFile->file,sizeof(pFile->file));
	//					 strcpy(pFile->file, pImageFileName);
	//				 }
	//
	//				 if(loadFromWeb)
	//				 {				 
	//					 m_pVGS2Wnd->m_LoadFiles.push_back(pFile);
	//					 //判断前一个Image是否已经下载完
	//					 if (pFile->index > 0)
	//					 {
	//						 INT preIndex = pFile->index - 1;
	//						 CDownLoadFile** ppFile = m_pVGS2Wnd->m_LoadFiles.begin();
	//						 CDownLoadFile* _pFile = *(ppFile + preIndex);
	//						 if (!_pFile->m_bAllDown)
	//							 return S_OK;
	//					 }
	//					 
	//					 //在后台下载Image文件
	//					 if (pFile->DownLoadHttpFile(pFile->file, NULL, m_pVGS2Wnd->m_hWnd, 1) == false)
	//					 {
	//                         m_pVGS2Wnd->EraseFileDownloader(pFile);
	//						 return E_FAIL;
	//					 }				 
	//				 }else
	//				 {
	//					 //转换BSTR
	//					 char pImageFileName[512];
	//					 strcpy(pImageFileName, m_webServer);
	//					 strcat(pImageFileName, pFile->file);
	//					 ZeroMemory(pFile->file,sizeof(pFile->file));
	//					 strcpy(pFile->file, pImageFileName);
	//
	//					 for (INT i = 0; i< 512 -4; i++)
	//		               	pFile->file[i] = pFile->file[i + 8];
	//					 //从本地创建贴图材质并替换
	//					 //To Do ...
	//					 //读入单机image文件
	//                     m_VGSEngine.vgsScene->CreateTexFromFile(pFile->file, (CTexture*)(pFile->m_DownForObj), ((CTexture*)pFile->m_DownForObj)->mipCount);
	//					 
	//					 SAFE_RELEASE(pFile);
	//					 SAFE_DELETE(pFile);				 
	//				 }
	//				 
	//				 
	//			 }		 
	//		 }
	//	   }
	//	}
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

//设置Edit对象的最大最小仰俯角
STDMETHODIMP CVGS2::SetEditCtrlMaxUpDownAngle(FLOAT upAngle, FLOAT downAngle)
{
	//	m_VGSEngine.editor.maxUpCosValue = cosf(upAngle * ATOS);
	//	m_VGSEngine.editor.maxDownCosValue = cosf(downAngle * ATOS);
	return S_OK;
}

//设置材质某一层的贴图
//mtrlName, 材质名
//textureLayerIndex, 贴图的编号
//textureType, 贴图的类型
//textureName, 贴图的名称
STDMETHODIMP CVGS2::SetMtrlTexture(BSTR mtrlName, INT textureLayerIndex, INT textureType, BSTR textureName)
{
	//	EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//	
	//	m_VGSEngine.SetMtrlTexture((char*)_bstr_t(mtrlName), textureLayerIndex, (N3D_TextureType)textureType, (char*)_bstr_t(textureName));
	//
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);

	return S_OK;
}

STDMETHODIMP CVGS2::AddFlashCtrlSemaphore(BSTR flashName, BSTR SemaphoreName, BSTR SemaphoreFunc)
{
	// TODO: Add your implementation code here
	//    if (m_pVGS2Wnd == NULL) return E_FAIL;
	//	std::string strSName =std::string((char *)_bstr_t(SemaphoreName));
	//	INT flashCnt = m_pVGS2Wnd->m_Flash.size();
	//	CATLFlashCtrl** ppFlash = m_pVGS2Wnd->m_Flash.begin();
	//	
	//	for (UINT i =0; i< flashCnt; i++)
	//	{
	//		CATLFlashCtrl* pFlash = *(ppFlash + i);
	//		if (strcmp(pFlash->name, (char *)_bstr_t(flashName)) == 0)
	//		{
	//			INT SemaphoreCnt = pFlash->SemaphoreList.size();
	//            for(UINT n = 0; n<SemaphoreCnt; n++)
	//			{
	//			   if(pFlash->SemaphoreList[n].SemaphoreName == strSName)
	//			     break;   
	//			}
	//			std::string strSFunc =std::string((char *)_bstr_t(SemaphoreFunc));
	//			Semaphore semap;
	//			semap.SemaphoreName = strSName;
	//            semap.SemaphoreFunc = strSFunc;
	//			pFlash->SemaphoreList.push_back(semap);
	//			break;
	//		}
	//	}
	return S_OK;
}

STDMETHODIMP CVGS2::DeleteFlashCtrlSemaphore(BSTR flashName, BSTR SemaphoreName)
{
	// TODO: Add your implementation code here
	// TODO: Add your implementation code here
	//    if (m_pVGS2Wnd == NULL) return E_FAIL;
	//	std::string strSName =std::string((char *)_bstr_t(SemaphoreName));
	//	INT flashCnt = m_pVGS2Wnd->m_Flash.size();
	//	CATLFlashCtrl** ppFlash = m_pVGS2Wnd->m_Flash.begin();
	//	
	//	for (UINT i =0; i< flashCnt; i++)
	//	{
	//		CATLFlashCtrl* pFlash = *(ppFlash + i);
	//		if (strcmp(pFlash->name, (char *)_bstr_t(flashName)) == 0)
	//		{
	//			INT SemaphoreCnt = pFlash->SemaphoreList.size();
	//            for(UINT n = 0; n<SemaphoreCnt; n++)
	//			{
	//			   if(pFlash->SemaphoreList[n].SemaphoreName == strSName)
	//			   {
	//                 pFlash->SemaphoreList.erase(pFlash->SemaphoreList.begin() + n);
	//			     break;
	//			   }
	//			}
	//			break;
	//		}
	//	}
	return S_OK;
}

STDMETHODIMP CVGS2::get_theFlashVar(BSTR *pVal)
{
	// TODO: Add your implementation code here
	//	_bstr_t bst(m_strflashVar.c_str());
	//	BSTR str = bst.copy();
	//    *pVal= str;
	return S_OK;
}

STDMETHODIMP CVGS2::GetFlashVaribleInfo(BSTR flashName, BSTR varibleName)
{
	// TODO: Add your implementation code here
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//	
	//	INT flashCnt = m_pVGS2Wnd->m_Flash.size();
	//	CATLFlashCtrl** ppFlash = m_pVGS2Wnd->m_Flash.begin();
	//	for (UINT i =0; i< flashCnt; i++)
	//	{
	//		CATLFlashCtrl* pFlash = *(ppFlash + i);
	//		if (strcmp(pFlash->name, (char *)_bstr_t(flashName)) == 0)
	//		{
	//			string strVarName = string((char *)_bstr_t(varibleName));
	//			m_strflashVar = pFlash->GetFlashVariable(strVarName);
	//			break;
	//		}
	//	}
	return S_OK;
}

STDMETHODIMP CVGS2::SetFlashVarible(BSTR flashName, BSTR varibleName, BSTR varibleValue)
{
	// TODO: Add your implementation code here
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//	
	//	INT flashCnt = m_pVGS2Wnd->m_Flash.size();
	//	CATLFlashCtrl** ppFlash = m_pVGS2Wnd->m_Flash.begin();
	//	for (UINT i =0; i< flashCnt; i++)
	//	{
	//		CATLFlashCtrl* pFlash = *(ppFlash + i);
	//		if (strcmp(pFlash->name, (char *)_bstr_t(flashName)) == 0)
	//		{
	//			string strVarName = string((char *)_bstr_t(varibleName));
	//			string strVarValue = string((char *)_bstr_t(varibleValue));
	//			pFlash->SetFlashVariable( strVarName, strVarValue);
	//			break;
	//		}
	//	}
	return S_OK;
}

STDMETHODIMP CVGS2::LoadN3DScene(BSTR SceneName, BSTR ParentSceneName, BSTR N3DFileURL, FLOAT posX, FLOAT posY, FLOAT posZ, FLOAT rotX, FLOAT rotY, FLOAT rotZ, FLOAT scaleX, FLOAT scaleY, FLOAT scaleZ)
{
	//场景的空间变换(位置，转角，缩放)
	//    D3DXQUATERNION quat;
	//	D3DXMATRIX N3DMat;
	//	D3DXQuaternionRotationYawPitchRoll(&quat, rotY, rotX, rotZ);
	//    D3DXVECTOR3 scale = D3DXVECTOR3(scaleX, scaleY, scaleZ);
	//	D3DXVECTOR3 pos = D3DXVECTOR3(posX, posY, posZ);
	//
	//	D3DXMatrixTransformation(&N3DMat, NULL, NULL, &scale, NULL, &quat, &pos);
	//
	//    if (m_pVGS2Wnd == NULL) return E_FAIL;
	//	if (m_VGSEngine.vgsScene == NULL) return E_FAIL;
	//	if (m_hInternetSession == NULL) return E_FAIL;
	//
	//	//转换BSTR
	//	char pN3DFileName[512];
	//	strcpy(pN3DFileName, (char*)_bstr_t(N3DFileURL));
	//
	//	//创建管理场景资源的对象
	//	CSceneResource* pSceneResource = m_VGSEngine.CreateSceneResource((char*)_bstr_t(SceneName), &N3DMat);
	//
	//	//将Load的N3D文件放进队列
	//	CDownLoadN3DFile* pN3D = new CDownLoadN3DFile(m_VGSEngine.vgsScene,
	//												  &m_ctrl_cs,
	//												  &m_render_cs,
	//												  m_hInternetSession);
	//
	//	strcpy(pN3D->name, (char*)_bstr_t(SceneName));
	//	strcpy(pN3D->file, pN3DFileName);
	//	pN3D->type = 0;  //下载并直接加载到场景模式
	//	pN3D->index = m_pVGS2Wnd->m_LoadN3DObj.size();
	//	pN3D->m_pSceneResource = pSceneResource;
	//	m_pVGS2Wnd->m_LoadN3DObj.push_back(pN3D);
	//
	//	
	//	//----------------------------------------------------------
	//	//是远程文件
	//	//----------------------------------------------------------
	//	if (pN3DFileName[0] == 'h' && pN3DFileName[1] == 't' && pN3DFileName[2] == 't' && pN3DFileName[3] == 'p')
	//	{
	//		m_IsWebFile = TRUE;
	//		//判断前一个N3D是否已经下载完
	//		if (pN3D->index > 0)  //没有下载完
	//		{
	//			INT preIndex = pN3D->index - 1;
	//			CDownLoadN3DFile** ppN3D = m_pVGS2Wnd->m_LoadN3DObj.begin();
	//			CDownLoadN3DFile* _pN3D = *(ppN3D + preIndex);
	//			if (!_pN3D->m_bAllDown)
	//				return S_OK;
	//		}
	//			
	//		//在后台下载N3D文件
	//		if (pN3D->DownLoadHttpFile(pN3DFileName, NULL, m_pVGS2Wnd->m_hWnd, pSceneResource) == false)
	//			m_VGSEngine.DeleteSceneResource(pSceneResource);
	//	}
	return S_OK;
}

STDMETHODIMP CVGS2::GetInfoFromSceneResource(BSTR SceneName)
{
	// TODO: Add your implementation code here
	//    CSceneResource* pSceneResource = m_VGSEngine.GetSceneResource(std::string((char*)_bstr_t(SceneName)));
	//	m_theSceneResHandle = (DWORD)pSceneResource;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theScenePosX(float *pVal)
{
	// TODO: Add your implementation code here
	//    CSceneResource* pSceneRes=(CSceneResource*)m_theSceneResHandle;
	//	*pVal= pSceneRes->m_pRootNode->worldPosition.x;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theScenePosY(float *pVal)
{
	// TODO: Add your implementation code here
	//    CSceneResource* pSceneRes=(CSceneResource*)m_theSceneResHandle;
	//	*pVal= pSceneRes->m_pRootNode->worldPosition.y;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theScenePosZ(float *pVal)
{
	// TODO: Add your implementation code here
	//    CSceneResource* pSceneRes=(CSceneResource*)m_theSceneResHandle;
	//	*pVal= pSceneRes->m_pRootNode->worldPosition.z;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theSceneRotX(float *pVal)
{
	// TODO: Add your implementation code here
	//    CSceneResource* pSceneRes=(CSceneResource*)m_theSceneResHandle;
	//	*pVal= pSceneRes->m_pRootNode->worldRotation.x;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theSceneRotY(float *pVal)
{
	// TODO: Add your implementation code here
	//    CSceneResource* pSceneRes=(CSceneResource*)m_theSceneResHandle;
	//	*pVal= pSceneRes->m_pRootNode->worldRotation.y;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theSceneRotZ(float *pVal)
{
	// TODO: Add your implementation code here
	//    CSceneResource* pSceneRes=(CSceneResource*)m_theSceneResHandle;
	//	*pVal= pSceneRes->m_pRootNode->worldRotation.z;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theSceneScaleX(float *pVal)
{
	// TODO: Add your implementation code here
	//    CSceneResource* pSceneRes=(CSceneResource*)m_theSceneResHandle;
	//	*pVal= pSceneRes->m_pRootNode->worldScale.x;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theSceneScaleY(float *pVal)
{
	// TODO: Add your implementation code here
	//    CSceneResource* pSceneRes=(CSceneResource*)m_theSceneResHandle;
	//	*pVal= pSceneRes->m_pRootNode->worldScale.y;
	return S_OK;
}

STDMETHODIMP CVGS2::get_theSceneScaleZ(float *pVal)
{
	// TODO: Add your implementation code here
	//    CSceneResource* pSceneRes=(CSceneResource*)m_theSceneResHandle;
	//	*pVal= pSceneRes->m_pRootNode->worldScale.z;
	return S_OK;
}

STDMETHODIMP CVGS2::SetSceneResColor(BSTR SceneName, INT red, INT green, INT blue, INT alpha)
{
	// TODO: Add your implementation code here
	//进入临界区
	//    EnterCriticalSection(&m_ctrl_cs);
	//	EnterCriticalSection(&m_render_cs);
	//
	//    CSceneResource* pSceneResource = m_VGSEngine.GetSceneResource(std::string((char*)_bstr_t(SceneName)));
	//    UINT sceneModelCnt = pSceneResource->model.size();
	//	CModel** ppModel = pSceneResource->model.begin();
	//	for(UINT i = 0; i < sceneModelCnt; i++)
	//	{
	//		CModel* pModel = *(ppModel + i);
	//		pModel->SetColor(red, green, blue, alpha);
	//		pModel->mouseUpColor = D3DCOLOR_RGBA(red, green, blue, alpha);
	//	}
	//	LeaveCriticalSection(&m_render_cs);
	//	LeaveCriticalSection(&m_ctrl_cs);
	return S_OK;
}

STDMETHODIMP CVGS2::SetButtonRect(BSTR buttonName, INT x, INT y, INT width, INT height)
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//	
	//	//转换BSTR
	//	CHAR strBtnName[64];
	//	ZeroMemory(strBtnName, 64);
	//	strncpy(strBtnName, (char*)_bstr_t(buttonName), 63);
	//
	//	CButton* pButton = (CButton*)m_VGSEngine.SunGetButtonByName(strBtnName);
	//	if (pButton && pButton->btnOverlay)
	//	{
	//		RECT rect;
	//		rect.left = x;
	//		rect.top = y;
	//		rect.right = rect.left + width;
	//		rect.bottom = rect.top + height;
	//		
	//		//进入临界区
	//		EnterCriticalSection(&m_ctrl_cs);
	//		EnterCriticalSection(&m_render_cs);
	//		m_VGSEngine.SunSetOverlayRect((DWORD)pButton->btnOverlay, &rect);
	//
	//		//进入临界区
	//		LeaveCriticalSection(&m_render_cs);
	//		LeaveCriticalSection(&m_ctrl_cs);
	//	}
	return S_OK;
}

// 设置VGS响应的第二套按键（除了方向键）是否有效
STDMETHODIMP CVGS2::SetAddtionalKeysStatus(BOOL bEnable) 
{
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//	m_VGSEngine.SetAddtionalKeysStatus(bEnable); 
	return S_OK;
}

// 设置VGS响应的第二套按键值，需要传入按键的ascii码
STDMETHODIMP CVGS2::SetAddtionalKeys(BYTE left, BYTE right, BYTE up, BYTE down)
{	
	//	if (m_pVGS2Wnd == NULL) return E_FAIL;
	//	m_VGSEngine.SetAddtionalKeys(left, right, up, down);
	return S_OK; 
}


STDMETHODIMP CVGS2::SetSubBrowserType(LONG type) 
{
	g_BrowserSubType = (WEBBROWSER_SUBTYPE)type;
	return S_OK;
}

STDMETHODIMP CVGS2::get_wbType(LONG* pVal)
{
	*pVal = (long)m_BrowserType;

	return S_OK;
}

STDMETHODIMP CVGS2::put_wbType(LONG newVal)
{
	m_BrowserType = (WEBBROWSERTYPE)newVal;
	g_BrowserType = m_BrowserType;
	return S_OK;
}
