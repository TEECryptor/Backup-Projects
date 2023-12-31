// SunMaxScreenServerDlg.cpp : implementation file
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "SunMaxScreenServer.h"
#include "SunMaxScreenServerDlg.h"
//-------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
// CAboutDlg dialog used for App About
//-------------------------------------------------------------------------
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}
//-------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
// CSunMaxScreenServerDlg dialog
//-------------------------------------------------------------------------
CSunMaxScreenServerDlg::CSunMaxScreenServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSunMaxScreenServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pServerSocket = NULL;
	m_pLogFile = NULL;
}
//-------------------------------------------------------------------------
void CSunMaxScreenServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER_MAIN, m_MainIEWindow);
}
//-------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CSunMaxScreenServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_MESSAGE(WM_RECEIVED_DATA, OnRecieveData)
	ON_MESSAGE(WM_SHOWTASK, OnShowTask)
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()
//-------------------------------------------------------------------------



//-------------------------------------------------------------------------
// CSunMaxScreenServerDlg message handlers
//-------------------------------------------------------------------------
BOOL CSunMaxScreenServerDlg::OnInitDialog()
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

	SetWindowText(_T("创图大屏投影控制程序-服务器端"));

	TCHAR tcExePath[MAX_PATH] = {_T("\0")};
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	CString strCfgFileName = tcExePath;
	CString strLogFileName = tcExePath;
	strCfgFileName = strCfgFileName.Left(strCfgFileName.ReverseFind(_T('\\')));
	strLogFileName = strLogFileName.Left(strLogFileName.ReverseFind(_T('\\')));
	strCfgFileName += _T("\\ServerCfg.ini");
	strLogFileName += _T("\\ServerLog.txt");
	m_pLogFile = _tfopen(strLogFileName, _T("w"));
	//
	int iPort = 80;
	TCHAR tcServerPort[64] = {_T("\0")};
	GetPrivateProfileString(_T("ServerCfg"), _T("Port"), _T("80"), tcServerPort, 64, strCfgFileName);
	iPort = _ttol(tcServerPort);

	AfxSocketInit();
	m_pServerSocket = new CServerSocket();
	m_pServerSocket->Create(iPort);
	m_pServerSocket->Init(m_hWnd);
	m_pServerSocket->Listen();

	ShowWindow(SW_MINIMIZE);
	SetTimer(0, 1000, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
//-------------------------------------------------------------------------
void CSunMaxScreenServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	if (nID == SC_MINIMIZE)
	{
		MinsizeToTray();
		//ShowWindow(SW_HIDE);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
//-------------------------------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//-------------------------------------------------------------------------
void CSunMaxScreenServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
//-------------------------------------------------------------------------
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
//-------------------------------------------------------------------------
HCURSOR CSunMaxScreenServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//-------------------------------------------------------------------------
BOOL CSunMaxScreenServerDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.dwExStyle |= WS_EX_TOOLWINDOW;

	return CDialog::PreCreateWindow(cs);
}
//-------------------------------------------------------------------------
void CSunMaxScreenServerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	fclose(m_pLogFile);
	m_pLogFile = NULL;
	//
	DeleteTray();
	if (NULL  != m_pServerSocket)
	{
		m_pServerSocket->Close();
		delete m_pServerSocket;
		m_pServerSocket = NULL;
	}
}
//-------------------------------------------------------------------------
void CSunMaxScreenServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);
	if (m_MainIEWindow.m_hWnd != NULL)
	{
		m_MainIEWindow.MoveWindow(&rcClient);
	}
}
//-------------------------------------------------------------------------
void CSunMaxScreenServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	MinsizeToTray();
	KillTimer(0);
	CDialog::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------
