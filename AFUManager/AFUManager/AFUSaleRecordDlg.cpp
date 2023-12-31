//----------------------------------------------------
//	File Name:AFUSaleRecordDlg.cpp
//	Introduce:Dialog class CAFUSaleRecordDlg implementation file
//----------------------------------------------------
#include "stdafx.h"
#include "AFUManager.h"
#include "AFUSaleRecordDlg.h"
//----------------------------------------------------
extern CADODataBase*	g_pDatabase;
//----------------------------------------------------

//----------------------------------------------------
// CAFUSaleRecordDlg dialog
//----------------------------------------------------
IMPLEMENT_DYNAMIC(CAFUSaleRecordDlg, CDialog)
CAFUSaleRecordDlg::CAFUSaleRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAFUSaleRecordDlg::IDD, pParent)
{
	m_pDatabase = NULL;
	m_bIsOldVIPMember = TRUE;
	m_tmDate = CTime::GetCurrentTime();
}
//----------------------------------------------------
CAFUSaleRecordDlg::~CAFUSaleRecordDlg()
{
}
//----------------------------------------------------
void CAFUSaleRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//----------------------------------------------------
BEGIN_MESSAGE_MAP(CAFUSaleRecordDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_OLDVIP, &CAFUSaleRecordDlg::OnBnClickedRadioOldvip)
	ON_BN_CLICKED(IDC_RADIO_NEWVIP, &CAFUSaleRecordDlg::OnBnClickedRadioNewvip)
	ON_BN_CLICKED(IDC_BTN_FINDADD, &CAFUSaleRecordDlg::OnBnClickedBtnFindadd)
	ON_CBN_SELCHANGE(IDC_COMBO_GOODSNAME, &CAFUSaleRecordDlg::OnCbnSelchangeComboGoodsname)
	ON_BN_CLICKED(IDOK, &CAFUSaleRecordDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_PRICE, &CAFUSaleRecordDlg::OnEnChangeEditPrice)
	ON_EN_CHANGE(IDC_EDIT_NUM, &CAFUSaleRecordDlg::OnEnChangeEditNum)
END_MESSAGE_MAP()
//----------------------------------------------------


//----------------------------------------------------
// CAFUSaleRecordDlg message handlers
//----------------------------------------------------
BOOL CAFUSaleRecordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CButton* pBtnOldVIP = (CButton*)GetDlgItem(IDC_RADIO_OLDVIP);
	pBtnOldVIP->SetCheck(BST_CHECKED);
	m_bIsOldVIPMember = TRUE;

	// If edit, so disable "Old VIP" and "New VIP" radio buttons
	if (m_dwVIPID > 0)
	{
		GetDlgItem(IDC_RADIO_OLDVIP)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_NEWVIP)->EnableWindow(FALSE);
	}

	CComboBox* pSexComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SEXLY);
	pSexComboBox->AddString(STR_FEMALE);
	pSexComboBox->AddString(STR_MALE);

	_FillGoodsComboBox();
	_FillEmployeeComboBox();
	_InitControlData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//----------------------------------------------------
