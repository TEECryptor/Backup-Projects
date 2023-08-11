// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SynoTFKeySDKTest.h"
#include "LoginDlg.h"
#include "./TFKeySDKTester/Defs.h"


// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg message handlers


BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox* pComboPinType = (CComboBox*)GetDlgItem(IDC_LOGIN_COMBO_TYPE);
	pComboPinType->ResetContent();

	ULONG ulIndex = pComboPinType->AddString(_T("SO PIN"));
	pComboPinType->SetItemData(ulIndex, USER_TYPE_SO);
	ulIndex = pComboPinType->AddString(_T("User PIN"));
	pComboPinType->SetItemData(ulIndex, USER_TYPE_USER);
	pComboPinType->SetCurSel(ulIndex);

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CLoginDlg::OnBnClickedOk()
{
	CComboBox* pComboPinType = (CComboBox*)GetDlgItem(IDC_LOGIN_COMBO_TYPE);
	ULONG ulIndex = pComboPinType->GetCurSel();
	m_ulUserType = pComboPinType->GetItemData(ulIndex);

	GetDlgItemText(IDC_LOGINDLG_EDIT_PIN, m_tcPin, 32);
	if (_tcslen(m_tcPin) < 4 || _tcslen(m_tcPin) > 15)
	{
		MessageBox(_T("PINÂë³¤¶È´íÎó£¡"), _T("´íÎó"));
		GetDlgItem(IDC_LOGINDLG_EDIT_PIN)->SetFocus();
		return;
	}

	CDialog::OnOK();
}
