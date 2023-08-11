// Test_Sun3D360Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test_Sun3D360.h"
#include "Test_Sun3D360Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_Sun3D360Dlg dialog

CTest_Sun3D360Dlg::CTest_Sun3D360Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTest_Sun3D360Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTest_Sun3D360Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_iInitType = 0;	
	m_fCameraAng = 45;
	m_fCameraNear = 0;
	m_fCameraFar = 1.0 ;	
	m_pSun3D360 = NULL;
	m_bPlay = TRUE;
	m_bAutoPlay = TRUE;
	m_bFullScreen = false;
}

void CTest_Sun3D360Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTest_Sun3D360Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTest_Sun3D360Dlg, CDialog)
	//{{AFX_MSG_MAP(CTest_Sun3D360Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_COMMAND(ID_AUTOPLAY, OnAutoplay)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_PLAY, OnPlay)
	ON_COMMAND(ID_STOP, OnStop)
	ON_COMMAND(ID_MANULPLAY, OnManulplay)
	ON_UPDATE_COMMAND_UI(ID_AUTOPLAY, OnUpdateAutoplay)
	ON_UPDATE_COMMAND_UI(ID_MANULPLAY, OnUpdateManulplay)
	ON_UPDATE_COMMAND_UI(ID_PLAY, OnUpdatePlay)
	ON_UPDATE_COMMAND_UI(ID_STOP, OnUpdateStop)
	ON_COMMAND(ID_FULL, OnFull)
	ON_WM_DESTROY()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// CTest_Sun3D360Dlg message handlers
//----------------------------------------------------------------
BOOL CTest_Sun3D360Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//exe所在的目录
	char strBuf[255] = {0};
	GetModuleFileName(NULL, strBuf, 255);
	CString strAppPath = strBuf;
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\') + 1);
	lstrcpy(m_szSceneFile, (LPCTSTR)strAppPath);

	//加在配置文件
	TCHAR szIniFile[MAX_PATH] = {0};
	lstrcpy(szIniFile, (LPCTSTR)strAppPath);
	lstrcat(szIniFile, _T("Config.ini"));
	if(!ReadIniFile(szIniFile))
	{
		MessageBox(_T("读取配置文件错误!"));
	}
	
	//创建Sun3D360
	m_pSun3D360 = Sun3D360_Create();
	if(m_pSun3D360)
	{
		RECT rc;
		GetClientRect(&rc);	
		m_pSun3D360->S3D360_Create(NULL, m_hWnd, rc);

		m_pSun3D360->S3D360_OpenFile(m_szSceneFile);
	}
	if(m_pSun3D360)
	{
		m_pSun3D360->S3D360_SetCameraInfo(m_fCameraAng, m_fCameraNear, m_fCameraFar);
		m_pSun3D360->S3D360_SetPlayType(m_iInitType);
	}		
	
	return TRUE;  // return TRUE  unless you set the focus to a control	
}
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
//----------------------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
//----------------------------------------------------------------
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
//----------------------------------------------------------------
HCURSOR CTest_Sun3D360Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//------------------------------------------------------
BOOL CTest_Sun3D360Dlg::ReadIniFile(LPCTSTR lpFile)
{	
	if(NULL == lpFile)
		return FALSE;

	TCHAR tmp[64] = {0};

	GetPrivateProfileString("SceneFile", "FileName", "", tmp, 64, lpFile);
	lstrcat(m_szSceneFile, tmp);

	GetPrivateProfileString("SceneControl", "IniType", "0", tmp, 64, lpFile);
	m_iInitType = atoi(tmp);
	
	GetPrivateProfileString("CameraParam", "CameraAng", "60", tmp, 64, lpFile);
	m_fCameraAng = (float)atof(tmp);

	GetPrivateProfileString("CameraParam", "CameraNear", "0", tmp, 64, lpFile);
	m_fCameraNear = (float)atof(tmp);

	GetPrivateProfileString("CameraParam", "CameraFar", "1", tmp, 64, lpFile);
	m_fCameraFar = (float)atof(tmp);

	return TRUE;
}
//------------------------------------------------------
void CTest_Sun3D360Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(m_pSun3D360)
	{
		RECT rc;
		GetClientRect(&rc);	
		m_pSun3D360->S3D360_Resize(rc);
	}	
}
//------------------------------------------------------
void CTest_Sun3D360Dlg::OnAutoplay() 
{
	// TODO: Add your command handler code here
	m_bAutoPlay=!m_bAutoPlay;
	if(m_pSun3D360)
	{
		m_pSun3D360->S3D360_SetPlayType(0);
	}	
}
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnExit() 
{
	::PostMessage(m_hWnd,WM_QUIT,NULL,NULL);
}
//----------------------------------------------------------------
//播放
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnPlay() 
{		
	m_bPlay = !m_bPlay;
	
		if(m_pSun3D360)
	{
		m_pSun3D360->S3D360_SetPause(false);
	}	
}
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnStop() 
{
	m_bPlay = !m_bPlay;

	if(m_pSun3D360)
	{
		m_pSun3D360->S3D360_SetPause(true);
	}	
}
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnManulplay() 
{
	m_bAutoPlay = !m_bAutoPlay;
	if(m_pSun3D360)
	{
		m_pSun3D360->S3D360_SetPlayType(1);
	}	
}
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnUpdateAutoplay(CCmdUI* pCmdUI) 
{
		pCmdUI->Enable(!m_bAutoPlay);
}
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnUpdateManulplay(CCmdUI* pCmdUI) 
{
		pCmdUI->Enable(m_bAutoPlay);
}
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnUpdatePlay(CCmdUI* pCmdUI) 
{
		pCmdUI->Enable(!m_bPlay);
}
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnUpdateStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bPlay);
}
//----------------------------------------------------------------



