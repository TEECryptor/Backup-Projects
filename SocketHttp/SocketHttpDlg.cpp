// SocketHttpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SocketHttp.h"
#include "SocketHttpDlg.h"

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


// CSocketHttpDlg dialog



CSocketHttpDlg::CSocketHttpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketHttpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Socket = 0;
	m_pFile = NULL;
}

void CSocketHttpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSocketHttpDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CSocketHttpDlg message handlers

BOOL CSocketHttpDlg::OnInitDialog()
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
	InitSocket();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSocketHttpDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSocketHttpDlg::OnPaint() 
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
HCURSOR CSocketHttpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CSocketHttpDlg::InitSocket()
{ 
	int iErr;
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD( 2, 2 ); 
	iErr = WSAStartup( wVersionRequested, &wsaData );

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if ( INVALID_SOCKET  == m_Socket)
	{
		iErr = WSAGetLastError();

		return FALSE;
	}

	hostent *he;
	he = gethostbyname("www.suntoward.com");

	sockaddr_in cl_addr; 
    cl_addr.sin_family = AF_INET; 
    cl_addr.sin_port = htons(80); 
	cl_addr.sin_addr = *((in_addr *)he->h_addr);

	iErr = connect(m_Socket, (struct sockaddr*)(&cl_addr), sizeof(cl_addr));
	if (SOCKET_ERROR == iErr)
    { 
		iErr = WSAGetLastError();
		return FALSE;
    } 

	char sendBuf[256] = {0};
	//strcpy(sendBuf, "GET //webmax2/expo/china/index.htm HTTP/1.1\r\nHost:222.73.205.30\r\nAccept:*/*\r\nConection:Keep-Alive\r\nRange: bytes=100-200\r\n\r\n");   
	//strcpy(sendBuf, "GET /webmax2/expo/china/01.n3d HTTP/1.1\r\nHost:www.suntoward.com\r\nAccept:*/*\r\n\r\nConection:Keep-Alive\r\n\r\n");   
	strcpy(sendBuf, "GET /webmax2/expo/china/index.htm HTTP/1.1\r\nHost:www.suntoward.com\r\nAccept:*/*\r\nConection:Keep-Alive\r\n\r\n");   
	iErr = send(m_Socket, sendBuf, strlen(sendBuf), 0);
	if (SOCKET_ERROR == iErr)
    { 
		iErr = WSAGetLastError();
		return FALSE;
    } 

	long lTotalDownloaded = 0;
	char recvBuf[1024] = {0};
	do
	{
		iErr = recv(m_Socket, recvBuf, 1024, 0); 
		
		float fHttpVsr;
		int iStatus;
		long lDataLen;
		char cTmep[1024] = {0};
		if (0 == lTotalDownloaded)
		{
			sscanf(strstr(recvBuf,"HTTP/"), "HTTP/%f %d", &fHttpVsr, &iStatus);
			sscanf(strstr(recvBuf,"Content-Length"), "Content-Length: %d", &lDataLen);

			strcpy(cTmep, strstr(recvBuf,"\r\n\r\n")+sizeof("\r\n\r\n")-1); 

			WriteToFile(cTmep, strlen(cTmep));
		}
		else
			WriteToFile(recvBuf, iErr);

		lTotalDownloaded += iErr;

	}while(iErr > 0);

	closesocket(m_Socket); 
	fclose(m_pFile);

	WSACleanup();

	return TRUE;

}

BOOL CSocketHttpDlg::WriteToFile(char* pBuf, int iLen)
{
	if (NULL == m_pFile)
		m_pFile = fopen("C:\\test", "w");
	
	fwrite(pBuf, 1, iLen, m_pFile);
	
	return TRUE;
}