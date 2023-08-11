// TestMatrixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestMatrix.h"
#include "TestMatrixDlg.h"
#include "MatrixSetHelper.h"

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
// CTestMatrixDlg dialog

CTestMatrixDlg::CTestMatrixDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestMatrixDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestMatrixDlg)
	m_iMaxAroundCnt = 0;
	m_iColCnt = 0;
	m_iIDCount = 0;
	m_iMaxLineCnt = 0;
	m_iRowCnt = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestMatrixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestMatrixDlg)
	DDX_Text(pDX, IDC_EDIT_AROUNDCNT, m_iMaxAroundCnt);
	DDX_Text(pDX, IDC_EDIT_COL, m_iColCnt);
	DDX_Text(pDX, IDC_EDIT_FINDCNT, m_iIDCount);
	DDX_Text(pDX, IDC_EDIT_LINECNT, m_iMaxLineCnt);
	DDX_Text(pDX, IDC_EDIT_ROW, m_iRowCnt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestMatrixDlg, CDialog)
	//{{AFX_MSG_MAP(CTestMatrixDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_FIND, OnBtnFind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestMatrixDlg message handlers

BOOL CTestMatrixDlg::OnInitDialog()
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
	CListCtrl* pList = ((CListCtrl*)GetDlgItem(IDC_LIST_RESULT));
	pList->SetExtendedStyle(LVS_EX_GRIDLINES);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestMatrixDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestMatrixDlg::OnPaint() 
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
HCURSOR CTestMatrixDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CTestMatrixDlg::OnBtnFind() 
{
	UpdateData();

	CListCtrl* pList = ((CListCtrl*)GetDlgItem(IDC_LIST_RESULT));
	pList->DeleteAllItems();
	
	int iColumnCount = pList->GetHeaderCtrl()->GetItemCount();
	for(int iCol = 0; iCol < iColumnCount; iCol++)
		pList->DeleteColumn(0);

	pList->InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	for(iCol = 0; iCol < m_iIDCount; iCol++)
	{
		TCHAR szID[8] = {_T("\0")};
		_stprintf(szID, _T("%d"), iCol);

		pList->InsertColumn(iCol+1, szID, LVCFMT_LEFT, 50);
	}	

	int* pResult = NULL;
	CMatrixSetHelper MSHelper;
	int iResultCnt = MSHelper.FindCorrentIDs(m_iRowCnt, m_iColCnt, m_iIDCount, m_iMaxLineCnt, m_iMaxAroundCnt, pResult);

	if(iResultCnt > 0)
	{
		for(int iRes = 0; iRes < iResultCnt; iRes++)
		{
			TCHAR szNo[8] = {_T("\0")};
			_stprintf(szNo, _T("%d"), iRes);
			pList->InsertItem(iRes, szNo);

			for(int iID = 0; iID < m_iIDCount; iID++)
			{
				TCHAR szID[8] = {_T("\0")};
				_stprintf(szID, _T("%d"), pResult[iRes*m_iIDCount + iID]);
				
				pList->SetItemText(iRes, iID+1, szID);
			}
		}

		delete []pResult;
		pResult = NULL;
	}
}
