//-----------------------------------------------------------------------
//	File Name:BMCustomManagerDlg.cpp
//	Introduce:Class CBMCustomManagerDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMCustomManagerDlg.h"
#include "ExcelExporter.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMCustomManagerDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMCustomManagerDlg, CDialog)
CBMCustomManagerDlg::CBMCustomManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(/*CBMCustomManagerDlg::IDD, pParent*/)
{
	m_pParentWnd = pParent;
	m_pCustomDataDlg = NULL;
	m_pCopyrightImage = NULL;
}
//-----------------------------------------------------------------------
CBMCustomManagerDlg::~CBMCustomManagerDlg()
{
	DestroyWindow();
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CUSTOMER, m_lstCtlCustomers);
	DDX_Control(pDX, IDC_LIST_CONTACTS, m_lstCtlContacts);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMCustomManagerDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ADD, &CBMCustomManagerDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_EDIT, &CBMCustomManagerDlg::OnBnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CBMCustomManagerDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_RETURN, &CBMCustomManagerDlg::OnBnClickedBtnReturn)
	ON_MESSAGE(WM_USER_DATAISUPDATED, OnDataRecordIsUpdated)	
	ON_NOTIFY(NM_CLICK, IDC_LIST_CUSTOMER, &CBMCustomManagerDlg::OnNMClickListCustomer)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CBMCustomManagerDlg::OnBnClickedBtnExport)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMCustomManagerDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMCustomManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Adjust controls postion
	UpdateControlsPos();

	// Initiliaze tables data struct
	InitTablesDataStruct();

	// Load image
	m_pCopyrightImage = new CBitmap();
	BOOL b = m_pCopyrightImage->LoadBitmap(IDB_BITMAP_COPYRIGHT);	

	// Initiliaze lists
	m_imgNULL.Create(1, 18, TRUE | ILC_COLOR32, 1, 0); 
	InitDataListCtrls();

	// Open tables and show records
	OpenTableAndShowRecords(m_stuCustomerTable.strName);

	// Select the first item in default
	HRESULT hr = 0;
	m_lstCtlCustomers.SetFocus();
	m_lstCtlCustomers.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);	
	OnNMClickListCustomer(NULL, &hr);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (cx > 0 && cy > 0)
	{
		UpdateControlsPos();
	}
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	BITMAP bm;
	m_pCopyrightImage->GetBitmap(&bm);

	HDC hDCMem = CreateCompatibleDC(dc.m_hDC);
	int iOldDcBkMode = SetBkMode(dc.m_hDC, TRANSPARENT);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDCMem, (HBITMAP)m_pCopyrightImage->m_hObject);	
	
	StretchBlt(	dc.m_hDC,
				m_rcCopyrightImage.left, m_rcCopyrightImage.top, m_rcCopyrightImage.Width(), m_rcCopyrightImage.Height(),
				hDCMem,
				0, 0, bm.bmWidth, bm.bmHeight,
				SRCCOPY);
			
	SelectObject(hDCMem, hOldBitmap);
	SetBkMode(hDCMem, iOldDcBkMode);
	DeleteDC(hDCMem);
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::OnDestroy()
{
	CDialog::OnDestroy();
	m_tbCustomInfo.CloseTable();
	m_tbContactInfo.CloseTable();
	if (NULL != m_pCustomDataDlg)
	{
		delete m_pCustomDataDlg;
		m_pCustomDataDlg = NULL;
	}
	if (NULL != m_pCopyrightImage)
	{
		delete m_pCopyrightImage;
		m_pCopyrightImage = NULL;
	}
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::OnBnClickedBtnAdd()
{
	CString strNewCustomID = GetNewCustomerID();
	 
	if (NULL == m_pCustomDataDlg)
	{
		m_pCustomDataDlg = new CBMCustomDataDlg(this);
		m_pCustomDataDlg->Create(IDD_DIALOG_CUSTOMDATA, this);
	}
	m_pCustomDataDlg->SetCustomerID(strNewCustomID, FALSE);
	m_pCustomDataDlg->ShowWindow(SW_SHOW);
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::OnBnClickedBtnEdit()
{
	int iSelItem = -1;
	POSITION pos = m_lstCtlCustomers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlCustomers.GetNextSelectedItem(pos);
	if (/*NULL == pos || */iSelItem < 0)
	{
		myMessageBox(IDS_EDIT_NOITEMSELECTED, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}

	CString strCompanyNo = m_lstCtlCustomers.GetItemText(iSelItem, 0);
	if (NULL == m_pCustomDataDlg)
	{
		m_pCustomDataDlg = new CBMCustomDataDlg(this);
		m_pCustomDataDlg->Create(IDD_DIALOG_CUSTOMDATA, this);
	}
	m_pCustomDataDlg->SetCustomerID(strCompanyNo, TRUE);
	m_pCustomDataDlg->ShowWindow(SW_SHOW);
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::OnBnClickedBtnDelete()
{
	int iSelItem = -1;
	POSITION pos = m_lstCtlCustomers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlCustomers.GetNextSelectedItem(pos);
	if (/*NULL == pos || */iSelItem < 0)
	{
		myMessageBox(IDS_DELETE_NOITEMSELECTED, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}

	if (IDYES != myMessageBox(IDS_DELETE_CONFIRM, IDS_DLGMSG_WARNING, MB_YESNO | MB_ICONQUESTION))
		return;

	CString strCompanyID = m_lstCtlCustomers.GetItemText(iSelItem, 0);
	DWORD dwDeletedCompanyID = _ttol(strCompanyID);
	if (TRUE == m_tbCustomInfo.DeleteItem(iSelItem))
	{
		m_tbCustomInfo.UpdateTable();
		
		// Remove its contacts information from table "ContactsInfo"	
		DWORD dwRecordCnt = m_tbContactInfo.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwRecordCnt; dwIndex++)
		{
			m_tbContactInfo.DeleteItem(0);
		}
		m_tbContactInfo.UpdateTable();
		m_tbContactInfo.CloseTable();
		
		OpenTableAndShowRecords(m_stuCustomerTable.strName);
	}
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::OnBnClickedBtnExport()
{
	CString strExcelFile;
	CFileDialog dlgSave(FALSE, _T(".xlsx"), NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, _T("Excel Workbook(*.xls)|*.xls||"));
	if (IDOK == dlgSave.DoModal())
		strExcelFile = dlgSave.GetPathName();
	else
		return;

	BOOL bResult = CExcelExporter::SaveDataFromTable(strExcelFile, _T("CustomInfo"), m_stuCustomerTable, m_tbCustomInfo);
	if (TRUE == bResult)
	{
		CString strSQL;
		CADOTable tbContactInfo;
		strSQL.Format(_T("SELECT * FROM %s"), g_tcTableName[eTableID_ContactsInfo]);
		tbContactInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strSQL);
		bResult = CExcelExporter::SaveDataFromTable(strExcelFile, _T("ConstactInfo"), m_stuContactTable, tbContactInfo);
		tbContactInfo.CloseTable();
	}

	if (TRUE == bResult)
		myMessageBox(IDS_EXCEL_EXPORTOK, IDS_MSGDLG_SUCCESS, MB_OK | MB_ICONINFORMATION);
	else
		myMessageBox(IDS_EXCEL_EXPORTFAILED, IDS_MSGDLG_FAILED, MB_OK | MB_ICONERROR);
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::OnBnClickedBtnReturn()
{
	::PostMessage(m_pParentWnd->m_hWnd, WM_USER_SHOWOFFERINFO, 0, 0);
}
//-----------------------------------------------------------------------
HRESULT CBMCustomManagerDlg::OnDataRecordIsUpdated(WPARAM wParam, LPARAM lParam)
{
	// Save the old selected companyID
	int iSelItem = -1;
	CString strCompanyID;
	POSITION pos = m_lstCtlCustomers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlCustomers.GetNextSelectedItem(pos);
	if (iSelItem >= 0)
		strCompanyID = m_lstCtlCustomers.GetItemText(iSelItem, 0);

	// Open tables and show records
	OpenTableAndShowRecords(m_stuCustomerTable.strName);

	// Select the old selected item
	HRESULT hr = 0;
	if (!strCompanyID.IsEmpty())
	{
		for (int iIndex = 0; iIndex < m_lstCtlCustomers.GetItemCount(); iIndex++)
		{
			CString strItemText = m_lstCtlCustomers.GetItemText(iIndex, 0);
			if (strItemText == strCompanyID)
			{
				m_lstCtlCustomers.SetItemState(iIndex, LVIS_SELECTED, LVIS_SELECTED);
				break;
			}
		}
	}
	else if (m_lstCtlCustomers.GetItemCount() > 0)
	{
		m_lstCtlCustomers.SetItemState(m_lstCtlCustomers.GetItemCount()-1, LVIS_SELECTED, LVIS_SELECTED);
	}
	OnNMClickListCustomer(NULL, &hr);

	return 1;
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMCustomManagerDlg::UpdateControlsPos()
{
	const int MARGIN_X = 10;
	const int MARGIN_Y = 10;

	CRect rcClient;
	GetClientRect(&rcClient);

	// Option Group
	CRect rcOptGrp;
	int iOptGrpOffsetX = 0;
	int iOptGrpOffsetY = 0;
	CWnd* pWndOptGrp = GetDlgItem(IDC_STATIC_OPTGROUP);
	if (NULL != pWndOptGrp)
	{
		pWndOptGrp->GetWindowRect(&rcOptGrp);
		ScreenToClient(&rcOptGrp);
		int iHeight = rcOptGrp.Height();
		int iOldRight = rcOptGrp.right;
		int iOldBottom = rcOptGrp.bottom;
		rcOptGrp.left = MARGIN_X;
		rcOptGrp.right = rcClient.right - (MARGIN_X + 300);	//300 is copyright image width
		rcOptGrp.bottom = rcClient.bottom - 2*MARGIN_Y;
		rcOptGrp.top = rcOptGrp.bottom - iHeight;
		iOptGrpOffsetX = rcOptGrp.right - iOldRight;
		iOptGrpOffsetY = rcOptGrp.bottom - iOldBottom;
		pWndOptGrp->MoveWindow(&rcOptGrp);
	}
	// Options buttons	
	UINT nBtnIDs[] = {IDC_BTN_ADD, IDC_BTN_EDIT, IDC_BTN_DELETE, IDC_BTN_EXPORT, IDC_BTN_RETURN};
	for (int i = 0; i < sizeof(nBtnIDs) / sizeof(nBtnIDs[0]); i++)
	{
		CWnd* pBtn = GetDlgItem(nBtnIDs[i]);
		if(NULL != pBtn) 
		{	
			CRect rc;
			pBtn->GetWindowRect(&rc);
			ScreenToClient(&rc);
			rc.OffsetRect(iOptGrpOffsetX / 2, iOptGrpOffsetY);
			pBtn->MoveWindow(&rc);
		}
	}

	// Copyright image rectagle
	InvalidateRect(&m_rcCopyrightImage);
	m_rcCopyrightImage.left = rcOptGrp.right + MARGIN_X;
	m_rcCopyrightImage.right = rcClient.right - MARGIN_X;
	m_rcCopyrightImage.top = rcOptGrp.top + (rcOptGrp.Height() - 160) / 2;	//160 is copyright image height
	m_rcCopyrightImage.bottom = m_rcCopyrightImage.top + 160;
	InvalidateRect(&m_rcCopyrightImage);

	// Data list control group
	CRect rcLstGrp;
	CWnd* pWndLstGrp = GetDlgItem(IDC_STATIC_CUSTOM);
	if (NULL != pWndLstGrp)
	{
		rcLstGrp.left = rcOptGrp.left;
		rcLstGrp.right = rcClient.right - MARGIN_X;
		rcLstGrp.top = MARGIN_Y;
		rcLstGrp.bottom = rcOptGrp.top - MARGIN_Y;
		pWndLstGrp->MoveWindow(&rcLstGrp);
	}

	// Contacts Information list control
	CRect rcContactLst;
	if (NULL != m_lstCtlContacts.m_hWnd)
	{
		m_lstCtlContacts.GetWindowRect(&rcContactLst);
		int iHeight = rcContactLst.Height();
		rcContactLst.left = rcLstGrp.left + MARGIN_X;
		rcContactLst.right = rcLstGrp.right - MARGIN_X;
		rcContactLst.bottom = rcLstGrp.bottom - MARGIN_Y;
		rcContactLst.top = rcContactLst.bottom - iHeight;
		m_lstCtlContacts.MoveWindow(&rcContactLst);
	}

	// Contacts information Static Control
	CRect rcContactStatic;
	CWnd* pWndContactStatic = GetDlgItem(IDC_STATIC_CONTACTS);
	if (NULL != pWndContactStatic)
	{
		pWndContactStatic->GetWindowRect(&rcContactStatic);
		int iWidth = rcContactStatic.Width();
		int iHeight = rcContactStatic.Height();
		rcContactStatic.left = rcLstGrp.left + MARGIN_X;
		rcContactStatic.right = rcContactStatic.left + iWidth;
		rcContactStatic.bottom = rcContactLst.top - MARGIN_Y/2;
		rcContactStatic.top = rcContactStatic.bottom - iHeight;
		pWndContactStatic->MoveWindow(&rcContactStatic);
	}

	// Data list control
	CRect rcDataLst;
	if (NULL != m_lstCtlCustomers.m_hWnd)
	{
		rcDataLst.left = rcLstGrp.left + MARGIN_X;
		rcDataLst.right = rcLstGrp.right - MARGIN_X;
		rcDataLst.top = rcLstGrp.top + 2*MARGIN_Y;
		rcDataLst.bottom = rcContactStatic.top - MARGIN_Y;
		m_lstCtlCustomers.MoveWindow(&rcDataLst);
	}
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::InitDataListCtrls()
{
	m_lstCtlContacts.SetExtendedStyle(m_lstCtlCustomers.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstCtlContacts.SetFullItemBKColor(RGB(214, 231, 255), RGB(94, 231, 221));
	m_lstCtlContacts.SetImageList(&m_imgNULL, LVSIL_SMALL);
	InitDataListCtrl(&m_lstCtlContacts, m_stuContactTable);
	//
	m_lstCtlCustomers.SetExtendedStyle(m_lstCtlCustomers.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstCtlCustomers.SetFullItemBKColor(RGB(214, 231, 255), RGB(94, 231, 221));
	m_lstCtlCustomers.SetImageList(&m_imgNULL, LVSIL_SMALL);
	InitDataListCtrl(&m_lstCtlCustomers, m_stuCustomerTable);
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::InitTablesDataStruct()
{
	// "CustomInfo" table
	m_stuCustomerTable.eID = eTableID_CustomerInfo;
	m_stuCustomerTable.strName = g_tcTableName[eTableID_CustomerInfo];
	m_stuCustomerTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"CompanyNo",		IDS_CUSTOMLIST_COMPANYNO,	eTableDataType_String));
	m_stuCustomerTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"CompanyShortName", IDS_CUSTOMLIST_SHORTNAME,	eTableDataType_String));
	m_stuCustomerTable.lstColumns.push_back(TableColumn(FALSE, 60,  L"Level",			IDS_CUSTOMLIST_LEVEL,		eTableDataType_DWORD));
	m_stuCustomerTable.lstColumns.push_back(TableColumn(FALSE, 150, L"FullName",		IDS_CUSTOMLIST_FULLNAME,	eTableDataType_String));
	m_stuCustomerTable.lstColumns.push_back(TableColumn(FALSE, 100, L"Address",			IDS_CUSTOMLIST_ADDRESS,		eTableDataType_String));
	m_stuCustomerTable.lstColumns.push_back(TableColumn(FALSE, 60,  L"PostCode",		IDS_CUSTOMLIST_POSTCODE,	eTableDataType_DWORD));
	m_stuCustomerTable.lstColumns.push_back(TableColumn(FALSE, 100, L"Tel",				IDS_CUSTOMLIST_TEL,			eTableDataType_String));
	m_stuCustomerTable.lstColumns.push_back(TableColumn(FALSE, 100, L"Fox",				IDS_CUSTOMLIST_FOX,			eTableDataType_String));
	m_stuCustomerTable.lstColumns.push_back(TableColumn(FALSE, 100, L"Http",			IDS_CUSTOMLIST_HTTP,		eTableDataType_String));
	//		
	// "ContactInfo" table
	m_stuContactTable.eID = eTableID_ContactsInfo;
	m_stuContactTable.strName = g_tcTableName[eTableID_ContactsInfo];
	m_stuContactTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"CompanyNo",		IDS_CUSTOMLIST_COMPANYNO,	eTableDataType_String));
	m_stuContactTable.lstColumns.push_back(TableColumn(FALSE, 100,  L"ContactName",		IDS_CUSTOMDLG_CONTACTNAME,	eTableDataType_String));
	m_stuContactTable.lstColumns.push_back(TableColumn(FALSE, 100,  L"Position",		IDS_CUSTOMDLG_CONTACTPOS,	eTableDataType_String));
	m_stuContactTable.lstColumns.push_back(TableColumn(FALSE, 100,  L"Tel",				IDS_CUSTOMDLG_CONTACTTEL,	eTableDataType_String));
	m_stuContactTable.lstColumns.push_back(TableColumn(FALSE, 100,  L"CellPhone",		IDS_CUSTOMDLG_CONTACTCELL,	eTableDataType_String));
	m_stuContactTable.lstColumns.push_back(TableColumn(FALSE, 200, L"Email",			IDS_CUSTOMDLG_CONTACTEMAIL,	eTableDataType_String));
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::OpenTableAndShowRecords(CString strTableName, CString strQuery)
{
	CStringW strOpenOperation;
	if (strQuery.IsEmpty())
		strOpenOperation.Format(L"SELECT * FROM %s", strTableName);
	else
		strOpenOperation.Format(strQuery, strTableName);

	BOOL bResult = TRUE;
	m_tbCustomInfo.CloseTable();
	bResult = m_tbCustomInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	ShowTableRecords(&m_tbCustomInfo, &m_lstCtlCustomers, m_stuCustomerTable);
}
//-----------------------------------------------------------------------
CString	CBMCustomManagerDlg::GetNewCustomerID()
{
	CString strNewID = g_LoginUserInfo.C30;

	CADOTable tbCustomers;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT CompanyNo FROM %s WHERE CompanyNo LIKE '%s%s' ORDER BY CompanyNo", g_tcTableName[eTableID_CustomerInfo], g_LoginUserInfo.C30, _T("%"));
	BOOL bResult = tbCustomers.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return _T("\0");

	CString strLastID = _T("\0");
	TCHAR tcLastID[32] = _T("\0");
	DWORD dwTotalCnt = tbCustomers.GetRecordCnt();
	if (0 == dwTotalCnt) strLastID = _T("000");		// 3 numbers
	else tbCustomers.GetRecordCell(dwTotalCnt - 1, L"CompanyNo", tcLastID, 32);
	tbCustomers.CloseTable();
	strLastID = tcLastID;

	LONG lNumber = 0;
	CString strNumber = strLastID.Right(3);			//3 numbers
	lNumber = _ttol(strNumber);
	lNumber++;
	strNumber.Format(_T("%03d"), lNumber);
	strNewID += strNumber;

	return strNewID;
}
//-----------------------------------------------------------------------
void CBMCustomManagerDlg::OnNMClickListCustomer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int iSelItem = -1;
	POSITION pos = m_lstCtlCustomers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlCustomers.GetNextSelectedItem(pos);
	if (iSelItem < 0)
		return;

	CStringW strOpenOperation;
	CString strCompanyID = m_lstCtlCustomers.GetItemText(iSelItem, 0);
	strOpenOperation.Format(L"SELECT * FROM %s WHERE CompanyNo='%s'", g_tcTableName[eTableID_ContactsInfo], strCompanyID);

	BOOL bResult = TRUE;
	m_tbContactInfo.CloseTable();
	bResult = m_tbContactInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	ShowTableRecords(&m_tbContactInfo, &m_lstCtlContacts, m_stuContactTable);

	*pResult = 0;
}
//-----------------------------------------------------------------------