LRESULT CSunMaxScreenServerDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{  
	if(wParam!=IDR_MAINFRAME)   
		return   1;   

	switch(lParam)   
	{   
	case WM_RBUTTONUP:  
		{   
		  POINT pnt;    
		  CMenu menu;   
		  ::GetCursorPos(&pnt); 
		  menu.CreatePopupMenu();
		  menu.AppendMenu(MF_STRING, SC_MAXIMIZE, _T("显示")); 
		  menu.AppendMenu(MF_STRING, SC_MINIMIZE, _T("隐藏")); 
		  menu.AppendMenu(MF_SEPARATOR, 0, _T("")); 		  
		  menu.AppendMenu(MF_STRING, WM_DESTROY, _T("退出")); 
		  menu.TrackPopupMenu(TPM_LEFTALIGN, pnt.x, pnt.y, this);  
		  HMENU hmenu = menu.Detach();   
		  menu.DestroyMenu();   
		}   
		break;   
	case WM_LBUTTONDBLCLK:
		{   
		  ShowWindow(SW_SHOW);
		}   
		break;   
	}   
	return   0;   
}
//-------------------------------------------------------------------------	
BOOL CSunMaxScreenServerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SC_MAXIMIZE:
		ShowWindow(SW_MAXIMIZE);
		break;
	case SC_MINIMIZE:
		MinsizeToTray();
		break;
	}

	return CDialog::OnCommand(wParam, lParam);
}
//-------------------------------------------------------------------------	
void CSunMaxScreenServerDlg::MinsizeToTray()
{
	NOTIFYICONDATA   nid;   
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);   
	nid.hWnd = m_hWnd;   
	nid.uID = IDR_MAINFRAME;   
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;   
	nid.uCallbackMessage = WM_SHOWTASK;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));   
	_tcscpy(nid.szTip, _T("创图大屏投影控制程序-服务器端"));
	Shell_NotifyIcon(NIM_ADD,&nid);
	ShowWindow(SW_HIDE);
}
//-------------------------------------------------------------------------
void CSunMaxScreenServerDlg::DeleteTray()
{
  NOTIFYICONDATA nid; 
  nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
  nid.hWnd=this->m_hWnd; 
  nid.uID=IDR_MAINFRAME; 
  nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ; 
  nid.uCallbackMessage=WM_SHOWTASK;
  nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
  _tcscpy(nid.szTip, _T("创图大屏投影控制程序-服务器端")); 
  Shell_NotifyIcon(NIM_DELETE,&nid);
}
//-------------------------------------------------------------------------



//-------------------------------------------------------------------------
LRESULT CSunMaxScreenServerDlg::OnRecieveData(WPARAM wParam, LPARAM lParam)
{
	char* pDataBuf = (char*)wParam;
	int iDataSize = lParam;
	if (iDataSize < 5)
		return -1;

	char csCmdLine[6] = {"0"};
	char csCmdParam[512] = {"0"};
	TCHAR tcURL[512] = {_T("\0")};
	strncpy(csCmdLine, pDataBuf, 4);
	if (iDataSize > 5)
	{
		strcpy(csCmdParam, &pDataBuf[5]);
#ifndef	UNICODE
		strcpy(tcURL, csCmdParam);
#else
		MultiByteToWideChar(CP_ACP, 0, csCmdParam, -1, tcURL, 512);
#endif	//UNICODE
	}

	if (stricmp(csCmdLine, "YCCK") == 0)
		ExecuteCmdProject();
	else if (stricmp(csCmdLine, "QZSX") == 0)
		ExecuteCmdRefresh();
	else if (stricmp(csCmdLine, "SWCK") == 0)
		ExecuteCmdShow3DStore(tcURL);
	else if (stricmp(csCmdLine, "ZJXM") == 0)
		ExecuteCmdShowProjectInfo(tcURL);
	else if (stricmp(csCmdLine, "YJXC") == 0)
		ExecuteCmdShowFieldInfo(tcURL);

	return 0;
}
//-------------------------------------------------------------------------
void CSunMaxScreenServerDlg::ExecuteCmdProject()
{
	LOG(_T("YCCK"));
	ShowWindow(SW_MINIMIZE);
}
//-------------------------------------------------------------------------
void CSunMaxScreenServerDlg::ExecuteCmdRefresh()
{
	LOG(_T("QZSX"));
	ShowWindow(SW_MAXIMIZE);
	if (m_MainIEWindow.GetLocationURL().GetLength() != 0)
	{
		m_MainIEWindow.Refresh();
	}
}
//-------------------------------------------------------------------------
void CSunMaxScreenServerDlg::ExecuteCmdShow3DStore(LPCTSTR lpszURL)
{
	LOG(_T("SWCK"));
	LOG(lpszURL);
	ShowWindow(SW_MAXIMIZE);
	m_MainIEWindow.Stop();
	m_MainIEWindow.Navigate(lpszURL, NULL, NULL, NULL, NULL);
}
//-------------------------------------------------------------------------
void CSunMaxScreenServerDlg::ExecuteCmdShowProjectInfo(LPCTSTR lpszURL)
{
	LOG(_T("ZJXM"));
	LOG(lpszURL);
	ShowWindow(SW_MAXIMIZE);
	m_MainIEWindow.Stop();
	m_MainIEWindow.Navigate(lpszURL, NULL, NULL, NULL, NULL);
}
//-------------------------------------------------------------------------
void CSunMaxScreenServerDlg::ExecuteCmdShowFieldInfo(LPCTSTR lpszURL)
{
	LOG(_T("YJXC"));
	LOG(lpszURL);
	ShowWindow(SW_MAXIMIZE);
	m_MainIEWindow.Stop();
	m_MainIEWindow.Navigate(lpszURL, NULL, NULL, NULL, NULL);
}
//-------------------------------------------------------------------------
