//--------------------------------------------------
// SocketServerDlg.cpp : implementation file
//
//--------------------------------------------------
#include "stdafx.h"
#include "SocketServer.h"
#include "SocketServerDlg.h"
//--------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//--------------------------------------------------
// CAboutDlg dialog used for App About
//--------------------------------------------------
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
//--------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}
//--------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}
//--------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//--------------------------------------------------
// CSocketServerDlg dialog
//--------------------------------------------------
CSocketServerDlg::CSocketServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSocketServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//--------------------------------------------------
void CSocketServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSocketServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//--------------------------------------------------
BEGIN_MESSAGE_MAP(CSocketServerDlg, CDialog)
	//{{AFX_MSG_MAP(CSocketServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(SKT_MSG_CLIENTCONNECTED, OnClientConnected)
	ON_MESSAGE(SKT_MSG_RECEIVEDMSG, OnRecievedMsg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//--------------------------------------------------
// CSocketServerDlg message handlers
//--------------------------------------------------
BOOL CSocketServerDlg::OnInitDialog()
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
	

	//Create the server socket
	m_pServerSocket = new CServerSocket();
	if(!m_pServerSocket->Create(m_hWnd, 200))
	{
		MessageBox(_T("Create the server socket error!"));
		return FALSE;
	}
	SetDlgItemText(IDC_STATIC_STATE, _T("�ͻ�������״̬:δ����!"));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//--------------------------------------------------
void CSocketServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//--------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//--------------------------------------------------
void CSocketServerDlg::OnPaint() 
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
//--------------------------------------------------
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSocketServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//--------------------------------------------------


//--------------------------------------------------
//	Name:OnClientConnected
//	Introduce:SKT_MSG_CLIENTCONNECTED message handler
//--------------------------------------------------
HRESULT CSocketServerDlg::OnClientConnected(WPARAM wParam, LPARAM lParam)
{
	SetDlgItemText(IDC_STATIC_STATE, _T("�ͻ�������״̬:������!"));

	return S_OK;
}
//--------------------------------------------------
//	Name:OnRecievedMsg
//	Introduce:SKT_MSG_RECEIVEDMSG message handler
//--------------------------------------------------
HRESULT CSocketServerDlg::OnRecievedMsg(WPARAM wParam, LPARAM lParam)
{
	LPBYTE pDataBuff = (LPBYTE)wParam;
	int	iDataSize = lParam;
	if(NULL == pDataBuff || iDataSize <= 0)
		return S_FALSE;

	//Backup the message data
	LPBYTE pMsgDataBuff = new BYTE[iDataSize];
	memcpy(pMsgDataBuff, pDataBuff, iDataSize);
	pDataBuff = pMsgDataBuff;

	//To read the message data
	int iCurReadCnt = 0;
	while(iCurReadCnt < iDataSize)
	{
		UINT uMsgType;
		memcpy(&uMsgType, pDataBuff, sizeof(UINT));

		int iReadCnt = 0;
		switch(uMsgType)
		{
		case MSG_TEST:		//Client information data message
			{
				LPSOCKETMSG lpSocketMsg = (LPSOCKETMSG)pDataBuff;

				char szParam1[32];
				sprintf(szParam1, "����1:%d", lpSocketMsg->iParam1);
				SetDlgItemText(IDC_STATIC_PARAM1, szParam1);
				//
				char szParam2[32];
				sprintf(szParam2, "����2:%f", lpSocketMsg->fParam2);
				SetDlgItemText(IDC_STATIC_PARAM2, szParam2);
				//
				char szParam3[32];
				sprintf(szParam3, "����3:%s", lpSocketMsg->szParam3);
				SetDlgItemText(IDC_STATIC_PARAM3, szParam3);
				//
				iReadCnt = sizeof(SocketMsg);
			}
			break;
		default:
			iReadCnt = sizeof(BYTE);
			break;
		}

		iCurReadCnt += iReadCnt;
		pDataBuff += iReadCnt;
	}

	delete []pMsgDataBuff;
	pMsgDataBuff = NULL;

	return S_OK;
}
//---------------------------------------------------
void CSocketServerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(NULL != m_pServerSocket)
	{
		delete m_pServerSocket;
		m_pServerSocket = NULL;
	}	
}
//---------------------------------------------------
