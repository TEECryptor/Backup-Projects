//----------------------------------------------------
//	File Name: AFULoginDlg.cpp
//	Introduce: Class CAFULoginDlg implement file
//
//----------------------------------------------------
#include "stdafx.h"
#include "AFUManager.h"
#include "AFULoginDlg.h"
//----------------------------------------------------


//----------------------------------------------------
// CAFULoginDlg dialog
//----------------------------------------------------
IMPLEMENT_DYNAMIC(CAFULoginDlg, CDialog)
CAFULoginDlg::CAFULoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAFULoginDlg::IDD, pParent)
{

}
//----------------------------------------------------
CAFULoginDlg::~CAFULoginDlg()
{
}
//----------------------------------------------------
void CAFULoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//----------------------------------------------------
BEGIN_MESSAGE_MAP(CAFULoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAFULoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()
//----------------------------------------------------



//----------------------------------------------------
// CAFULoginDlg message handlers
//----------------------------------------------------
void CAFULoginDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_USERNAME, m_strUserName);
	GetDlgItemText(IDC_EDIT_PASSWORD, m_strPassword);
	OnOK();
}
//----------------------------------------------------
void CAFULoginDlg::GetUserNameAndPassword(CString &strUserName, 
										  CString &strPassword)
{
	strUserName = m_strUserName;
	strPassword = m_strPassword;
}
//----------------------------------------------------
