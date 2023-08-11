// InputFileNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SynoTFKeySDKTest.h"
#include "InputFileNameDlg.h"
#include "afxdialogex.h"


// CInputFileNameDlg dialog

IMPLEMENT_DYNAMIC(CInputFileNameDlg, CDialog)

CInputFileNameDlg::CInputFileNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputFileNameDlg::IDD, pParent)
{

}

CInputFileNameDlg::~CInputFileNameDlg()
{
}

void CInputFileNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInputFileNameDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInputFileNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInputFileNameDlg message handlers


BOOL CInputFileNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_INPUTFILENAMEDLG_EDIT_FILENAME)->SetFocus();

	return FALSE; 
}


void CInputFileNameDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_INPUTFILENAMEDLG_EDIT_FILENAME, m_strFileName);
	if (m_strFileName.GetLength() <= 0)
	{		
		MessageBox(_T("文件名不能为空!"), _T("提示"));
		GetDlgItem(IDC_INPUTFILENAMEDLG_EDIT_FILENAME)->SetFocus();
		return;
	}

	CDialog::OnOK();
}
