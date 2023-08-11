//--------------------------------------------------------------------------
//  名称：ATLFlashCtrlUnit.cpp
//  简介：利用Atl.dll播放Flash文件
//  创建：yaoyoufang
//  日期：04-11-5
//--------------------------------------------------------------------------
#include "stdafx.h"
#include "ATLFlashCtrlUnit.h"
//--------------------------------------------------------------------------
#include <Atlhost.h>  
#include <Comdef.h>
//--------------------------------------------------------------------------
#include "InvokeJSFunc.h"
//--------------------------------------------------------------------------
#define PARENTRECT_WIDTH    1004
#define PARENTRECT_HEIGHT   620

CATLFlashCtrl* g_pWindow = NULL;

//--------------------------------------------------------------------------
//  构造函数 
//--------------------------------------------------------------------------
CATLFlashCtrl::CATLFlashCtrl(HWND parentHandle, IOleClientSite* pClientSite)
{
	g_pWindow = this;

    m_hFlashWnd = NULL;
    m_atlFlash = NULL;
    m_parentHandle = parentHandle;
    m_pClientSite = pClientSite;
	
	alignMode = 0;
	isFullWidth = FALSE;
	isFullHeight = FALSE;
	bPlayOver = FALSE;
	bLoadOver = FALSE;
	bCreated = FALSE;

	playStatus = 0;
	m_bVisible = true;
	m_pJSInvoker = NULL;
	SemaphoreList.clear();
}

//--------------------------------------------------------------------------
//  析构函数
//--------------------------------------------------------------------------
CATLFlashCtrl::~CATLFlashCtrl()
{
}      

//释放内存
void CATLFlashCtrl::Delete()
{
    //停止
    if(m_atlFlash) 
	{
		m_atlFlash->StopPlay();
		//ShowWindow(m_hFlashWnd, SW_HIDE);
		//::UpdateWindow(m_hFlashWnd);
		SAFE_RELEASE_D3D(m_atlFlash);
	}

    SemaphoreList.clear();
    //删除窗体, WM_CLOSE消息会导致VGS2类调用OnDestroy方法
    //PostMessage(m_hFlashWnd, WM_DESTROY, NULL, NULL);
	DestroyWindow(m_hFlashWnd);
}

//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wparam, LPARAM lparam )
{
	return (g_pWindow->MsgProc( hWnd, uMsg, wparam, lparam ));
}

LRESULT CATLFlashCtrl::MsgProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	UINT nActive = 0; // activation flag 
	UINT nMinimized = 0;// minimized flag 

	HBRUSH hbr; 

	switch (msg)
	{	
	//键盘事件
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		break;

	case WM_MOUSEMOVE:
		break;

	//鼠标左键事件 - 左键
	case WM_LBUTTONDOWN :	
		break;

	case WM_LBUTTONUP:
		break;

	//鼠标右键事件 -右键
	case WM_RBUTTONDOWN :
		break;

	case WM_RBUTTONUP :
		break;

	//鼠标中键事件 - 中键
	case WM_MBUTTONDOWN :
		break;
	case WM_MBUTTONUP :
		break;

	//鼠标滚轮事件 - 滚轮
	case 0x020A :
		break;

	//重画DC事件
	case WM_PAINT:	
		hbr = (HBRUSH)GetStockObject(GetSysColor(COLOR_3DFACE));
		::SetBkMode(::GetDC(m_hFlashWnd), TRANSPARENT);
		::FillRect(::GetDC(m_hFlashWnd), &m_stuCurFSRect, hbr);
		ValidateRect(hWnd, NULL);
		break;

	case WM_ENTERSIZEMOVE:
		break;

	case WM_SIZE:
		break;

	case WM_ACTIVATE:
		nActive = LOWORD(wparam);           // activation flag 
		nMinimized = (BOOL) HIWORD(wparam); // minimized flag 
		switch (nActive)
		{
		case WA_ACTIVE:
			break;
		case WA_CLICKACTIVE:
			break;
		case WA_INACTIVE:
			break;
		}
		break;


	//销毁窗体事件
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, msg, wparam, lparam));
}

