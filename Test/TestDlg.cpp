// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include <shlobj.h>
#include <Mmsystem.h>

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
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
	TCHAR szPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);

	m_hBtnBmp = LoadBitmap(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_BTN));

	HMIDIOUT hMidiOut = NULL;
	MMRESULT hr = midiOutOpen(&hMidiOut, (UINT)-1, 0, 0, CALLBACK_NULL); 
	hr = midiOutOpen(&hMidiOut, (UINT)-1, 0, 0, CALLBACK_NULL);
	
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
/*	if (IsIconic())
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

	char szText[] = "张三adfwfasdfasdf";
	int iSize = sizeof(szText);
//	TextOut(dc.m_hDC, 20, 100, szText, iSize-1); 

	Draw(dc.m_hDC, _T("测试"));

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CTestDlg::Draw(HDC hdc, LPCTSTR lpszText)
{
	HDC hMemDc = CreateCompatibleDC(hdc);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDc, m_hBtnBmp);
	int iOldMemBkMode = SetBkMode(hMemDc, TRANSPARENT);	
	COLORREF cOldColor = SetTextColor(hMemDc, RGB(255, 255, 255));

	BITMAP bm;
	bm.bmBits = NULL;
	GetObject(m_hBtnBmp, sizeof(BITMAP), &bm);

	CRect rcBtn(50, 50, 50+bm.bmWidth, 50+bm.bmHeight);

	HFONT hFont = CreateFont(14, 7, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, /*OUT_DEFAULT_PRECIS*/OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS, /*DEFAULT_QUALITY*/PROOF_QUALITY, FF_DONTCARE, 
							 /*_T("隶书")*/_T("隶书"));
	HFONT hOldFont = (HFONT)SelectObject(hMemDc, hFont);

	DrawText(hMemDc, lpszText, _tcslen(lpszText), CRect(0, 0, bm.bmWidth, bm.bmHeight), DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	
	StretchBlt(hdc, rcBtn.left, rcBtn.right, rcBtn.Width(), rcBtn.Height(),
			 hMemDc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	SetBkMode(hMemDc, iOldMemBkMode);
	SelectObject(hMemDc, hOldFont);
	SelectObject(hMemDc, hOldBmp);
	DeleteDC(hMemDc);
}