void CAFUSaleRecordDlg::OnBnClickedRadioOldvip()
{
	m_bIsOldVIPMember = TRUE;
	GetDlgItem(IDC_EDIT_VIPID)->SetFocus();
	SetDlgItemText(IDC_BTN_FINDADD, _T("查      找"));

	CEdit* pEditName = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
	pEditName->SetWindowText(_T(""));
	pEditName->SetReadOnly(TRUE);
	//
	GetDlgItem(IDC_COMBO_SEXLY)->EnableWindow(FALSE);
	GetDlgItem(IDC_DATETIMEPICKER_BIRTH)->EnableWindow(FALSE);
	//
	CEdit* pTelName = (CEdit*)GetDlgItem(IDC_EDIT_TEL);
	pTelName->SetWindowText(_T(""));
	pTelName->SetReadOnly(TRUE);
	//
	CEdit* pEMailName = (CEdit*)GetDlgItem(IDC_EDIT_EMAIL);
	pEMailName->SetWindowText(_T(""));
	pEMailName->SetReadOnly(TRUE);
	//
	CEdit* pAddressName = (CEdit*)GetDlgItem(IDC_EDIT_ADDRESS);
	pAddressName->SetWindowText(_T(""));
	pAddressName->SetReadOnly(TRUE);	
}
//----------------------------------------------------
void CAFUSaleRecordDlg::OnBnClickedRadioNewvip()
{
	m_bIsOldVIPMember = FALSE;
	GetDlgItem(IDC_EDIT_VIPID)->SetFocus();
	SetDlgItemText(IDC_BTN_FINDADD, _T("添      加"));

	CEdit* pEditName = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
	pEditName->SetWindowText(_T(""));
	pEditName->SetReadOnly(FALSE);
	//
	GetDlgItem(IDC_COMBO_SEXLY)->EnableWindow(TRUE);
	GetDlgItem(IDC_DATETIMEPICKER_BIRTH)->EnableWindow(TRUE);
	//
	CEdit* pTelName = (CEdit*)GetDlgItem(IDC_EDIT_TEL);
	pTelName->SetWindowText(_T(""));
	pTelName->SetReadOnly(FALSE);
	//
	CEdit* pEMailName = (CEdit*)GetDlgItem(IDC_EDIT_EMAIL);
	pEMailName->SetWindowText(_T(""));
	pEMailName->SetReadOnly(FALSE);
	//
	CEdit* pAddressName = (CEdit*)GetDlgItem(IDC_EDIT_ADDRESS);
	pAddressName->SetWindowText(_T(""));
	pAddressName->SetReadOnly(FALSE);
}
//----------------------------------------------------
void CAFUSaleRecordDlg::OnBnClickedBtnFindadd()
{
	if (TRUE == m_bIsOldVIPMember)
	{
		_FillVIPMemberInfomation();
	}
	else
	{
		_AddNewVIPMemberInfomation();
	}
}
//----------------------------------------------------
void CAFUSaleRecordDlg::OnCbnSelchangeComboGoodsname()
{
	CComboBox* pGoodsComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GOODSNAME);
	int iSelIndex = pGoodsComboBox->GetCurSel();
	float fPrice = pGoodsComboBox->GetItemData(iSelIndex) / 1000.0f;

	CString strPrice;
	strPrice.Format(_T("%.2f"), fPrice);
	SetDlgItemText(IDC_EDIT_PRICE, strPrice);
}
//----------------------------------------------------
void CAFUSaleRecordDlg::OnEnChangeEditPrice()
{
	CString strPrice;
	CString strCount;
	GetDlgItemText(IDC_EDIT_PRICE, strPrice);
	GetDlgItemText(IDC_EDIT_NUM, strCount);
	float fPrice = (float)_tstof(strPrice);
	long lCount = _ttol(strCount);
	float fTotalCost = lCount * fPrice;
	//
	CString strTotalCost;
	strTotalCost.Format(_T("%.2f"), fTotalCost);
	SetDlgItemText(IDC_EDIT_COST, strTotalCost);
}
//----------------------------------------------------
void CAFUSaleRecordDlg::OnEnChangeEditNum()
{
	CString strPrice;
	CString strCount;
	GetDlgItemText(IDC_EDIT_PRICE, strPrice);
	GetDlgItemText(IDC_EDIT_NUM, strCount);
	float fPrice = (float)_tstof(strPrice);
	long lCount = _ttol(strCount);
	float fTotalCost = lCount * fPrice;
	//
	CString strTotalCost;
	strTotalCost.Format(_T("%.2f"), fTotalCost);
	SetDlgItemText(IDC_EDIT_COST, strTotalCost);
}
//----------------------------------------------------
void CAFUSaleRecordDlg::OnBnClickedOk()
{
	CString strVIPID;
	CString strVIPName;
	GetDlgItemText(IDC_EDIT_VIPID, strVIPID);
	GetDlgItemText(IDC_EDIT_NAME, strVIPName);
	if (strVIPID.GetLength() == 0)
	{
		MessageBox(_T("请输入会员信息！"), _T("警告"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_VIPID)->SetFocus();
		return;
	}
	else if (strVIPName.GetLength() == 0) 
	{
		if (TRUE == m_bIsOldVIPMember) 
		{
			if (FALSE == _FillVIPMemberInfomation())
				return;
		}
		else
		{
			MessageBox(_T("请输入会员姓名！"), _T("警告"), MB_OK | MB_ICONWARNING);
			GetDlgItem(IDC_EDIT_NAME)->SetFocus();
			return;
		}
	}

	m_dwVIPID = _ttol(strVIPID);
	m_strVIPName = strVIPName;
	//
	CString strPrice;
	CString strCount;
	CString strTotalCost;
	CString strPoints;
	CDateTimeCtrl* pSaleDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_DATE);
	pSaleDateCtrl->GetTime(m_tmDate);
	GetDlgItemText(IDC_EDIT_SALENAME, m_strSaleName);
	GetDlgItemText(IDC_COMBO_GOODSNAME, m_strGoodsName);
	GetDlgItemText(IDC_EDIT_PRICE, strPrice);
	GetDlgItemText(IDC_EDIT_NUM, strCount);
	GetDlgItemText(IDC_EDIT_COST, strTotalCost);
	GetDlgItemText(IDC_EDIT_NEWPOINTS, strPoints);
	GetDlgItemText(IDC_COMBO_EMPLOYEE, m_strEmployee);
	GetDlgItemText(IDC_EDIT_COMMENT, m_strOther);
	CComboBox* pEmployeeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_EMPLOYEE);
	int iSelIndex = pEmployeeComboBox->GetCurSel();
	m_fPrice = (float)_tstof(strPrice);
	m_lCount = _ttol(strCount);
	m_fTotalCost = (float)_tstof(strTotalCost);
	m_lPoints = _ttol(strPoints);

	OnOK();
}
//----------------------------------------------------




