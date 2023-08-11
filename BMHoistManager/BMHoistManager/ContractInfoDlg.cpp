// ContractInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BMHoistManager.h"
#include "ContractInfoDlg.h"


// CContractInfoDlg dialog

IMPLEMENT_DYNAMIC(CContractInfoDlg, CDialog)

CContractInfoDlg::CContractInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContractInfoDlg::IDD, pParent)
{
	m_pEquipmentTable = NULL;
	m_strContractID = _T("");
	m_strCustomContractID = _T("");
	m_strContactTel = _T("");
	m_strProjectName = _T("");
	m_strEquipment = _T("");
	m_fTotalPrice = 0.0f;
	m_strSaler = _T("");
	m_strComment = _T("");
	m_strEquipment2 = _T("");
	m_strModel = _T("");
	m_dwCount2 = 1;
	m_fPrice = 0.0f;
	m_fTotalPrice2 = 0.0f;
	m_strComment2 = _T("");	
	m_strCurSelCustomShortName = _T("");
	m_strCurSelCustomContact = _T("");
}

CContractInfoDlg::~CContractInfoDlg()
{
}

void CContractInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CONTRACTDLG_TYPE, m_ContractTypeCombox);
	DDX_Control(pDX, IDC_COMBO_CONTRACTDLG_CUSTOM, m_CustomNameCombox);
	DDX_Control(pDX, IDC_COMBO_CONTRACTDLG_CONTACT, m_ContactCombox);
	DDX_Control(pDX, IDC_COMBO_CONTRACTDLG_STATUS, m_ContractStatusCombox);
	DDX_Control(pDX, IDC_LIST_CONTRACTDLG_EQUIPMENT, m_EquipmentList);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_ID, m_strContractID);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_ID2, m_strCustomContractID);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_TEL, m_strContactTel);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_PROJECT, m_strProjectName);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_EQUIPMENT, m_strEquipment);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_TOTALPRICE, m_fTotalPrice);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_SALER, m_strSaler);	
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_COMMENT, m_strComment);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_EQUIPMENT2, m_strEquipment2);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_MODEL, m_strModel);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_COUNT2, m_dwCount2);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_PRICE, m_fPrice);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_TOTALPRICE2, m_fTotalPrice2);
	DDX_Text(pDX, IDC_EDIT_CONTRACTDLG_COMMENT2, m_strComment2);
	DDX_Control(pDX, IDC_DATETIMEPICKER_CONTRACTDLG_START, m_ContractStartDateCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER_CONTRACTDLG_END, m_ContractEndDateCtrl);
}


BEGIN_MESSAGE_MAP(CContractInfoDlg, CDialog)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_CONTRACTDLG_CUSTOM, &CContractInfoDlg::OnCbnSelchangeComboContractdlgCustom)
	ON_CBN_SELCHANGE(IDC_COMBO_CONTRACTDLG_CONTACT, &CContractInfoDlg::OnCbnSelchangeComboContractdlgContact)
	ON_BN_CLICKED(IDC_BTN_CONTRACTDLG_EQUIPMENT_NEW, &CContractInfoDlg::OnBnClickedBtnContractdlgEquipmentNew)
	ON_BN_CLICKED(IDC_BTN_CONTRACTDLG_EQUIPMENT_MODIFY, &CContractInfoDlg::OnBnClickedBtnContractdlgEquipmentModify)
	ON_BN_CLICKED(IDC_BTN_CONTRACTDLG_EQUIPMENT_DELETE, &CContractInfoDlg::OnBnClickedBtnContractdlgEquipmentDelete)
	ON_BN_CLICKED(IDOK, &CContractInfoDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTRACTDLG_EQUIPMENT, &CContractInfoDlg::OnNMClickListContractdlgEquipment)
	ON_BN_CLICKED(IDC_BTN_CONTRACTDLG_NEW, &CContractInfoDlg::OnBnClickedBtnContractdlgNew)
	ON_BN_CLICKED(IDC_BTN_CONTRACTDLG_DELETE, &CContractInfoDlg::OnBnClickedBtnContractdlgDelete)
	ON_EN_CHANGE(IDC_EDIT_CONTRACTDLG_COUNT2, &CContractInfoDlg::OnEnChangeEditContractdlgCount2)
	ON_EN_CHANGE(IDC_EDIT_CONTRACTDLG_PRICE, &CContractInfoDlg::OnEnChangeEditContractdlgPrice)
