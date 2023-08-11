//-----------------------------------------------------------------------
//	File Name:BMOfferCalcRailDlg.cpp
//	Introduce:CBMOfferCalcRailDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcRailDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcRailDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferCalcRailDlg, CDialog)
CBMOfferCalcRailDlg::CBMOfferCalcRailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferCalcRailDlg::IDD, pParent)
{
	m_pParamCalculator = NULL;
	m_bMaxPressOK = FALSE;
	m_bAvgPressOK = FALSE;
	m_pPassImage = NULL;
	m_pNotPassImage = NULL;
}
//-----------------------------------------------------------------------
CBMOfferCalcRailDlg::~CBMOfferCalcRailDlg()
{
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CALCRAIL_STDMAXWHEPRESS, m_staticMaxPressResult);
	DDX_Control(pDX, IDC_STATIC_CALCRAIL_DENGXIAOWHEPRESS, m_staticAvgPressResult);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferCalcRailDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_RAILCALC_RESET, &CBMOfferCalcRailDlg::OnBnClickedBtnRailcalcReset)
	ON_BN_CLICKED(IDC_BTN_RAILCALC_CALC, &CBMOfferCalcRailDlg::OnBnClickedBtnRailcalcCalc)
	ON_CBN_SELCHANGE(IDC_COMBO_CALCRAIL_SPEED, &CBMOfferCalcRailDlg::OnCbnSelchangeComboCalcrailSpeed)
	ON_CBN_SELCHANGE(IDC_COMBO_CALCRAIL_RAILTYPE, &CBMOfferCalcRailDlg::OnCbnSelchangeComboCalcrailRailtype)
	ON_CBN_SELCHANGE(IDC_COMBO_CALCRAIL_RAILDIR, &CBMOfferCalcRailDlg::OnCbnSelchangeComboCalcrailRaildir)
	ON_EN_KILLFOCUS(IDC_EDIT_CALCRAIL_MAXWHEPRESS, &CBMOfferCalcRailDlg::OnEnKillfocusEditCalcrailMaxwhepress)
	ON_EN_KILLFOCUS(IDC_EDIT_CALCRAIL_MINWHEPRESS, &CBMOfferCalcRailDlg::OnEnKillfocusEditCalcrailMinwhepress)
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CBMOfferCalcRailDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferCalcRailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPassImage = new CImage();
	m_pNotPassImage = new CImage();
	m_pPassImage->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_CALCPASS);
	m_pNotPassImage->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_CALCNOTPASS);

	const int iImgX = 24;
	const int iImgY = 24;
	m_rcMaxPress = CRect(417, 185, 417 + iImgX, 185 + iImgY);
	m_rcAvgPress = CRect(417, 214, 417 + iImgX, 214 + iImgY);

	FillRailTypeCombobox();
	FillRailDirCombobox();
	FillRailSpeedCombobox();
	//
	InitDataWithDefaultSetting();
	DoRailCalculation();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pPassImage != NULL)
	{
		delete m_pPassImage;
		m_pPassImage = NULL;
	}
	if (m_pNotPassImage != NULL)
	{
		delete m_pNotPassImage;
		m_pNotPassImage = NULL;
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
		
	if (m_bMaxPressOK) m_pPassImage->StretchBlt(dc.m_hDC, m_rcMaxPress.left, m_rcMaxPress.top, m_rcMaxPress.Width(), m_rcMaxPress.Height());
	else m_pNotPassImage->StretchBlt(dc.m_hDC, m_rcMaxPress.left, m_rcMaxPress.top, m_rcMaxPress.Width(), m_rcMaxPress.Height());
	if (m_bAvgPressOK) m_pPassImage->StretchBlt(dc.m_hDC, m_rcAvgPress.left, m_rcAvgPress.top, m_rcAvgPress.Width(), m_rcAvgPress.Height());
	else m_pNotPassImage->StretchBlt(dc.m_hDC, m_rcAvgPress.left, m_rcAvgPress.top, m_rcAvgPress.Width(), m_rcAvgPress.Height());
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::OnCbnSelchangeComboCalcrailRailtype()
{
	GetDlgItemText(IDC_COMBO_CALCRAIL_RAILTYPE, m_pParamCalculator->m_CalcDatalst.RA01);
	m_pParamCalculator->UpdateRailDataR08R09();
	//
	CString strTemp;
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.RA08);
	SetDlgItemText(IDC_EDIT_CALCRAIL_RAILWIDTH, strTemp);
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.RA09);
	SetDlgItemText(IDC_EDIT_CALCRAIL_RAILPRESSPARAM, strTemp);
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::OnCbnSelchangeComboCalcrailRaildir()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CALCRAIL_RAILDIR);
	int iIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_CalcDatalst.RA02 = pComboBox->GetItemData(iIndex);
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::OnCbnSelchangeComboCalcrailSpeed()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CALCRAIL_SPEED);
	int iIndex = pComboBox->GetCurSel();
	GetDlgItemText(IDC_COMBO_CALCRAIL_SPEED, m_pParamCalculator->m_CalcDatalst.RA03);
	m_pParamCalculator->m_CalcDatalst.RA031 = pComboBox->GetItemData(iIndex);
	m_pParamCalculator->UpdateRailDataR04R05();
	//
	CString strTemp;
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.RA04);
	SetDlgItemText(IDC_EDIT_CALCRAIL_PILAOQIANGDU, strTemp);
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.RA05);
	SetDlgItemText(IDC_EDIT_CALCRAIL_JINGQIANGDU, strTemp);
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::OnEnKillfocusEditCalcrailMaxwhepress()
{
	CString strTemp;
	GetDlgItemText(IDC_EDIT_CALCRAIL_MAXWHEPRESS, strTemp);
	m_pParamCalculator->m_CalcDatalst.RA06 = _tstof(strTemp);
	m_pParamCalculator->m_CalcDatalst.RA12 = m_pParamCalculator->m_CalcDatalst.RA06;
	SetDlgItemText(IDC_EDIT_CALCRAIL_MAXWHEPRESS2, strTemp);
	//
	m_pParamCalculator->m_CalcDatalst.RA13 = GetRA13();
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.RA13);
	SetDlgItemText(IDC_EDIT_CALCRAIL_MINDENGXIAOWHEPRESS, strTemp);
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::OnEnKillfocusEditCalcrailMinwhepress()
{
	CString strTemp;
	GetDlgItemText(IDC_EDIT_CALCRAIL_MINWHEPRESS, strTemp);
	m_pParamCalculator->m_CalcDatalst.RA07 = _tstof(strTemp);
	m_pParamCalculator->m_CalcDatalst.RA13 = GetRA13();
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.RA13);
	SetDlgItemText(IDC_EDIT_CALCRAIL_MINDENGXIAOWHEPRESS, strTemp);
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::OnBnClickedBtnRailcalcReset()
{
	InitDataWithDefaultSetting();
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::OnBnClickedBtnRailcalcCalc()
{
	// Calculating
	DoRailCalculation();
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::FillRailTypeCombobox()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CALCRAIL_RAILTYPE);
	pComboBox->ResetContent();

	list<CString> lstRailType;
	GetValueListInTable(g_tcTableName[eTableID_DatRai], _T("RaiTyp"), eTableDataType_String, lstRailType);
	if (lstRailType.size() == 0)
		return;

	for (list<CString>::iterator it = lstRailType.begin(); it != lstRailType.end(); it++)
	{
		pComboBox->AddString(*it);
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::FillRailDirCombobox()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CALCRAIL_RAILDIR);
	pComboBox->ResetContent();

	list<CString> lstRailWheCode;
	GetValueListInTable(g_tcTableName[eTableID_WheMot], _T("WheCode"), eTableDataType_String, lstRailWheCode);
	if (lstRailWheCode.size() == 0)
		return;

	for (list<CString>::iterator it = lstRailWheCode.begin(); it != lstRailWheCode.end(); it++)
	{
		CString strRailWheCode = *it;
		long lWheCode = _ttol(strRailWheCode);
		int iItemIndex = pComboBox->AddString(strRailWheCode);
		pComboBox->SetItemData(iItemIndex, lWheCode);
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::FillRailSpeedCombobox()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CALCRAIL_SPEED);
	pComboBox->ResetContent();
	int iIndex = pComboBox->AddString(_T("20/5"));
	pComboBox->SetItemData(iIndex, 20);
	iIndex = pComboBox->AddString(_T("32/8"));
	pComboBox->SetItemData(iIndex, 32);
	iIndex = pComboBox->AddString(_T("40/10"));
	pComboBox->SetItemData(iIndex, 40);
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::InitDataWithDefaultSetting()
{
	m_pParamCalculator->m_CalcDatalst.RA01 = m_pParamCalculator->m_CalcDatalst.DA4;
	m_pParamCalculator->m_CalcDatalst.RA02 = _ttol(m_pParamCalculator->m_CalcDatalst.DB21);
	m_pParamCalculator->m_CalcDatalst.RA03 = m_pParamCalculator->m_CalcDatalst.GC3;
	m_pParamCalculator->m_CalcDatalst.RA06 = m_pParamCalculator->m_CalcDatalst.DC11;
	m_pParamCalculator->m_CalcDatalst.RA07 = m_pParamCalculator->m_CalcDatalst.DC13;

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CALCRAIL_RAILTYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.RA01);
	OnCbnSelchangeComboCalcrailRailtype();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CALCRAIL_RAILDIR);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.RA02);
	OnCbnSelchangeComboCalcrailRaildir();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CALCRAIL_SPEED);	
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.RA03);
	OnCbnSelchangeComboCalcrailSpeed();

	CString strTemp;
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.RA06);
	SetDlgItemText(IDC_EDIT_CALCRAIL_MAXWHEPRESS, strTemp);
	OnEnKillfocusEditCalcrailMaxwhepress();
	//
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.RA07);
	SetDlgItemText(IDC_EDIT_CALCRAIL_MINWHEPRESS, strTemp);
	OnEnKillfocusEditCalcrailMinwhepress();
}
//-----------------------------------------------------------------------
float CBMOfferCalcRailDlg::GetRA10()
{
	float fRA10 = 0.0f;

	fRA10 = (m_pParamCalculator->m_CalcDatalst.RA04*m_pParamCalculator->m_CalcDatalst.RA08*m_pParamCalculator->m_CalcDatalst.RA09) / 1000;

	return fRA10;
}
//-----------------------------------------------------------------------
float CBMOfferCalcRailDlg::GetRA11()
{
	float fRA11 = 0.0f;

	fRA11 = (m_pParamCalculator->m_CalcDatalst.RA05*m_pParamCalculator->m_CalcDatalst.RA08*m_pParamCalculator->m_CalcDatalst.RA09) / 1000;

	return fRA11;
}
//-----------------------------------------------------------------------
float CBMOfferCalcRailDlg::GetRA13()
{
	float fRA13 = 0.0f;

	fRA13 = (m_pParamCalculator->m_CalcDatalst.RA06*2+m_pParamCalculator->m_CalcDatalst.RA07) / 3;

	return fRA13;
}
//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::DoRailCalculation()
{
	m_pParamCalculator->m_CalcDatalst.RA10 = GetRA10();
	m_pParamCalculator->m_CalcDatalst.RA11 = GetRA11();

	// Update new result to UI
	CString strTemp;
	if (m_pParamCalculator->m_CalcDatalst.RA10 >= m_pParamCalculator->m_CalcDatalst.RA12)
	{
		m_bMaxPressOK = TRUE;
		strTemp.Format(_T("%0.2f >="), m_pParamCalculator->m_CalcDatalst.RA10);
	}
	else
	{
		m_bMaxPressOK = FALSE;
		strTemp.Format(_T("%0.2f <"), m_pParamCalculator->m_CalcDatalst.RA10);
	}	
	m_staticMaxPressResult.SetColors(m_bMaxPressOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticMaxPressResult.SetText(strTemp);
	//
	if (m_pParamCalculator->m_CalcDatalst.RA11 >= m_pParamCalculator->m_CalcDatalst.RA13)
	{
		m_bAvgPressOK = TRUE;
		strTemp.Format(_T("%0.2f >="), m_pParamCalculator->m_CalcDatalst.RA11);
	}
	else
	{
		m_bAvgPressOK = FALSE;
		strTemp.Format(_T("%0.2f <"), m_pParamCalculator->m_CalcDatalst.RA11);
	}	
	m_staticAvgPressResult.SetColors(m_bAvgPressOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticAvgPressResult.SetText(strTemp);

	InvalidateRect(&m_rcMaxPress);
	InvalidateRect(&m_rcAvgPress);
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferCalcRailDlg::SetParamCalculator(CBMParamCalculator* pCalculator)
{
	m_pParamCalculator = pCalculator;
}
//-----------------------------------------------------------------------