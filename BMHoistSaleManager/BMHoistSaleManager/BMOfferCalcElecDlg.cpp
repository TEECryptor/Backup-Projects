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
	ON_CBN_SELCHANGE(IDC_COMBO_ELEC_QTY, &CBMOfferCalcElecDlg::OnCbnSelchangeComboElecQty)
	ON_BN_CLICKED(IDC_BTN_ELEC_RESETKW, &CBMOfferCalcElecDlg::OnBnClickedBtnElecResetkw)
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
	FillSelectionComboBoxes();

	m_pParamCalculator->m_CalcDatalst.EA2 = GetEA2();
	m_pParamCalculator->m_CalcDatalst.EA21 = GetEA21();
	m_pParamCalculator->m_CalcDatalst.EA24 = GetEA24();
	m_pParamCalculator->m_CalcDatalst.EA22.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.EA21.GetAt(1));

	// Set initliaze data to UI
//	if (FALSE == m_bDataIsInitliazed)
	{
		if (m_pParamCalculator->m_bIsEditData)
		{
			InitUIWithDatabaseData();
			OnBnClickedBtnElecCalc();
		}
		else
		{
			InitUIWithDefaultData();
		}
		m_bDataIsInitliazed = TRUE;
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPannelinverter()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ELEC_PANNELINVERTER);
	int iIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.EA1 = (TCHAR)pComboBox->GetItemData(iIndex);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPannelsize()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ELEC_PANNELSIZE);
	int iIndex = pComboBox->GetCurSel();
	pComboBox->GetWindowText(m_pParamCalculator->m_CalcDatalst.EA2);
	m_pParamCalculator->m_CalcDatalst.EA23 = pComboBox->GetItemData(iIndex);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPannelselection()
{
	GetDlgItemText(IDC_COMBO_ELEC_PANNELSELECTION, m_pParamCalculator->m_CalcDatalst.EA3);
	m_pParamCalculator->UpdateElecPannelselection();
	m_pParamCalculator->m_CalcDatalst.EA2 = GetEA2();
	//
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ELEC_PANNELSIZE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EA2);
	OnCbnSelchangeComboElecPannelsize();
	OnBnClickedBtnElecResetkw();
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPanneltransformer()
{
	GetDlgItemText(IDC_COMBO_ELEC_PANNELTRANSFORMER, m_pParamCalculator->m_CalcDatalst.EA4);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecFtnpwrtype()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ELEC_FTNPWRTYPE);
	int iIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.EB1 = (TCHAR)pComboBox->GetItemData(iIndex);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecFtnctrltype()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNCTRLTYPE);
	int iIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.EB2 = (TCHAR)pComboBox->GetItemData(iIndex);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecFtnselection()
{
	GetDlgItemText(IDC_COMBO_ELEC_FTNSELECTION, m_pParamCalculator->m_CalcDatalst.EB3);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecOuctrltype()
{
	GetDlgItemText(IDC_COMBO_ELEC_OUCTRLTYPE, m_pParamCalculator->m_CalcDatalst.EC1);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUCTRLTYPE);
	int iIndex = pComboBox->GetCurSel();
	switch(iIndex)
	{
	case 0:
		m_pParamCalculator->m_CalcDatalst.EC11 = _T("GG");
		m_pParamCalculator->m_CalcDatalst.EC12 = _T("Pendant");
		m_pParamCalculator->m_CalcDatalst.EC13 = _T("P");
		break;
	case 1:
		m_pParamCalculator->m_CalcDatalst.EC11 = _T("RH");
		m_pParamCalculator->m_CalcDatalst.EC12 = _T("Remote + Backup Pandent");
		m_pParamCalculator->m_CalcDatalst.EC13 = _T("R");
		break;
	case 2:
		m_pParamCalculator->m_CalcDatalst.EC11 = _T("RY");
		m_pParamCalculator->m_CalcDatalst.EC12 = _T("Remote + Backup Pandent");
		m_pParamCalculator->m_CalcDatalst.EC13 = _T("R");
		break;
	case 3:
		m_pParamCalculator->m_CalcDatalst.EC11 = _T("RB");
		m_pParamCalculator->m_CalcDatalst.EC12 = _T("Remote + Backup Pandent");
		m_pParamCalculator->m_CalcDatalst.EC13 = _T("B");
		break;
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecOubtnnumber()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUBTNNUMBER);
	int iIndex = pComboBox->GetCurSel();
	int iNumber = pComboBox->GetItemData(iIndex);
	m_pParamCalculator->m_CalcDatalst.EC2.Format(_T("%02d"), iNumber);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecHornbtn()
{
	GetDlgItemText(IDC_COMBO_ELEC_HORNBTN, m_pParamCalculator->m_CalcDatalst.EC3);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecOuselection()
{
	GetDlgItemText(IDC_COMBO_ELEC_OUSELECTION, m_pParamCalculator->m_CalcDatalst.EC4);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecOuconnecttype()
{
	CString strTemp;
	GetDlgItemText(IDC_COMBO_ELEC_OUCONNECTTYPE, strTemp);
	m_pParamCalculator->m_CalcDatalst.EC5.Format(_T("%c%c"), strTemp.GetAt(0), strTemp.GetAt(1));
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPowpowtype()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWPOWTYPE);
	int iIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.ED1 = (TCHAR)pComboBox->GetItemData(iIndex);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPowbracket()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWBRACKET);
	int iIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.ED2 = pComboBox->GetItemData(iIndex);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecPowrailbeamwdt()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWRAILBEAMWDT);
	int iIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.ED3 = (TCHAR)pComboBox->GetItemData(iIndex);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnCbnSelchangeComboElecQty()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_QTY);
	int iIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.ED4 = pComboBox->GetItemData(iIndex);
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::OnBnClickedBtnElecResetkw()
{		
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_QTY);
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboElecQty();
	//
	m_pParamCalculator->m_CalcDatalst.ED51 = m_pParamCalculator->m_CalcDatalst.EA20 + 
										     m_pParamCalculator->m_CalcDatalst.HB215*m_pParamCalculator->m_CalcDatalst.HB216 +
											 m_pParamCalculator->m_CalcDatalst.DB42;
	CString strTemp;
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.ED51);
	SetDlgItemText(IDC_EDIT_ELEC_KW, strTemp);
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

	m_pParamCalculator->m_CalcDatalst.EG1 = GetCranePannelCodeAndPrice();
	SetDlgItemText(IDC_EDIT_ELEC_PANNEL, m_pParamCalculator->m_CalcDatalst.EG1);
	//
	m_pParamCalculator->m_CalcDatalst.EG2 = GetFastoonCodeAndPrice();
	SetDlgItemText(IDC_EDIT_ELEC_FASTOON, m_pParamCalculator->m_CalcDatalst.EG2);
	//
	m_pParamCalculator->m_CalcDatalst.EG3 = GetOperterUnitCodeAndPrice();
	SetDlgItemText(IDC_EDIT_ELEC_OPERUNIT, m_pParamCalculator->m_CalcDatalst.EG3);
	//
	m_pParamCalculator->m_CalcDatalst.EG5 = GetCranePowSupplyCodeAndPrice();
	SetDlgItemText(IDC_EDIT_ELEC_POWER, m_pParamCalculator->m_CalcDatalst.EG5);
	//
	UpdatePrices();
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
BOOL CBMOfferCalcElecDlg::CanDoNextStep()
{
	if (m_pParamCalculator->m_CalcDatalst.EG1.IsEmpty() ||
		m_pParamCalculator->m_CalcDatalst.EG2.IsEmpty() ||
		m_pParamCalculator->m_CalcDatalst.EG3.IsEmpty() ||
		m_pParamCalculator->m_CalcDatalst.EG5.IsEmpty())
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
	OnCbnSelchangeComboElecPannelinverter();
	/*
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELSELECTION);
	iIndex = pComboBox->AddString(_T("00"));
	iIndex = pComboBox->AddString(_T("V2"));
	iIndex = pComboBox->AddString(_T("V3"));
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboElecPannelselection();*/
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELTRANSFORMER);
	iIndex = pComboBox->AddString(_T("S"));
	iIndex = pComboBox->AddString(_T("C"));
	pComboBox->SetCurSel(1);
	OnCbnSelchangeComboElecPanneltransformer();


	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNPWRTYPE);
	iIndex = pComboBox->AddString(_T("Flat Cable"));
	pComboBox->SetItemData(iIndex, int(_T('F')));
	iIndex = pComboBox->AddString(_T("Condoctor"));
	pComboBox->SetItemData(iIndex, int(_T('H')));
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboElecFtnpwrtype();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNCTRLTYPE);
	iIndex = pComboBox->AddString(_T("Pandent"));
	pComboBox->SetItemData(iIndex, int(_T('P')));
	iIndex = pComboBox->AddString(_T("Radio"));
	pComboBox->SetItemData(iIndex, int(_T('R')));
	iIndex = pComboBox->AddString(_T("Bluetooth"));
	pComboBox->SetItemData(iIndex, int(_T('B')));
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboElecFtnctrltype();
	//
