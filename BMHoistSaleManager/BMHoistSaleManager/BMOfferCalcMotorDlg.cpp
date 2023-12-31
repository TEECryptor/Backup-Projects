//-----------------------------------------------------------------------
//	File Name:BMOfferCalcMotorDlg.cpp
//	Introduce:CBMOfferCalcMotorDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcMotorDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcMotorDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferCalcMotorDlg, CDialog)
CBMOfferCalcMotorDlg::CBMOfferCalcMotorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferCalcMotorDlg::IDD, pParent)
{
	m_pParamCalculator = NULL;
	m_pGridCtrl = NULL;
	m_pParamTabDlg = NULL;
}
//-----------------------------------------------------------------------
CBMOfferCalcMotorDlg::~CBMOfferCalcMotorDlg()
{
}
//-----------------------------------------------------------------------
void CBMOfferCalcMotorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferCalcMotorDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_MOTORCALC_RESET, &CBMOfferCalcMotorDlg::OnBnClickedBtnMotorcalcReset)
	ON_BN_CLICKED(IDC_BTN_MOTORCALC_CALC, &CBMOfferCalcMotorDlg::OnBnClickedBtnMotorcalcCalc)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PARAMS, &CBMOfferCalcMotorDlg::OnTcnSelchangeTabParams)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcMotorDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferCalcMotorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