//--------------------------------------------------------------------------
//  创建  
//--------------------------------------------------------------------------
void CATLFlashCtrl::Create(string strName, RECT& parentRect, int iLeft, int iTop,
                    int iWidth, int iHeight, BSTR pszMovieFile, int iBlend, bool bVisible)
{
    strcpy(name, strName.c_str());

	left = iLeft;
	top = iTop;
	width = iWidth;
	height = iHeight;

	if (width < 0)
		isFullWidth = TRUE;
	if (height < 0)
		isFullHeight = TRUE;

	m_strMovieName = pszMovieFile;

    m_stuParentRect = parentRect;
    m_stuCurFSRect.left = iLeft;
    m_stuCurFSRect.top = iTop;
    m_stuCurFSRect.right = iLeft + iWidth;
    m_stuCurFSRect.bottom = iTop + iHeight;

    if(m_parentHandle == NULL) return;

    AtlAxWinInit();
	int iwidth = m_stuCurFSRect.right - m_stuCurFSRect.left;
	int iheight = m_stuCurFSRect.bottom - m_stuCurFSRect.top;

    m_hFlashWnd = CreateWindowEx(WS_EX_TOPMOST, 
		                         LPCWSTR("AtlAxWin"), 
								 LPCWSTR("ShockwaveFlash.ShockwaveFlash"),
								 WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, 
								 m_stuCurFSRect.left, m_stuCurFSRect.top, iwidth, iheight, 
								 m_parentHandle, 0, 0, 0);
	
	
    if(m_hFlashWnd == NULL) return;
	
    ShowWindow(m_hFlashWnd, bVisible ? SW_SHOW : SW_HIDE);
	::UpdateWindow(m_hFlashWnd);
	
	IUnknown *iUnk;
    AtlAxGetControl(m_hFlashWnd, &iUnk);    

     //挂事件     
    DWORD ok = 0;       
    AtlAdvise(iUnk, new TFlashEventHandle(m_pClientSite), DIID__IShockwaveFlashEvents, &ok);

    iUnk->QueryInterface(IID_IShockwaveFlash, (void**)&m_atlFlash);
	iUnk->Release();


	if(m_atlFlash == NULL)
	{
		::MessageBox(NULL, LPCWSTR("VGS has not find Flash player in your system, \nPlease install Flash player first via Adobe Flash player installer website."), 
			         LPCWSTR("Flash Player Error..."), MB_ICONASTERISK | MB_OK);
		return;
	}
	
	m_atlFlash->set_Movie(m_strMovieName);
	Stop();

	bCreated = TRUE;
    
	//Edit by Lixin 2007/10/12取消了自定义的消息事件
	//设置窗体事件
//	::SetWindowLong(m_hFlashWnd, GWL_WNDPROC, (LONG)WndProc);
}        
      
//设置invoker 
void CATLFlashCtrl::SetJSInvoker(CInvokeJSFunc* JSInvoker)
{
       m_pJSInvoker = JSInvoker;
}
//--------------------------------------------------------------------------
//  设置是否看见     
//--------------------------------------------------------------------------
void CATLFlashCtrl::SetVisible(BOOL bVisible)
{
    if(bVisible)   
	{
		ShowWindow(m_hFlashWnd, SW_SHOW);
		::UpdateWindow(m_hFlashWnd);
	}
    else  
	{
		ShowWindow(m_hFlashWnd, SW_HIDE);  
		::UpdateWindow(m_hFlashWnd); 
	}
	m_bVisible = bVisible; 
}

//--------------------------------------------------------------------------
//  播放
//--------------------------------------------------------------------------
void CATLFlashCtrl::Play(BOOL bLoop)
{
	playStatus = 1;
    if(m_atlFlash == NULL) return;
    
	//m_atlFlash->set_Movie(m_strMovieName);
    m_atlFlash->set_Loop(bLoop);
    m_atlFlash->Play();

	//CComBSTR mode("transparent");
    //m_atlFlash->set_WMode(mode);
}

//--------------------------------------------------------------------------
//  暂停
//--------------------------------------------------------------------------
void CATLFlashCtrl::Pause()
{
	playStatus = 0;
    if(m_atlFlash == NULL) return;
    m_atlFlash->StopPlay();

}


//--------------------------------------------------------------------------
//  停止
//--------------------------------------------------------------------------
void CATLFlashCtrl::Stop()
{                  
	playStatus = 2;
	
    if(m_atlFlash == NULL) return;
    m_atlFlash->Rewind();
    m_atlFlash->StopPlay();
}

//--------------------------------------------------------------------------
//	跳至指定的帧
//--------------------------------------------------------------------------
void CATLFlashCtrl::SetCurFrm(long frm)
{
    if(m_atlFlash == NULL) return;
	m_atlFlash->set_FrameNum(frm);
}

