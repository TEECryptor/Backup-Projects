// CustomInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BMHoistManager.h"
#include "CustomInfoDlg.h"
#include "CommonDef.h"

// CCustomInfoDlg dialog

IMPLEMENT_DYNAMIC(CCustomInfoDlg, CDialog)

CCustomInfoDlg::CCustomInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomInfoDlg::IDD, pParent)
	, m_strCustomID(_T(""))
	, m_strCustomInputer(_T(""))
	, m_strCustomName(_T(""))
	, m_strCustomShortName(_T(""))
	, m_strCustomFindCode(_T(""))
	, m_strCustomTel(_T(""))
	, m_strCustomFox(_T(""))
	, m_strCustomHttp(_T(""))
	, m_strCustomContact(_T(""))
	, m_strCustomContactTel(_T(""))
	, m_strCustomEmail(_T(""))
	, m_strCustomAddress(_T(""))
	, m_strCustomPostCode(_T(""))
	, m_strCustomBank(_T(""))
	, m_strCustomBankID(_T(""))
	, m_strCustomAccount(_T(""))
	, m_strCustomTaxID(_T(""))
	, m_strCustomComment(_T(""))
	, m_strContactName(_T(""))
	, m_strContactFindCode(_T(""))
	, m_strContactPosition(_T(""))
	, m_strContactTel(_T(""))
	, m_strContactFox(_T(""))
	, m_strContactCellTel(_T(""))
	, m_strContactEmail(_T(""))
	, m_strContactAddress(_T(""))
	, m_strContactPostCode(_T(""))
	, m_strContactComment(_T(""))
	, m_pCurTable(NULL)
	, m_pContactTable(NULL)
	, m_iSelIndex(-1)
	, m_bIsModifyContact(FALSE)
{

}

CCustomInfoDlg::~CCustomInfoDlg()
{
	if (m_pContactTable)
	{
		m_pContactTable->CloseTable();
		delete m_pContactTable;
		m_pContactTable = NULL;
	}
}

void CCustomInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUSTOMDLG_TYPE, m_CustomTypeCombox);
	DDX_Control(pDX, IDC_COMBO_CUSTOMDLG_SHARED, m_SharedOrNotCombox);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_ID, m_strCustomID);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_INPUTER, m_strCustomInputer);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_FULLNAME, m_strCustomName);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_SHORTNAME, m_strCustomShortName);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_FINDCODE, m_strCustomFindCode);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_TEL, m_strCustomTel);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_FOX, m_strCustomFox);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_HTTP, m_strCustomHttp);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACT, m_strCustomContact);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACTTEL, m_strCustomContactTel);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACTEMAIL, m_strCustomEmail);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_ADRESS, m_strCustomAddress);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_POSTCODE, m_strCustomPostCode);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_BANK, m_strCustomBank);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_BANKID, m_strCustomBankID);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_ACCOUNT, m_strCustomAccount);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_TAXID, m_strCustomTaxID);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_COMMENT, m_strCustomComment);
	DDX_Control(pDX, IDC_LIST_CUSTOMDLG_CONTACTS, m_ContactListCtrl);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACT_NAME, m_strContactName);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACT_FINDCODE, m_strContactFindCode);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACT_POSITION, m_strContactPosition);
	DDX_Control(pDX, IDC_COMBO_CUSTOMDLG_CONTACT_SEX, m_ContactSexCombox);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACT_TEL, m_strContactTel);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACT_FOX, m_strContactFox);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACT_CELLTEL, m_strContactCellTel);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACT_EMAIL, m_strContactEmail);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACT_ADDRESS, m_strContactAddress);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACT_POSTCODE, m_strContactPostCode);
	DDX_Text(pDX, IDC_EDIT_CUSTOMDLG_CONTACT_COMMENT, m_strContactComment);
}


