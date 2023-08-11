// OfferInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BMHoistManager.h"
#include "OfferInfoDlg.h"

// COfferInfoDlg dialog

IMPLEMENT_DYNAMIC(COfferInfoDlg, CDialog)

COfferInfoDlg::COfferInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COfferInfoDlg::IDD, pParent)
{
	m_pEquipmentTable = NULL;
	m_strOfferID = _T("");
	m_strOfferInputer = _T("");
	m_strContactTel = _T("");
	m_strProjectName = _T("");
	m_strEquipment = _T("");
	m_dwCount = 1;
	m_fTotalPrice = 0.0f;
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

COfferInfoDlg::~COfferInfoDlg()
{
}

void COfferInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OFFERDLG_TYPE, m_OfferTypeCombox);
	DDX_Control(pDX, IDC_COMBO_OFFERDLG_CUSTOM, m_CustomNameCombox);
	DDX_Control(pDX, IDC_COMBO_OFFERDLG_CONTACT, m_ContactCombox);
	DDX_Control(pDX, IDC_COMBO_OFFERDLG_STATUS, m_OfferStatusCombox);
	DDX_Control(pDX, IDC_LIST_OFFERDLG_EQUIPMENT, m_EquipmentList);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_ID, m_strOfferID);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_INPUTER, m_strOfferInputer);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_TEL, m_strContactTel);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_PROJECT, m_strProjectName);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_EQUIPMENT, m_strEquipment);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_COUNT, m_dwCount);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_TOTALPRICE, m_fTotalPrice);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_COMMENT, m_strComment);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_EQUIPMENT2, m_strEquipment2);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_MODEL, m_strModel);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_COUNT2, m_dwCount2);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_PRICE, m_fPrice);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_TOTALPRICE2, m_fTotalPrice2);
	DDX_Text(pDX, IDC_EDIT_OFFERDLG_COMMENT2, m_strComment2);
	DDX_Control(pDX, IDC_DATETIMEPICKER_OFFERDLG_DATE, m_OfferDateCtrl);
}


BEGIN_MESSAGE_MAP(COfferInfoDlg, CDialog)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_OFFERDLG_CUSTOM, &COfferInfoDlg::OnCbnSelchangeComboOfferdlgCustom)
	ON_CBN_SELCHANGE(IDC_COMBO_OFFERDLG_CONTACT, &COfferInfoDlg::OnCbnSelchangeComboOfferdlgContact)
	ON_BN_CLICKED(IDC_BTN_CONTRACTDLG_EQUIMENT_NEW, &COfferInfoDlg::OnBnClickedBtnContractdlgEquimentNew)
	ON_BN_CLICKED(IDC_BTN_OFFERDLG_EQUIPMENT_MODIFY, &COfferInfoDlg::OnBnClickedBtnOfferdlgEquipmentModify)
	ON_BN_CLICKED(IDC_BTN_OFFERDLG_EQUIPMENT_DELETE, &COfferInfoDlg::OnBnClickedBtnOfferdlgEquipmentDelete)
	ON_BN_CLICKED(IDOK, &COfferInfoDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_OFFERDLG_EQUIPMENT, &COfferInfoDlg::OnNMClickListOfferdlgEquipment)
	ON_BN_CLICKED(IDC_BTN_OFFERDLG_NEW, &COfferInfoDlg::OnBnClickedBtnOfferdlgNew)
	ON_BN_CLICKED(IDC_BTN_OFFERDLG_DELETE, &COfferInfoDlg::OnBnClickedBtnOfferdlgDelete)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_COUNT2, &COfferInfoDlg::OnEnChangeEditOfferdlgCount2)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_PRICE, &COfferInfoDlg::OnEnChangeEditOfferdlgPrice)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_COUNT, &COfferInfoDlg::OnEnChangeEditOfferdlgCount)
END_MESSAGE_MAP()


// COfferInfoDlg message handlers

