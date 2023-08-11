//-----------------------------------------------------------------------
//	File Name:BMCombinDataDlg.cpp
//	Introduce:Class CBMCombinDataDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMCombinDataDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMCombinDataDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMCombinDataDlg, CDialog)
CBMCombinDataDlg::CBMCombinDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMCombinDataDlg::IDD, pParent)
{

}
//-----------------------------------------------------------------------
CBMCombinDataDlg::~CBMCombinDataDlg()
{
}
//-----------------------------------------------------------------------
void CBMCombinDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMCombinDataDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_IMPDLG_OPENSRC1, &CBMCombinDataDlg::OnBnClickedBtnImpdlgOpensrc1)
	ON_BN_CLICKED(IDC_BTN_IMPDLG_OPENSRC2, &CBMCombinDataDlg::OnBnClickedBtnImpdlgOpensrc2)
	ON_BN_CLICKED(IDC_CHECK_IMPDLG_SAMEAS, &CBMCombinDataDlg::OnBnClickedCheckImpdlgSameas)
	ON_BN_CLICKED(IDC_BTN_OPENDEST, &CBMCombinDataDlg::OnBnClickedBtnOpendest)
	ON_BN_CLICKED(IDC_BTN_IMPDLG_GO, &CBMCombinDataDlg::OnBnClickedBtnImpdlgGo)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMCombinDataDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMCombinDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_EDIT_IMPDLG_SRCDATA1)->SetFocus();
	CButton* pCheckBtn = (CButton*)GetDlgItem(IDC_CHECK_IMPDLG_SAMEAS);
	pCheckBtn->SetCheck(BST_CHECKED);
	OnBnClickedCheckImpdlgSameas();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMCombinDataDlg::OnBnClickedBtnImpdlgOpensrc1()
{
	// TODO: Add your control notification handler code here
}
//-----------------------------------------------------------------------
void CBMCombinDataDlg::OnBnClickedBtnImpdlgOpensrc2()
{
	// TODO: Add your control notification handler code here
}
//-----------------------------------------------------------------------
void CBMCombinDataDlg::OnBnClickedCheckImpdlgSameas()
{
	CButton* pCheckBtn = (CButton*)GetDlgItem(IDC_CHECK_IMPDLG_SAMEAS);
	if (BST_CHECKED == pCheckBtn->GetCheck())
	{
		GetDlgItem(IDC_EDIT_IMPDLG_DESTDATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_OPENDEST)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_IMPDLG_DESTDATA)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_OPENDEST)->EnableWindow(TRUE);
	}
}
//-----------------------------------------------------------------------
void CBMCombinDataDlg::OnBnClickedBtnOpendest()
{
	// TODO: Add your control notification handler code here
}
//-----------------------------------------------------------------------
void CBMCombinDataDlg::OnBnClickedBtnImpdlgGo()
{
	// TODO: Add your control notification handler code here
}
//-----------------------------------------------------------------------