BEGIN_MESSAGE_MAP(CCustomInfoDlg, CDialog)
	ON_COMMAND(ID_CUSTOMDLG_MENU_SETDEFAULT, OnMenuSetAsDefaultContact)
	ON_BN_CLICKED(IDC_BTN_CUSTOMDLG_CONTACT_NEW, &CCustomInfoDlg::OnBnClickedBtnCustomdlgContactNew)
	ON_BN_CLICKED(IDC_BTN_CUSTOMDLG_CONTACT_MODIFY, &CCustomInfoDlg::OnBnClickedBtnCustomdlgContactModify)
	ON_BN_CLICKED(IDC_BTN_CUSTOMDLG_CONTACT_DELETE, &CCustomInfoDlg::OnBnClickedBtnCustomdlgContactDelete)
	ON_BN_CLICKED(IDOK, &CCustomInfoDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOMDLG_TYPE, &CCustomInfoDlg::OnCbnSelchangeComboCustomdlgType)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CUSTOMDLG_CONTACTS, &CCustomInfoDlg::OnNMClickListCustomdlgContacts)
	ON_BN_CLICKED(IDC_BTN_CUSTOMDLG_NEW, &CCustomInfoDlg::OnBnClickedBtnCustomdlgNew)
	ON_BN_CLICKED(IDC_BTN_CUSTOMDLG_DELETE, &CCustomInfoDlg::OnBnClickedBtnCustomdlgDelete)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CUSTOMDLG_CONTACTS, &CCustomInfoDlg::OnNMRClickListCustomdlgContacts)
END_MESSAGE_MAP()


// CCustomInfoDlg message handlers

BOOL CCustomInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	int iItemIndex = 0;
	m_CustomTypeCombox.ResetContent();
	for (int i = 0; i < 5; i++)
	{
		iItemIndex = m_CustomTypeCombox.AddString(g_strCompanyType[i].strTypeName);
		m_CustomTypeCombox.SetItemData(iItemIndex, i);
	}
	m_CustomTypeCombox.SetCurSel(0);
	//
	m_SharedOrNotCombox.ResetContent();
	m_SharedOrNotCombox.AddString(_T("是"));
	m_SharedOrNotCombox.AddString(_T("否"));
	m_SharedOrNotCombox.SetCurSel(0);
	//
	m_ContactSexCombox.ResetContent();
	m_ContactSexCombox.AddString(_T("男"));
	m_ContactSexCombox.AddString(_T("女"));	
	m_ContactSexCombox.SetCurSel(0);
	//	
	InitListHeader(&m_ContactListCtrl, &g_Tables[eTableID_Contact]);
	m_ContactListCtrl.SetExtendedStyle(m_ContactListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	//
	GetDlgItem(IDC_BTN_CUSTOMDLG_CONTACT_NEW)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CUSTOMDLG_CONTACT_MODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CUSTOMDLG_CONTACT_DELETE)->EnableWindow(FALSE);
	//
	if (!m_bIsModifyContact) 
	{
		_GetInitializeData();
		GetDlgItem(IDC_EDIT_CUSTOMDLG_FULLNAME)->SetFocus();
	}
	else 
	{
		_GetInitializeDataByContact();
		GetDlgItem(IDC_COMBO_CUSTOMDLG_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CUSTOMDLG_SHARED)->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_FULLNAME))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_SHORTNAME))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_FINDCODE))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_TEL))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_FOX))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_HTTP))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_ADRESS))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_POSTCODE))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_BANK))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_BANKID))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_ACCOUNT))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_TAXID))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CUSTOMDLG_COMMENT))->SetReadOnly(TRUE);
		GetDlgItem(IDC_BTN_CUSTOMDLG_NEW)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CUSTOMDLG_DELETE)->EnableWindow(FALSE);
		SetDlgItemText(IDCANCEL, _T("关  闭"));
		
		GetDlgItem(IDC_EDIT_CUSTOMDLG_CONTACT_NAME)->SetFocus();
	}

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCustomInfoDlg::PreTranslateMessage(MSG* pMsg)
{   
	if (pMsg->message==WM_KEYDOWN)
    {
       if (pMsg->wParam==VK_RETURN)
           return TRUE;
    }

	return CDialog::PreTranslateMessage(pMsg);
}