BOOL COfferInfoDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL COfferInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	FillEquipmentsComboBox(&m_OfferTypeCombox);
	_FillCustomNameComboBox();
	//
	m_OfferStatusCombox.ResetContent();
	m_OfferStatusCombox.AddString(_T("进行中"));
	m_OfferStatusCombox.AddString(_T("订单"));
	m_OfferStatusCombox.AddString(_T("延期"));
	m_OfferStatusCombox.AddString(_T("取消"));
	m_OfferStatusCombox.AddString(_T("丢单"));
	m_OfferStatusCombox.SetCurSel(0);
	//
	CTime tm = CTime::GetCurrentTime();
	m_OfferDateCtrl.SetTime(&tm);
	//
	InitListHeader(&m_EquipmentList, &g_Tables[eTableID_OfferInfo]);
	m_EquipmentList.SetExtendedStyle(m_EquipmentList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	//
	_GetInitializeData();
	//	
	GetDlgItem(IDC_EDIT_OFFERDLG_PROJECT)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COfferInfoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void COfferInfoDlg::OnCbnSelchangeComboOfferdlgCustom()
{
	CString strCurSelCustom;
	m_CustomNameCombox.GetWindowText(strCurSelCustom);

	m_strCurSelCustomShortName = _T("");
	m_strCurSelCustomContact = _T("");


	CADOTable tbCustoms;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT ShortName, Contact FROM %s WHERE FullName = '%s'", 
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

void COfferInfoDlg::OnCbnSelchangeComboOfferdlgContact()
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
	SetDlgItemText(IDC_EDIT_OFFERDLG_TEL, m_strContactTel);
}

void COfferInfoDlg::OnEnChangeEditOfferdlgCount()
{
	CString strValue;
	float fOfferPrice = m_fTotalPrice / m_dwCount;
	DWORD dwOldCount = m_dwCount;

	UpdateData(TRUE);
	if (m_dwCount <= 0)
	{
		strValue.Format(_T("%d"), dwOldCount);
		m_dwCount = dwOldCount;
		MessageBox(_T("数量 不能为0!"), _T("错误"), MB_OK | MB_ICONERROR);
		SetDlgItemText(IDC_EDIT_OFFERDLG_COUNT, strValue);
		return;
	}

	m_fTotalPrice = fOfferPrice * m_dwCount;
	strValue.Format(_T("%02f"), m_fTotalPrice);
	SetDlgItemText(IDC_EDIT_OFFERDLG_TOTALPRICE, strValue);
}

void COfferInfoDlg::OnNMClickListOfferdlgEquipment(NMHDR *pNMHDR, LRESULT *pResult)
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
	SetDlgItemText(IDC_EDIT_OFFERDLG_EQUIPMENT2, m_strEquipment2);
	SetDlgItemText(IDC_EDIT_OFFERDLG_MODEL, m_strModel);
	strValue.Format(_T("%d"), dwCount2);
	SetDlgItemText(IDC_EDIT_OFFERDLG_COUNT2, strValue);
	strValue.Format(_T("%.02f"), fPrice);
	SetDlgItemText(IDC_EDIT_OFFERDLG_PRICE, strValue);
	strValue.Format(_T("%.02f"), fTotalPrice2);
	SetDlgItemText(IDC_EDIT_OFFERDLG_TOTALPRICE2, strValue);
	SetDlgItemText(IDC_EDIT_OFFERDLG_COMMENT2, m_strComment2);
	//
	m_dwCount2 = dwCount2;
	m_fPrice = fPrice;
	m_fTotalPrice2 = fTotalPrice2;

	*pResult = 0;
}

void COfferInfoDlg::OnEnChangeEditOfferdlgCount2()
{
	UpdateData(TRUE);

	m_fTotalPrice2 = m_fPrice * m_dwCount2;

	CString strValue;
	strValue.Format(_T("%02f"), m_fTotalPrice2);
	SetDlgItemText(IDC_EDIT_OFFERDLG_TOTALPRICE2, strValue);
}

void COfferInfoDlg::OnEnChangeEditOfferdlgPrice()
{
	UpdateData(TRUE);

	m_fTotalPrice2 = m_fPrice * m_dwCount2;

	CString strValue;
	strValue.Format(_T("%02f"), m_fTotalPrice2);
	SetDlgItemText(IDC_EDIT_OFFERDLG_TOTALPRICE2, strValue);
}


void COfferInfoDlg::OnBnClickedBtnContractdlgEquimentNew()
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
	m_pEquipmentTable->SetRecordCell(-1, L"OfferID", m_strOfferID);
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
		_ShowEquipmentInfo(m_strOfferID);
		m_fTotalPrice = m_dwCount * _CountEquipmentCost(m_strOfferID);
		strValue.Format(_T("%02f"), m_fTotalPrice);
		SetDlgItemText(IDC_EDIT_OFFERDLG_TOTALPRICE, strValue);
	}
	else
	{
		MessageBox(_T("数据添加失败!"), _T("失败"), MB_OK | MB_ICONERROR);
	}
}

