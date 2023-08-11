//----------------------------------------------------
//	File Name:AFUMemberDlg.cpp
//	Introduce:Dialog class CAFUMemberDlg implementation file
//----------------------------------------------------
#include "stdafx.h"
#include "AFUManager.h"
#include "AFUMemberDlg.h"
#include "DatabaseDefine.h"
//----------------------------------------------------


//----------------------------------------------------
// CAFUMemberDlg dialog
//----------------------------------------------------
IMPLEMENT_DYNAMIC(CAFUMemberDlg, CDialog)
CAFUMemberDlg::CAFUMemberDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAFUMemberDlg::IDD, pParent)
{
	m_strName = _T("");
	m_strVIPID = _T("100000");
	m_bSexIsFemale = TRUE;
	m_strTel = _T("");
	m_dateBirth = CTime(1990, 1, 1, 0, 0, 0);
	m_strEmail = _T("");
	m_strAddress = _T("");
	m_strOtherInfo = _T("");
}
//----------------------------------------------------
CAFUMemberDlg::~CAFUMemberDlg()
{
}
//----------------------------------------------------
void CAFUMemberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//----------------------------------------------------
BEGIN_MESSAGE_MAP(CAFUMemberDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAFUMemberDlg::OnBnClickedOk)
END_MESSAGE_MAP()
//----------------------------------------------------


//----------------------------------------------------
// CAFUMemberDlg message handlers
//----------------------------------------------------
BOOL CAFUMemberDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Initialize data
	SetDlgItemText(IDC_EDIT_NAME, m_strName);
	SetDlgItemText(IDC_EDIT_VIPID, m_strVIPID);
	SetDlgItemText(IDC_EDIT_TEL, m_strTel);
	SetDlgItemText(IDC_EDIT_EMAIL, m_strEmail);
	SetDlgItemText(IDC_EDIT_ADDRESS, m_strAddress);
	SetDlgItemText(IDC_EDIT_COMMENT, m_strOtherInfo);
	//
	CComboBox* pSexComoboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SEX);
	pSexComoboBox->ResetContent();
	pSexComoboBox->AddString(STR_FEMALE);
	pSexComoboBox->AddString(STR_MALE);
	pSexComoboBox->SetCurSel(m_bSexIsFemale ? 0 : 1);
	//
	CDateTimeCtrl* pBirthDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_BIRTH);
	pBirthDateCtrl->SetTime(&m_dateBirth);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//----------------------------------------------------
void CAFUMemberDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_NAME, m_strName);
	GetDlgItemText(IDC_EDIT_VIPID, m_strVIPID);
	GetDlgItemText(IDC_EDIT_TEL, m_strTel);
	GetDlgItemText(IDC_EDIT_EMAIL, m_strEmail);
	GetDlgItemText(IDC_EDIT_ADDRESS, m_strAddress);
	GetDlgItemText(IDC_EDIT_COMMENT, m_strOtherInfo);
	//
	CComboBox* pSexComoboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SEX);
	m_bSexIsFemale = (0 == pSexComoboBox->GetCurSel()) ? TRUE : FALSE;
	//
	CDateTimeCtrl* pBirthDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_BIRTH);
	pBirthDateCtrl->GetTime(m_dateBirth);

	OnOK();
}
//----------------------------------------------------


//----------------------------------------------------
void CAFUMemberDlg::SetInitializeData(CString strName, 
									  DWORD dwVIPID, 
									  BOOL bSexIsFemale, 
									  CString strTel, 
									  CTime& dateBirth, 
									  CString strEmail, 
									  CString strAddress, 
									  CString strOther)
{
	m_strName = strName;
	m_strVIPID.Format(_T("%d"), dwVIPID);
	m_bSexIsFemale = bSexIsFemale;
	m_strTel = strTel;
	m_dateBirth = dateBirth;
	m_strEmail = strEmail;
	m_strAddress = strAddress;
	m_strOtherInfo = strOther;
}	
//----------------------------------------------------
void CAFUMemberDlg::GetUpdatedData(CString &strName, 
								   DWORD &dwVIPID, 
								   BOOL &bSexIsFemale, 
								   CString &strTel, 
								   CTime& dateBirth, 
								   CString &strEmail, 
								   CString &strAddress, 
								   CString &strOther)
{
	strName = m_strName;
	dwVIPID = _ttol(m_strVIPID);
	bSexIsFemale = m_bSexIsFemale;
	strTel = m_strTel;
	dateBirth = m_dateBirth;
	strEmail = m_strEmail;
	strAddress = m_strAddress;
	strOther = m_strOtherInfo;
}
//----------------------------------------------------
