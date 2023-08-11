// TestMhtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestMht.h"
#include "TestMhtDlg.h"
#include ".\testmhtdlg.h"

#include "cdoex.h"
#include "msado15.h"
using namespace CDO;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestMhtDlg dialog



CTestMhtDlg::CTestMhtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestMhtDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestMhtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestMhtDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CTestMhtDlg message handlers

BOOL CTestMhtDlg::OnInitDialog()
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

void CTestMhtDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestMhtDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestMhtDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void SaveWholePage(BSTR bstrUrl, BSTR bstrFileName)
{
	CoInitialize(NULL);
	IMessage *pMsg;
	IConfiguration *pConfig;
	HRESULT hr = CoCreateInstance(__uuidof(Message), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMessage), (void**)&pMsg);
	hr = CoCreateInstance(__uuidof(Configuration), NULL, CLSCTX_INPROC_SERVER,	__uuidof(IConfiguration), (void**)&pConfig);
	pMsg->put_Configuration(pConfig);

	hr = S_OK;
	try
	{
		CString strNULL = _T("");
		hr = pMsg->CreateMHTMLBody(bstrUrl, cdoSuppressNone, strNULL.AllocSysString(), strNULL.AllocSysString());
		//hr = pMsg->CreateMHTMLBody(L"file://D:/Google.htm", cdoSuppressNone, L"", L"");
	}
	catch(...)
	{
		MessageBox(NULL, _T("Error!"), NULL, MB_OK);
		return;
	}
	ADOStream *pStream;	 
	//_Stream* pStream = NULL;
	hr = pMsg->GetStream(&pStream);     
/*	
	_variant_t varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);
	pStream->put_Type(adTypeText);
	pStream->put_Charset(L"US-ASCII");

	hr = pStream->Open(   varOptional,
                     adModeUnknown,
                     adOpenStreamUnspecified,
                     NULL,
                     NULL);
*/
	hr = pStream->SaveToFile(bstrFileName, adSaveCreateOverWrite);
	hr = (ErrorValueEnum)hr;
	pMsg->Release();
	pStream->Release();
	CoUninitialize();
}


void CTestMhtDlg::OnBnClickedButton1()
{
	CString strURL = _T("file://D:/вш╨опео╒.htm");
	CString strFilename = _T("D:/aaa1.mht");
	SaveWholePage(strURL.AllocSysString(), strFilename.AllocSysString());
}