void CCustomInfoDlg::OnCbnSelchangeComboCustomdlgType()
{
	int iItemIndex = 0;
	int iTypeIndex = 0;
	int iNewRecordIndex = 0;
	CString strIDValue;

	iItemIndex = m_CustomTypeCombox.GetCurSel();
	iTypeIndex = m_CustomTypeCombox.GetItemData(iItemIndex);
	iNewRecordIndex = GetNewRecordIndex(g_Tables[eTableID_Custom].strName, _T("CustomID"), g_strCompanyType[iTypeIndex].strShortName);
	strIDValue.Format(_T("%s%04d"), g_strCompanyType[iTypeIndex].strShortName, iNewRecordIndex);
	SetDlgItemText(IDC_EDIT_CUSTOMDLG_ID, strIDValue);
}

void CCustomInfoDlg::OnNMClickListCustomdlgContacts(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// Get current selected item
	POSITION pos = m_ContactListCtrl.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		m_strContactName = _T("");
		m_strContactFindCode = _T("");
		m_strContactPosition = _T("");
		m_strContactTel = _T("");
		m_strContactFox = _T("");
		m_strContactCellTel = _T("");
		m_strContactEmail = _T("");
		m_strContactAddress = _T("");
		m_strContactPostCode = _T("");
		m_strContactComment = _T("");
		m_ContactSexCombox.SetCurSel(0);
		GetDlgItem(IDC_BTN_CUSTOMDLG_CONTACT_NEW)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_CUSTOMDLG_CONTACT_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CUSTOMDLG_CONTACT_DELETE)->EnableWindow(FALSE);
	}
	else
	{
		CString strSex;
		int iSelItem = m_ContactListCtrl.GetNextSelectedItem(pos);
		m_strContactName = m_ContactListCtrl.GetItemText(iSelItem, 2);
		m_strContactFindCode = m_ContactListCtrl.GetItemText(iSelItem, 3);
		m_strContactPosition = m_ContactListCtrl.GetItemText(iSelItem, 5);
		strSex = m_ContactListCtrl.GetItemText(iSelItem, 6);
		m_strContactTel = m_ContactListCtrl.GetItemText(iSelItem, 7);
		m_strContactFox = m_ContactListCtrl.GetItemText(iSelItem, 8);
		m_strContactCellTel = m_ContactListCtrl.GetItemText(iSelItem, 9);
		m_strContactEmail = m_ContactListCtrl.GetItemText(iSelItem, 10);
		m_strContactAddress = m_ContactListCtrl.GetItemText(iSelItem, 11);
		m_strContactPostCode = m_ContactListCtrl.GetItemText(iSelItem, 12);
		m_strContactComment = m_ContactListCtrl.GetItemText(iSelItem, 13);
		if (strSex == _T("男")) m_ContactSexCombox.SetCurSel(0);
		else m_ContactSexCombox.SetCurSel(1);
		GetDlgItem(IDC_BTN_CUSTOMDLG_CONTACT_NEW)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CUSTOMDLG_CONTACT_MODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_CUSTOMDLG_CONTACT_DELETE)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);

	*pResult = 0;
}

void CCustomInfoDlg::OnNMRClickListCustomdlgContacts(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	//	如果是编辑联系人信息，则不支持修改缺省联系人操作
	if (m_bIsModifyContact)
	{
		*pResult = 0;
		return;
	}

	//	先选中当前联系人
	OnNMClickListCustomdlgContacts(pNMHDR, pResult);

	//	弹出菜单
	CPoint point;
	CMenu ppMenu;  
	GetCursorPos(&point);
	ppMenu.LoadMenu(IDR_MENU_CUSTOMDLG);  
	CMenu *pSubMenu = ppMenu.GetSubMenu(0);  
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this); 

	*pResult = 0;
}

void CCustomInfoDlg::OnMenuSetAsDefaultContact()
{
	m_strCustomContact = m_strContactName;
	m_strCustomContactTel = m_strContactTel;
	m_strCustomEmail = m_strContactEmail;
	UpdateData(FALSE);
}

