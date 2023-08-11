// SocketClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SocketClient.h"
#include "SocketClientDlg.h"

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
// CSocketClientDlg dialog

CSocketClientDlg::CSocketClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSocketClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSocket = NULL;
}

void CSocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSocketClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSocketClientDlg, CDialog)
	//{{AFX_MSG_MAP(CSocketClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSocketClientDlg message handlers

BOOL CSocketClientDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_EDIT_PORT, "200");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSocketClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSocketClientDlg::OnPaint() 
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSocketClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//--------------------------------------------------



//--------------------------------------------------
void CSocketClientDlg::OnBtnConnect() 
{
	if(NULL == m_pSocket)
	{
		m_pSocket = new CClientSocket(m_hWnd);
		if(!m_pSocket->Create(1))
			return;
	}
	
	TCHAR szServer[32];
	TCHAR szPort[32];
	GetDlgItemText(IDC_EDIT_IP, szServer, 32);
	GetDlgItemText(IDC_EDIT_PORT, szPort, 32);
	int iPort = _ttoi(szPort);
	if(m_pSocket->Connect(szServer, iPort))
	{
		MessageBox(_T("连接服务器成功!"));
	}
	else
	{
		MessageBox(_T("连接服务器错误!"));
	}
}
//--------------------------------------------------


//--------------------------------------------------
void CSocketClientDlg::OnBtnSend() 
{
	if(NULL == m_pSocket)
	{
		MessageBox(_T("未连接服务器!"));
		return;
	}

	SocketMsg msg;
	msg.iMsgType = MSG_TEST;

	char szParam1[32];
	GetDlgItemText(IDC_EDIT_PARAM1, szParam1, 32);
	msg.iParam1 = atoi(szParam1);
	//
	char szParam2[32];
	GetDlgItemText(IDC_EDIT_PARAM2, szParam2, 32);
	msg.fParam2 = (float)atof(szParam2);
	//
	char szParam3[32];
	GetDlgItemText(IDC_EDIT_PARAM3, szParam3, 32);
	strcpy(msg.szParam3, szParam3);	
	//
	m_pSocket->SendData(&msg, sizeof(SocketMsg));
}
//--------------------------------------------------
void CSocketClientDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(NULL != m_pSocket)
	{
		delete m_pSocket;
		m_pSocket = NULL;
	}	
}
//--------------------------------------------------
