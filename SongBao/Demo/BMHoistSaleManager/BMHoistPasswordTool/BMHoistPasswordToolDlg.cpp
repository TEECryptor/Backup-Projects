//----------------------------------------------------
// BMHoistPasswordToolDlg.cpp : implementation file
//
//----------------------------------------------------
#include "stdafx.h"
#include "BMHoistPasswordTool.h"
#include "BMHoistPasswordToolDlg.h"
#include "..\Common\PasswordHelper.h"
//----------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//----------------------------------------------------



//----------------------------------------------------
// CAboutDlg dialog used for App About
//----------------------------------------------------
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
//----------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}
//----------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//----------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
//----------------------------------------------------




//----------------------------------------------------
// CBMHoistPasswordToolDlg dialog
//----------------------------------------------------
CBMHoistPasswordToolDlg::CBMHoistPasswordToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMHoistPasswordToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//----------------------------------------------------
void CBMHoistPasswordToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//----------------------------------------------------
BEGIN_MESSAGE_MAP(CBMHoistPasswordToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_BROWSER, &CBMHoistPasswordToolDlg::OnBnClickedBtnBrowser)
//	ON_BN_CLICKED(IDOK, &CBMHoistPasswordToolDlg::OnBnClickedOk)
ON_BN_CLICKED(IDC_BTN_CREATE, &CBMHoistPasswordToolDlg::OnBnClickedBtnCreate)
END_MESSAGE_MAP()
//----------------------------------------------------


//----------------------------------------------------
// CBMHoistPasswordToolDlg message handlers
//----------------------------------------------------
BOOL CBMHoistPasswordToolDlg::OnInitDialog()
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
//----------------------------------------------------
void CBMHoistPasswordToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//----------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//----------------------------------------------------
void CBMHoistPasswordToolDlg::OnPaint()
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
//----------------------------------------------------
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
//----------------------------------------------------
HCURSOR CBMHoistPasswordToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//----------------------------------------------------
void CBMHoistPasswordToolDlg::OnBnClickedBtnBrowser()
{
	BROWSEINFO   bi;   
	memset(&bi, 0, sizeof(bi));
	bi.hwndOwner		= m_hWnd;   
	bi.pidlRoot			= NULL;   
	bi.pszDisplayName   = NULL;    
	bi.lpszTitle		= _T("选择目录");    
	bi.ulFlags			= BIF_BROWSEFORCOMPUTER & BIF_RETURNONLYFSDIRS;   
	bi.lpfn				= NULL;   
	bi.lParam			= NULL;   

	LPITEMIDLIST   pidl;   
	if(pidl = SHBrowseForFolder(&bi))
	{   
		TCHAR tcPath[MAX_PATH] = {0};
		SHGetPathFromIDList(pidl, tcPath); 
		//
		CString strPswFile = tcPath;
		strPswFile += _T("\\BMCert.dat");
		SetDlgItemText(IDC_EDIT_FILENAME, strPswFile);
	} 
}
//----------------------------------------------------
void CBMHoistPasswordToolDlg::OnBnClickedBtnCreate()
{
	CString strPassword;
	CString strConfirmPsw;
	GetDlgItemText(IDC_EDIT_SETPASSWORD, strPassword);
	GetDlgItemText(IDC_EDIT_CONFIRMPASSWORD, strConfirmPsw);
	if (strPassword.IsEmpty())
	{
		MessageBox(_T("密码不能为空，请重新输入！"), _T("错误"), MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_SETPASSWORD)->SetFocus();
	}

	if (strPassword != strConfirmPsw)
	{
		MessageBox(_T("两次输入的密码不一致，请重新输入！"), _T("错误"), MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_SETPASSWORD)->SetFocus();
	}

	CString strFileName;
	GetDlgItemText(IDC_EDIT_FILENAME, strFileName);
	if (strFileName.IsEmpty())
	{
		MessageBox(_T("请选择密码文件的路径！"), _T("错误"), MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_FILENAME)->SetFocus();
	}

	LPCWSTR lpszEncodedPsw = CPasswordHelper::EncodePasswordString(strPassword);
	
	FILE* file = _wfopen(strFileName.GetBuffer(strFileName.GetLength()), L"wb");
	if (file)
	{
		fwrite(lpszEncodedPsw, sizeof(WCHAR), wcslen(lpszEncodedPsw), file);
		fclose(file);

		MessageBox(_T("密码文件创建成功！"), _T("完成"), MB_OK | MB_ICONINFORMATION);
	}
}
//----------------------------------------------------
