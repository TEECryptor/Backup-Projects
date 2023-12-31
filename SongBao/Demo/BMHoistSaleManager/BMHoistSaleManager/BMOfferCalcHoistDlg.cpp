//-----------------------------------------------------------------------
//	File Name:BMOfferCalcHoistDlg.cpp
//	Introduce:implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcHoistDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcHoistDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferCalcHoistDlg, CDialog)

CBMOfferCalcHoistDlg::CBMOfferCalcHoistDlg(CWnd* pParent /*=NULL*/)
	: CBMCalcOptionsDlg(CBMOfferCalcHoistDlg::IDD, pParent)
{
	m_iSelHoistPrice = 0;
	m_pHoistLayoutDlg = NULL;
}
//-----------------------------------------------------------------------
CBMOfferCalcHoistDlg::~CBMOfferCalcHoistDlg()
{
	if (NULL != m_pHoistLayoutDlg)
	{
		delete m_pHoistLayoutDlg;
		m_pHoistLayoutDlg = NULL;
	}

	m_tbHoistSel.CloseTable();
	DestroyWindow();
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HOIST, m_lstCtlHoists);
	DDX_Control(pDX, IDC_LIST_ADDITION, m_lstCtlAdditions);	
	DDX_Control(pDX, IDC_STATIC_HOISTCODE, m_staticHoistCode);
	DDX_Control(pDX, IDC_STATIC_HOISTPRI, m_staticHoistPrice);
	DDX_Control(pDX, IDC_STATIC_ADDPRI, m_staticAddPrice);
	DDX_Control(pDX, IDC_STATIC_UNITPRI, m_staticUnitPrice);
	DDX_Control(pDX, IDC_STATIC_AMOUNT, m_staticCount);	
	DDX_Control(pDX, IDC_STATIC_TOTALPRI, m_staticTotalPrice);
	DDX_Control(pDX, IDC_TAB_LAYOUT, m_tabHoistLayout);	
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferCalcHoistDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CBMOfferCalcHoistDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CBMOfferCalcHoistDlg::OnBnClickedBtnClear)
	ON_NOTIFY(NM_CLICK, IDC_LIST_HOIST, &CBMOfferCalcHoistDlg::OnNMClickListHoist)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LAYOUT, &CBMOfferCalcHoistDlg::OnTcnSelchangeTabLayout)
//	ON_EN_CHANGE(IDC_EDIT_AMOUNT, &CBMOfferCalcHoistDlg::OnEnChangeEditAmount)
	ON_BN_CLICKED(IDC_BTN_ADD, &CBMOfferCalcHoistDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CBMOfferCalcHoistDlg::OnBnClickedBtnDel)
	ON_CBN_SELCHANGE(IDC_COMBO_REEVINGTYPE, &CBMOfferCalcHoistDlg::OnCbnSelchangeComboReevingtype)
	ON_CBN_SELCHANGE(IDC_COMBO_STOCK, &CBMOfferCalcHoistDlg::OnCbnSelchangeComboStock)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
