// HandDiscInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HandDiscInfo.h"
#include "HandDiscInfoDlg.h"
#include "HandDiscInfoHelper.h"

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
// CHandDiscInfoDlg dialog

CHandDiscInfoDlg::CHandDiscInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHandDiscInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHandDiscInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHandDiscInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHandDiscInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHandDiscInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CHandDiscInfoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHandDiscInfoDlg message handlers

BOOL CHandDiscInfoDlg::OnInitDialog()
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

void CHandDiscInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHandDiscInfoDlg::OnPaint() 
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
HCURSOR CHandDiscInfoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



int CHandDiscInfoDlg::GetHDInfo(LPTSTR lpszInfo, int iDriver)
{
	TCHAR szFilePath[MAX_PATH] = {_T("\0")};
	_stprintf(szFilePath, _T("\\\\.\\PHYSICALDRIVE%d"), iDriver);

	HANDLE hFile = INVALID_HANDLE_VALUE; 
	hFile = CreateFile(	szFilePath, 
						GENERIC_READ|GENERIC_WRITE, 
						FILE_SHARE_READ|FILE_SHARE_WRITE, 
						NULL, 
						OPEN_EXISTING, 
						0, NULL); 

	if(INVALID_HANDLE_VALUE == hFile) 
		return -1; 
/*
	DWORD dwBytesReturned; 
	GETVERSIONINPARAMS gvopVersionParams; 
	DeviceIoControl(hFile, SMART_GET_VERSION, NULL, 0, &gvopVersionParams, sizeof(gvopVersionParams), &dwBytesReturned, NULL); 
	if(gvopVersionParams.bIDEDeviceMap <= 0) 
		return -2; 

	//IDE or ATAPI IDENTIFY cmd 
	int btIDCmd = 0; 
	SENDCMDINPARAMS InParams; 
	int nDrive = 0; 
	btIDCmd = (gvopVersionParams.bIDEDeviceMap >> nDrive & 0x10) ? IDE_ATAPI_IDENTIFY:IDE_ATA_IDENTIFY; 


	//输出参数 
	BYTE btIDOutCmd[sizeof(SENDCMDOUTPARAMS)+IDENTIFY_BUFFER_SIZ -1]; 
	if(DoIdentify(hFile, &InParams, (PSENDCMDOUTPARAMS)btIDOutCmd, (BYTE)btIDCmd, (BYTE)nDrive, &dwBytesReturned) == FALSE) 
		return -3; 
	::CloseHandle(hFile); 

	DWORD dwDiskData[256]; 
	USHORT *pIDSector;   //对应结构IDSECTOR，见头文件 

	pIDSector = (USHORT*)((SENDCMDOUTPARAMS*)btIDOutCmd)->bBuffer; 
	for(int i=0; i < 256; i++) 
		dwDiskData[i] = pIDSector[i]; 

	//取系列号 
	ZeroMemory(szSerialNumber, sizeof(szSerialNumber)); 
	strcpy(szSerialNumber, ConvertToString(dwDiskData, 10, 19)); 

	//取模型号 
	ZeroMemory(szModelNumber,  sizeof(szModelNumber)); 
	strcpy(szModelNumber, ConvertToString(dwDiskData, 27, 46)); 
*/
	return   0; 
}