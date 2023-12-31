//-----------------------------------------------------------------------
//	File Name:BMOfferCalcBasicDataDlg.cpp
//	Introduce:Class CBMOfferCalcBasicDataDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcBasicDataDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcBasicDataDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferCalcBasicDataDlg, CDialog)
CBMOfferCalcBasicDataDlg::CBMOfferCalcBasicDataDlg(CWnd* pParent /*=NULL*/)
	: CBMCalcOptionsDlg(CBMOfferCalcBasicDataDlg::IDD, pParent)
{
	m_eCurCraneType = eCraneType_GTD;
	m_eCurCtrlType = eCraneCtrlType_Pendent;
	for (int i = 0; i < eCraneType_Cnt; i++)
		m_pHoistImages[i] = NULL;
	for (int i = 0; i < eCraneCtrlType_Cnt; i++)
		m_pCtrlTypeImages[i] = NULL;
	for (int i = 0; i < 5; i++)
		m_iHoistParams[i] = 0;
}
//-----------------------------------------------------------------------
CBMOfferCalcBasicDataDlg::~CBMOfferCalcBasicDataDlg()
{
	DestroyWindow();
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferCalcBasicDataDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_CTRLTYPE, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboCtrltype)
	ON_CBN_SELCHANGE(IDC_COMBO_GAITYPE, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboGaitype)
	ON_EN_KILLFOCUS(IDC_EDIT_CRANELOAD, &CBMOfferCalcBasicDataDlg::OnEnKillfocusEditCraneload)
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_EDIT_HEIGTH, &CBMOfferCalcBasicDataDlg::OnEnKillfocusEditHeigth)
	ON_EN_KILLFOCUS(IDC_EDIT_LENGTH, &CBMOfferCalcBasicDataDlg::OnEnKillfocusEditLength)
	ON_EN_KILLFOCUS(IDC_EDIT_TOPTOFLOOR, &CBMOfferCalcBasicDataDlg::OnEnKillfocusEditToptofloor)
	ON_EN_KILLFOCUS(IDC_EDIT_ROOFTOTOP, &CBMOfferCalcBasicDataDlg::OnEnKillfocusEditRooftotop)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO_HOISTTYPE, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboHoisttype)
	ON_CBN_SELCHANGE(IDC_COMBO_HOISTLOAD, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboHoistload)
	ON_CBN_SELCHANGE(IDC_COMBO_LEVEL, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboLevel)
	ON_CBN_SELCHANGE(IDC_COMBO_HOISTLEVEL, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboHoistlevel)
	ON_CBN_SELCHANGE(IDC_COMBO_POWER, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboPower)
	ON_CBN_SELCHANGE(IDC_COMBO_CTRLVOL, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboCtrlvol)
	ON_CBN_SELCHANGE(IDC_COMBO_WORKINGMODE, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboWorkingmode)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboSpeed)
	ON_CBN_SELCHANGE(IDC_COMBO_RAISE_CTLTYPE, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboRaiseCtltype)
	ON_CBN_SELCHANGE(IDC_COMBO_MOVESPEED, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboMovespeed)
	ON_CBN_SELCHANGE(IDC_COMBO_MOVE_CTLTYPE, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboMoveCtltype)
	ON_CBN_SELCHANGE(IDC_COMBO_MAINCARSPEED, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboMaincarspeed)
	ON_CBN_SELCHANGE(IDC_COMBO_MAINCAR_CTLTYPE, &CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboMaincarCtltype)
END_MESSAGE_MAP()


