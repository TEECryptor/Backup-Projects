// TestXMLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestXML.h"
#include "TestXMLDlg.h"
//------------------------------------------------	
#import "msxml4.dll" 
using namespace MSXML2;
//------------------------------------------------	
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//------------------------------------------------
// CAboutDlg dialog used for App About
//------------------------------------------------
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
// CTestXMLDlg dialog

CTestXMLDlg::CTestXMLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestXMLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestXMLDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestXMLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestXMLDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestXMLDlg, CDialog)
	//{{AFX_MSG_MAP(CTestXMLDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestXMLDlg message handlers

BOOL CTestXMLDlg::OnInitDialog()
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

void CTestXMLDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestXMLDlg::OnPaint() 
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
HCURSOR CTestXMLDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestXMLDlg::OnBtnSave() 
{
	MSXML2::IXMLDOMDocumentPtr	pXMLDoc;
	MSXML2::IXMLDOMNodePtr pRootNode;
	MSXML2::IXMLDOMNodePtr pNode;
	MSXML2::IXMLDOMElementPtr pElement;

	HRESULT hr = S_OK;
	hr = CoInitialize(NULL);
	if(FAILED(hr))
		return;

	if(NULL == pXMLDoc)
	{
		hr = pXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
		if(FAILED(hr)) 
			return;
	}
	pXMLDoc->async = FALSE;  

	//The root
	pRootNode = pXMLDoc->createElement(_bstr_t("Games"));
	pXMLDoc->appendChild(pRootNode);

	//The "Game" node
	pNode = pXMLDoc->createElement(_bstr_t("Game"));
	pRootNode->appendChild(pNode);
	pElement = pNode;
	pElement->setAttribute("gameid", _variant_t((long)1));
	pElement->setAttribute("factorid", _variant_t((long)1));

	//
	pRootNode = pNode;
	pNode = pXMLDoc->createElement(_bstr_t("Result"));
	pRootNode->appendChild(pNode);
	pElement = pNode;
	pElement->setAttribute("Level", _variant_t((long)1));
	pElement->setAttribute("Time", _variant_t((long)275));
	pElement->setAttribute("Amount", _variant_t((long)1));
	pElement->setAttribute("RightCnt", _variant_t((long)1));
	pElement->setAttribute("Pass", _variant_t((long)1));

	_bstr_t bstrXML = pXMLDoc->xml;

	_variant_t varString = _T("C:\\sample.xml");

	pXMLDoc->save(varString);
}
