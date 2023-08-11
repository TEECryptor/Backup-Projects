// TestMediaHostDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestMediaHost.h"
#include "TestMediaHostDlg.h"

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestMediaHostDlg dialog

CTestMediaHostDlg::CTestMediaHostDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestMediaHostDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestMediaHostDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pBKMusic = NULL;
	m_pBtnMusic = NULL;
}

void CTestMediaHostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestMediaHostDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestMediaHostDlg, CDialog)
	//{{AFX_MSG_MAP(CTestMediaHostDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPENBK, OnBtnOpenbk)
	ON_BN_CLICKED(IDC_BTN_PLAYBK, OnBtnPlaybk)
	ON_BN_CLICKED(IDC_BTN_PAUSEBK, OnBtnPausebk)
	ON_BN_CLICKED(IDC_BTN_STOPBK, OnBtnStopbk)
	ON_BN_CLICKED(IDC_BTN_OPENBT, OnBtnOpenbt)
	ON_BN_CLICKED(IDC_BTN_PAUSEBT, OnBtnPausebt)
	ON_BN_CLICKED(IDC_BTN_PLAYBT, OnBtnPlaybt)
	ON_BN_CLICKED(IDC_BTN_STOPBT, OnBtnStopbt)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestMediaHostDlg message handlers

BOOL CTestMediaHostDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestMediaHostDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestMediaHostDlg::OnPaint() 
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
HCURSOR CTestMediaHostDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestMediaHostDlg::OnBtnOpenbk() 
{
	CFileDialog openDlg(TRUE, NULL, NULL, 
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						_T("mp3 Files(*.mp3)|*.mp3|wave Files(*.wav)|*.wav||"),
						NULL);
	if(openDlg.DoModal() == IDOK)
	{
		CString strMusicFile = openDlg.GetPathName();

		if(m_pBKMusic)
			MediaLib_DestroyMusic(m_pBKMusic);
		m_pBKMusic = MediaLib_CreateMusic();

		if(m_pBKMusic)
		{
			m_pBKMusic->LoadFromFile(strMusicFile);

			SetDlgItemText(IDC_STATIC_BK, strMusicFile);
		}
	}	
}

void CTestMediaHostDlg::OnBtnPlaybk() 
{
	if(m_pBKMusic)
		m_pBKMusic->Play();

	SetTimer(0, 1000, NULL);
}

void CTestMediaHostDlg::OnBtnPausebk() 
{
	if(m_pBKMusic)
		m_pBKMusic->Pause();	
}

void CTestMediaHostDlg::OnBtnStopbk() 
{	
	if(m_pBKMusic)
		m_pBKMusic->Stop();	
}

void CTestMediaHostDlg::OnBtnOpenbt() 
{
	CFileDialog openDlg(TRUE, NULL, NULL, 
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						_T("mp3 Files(*.mp3)|*.mp3|wave Files(*.wav)|*.wav||"),
						NULL);
	if(openDlg.DoModal() == IDOK)
	{
		CString strMusicFile = openDlg.GetPathName();

		if(m_pBtnMusic)
			MediaLib_DestroyMusic(m_pBtnMusic);
		m_pBtnMusic = MediaLib_CreateMusic();

		if(m_pBtnMusic)
		{
			m_pBtnMusic->LoadFromFile(strMusicFile);

			SetDlgItemText(IDC_STATIC_BT, strMusicFile);
		}
	}	
}


void CTestMediaHostDlg::OnBtnPlaybt() 
{	
	if(m_pBtnMusic)
		m_pBtnMusic->Play();
}

void CTestMediaHostDlg::OnBtnPausebt() 
{
	if(m_pBtnMusic)
		m_pBtnMusic->Pause();
}

void CTestMediaHostDlg::OnBtnStopbt() 
{
	if(m_pBtnMusic)
		m_pBtnMusic->Stop();	
}

void CTestMediaHostDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_pBKMusic)
	{
		MediaLib_DestroyMusic(m_pBKMusic);
		m_pBKMusic = NULL;
	}

	if(m_pBtnMusic)
	{
		MediaLib_DestroyMusic(m_pBtnMusic);
		m_pBtnMusic = NULL;
	}	
}

void CTestMediaHostDlg::OnTimer(UINT nIDEvent) 
{
	UINT uState = 0;
	if(NULL != m_pBKMusic)
	{
		uState = m_pBKMusic->GetState();
	}
	
	CDialog::OnTimer(nIDEvent);
}
