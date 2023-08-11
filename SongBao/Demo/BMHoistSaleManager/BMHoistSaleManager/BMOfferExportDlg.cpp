//-----------------------------------------------------------------------
//	File Name:BMOfferExportDlg.cpp
//	Introduce:Class CBMOfferExportDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferExportDlg.h"
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CBMOfferExportDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferExportDlg, CDialog)
CBMOfferExportDlg::CBMOfferExportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferExportDlg::IDD, pParent)
{
	m_strSelOfferNo = _T("");
	m_bExportAllOffers = TRUE;
}
//-----------------------------------------------------------------------
CBMOfferExportDlg::~CBMOfferExportDlg()
{
}
//-----------------------------------------------------------------------
void CBMOfferExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferExportDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CBMOfferExportDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CBMOfferExportDlg::OnBnClickedBtnExport)
	ON_BN_CLICKED(IDC_RADIO_EXPORTALL, &CBMOfferExportDlg::OnBnClickedRadioExportall)
	ON_BN_CLICKED(IDC_RADIO_EXPORTSEL, &CBMOfferExportDlg::OnBnClickedRadioExportsel)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferExportDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferExportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitTablesDataStruct();
	((CButton*)GetDlgItem(IDC_RADIO_EXPORTALL))->SetCheck(BST_CHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferExportDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}