BOOL CBMOfferCalcHoistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_eHoistType = eCraneType_GTD;

	// Initiliaze database table struction
	m_imgNULL.Create(1, 18, TRUE | ILC_COLOR32, 1, 0); 
	InitTablesDataStruct();
	InitDataLstControls();

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_REEVINGTYPE);
	pComboBox->ResetContent();
	int iIndex = pComboBox->AddString(_T("     All"));
	pComboBox->SetItemData(iIndex, eReevingType_All);
	iIndex = pComboBox->AddString(_T("     True"));
	pComboBox->SetItemData(iIndex, eReevingType_True);
	iIndex = pComboBox->AddString(_T("     Center"));
	pComboBox->SetItemData(iIndex, eReevingType_Center);	
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_eReevingType);	
	
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STOCK);
	pComboBox->ResetContent();
	iIndex = pComboBox->AddString(_T("     All"));
	pComboBox->SetItemData(iIndex, 0);
	iIndex = pComboBox->AddString(_T("     Y"));
	pComboBox->SetItemData(iIndex, int(_T('Y')));
	iIndex = pComboBox->AddString(_T("     N"));
	pComboBox->SetItemData(iIndex, int(_T('N')));
	pComboBox->SetCurSel(0);

	CRect rcTab;
	GetDlgItem(IDC_STATIC_LAYOUT)->GetWindowRect(&rcTab);
	ScreenToClient(&rcTab);
	CFont* pDlgFont = GetFont();
	m_tabHoistLayout.SetBkColor(RGB(239, 235, 222));
	m_tabHoistLayout.SetFont(pDlgFont);
	m_tabHoistLayout.InsertItem(0, _T("Main View"));
	m_tabHoistLayout.InsertItem(1, _T("Top View"));
	m_tabHoistLayout.InsertItem(2, _T("Side View"));
	m_tabHoistLayout.MoveWindow(&rcTab);
	m_tabHoistLayout.UpdateWindow();
	//
	CRect rcTabCtrl;
	m_tabHoistLayout.GetClientRect(&rcTabCtrl);
	rcTabCtrl.left += 2;
	rcTabCtrl.right -= 2;
	rcTabCtrl.top += 22;
	rcTabCtrl.bottom -= 2;
	m_pHoistLayoutDlg = new CBMHoistLayotsDlg(&m_tabHoistLayout);
	m_pHoistLayoutDlg->SetCraneParamObject(m_pParamCalculator);
	m_pHoistLayoutDlg->Create(IDD_DIALOG_HOISTLAYOT, &m_tabHoistLayout);
	m_pHoistLayoutDlg->MoveWindow(&rcTabCtrl);
	m_pHoistLayoutDlg->ShowWindow(SW_SHOW);
	m_pHoistLayoutDlg->UpdateWindow();
	//
	LRESULT lResult = 0;
	m_tabHoistLayout.SetCurSel(0);
	OnTcnSelchangeTabLayout(NULL, &lResult);

	m_staticHoistCode.SetColors(COLOR_PRICE);
	m_staticHoistPrice.SetColors(COLOR_PRICE);
	m_staticAddPrice.SetColors(COLOR_PRICE);
	m_staticUnitPrice.SetColors(COLOR_PRICE);
	m_staticCount.SetColors(COLOR_PRICE);
	m_staticTotalPrice.SetColors(COLOR_PRICE);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);	

	if (FALSE == bShow)
		return;
	
	// Initiliaze database table struction
	if (m_eHoistType != m_pParamCalculator->m_eCraneType)
	{
		m_eHoistType = m_pParamCalculator->m_eCraneType;
		InitTablesDataStruct();
		InitDataLstControls();		
		FillDefaultStdAddtions();
	}

	// Set initliaze data to UI;
	if (FALSE == m_bDataIsInitliazed)
	{
		/*if (m_pParamCalculator->m_bIsEditData) InitUIWithDatabaseData();
		else*/ InitUIWithDefaultData();
		m_bDataIsInitliazed = TRUE;
	}

	switch(m_pParamCalculator->m_eCraneType)
	{
	case eCraneType_GTD:			//
		SetDlgItemText(IDC_EDIT_HOISTTYPE, _T("BMD"));
		break;
	case eCraneType_GTS:			//
		SetDlgItemText(IDC_EDIT_HOISTTYPE, _T("BMS"));
		break;
	case eCraneType_GTF:			//
		SetDlgItemText(IDC_EDIT_HOISTTYPE, _T("BMF"));
		break;
	}

	CString strLoad;
	strLoad.Format(_T("%d"), m_pParamCalculator->m_iLoadOfHoist);
	SetDlgItemText(IDC_EDIT_LOAD, strLoad);
	SetDlgItemText(IDC_EDIT_HOISTDUTY, m_pParamCalculator->m_strDutyOfHoist);
	SetDlgItemText(IDC_EDIT_RAISESPEED, m_pParamCalculator->m_strHoistingSpeed);
	SetDlgItemText(IDC_EDIT_TROLLEYSPEED, m_pParamCalculator->m_strTrolleySpeed);	
	//
	CString strHOL;
	strHOL.Format(_T("%0.2f"), m_pParamCalculator->m_fHOL);	
	SetDlgItemText(IDC_EDIT_RAISEHEIGHT, strHOL);

	if (TRUE == m_pParamCalculator->m_bOnlyHoist)
	{
		m_staticCount.SetText(_T("1"));
	}
	else
	{
		CString strNumOfHoist;
		strNumOfHoist.Format(_T("%d"),  m_pParamCalculator->m_iNumOfHoist);
		m_staticCount.SetText(strNumOfHoist);
	}

	if (NULL != m_pHoistLayoutDlg)
	{
		m_pHoistLayoutDlg->SetCurrentViewType(eLayoutViewType_Main);
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnBnClickedBtnSearch()
{
	/* commented by Demo
	CString strQueryText = GetQuerySQLText();
	*/
	CString strQueryText;
	strQueryText.Format(_T("SELECT * FROM %s WHERE HoiTyeCod='BM441050P35ESB'"), m_stuHoistTable.strName);
	OpenAndShowHoistTable(strQueryText);

	HRESULT hr = S_OK;
	m_lstCtlHoists.SetFocus();
	m_lstCtlHoists.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);	
	OnNMClickListHoist(NULL, &hr);
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnBnClickedBtnClear()
{
	/* commented by Demo
	OpenAndShowHoistTable();
	*/
	m_lstCtlHoists.DeleteAllItems();	

	HRESULT hr = S_OK;
	m_lstCtlHoists.SetFocus();
	m_lstCtlHoists.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);	
	OnNMClickListHoist(NULL, &hr);
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnBnClickedBtnAdd()
{
	StringList lstSelAddNames;
	int iSelAddCnt = m_lstCtlAdditions.GetItemCount();
	for (int iIndex = 0; iIndex < iSelAddCnt; iIndex++)
	{
		CString strAddCode;
		strAddCode = m_lstCtlAdditions.GetItemText(iIndex, 1);
		lstSelAddNames.push_back(strAddCode);
	}

	StringList lstNewAddNames;
	CBMSelectAdditionsDlg dlg(eAdditionType_Hoist);
	dlg.SetSelAdditions(lstSelAddNames);
	if (dlg.DoModal() == IDOK)
	{
		int iAddGauageLevel = 0;
		dlg.GetSelAdditions(lstNewAddNames);
		m_lstCtlAdditions.DeleteAllItems();
		
		BOOL bResult = TRUE;
		CADOTable tbAdditions;	
		CStringW strOpenOperation;
		strOpenOperation.Format(L"SELECT * FROM %s", m_stuAdditionTable.strName);
		bResult = tbAdditions.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
		if (TRUE == bResult)
		{
			DWORD dwSelIndex = 0;
			DWORD dwAdditionCnt = tbAdditions.GetRecordCnt();
			for (DWORD dwIndex = 0; dwIndex < dwAdditionCnt; dwIndex++)
			{
				CString strAddCode;
				CString strAddComment;
				DWORD dwAddPrice = 0;
				TCHAR tcAddCode[64] = {0};
				tbAdditions.GetRecordCell(dwIndex, _T("Code"), tcAddCode, 64);
				tbAdditions.GetRecordCell(dwIndex, _T("Price"), dwAddPrice);
				tbAdditions.GetRecordCell(dwIndex, _T("Comment"), strAddComment.GetBuffer(64), 64);
				strAddCode = tcAddCode;
				if (strAddCode.IsEmpty()) continue;
				//
				for (StringList::iterator it = lstNewAddNames.begin();
					 it != lstNewAddNames.end();
					 it++)
				{
					if (strAddCode == *it)
					{
						CString strIndex;
						CString strPrice;
						strIndex.Format(_T("%d"), dwSelIndex+1);
						strPrice.Format(_T("%d"), dwAddPrice);
						m_lstCtlAdditions.InsertItem(dwSelIndex, strIndex);
						m_lstCtlAdditions.SetItemText(dwSelIndex, 1, strAddCode);
						m_lstCtlAdditions.SetItemText(dwSelIndex, 2, strPrice);
						m_lstCtlAdditions.SetItemText(dwSelIndex, 3, strAddComment);
						dwSelIndex++;
						//
						strAddCode.MakeLower();
						if (strAddCode.Find(STR_HOIST_ADDGAU) != -1)
						{
							CString strAddGauLevel = strAddCode.Right(2);
							iAddGauageLevel += _ttol(strAddGauLevel);
						}
						break;
					}
				}
			}
		}
		tbAdditions.CloseTable();

		UpdatePrices();
		UpdateAdditionTechData();
		UpdateRailGauageWithAdd(m_pParamCalculator->m_strRaiGauge, iAddGauageLevel);
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnBnClickedBtnDel()
{
	POSITION pos = m_lstCtlAdditions.GetFirstSelectedItemPosition();
	if (NULL == pos)
		return;

	int iAddGauageLevel = 0;
	while (pos)
	{
		int iSelItem = m_lstCtlAdditions.GetNextSelectedItem(pos);
		//
		CString strAddName = m_lstCtlAdditions.GetItemText(iSelItem, 1);
		m_lstCtlAdditions.DeleteItem(iSelItem);
		strAddName.MakeLower();
		if (strAddName.Find(STR_HOIST_ADDGAU) != -1)
		{
			CString strAddGauLevel = strAddName.Right(2);
			iAddGauageLevel -= _ttol(strAddGauLevel);
		}
	}
	UpdatePrices();
	UpdateAdditionTechData();
	UpdateRailGauageWithAdd(m_pParamCalculator->m_strRaiGauge, iAddGauageLevel);
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnNMClickListHoist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	int iSelItem = -1;
	POSITION pos = m_lstCtlHoists.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlHoists.GetNextSelectedItem(pos);
	if (iSelItem < 0)
		return;

	/* Get selected hoist data */
	CString strHostCode = m_lstCtlHoists.GetItemText(iSelItem, 1);
	CString strHostLoad = m_lstCtlHoists.GetItemText(iSelItem, 3);
	CString strHostHOL = m_lstCtlHoists.GetItemText(iSelItem, 5);
	CString strHostSpe = m_lstCtlHoists.GetItemText(iSelItem, 6);
	CString strHostDuty = m_lstCtlHoists.GetItemText(iSelItem, 7);
	CString strRaiGauge = m_lstCtlHoists.GetItemText(iSelItem, 9);
	CString strHostPrice = m_lstCtlHoists.GetItemText(iSelItem, 10);
	m_iSelHoistPrice = _ttoi(strHostPrice);
	UpdateRailGauageWithAdd(strRaiGauge);
	//
	m_pParamCalculator->m_strHoistCode = strHostCode;
	m_pParamCalculator->m_iHoistCap = _ttol(strHostLoad);
	m_pParamCalculator->m_fHoistHOL = (float)_tstof(strHostHOL);
	m_pParamCalculator->m_strHoistingSpeed = strHostSpe;
	m_pParamCalculator->m_strDutyOfHoist = strHostDuty;
	m_pParamCalculator->UpdateSelectCodes();
	UpdatePrices();

	/*	Update hoist data to UI */
	m_staticHoistCode.SetText(strHostCode);
	SetDlgItemText(IDC_EDIT_LOAD, strHostLoad);
	SetDlgItemText(IDC_EDIT_RAISEHEIGHT, strHostHOL);
	SetDlgItemText(IDC_EDIT_HOISTDUTY, m_pParamCalculator->m_strDutyOfHoist);
	SetDlgItemText(IDC_EDIT_RAISESPEED, m_pParamCalculator->m_strHoistingSpeed);

	/*	Set current view type, so that we can load selected hoist dim data */
	int iViewIndex = m_tabHoistLayout.GetCurSel();
	if (iViewIndex < eLayoutViewType_Cnt)
	{
		m_pHoistLayoutDlg->SetCurrentViewType((eLayoutViewType)iViewIndex);
	}

	*pResult = 0;
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnTcnSelchangeTabLayout(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iSelIndex = m_tabHoistLayout.GetCurSel();
	if (iSelIndex < eLayoutViewType_Cnt)
	{
		m_pHoistLayoutDlg->SetCurrentViewType((eLayoutViewType)iSelIndex);
	}
	else
	{
		m_pHoistLayoutDlg->ShowWindow(SW_HIDE);
	}

	*pResult = 0;
}
//-----------------------------------------------------------------------
/*
void CBMOfferCalcHoistDlg::OnEnChangeEditAmount()
{
	UpdatePrices();
}*/
//-----------------------------------------------------------------------







//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::InitTablesDataStruct()
{
	// "HoiSel" table
	switch (m_pParamCalculator->m_eCraneType)
	{
	case eCraneType_GTD:
		m_stuHoistTable.eID = eTableID_HoiSel_D;
		break;
	case eCraneType_GTS:
		m_stuHoistTable.eID = eTableID_HoiSel_S;
		break;
	case eCraneType_GTF:
		m_stuHoistTable.eID = eTableID_HoiSel_F;
		break;
	}
	m_stuHoistTable.strName = g_tcTableName[m_stuHoistTable.eID];
	m_stuHoistTable.lstColumns.clear();
	m_stuHoistTable.lstColumns.push_back(TableColumn(TRUE,  45,  L"Index",		IDS_RECORD_INDEX,			eTableDataType_DWORD));
	m_stuHoistTable.lstColumns.push_back(TableColumn(FALSE, 100, L"HoiTyeCod",	IDS_HOISTLIST_HOISTCODE,	eTableDataType_String));
	m_stuHoistTable.lstColumns.push_back(TableColumn(FALSE, 40,  L"DSF",		IDS_HOISTLIST_HOISTTYPE,	eTableDataType_String));
	m_stuHoistTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"HoiLod",		IDS_HOISTLIST_HOISTLOAD,	eTableDataType_DWORD));
	m_stuHoistTable.lstColumns.push_back(TableColumn(FALSE, 60,  L"Reeving",	IDS_HOISTLIST_REEVING,		eTableDataType_String));
	m_stuHoistTable.lstColumns.push_back(TableColumn(FALSE, 45,  L"HOL",		IDS_HOISTLIST_HOL,			eTableDataType_FLOAT));
	m_stuHoistTable.lstColumns.push_back(TableColumn(FALSE, 60,  L"HoiSpe",		IDS_HOISTLIST_HOLSPE,		eTableDataType_String));
	m_stuHoistTable.lstColumns.push_back(TableColumn(FALSE, 60,  L"DutCla",		IDS_HOISTLIST_DUTY,			eTableDataType_String));
	m_stuHoistTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"HoiMacMot",	IDS_HOISTLIST_MOTOR,		eTableDataType_String));
	m_stuHoistTable.lstColumns.push_back(TableColumn(FALSE, 55,  L"B0",			IDS_HOISTLIST_GAUAGE,		eTableDataType_String));
	m_stuHoistTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"PriLisHoi",	IDS_HOISTLIST_PRICE,		eTableDataType_DWORD));
	m_stuHoistTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"Stock",		IDS_HOISTLIST_STOCK,		eTableDataType_String));

	// "AdditionInfo" table
	m_stuAdditionTable.eID = eTableID_Additions;
	m_stuAdditionTable.strName = g_tcTableName[eTableID_Additions];
	m_stuAdditionTable.lstColumns.clear();
	m_stuAdditionTable.lstColumns.push_back(TableColumn(TRUE,  50,  L"Index",	IDS_RECORD_INDEX,			eTableDataType_DWORD));
	m_stuAdditionTable.lstColumns.push_back(TableColumn(FALSE, 100, L"Code",	IDS_ADDITIONLIST_NAME,		eTableDataType_String));
	m_stuAdditionTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"Price",	IDS_ADDITIONLIST_PRICE,		eTableDataType_DWORD));
	m_stuAdditionTable.lstColumns.push_back(TableColumn(FALSE, 150, L"Comment",	IDS_ADDITIONLIST_COMMENT,	eTableDataType_String));
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::InitDataLstControls()
{
	m_lstCtlHoists.SetExtendedStyle(m_lstCtlHoists.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_lstCtlHoists.SetFullItemBKColor(RGB(214, 231, 255), RGB(94, 231, 221));
	m_lstCtlHoists.SetImageList(&m_imgNULL, LVSIL_SMALL);
	m_lstCtlHoists.DeleteAllItems();
	InitDataListCtrl(&m_lstCtlHoists, m_stuHoistTable);
	//
	m_lstCtlAdditions.SetExtendedStyle(m_lstCtlAdditions.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_lstCtlAdditions.SetFullItemBKColor(RGB(214, 231, 255), RGB(94, 231, 221));
	m_lstCtlAdditions.SetImageList(&m_imgNULL, LVSIL_SMALL);
	m_lstCtlAdditions.DeleteAllItems();
	InitDataListCtrl(&m_lstCtlAdditions, m_stuAdditionTable);
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::InitUIWithDefaultData()
{
	FillDefaultStdAddtions();
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::InitUIWithDatabaseData()
{
	// Set Reeving type
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_REEVINGTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_eReevingType);	
	
	// Set additions list
	CString strSQL;
	CADOTable tbAddtions;
	strSQL.Format(_T("SELECT * FROM %s"), m_stuAdditionTable.strName);
	if (tbAddtions.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL))
	{
		m_lstCtlAdditions.DeleteAllItems();

		int iSelCount = 0;
		int iCount = tbAddtions.GetRecordCnt();
		for (int iIndex = 0; iIndex < iCount; iIndex++)
		{
			TCHAR tcAddName[128] = {0};
			tbAddtions.GetRecordCell(iIndex, L"Code", tcAddName, 128);

			BOOL bIsSelected = FALSE;
			for (list<CString>::iterator it = m_pParamCalculator->m_lstStdHoistAdditions.begin();
				it != m_pParamCalculator->m_lstStdHoistAdditions.end();
				it++)
			{	
				CString strName = *it;
				if (_tcsicmp(strName, tcAddName) == 0)
				{
					bIsSelected = TRUE;
					break;
				}
			}
			//
			if (FALSE == bIsSelected)
			{
				for (list<CString>::iterator it = m_pParamCalculator->m_lstOtherHoistAdditions.begin();
					it != m_pParamCalculator->m_lstOtherHoistAdditions.end();
					it++)
				{	
					CString strName = *it;
					if (_tcsicmp(strName, tcAddName) == 0)
					{
						bIsSelected = TRUE;
						break;
					}
				}
			}
			//
			if (TRUE == bIsSelected)
			{
				DWORD dwPrice = 0;
				TCHAR tcIndex[8] = {0};
				TCHAR tcPrice[32] = {0};
				TCHAR tcComment[256] = {0};
				_stprintf(tcIndex, _T("%d"), iSelCount+1);
				tbAddtions.GetRecordCell(iIndex, L"Price", dwPrice);
				tbAddtions.GetRecordCell(iIndex, L"Comment", tcComment, 256);
				_stprintf(tcPrice, _T("%d"), dwPrice);
				//
				m_lstCtlAdditions.InsertItem(iSelCount, tcIndex);
				m_lstCtlAdditions.SetItemText(iSelCount, 1, tcAddName);
				m_lstCtlAdditions.SetItemText(iSelCount, 2, tcPrice);
				m_lstCtlAdditions.SetItemText(iSelCount, 3, tcComment);
				iSelCount++;
			}
		}

		tbAddtions.CloseTable();
	}

	// Select the hoist
	HRESULT hr = 0;
	strSQL.Format(L"SELECT * FROM %s WHERE HoiTyeCod='%s'", m_stuHoistTable.strName, m_pParamCalculator->m_strHoistCode);
	OpenAndShowHoistTable(strSQL);
	m_lstCtlHoists.SetFocus();
	m_lstCtlHoists.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);	
	OnNMClickListHoist(NULL, &hr);
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OpenAndShowHoistTable(CString strQuery)
{
	CStringW strOpenOperation;
	if (strQuery.IsEmpty())
		strOpenOperation.Format(L"SELECT * FROM %s", m_stuHoistTable.strName);
	else
		strOpenOperation = strQuery;

	BOOL bResult = TRUE;
	m_tbHoistSel.CloseTable();
	bResult = m_tbHoistSel.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	ShowTableRecords(&m_tbHoistSel, &m_lstCtlHoists, m_stuHoistTable);

	CString strCountInfo;
	DWORD dwCount = m_tbHoistSel.GetRecordCnt();
	strCountInfo.Format(IDS_CALCHOIST_COUNTINFO, dwCount);
	SetDlgItemText(IDC_STATIC_RESULT, strCountInfo);
}
//-----------------------------------------------------------------------
CString	CBMOfferCalcHoistDlg::GetQuerySQLText(BOOL bIncludeRailGau, int iRailGua)
{
	CString strQuerySQL = _T("\0");

	CString strHoistType;
	switch(m_pParamCalculator->m_eCraneType)
	{
	case eCraneType_GTD:
		strHoistType = _T("D");
		break;
	case eCraneType_GTS:
		strHoistType = _T("S");
		break;
	case eCraneType_GTF:
		strHoistType = _T("F");
		break;
	default:
		strHoistType = _T("D");
		break;
	};
	
	// Reeving type
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_REEVINGTYPE);	
	int iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_eReevingType = (eReevingType)pComboBox->GetItemData(iSelIndex);

	// Stock type
	CString strStockType = _T("");	
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STOCK);
	iSelIndex = pComboBox->GetCurSel();
	int iItemData = pComboBox->GetItemData(iSelIndex);
	if (iItemData != 0)
	{
		strStockType.Format(_T("%c"), TCHAR(iItemData));
	}

	if (eReevingType_All == m_pParamCalculator->m_eReevingType)
	{
		if (strStockType.IsEmpty())
		{
			strQuerySQL.Format(_T("SELECT * FROM %s WHERE DSF='%s' AND HoiLod>=%d AND HoiSpe1>=%f AND DutCla1>=%d"),
				m_stuHoistTable.strName, strHoistType, m_pParamCalculator->m_iLoadOfHoist, m_pParamCalculator->m_fHoistSpeed, m_pParamCalculator->m_iDutyOfHoist);
		}
		else
		{			
			strQuerySQL.Format(_T("SELECT * FROM %s WHERE DSF='%s' AND HoiLod>=%d AND HoiSpe1>=%f AND DutCla1>=%d AND Stock='%s'"),
				m_stuHoistTable.strName, strHoistType, m_pParamCalculator->m_iLoadOfHoist, m_pParamCalculator->m_fHoistSpeed, m_pParamCalculator->m_iDutyOfHoist, strStockType);
		}
	}
	else
	{		
		if (strStockType.IsEmpty())
		{
			strQuerySQL.Format(_T("SELECT * FROM %s WHERE DSF='%s' AND HoiLod>=%d AND HoiSpe1>=%f AND Reeving LIKE '%s%d' AND DutCla1>=%d"),
				m_stuHoistTable.strName, strHoistType, m_pParamCalculator->m_iLoadOfHoist, m_pParamCalculator->m_fHoistSpeed, _T("%"), m_pParamCalculator->m_eReevingType, m_pParamCalculator->m_iDutyOfHoist);
		}
		else
		{			
			strQuerySQL.Format(_T("SELECT * FROM %s WHERE DSF='%s' AND HoiLod>=%d AND HoiSpe1>=%f AND Reeving LIKE '%s%d' AND DutCla1>=%d AND Stock='%s'"),
				m_stuHoistTable.strName, strHoistType, m_pParamCalculator->m_iLoadOfHoist, m_pParamCalculator->m_fHoistSpeed, _T("%"), m_pParamCalculator->m_eReevingType, m_pParamCalculator->m_iDutyOfHoist, strStockType);
		}
	}
	return strQuerySQL;
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::FillDefaultStdAddtions()
{
	/* Remove the default additions as BM's requestion
	CString strSQL;
	CADOTable tbAddtions;
	strSQL.Format(_T("SELECT * FROM %s WHERE Code LIKE '%s%s'"), m_stuAdditionTable.strName, STR_HOIST_STDADD, _T("%"));
	if (tbAddtions.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL))
	{
		ShowTableRecords(&tbAddtions, &m_lstCtlAdditions, m_stuAdditionTable);
		UpdateAdditionTechData();
		tbAddtions.CloseTable();
	}*/
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::UpdatePrices()
{
	// Selected hoist unit price
	CString strHostPrice;
	strHostPrice.Format(_T("%d RMB"), m_iSelHoistPrice);
	m_staticHoistPrice.SetText(strHostPrice);

	// Get all additions price
	DWORD dwTotalAddPrice = 0;
	int iSelAddCnt = m_lstCtlAdditions.GetItemCount();
	for (int iIndex = 0; iIndex < iSelAddCnt; iIndex++)
	{
		CString strPrice;
		strPrice = m_lstCtlAdditions.GetItemText(iIndex, 2);
		dwTotalAddPrice += _ttol(strPrice);
	}
	//
	CString strAddsPrice;
	strAddsPrice.Format(_T("%d RMB"), dwTotalAddPrice);
	m_staticAddPrice.SetText(strAddsPrice);
	
	// Unit price of Hoist+Additions
	CString strUnitPrice;
	strUnitPrice.Format(_T("%d RMB"), dwTotalAddPrice + m_iSelHoistPrice);
	m_staticUnitPrice.SetText(strUnitPrice);

	//	Total hoist price (Count * (hoist price + additions price))
	CString strCount;
	CString strTotalPrice;
	GetDlgItemText(IDC_STATIC_AMOUNT, strCount);
	m_pParamCalculator->m_iNumOfHoist = _ttol(strCount);
	m_pParamCalculator->m_iHoistPrice = (dwTotalAddPrice + m_iSelHoistPrice);
	strTotalPrice.Format(_T("%d RMB"), m_pParamCalculator->m_iNumOfHoist * m_pParamCalculator->m_iHoistPrice);
	m_staticTotalPrice.SetText(strTotalPrice);
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::UpdateAdditionTechData()
{
	m_pParamCalculator->m_lstStdHoistAdditions.clear();
	m_pParamCalculator->m_lstOtherHoistAdditions.clear();

	int iAddGauageLevel = 0;
	int iSelAddCnt = m_lstCtlAdditions.GetItemCount();
	for (int iIndex = 0; iIndex < iSelAddCnt; iIndex++)
	{
		CString strAddName;
		CString strAddComment;
		strAddName = m_lstCtlAdditions.GetItemText(iIndex, 1);
		strAddName.MakeLower();
		if (strAddName.Find(STR_HOIST_STDADD) != -1)
		{
			m_pParamCalculator->m_lstStdHoistAdditions.push_back(strAddName);
		}
		else
		{
			m_pParamCalculator->m_lstOtherHoistAdditions.push_back(strAddName);
		}
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::UpdateRailGauageWithAdd(CString strSelHoistGauage, int iAddGauageLevel)
{
	// For GTS, rail gauage is a TRUE string
	if (eCraneType_GTS == m_pParamCalculator->m_eCraneType)
	{
		m_pParamCalculator->m_strRaiGauge = strSelHoistGauage;
		SetDlgItemText(IDC_EDIT_RAILGAUGE, strSelHoistGauage);
		return;
	}

	// Get additions gauage level from list control
	if (iAddGauageLevel == 0)
	{
		int iCount = m_lstCtlAdditions.GetItemCount();
		for (int i = 0; i < iCount; i++)
		{
			CString strAddName = m_lstCtlAdditions.GetItemText(i, 1);
			strAddName.MakeLower();
			if (strAddName.Find(STR_HOIST_ADDGAU) != -1)
			{
				CString strAddGauLevel = strAddName.Right(2);
				iAddGauageLevel += _ttol(strAddGauLevel);
			}
		}
	}

	// Update Rail Gauage
	int iCurLevel = 0;
	int iSelHoistGauage = _ttol(strSelHoistGauage);
	for (int i = 0; i < sizeof(g_iRailGauage) / sizeof(g_iRailGauage[0]); i++)
	{
		if (iSelHoistGauage > g_iRailGauage[i])
			continue;
		iCurLevel = i;
		break;
	}
	int iNewLevel = iCurLevel + iAddGauageLevel;
	iNewLevel = min(iNewLevel, sizeof(g_iRailGauage) / sizeof(g_iRailGauage[0]) - 1);
	int iNewRailGauage = g_iRailGauage[iNewLevel];
	m_pParamCalculator->m_strRaiGauge.Format(_T("%d"), iNewRailGauage);
	SetDlgItemText(IDC_EDIT_RAILGAUGE, m_pParamCalculator->m_strRaiGauge);
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
BOOL CBMOfferCalcHoistDlg::CanDoNextStep()
{
	GetDlgItemText(IDC_STATIC_HOISTCODE, m_pParamCalculator->m_strHoistCode);
	if (m_pParamCalculator->m_strHoistCode.IsEmpty())
	{
		myMessageBox(IDS_CALC_HOISTCODEISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		return FALSE;
	}
	
	// Data from table "HoiMacIndCod"
	CString strSQL;
	CADOTable tbHoiMacIndCod;
	strSQL.Format(_T("SELECT * FROM %s WHERE HoiMacIndCod = '%s'"), g_tcTableName[eTableID_HoiMacIndCod], m_pParamCalculator->m_strHoiMacIndCode);
	if (tbHoiMacIndCod.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbHoiMacIndCod.GetRecordCnt() > 0)
	{
		TCHAR tcValue[32] = {0};
		tbHoiMacIndCod.GetRecordCell(0, _T("PowHoiMot"), tcValue, 32);	
		tbHoiMacIndCod.GetRecordCell(0, _T("PowHoiMotH"), m_pParamCalculator->m_fHoist_PowH);
		m_pParamCalculator->m_strHoist_Pow = tcValue;
		tbHoiMacIndCod.CloseTable();
	}

	// Data from table "TroDriTyp"
	CADOTable tbTroDriTyp;
	strSQL.Format(_T("SELECT * FROM %s WHERE HoiDimIndCod = '%s'"), g_tcTableName[eTableID_TroDriTyp], m_pParamCalculator->m_strHoiDimIndCode);
	if (tbTroDriTyp.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbTroDriTyp.GetRecordCnt() > 0)
	{
		DWORD dwTroDriNum = 1;
		TCHAR tcTroDriTyp[32] = {0};
		tbTroDriTyp.GetRecordCell(0, _T("TroDriTyp"), tcTroDriTyp, 32);		
		tbTroDriTyp.GetRecordCell(0, _T("TroDriNum"), dwTroDriNum);	
		tbTroDriTyp.CloseTable();
		//	
		m_pParamCalculator->m_strTroMachineryType = tcTroDriTyp;
		m_pParamCalculator->m_iTroDriNum = dwTroDriNum;
		//
		strSQL.Format(_T("SELECT * FROM %s WHERE TroDriTyp = '%s'"), g_tcTableName[eTableID_TroDriDat], m_pParamCalculator->m_strTroMachineryType);
		m_pParamCalculator->m_fHoist_DriPow = ::GetFLOATValueFromTable(strSQL, _T("PowTraDri"));
	}

	return TRUE;
};
//-----------------------------------------------------------------------






//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnCbnSelchangeComboReevingtype()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_REEVINGTYPE);
	SelectComboBoxItem(pComboBox, eReevingType_All);
}

void CBMOfferCalcHoistDlg::OnCbnSelchangeComboStock()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STOCK);
	SelectComboBoxItem(pComboBox, 0);
}
//-----------------------------------------------------------------------
