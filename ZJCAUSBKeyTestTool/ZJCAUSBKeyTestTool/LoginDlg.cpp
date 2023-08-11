// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ZJCAUSBKeyTestTool.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


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

	GetDlgItem(IDC_EDIT_USERPIN)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CLoginDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_USERPIN, m_strUserPIN);
	if (m_strUserPIN.GetLength() == 0)
	{
		MessageBox(_T("用户PIN不能为空！"));
		return;
	}
	if (m_strUserPIN.GetLength() > 16)
	{
		MessageBox(_T("用户PIN长度应该小于16！"));
		return;
	}

	CDialog::OnOK();
}