//--------------------------------------------------------------------------
//  设置父窗体的大小
//--------------------------------------------------------------------------
void CATLFlashCtrl::SetParentRect(RECT parentRect)
{
    m_stuParentRect = parentRect;

    CalcCurFlashRect();

    if(m_hFlashWnd != NULL)
    {
		int iCurWidth = m_stuCurFSRect.right - m_stuCurFSRect.left;
		int iCurHeight = m_stuCurFSRect.bottom - m_stuCurFSRect.top;
        ::MoveWindow(m_hFlashWnd, m_stuCurFSRect.left, m_stuCurFSRect.top, iCurWidth, iCurHeight, false);
		::UpdateWindow(m_hFlashWnd);
    }
}

//--------------------------------------------------------------------------
//  设置Flash窗体的Rect
//--------------------------------------------------------------------------
void CATLFlashCtrl::SetRect(int _left, int _top, int _width, int _height)
{
	left = _left;
	top = _top;
	width = _width;
	height = _height;

    m_stuCurFSRect.left  = left;
    m_stuCurFSRect.right = left + width;
    m_stuCurFSRect.top += top;
    m_stuCurFSRect.bottom += top + height;

    if(m_hFlashWnd != NULL)
	{
	

		/*
		::SetWindowPos(
		  m_hFlashWnd,             // handle to window
		  m_parentHandle,		// placement-order handle
		  left,                 // horizontal position
		  top,                 // vertical position
		  width,                // width
		  height,                // height
		  SWP_SHOWWINDOW            // window-positioning flags
		);
		*/

        ::MoveWindow(m_hFlashWnd, left, top, width, height, true);
		//::InvalidateRect(m_hFlashWnd, NULL, false);
		//ShowWindow(m_hFlashWnd, SW_SHOW);
		::UpdateWindow(m_hFlashWnd);
	}
}

//--------------------------------------------------------------------------
//  根据父窗体的大小调整Flash窗体的大小和位置
//--------------------------------------------------------------------------
void CATLFlashCtrl::CalcCurFlashRect()
{
    //大小
    int iWidth = (m_stuParentRect.right - m_stuParentRect.left) * (m_stuStdFSRect.right - m_stuStdFSRect.left) / PARENTRECT_WIDTH;
    int iHeight = (m_stuParentRect.bottom - m_stuParentRect.top) * (m_stuStdFSRect.bottom - m_stuStdFSRect.top) / PARENTRECT_HEIGHT;

    //位置
    int iLeft = (m_stuParentRect.right-m_stuParentRect.left) * m_stuStdFSRect.left / PARENTRECT_WIDTH;
    int iTop = (m_stuParentRect.bottom-m_stuParentRect.top) * m_stuStdFSRect.top / PARENTRECT_HEIGHT;

    //Flash实际区域
    m_stuCurFSRect.left = iLeft;
    m_stuCurFSRect.top = iTop;
    m_stuCurFSRect.right = m_stuCurFSRect.left + iWidth;
    m_stuCurFSRect.bottom = m_stuCurFSRect.top + iHeight;
}
/*
//------------------------------------------------------------------------------
HRESULT CATLFlashCtrl::OnFSCommand(BSTR cmd, BSTR args)
{
	int a = 0;
	return S_OK;
}
*/

//得到当前Flash的总帧数
int	CATLFlashCtrl::GetFrameCnt()
{
	if (m_atlFlash == NULL)
		return 0;

	long frameCnt = 0;
	m_atlFlash->get_TotalFrames(&frameCnt);
	return frameCnt;
}

//得到Flash的当前帧
int	CATLFlashCtrl::GetCurrentFrame()
{
	if (m_atlFlash == NULL)
		return 0;

	long currentFrame = 0;
	m_atlFlash->CurrentFrame(&currentFrame);
	return currentFrame;
}

//得到Load的百分比
int	CATLFlashCtrl::GetLoadPercent()
{
	if (m_atlFlash == NULL)
		return 0;

	long percent = 0;
	m_atlFlash->PercentLoaded(&percent);
	return percent;
}
//获得flash的变量
string CATLFlashCtrl::GetFlashVariable(string variableName)
{
	BSTR d = NULL;
	_bstr_t bst(variableName.c_str());
	m_atlFlash->GetVariable(bst,&d);
	string c;
    if(d!=NULL)
		c = string(_bstr_t(d));
	return c;
}
//设置flash中的变量
void CATLFlashCtrl::SetFlashVariable(string variableName, string variable)
{
	m_atlFlash->SetVariable(_bstr_t(variableName.c_str()), _bstr_t(variable.c_str()));
}

