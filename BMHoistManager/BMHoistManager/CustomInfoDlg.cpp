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
	m_SharedOrNotCombox.AddString(_T("��"));
	m_SharedOrNotCombox.AddString(_T("��"));
	m_SharedOrNotCombox.SetCurSel(0);
	//
	m_ContactSexCombox.ResetContent();
	m_ContactSexCombox.AddString(_T("��"));
	m_ContactSexCombox.AddString(_T("Ů"));	
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
		SetDlgItemText(IDCANCEL, _T("��  ��"));
		
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
		if (strSex == _T("��")) m_ContactSexCombox.SetCurSel(0);
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
	
	//	����Ǳ༭��ϵ����Ϣ����֧���޸�ȱʡ��ϵ�˲���
	if (m_bIsModifyContact)
	{
		*pResult = 0;
		return;
	}

	//	��ѡ�е�ǰ��ϵ��
	OnNMClickListCustomdlgContacts(pNMHDR, pResult);

	//	�����˵�
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
		MessageBox(_T("��ϵ������ ����Ϊ��!"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strContactFindCode.IsEmpty())
	{
		MessageBox(_T("��ϵ�˲�ѯ�� ����Ϊ��!"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strCustomShortName.IsEmpty())
	{
		MessageBox(_T("��λ��� ����Ϊ��!"), _T("����"), MB_OK | MB_ICONERROR);
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
		MessageBox(_T("������ӳɹ�!"), _T("�ɹ�"), MB_OK | MB_ICONINFORMATION);
		_ShowContactInfo(strCompanyType, m_strCustomShortName);

		// ����ǵ�һ����ϵ����Ϣ���Զ�����Ϊȱʡ��ϵ��
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
		MessageBox(_T("�������ʧ��!"), _T("ʧ��"), MB_OK | MB_ICONERROR);
	}
}

void CCustomInfoDlg::OnBnClickedBtnCustomdlgContactModify()
{
	CString strCompanyType;
	CString strContactSex;

	POSITION pos = m_ContactListCtrl.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("����ѡ����Ҫ�޸ĵļ�¼!"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}
	int iSelItem = m_ContactListCtrl.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		MessageBox(_T("����ѡ����Ҫ�޸ĵļ�¼!"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}

	UpdateData(TRUE);
	m_CustomTypeCombox.GetWindowText(strCompanyType);
	m_ContactSexCombox.GetWindowText(strContactSex);

	if (m_strContactName.IsEmpty())
	{
		MessageBox(_T("��ϵ������ ����Ϊ��!"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strContactFindCode.IsEmpty())
	{
		MessageBox(_T("��ϵ�˲�ѯ�� ����Ϊ��!"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strCustomShortName.IsEmpty())
	{
		MessageBox(_T("��λ��� ����Ϊ��!"), _T("����"), MB_OK | MB_ICONERROR);
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
		MessageBox(_T("�����޸ĳɹ�!"), _T("�ɹ�"), MB_OK | MB_ICONINFORMATION);
		_ShowContactInfo(strCompanyType, m_strCustomShortName);
	}
	else
	{
		MessageBox(_T("�����޸�ʧ��!"), _T("ʧ��"), MB_OK | MB_ICONERROR);
	}
}

void CCustomInfoDlg::OnBnClickedBtnCustomdlgContactDelete()
{
	CString strCompanyType;
	m_CustomTypeCombox.GetWindowText(strCompanyType);

	POSITION pos = m_ContactListCtrl.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("����ѡ����Ҫɾ���ļ�¼!"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}
	int iSelItem = m_ContactListCtrl.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		MessageBox(_T("����ѡ����Ҫɾ���ļ�¼!"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}

	if (IDYES == MessageBox(_T("��ȷ��Ҫɾ��ѡ�еļ�¼��"), _T("ȷ��"), MB_YESNO | MB_ICONQUESTION))
	{
		if (m_pContactTable->DeleteItem(iSelItem) && m_pContactTable->UpdateTable())
		{
			LRESULT res;
			_ShowContactInfo(strCompanyType, m_strCustomShortName);
			OnNMClickListCustomdlgContacts(NULL, &res);
			MessageBox(_T("����ɾ���ɹ���"), _T("�ɹ�"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(_T("����ɾ��ʧ��!"), _T("ʧ��"), MB_OK | MB_ICONERROR);
		}
	}
}


void CCustomInfoDlg::OnBnClickedBtnCustomdlgNew()
{
	m_eTableOption = eTableOptions_New;

	UpdateData();		//����ʡ
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
		MessageBox(_T("��λ���� ����Ϊ��!"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strCustomShortName.IsEmpty())
	{
		MessageBox(_T("��λ��� ����Ϊ��!"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strCustomFindCode.IsEmpty())
	{
		MessageBox(_T("��λ��ѯ�� ����Ϊ��!"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_strCustomContact.IsEmpty())
	{
		MessageBox(_T("��ϵ�� ����Ϊ��!"), _T("����"), MB_OK | MB_ICONERROR);
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
	// ¼����
	m_pCurTable->SetRecordCell(-1, L"Inputer", m_strCustomInputer);
	// ��λ����
	m_pCurTable->SetRecordCell(-1, L"FullName", m_strCustomName);
	// ��λ���
	m_pCurTable->SetRecordCell(-1, L"ShortName", m_strCustomShortName);
	// ��ѯ��
	m_pCurTable->SetRecordCell(-1, L"Code", m_strCustomFindCode);
	// ��λ�绰
	m_pCurTable->SetRecordCell(-1, L"Tel", m_strCustomTel);
	// ��λ����
	m_pCurTable->SetRecordCell(-1, L"Fox", m_strCustomFox);
	// ��ַ
	m_pCurTable->SetRecordCell(-1, L"Websit", m_strCustomHttp);
	// ��ϵ��
	m_pCurTable->SetRecordCell(-1, L"Contact", m_strCustomContact);
	// ��ϵ�˵绰
	m_pCurTable->SetRecordCell(-1, L"ContactTel", m_strCustomContactTel);
	// ��ϵ��Email
	m_pCurTable->SetRecordCell(-1, L"Email", m_strCustomEmail);
	// ��λ��ַ
	m_pCurTable->SetRecordCell(-1, L"Address", m_strCustomAddress);
	// �ʱ�
	m_pCurTable->SetRecordCell(-1, L"PostCode", m_strCustomPostCode);
	// ������
	m_pCurTable->SetRecordCell(-1, L"Bank", m_strCustomBank);
	// �����к�
	m_pCurTable->SetRecordCell(-1, L"BankNumber", m_strCustomBankID);
	// �˺�
	m_pCurTable->SetRecordCell(-1, L"Account", m_strCustomAccount);
	// ˰��
	m_pCurTable->SetRecordCell(-1, L"TaxNumber", m_strCustomTaxID);
	// ��ע
	m_pCurTable->SetRecordCell(-1, L"Others", m_strCustomComment);

	if (TRUE == m_pCurTable->UpdateTable())
	{
		if (eTableOptions_New == m_eTableOption) MessageBox(_T("������ӳɹ�!"), _T("�ɹ�"), MB_OK | MB_ICONINFORMATION);
		else  MessageBox(_T("�����޸ĳɹ�!"), _T("�ɹ�"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		if (eTableOptions_New == m_eTableOption) MessageBox(_T("�������ʧ��!"), _T("ʧ��"), MB_OK | MB_ICONERROR);
		else  MessageBox(_T("�����޸�ʧ��!"), _T("ʧ��"), MB_OK | MB_ICONERROR);
	}

	OnOK();
}

void CCustomInfoDlg::OnBnClickedBtnCustomdlgDelete()
{
	if (IDYES == MessageBox(_T("��ȷ��Ҫɾ��ѡ�еļ�¼��"), _T("ȷ��"), MB_YESNO | MB_ICONQUESTION))
	{
		m_pCurTable->DeleteItem(m_iSelIndex);
		if (m_pCurTable->UpdateTable())
		{
			MessageBox(_T("����ɾ���ɹ���"), _T("�ɹ�"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(_T("����ɾ��ʧ��!"), _T("ʧ��"), MB_OK | MB_ICONERROR);
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

		// ��λ���
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
		// �ͻ���
		m_pCurTable->GetRecordCell(-1, L"CustomID", m_strCustomID.GetBuffer(512), 512);
		// ¼����
		m_pCurTable->GetRecordCell(-1, L"Inputer", m_strCustomInputer.GetBuffer(512), 512);
		if (m_strCustomInputer.IsEmpty()) m_strCustomInputer = g_strUserName;
		// �Ƿ���
		m_pCurTable->GetRecordCell(m_iSelIndex, L"ShareOrNot", strShareValue.GetBuffer(512), 512);
		if (_tcsicmp(strShareValue, _T("��")) == 0) m_SharedOrNotCombox.SetCurSel(0);
		else m_SharedOrNotCombox.SetCurSel(1);
		// ��λ����
		m_pCurTable->GetRecordCell(-1, L"FullName", m_strCustomName.GetBuffer(512), 512);
		// ��λ���
		m_pCurTable->GetRecordCell(-1, L"ShortName", m_strCustomShortName.GetBuffer(512), 512);
		// ��ѯ��
		m_pCurTable->GetRecordCell(-1, L"Code", m_strCustomFindCode.GetBuffer(512), 512);
		// ��λ�绰
		m_pCurTable->GetRecordCell(-1, L"Tel", m_strCustomTel.GetBuffer(512), 512);
		// ��λ����
		m_pCurTable->GetRecordCell(-1, L"Fox", m_strCustomFox.GetBuffer(512), 512);
		// ��ַ
		m_pCurTable->GetRecordCell(-1, L"Websit", m_strCustomHttp.GetBuffer(512), 512);
		// ��ϵ��
		m_pCurTable->GetRecordCell(-1, L"Contact", m_strCustomContact.GetBuffer(512), 512);
		// ��ϵ�˵绰
		m_pCurTable->GetRecordCell(-1, L"ContactTel", m_strCustomContactTel.GetBuffer(512), 512);
		// ��ϵ��Email
		m_pCurTable->GetRecordCell(-1, L"Email", m_strCustomEmail.GetBuffer(512), 512);
		// ��λ��ַ
		m_pCurTable->GetRecordCell(-1, L"Address", m_strCustomAddress.GetBuffer(512), 512);
		// �ʱ�
		m_pCurTable->GetRecordCell(-1, L"PostCode", m_strCustomPostCode.GetBuffer(512), 512);
		// ������
		m_pCurTable->GetRecordCell(-1, L"Bank", m_strCustomBank.GetBuffer(512), 512);
		// �����к�
		m_pCurTable->GetRecordCell(-1, L"BankNumber", m_strCustomBankID.GetBuffer(512), 512);
		// �˺�
		m_pCurTable->GetRecordCell(-1, L"Account", m_strCustomAccount.GetBuffer(512), 512);
		// ˰��
		m_pCurTable->GetRecordCell(-1, L"TaxNumber", m_strCustomTaxID.GetBuffer(512), 512);
		// ��ע
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

	// ����ϵ����Ϣ��
	_ShowContactInfo(strTypeValue, m_strCustomShortName);
}

void CCustomInfoDlg::_GetInitializeDataByContact()
{
	//	��ȡ����ϵ�˶�Ӧ�Ĺ�˾���ͺͼ��
	CString strCompanyType;
	CString strCompanyShortName;	
	m_pCurTable->GetRecordCell(m_iSelIndex, L"Type", strCompanyType);
	m_pCurTable->GetRecordCell(m_iSelIndex, L"CompanyName", strCompanyShortName);

	//	���ݹ�˾���ͺͼ�ƣ���ѯ��˾��ϸ
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
	
	//	ʹ�ò��ҵ��Ĺ�˾��ϸ��ʼ��UI
	DWORD dwValue = 0;
	CString strShareValue;

	// ��λ���
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
	// �ͻ���
	table.GetRecordCell(-1, L"CustomID", m_strCustomID);
	// ¼����
	table.GetRecordCell(-1, L"Inputer", m_strCustomInputer);
	if (m_strCustomInputer.IsEmpty()) m_strCustomInputer = g_strUserName;
	// �Ƿ���
	table.GetRecordCell(m_iSelIndex, L"ShareOrNot", strShareValue.GetBuffer(512), 512);
	if (_tcsicmp(strShareValue, _T("��")) == 0) m_SharedOrNotCombox.SetCurSel(0);
	else m_SharedOrNotCombox.SetCurSel(1);
	// ��λ����
	table.GetRecordCell(-1, L"FullName", m_strCustomName);
	// ��λ���
	table.GetRecordCell(-1, L"ShortName", m_strCustomShortName);
	// ��ѯ��
	table.GetRecordCell(-1, L"Code", m_strCustomFindCode);
	// ��λ�绰
	table.GetRecordCell(-1, L"Tel", m_strCustomTel);
	// ��λ����
	table.GetRecordCell(-1, L"Fox", m_strCustomFox);
	// ��ַ
	table.GetRecordCell(-1, L"Websit", m_strCustomHttp);
	// ��ϵ��
	table.GetRecordCell(-1, L"Contact", m_strCustomContact);
	// ��ϵ�˵绰
	table.GetRecordCell(-1, L"ContactTel", m_strCustomContactTel);
	// ��ϵ��Email
	table.GetRecordCell(-1, L"Email", m_strCustomEmail);
	// ��λ��ַ
	table.GetRecordCell(-1, L"Address", m_strCustomAddress);
	// �ʱ�
	table.GetRecordCell(-1, L"PostCode", m_strCustomPostCode);
	// ������
	table.GetRecordCell(-1, L"Bank", m_strCustomBank);
	// �����к�
	table.GetRecordCell(-1, L"BankNumber", m_strCustomBankID);
	// �˺�
	table.GetRecordCell(-1, L"Account", m_strCustomAccount);
	// ˰��
	table.GetRecordCell(-1, L"TaxNumber", m_strCustomTaxID);
	// ��ע
	table.GetRecordCell(-1, L"Others", m_strCustomComment);
	//
	UpdateData(FALSE);

	// ����ϵ����Ϣ��
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