//----------------------------------------------------
void CAFUSaleRecordDlg::SetDatabaseAndTableName(CADODataBase* pDatabase, 
												CString strVIPMemberTable, 
												CString strGoodsTable, 
												CString strEmployeeTable)
{
	m_pDatabase = pDatabase;
	m_strVIPMemberTable = strVIPMemberTable;
	m_strGoodsTable = strGoodsTable;
	m_strEmployeeTable = strEmployeeTable;
}
//----------------------------------------------------
void CAFUSaleRecordDlg::SetSaleRecordInfo(DWORD dwVIPID, 
										  CTime &date, 
										  CString strSaleName, 
										  CString strGoodsName, 
										  float fPrice, 
										  LONG lCount, 
										  float fTotalCost,
										  LONG lPoints, 
										  CString strEmployee, 
										  CString strOther)
{
	m_dwVIPID = dwVIPID;
	m_tmDate = date;
	m_strSaleName = strSaleName;
	m_strGoodsName = strGoodsName;
	m_fPrice = fPrice;
	m_lCount = lCount;
	m_fTotalCost = fTotalCost;
	m_lPoints = lPoints;
	m_strEmployee = strEmployee;
	m_strOther = strOther;
}
//----------------------------------------------------
void CAFUSaleRecordDlg::GetSaleRecordInfo(DWORD &dwVIPID, 
										  CString &strVIPName,
										  CTime &date, 
										  CString &strSaleName, 
										  CString &strGoodsName, 
										  float &fPrice, 
										  LONG &lCount, 
										  float &fTotalCost,
										  LONG &lPoints, 
										  CString &strEmployee, 
										  CString &strOther)
{
	dwVIPID = m_dwVIPID;
	strVIPName = m_strVIPName;
	date = m_tmDate;
	strSaleName = m_strSaleName;
	strGoodsName = m_strGoodsName;
	fPrice = m_fPrice;
	lCount = m_lCount;
	fTotalCost = m_fTotalCost;
	lPoints = m_lPoints;
	strEmployee = m_strEmployee;
	strOther = m_strOther;
}
//----------------------------------------------------




