// DevInitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SynoTFKeySDKTest.h"
#include "DevInitDlg.h"
#include "afxdialogex.h"


// CDevInitDlg dialog

IMPLEMENT_DYNAMIC(CDevInitDlg, CDialog)

CDevInitDlg::CDevInitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDevInitDlg::IDD, pParent)
{

}

CDevInitDlg::~CDevInitDlg()
{
}

void CDevInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDevInitDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CDevInitDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDevInitDlg message handlers


BOOL CDevInitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	SetDlgItemText(IDC_INITDLG_EDIT_LABEL, _T("Syno Key"));
	SetDlgItemText(IDC_INITDLG_EDIT_SOPIN, _T("111111"));
	SetDlgItemText(IDC_INITDLG_EDIT_SOPIN_RETRYTIMES, _T("10"));
	SetDlgItemText(IDC_INITDLG_EDIT_USERPIN, _T("111111"));
	SetDlgItemText(IDC_INITDLG_EDIT_USERPIN_RETRYTIMES, _T("10"));
	GetDlgItem(IDC_INITDLG_EDIT_LABEL)->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
}


void CDevInitDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}


void CDevInitDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_INITDLG_EDIT_LABEL, m_strLabel);
	if (m_strLabel.GetLength() <= 6 || m_strLabel.GetLength() > 16)
	{
		MessageBox(_T("标签长度错误！正确长度为:6-16字符！"));
		GetDlgItem(IDC_INITDLG_EDIT_LABEL)->SetFocus();
		return;
	}
	
	GetDlgItemText(IDC_INITDLG_EDIT_SOPIN, m_strSOPin);
	if (m_strSOPin.GetLength() < 4 || m_strSOPin.GetLength() > 16)
	{
		MessageBox(_T("SO PIN长度错误！正确长度为:4-16字符！"));
		GetDlgItem(IDC_INITDLG_EDIT_SOPIN)->SetFocus();
		return;
	}
	
	CString strSOPinRetryTimes;
	GetDlgItemText(IDC_INITDLG_EDIT_SOPIN_RETRYTIMES, strSOPinRetryTimes);
	m_ulSOPinRetryTimes = _ttol(strSOPinRetryTimes);
	if (m_ulSOPinRetryTimes < 5 || m_ulSOPinRetryTimes > 15)
	{
		MessageBox(_T("SO PIN重复次数错误！正确次数为:5-15次！"));
		GetDlgItem(IDC_INITDLG_EDIT_SOPIN_RETRYTIMES)->SetFocus();
		return;
	}
	
	GetDlgItemText(IDC_INITDLG_EDIT_USERPIN, m_strUserPin);
	if (m_strUserPin.GetLength() < 4 || m_strUserPin.GetLength() > 16)
	{
		MessageBox(_T("User PIN长度错误！正确长度为:4-16字符！"));
		GetDlgItem(IDC_INITDLG_EDIT_USERPIN)->SetFocus();
		return;
	}
	
	CString strUserPinRetryTimes;
	GetDlgItemText(IDC_INITDLG_EDIT_USERPIN_RETRYTIMES, strUserPinRetryTimes);
	m_ulUserPinRetryTimes = _ttol(strUserPinRetryTimes);
	if (m_ulUserPinRetryTimes < 5 || m_ulUserPinRetryTimes > 15)
	{
		MessageBox(_T("User PIN重复次数错误！正确次数为:5-15次！"));
		GetDlgItem(IDC_INITDLG_EDIT_USERPIN_RETRYTIMES)->SetFocus();
		return;
	}

	CDialog::OnOK();
}