void COfferInfoDlg::OnBnClickedBtnOfferdlgEquipmentModify()
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
		_ShowEquipmentInfo(m_strOfferID);
		m_fTotalPrice = m_dwCount * _CountEquipmentCost(m_strOfferID);
		strValue.Format(_T("%02f"), m_fTotalPrice);
		SetDlgItemText(IDC_EDIT_OFFERDLG_TOTALPRICE, strValue);
	}
	else
	{
		MessageBox(_T("数据修改成功!"), _T("失败"), MB_OK | MB_ICONERROR);
	}
}

void COfferInfoDlg::OnBnClickedBtnOfferdlgEquipmentDelete()
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
			_ShowEquipmentInfo(m_strOfferID);
			m_fTotalPrice = m_dwCount * _CountEquipmentCost(m_strOfferID);
			strValue.Format(_T("%02f"), m_fTotalPrice);
			SetDlgItemText(IDC_EDIT_OFFERDLG_TOTALPRICE, strValue);
			OnNMClickListOfferdlgEquipment(NULL, &res);
			MessageBox(_T("数据删除成功！"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(_T("数据删除失败!"), _T("失败"), MB_OK | MB_ICONERROR);
		}
	}
}

void COfferInfoDlg::OnBnClickedBtnOfferdlgNew()
{
	m_eTableOption = eTableOptions_New;

	UpdateData();		//不能省
	_GetInitializeData();	
	GetDlgItem(IDC_EDIT_OFFERDLG_PROJECT)->SetFocus();
}

