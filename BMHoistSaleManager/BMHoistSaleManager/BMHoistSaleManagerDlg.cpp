//-----------------------------------------------------------------------
//	File Name:BMHoistSaleManagerDlg.cpp
//	Introduce:Class CBMHoistSaleManagerDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMHoistSaleManagerDlg.h"
#include "BMOfferManagerDlg.h"
#include "BMCustomManagerDlg.h"
//-----------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMHoistSaleManagerDlg dialog
//-----------------------------------------------------------------------
CBMHoistSaleManagerDlg::CBMHoistSaleManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(/*CBMHoistSaleManagerDlg::IDD, pParent*/)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pOfferManagerDlg = NULL;
	m_pCustomManagerDlg = NULL;
}
//-----------------------------------------------------------------------
void CBMHoistSaleManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMHoistSaleManagerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_DESTROY()	
	ON_MESSAGE(WM_USER_SHOWCUSTOMER, OnCustomerManage)
	ON_MESSAGE(WM_USER_SHOWOFFERINFO, OnOfferInfoManage)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMHoistSaleManagerDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMHoistSaleManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	const int APPSIZE_X = 1000;
	const int APPSIZE_Y = 680;
	SetWindowPos(NULL, 0, 0, APPSIZE_X, APPSIZE_Y, SWP_SHOWWINDOW);
	CenterWindow();
	//ShowWindow(SW_SHOWMAXIMIZED);

	CRect rcClient;
	GetClientRect(&rcClient);
	m_pOfferManagerDlg = new CBMOfferManagerDlg(this);
	m_pOfferManagerDlg->Create(IDD_DIALOG_OFFERMANAGER, this);
	m_pOfferManagerDlg->MoveWindow(&rcClient);
	m_pOfferManagerDlg->ShowWindow(SW_SHOW);
	m_pOfferManagerDlg->UpdateWindow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}
//-----------------------------------------------------------------------
void CBMHoistSaleManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
//-----------------------------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//-----------------------------------------------------------------------
void CBMHoistSaleManagerDlg::OnPaint()
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
//-----------------------------------------------------------------------
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
//-----------------------------------------------------------------------
HCURSOR CBMHoistSaleManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//-----------------------------------------------------------------------
void CBMHoistSaleManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (NULL != m_hWnd)
	{		
		CRect rcClient;
		GetClientRect(&rcClient);
		if (NULL != m_pOfferManagerDlg)
		{
			m_pOfferManagerDlg->MoveWindow(&rcClient);
			m_pOfferManagerDlg->UpdateWindow();
		}
		if (NULL != m_pCustomManagerDlg)
		{
			m_pCustomManagerDlg->MoveWindow(&rcClient);
			m_pCustomManagerDlg->UpdateWindow();
		}
	}
}
//-----------------------------------------------------------------------
void CBMHoistSaleManagerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (NULL != m_pOfferManagerDlg)
	{
		delete m_pOfferManagerDlg;
		m_pOfferManagerDlg = NULL;
	}
	if (NULL != m_pCustomManagerDlg)
	{
		delete m_pCustomManagerDlg;
		m_pCustomManagerDlg = NULL;
	}
}
//-----------------------------------------------------------------------
HRESULT CBMHoistSaleManagerDlg::OnCustomerManage(WPARAM wParam, LPARAM lParam)
{	
	if (NULL != m_pOfferManagerDlg)
		m_pOfferManagerDlg->ShowWindow(SW_HIDE);

	CRect rcClient;
	GetClientRect(&rcClient);
	
	if (NULL == m_pCustomManagerDlg)
	{
		m_pCustomManagerDlg = new CBMCustomManagerDlg(this);
		m_pCustomManagerDlg->Create(IDD_DIALOG_CUSTOMER, this);
	}
	m_pCustomManagerDlg->MoveWindow(&rcClient);
	m_pCustomManagerDlg->ShowWindow(SW_SHOW);
	m_pCustomManagerDlg->UpdateWindow();

	return 1;
}
//-----------------------------------------------------------------------
HRESULT CBMHoistSaleManagerDlg::OnOfferInfoManage(WPARAM wParam, LPARAM lParam)
{
	if (NULL != m_pCustomManagerDlg)
		m_pCustomManagerDlg->ShowWindow(SW_HIDE);

	CRect rcClient;
	GetClientRect(&rcClient);
	
	if (NULL != m_pOfferManagerDlg)
	{
		m_pOfferManagerDlg->MoveWindow(&rcClient);
		m_pOfferManagerDlg->ShowWindow(SW_SHOW);
		m_pOfferManagerDlg->UpdateWindow();
	}

	return 1;
}
//-----------------------------------------------------------------------
