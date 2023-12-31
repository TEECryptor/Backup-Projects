//-----------------------------------------------------------------------
//	File Name:BMOfferCalcHoistDlg.cpp
//	Introduce:implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcHoistDlg.h"
#include "BMOfferCalcHoistAuxDlg.h"
#include "BMCalcParameterDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcHoistDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferCalcHoistDlg, CDialog)

CBMOfferCalcHoistDlg::CBMOfferCalcHoistDlg(CWnd* pParent /*=NULL*/)
	: CBMCalcOptionsDlg(CBMOfferCalcHoistDlg::IDD, pParent)
{
	//m_pHoistLayoutDlg = NULL;
}
//-----------------------------------------------------------------------
CBMOfferCalcHoistDlg::~CBMOfferCalcHoistDlg()
{/*
	if (NULL != m_pHoistLayoutDlg)
	{
		delete m_pHoistLayoutDlg;
		m_pHoistLayoutDlg = NULL;
	}
*/
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
	DDX_Control(pDX, IDC_STATIC_AUXHOISTCODE, m_staticAuxHoistCode);
	DDX_Control(pDX, IDC_STATIC_AUXHOISTPRI, m_staticAuxHoistPrice);
	DDX_Control(pDX, IDC_STATIC_ADDPRI, m_staticAddPrice);	
	DDX_Control(pDX, IDC_STATIC_UNITPRI, m_staticUnitPrice);
	DDX_Control(pDX, IDC_STATIC_AMOUNT, m_staticCount);	
	DDX_Control(pDX, IDC_STATIC_TOTALPRI, m_staticTotalPrice);
	//DDX_Control(pDX, IDC_TAB_LAYOUT, m_tabHoistLayout);	
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferCalcHoistDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CBMOfferCalcHoistDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CBMOfferCalcHoistDlg::OnBnClickedBtnClear)
	ON_NOTIFY(NM_CLICK, IDC_LIST_HOIST, &CBMOfferCalcHoistDlg::OnNMClickListHoist)
//	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LAYOUT, &CBMOfferCalcHoistDlg::OnTcnSelchangeTabLayout)
//	ON_EN_CHANGE(IDC_EDIT_AMOUNT, &CBMOfferCalcHoistDlg::OnEnChangeEditAmount)
	ON_BN_CLICKED(IDC_BTN_ADD, &CBMOfferCalcHoistDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CBMOfferCalcHoistDlg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_AUXHOIST, &CBMOfferCalcHoistDlg::OnBnClickedBtnAuxhoist)
	ON_BN_CLICKED(IDC_BTN_PARAMETER, &CBMOfferCalcHoistDlg::OnBnClickedBtnParameter)
	ON_CBN_SELCHANGE(IDC_COMBO_HOIST_BRAND, &CBMOfferCalcHoistDlg::OnCbnSelchangeComboHoistBrand)
	ON_CBN_SELCHANGE(IDC_COMBO_REEVINGTYPE, &CBMOfferCalcHoistDlg::OnCbnSelchangeComboReevingtype)
	ON_CBN_SELCHANGE(IDC_COMBO_STOCK, &CBMOfferCalcHoistDlg::OnCbnSelchangeComboStock)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
