//-----------------------------------------------------------------------
//	File Name:BMOfferCalcElecDlg.cpp
//	Introduce:Class CBMOfferCalcElecDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcElecDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcElecDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferCalcElecDlg, CDialog)
CBMOfferCalcElecDlg::CBMOfferCalcElecDlg(CWnd* pParent /*=NULL*/)
	: CBMCalcOptionsDlg(CBMOfferCalcElecDlg::IDD, pParent)
{
}
//-----------------------------------------------------------------------
CBMOfferCalcElecDlg::~CBMOfferCalcElecDlg()
{
	DestroyWindow();
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ELEC_ADDITION, m_lstCtlAdditions);
	DDX_Control(pDX, IDC_STATIC_ELEC_ADDPRICE_EL, m_staticAddPriceEL);
	DDX_Control(pDX, IDC_STATIC_ELEC_ADDPRICE_EFS, m_staticAddPriceEFS);
	DDX_Control(pDX, IDC_STATIC_ELEC_ADDPRICE_EOP, m_staticAddPriceEOP);
	DDX_Control(pDX, IDC_STATIC_ELEC_ADDPRICE_ECP, m_staticAddPriceECP);
	DDX_Control(pDX, IDC_STATIC_ELEC_PANNELPRICE, m_staticPannelPrice);
	DDX_Control(pDX, IDC_STATIC_ELEC_FASTOONPRICE, m_staticFastoonPrice);
	DDX_Control(pDX, IDC_STATIC_ELEC_OPERUNITPRICE, m_staticOperterUnitPrice);
	DDX_Control(pDX, IDC_STATIC_ELEC_POWERPRICE, m_staticPowerSupplyPrice);
	DDX_Control(pDX, IDC_STATIC_ELEC_TOTALPRICE, m_staticTotalPrice);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferCalcElecDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_ELEC_ADDADD, &CBMOfferCalcElecDlg::OnBnClickedBtnElecAddadd)
	ON_BN_CLICKED(IDC_BTN_ELEC_DELADD, &CBMOfferCalcElecDlg::OnBnClickedBtnElecDeladd)
	ON_BN_CLICKED(IDC_BTN_ELEC_CALC, &CBMOfferCalcElecDlg::OnBnClickedBtnElecCalc)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_PANNELINVERTER, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPannelinverter)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_PANNELSIZE, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPannelsize)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_PANNELSELECTION, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPannelselection)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_PANNELTRANSFORMER, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPanneltransformer)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_FTNPWRTYPE, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecFtnpwrtype)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_FTNCTRLTYPE, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecFtnctrltype)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_FTNSELECTION, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecFtnselection)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_OUCTRLTYPE, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecOuctrltype)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_OUBTNNUMBER, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecOubtnnumber)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_HORNBTN, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecHornbtn)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_OUSELECTION, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecOuselection)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_OUCONNECTTYPE, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecOuconnecttype)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_POWPOWTYPE, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPowpowtype)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_POWBRACKET, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPowbracket)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_POWRAILBEAMWDT, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPowrailbeamwdt)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcElecDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferCalcElecDlg::OnInitDialog()
{
	CBMCalcOptionsDlg::OnInitDialog();

	// Initiliaze database table struction
	InitTablesDataStruct();

	// Initiliaze lists
	m_imgNULL.Create(1, 18, TRUE | ILC_COLOR32, 1, 0); 
	InitDataLstControls();

	FillComboBoxes();

	m_staticAddPriceEL.SetColors(COLOR_PRICE);
	m_staticAddPriceEFS.SetColors(COLOR_PRICE);
	m_staticAddPriceEOP.SetColors(COLOR_PRICE);
	m_staticAddPriceECP.SetColors(COLOR_PRICE);
	m_staticPannelPrice.SetColors(COLOR_PRICE);
	m_staticFastoonPrice.SetColors(COLOR_PRICE);		
	m_staticOperterUnitPrice.SetColors(COLOR_PRICE);
	m_staticPowerSupplyPrice.SetColors(COLOR_PRICE);
	m_staticTotalPrice.SetColors(COLOR_PRICE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBMCalcOptionsDlg::OnShowWindow(bShow, nStatus);
	if (!bShow)
		return;

	FillPannelSizeComboBox();

	// Set initliaze data to UI
	if (FALSE == m_bDataIsInitliazed)
	{
		/* commented by Demo
		if (m_pParamCalculator->m_bIsEditData)
		{
			InitUIWithDatabaseData();
		}
		else*/
		{
			InitUIWithDefaultData();
		}
		m_bDataIsInitliazed = TRUE;
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnBnClickedBtnElecAddadd()
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
	CBMSelectAdditionsDlg dlg(eAdditionType_Elec);
	dlg.SetSelAdditions(lstSelAddNames);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetSelAdditions(lstNewAddNames);
		m_lstCtlAdditions.DeleteAllItems();
		m_pParamCalculator->m_lstElecCtrlAdditions.clear();
		
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
				tbAdditions.GetRecordCell(dwIndex, _T("Code"), strAddCode.GetBuffer(32), 32);
				tbAdditions.GetRecordCell(dwIndex, _T("Price"), dwAddPrice);
				tbAdditions.GetRecordCell(dwIndex, _T("Comment"), strAddComment.GetBuffer(64), 64);
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
						m_pParamCalculator->m_lstElecCtrlAdditions.push_back(strAddCode);
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
void CBMOfferCalcElecDlg::OnBnClickedBtnElecDeladd()
{
	POSITION pos = m_lstCtlAdditions.GetFirstSelectedItemPosition();
	if (NULL == pos)
		return;

	while (pos)
	{
		int iSelItem = m_lstCtlAdditions.GetNextSelectedItem(pos);
		m_lstCtlAdditions.DeleteItem(iSelItem);

	}
	UpdatePrices();
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnBnClickedBtnElecCalc()
{
	//TBD:Do calculation here!

	m_pParamCalculator->m_strCraPannelCode = GetCranePannelCode();
	SetDlgItemText(IDC_EDIT_ELEC_PANNEL, m_pParamCalculator->m_strCraPannelCode);
	//
	m_pParamCalculator->m_strFastoonCode = GetFastoonCode();
	SetDlgItemText(IDC_EDIT_ELEC_FASTOON, m_pParamCalculator->m_strFastoonCode);
	//
	m_pParamCalculator->m_strOperterUnitCode = GetOperterUnitCode();
	SetDlgItemText(IDC_EDIT_ELEC_OPERUNIT, m_pParamCalculator->m_strOperterUnitCode);
	//
	m_pParamCalculator->m_strCraPowSupplyCode = GetCranePowSupplyCode();
	SetDlgItemText(IDC_EDIT_ELEC_POWER, m_pParamCalculator->m_strCraPowSupplyCode);

	CString strPrice;
	m_pParamCalculator->m_iCranePannelPrice = GetCranePannelPrice();
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_iCranePannelPrice);
	m_staticPannelPrice.SetText(strPrice);
	//
	m_pParamCalculator->m_iFastoonPrice = GetFastoonPrice();
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_iFastoonPrice);
	m_staticFastoonPrice.SetText(strPrice);
	//
	m_pParamCalculator->m_iOperterUnitPrice = GetOperterUnitPrice();
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_iOperterUnitPrice);
	m_staticOperterUnitPrice.SetText(strPrice);
	//
	m_pParamCalculator->m_iPowerSupplyPrice = GetCranePowSupplyPrice();
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_iPowerSupplyPrice);
	m_staticPowerSupplyPrice.SetText(strPrice);
	//
	UpdatePrices();
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
BOOL CBMOfferCalcElecDlg::CanDoNextStep()
{
	if (m_pParamCalculator->m_strCraPannelCode.IsEmpty() ||
		m_pParamCalculator->m_strFastoonCode.IsEmpty() ||
		m_pParamCalculator->m_strOperterUnitCode.IsEmpty() ||
		m_pParamCalculator->m_strCraPowSupplyCode.IsEmpty())
	{
		myMessageBox(IDS_CALC_ELECNOTSELECTED, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
};
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::InitTablesDataStruct()
{
	// "AdditionSel" table
	m_stuAdditionTable.eID = eTableID_Additions;
	m_stuAdditionTable.strName = g_tcTableName[eTableID_Additions];
	m_stuAdditionTable.lstColumns.push_back(TableColumn(TRUE,  50,  L"Index",	IDS_RECORD_INDEX,			eTableDataType_DWORD));
	m_stuAdditionTable.lstColumns.push_back(TableColumn(FALSE, 100, L"Code",	IDS_ADDITIONLIST_NAME,		eTableDataType_String));
	m_stuAdditionTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"Price",	IDS_ADDITIONLIST_PRICE,		eTableDataType_DWORD));
	m_stuAdditionTable.lstColumns.push_back(TableColumn(FALSE, 150, L"Comment",	IDS_ADDITIONLIST_COMMENT,	eTableDataType_String));
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::InitDataLstControls()
{
	m_lstCtlAdditions.SetExtendedStyle(m_lstCtlAdditions.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_lstCtlAdditions.SetFullItemBKColor(RGB(214, 231, 255), RGB(94, 231, 221));
	m_lstCtlAdditions.SetImageList(&m_imgNULL, LVSIL_SMALL);
	InitDataListCtrl(&m_lstCtlAdditions, m_stuAdditionTable);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::FillComboBoxes()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ELEC_PANNELINVERTER);
	int iIndex = pComboBox->AddString(_T("Schneider"));
	pComboBox->SetItemData(iIndex, int(_T('S')));
	iIndex = pComboBox->AddString(_T("Other"));
	pComboBox->SetItemData(iIndex, int(_T('H')));
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELSELECTION);
	iIndex = pComboBox->AddString(_T("00"));
	iIndex = pComboBox->AddString(_T("V2"));
	iIndex = pComboBox->AddString(_T("V3"));
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELTRANSFORMER);
	iIndex = pComboBox->AddString(_T("S"));
	iIndex = pComboBox->AddString(_T("C"));
	pComboBox->SetCurSel(1);


	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNPWRTYPE);
	iIndex = pComboBox->AddString(_T("Flat Cable"));
	pComboBox->SetItemData(iIndex, int(_T('F')));
	iIndex = pComboBox->AddString(_T("Condoctor"));
	pComboBox->SetItemData(iIndex, int(_T('H')));
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNCTRLTYPE);
	iIndex = pComboBox->AddString(_T("Pandent"));
	pComboBox->SetItemData(iIndex, int(_T('P')));
	iIndex = pComboBox->AddString(_T("Radio"));
	pComboBox->SetItemData(iIndex, int(_T('R')));
	iIndex = pComboBox->AddString(_T("Bluetooth"));
	pComboBox->SetItemData(iIndex, int(_T('B')));
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNSELECTION);
	iIndex = pComboBox->AddString(_T("00"));
	iIndex = pComboBox->AddString(_T("V2"));
	iIndex = pComboBox->AddString(_T("V3"));
	pComboBox->SetCurSel(0);


	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUCTRLTYPE);
	iIndex = pComboBox->AddString(_T("Pandent"));
	pComboBox->SetItemData(iIndex, int(_T('P')));
	iIndex = pComboBox->AddString(_T("Radio"));
	pComboBox->SetItemData(iIndex, int(_T('R')));
	iIndex = pComboBox->AddString(_T("Bluetooth"));
	pComboBox->SetItemData(iIndex, int(_T('B')));
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUBTNNUMBER);
	iIndex = pComboBox->AddString(_T("4"));
	pComboBox->SetItemData(iIndex, 4);
	iIndex = pComboBox->AddString(_T("6"));
	pComboBox->SetItemData(iIndex, 6);
	iIndex = pComboBox->AddString(_T("8"));
	pComboBox->SetItemData(iIndex, 8);
	iIndex = pComboBox->AddString(_T("10"));
	pComboBox->SetItemData(iIndex, 10);
	iIndex = pComboBox->AddString(_T("12"));
	pComboBox->SetItemData(iIndex, 12);
	pComboBox->SetCurSel(2);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_HORNBTN);
	iIndex = pComboBox->AddString(_T("0"));
	iIndex = pComboBox->AddString(_T("1"));
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUSELECTION);
	iIndex = pComboBox->AddString(_T("00"));
	iIndex = pComboBox->AddString(_T("V2"));
	iIndex = pComboBox->AddString(_T("V3"));
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUCONNECTTYPE);
	iIndex = pComboBox->AddString(_T("J1-Fastoon"));
	iIndex = pComboBox->AddString(_T("J2-Panel"));
	iIndex = pComboBox->AddString(_T("J3-Hoist"));
	pComboBox->SetCurSel(0);

	
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWPOWTYPE);
	iIndex = pComboBox->AddString(_T("Flat Cable"));
	pComboBox->SetItemData(iIndex, int(_T('F')));
	iIndex = pComboBox->AddString(_T("Condoctor"));
	pComboBox->SetItemData(iIndex, int(_T('H')));
	pComboBox->SetCurSel(1);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWBRACKET);
	iIndex = pComboBox->AddString(_T("300"));
	pComboBox->SetItemData(iIndex, 3);
	iIndex = pComboBox->AddString(_T("500"));
	pComboBox->SetItemData(iIndex, 5);
	iIndex = pComboBox->AddString(_T("700"));
	pComboBox->SetItemData(iIndex, 7);
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWRAILBEAMWDT);
	iIndex = pComboBox->AddString(_T("Welded"));
	pComboBox->SetItemData(iIndex, int(_T('W')));
	iIndex = pComboBox->AddString(_T("Clanp"));
	pComboBox->SetItemData(iIndex, int(_T('C')));
	pComboBox->SetCurSel(0);	
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::FillPannelSizeComboBox()
{
	int iIndex = 0;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELSIZE);
	pComboBox->ResetContent();
	if (eCraneType_GTS == m_pParamCalculator->m_eCraneType)
	{
		iIndex = pComboBox->AddString(_T("5-S"));
		pComboBox->SetItemData(iIndex, 5);
		pComboBox->SetCurSel(0);
	}
	else
	{
		iIndex = pComboBox->AddString(_T("6-D"));
		pComboBox->SetItemData(iIndex, 6);
		iIndex = pComboBox->AddString(_T("7-D"));
		pComboBox->SetItemData(iIndex, 7);		
		pComboBox->SetCurSel((m_pParamCalculator->m_iLoadOfCrane < 10000) ? 0 : 1);
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::InitUIWithDefaultData()
{
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::InitUIWithDatabaseData()
{
	// List additions	
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
			for (list<CString>::iterator it = m_pParamCalculator->m_lstElecCtrlAdditions.begin();
				it != m_pParamCalculator->m_lstElecCtrlAdditions.end();
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

	CComboBox* pComboBox = NULL;

	// Crane panel
	CString strPannelSize;
	CString strPannelSelection;
	CString strPannelTransformer;
	TCHAR cInverterType = m_pParamCalculator->m_strCraPannelCode.GetAt(13);
	strPannelSize.Format(_T("%c"), m_pParamCalculator->m_strCraPannelCode.GetAt(14));
	strPannelSelection.Format(_T("%c%c"), m_pParamCalculator->m_strCraPannelCode.GetAt(15), m_pParamCalculator->m_strCraPannelCode.GetAt(16));
	strPannelTransformer.Format(_T("%c"), m_pParamCalculator->m_strCraPannelCode.GetAt(17));
	pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ELEC_PANNELINVERTER);
	SelectComboBoxItem(pComboBox, int(cInverterType));
	pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ELEC_PANNELSIZE);
	SelectComboBoxItem(pComboBox, _ttol(strPannelSize));
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELSELECTION);
	SelectComboBoxItem(pComboBox, strPannelSelection);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELTRANSFORMER);
	SelectComboBoxItem(pComboBox, strPannelTransformer);

	// Fastoon
	CString strFastoonSelection;
	TCHAR cFastoonType = m_pParamCalculator->m_strFastoonCode.GetAt(7);
	TCHAR cFastoonCtrl = m_pParamCalculator->m_strFastoonCode.GetAt(17);
	strFastoonSelection.Format(_T("%c%c"), m_pParamCalculator->m_strFastoonCode.GetAt(18), m_pParamCalculator->m_strFastoonCode.GetAt(19));
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNPWRTYPE);
	SelectComboBoxItem(pComboBox, int(cFastoonType));
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNCTRLTYPE);
	SelectComboBoxItem(pComboBox, int(cFastoonCtrl));
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNSELECTION);
	SelectComboBoxItem(pComboBox, strFastoonSelection);

	// Operter unit
	CString strBtnNumber;
	CString strHornBtn;
	CString strOperterSelection;
	CString strOperterCtrl;
	TCHAR cOperterUnitType = m_pParamCalculator->m_strOperterUnitCode.GetAt(2);
	strBtnNumber.Format(_T("%c%c"), m_pParamCalculator->m_strOperterUnitCode.GetAt(3), m_pParamCalculator->m_strOperterUnitCode.GetAt(4));
	strHornBtn.Format(_T("%c"), m_pParamCalculator->m_strOperterUnitCode.GetAt(5));
	strOperterSelection.Format(_T("%c%c"), m_pParamCalculator->m_strOperterUnitCode.GetAt(6), m_pParamCalculator->m_strOperterUnitCode.GetAt(7));
	strOperterCtrl.Format(_T("%c%c"), m_pParamCalculator->m_strOperterUnitCode.GetAt(11), m_pParamCalculator->m_strOperterUnitCode.GetAt(12));
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUCTRLTYPE);
	SelectComboBoxItem(pComboBox, int(cOperterUnitType));
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUBTNNUMBER);
	SelectComboBoxItem(pComboBox, _ttol(strBtnNumber));
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_HORNBTN);
	SelectComboBoxItem(pComboBox, strHornBtn);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUSELECTION);
	SelectComboBoxItem(pComboBox, strOperterSelection);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUCONNECTTYPE);
	for (int i = 0; i < pComboBox->GetCount(); i++)
	{
		CString strText;		
		pComboBox->GetLBText(i, strText);
		if (strText.Left(2) == strOperterCtrl)
		{
			pComboBox->SetCurSel(i);
			break;
		}
	}

	// Crane power supply	
	CString strBracket;
	TCHAR cCranePowerType = m_pParamCalculator->m_strCraPowSupplyCode.GetAt(6);
	TCHAR cCranePowerFixed = m_pParamCalculator->m_strCraPowSupplyCode.GetAt(8);
	strBracket.Format(_T("%c"), m_pParamCalculator->m_strCraPowSupplyCode.GetAt(9));
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWPOWTYPE);
	SelectComboBoxItem(pComboBox, int(cCranePowerType));
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWBRACKET);
	SelectComboBoxItem(pComboBox, _ttol(strBracket));
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWRAILBEAMWDT);
	SelectComboBoxItem(pComboBox, int(cCranePowerFixed));

	/*	Set code and price to UI*/
	SetDlgItemText(IDC_EDIT_ELEC_PANNEL, m_pParamCalculator->m_strCraPannelCode);
	SetDlgItemText(IDC_EDIT_ELEC_FASTOON, m_pParamCalculator->m_strFastoonCode);
	SetDlgItemText(IDC_EDIT_ELEC_OPERUNIT, m_pParamCalculator->m_strOperterUnitCode);
	SetDlgItemText(IDC_EDIT_ELEC_POWER, m_pParamCalculator->m_strCraPowSupplyCode);
	//
	CString strPrice;
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_iCranePannelPrice);
	m_staticPannelPrice.SetText(strPrice);
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_iFastoonPrice);
	m_staticFastoonPrice.SetText(strPrice);
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_iOperterUnitPrice);
	m_staticOperterUnitPrice.SetText(strPrice);
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_iPowerSupplyPrice);
	m_staticPowerSupplyPrice.SetText(strPrice);
	//
	UpdatePrices();
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::UpdatePrices()
{
	CString strPrice;

	// Get all additions price
	m_iAddPriceEL = 0;
	m_iAddPriceEFS = 0;
	m_iAddPriceEOP = 0;
	m_iAddPriceECP = 0;
	int iSelAddCnt = m_lstCtlAdditions.GetItemCount();
	for (int iIndex = 0; iIndex < iSelAddCnt; iIndex++)
	{
		CString strName;
		CString strPrice;
		strName = m_lstCtlAdditions.GetItemText(iIndex, 1);
		strPrice = m_lstCtlAdditions.GetItemText(iIndex, 2);
		if (strName.Find(_T("EFS")) != -1)
		{
			m_iAddPriceEFS += _ttol(strPrice);
		}
		else if (strName.Find(_T("EOP")) != -1)
		{
			m_iAddPriceEOP += _ttol(strPrice);
		}
		else if (strName.Find(_T("ECP")) != -1)
		{
			m_iAddPriceECP += _ttol(strPrice);
		}
		else
		{
			m_iAddPriceEL += _ttol(strPrice);
		}
	}
	m_pParamCalculator->m_iELAdditionsPrice = m_iAddPriceEL + m_iAddPriceEFS + m_iAddPriceEOP + m_iAddPriceECP;
	strPrice.Format(_T("%d RMB"), m_iAddPriceEL);
	m_staticAddPriceEL.SetText(strPrice);
	strPrice.Format(_T("%d RMB"), m_iAddPriceEFS);
	m_staticAddPriceEFS.SetText(strPrice);
	strPrice.Format(_T("%d RMB"), m_iAddPriceEOP);
	m_staticAddPriceEOP.SetText(strPrice);
	strPrice.Format(_T("%d RMB"), m_iAddPriceECP);
	m_staticAddPriceECP.SetText(strPrice);
	//
	DWORD dwTotalPrice;
	dwTotalPrice = m_pParamCalculator->m_iELAdditionsPrice + m_pParamCalculator->m_iCranePannelPrice + m_pParamCalculator->m_iFastoonPrice + m_pParamCalculator->m_iOperterUnitPrice + m_pParamCalculator->m_iPowerSupplyPrice;
	strPrice.Format(_T("%d RMB"), dwTotalPrice);
	m_staticTotalPrice.SetText(strPrice);
}
//-----------------------------------------------------------------------
CString CBMOfferCalcElecDlg::GetCranePannelCode()
{
	CString strCode;

	TCHAR cPannelInvert;
	int iPannelSize;
	CString strPannelSel;
	CString strPannelTrans;
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ELEC_PANNELINVERTER);
	int iIndex = pComboBox->GetCurSel();
	cPannelInvert = (TCHAR)pComboBox->GetItemData(iIndex);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELSIZE);
	iIndex = pComboBox->GetCurSel();
	iPannelSize = pComboBox->GetItemData(iIndex);
	//
	GetDlgItemText(IDC_COMBO_ELEC_PANNELSELECTION, strPannelSel);
	GetDlgItemText(IDC_COMBO_ELEC_PANNELTRANSFORMER, strPannelTrans);


	CString strCraType;
	if (eCraneType_GTS == m_pParamCalculator->m_eCraneType) strCraType = _T("S");
	else strCraType = _T("D");

	CString strHoistMotorPow;
	if (m_pParamCalculator->m_fHoist_PowH <= 3.2f)
	{
		strHoistMotorPow = _T("PA");
	}
	else if (m_pParamCalculator->m_fHoist_PowH <= 6.1f)
	{
		strHoistMotorPow = _T("PB");
	}
	else if (m_pParamCalculator->m_fHoist_PowH <= 9.8f)
	{
		strHoistMotorPow = _T("PC");
	}
	else if (m_pParamCalculator->m_fHoist_PowH <= 16.0f)
	{
		strHoistMotorPow = _T("PD");
	}
	else if (m_pParamCalculator->m_fHoist_PowH <= 20.0f)
	{
		strHoistMotorPow = _T("PE");
	}
	else if (m_pParamCalculator->m_fHoist_PowH <= 38.0f)
	{
		strHoistMotorPow = _T("PF");
	}
	else
	{
		strHoistMotorPow = _T("PG");
	}	

	CString strEndCarDriPow;
	float fTotalPow = /*m_pParamCalculator->m_iNumOfMotor * */m_pParamCalculator->m_fEndCar_DriPow;
	if (fTotalPow <= 0.31f)		//0-0.31
	{
		strEndCarDriPow = _T("T1");
	}
	else if(fTotalPow <= 0.44f)	//0.32-0.44
	{
		strEndCarDriPow = _T("T2");
	}
	else if(fTotalPow <= 0.96f)	//0.45-0.96
	{
		strEndCarDriPow = _T("T3");
	}
	else if(fTotalPow <= 1.3f)	//0.97-1.3
	{
		strEndCarDriPow = _T("T4");
	}
	else if(fTotalPow <= 1.9f)	//1.31-1.9
	{
		strEndCarDriPow = _T("T5");
	}
	else if(fTotalPow <= 3.0f)	//1.91-3.0
	{
		strEndCarDriPow = _T("T6");
	}
	else						//>3.0
	{
		strEndCarDriPow = _T("T7");
	}
	//
	CString strV3Code;
	if (eElecCtrlType_2Speeds == m_pParamCalculator->m_eBridgeCtrlType)
	{
		strV3Code.Format(_T("P%d"), m_pParamCalculator->m_iHoist_V3);
	}
	else
	{
		strV3Code.Format(_T("T%d"), m_pParamCalculator->m_iHoist_V3);
	}

	strCode.Format(	_T("DXC%s%s%d%s%d%s%c%d%s%s%d"), 
					strCraType, 
					strHoistMotorPow, 
					m_pParamCalculator->m_iNumOfHoist, 
					strEndCarDriPow, 
					m_pParamCalculator->m_iNumOfMotor, 
					strV3Code, 
					cPannelInvert, 
					iPannelSize, 
					strPannelSel, 
					strPannelTrans, 
					m_pParamCalculator->m_iVoltage1_Main);
	return strCode;
}
//-----------------------------------------------------------------------
CString CBMOfferCalcElecDlg::GetFastoonCode()
{
	CString strCode;

	TCHAR cType;
	TCHAR cCtrlType;
	CString strSelection;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNPWRTYPE);
	int iIndex = pComboBox->GetCurSel();
	cType = (TCHAR)pComboBox->GetItemData(iIndex);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNCTRLTYPE);
	iIndex = pComboBox->GetCurSel();
	cCtrlType = (TCHAR)pComboBox->GetItemData(iIndex);
	//
	GetDlgItemText(IDC_COMBO_ELEC_FTNSELECTION, strSelection);

	CString strCraType;
	CString strGirderWidth;
	if (eCraneType_GTS == m_pParamCalculator->m_eCraneType) 
	{
		strCraType = _T("S");
		strGirderWidth = _T("000");
	}
	else 
	{
		strCraType = _T("D");
		strGirderWidth.Format(_T("%d"), m_pParamCalculator->m_iGirder_w1);
	}

	CString strPowCode;
	if (m_pParamCalculator->m_fHoist_PowH <= 3.2f)
	{
		strPowCode = _T("PA");
	}
	else if (m_pParamCalculator->m_fHoist_PowH <= 6.1f)
	{
		strPowCode = _T("PB");
	}
	else if (m_pParamCalculator->m_fHoist_PowH <= 9.8f)
	{
		strPowCode = _T("PC");
	}
	else if (m_pParamCalculator->m_fHoist_PowH <= 16.0f)
	{
		strPowCode = _T("PD");
	}
	else if (m_pParamCalculator->m_fHoist_PowH <= 20.0f)
	{
		strPowCode = _T("PE");
	}
	else if (m_pParamCalculator->m_fHoist_PowH <= 38.0f)
	{
		strPowCode = _T("PF");
	}
	else
	{
		strPowCode = _T("PG");
	}	

	strCode.Format(	_T("PST%s%d%c%d%s%s%d%c%s"), 
					strCraType, 
					int(m_pParamCalculator->m_fSpan*10), 
					cType, 
					m_pParamCalculator->m_iNumOfHoist, 
					strGirderWidth, 
					strPowCode, 
					m_pParamCalculator->m_iVoltage1_Main, 
					cCtrlType, 
					strSelection);

	return strCode;
}
//-----------------------------------------------------------------------
CString CBMOfferCalcElecDlg::GetOperterUnitCode()
{
	CString strCode;

	TCHAR cType;
	CString strNumber;
	CString strHornBtn;
	CString strSelection;
	CString strCtrlType;

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUCTRLTYPE);
	int iIndex = pComboBox->GetCurSel();
	cType = (TCHAR)pComboBox->GetItemData(iIndex);
	//
	GetDlgItemText(IDC_COMBO_ELEC_OUBTNNUMBER, strNumber);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUBTNNUMBER);
	iIndex = pComboBox->GetCurSel();
	int iBtnNum = (TCHAR)pComboBox->GetItemData(iIndex);
	strNumber.Format(_T("%02d"), iBtnNum);
	//
	GetDlgItemText(IDC_COMBO_ELEC_HORNBTN, strHornBtn);
	GetDlgItemText(IDC_COMBO_ELEC_OUSELECTION, strSelection);
	GetDlgItemText(IDC_COMBO_ELEC_OUCONNECTTYPE, strCtrlType);

	CString strHOLCode;
	strHOLCode.Format(_T("%03d"), int(10 * (m_pParamCalculator->m_fHOL)));
	strCode.Format(	_T("SC%c%s%s%s%s%s"), 
					cType, 
					strNumber, 
					strHornBtn, 
					strSelection, 
					strHOLCode, 
					strCtrlType.Left(2));

	return strCode;
}
//-----------------------------------------------------------------------
CString CBMOfferCalcElecDlg::GetCranePowSupplyCode()
{
	CString strCode;

	TCHAR cType;
	TCHAR cFixedType;
	int iBacketLen;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWPOWTYPE);
	int iIndex = pComboBox->GetCurSel();
	cType = (TCHAR)pComboBox->GetItemData(iIndex);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWRAILBEAMWDT);
	iIndex = pComboBox->GetCurSel();
	cFixedType = (TCHAR)pComboBox->GetItemData(iIndex);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWBRACKET);
	iIndex = pComboBox->GetCurSel();
	iBacketLen = pComboBox->GetItemData(iIndex);

	CString strRailLenCode;
	strRailLenCode.Format(_T("%03d"), (int)(m_pParamCalculator->m_fRailLenght+0.5f));

	int iCraneNum = 1;
	float fTotalPow = m_pParamCalculator->m_fHoist_PowH + m_pParamCalculator->m_fHoist_DriPow + m_pParamCalculator->m_iNumOfMotor * m_pParamCalculator->m_fEndCar_DriPow;

	CString strCraPowCode = _T("A");
	CADOTable tbCalcParams;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE ParamName LIKE '%s%s' ORDER BY Param1", g_tcTableName[eTableID_CalcParams], _T("Cra_Pow"), _T("%"));
	if (tbCalcParams.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation))
	{
		DWORD dwRecordCnt = tbCalcParams.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwRecordCnt; dwIndex++)
		{
			TCHAR tcMaxPow[8] = {0};
			tbCalcParams.GetRecordCell(dwIndex, _T("Param2"), tcMaxPow, 8);
			//
			float fMaxPow = _tstof(tcMaxPow);
			if (fTotalPow > fMaxPow) continue;
			//			
			TCHAR tcCode[8] = {0};
			tbCalcParams.GetRecordCell(dwIndex, _T("Param1"), tcCode, 8);
			strCraPowCode = tcCode;
			break;
		}
		tbCalcParams.CloseTable();
	}

	strCode.Format(	_T("PSC%s%c%d%c%d%s%d"), 
					strRailLenCode, 
					cType, 
					iCraneNum, 
					cFixedType, 
					iBacketLen, 
					strCraPowCode, 
					m_pParamCalculator->m_iVoltage1_Main);

	return strCode;
}
//-----------------------------------------------------------------------
int CBMOfferCalcElecDlg::GetCranePannelPrice()
{
	int iPrice = 0;

	CString strCraType;
	CString strPowCode;
	int iHoistQty;
	int	iPannelSize;
	if (eCraneType_GTS == m_pParamCalculator->m_eCraneType) 
	{
		 strCraType = _T("S");
	}
	else
	{
		 strCraType = _T("D");
	}
	//
	strPowCode.Format(_T("%c%c"), m_pParamCalculator->m_strCraPannelCode.GetAt(4), m_pParamCalculator->m_strCraPannelCode.GetAt(5));
	iHoistQty = m_pParamCalculator->m_iNumOfHoist;
	iPannelSize = _ttol(CString(m_pParamCalculator->m_strCraPannelCode.GetAt(14)));
	//
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE CraTyp='%s' AND PowHoiMotH='%s' AND HoiQty=%d AND CraPanSiz=%d"), g_tcTableName[eTableID_CraPanPri1], strCraType, strPowCode, iHoistQty, iPannelSize);
	int iPrice1 = ::GetLONGValueFromTable(strSQL, _T("CraPanPri1"));

	
	CString strBriDivPow;
	CString strInvTyp;
	strBriDivPow.Format(_T("%c%c"), m_pParamCalculator->m_strCraPannelCode.GetAt(7), m_pParamCalculator->m_strCraPannelCode.GetAt(8));
	strInvTyp.Format(_T("%c"), m_pParamCalculator->m_strCraPannelCode.GetAt(13));
	int	iMotorCnt = m_pParamCalculator->m_iNumOfMotor;
	strSQL.Format(_T("SELECT * FROM %s WHERE BriDivPow='%s' AND InvTyp='%s' AND MotQty=%d"), g_tcTableName[eTableID_CraPanPri2],  strBriDivPow, strInvTyp, iMotorCnt);
	int iPrice2 = ::GetLONGValueFromTable(strSQL, _T("CraPanPri2"));

	// Check prices are right or not
	if (iPrice1 == 0 || iPrice2 == 0)
	{
		myMessageBox(IDS_ELECALC_CRANEPANNELPRICEISWRONG, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		m_staticPannelPrice.SetColors(RGB(255, 255, 0));
	}
	else
	{
		m_staticPannelPrice.SetColors(COLOR_PRICE);
	}

	return iPrice1 + iPrice2;
}
//-----------------------------------------------------------------------
int CBMOfferCalcElecDlg::GetFastoonPrice()
{
	int iPrice = 0;

	CString strCraType;
	CString strPowHoiMotH;
	CString strCraCtrlType;
	int iHoistQty;
	if (eCraneType_GTS == m_pParamCalculator->m_eCraneType) 
	{
		 strCraType = _T("S");
	}
	else
	{
		 strCraType = _T("D");
	}
	//
	iHoistQty = m_pParamCalculator->m_iNumOfHoist;
	strPowHoiMotH.Format(_T("%c%c"), m_pParamCalculator->m_strFastoonCode.GetAt(12), m_pParamCalculator->m_strFastoonCode.GetAt(13));
	strCraCtrlType.Format(_T("%c"),  m_pParamCalculator->m_strFastoonCode.GetAt(17));
	//
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE CraTyp='%s' AND PowHoiMotH='%s' AND HoiQty=%d AND ConTyp='%s'"), g_tcTableName[eTableID_FasPri], strCraType, strPowHoiMotH, iHoistQty, strCraCtrlType);
	int iFixedPrice = ::GetLONGValueFromTable(strSQL, _T("FixPri"));
	int iMetPrice = ::GetLONGValueFromTable(strSQL, _T("MetPri"));

	return iFixedPrice + (iMetPrice * m_pParamCalculator->m_fSpan);
}
//-----------------------------------------------------------------------
int CBMOfferCalcElecDlg::GetOperterUnitPrice()
{
	int iPrice = 0;
	CString strConTyp;
	CString strBtnNum;
	CString strCableLen;
	strConTyp.Format(_T("%c"), m_pParamCalculator->m_strOperterUnitCode.GetAt(2));
	strBtnNum.Format(_T("%c%c"), m_pParamCalculator->m_strOperterUnitCode.GetAt(3), m_pParamCalculator->m_strOperterUnitCode.GetAt(4));
	//strCableLen.Format(_T("%c%c%c"), m_pParamCalculator->m_strOperterUnitCode.GetAt(7), m_pParamCalculator->m_strOperterUnitCode.GetAt(8), m_pParamCalculator->m_strOperterUnitCode.GetAt(9));
	//
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE ConTyp='%s' AND PusBut=%d"), g_tcTableName[eTableID_PanPri], strConTyp, _ttol(strBtnNum));
	int iPrice1 = ::GetLONGValueFromTable(strSQL, _T("OpePri"));
	int iPrice2 = ::GetLONGValueFromTable(strSQL, _T("CabPrimet"));

	return iPrice1 + iPrice2 * (m_pParamCalculator->m_fHOL-1);
}
//-----------------------------------------------------------------------
int CBMOfferCalcElecDlg::GetCranePowSupplyPrice()
{
	int iPrice = 0;
	
	CString strPowCode;
	CString strCraNum;
	strPowCode.Format(_T("%c"), m_pParamCalculator->m_strCraPowSupplyCode.GetAt(11));
	strCraNum.Format(_T("%c"), m_pParamCalculator->m_strCraPowSupplyCode.GetAt(7));

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE CrPaowCod='%s' AND Craty=%d AND Length>=%d ORDER BY Length"), g_tcTableName[eTableID_CraPowPri], strPowCode, _ttol(strCraNum), (int)m_pParamCalculator->m_fRailLenght);
	int iPrice1 = ::GetLONGValueFromTable(strSQL, _T("FixPri"));
	int iPrice2 = ::GetLONGValueFromTable(strSQL, _T("MetPri"));

	return iPrice1 + iPrice2 * m_pParamCalculator->m_fRailLenght;
}
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPannelinverter()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ELEC_PANNELINVERTER);
	pComboBox->SetCurSel(0);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPannelsize()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELSIZE);
	pComboBox->SetCurSel(0);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPannelselection()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELSELECTION);
	pComboBox->SetCurSel(0);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPanneltransformer()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELTRANSFORMER);
	pComboBox->SetCurSel(1);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecFtnpwrtype()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNPWRTYPE);
	pComboBox->SetCurSel(0);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecFtnctrltype()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNCTRLTYPE);
	pComboBox->SetCurSel(0);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecFtnselection()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNSELECTION);
	pComboBox->SetCurSel(0);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecOuctrltype()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUCTRLTYPE);
	pComboBox->SetCurSel(0);	
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecOubtnnumber()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUBTNNUMBER);
	pComboBox->SetCurSel(2);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecHornbtn()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_HORNBTN);
	pComboBox->SetCurSel(0);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecOuselection()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUSELECTION);
	pComboBox->SetCurSel(0);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecOuconnecttype()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUCONNECTTYPE);
	pComboBox->SetCurSel(0);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPowpowtype()
{	
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWPOWTYPE);
	pComboBox->SetCurSel(1);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPowbracket()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWBRACKET);
	pComboBox->SetCurSel(0);
}

void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPowrailbeamwdt()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWRAILBEAMWDT);
	pComboBox->SetCurSel(0);	
}
//-----------------------------------------------------------------------
