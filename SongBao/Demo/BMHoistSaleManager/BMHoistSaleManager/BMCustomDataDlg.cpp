//-----------------------------------------------------------------------
//	File Name:BMCustomDataDlg.cpp
//	Introduce:Class CBMCustomDataDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMCustomDataDlg.h"
#include "Localization.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMCustomDataDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMCustomDataDlg, CDialog)

CBMCustomDataDlg::CBMCustomDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMCustomDataDlg::IDD, pParent)
{
}
//-----------------------------------------------------------------------
CBMCustomDataDlg::~CBMCustomDataDlg()
{
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONTACTLIST, m_lstCtlContacts);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMCustomDataDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CBMCustomDataDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CBMCustomDataDlg::OnBnClickedBtnModify)
	ON_BN_CLICKED(IDC_BTN_REMOVE, &CBMCustomDataDlg::OnBnClickedBtnRemove)
	ON_BN_CLICKED(IDC_BTN_OK, &CBMCustomDataDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CBMCustomDataDlg::OnBnClickedBtnClear)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMCustomDataDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMCustomDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CenterWindow();

	// Initiliaze company level CComboBox items
	InitLevelComboBox();

	// Initiliaze tables data struct
	InitTablesDataStruct();

	// Initiliaze lists
	m_imgNULL.Create(1, 18, TRUE | ILC_COLOR32, 1, 0); 
	InitDataListCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if (FALSE == bShow)
		return;

	// Initliaze data for default
	OnBnClickedBtnClear();
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::OnClose()
{
	m_bIsEditOpt = FALSE;
	m_strInitCompanyID = _T("\0");

	CDialog::OnClose();
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::OnBnClickedBtnAdd()
{
	UpdateContactsInfoList();
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::OnBnClickedBtnModify()
{
	int iSelItem = -1;
	POSITION pos = m_lstCtlContacts.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlContacts.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		myMessageBox(IDS_EDIT_NOITEMSELECTED, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}

	CString strBtnText;
	GetDlgItemText(IDC_BTN_MODIFY, strBtnText);
	if (strBtnText == CString(myGetText(IDS_CUSTOMDLG_EDITBTN)))
	{
		CString strTmp;
		strTmp = m_lstCtlContacts.GetItemText(iSelItem, 0);
		SetDlgItemText(IDC_EDIT_CONTACTNAME, strTmp);
		strTmp = m_lstCtlContacts.GetItemText(iSelItem, 1);
		SetDlgItemText(IDC_EDIT_CONTACTPOS, strTmp);
		strTmp = m_lstCtlContacts.GetItemText(iSelItem, 2);
		SetDlgItemText(IDC_EDIT_CONTACTTEL, strTmp);
		strTmp = m_lstCtlContacts.GetItemText(iSelItem, 3);
		SetDlgItemText(IDC_EDIT_CONTACTCELLPHONE, strTmp);
		strTmp = m_lstCtlContacts.GetItemText(iSelItem, 4);
		SetDlgItemText(IDC_EDIT_CONTACTEMAIL, strTmp);
		//
		SetDlgItemText(IDC_BTN_MODIFY, myGetText(IDS_CUSTOMDLG_UPDATEBTN));
		GetDlgItem(IDC_BTN_ADD)->EnableWindow(FALSE);
	}
	else
	{
		UpdateContactsInfoList(iSelItem);
		SetDlgItemText(IDC_BTN_MODIFY, myGetText(IDS_CUSTOMDLG_EDITBTN));
		GetDlgItem(IDC_BTN_ADD)->EnableWindow(TRUE);
	}
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::OnBnClickedBtnRemove()
{
	int iSelItem = -1;
	POSITION pos = m_lstCtlContacts.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlContacts.GetNextSelectedItem(pos);
	if (/*NULL == pos || */iSelItem < 0)
	{
		myMessageBox(IDS_DELETE_NOITEMSELECTED, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}

	if (IDYES != myMessageBox(IDS_DELETE_CONFIRM, IDS_DLGMSG_WARNING, MB_YESNO | MB_ICONQUESTION))
		return;

	m_lstCtlContacts.DeleteItem(iSelItem);
	m_lstCtlContacts.UpdateWindow();
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::OnBnClickedBtnOk()
{
	if (NULL == g_pOfferDatabase)
		return;

	CStringW strOpenOperation;
	if (TRUE == m_bIsEditOpt)
		strOpenOperation.Format(L"SELECT * FROM %s WHERE CompanyNo='%s'", g_tcTableName[eTableID_CustomerInfo], m_strInitCompanyID);
	else
		strOpenOperation.Format(L"SELECT * FROM %s", g_tcTableName[eTableID_CustomerInfo]);

	// Get new data from UI
	DWORD dwLevel;
	DWORD dwPostCode;
	CString strCompanyID;
	CString strShortName;
	CString strFullName;
	CString strAddress;
	CString strTel;
	CString strFox;	
	CString strHttp;
	GetDlgItemText(IDC_EDIT_NO, strCompanyID);
	GetDlgItemText(IDC_EDIT_SHORTNAME, strShortName);
	GetDlgItemText(IDC_EDIT_FULLNAME, strFullName);
	GetDlgItemText(IDC_EDIT_ADDRESS, strAddress);
	GetDlgItemText(IDC_EDIT_TEL, strTel);
	GetDlgItemText(IDC_EDIT_FOX, strFox);
	GetDlgItemText(IDC_EDIT_HTTP, strHttp);
	//
	CComboBox* pLevelComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LEVEL);
	dwLevel = pLevelComboBox->GetItemData(pLevelComboBox->GetCurSel());
	//
	CString strTmp;
	GetDlgItemText(IDC_EDIT_POSTCODE, strTmp);
	dwPostCode = _ttol(strTmp);
	//
	if (strCompanyID.IsEmpty())
	{		
		myMessageBox(IDS_CUSTOMDLG_COMPANYIDISNULL, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_NO)->SetFocus();
		return;
	}
	if (strShortName.IsEmpty())
	{		
		myMessageBox(IDS_CUSTOMDLG_COMPANYSHORTNAMEISNULL, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_SHORTNAME)->SetFocus();
		return;
	}

	// Open CustomInfo table from database
	BOOL bResult = TRUE;
	CADOTable tbCustomInfo;
	bResult = tbCustomInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	// Update new data to databse
	if (TRUE == m_bIsEditOpt)
	{
		tbCustomInfo.SetRecordCell(0, L"CompanyNo", strCompanyID.GetBuffer(32));
	}
	else
	{
		tbCustomInfo.AddNewItem();
		tbCustomInfo.SetRecordCell(-1, L"CompanyNo", strCompanyID.GetBuffer(32));
	}
	tbCustomInfo.SetRecordCell(-1, _T("CompanyShortName"), strShortName.GetBuffer(16));
	tbCustomInfo.SetRecordCell(-1, _T("Level"), dwLevel);
	tbCustomInfo.SetRecordCell(-1, _T("FullName"), strFullName.GetBuffer(64));
	tbCustomInfo.SetRecordCell(-1, _T("Address"), strAddress.GetBuffer(64));
	tbCustomInfo.SetRecordCell(-1, _T("PostCode"), dwPostCode);
	tbCustomInfo.SetRecordCell(-1, _T("Tel"), strTel.GetBuffer(32));
	tbCustomInfo.SetRecordCell(-1, _T("Fox"), strFox.GetBuffer(32));
	tbCustomInfo.SetRecordCell(-1, _T("Http"), strHttp.GetBuffer(64));
	if (FALSE == tbCustomInfo.UpdateTable())
	{
		if (TRUE == m_bIsEditOpt) myMessageBox(IDS_MSGDLG_EDITFAILED, IDS_MSGDLG_FAILED, MB_OK | MB_ICONERROR);
		else  myMessageBox(IDS_MSGDLG_ADDFAILED, IDS_MSGDLG_FAILED, MB_OK | MB_ICONERROR);
		return;
	}
	tbCustomInfo.CloseTable();

	// Add contacts information to database
	CADOTable tbContactsInfo;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE CompanyNo='%s'", g_tcTableName[eTableID_ContactsInfo], m_strInitCompanyID);
	bResult = tbContactsInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (TRUE == bResult)
	{
		// Remove the old contacts information in database
		DWORD dwRecordCnt = tbContactsInfo.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwRecordCnt; dwIndex++)
		{
			tbContactsInfo.DeleteItem(0);
		}
		tbContactsInfo.UpdateTable();

		// Add new contacts information to database
		DWORD dwNewContactCnt = m_lstCtlContacts.GetItemCount();
		for (DWORD dwIndex = 0; dwIndex < dwNewContactCnt; dwIndex++)
		{
			tbContactsInfo.AddNewItem();

			CString strTmp;
			int iColumnIndex = 0;
			for (TableColumnsList::iterator it = m_stuContactsTable.lstColumns.begin();
				it != m_stuContactsTable.lstColumns.end();
				it++, iColumnIndex++)
			{	
				// This is companyID
				if (0 == iColumnIndex)
				{
					tbContactsInfo.SetRecordCell(-1, (*it).strName, strCompanyID.GetBuffer(32));
					continue;
				}

				CString strTmp = m_lstCtlContacts.GetItemText(dwIndex, iColumnIndex - 1);
				switch((*it).eDataType)
				{
				case eTableDataType_String:
					tbContactsInfo.SetRecordCell(-1, (*it).strName, strTmp.GetBuffer(64));
					break;
				case eTableDataType_DWORD:
					tbContactsInfo.SetRecordCell(-1, (*it).strName, (DWORD)_ttol(strTmp));
					break;
				}
			}
		}
		tbContactsInfo.UpdateTable();
		tbContactsInfo.CloseTable();
	}

	// Tell main dailog to update data list
	::PostMessage(m_pParentWnd->m_hWnd, WM_USER_DATAISUPDATED, 0, 0);

	if (TRUE == m_bIsEditOpt) myMessageBox(IDS_MSGDLG_EDITSUCCESS, IDS_MSGDLG_SUCCESS, MB_OK | MB_ICONINFORMATION);
	else  myMessageBox(IDS_MSGDLG_ADDSUCCESS, IDS_MSGDLG_SUCCESS, MB_OK | MB_ICONINFORMATION);
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::OnBnClickedBtnClear()
{
	if (TRUE ==  m_bIsEditOpt)
	{
		InitDataFromDatabase();
	}
	else
	{	
		SetDlgItemText(IDC_EDIT_NO, m_strInitCompanyID);
		SetDlgItemText(IDC_EDIT_SHORTNAME, _T(""));
		SelectComboBoxItem((CComboBox*)GetDlgItem(IDC_COMBO_LEVEL), 1);
		SetDlgItemText(IDC_EDIT_FULLNAME, _T(""));
		SetDlgItemText(IDC_EDIT_ADDRESS, _T(""));
		SetDlgItemText(IDC_EDIT_POSTCODE, _T(""));
		SetDlgItemText(IDC_EDIT_TEL, _T(""));
		SetDlgItemText(IDC_EDIT_FOX, _T(""));
		SetDlgItemText(IDC_EDIT_HTTP, _T(""));
		GetDlgItem(IDC_EDIT_NO)->SetFocus();
	}
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void CBMCustomDataDlg::SetCustomerID(CString strCompanyID, BOOL bIsEdit)
{
	m_bIsEditOpt = bIsEdit;
	m_strInitCompanyID = strCompanyID;
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::InitLevelComboBox()
{
	CComboBox* pLevelCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LEVEL);
	int iIndex = pLevelCombo->AddString(_T("1"));
	pLevelCombo->SetItemData(iIndex, 1);
	iIndex = pLevelCombo->AddString(_T("2"));
	pLevelCombo->SetItemData(iIndex, 2);
	iIndex = pLevelCombo->AddString(_T("3"));
	pLevelCombo->SetItemData(iIndex, 3);
	pLevelCombo->SetCurSel(0);
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::InitDataListCtrls()
{
	m_lstCtlContacts.SetExtendedStyle(m_lstCtlContacts.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_lstCtlContacts.SetFullItemBKColor(RGB(214, 231, 255), RGB(94, 231, 221));
	m_lstCtlContacts.SetImageList(&m_imgNULL, LVSIL_SMALL);
	InitDataListCtrl(&m_lstCtlContacts, m_stuContactsTable);
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::InitTablesDataStruct()
{
	// "ContactInfo" table
	m_stuContactsTable.eID = eTableID_ContactsInfo;
	m_stuContactsTable.strName = g_tcTableName[eTableID_ContactsInfo];
	m_stuContactsTable.lstColumns.push_back(TableColumn(FALSE, 60,  L"CompanyNo",		IDS_CUSTOMLIST_COMPANYNO,	eTableDataType_String, FALSE));
	m_stuContactsTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"ContactName",		IDS_CUSTOMDLG_CONTACTNAME,	eTableDataType_String));
	m_stuContactsTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"Position",		IDS_CUSTOMDLG_CONTACTPOS,	eTableDataType_String));
	m_stuContactsTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"Tel",				IDS_CUSTOMDLG_CONTACTTEL,	eTableDataType_String));
	m_stuContactsTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"CellPhone",		IDS_CUSTOMDLG_CONTACTCELL,	eTableDataType_String));
	m_stuContactsTable.lstColumns.push_back(TableColumn(FALSE, 100, L"Email",			IDS_CUSTOMDLG_CONTACTEMAIL,	eTableDataType_String));
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::InitDataFromDatabase()
{
	if (NULL == g_pOfferDatabase)
		return;

	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE CompanyNo='%s'", g_tcTableName[eTableID_CustomerInfo], m_strInitCompanyID);

	// Open CustomInfo table from database
	BOOL bResult = TRUE;
	CADOTable tbCustomInfo;
	bResult = tbCustomInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult || tbCustomInfo.GetRecordCnt() == 0)
		return;

	DWORD dwTmp;
	CString strTmp;
	tbCustomInfo.GetRecordCell(0, _T("CompanyNo"), strTmp.GetBuffer(32), 32);
	SetDlgItemText(IDC_EDIT_NO, strTmp);
	//
	tbCustomInfo.GetRecordCell(0, _T("CompanyShortName"), strTmp.GetBuffer(16), 16);
	SetDlgItemText(IDC_EDIT_SHORTNAME, strTmp);
	//
	tbCustomInfo.GetRecordCell(0, _T("Level"), dwTmp);
	SelectComboBoxItem((CComboBox*)GetDlgItem(IDC_COMBO_LEVEL), dwTmp);
	//
	tbCustomInfo.GetRecordCell(0, _T("FullName"), strTmp.GetBuffer(64), 64);
	SetDlgItemText(IDC_EDIT_FULLNAME, strTmp);
	//
	tbCustomInfo.GetRecordCell(0, _T("Address"), strTmp.GetBuffer(64), 64);
	SetDlgItemText(IDC_EDIT_ADDRESS, strTmp);
	//
	tbCustomInfo.GetRecordCell(0, _T("PostCode"), dwTmp);
	strTmp.Format(_T("%d"), dwTmp);
	SetDlgItemText(IDC_EDIT_POSTCODE, strTmp);
	//
	tbCustomInfo.GetRecordCell(0, _T("Tel"), strTmp.GetBuffer(32), 32);
	SetDlgItemText(IDC_EDIT_TEL, strTmp);
	//
	tbCustomInfo.GetRecordCell(0, _T("Fox"), strTmp.GetBuffer(32), 32);
	SetDlgItemText(IDC_EDIT_FOX, strTmp);
	//
	tbCustomInfo.GetRecordCell(0, _T("Http"), strTmp.GetBuffer(64), 64);
	SetDlgItemText(IDC_EDIT_HTTP, strTmp);

	// Open ContactInfo table from database
	CADOTable tbContactInfo;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE CompanyNo='%s'", g_tcTableName[eTableID_ContactsInfo], m_strInitCompanyID);
	bResult = tbContactInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult || tbContactInfo.GetRecordCnt() == 0)
		return;
	ShowTableRecords(&tbContactInfo, &m_lstCtlContacts, m_stuContactsTable);
}
//-----------------------------------------------------------------------
void CBMCustomDataDlg::UpdateContactsInfoList(int iEditItem)
{
	CString strName, strPos, strTel, strCellPhone, strEmail;
	GetDlgItemText(IDC_EDIT_CONTACTNAME, strName);
	GetDlgItemText(IDC_EDIT_CONTACTPOS, strPos);
	GetDlgItemText(IDC_EDIT_CONTACTTEL, strTel);
	GetDlgItemText(IDC_EDIT_CONTACTCELLPHONE, strCellPhone);
	GetDlgItemText(IDC_EDIT_CONTACTEMAIL, strEmail);
	if (strName.IsEmpty())
	{
		myMessageBox(IDS_CUSTOMDLG_CONTNAMEISNULL, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_CONTACTNAME)->SetFocus();
		return;
	}
	if (strTel.IsEmpty() && strCellPhone.IsEmpty())
	{
		myMessageBox(IDS_CUSTOMDLG_CONTTELISNULL, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		if (strTel.IsEmpty()) GetDlgItem(IDC_EDIT_CONTACTTEL)->SetFocus();
		else GetDlgItem(IDC_EDIT_CONTACTCELLPHONE)->SetFocus();
		return;
	}
	
	// Add as a new item	
	if (-1 == iEditItem)
	{
		iEditItem = m_lstCtlContacts.GetItemCount();
		m_lstCtlContacts.InsertItem(iEditItem, strName);
	}
	else
	{
		m_lstCtlContacts.SetItemText(iEditItem, 0, strName);
	}
	m_lstCtlContacts.SetItemText(iEditItem, 1, strPos);
	m_lstCtlContacts.SetItemText(iEditItem, 2, strTel);
	m_lstCtlContacts.SetItemText(iEditItem, 3, strCellPhone);
	m_lstCtlContacts.SetItemText(iEditItem, 4, strEmail);
}
//-----------------------------------------------------------------------