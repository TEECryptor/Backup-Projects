// TestRollLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestRollLine.h"
#include "TestRollLineDlg.h"
#include <math.h>

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
// CTestRollLineDlg dialog

CTestRollLineDlg::CTestRollLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestRollLineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestRollLineDlg)
	m_iBigR = 300;
	m_iMiniR = 100;
	m_iStartAngle = 90;
	m_iEndAngle = 180;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestRollLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestRollLineDlg)
	DDX_Text(pDX, IDC_EDIT_BIGR, m_iBigR);
	DDX_Text(pDX, IDC_EDIT_MINIR, m_iMiniR);
	DDX_Text(pDX, IDC_EDIT_ANGLE1, m_iStartAngle);
	DDX_Text(pDX, IDC_EDIT_ANGLE2, m_iEndAngle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestRollLineDlg, CDialog)
	//{{AFX_MSG_MAP(CTestRollLineDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestRollLineDlg message handlers

BOOL CTestRollLineDlg::OnInitDialog()
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
	GetDlgItem(IDC_STATIC_RECT)->SetWindowPos(NULL, 11, 11, 500, 500, SWP_SHOWWINDOW);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestRollLineDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestRollLineDlg::OnPaint() 
{	
	CPaintDC dc(this); // device context for painting
			
	RECT rc;	
	GetDlgItem(IDC_STATIC_RECT)->GetClientRect(&rc);
	GetDlgItem(IDC_STATIC_RECT)->ClientToScreen(&rc);
	ScreenToClient(&rc);
	int iWidth = rc.right - rc.left;
	int iHeight = rc.bottom - rc.top;

	HDC hDCMem = CreateCompatibleDC(dc.m_hDC);
	HBITMAP hMemBmp = CreateCompatibleBitmap(dc.m_hDC, iWidth, iHeight);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDCMem, hMemBmp);	
	CBrush brush(RGB(225, 225, 225));
	FillRect(hDCMem, CRect(0, 0, iWidth, iHeight), (HBRUSH)brush);
	
	DrawCoordinate(hDCMem, iWidth, iHeight);
	DrawCircle(hDCMem, iWidth/2, iHeight/2, m_iMiniR);
	DrawArcLine(hDCMem, iWidth/2, iHeight/2, m_iBigR, m_iMiniR);


	BitBlt(	dc.m_hDC, 
			rc.left, rc.top, iWidth, iHeight,
			hDCMem,
			0, 0,
			SRCCOPY);
			
	SelectObject(dc.m_hDC, hOldBitmap);
	DeleteObject(hMemBmp);
	DeleteDC(hDCMem);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestRollLineDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//------------------------------------------
void CTestRollLineDlg::DrawCoordinate(HDC hdc, int iWidth, int iHeight)
{
	MoveToEx(hdc, 0, iHeight/2, NULL);
	LineTo(hdc, iWidth, iHeight/2);
	MoveToEx(hdc, iWidth/2, 0, NULL);
	LineTo(hdc, iWidth/2, iHeight);
}
//------------------------------------------
void CTestRollLineDlg::DrawCircle(HDC hdc, int iPosX, int iPosY, int iRadius)
{
	const float fPI = 3.1415926f;
	const int iDivCnt = 720;

	MoveToEx(hdc, iPosX+iRadius, iPosY, NULL);
	for(int i = 0; i <= iDivCnt; i++)
	{
		float fX = iPosX + iRadius*cosf((360.0f/iDivCnt)/180.0f * fPI * i);
		float fY = iPosY - iRadius*sinf((360.0f/iDivCnt)/180.0f * fPI * i);

		LineTo(hdc, (int)fX, (int)fY);
	}
}
//------------------------------------------
void CTestRollLineDlg::DrawArcLine(HDC hdc, int iPosX, int iPosY, int iStartR, int iEndR)
{
/*
	iStartR = 300;
	iEndR = 100
	m_iStartAngle = 62;
	m_iEndAngle = 180;
*/

	const float fPI = 3.1415926f;
	const int iDivCnt = 720;

	int iStartX = (int)(iStartR*cosf(m_iStartAngle/180.0f * fPI));
	int iStartY = (int)(iStartR*sinf(m_iStartAngle/180.0f * fPI));

	int iEndX = (int)(iEndR*cosf(m_iEndAngle/180.0f * fPI));
	int iEndY = (int)(iEndR*sinf(m_iEndAngle/180.0f * fPI));

	int iPntOnArcX = iStartX - iEndX;
	int iPntOnArcY = iStartY;

	float fC = (iPntOnArcY*iPntOnArcY) / (float)iPntOnArcX; 

	for(int iX = 0; iX <= iPntOnArcX; iX++)
	{
		int x = iPosX + iX + iEndX;
		int y = iPosY - (int)(sqrtf(fC*iX));

		LineTo(hdc, (int)x, (int)y);
	}

}
//------------------------------------------
void CTestRollLineDlg::OnBtnOk() 
{
	UpdateData();
	Invalidate();
}
//------------------------------------------
