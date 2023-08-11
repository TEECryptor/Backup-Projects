// TestPngImageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestPngImage.h"
#include "TestPngImageDlg.h"
#include ".\testpngimagedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CTestPngImageDlg �Ի���



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


// CTestPngImageDlg ��Ϣ�������

BOOL CTestPngImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


   GdiplusStartupInput gdiplusStartupInput;
   GdiplusStartup(&m_GdiplusToken, &gdiplusStartupInput, NULL);

   m_pImage = new CGDIPlusImage();
   //m_pImage->LoadFromFile(_T("D:\\VC Projects\\TestPngImage\\Debug\\Test.png"));
   m_pImage->LoadFromResource(AfxGetApp()->m_hInstance, _T("PNG"), IDR_PNG1);


	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