END_MESSAGE_MAP()


// CContractInfoDlg message handlers

BOOL CContractInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	FillEquipmentsComboBox(&m_ContractTypeCombox);
	_FillCustomNameComboBox();
	//
	m_ContractStatusCombox.ResetContent();
	m_ContractStatusCombox.AddString(_T("进行中"));
	m_ContractStatusCombox.AddString(_T("订单"));
	m_ContractStatusCombox.AddString(_T("延期"));
	m_ContractStatusCombox.AddString(_T("取消"));
	m_ContractStatusCombox.AddString(_T("丢单"));
	m_ContractStatusCombox.SetCurSel(0);
	//	
	CTime tm = CTime::GetCurrentTime();
	m_ContractStartDateCtrl.SetTime(&tm);
	m_ContractEndDateCtrl.SetTime(&tm);
	//
	InitListHeader(&m_EquipmentList, &g_Tables[eTableID_ContractInfo]);
	m_EquipmentList.SetExtendedStyle(m_EquipmentList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	//
	_GetInitializeData();
	//
	GetDlgItem(IDC_EDIT_CONTRACTDLG_PROJECT)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CContractInfoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CContractInfoDlg::OnCbnSelchangeComboContractdlgCustom()
{
	CString strCurSelCustom;
	m_CustomNameCombox.GetWindowText(strCurSelCustom);

	m_strCurSelCustomShortName = _T("");
	m_strCurSelCustomContact = _T("");

	CADOTable tbCustoms;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT ShortName,Contact FROM %s WHERE FullName = '%s'", 
		g_Tables[eTableID_Custom].strName, strCurSelCustom);
	if (tbCustoms.OpenTable(g_pDatabase->GetConnectionPtr(), strOpenOperation))
	{
		DWORD dwRecordCnt = tbCustoms.GetRecordCnt();
		if (dwRecordCnt > 0)
		{
			tbCustoms.GetRecordCell(0, L"ShortName", m_strCurSelCustomShortName);	
			tbCustoms.GetRecordCell(0, L"Contact", m_strCurSelCustomContact);		
		}
		tbCustoms.CloseTable();
	}

	_FillContactComboBox();
}

void CContractInfoDlg::OnCbnSelchangeComboContractdlgContact()
{
	CString strCurSelContact;
	m_ContactCombox.GetWindowText(strCurSelContact);

	m_strContactTel = _T("");

	CADOTable tbContacts;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT Tel FROM %s WHERE ContactName = '%s'", 
		g_Tables[eTableID_Contact].strName, strCurSelContact);
	if (tbContacts.OpenTable(g_pDatabase->GetConnectionPtr(), strOpenOperation))
	{
		DWORD dwRecordCnt = tbContacts.GetRecordCnt();
		if (dwRecordCnt > 0)
		{
			tbContacts.GetRecordCell(0, L"Tel", m_strContactTel);
		}
	}
	SetDlgItemText(IDC_EDIT_CONTRACTDLG_TEL, m_strContactTel);
}

void CContractInfoDlg::OnNMClickListContractdlgEquipment(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	DWORD dwCount2 = 0;
	float fPrice = 0.0f;
	float fTotalPrice2 = 0.0f;

	// Get current selected item
	POSITION pos = m_EquipmentList.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		m_strEquipment2 = _T("");
		m_strModel = _T("");
		dwCount2 = 0;
		fPrice = 0.0f;
		fTotalPrice2 = 0.0f;
		m_strComment2 = _T("");
	}
	else
	{
		int iSelItem = m_EquipmentList.GetNextSelectedItem(pos);
		m_strEquipment2 = m_EquipmentList.GetItemText(iSelItem, 2);
		m_strModel = m_EquipmentList.GetItemText(iSelItem, 3);
		dwCount2 = _ttol(m_EquipmentList.GetItemText(iSelItem, 4));
		fPrice = (float)_tstof(m_EquipmentList.GetItemText(iSelItem, 5));
		fTotalPrice2 = (float)_tstof(m_EquipmentList.GetItemText(iSelItem, 6));
		m_strComment2 = m_EquipmentList.GetItemText(iSelItem, 7);
	}

	CString strValue;
	SetDlgItemText(IDC_EDIT_CONTRACTDLG_EQUIPMENT2, m_strEquipment2);
	SetDlgItemText(IDC_EDIT_CONTRACTDLG_MODEL, m_strModel);
	strValue.Format(_T("%d"), dwCount2);
	SetDlgItemText(IDC_EDIT_CONTRACTDLG_COUNT2, strValue);
	strValue.Format(_T("%.02f"), fPrice);
	SetDlgItemText(IDC_EDIT_CONTRACTDLG_PRICE, strValue);
	strValue.Format(_T("%.02f"), fTotalPrice2);
	SetDlgItemText(IDC_EDIT_CONTRACTDLG_TOTALPRICE2, strValue);
	SetDlgItemText(IDC_EDIT_CONTRACTDLG_COMMENT2, m_strComment2);
	//
	m_dwCount2 = dwCount2;
	m_fPrice = fPrice;
	m_fTotalPrice2 = fTotalPrice2;

	*pResult = 0;
}

