//--------------------------------------------
//	File Name:OptionsDlg.cpp
//	Introduce:The implement file of class COptionsDlg
//	Date:2010-05-10
//--------------------------------------------
#include "stdafx.h"
#include "TestHTTPDownload.h"
#include "OptionsDlg.h"
#include ".\optionsdlg.h"
//--------------------------------------------
// COptionsDlg dialog
//--------------------------------------------
IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)
COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	m_dwMaxTaskCnt = 5;
	m_dwMaxThreadCnt = 5;
}
//--------------------------------------------
COptionsDlg::~COptionsDlg()
{
}
//--------------------------------------------
void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//--------------------------------------------
BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()
//--------------------------------------------


//--------------------------------------------
// COptionsDlg message handlers
//--------------------------------------------
BOOL COptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox* pTaskCntCom = (CComboBox*)GetDlgItem(IDC_COMBO_TASKCOUNT);
	CComboBox* pThreadCntCom = (CComboBox*)GetDlgItem(IDC_COMBO_THREADCOUNT);
	for (int iIndex = 0; iIndex < 10; iIndex++)
	{
		TCHAR tcNum[8] = {_T("\0")};
		_stprintf(tcNum, _T("%d"), iIndex + 1);
		pTaskCntCom->AddString(tcNum);
		pThreadCntCom->AddString(tcNum);
	}
	pTaskCntCom->SetCurSel(4);
	pThreadCntCom->SetCurSel(4);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//--------------------------------------------
void COptionsDlg::OnBnClickedOk()
{
	CComboBox* pTaskCntCom = (CComboBox*)GetDlgItem(IDC_COMBO_TASKCOUNT);
	CComboBox* pThreadCntCom = (CComboBox*)GetDlgItem(IDC_COMBO_THREADCOUNT);
	m_dwMaxTaskCnt = pTaskCntCom->GetCurSel() + 1;
	m_dwMaxThreadCnt = pThreadCntCom->GetCurSel() + 1;

	OnOK();
}
//--------------------------------------------
