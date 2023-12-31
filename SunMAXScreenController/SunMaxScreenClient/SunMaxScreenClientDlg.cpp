//-------------------------------------------------------------------------
// SunMaxScreenClientDlg.cpp : implementation file
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "SunMaxScreenClient.h"
#include "SunMaxScreenClientDlg.h"
//-------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
// CAboutDlg dialog used for App About
//-------------------------------------------------------------------------
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
//-------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}
//-------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
//-------------------------------------------------------------------------



//-------------------------------------------------------------------------
// CSunMaxScreenClientDlg dialog
//-------------------------------------------------------------------------
CSunMaxScreenClientDlg::CSunMaxScreenClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSunMaxScreenClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bConnectIsShown = TRUE;
	m_bIsConnected = FALSE;
}
//-------------------------------------------------------------------------
void CSunMaxScreenClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CSunMaxScreenClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CSunMaxScreenClientDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_PROJECT, &CSunMaxScreenClientDlg::OnBnClickedBtnProject)
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CSunMaxScreenClientDlg::OnBnClickedBtnRefresh)
	ON_BN_CLICKED(IDOK, &CSunMaxScreenClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SEND, &CSunMaxScreenClientDlg::OnBnClickedBtnSend)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_CONNECT, &CSunMaxScreenClientDlg::OnNMClickSyslinkConnect)
END_MESSAGE_MAP()
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
// CSunMaxScreenClientDlg message handlers
//-------------------------------------------------------------------------
BOOL CSunMaxScreenClientDlg::OnInitDialog()
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

	TCHAR tcExePath[MAX_PATH] = {_T("\0")};
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	m_strCfgFileName = tcExePath;
	m_strCfgFileName = m_strCfgFileName.Left(m_strCfgFileName.ReverseFind(_T('\\')));
	m_strCfgFileName += _T("\\ClientCfg.ini");
	//
	TCHAR tcServerIP[64] = {_T("\0")};
	TCHAR tcServerPort[64] = {_T("\0")};
	GetPrivateProfileString(_T("ServerInfo"), _T("IP"), _T("0.0.0.0"), tcServerIP, 64, m_strCfgFileName);
	GetPrivateProfileString(_T("ServerInfo"), _T("Port"), _T("8000"), tcServerPort, 64, m_strCfgFileName);
	int iTestOptionEnabled = GetPrivateProfileInt(_T("TestOptionSetting"), _T("Enabled"), 1, m_strCfgFileName);
	//
	CString strField;
	CString strServerIP = tcServerIP;
	BYTE btIPField0, btIPField1, btIPField2, btIPField3;
	strField = strServerIP.Left(strServerIP.Find(_T('.')));
	strServerIP = strServerIP.Right(strServerIP.GetLength() - (strServerIP.Find(_T('.'))+1));
	btIPField0 = _ttoi(strField);
	//
	strField = strServerIP.Left(strServerIP.Find(_T('.')));
	strServerIP = strServerIP.Right(strServerIP.GetLength() - (strServerIP.Find(_T('.'))+1));
	btIPField1 = _ttoi(strField);
	//
	strField = strServerIP.Left(strServerIP.Find(_T('.')));
	strServerIP = strServerIP.Right(strServerIP.GetLength() - (strServerIP.Find(_T('.'))+1));
	btIPField2 = _ttoi(strField);
	//
	strField = strServerIP;
	btIPField3 = _ttoi(strField);

	// Set the configure setting
	CIPAddressCtrl* pIPAddressCtrl = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SERVER);
	pIPAddressCtrl->SetAddress(btIPField0, btIPField1, btIPField2, btIPField3);
	SetDlgItemText(IDC_EDIT_PORT, tcServerPort);