void CContractInfoDlg::OnEnChangeEditContractdlgCount2()
{
	UpdateData(TRUE);

	m_fTotalPrice2 = m_fPrice * m_dwCount2;

	CString strValue;
	strValue.Format(_T("%02f"), m_fTotalPrice2);
	SetDlgItemText(IDC_EDIT_CONTRACTDLG_TOTALPRICE2, strValue);
}

void CContractInfoDlg::OnEnChangeEditContractdlgPrice()
{
	UpdateData(TRUE);

	m_fTotalPrice2 = m_fPrice * m_dwCount2;

	CString strValue;
	strValue.Format(_T("%02f"), m_fTotalPrice2);
	SetDlgItemText(IDC_EDIT_CONTRACTDLG_TOTALPRICE2, strValue);
}

void CContractInfoDlg::OnBnClickedBtnContractdlgEquipmentNew()
{	
	UpdateData(TRUE);

	if (m_strEquipment2.IsEmpty())
	{
		MessageBox(_T("设备名 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strModel.IsEmpty())
	{
		MessageBox(_T("型号 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_dwCount2 == 0)
	{
		MessageBox(_T("数量 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_fPrice == 0)
	{
		MessageBox(_T("价格 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}

	m_pEquipmentTable->AddNewItem();
	m_pEquipmentTable->SetRecordCell(-1, L"OfferID", m_strContractID);
	m_pEquipmentTable->SetRecordCell(-1, L"Equipment", m_strEquipment2);
	m_pEquipmentTable->SetRecordCell(-1, L"Model", m_strModel);
	m_pEquipmentTable->SetRecordCell(-1, L"Number", m_dwCount2);
	m_pEquipmentTable->SetRecordCell(-1, L"Price", m_fPrice);
	m_pEquipmentTable->SetRecordCell(-1, L"Cost", m_fTotalPrice2);
	m_pEquipmentTable->SetRecordCell(-1, L"Comment", m_strComment2);
	
	if (TRUE == m_pEquipmentTable->UpdateTable())
	{
		CString strValue;
		MessageBox(_T("数据添加成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		_ShowEquipmentInfo(m_strContractID);
		m_fTotalPrice = 1 * _CountEquipmentCost(m_strContractID);
		strValue.Format(_T("%02f"), m_fTotalPrice);
		SetDlgItemText(IDC_EDIT_CONTRACTDLG_TOTALPRICE, strValue);
	}
	else
	{
		MessageBox(_T("数据添加失败!"), _T("失败"), MB_OK | MB_ICONERROR);
	}
}

void CContractInfoDlg::OnBnClickedBtnContractdlgEquipmentModify()
{	
	POSITION pos = m_EquipmentList.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("请先选择需要修改的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	int iSelItem = m_EquipmentList.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		MessageBox(_T("请先选择需要修改的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}

	UpdateData(TRUE);

	if (m_strEquipment2.IsEmpty())
	{
		MessageBox(_T("设备名 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strModel.IsEmpty())
	{
		MessageBox(_T("型号 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_dwCount2 == 0)
	{
		MessageBox(_T("数量 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_fPrice == 0)
	{
		MessageBox(_T("价格 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}

	m_pEquipmentTable->SetRecordCell(iSelItem, L"Equipment", m_strEquipment2);
	m_pEquipmentTable->SetRecordCell(-1, L"Model", m_strModel);
	m_pEquipmentTable->SetRecordCell(-1, L"Number", m_dwCount2);
	m_pEquipmentTable->SetRecordCell(-1, L"Price", m_fPrice);
	m_pEquipmentTable->SetRecordCell(-1, L"Cost", m_fTotalPrice2);
	m_pEquipmentTable->SetRecordCell(-1, L"Comment", m_strComment2);
	
	if (TRUE == m_pEquipmentTable->UpdateTable())
	{
		CString strValue;
		MessageBox(_T("数据修改成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		_ShowEquipmentInfo(m_strContractID);
		m_fTotalPrice = 1 * _CountEquipmentCost(m_strContractID);
		strValue.Format(_T("%02f"), m_fTotalPrice);
		SetDlgItemText(IDC_EDIT_CONTRACTDLG_TOTALPRICE, strValue);
	}
	else
	{
		MessageBox(_T("数据修改成功!"), _T("失败"), MB_OK | MB_ICONERROR);
	}
}

void CContractInfoDlg::OnBnClickedBtnContractdlgEquipmentDelete()
{
	POSITION pos = m_EquipmentList.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("请先选择需要删除的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	int iSelItem = m_EquipmentList.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		MessageBox(_T("请先选择需要删除的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}

	if (IDYES == MessageBox(_T("您确定要删除选中的记录吗？"), _T("确认"), MB_YESNO | MB_ICONQUESTION))
	{
		if (m_pEquipmentTable->DeleteItem(iSelItem) && m_pEquipmentTable->UpdateTable())
		{
			LRESULT res;
			CString strValue;
			_ShowEquipmentInfo(m_strContractID);
			m_fTotalPrice = 1 * _CountEquipmentCost(m_strContractID);
			strValue.Format(_T("%02f"), m_fTotalPrice);
			SetDlgItemText(IDC_EDIT_CONTRACTDLG_TOTALPRICE, strValue);
			OnNMClickListContractdlgEquipment(NULL, &res);
			MessageBox(_T("数据删除成功！"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(_T("数据删除失败!"), _T("失败"), MB_OK | MB_ICONERROR);
		}
	}
}

void CContractInfoDlg::OnBnClickedBtnContractdlgNew()
{
	m_eTableOption = eTableOptions_New;

	UpdateData();		//不能省
	_GetInitializeData();	
	GetDlgItem(IDC_EDIT_CONTRACTDLG_PROJECT)->SetFocus();
}

void CContractInfoDlg::OnBnClickedOk()
{	
	CTime tmStartDate;
	CTime tmFinishDate;
	CString strContractType;
	CString strCustomName;
	CString strContactName;
	CString strStatus;

	UpdateData(TRUE);
	m_ContractTypeCombox.GetWindowText(strContractType);
	m_CustomNameCombox.GetWindowText(strCustomName);
	m_ContactCombox.GetWindowText(strContactName);
	m_ContractStatusCombox.GetWindowText(strStatus);
	m_ContractStartDateCtrl.GetTime(tmStartDate);
	m_ContractEndDateCtrl.GetTime(tmFinishDate);

	if (m_strProjectName.IsEmpty())
	{
		MessageBox(_T("项目名 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strEquipment.IsEmpty())
	{
		MessageBox(_T("设备名 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}

	if (eTableOptions_New == m_eTableOption)
	{
		m_pCurTable->AddNewItem();
		m_pCurTable->SetRecordCell(-1, L"ContractID", m_strContractID);
		m_pCurTable->SetRecordCell(-1, L"Type", strContractType);
	}
	else
	{
		m_pCurTable->SetRecordCell(m_iSelIndex, L"Type", strContractType);
	}
	m_pCurTable->SetRecordCell(-1, L"Custom", strCustomName);
	m_pCurTable->SetRecordCell(-1, L"Saler", m_strSaler);
	m_pCurTable->SetRecordCell(-1, L"Contact", strContactName);
	m_pCurTable->SetRecordCell(-1, L"Tel", m_strContactTel);
	m_pCurTable->SetRecordCell(-1, L"ProjectName", m_strProjectName);
	m_pCurTable->SetRecordCell(-1, L"Equipment", m_strEquipment);
	m_pCurTable->SetRecordCell(-1, L"ContractDate", tmStartDate);
	m_pCurTable->SetRecordCell(-1, L"Cost", m_fTotalPrice);
	m_pCurTable->SetRecordCell(-1, L"Status", strStatus);
	m_pCurTable->SetRecordCell(-1, L"FinishDate", tmFinishDate);
	m_pCurTable->SetRecordCell(-1, L"Comment", m_strComment);
	
	if (TRUE == m_pCurTable->UpdateTable())
	{
		if (eTableOptions_New == m_eTableOption) MessageBox(_T("数据添加成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		else  MessageBox(_T("数据修改成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		if (eTableOptions_New == m_eTableOption) MessageBox(_T("数据添加失败!"), _T("失败"), MB_OK | MB_ICONERROR);
		else  MessageBox(_T("数据修改失败!"), _T("失败"), MB_OK | MB_ICONERROR);
	}

	OnOK();
}

void CContractInfoDlg::OnBnClickedBtnContractdlgDelete()
{
	if (IDYES == MessageBox(_T("您确定要删除选中的记录吗？"), _T("确认"), MB_YESNO | MB_ICONQUESTION))
	{
		m_pCurTable->DeleteItem(m_iSelIndex);
		if (m_pCurTable->UpdateTable())
		{
			MessageBox(_T("数据删除成功！"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(_T("数据删除失败!"), _T("失败"), MB_OK | MB_ICONERROR);
		}
	}
	OnOK();
}


void CContractInfoDlg::_FillCustomNameComboBox()
{
	CStringW strSQL;
	strSQL.Format(L"SELECT FullName FROM %s", g_Tables[eTableID_Custom].strName);
	FillComboBoxFromTable(strSQL, _T("FullName"), _T(""), &m_CustomNameCombox);
	OnCbnSelchangeComboContractdlgCustom();
}

void CContractInfoDlg::_FillContactComboBox()
{
	if (m_strCurSelCustomShortName.IsEmpty())
	{
		return;
	}

	CStringW strSQL;
	strSQL.Format(L"SELECT ContactName FROM %s WHERE CompanyName = '%s'", 
		g_Tables[eTableID_Contact].strName, m_strCurSelCustomShortName);
	FillComboBoxFromTable(strSQL, _T("ContactName"), m_strCurSelCustomContact, &m_ContactCombox);
	OnCbnSelchangeComboContractdlgContact();
}

void CContractInfoDlg::_GetInitializeData()
{
	CString strValue;
	CTime tmStartDate;
	CTime tmFinishDate;

	if (eTableOptions_Modify == m_eTableOption)
	{
		// 合同号
		m_pCurTable->GetRecordCell(m_iSelIndex, L"ContractID", m_strContractID);
		// 客户合同号
		m_pCurTable->GetRecordCell(-1, L"CustomContractID", m_strCustomContractID);		
		// 合同类别
		m_pCurTable->GetRecordCell(-1, L"Type", strValue);
		for (int i = 0; i < m_ContractTypeCombox.GetCount(); i++)
		{
			CString strText;
			m_ContractTypeCombox.GetLBText(i, strText);
			if (_tcsicmp(strText, strValue) == 0)
			{
				m_ContractTypeCombox.SetCurSel(i);
				OnCbnSelchangeComboContractdlgCustom();
				break;
			}
		}
		// 销售员
		m_pCurTable->GetRecordCell(-1, L"Saler", m_strSaler);
		if (m_strSaler.IsEmpty()) m_strSaler = g_strUserName;
		// 客户名称
		m_pCurTable->GetRecordCell(-1, L"Custom", strValue);
		for (int i = 0; i < m_CustomNameCombox.GetCount(); i++)
		{
			CString strText;
			m_CustomNameCombox.GetLBText(i, strText);
			if (_tcsicmp(strText, strValue) == 0)
			{
				m_CustomNameCombox.SetCurSel(i);
				OnCbnSelchangeComboContractdlgContact();
				break;
			}
		}
		// 联系人
		m_pCurTable->GetRecordCell(-1, L"Contact", strValue);
		for (int i = 0; i < m_ContactCombox.GetCount(); i++)
		{
			CString strText;
			m_ContactCombox.GetLBText(i, strText);
			if (_tcsicmp(strText, strValue) == 0)
			{
				m_ContactCombox.SetCurSel(i);
				break;
			}
		}
		// 电话
		m_pCurTable->GetRecordCell(-1, L"Tel", m_strContactTel);
		// 项目名称
		m_pCurTable->GetRecordCell(-1, L"ProjectName", m_strProjectName);
		// 设备
		m_pCurTable->GetRecordCell(-1, L"Equipment", m_strEquipment);
		// 合同日期
		m_pCurTable->GetRecordCell(-1, L"ContractDate", tmStartDate);
		// 交货日期
		m_pCurTable->GetRecordCell(-1, L"FinishDate", tmFinishDate);
		// 总金额
		m_pCurTable->GetRecordCell(-1, L"Cost", m_fTotalPrice);
		// 状态
		m_pCurTable->GetRecordCell(-1, L"Status", strValue);
		for (int i = 0; i < m_ContractStatusCombox.GetCount(); i++)
		{
			CString strText;
			m_ContractStatusCombox.GetLBText(i, strText);
			if (_tcsicmp(strText, strValue) == 0)
			{
				m_ContractStatusCombox.SetCurSel(i);
				break;
			}
		}
		// 备注
		m_pCurTable->GetRecordCell(-1, L"Comment", m_strComment);
	}
	else
	{
		int iNewRecordIndex = GetNewRecordIndex(g_Tables[eTableID_ContractInfo].strName, _T("ContractID"), _T("HT"));
		// 合同号
		m_strContractID.Format(_T("%s%04d"), _T("HT"), iNewRecordIndex);
		// 客户合同号
		m_strCustomContractID = _T("");
		// 合同类别
		m_ContractTypeCombox.SetCurSel(0);
		// 销售员
		m_strSaler = g_strUserName;
		// 客户名称
		m_CustomNameCombox.SetCurSel(0);
		// 联系人
		m_ContactCombox.SetCurSel(0);
		// 电话
		m_strContactTel = _T("");
		// 项目名称
		m_strProjectName = _T("");
		// 设备
		m_strEquipment = _T("");
		// 合同日期
		tmStartDate = CTime::GetCurrentTime();
		// 交货日期
		tmFinishDate = CTime::GetCurrentTime();
		// 总金额
		m_fTotalPrice = 0.0f;
		// 状态
		m_ContractStatusCombox.SetCurSel(0);
		// 备注
		m_strComment = _T("");
	}
	UpdateData(FALSE);
	m_ContractStartDateCtrl.SetTime(&tmStartDate);
	m_ContractEndDateCtrl.SetTime(&tmFinishDate);

	// 打开设备信息表
	_ShowEquipmentInfo(m_strContractID);
}

void CContractInfoDlg::_ShowEquipmentInfo(CString strContractID)
{	
	BOOL bResult = FALSE;
	DWORD dwItemCnt = 0;
	CString strSQL;

	m_EquipmentList.DeleteAllItems();

	strSQL.Format(_T("SELECT * FROM %s WHERE OfferID = '%s'"), 
		g_Tables[eTableID_ContractInfo].strName, strContractID);

	if (!m_pEquipmentTable) m_pEquipmentTable = new CADOTable();
	else m_pEquipmentTable->CloseTable();

	bResult = m_pEquipmentTable->OpenTable(g_pDatabase->GetConnectionPtr(), strSQL);
	if (FALSE == bResult)
	{
		return;
	}
	
	dwItemCnt = m_pEquipmentTable->GetRecordCnt();
	FillListFromTable(&m_EquipmentList, m_pEquipmentTable, &g_Tables[eTableID_ContractInfo], 0, dwItemCnt);
}

float CContractInfoDlg::_CountEquipmentCost(CString strContractID)
{
	CStringW strSQL;
	CADOTable table;
	BOOL bResult = FALSE;
	float fTotalCost = 0.0f;

	strSQL.Format(L"SELECT Cost FROM %s WHERE ContractID LIKE '%s'", g_Tables[eTableID_ContractInfo].strName, strContractID);

	bResult = table.OpenTable(g_pDatabase->GetConnectionPtr(), strSQL);
	if (FALSE == bResult)
	{
		return 0;
	}
	
	for (DWORD dwIndex = 0; dwIndex < table.GetRecordCnt(); dwIndex++)
	{
		float fCost = 0;
		table.GetRecordCell(dwIndex, L"Cost", fCost);
		fTotalCost += fCost;
	}

	return fTotalCost;
}