void CCustomInfoDlg::OnBnClickedBtnCustomdlgContactNew()
{
	CString strCompanyType;
	CString strContactSex;

	UpdateData(TRUE);
	m_CustomTypeCombox.GetWindowText(strCompanyType);
	m_ContactSexCombox.GetWindowText(strContactSex);

	if (m_strContactName.IsEmpty())
	{
		MessageBox(_T("联系人姓名 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strContactFindCode.IsEmpty())
	{
		MessageBox(_T("联系人查询码 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strCustomShortName.IsEmpty())
	{
		MessageBox(_T("单位简称 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}

	m_pContactTable->AddNewItem();
	m_pContactTable->SetRecordCell(-1, L"Type", strCompanyType);
	m_pContactTable->SetRecordCell(-1, L"ContactName", m_strContactName);
	m_pContactTable->SetRecordCell(-1, L"Code", m_strContactFindCode);
	m_pContactTable->SetRecordCell(-1, L"CompanyName", m_strCustomShortName);
	m_pContactTable->SetRecordCell(-1, L"Position", m_strContactPosition);
	m_pContactTable->SetRecordCell(-1, L"Sex", strContactSex);
	m_pContactTable->SetRecordCell(-1, L"Tel", m_strContactTel);
	m_pContactTable->SetRecordCell(-1, L"Fox", m_strContactFox);
	m_pContactTable->SetRecordCell(-1, L"CellTel", m_strContactCellTel);
	m_pContactTable->SetRecordCell(-1, L"Email", m_strContactEmail);
	m_pContactTable->SetRecordCell(-1, L"Address", m_strContactAddress);
	m_pContactTable->SetRecordCell(-1, L"PostCode", m_strContactPostCode);
	m_pContactTable->SetRecordCell(-1, L"Comment", m_strContactComment);
	
	if (TRUE == m_pContactTable->UpdateTable())
	{
		MessageBox(_T("数据添加成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		_ShowContactInfo(strCompanyType, m_strCustomShortName);

		// 如果是第一条联系人信息，自动设置为缺省联系人
		if (m_ContactListCtrl.GetItemCount() == 1)
		{
			m_strCustomContact = m_strContactName;
			m_strCustomContactTel = m_strContactTel;
			m_strCustomEmail = m_strContactEmail;
			UpdateData(FALSE);
		}
	}
	else
	{
		MessageBox(_T("数据添加失败!"), _T("失败"), MB_OK | MB_ICONERROR);
	}
}

void CCustomInfoDlg::OnBnClickedBtnCustomdlgContactModify()
{
	CString strCompanyType;
	CString strContactSex;

	POSITION pos = m_ContactListCtrl.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("请先选择需要修改的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	int iSelItem = m_ContactListCtrl.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		MessageBox(_T("请先选择需要修改的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}

	UpdateData(TRUE);
	m_CustomTypeCombox.GetWindowText(strCompanyType);
	m_ContactSexCombox.GetWindowText(strContactSex);

	if (m_strContactName.IsEmpty())
	{
		MessageBox(_T("联系人姓名 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strContactFindCode.IsEmpty())
	{
		MessageBox(_T("联系人查询码 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strCustomShortName.IsEmpty())
	{
		MessageBox(_T("单位简称 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}

	m_pContactTable->SetRecordCell(iSelItem, L"ContactName", m_strContactName);
	m_pContactTable->SetRecordCell(-1, L"Code", m_strContactFindCode);
	m_pContactTable->SetRecordCell(-1, L"CompanyName", m_strCustomShortName);
	m_pContactTable->SetRecordCell(-1, L"Position", m_strContactPosition);
	m_pContactTable->SetRecordCell(-1, L"Sex", strContactSex);
	m_pContactTable->SetRecordCell(-1, L"Tel", m_strContactTel);
	m_pContactTable->SetRecordCell(-1, L"Fox", m_strContactFox);
	m_pContactTable->SetRecordCell(-1, L"CellTel", m_strContactCellTel);
	m_pContactTable->SetRecordCell(-1, L"Email", m_strContactEmail);
	m_pContactTable->SetRecordCell(-1, L"Address", m_strContactAddress);
	m_pContactTable->SetRecordCell(-1, L"PostCode", m_strContactPostCode);
	m_pContactTable->SetRecordCell(-1, L"Comment", m_strContactComment);
	
	if (TRUE == m_pContactTable->UpdateTable())
	{
		MessageBox(_T("数据修改成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		_ShowContactInfo(strCompanyType, m_strCustomShortName);
	}
	else
	{
		MessageBox(_T("数据修改失败!"), _T("失败"), MB_OK | MB_ICONERROR);
	}
}

void CCustomInfoDlg::OnBnClickedBtnCustomdlgContactDelete()
{
	CString strCompanyType;
	m_CustomTypeCombox.GetWindowText(strCompanyType);

	POSITION pos = m_ContactListCtrl.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("请先选择需要删除的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	int iSelItem = m_ContactListCtrl.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		MessageBox(_T("请先选择需要删除的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}

	if (IDYES == MessageBox(_T("您确定要删除选中的记录吗？"), _T("确认"), MB_YESNO | MB_ICONQUESTION))
	{
		if (m_pContactTable->DeleteItem(iSelItem) && m_pContactTable->UpdateTable())
		{
			LRESULT res;
			_ShowContactInfo(strCompanyType, m_strCustomShortName);
			OnNMClickListCustomdlgContacts(NULL, &res);
			MessageBox(_T("数据删除成功！"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(_T("数据删除失败!"), _T("失败"), MB_OK | MB_ICONERROR);
		}
	}
}


void CCustomInfoDlg::OnBnClickedBtnCustomdlgNew()
{
	m_eTableOption = eTableOptions_New;

	UpdateData();		//不能省
	_GetInitializeData();	
	GetDlgItem(IDC_EDIT_CUSTOMDLG_FULLNAME)->SetFocus();
}


void CCustomInfoDlg::OnBnClickedOk()
{
	CString strShared;

	UpdateData(TRUE);
	m_SharedOrNotCombox.GetWindowText(strShared);

	if (m_strCustomName.IsEmpty())
	{
		MessageBox(_T("单位名称 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strCustomShortName.IsEmpty())
	{
		MessageBox(_T("单位简称 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strCustomFindCode.IsEmpty())
	{
		MessageBox(_T("单位查询码 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strCustomContact.IsEmpty())
	{
		MessageBox(_T("联系人 不能为空!"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}

	if (eTableOptions_New == m_eTableOption)
	{
		CString strType;
		m_CustomTypeCombox.GetWindowText(strType);
		//
		m_pCurTable->AddNewItem();
		m_pCurTable->SetRecordCell(-1, _T("CustomID"), m_strCustomID.GetBuffer(m_strCustomID.GetLength()));
		m_pCurTable->SetRecordCell(-1, _T("Type"), strType.GetBuffer(strType.GetLength()));
		m_pCurTable->SetRecordCell(-1, _T("ShareOrNot"), strShared.GetBuffer(strShared.GetLength()));
	}
	else
	{
		m_pCurTable->SetRecordCell(m_iSelIndex, _T("ShareOrNot"), strShared.GetBuffer(strShared.GetLength()));
	}
	// 录入人
	m_pCurTable->SetRecordCell(-1, L"Inputer", m_strCustomInputer);
	// 单位名称
	m_pCurTable->SetRecordCell(-1, L"FullName", m_strCustomName);
	// 单位简称
	m_pCurTable->SetRecordCell(-1, L"ShortName", m_strCustomShortName);
	// 查询码
	m_pCurTable->SetRecordCell(-1, L"Code", m_strCustomFindCode);
	// 单位电话
	m_pCurTable->SetRecordCell(-1, L"Tel", m_strCustomTel);
	// 单位传真
	m_pCurTable->SetRecordCell(-1, L"Fox", m_strCustomFox);
	// 网址
	m_pCurTable->SetRecordCell(-1, L"Websit", m_strCustomHttp);
	// 联系人
	m_pCurTable->SetRecordCell(-1, L"Contact", m_strCustomContact);
	// 联系人电话
	m_pCurTable->SetRecordCell(-1, L"ContactTel", m_strCustomContactTel);
	// 联系人Email
	m_pCurTable->SetRecordCell(-1, L"Email", m_strCustomEmail);
	// 单位地址
	m_pCurTable->SetRecordCell(-1, L"Address", m_strCustomAddress);
	// 邮编
	m_pCurTable->SetRecordCell(-1, L"PostCode", m_strCustomPostCode);
	// 开户行
	m_pCurTable->SetRecordCell(-1, L"Bank", m_strCustomBank);
	// 开户行号
	m_pCurTable->SetRecordCell(-1, L"BankNumber", m_strCustomBankID);
	// 账号
	m_pCurTable->SetRecordCell(-1, L"Account", m_strCustomAccount);
	// 税号
	m_pCurTable->SetRecordCell(-1, L"TaxNumber", m_strCustomTaxID);
	// 备注
	m_pCurTable->SetRecordCell(-1, L"Others", m_strCustomComment);

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

void CCustomInfoDlg::OnBnClickedBtnCustomdlgDelete()
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

void CCustomInfoDlg::_GetInitializeData()
{
	CString strTypeValue;

	if (eTableOptions_Modify == m_eTableOption)
	{
		DWORD dwValue = 0;
		CString strShareValue;

		// 单位类别
		m_pCurTable->GetRecordCell(m_iSelIndex, L"Type", strTypeValue.GetBuffer(512), 512);
		for (int i = 0; i < m_CustomTypeCombox.GetCount(); i++)
		{
			CString strText;
			m_CustomTypeCombox.GetLBText(i, strText);
			if (_tcsicmp(strText, strTypeValue) == 0)
			{
				m_CustomTypeCombox.SetCurSel(i);
				break;
			}
		}
		// 客户号
		m_pCurTable->GetRecordCell(-1, L"CustomID", m_strCustomID.GetBuffer(512), 512);
		// 录入人
		m_pCurTable->GetRecordCell(-1, L"Inputer", m_strCustomInputer.GetBuffer(512), 512);
		if (m_strCustomInputer.IsEmpty()) m_strCustomInputer = g_strUserName;
		// 是否共享
		m_pCurTable->GetRecordCell(m_iSelIndex, L"ShareOrNot", strShareValue.GetBuffer(512), 512);
		if (_tcsicmp(strShareValue, _T("是")) == 0) m_SharedOrNotCombox.SetCurSel(0);
		else m_SharedOrNotCombox.SetCurSel(1);
		// 单位名称
		m_pCurTable->GetRecordCell(-1, L"FullName", m_strCustomName.GetBuffer(512), 512);
		// 单位简称
		m_pCurTable->GetRecordCell(-1, L"ShortName", m_strCustomShortName.GetBuffer(512), 512);
		// 查询码
		m_pCurTable->GetRecordCell(-1, L"Code", m_strCustomFindCode.GetBuffer(512), 512);
		// 单位电话
		m_pCurTable->GetRecordCell(-1, L"Tel", m_strCustomTel.GetBuffer(512), 512);
		// 单位传真
		m_pCurTable->GetRecordCell(-1, L"Fox", m_strCustomFox.GetBuffer(512), 512);
		// 网址
		m_pCurTable->GetRecordCell(-1, L"Websit", m_strCustomHttp.GetBuffer(512), 512);
		// 联系人
		m_pCurTable->GetRecordCell(-1, L"Contact", m_strCustomContact.GetBuffer(512), 512);
		// 联系人电话
		m_pCurTable->GetRecordCell(-1, L"ContactTel", m_strCustomContactTel.GetBuffer(512), 512);
		// 联系人Email
		m_pCurTable->GetRecordCell(-1, L"Email", m_strCustomEmail.GetBuffer(512), 512);
		// 单位地址
		m_pCurTable->GetRecordCell(-1, L"Address", m_strCustomAddress.GetBuffer(512), 512);
		// 邮编
		m_pCurTable->GetRecordCell(-1, L"PostCode", m_strCustomPostCode.GetBuffer(512), 512);
		// 开户行
		m_pCurTable->GetRecordCell(-1, L"Bank", m_strCustomBank.GetBuffer(512), 512);
		// 开户行号
		m_pCurTable->GetRecordCell(-1, L"BankNumber", m_strCustomBankID.GetBuffer(512), 512);
		// 账号
		m_pCurTable->GetRecordCell(-1, L"Account", m_strCustomAccount.GetBuffer(512), 512);
		// 税号
		m_pCurTable->GetRecordCell(-1, L"TaxNumber", m_strCustomTaxID.GetBuffer(512), 512);
		// 备注
		m_pCurTable->GetRecordCell(-1, L"Others", m_strCustomComment.GetBuffer(512), 512);
		
		m_CustomTypeCombox.EnableWindow(FALSE);
	}
	else
	{
		int iNewRecordIndex = GetNewRecordIndex(g_Tables[eTableID_Custom].strName, _T("CustomID"), g_strCompanyType[0].strShortName);
		m_CustomTypeCombox.SetCurSel(0);
		m_CustomTypeCombox.GetWindowText(strTypeValue);
		m_strCustomID.Format(_T("%s%04d"), g_strCompanyType[0].strShortName, iNewRecordIndex);
		m_strCustomInputer = g_strUserName;
		m_SharedOrNotCombox.SetCurSel(0);
		m_strCustomName = _T("");
		m_strCustomShortName = _T("");
		m_strCustomFindCode = _T("");
		m_strCustomTel = _T("");
		m_strCustomFox = _T("");
		m_strCustomHttp = _T("");
		m_strCustomContact.Empty();
		m_strCustomContactTel = _T("");
		m_strCustomEmail = _T("");
		m_strCustomAddress = _T("");
		m_strCustomPostCode = _T("");
		m_strCustomBank = _T("");
		m_strCustomBankID = _T("");
		m_strCustomAccount = _T("");
		m_strCustomTaxID = _T("");
		m_strCustomComment = _T("");

		m_CustomTypeCombox.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	// 打开联系人信息表
	_ShowContactInfo(strTypeValue, m_strCustomShortName);
}

void CCustomInfoDlg::_GetInitializeDataByContact()
{
	//	获取该联系人对应的公司类型和简称
	CString strCompanyType;
	CString strCompanyShortName;	
	m_pCurTable->GetRecordCell(m_iSelIndex, L"Type", strCompanyType);
	m_pCurTable->GetRecordCell(m_iSelIndex, L"CompanyName", strCompanyShortName);

	//	根据公司类型和简称，查询公司详细
	CString strSQL;
	CADOTable table;
	BOOL bResult = FALSE;
	strSQL.Format(_T("SELECT * FROM %s WHERE Type='%s' AND ShortName='%s'"), 
		g_Tables[eTableID_Custom].strName, strCompanyType, strCompanyShortName);
	bResult = table.OpenTable(g_pDatabase->GetConnectionPtr(), strSQL);
	if (FALSE == bResult || table.GetRecordCnt() == 0)
	{
		return;
	}
	
	//	使用查找到的公司详细初始化UI
	DWORD dwValue = 0;
	CString strShareValue;

	// 单位类别
	for (int i = 0; i < m_CustomTypeCombox.GetCount(); i++)
	{
		CString strText;
		m_CustomTypeCombox.GetLBText(i, strText);
		if (_tcsicmp(strText, strCompanyType) == 0)
		{
			m_CustomTypeCombox.SetCurSel(i);
			break;
		}
	}
	// 客户号
	table.GetRecordCell(-1, L"CustomID", m_strCustomID);
	// 录入人
	table.GetRecordCell(-1, L"Inputer", m_strCustomInputer);
	if (m_strCustomInputer.IsEmpty()) m_strCustomInputer = g_strUserName;
	// 是否共享
	table.GetRecordCell(m_iSelIndex, L"ShareOrNot", strShareValue.GetBuffer(512), 512);
	if (_tcsicmp(strShareValue, _T("是")) == 0) m_SharedOrNotCombox.SetCurSel(0);
	else m_SharedOrNotCombox.SetCurSel(1);
	// 单位名称
	table.GetRecordCell(-1, L"FullName", m_strCustomName);
	// 单位简称
	table.GetRecordCell(-1, L"ShortName", m_strCustomShortName);
	// 查询码
	table.GetRecordCell(-1, L"Code", m_strCustomFindCode);
	// 单位电话
	table.GetRecordCell(-1, L"Tel", m_strCustomTel);
	// 单位传真
	table.GetRecordCell(-1, L"Fox", m_strCustomFox);
	// 网址
	table.GetRecordCell(-1, L"Websit", m_strCustomHttp);
	// 联系人
	table.GetRecordCell(-1, L"Contact", m_strCustomContact);
	// 联系人电话
	table.GetRecordCell(-1, L"ContactTel", m_strCustomContactTel);
	// 联系人Email
	table.GetRecordCell(-1, L"Email", m_strCustomEmail);
	// 单位地址
	table.GetRecordCell(-1, L"Address", m_strCustomAddress);
	// 邮编
	table.GetRecordCell(-1, L"PostCode", m_strCustomPostCode);
	// 开户行
	table.GetRecordCell(-1, L"Bank", m_strCustomBank);
	// 开户行号
	table.GetRecordCell(-1, L"BankNumber", m_strCustomBankID);
	// 账号
	table.GetRecordCell(-1, L"Account", m_strCustomAccount);
	// 税号
	table.GetRecordCell(-1, L"TaxNumber", m_strCustomTaxID);
	// 备注
	table.GetRecordCell(-1, L"Others", m_strCustomComment);
	//
	UpdateData(FALSE);

	// 打开联系人信息表
	_ShowContactInfo(strCompanyType, m_strCustomShortName);
}

DWORD CCustomInfoDlg::_GetNewCompanyIndex(CString strTypeShortName)
{
	BOOL bResult = FALSE;
	DWORD dwCurIndex = 0;
	DWORD dwItemCnt = 0;
	CString strSQL;
	CADOTable table;

	strSQL.Format(_T("SELECT CustomID FROM %s WHERE CustomID LIKE '%s%s' ORDER BY CustomID"), 
		g_Tables[eTableID_Custom].strName, strTypeShortName, _T("%"));

	table.CloseTable();
	bResult = table.OpenTable(g_pDatabase->GetConnectionPtr(), strSQL);
	if (FALSE == bResult)
	{
		return 0;
	}
	
	dwItemCnt = table.GetRecordCnt();
	if (dwItemCnt > 0)
	{
		CString strID;
		TCHAR tcIDValue[32] = {0};
		table.GetRecordCell(dwItemCnt-1, _T("CustomID"), tcIDValue, 32);
		strID = tcIDValue;
		strID = strID.Right(strID.GetLength() - strTypeShortName.GetLength());
		dwCurIndex = _ttol(strID);
	}
	dwCurIndex += 1;
	table.CloseTable();

	return dwCurIndex;
}

void CCustomInfoDlg::_ShowContactInfo(CString strType, CString strCompanyShotrName)
{
	BOOL bResult = FALSE;
	DWORD dwItemCnt = 0;
	CString strSQL;

	m_ContactListCtrl.DeleteAllItems();

	strSQL.Format(_T("SELECT * FROM %s WHERE Type = '%s' AND CompanyName = '%s'"), 
		g_Tables[eTableID_Contact].strName, strType, strCompanyShotrName);

	if (!m_pContactTable) m_pContactTable = new CADOTable();
	else if (m_pContactTable) m_pContactTable->CloseTable();

	bResult = m_pContactTable->OpenTable(g_pDatabase->GetConnectionPtr(), strSQL);
	if (FALSE == bResult)
	{
		return;
	}
	
	dwItemCnt = m_pContactTable->GetRecordCnt();
	FillListFromTable(&m_ContactListCtrl, m_pContactTable, &g_Tables[eTableID_Contact], 0, dwItemCnt);
}
