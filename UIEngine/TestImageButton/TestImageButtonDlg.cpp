// TestImageButtonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestImageButton.h"
#include "TestImageButtonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ULONG_PTR			gdiplusToken;
GdiplusStartupInput gdiplusStartupInput;

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
// CTestImageButtonDlg dialog

CTestImageButtonDlg::CTestImageButtonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestImageButtonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestImageButtonDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pGDIPlusImage = NULL;
}

void CTestImageButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestImageButtonDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestImageButtonDlg, CDialog)
	//{{AFX_MSG_MAP(CTestImageButtonDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestImageButtonDlg message handlers

BOOL CTestImageButtonDlg::OnInitDialog()
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
	int iMainWndWidth = 1024;
	int iMainWndHeight = 768;
	SetWindowPos(NULL, 0, 0, iMainWndWidth, iMainWndHeight, SWP_SHOWWINDOW);
	CenterWindow(NULL);

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


	m_pGDIPlusImage = new CStillImage();
	m_pGDIPlusImage->LoadFromFile("D:\\VC Projects\\UIEngine\\BK.png");

/*	
	RECT rc;
	GetClientRect(&rc);
	m_pGDIPlusImage = new CGIFImage();
	m_pGDIPlusImage->LoadFromFile("D:\\VC Projects\\UIEngine\\Test.gif");
	//m_pGDIPlusImage->Draw(m_hWnd, GetDC()->m_hDC, rc);
*/
	RECT btnRc;
	btnRc.left = 100;
	btnRc.top = 100;
	btnRc.right = 233;
	btnRc.bottom = 205;
	m_Button.Create(_T(""), WS_CHILD | WS_VISIBLE, btnRc, this, 0, 
					"D:\\VC Projects\\UIEngine\\Btn_Baby01.png", 
					"D:\\VC Projects\\UIEngine\\Btn_Baby02.gif",
					"D:\\VC Projects\\UIEngine\\Btn_Baby03.png");
	m_Button.SetParentBkImage(m_pGDIPlusImage);

	btnRc.left = 100;
	btnRc.top = 250;
	btnRc.right = 187;
	btnRc.bottom = 383;
	m_Button2.Create(_T(""), WS_CHILD | WS_VISIBLE, btnRc, this, 0, 
					"D:\\VC Projects\\UIEngine\\Btn_Story01.png", 
					"D:\\VC Projects\\UIEngine\\Btn_Story02.gif",
					"D:\\VC Projects\\UIEngine\\Btn_Story03.png");
	m_Button2.SetParentBkImage(m_pGDIPlusImage);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestImageButtonDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestImageButtonDlg::OnPaint() 
{
/*
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
*/

	

	CPaintDC dc(this);
/*
	RECT rcRefresh;
	dc.GetClipBox(&rcRefresh);
*/
	RECT rc;
	GetClientRect(&rc);

	if(NULL != m_pGDIPlusImage)
		m_pGDIPlusImage->Draw(m_hWnd, dc.m_hDC, rc, rc);
		//	m_pGDIPlusImage->Draw(m_hWnd, dc.m_hDC, rc);

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestImageButtonDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestImageButtonDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if(NULL != m_pGDIPlusImage)
	{
		delete m_pGDIPlusImage;
		m_pGDIPlusImage = NULL;
	}
	
	GdiplusShutdown(gdiplusToken);	
}