/*
	m_pGridCtrl = new CGridCtrl();
	m_pGridCtrl->Create(CRect(228, 35, 620, 150), this, 100);
	m_pGridCtrl->SetEditable(FALSE);
	m_pGridCtrl->EnableDragAndDrop(FALSE); 	
	m_pGridCtrl->SetRowCount(7);
	m_pGridCtrl->SetColumnCount(9);
	m_pGridCtrl->SetFixedRowCount(2);
	m_pGridCtrl->SetFixedColumnCount(2);
	//
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_WORDBREAK;
	Item.row = 0;
	Item.col = 0;
	Item.szText.Format(_T("%s"), _T("车轮材料"));
	m_pGridCtrl->SetItem(&Item);
	m_pGridCtrl->SetFixedCellCombine(0, 0, 1, 0);
	m_pGridCtrl->SetFixedCellCombine(0, 1, 1, 0);
	m_pGridCtrl->SetFixedCellCombine(0, 2, 0, 6);
	m_pGridCtrl->SetColumnWidth(0, 50);
	m_pGridCtrl->SetColumnWidth(1, 50);
	m_pGridCtrl->SetColumnWidth(2, 50);
*/
	//
	CTabCtrl* pParamsTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_PARAMS);
	pParamsTabCtrl->InsertItem(0, _T("摩擦系数"));
	pParamsTabCtrl->InsertItem(1, _T("轴承摩擦系数"));
	pParamsTabCtrl->InsertItem(2, _T("附加阻力系数"));
	pParamsTabCtrl->InsertItem(3, _T("摩擦阻力系数"));
	pParamsTabCtrl->UpdateWindow();
	pParamsTabCtrl->SetParent(this);
	pParamsTabCtrl->SetCurSel(0);
	//	
	CRect rcTabCtrl;
	pParamsTabCtrl->GetClientRect(&rcTabCtrl);
	rcTabCtrl.left += 2;
	rcTabCtrl.right -= 3;
	rcTabCtrl.top += 22;
	rcTabCtrl.bottom -= 3;
	m_pParamTabDlg = new CBMOfferCalcMotorParamTabDlg(pParamsTabCtrl);
	m_pParamTabDlg->Create(IDD_DIALOG_MOTOR_PARAMTAB, pParamsTabCtrl);
	m_pParamTabDlg->MoveWindow(&rcTabCtrl);
	m_pParamTabDlg->ShowWindow(SW_SHOW);
	m_pParamTabDlg->UpdateWindow();
	m_pParamTabDlg->SetCurParamTableIndex(0);
	
	InitDataWithDefaultSetting();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferCalcMotorDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (NULL != m_pGridCtrl)
	{
		delete m_pGridCtrl;
		m_pGridCtrl = NULL;
	}
	if (NULL != m_pParamTabDlg)
	{
		delete m_pParamTabDlg;
		m_pParamTabDlg = NULL;
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcMotorDlg::OnBnClickedBtnMotorcalcReset()
{
	InitDataWithDefaultSetting();
}
//-----------------------------------------------------------------------
void CBMOfferCalcMotorDlg::OnBnClickedBtnMotorcalcCalc()
{
	/*Get new data from UI at first*/
	CString strTemp;
	GetDlgItemText(IDC_EDIT_CALCMOTOR_LOADWGHT, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO01 = _tstof(strTemp);
	//	
	GetDlgItemText(IDC_EDIT_CALCMOTOR_SELWGHT, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO02 = _tstof(strTemp);
	//	
	GetDlgItemText(IDC_EDIT_CALCMOTOR_SPEED, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO03 = _tstof(strTemp);
	//	
	GetDlgItemText(IDC_EDIT_CALCMOTOR_GUNDONGMUCHAPARAM, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO04 = _tstof(strTemp);
	//	
	GetDlgItemText(IDC_EDIT_CALCMOTOR_WHEMUCHAPARAM, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO05 = _tstof(strTemp);
	//	
	GetDlgItemText(IDC_EDIT_CALCMOTOR_WHEZHOUDIR, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO06 = _tstof(strTemp);
	//	
	GetDlgItemText(IDC_EDIT_CALCMOTOR_WHETAMIANDIR, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO07 = _tstof(strTemp);
	//	
	GetDlgItemText(IDC_EDIT_CALCMOTOR_ADDMUCHAPARAM, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO08 = _tstof(strTemp);
	//	
	GetDlgItemText(IDC_EDIT_CALCMOTOR_ZULIMUCHAPARAM, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO09 = _tstof(strTemp);
	//	
	GetDlgItemText(IDC_EDIT_CALCMOTOR_PODU, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO11 = _tstof(strTemp);
	//	
	GetDlgItemText(IDC_EDIT_CALCMOTOR_CHUANDONGXIAOLV, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO13 = _tstof(strTemp);
	//	
	GetDlgItemText(IDC_EDIT_CALCMOTOR_MOTORCNT, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO14 = _tstof(strTemp);
	//	
	GetDlgItemText(IDC_EDIT_CALCMOTOR_GONGLVZENGDAPARAM, strTemp);
	m_pParamCalculator->m_CalcDatalst.MO16 = _tstof(strTemp);

	// Calculating
	m_pParamCalculator->m_CalcDatalst.MO10 = GetMO10();
	m_pParamCalculator->m_CalcDatalst.MO12 = GetMO12();
	m_pParamCalculator->m_CalcDatalst.MO15 = GetMO15();
	m_pParamCalculator->m_CalcDatalst.MO17 = GetMO17();
	m_pParamCalculator->m_CalcDatalst.MO18 = GetMO18();

	// Update new result to UI
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO10);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_MAXMUCHAPARAM, strTemp);
	//
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO12);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_PODUZULI, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO15);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_MOTORJINGXIAOLV, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO17);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_DEFAULTGONGLV, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO18);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_ZHUANSU, strTemp);
}
//-----------------------------------------------------------------------
void CBMOfferCalcMotorDlg::OnTcnSelchangeTabParams(NMHDR *pNMHDR, LRESULT *pResult)
{
	CTabCtrl* pParamsTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_PARAMS);
	int iCurSel = pParamsTabCtrl->GetCurSel();
	if (NULL != m_pParamTabDlg)
	{
		m_pParamTabDlg->SetCurParamTableIndex(iCurSel);
	}

	*pResult = 0;
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void CBMOfferCalcMotorDlg::InitDataWithDefaultSetting()
{
	m_pParamCalculator->m_CalcDatalst.MO01 = m_pParamCalculator->m_CalcDatalst.GA3;
	m_pParamCalculator->m_CalcDatalst.MO02 = m_pParamCalculator->m_CalcDatalst.DC12;
	m_pParamCalculator->m_CalcDatalst.MO03 = m_pParamCalculator->m_CalcDatalst.GC31;
	m_pParamCalculator->m_CalcDatalst.MO04 = m_pParamCalculator->m_CalcDatalst.DC52;
	m_pParamCalculator->m_CalcDatalst.MO05 = m_pParamCalculator->m_CalcDatalst.DC59;
	m_pParamCalculator->m_CalcDatalst.MO06 = m_pParamCalculator->m_CalcDatalst.DC53;
	m_pParamCalculator->m_CalcDatalst.MO07 = m_pParamCalculator->m_CalcDatalst.DB34;
	m_pParamCalculator->m_CalcDatalst.MO08 = 1.5f;
	m_pParamCalculator->m_CalcDatalst.MO09 = m_pParamCalculator->m_CalcDatalst.DC54;
	m_pParamCalculator->m_CalcDatalst.MO10 = GetMO10();
	m_pParamCalculator->m_CalcDatalst.MO11 = 0.001f;
	m_pParamCalculator->m_CalcDatalst.MO12 = GetMO12();
	m_pParamCalculator->m_CalcDatalst.M13 = 0.95f;
	m_pParamCalculator->m_CalcDatalst.MO14 = m_pParamCalculator->m_CalcDatalst.DB4;
	m_pParamCalculator->m_CalcDatalst.MO15 = GetMO15();
	m_pParamCalculator->m_CalcDatalst.MO16 = 1.3f;
	m_pParamCalculator->m_CalcDatalst.MO17 = GetMO17();
	m_pParamCalculator->m_CalcDatalst.MO18 = GetMO18();
	m_pParamCalculator->m_CalcDatalst.MO23 = m_pParamCalculator->m_CalcDatalst.DB35;
	m_pParamCalculator->m_CalcDatalst.MO24 = m_pParamCalculator->m_CalcDatalst.DB4;
	m_pParamCalculator->m_CalcDatalst.MO25 = m_pParamCalculator->m_CalcDatalst.DB310;
	m_pParamCalculator->m_CalcDatalst.MO26 = m_pParamCalculator->m_CalcDatalst.DB39;
	//
	CString strTemp;
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.MO01);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_LOADWGHT, strTemp);
	//	
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.MO02);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_SELWGHT, strTemp);
	//	
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.MO03);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_SPEED, strTemp);
	//	
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.MO04);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_GUNDONGMUCHAPARAM, strTemp);
	//	
	strTemp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.MO05);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_WHEMUCHAPARAM, strTemp);
	//	
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.MO06);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_WHEZHOUDIR, strTemp);
	//	
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.MO07);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_WHETAMIANDIR, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO08);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_ADDMUCHAPARAM, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO09);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_ZULIMUCHAPARAM, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO10);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_MAXMUCHAPARAM, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO11);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_PODU, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO12);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_PODUZULI, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO13);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_CHUANDONGXIAOLV, strTemp);
	//	
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.MO14);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_MOTORCNT, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO15);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_MOTORJINGXIAOLV, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO16);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_GONGLVZENGDAPARAM, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO17);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_DEFAULTGONGLV, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO18);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_ZHUANSU, strTemp);
	//	
	strTemp.Format(_T("%0.3f"), m_pParamCalculator->m_CalcDatalst.MO23);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_POWER, strTemp);
	//	
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.MO24);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_CNT, strTemp);
	//	
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.MO25);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_ZHUANSU2, strTemp);
	//	
	strTemp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.MO26);
	SetDlgItemText(IDC_EDIT_CALCMOTOR_QUDONGBI, strTemp);
}
//-----------------------------------------------------------------------
float CBMOfferCalcMotorDlg::GetMO10()
{
	float fMO10 = 0.0f;

	fMO10 = ((m_pParamCalculator->m_CalcDatalst.MO01 + m_pParamCalculator->m_CalcDatalst.MO02) * 9.81 * (2*m_pParamCalculator->m_CalcDatalst.MO04+m_pParamCalculator->m_CalcDatalst.MO05*m_pParamCalculator->m_CalcDatalst.MO06)*m_pParamCalculator->m_CalcDatalst.MO08) / m_pParamCalculator->m_CalcDatalst.MO07;

	return fMO10;
}
//-----------------------------------------------------------------------
float CBMOfferCalcMotorDlg::GetMO12()
{
	float fMO12 = 0.0f;

	fMO12 = (m_pParamCalculator->m_CalcDatalst.MO01 + m_pParamCalculator->m_CalcDatalst.MO02) * 9.81 * m_pParamCalculator->m_CalcDatalst.MO11;

	return fMO12;
}
//-----------------------------------------------------------------------
float CBMOfferCalcMotorDlg::GetMO15()
{
	float fMO15 = 0.0f;

	fMO15 = ((m_pParamCalculator->m_CalcDatalst.MO10+m_pParamCalculator->m_CalcDatalst.MO12) * (m_pParamCalculator->m_CalcDatalst.MO03/60.0f))/(1000*m_pParamCalculator->m_CalcDatalst.MO13*m_pParamCalculator->m_CalcDatalst.MO14);

	return fMO15;
}
//-----------------------------------------------------------------------
float CBMOfferCalcMotorDlg::GetMO17()
{
	float fMO17 = 0.0f;

	fMO17 = m_pParamCalculator->m_CalcDatalst.MO15 * m_pParamCalculator->m_CalcDatalst.MO16;

	return fMO17;
}
//-----------------------------------------------------------------------
float CBMOfferCalcMotorDlg::GetMO18()
{
	float fMO18 = 0.0f;

	fMO18 = (m_pParamCalculator->m_CalcDatalst.MO03*1000) / (3.14*m_pParamCalculator->m_CalcDatalst.MO07);

	return fMO18;
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferCalcMotorDlg::SetParamCalculator(CBMParamCalculator* pCalculator)
{
	m_pParamCalculator = pCalculator;
}
//-----------------------------------------------------------------------