//-----------------------------------------------------------------------
// CBMOfferCalcBasicDataDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferCalcBasicDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bIsClosing = FALSE;
	FillComboBoxes();
	FillCraneCtrlTypeComboBox();
	FillHoistSpeedComboBox();
	if (FALSE == m_bDataIsInitliazed)
	{/*
		if (m_pParamCalculator->m_bIsEditData) InitUIWithDatabaseData();
		else */InitUIWithDefaultData();
		m_bDataIsInitliazed = TRUE;
	}

	// Load images
	for (int i = 0; i < eCraneType_Cnt; i++)
	{
		m_pHoistImages[i] =  new CImage();
	}
	m_pHoistImages[eCraneType_GTD]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOIST_D);
	m_pHoistImages[eCraneType_GTS]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOIST_S);
	m_pHoistImages[eCraneType_GTF]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOIST_D);
	m_rcHoistImage.left = 270;	m_rcHoistImage.right = m_rcHoistImage.left + 425;
	m_rcHoistImage.top = 140;	m_rcHoistImage.bottom = m_rcHoistImage.top + 280;	
	//
	for (int i = 1; i < eCraneCtrlType_Cnt; i++)
	{
		m_pCtrlTypeImages[i] =  new CImage();
	}
	m_pCtrlTypeImages[eCraneCtrlType_Remote]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_REMOTECTRL);
	m_pCtrlTypeImages[eCraneCtrlType_BlueTooth]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_BLUETOOTH);
	m_pCtrlTypeImages[eCraneCtrlType_Cabin]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_CABIN);
	//
	m_rcCtrlTypes[eCraneCtrlType_Remote].left = 150;	m_rcCtrlTypes[eCraneCtrlType_Remote].right = m_rcCtrlTypes[1].left + 100;
	m_rcCtrlTypes[eCraneCtrlType_Remote].bottom = 258;	m_rcCtrlTypes[eCraneCtrlType_Remote].top = m_rcCtrlTypes[1].bottom - 55;
	m_rcCtrlTypes[eCraneCtrlType_Remote].OffsetRect(m_rcHoistImage.left, m_rcHoistImage.top);
	//
	m_rcCtrlTypes[eCraneCtrlType_BlueTooth] = m_rcCtrlTypes[eCraneCtrlType_Remote];
	m_rcCtrlTypes[eCraneCtrlType_BlueTooth].right = m_rcCtrlTypes[eCraneCtrlType_BlueTooth].left + 116;
	//
	m_rcCtrlTypes[eCraneCtrlType_Cabin].left = 90;	m_rcCtrlTypes[eCraneCtrlType_Cabin].right = m_rcCtrlTypes[eCraneCtrlType_Cabin].left + 52;
	m_rcCtrlTypes[eCraneCtrlType_Cabin].top = 106;	m_rcCtrlTypes[eCraneCtrlType_Cabin].bottom = m_rcCtrlTypes[eCraneCtrlType_Cabin].top + 60;
	m_rcCtrlTypes[eCraneCtrlType_Cabin].OffsetRect(m_rcHoistImage.left, m_rcHoistImage.top);

	// Set parameters drawing rectages
	m_rcRail.left = 336;	m_rcRail.top = 122;
	m_rcRail.right = 368;	m_rcRail.bottom = 136;
	m_rcRail.OffsetRect(m_rcHoistImage.left, m_rcHoistImage.top);
	//
	m_rcMin.left = 336;		m_rcMin.top = 12;
	m_rcMin.right = 376;	m_rcMin.bottom = 26;
	m_rcMin.OffsetRect(m_rcHoistImage.left, m_rcHoistImage.top);
	//
	m_rcClearance.left = 54;	m_rcClearance.top = 10;
	m_rcClearance.right = 94;	m_rcClearance.bottom = 20;
	m_rcClearance.OffsetRect(m_rcHoistImage.left, m_rcHoistImage.top);
	//
	m_rcLoad.left = 140;		m_rcLoad.top = 72;
	m_rcLoad.right = 210;		m_rcLoad.bottom = 86;
	m_rcLoad.OffsetRect(m_rcHoistImage.left, m_rcHoistImage.top);
	//
	m_rcHOL.left = 210;			m_rcHOL.top = 140;
	m_rcHOL.right = 300;		m_rcHOL.bottom = 156; 
	m_rcHOL.OffsetRect(m_rcHoistImage.left, m_rcHoistImage.top);
	//
	m_rcSpan.left = 170;		m_rcSpan.top = 180;
	m_rcSpan.right = 270;		m_rcSpan.bottom = 196; 
	m_rcSpan.OffsetRect(m_rcHoistImage.left, m_rcHoistImage.top);
	//
	m_rcTopToFloor.left = -4;	m_rcTopToFloor.top = 140;
	m_rcTopToFloor.right = 8;	m_rcTopToFloor.bottom = 240; 
	m_rcTopToFloor.OffsetRect(m_rcHoistImage.left, m_rcHoistImage.top);
	//
	m_rcTopToRoof.left = -4;	m_rcTopToRoof.top = 50;
	m_rcTopToRoof.right = 8;	m_rcTopToRoof.bottom = 120; 
	m_rcTopToRoof.OffsetRect(m_rcHoistImage.left, m_rcHoistImage.top);
	//
	m_rcTopToCraneTop.left = 10;	m_rcTopToCraneTop.top = 60;
	m_rcTopToCraneTop.right = 22;	m_rcTopToCraneTop.bottom = 110; 
	m_rcTopToCraneTop.OffsetRect(m_rcHoistImage.left, m_rcHoistImage.top);

	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_lFont);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBMCalcOptionsDlg::OnShowWindow(bShow, nStatus);
	if (!bShow)
		return;

	// Re-select the rail type here, because user can change rail type in EndCar page
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GAITYPE);
	SelectComboBoxItem(pComboBox, _T("P22")/*m_pParamCalculator->m_strRailType*/);
	//OnCbnSelchangeComboGaitype(); commented by Demo
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (NULL == m_pHoistImages[m_eCurCraneType])
		return;

	int iOldBKMode = dc.SetBkMode(TRANSPARENT);

	// Draw crane image
	if (NULL != m_pHoistImages[m_eCurCraneType])
	{
		CRect rcImage(0, 0, m_pHoistImages[m_eCurCraneType]->GetWidth(), m_pHoistImages[m_eCurCraneType]->GetHeight());
		m_pHoistImages[m_eCurCraneType]->StretchBlt(dc.m_hDC, m_rcHoistImage, rcImage);
	}

	// Draw params here!!!
	HFONT hFont = CreateFontIndirect(&m_lFont);
	HFONT hOldFont = (HFONT)SelectObject(dc.m_hDC, hFont);
	DrawText(dc.m_hDC, m_strRail, m_strRail.GetLength(), &m_rcRail, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, m_strMin, m_strMin.GetLength(), &m_rcMin, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, m_strClearance, m_strClearance.GetLength(), &m_rcClearance, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, m_strLoad, m_strLoad.GetLength(), &m_rcLoad, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, m_strHOL, m_strHOL.GetLength(), &m_rcHOL, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, m_strSpan, m_strSpan.GetLength(), &m_rcSpan, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	//
	LOGFONT lfV;
	memcpy(&lfV, &m_lFont, sizeof(LOGFONT));
	lfV.lfEscapement = 900;
	lfV.lfOrientation = 900;
	HFONT hVFont = CreateFontIndirect(&lfV);
	SelectObject(dc.m_hDC, hVFont);
	DrawText(dc.m_hDC, m_strTopToFloor, m_strTopToFloor.GetLength(), &m_rcTopToFloor, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, m_strTopToRoof, m_strTopToRoof.GetLength(), &m_rcTopToRoof, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, m_strTopToCraneTop, m_strTopToCraneTop.GetLength(), &m_rcTopToCraneTop, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	//
	SelectObject(dc.m_hDC, hOldFont);
	DeleteObject(hFont);
	DeleteObject(hVFont);

	// Draw control type image
	if (m_pCtrlTypeImages[m_eCurCtrlType] != NULL)
	{
		CRect rcImage(0, 0, m_pCtrlTypeImages[m_eCurCtrlType]->GetWidth(), m_pCtrlTypeImages[m_eCurCtrlType]->GetHeight());
		m_pCtrlTypeImages[m_eCurCtrlType]->StretchBlt(dc.m_hDC, m_rcCtrlTypes[m_eCurCtrlType], rcImage);
	}		

	dc.SetBkMode(iOldBKMode);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnDestroy()
{
	CDialog::OnDestroy();

	for (int i = 0; i < 3; i++)
	{
		if (NULL != m_pHoistImages[i])
		{
			delete m_pHoistImages[i];
			m_pHoistImages[i] = NULL;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (NULL != m_pCtrlTypeImages[i])
		{
			delete m_pCtrlTypeImages[i];
			m_pCtrlTypeImages[i] = NULL;
		}
	}	
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnClose()
{
	m_bIsClosing = TRUE;

	CDialog::OnClose();
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboType()
{		
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	SelectComboBoxItem(pComboBox, eCraneType_GTS);
	
	/*Comment by Demo
	m_eCurCraneType = (eCraneType)pComboBox->GetCurSel();
	UpdateControlsStatus(m_eCurCraneType);
	FillHoistTypeComboBox();
	FillHoistSpeedComboBox();
	FillCraneCtrlTypeComboBox();
	*/
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboCtrltype()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLTYPE);
	pComboBox->SetCurSel(0);
	/*
	int iSelIndex = pComboBox->GetCurSel();
	m_eCurCtrlType = (eCraneCtrlType)pComboBox->GetItemData(iSelIndex);
	InvalidateRect(&m_rcHoistImage);
	*/
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboGaitype()
{	
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GAITYPE);
	pComboBox->SetCurSel(0);
/*
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GAITYPE);
	pComboBox->GetWindowText(m_strRail);
	InvalidateRect(&m_rcHoistImage);*/
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnEnKillfocusEditCraneload()
{
	if (TRUE == m_bIsClosing)
		return;

	CString strTmp;
	LONG lCraneLoad = 0;
	GetDlgItemText(IDC_EDIT_CRANELOAD, strTmp);
	lCraneLoad = _ttol(strTmp);
	GetDlgItemText(IDC_EDIT_ROOFTOTOP, strTmp);
	int iRailTopToRoof = _ttol(strTmp);

	int iMin = 0;
	CBMParamCalculator::GetMinAndClearanceSize(m_eCurCraneType, lCraneLoad, iMin, m_iClearance);

	m_strMin.Format(_T("%d mm"), iMin);
	m_strClearance.Format(_T("%d mm"), m_iClearance);
	m_strLoad.Format(_T("%.1f T"), lCraneLoad/1000.0f);
	//	
	int iTopToCraneTop = max(0, iRailTopToRoof - m_iClearance);
	m_strTopToCraneTop.Format(_T("%d"), iTopToCraneTop);

	InvalidateRect(&m_rcHoistImage);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnEnKillfocusEditHeigth()
{
	if (TRUE == m_bIsClosing)
		return;

	CString strTmp;
	GetDlgItemText(IDC_EDIT_HEIGTH, strTmp);
	double fHOL = _tstof(strTmp);
	m_strHOL.Format(_T("HOL = %d mm "), (int)(fHOL*1000));
	InvalidateRect(&m_rcHoistImage);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnEnKillfocusEditLength()
{
	if (TRUE == m_bIsClosing)
		return;

	CString strTmp;
	double fSpan = 0;
	GetDlgItemText(IDC_EDIT_LENGTH, strTmp);
	fSpan = _tstof(strTmp);
	m_strSpan.Format(_T("Span = %d mm "), int(fSpan*1000));
	InvalidateRect(&m_rcHoistImage);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnEnKillfocusEditToptofloor()
{
	if (TRUE == m_bIsClosing)
		return;

	CString strTmp;
	LONG lRailTopToFloor = 0;
	GetDlgItemText(IDC_EDIT_TOPTOFLOOR, strTmp);
	lRailTopToFloor = _ttol(strTmp);
	m_strTopToFloor.Format(_T("%d "), lRailTopToFloor);
	InvalidateRect(&m_rcHoistImage);	
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnEnKillfocusEditRooftotop()
{
	if (TRUE == m_bIsClosing)
		return;

	CString strTmp;
	GetDlgItemText(IDC_EDIT_ROOFTOTOP, strTmp);
	int iRailTopToRoof = _ttol(strTmp);
	m_strTopToRoof.Format(_T("%d "), iRailTopToRoof);
	//
	int iTopToCraneTop = max(0, iRailTopToRoof - m_iClearance);
	m_strTopToCraneTop.Format(_T("%d"), iTopToCraneTop);

	InvalidateRect(&m_rcHoistImage);	
}
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
BOOL CBMOfferCalcBasicDataDlg::CanDoNextStep()
{
	CString strTmp;
	CComboBox* pComboBox = NULL;

	//Crane Type
	m_pParamCalculator->m_eCraneType = m_eCurCraneType;

	//Number of hoist
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTTYPE);
	int iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_eNumOfHoist = (eHoistNumType)pComboBox->GetItemData(iSelIndex);
	switch(m_pParamCalculator->m_eNumOfHoist)
	{
	case eHoistNumType_One:
	case eHoistNumType_MainAux:
		m_pParamCalculator->m_iNumOfHoist = 1;
		break;
	case eHoistNumType_TwoSimilar:
		m_pParamCalculator->m_iNumOfHoist = 2;
		break;		 
	default:
		m_pParamCalculator->m_iNumOfHoist = 1;
		break;
	}

	//Load of crane
	GetDlgItemText(IDC_EDIT_CRANELOAD, strTmp);
	m_pParamCalculator->m_iLoadOfCrane = _ttol(strTmp);	
	if ((eCraneType_GTD == m_pParamCalculator->m_eCraneType) && (m_pParamCalculator->m_iLoadOfCrane < 1000 || m_pParamCalculator->m_iLoadOfCrane > 160000) ||
		(eCraneType_GTS == m_pParamCalculator->m_eCraneType) && (m_pParamCalculator->m_iLoadOfCrane > 20000))
	{
		myMessageBox(IDS_CALC_LOADISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_CRANELOAD)->SetFocus();
		return FALSE;
	}

	//Load of hoist
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLOAD);
	pComboBox->GetWindowText(strTmp);
	m_pParamCalculator->m_iLoadOfHoist = _ttol(strTmp);

	//Span
	GetDlgItemText(IDC_EDIT_LENGTH, strTmp);
	m_pParamCalculator->m_fSpan = _tstof(strTmp);
	if (m_pParamCalculator->m_fSpan <= 0)
	{
		myMessageBox(IDS_CALC_SPANISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_LENGTH)->SetFocus();
		return FALSE;
	}

	//HOL
	GetDlgItemText(IDC_EDIT_HEIGTH, strTmp);
	m_pParamCalculator->m_fHOL = _tstof(strTmp);
	if (m_pParamCalculator->m_fHOL <= 0)
	{
		myMessageBox(IDS_CALC_HOLISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_HEIGTH)->SetFocus();
		return FALSE;
	}

	//Crane Duty
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LEVEL);
	pComboBox->GetWindowText(m_pParamCalculator->m_strDutyOfCrane);
	
	//Hoist Duty
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLEVEL);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_iDutyOfHoist = pComboBox->GetItemData(iSelIndex);
	pComboBox->GetWindowText(m_pParamCalculator->m_strDutyOfHoist);	

	//Rail Type
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GAITYPE);
	pComboBox->GetWindowText(m_pParamCalculator->m_strRailType);
	strTmp.Format(_T("SELECT * FROM %s WHERE RaiTyp='%s'"), g_tcTableName[eTableID_DatRai], m_pParamCalculator->m_strRailType);
	m_pParamCalculator->m_iRaiWidTop = (int)GetFLOATValueFromTable(strTmp, _T("RaiWidTop"));

	//Rail length
	GetDlgItemText(IDC_EDIT_GAILENGTH, strTmp);
	m_pParamCalculator->m_fRailLenght = _tstof(strTmp);
	if (m_pParamCalculator->m_fRailLenght <= 0)
	{
		myMessageBox(IDS_CALC_RIALLENGHTISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_GAILENGTH)->SetFocus();
		return FALSE;
	}

	//Rail top to floor
	GetDlgItemText(IDC_EDIT_TOPTOFLOOR, strTmp);
	m_pParamCalculator->m_iRailTopToFloor = _ttol(strTmp);
/*	if (m_pParamCalculator->m_iRailTopToFloor <= 0)
	{
		myMessageBox(IDS_CALC_TOPTOFLOORISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_TOPTOFLOOR)->SetFocus();
		return FALSE;
	}*/

	//Roof to rail top
	GetDlgItemText(IDC_EDIT_ROOFTOTOP, strTmp);
	m_pParamCalculator->m_iRoofToRailTop = _ttol(strTmp);
	/*if (m_pParamCalculator->m_iRoofToRailTop <= 0)
	{
		myMessageBox(IDS_CALC_ROOFTOTOPISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_ROOFTOTOP)->SetFocus();
		return FALSE;
	}
*/
	//Power supply
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_POWER);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_iVoltage1_Main = pComboBox->GetItemData(iSelIndex);

	//Control voltge
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLVOL);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_iVoltage2_Ctrl = pComboBox->GetItemData(iSelIndex);

	//Crane control
	m_pParamCalculator->m_eCraneControl = m_eCurCtrlType;

	//Main use
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WORKINGMODE);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_eMainUseType = (eCraneMainUseType)pComboBox->GetItemData(iSelIndex);

	//Hoisting speed
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SPEED);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_fHoistSpeed = pComboBox->GetItemData(iSelIndex) / 100.0f;
	pComboBox->GetWindowText(m_pParamCalculator->m_strHoistingSpeed);

	//Hoist control type 
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAISE_CTLTYPE);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_eHoistCtrlType = (eElecCtrlType)pComboBox->GetItemData(iSelIndex);

	//Trolley speed
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVESPEED);
	pComboBox->GetWindowText(m_pParamCalculator->m_strTrolleySpeed);

	//Trolley control type
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVE_CTLTYPE);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_eTrolleyCtrlType = (eElecCtrlType)pComboBox->GetItemData(iSelIndex);

	//Bridge speed
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCARSPEED);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_iHoist_V3 = pComboBox->GetItemData(iSelIndex);
	pComboBox->GetWindowText(m_pParamCalculator->m_strBridgeSpeed);

	//Bridge control type
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCAR_CTLTYPE);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_eBridgeCtrlType = (eElecCtrlType)pComboBox->GetItemData(iSelIndex);

	return TRUE;
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::FillComboBoxes()
{
	int iItemIndex = 0;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	pComboBox->ResetContent();
	iItemIndex = pComboBox->AddString(_T("Double girder cranes"));
	pComboBox->SetItemData(iItemIndex, eCraneType_GTD);
	iItemIndex = pComboBox->AddString(_T("Single girder cranes"));
	pComboBox->SetItemData(iItemIndex, eCraneType_GTS);
	if (m_pParamCalculator->m_bOnlyHoist)
	{
		iItemIndex = pComboBox->AddString(_T("Only fixed hoist"));
		pComboBox->SetItemData(iItemIndex, eCraneType_GTF);
	}
	pComboBox->SetCurSel(1);
	//
	FillHoistTypeComboBox();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLOAD);
	pComboBox->ResetContent();
	iItemIndex = pComboBox->AddString(_T("1000"));
	pComboBox->SetItemData(iItemIndex, 1000);
	iItemIndex = pComboBox->AddString(_T("1600"));
	pComboBox->SetItemData(iItemIndex, 1600);
	iItemIndex = pComboBox->AddString(_T("2000"));
	pComboBox->SetItemData(iItemIndex, 2000);
	iItemIndex = pComboBox->AddString(_T("2500"));
	pComboBox->SetItemData(iItemIndex, 2500);
	iItemIndex = pComboBox->AddString(_T("3000"));
	pComboBox->SetItemData(iItemIndex, 3200);
	iItemIndex = pComboBox->AddString(_T("4000"));
	pComboBox->SetItemData(iItemIndex, 4000);
	iItemIndex = pComboBox->AddString(_T("5000"));
	pComboBox->SetItemData(iItemIndex, 5000);
	iItemIndex = pComboBox->AddString(_T("6000"));
	pComboBox->SetItemData(iItemIndex, 6000);
	iItemIndex = pComboBox->AddString(_T("6300"));
	pComboBox->SetItemData(iItemIndex, 6300);
	iItemIndex = pComboBox->AddString(_T("8000"));
	pComboBox->SetItemData(iItemIndex, 8000);
	iItemIndex = pComboBox->AddString(_T("10000"));
	pComboBox->SetItemData(iItemIndex, 10000);
	iItemIndex = pComboBox->AddString(_T("12500"));
	pComboBox->SetItemData(iItemIndex, 12500);
	iItemIndex = pComboBox->AddString(_T("15000"));
	pComboBox->SetItemData(iItemIndex, 15000);
	iItemIndex = pComboBox->AddString(_T("16000"));
	pComboBox->SetItemData(iItemIndex, 16000);
	iItemIndex = pComboBox->AddString(_T("20000"));
	pComboBox->SetItemData(iItemIndex, 20000);
	iItemIndex = pComboBox->AddString(_T("25000"));
	pComboBox->SetItemData(iItemIndex, 25000);
	iItemIndex = pComboBox->AddString(_T("32000"));
	pComboBox->SetItemData(iItemIndex, 32000);
	iItemIndex = pComboBox->AddString(_T("40000"));
	pComboBox->SetItemData(iItemIndex, 40000);
	iItemIndex = pComboBox->AddString(_T("50000"));
	pComboBox->SetItemData(iItemIndex, 50000);
	iItemIndex = pComboBox->AddString(_T("63000"));
	pComboBox->SetItemData(iItemIndex, 63000);
	iItemIndex = pComboBox->AddString(_T("70000"));
	pComboBox->SetItemData(iItemIndex, 70000);
	iItemIndex = pComboBox->AddString(_T("75000"));
	pComboBox->SetItemData(iItemIndex, 75000);
	iItemIndex = pComboBox->AddString(_T("80000"));
	pComboBox->SetItemData(iItemIndex, 80000);
	pComboBox->SetCurSel(6);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LEVEL);
	pComboBox->ResetContent();
	pComboBox->AddString(_T("A3"));
	pComboBox->AddString(_T("A4"));
	pComboBox->AddString(_T("A5"));
	pComboBox->AddString(_T("A6"));
	pComboBox->SetCurSel(2);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLEVEL);
	pComboBox->ResetContent();
	iItemIndex = pComboBox->AddString(_T("1Am/M4"));
	pComboBox->SetItemData(iItemIndex, 4);
	iItemIndex = pComboBox->AddString(_T("2m/M5"));
	pComboBox->SetItemData(iItemIndex, 5);
	iItemIndex = pComboBox->AddString(_T("3m/M6"));
	pComboBox->SetItemData(iItemIndex, 6);
	iItemIndex = pComboBox->AddString(_T("4m/M7"));
	pComboBox->SetItemData(iItemIndex, 7);
	pComboBox->SetCurSel(1);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GAITYPE);
	pComboBox->ResetContent();
	pComboBox->AddString(_T("P22"));
	pComboBox->AddString(_T("P24"));
	pComboBox->AddString(_T("P30"));
	pComboBox->AddString(_T("P38"));
	pComboBox->AddString(_T("P43"));
	pComboBox->AddString(_T("QU70"));
	pComboBox->AddString(_T("QU80"));
	pComboBox->SetCurSel(0);	
	//OnCbnSelchangeComboGaitype(); commented by Demo
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_POWER);
	pComboBox->ResetContent();
	iItemIndex = pComboBox->AddString(_T("380"));
	pComboBox->SetItemData(iItemIndex, 380);
	iItemIndex = pComboBox->AddString(_T("400"));
	pComboBox->SetItemData(iItemIndex, 400);
	iItemIndex = pComboBox->AddString(_T("415"));
	pComboBox->SetItemData(iItemIndex, 415);
	iItemIndex = pComboBox->AddString(_T("460"));
	pComboBox->SetItemData(iItemIndex, 460);
	iItemIndex = pComboBox->AddString(_T("500"));
	pComboBox->SetItemData(iItemIndex, 500);
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLVOL);
	pComboBox->ResetContent();
	iItemIndex = pComboBox->AddString(_T("48"));
	pComboBox->SetItemData(iItemIndex, 48);
	iItemIndex = pComboBox->AddString(_T("115"));
	pComboBox->SetItemData(iItemIndex, 115);
	iItemIndex = pComboBox->AddString(_T("220"));
	pComboBox->SetItemData(iItemIndex, 220);
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WORKINGMODE);
	pComboBox->ResetContent();
	iItemIndex = pComboBox->AddString(_T("Indoors"));
	pComboBox->SetItemData(iItemIndex, eCraneMainUseType_Indoors);
	iItemIndex = pComboBox->AddString(_T("Outdoors"));
	pComboBox->SetItemData(iItemIndex, eCraneMainUseType_Outdoors);
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAISE_CTLTYPE);
	iItemIndex = pComboBox->AddString(_T("2-SPEED"));
	pComboBox->SetItemData(iItemIndex, eElecCtrlType_2Speeds);
	iItemIndex = pComboBox->AddString(_T("Inverter"));
	pComboBox->SetItemData(iItemIndex, eElecCtrlType_Inverter);
	pComboBox->SetCurSel(1);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVE_CTLTYPE);
	iItemIndex = pComboBox->AddString(_T("2-SPEED"));
	pComboBox->SetItemData(iItemIndex, eElecCtrlType_2Speeds);
	iItemIndex = pComboBox->AddString(_T("Inverter"));
	pComboBox->SetItemData(iItemIndex, eElecCtrlType_Inverter);
	pComboBox->SetCurSel(1);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCAR_CTLTYPE);
	iItemIndex = pComboBox->AddString(_T("2-SPEED"));
	pComboBox->SetItemData(iItemIndex, eElecCtrlType_2Speeds);
	iItemIndex = pComboBox->AddString(_T("Inverter"));
	pComboBox->SetItemData(iItemIndex, eElecCtrlType_Inverter);
	pComboBox->SetCurSel(1);			
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVESPEED);
	iItemIndex = pComboBox->AddString(_T("20/5"));
	pComboBox->SetItemData(iItemIndex, 20);
	//iItemIndex = pComboBox->AddString(_T("32/8"));
	//pComboBox->SetItemData(iItemIndex, 32);
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCARSPEED);
	iItemIndex = pComboBox->AddString(_T("20/5"));
	pComboBox->SetItemData(iItemIndex, 20);
	iItemIndex = pComboBox->AddString(_T("32/8"));
	pComboBox->SetItemData(iItemIndex, 32);
	iItemIndex = pComboBox->AddString(_T("40/10"));
	pComboBox->SetItemData(iItemIndex, 40);
	pComboBox->SetCurSel(0);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::FillHoistTypeComboBox()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTTYPE);
	pComboBox->ResetContent();
	int iItemIndex = pComboBox->AddString(_T("One hoist"));
	pComboBox->SetItemData(iItemIndex, eHoistNumType_One);
	if (FALSE == m_pParamCalculator->m_bOnlyHoist)
	{
		if (eCraneType_GTD == m_eCurCraneType)
		{
			iItemIndex = pComboBox->AddString(_T("One main/Aux.hoist"));
			pComboBox->SetItemData(iItemIndex, eHoistNumType_MainAux);
			//iItemIndex = pComboBox->AddString(_T("Two similar hoist"));
			//pComboBox->SetItemData(iItemIndex, eHoistNumType_TwoSimilar);
		}
	}
	pComboBox->SetCurSel(0);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::FillCraneCtrlTypeComboBox()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLTYPE);
	pComboBox->ResetContent();
	int iItemIndex = pComboBox->AddString(_T("Pendent"));
	pComboBox->SetItemData(iItemIndex, eCraneCtrlType_Pendent);
	iItemIndex = pComboBox->AddString(_T("Remote control"));
	pComboBox->SetItemData(iItemIndex, eCraneCtrlType_Remote);
	iItemIndex = pComboBox->AddString(_T("Blue tooth"));
	pComboBox->SetItemData(iItemIndex, eCraneCtrlType_BlueTooth);
	iItemIndex = pComboBox->AddString(_T("Cabin"));
	pComboBox->SetItemData(iItemIndex, eCraneCtrlType_Cabin);
	pComboBox->SetCurSel(0);
	//OnCbnSelchangeComboCtrltype(); commented by Demo
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::FillHoistSpeedComboBox()
{
	eTableID tbID;
	switch (m_eCurCraneType)
	{
	case eCraneType_GTD:
		tbID = eTableID_HoiSel_D;
		break;
	case eCraneType_GTS:
		tbID = eTableID_HoiSel_S;
		break;
	case eCraneType_GTF:
		tbID = eTableID_HoiSel_F;
		break;
	}
	//
	CADOTable tbHoistInfo;
	CString strOperations;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SPEED);
	pComboBox->ResetContent();
	//
	strOperations.Format(_T("SELECT DISTINCT HoiSpe FROM %s"), g_tcTableName[tbID]);
	if (TRUE == tbHoistInfo.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOperations))
	{
		DWORD dwUserCnt = tbHoistInfo.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwUserCnt; dwIndex++)
		{
			CString strSpeed;
			TCHAR tcSpeed[32] = {0};
			tbHoistInfo.GetRecordCell(dwIndex, L"HoiSpe", tcSpeed, 32);
			strSpeed = tcSpeed;
			if (!strSpeed.IsEmpty())
			{
				if (strSpeed.Find(_T('/')) != -1)
					strSpeed = strSpeed.Right(strSpeed.GetLength() - (strSpeed.Find(_T('/'))+1));
				else if (strSpeed.Find(_T('-')) != -1)
					strSpeed = strSpeed.Right(strSpeed.GetLength() - (strSpeed.Find(_T('-'))+1));
				int iItemIndex = pComboBox->AddString(tcSpeed);
				pComboBox->SetItemData(iItemIndex, int(_tstof(strSpeed) * 100));
			}
		}
		tbHoistInfo.CloseTable();
	}		
	
	pComboBox->SetCurSel(0);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::InitUIWithDefaultData()
{
	/* Initliaze data by default values*/
	CString strTemp;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	SelectComboBoxItem(pComboBox, eCraneType_GTS);
	m_eCurCraneType = eCraneType_GTS;
	//
	UpdateControlsStatus(m_eCurCraneType);
	FillHoistTypeComboBox();
	FillHoistSpeedComboBox();
	FillCraneCtrlTypeComboBox();
	//OnCbnSelchangeComboType(); commented by Demo
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTTYPE);
	SelectComboBoxItem(pComboBox, eHoistNumType_One);
	//
	strTemp.Format(_T("%d"), 5000);
	SetDlgItemText(IDC_EDIT_CRANELOAD, strTemp);
	OnEnKillfocusEditCraneload();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLOAD);
	SelectComboBoxItem(pComboBox, 5000);
	//
	strTemp.Format(_T("%0.2f"), 22.5f);
	SetDlgItemText(IDC_EDIT_LENGTH, strTemp);
	OnEnKillfocusEditLength();
	//
	strTemp.Format(_T("%0.2f"), 9.0f);
	SetDlgItemText(IDC_EDIT_HEIGTH, strTemp);
	OnEnKillfocusEditHeigth();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LEVEL);
	SelectComboBoxItem(pComboBox, _T("A5"));
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLEVEL);
	SelectComboBoxItem(pComboBox, _T("2m/M5"));
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GAITYPE);
	SelectComboBoxItem(pComboBox, _T("P22"));
	//OnCbnSelchangeComboGaitype();  commented by Demo
	//
	strTemp.Format(_T("%0.2f"), 50.0f);
	SetDlgItemText(IDC_EDIT_GAILENGTH, strTemp);
	//
	strTemp.Format(_T("%d"), 9000);
	SetDlgItemText(IDC_EDIT_TOPTOFLOOR, strTemp);
	OnEnKillfocusEditToptofloor();
	//
	strTemp.Format(_T("%d"), 2000);
	SetDlgItemText(IDC_EDIT_ROOFTOTOP, strTemp);
	OnEnKillfocusEditRooftotop();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_POWER);
	SelectComboBoxItem(pComboBox, 380);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLVOL);
	SelectComboBoxItem(pComboBox, 48);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLTYPE);
	SelectComboBoxItem(pComboBox, eCraneCtrlType_Pendent);
	InvalidateRect(&m_rcHoistImage);
	//OnCbnSelchangeComboCtrltype(); commented by Demo
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WORKINGMODE);
	SelectComboBoxItem(pComboBox, eCraneMainUseType_Indoors);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SPEED);
	SelectComboBoxItem(pComboBox, _T("0.8/5.0"));
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAISE_CTLTYPE);
	SelectComboBoxItem(pComboBox, eElecCtrlType_2Speeds);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVESPEED);
	SelectComboBoxItem(pComboBox, _T("20/5"));
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVE_CTLTYPE);
	SelectComboBoxItem(pComboBox, eElecCtrlType_Inverter);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCARSPEED);
	SelectComboBoxItem(pComboBox, _T("32/8"));
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCAR_CTLTYPE);
	SelectComboBoxItem(pComboBox, eElecCtrlType_Inverter);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::InitUIWithDatabaseData()
{
	/* Initliaze data by database values*/
	CString strTemp;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_eCraneType);
	OnCbnSelchangeComboType();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_eNumOfHoist);
	//
	strTemp.Format(_T("%d"), m_pParamCalculator->m_iLoadOfCrane);
	SetDlgItemText(IDC_EDIT_CRANELOAD, strTemp);
	OnEnKillfocusEditCraneload();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLOAD);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iLoadOfHoist);
	//
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_fSpan);
	SetDlgItemText(IDC_EDIT_LENGTH, strTemp);
	OnEnKillfocusEditLength();
	//
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_fHOL);
	SetDlgItemText(IDC_EDIT_HEIGTH, strTemp);
	OnEnKillfocusEditHeigth();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LEVEL);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_strDutyOfCrane);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLEVEL);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_strDutyOfHoist);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GAITYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_strRailType);
	//OnCbnSelchangeComboGaitype(); commented by Demo
	//
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_fRailLenght);
	SetDlgItemText(IDC_EDIT_GAILENGTH, strTemp);
	//
	strTemp.Format(_T("%d"), m_pParamCalculator->m_iRailTopToFloor);
	SetDlgItemText(IDC_EDIT_TOPTOFLOOR, strTemp);
	OnEnKillfocusEditToptofloor();
	//
	strTemp.Format(_T("%d"), m_pParamCalculator->m_iRoofToRailTop);
	SetDlgItemText(IDC_EDIT_ROOFTOTOP, strTemp);
	OnEnKillfocusEditRooftotop();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_POWER);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iVoltage1_Main);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLVOL);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iVoltage2_Ctrl);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_eCraneControl);
	//OnCbnSelchangeComboCtrltype(); commente by Demo
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WORKINGMODE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_eMainUseType);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SPEED);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_strHoistingSpeed);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAISE_CTLTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_eHoistCtrlType);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVESPEED);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_strTrolleySpeed);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVE_CTLTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_eTrolleyCtrlType);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCARSPEED);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_strBridgeSpeed);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCAR_CTLTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_eBridgeCtrlType);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::UpdateControlsStatus(eCraneType eCraneType)
{
	if (eCraneType_GTF == eCraneType)
	{
		GetDlgItem(IDC_COMBO_HOISTTYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CRANELOAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_GAITYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAILENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TOPTOFLOOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ROOFTOTOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_MOVESPEED)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_MOVE_CTLTYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_MAINCARSPEED)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_MAINCAR_CTLTYPE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_HOISTTYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CRANELOAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_LEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_GAITYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAILENGTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TOPTOFLOOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROOFTOTOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_MOVESPEED)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_MOVE_CTLTYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_MAINCARSPEED)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_MAINCAR_CTLTYPE)->EnableWindow(TRUE);
	}
}
//-----------------------------------------------------------------------