//-----------------------------------------------------------------------
void CBMOfferExportDlg::OnBnClickedRadioExportall()
{
	m_bExportAllOffers = TRUE;
}
//-----------------------------------------------------------------------
void CBMOfferExportDlg::OnBnClickedRadioExportsel()
{
	CButton* pBtnExportSel = (CButton*)GetDlgItem(IDC_RADIO_EXPORTSEL);
	int iStatus = pBtnExportSel->GetCheck();
	if (BST_CHECKED == iStatus)
	{
		if (m_strSelOfferNo.IsEmpty())
		{
			pBtnExportSel->SetCheck(BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_RADIO_EXPORTALL))->SetCheck(BST_CHECKED);
			((CButton*)GetDlgItem(IDC_RADIO_EXPORTALL))->SetFocus();
			myMessageBox(IDS_OFFEREXPORTDLG_NOSELOFFER, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		}
		else
		{
			m_bExportAllOffers = FALSE;
		}
	}
}
//-----------------------------------------------------------------------
void CBMOfferExportDlg::OnBnClickedBtnBrowse()
{
	CFileDialog dlgSave(FALSE, _T(".xls"), NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, _T("Excel Workbook(*.xls)|*.xls||"));
	if (IDOK == dlgSave.DoModal())
	{
		CString strExcelFile;
		strExcelFile = dlgSave.GetPathName();
		SetDlgItemText(IDC_EDIT_SAVEFILE, strExcelFile);
	}
}
//-----------------------------------------------------------------------
void CBMOfferExportDlg::OnBnClickedBtnExport()
{
	CString strExcelFile;
	GetDlgItemText(IDC_EDIT_SAVEFILE, strExcelFile);
	
	CString strSQL;
	CADOTable tbOffer;
	if (TRUE == m_bExportAllOffers)
	{
		strSQL.Format(_T("SELECT * FROM %s"), g_tcTableName[eTableID_OffersInfo]);
	}
	else
	{
		strSQL.Format(_T("SELECT * FROM %s WHERE OfferNo='%s'"), g_tcTableName[eTableID_OffersInfo], m_strSelOfferNo);
	}
	tbOffer.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strSQL);
	BOOL bResult = CExcelExporter::SaveDataFromTable(strExcelFile, _T("OfferData"), m_stuOfferInfoTable, tbOffer);
	tbOffer.CloseTable();
	if (TRUE == bResult)
	{
		CADOTable tbComponent;
		if (TRUE == m_bExportAllOffers)
		{
			strSQL.Format(_T("SELECT * FROM %s"), g_tcTableName[eTableID_ComponentInfo]);
		}
		else
		{
			strSQL.Format(_T("SELECT * FROM %s WHERE OfferNo='%s'"), g_tcTableName[eTableID_ComponentInfo], m_strSelOfferNo);
		}
		tbComponent.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strSQL);
		bResult = CExcelExporter::SaveDataFromTable(strExcelFile, _T("DetailInfo"), m_stuDetailInfoTable, tbComponent);
		tbComponent.CloseTable();
	}

	if (TRUE == bResult)
		myMessageBox(IDS_EXCEL_EXPORTOK, IDS_MSGDLG_SUCCESS, MB_OK | MB_ICONINFORMATION);
	else
		myMessageBox(IDS_EXCEL_EXPORTFAILED, IDS_MSGDLG_FAILED, MB_OK | MB_ICONERROR);
}
//-----------------------------------------------------------------------
void CBMOfferExportDlg::SetCurSelOfferNo(CString strSelOfferNo)
{
	m_strSelOfferNo = strSelOfferNo;
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferExportDlg::InitTablesDataStruct()
{	
	// "OfferInfo" table
	m_stuOfferInfoTable.eID = eTableID_OffersInfo;
	m_stuOfferInfoTable.strName = g_tcTableName[eTableID_OffersInfo];
	m_stuOfferInfoTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"OfferNo",			IDS_OFFERLIST_OFFERNO,			eTableDataType_String));
	m_stuOfferInfoTable.lstColumns.push_back(TableColumn(FALSE,	80,  L"CompanyShortName",	IDS_OFFERLIST_CUSTOMSHORTNAME,	eTableDataType_String));
	m_stuOfferInfoTable.lstColumns.push_back(TableColumn(FALSE,	80,  L"ContactName",		IDS_CUSTOMDLG_CONTACTNAME,		eTableDataType_String));
	m_stuOfferInfoTable.lstColumns.push_back(TableColumn(FALSE, 100, L"ProjectName",		IDS_OFFERLIST_PROJECTNAME,		eTableDataType_String));
	m_stuOfferInfoTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"ComponentNum",		IDS_OFFERLIST_COMPONENTNUM,		eTableDataType_DWORD));
	m_stuOfferInfoTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"Status",				IDS_OFFERLIST_STATUS,			eTableDataType_String));
	m_stuOfferInfoTable.lstColumns.push_back(TableColumn(FALSE, 100, L"Employee",			IDS_OFFERLIST_EMPLOYEE,			eTableDataType_String));
	m_stuOfferInfoTable.lstColumns.push_back(TableColumn(FALSE, 100, L"Date",				IDS_OFFERLIST_DATE,				eTableDataType_Date));
	m_stuOfferInfoTable.lstColumns.push_back(TableColumn(FALSE, 200, L"Comment",			IDS_OFFERLIST_COMMENT,			eTableDataType_String));
	
	// "ComponentInfo" table
	m_stuDetailInfoTable.eID = eTableID_ComponentInfo;
	m_stuDetailInfoTable.strName = g_tcTableName[eTableID_ComponentInfo];
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(TRUE,  50,  L"Index",			IDS_RECORD_INDEX,		eTableDataType_DWORD));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"OfferNo",		IDS_OFFERLIST_OFFERNO,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 150, L"ComponentName",	IDS_DETAILLIST_NAME,	eTableDataType_String));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"ComponentType",	IDS_DETAILLIST_TYPE,	eTableDataType_String));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 80,  L"ComponentPrice",IDS_DETAILLIST_PRICE,	eTableDataType_DWORD));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 100, L"ComponentNote", IDS_DETAILLIST_NOTE,	eTableDataType_String));
	//	
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"CraneType",			0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"HoistNumberType",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"CraneLoad",			0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"HoistLoad",			0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"Span",				0,	eTableDataType_FLOAT, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"HOL",				0,	eTableDataType_FLOAT, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"CraneDuty",			0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"HoistDuty",			0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"RailType",			0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"RailLength",		0,	eTableDataType_FLOAT, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"RailTopToFloor",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"RailRoofToTop",		0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"PowerSupply",		0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"ControlVol",		0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"CraneCtrlType",		0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MainUseType",		0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"HoistSpeed",		0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"TrolleySpeed",		0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"BridgeSpeed",		0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"HoistCtrlType",		0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"TrolleyCtrlType",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"BridgeCtrlType",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"ReevingType",		0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"HoistCode",			0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"HoistAdditions",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"HoistTotalPrice",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MainGirderCode",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MainGirderPrice",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_W1",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_h1",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_t1",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_t2",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_t3",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_t4",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_TroRail",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_Matrial",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_TrolleyRail_W",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_TrolleyRail_H",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_w3",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_c1",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_t5",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"MG_h2",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"EC_Type",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"EC_Code",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"EC_Drive",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"EC_MotorQty",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"EC_JointPlate",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"EC_Matrial",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"EC_Structure",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"EC_Wheelbase",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"EC_Additions",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"EC_TotalPrice",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"CranePannelCode",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"FastoonCode",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"OperterUnitCode",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"CranePowerSupplyCode",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"EL_Addtions",	0,	eTableDataType_String, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"CranePannelPrice",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"FastoonPrice",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"OperterUnitPrice",	0,	eTableDataType_DWORD, FALSE));
	m_stuDetailInfoTable.lstColumns.push_back(TableColumn(FALSE, 50,  L"CranePowerSupplyPrice",	0,	eTableDataType_DWORD, FALSE));
}
//-----------------------------------------------------------------------