/*
	// Fill CMD combobox
	CComboBox* pCmdComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CMD);
	pCmdComboBox->AddString(_T("SWCK"));
	pCmdComboBox->AddString(_T("ZJXM"));
	pCmdComboBox->AddString(_T("YJXC"));
	pCmdComboBox->SetCurSel(0);
	if (0 == iTestOptionEnabled)
	{
		pCmdComboBox->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_URL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
	}
*/
	// Hide "Connect Server" in default
	HRESULT hr = 0;
	OnNMClickSyslinkConnect(NULL, &hr);

	// Initialize Message Sender
	SMS_Init();

	return TRUE;  // return TRUE  unless you set the focus to a control
}
//-------------------------------------------------------------------------
void CSunMaxScreenClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//-------------------------------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//-------------------------------------------------------------------------
void CSunMaxScreenClientDlg::OnPaint()
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
//-------------------------------------------------------------------------
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
//-------------------------------------------------------------------------
HCURSOR CSunMaxScreenClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
void CSunMaxScreenClientDlg::OnBnClickedBtnConnect()
{
	char csServerIP[64] = {0};
	CString strServerPort;
	int iServerPort = 8000;
	BYTE btIPField0, btIPField1, btIPField2, btIPField3;
	CIPAddressCtrl* pIPAddressCtrl = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SERVER);
	pIPAddressCtrl->GetAddress(btIPField0, btIPField1, btIPField2, btIPField3);
	sprintf(csServerIP, "%d.%d.%d.%d", btIPField0, btIPField1, btIPField2, btIPField3);
	GetDlgItemText(IDC_EDIT_PORT, strServerPort);
	iServerPort = _ttoi(strServerPort);

	if (0 != SMS_ConnectServer(csServerIP, iServerPort))
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("连接服务器失败！"));
	}
	else
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("连接服务器成功！"));
		//
		CString strServerIP;
#ifndef UNICODE
		strServerIP = csServerIP;
#else
		MultiByteToWideChar(CP_ACP, 0, csServerIP, -1, strServerIP.GetBuffer(64), 64);
#endif	//UNICODE
		WritePrivateProfileString(_T("ServerInfo"), _T("IP"), strServerIP, m_strCfgFileName);
		WritePrivateProfileString(_T("ServerInfo"), _T("Port"), strServerPort, m_strCfgFileName);
		//
		GetDlgItem(IDC_BTN_PROJECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_REFRESH)->EnableWindow(TRUE);
	}
}
//-------------------------------------------------------------------------
void CSunMaxScreenClientDlg::OnBnClickedBtnProject()
{
	const char csProjectCmd[] = "YCCK:\0";
	SMS_SendMsg(csProjectCmd, strlen(csProjectCmd));
}
//-------------------------------------------------------------------------
void CSunMaxScreenClientDlg::OnBnClickedBtnRefresh()
{
	const char csProjectCmd[] = "QZSX:\0";
	SMS_SendMsg(csProjectCmd, strlen(csProjectCmd));
}
//-------------------------------------------------------------------------
void CSunMaxScreenClientDlg::OnBnClickedOk()
{
	SMS_Disconnect();
	SMS_Exit();
	OnOK();
}
//-------------------------------------------------------------------------
void CSunMaxScreenClientDlg::OnBnClickedBtnSend()
{
	CString strCmdText;
	CString strCmdParam;
	GetDlgItemText(IDC_COMBO_CMD, strCmdText);
	GetDlgItemText(IDC_EDIT_URL, strCmdParam);
	//
	CString strCmdMsg;
	strCmdMsg = strCmdText;
	strCmdMsg += _T(":");
	strCmdMsg += strCmdParam;

	char csCmdMsg[1024] = {0};
#ifndef	UNICODE
	strcpy(csCmdMsg, (LPCTSTR)strCmdMsg);
#else
	int iLen = WideCharToMultiByte(CP_ACP, 0, strCmdMsg, strCmdMsg.GetLength(), csCmdMsg, 1024, NULL, NULL);
	csCmdMsg[iLen] = '\0';
#endif	//UNICODE

	SMS_SendMsg(csCmdMsg, strlen(csCmdMsg));
}
//-------------------------------------------------------------------------
void CSunMaxScreenClientDlg::OnNMClickSyslinkConnect(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (TRUE == m_bConnectIsShown)
	{
		m_bConnectIsShown = FALSE;
		SetDlgItemText(IDC_SYSLINK_CONNECT, _T("<a>连接服务器 >></a>"));
		//
		CRect rcWnd;
		GetWindowRect(&rcWnd);
		rcWnd.bottom -= 120;
		MoveWindow(&rcWnd);
	}
	else
	{
		SetDlgItemText(IDC_SYSLINK_CONNECT, _T("<a>连接服务器 <<</a>"));
		m_bConnectIsShown = TRUE;
		//
		CRect rcWnd;
		GetWindowRect(&rcWnd);
		rcWnd.bottom += 120;
		MoveWindow(&rcWnd);
	}

	*pResult = 0;
}
//-------------------------------------------------------------------------