//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboHoisttype()
{	
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTTYPE);
	SelectComboBoxItem(pComboBox, eHoistNumType_One);
}

void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboHoistload()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLOAD);
	SelectComboBoxItem(pComboBox, 50000);
}

void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboLevel()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LEVEL);
	SelectComboBoxItem(pComboBox, _T("A5"));
}

void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboHoistlevel()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLEVEL);
	SelectComboBoxItem(pComboBox, _T("2m/M5"));
}

void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboPower()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_POWER);
	SelectComboBoxItem(pComboBox, 380);
}

void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboCtrlvol()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLVOL);
	SelectComboBoxItem(pComboBox, 48);
}

void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboWorkingmode()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WORKINGMODE);
	SelectComboBoxItem(pComboBox, eCraneMainUseType_Indoors);
}

void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboSpeed()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SPEED);
	SelectComboBoxItem(pComboBox, _T("0.53-3.2"));
}

void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboRaiseCtltype()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAISE_CTLTYPE);
	SelectComboBoxItem(pComboBox, eElecCtrlType_2Speeds);
}

void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboMovespeed()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVESPEED);
	SelectComboBoxItem(pComboBox, _T("20/5"));
}

void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboMoveCtltype()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVE_CTLTYPE);
	SelectComboBoxItem(pComboBox, eElecCtrlType_Inverter);
}

void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboMaincarspeed()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCARSPEED);
	SelectComboBoxItem(pComboBox, _T("32/8"));
}

void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboMaincarCtltype()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCAR_CTLTYPE);
	SelectComboBoxItem(pComboBox, eElecCtrlType_Inverter);
}
