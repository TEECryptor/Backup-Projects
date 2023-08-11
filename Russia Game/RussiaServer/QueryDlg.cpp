// QueryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RussiaServer.h"
#include "QueryDlg.h"
#include "RussiaServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// QueryDlg dialog


QueryDlg::QueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(QueryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(QueryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void QueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(QueryDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(QueryDlg, CDialog)
	//{{AFX_MSG_MAP(QueryDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// QueryDlg message handlers

BOOL QueryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CListCtrl* plst = (CListCtrl*)GetDlgItem(IDC_LIST_SCOREHISTORY);
	plst->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	plst->InsertColumn(0, "�ֻ���", LVCFMT_LEFT, 70);
	plst->InsertColumn(1, "��/��/��", LVCFMT_LEFT, 100);
	plst->InsertColumn(2, "ʱ/��/��", LVCFMT_LEFT, 100);
	plst->InsertColumn(3, "����", LVCFMT_LEFT, 80);
	plst->InsertColumn(4, "��ֵ", LVCFMT_LEFT, 80);

	CRussiaServerDlg* pDlg = (CRussiaServerDlg* )::AfxGetMainWnd();
	pDlg->ReadData2QueryDlgList(plst);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
