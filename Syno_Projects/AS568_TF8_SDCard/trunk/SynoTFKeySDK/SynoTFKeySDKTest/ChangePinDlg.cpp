// ChangePinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SynoTFKeySDKTest.h"
#include "ChangePinDlg.h"


// CChangePinDlg dialog

IMPLEMENT_DYNAMIC(CChangePinDlg, CDialog)

CChangePinDlg::CChangePinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangePinDlg::IDD, pParent)
{

}

CChangePinDlg::~CChangePinDlg()
{
}

void CChangePinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChangePinDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CChangePinDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChangePinDlg message handlers


BOOL CChangePinDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CChangePinDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_CHANGEPINDLG_EDIT_OLDPIN, m_tcOldPin, 32);
	if (_tcslen(m_tcOldPin) < 4 || _tcslen(m_tcOldPin) > 15)
	{
		MessageBox(_T("ÀÏPINÂë³¤¶È´íÎó£¡"), _T("´íÎó"));
		GetDlgItem(IDC_CHANGEPINDLG_EDIT_OLDPIN)->SetFocus();
		return;
	}
	GetDlgItemText(IDC_CHANGEPINDLG_EDIT_NEWPIN, m_tcNewPin, 32);
	if (_tcslen(m_tcNewPin) < 4 || _tcslen(m_tcNewPin) > 15)
	{
		MessageBox(_T("ÐÂPINÂë³¤¶È´íÎó£¡"), _T("´íÎó"));
		GetDlgItem(IDC_CHANGEPINDLG_EDIT_NEWPIN)->SetFocus();
		return;
	}

	CDialog::OnOK();
}
