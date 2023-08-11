// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "winsock2.h"
//----------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//----------------------------------------------
//	Name:ServerMsgCallback
//	Introduce:Server msg manager callback function
//----------------------------------------------
DWORD ServerMsgCallback(DWORD dwCode, DWORD dwParam, LPVOID lpParam, LPVOID lpUser)
{
	if(NULL != lpUser)
		return ((CTestDlg*)lpUser)->ServerCallbackFunc(dwCode, dwParam, lpParam);

	return 0;
}
//----------------------------------------------
//	Name:ClientMsgCallback
//	Introduce:Client msg manager callback function
//----------------------------------------------
DWORD ClientMsgCallback(DWORD dwCode, DWORD dwParam, LPVOID lpParam, LPVOID lpUser)
{
	if(NULL != lpUser)
		return ((CTestDlg*)lpUser)->ClientCallbackFunc(dwCode, dwParam, lpParam);

	return 0;
}
//----------------------------------------------

//----------------------------------------------
// CAboutDlg dialog used for App About
//----------------------------------------------
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
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_pServer = NULL;
	m_pClient = NULL;
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
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

	m_pServer = MSGManager_CreateServer();
	m_pServer->Create(200, ServerMsgCallback, this);
	m_pServer->Listen();

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestDlg::OnPaint() 
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
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//----------------------------------------------


//----------------------------------------------
//	Name:ServerCallbackFunc
//	Introduce:Server msg manager callback function
//----------------------------------------------
DWORD CTestDlg::ServerCallbackFunc(DWORD dwCode, DWORD dwParam, LPVOID lpParam)
{
	switch(dwCode)
	{
	case MSG_CODE_CLIENTCONNECTED:	//A client is connected
		OnClientConnected(dwParam);
		break;
	case MSG_CODE_RECIEVEDATA:		//Recieved a data message
		OnServerRecvData((BYTE*)lpParam, dwParam);
		break;
	}

	return 0;
}
//----------------------------------------------
//	Name:OnClientConnected
//	Introduce:MSG_CODE_CLIENTCONNECTED message handler
//----------------------------------------------
void CTestDlg::OnClientConnected(DWORD dwClientID)
{
	if(NULL != m_pServer)
	{
		UINT uMsgType = MSG_STC_GETCLIENTINFO;
		m_pServer->SendData((BYTE*)&uMsgType, sizeof(UINT), dwClientID);
	}
}
//----------------------------------------------
//	Name:OnServerRecvData
//	Introduce:MSG_CODE_RECIEVEDATA message handler
//----------------------------------------------
DWORD CTestDlg::OnServerRecvData(BYTE* lpData, DWORD dwSize)
{
	return 0;
}
//----------------------------------------------


//----------------------------------------------
//	Name:OnBtnConnect
//	Introduce:To connect the server
//----------------------------------------------
void CTestDlg::OnBtnConnect() 
{
	m_pClient = MSGManager_CreateClient();	
	m_pClient->Create(ClientMsgCallback, this);
	m_pClient->Connect(_T("127.0.0.1"), 200);
}
//----------------------------------------------
//	Name:ClientCallbackFunc
//	Introduce:Client msg manager callback function
//----------------------------------------------
DWORD CTestDlg::ClientCallbackFunc(DWORD dwCode, DWORD dwParam, LPVOID lpParam)
{
	switch(dwCode)
	{
	case MSG_CODE_RECIEVEDATA:		//Recieved a data message
		OnClientRecvData((BYTE*)lpParam, dwParam);
		break;
	}

	return 0;
}
//----------------------------------------------
//	Name:OnClientRecvData
//	Introduce:MSG_CODE_RECIEVEDATA message handler
//----------------------------------------------
DWORD CTestDlg::OnClientRecvData(BYTE* lpData, DWORD dwSize)
{
	if(NULL == lpData || dwSize <= 0)
		return 0;

	UINT uMsgCode = 0;
	memcpy(&uMsgCode, lpData, sizeof(UINT));
	switch(uMsgCode)
	{
	case MSG_STC_GETCLIENTINFO:
		{
			ClientInfo info;
			info.uMsgType = MSG_CTS_CLIENTINFO;
			info.uClientID = 10;
			lstrcpy(info.szClientName, _T("Client 10"));
			m_pClient->SendData((BYTE*)&info, sizeof(ClientInfo));
		}
		break;
	}

	return 0;
}
//----------------------------------------------

