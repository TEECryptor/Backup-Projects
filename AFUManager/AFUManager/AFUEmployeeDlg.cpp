//----------------------------------------------------
//	File Name: AFUEmployeeDlg.cpp
//	Introduce: Class CAFUEmployeeDlg implement file
//
//----------------------------------------------------
#include "stdafx.h"
#include "AFUManager.h"
#include "AFUEmployeeDlg.h"
//----------------------------------------------------


//----------------------------------------------------
//	Name:CAFUEmployeeDlg
//	Introduce:Construction
//----------------------------------------------------
IMPLEMENT_DYNAMIC(CAFUEmployeeDlg, CDialog)
CAFUEmployeeDlg::CAFUEmployeeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAFUEmployeeDlg::IDD, pParent)
{

}
//----------------------------------------------------
CAFUEmployeeDlg::~CAFUEmployeeDlg()
{
}
//----------------------------------------------------
void CAFUEmployeeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//----------------------------------------------------
BEGIN_MESSAGE_MAP(CAFUEmployeeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAFUEmployeeDlg::OnBnClickedOk)
END_MESSAGE_MAP()
//----------------------------------------------------


//----------------------------------------------------
// CAFUEmployeeDlg message handlers
//----------------------------------------------------
BOOL CAFUEmployeeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strID;
	strID.Format(_T("%d"), m_dwEmployeeID);
	SetDlgItemText(IDC_EDIT_EMPLOYEEID, strID);	
	SetDlgItemText(IDC_EDIT_USERNAME, m_strUserName);
	SetDlgItemText(IDC_EDIT_NAME, m_strEmployeeName);
	SetDlgItemText(IDC_EDIT_TEL, m_strTel);
	SetDlgItemText(IDC_EDIT_ADDRESS, m_strAddress);
	//
	CComboBox* pGroupComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GROUP);
	int iIndex = pGroupComboBox->AddString(_T("系统管理员"));
	pGroupComboBox->SetItemData(iIndex, eEmployeeGroup_Admin);
	iIndex = pGroupComboBox->AddString(_T("普通用户"));
	pGroupComboBox->SetItemData(iIndex, eEmployeeGroup_Normal);
	iIndex = pGroupComboBox->AddString(_T("浏览用户"));
	pGroupComboBox->SetItemData(iIndex, eEmployeeGroup_Viewer);
	for (int iIndex = 0; iIndex < pGroupComboBox->GetCount(); iIndex++)
	{
		DWORD dwItemData = pGroupComboBox->GetItemData(iIndex);
		if (m_eUserGroup == (eEmployeeGroup)dwItemData)
		{
			pGroupComboBox->SetCurSel(iIndex);
			break;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//----------------------------------------------------
void CAFUEmployeeDlg::OnBnClickedOk()
{
	CString strID;
	GetDlgItemText(IDC_EDIT_EMPLOYEEID, strID);
	GetDlgItemText(IDC_EDIT_USERNAME, m_strUserName);
	GetDlgItemText(IDC_EDIT_NAME, m_strEmployeeName);
	GetDlgItemText(IDC_EDIT_TEL, m_strTel);
	GetDlgItemText(IDC_EDIT_ADDRESS, m_strAddress);
	m_dwEmployeeID = _ttol(strID);
	//
	if (strID.IsEmpty())
	{
		MessageBox(_T("员工工号不能为空！"), _T("警告"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_EMPLOYEEID)->SetFocus();
		return;
	}
	if (m_strUserName.IsEmpty())
	{
		MessageBox(_T("员工账号用户名不能为空！"), _T("警告"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_USERNAME)->SetFocus();
		return;
	}
	if (m_strEmployeeName.IsEmpty())
	{
		MessageBox(_T("员工姓名不能为空！"), _T("警告"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_NAME)->SetFocus();
		return;
	}

	CComboBox* pGroupComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GROUP);
	int iIndex = pGroupComboBox->GetCurSel();
	m_eUserGroup = (eEmployeeGroup)pGroupComboBox->GetItemData(iIndex);

	OnOK();
}
//----------------------------------------------------


//----------------------------------------------------
void CAFUEmployeeDlg::SetInitializeData(DWORD dwEmployeeID, 
										CString strUserName,
										CString strEmployeeName, 
										eEmployeeGroup eGroup, 
										CString strTel, 
										CString strAddress)
{
	m_dwEmployeeID = dwEmployeeID;
	m_strUserName = strUserName;
	m_strEmployeeName = strEmployeeName;
	m_eUserGroup = eGroup;
	m_strTel = strTel;
	m_strAddress = strAddress;
}	
//----------------------------------------------------
void CAFUEmployeeDlg::GetUpdatedData(DWORD &dwEmployeeID, 
									CString &strUserName,
									 CString &strEmployeeName, 
									 eEmployeeGroup &eGroup, 
									 CString &strTel, 
									 CString &strAddress)
{
	dwEmployeeID = m_dwEmployeeID;
	strUserName = m_strUserName;
	strEmployeeName = m_strEmployeeName;
	eGroup = m_eUserGroup;
	strTel = m_strTel;
	strAddress = m_strAddress;
}
//----------------------------------------------------

