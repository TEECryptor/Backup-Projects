//-----------------------------------------------------------------------
//	File Name:BMMessageDlg.cpp
//	Introduce:Class CBMMessageDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMMessageDlg.h"
//-----------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMMessageDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMMessageDlg, CDialog)
CBMMessageDlg::CBMMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMMessageDlg::IDD, pParent)
{
	m_strCaption = _T("\0");
	m_strText = _T("\0");
	m_nBtnFlags = 0;
	m_hIcon = NULL;
}
//-----------------------------------------------------------------------
CBMMessageDlg::~CBMMessageDlg()
{
}
//-----------------------------------------------------------------------
void CBMMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMMessageDlg, CDialog)
	ON_BN_CLICKED(IDYES, &CBMMessageDlg::OnBnClickedYes)
	ON_BN_CLICKED(IDNO, &CBMMessageDlg::OnBnClickedNo)
	ON_WM_PAINT()
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMMessageDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMMessageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int iBtnCnt = 0;
	UINT nBtnIDs[3];
	SetWindowText(m_strCaption);
	if (m_nBtnFlags == MB_OK)
	{
		nBtnIDs[iBtnCnt++] = IDOK;
		GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
	}
	else if (m_nBtnFlags == MB_YESNO)
	{
		nBtnIDs[iBtnCnt++] = IDYES;
		nBtnIDs[iBtnCnt++] = IDNO;
		GetDlgItem(IDYES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDNO)->ShowWindow(SW_SHOW);
	}

	CDC* pDC = GetDC();
	TEXTMETRIC metric;
	metric.tmAveCharWidth = 7;
	metric.tmMaxCharWidth = 14;
	if (NULL != pDC)
	{
		GetTextMetrics(pDC->m_hDC, &metric);		
	}
	//
	CRect rcText;
	CWnd* pStaticText = GetDlgItem(IDC_STATIC_TEXT);
	pStaticText->GetWindowRect(&rcText);
	ScreenToClient(&rcText);
	int iTextCnt = (m_strText.Find(_T('\n')) == -1) ? m_strText.GetLength() : m_strText.Find(_T('\n'));
	int iTextWidth = (iTextCnt + 2) * metric.tmAveCharWidth;
	iTextWidth = max(iTextWidth, 280);
	rcText.right = rcText.left + iTextWidth;
	pStaticText->MoveWindow(&rcText);
	pStaticText->SetWindowText(m_strText);
	//
	CRect rcDlg;
	GetClientRect(&rcDlg);
	rcDlg.right = rcText.right + 20;
	rcDlg.bottom += 32;
	MoveWindow(&rcDlg);
	CenterWindow();
	//
	CRect rcBtn;
	int iBtnWidth = 0;
	int iBtnBaseLeft = 0;
	const int iBtnInter = 10;
	GetDlgItem(nBtnIDs[0])->GetWindowRect(&rcBtn);
	ScreenToClient(&rcBtn);
	iBtnWidth = rcBtn.Width();
	iBtnBaseLeft = (rcDlg.Width() - (iBtnCnt*rcBtn.Width() + (iBtnCnt-1)*iBtnInter )) / 2;
	for (int i = 0; i < iBtnCnt; i++)
	{
		GetDlgItem(nBtnIDs[i])->GetWindowRect(&rcBtn);
		ScreenToClient(&rcBtn);
		rcBtn.left = iBtnBaseLeft + i * (iBtnWidth + iBtnInter);
		rcBtn.right = rcBtn.left + iBtnWidth;
		GetDlgItem(nBtnIDs[i])->MoveWindow(&rcBtn);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMMessageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here

	dc.DrawIcon(20, 20, m_hIcon);
}
//-----------------------------------------------------------------------
void CBMMessageDlg::OnBnClickedYes()
{
	EndDialog(IDYES);
}
//-----------------------------------------------------------------------
void CBMMessageDlg::OnBnClickedNo()
{
	EndDialog(IDNO);
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void CBMMessageDlg::SetMsgTextAndIcon(CString strCaption, CString strText, UINT nFlags)
{
	m_strCaption = strCaption;
	m_strText = strText;
	m_nBtnFlags = nFlags & 0x0f;
	switch(nFlags & 0xf0)
	{
	case MB_ICONEXCLAMATION:
		m_hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
		break;
	case MB_ICONINFORMATION:
		m_hIcon = LoadIcon(NULL, IDI_INFORMATION);
		break;
	case MB_ICONQUESTION:
		m_hIcon = LoadIcon(NULL, IDI_QUESTION);
		break;
	case MB_ICONHAND:
		m_hIcon = LoadIcon(NULL, IDI_HAND);
		break;
	}
}
//-----------------------------------------------------------------------