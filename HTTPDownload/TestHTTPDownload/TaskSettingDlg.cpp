//--------------------------------------------
//	File Name:TaskSettingDlg.cpp
//	Introduce:The implement file of class CTaskSettingDlg
//	Date:2010-07-02
//--------------------------------------------
#include "stdafx.h"
#include "TestHTTPDownload.h"
#include "TaskSettingDlg.h"
//--------------------------------------------
// CTaskSettingDlg dialog
//--------------------------------------------
IMPLEMENT_DYNAMIC(CTaskSettingDlg, CDialog)
CTaskSettingDlg::CTaskSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaskSettingDlg::IDD, pParent)
{
	m_dwMaxThreadCnt = 5;
	m_dwSettingThreadCnt = 5;
}
//--------------------------------------------
CTaskSettingDlg::~CTaskSettingDlg()
{
}
//--------------------------------------------
void CTaskSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//--------------------------------------------
BEGIN_MESSAGE_MAP(CTaskSettingDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_THREADCNT, OnCbnSelchangeComboThreadcnt)
END_MESSAGE_MAP()
//--------------------------------------------


//--------------------------------------------
// CTaskSettingDlg message handlers
//--------------------------------------------
BOOL CTaskSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int iInitIndex = -1;
	CComboBox* pThreadCntCom = (CComboBox*)GetDlgItem(IDC_COMBO_THREADCNT);
	for (int iIndex = 1; iIndex <= m_dwMaxThreadCnt; iIndex++)
	{
		CString strText;
		strText.Format(_T("%d"), iIndex);
		pThreadCntCom->AddString(strText);
		if (m_dwSettingThreadCnt == iIndex)
			iInitIndex = (iIndex - 1);
	}
	pThreadCntCom->SetCurSel(iInitIndex != -1 ? iInitIndex : 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//--------------------------------------------
void CTaskSettingDlg::OnCbnSelchangeComboThreadcnt()
{
	CComboBox* pThreadCntCom = (CComboBox*)GetDlgItem(IDC_COMBO_THREADCNT);
	int iSelIndex = pThreadCntCom->GetCurSel();
	m_dwSettingThreadCnt = iSelIndex + 1;
}
//--------------------------------------------