/*	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNSELECTION);
	iIndex = pComboBox->AddString(_T("00"));
	iIndex = pComboBox->AddString(_T("V2"));
	iIndex = pComboBox->AddString(_T("V3"));
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboElecFtnselection();*/

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUCTRLTYPE);
	iIndex = pComboBox->AddString(_T("Pandent"));
	pComboBox->SetItemData(iIndex, int(_T('P')));
	iIndex = pComboBox->AddString(_T("Radio-H"));
	pComboBox->SetItemData(iIndex, int(_T('R')));
	iIndex = pComboBox->AddString(_T("Radio-Y"));
	pComboBox->SetItemData(iIndex, int(_T('R')));
	iIndex = pComboBox->AddString(_T("Bluetooth"));
	pComboBox->SetItemData(iIndex, int(_T('B')));
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboElecOuctrltype();
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
	OnCbnSelchangeComboElecOubtnnumber();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_HORNBTN);
	iIndex = pComboBox->AddString(_T("0"));
	iIndex = pComboBox->AddString(_T("1"));
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboElecHornbtn();
	//
/*	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUSELECTION);
	iIndex = pComboBox->AddString(_T("00"));
	iIndex = pComboBox->AddString(_T("V2"));
	iIndex = pComboBox->AddString(_T("V3"));
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboElecOuselection();*/
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUCONNECTTYPE);
	iIndex = pComboBox->AddString(_T("J1-Fastoon"));
	iIndex = pComboBox->AddString(_T("J2-Panel"));
	iIndex = pComboBox->AddString(_T("J3-Hoist"));
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboElecOuconnecttype();

	
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWPOWTYPE);
	iIndex = pComboBox->AddString(_T("Flat Cable"));
	pComboBox->SetItemData(iIndex, int(_T('F')));
	iIndex = pComboBox->AddString(_T("Condoctor"));
	pComboBox->SetItemData(iIndex, int(_T('C')));
	pComboBox->SetCurSel(1);
	OnCbnSelchangeComboElecPowpowtype();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWBRACKET);
	iIndex = pComboBox->AddString(_T("300"));
	pComboBox->SetItemData(iIndex, 3);
	iIndex = pComboBox->AddString(_T("500"));
	pComboBox->SetItemData(iIndex, 5);
	iIndex = pComboBox->AddString(_T("700"));
	pComboBox->SetItemData(iIndex, 7);
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboElecPowbracket();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWRAILBEAMWDT);
	iIndex = pComboBox->AddString(_T("Welded"));
	pComboBox->SetItemData(iIndex, int(_T('W')));
	iIndex = pComboBox->AddString(_T("Clanp"));
	pComboBox->SetItemData(iIndex, int(_T('C')));
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboElecPowrailbeamwdt();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_QTY);
	iIndex = pComboBox->AddString(_T("1"));
	pComboBox->SetItemData(iIndex, 1);
	iIndex = pComboBox->AddString(_T("2"));
	pComboBox->SetItemData(iIndex, 2);
	iIndex = pComboBox->AddString(_T("3"));
	pComboBox->SetItemData(iIndex, 3);
	iIndex = pComboBox->AddString(_T("4"));
	pComboBox->SetItemData(iIndex, 4);
	iIndex = pComboBox->AddString(_T("5"));
	pComboBox->SetItemData(iIndex, 5);
	pComboBox->SetCurSel(0);	
	OnCbnSelchangeComboElecQty();
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::FillPannelSizeComboBox()
{
	int iIndex = 0;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELSIZE);
	pComboBox->ResetContent();
	if (_T("S") == m_pParamCalculator->m_CalcDatalst.HB209)
	{
		iIndex = pComboBox->AddString(_T("5-S"));
		pComboBox->SetItemData(iIndex, 5);
	}
	else
	{
		iIndex = pComboBox->AddString(_T("6-D"));
		pComboBox->SetItemData(iIndex, 6);
		iIndex = pComboBox->AddString(_T("7-D"));
		pComboBox->SetItemData(iIndex, 7);
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::FillSelectionComboBoxes()
{
	CComboBox* pPannelSelComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELSELECTION);
	CComboBox* pFtnComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNSELECTION);
	CComboBox* pOuSelComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUSELECTION);
	pPannelSelComboBox->ResetContent();
	pFtnComboBox->ResetContent();
	pOuSelComboBox->ResetContent();
	if (_T("Z") == m_pParamCalculator->m_CalcDatalst.GA13 ||
		2 == m_pParamCalculator->m_CalcDatalst.HB8)
	{
		pPannelSelComboBox->AddString(_T("00"));
		pPannelSelComboBox->AddString(_T("V2"));
		pPannelSelComboBox->AddString(_T("V3"));
		//
		pFtnComboBox->AddString(_T("00"));
		pFtnComboBox->AddString(_T("V2"));
		pFtnComboBox->AddString(_T("V3"));
		//
		pOuSelComboBox->AddString(_T("00"));
		pOuSelComboBox->AddString(_T("V2"));
		pOuSelComboBox->AddString(_T("V3"));
	}
	else
	{
		pPannelSelComboBox->AddString(_T("00"));
		pFtnComboBox->AddString(_T("00"));
		pOuSelComboBox->AddString(_T("00"));
	}
	pPannelSelComboBox->SetCurSel(0);
	pFtnComboBox->SetCurSel(0);
	pOuSelComboBox->SetCurSel(0);
	OnCbnSelchangeComboElecPannelselection();
	OnCbnSelchangeComboElecFtnselection();
	OnCbnSelchangeComboElecOuselection();
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::InitUIWithDefaultData()
{
	m_pParamCalculator->m_CalcDatalst.EA3 = m_pParamCalculator->m_CalcDatalst.GA1 == _T('BMZ') ? _T("V2") : _T("00");
	m_pParamCalculator->m_CalcDatalst.EB3 = m_pParamCalculator->m_CalcDatalst.GA1 == _T('BMZ') ? _T("V2") : _T("00");
	m_pParamCalculator->m_CalcDatalst.EC2 = m_pParamCalculator->m_CalcDatalst.GA1 == _T('BMZ') ? _T("10") : _T("08");
	m_pParamCalculator->m_CalcDatalst.EC4 = m_pParamCalculator->m_CalcDatalst.GA1 == _T('BMZ') ? _T("V2") : _T("00");
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELSIZE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EA2);
	OnCbnSelchangeComboElecPannelsize();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELSELECTION);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EA3);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNSELECTION);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EB3);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUBTNNUMBER);
	SelectComboBoxItem(pComboBox, _ttol(m_pParamCalculator->m_CalcDatalst.EC2));
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUSELECTION);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EC4);
	//
	OnBnClickedBtnElecResetkw();
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

	m_pParamCalculator->UpdateElecCodeData();

	CComboBox* pComboBox = NULL;

	// Crane panel
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EA1);
	pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ELEC_PANNELSIZE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EA2);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELSELECTION);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EA3);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_PANNELTRANSFORMER);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EA4);

	// Fastoon
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EB1);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNCTRLTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EB2);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_FTNSELECTION);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EB3);

	// Operter unit
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUCTRLTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EC1);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUBTNNUMBER);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EC2);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_HORNBTN);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EC3);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUSELECTION);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EC4);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_OUCONNECTTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.EC5);

	// Crane power supply	
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWPOWTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.ED1);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWBRACKET);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.ED2);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEC_POWRAILBEAMWDT);
	SelectComboBoxItem(pComboBox,m_pParamCalculator->m_CalcDatalst.ED3);

	/*	Set code and price to UI*/
	SetDlgItemText(IDC_EDIT_ELEC_PANNEL, m_pParamCalculator->m_CalcDatalst.EG1);
	SetDlgItemText(IDC_EDIT_ELEC_FASTOON, m_pParamCalculator->m_CalcDatalst.EG2);
	SetDlgItemText(IDC_EDIT_ELEC_OPERUNIT, m_pParamCalculator->m_CalcDatalst.EG3);
	SetDlgItemText(IDC_EDIT_ELEC_POWER, m_pParamCalculator->m_CalcDatalst.EG5);
	//
	CString strPrice;
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EH1);
	m_staticPannelPrice.SetText(strPrice);
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EH2);
	m_staticFastoonPrice.SetText(strPrice);
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EH3);
	m_staticOperterUnitPrice.SetText(strPrice);
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EH5);
	m_staticPowerSupplyPrice.SetText(strPrice);
	//
	UpdatePrices();
}
//-----------------------------------------------------------------------
void CBMOfferCalcElecDlg::UpdatePrices()
{
	CString strPrice;

	// Get all additions price
	m_pParamCalculator->m_CalcDatalst.EF1 = 0;
	m_pParamCalculator->m_CalcDatalst.EF2 = 0;
	m_pParamCalculator->m_CalcDatalst.EF3 = 0;
	m_pParamCalculator->m_CalcDatalst.EF4 = 0;
	int iSelAddCnt = m_lstCtlAdditions.GetItemCount();
	for (int iIndex = 0; iIndex < iSelAddCnt; iIndex++)
	{
		CString strName;
		CString strPrice;
		strName = m_lstCtlAdditions.GetItemText(iIndex, 1);
		strPrice = m_lstCtlAdditions.GetItemText(iIndex, 2);
		if (strName.Find(_T("EFS")) != -1)
		{
			m_pParamCalculator->m_CalcDatalst.EF1 += _ttol(strPrice);
		}
		else if (strName.Find(_T("EOP")) != -1)
		{
			m_pParamCalculator->m_CalcDatalst.EF2 += _ttol(strPrice);
		}
		else if (strName.Find(_T("ECP")) != -1)
		{
			m_pParamCalculator->m_CalcDatalst.EF3 += _ttol(strPrice);
		}
		else
		{
			m_pParamCalculator->m_CalcDatalst.EF4 += _ttol(strPrice);
		}
	}
	m_pParamCalculator->m_iELAdditionsPrice = m_pParamCalculator->m_CalcDatalst.EF1 + 
											  m_pParamCalculator->m_CalcDatalst.EF2 + 
											  m_pParamCalculator->m_CalcDatalst.EF3 + 
											  m_pParamCalculator->m_CalcDatalst.EF4;
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EF1);
	m_staticAddPriceEL.SetText(strPrice);
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EF2);
	m_staticAddPriceEFS.SetText(strPrice);
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EF3);
	m_staticAddPriceEOP.SetText(strPrice);
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EF4);
	m_staticAddPriceECP.SetText(strPrice);
	//
	m_pParamCalculator->m_CalcDatalst.EH1 = m_pParamCalculator->m_CalcDatalst.EA29 + m_pParamCalculator->m_CalcDatalst.EF1;
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EH1);
	m_staticPannelPrice.SetText(strPrice);
	//
	m_pParamCalculator->m_CalcDatalst.EH2 = m_pParamCalculator->m_CalcDatalst.EB34 + m_pParamCalculator->m_CalcDatalst.EF2;
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EH2);
	m_staticFastoonPrice.SetText(strPrice);
	//
	m_pParamCalculator->m_CalcDatalst.EH3  = m_pParamCalculator->m_CalcDatalst.EC55 + m_pParamCalculator->m_CalcDatalst.EF3;
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EH3);
	m_staticOperterUnitPrice.SetText(strPrice);
	//
	m_pParamCalculator->m_CalcDatalst.EH5 = m_pParamCalculator->m_CalcDatalst.ED56 + m_pParamCalculator->m_CalcDatalst.EF4;
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EH5);
	m_staticPowerSupplyPrice.SetText(strPrice);
	//
	m_pParamCalculator->m_CalcDatalst.EH6 = m_pParamCalculator->m_CalcDatalst.EH1 +
											m_pParamCalculator->m_CalcDatalst.EH2 +
											m_pParamCalculator->m_CalcDatalst.EH3 +
											m_pParamCalculator->m_CalcDatalst.EH5;
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.EH6);
	m_staticTotalPrice.SetText(strPrice);
}
//-----------------------------------------------------------------------
CString	CBMOfferCalcElecDlg::GetEA2()
{
	CString strEA2;

	if (_T("S") == m_pParamCalculator->m_CalcDatalst.HB209)
	{
		strEA2 = _T("5-S");
	}
	else
	{
		if (m_pParamCalculator->m_CalcDatalst.EA20 <= 9.8)
		{
			strEA2 = _T("6-D");
		}
		else
		{
			strEA2 = _T("7-D");
		}
	}

	return strEA2;
}
//-----------------------------------------------------------------------
CString	CBMOfferCalcElecDlg::GetEA21()
{
	CString strEA21 = _T("PA");

	if (m_pParamCalculator->m_CalcDatalst.EA20 <= 3.2f)
	{
		strEA21 = _T("PA");
	}
	else if (m_pParamCalculator->m_CalcDatalst.EA20 <= 6.1f)
	{
		strEA21 = _T("PB");
	}
	else if (m_pParamCalculator->m_CalcDatalst.EA20 <= 9.8f)
	{
		strEA21 = _T("PC");
	}
	else if (m_pParamCalculator->m_CalcDatalst.EA20 <= 16.0f)
	{
		strEA21 = _T("PD");
	}
	else if (m_pParamCalculator->m_CalcDatalst.EA20 <= 20.0f)
	{
		strEA21 = _T("PE");
	}
	else if (m_pParamCalculator->m_CalcDatalst.EA20 <= 38.0f)
	{
		strEA21 = _T("PF");
	}
	else
	{
		strEA21 = _T("PG");
	}	

	return strEA21;
}
//-----------------------------------------------------------------------
CString CBMOfferCalcElecDlg::GetEA24()
{
	CString strEA24 = _T("T1");

	if (m_pParamCalculator->m_CalcDatalst.DB35 <= 0.31f)		//0-0.31
	{
		strEA24 = _T("T1");
	}
	else if(m_pParamCalculator->m_CalcDatalst.DB35 <= 0.44f)	//0.32-0.44
	{
		strEA24 = _T("T2");
	}
	else if(m_pParamCalculator->m_CalcDatalst.DB35 <= 0.96f)	//0.45-0.96
	{
		strEA24 = _T("T3");
	}
	else if(m_pParamCalculator->m_CalcDatalst.DB35 <= 1.3f)	//0.97-1.3
	{
		strEA24 = _T("T4");
	}
	else if(m_pParamCalculator->m_CalcDatalst.DB35 <= 1.9f)	//1.31-1.9
	{
		strEA24 = _T("T5");
	}
	else if(m_pParamCalculator->m_CalcDatalst.DB35 <= 3.0f)	//1.91-3.0
	{
		strEA24 = _T("T6");
	}
	else						//>3.0
	{
		strEA24 = _T("T7");
	}

	return strEA24;
}
//-----------------------------------------------------------------------
CString CBMOfferCalcElecDlg::GetCranePannelCodeAndPrice()
{
	CString strCode;

	strCode.Format(	_T("DXC%s%s%s%d%s%d%s%d%s%d%s%s%d"), 
					m_pParamCalculator->m_CalcDatalst.GA13, 
					m_pParamCalculator->m_CalcDatalst.HB205, 
					m_pParamCalculator->m_CalcDatalst.EA22, 
					m_pParamCalculator->m_CalcDatalst.HB8, 
					m_pParamCalculator->m_CalcDatalst.EA24, 
					m_pParamCalculator->m_CalcDatalst.DB4, 
					m_pParamCalculator->m_CalcDatalst.GC61, 
					m_pParamCalculator->m_CalcDatalst.GC31, 
					m_pParamCalculator->m_CalcDatalst.EA1, 
					m_pParamCalculator->m_CalcDatalst.EA23, 
					m_pParamCalculator->m_CalcDatalst.EA3, 
					m_pParamCalculator->m_CalcDatalst.EA4, 
					m_pParamCalculator->m_CalcDatalst.GB4);

	m_pParamCalculator->m_CalcDatalst.EA25.Format(_T("%s%s%d"),
								 				  m_pParamCalculator->m_CalcDatalst.GA12,
								 				  m_pParamCalculator->m_CalcDatalst.EA21,
								 				  m_pParamCalculator->m_CalcDatalst.HB8);
	m_pParamCalculator->m_CalcDatalst.EA27.Format(_T("%s%d%s"),
								 				  m_pParamCalculator->m_CalcDatalst.EA24,
								 				  m_pParamCalculator->m_CalcDatalst.DB4,
								 				  m_pParamCalculator->m_CalcDatalst.EA1);

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE PanCod='%s'"), g_tcTableName[eTableID_CraPanPri1], m_pParamCalculator->m_CalcDatalst.EA25);
	m_pParamCalculator->m_CalcDatalst.EA26 = ::GetLONGValueFromTable(strSQL, _T("CraPanPri1"));
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE DriCod='%s'"), g_tcTableName[eTableID_CraPanPri2], m_pParamCalculator->m_CalcDatalst.EA27);
	m_pParamCalculator->m_CalcDatalst.EA28 = ::GetLONGValueFromTable(strSQL, _T("CraPanPri2"));
	m_pParamCalculator->m_CalcDatalst.EA29 = m_pParamCalculator->m_CalcDatalst.EA26 + m_pParamCalculator->m_CalcDatalst.EA28;

	if (m_pParamCalculator->m_CalcDatalst.EA26 <= 0 || m_pParamCalculator->m_CalcDatalst.EA28 <= 0)
	{
		MessageBox(_T("One of pannel price is error!"), _T("Error"), MB_OK | MB_ICONERROR);
	}

	return strCode;
}
//-----------------------------------------------------------------------
CString CBMOfferCalcElecDlg::GetFastoonCodeAndPrice()
{
	CString strCode;

	strCode.Format(	_T("PST%s%s%s%d%d%s%s%d%s%s"), 
					m_pParamCalculator->m_CalcDatalst.GA13, 
					m_pParamCalculator->m_CalcDatalst.GA51, 
					m_pParamCalculator->m_CalcDatalst.EB1, 
					m_pParamCalculator->m_CalcDatalst.HB8, 
					m_pParamCalculator->m_CalcDatalst.MA2, 
					m_pParamCalculator->m_CalcDatalst.HB205, 
					m_pParamCalculator->m_CalcDatalst.EA22, 
					m_pParamCalculator->m_CalcDatalst.GB4, 
					m_pParamCalculator->m_CalcDatalst.EB2, 
					m_pParamCalculator->m_CalcDatalst.EB3);
/*
	m_pParamCalculator->m_CalcDatalst.EB31.Format(_T("%s%s%d%s%s"),
								 				  m_pParamCalculator->m_CalcDatalst.HB209,
								 				  m_pParamCalculator->m_CalcDatalst.EA21,
								 				  m_pParamCalculator->m_CalcDatalst.HB8,
								 				  m_pParamCalculator->m_CalcDatalst.EB2,												  
								 				  m_pParamCalculator->m_CalcDatalst.EB1);

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE FasCod='%s'"), g_tcTableName[eTableID_FasPri], m_pParamCalculator->m_CalcDatalst.EB31);
	m_pParamCalculator->m_CalcDatalst.EB32 = ::GetLONGValueFromTable(strSQL, _T("FixPri"));
	m_pParamCalculator->m_CalcDatalst.EB33 = ::GetLONGValueFromTable(strSQL, _T("MetPri"));
	m_pParamCalculator->m_CalcDatalst.EB34 = m_pParamCalculator->m_CalcDatalst.EB32 + m_pParamCalculator->m_CalcDatalst.EB33 * m_pParamCalculator->m_CalcDatalst.GA5;
*/

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE Code='%s'"), g_tcTableName[eTableID_Powlist], m_pParamCalculator->m_CalcDatalst.EA21);
	m_pParamCalculator->m_CalcDatalst.FS01 =  _T("YDL1") + GetStringValueFromTable(strSQL, _T("Cable"));
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS01);
	m_pParamCalculator->m_CalcDatalst.FS011 =  GetLONGValueFromTable(strSQL, _T("Price"));
	//
	if (_T("S") == m_pParamCalculator->m_CalcDatalst.GA12) m_pParamCalculator->m_CalcDatalst.FS012 = 6;
	else m_pParamCalculator->m_CalcDatalst.FS012 = 8;
	
	if ( m_pParamCalculator->m_CalcDatalst.DB35 >= 2.2f) m_pParamCalculator->m_CalcDatalst.FS02 =  _T("YDL26*2.5");
	else m_pParamCalculator->m_CalcDatalst.FS02 =  _T("YDL26*1.5");
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS02);
	m_pParamCalculator->m_CalcDatalst.FS021 =  GetLONGValueFromTable(strSQL, _T("Price"));
	//
	if (2 == m_pParamCalculator->m_CalcDatalst.DB4) 
		m_pParamCalculator->m_CalcDatalst.FS022 = int((m_pParamCalculator->m_CalcDatalst.GA5 * 1.1 + 11) + 0.99f);
	else 
	{
		if (_T("S") == m_pParamCalculator->m_CalcDatalst.GA12) m_pParamCalculator->m_CalcDatalst.FS022 = int(m_pParamCalculator->m_CalcDatalst.GA5 * 2.2 + 15 + 0.99f);
		else m_pParamCalculator->m_CalcDatalst.FS022 = int(m_pParamCalculator->m_CalcDatalst.GA5 * 2.2 + 20 + 0.99f);
	}

	CString strFs03Code;
	if (_T("V3") == m_pParamCalculator->m_CalcDatalst.EA3) 
	{
		strFs03Code =  m_pParamCalculator->m_CalcDatalst.EA21;
	}
	else
	{
		if (m_pParamCalculator->m_CalcDatalst.HB217 <= 3.2f)
		{
			strFs03Code = _T("PA");
		}
		else if (m_pParamCalculator->m_CalcDatalst.HB217 <= 6.1f)
		{
			strFs03Code = _T("PB");
		}
		else if (m_pParamCalculator->m_CalcDatalst.HB217 <= 9.8f)
		{
			strFs03Code = _T("PC");
		}
		else if (m_pParamCalculator->m_CalcDatalst.HB217 <= 16.0f)
		{
			strFs03Code = _T("PD");
		}
		else if (m_pParamCalculator->m_CalcDatalst.HB217 <= 20.0f)
		{
			strFs03Code = _T("PE");
		}
		else if (m_pParamCalculator->m_CalcDatalst.HB217 <= 38.0f)
		{
			strFs03Code = _T("PF");
		}
		else
		{
			strFs03Code = _T("PG");
		}	
	}	
	strSQL.Format(_T("SELECT * FROM %s WHERE Code='%s'"), g_tcTableName[eTableID_Powlist], strFs03Code);
	m_pParamCalculator->m_CalcDatalst.FS03 =  _T("BDL1") + GetStringValueFromTable(strSQL, _T("Cable"));
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS03);
	m_pParamCalculator->m_CalcDatalst.FS031 =  GetLONGValueFromTable(strSQL, _T("Price"));
	//
	if (_T("S") == m_pParamCalculator->m_CalcDatalst.GA12) m_pParamCalculator->m_CalcDatalst.FS032 = int((m_pParamCalculator->m_CalcDatalst.GA5*1.2 + 3) * m_pParamCalculator->m_CalcDatalst.HB8 + 0.99f);
	else m_pParamCalculator->m_CalcDatalst.FS032 = int((m_pParamCalculator->m_CalcDatalst.GA5*1.2 + 10) * m_pParamCalculator->m_CalcDatalst.HB8 + 0.99f);
	if (2 == m_pParamCalculator->m_CalcDatalst.HB8) m_pParamCalculator->m_CalcDatalst.FS032 += m_pParamCalculator->m_CalcDatalst.GA5;

	if (_T("V3") == m_pParamCalculator->m_CalcDatalst.EA3) m_pParamCalculator->m_CalcDatalst.FS04 = _T("BDL216*1.5");
	else m_pParamCalculator->m_CalcDatalst.FS04 = _T("BDL210*1.5");
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS04);
	m_pParamCalculator->m_CalcDatalst.FS041 =  GetLONGValueFromTable(strSQL, _T("Price"));
	m_pParamCalculator->m_CalcDatalst.FS042 = m_pParamCalculator->m_CalcDatalst.FS032;

	m_pParamCalculator->m_CalcDatalst.FS05 = _T("BDL316*1.5");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS05);
	m_pParamCalculator->m_CalcDatalst.FS051 =  GetLONGValueFromTable(strSQL, _T("Price"));
	if (_T("S") == m_pParamCalculator->m_CalcDatalst.GA12) m_pParamCalculator->m_CalcDatalst.FS052 = int(m_pParamCalculator->m_CalcDatalst.GA5*1.2 + 3 + 0.99f);
	else m_pParamCalculator->m_CalcDatalst.FS052 = int(m_pParamCalculator->m_CalcDatalst.GA5*1.2 + 5 + 0.99f);

	CString strTemp069;
	if (2 == m_pParamCalculator->m_CalcDatalst.HB8) 
		m_pParamCalculator->m_CalcDatalst.FS06 = _T("HCTC40");
	else
	{
		if (m_pParamCalculator->m_CalcDatalst.EA20 >= 20)
		{
			strTemp069 = _T("C40");
			m_pParamCalculator->m_CalcDatalst.FS06 = _T("HCTC40");
		}
		else
		{
			strTemp069 = _T("C30");
			m_pParamCalculator->m_CalcDatalst.FS06 = _T("HCTC30");
		}
	}
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS06);
	m_pParamCalculator->m_CalcDatalst.FS061 =  GetLONGValueFromTable(strSQL, _T("Price"));
	//
	if (_T("J1") == m_pParamCalculator->m_CalcDatalst.EC5) m_pParamCalculator->m_CalcDatalst.FS062 = 1;
	else m_pParamCalculator->m_CalcDatalst.FS062 = 0;

	strSQL.Format(_T("SELECT * FROM %s WHERE Code='%s%d'"), g_tcTableName[eTableID_Powlist], m_pParamCalculator->m_CalcDatalst.GA12, m_pParamCalculator->m_CalcDatalst.HB8);
	m_pParamCalculator->m_CalcDatalst.FS07.Format(_T("CR1%s*%s"), strTemp069, GetStringValueFromTable(strSQL, _T("PowList")));
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS07);
	m_pParamCalculator->m_CalcDatalst.FS071 =  GetLONGValueFromTable(strSQL, _T("Price"));
	//
	m_pParamCalculator->m_CalcDatalst.FS072 = int((m_pParamCalculator->m_CalcDatalst.GA5 / 2 + 3) + 0.99f);

	m_pParamCalculator->m_CalcDatalst.FS08 = _T("CR1") + strTemp069 + _T("*4000");
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS08);
	m_pParamCalculator->m_CalcDatalst.FS081 =  GetLONGValueFromTable(strSQL, _T("Price"));
	//
	m_pParamCalculator->m_CalcDatalst.FS082 = int(((m_pParamCalculator->m_CalcDatalst.GA5 / 4 + 1) *(m_pParamCalculator->m_CalcDatalst.HB8 + 1)) + 0.99f);

	m_pParamCalculator->m_CalcDatalst.FS09 = _T("CR2") + strTemp069;
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS09);
	m_pParamCalculator->m_CalcDatalst.FS091 =  GetLONGValueFromTable(strSQL, _T("Price"));
	//
	m_pParamCalculator->m_CalcDatalst.FS092 = int((m_pParamCalculator->m_CalcDatalst.GA5 / 1.5 + 1) * (m_pParamCalculator->m_CalcDatalst.HB8 + 1) + 0.99f);

	m_pParamCalculator->m_CalcDatalst.FS10 = _T("CR3") + strTemp069;
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS10);
	m_pParamCalculator->m_CalcDatalst.FS101 =  GetLONGValueFromTable(strSQL, _T("Price"));
	//
	m_pParamCalculator->m_CalcDatalst.FS102 = m_pParamCalculator->m_CalcDatalst.FS082;

	m_pParamCalculator->m_CalcDatalst.FS11 = _T("CR4") + strTemp069;
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS11);
	m_pParamCalculator->m_CalcDatalst.FS111 =  GetLONGValueFromTable(strSQL, _T("Price"));
	//
	m_pParamCalculator->m_CalcDatalst.FS112 = m_pParamCalculator->m_CalcDatalst.FS072 * (m_pParamCalculator->m_CalcDatalst.HB8 + 1) + 1;

	m_pParamCalculator->m_CalcDatalst.FS12 = _T("CR6") + strTemp069;
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS12);
	m_pParamCalculator->m_CalcDatalst.FS121 =  GetLONGValueFromTable(strSQL, _T("Price"));
	//
	m_pParamCalculator->m_CalcDatalst.FS122 = m_pParamCalculator->m_CalcDatalst.HB8 + 1;

	m_pParamCalculator->m_CalcDatalst.FS13 = _T("CR7") + strTemp069;
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS13);
	m_pParamCalculator->m_CalcDatalst.FS131 =  GetLONGValueFromTable(strSQL, _T("Price"));
	//
	m_pParamCalculator->m_CalcDatalst.FS132 = 2*(m_pParamCalculator->m_CalcDatalst.HB8 + 1);

	
	if (_T("S") == m_pParamCalculator->m_CalcDatalst.GA12) m_pParamCalculator->m_CalcDatalst.FS14 = _T("DDJA");
	else 
	{
		if (2 == m_pParamCalculator->m_CalcDatalst.HB8) m_pParamCalculator->m_CalcDatalst.FS14 = _T("DDJC");
		m_pParamCalculator->m_CalcDatalst.FS14 = _T("DDJB");
	}
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS14);
	m_pParamCalculator->m_CalcDatalst.FS141 =  GetLONGValueFromTable(strSQL, _T("Price"));
	//
	m_pParamCalculator->m_CalcDatalst.FS142 = m_pParamCalculator->m_CalcDatalst.HB8;

	
	if (m_pParamCalculator->m_CalcDatalst.EA20 > 20) m_pParamCalculator->m_CalcDatalst.FS15 = _T("DDJY");
	else m_pParamCalculator->m_CalcDatalst.FS15 = _T("DDJX");
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS15);
	m_pParamCalculator->m_CalcDatalst.FS151 =  GetLONGValueFromTable(strSQL, _T("Price"));
	m_pParamCalculator->m_CalcDatalst.FS152 = 1;

	m_pParamCalculator->m_CalcDatalst.FS16 = _T("XWQD");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS16);
	m_pParamCalculator->m_CalcDatalst.FS161 =  GetLONGValueFromTable(strSQL, _T("Price"));
	m_pParamCalculator->m_CalcDatalst.FS162 = 1;

	m_pParamCalculator->m_CalcDatalst.FS17 = _T("XBGS");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS17);
	m_pParamCalculator->m_CalcDatalst.FS171 =  GetLONGValueFromTable(strSQL, _T("Price"));
	m_pParamCalculator->m_CalcDatalst.FS172 = 2;


	m_pParamCalculator->m_CalcDatalst.FS18 = _T("XWJD");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS18);
	m_pParamCalculator->m_CalcDatalst.FS181 =  GetLONGValueFromTable(strSQL, _T("Price"));
	m_pParamCalculator->m_CalcDatalst.FS182 = 1;

	m_pParamCalculator->m_CalcDatalst.FS19 = _T("CR1C40*2000");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS19);
	m_pParamCalculator->m_CalcDatalst.FS191 =  GetLONGValueFromTable(strSQL, _T("Price"));
	if (_T("S") == m_pParamCalculator->m_CalcDatalst.GA12) m_pParamCalculator->m_CalcDatalst.FS192 = 2;
	m_pParamCalculator->m_CalcDatalst.FS192 = 0;

	m_pParamCalculator->m_CalcDatalst.FS20 = _T("CR5C40");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS20);
	m_pParamCalculator->m_CalcDatalst.FS201 =  GetLONGValueFromTable(strSQL, _T("Price"));
	m_pParamCalculator->m_CalcDatalst.FS202 = 2 * m_pParamCalculator->m_CalcDatalst.FS192;

	m_pParamCalculator->m_CalcDatalst.FS21 = _T("CR9") + strTemp069;
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS21);
	m_pParamCalculator->m_CalcDatalst.FS211 =  GetLONGValueFromTable(strSQL, _T("Price"));
	m_pParamCalculator->m_CalcDatalst.FS212 = int(2 * m_pParamCalculator->m_CalcDatalst.FS072 * m_pParamCalculator->m_CalcDatalst.FS192 / 2.0f + 0.99f);

	m_pParamCalculator->m_CalcDatalst.FS22 = _T("CR5") + strTemp069;
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS22);
	m_pParamCalculator->m_CalcDatalst.FS221 =  GetLONGValueFromTable(strSQL, _T("Price"));
	m_pParamCalculator->m_CalcDatalst.FS222 = int((2 * m_pParamCalculator->m_CalcDatalst.FS072 + 2) * m_pParamCalculator->m_CalcDatalst.FS192 / 2.0f + 0.99f);

	m_pParamCalculator->m_CalcDatalst.FS23 = _T("XCZD");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS23);
	m_pParamCalculator->m_CalcDatalst.FS231 =  GetLONGValueFromTable(strSQL, _T("Price"));
	m_pParamCalculator->m_CalcDatalst.FS232 = m_pParamCalculator->m_CalcDatalst.FS192;

	m_pParamCalculator->m_CalcDatalst.FS24 = _T("CR8") + strTemp069;
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS24);
	m_pParamCalculator->m_CalcDatalst.FS241 =  GetLONGValueFromTable(strSQL, _T("Price"));
	if (_T("S") == m_pParamCalculator->m_CalcDatalst.GA12) m_pParamCalculator->m_CalcDatalst.FS242 = 0;
		m_pParamCalculator->m_CalcDatalst.FS242 = m_pParamCalculator->m_CalcDatalst.FS072;

	m_pParamCalculator->m_CalcDatalst.FS25 = _T("DXC1");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS25);
	m_pParamCalculator->m_CalcDatalst.FS251 =  GetLONGValueFromTable(strSQL, _T("Price"));
	if (_T("S") == m_pParamCalculator->m_CalcDatalst.GA12) m_pParamCalculator->m_CalcDatalst.FS252 = 0;
		m_pParamCalculator->m_CalcDatalst.FS252 = 2;

	m_pParamCalculator->m_CalcDatalst.FS26 = _T("DXC2");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS26);
	m_pParamCalculator->m_CalcDatalst.FS261 =  GetLONGValueFromTable(strSQL, _T("Price"));
	if (_T("S") == m_pParamCalculator->m_CalcDatalst.GA12) m_pParamCalculator->m_CalcDatalst.FS262 = 0;
		m_pParamCalculator->m_CalcDatalst.FS262 = int((m_pParamCalculator->m_CalcDatalst.GA5 / 3 + 2) + 0.99f);

	m_pParamCalculator->m_CalcDatalst.FS27 = _T("DXC3");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS27);
	m_pParamCalculator->m_CalcDatalst.FS271 =  GetLONGValueFromTable(strSQL, _T("Price"));
	if (_T("S") == m_pParamCalculator->m_CalcDatalst.GA12) m_pParamCalculator->m_CalcDatalst.FS272 = 0;
		m_pParamCalculator->m_CalcDatalst.FS272 = 6;

	m_pParamCalculator->m_CalcDatalst.FS28 = _T("JXFJ");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS28);
	m_pParamCalculator->m_CalcDatalst.FS281 =  GetLONGValueFromTable(strSQL, _T("Price"));
	m_pParamCalculator->m_CalcDatalst.FS282 = 1;

	m_pParamCalculator->m_CalcDatalst.FS29 = _T("DJFJ");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS29);
	m_pParamCalculator->m_CalcDatalst.FS291 =  GetLONGValueFromTable(strSQL, _T("Price"));
	m_pParamCalculator->m_CalcDatalst.FS292 = 1;

	m_pParamCalculator->m_CalcDatalst.FS30 = _T("DQFJ");
	strSQL.Format(_T("SELECT * FROM %s WHERE Code1='%s'"), g_tcTableName[eTableID_FasPri1], m_pParamCalculator->m_CalcDatalst.FS30);
	m_pParamCalculator->m_CalcDatalst.FS301 =  GetLONGValueFromTable(strSQL, _T("Price"));
	m_pParamCalculator->m_CalcDatalst.FS302 = 1;

	m_pParamCalculator->m_CalcDatalst.FS013 = m_pParamCalculator->m_CalcDatalst.FS011 * m_pParamCalculator->m_CalcDatalst.FS012;
	m_pParamCalculator->m_CalcDatalst.FS023 = m_pParamCalculator->m_CalcDatalst.FS021 * m_pParamCalculator->m_CalcDatalst.FS022;
	m_pParamCalculator->m_CalcDatalst.FS033 = m_pParamCalculator->m_CalcDatalst.FS031 * m_pParamCalculator->m_CalcDatalst.FS032;
	m_pParamCalculator->m_CalcDatalst.FS043 = m_pParamCalculator->m_CalcDatalst.FS041 * m_pParamCalculator->m_CalcDatalst.FS042;
	m_pParamCalculator->m_CalcDatalst.FS053 = m_pParamCalculator->m_CalcDatalst.FS051 * m_pParamCalculator->m_CalcDatalst.FS052;
	m_pParamCalculator->m_CalcDatalst.FS063 = m_pParamCalculator->m_CalcDatalst.FS061 * m_pParamCalculator->m_CalcDatalst.FS062;
	m_pParamCalculator->m_CalcDatalst.FS073 = m_pParamCalculator->m_CalcDatalst.FS071 * m_pParamCalculator->m_CalcDatalst.FS072;
	m_pParamCalculator->m_CalcDatalst.FS083 = m_pParamCalculator->m_CalcDatalst.FS081 * m_pParamCalculator->m_CalcDatalst.FS082;
	m_pParamCalculator->m_CalcDatalst.FS093 = m_pParamCalculator->m_CalcDatalst.FS091 * m_pParamCalculator->m_CalcDatalst.FS092;
	m_pParamCalculator->m_CalcDatalst.FS103 = m_pParamCalculator->m_CalcDatalst.FS101 * m_pParamCalculator->m_CalcDatalst.FS102;	
	m_pParamCalculator->m_CalcDatalst.FS113 = m_pParamCalculator->m_CalcDatalst.FS111 * m_pParamCalculator->m_CalcDatalst.FS112;
	m_pParamCalculator->m_CalcDatalst.FS123 = m_pParamCalculator->m_CalcDatalst.FS121 * m_pParamCalculator->m_CalcDatalst.FS122;
	m_pParamCalculator->m_CalcDatalst.FS133 = m_pParamCalculator->m_CalcDatalst.FS131 * m_pParamCalculator->m_CalcDatalst.FS132;
	m_pParamCalculator->m_CalcDatalst.FS143 = m_pParamCalculator->m_CalcDatalst.FS141 * m_pParamCalculator->m_CalcDatalst.FS142;
	m_pParamCalculator->m_CalcDatalst.FS153 = m_pParamCalculator->m_CalcDatalst.FS151 * m_pParamCalculator->m_CalcDatalst.FS152;
	m_pParamCalculator->m_CalcDatalst.FS163 = m_pParamCalculator->m_CalcDatalst.FS161 * m_pParamCalculator->m_CalcDatalst.FS162;
	m_pParamCalculator->m_CalcDatalst.FS173 = m_pParamCalculator->m_CalcDatalst.FS171 * m_pParamCalculator->m_CalcDatalst.FS172;
	m_pParamCalculator->m_CalcDatalst.FS183 = m_pParamCalculator->m_CalcDatalst.FS181 * m_pParamCalculator->m_CalcDatalst.FS182;
	m_pParamCalculator->m_CalcDatalst.FS193 = m_pParamCalculator->m_CalcDatalst.FS191 * m_pParamCalculator->m_CalcDatalst.FS192;
	m_pParamCalculator->m_CalcDatalst.FS203 = m_pParamCalculator->m_CalcDatalst.FS201 * m_pParamCalculator->m_CalcDatalst.FS202;	
	m_pParamCalculator->m_CalcDatalst.FS213 = m_pParamCalculator->m_CalcDatalst.FS211 * m_pParamCalculator->m_CalcDatalst.FS212;
	m_pParamCalculator->m_CalcDatalst.FS223 = m_pParamCalculator->m_CalcDatalst.FS221 * m_pParamCalculator->m_CalcDatalst.FS222;
	m_pParamCalculator->m_CalcDatalst.FS233 = m_pParamCalculator->m_CalcDatalst.FS231 * m_pParamCalculator->m_CalcDatalst.FS232;
	m_pParamCalculator->m_CalcDatalst.FS243 = m_pParamCalculator->m_CalcDatalst.FS241 * m_pParamCalculator->m_CalcDatalst.FS242;
	m_pParamCalculator->m_CalcDatalst.FS253 = m_pParamCalculator->m_CalcDatalst.FS251 * m_pParamCalculator->m_CalcDatalst.FS252;
	m_pParamCalculator->m_CalcDatalst.FS263 = m_pParamCalculator->m_CalcDatalst.FS261 * m_pParamCalculator->m_CalcDatalst.FS262;
	m_pParamCalculator->m_CalcDatalst.FS273 = m_pParamCalculator->m_CalcDatalst.FS271 * m_pParamCalculator->m_CalcDatalst.FS272;
	m_pParamCalculator->m_CalcDatalst.FS283 = m_pParamCalculator->m_CalcDatalst.FS281 * m_pParamCalculator->m_CalcDatalst.FS282;
	m_pParamCalculator->m_CalcDatalst.FS293 = m_pParamCalculator->m_CalcDatalst.FS291 * m_pParamCalculator->m_CalcDatalst.FS292;
	m_pParamCalculator->m_CalcDatalst.FS303 = m_pParamCalculator->m_CalcDatalst.FS301 * m_pParamCalculator->m_CalcDatalst.FS302;


	m_pParamCalculator->m_CalcDatalst.EB34 =	m_pParamCalculator->m_CalcDatalst.FS013 + 
												m_pParamCalculator->m_CalcDatalst.FS023 + 
												m_pParamCalculator->m_CalcDatalst.FS033 + 
												m_pParamCalculator->m_CalcDatalst.FS043 + 
												m_pParamCalculator->m_CalcDatalst.FS053 + 
												m_pParamCalculator->m_CalcDatalst.FS063 + 
												m_pParamCalculator->m_CalcDatalst.FS073 + 
												m_pParamCalculator->m_CalcDatalst.FS083 + 
												m_pParamCalculator->m_CalcDatalst.FS093 + 
												m_pParamCalculator->m_CalcDatalst.FS103 + 
												m_pParamCalculator->m_CalcDatalst.FS113 + 
												m_pParamCalculator->m_CalcDatalst.FS123 + 
												m_pParamCalculator->m_CalcDatalst.FS133 + 
												m_pParamCalculator->m_CalcDatalst.FS143 + 
												m_pParamCalculator->m_CalcDatalst.FS153 + 
												m_pParamCalculator->m_CalcDatalst.FS163 + 
												m_pParamCalculator->m_CalcDatalst.FS173 + 
												m_pParamCalculator->m_CalcDatalst.FS183 + 
												m_pParamCalculator->m_CalcDatalst.FS193 + 
												m_pParamCalculator->m_CalcDatalst.FS203 + 
												m_pParamCalculator->m_CalcDatalst.FS213 + 
												m_pParamCalculator->m_CalcDatalst.FS223 + 
												m_pParamCalculator->m_CalcDatalst.FS233 + 
												m_pParamCalculator->m_CalcDatalst.FS243 + 
												m_pParamCalculator->m_CalcDatalst.FS253 + 
												m_pParamCalculator->m_CalcDatalst.FS263 + 
												m_pParamCalculator->m_CalcDatalst.FS273 + 
												m_pParamCalculator->m_CalcDatalst.FS283 + 
												m_pParamCalculator->m_CalcDatalst.FS293 + 
												m_pParamCalculator->m_CalcDatalst.FS303;


	return strCode;
}
//-----------------------------------------------------------------------
CString CBMOfferCalcElecDlg::GetOperterUnitCodeAndPrice()
{
	CString strCode;

	strCode.Format(	_T("%s-SCP%s%s%s%s%s"), 
					m_pParamCalculator->m_CalcDatalst.EC1, 
					m_pParamCalculator->m_CalcDatalst.EC2, 
					m_pParamCalculator->m_CalcDatalst.EC3, 
					m_pParamCalculator->m_CalcDatalst.EC4, 
					m_pParamCalculator->m_CalcDatalst.GA61, 
					m_pParamCalculator->m_CalcDatalst.EC5);

	m_pParamCalculator->m_CalcDatalst.EC51.Format(_T("%s%s"),
								 				  m_pParamCalculator->m_CalcDatalst.EC11,
								 				  m_pParamCalculator->m_CalcDatalst.EC2);

	CString strSQL;
	CADOTable table;
	strSQL.Format(_T("SELECT * FROM %s WHERE ConTyp='%s'"), g_tcTableName[eTableID_PanPri], m_pParamCalculator->m_CalcDatalst.EC51);
	if (table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && table.GetRecordCnt() > 0)
	{
		table.GetRecordCell(0, _T("GGPri"), (DWORD&)m_pParamCalculator->m_CalcDatalst.EC52);
		table.GetRecordCell(0, _T("Rpri"), (DWORD&)m_pParamCalculator->m_CalcDatalst.EC53);
		table.GetRecordCell(0, _T("CabPrimet"), (DWORD&)m_pParamCalculator->m_CalcDatalst.EC54);
	}
	table.CloseTable();
	m_pParamCalculator->m_CalcDatalst.EC55 = m_pParamCalculator->m_CalcDatalst.EC52 + 
											 m_pParamCalculator->m_CalcDatalst.EC53 + 
											 m_pParamCalculator->m_CalcDatalst.EC54 * m_pParamCalculator->m_CalcDatalst.GA6;

	return strCode;
}
//-----------------------------------------------------------------------
CString CBMOfferCalcElecDlg::GetCranePowSupplyCodeAndPrice()
{
	CString strCode;

	strCode.Format(	_T("PSC%s%s%d%s%d%s%d"), 
					m_pParamCalculator->m_CalcDatalst.GB11, 
					m_pParamCalculator->m_CalcDatalst.ED1, 
					m_pParamCalculator->m_CalcDatalst.ED4, 
					m_pParamCalculator->m_CalcDatalst.ED3, 
					m_pParamCalculator->m_CalcDatalst.ED2, 
					m_pParamCalculator->m_CalcDatalst.ED52, 
					m_pParamCalculator->m_CalcDatalst.GB4);
/*
	m_pParamCalculator->m_CalcDatalst.ED51 = m_pParamCalculator->m_CalcDatalst.EA20 + 
										     m_pParamCalculator->m_CalcDatalst.HB215*m_pParamCalculator->m_CalcDatalst.HB216 +
											 m_pParamCalculator->m_CalcDatalst.DB42;
*/
	CString strTemp;
	GetDlgItemText(IDC_EDIT_ELEC_KW, strTemp);
	m_pParamCalculator->m_CalcDatalst.ED51 = _ttol(strTemp);

	if (m_pParamCalculator->m_CalcDatalst.ED51 <= 15)
		m_pParamCalculator->m_CalcDatalst.ED52 = _T("A");
	else if (m_pParamCalculator->m_CalcDatalst.ED51 <= 25)
		m_pParamCalculator->m_CalcDatalst.ED52 = _T("B");
	else if (m_pParamCalculator->m_CalcDatalst.ED51 <= 35)
		m_pParamCalculator->m_CalcDatalst.ED52 = _T("C");
	else if (m_pParamCalculator->m_CalcDatalst.ED51 <= 45)
		m_pParamCalculator->m_CalcDatalst.ED52 = _T("D");
	else if (m_pParamCalculator->m_CalcDatalst.ED51 <= 55)
		m_pParamCalculator->m_CalcDatalst.ED52 = _T("E");
	else if (m_pParamCalculator->m_CalcDatalst.ED51 <= 65)
		m_pParamCalculator->m_CalcDatalst.ED52 = _T("F");
	else
		m_pParamCalculator->m_CalcDatalst.ED52 = _T("G");

	m_pParamCalculator->m_CalcDatalst.ED53.Format(_T("%s%d%s%s"),
												  m_pParamCalculator->m_CalcDatalst.ED1,
												  m_pParamCalculator->m_CalcDatalst.ED4,
												  m_pParamCalculator->m_CalcDatalst.ED52,
												  m_pParamCalculator->m_CalcDatalst.ED3);


	CString strSQL;
	CADOTable table;
	strSQL.Format(_T("SELECT * FROM %s WHERE CraPowCod='%s'"), g_tcTableName[eTableID_CraPowPri], m_pParamCalculator->m_CalcDatalst.ED53);
	if (table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && table.GetRecordCnt() > 0)
	{
		table.GetRecordCell(0, _T("FixPri"), (DWORD&)m_pParamCalculator->m_CalcDatalst.ED54);
		table.GetRecordCell(0, _T("MetPri"), (DWORD&)m_pParamCalculator->m_CalcDatalst.ED55);
	}
	table.CloseTable();
	m_pParamCalculator->m_CalcDatalst.ED56 = m_pParamCalculator->m_CalcDatalst.ED54 + 
											 m_pParamCalculator->m_CalcDatalst.ED55 * m_pParamCalculator->m_CalcDatalst.GB1;

	return strCode;
}
//-----------------------------------------------------------------------