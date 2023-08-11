// TestTipDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestTip.h"
#include "TestTipDlg.h"
#include ".\testtipdlg.h"

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


// CTestTipDlg �Ի���



CTestTipDlg::CTestTipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestTipDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestTipDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_NCPAINT()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CTestTipDlg ��Ϣ�������

BOOL CTestTipDlg::OnInitDialog()
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

    EnableToolTips(TRUE);

	m_pTipCtrl = new CToolTipCtrl;
	m_pTipCtrl->Create(this);
	m_pTipCtrl->AddTool(GetDlgItem(IDC_BUTTON1), _T("aaaaaaaaaaa"));
	m_pTipCtrl->AddTool(GetDlgItem(IDC_BUTTON2), _T("nnnnnnnnnnnnnnn"));
	m_pTipCtrl->Activate(TRUE);
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CTestTipDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestTipDlg::OnPaint() 
{
	/*
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}*/
	OnNcPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CTestTipDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CTestTipDlg::PreTranslateMessage(MSG* pMsg)
{
	if(NULL != m_pTipCtrl && NULL != m_pTipCtrl->m_hWnd)  
		m_pTipCtrl->RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CTestTipDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	//if(NULL != m_pTipCtrl)
	//	m_pTipCtrl->UpdateTipText(_T("aaa"), this);

	CDialog::OnMouseMove(nFlags, point);
}

void CTestTipDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
/*
	if(TRUE == bMinimized && nState == WA_CLICKACTIVE)
	{		
		ShowWindow(SW_RESTORE);
	}
	else if(nState == WA_INACTIVE)
	{
		ShowWindow(SW_MINIMIZE);
	}*/
}

void CTestTipDlg::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CDialog::OnActivateApp(bActive, dwThreadID);

}

void CTestTipDlg::OnNcPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	HBRUSH hBKBrush = CreateSolidBrush(RGB(255, 255, 255));

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	FillRect(dc.m_hDC, CRect(0, 0, rcWnd.Width(), rcWnd.Height()),  hBKBrush);

	DeleteObject(hBKBrush);
}

void CTestTipDlg::OnBnClickedButton2()
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	int iIndex = pEdit->LineIndex();

	int a =0;
}
