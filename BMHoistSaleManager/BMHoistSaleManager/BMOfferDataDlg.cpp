//-----------------------------------------------------------------------
//	File Name:BMOfferDataDlg.cpp
//	Introduce:Class CBMOfferDataDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferDataDlg.h"
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CBMOfferDataDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferDataDlg, CDialog)
CBMOfferDataDlg::CBMOfferDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferDataDlg::IDD, pParent)
{

}
//-----------------------------------------------------------------------
CBMOfferDataDlg::~CBMOfferDataDlg()
{
	::DestroyWindow(m_hWnd);
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COMPONENTS, m_lstComponents);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferDataDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOM, &CBMOfferDataDlg::OnCbnSelchangeComboCustom)
	ON_BN_CLICKED(IDC_BTN_ADD, &CBMOfferDataDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CBMOfferDataDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_YES, &CBMOfferDataDlg::OnBnClickedBtnYes)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CBMOfferDataDlg::OnBnClickedBtnCancel)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferDataDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitComponentLstCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if (FALSE == bShow)
		return;

	CenterWindow();
	FillCustomComboBox();
	FillContactComboBox();
	FillStatusComboBox();
	FillComponentTypeComboBox();

	// Initliaze data for default
	OnBnClickedBtnCancel();
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::OnClose()
{
	m_bIsEditOpt = FALSE;
	m_strInitOfferID = _T("");

	CDialog::OnClose();
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::OnDestroy()
{
	CDialog::OnDestroy();
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::OnCbnSelchangeComboCustom()
{
	FillContactComboBox();
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::OnBnClickedBtnAdd()
{
	int iComCnt = m_lstComponents.GetItemCount();
	if (iComCnt >= MAX_COMPONENT_CNT)
	{
		myMessageBox(IDS_COMPONENTDLG_ISMAXCNT, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);		
		return;
	}

	CString strNewCom;
	GetDlgItemText(IDC_EDIT_COMNAME, strNewCom);
	if (strNewCom.IsEmpty())
	{
		myMessageBox(IDS_COMPONENTDLG_NAMEISNULL, IDS_DLGMSG_ERROR, MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_COMNAME)->SetFocus();
		return;
	}

	BOOL bHasSameName = FALSE;
	DWORD dwItemCnt = m_lstComponents.GetItemCount();
	for (DWORD dwIndex = 0; dwIndex < dwItemCnt; dwIndex++)
	{
		CString strName = m_lstComponents.GetItemText(dwIndex, 1);
		if (strName == strNewCom)
		{
			bHasSameName = TRUE;
			break;
		}
	}
	if (TRUE == bHasSameName)
	{
		myMessageBox(IDS_COMPONENTDLG_NAMEISSAME, IDS_DLGMSG_ERROR, MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_COMNAME)->SetFocus();
		return;
	}


	CString strID;
	CString strType;
	CString strNote;
	strID.Format(_T("%d"), dwItemCnt+1);
	GetDlgItemText(IDC_COMBO_COMTYPE, strType);
	GetDlgItemText(IDC_EDIT_COMNOTE, strNote);
	m_lstComponents.InsertItem(dwItemCnt, strID);
	m_lstComponents.SetItemText(dwItemCnt, 1, strNewCom);
	m_lstComponents.SetItemText(dwItemCnt, 2, strType);
	m_lstComponents.SetItemText(dwItemCnt, 3, strNote);
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::OnBnClickedBtnDelete()
{
	int iSelItem = -1;
	POSITION pos = m_lstComponents.GetFirstSelectedItemPosition();
	iSelItem = m_lstComponents.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{		
		myMessageBox(IDS_DELETE_NOITEMSELECTED, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}
	//
	int iItemCnt = m_lstComponents.GetItemCount();
	if (1 == iItemCnt)
	{
		if (IDYES != myMessageBox(IDS_DELETELAST_CONFIRM, IDS_DLGMSG_WARNING, MB_YESNO | MB_ICONQUESTION))
			return;
	}
	m_lstComponents.DeleteItem(iSelItem);
	m_lstComponents.UpdateWindow();
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::OnBnClickedBtnYes()
{
	if (NULL == g_pOfferDatabase)
		return;

	// Get new data from UI
	CTime tmDate;
	CString strOfferNo;
	CString strProjectName;
	CString strCompanyName;
	CString strContactName;
	CString strEmplayeeName;
	CString strOfferState;
	CString strComment;
	DWORD dwComponentCnt = 0;
	GetDlgItemText(IDC_EDIT_NO, strOfferNo);
	GetDlgItemText(IDC_EDIT_PRONAME, strProjectName);
	GetDlgItemText(IDC_COMBO_CUSTOM, strCompanyName);
	GetDlgItemText(IDC_COMBO_CONTACT, strContactName);
	GetDlgItemText(IDC_EDIT_EMPLOYEE, strEmplayeeName);
	GetDlgItemText(IDC_COMBO_OFFERSTATUS, strOfferState);
	GetDlgItemText(IDC_EDIT_OFFERCOMMENT, strComment);
	dwComponentCnt = m_lstComponents.GetItemCount();
	//
	CDateTimeCtrl* pDateDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATE_OFFER);
	pDateDateCtrl->GetTime(tmDate);
	//
	if (strOfferNo.IsEmpty())
	{		
		myMessageBox(IDS_OFFERDATADLG_OFFERIDISNULL, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_NO)->SetFocus();
		return;
	}
	if (strEmplayeeName.IsEmpty())
	{		
		myMessageBox(IDS_OFFERDATADLG_EMPLOYEEISNULL, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_EMPLOYEE)->SetFocus();
		return;
	}
	if (strProjectName.IsEmpty())
	{		
		myMessageBox(IDS_OFFERDATADLG_PROJECTNAMEISNULL, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_PRONAME)->SetFocus();
		return;
	}

	// Open OffersInfo table from database
	BOOL bResult = TRUE;
	CADOTable tbOffersInfo;
	CStringW strOpenOperation;
	if (TRUE == m_bIsEditOpt)
		strOpenOperation.Format(L"SELECT * FROM %s WHERE OfferNo='%s'", g_tcTableName[eTableID_OffersInfo], m_strInitOfferID);
	else
		strOpenOperation.Format(L"SELECT * FROM %s", g_tcTableName[eTableID_OffersInfo]);
	bResult = tbOffersInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	// For "Add" operation, add a new record and set 'OfferNo' value
	if (FALSE == m_bIsEditOpt)
	{
		tbOffersInfo.AddNewItem();
		tbOffersInfo.SetRecordCell(-1, L"OfferNo", strOfferNo.GetBuffer(64));
	}
	tbOffersInfo.SetRecordCell(-1, _T("CompanyShortName"),	strCompanyName.GetBuffer(64));
	tbOffersInfo.SetRecordCell(-1, _T("ContactName"),		strContactName.GetBuffer(64));
	tbOffersInfo.SetRecordCell(-1, _T("ProjectName"),		strProjectName.GetBuffer(64));
	tbOffersInfo.SetRecordCell(-1, _T("ComponentNum"),		dwComponentCnt);
	tbOffersInfo.SetRecordCell(-1, _T("Status"),			strOfferState.GetBuffer(64));
	tbOffersInfo.SetRecordCell(-1, _T("Employee"),			strEmplayeeName.GetBuffer(64));
	tbOffersInfo.SetRecordCell(-1, _T("Date"),				tmDate);
	tbOffersInfo.SetRecordCell(-1, _T("Comment"),			strComment.GetBuffer(256));
	if (FALSE == tbOffersInfo.UpdateTable())
	{
		if (TRUE == m_bIsEditOpt) myMessageBox(IDS_MSGDLG_EDITFAILED, IDS_MSGDLG_FAILED, MB_OK | MB_ICONERROR);
		else  myMessageBox(IDS_MSGDLG_ADDFAILED, IDS_MSGDLG_FAILED, MB_OK | MB_ICONERROR);
		return;
	}
	tbOffersInfo.CloseTable();

	// Update component information to ComponentInfo table
	CADOTable tbComponentsInfo;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE OfferNo='%s'", g_tcTableName[eTableID_ComponentInfo], m_strInitOfferID);
	bResult = tbComponentsInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (TRUE == bResult)
	{
		int iComItemCnt = m_lstComponents.GetItemCount();

		// If a component name exists in database but doesn't in UI list, remove it from database!
		for (int iRecord = 0; iRecord < (int)tbComponentsInfo.GetRecordCnt(); iRecord++)
		{
			// Set new OfferNO
			tbComponentsInfo.SetRecordCell(iRecord, L"OfferNo", strOfferNo.GetBuffer(64));

			CString strRecComName;
			BOOL bIsInList = FALSE;
			tbComponentsInfo.GetRecordCell(iRecord, L"ComponentName", strRecComName.GetBuffer(64), 64);
			for (int iIndex = 0; iIndex < iComItemCnt; iIndex++)
			{
				CString strItemComName = m_lstComponents.GetItemText(iIndex, 1);
				if (strRecComName == strItemComName)
				{
					bIsInList = TRUE;
					break;
				}
			}
			if (FALSE == bIsInList) 
			{
				tbComponentsInfo.DeleteItem(iRecord);
				iRecord--;
			}
		}
		tbComponentsInfo.UpdateTable();
		
		// If a component name exists in UI list but doesn't in database, add it to database!
		for (int iIndex = 0; iIndex < iComItemCnt; iIndex++)
		{
			BOOL bIsInDatabse = FALSE;
			CString strItemComName = m_lstComponents.GetItemText(iIndex, 1);
			for (int iRecord = 0; iRecord < (int)tbComponentsInfo.GetRecordCnt(); iRecord++)
			{
				CString strRecComName;
				tbComponentsInfo.GetRecordCell(iRecord, L"ComponentName", strRecComName.GetBuffer(64), 64);
				if (strRecComName == strItemComName)
				{
					bIsInDatabse = TRUE;
					break;
				}
			}
			if (FALSE == bIsInDatabse)
			{
				CString strComType = m_lstComponents.GetItemText(iIndex, 2);
				CString strComNote = m_lstComponents.GetItemText(iIndex, 4);
				tbComponentsInfo.AddNewItem();
				tbComponentsInfo.SetRecordCell(-1, L"OfferNo", strOfferNo.GetBuffer(64));
				tbComponentsInfo.SetRecordCell(-1, L"ComponentName", strItemComName.GetBuffer(64));
				tbComponentsInfo.SetRecordCell(-1, L"ComponentType", strComType.GetBuffer(32));
				tbComponentsInfo.SetRecordCell(-1, L"ComponentPrice", (DWORD)0);
				tbComponentsInfo.SetRecordCell(-1, L"ComponentNote", strComNote.GetBuffer(64));
			}
		}
		tbComponentsInfo.UpdateTable();
		tbComponentsInfo.CloseTable();
	}

	// Tell main dailog to update data list
	::PostMessage(m_pParentWnd->m_hWnd, WM_USER_DATAISUPDATED, 0, 0);

	if (TRUE == m_bIsEditOpt) myMessageBox(IDS_MSGDLG_EDITSUCCESS, IDS_MSGDLG_SUCCESS, MB_OK | MB_ICONINFORMATION);
	else  myMessageBox(IDS_MSGDLG_ADDSUCCESS, IDS_MSGDLG_SUCCESS, MB_OK | MB_ICONINFORMATION);
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::OnBnClickedBtnCancel()
{
	if (TRUE == m_bIsEditOpt)
	{
		InitDataFromDatabase();
	}
	else
	{
		SetDlgItemText(IDC_EDIT_NO, m_strInitOfferID);
		SetDlgItemText(IDC_EDIT_PRONAME, _T(""));
		SetDlgItemText(IDC_EDIT_EMPLOYEE, g_LoginUserInfo.C31);
		SetDlgItemText(IDC_EDIT_OFFERCOMMENT, _T(""));
		((CComboBox*)GetDlgItem(IDC_COMBO_CUSTOM))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_COMBO_CONTACT))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_COMBO_OFFERSTATUS))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_COMBO_COMTYPE))->SetCurSel(0);
		//
		CTime tmDate = CTime::GetCurrentTime();
		CDateTimeCtrl* pDateDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATE_OFFER);
		pDateDateCtrl->SetTime(&tmDate);
		//
		m_lstComponents.DeleteAllItems();
	}
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::SetOfferID(CString strOfferID, BOOL bIsEdit)
{
	m_bIsEditOpt = bIsEdit;
	m_strInitOfferID = strOfferID;
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void CBMOfferDataDlg::FillCustomComboBox()
{
	CComboBox* pCompanyComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CUSTOM);
	pCompanyComboBox->ResetContent();
	m_mapCompanyNo.clear();

	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s", g_tcTableName[eTableID_CustomerInfo]);
	
	BOOL bResult = TRUE;
	CADOTable tbCustomInfo;
	bResult = tbCustomInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	DWORD dwRecordCnt = tbCustomInfo.GetRecordCnt();
	for (DWORD dwIndex = 0; dwIndex < dwRecordCnt; dwIndex++)
	{
		CString strCompanyID;
		CString strCompanyShortName;
		tbCustomInfo.GetRecordCell(dwIndex, _T("CompanyNo"), strCompanyID.GetBuffer(32), 32);
		tbCustomInfo.GetRecordCell(dwIndex, _T("CompanyShortName"), strCompanyShortName.GetBuffer(64), 64);
		pCompanyComboBox->AddString(strCompanyShortName);
		m_mapCompanyNo[(int)dwIndex] = strCompanyID;
	}
	tbCustomInfo.CloseTable();
	if (dwRecordCnt > 0) pCompanyComboBox->SetCurSel(0);
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::FillContactComboBox()
{
	CComboBox* pCompanyComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CUSTOM);
	CComboBox* pContactComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CONTACT);
	pContactComboBox->ResetContent();

	CString strCompanyNo;
	int iSelCompany = pCompanyComboBox->GetCurSel();
	strCompanyNo = m_mapCompanyNo[iSelCompany];

	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE CompanyNo='%s'", g_tcTableName[eTableID_ContactsInfo], strCompanyNo);
	
	BOOL bResult = TRUE;
	CADOTable tbContactInfo;
	bResult = tbContactInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	DWORD dwRecordCnt = tbContactInfo.GetRecordCnt();
	for (DWORD dwIndex = 0; dwIndex < dwRecordCnt; dwIndex++)
	{
		CString strContactName;
		tbContactInfo.GetRecordCell(dwIndex, _T("ContactName"), strContactName.GetBuffer(64), 64);
		int iIndex = pContactComboBox->AddString(strContactName);
	}
	tbContactInfo.CloseTable();
	if (dwRecordCnt > 0) pContactComboBox->SetCurSel(0);
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::FillStatusComboBox()
{
	CComboBox* pStatusComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_OFFERSTATUS);
	pStatusComboBox->ResetContent();
	for (int iStatus = 0; iStatus < eOfferStatus_Cnt; iStatus++)
	{
		int iIndex = pStatusComboBox->AddString(GetOfferStatusText((eOfferStatus)iStatus));
		pStatusComboBox->SetItemData(iIndex, iStatus);
	}
	pStatusComboBox->SetCurSel(0);
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::FillComponentTypeComboBox()
{	
	CComboBox* pComTypeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_COMTYPE);
	pComTypeComboBox->ResetContent();
	pComTypeComboBox->AddString(STR_COMTYPE_HOIST);
	pComTypeComboBox->AddString(STR_COMTYPE_PACKAGE);
	pComTypeComboBox->SetCurSel(0);
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::InitComponentLstCtrl()
{
	m_lstComponents.SetExtendedStyle(m_lstComponents.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstComponents.SetFullItemBKColor(RGB(214, 231, 255), RGB(94, 231, 221));
	m_lstComponents.SetImageList(&m_imgNULL, LVSIL_SMALL);
	//
	m_lstComponents.InsertColumn(0, myGetText(IDS_RECORD_INDEX), 0, 80);
	m_lstComponents.InsertColumn(1, myGetText(IDS_DETAILLIST_NAME), 0, 150);
	m_lstComponents.InsertColumn(2, myGetText(IDS_DETAILLIST_TYPE), 0, 100);
	m_lstComponents.InsertColumn(3, myGetText(IDS_DETAILLIST_NOTE), 0, 100);
}
//-----------------------------------------------------------------------
void CBMOfferDataDlg::InitDataFromDatabase()
{
	if (NULL == g_pOfferDatabase)
		return;
	
	// Open OfferInfo table from database
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE OfferNo='%s'", g_tcTableName[eTableID_OffersInfo], m_strInitOfferID);
	//
	BOOL bResult = TRUE;
	CADOTable tbOfferInfo;
	bResult = tbOfferInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult || tbOfferInfo.GetRecordCnt() == 0)
		return;

	TCHAR tcTmp[128] = {_T("\0")};
	tbOfferInfo.GetRecordCell(0, L"OfferNo", tcTmp, 128);
	SetDlgItemText(IDC_EDIT_NO, tcTmp);
	//
	memset(tcTmp, 0, sizeof(TCHAR)*128);
	tbOfferInfo.GetRecordCell(0, L"ProjectName", tcTmp, 128);
	SetDlgItemText(IDC_EDIT_PRONAME, tcTmp);
	//
	memset(tcTmp, 0, sizeof(TCHAR)*128);
	tbOfferInfo.GetRecordCell(0, L"CompanyShortName", tcTmp, 128);
	SelectComboBoxItem((CComboBox*)GetDlgItem(IDC_COMBO_CUSTOM), tcTmp);
	FillContactComboBox();
	//
	memset(tcTmp, 0, sizeof(TCHAR)*128);
	tbOfferInfo.GetRecordCell(0, L"ContactName", tcTmp, 128);
	SelectComboBoxItem((CComboBox*)GetDlgItem(IDC_COMBO_CONTACT), tcTmp);
	//
	memset(tcTmp, 0, sizeof(TCHAR)*128);
	tbOfferInfo.GetRecordCell(0, L"Employee", tcTmp, 128);
	SetDlgItemText(IDC_EDIT_EMPLOYEE, tcTmp);
	//
	CTime tmDate;
	tbOfferInfo.GetRecordCell(0, L"Date", tmDate);
	CDateTimeCtrl* pDateDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATE_OFFER);
	pDateDateCtrl->SetTime(&tmDate);
	//
	memset(tcTmp, 0, sizeof(TCHAR)*128);
	tbOfferInfo.GetRecordCell(0, L"Status", tcTmp, 128);
	SelectComboBoxItem((CComboBox*)GetDlgItem(IDC_COMBO_OFFERSTATUS), tcTmp);
	//
	memset(tcTmp, 0, sizeof(TCHAR)*128);
	tbOfferInfo.GetRecordCell(0, L"Comment", tcTmp, 128);
	SetDlgItemText(IDC_EDIT_OFFERCOMMENT, tcTmp);
	tbOfferInfo.CloseTable();

	// Open "ComponentInfo" table from database
	m_lstComponents.DeleteAllItems();
	strOpenOperation.Format(L"SELECT * FROM %s WHERE OfferNo='%s'", g_tcTableName[eTableID_ComponentInfo], m_strInitOfferID);
	//
	CADOTable tbComponentInfo;
	bResult = tbComponentInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult || tbComponentInfo.GetRecordCnt() == 0)
		return;
	DWORD dwComponentCnt = tbComponentInfo.GetRecordCnt();
	for (DWORD dwIndex = 0; dwIndex < dwComponentCnt; dwIndex++)
	{
		CString strID;
		CString strComponent;
		CString strComponentType;
		CString strComponentNote;
		strID.Format(_T("%d"), dwIndex + 1);
		tbComponentInfo.GetRecordCell(dwIndex, L"ComponentName", strComponent.GetBuffer(64), 64);
		tbComponentInfo.GetRecordCell(dwIndex, L"ComponentType", strComponentType.GetBuffer(64), 64);
		tbComponentInfo.GetRecordCell(dwIndex, L"ComponentNote", strComponentNote.GetBuffer(128), 128);
		m_lstComponents.InsertItem(dwIndex, strID);
		m_lstComponents.SetItemText(dwIndex, 1, strComponent);
		m_lstComponents.SetItemText(dwIndex, 2, strComponentType);
		m_lstComponents.SetItemText(dwIndex, 3, strComponentNote);
	}
	tbComponentInfo.CloseTable();
}
//-----------------------------------------------------------------------