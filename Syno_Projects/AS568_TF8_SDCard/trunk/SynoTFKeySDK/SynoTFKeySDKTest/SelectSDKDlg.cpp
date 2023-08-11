// SelectSDKDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SynoTFKeySDKTest.h"
#include "SelectSDKDlg.h"


// CSelectSDKDlg dialog

IMPLEMENT_DYNAMIC(CSelectSDKDlg, CDialog)

CSelectSDKDlg::CSelectSDKDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectSDKDlg::IDD, pParent)
{

}

CSelectSDKDlg::~CSelectSDKDlg()
{
}

void CSelectSDKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectSDKDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_SELECTSDKDLG_COMBO_SDK, &CSelectSDKDlg::OnCbnSelchangeSelectsdkComboSdk)
	ON_BN_CLICKED(IDC_SELECTSDKDLG_BTN_OPENFILE, &CSelectSDKDlg::OnBnClickedSelectsdkdlgBtnOpenfile)
	ON_BN_CLICKED(IDOK, &CSelectSDKDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSelectSDKDlg message handlers

BOOL CSelectSDKDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strDllFile = _T("");
	CComboBox* pSDKComboBox = (CComboBox*)GetDlgItem(IDC_SELECTSDKDLG_COMBO_SDK);
	pSDKComboBox->ResetContent();
	int nIndex = pSDKComboBox->AddString(_T("国密SDK"));
	pSDKComboBox->SetItemData(nIndex, SDK_TYPE_SKF);
	nIndex = pSDKComboBox->AddString(_T("PKCS11 SDK"));
	pSDKComboBox->SetItemData(nIndex, SDK_TYPE_PKCS11);
	pSDKComboBox->SetCurSel(nIndex);
	OnCbnSelchangeSelectsdkComboSdk();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectSDKDlg::OnCbnSelchangeSelectsdkComboSdk()
{
	CComboBox* pSDKComboBox = (CComboBox*)GetDlgItem(IDC_SELECTSDKDLG_COMBO_SDK);
	int nSelIndex = pSDKComboBox->GetCurSel();
	m_eSelSDKType = (SDK_TYPE)pSDKComboBox->GetItemData(nSelIndex);
	if (SDK_TYPE_SKF == m_eSelSDKType)
	{
		MessageBox(_T("该功能暂时未实现!"), _T("提示"));
		pSDKComboBox->SetCurSel(1);
		OnCbnSelchangeSelectsdkComboSdk();
	}
}

void CSelectSDKDlg::OnBnClickedSelectsdkdlgBtnOpenfile()
{	
	CFileDialog dlgOpen(TRUE, _T("*.dll"), NULL, OPEN_EXISTING, _T("DLL file|*.dll|All files|*.*||"), NULL);
	
	if (dlgOpen.DoModal() == IDOK)
	{		
		m_strDllFile = dlgOpen.GetPathName();
		SetDlgItemText(IDC_SELECTSDKDLG_EDIT_FILE, m_strDllFile);
	}
}

void CSelectSDKDlg::OnBnClickedOk()
{
	if (m_strDllFile.GetLength() == 0)
	{
		MessageBox(_T("请选择SDK的DLL文件!"), _T("提示"));
		return;
	}

	OnOK();
}
