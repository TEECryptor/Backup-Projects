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
		MessageBox(_T("�û�PIN����Ϊ�գ�"));
		return;
	}
	if (m_strUserPIN.GetLength() > 16)
	{
		MessageBox(_T("�û�PIN����Ӧ��С��16��"));
		return;
	}

	CDialog::OnOK();
}

