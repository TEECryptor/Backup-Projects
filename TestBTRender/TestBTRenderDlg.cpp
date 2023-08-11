// TestBTRenderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestBTRender.h"
#include "TestBTRenderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//------------------------------------------------
CTestBTRenderDlg*	g_pDlg = NULL;
CBTObject*			g_Obj = NULL;
CBTTexture*			g_Tex = NULL;
//------------------------------------------------
//	Name:GameIslandFrameFunc
//	Introduce:Frame callback function
//------------------------------------------------
bool GameIslandFrameFunc()
{
	if(NULL != g_pDlg)
	{
		return g_pDlg->FrameFunc();
	}

	return true;
}
//------------------------------------------------
//	Name:GameIslandRenderFunc
//	Introduce:Render callback function
//------------------------------------------------
bool GameIslandRenderFunc()
{
	if(NULL != g_pDlg)
	{
		return g_pDlg->RenderFunc();
	}

	return true;
}

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
// CTestBTRenderDlg dialog

CTestBTRenderDlg::CTestBTRenderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestBTRenderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestBTRenderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	g_pDlg = this;
}

void CTestBTRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestBTRenderDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestBTRenderDlg, CDialog)
	//{{AFX_MSG_MAP(CTestBTRenderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestBTRenderDlg message handlers

BOOL CTestBTRenderDlg::OnInitDialog()
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
	m_pRender = BTRender_CreateRender();
	m_pRender->Create();
	
	m_pRender->SetState(BT_LOGFILE, "GameIsland.log");
	m_pRender->SetState(BT_FRAMEFUNC, GameIslandFrameFunc);
	m_pRender->SetState(BT_RENDERFUNC, GameIslandRenderFunc);
	m_pRender->SetState(BT_FPS, 35);
	m_pRender->SetState(BT_WINDOWED, true);
	m_pRender->SetState(BT_HWNDPARENT, m_hWnd);
	m_pRender->SetState(BT_HIDEMOUSE, false);
	m_pRender->SetState(BT_SCREENWIDTH, 800);
	m_pRender->SetState(BT_SCREENHEIGHT, 600);
	m_pRender->SetState(BT_SCREENBPP, 32);

	BOOL bRes = FALSE;
	bRes = m_pRender->Initiate();

	g_Tex =	BTRender_CreateTexture();
	g_Tex->SetTexFilePath(_T("D:\\VC Projects\\TestBTRender\\Debug\\"));
	g_Tex->LoadFromFile(_T("aa"), m_pRender->GetRenderHandle(), _T("Tech.dtx"), TRUE);

	g_Obj = BTRender_CreateObject();
	g_Obj->Create(_T("a"), 0, 0, 392, 510);
	g_Obj->SetTexture(g_Tex);
	g_Obj->Play(BTOAT_TEXCHANGE, BTOAT_TEXCHANGE, BTOAT_NONE);
	SetTimer(0, 33, NULL);

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestBTRenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestBTRenderDlg::OnPaint() 
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
HCURSOR CTestBTRenderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


bool CTestBTRenderDlg::FrameFunc()
{	
	float fDelta = m_pRender->GetTimeDelta();
	g_Obj->UpdateTimeDelta(fDelta);

//	g_Obj->UpdateTimeDelta(0.033);

	return false;
}

bool CTestBTRenderDlg::RenderFunc()
{
	m_pRender->BeginScene();
	m_pRender->Clear(0xFFF0000);

	g_Obj->Render();

	m_pRender->EndScene();

	return false;
}

void CTestBTRenderDlg::OnTimer(UINT nIDEvent) 
{
	m_pRender->RenderStart();
	
	CDialog::OnTimer(nIDEvent);
}
