//-----------------------------------------------------------------------
//	File Name:BMOfferManagerDlg.cpp
//	Introduce:Class CBMOfferManagerDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferManagerDlg.h"
#include "BMOfferExportDlg.h"
#include "BMOfferSelectDlg.h"
#include "BMOrderMakerDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferManagerDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferManagerDlg, CDialog)
CBMOfferManagerDlg::CBMOfferManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(/*CBMOfferManagerDlg::IDD, pParent*/)
{
	m_pParentWnd = pParent;
	m_pOfferDataDlg = NULL;
	m_bIsInCopyPaste = FALSE;
	m_iCopiedItem = -1;
}
//-----------------------------------------------------------------------
CBMOfferManagerDlg::~CBMOfferManagerDlg()
{
	DestroyWindow();
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OFFER, m_lstCtlOffers);
	DDX_Control(pDX, IDC_LIST_OFFERPART, m_lstCtlDetails);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferManagerDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_EXIT, &CBMOfferManagerDlg::OnBnClickedBtnExist)
	ON_BN_CLICKED(IDC_BTN_CUSTOM, &CBMOfferManagerDlg::OnBnClickedBtnCustom)
	ON_BN_CLICKED(IDC_BTN_NEW, &CBMOfferManagerDlg::OnBnClickedBtnNew)
	ON_BN_CLICKED(IDC_BTN_EDIT, &CBMOfferManagerDlg::OnBnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CBMOfferManagerDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_CALC, &CBMOfferManagerDlg::OnBnClickedBtnCalc)
	ON_BN_CLICKED(IDC_BTN_REPORTPRICE, &CBMOfferManagerDlg::OnBnClickedBtnReportprice)
	ON_BN_CLICKED(IDC_BTN_ORDERS, &CBMOfferManagerDlg::OnBnClickedBtnOrders)
	ON_BN_CLICKED(IDC_BTN_COPY, &CBMOfferManagerDlg::OnBnClickedBtnCopy)
	ON_BN_CLICKED(IDC_BTN_PASTE, &CBMOfferManagerDlg::OnBnClickedBtnPaste)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CBMOfferManagerDlg::OnBnClickedBtnImport)
	ON_NOTIFY(NM_CLICK, IDC_LIST_OFFER, &CBMOfferManagerDlg::OnNMClickListOffer)
	ON_MESSAGE(WM_USER_DATAISUPDATED, OnDataRecordIsUpdated)	
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_FIND, &CBMOfferManagerDlg::OnBnClickedBtnFind)
	ON_CBN_SELCHANGE(IDC_COMBO_FROMYEAR, &CBMOfferManagerDlg::OnCbnSelchangeComboFromyear)
	ON_CBN_SELCHANGE(IDC_COMBO_FROMMONTH, &CBMOfferManagerDlg::OnCbnSelchangeComboFrommonth)
	ON_CBN_SELCHANGE(IDC_COMBO_TOYEAR, &CBMOfferManagerDlg::OnCbnSelchangeComboToyear)
	ON_CBN_SELCHANGE(IDC_COMBO_TOMONTH, &CBMOfferManagerDlg::OnCbnSelchangeComboTomonth)
	ON_BN_CLICKED(IDC_BTN_FINDALL, &CBMOfferManagerDlg::OnBnClickedBtnFindall)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferManagerDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Adjust controls postion
	UpdateControlsPos();

	// Initliaze find options controls
	InitFindOptionsCtrls();

	// Initiliaze tables data struct
	InitTablesDataStruct();

	// Initiliaze lists
	m_imgNULL.Create(1, 18, TRUE | ILC_COLOR32, 1, 0); 
	InitDataListCtrls();

	// Load image
	m_pCopyrightImage = new CBitmap();
	BOOL b = m_pCopyrightImage->LoadBitmap(IDB_BITMAP_COPYRIGHT);

	// Update Paste button state
	UpdatePasteBtnState(FALSE);

	// Open tables and show records
	OpenTableAndShowRecords(m_stuOfferInfoTable.strName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	UpdateControlsPos();
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnPaint()
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
void CBMOfferManagerDlg::OnDestroy()
{
	CDialog::OnDestroy();
	
	m_tbOffersInfo.CloseTable();
	m_tbComponentInfo.CloseTable();
	m_imgNULL.DeleteImageList();
	if (NULL != m_pOfferDataDlg)
	{
		delete m_pOfferDataDlg;
		m_pOfferDataDlg = NULL;
	}
	if (NULL != m_pCopyrightImage)
	{
		delete m_pCopyrightImage;
		m_pCopyrightImage = NULL;
	}
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferManagerDlg::UpdateControlsPos()
{
	const int MARGIN_X = 10;
	const int MARGIN_Y = 10;

	CRect rcClient;
	GetClientRect(&rcClient);

	// Options Group
	CRect rcOptGrp;
	int iOptGrpOffsetX = 0;
	CWnd* pWndOptGrp = GetDlgItem(IDC_STATIC_OPTIONS);
	if (NULL != pWndOptGrp)
	{
		pWndOptGrp->GetWindowRect(&rcOptGrp);
		ScreenToClient(&rcOptGrp);
		int iOldRight = rcOptGrp.right;
		int iOptGrpWidth = rcOptGrp.Width();
		rcOptGrp.right = rcClient.Width() - MARGIN_X;
		rcOptGrp.left = rcOptGrp.right - iOptGrpWidth;
		rcOptGrp.top = MARGIN_Y;
		rcOptGrp.bottom = rcClient.Height() - 2*MARGIN_Y;
		iOptGrpOffsetX = rcOptGrp.right - iOldRight;
		pWndOptGrp->MoveWindow(&rcOptGrp);
	}
	// Options buttons	
	UINT nBtnIDs[] = {IDC_BTN_CUSTOM, IDC_BTN_NEW, IDC_BTN_EDIT, IDC_BTN_DELETE, 
					  IDC_BTN_CALC, IDC_BTN_REPORTPRICE, /*IDC_BTN_ORDERS, IDC_BTN_COPY, 
					  IDC_BTN_PASTE, IDC_BTN_IMPORT, */ IDC_BTN_EXIT};
	for (int i = 0; i < sizeof(nBtnIDs) / sizeof(nBtnIDs[0]); i++)
	{
		CWnd* pBtn = GetDlgItem(nBtnIDs[i]);
		if(NULL != pBtn) 
		{	
			CRect rc;
			pBtn->GetWindowRect(&rc);
			ScreenToClient(&rc);
			if (i == 0)
			{
				rc.OffsetRect(iOptGrpOffsetX, 0);
				pBtn->MoveWindow(&rc);
			}
			else
			{
				CRect rcBase;
				CWnd* pBaseBtn = GetDlgItem(nBtnIDs[i-1]);
				pBaseBtn->GetWindowRect(&rcBase);
				ScreenToClient(&rcBase);
				int iOffsetY = rcBase.bottom - (rc.top - 2);
				rc.OffsetRect(iOptGrpOffsetX, iOffsetY);
				pBtn->MoveWindow(&rc);
			}
		}
	}
	
	// Find Setting Group
	CRect rcFindGrp;
	CWnd* pWndFindGrp = GetDlgItem(IDC_STATIC_QUERY);
	if (NULL != pWndFindGrp)
	{
		pWndFindGrp->GetWindowRect(&rcFindGrp);
		ScreenToClient(&rcFindGrp);
		int iFingGrpHeight = rcFindGrp.Height();
		rcFindGrp.left = MARGIN_X;
		rcFindGrp.right = rcOptGrp.left - MARGIN_X;
		rcFindGrp.top = MARGIN_Y;
		rcFindGrp.bottom = rcFindGrp.top + iFingGrpHeight;
		pWndFindGrp->MoveWindow(&rcFindGrp);
	}

	// Part List Group
	CRect rcLstGrp;
	CRect rcPartLst;
	CWnd* pWndLstGrp = GetDlgItem(IDC_STATIC_PART);
	CWnd* pWndPartLst = GetDlgItem(IDC_LIST_OFFERPART);
	if (NULL != pWndLstGrp && NULL != pWndPartLst)
	{
		pWndLstGrp->GetWindowRect(&rcLstGrp);
		pWndPartLst->GetWindowRect(&rcPartLst);
		ScreenToClient(&rcLstGrp);
		ScreenToClient(&rcPartLst);
		int ilstGrpHeight = rcLstGrp.Height();
		rcLstGrp.left = MARGIN_X;
		rcLstGrp.right = rcOptGrp.left - MARGIN_X;
		rcLstGrp.bottom = rcOptGrp.bottom;
		rcLstGrp.top = rcLstGrp.bottom - ilstGrpHeight;
		pWndLstGrp->MoveWindow(&rcLstGrp);
		//
		rcPartLst.left = rcLstGrp.left + 5;
		rcPartLst.right = rcLstGrp.right - 5;
		rcPartLst.top = rcLstGrp.top + 20;
		rcPartLst.bottom = rcLstGrp.bottom - 5;
		pWndPartLst->MoveWindow(&rcPartLst);
	}

	// Offer Data List
	CRect rcOfferLst;
	CWnd* pWndOfferLst = GetDlgItem(IDC_LIST_OFFER);
	if (NULL != pWndOfferLst)
	{
		pWndOfferLst->GetWindowRect(&rcOfferLst);
		ScreenToClient(&rcOfferLst);
		rcOfferLst.left = rcLstGrp.left;
		rcOfferLst.right = rcLstGrp.right;
		rcOfferLst.top = rcFindGrp.bottom + MARGIN_Y;
		rcOfferLst.bottom = rcLstGrp.top - MARGIN_Y;
		pWndOfferLst->MoveWindow(&rcOfferLst);
	}

	// Re-adjust options group	
	if (NULL != pWndOptGrp)
	{
		pWndOptGrp->GetWindowRect(&rcOptGrp);
		ScreenToClient(&rcOptGrp);
		rcOptGrp.bottom = rcOfferLst.bottom;
		pWndOptGrp->MoveWindow(&rcOptGrp);
	}

	// Copyright image rectagle
	InvalidateRect(&m_rcCopyrightImage);
	m_rcCopyrightImage.left = rcOptGrp.left;
	m_rcCopyrightImage.right = rcOptGrp.right;
	m_rcCopyrightImage.top = rcLstGrp.top + (rcLstGrp.Height() - 160) / 2;	//160 is copyright image height
	m_rcCopyrightImage.bottom = m_rcCopyrightImage.top + 160;
	InvalidateRect(&m_rcCopyrightImage);
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::InitDataListCtrls()
{
	m_lstCtlOffers.SetExtendedStyle(m_lstCtlOffers.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_lstCtlOffers.SetFullItemBKColor(RGB(214, 231, 255), RGB(94, 231, 221));
	m_lstCtlOffers.SetImageList(&m_imgNULL, LVSIL_SMALL);
	InitDataListCtrl(&m_lstCtlOffers, m_stuOfferInfoTable);
	//
	m_lstCtlDetails.SetExtendedStyle(m_lstCtlDetails.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_lstCtlDetails.SetFullItemBKColor(RGB(214, 231, 255), RGB(94, 231, 221));
	m_lstCtlDetails.SetImageList(&m_imgNULL, LVSIL_SMALL);
	InitDataListCtrl(&m_lstCtlDetails, m_stuDetailInfoTable);
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::InitTablesDataStruct()
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
void CBMOfferManagerDlg::InitFindOptionsCtrls()
{
	CString strOptions;

	// Orders status list
	CComboBox* pStatusComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STATUS);
	int iIndex = pStatusComboBox->AddString(myGetText(IDS_COMBOBOX_ALL));
	pStatusComboBox->SetItemData(iIndex, -1);
	for (int iStatus = 0; iStatus < eOfferStatus_Cnt; iStatus++)
	{
		iIndex = pStatusComboBox->AddString(GetOfferStatusText((eOfferStatus)iStatus));
		pStatusComboBox->SetItemData(iIndex, iStatus);
	}
	pStatusComboBox->SetCurSel(0);

	// Custom short name list
	CComboBox* pCustomNameComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CUSTOMNAME);
	pCustomNameComboBox->AddString(myGetText(IDS_COMBOBOX_ALL));
	//
	CADOTable tbCustomInfo;
	strOptions.Format(L"SELECT CompanyShortName FROM %s", g_tcTableName[eTableID_CustomerInfo]);
	if (TRUE == tbCustomInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOptions))
	{
		for (DWORD dwIndex = 0; dwIndex < tbCustomInfo.GetRecordCnt(); dwIndex++)
		{
			CString strShortName;
			tbCustomInfo.GetRecordCell(dwIndex, L"CompanyShortName", strShortName.GetBuffer(64), 64);
			pCustomNameComboBox->AddString(strShortName);
		}
		tbCustomInfo.CloseTable();
	}
	pCustomNameComboBox->SetCurSel(0);

	// Employee name list
	CComboBox* pEmployeeNameComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_EMPLOYEE);
	pEmployeeNameComboBox->AddString(myGetText(IDS_COMBOBOX_ALL));
	//
	CADOTable tbEmployeeInfo;
	strOptions.Format(L"SELECT UserName FROM %s", g_tcTableName[eTableID_EmployeeInfo]);
	if (TRUE == tbEmployeeInfo.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOptions))
	{
		for (DWORD dwIndex = 0; dwIndex < tbEmployeeInfo.GetRecordCnt(); dwIndex++)
		{
			CString strEmployeeName;
			tbEmployeeInfo.GetRecordCell(dwIndex, L"UserName", strEmployeeName.GetBuffer(64), 64);
			pEmployeeNameComboBox->AddString(strEmployeeName);
		}
		tbEmployeeInfo.CloseTable();
	}
	pEmployeeNameComboBox->SetCurSel(0);

	// From date lists	
	CComboBox* pFromYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FROMYEAR);
	CComboBox* pFromMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FROMMONTH);
	iIndex = pFromYearComboBox->AddString(myGetText(IDS_DATE_YEAR));
	pFromYearComboBox->SetItemData(iIndex, 0);
	iIndex = pFromMonthComboBox->AddString(myGetText(IDS_DATE_MONTH));
	pFromMonthComboBox->SetItemData(iIndex, 0);
	//
	CTime tmNow;
	tmNow = CTime::GetCurrentTime();
	for (int iYear = 2010; iYear <= tmNow.GetYear(); iYear++)
	{
		CString strYear;
		strYear.Format(_T("%d"), iYear);
		int iIndex = pFromYearComboBox->AddString(strYear);
		pFromYearComboBox->SetItemData(iIndex, iYear);
	}
	for (int iMonth = 1; iMonth <= 12; iMonth++)
	{
		CString strMonth;
		strMonth.Format(_T("%02d"), iMonth);
		int iIndex = pFromMonthComboBox->AddString(strMonth);
		pFromMonthComboBox->SetItemData(iIndex, iMonth);
	}
	pFromYearComboBox->SetCurSel(0);
	pFromMonthComboBox->SetCurSel(0);
	FillDateDayComboBox(0, 0, (CComboBox*)GetDlgItem(IDC_COMBO_FROMDAY));

	// To date lists	
	CComboBox* pToYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TOYEAR);
	CComboBox* pToMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TOMONTH);
	iIndex = pToYearComboBox->AddString(myGetText(IDS_DATE_YEAR));
	pToYearComboBox->SetItemData(iIndex, 0);
	iIndex = pToMonthComboBox->AddString(myGetText(IDS_DATE_MONTH));
	pToMonthComboBox->SetItemData(iIndex, 0);
	for (int iYear = 2010; iYear <= tmNow.GetYear(); iYear++)
	{
		CString strYear;
		strYear.Format(_T("%d"), iYear);
		int iIndex = pToYearComboBox->AddString(strYear);
		pToYearComboBox->SetItemData(iIndex, iYear);
	}
	for (int iMonth = 1; iMonth <= 12; iMonth++)
	{
		CString strMonth;
		strMonth.Format(_T("%02d"), iMonth);
		int iIndex = pToMonthComboBox->AddString(strMonth);
		pToMonthComboBox->SetItemData(iIndex, iMonth);
	}
	pToYearComboBox->SetCurSel(0);
	pToMonthComboBox->SetCurSel(0);
	FillDateDayComboBox(0, 0, (CComboBox*)GetDlgItem(IDC_COMBO_TODAY));
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::UpdatePasteBtnState(BOOL bIsInCopying)
{
	GetDlgItem(IDC_BTN_PASTE)->EnableWindow(bIsInCopying);
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::FillDateDayComboBox(int iYear, int iMonth, CComboBox* pDayComboBox)
{
	if (NULL == pDayComboBox)
		return;

	pDayComboBox->ResetContent();
	int iIndex = pDayComboBox->AddString(myGetText(IDS_DATE_DAY));
	pDayComboBox->SetItemData(iIndex, 0);

	int iDayCount = 0;
	if (1 == iMonth || 3 == iMonth || 5 == iMonth || 7 == iMonth || 8 == iMonth || 10 == iMonth || 12 == iMonth)
		iDayCount = 31;
	else if (4 == iMonth || 6 == iMonth || 9 == iMonth || 11 == iMonth)
		iDayCount = 30;
	else if (2 == iMonth)
		iDayCount = (iYear % 4 == 0) ? 29 : 28;
	else
		iDayCount = 31;

	for (int iDay = 1; iDay <= iDayCount; iDay++)
	{
		CString strDay;
		strDay.Format(_T("%02d"), iDay);
		int iIndex = pDayComboBox->AddString(strDay);
		pDayComboBox->SetItemData(iIndex, iDay);
	}
	pDayComboBox->SetCurSel(0);
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OpenTableAndShowRecords(CString strTableName, CString strQuery)
{
	CStringW strOpenOperation;
	if (strQuery.IsEmpty())
		strOpenOperation.Format(L"SELECT * FROM %s", strTableName);
	else
		strOpenOperation = strQuery;

	BOOL bResult = TRUE;

	if (strTableName == m_stuOfferInfoTable.strName)
	{
		m_tbOffersInfo.CloseTable();
		bResult = m_tbOffersInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
		if (FALSE == bResult)
			return;
		ShowTableRecords(&m_tbOffersInfo, &m_lstCtlOffers, m_stuOfferInfoTable);
		
		// Select the first item in default
		HRESULT hr = 0;
		m_lstCtlOffers.SetFocus();
		m_lstCtlOffers.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);	
		OnNMClickListOffer(NULL, &hr);
	}
	else if (strTableName == m_stuDetailInfoTable.strName)
	{
		m_tbComponentInfo.CloseTable();
		bResult = m_tbComponentInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
		if (FALSE == bResult)
			return;
		ShowTableRecords(&m_tbComponentInfo, &m_lstCtlDetails, m_stuDetailInfoTable);
	}
}
//-----------------------------------------------------------------------
CString	CBMOfferManagerDlg::GetNewOfferID()
{
	CString strNewOfferID = g_strUserID;

	CADOTable tbOffers;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT OfferNo FROM %s WHERE OfferNo LIKE '%s%s' ORDER BY OfferNo", g_tcTableName[eTableID_OffersInfo], g_strUserID, _T("%"));
	BOOL bResult = tbOffers.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return _T("\0");

	CTime tm = CTime::GetCurrentTime();
	DWORD dwOfferCnt = tbOffers.GetRecordCnt();

	int iIndex = 0;
	BOOL bNewOfferIDIsFound = FALSE;
	do
	{
		iIndex++;
		if (iIndex < 100) strNewOfferID.Format(_T("%s%04d%02d%02d"), g_strUserID, tm.GetYear(), tm.GetMonth(), iIndex);
		else if (iIndex < 1000) strNewOfferID.Format(_T("%s%04d%02d%03d"), g_strUserID, tm.GetYear(), tm.GetMonth(), iIndex);
		else strNewOfferID.Format(_T("%s%04d%02d%04d"), g_strUserID, tm.GetYear(), tm.GetMonth(), iIndex);
		
		if (dwOfferCnt == 0)
		{
			bNewOfferIDIsFound = TRUE;
		}
		else
		{
			CString strLastID;
			tbOffers.GetRecordCell(dwOfferCnt-1, L"OfferNo", strLastID.GetBuffer(32), 32);
			if (_tcsicmp(strNewOfferID, strLastID) > 0)
				bNewOfferIDIsFound = TRUE;
		}

	}while(!bNewOfferIDIsFound && iIndex < 100);
	tbOffers.CloseTable();

	return strNewOfferID;
}
//-----------------------------------------------------------------------
int CBMOfferManagerDlg::CopyOfferDataInTable(int iCopyItem)
{
	CString strCopiedOfferNo = m_lstCtlOffers.GetItemText(iCopyItem, 0);
	CString strNewOfferNo = GetNewOfferID();

	// Open source table
	CADOTable tbSource;
	CString strOperations;
	strOperations.Format(_T("SELECT * FROM %s WHERE OfferNo = '%s'"), g_tcTableName[eTableID_OffersInfo], strCopiedOfferNo);
	if (FALSE == tbSource.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOperations) || tbSource.GetRecordCnt() == 0)
		return -1;

	int iNewItemIndex = -1;
	m_tbOffersInfo.AddNewItem();
	m_tbOffersInfo.SetRecordCell(-1, L"OfferNo", strNewOfferNo.GetBuffer(32));
	m_tbOffersInfo.UpdateTable();
	iNewItemIndex = m_tbOffersInfo.GetRecordCnt() - 1;

	// Copy offers record
	CopyDataInTables(&tbSource, 0, &m_tbOffersInfo, iNewItemIndex, m_stuOfferInfoTable, 1);
	tbSource.CloseTable();

	// Set "Copy from XXXXX" in Comment column
	CString strComment;
	strComment.Format(_T("Copy from offer %s"), strCopiedOfferNo);
	m_tbOffersInfo.SetRecordCell(iNewItemIndex, L"Comment", strComment.GetBuffer(strComment.GetLength() + 1));
	if (FALSE == m_tbOffersInfo.UpdateTable())
		return -1;

	// Copy offers component records
	CADOTable tbComSrc;
	CADOTable tbComDes;
	strOperations.Format(_T("SELECT * FROM %s WHERE OfferNo = '%s'"), g_tcTableName[eTableID_ComponentInfo], strCopiedOfferNo);
	if (TRUE == tbComSrc.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOperations) && tbComSrc.GetRecordCnt() > 0)
	{
		int iSrcRecordCnt = tbComSrc.GetRecordCnt();
		strOperations.Format(_T("SELECT * FROM %s"), g_tcTableName[eTableID_ComponentInfo]);
		if (TRUE == tbComDes.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOperations))
		{
			for (int iIndex = 0; iIndex < iSrcRecordCnt; iIndex++)
			{
				tbComDes.AddNewItem();
				tbComDes.SetRecordCell(-1, L"OfferNo", strNewOfferNo.GetBuffer(32));
				tbComDes.UpdateTable();
				int iNewItem = tbComDes.GetRecordCnt() - 1;
				CopyDataInTables(&tbComSrc, iIndex, &tbComDes, iNewItem, m_stuDetailInfoTable, 2);
			}
			tbComDes.UpdateTable();
			tbComDes.CloseTable();
		}		
		tbComSrc.CloseTable();
	}

	return iNewItemIndex;
}
//-----------------------------------------------------------------------
BOOL CBMOfferManagerDlg::CopyDataInTables(CADOTable* pSrcTable, int iSrcIndex, 
										  CADOTable* pDestTable, int iDestIndex,
										  TableData &tb, int iFromPos)
{
	if (NULL == pSrcTable || NULL == pDestTable)
		return FALSE;
	if (iFromPos < 0 || iFromPos >= (int)tb.lstColumns.size()) 
		return FALSE;

	int iIndex = 0;
	for (TableColumnsList::iterator it = tb.lstColumns.begin();
		it != tb.lstColumns.end();
		it++, iIndex++)
	{
		TableColumn cl = *it;
		if (cl.bIsIndex) continue;
		if (iIndex < iFromPos) continue;

		switch(cl.eDataType)
		{
		case eTableDataType_DWORD:
			{
				DWORD dwCellValue = 0;
				pSrcTable->GetRecordCell(iSrcIndex, cl.strName, dwCellValue);
				pDestTable->SetRecordCell(iDestIndex, cl.strName, dwCellValue);
			}
			break;
		case eTableDataType_FLOAT:
			{
				float fCellValue = 0;
				pSrcTable->GetRecordCell(iSrcIndex, cl.strName, fCellValue);
				pDestTable->SetRecordCell(iDestIndex, cl.strName, fCellValue);
			}
			break;
		case eTableDataType_Date:
			{
				CTime tm;
				pSrcTable->GetRecordCell(iSrcIndex, cl.strName, tm);
				pDestTable->SetRecordCell(iDestIndex, cl.strName, tm);
			}
			break;
		case eTableDataType_String:
		default:
			{
				WCHAR wcCellValue[256] = {L"\0"};
				pSrcTable->GetRecordCell(iSrcIndex, cl.strName, wcCellValue, 256);
				pDestTable->SetRecordCell(iDestIndex, cl.strName, wcCellValue);
			}
			break;
		}
	}

	return TRUE;
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnNMClickListOffer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	m_lstCtlDetails.DeleteAllItems();

	if (TRUE == m_bIsInCopyPaste)
	{
		m_lstCtlOffers.SetItemData(m_iCopiedItem, 0);
		m_lstCtlOffers.Invalidate();

		m_bIsInCopyPaste = FALSE;
		m_iCopiedItem = -1;
		UpdatePasteBtnState(m_bIsInCopyPaste);
	}

	int iSelItem = -1;
	POSITION pos = m_lstCtlOffers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlOffers.GetNextSelectedItem(pos);
	if (iSelItem < 0)
		return;

	CStringW strOpenOperation;
	CString strOfferID = m_lstCtlOffers.GetItemText(iSelItem, 0);
	strOpenOperation.Format(L"SELECT * FROM %s WHERE OfferNo='%s'", g_tcTableName[eTableID_ComponentInfo], strOfferID);

	OpenTableAndShowRecords(g_tcTableName[eTableID_ComponentInfo], strOpenOperation);

	*pResult = 0;
}
//-----------------------------------------------------------------------
HRESULT CBMOfferManagerDlg::OnDataRecordIsUpdated(WPARAM wParam, LPARAM lParam)
{
	// Save the old selected OfferID
	int iSelItem = -1;
	CString strOfferID;
	POSITION pos = m_lstCtlOffers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlOffers.GetNextSelectedItem(pos);
	if (iSelItem >= 0)
		strOfferID = m_lstCtlOffers.GetItemText(iSelItem, 0);

	// Open tables and show records
	OpenTableAndShowRecords(m_stuOfferInfoTable.strName);

	// Select the old selected item
	HRESULT hr = 0;
	if (!strOfferID.IsEmpty())
	{
		for (int iIndex = 0; iIndex < m_lstCtlOffers.GetItemCount(); iIndex++)
		{
			CString strItemText = m_lstCtlOffers.GetItemText(iIndex, 0);
			if (strItemText == strOfferID)
			{
				m_lstCtlOffers.SetItemState(iIndex, LVIS_SELECTED, LVIS_SELECTED);
				break;
			}
		}
	}
	else if (m_lstCtlOffers.GetItemCount() > 0)
	{
		m_lstCtlOffers.SetItemState(m_lstCtlOffers.GetItemCount() - 1, LVIS_SELECTED, LVIS_SELECTED);
	}
	OnNMClickListOffer(NULL, &hr);

	return 1;
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnCbnSelchangeComboFromyear()
{
	CComboBox* pYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FROMYEAR);
	CComboBox* pMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FROMMONTH);
	int iSelIndex = pYearComboBox->GetCurSel();
	int iYear = pYearComboBox->GetItemData(iSelIndex);
	iSelIndex = pMonthComboBox->GetCurSel();
	int iMonth = pMonthComboBox->GetItemData(iSelIndex);
	FillDateDayComboBox(iYear, iMonth, (CComboBox*)GetDlgItem(IDC_COMBO_FROMDAY));
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnCbnSelchangeComboFrommonth()
{
	CComboBox* pYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FROMYEAR);
	CComboBox* pMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FROMMONTH);
	int iSelIndex = pYearComboBox->GetCurSel();
	int iYear = pYearComboBox->GetItemData(iSelIndex);
	iSelIndex = pMonthComboBox->GetCurSel();
	int iMonth = pMonthComboBox->GetItemData(iSelIndex);
	FillDateDayComboBox(iYear, iMonth, (CComboBox*)GetDlgItem(IDC_COMBO_FROMDAY));
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnCbnSelchangeComboToyear()
{
	CComboBox* pYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TOYEAR);
	CComboBox* pMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TOMONTH);
	int iSelIndex = pYearComboBox->GetCurSel();
	int iYear = pYearComboBox->GetItemData(iSelIndex);
	iSelIndex = pMonthComboBox->GetCurSel();
	int iMonth = pMonthComboBox->GetItemData(iSelIndex);
	FillDateDayComboBox(iYear, iMonth, (CComboBox*)GetDlgItem(IDC_COMBO_TODAY));
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnCbnSelchangeComboTomonth()
{
	CComboBox* pYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TOYEAR);
	CComboBox* pMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TOMONTH);
	int iSelIndex = pYearComboBox->GetCurSel();
	int iYear = pYearComboBox->GetItemData(iSelIndex);
	iSelIndex = pMonthComboBox->GetCurSel();
	int iMonth = pMonthComboBox->GetItemData(iSelIndex);
	FillDateDayComboBox(iYear, iMonth, (CComboBox*)GetDlgItem(IDC_COMBO_TODAY));
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnFind()
{
	BOOL bHasWhereString = FALSE;
	CString strFindSetting;
	strFindSetting.Format(_T("SELECT * FROM %s"), g_tcTableName[eTableID_OffersInfo]);
	
	// Offer ID
	CString strOfferID;
	GetDlgItemText(IDC_EDIT_OFFERNO, strOfferID);
	if (!strOfferID.IsEmpty())
	{
		CString strTemp;
		strTemp.Format(_T(" WHERE OfferNo LIKE '%s%s'"), strOfferID, _T("%"));
		strFindSetting += strTemp;
		bHasWhereString = TRUE;
	}
	
	// Project Name
	CString strProjectName;
	GetDlgItemText(IDC_EDIT_PROJECT, strProjectName);
	if (!strProjectName.IsEmpty())
	{
		CString strTemp;
		if (FALSE == bHasWhereString)
			strTemp.Format(_T(" WHERE ProjectName LIKE '%s%s'"), strProjectName, _T("%"));
		else
			strTemp.Format(_T(" AND ProjectName LIKE '%s%s'"), strProjectName, _T("%"));
		strFindSetting += strTemp;
		bHasWhereString = TRUE;
	}

	// Custom Name
	CString strCustomName;
	CComboBox* pCustomComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CUSTOMNAME);
	pCustomComboBox->GetWindowText(strCustomName);
	if (pCustomComboBox->GetCurSel() > 0 && !strCustomName.IsEmpty())
	{
		CString strTemp;
		if (FALSE == bHasWhereString)
			strTemp.Format(_T(" WHERE CompanyShortName LIKE '%s%s'"), strCustomName, _T("%"));
		else
			strTemp.Format(_T(" AND CompanyShortName LIKE '%s%s'"), strCustomName, _T("%"));
		strFindSetting += strTemp;
		bHasWhereString = TRUE;
	}

	// Offer status
	CString strStatus;
	CComboBox* pStateComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STATUS);
	int iSelIndex = pStateComboBox->GetCurSel();
	int iState = pStateComboBox->GetItemData(iSelIndex);
	strStatus = GetOfferStatusText((eOfferStatus)iState);
	if (!strStatus.IsEmpty())
	{
		CString strTemp;
		if (FALSE == bHasWhereString)
			strTemp.Format(_T(" WHERE Status LIKE '%s%s'"), strStatus, _T("%"));
		else
			strTemp.Format(_T(" AND Status LIKE '%s%s'"), strStatus, _T("%"));
		strFindSetting += strTemp;
		bHasWhereString = TRUE;
	}

	// Offer employee name
	CString strEmployeeName;
	CComboBox* pEmployeeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_EMPLOYEE);
	pEmployeeComboBox->GetWindowText(strEmployeeName);
	if (pEmployeeComboBox->GetCurSel() > 0 && !strEmployeeName.IsEmpty())
	{
		CString strTemp;
		if (FALSE == bHasWhereString)
			strTemp.Format(_T(" WHERE Employee LIKE '%s%s'"), strEmployeeName, _T("%"));
		else
			strTemp.Format(_T(" AND Employee LIKE '%s%s'"), strEmployeeName, _T("%"));
		strFindSetting += strTemp;
		bHasWhereString = TRUE;
	}
	
	// Offer date
	CComboBox* pFromYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FROMYEAR);
	CComboBox* pFromMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FROMMONTH);
	CComboBox* pFromDayComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FROMDAY);
	CComboBox* pToYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TOYEAR);
	CComboBox* pToMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TOMONTH);
	CComboBox* pToDayComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TODAY);
	//
	CString strFromDate;
	CString strToDate;
	int iCurSel = pFromYearComboBox->GetCurSel();
	long lItemData = pFromYearComboBox->GetItemData(iCurSel);
	if (lItemData > 0)
	{
		CString strTemp;
		strTemp.Format(_T("%d"), lItemData);
		strFromDate += strTemp;
		//
		iCurSel = pFromMonthComboBox->GetCurSel();
		lItemData = pFromMonthComboBox->GetItemData(iCurSel);
		if (lItemData > 0)
		{
			strTemp.Format(_T("-%d"), lItemData);
			strFromDate += strTemp;
			//
			iCurSel = pFromDayComboBox->GetCurSel();
			lItemData = pFromDayComboBox->GetItemData(iCurSel);
			if (lItemData > 0)
			{
				strTemp.Format(_T("-%d"), lItemData);
				strFromDate += strTemp;
			}
		}
	}
	iCurSel = pToYearComboBox->GetCurSel();
	lItemData = pToYearComboBox->GetItemData(iCurSel);
	if (lItemData > 0)
	{
		CString strTemp;
		strTemp.Format(_T("%d"), lItemData);
		strToDate += strTemp;
		//
		iCurSel = pToMonthComboBox->GetCurSel();
		lItemData = pToMonthComboBox->GetItemData(iCurSel);
		if (lItemData > 0)
		{
			strTemp.Format(_T("-%d"), lItemData);
			strToDate += strTemp;
			//
			iCurSel = pToDayComboBox->GetCurSel();
			lItemData = pToDayComboBox->GetItemData(iCurSel);
			if (lItemData > 0)
			{
				strTemp.Format(_T("-%d"), lItemData);
				strToDate += strTemp;
			}
		}
	}
	//
	CString strTempDate;
	if (!strFromDate.IsEmpty() && strToDate.IsEmpty())
	{
		if (FALSE == bHasWhereString)
			strTempDate.Format(_T(" WHERE Date >= #%s#"), strFromDate);
		else
			strTempDate.Format(_T(" AND Date >= #%s#"), strFromDate);
	}
	else if (!strFromDate.IsEmpty() && !strToDate.IsEmpty())
	{
		if (FALSE == bHasWhereString)
			strTempDate.Format(_T(" WHERE Date >= #%s# AND Date <= #%s#"), strFromDate, strToDate);
		else
			strTempDate.Format(_T(" AND Date >= #%s# AND Date <= #%s#"), strFromDate, strToDate);
	}
	else if(strFromDate.IsEmpty() && !strToDate.IsEmpty())
	{			
		if (FALSE == bHasWhereString)
			strTempDate.Format(_T(" WHERE Date <= #%s#"), strToDate);
		else
			strTempDate.Format(_T(" AND Date <= #%s#"), strToDate);
	}
	if (!strTempDate.IsEmpty())
	{
		strFindSetting += strTempDate;
		bHasWhereString = TRUE;
	}

	// Open table by Find Setting
	OpenTableAndShowRecords(g_tcTableName[eTableID_OffersInfo], strFindSetting);
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnFindall()
{
	SetDlgItemText(IDC_EDIT_OFFERNO, _T(""));
	SetDlgItemText(IDC_EDIT_PROJECT, _T(""));
	((CComboBox*)GetDlgItem(IDC_COMBO_CUSTOMNAME))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_STATUS))->SetCurSel(0);	
	((CComboBox*)GetDlgItem(IDC_COMBO_EMPLOYEE))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_FROMYEAR))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_FROMMONTH))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_FROMDAY))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_TOYEAR))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_TOMONTH))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_TODAY))->SetCurSel(0);
	OpenTableAndShowRecords(g_tcTableName[eTableID_OffersInfo]);
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnCustom()
{
	::PostMessage(m_pParentWnd->m_hWnd, WM_USER_SHOWCUSTOMER, 0, 0);
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnNew()
{
	CString strNewOfferID = GetNewOfferID();

	if (NULL == m_pOfferDataDlg)
	{
		m_pOfferDataDlg = new CBMOfferDataDlg(this);
		m_pOfferDataDlg->Create(IDD_DIALOG_OFFERDATA, this);
	}

	m_pOfferDataDlg->SetOfferID(strNewOfferID, FALSE);
	m_pOfferDataDlg->ShowWindow(SW_SHOW);
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnEdit()
{
	int iSelItem = -1;
	POSITION pos = m_lstCtlOffers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlOffers.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		myMessageBox(IDS_EDIT_NOITEMSELECTED, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}
	CString strOfferNo = m_lstCtlOffers.GetItemText(iSelItem, 0);

	if (NULL == m_pOfferDataDlg)
	{
		m_pOfferDataDlg = new CBMOfferDataDlg(this);
		m_pOfferDataDlg->Create(IDD_DIALOG_OFFERDATA, this);
	}
	m_pOfferDataDlg->SetOfferID(strOfferNo, TRUE);
	m_pOfferDataDlg->ShowWindow(SW_SHOW);
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnDelete()
{
	int iSelItem = -1;
	POSITION pos = m_lstCtlOffers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlOffers.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		myMessageBox(IDS_DELETE_NOITEMSELECTED, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}

	if (IDYES != myMessageBox(IDS_DELETE_CONFIRM, IDS_DLGMSG_WARNING, MB_YESNO | MB_ICONQUESTION))
		return;

	// Delete this offer from database
	if (TRUE == m_tbOffersInfo.DeleteItem(iSelItem))
	{
		m_tbOffersInfo.UpdateTable();
		
		// Remove its components information from table "ComponentInfo"	
		DWORD dwRecordCnt = m_tbComponentInfo.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwRecordCnt; dwIndex++)
		{
			m_tbComponentInfo.DeleteItem(0);
		}
		m_tbComponentInfo.UpdateTable();
		m_tbComponentInfo.CloseTable();
		
		OpenTableAndShowRecords(m_stuOfferInfoTable.strName);
	}
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnCalc()
{
	// Get Offer ID
	int iSelItem = -1;
	POSITION pos = m_lstCtlOffers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlOffers.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		myMessageBox(IDS_CALC_NOITEMSELECTED, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}
	CString strOfferNo = m_lstCtlOffers.GetItemText(iSelItem, 0);

	// Get Components number
	int iComponentCnt = this->m_lstCtlDetails.GetItemCount();
	if (iComponentCnt <= 0)
	{
		myMessageBox(IDS_CALC_NOCOMPONENTS, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}

	// Show calcuate dialog
	CBMOfferCalcDlg dlg;
	dlg.SetOfferNo(strOfferNo);
	dlg.DoModal();

	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE OfferNo='%s'", g_tcTableName[eTableID_ComponentInfo], strOfferNo);
	OpenTableAndShowRecords(g_tcTableName[eTableID_ComponentInfo], strOpenOperation);
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnReportprice()
{
	// Get Offer ID
	int iSelItem = -1;
	POSITION pos = m_lstCtlOffers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlOffers.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		myMessageBox(IDS_OFFER_NOITEMSELECTED, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}
	CString strOfferNo = m_lstCtlOffers.GetItemText(iSelItem, 0);

	// Get Components number
	int iComponentCnt = m_lstCtlDetails.GetItemCount();
	if (iComponentCnt <= 0)
	{
		myMessageBox(IDS_CALC_NOCOMPONENTS, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}

	// Show Offer Select dialog
	CBMOfferSelectDlg dlg;
	dlg.SetOfferNo(strOfferNo);
	dlg.DoModal();
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnOrders()
{
	// Get Offer ID
	int iSelItem = -1;
	POSITION pos = m_lstCtlOffers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlOffers.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		myMessageBox(IDS_OFFER_NOITEMSELECTED, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}
	CString strOfferNo = m_lstCtlOffers.GetItemText(iSelItem, 0);

	// Get Components number
	int iComponentCnt = m_lstCtlDetails.GetItemCount();
	if (iComponentCnt <= 0)
	{
		myMessageBox(IDS_CALC_NOCOMPONENTS, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}

	// Show Order Select dialog
	CBMOrderMakerDlg dlg;
	dlg.SetOfferNo(strOfferNo);
	dlg.DoModal();
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnCopy()
{
	int iSelItem = -1;
	POSITION pos = m_lstCtlOffers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlOffers.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		myMessageBox(IDS_COPY_NOITEMSELECTED, IDS_DLGMSG_WARNING, MB_OK | MB_ICONWARNING);
		return;
	}

	LONG lItemData = MAKELONG(1, 0);
	m_lstCtlOffers.SetItemData(iSelItem, lItemData);
	m_lstCtlOffers.Invalidate();

	m_bIsInCopyPaste = TRUE;
	m_iCopiedItem = iSelItem;
	UpdatePasteBtnState(m_bIsInCopyPaste);
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnPaste()
{
	DWORD dwItemCnt = m_lstCtlOffers.GetItemCount();
	if (FALSE == m_bIsInCopyPaste || (m_iCopiedItem < 0 || m_iCopiedItem >= (int)dwItemCnt))
	{
		m_bIsInCopyPaste = FALSE;
		m_iCopiedItem = -1;
		UpdatePasteBtnState(FALSE);
	}

	// Copy data in table
	int iNewItem = CopyOfferDataInTable(m_iCopiedItem);
	if (iNewItem <= 0)
		return;

	// Show new data in UI
	ShowTableRecord(&m_tbOffersInfo, iNewItem, &m_lstCtlOffers, m_stuOfferInfoTable);
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnImport()
{
	// Get Offer ID
	int iSelItem = -1;
	CString strOfferNo = _T("");
	POSITION pos = m_lstCtlOffers.GetFirstSelectedItemPosition();
	iSelItem = m_lstCtlOffers.GetNextSelectedItem(pos);
	if (iSelItem >= 0)
	{
		strOfferNo = m_lstCtlOffers.GetItemText(iSelItem, 0);
	}

	CBMOfferExportDlg dlg;
	dlg.SetCurSelOfferNo(strOfferNo);
	dlg.DoModal();
}
//-----------------------------------------------------------------------
void CBMOfferManagerDlg::OnBnClickedBtnExist()
{
	if (IDYES != myMessageBox(IDS_EXIT_CONFIRM, IDS_DLGMSG_WARNING, MB_YESNO | MB_ICONQUESTION))
		return;

	::PostQuitMessage(0);
}
//-----------------------------------------------------------------------