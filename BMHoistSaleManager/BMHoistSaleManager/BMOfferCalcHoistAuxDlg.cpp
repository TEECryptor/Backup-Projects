//-----------------------------------------------------------------------
//	File Name:BMOfferCalcHoistAuxDlg.cpp
//	Introduce:implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcHoistAuxDlg.h"
#include "BMCalcParameterDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcHoistAuxDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferCalcHoistAuxDlg, CDialog)
CBMOfferCalcHoistAuxDlg::CBMOfferCalcHoistAuxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferCalcHoistAuxDlg::IDD, pParent)
{
	m_pParamCalculator = NULL;
}
//-----------------------------------------------------------------------
CBMOfferCalcHoistAuxDlg::~CBMOfferCalcHoistAuxDlg()
{
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_AUXHOIST, m_lstCtlHoists);
	DDX_Control(pDX, IDC_LIST_AUXHOIST_ADDITION, m_lstCtlAdditions);
	DDX_Control(pDX, IDC_STATIC_AUXHOIST_CODE, m_staticAuxHoistCode);
	DDX_Control(pDX, IDC_STATIC_AUXHOIST_PRI, m_staticAuxHoistPrice);	
	DDX_Control(pDX, IDC_STATIC_AUXHOIST_ADDPRI, m_staticAuxHoistAddPrice);
	DDX_Control(pDX, IDC_STATIC_AUXHOIST_TOTALPRI, m_staticAuxHoistTotalPrice);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferCalcHoistAuxDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_AUXHOIST_ADD, &CBMOfferCalcHoistAuxDlg::OnBnClickedBtnAuxhoistAdd)
	ON_BN_CLICKED(IDC_BTN_AUXHOIST_FIND, &CBMOfferCalcHoistAuxDlg::OnBnClickedBtnAuxhoistFind)
	//ON_CBN_SELCHANGE(IDC_COMBO_AUXHOIST_LOAD, &CBMOfferCalcHoistAuxDlg::OnCbnSelchangeComboAuxhoistLoad)
	//ON_CBN_SELCHANGE(IDC_COMBO_AUXHOIST_BRAND, &CBMOfferCalcHoistAuxDlg::OnCbnSelchangeComboAuxhoistBrand)
	ON_BN_CLICKED(IDC_BTN_AUXHOIST_DEL, &CBMOfferCalcHoistAuxDlg::OnBnClickedBtnAuxhoistDel)
	ON_BN_CLICKED(IDC_BTN_AUXHOIST_PARAMETER, &CBMOfferCalcHoistAuxDlg::OnBnClickedBtnAuxhoistParameter)
	ON_NOTIFY(NM_CLICK, IDC_LIST_AUXHOIST, &CBMOfferCalcHoistAuxDlg::OnNMClickListAuxhoist)
	ON_CBN_SELCHANGE(IDC_COMBO_AUXHOIST_LOAD, &CBMOfferCalcHoistAuxDlg::OnCbnSelchangeComboAuxhoistLoad)
	ON_CBN_SELCHANGE(IDC_COMBO_AUXHOIST_BRAND, &CBMOfferCalcHoistAuxDlg::OnCbnSelchangeComboAuxhoistBrand)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
// CBMOfferCalcHoistAuxDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferCalcHoistAuxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_pParamCalculator->m_CalcDatalst.AU0.GetLength() == 0)
	{
		m_pParamCalculator->m_CalcDatalst.AU0 = g_LoginUserInfo.C38;
	}
	InitBrandComobox();
	InitLoadComobox();

	// Initiliaze database table struction
	m_imgNULL.Create(1, 18, TRUE | ILC_COLOR32, 1, 0); 
	InitTablesDataStruct();
	InitDataLstControls();	

	m_staticAuxHoistCode.SetColors(COLOR_PRICE);
	m_staticAuxHoistPrice.SetColors(COLOR_PRICE);
	m_staticAuxHoistAddPrice.SetColors(COLOR_PRICE);
	m_staticAuxHoistTotalPrice.SetColors(COLOR_PRICE);

	if (m_pParamCalculator->m_CalcDatalst.AU401.GetLength() != 0)
	{		
		HRESULT hr = 0;
		CString strSQL;
		CADOTable tbHoistSel;
		strSQL.Format(L"SELECT * FROM %s WHERE HoiTyeCod='%s'", m_stuHoistTable.strName, m_pParamCalculator->m_CalcDatalst.AU401);
		if (TRUE == tbHoistSel.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL))
		{
			ShowTableRecords(&tbHoistSel, &m_lstCtlHoists, m_stuHoistTable);
			m_lstCtlHoists.SetFocus();
			m_lstCtlHoists.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);	
			OnNMClickListAuxhoist(NULL, &hr);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::OnBnClickedBtnAuxhoistAdd()
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
						break;
					}
				}
			}		
		}
		tbAdditions.CloseTable();

		UpdatePrices();
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::OnCbnSelchangeComboAuxhoistLoad()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_AUXHOIST_LOAD);
	int iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.AU1 = pComboBox->GetItemData(iSelIndex);
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::OnCbnSelchangeComboAuxhoistBrand()
{
	GetDlgItemText(IDC_COMBO_AUXHOIST_BRAND, m_pParamCalculator->m_CalcDatalst.AU0);
	if (_T("BM") == m_pParamCalculator->m_CalcDatalst.AU0 ||
		_T("GT") == m_pParamCalculator->m_CalcDatalst.AU0)
	{
		m_pParamCalculator->m_CalcDatalst.AU01 = _T("BM");
		m_stuHoistTable.strName = g_tcTableName[eTableID_HoiSel_F];
	}
	else
	{
		m_pParamCalculator->m_CalcDatalst.AU01 = _T("NF");
		m_stuHoistTable.strName = g_tcTableName[eTableID_SHoiSel_F];
	}
	InitLoadComobox();
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::OnBnClickedBtnAuxhoistFind()
{
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE HoiLod >= %d ORDER BY PriLisHoi", m_stuHoistTable.strName, m_pParamCalculator->m_CalcDatalst.AU1);

	BOOL bResult = TRUE;
	CADOTable tbHoistSel;
	bResult = tbHoistSel.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	ShowTableRecords(&tbHoistSel, &m_lstCtlHoists, m_stuHoistTable);
	tbHoistSel.CloseTable();
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::OnBnClickedBtnAuxhoistDel()
{	
	POSITION pos = m_lstCtlAdditions.GetFirstSelectedItemPosition();
	if (NULL == pos)
		return;

	//int iAddGauageLevel = 0;
	while (pos)
	{
		int iSelItem = m_lstCtlAdditions.GetNextSelectedItem(pos);
		//
		CString strAddName = m_lstCtlAdditions.GetItemText(iSelItem, 1);
		m_lstCtlAdditions.DeleteItem(iSelItem);
		/*strAddName.MakeLower();
		if (strAddName.Find(STR_HOIST_ADDGAU) != -1)
		{
			CString strAddGauLevel = strAddName.Right(2);
			iAddGauageLevel -= _ttol(strAddGauLevel);
		}*/
	}
	UpdatePrices();
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::OnBnClickedBtnAuxhoistParameter()
{
	CBMCalcParameterDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::OnNMClickListAuxhoist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int iSelItem = -1;
	POSITION pos = m_lstCtlHoists.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlHoists.GetNextSelectedItem(pos);
	if (iSelItem < 0)
		return;

	/* Get selected hoist data */
	m_pParamCalculator->m_CalcDatalst.AU401 = m_lstCtlHoists.GetItemText(iSelItem, 1);
	m_pParamCalculator->ParserSelAuxHoistData();

	/* Show in UI*/
	UpdatePrices();
	m_staticAuxHoistCode.SetText(m_pParamCalculator->m_CalcDatalst.AU4);

	*pResult = 0;
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::SetParamCalculator(CBMParamCalculator* pCalculator)
{
	m_pParamCalculator = pCalculator;
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::InitLoadComobox()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_AUXHOIST_LOAD);
	pComboBox->ResetContent();
	
	list<CString> lstHoistLoads;
	GetValueListInTable(m_stuHoistTable.strName, _T("HoiLod"), eTableDataType_DWORD, lstHoistLoads);
	if (lstHoistLoads.size() == 0)
		return;

	int iIndex = 0;
	int iDefaultIndex = 0;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_AUXHOIST_LOAD);
	pComboBox->ResetContent();
	for (list<CString>::iterator it = lstHoistLoads.begin(); it != lstHoistLoads.end(); it++, iIndex++)
	{
		CString strHoistLoad = *it;
		long lLoad = _ttol(strHoistLoad);
		int iItemIndex = pComboBox->AddString(strHoistLoad);
		pComboBox->SetItemData(iItemIndex, lLoad);
	}
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.AU1);
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::InitBrandComobox()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_AUXHOIST_BRAND);
	pComboBox->ResetContent();
	int iItemIndex = pComboBox->AddString(_T("BM"));
	iItemIndex = pComboBox->AddString(_T("GT"));
	iItemIndex = pComboBox->AddString(_T("NF"));
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.AU0);
	OnCbnSelchangeComboAuxhoistBrand();
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::InitTablesDataStruct()
{
	// "HoiSel" table
	CString strTable;
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
void CBMOfferCalcHoistAuxDlg::InitDataLstControls()
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
void CBMOfferCalcHoistAuxDlg::ParserSelHoistData()
{
	if (_T("BM") == m_pParamCalculator->m_CalcDatalst.AU01)
	{
		m_pParamCalculator->m_CalcDatalst.AU402.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(2));
		m_pParamCalculator->m_CalcDatalst.AU403.Format(_T("%c%c%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(0), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(1), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(2));
		m_pParamCalculator->m_CalcDatalst.AU404.Format(_T("%c%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(3), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(4));
		m_pParamCalculator->m_CalcDatalst.AU404A.Format(_T("%c%c%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(5), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(6), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(7));
		m_pParamCalculator->m_CalcDatalst.AU405.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(8));
		m_pParamCalculator->m_CalcDatalst.AU406.Format(_T("%c%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(8), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(9));
		m_pParamCalculator->m_CalcDatalst.AU407.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(10));
		m_pParamCalculator->m_CalcDatalst.AU408.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(11));
		m_pParamCalculator->m_CalcDatalst.AU409.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(12));
		m_pParamCalculator->m_CalcDatalst.AU410.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(13));
		//
		m_pParamCalculator->m_CalcDatalst.AU4 =	m_pParamCalculator->m_CalcDatalst.AU0 + 
												m_pParamCalculator->m_CalcDatalst.AU402 +
												m_pParamCalculator->m_CalcDatalst.AU404 +
												m_pParamCalculator->m_CalcDatalst.AU404A +
												m_pParamCalculator->m_CalcDatalst.AU406 +
												m_pParamCalculator->m_CalcDatalst.AU407 +
												m_pParamCalculator->m_CalcDatalst.AU408 +
												m_pParamCalculator->m_CalcDatalst.AU409 +
												m_pParamCalculator->m_CalcDatalst.AU410;
		m_pParamCalculator->m_CalcDatalst.AU411 = m_pParamCalculator->m_CalcDatalst.AU403 +
												  m_pParamCalculator->m_CalcDatalst.AU406 +
												  m_pParamCalculator->m_CalcDatalst.AU407 +
												  m_pParamCalculator->m_CalcDatalst.AU408;
		m_pParamCalculator->m_CalcDatalst.AU412 = m_pParamCalculator->m_CalcDatalst.AU403 +
												  m_pParamCalculator->m_CalcDatalst.AU404 +
												  m_pParamCalculator->m_CalcDatalst.AU409 +
												  m_pParamCalculator->m_CalcDatalst.AU410;

		CString strSQL;
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiMacIndCod='%s'"), g_tcTableName[eTableID_HoiMacIndCod], m_pParamCalculator->m_CalcDatalst.AU411);
		m_pParamCalculator->m_CalcDatalst.AU413 = GetFLOATValueFromTable(strSQL, _T("PowHoiMotH"));
		m_pParamCalculator->m_CalcDatalst.AU414 = GetStringValueFromTable(strSQL, _T("PowHoiMot"));

		CString strTable;
		CADOTable tbSearch;
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiTyeCod='%s'"), g_tcTableName[eTableID_HoiSel_F], m_pParamCalculator->m_CalcDatalst.AU401);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			TCHAR tcTemp[256] = {0};
			tbSearch.GetRecordCell(0, L"HoiSpe", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.AU415 = tcTemp;
			tbSearch.GetRecordCell(0, L"DutCla", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.AU416 = tcTemp;
			tbSearch.GetRecordCell(0, L"HOL", m_pParamCalculator->m_CalcDatalst.AU417);
			tbSearch.GetRecordCell(0, L"PriLisHoi", (DWORD&)m_pParamCalculator->m_CalcDatalst.AU418);
		}
		tbSearch.CloseTable();
		//
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiDimIndCod='%s'"), g_tcTableName[eTableID_DimHoi_F], m_pParamCalculator->m_CalcDatalst.AU412);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			TCHAR tcTemp[256] = {0};
			tbSearch.GetRecordCell(0, L"WgtHoi", (DWORD&)m_pParamCalculator->m_CalcDatalst.AU419);
			tbSearch.GetRecordCell(0, L"C", (DWORD&)m_pParamCalculator->m_CalcDatalst.AU420); 
		}
		tbSearch.CloseTable();		
		if (_T("T") == m_pParamCalculator->m_CalcDatalst.AU405) m_pParamCalculator->m_CalcDatalst.AU421 = _T("Inverter");
		else m_pParamCalculator->m_CalcDatalst.AU421 = _T("2-Speed");
		m_pParamCalculator->m_CalcDatalst.AU422 = m_pParamCalculator->m_CalcDatalst.HA10 + m_pParamCalculator->m_CalcDatalst.AU402;	
	}
	else if (_T("NF") == m_pParamCalculator->m_CalcDatalst.AU01)
	{
		m_pParamCalculator->m_CalcDatalst.AU4 = m_pParamCalculator->m_CalcDatalst.AU401;

		m_pParamCalculator->m_CalcDatalst.AU402.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(1));
		m_pParamCalculator->m_CalcDatalst.AU403.Format(_T("%c%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(0), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(1));
		m_pParamCalculator->m_CalcDatalst.AU405.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(8));
		m_pParamCalculator->m_CalcDatalst.AU406.Format(_T("%c%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(8), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(9));
		m_pParamCalculator->m_CalcDatalst.AU407.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(5));
		m_pParamCalculator->m_CalcDatalst.AU408.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(7));
		m_pParamCalculator->m_CalcDatalst.AU409.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(4));
		m_pParamCalculator->m_CalcDatalst.AU410.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.AU401.GetAt(6));
		//
		CString strSQL;
		CADOTable tbSearch;
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiTyeCod='%s'"), g_tcTableName[eTableID_SHoiSel_F], m_pParamCalculator->m_CalcDatalst.AU401);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			TCHAR tcTemp[256] = {0};
			tbSearch.GetRecordCell(0, L"Reeving", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.AU404 = tcTemp;
			tbSearch.GetRecordCell(0, L"PowHoiMotH", m_pParamCalculator->m_CalcDatalst.AU413); 
			tbSearch.GetRecordCell(0, L"PowHoiMot", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.AU414 = tcTemp;
			tbSearch.GetRecordCell(0, L"HoiSpe",  tcTemp, 256); m_pParamCalculator->m_CalcDatalst.AU415 = tcTemp;
			tbSearch.GetRecordCell(0, L"DutCla", tcTemp, 256); m_pParamCalculator->m_CalcDatalst.AU416 = tcTemp;
			tbSearch.GetRecordCell(0, L"HOL", m_pParamCalculator->m_CalcDatalst.AU417);
			tbSearch.GetRecordCell(0, L"PriLisHoi", (DWORD&)m_pParamCalculator->m_CalcDatalst.AU418);
			//
			tbSearch.GetRecordCell(0, L"WgtHoi", (DWORD&)m_pParamCalculator->m_CalcDatalst.AU419);
			tbSearch.GetRecordCell(0, L"C", (DWORD&)m_pParamCalculator->m_CalcDatalst.AU420); 
		}
		tbSearch.CloseTable();
		if (_T("T") == m_pParamCalculator->m_CalcDatalst.AU405) m_pParamCalculator->m_CalcDatalst.AU421 = _T("Inverter");
		else m_pParamCalculator->m_CalcDatalst.AU421 = _T("2-Speed");
		m_pParamCalculator->m_CalcDatalst.AU422 = m_pParamCalculator->m_CalcDatalst.AU402;
	}

	if (_T("BM") == m_pParamCalculator->m_CalcDatalst.AU01) m_pParamCalculator->m_CalcDatalst.AU5 = m_pParamCalculator->m_CalcDatalst.AU418;
	else  m_pParamCalculator->m_CalcDatalst.AU5 = m_pParamCalculator->m_CalcDatalst.AU418 * m_pParamCalculator->m_CalcDatalst.P26;
}
//-----------------------------------------------------------------------
void CBMOfferCalcHoistAuxDlg::UpdatePrices()
{
	// Aux.Hoist price
	CString strAuxHostPrice;
	strAuxHostPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.AU5);
	m_staticAuxHoistCode.SetText(m_pParamCalculator->m_CalcDatalst.AU4);
	m_staticAuxHoistPrice.SetText(strAuxHostPrice);

	// Aux.Hoist additional price	
	m_pParamCalculator->m_CalcDatalst.AU6 = 0;
	int iSelAddCnt = m_lstCtlAdditions.GetItemCount();
	for (int iIndex = 0; iIndex < iSelAddCnt; iIndex++)
	{
		CString strPrice;
		strPrice = m_lstCtlAdditions.GetItemText(iIndex, 2);
		m_pParamCalculator->m_CalcDatalst.AU6 += _ttol(strPrice);
	}
	CString strAuxHostAddPrice;
	strAuxHostAddPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.AU6);
	m_staticAuxHoistAddPrice.SetText(strAuxHostAddPrice);

	// Totatl price
	CString strAuxHostTotalPrice;
	m_pParamCalculator->m_CalcDatalst.AU7 = m_pParamCalculator->m_CalcDatalst.AU5 + m_pParamCalculator->m_CalcDatalst.AU6;
	strAuxHostTotalPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.AU7);
	m_staticAuxHoistTotalPrice.SetText(strAuxHostTotalPrice);
}
//-----------------------------------------------------------------------