BOOL CBMOfferCalcHoistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strLastCraneType = _T("D");

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
	pComboBox->SetCurSel(0);	
	OnCbnSelchangeComboReevingtype();
	
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STOCK);
	pComboBox->ResetContent();
	iIndex = pComboBox->AddString(_T("     All"));
	pComboBox->SetItemData(iIndex, int(_T('A')));
	iIndex = pComboBox->AddString(_T("     Y"));
	pComboBox->SetItemData(iIndex, int(_T('Y')));
	iIndex = pComboBox->AddString(_T("     N"));
	pComboBox->SetItemData(iIndex, int(_T('N')));
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboStock();
/*
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
	OnTcnSelchangeTabLayout(NULL, &lResult);*/

	m_staticHoistCode.SetColors(COLOR_PRICE);
	m_staticHoistPrice.SetColors(COLOR_PRICE);
	m_staticAuxHoistCode.SetColors(COLOR_PRICE);
	m_staticAuxHoistPrice.SetColors(COLOR_PRICE);
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
	
	//Initiliaze hoist data based on gerenal data
	m_pParamCalculator->m_CalcDatalst.HA1.Format(_T("%s%s"), g_LoginUserInfo.C38, m_pParamCalculator->m_CalcDatalst.GA13);
	m_pParamCalculator->m_CalcDatalst.HA10 = (m_pParamCalculator->m_CalcDatalst.HA10.GetLength() == 0) ? g_LoginUserInfo.C38 : m_pParamCalculator->m_CalcDatalst.HA10;
	m_pParamCalculator->m_CalcDatalst.HA2 = m_pParamCalculator->m_CalcDatalst.GA8;
	m_pParamCalculator->m_CalcDatalst.HA3 = m_pParamCalculator->m_CalcDatalst.GA4;
	m_pParamCalculator->m_CalcDatalst.HA4 = m_pParamCalculator->m_CalcDatalst.GA6;
	m_pParamCalculator->m_CalcDatalst.HA5 = m_pParamCalculator->m_CalcDatalst.GC1;
	m_pParamCalculator->m_CalcDatalst.HA6 = m_pParamCalculator->m_CalcDatalst.GC2;
	m_pParamCalculator->m_CalcDatalst.HB8 = m_pParamCalculator->m_CalcDatalst.GA2;

	//Initiliaze brand combobox
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOIST_BRAND);
	pComboBox->ResetContent();
	int iIndex = pComboBox->AddString(_T("BM"));
	iIndex = pComboBox->AddString(_T("GT"));
	iIndex = pComboBox->AddString(_T("NS"));
	if (_T("D") == m_pParamCalculator->m_CalcDatalst.GA12)
	{
		iIndex = pComboBox->AddString(_T("NF"));
	}
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.HA10);
	OnCbnSelchangeComboHoistBrand();

	/* Disable Aux.Hoist for BMZ */
	if (_T("Z") == m_pParamCalculator->m_CalcDatalst.GA13)
	{
		GetDlgItem(IDC_BTN_AUXHOIST)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_AUXHOISTCODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_AUXHOISTPRI)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_HOIST_AUXHOISTCODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_HOIST_AUXHOISTPRI)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_AUXHOIST)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_AUXHOISTCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_AUXHOISTPRI)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_HOIST_AUXHOISTCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_HOIST_AUXHOISTPRI)->EnableWindow(FALSE);
	}
/*
	// Initiliaze database table struction
	if (m_strLastCraneType != m_pParamCalculator->m_CalcDatalst.GA12)
	{
		m_strLastCraneType = m_pParamCalculator->m_pParamCalculator->m_CalcDatalst.GA12;
		InitTablesDataStruct();
		InitDataLstControls();		
		FillDefaultStdAddtions();
	}
*/
	// Set initliaze data to UI;
	if (FALSE == m_bDataIsInitliazed)
	{
		if (m_pParamCalculator->m_bIsEditData) InitUIWithDatabaseData();
		else InitUIWithDefaultData();
		m_bDataIsInitliazed = TRUE;
	}

	// Show initiliaze hoist data
	CString strTmp;
	SetDlgItemText(IDC_EDIT_HOISTTYPE, m_pParamCalculator->m_CalcDatalst.HA1);
	strTmp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.HA3);
	SetDlgItemText(IDC_EDIT_LOAD, strTmp);
	SetDlgItemText(IDC_EDIT_RAISESPEED, m_pParamCalculator->m_CalcDatalst.HA5);
	SetDlgItemText(IDC_EDIT_HOISTDUTY, m_pParamCalculator->m_CalcDatalst.HA2);
	strTmp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.HA4);
	SetDlgItemText(IDC_EDIT_RAISEHEIGHT, strTmp);
	SetDlgItemText(IDC_EDIT_TROLLEYSPEED, m_pParamCalculator->m_CalcDatalst.HA6);

	if (TRUE == m_pParamCalculator->m_bOnlyHoist)
	{
		m_staticCount.SetText(_T("1"));
	}
	else
	{
		CString strNumOfHoist;
		strNumOfHoist.Format(_T("%d"),  m_pParamCalculator->m_CalcDatalst.HB8);
		m_staticCount.SetText(strNumOfHoist);
	}