//----------------------------------------------------------------
//	满屏
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnFull() 
{
	GetWindowPlacement(&m_OldWndPlacement);
	
	CRect WindowRect;
	GetWindowRect(&WindowRect);
	
	CRect ClientRect;
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &ClientRect);
	ClientToScreen(&ClientRect);
	
	//获取屏幕的分辨率
	int nFullWidth = GetSystemMetrics(SM_CXSCREEN) + 2;
	int nFullHeight = GetSystemMetrics(SM_CYSCREEN) + 2;

	//将除控制条外的客户区全屏显示到从(0,0)到(nFullWidth, nFullHeight)区域, 
	//将(0,0)和(nFullWidth, nFullHeight)两个点外扩充原窗口和除控制条之外的 
	//客户区位置间的差值, 就得到全屏显示的窗口位置
	m_FullScreenRect.left = WindowRect.left - ClientRect.left - 1;
	m_FullScreenRect.top = WindowRect.top - ClientRect.top - 1;
	m_FullScreenRect.right = WindowRect.right - ClientRect.right + nFullWidth;
	m_FullScreenRect.bottom = WindowRect.bottom - ClientRect.bottom + nFullHeight;

	//设置全屏显示标志为 TRUE
	m_bFullScreen = TRUE;
	
	//进入全屏显示状态
	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof(WINDOWPLACEMENT);
	wndpl.flags = 0;
	wndpl.showCmd = SW_SHOWNORMAL;
	wndpl.rcNormalPosition = m_FullScreenRect;
	SetWindowPlacement(&wndpl);
}
//----------------------------------------------------------------
//	销毁消息函数
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_pSun3D360)
	{
		Sun3D360_Destroy(m_pSun3D360);
		m_pSun3D360 = NULL;
	}	
}
//----------------------------------------------------------------
//	满屏消息
//----------------------------------------------------------------
void CTest_Sun3D360Dlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if(m_bFullScreen)
	{
		lpMMI->ptMaxSize.x = m_FullScreenRect.Width();
		lpMMI->ptMaxSize.y = m_FullScreenRect.Height();
		lpMMI->ptMaxPosition.x = m_FullScreenRect.Width();
		lpMMI->ptMaxPosition.y = m_FullScreenRect.Height();
		//
		lpMMI->ptMaxTrackSize.x = m_FullScreenRect.Width();
		lpMMI->ptMaxTrackSize.y = m_FullScreenRect.Height();
	}
	
	CDialog::OnGetMinMaxInfo(lpMMI);
}
//----------------------------------------------------------------
//	处理Esc按键消息
//----------------------------------------------------------------
BOOL CTest_Sun3D360Dlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)  
	{
		if(m_bFullScreen)
		{
			//退出全屏显示, 恢复原窗口显示
			ShowWindow(SW_HIDE);
			SetWindowPlacement(&m_OldWndPlacement);
		}
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE; 
	else 
		return CDialog::PreTranslateMessage(pMsg);
}
//----------------------------------------------------------------
