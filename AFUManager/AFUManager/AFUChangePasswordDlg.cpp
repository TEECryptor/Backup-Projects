//----------------------------------------------------
//	File Name: AFUChangePasswordDlg.cpp
//	Introduce: Class CAFUChangePasswordDlg implement file
//
//----------------------------------------------------
#include "stdafx.h"
#include "AFUManager.h"
#include "AFUChangePasswordDlg.h"
//----------------------------------------------------



//----------------------------------------------------
// CAFUChangePasswordDlg dialog
//----------------------------------------------------
IMPLEMENT_DYNAMIC(CAFUChangePasswordDlg, CDialog)
CAFUChangePasswordDlg::CAFUChangePasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAFUChangePasswordDlg::IDD, pParent)
{

}
//----------------------------------------------------
CAFUChangePasswordDlg::~CAFUChangePasswordDlg()
{
}
//----------------------------------------------------


//----------------------------------------------------
void CAFUChangePasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//----------------------------------------------------
BEGIN_MESSAGE_MAP(CAFUChangePasswordDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAFUChangePasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()
//----------------------------------------------------


//----------------------------------------------------
// CAFUChangePasswordDlg message handlers
//----------------------------------------------------

BOOL CAFUChangePasswordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//----------------------------------------------------
void CAFUChangePasswordDlg::OnBnClickedOk()
{
	CString strOldPassword;
	GetDlgItemText(IDC_EDIT_OLDPASSWORD, strOldPassword);
	if (_tcsicmp(strOldPassword, m_strOldPassword) != 0)
	{
		MessageBox(_T(" ‰»Îµƒæ…√‹¬Î¥ÌŒÛ£°"), _T("¥ÌŒÛ"), MB_OK | MB_ICONERROR);
		return;
	}

	CString strNewPassword1;
	CString strNewPassword2;
	GetDlgItemText(IDC_EDIT_NEWPASSWORD1, strNewPassword1);
	GetDlgItemText(IDC_EDIT_NEWPASSWORD2, strNewPassword2);
	if (strNewPassword1 != strNewPassword2)
	{
		MessageBox(_T(" ‰»Îµƒ–¬√‹¬Î≤ª“ª÷¬£°"), _T("¥ÌŒÛ"), MB_OK | MB_ICONERROR);
		return;
	}

	m_strNewPassword = strNewPassword1;

	OnOK();
}
//----------------------------------------------------


//----------------------------------------------------
void CAFUChangePasswordDlg::SetOldPassword(CString strOldPassword)
{
	m_strOldPassword = strOldPassword;
}
//----------------------------------------------------
void CAFUChangePasswordDlg::GetNewPassword(CString &strNewPassword)
{
	strNewPassword = m_strNewPassword;
}
//----------------------------------------------------