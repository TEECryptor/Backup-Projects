// TestXMLLoaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestXMLLoader.h"
#include "TestXMLLoaderDlg.h"

#include "UIEngine.h"
#include "GameInfoLoader.h"

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
// CTestXMLLoaderDlg dialog

CTestXMLLoaderDlg::CTestXMLLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestXMLLoaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestXMLLoaderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	lstrcpy(m_szSkinXMLFile, _T("\0"));
	lstrcpy(m_szGameXMLFile, _T("\0"));
}

void CTestXMLLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestXMLLoaderDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestXMLLoaderDlg, CDialog)
	//{{AFX_MSG_MAP(CTestXMLLoaderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOAD, OnBtnLoad)
	ON_BN_CLICKED(IDC_BTN_OPEN_SKIN, OnBtnOpenSkin)
	ON_BN_CLICKED(IDC_BTN_OPEN_GAME, OnBtnOpenGame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestXMLLoaderDlg message handlers

BOOL CTestXMLLoaderDlg::OnInitDialog()
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
	CoInitialize(NULL);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestXMLLoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestXMLLoaderDlg::OnPaint() 
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
HCURSOR CTestXMLLoaderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//------------------------------------------------



//------------------------------------------------
void CTestXMLLoaderDlg::OnBtnOpenSkin() 
{
	CFileDialog dlgOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("XML Files (*.xml)|*.xml||"));
	if(dlgOpen.DoModal() == IDOK)
	{
		lstrcpy(m_szSkinXMLFile, (LPCTSTR)dlgOpen.GetPathName());
		SetDlgItemText(IDC_STATIC_FILE_SKIN, m_szSkinXMLFile);
	}
}
//------------------------------------------------
void CTestXMLLoaderDlg::OnBtnOpenGame() 
{
	CFileDialog dlgOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("XML Files (*.xml)|*.xml||"));
	if(dlgOpen.DoModal() == IDOK)
	{
		lstrcpy(m_szGameXMLFile, (LPCTSTR)dlgOpen.GetPathName());
		SetDlgItemText(IDC_STATIC_FILE_GAME, m_szGameXMLFile);
	}
}
//------------------------------------------------
void CTestXMLLoaderDlg::OnBtnLoad() 
{
	CUIEngine UIEngine;
	if(lstrlen(m_szSkinXMLFile) > 0)
	{
		if(!UIEngine.SetSkinFile(m_szSkinXMLFile))
			return;
	}

	CGameInfoLoader GameInfoLoader;
	if(lstrlen(m_szGameXMLFile) > 0)
	{
		if(!GameInfoLoader.SetGameInfoFile(m_szGameXMLFile))
			return;
	}
}
//------------------------------------------------