void COfferInfoDlg::OnBnClickedOk()
{	
	CTime tmOfferDate;
	CString strOfferType;
	CString strCustomName;
	CString strContactName;
	CString strStatus;

	UpdateData(TRUE);
	m_OfferTypeCombox.GetWindowText(strOfferType);
	m_CustomNameCombox.GetWindowText(strCustomName);
	m_ContactCombox.GetWindowText(strContactName);
	m_OfferStatusCombox.GetWindowText(strStatus);
	m_OfferDateCtrl.GetTime(tmOfferDate);

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
	if (m_dwCount == 0)
	{
		MessageBox(_T("数量 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}

	if (eTableOptions_New == m_eTableOption)
	{
		m_pCurTable->AddNewItem();
		m_pCurTable->SetRecordCell(-1, L"OfferID", m_strOfferID);
		m_pCurTable->SetRecordCell(-1, L"OfferType", strOfferType);
	}
	else
	{
		m_pCurTable->SetRecordCell(m_iSelIndex, L"OfferType", strOfferType);
	}
	m_pCurTable->SetRecordCell(-1, L"Custom", strCustomName);
	m_pCurTable->SetRecordCell(-1, L"OfferPeople", m_strOfferInputer);
	m_pCurTable->SetRecordCell(-1, L"Contact", strContactName);
	m_pCurTable->SetRecordCell(-1, L"Tel", m_strContactTel);
	m_pCurTable->SetRecordCell(-1, L"ProjectName", m_strProjectName);
	m_pCurTable->SetRecordCell(-1, L"Equipment", m_strEquipment);
	m_pCurTable->SetRecordCell(-1, L"Number", m_dwCount);
	m_pCurTable->SetRecordCell(-1, L"OfferDate", tmOfferDate);
	m_pCurTable->SetRecordCell(-1, L"Cost", m_fTotalPrice);
	m_pCurTable->SetRecordCell(-1, L"Status", strStatus);
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

void COfferInfoDlg::OnBnClickedBtnOfferdlgDelete()
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

void COfferInfoDlg::_FillCustomNameComboBox()
{
	CStringW strSQL;
	strSQL.Format(L"SELECT FullName FROM %s", g_Tables[eTableID_Custom].strName);
	FillComboBoxFromTable(strSQL, _T("FullName"), _T(""), &m_CustomNameCombox);
	OnCbnSelchangeComboOfferdlgCustom();
}

void COfferInfoDlg::_FillContactComboBox()
{
	if (m_strCurSelCustomShortName.IsEmpty())
	{
		return;
	}

	CStringW strSQL;
	strSQL.Format(L"SELECT ContactName FROM %s WHERE CompanyName = '%s'", 
		g_Tables[eTableID_Contact].strName, m_strCurSelCustomShortName);
	FillComboBoxFromTable(strSQL, _T("ContactName"), m_strCurSelCustomContact, &m_ContactCombox);
	OnCbnSelchangeComboOfferdlgContact();
}

void COfferInfoDlg::_GetInitializeData()
{
	CString strValue;
	CTime tmOfferDate;

	if (eTableOptions_Modify == m_eTableOption)
	{
		// 报价单号
		m_pCurTable->GetRecordCell(m_iSelIndex, L"OfferID", m_strOfferID);
		// 报价类别
		m_pCurTable->GetRecordCell(-1, L"OfferType", strValue);
		for (int i = 0; i < m_OfferTypeCombox.GetCount(); i++)
		{
			CString strText;
			m_OfferTypeCombox.GetLBText(i, strText);
			if (_tcsicmp(strText, strValue) == 0)
			{
				m_OfferTypeCombox.SetCurSel(i);
				break;
			}
		}
		// 报价人
		m_pCurTable->GetRecordCell(-1, L"OfferPeople", m_strOfferInputer);
		if (m_strOfferInputer.IsEmpty()) m_strOfferInputer = g_strUserName;
		// 客户名称
		m_pCurTable->GetRecordCell(-1, L"Custom", strValue);
		for (int i = 0; i < m_CustomNameCombox.GetCount(); i++)
		{
			CString strText;
			m_CustomNameCombox.GetLBText(i, strText);
			if (_tcsicmp(strText, strValue) == 0)
			{
				m_CustomNameCombox.SetCurSel(i);
				OnCbnSelchangeComboOfferdlgCustom();
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
				OnCbnSelchangeComboOfferdlgContact();
				break;
			}
		}
		// 电话
		m_pCurTable->GetRecordCell(-1, L"Tel", m_strContactTel);
		// 项目名称
		m_pCurTable->GetRecordCell(-1, L"ProjectName", m_strProjectName);
		// 设备
		m_pCurTable->GetRecordCell(-1, L"Equipment", m_strEquipment);
		// 数量
		m_pCurTable->GetRecordCell(-1, L"Number", m_dwCount);
		// 报价日期
		m_pCurTable->GetRecordCell(-1, L"OfferDate", tmOfferDate);
		// 总金额
		m_pCurTable->GetRecordCell(-1, L"Cost", m_fTotalPrice);
		// 状态
		m_pCurTable->GetRecordCell(-1, L"Status", strValue);
		for (int i = 0; i < m_OfferStatusCombox.GetCount(); i++)
		{
			CString strText;
			m_OfferStatusCombox.GetLBText(i, strText);
			if (_tcsicmp(strText, strValue) == 0)
			{
				m_OfferStatusCombox.SetCurSel(i);
				break;
			}
		}
		// 备注
		m_pCurTable->GetRecordCell(-1, L"Comment", m_strComment);
	}
	else
	{
		int iNewRecordIndex = GetNewRecordIndex(g_Tables[eTableID_Offer].strName, _T("OfferID"), _T("BJ"));
		// 报价单号
		m_strOfferID.Format(_T("%s%04d"), _T("BJ"), iNewRecordIndex);
		// 报价类别
		m_OfferTypeCombox.SetCurSel(0);
		// 报价人
		m_strOfferInputer = g_strUserName;
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
		// 数量
		m_dwCount = 1;
		// 报价日期
		tmOfferDate = CTime::GetCurrentTime();
		// 总金额
		m_fTotalPrice = 0.0f;
		// 状态
		m_OfferStatusCombox.SetCurSel(0);
		// 备注
		m_strComment = _T("");
	}
	UpdateData(FALSE);
	m_OfferDateCtrl.SetTime(&tmOfferDate);

	// 打开设备信息表
	_ShowEquipmentInfo(m_strOfferID);
}

void COfferInfoDlg::_ShowEquipmentInfo(CString strOfferID)
{	
	BOOL bResult = FALSE;
	DWORD dwItemCnt = 0;
	CString strSQL;

	m_EquipmentList.DeleteAllItems();

	strSQL.Format(_T("SELECT * FROM %s WHERE OfferID = '%s'"), 
		g_Tables[eTableID_OfferInfo].strName, strOfferID);

	if (!m_pEquipmentTable) m_pEquipmentTable = new CADOTable();
	else m_pEquipmentTable->CloseTable();

	bResult = m_pEquipmentTable->OpenTable(g_pDatabase->GetConnectionPtr(), strSQL);
	if (FALSE == bResult)
	{
		return;
	}
	
	dwItemCnt = m_pEquipmentTable->GetRecordCnt();
	FillListFromTable(&m_EquipmentList, m_pEquipmentTable, &g_Tables[eTableID_OfferInfo], 0, dwItemCnt);
}

float COfferInfoDlg::_CountEquipmentCost(CString strOfferID)
{
	CStringW strSQL;
	CADOTable table;
	BOOL bResult = FALSE;
	float fTotalCost = 0.0f;

	strSQL.Format(L"SELECT Cost FROM %s WHERE OfferID LIKE '%s'", g_Tables[eTableID_OfferInfo].strName, strOfferID);

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