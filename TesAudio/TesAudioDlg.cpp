// TesAudioDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TesAudio.h"
#include "TesAudioDlg.h"

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
// CTesAudioDlg dialog

CTesAudioDlg::CTesAudioDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTesAudioDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTesAudioDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_uMusicState = MS_Stopped;
}

void CTesAudioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTesAudioDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTesAudioDlg, CDialog)
	//{{AFX_MSG_MAP(CTesAudioDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBtnOpen)
	ON_BN_CLICKED(IDC_BTN_PLAY, OnBtnPlay)
	ON_BN_CLICKED(IDC_BTN_PAUSE, OnBtnPause)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTesAudioDlg message handlers

BOOL CTesAudioDlg::OnInitDialog()
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
	
    CoInitialize(NULL);
	InitDirectShow();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTesAudioDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTesAudioDlg::OnPaint() 
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
HCURSOR CTesAudioDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//------------------------------------------------------
HRESULT	CTesAudioDlg::InitDirectShow()
{
	HRESULT hr = S_OK;

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, 
                          IID_IGraphBuilder, (void **)&m_pGraphBuilder);
    if(FAILED(hr))
        return hr;

	if(FAILED(m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl)))
        return hr;

    return S_OK;
}
//------------------------------------------------------
void CTesAudioDlg::OnBtnOpen() 
{
	CFileDialog openDlg(TRUE, NULL, NULL, 
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						_T("mp3 Files(*.mp3)|*.mp3|wave Files(*.wav)|*.wav||"),
						NULL);
	if(openDlg.DoModal() == IDOK)
	{
		m_strMusicFile = openDlg.GetPathName();

		SetDlgItemText(IDC_STATIC_FILE, m_strMusicFile);
	}
}
//------------------------------------------------------
void CTesAudioDlg::OnBtnPlay() 
{
	HRESULT hr = S_OK;

	wchar_t wcsMusicFile[MAX_PATH] = {0};
	MultiByteToWideChar(CP_ACP, 0, m_strMusicFile.GetBuffer(m_strMusicFile.GetLength()), m_strMusicFile.GetLength(), wcsMusicFile, MAX_PATH);
	m_strMusicFile.ReleaseBuffer();

	hr = m_pGraphBuilder->RenderFile(wcsMusicFile, NULL);
	if(FAILED(hr))
		return;

	hr = m_pMediaControl->Run();
	if(FAILED(hr))
		return;

	m_uMusicState = MS_Playing;
}
//------------------------------------------------------
void CTesAudioDlg::OnBtnPause() 
{
    if (!m_pGraphBuilder)
        return;

    if(m_uMusicState != MS_Playing)
        return;

    HRESULT hr=S_OK;
    hr = m_pMediaControl->Pause();
    if (FAILED(hr))
        return;

    m_uMusicState = MS_Paused;	
}
//------------------------------------------------------
void CTesAudioDlg::OnBtnStop() 
{	
    if (!m_pGraphBuilder)
        return;

    if (m_uMusicState == MS_Stopped)
        return;

    HRESULT hr = S_OK;
    hr = m_pMediaControl->Stop();
	if(FAILED(hr))
		return;

	m_uMusicState = MS_Stopped;
}
//------------------------------------------------------
