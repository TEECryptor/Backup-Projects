// TestPngImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestPngImage.h"
#include "TestPngImageDlg.h"
#include ".\testpngimagedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTestPngImageDlg 对话框



CTestPngImageDlg::CTestPngImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestPngImageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pImage = NULL;
}

void CTestPngImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestPngImageDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTestPngImageDlg 消息处理程序

BOOL CTestPngImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


   GdiplusStartupInput gdiplusStartupInput;
   GdiplusStartup(&m_GdiplusToken, &gdiplusStartupInput, NULL);

   m_pImage = new CGDIPlusImage();
   //m_pImage->LoadFromFile(_T("D:\\VC Projects\\TestPngImage\\Debug\\Test.png"));
   m_pImage->LoadFromResource(AfxGetApp()->m_hInstance, _T("PNG"), IDR_PNG1);


	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CTestPngImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestPngImageDlg::OnPaint() 
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(&rcClient);

	HBRUSH hBKBrush = CreateSolidBrush(RGB(255, 255, 200));
	FillRect(dc.m_hDC, rcClient, hBKBrush);
	DeleteObject(hBKBrush);

	if(NULL != m_pImage)
		delete m_pImage;

   m_pImage = new CGDIPlusImage();
	m_pImage->LoadFromResource(AfxGetApp()->m_hInstance, _T("PNG"), IDR_PNG1); 

	if(NULL != m_pImage)
	{
		m_pImage->Draw(dc.m_hDC, rcClient, CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight()));
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CTestPngImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestPngImageDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if(NULL != m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
	GdiplusShutdown(m_GdiplusToken);
}
