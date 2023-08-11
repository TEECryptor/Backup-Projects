// UnlockPinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SynoTFKeySDKTest.h"
#include "UnlockPinDlg.h"


// CUnlockPinDlg dialog

IMPLEMENT_DYNAMIC(CUnlockPinDlg, CDialog)

CUnlockPinDlg::CUnlockPinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUnlockPinDlg::IDD, pParent)
{

}

CUnlockPinDlg::~CUnlockPinDlg()
{
}

void CUnlockPinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUnlockPinDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CUnlockPinDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUnlockPinDlg message handlers


BOOL CUnlockPinDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
}


void CUnlockPinDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_UNLOCKPINDLG_EDIT_SOPIN, m_tcSOPin, 32);
	if (_tcslen(m_tcSOPin) < 4 || _tcslen(m_tcSOPin) > 15)
	{
		MessageBox(_T("SO PINÂë³¤¶È´íÎó£¡"), _T("´íÎó"));
		GetDlgItem(IDC_UNLOCKPINDLG_EDIT_SOPIN)->SetFocus();
		return;
	}
	GetDlgItemText(IDC_UNLOCKPINDLG_EDIT_NEWPIN, m_tcNewUserPin, 32);
	if (_tcslen(m_tcNewUserPin) < 4 || _tcslen(m_tcNewUserPin) > 15)
	{
		MessageBox(_T("ÐÂPINÂë³¤¶È´íÎó£¡"), _T("´íÎó"));
		GetDlgItem(IDC_UNLOCKPINDLG_EDIT_NEWPIN)->SetFocus();
		return;
	}

	CDialog::OnOK();
}
