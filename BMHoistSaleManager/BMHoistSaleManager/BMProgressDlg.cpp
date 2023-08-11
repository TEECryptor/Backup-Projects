//-----------------------------------------------------------------------
//	File Name:BMProgressDlg.cpp
//	Introduce:implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMProgressDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMProgressDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMProgressDlg, CDialog)
//-----------------------------------------------------------------------
CBMProgressDlg::CBMProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMProgressDlg::IDD, pParent)
{
	m_iTotalProcessCnt = 0;
	m_iCurProcessIndex = 0;
	m_strCurProcessName = _T("\0");
}
//-----------------------------------------------------------------------
CBMProgressDlg::~CBMProgressDlg()
{
}
//-----------------------------------------------------------------------
void CBMProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_CUR, m_curProgress);
	DDX_Control(pDX, IDC_PROGRESS_TOTAL, m_totalProgress);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMProgressDlg, CDialog)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMProgressDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMProgressDlg::SetProcessCnt(int iCnt)
{
	m_iTotalProcessCnt = iCnt;
}
//-----------------------------------------------------------------------
void CBMProgressDlg::SetCurrentProcessName(int iIndex, CString strProcessName)
{
	m_iCurProcessIndex = iIndex;
	m_strCurProcessName = strProcessName;
}
//-----------------------------------------------------------------------
void CBMProgressDlg::SetCurrentProgress(int iPos)
{
	float fTotalProgress = float(m_iCurProcessIndex*100 + iPos) / (m_iTotalProcessCnt*100);
	int iCurPos = fTotalProgress * 100;
	//
	CString strCurInfo;
	strCurInfo.Format(_T("Current component:%s (%d of %d) (%d%s)"), m_strCurProcessName, m_iCurProcessIndex+1, m_iTotalProcessCnt, iPos, _T("%"));
	SetDlgItemText(IDC_STATIC_CURINFO, strCurInfo);
	//
	CString strTotalInfo;
	strTotalInfo.Format(_T("Total progress: %d%s"), iCurPos, _T("%"));
	SetDlgItemText(IDC_STATIC_TOTALINFO, strTotalInfo);
}
//-----------------------------------------------------------------------
