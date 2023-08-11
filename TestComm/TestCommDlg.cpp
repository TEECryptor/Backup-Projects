// TestCommDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestComm.h"
#include "TestCommDlg.h"

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
// CTestCommDlg dialog

CTestCommDlg::CTestCommDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestCommDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestCommDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hPrinterCom = NULL;
}

void CTestCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestCommDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestCommDlg, CDialog)
	//{{AFX_MSG_MAP(CTestCommDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestCommDlg message handlers

BOOL CTestCommDlg::OnInitDialog()
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

void CTestCommDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestCommDlg::OnPaint() 
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
HCURSOR CTestCommDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//------------------------------------------------------------------
void CTestCommDlg::OnBtnConnect() 
{
	if(NULL != m_hPrinterCom)
		CloseHandle(m_hPrinterCom);

	m_hPrinterCom = CreateFile(	_T("COM1"),						//COM1��
								GENERIC_READ|GENERIC_WRITE, //�������д
								0,							//��ռ��ʽ
								NULL,
								OPEN_EXISTING,				//�򿪶����Ǵ���
								0,							//ͬ����ʽ
								NULL);

	if(m_hPrinterCom == (HANDLE)-1 )
	{
		SetDlgItemText(IDC_STATIC_STATE, _T("COM����ʧ�ܣ�"));
		return;
	}

	SetDlgItemText(IDC_STATIC_STATE, _T("COM���ӳɹ���"));

	//Set the buffer size
	SetupComm(m_hPrinterCom, 512, 512);
	
	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ
	//�ڶ�һ�����뻺���������ݺ���������������أ�
	//�������Ƿ������Ҫ����ַ���
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;

	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 100;
	TimeOuts.WriteTotalTimeoutConstant = 500;

	//���ó�ʱ
	SetCommTimeouts(m_hPrinterCom, &TimeOuts); 

	DCB dcb;
	GetCommState(m_hPrinterCom, &dcb);
	dcb.BaudRate = 9600;				//������Ϊ9600
	dcb.ByteSize = 8;					//ÿ���ֽ���8λ
	dcb.Parity = NOPARITY;				//����żУ��λ
	dcb.StopBits = TWOSTOPBITS;			//����ֹͣλ
	SetCommState(m_hPrinterCom,&dcb);

	PurgeComm(m_hPrinterCom, PURGE_TXCLEAR|PURGE_RXCLEAR);
}
//------------------------------------------------------------------
void CTestCommDlg::OnBtnSend() 
{/*
	char lpOutBuffer[7];
	memset(lpOutBuffer, '\0', 7);	//ǰ7���ֽ�������
	lpOutBuffer[0] = '\x11';		//���ͻ������ĵ�1���ֽ�ΪDC1
	lpOutBuffer[1] = '0';			//��2���ֽ�Ϊ�ַ�0(30H)
	lpOutBuffer[2] = '0';			//��3���ֽ�Ϊ�ַ�0(30H)
	lpOutBuffer[3] = '1';			// ��4���ֽ�Ϊ�ַ�1(31H)
	lpOutBuffer[4] = '0';			//��5���ֽ�Ϊ�ַ�0(30H)
	lpOutBuffer[5] = '1';			//��6���ֽ�Ϊ�ַ�1(31H)
	lpOutBuffer[6] = '\x03';		//��7���ֽ�Ϊ�ַ�ETX
*/
	char lpOutBuffer[3];
	memset(lpOutBuffer, '\0', 3);	//ǰ3���ֽ�������
	lpOutBuffer[0] = '\x10';		//���ͻ������ĵ�1���ֽ�ΪDLE(10H)
	lpOutBuffer[1] = '\x04';		//��2���ֽ�Ϊ�ַ�EOT(04H)
	lpOutBuffer[2] = '\x02';		//��3���ֽ�Ϊ�ַ�n
	
	BYTE bBuffer[3];
	memset(bBuffer, 0, 3);			//ǰ3���ֽ�������
	bBuffer[0] = 0x10;				//���ͻ������ĵ�1���ֽ�ΪDLE(10H)
	bBuffer[1] = 0x04;				//��2���ֽ�Ϊ�ַ�EOT(04H)
	bBuffer[2] = 0x02;				//��3���ֽ�Ϊ�ַ�n

/*
	BYTE bBuffer[5];
	memset(bBuffer, 0, 5);
	bBuffer[0] = 0x1B;		
	bBuffer[1] = 0x70;		
	bBuffer[2] = 0x01;		
	bBuffer[3] = 0x03;		
	bBuffer[4] = 0x01;			
*/
	//�Ӹöδ�����Կ������Ǳ��ͨѶ��ַΪ001	
	DWORD dwBytesWrite = 3;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	ClearCommError(m_hPrinterCom, &dwErrorFlags, &ComStat);
	bWriteStat = WriteFile(m_hPrinterCom, /*lpOutBuffer*/bBuffer, dwBytesWrite, &dwBytesWrite, NULL);
	if(!bWriteStat)
	{
		AfxMessageBox("д����ʧ��!");
	}	
/*
	char str[100];
	memset(str, '\0', 100);*/
	BYTE bReadBuff[16] = {0};
	DWORD wCount = 100;//��ȡ���ֽ���
	BOOL bReadStat;
	bReadStat = ReadFile(m_hPrinterCom, bReadBuff, wCount, &wCount,NULL);
	if(!bReadStat)
		AfxMessageBox("������ʧ��!");
	PurgeComm(m_hPrinterCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
}
//------------------------------------------------------------------
