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
	for (int i = 0; i < eCraneType_Cnt; i++)
		m_pHoistImages[i] = NULL;
	for (int i = 0; i < eCraneCtrlType_Cnt; i++)
		m_pCtrlTypeImages[i] = NULL;
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
END_MESSAGE_MAP()


//-----------------------------------------------------------------------
// CBMOfferCalcBasicDataDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferCalcBasicDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bIsClosing = FALSE;
	FillComboBoxes();
	FillRailTypeComboBox();
	FillCraneCtrlTypeComboBox();
	FillHoistSpeedComboBox();
	if (FALSE == m_bDataIsInitliazed)
	{
		if (m_pParamCalculator->m_bIsEditData) InitUIWithDatabaseData();
		else InitUIWithDefaultData();
		m_bDataIsInitliazed = TRUE;
	}

	// Load images
	for (int i = 0; i < eCraneType_Cnt; i++)
	{
		m_pHoistImages[i] =  new CImage();
	}
	m_pHoistImages[eCraneType_GTD]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOIST_D);
	m_pHoistImages[eCraneType_GTS]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOIST_S);
	m_pHoistImages[eCraneType_GTZ]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOIST_D);
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
	m_rcTopToCraneTop.right = 22;	m_rcTopToCraneTop.bottom = 130; 
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
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GA9);
	OnCbnSelchangeComboGaitype();
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CImage* pCurCraneImage = NULL;
	if (_T("BMD") == m_pParamCalculator->m_CalcDatalst.GA1)
		pCurCraneImage = m_pHoistImages[eCraneType_GTD];
	else if (_T("BMS") == m_pParamCalculator->m_CalcDatalst.GA1)
		pCurCraneImage = m_pHoistImages[eCraneType_GTS];
	else if (_T("BMZ") == m_pParamCalculator->m_CalcDatalst.GA1)
		pCurCraneImage = m_pHoistImages[eCraneType_GTZ];
	else if (_T("BMU") == m_pParamCalculator->m_CalcDatalst.GA1)
		pCurCraneImage = NULL;
	else
		pCurCraneImage = NULL;

	if (NULL == pCurCraneImage)
		return;

	int iOldBKMode = dc.SetBkMode(TRANSPARENT);

	// Draw crane image
	if (NULL != pCurCraneImage)
	{
		CRect rcImage(0, 0, pCurCraneImage->GetWidth(), pCurCraneImage->GetHeight());
		pCurCraneImage->StretchBlt(dc.m_hDC, m_rcHoistImage, rcImage);
	}

	// Draw params here!!!
	CString strRail;
	CString strMin;	
	CString strClearance;	
	CString strLoad;	
	CString strHOL;	
	CString strSpan;	
	strRail = m_pParamCalculator->m_CalcDatalst.GD3;
	strMin.Format(_T("%d mm"), m_pParamCalculator->m_CalcDatalst.GD5);
	strClearance.Format(_T("%d mm"), m_pParamCalculator->m_CalcDatalst.GD8);
	strLoad.Format(_T("%.1f T"), m_pParamCalculator->m_CalcDatalst.GA3/1000.0f);
	strHOL.Format(_T("HOL = %d mm "), (int)(m_pParamCalculator->m_CalcDatalst.GA6*1000));
	strSpan.Format(_T("Span = %d mm "), int(m_pParamCalculator->m_CalcDatalst.GA5*1000));
	//
	HFONT hFont = CreateFontIndirect(&m_lFont);
	HFONT hOldFont = (HFONT)SelectObject(dc.m_hDC, hFont);
	DrawText(dc.m_hDC, strRail, strRail.GetLength(), &m_rcRail, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, strMin, strMin.GetLength(), &m_rcMin, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, strClearance, strClearance.GetLength(), &m_rcClearance, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, strLoad, strLoad.GetLength(), &m_rcLoad, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, strHOL, strHOL.GetLength(), &m_rcHOL, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, strSpan, strSpan.GetLength(), &m_rcSpan, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	//
	CString strTopToFloor;
	CString strTopToRoof;
	CString strTopToCraneTop;	
	strTopToFloor.Format(_T("%d "), m_pParamCalculator->m_CalcDatalst.GD6);
	strTopToRoof.Format(_T("%d "), m_pParamCalculator->m_CalcDatalst.GD7);
	strTopToCraneTop.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.GD9);

	LOGFONT lfV;
	memcpy(&lfV, &m_lFont, sizeof(LOGFONT));
	lfV.lfEscapement = 900;
	lfV.lfOrientation = 900;
	HFONT hVFont = CreateFontIndirect(&lfV);
	SelectObject(dc.m_hDC, hVFont);
	DrawText(dc.m_hDC, strTopToFloor, strTopToFloor.GetLength(), &m_rcTopToFloor, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, strTopToRoof, strTopToRoof.GetLength(), &m_rcTopToRoof, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DrawText(dc.m_hDC, strTopToCraneTop, strTopToCraneTop.GetLength(), &m_rcTopToCraneTop, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	//
	SelectObject(dc.m_hDC, hOldFont);
	DeleteObject(hFont);
	DeleteObject(hVFont);

	// Draw control type image
	CRect rcCurCtrlType;
	CImage* pCurCtrlTypeImage = NULL;
	if (_T("R") == m_pParamCalculator->m_CalcDatalst.GB6)
	{
		rcCurCtrlType = m_rcCtrlTypes[eCraneCtrlType_Remote];
		pCurCtrlTypeImage = m_pCtrlTypeImages[eCraneCtrlType_Remote];
	}
	else if (_T("B") == m_pParamCalculator->m_CalcDatalst.GB6)
	{
		rcCurCtrlType = m_rcCtrlTypes[eCraneCtrlType_BlueTooth];
		pCurCtrlTypeImage = m_pCtrlTypeImages[eCraneCtrlType_BlueTooth];
	}
	else if (_T("C") == m_pParamCalculator->m_CalcDatalst.GB6)
	{
		rcCurCtrlType = m_rcCtrlTypes[eCraneCtrlType_Cabin];
		pCurCtrlTypeImage = m_pCtrlTypeImages[eCraneCtrlType_Cabin];
	}
	if (pCurCtrlTypeImage != NULL)
	{
		CRect rcImage(0, 0, pCurCtrlTypeImage->GetWidth(), pCurCtrlTypeImage->GetHeight());
		pCurCtrlTypeImage->StretchBlt(dc.m_hDC, rcCurCtrlType, rcImage);
	}		

	dc.SetBkMode(iOldBKMode);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnDestroy()
{
	CDialog::OnDestroy();

	for (int i = 0; i < eCraneType_Cnt; i++)
	{
		if (NULL != m_pHoistImages[i])
		{
			delete m_pHoistImages[i];
			m_pHoistImages[i] = NULL;
		}
	}
	for (int i = 0; i < eCraneCtrlType_Cnt; i++)
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
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	int iCurSel = (eCraneType)pComboBox->GetCurSel();
	switch (iCurSel)
	{
	case 0:
		m_pParamCalculator->m_CalcDatalst.GA1 = _T("BMD");
		m_pParamCalculator->m_CalcDatalst.GA11 = _T("BMD");
		m_pParamCalculator->m_CalcDatalst.GA12 = _T("D");
		m_pParamCalculator->m_CalcDatalst.GA13 = _T("D");
		break;
	case 1:
		m_pParamCalculator->m_CalcDatalst.GA1 = _T("BMZ");
		m_pParamCalculator->m_CalcDatalst.GA11 = _T("BMD");
		m_pParamCalculator->m_CalcDatalst.GA12 = _T("D");
		m_pParamCalculator->m_CalcDatalst.GA13 = _T("Z");
		break;
	case 2:
		m_pParamCalculator->m_CalcDatalst.GA1 = _T("BMS");
		m_pParamCalculator->m_CalcDatalst.GA11 = _T("BMS");
		m_pParamCalculator->m_CalcDatalst.GA12 = _T("S");
		m_pParamCalculator->m_CalcDatalst.GA13 = _T("S");
		break;
	case 3:
		m_pParamCalculator->m_CalcDatalst.GA1 = _T("BMU");
		m_pParamCalculator->m_CalcDatalst.GA11 = _T("BMS");
		m_pParamCalculator->m_CalcDatalst.GA12 = _T("S");
		m_pParamCalculator->m_CalcDatalst.GA13 = _T("U");
		break;
	}

	FillHoistTypeComboBox();
	//FillHoistLoadComboBox();	
	FillRailTypeComboBox();
	FillHoistSpeedComboBox();
	FillCraneCtrlTypeComboBox();	
	InvalidateRect(&m_rcHoistImage);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboHoisttype()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTTYPE);
	int iItemIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.GA2 = pComboBox->GetItemData(iItemIndex);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboCtrltype()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLTYPE);
	int iSelIndex = pComboBox->GetCurSel();
	switch(iSelIndex)
	{
	case 0:
		m_pParamCalculator->m_CalcDatalst.GB6 = _T("P");
		break;
	case 1:
		m_pParamCalculator->m_CalcDatalst.GB6 = _T("R");
		break;
	case 2:
		m_pParamCalculator->m_CalcDatalst.GB6 = _T("B");
		break;
	case 3:
		m_pParamCalculator->m_CalcDatalst.GB6 = _T("C");
		break;
	}
	InvalidateRect(&m_rcHoistImage);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnCbnSelchangeComboGaitype()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GAITYPE);
	pComboBox->GetWindowText(m_pParamCalculator->m_CalcDatalst.GA9);
	m_pParamCalculator->m_CalcDatalst.GD3 = m_pParamCalculator->m_CalcDatalst.GA9;
	InvalidateRect(&m_rcHoistImage);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnEnKillfocusEditCraneload()
{
	if (TRUE == m_bIsClosing)
		return;

	CString strTableName;
	if (_T("D") == m_pParamCalculator->m_CalcDatalst.GA12)
		strTableName = g_tcTableName[eTableID_HoiSel_D];
	else
		strTableName = g_tcTableName[eTableID_HoiSel_S];

	CString strTmp;
	GetDlgItemText(IDC_EDIT_CRANELOAD, strTmp);
	m_pParamCalculator->m_CalcDatalst.GA3 = _ttol(strTmp);
	m_pParamCalculator->m_CalcDatalst.GA31 = GetNearBigValue(strTableName, _T("HoiLod"), m_pParamCalculator->m_CalcDatalst.GA3);

	GetDlgItemText(IDC_EDIT_ROOFTOTOP, strTmp);
	m_pParamCalculator->m_CalcDatalst.GB3 = _ttol(strTmp);
	m_pParamCalculator->m_CalcDatalst.GD7 = m_pParamCalculator->m_CalcDatalst.GB3;

	if (_T("D") == m_pParamCalculator->m_CalcDatalst.GA12)
	{
		if (m_pParamCalculator->m_CalcDatalst.GA3 <= 5000)
		{
			m_pParamCalculator->m_CalcDatalst.GD5 = 80;
			m_pParamCalculator->m_CalcDatalst.GD8 = 300;
		}
		else if (m_pParamCalculator->m_CalcDatalst.GA3 <= 25000)
		{
			m_pParamCalculator->m_CalcDatalst.GD5 = 100;
			m_pParamCalculator->m_CalcDatalst.GD8 = 300;
		}
		else
		{
			m_pParamCalculator->m_CalcDatalst.GD5 = 100;
			m_pParamCalculator->m_CalcDatalst.GD8 = 400;
		}
	}
	else
	{
		m_pParamCalculator->m_CalcDatalst.GD5 = 100;
		m_pParamCalculator->m_CalcDatalst.GD8 = 200;
	}
	m_pParamCalculator->m_CalcDatalst.GD9 = max(0, m_pParamCalculator->m_CalcDatalst.GD7 - m_pParamCalculator->m_CalcDatalst.GD8);
	InvalidateRect(&m_rcHoistImage);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnEnKillfocusEditHeigth()
{
	if (TRUE == m_bIsClosing)
		return;

	CString strTmp;
	GetDlgItemText(IDC_EDIT_HEIGTH, strTmp);
	m_pParamCalculator->m_CalcDatalst.GA6 = _tstof(strTmp);
	m_pParamCalculator->m_CalcDatalst.GA61.Format(_T("%03d"), int(m_pParamCalculator->m_CalcDatalst.GA6*10));
	InvalidateRect(&m_rcHoistImage);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnEnKillfocusEditLength()
{
	if (TRUE == m_bIsClosing)
		return;

	CString strTmp;
	GetDlgItemText(IDC_EDIT_LENGTH, strTmp);
	m_pParamCalculator->m_CalcDatalst.GA5 = _tstof(strTmp);
	m_pParamCalculator->m_CalcDatalst.GA51.Format(_T("%3d"), int(m_pParamCalculator->m_CalcDatalst.GA5 * 10));
	m_pParamCalculator->m_CalcDatalst.GA52 = GetNearBigValue(g_tcTableName[eTableID_CraneData], _T("CraSpa"), m_pParamCalculator->m_CalcDatalst.GA5);
	InvalidateRect(&m_rcHoistImage);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnEnKillfocusEditToptofloor()
{
	if (TRUE == m_bIsClosing)
		return;

	CString strTmp;
	GetDlgItemText(IDC_EDIT_TOPTOFLOOR, strTmp);
	m_pParamCalculator->m_CalcDatalst.GB2 = _ttol(strTmp);
	m_pParamCalculator->m_CalcDatalst.GD6 = m_pParamCalculator->m_CalcDatalst.GB2;
	InvalidateRect(&m_rcHoistImage);	
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::OnEnKillfocusEditRooftotop()
{
	if (TRUE == m_bIsClosing)
		return;

	CString strTmp;
	GetDlgItemText(IDC_EDIT_ROOFTOTOP, strTmp);
	m_pParamCalculator->m_CalcDatalst.GB3 = _ttol(strTmp);
	m_pParamCalculator->m_CalcDatalst.GD7 = m_pParamCalculator->m_CalcDatalst.GB3;
	//
	m_pParamCalculator->m_CalcDatalst.GD9 = max(0, m_pParamCalculator->m_CalcDatalst.GD7 - m_pParamCalculator->m_CalcDatalst.GD8);
	InvalidateRect(&m_rcHoistImage);	
}
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
BOOL CBMOfferCalcBasicDataDlg::CanDoNextStep()
{
	CString strTmp;
	CComboBox* pComboBox = NULL;

	//Load of crane
	GetDlgItemText(IDC_EDIT_CRANELOAD, strTmp);
	m_pParamCalculator->m_CalcDatalst.GA3 = _ttol(strTmp);	
	if ((_T("D") == m_pParamCalculator->m_CalcDatalst.GA12) && (m_pParamCalculator->m_CalcDatalst.GA3 < 1000 || m_pParamCalculator->m_CalcDatalst.GA3 > 160000) ||
		(_T("S") == m_pParamCalculator->m_CalcDatalst.GA12) && (m_pParamCalculator->m_CalcDatalst.GA3 > 20000))
	{
		myMessageBox(IDS_CALC_LOADISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_CRANELOAD)->SetFocus();
		return FALSE;
	}

	//Load of hoist
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLOAD);
	pComboBox->GetWindowText(strTmp);
	m_pParamCalculator->m_CalcDatalst.GA4 = _ttol(strTmp);

	//Span
	GetDlgItemText(IDC_EDIT_LENGTH, strTmp);
	m_pParamCalculator->m_CalcDatalst.GA5 = _tstof(strTmp);
	if (m_pParamCalculator->m_CalcDatalst.GA5 <= 0)
	{
		myMessageBox(IDS_CALC_SPANISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_LENGTH)->SetFocus();
		return FALSE;
	}

	//HOL
	GetDlgItemText(IDC_EDIT_HEIGTH, strTmp);
	m_pParamCalculator->m_CalcDatalst.GA6 = _tstof(strTmp);
	if (m_pParamCalculator->m_CalcDatalst.GA6 <= 0)
	{
		myMessageBox(IDS_CALC_HOLISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_HEIGTH)->SetFocus();
		return FALSE;
	}

	//Crane Duty
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LEVEL);
	pComboBox->GetWindowText(m_pParamCalculator->m_CalcDatalst.GA7);
	
	//Hoist Duty
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLEVEL);
	int iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.GA8.Format(_T("M%d"), pComboBox->GetItemData(iSelIndex));
	pComboBox->GetWindowText(m_pParamCalculator->m_CalcDatalst.GA81);	

	//Rail Type
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GAITYPE);
	pComboBox->GetWindowText(m_pParamCalculator->m_CalcDatalst.GA9);

	//Rail length
	GetDlgItemText(IDC_EDIT_GAILENGTH, strTmp);
	m_pParamCalculator->m_CalcDatalst.GB1 = _tstof(strTmp);
	if (m_pParamCalculator->m_CalcDatalst.GB1 <= 0)
	{
		myMessageBox(IDS_CALC_RIALLENGHTISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_GAILENGTH)->SetFocus();
		return FALSE;
	}
	m_pParamCalculator->m_CalcDatalst.GB11.Format(_T("%03d"), int(m_pParamCalculator->m_CalcDatalst.GB1+0.5f));

	//Power supply
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_POWER);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.GB4 = pComboBox->GetItemData(iSelIndex);

	//Control voltge
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLVOL);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.GB5 = pComboBox->GetItemData(iSelIndex);

	//Main use
	GetDlgItemText(IDC_COMBO_WORKINGMODE, m_pParamCalculator->m_CalcDatalst.GB7);

	//Hoisting speed
	GetDlgItemText(IDC_COMBO_SPEED, m_pParamCalculator->m_CalcDatalst.GC1);

	//Hoist control type 
	GetDlgItemText(IDC_COMBO_RAISE_CTLTYPE, m_pParamCalculator->m_CalcDatalst.GC4);

	//Trolley speed
	GetDlgItemText(IDC_COMBO_MOVESPEED, m_pParamCalculator->m_CalcDatalst.GC2);

	//Trolley control type
	GetDlgItemText(IDC_COMBO_MOVE_CTLTYPE, m_pParamCalculator->m_CalcDatalst.GC5);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVE_CTLTYPE);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.GC51 = (TCHAR)pComboBox->GetItemData(iSelIndex);

	//Bridge speed
	GetDlgItemText(IDC_COMBO_MAINCARSPEED, m_pParamCalculator->m_CalcDatalst.GC3);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCARSPEED);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.GC31 = pComboBox->GetItemData(iSelIndex);

	//Bridge control type
	GetDlgItemText(IDC_COMBO_MAINCAR_CTLTYPE, m_pParamCalculator->m_CalcDatalst.GC6);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCAR_CTLTYPE);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.GC61 = (TCHAR)pComboBox->GetItemData(iSelIndex);

	m_pParamCalculator->UpdateBasicDrawingData();

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
	iItemIndex = pComboBox->AddString(_T("Double girder cranes (M/A)"));
	pComboBox->SetItemData(iItemIndex, eCraneType_GTD);
	iItemIndex = pComboBox->AddString(_T("Single girder cranes"));
	pComboBox->SetItemData(iItemIndex, eCraneType_GTS);
	iItemIndex = pComboBox->AddString(_T("Under runningcranes"));
	pComboBox->SetItemData(iItemIndex, eCraneType_GTU);
	pComboBox->SetCurSel(0);
	//
	FillHoistTypeComboBox();
	//
	FillHoistLoadComboBox();
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
	pComboBox->SetItemData(iItemIndex, int(_T('P')));
	iItemIndex = pComboBox->AddString(_T("Inverter"));
	pComboBox->SetItemData(iItemIndex, int(_T('T')));
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVE_CTLTYPE);
	iItemIndex = pComboBox->AddString(_T("2-SPEED"));
	pComboBox->SetItemData(iItemIndex, int(_T('P')));
	iItemIndex = pComboBox->AddString(_T("Inverter"));
	pComboBox->SetItemData(iItemIndex, int(_T('T')));
	pComboBox->SetCurSel(1);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCAR_CTLTYPE);
	iItemIndex = pComboBox->AddString(_T("2-SPEED"));
	pComboBox->SetItemData(iItemIndex, int(_T('P')));
	iItemIndex = pComboBox->AddString(_T("Inverter"));
	pComboBox->SetItemData(iItemIndex, int(_T('T')));
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
	pComboBox->SetItemData(iItemIndex, 1);
	if (FALSE == m_pParamCalculator->m_bOnlyHoist)
	{
		if (_T("D") == m_pParamCalculator->m_CalcDatalst.GA13 ||
			_T("S") == m_pParamCalculator->m_CalcDatalst.GA13)
		{
			iItemIndex = pComboBox->AddString(_T("Two hoist"));
			pComboBox->SetItemData(iItemIndex, 2);
		}
	}
	pComboBox->SetCurSel(0);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::FillHoistLoadComboBox()
{
	CString strTableName = g_tcTableName[eTableID_HoiSel_D];
	/*if (_T("BMD") == m_pParamCalculator->m_CalcDatalst.GA11)
		strTableName = g_tcTableName[eTableID_HoiSel_D];
	else
		strTableName = g_tcTableName[eTableID_HoiSel_S];*/
	
	list<CString> lstHoistLoads;
	GetValueListInTable(strTableName, _T("HoiLod"), eTableDataType_DWORD, lstHoistLoads);
	if (lstHoistLoads.size() == 0)
		return;

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLOAD);
	pComboBox->ResetContent();
	for (list<CString>::iterator it = lstHoistLoads.begin(); it != lstHoistLoads.end(); it++)
	{
		CString strHoistLoad = *it;
		int iItemIndex = pComboBox->AddString(strHoistLoad);
		pComboBox->SetItemData(iItemIndex, _ttol(strHoistLoad));
	}
	SelectComboBoxItem(pComboBox, 10000);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::FillRailTypeComboBox()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GAITYPE);
	pComboBox->ResetContent();
	if (m_pParamCalculator->m_CalcDatalst.GA1 == _T("BMU"))
	{
		pComboBox->AddString(_T("I20"));
		pComboBox->AddString(_T("I22"));
		pComboBox->AddString(_T("I25"));
		pComboBox->AddString(_T("I32"));
	}
	else
	{
		pComboBox->AddString(_T("P22"));
		pComboBox->AddString(_T("P24"));
		pComboBox->AddString(_T("P30"));
		pComboBox->AddString(_T("P38"));
		pComboBox->AddString(_T("P43"));
		pComboBox->AddString(_T("QU70"));
		pComboBox->AddString(_T("QU80"));
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
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::FillHoistSpeedComboBox()
{
	eTableID tbID;
	if (_T("BMD") == m_pParamCalculator->m_CalcDatalst.GA11)
	{
		tbID = eTableID_HoiSel_D;
	}
	else
	{
		tbID = eTableID_HoiSel_S;
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
	SelectComboBoxItem(pComboBox, eCraneType_GTD);
	OnCbnSelchangeComboType();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTTYPE);
	SelectComboBoxItem(pComboBox, eHoistNumType_One);
	//
	strTemp.Format(_T("%d"), 10000);
	SetDlgItemText(IDC_EDIT_CRANELOAD, strTemp);
	OnEnKillfocusEditCraneload();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLOAD);
	SelectComboBoxItem(pComboBox, 10000);
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
	OnCbnSelchangeComboGaitype();
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
	OnCbnSelchangeComboCtrltype();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WORKINGMODE);
	SelectComboBoxItem(pComboBox, eCraneMainUseType_Indoors);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAISE_CTLTYPE);
	SelectComboBoxItem(pComboBox, _T("2-SPEED"));
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVESPEED);
	SelectComboBoxItem(pComboBox, _T("20/5"));
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVE_CTLTYPE);
	SelectComboBoxItem(pComboBox, _T("Inverter"));
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCARSPEED);
	SelectComboBoxItem(pComboBox, _T("32/8"));
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCAR_CTLTYPE);
	SelectComboBoxItem(pComboBox, _T("Inverter"));
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::InitUIWithDatabaseData()
{
	/* Initliaze data by database values*/
	CString strTemp;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	if (_T("BMD") == m_pParamCalculator->m_CalcDatalst.GA1)
		pComboBox->SetCurSel(0);
	else if(_T("BMZ") == m_pParamCalculator->m_CalcDatalst.GA1)
		pComboBox->SetCurSel(1);
	else if(_T("BMS") == m_pParamCalculator->m_CalcDatalst.GA1)
		pComboBox->SetCurSel(2);
	else		
		pComboBox->SetCurSel(3);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GA2);
	//
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.GA3);
	SetDlgItemText(IDC_EDIT_CRANELOAD, strTemp);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLOAD);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GA4);
	//
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.GA5);
	SetDlgItemText(IDC_EDIT_LENGTH, strTemp);
	//
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.GA6);
	SetDlgItemText(IDC_EDIT_HEIGTH, strTemp);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LEVEL);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GA7);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HOISTLEVEL);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GA81);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GAITYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GA9);
	//
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.GB1);
	SetDlgItemText(IDC_EDIT_GAILENGTH, strTemp);
	//
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.GB2);
	SetDlgItemText(IDC_EDIT_TOPTOFLOOR, strTemp);
	//
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.GB3);
	m_pParamCalculator->m_CalcDatalst.GD7 = m_pParamCalculator->m_CalcDatalst.GB3;
	SetDlgItemText(IDC_EDIT_ROOFTOTOP, strTemp);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_POWER);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GB4);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLVOL);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GB5);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLTYPE);
	if (_T("P") == m_pParamCalculator->m_CalcDatalst.GB6)
		SelectComboBoxItem(pComboBox, eCraneCtrlType_Pendent);
	else if (_T("R") == m_pParamCalculator->m_CalcDatalst.GB6)
		SelectComboBoxItem(pComboBox, eCraneCtrlType_Remote);
	else if (_T("B") == m_pParamCalculator->m_CalcDatalst.GB6)
		SelectComboBoxItem(pComboBox, eCraneCtrlType_BlueTooth);
	else
		SelectComboBoxItem(pComboBox, eCraneCtrlType_Cabin);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WORKINGMODE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GB7);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SPEED);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GC1);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAISE_CTLTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GC4);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVESPEED);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GC2);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOVE_CTLTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GC5);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCARSPEED);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GC3);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAINCAR_CTLTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.GC6);
}
//-----------------------------------------------------------------------
void CBMOfferCalcBasicDataDlg::UpdateControlsStatus(eCraneType eCraneType)
{
	if (eCraneType_GTZ == eCraneType)
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