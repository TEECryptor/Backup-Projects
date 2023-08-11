// TestEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestEdit.h"
#include "TestEditDlg.h"
#include ".\testeditdlg.h"


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


// CTestEditDlg �Ի���



CTestEditDlg::CTestEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestEditDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMyEdit = NULL;
	m_pInfoView = NULL;
}

void CTestEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestEditDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_FONT, OnBnClickedBtnFont)
	ON_BN_CLICKED(IDC_BTN_COLOR, OnBnClickedBtnColor)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_ACTIVATEAPP()
END_MESSAGE_MAP()


// CTestEditDlg ��Ϣ�������

BOOL CTestEditDlg::OnInitDialog()
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


	CRect rcWnd;
	CRect rcClient;
	CRect rcShow;
	GetWindowRect(&rcWnd);
	GetClientRect(rcClient);
	rcShow.left = 3;
	rcShow.top = 29;
	rcShow.right = rcShow.left + rcClient.Width();
	rcShow.bottom = rcShow.top + rcClient.Height();
	//
	CRgn rgnWindow;
	//rgnWindow.CreateRoundRectRgn(rcShow.left, rcShow.top, rcShow.right, rcShow.bottom, 0, 0);
	SetWindowRgn((HRGN)rgnWindow.Detach(), TRUE);

	CString strText;
	CStringList strTextLst;
	for(int i = 0; i < 20; i++)
	{
		CString strTmp;
		strTmp.Format(_T("%d:%s"), i, _T("aaaaaaaaaaaaabbbbbbbbbbbbaassSSSSSSSSSSSSSSSSSSSSS"));
		strText += "aaaaaaaaaaaaabbbbbbbbbbbbaassSSSSSSSSSSSSSSSSSSSSS";
		strTextLst.AddTail(strTmp);
		//
		strText += "\n\r\n\r";
	}

	CFont* pFont = GetFont();
	m_pMyEdit1 = new CMyEdit1();
	m_pMyEdit1->Create(WS_CHILD|WS_VISIBLE|WS_EX_CLIENTEDGE|ES_MULTILINE|ES_WANTRETURN, CRect(100, 100, 350, 250), this, 0);
	m_pMyEdit1->SetFont(pFont);

	//m_pMyEdit = new CMyEdit();
	////m_pMyEdit->Create(WS_CHILD|WS_VISIBLE|WS_EX_CLIENTEDGE|WS_HSCROLL|ES_AUTOHSCROLL|WS_VSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|ES_WANTRETURN, CRect(100, 100, 350, 250), this, 0);
	//m_pMyEdit->CreateEx(WS_EX_CLIENTEDGE, _T("Edit"), _T(""), WS_CHILD|WS_VISIBLE/*|WS_HSCROLL|ES_AUTOHSCROLL*/|WS_VSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|ES_WANTRETURN, CRect(100, 100, 350, 250), this, 0);

	////m_pMyEdit->SetWindowText(strText);
	//m_pMyEdit->Invalidate();
	//m_pMyEdit->UpdateWindow();

/*
	LOGFONT logFont;
	CFont* pFont = GetFont();
	pFont->GetLogFont(&logFont);

	m_pInfoView = new CInfoView();
	m_pInfoView->Create(NULL, NULL, WS_CHILD|WS_BORDER|WS_VISIBLE, CRect(10, 100, 10+INFOVIEW_WIDTH, 100+INFOVIEW_HEIGHT), this, 0);
	m_pInfoView->SetScrollSizes(MM_TEXT, CSize(1, 1));
	//m_pInfoView->SetScrollSizes(MM_TEXT, CSize(INFOVIEW_WIDTH, INFOVIEW_HEIGHT));
	m_pInfoView->SetTextFont(logFont);
	m_pInfoView->SetTextColor(RGB(0,0,0));
	m_pInfoView->SetBKColor(RGB(255,255,255));
	m_pInfoView->SetStrings(strTextLst);
*/
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CTestEditDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestEditDlg::OnPaint() 
{
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
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CTestEditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestEditDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_pMyEdit)
	{
		delete m_pMyEdit;
		m_pMyEdit = NULL;
	}
	if(m_pInfoView)
	{
		delete m_pInfoView;
		m_pInfoView = NULL;
	}
}

HBRUSH CTestEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CTestEditDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnEraseBkgnd(pDC);
}

void CTestEditDlg::OnBnClickedBtnFont()
{
	CFontDialog dlg;
	if(dlg.DoModal() == IDOK)
	{
		LOGFONT logFont;
		COLORREF clrText;
		dlg.GetCurrentFont(&logFont);
		clrText = dlg.GetColor();

		m_pInfoView->SetTextFont(logFont);
		m_pInfoView->SetTextColor(clrText);
	}
}

void CTestEditDlg::OnBnClickedBtnColor()
{
	CColorDialog dlg;
	if(dlg.DoModal() == IDOK)
	{
		COLORREF clrBK;
		clrBK = dlg.GetColor();

		m_pInfoView->SetBKColor(clrBK);
	}
}

void CTestEditDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);
}

UINT CTestEditDlg::OnNcHitTest(CPoint point)
{
	//TRACE(_T("OnNcHitTest\n"));

	return CDialog::OnNcHitTest(point);
}

void CTestEditDlg::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CDialog::OnActivateApp(bActive, dwThreadID);

	TRACE(_T("OnActivateApp\n"));
}