/*
	if (NULL != m_pHoistLayoutDlg)
	{
		m_pHoistLayoutDlg->SetCurrentViewType(eLayoutViewType_Main);
	}*/
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnCbnSelchangeComboHoistBrand()
{
	GetDlgItemText(IDC_COMBO_HOIST_BRAND, m_pParamCalculator->m_CalcDatalst.HA10);
	if (_T("BM") == m_pParamCalculator->m_CalcDatalst.HA10)
		m_pParamCalculator->m_CalcDatalst.HA101 = _T("BM");
	else if (_T("GT") == m_pParamCalculator->m_CalcDatalst.HA10)
		m_pParamCalculator->m_CalcDatalst.HA101 = _T("BM");
	else if (_T("NS") == m_pParamCalculator->m_CalcDatalst.HA10)
		m_pParamCalculator->m_CalcDatalst.HA101 = _T("NS");
	else if (_T("NF") == m_pParamCalculator->m_CalcDatalst.HA10)
		m_pParamCalculator->m_CalcDatalst.HA101 = _T("NF");
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnCbnSelchangeComboReevingtype()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_REEVINGTYPE);
	int iIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.HA7 = pComboBox->GetItemData(iIndex);	
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnCbnSelchangeComboStock()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STOCK);
	int iIndex = pComboBox->GetCurSel();
	int iItemData = pComboBox->GetItemData(iIndex);	
	m_pParamCalculator->m_CalcDatalst.HA9.Format(_T("%c"), TCHAR(iItemData));
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnBnClickedBtnSearch()
{
	CString strQueryText = GetQuerySQLText();
	OpenAndShowHoistTable(strQueryText);
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnBnClickedBtnClear()
{
	OpenAndShowHoistTable();
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
		UpdateRailGauageWithAdd(m_pParamCalculator->m_CalcDatalst.HA8, /*iAddGauageLevel*/0);
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
	UpdateRailGauageWithAdd(m_pParamCalculator->m_CalcDatalst.HA8, /*iAddGauageLevel*/0);
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
	m_pParamCalculator->m_CalcDatalst.HB201 = m_lstCtlHoists.GetItemText(iSelItem, 1);
	m_pParamCalculator->m_CalcDatalst.HA8 = m_lstCtlHoists.GetItemText(iSelItem, 9);
	UpdateRailGauageWithAdd(m_pParamCalculator->m_CalcDatalst.HA8);
	m_pParamCalculator->ParserSelMainHoistData();

	UpdatePrices();

	/*	Update hoist data to UI */
	m_staticHoistCode.SetText(m_pParamCalculator->m_CalcDatalst.HB2);

	*pResult = 0;
}
//-----------------------------------------------------------------------
/*
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
}*/
//-----------------------------------------------------------------------
/*
void CBMOfferCalcHoistDlg::OnEnChangeEditAmount()
{
	UpdatePrices();
}*/
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnBnClickedBtnAuxhoist()
{
	CBMOfferCalcHoistAuxDlg dlg;
	dlg.SetParamCalculator(m_pParamCalculator);
	if (dlg.DoModal() == IDOK)
	{
		m_pParamCalculator->m_CalcDatalst.HB4 = m_pParamCalculator->m_CalcDatalst.AU4;
		m_pParamCalculator->m_CalcDatalst.HB5 = m_pParamCalculator->m_CalcDatalst.AU7;
		//
		m_staticAuxHoistCode.SetText(m_pParamCalculator->m_CalcDatalst.HB4);
		UpdatePrices();
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::OnBnClickedBtnParameter()
{
	CBMCalcParameterDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
	}
}
//-----------------------------------------------------------------------








//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::InitTablesDataStruct()
{
	// "HoiSel" table
	m_stuHoistTable.strName = GetHoistSelTableName(m_pParamCalculator->m_CalcDatalst.GA12, m_pParamCalculator->m_CalcDatalst.HA10);
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
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.HA7);	
	
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
	strSQL.Format(L"SELECT * FROM %s WHERE HoiTyeCod='%s'", GetCurHoistTableName(), m_pParamCalculator->m_CalcDatalst.HB201);
	OpenAndShowHoistTable(strSQL);
	m_lstCtlHoists.SetFocus();
	m_lstCtlHoists.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);	
	OnNMClickListHoist(NULL, &hr);

	// Aux hoist
	CString strAuxHostPrice;
	strAuxHostPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.HB5);
	m_staticAuxHoistCode.SetText(m_pParamCalculator->m_CalcDatalst.HB4);
	m_staticAuxHoistPrice.SetText(strAuxHostPrice);
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
	m_pParamCalculator->m_CalcDatalst.HB1 = m_tbHoistSel.GetRecordCnt();
	strCountInfo.Format(IDS_CALCHOIST_COUNTINFO, m_pParamCalculator->m_CalcDatalst.HB1);
	SetDlgItemText(IDC_STATIC_RESULT, strCountInfo);
}
//-----------------------------------------------------------------------
CString	CBMOfferCalcHoistDlg::GetQuerySQLText(BOOL bIncludeRailGau, int iRailGua)
{
	CString strQuerySQL = _T("\0");

	// Get hoist table name
	CString strHoistTable = GetHoistSelTableName(m_pParamCalculator->m_CalcDatalst.GA12, m_pParamCalculator->m_CalcDatalst.HA10);

	// Create query text string
	if (eReevingType_All == m_pParamCalculator->m_CalcDatalst.HA7)
	{
		if (m_pParamCalculator->m_CalcDatalst.HA9 == _T("A"))
		{
			strQuerySQL.Format(_T("SELECT * FROM %s WHERE HoiLod>=%d ORDER BY PriLisHoi"),	strHoistTable, m_pParamCalculator->m_CalcDatalst.HA3);
		}
		else
		{			
			strQuerySQL.Format(_T("SELECT * FROM %s WHERE HoiLod>=%d AND Stock='%s' ORDER BY PriLisHoi"), strHoistTable, m_pParamCalculator->m_CalcDatalst.HA3, m_pParamCalculator->m_CalcDatalst.HA9);
		}
	}
	else
	{		
		if (m_pParamCalculator->m_CalcDatalst.HA9 == _T("A"))
		{
			strQuerySQL.Format(_T("SELECT * FROM %s WHERE HoiLod>=%d AND Reeving LIKE '%s%d' ORDER BY PriLisHoi"), strHoistTable, m_pParamCalculator->m_CalcDatalst.HA3, _T("%"), m_pParamCalculator->m_CalcDatalst.HA7);
		}
		else
		{			
			strQuerySQL.Format(_T("SELECT * FROM %s WHERE HoiLod>=%d AND Reeving LIKE '%s%d' AND Stock='%s' ORDER BY PriLisHoi"), strHoistTable, m_pParamCalculator->m_CalcDatalst.HA3, _T("%"), m_pParamCalculator->m_CalcDatalst.HA7, m_pParamCalculator->m_CalcDatalst.HA9);
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
CString	CBMOfferCalcHoistDlg::GetCurHoistTableName()
{
	CString strHoistTable;
	if (m_pParamCalculator->m_CalcDatalst.GA12 == _T("D"))
	{
		if (m_pParamCalculator->m_CalcDatalst.HA101 == _T("BM"))
			strHoistTable = g_tcTableName[eTableID_HoiSel_D];
		else if (m_pParamCalculator->m_CalcDatalst.HA101 == _T("NS"))
			strHoistTable = g_tcTableName[eTableID_SHoiSel_D];
		else
			strHoistTable = g_tcTableName[eTableID_SHoiSel_F];
	}
	else if (m_pParamCalculator->m_CalcDatalst.GA12 == _T("S"))
	{
		if (m_pParamCalculator->m_CalcDatalst.HA101 == _T("BM"))
			strHoistTable = g_tcTableName[eTableID_HoiSel_S];
		else
			strHoistTable = g_tcTableName[eTableID_SHoiSel_S];
	}

	return strHoistTable;
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::ParserSelHoistData()
{
	if (_T("BM") == m_pParamCalculator->m_CalcDatalst.HA101)
	{
		m_pParamCalculator->m_CalcDatalst.HB202.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(2));
		m_pParamCalculator->m_CalcDatalst.HB203.Format(_T("%c%c%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(0), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(1), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(2));
		m_pParamCalculator->m_CalcDatalst.HB204.Format(_T("%c%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(3), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(4));
		m_pParamCalculator->m_CalcDatalst.HB204A.Format(_T("%c%c%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(5), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(6), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(7));
		m_pParamCalculator->m_CalcDatalst.HB205.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(8));
		m_pParamCalculator->m_CalcDatalst.HB206.Format(_T("%c%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(8), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(9));
		m_pParamCalculator->m_CalcDatalst.HB207.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(10));
		m_pParamCalculator->m_CalcDatalst.HB208.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(11));
		m_pParamCalculator->m_CalcDatalst.HB209.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(12));
		m_pParamCalculator->m_CalcDatalst.HB210.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(13));
		//
		m_pParamCalculator->m_CalcDatalst.HB2 = m_pParamCalculator->m_CalcDatalst.HA10 + 
												m_pParamCalculator->m_CalcDatalst.HB202 +
												m_pParamCalculator->m_CalcDatalst.HB204 +
												m_pParamCalculator->m_CalcDatalst.HB204A +
												m_pParamCalculator->m_CalcDatalst.HB206 +
												m_pParamCalculator->m_CalcDatalst.HB207 +
												m_pParamCalculator->m_CalcDatalst.HB208 +
												m_pParamCalculator->m_CalcDatalst.HB209 +
												m_pParamCalculator->m_CalcDatalst.HB210;
		//
		if (_T("T") == m_pParamCalculator->m_CalcDatalst.HB205) m_pParamCalculator->m_CalcDatalst.HB211 = _T("Inverter");
		else m_pParamCalculator->m_CalcDatalst.HB211 = _T("2-Speed");
		//
		m_pParamCalculator->m_CalcDatalst.HB212 = m_pParamCalculator->m_CalcDatalst.HB203 +
												  m_pParamCalculator->m_CalcDatalst.HB206 +
												  m_pParamCalculator->m_CalcDatalst.HB207 +
												  m_pParamCalculator->m_CalcDatalst.HB208;
		m_pParamCalculator->m_CalcDatalst.HB213 = m_pParamCalculator->m_CalcDatalst.HB203 +
												  m_pParamCalculator->m_CalcDatalst.HB204 +
												  m_pParamCalculator->m_CalcDatalst.HB209 +
												  m_pParamCalculator->m_CalcDatalst.HB210;

		CString strSQL;
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiDimIndCod='%s'"), g_tcTableName[eTableID_TroDriTyp], m_pParamCalculator->m_CalcDatalst.HB213);
		m_pParamCalculator->m_CalcDatalst.HB214 = GetStringValueFromTable(strSQL, _T("TroDriTyp"));
		m_pParamCalculator->m_CalcDatalst.HB215 = GetLONGValueFromTable(strSQL, _T("TroDriNum"));
		
		strSQL.Format(_T("SELECT * FROM %s WHERE TroDriTyp='%s'"), g_tcTableName[eTableID_TroDriDat], m_pParamCalculator->m_CalcDatalst.HB214);
		m_pParamCalculator->m_CalcDatalst.HB216 = GetFLOATValueFromTable(strSQL, _T("PowTraDri"));

		strSQL.Format(_T("SELECT * FROM %s WHERE HoiMacIndCod='%s'"), g_tcTableName[eTableID_HoiMacIndCod], m_pParamCalculator->m_CalcDatalst.HB212);
		m_pParamCalculator->m_CalcDatalst.HB217 = GetFLOATValueFromTable(strSQL, _T("PowHoiMotH"));
		m_pParamCalculator->m_CalcDatalst.HB218 = GetStringValueFromTable(strSQL, _T("PowHoiMot"));

		CString strTable;
		CADOTable tbSearch;
		if (_T("D") == m_pParamCalculator->m_CalcDatalst.GA12) strTable = g_tcTableName[eTableID_HoiSel_D];
		else strTable = g_tcTableName[eTableID_HoiSel_S];
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiTyeCod='%s'"), strTable, m_pParamCalculator->m_CalcDatalst.HB201);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			TCHAR tcTemp[256] = {0};
			tbSearch.GetRecordCell(0, L"HoiSpe", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB219 = tcTemp;
			tbSearch.GetRecordCell(0, L"HoiSpe1", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB220);
			tbSearch.GetRecordCell(0, L"B0", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB221 = _ttol(tcTemp); 
			tbSearch.GetRecordCell(0, L"DutCla", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB222 = tcTemp;
			tbSearch.GetRecordCell(0, L"HoiMacMot", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB223 = tcTemp;
			tbSearch.GetRecordCell(0, L"HOL", m_pParamCalculator->m_CalcDatalst.HB224);
			tbSearch.GetRecordCell(0, L"PriLisHoi", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB225);
			tbSearch.GetRecordCell(0, L"Stock", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB226 = tcTemp;
		}
		tbSearch.CloseTable();
		//
		if (_T("D") == m_pParamCalculator->m_CalcDatalst.GA12) strTable = g_tcTableName[eTableID_DimHoi_D];
		else strTable = g_tcTableName[eTableID_DimHoi_S];
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiDimIndCod='%s'"), strTable, m_pParamCalculator->m_CalcDatalst.HB213);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			TCHAR tcTemp[256] = {0};
			tbSearch.GetRecordCell(0, L"WgtHoi", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB227);
			tbSearch.GetRecordCell(0, L"b", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB228);
			tbSearch.GetRecordCell(0, L"b1", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB229); 
			tbSearch.GetRecordCell(0, L"b2", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB230); 
			tbSearch.GetRecordCell(0, L"H2", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB231); 
			tbSearch.GetRecordCell(0, L"H3", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB232); 
			tbSearch.GetRecordCell(0, L"W", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB233); 
			tbSearch.GetRecordCell(0, L"H1", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB234); 
			tbSearch.GetRecordCell(0, L"C", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB235); 
			tbSearch.GetRecordCell(0, L"S4", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB236); 
			tbSearch.GetRecordCell(0, L"S1", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB237); 
			tbSearch.GetRecordCell(0, L"S2", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB238); 
			tbSearch.GetRecordCell(0, L"S3", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB239); 
		}
		tbSearch.CloseTable();
		m_pParamCalculator->m_CalcDatalst.HB240 = m_pParamCalculator->m_CalcDatalst.HA10 + m_pParamCalculator->m_CalcDatalst.HB202;
		m_pParamCalculator->m_CalcDatalst.HB241 = m_pParamCalculator->m_CalcDatalst.HB215 * m_pParamCalculator->m_CalcDatalst.HB216;
	
	}
	else if (_T("NS") == m_pParamCalculator->m_CalcDatalst.HA101 ||
			 _T("NF") == m_pParamCalculator->m_CalcDatalst.HA101)
	{
		m_pParamCalculator->m_CalcDatalst.HB2 = m_pParamCalculator->m_CalcDatalst.HB201;

		m_pParamCalculator->m_CalcDatalst.HB202.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(1));
		m_pParamCalculator->m_CalcDatalst.HB203.Format(_T("%c%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(0), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(1));
		m_pParamCalculator->m_CalcDatalst.HB205.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(8));
		m_pParamCalculator->m_CalcDatalst.HB206.Format(_T("%c%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(8), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(9));
		m_pParamCalculator->m_CalcDatalst.HB207.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(5));
		m_pParamCalculator->m_CalcDatalst.HB208.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(7));
		m_pParamCalculator->m_CalcDatalst.HB209.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(4));
		m_pParamCalculator->m_CalcDatalst.HB210.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.HB201.GetAt(6));
		//
		if (_T("T") == m_pParamCalculator->m_CalcDatalst.HB205) m_pParamCalculator->m_CalcDatalst.HB211 = _T("Inverter");
		else m_pParamCalculator->m_CalcDatalst.HB211 = _T("2-Speed");
		//

		CString strSQL;
		CString strTable;
		CADOTable tbSearch;
		strTable = GetHoistSelTableName(m_pParamCalculator->m_CalcDatalst.GA12, m_pParamCalculator->m_CalcDatalst.HA10);
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiTyeCod='%s'"), strTable, m_pParamCalculator->m_CalcDatalst.HB201);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			TCHAR tcTemp[256] = {0};
			tbSearch.GetRecordCell(0, L"Reeving", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB204 = tcTemp;
			tbSearch.GetRecordCell(0, L"TroDriTyp", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB214 = tcTemp;
			tbSearch.GetRecordCell(0, L"TroDriNum", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB215); 
			tbSearch.GetRecordCell(0, L"PowTraDri", m_pParamCalculator->m_CalcDatalst.HB216); 
			tbSearch.GetRecordCell(0, L"PowHoiMotH", m_pParamCalculator->m_CalcDatalst.HB217); 
			tbSearch.GetRecordCell(0, L"PowHoiMot", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB218 = tcTemp;
			tbSearch.GetRecordCell(0, L"HoiSpe",  tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB219 = tcTemp;
			tbSearch.GetRecordCell(0, L"HoiSpe1", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB220);
			tbSearch.GetRecordCell(0, L"B0", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB221 = _ttol(tcTemp);
			tbSearch.GetRecordCell(0, L"DutCla", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB222 = tcTemp;
			tbSearch.GetRecordCell(0, L"HoiMacMot", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB223 = tcTemp;
			tbSearch.GetRecordCell(0, L"HOL", m_pParamCalculator->m_CalcDatalst.HB224);
			tbSearch.GetRecordCell(0, L"PriLisHoi", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB225);
			tbSearch.GetRecordCell(0, L"Stock", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.HB226 = tcTemp;
			//
			tbSearch.GetRecordCell(0, L"WgtHoi", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB227);
			tbSearch.GetRecordCell(0, L"b", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB228);
			tbSearch.GetRecordCell(0, L"b1", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB229); 
			tbSearch.GetRecordCell(0, L"b2", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB230); 
			tbSearch.GetRecordCell(0, L"H2", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB231); 
			tbSearch.GetRecordCell(0, L"H3", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB232); 
			tbSearch.GetRecordCell(0, L"W", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB233); 
			tbSearch.GetRecordCell(0, L"H1", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB234); 
			tbSearch.GetRecordCell(0, L"C", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB235); 
			tbSearch.GetRecordCell(0, L"S4", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB236); 
			tbSearch.GetRecordCell(0, L"S1", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB237); 
			tbSearch.GetRecordCell(0, L"S2", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB238); 
			tbSearch.GetRecordCell(0, L"S3", (DWORD&)m_pParamCalculator->m_CalcDatalst.HB239); 
		}
		tbSearch.CloseTable();
		m_pParamCalculator->m_CalcDatalst.HB240 = m_pParamCalculator->m_CalcDatalst.HB202;
		m_pParamCalculator->m_CalcDatalst.HB241 = m_pParamCalculator->m_CalcDatalst.HB215 * m_pParamCalculator->m_CalcDatalst.HB216;

		strSQL.Format(_T("SELECT * FROM %s WHERE TroDriTyp='%s'"), g_tcTableName[eTableID_TroDriDat], m_pParamCalculator->m_CalcDatalst.HB214);
		m_pParamCalculator->m_CalcDatalst.HB216 = GetFLOATValueFromTable(strSQL, _T("PowTraDri"));
	}

	if (_T("BM") == m_pParamCalculator->m_CalcDatalst.HA101) m_pParamCalculator->m_CalcDatalst.HB3 = m_pParamCalculator->m_CalcDatalst.HB225;
	else if (_T("NS") == m_pParamCalculator->m_CalcDatalst.HA101) m_pParamCalculator->m_CalcDatalst.HB3 = m_pParamCalculator->m_CalcDatalst.HB225 * m_pParamCalculator->m_CalcDatalst.P25;
	else m_pParamCalculator->m_CalcDatalst.HB3 = m_pParamCalculator->m_CalcDatalst.HB225 * m_pParamCalculator->m_CalcDatalst.P26;
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistDlg::UpdatePrices()
{
	// Selected hoist unit price
	CString strHostPrice;
	strHostPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.HB3);
	m_staticHoistPrice.SetText(strHostPrice);

	// Selected Aux.Hoist unit price
	CString strAuxHostPrice;
	strAuxHostPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.HB5);
	m_staticAuxHoistPrice.SetText(strAuxHostPrice);

	// Get all additions price
	m_pParamCalculator->m_CalcDatalst.HB7 = 0;
	int iSelAddCnt = m_lstCtlAdditions.GetItemCount();
	for (int iIndex = 0; iIndex < iSelAddCnt; iIndex++)
	{
		CString strPrice;
		strPrice = m_lstCtlAdditions.GetItemText(iIndex, 2);
		m_pParamCalculator->m_CalcDatalst.HB7 += _ttol(strPrice);
	}
	//
	CString strAddsPrice;
	strAddsPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.HB7);
	m_staticAddPrice.SetText(strAddsPrice);
	
	// Unit price of Hoist+Aux.Hoist+Additions
	CString strUnitPrice;
	strUnitPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.HB7 + m_pParamCalculator->m_CalcDatalst.HB3 + m_pParamCalculator->m_CalcDatalst.HB5);
	m_staticUnitPrice.SetText(strUnitPrice);

	//	Total hoist price (Count * (hoist price + additions price))
	CString strCount;
	CString strTotalPrice;
	GetDlgItemText(IDC_STATIC_AMOUNT, strCount);
	m_pParamCalculator->m_CalcDatalst.HB9 = (m_pParamCalculator->m_CalcDatalst.HB7 + m_pParamCalculator->m_CalcDatalst.HB3 + m_pParamCalculator->m_CalcDatalst.HB5);
	m_pParamCalculator->m_CalcDatalst.HC1 = m_pParamCalculator->m_CalcDatalst.HB8 * m_pParamCalculator->m_CalcDatalst.HB9;
	strTotalPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.HC1);
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
	if (_T("S") == m_pParamCalculator->m_CalcDatalst.GA12)
	{
		m_pParamCalculator->m_CalcDatalst.HA8 = strSelHoistGauage;
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
/*
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
*/
	int iSelHoistGauage = _ttol(strSelHoistGauage);
	int iNewRailGauage = iSelHoistGauage + g_iRailGauage[iAddGauageLevel];
	m_pParamCalculator->m_CalcDatalst.HA8.Format(_T("%d"), iNewRailGauage);
	SetDlgItemText(IDC_EDIT_RAILGAUGE, m_pParamCalculator->m_CalcDatalst.HA8);
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
BOOL CBMOfferCalcHoistDlg::CanDoNextStep()
{
	if (m_pParamCalculator->m_CalcDatalst.HB2.IsEmpty())
	{
		myMessageBox(IDS_CALC_HOISTCODEISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	m_pParamCalculator->m_CalcDatalst.HD03 = m_pParamCalculator->m_CalcDatalst.HB203 +
											 m_pParamCalculator->m_CalcDatalst.HB204 +
											 m_pParamCalculator->m_CalcDatalst.AU403 +
											 m_pParamCalculator->m_CalcDatalst.AU404;

	CString strSQL;
	CADOTable tbSearch;
	strSQL.Format(_T("SELECT * FROM %s WHERE MainAux='%s'"), g_tcTableName[eTableID_MainAuxH], m_pParamCalculator->m_CalcDatalst.HD03);
	if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
	{
		TCHAR tcTemp[256] = {0};
		tbSearch.GetRecordCell(0, L"HookDim", (DWORD&)m_pParamCalculator->m_CalcDatalst.HD04);
		tbSearch.GetRecordCell(0, L"mb", (DWORD&)m_pParamCalculator->m_CalcDatalst.HD05);
		tbSearch.GetRecordCell(0, L"mb1", (DWORD&)m_pParamCalculator->m_CalcDatalst.HD06);
		tbSearch.GetRecordCell(0, L"mb2", (DWORD&)m_pParamCalculator->m_CalcDatalst.HD07);
	}
	tbSearch.CloseTable();
	
	if (_T("D") == m_pParamCalculator->m_CalcDatalst.GA12 || _T("S") == m_pParamCalculator->m_CalcDatalst.GA12)
	{
		m_pParamCalculator->m_CalcDatalst.HD01 = m_pParamCalculator->m_CalcDatalst.HB227;
		m_pParamCalculator->m_CalcDatalst.HD02 = m_pParamCalculator->m_CalcDatalst.HB2;
		m_pParamCalculator->m_CalcDatalst.HD08 = m_pParamCalculator->m_CalcDatalst.HB228;
		m_pParamCalculator->m_CalcDatalst.HD09 = m_pParamCalculator->m_CalcDatalst.HB229;
		m_pParamCalculator->m_CalcDatalst.HD10 = m_pParamCalculator->m_CalcDatalst.HB230;
	}
	else
	{
		m_pParamCalculator->m_CalcDatalst.HD01 = m_pParamCalculator->m_CalcDatalst.HB227 + m_pParamCalculator->m_CalcDatalst.AU419;
		m_pParamCalculator->m_CalcDatalst.HD02 = m_pParamCalculator->m_CalcDatalst.HB2 + _T("+") + m_pParamCalculator->m_CalcDatalst.AU4;
		m_pParamCalculator->m_CalcDatalst.HD08 = m_pParamCalculator->m_CalcDatalst.HD05;
		m_pParamCalculator->m_CalcDatalst.HD09 = m_pParamCalculator->m_CalcDatalst.HD06;
		m_pParamCalculator->m_CalcDatalst.HD10 = m_pParamCalculator->m_CalcDatalst.HD07;
	}

	return TRUE;
};
//-----------------------------------------------------------------------