//----------------------------------------------------
void CAFUSaleRecordDlg::_FillGoodsComboBox()
{
	CComboBox* pGoodsComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GOODSNAME);
	pGoodsComboBox->ResetContent();

	CADOTable tbGoods;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT Name, Price FROM %s", m_strGoodsTable);
	if (tbGoods.OpenTable(m_pDatabase->GetConnectionPtr(), strOpenOperation))
	{
		DWORD dwRecordCnt = tbGoods.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwRecordCnt; dwIndex++)
		{
			float fGoodsPrice = 0.0f;
			WCHAR wzGoodsName[128] = {0};
			tbGoods.GetRecordCell(dwIndex, L"Name", wzGoodsName, 128);
			tbGoods.GetRecordCell(dwIndex, L"Price", fGoodsPrice);
			//
			int iIndex = pGoodsComboBox->AddString(wzGoodsName);
			pGoodsComboBox->SetItemData(iIndex, (int)(1000*fGoodsPrice));
		}
		tbGoods.CloseTable();
		pGoodsComboBox->SetCurSel(0);
		OnCbnSelchangeComboGoodsname();
	}
}
//----------------------------------------------------
void CAFUSaleRecordDlg::_FillEmployeeComboBox()
{
	CComboBox* pEmployeeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_EMPLOYEE);
	pEmployeeComboBox->ResetContent();

	CADOTable tbEmployees;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT ID, Name FROM %s", m_strEmployeeTable);
	if (tbEmployees.OpenTable(m_pDatabase->GetConnectionPtr(), strOpenOperation))
	{
		DWORD dwRecordCnt = tbEmployees.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwRecordCnt; dwIndex++)
		{
			DWORD dwEmployeeID = 0;
			WCHAR wzEmployeeName[128] = {0};
			tbEmployees.GetRecordCell(dwIndex, L"Name", wzEmployeeName, 128);
			tbEmployees.GetRecordCell(dwIndex, L"ID", dwEmployeeID);
			//
			int iIndex = pEmployeeComboBox->AddString(wzEmployeeName);
			pEmployeeComboBox->SetItemData(iIndex, dwEmployeeID);
		}
		tbEmployees.CloseTable();
		pEmployeeComboBox->SetCurSel(0);
	}
}
//----------------------------------------------------
void CAFUSaleRecordDlg::_InitControlData()
{
	// Fill VIP member information
	CString strVIPID;
	strVIPID.Format(_T("%d"), m_dwVIPID);
	SetDlgItemText(IDC_EDIT_VIPID, strVIPID);
	if (0 != m_dwVIPID)	OnBnClickedBtnFindadd();

	// Fill sale record information
	CDateTimeCtrl* pDateDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_DATE);
	pDateDateCtrl->SetTime(&m_tmDate);
	//
	SetDlgItemText(IDC_EDIT_SALENAME, m_strSaleName);
	//
	int iSelIndex = -1;
	CComboBox* pGoodsNameComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GOODSNAME);
	int iItemCnt = pGoodsNameComboBox->GetCount();
	for (int iIndex = 0; iIndex < iItemCnt; iIndex++)
	{
		CString strItemText;
		pGoodsNameComboBox->GetLBText(iIndex, strItemText);
		if (strItemText == m_strGoodsName)
		{
			iSelIndex = iIndex;
			break;
		}
	}
	if (-1 == iSelIndex)
	{
		iSelIndex = 0;
		m_fPrice = pGoodsNameComboBox->GetItemData(0) / 1000.0f;
	}
	//
	CString strPrice;
	strPrice.Format(_T("%.2f"), m_fPrice);
	SetDlgItemText(IDC_EDIT_PRICE, strPrice);
	//
	CString strCount;
	strCount.Format(_T("%d"), m_lCount);
	SetDlgItemText(IDC_EDIT_NUM, strCount);
	//
	CString strTotalCost;
	m_fTotalCost = m_fTotalCost > 0 ? m_fTotalCost : m_fPrice*m_lCount;
	strTotalCost.Format(_T("%.2f"), m_fTotalCost);
	SetDlgItemText(IDC_EDIT_COST, strTotalCost);
	//
	CString strPoints;
	strPoints.Format(_T("%d"), m_lPoints);
	SetDlgItemText(IDC_EDIT_NEWPOINTS, strPoints);
	//
	CComboBox* pEmployeeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_EMPLOYEE);
	iItemCnt = pEmployeeComboBox->GetCount();
	for (int iIndex = 0; iIndex < iItemCnt; iIndex++)
	{
		CString strItemText;
		pEmployeeComboBox->GetLBText(iIndex, strItemText);
		if (m_strEmployee == strItemText)
		{
			pEmployeeComboBox->SetCurSel(iIndex);
			break;
		}
	}
	//
	SetDlgItemText(IDC_EDIT_COMMENT, m_strOther);
}
//----------------------------------------------------
BOOL CAFUSaleRecordDlg::_FillVIPMemberInfomation()
{
	LONG lVIPID = 0;
	WCHAR wcVIPID[32] = {0};
	GetDlgItemText(IDC_EDIT_VIPID, wcVIPID, 32);
	lVIPID = _wtol(wcVIPID);

	CADOTable tbVIPMember;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE VIPID=%d", m_strVIPMemberTable, lVIPID);
	if (tbVIPMember.OpenTable(m_pDatabase->GetConnectionPtr(), strOpenOperation))
	{
		DWORD dwRecordCnt = tbVIPMember.GetRecordCnt();
		if (0 == dwRecordCnt)
		{
			MessageBox(_T("没有找到该会员的信息！"), _T("警告"), MB_OK | MB_ICONWARNING);
			return FALSE;
		}

		WCHAR wcVIPMemberName[64] = {0};
		WCHAR wcSex[64] = {0};
		WCHAR wcTel[64] = {0};
		WCHAR wcEMail[64] = {0};
		WCHAR wcAddress[64] = {0};
		CTime dateTime = CTime();
		tbVIPMember.GetRecordCell(0, L"Name", wcVIPMemberName, 64);
		tbVIPMember.GetRecordCell(0, L"Sex", wcSex, 64);
		tbVIPMember.GetRecordCell(0, L"Tel", wcTel, 64);
		tbVIPMember.GetRecordCell(0, L"Email", wcEMail, 64);
		tbVIPMember.GetRecordCell(0, L"Address", wcAddress, 64);
		tbVIPMember.GetRecordCell(0, L"Birth", dateTime);
		tbVIPMember.CloseTable();
		//
		SetDlgItemText(IDC_EDIT_NAME, wcVIPMemberName);
		SetDlgItemText(IDC_EDIT_TEL, wcTel);
		SetDlgItemText(IDC_EDIT_EMAIL, wcEMail);
		SetDlgItemText(IDC_EDIT_ADDRESS, wcAddress);
		CComboBox* pSexComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SEXLY);
		pSexComboBox->SetCurSel(wcscmp(wcSex, STR_FEMALE) == 0 ? 0 : 1);
		CDateTimeCtrl* pBirthDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_BIRTH);
		pBirthDateCtrl->SetTime(&dateTime);
	}

	return TRUE;
}
//----------------------------------------------------
BOOL CAFUSaleRecordDlg::_AddNewVIPMemberInfomation()
{
	CADOTable tbVIPMember;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s", m_strVIPMemberTable);
	if (tbVIPMember.OpenTable(m_pDatabase->GetConnectionPtr(), strOpenOperation))
	{
		DWORD dwVIPID = 0;
		WCHAR wcVIPID[64] = {0};
		WCHAR wcVIPMemberName[64] = {0};
		WCHAR wcTel[64] = {0};
		WCHAR wcEMail[64] = {0};
		WCHAR wcAddress[64] = {0};
		CTime dateTime = CTime();

		GetDlgItemText(IDC_EDIT_VIPID, wcVIPID, 64);
		GetDlgItemText(IDC_EDIT_NAME, wcVIPMemberName, 64);
		GetDlgItemText(IDC_EDIT_TEL, wcTel, 64);
		GetDlgItemText(IDC_EDIT_EMAIL, wcEMail, 64);
		GetDlgItemText(IDC_EDIT_ADDRESS, wcAddress, 64);
		//
		dwVIPID = _wtol(wcVIPID);
		CDateTimeCtrl* pBirthDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_BIRTH);
		pBirthDateCtrl->GetTime(dateTime);
		CComboBox* pSexComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SEXLY);
		int iSelSex = pSexComboBox->GetCurSel();

		tbVIPMember.AddNewItem();
		tbVIPMember.SetRecordCell(-1, L"VIPID", dwVIPID);
		tbVIPMember.SetRecordCell(-1, L"Name", wcVIPMemberName);
		tbVIPMember.SetRecordCell(-1, L"Sex", (0 == iSelSex) ? STR_FEMALE : STR_MALE);
		tbVIPMember.SetRecordCell(-1, L"Tel", wcTel);
		tbVIPMember.SetRecordCell(-1, L"Email", wcEMail);
		tbVIPMember.SetRecordCell(-1, L"Address", wcAddress);
		tbVIPMember.SetRecordCell(-1, L"Birth", dateTime);
		//
		BOOL bRes = tbVIPMember.UpdateTable();
		tbVIPMember.CloseTable();
		if (TRUE == bRes)
		{
			MessageBox(_T("新的会员信息添加成功！"), _T("成功"), MB_OK | MB_ICONINFORMATION);
			return TRUE;
		}
	}

	return FALSE;
}
//----------------------------------------------------