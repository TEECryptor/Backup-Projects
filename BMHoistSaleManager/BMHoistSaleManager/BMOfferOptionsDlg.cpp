//-----------------------------------------------------------------------
//	File Name:BMOfferOptionsDlg.cpp
//	Introduce:Class CBMOfferOptionsDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferOptionsDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferOptionsDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferOptionsDlg, CDialog)

CBMOfferOptionsDlg::CBMOfferOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferOptionsDlg::IDD, pParent)
{
	m_iComCount = 0;
	m_iComToatlPrice = 0;
	//
	m_pCraneCalculator = NULL;
}
//-----------------------------------------------------------------------
CBMOfferOptionsDlg::~CBMOfferOptionsDlg()
{
	if (NULL != m_pCraneCalculator)
	{
		delete m_pCraneCalculator;
		m_pCraneCalculator = NULL;
	}
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferOptionsDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_HOIST_CNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgHoistCnt)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_ENDCAR_CNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgEndcarCnt)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_BRIDGEDRI_CNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgBridgedriCnt)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_BRIDGEPANNEL_CNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgBridgepannelCnt)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_FASTOON_CNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgFastoonCnt)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_OPERATEUNIT_CNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgOperateunitCnt)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_CRANEPOWER_CNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgCranepowerCnt)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_MAINGIRDER_CNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgMaingirderCnt)
	ON_BN_CLICKED(IDC_CHECK_HOIST, &CBMOfferOptionsDlg::OnBnClickedCheckHoist)
	ON_BN_CLICKED(IDC_CHECK_ENDCAR, &CBMOfferOptionsDlg::OnBnClickedCheckEndcar)
	ON_BN_CLICKED(IDC_CHECK_BRIDGEDRI, &CBMOfferOptionsDlg::OnBnClickedCheckBridgedri)
	ON_BN_CLICKED(IDC_CHECK_BRIDGEPANNEL, &CBMOfferOptionsDlg::OnBnClickedCheckBridgepannel)
	ON_BN_CLICKED(IDC_CHECK_FASTOON, &CBMOfferOptionsDlg::OnBnClickedCheckFastoon)
	ON_BN_CLICKED(IDC_CHECK_OPERATEUNIT, &CBMOfferOptionsDlg::OnBnClickedCheckOperateunit)
	ON_BN_CLICKED(IDC_CHECK_CRANEPOWER, &CBMOfferOptionsDlg::OnBnClickedCheckCranepower)
	ON_BN_CLICKED(IDC_CHECK_MAINGIRDER, &CBMOfferOptionsDlg::OnBnClickedCheckMaingirder)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_HOIST_DISCOUNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgHoistDiscount)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_ENDCAR_DISCOUNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgEndcarDiscount)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_BRIDGEDRI_DISCOUNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgBridgedriDiscount)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_BRIDGEPANNEL_DISCOUNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgBridgepannelDiscount)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_FASTOON_DISCOUNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgFastoonDiscount)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_OPERATEUNIT_DISCOUNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgOperateunitDiscount)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_CRANEPOWER_DISCOUNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgCranepowerDiscount)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_MAINGIRDER_DISCOUNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgMaingirderDiscount)
	ON_EN_CHANGE(IDC_EDIT_OFFERDLG_CNT, &CBMOfferOptionsDlg::OnEnChangeEditOfferdlgCnt)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CBMOfferOptionsDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferOptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Initliaze controls arrary
	m_DetailPricesCtrls[eComponentType_Hoist].SetDialog(m_hWnd);
	m_DetailPricesCtrls[eComponentType_Hoist].nCheckBox = IDC_CHECK_HOIST;
	m_DetailPricesCtrls[eComponentType_Hoist].nQtyEdit = IDC_EDIT_OFFERDLG_HOIST_CNT;
	m_DetailPricesCtrls[eComponentType_Hoist].nCodeEdit = IDC_EDIT_OFFERDLG_HOIST;
	m_DetailPricesCtrls[eComponentType_Hoist].nUnitPriceEdit = IDC_EDIT_OFFERDLG_HOIST_PRICE;
	m_DetailPricesCtrls[eComponentType_Hoist].nUnitUnitStatic = IDC_STATIC_HOIST_PRICEUNIT;
	m_DetailPricesCtrls[eComponentType_Hoist].nDiscountEdit = IDC_EDIT_OFFERDLG_HOIST_DISCOUNT;
	m_DetailPricesCtrls[eComponentType_Hoist].nTotalPriceEdit = IDC_EDIT_OFFERDLG_HOIST_PRICE2;
	m_DetailPricesCtrls[eComponentType_Hoist].nTotalUnitStatic = IDC_STATIC_HOIST_PRICEUNIT2;
	//
	m_DetailPricesCtrls[eComponentType_EndCar].SetDialog(m_hWnd);
	m_DetailPricesCtrls[eComponentType_EndCar].nCheckBox = IDC_CHECK_ENDCAR;
	m_DetailPricesCtrls[eComponentType_EndCar].nQtyEdit = IDC_EDIT_OFFERDLG_ENDCAR_CNT;
	m_DetailPricesCtrls[eComponentType_EndCar].nCodeEdit = IDC_EDIT_OFFERDLG_ENDCAR;
	m_DetailPricesCtrls[eComponentType_EndCar].nUnitPriceEdit = IDC_EDIT_OFFERDLG_ENDCAR_PRICE;
	m_DetailPricesCtrls[eComponentType_EndCar].nUnitUnitStatic = IDC_STATIC_ENDCAR_PRIUNIT;
	m_DetailPricesCtrls[eComponentType_EndCar].nDiscountEdit = IDC_EDIT_OFFERDLG_ENDCAR_DISCOUNT;
	m_DetailPricesCtrls[eComponentType_EndCar].nTotalPriceEdit = IDC_EDIT_OFFERDLG_ENDCAR_PRICE2;
	m_DetailPricesCtrls[eComponentType_EndCar].nTotalUnitStatic = IDC_STATIC_ENDCAR_PRIUNIT2;
	//
	m_DetailPricesCtrls[eComponentType_BridgeDri].SetDialog(m_hWnd);
	m_DetailPricesCtrls[eComponentType_BridgeDri].nCheckBox = IDC_CHECK_BRIDGEDRI;
	m_DetailPricesCtrls[eComponentType_BridgeDri].nQtyEdit = IDC_EDIT_OFFERDLG_BRIDGEDRI_CNT;
	m_DetailPricesCtrls[eComponentType_BridgeDri].nCodeEdit = IDC_EDIT_OFFERDLG_BRIDGEDRI;
	m_DetailPricesCtrls[eComponentType_BridgeDri].nUnitPriceEdit = IDC_EDIT_OFFERDLG_BRIDGEDRI_PRICE;
	m_DetailPricesCtrls[eComponentType_BridgeDri].nUnitUnitStatic = IDC_STATIC_BRIDGEDRI_PRIUNIT;
	m_DetailPricesCtrls[eComponentType_BridgeDri].nDiscountEdit = IDC_EDIT_OFFERDLG_BRIDGEDRI_DISCOUNT;
	m_DetailPricesCtrls[eComponentType_BridgeDri].nTotalPriceEdit = IDC_EDIT_OFFERDLG_BRIDGEDRI_PRICE2;
	m_DetailPricesCtrls[eComponentType_BridgeDri].nTotalUnitStatic = IDC_STATIC_BRIDGEDRI_PRIUNIT2;
	//
	m_DetailPricesCtrls[eComponentType_BridgePannel].SetDialog(m_hWnd);
	m_DetailPricesCtrls[eComponentType_BridgePannel].nCheckBox = IDC_CHECK_BRIDGEPANNEL;
	m_DetailPricesCtrls[eComponentType_BridgePannel].nQtyEdit = IDC_EDIT_OFFERDLG_BRIDGEPANNEL_CNT;
	m_DetailPricesCtrls[eComponentType_BridgePannel].nCodeEdit = IDC_EDIT_OFFERDLG_BRIDGEPANNEL;
	m_DetailPricesCtrls[eComponentType_BridgePannel].nUnitPriceEdit = IDC_EDIT_OFFERDLG_BRIDGEPANNEL_PRICE;
	m_DetailPricesCtrls[eComponentType_BridgePannel].nUnitUnitStatic = IDC_STATIC_BRIDGEPANNEL_PRIUNIT;
	m_DetailPricesCtrls[eComponentType_BridgePannel].nDiscountEdit = IDC_EDIT_OFFERDLG_BRIDGEPANNEL_DISCOUNT;
	m_DetailPricesCtrls[eComponentType_BridgePannel].nTotalPriceEdit = IDC_EDIT_OFFERDLG_BRIDGEPANNEL_PRICE2;
	m_DetailPricesCtrls[eComponentType_BridgePannel].nTotalUnitStatic = IDC_STATIC_BRIDGEPANNEL_PRIUNIT2;
	//
	m_DetailPricesCtrls[eComponentType_Fastoon].SetDialog(m_hWnd);
	m_DetailPricesCtrls[eComponentType_Fastoon].nCheckBox = IDC_CHECK_FASTOON;
	m_DetailPricesCtrls[eComponentType_Fastoon].nQtyEdit = IDC_EDIT_OFFERDLG_FASTOON_CNT;
	m_DetailPricesCtrls[eComponentType_Fastoon].nCodeEdit = IDC_EDIT_OFFERDLG_FASTOON;
	m_DetailPricesCtrls[eComponentType_Fastoon].nUnitPriceEdit = IDC_EDIT_OFFERDLG_FASTOON_PRICE;
	m_DetailPricesCtrls[eComponentType_Fastoon].nUnitUnitStatic = IDC_STATIC_FASTOON_PRIUNIT;
	m_DetailPricesCtrls[eComponentType_Fastoon].nDiscountEdit = IDC_EDIT_OFFERDLG_FASTOON_DISCOUNT;
	m_DetailPricesCtrls[eComponentType_Fastoon].nTotalPriceEdit = IDC_EDIT_OFFERDLG_FASTOON_PRICE2;
	m_DetailPricesCtrls[eComponentType_Fastoon].nTotalUnitStatic = IDC_STATIC_FASTOON_PRIUNIT2;
	//
	m_DetailPricesCtrls[eComponentType_OperateUnit].SetDialog(m_hWnd);
	m_DetailPricesCtrls[eComponentType_OperateUnit].nCheckBox = IDC_CHECK_OPERATEUNIT;
	m_DetailPricesCtrls[eComponentType_OperateUnit].nQtyEdit = IDC_EDIT_OFFERDLG_OPERATEUNIT_CNT;
	m_DetailPricesCtrls[eComponentType_OperateUnit].nCodeEdit = IDC_EDIT_OFFERDLG_OPERATEUNIT;
	m_DetailPricesCtrls[eComponentType_OperateUnit].nUnitPriceEdit = IDC_EDIT_OFFERDLG_OPERATEUNIT_PRICE;
	m_DetailPricesCtrls[eComponentType_OperateUnit].nUnitUnitStatic = IDC_STATIC_OPERATEUNIT_PRIUNIT;
	m_DetailPricesCtrls[eComponentType_OperateUnit].nDiscountEdit = IDC_EDIT_OFFERDLG_OPERATEUNIT_DISCOUNT;
	m_DetailPricesCtrls[eComponentType_OperateUnit].nTotalPriceEdit = IDC_EDIT_OFFERDLG_OPERATEUNIT_PRICE2;
	m_DetailPricesCtrls[eComponentType_OperateUnit].nTotalUnitStatic = IDC_STATIC_OPERATEUNIT_PRIUNIT2;
	//
	m_DetailPricesCtrls[eComponentType_CranePower].SetDialog(m_hWnd);
	m_DetailPricesCtrls[eComponentType_CranePower].nCheckBox = IDC_CHECK_CRANEPOWER;
	m_DetailPricesCtrls[eComponentType_CranePower].nQtyEdit = IDC_EDIT_OFFERDLG_CRANEPOWER_CNT;
	m_DetailPricesCtrls[eComponentType_CranePower].nCodeEdit = IDC_EDIT_OFFERDLG_CRANEPOWER;
	m_DetailPricesCtrls[eComponentType_CranePower].nUnitPriceEdit = IDC_EDIT_OFFERDLG_CRANEPOWER_PRICE;
	m_DetailPricesCtrls[eComponentType_CranePower].nUnitUnitStatic = IDC_STATIC_CRANEPOWER_PRIUNIT;
	m_DetailPricesCtrls[eComponentType_CranePower].nDiscountEdit = IDC_EDIT_OFFERDLG_CRANEPOWER_DISCOUNT;
	m_DetailPricesCtrls[eComponentType_CranePower].nTotalPriceEdit = IDC_EDIT_OFFERDLG_CRANEPOWER_PRICE2;
	m_DetailPricesCtrls[eComponentType_CranePower].nTotalUnitStatic = IDC_STATIC_CRANEPOWER_PRIUNIT2;
	//
	m_DetailPricesCtrls[eComponentType_MainGirder].SetDialog(m_hWnd);
	m_DetailPricesCtrls[eComponentType_MainGirder].nCheckBox = IDC_CHECK_MAINGIRDER;
	m_DetailPricesCtrls[eComponentType_MainGirder].nQtyEdit = IDC_EDIT_OFFERDLG_MAINGIRDER_CNT;
	m_DetailPricesCtrls[eComponentType_MainGirder].nCodeEdit = IDC_EDIT_OFFERDLG_MAINGIRDER;
	m_DetailPricesCtrls[eComponentType_MainGirder].nUnitPriceEdit = IDC_EDIT_OFFERDLG_MAINGIRDER_PRICE;
	m_DetailPricesCtrls[eComponentType_MainGirder].nUnitUnitStatic = IDC_STATIC_MAINGIRDER_PRIUNIT;
	m_DetailPricesCtrls[eComponentType_MainGirder].nDiscountEdit = IDC_EDIT_OFFERDLG_MAINGIRDER_DISCOUNT;
	m_DetailPricesCtrls[eComponentType_MainGirder].nTotalPriceEdit = IDC_EDIT_OFFERDLG_MAINGIRDER_PRICE2;
	m_DetailPricesCtrls[eComponentType_MainGirder].nTotalUnitStatic = IDC_STATIC_MAINGIRDER_PRIUNIT2;
	//
	m_DetailPricesCtrls[eComponentType_Hoist].SetInitData(m_pCraneCalculator->m_CalcDatalst.GA2, m_pCraneCalculator->m_CalcDatalst.HC1, 0, m_pCraneCalculator->m_CalcDatalst.T25);
	if (!m_pCraneCalculator->m_bOnlyHoist)
	{
		m_DetailPricesCtrls[eComponentType_EndCar].SetInitData(2, m_pCraneCalculator->m_CalcDatalst.DE41, 0, m_pCraneCalculator->m_CalcDatalst.T26, TRUE);
		m_DetailPricesCtrls[eComponentType_BridgeDri].SetInitData(m_pCraneCalculator->m_CalcDatalst.DB4, m_pCraneCalculator->m_CalcDatalst.DE5, 0, m_pCraneCalculator->m_CalcDatalst.T27);
		m_DetailPricesCtrls[eComponentType_BridgePannel].SetInitData(1, m_pCraneCalculator->m_CalcDatalst.EH1, 0, m_pCraneCalculator->m_CalcDatalst.T28);
		m_DetailPricesCtrls[eComponentType_Fastoon].SetInitData(1, m_pCraneCalculator->m_CalcDatalst.EH2, 0, m_pCraneCalculator->m_CalcDatalst.T29);
		m_DetailPricesCtrls[eComponentType_OperateUnit].SetInitData(1, m_pCraneCalculator->m_CalcDatalst.EH3, 0, m_pCraneCalculator->m_CalcDatalst.T30);
		m_DetailPricesCtrls[eComponentType_CranePower].SetInitData(1, m_pCraneCalculator->m_CalcDatalst.EH5, 0, m_pCraneCalculator->m_CalcDatalst.T32);
		m_DetailPricesCtrls[eComponentType_MainGirder].SetInitData(1, m_pCraneCalculator->m_CalcDatalst.MF1, 0, m_pCraneCalculator->m_CalcDatalst.T33);
	}

	for (int i = 0; i < eComponentType_Cnt; i++)
	{				
		if (m_pCraneCalculator->m_bOnlyHoist && i > 0)
		{
			GetDlgItem(m_DetailPricesCtrls[i].nCheckBox)->EnableWindow(FALSE);
			((CButton*)GetDlgItem(m_DetailPricesCtrls[i].nCheckBox))->SetCheck(BST_UNCHECKED);
		}
		else
		{
			((CButton*)GetDlgItem(m_DetailPricesCtrls[i].nCheckBox))->SetCheck(BST_CHECKED);
		}
		m_DetailPricesCtrls[i].UpdateCheck();
	}

	SetDlgItemText(IDC_EDIT_OFFERDLG_CNT, _T("1"));
	UpdateTotalPrice();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (FALSE == bShow)
		return;
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
BOOL CBMOfferOptionsDlg::SetOfferNoAndComName(CString strOfferNo, CString strComName)
{
	m_strOfferNo = strOfferNo;
	m_strComName = strComName;

	// Use calculator object to load offer data from database
	if (NULL == m_pCraneCalculator)
	{
		m_pCraneCalculator = new CBMParamCalculator();
	}
	m_pCraneCalculator->LoadDataFromDatabase(m_strOfferNo, m_strComName);
	if (!m_pCraneCalculator->m_bOnlyHoist)
	{
		m_pCraneCalculator->CalcMainGirder();
		m_pCraneCalculator->CalcEndCar();
	}
	m_pCraneCalculator->ConstructTechData();

	return TRUE;
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::SetCurrentCurrency(eCurrencyType eType, float fRate)
{
	for (int i = 0; i < eComponentType_Cnt; i++)
	{				
		m_DetailPricesCtrls[i].SetCurrency(eType, fRate);
	}
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::SetVATRate(float fVAT)
{
	for (int i = 0; i < eComponentType_Cnt; i++)
	{				
		m_DetailPricesCtrls[i].SetVATRate(fVAT);
	}
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::GetDetialPriceInfo(DetailPriceList &lstPrices)
{
	lstPrices.clear();
	for (int i = 0; i < eComponentType_Cnt; i++)
	{
		if (m_pCraneCalculator->m_bOnlyHoist && i > eComponentType_Hoist)
			break;

		CString strCode;
		CString strCount;
		GetDlgItemText(m_DetailPricesCtrls[i].nCodeEdit, strCode);
		GetDlgItemText(m_DetailPricesCtrls[i].nQtyEdit, strCount);

		CButton *pBtn = (CButton*)GetDlgItem(m_DetailPricesCtrls[i].nCheckBox);
		if (BST_CHECKED == pBtn->GetCheck())
		{
			lstPrices.push_back(DetailPriceInfo(eComponentType(i), strCode, _ttol(strCount), m_DetailPricesCtrls[i].iTotalOfferPrice));
		}		
		else
		{
			lstPrices.push_back(DetailPriceInfo(eComponentType(i), strCode, _ttol(strCount), 0));
		}
	}
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::UpdateTotalPrice()
{
	CString strComCount;
	CString strComTotalPrice;
	GetDlgItemText(IDC_EDIT_OFFERDLG_CNT, strComCount);
	m_iComCount = _ttol(strComCount);
	m_iComToatlPrice = 0;
	for (int i = 0; i < eComponentType_Cnt; i++)
	{
		CButton* pCheckBox = (CButton*)GetDlgItem(m_DetailPricesCtrls[i].nCheckBox);
		if (pCheckBox && pCheckBox->GetCheck() == BST_CHECKED)
		{
			m_iComToatlPrice += m_DetailPricesCtrls[i].iTotalOfferPrice;
		}
	}
	m_iComToatlPrice *= m_iComCount;
	strComTotalPrice.Format(_T("%d"), m_iComToatlPrice);
	SetDlgItemText(IDC_EDIT_OFFERDLG_TOTALPRICE, strComTotalPrice);

	::PostMessage(m_hMsgParent, WM_USER_UPDATETOTALPRICE, 0, 0);
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnBnClickedCheckHoist()
{
	m_DetailPricesCtrls[eComponentType_Hoist].UpdateCheck();
	OnEnChangeEditOfferdlgHoistCnt();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnBnClickedCheckEndcar()
{	
	m_DetailPricesCtrls[eComponentType_EndCar].UpdateCheck();
	OnEnChangeEditOfferdlgEndcarCnt();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnBnClickedCheckBridgedri()
{	
	m_DetailPricesCtrls[eComponentType_BridgeDri].UpdateCheck();
	OnEnChangeEditOfferdlgBridgedriCnt();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnBnClickedCheckBridgepannel()
{
	m_DetailPricesCtrls[eComponentType_BridgePannel].UpdateCheck();
	OnEnChangeEditOfferdlgBridgepannelCnt();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnBnClickedCheckFastoon()
{
	m_DetailPricesCtrls[eComponentType_Fastoon].UpdateCheck();
	OnEnChangeEditOfferdlgFastoonCnt();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnBnClickedCheckOperateunit()
{
	m_DetailPricesCtrls[eComponentType_OperateUnit].UpdateCheck();
	OnEnChangeEditOfferdlgOperateunitCnt();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnBnClickedCheckCranepower()
{
	m_DetailPricesCtrls[eComponentType_CranePower].UpdateCheck();
	OnEnChangeEditOfferdlgCranepowerCnt();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnBnClickedCheckMaingirder()
{
	m_DetailPricesCtrls[eComponentType_MainGirder].UpdateCheck();
	OnEnChangeEditOfferdlgMaingirderCnt();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgHoistCnt()
{
	m_DetailPricesCtrls[eComponentType_Hoist].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgEndcarCnt()
{	
	m_DetailPricesCtrls[eComponentType_EndCar].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgBridgedriCnt()
{
	m_DetailPricesCtrls[eComponentType_BridgeDri].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgBridgepannelCnt()
{
	m_DetailPricesCtrls[eComponentType_BridgePannel].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgFastoonCnt()
{
	m_DetailPricesCtrls[eComponentType_Fastoon].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgOperateunitCnt()
{
	m_DetailPricesCtrls[eComponentType_OperateUnit].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgCranepowerCnt()
{
	m_DetailPricesCtrls[eComponentType_CranePower].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgMaingirderCnt()
{
	m_DetailPricesCtrls[eComponentType_MainGirder].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgHoistDiscount()
{
	m_DetailPricesCtrls[eComponentType_Hoist].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgEndcarDiscount()
{
	m_DetailPricesCtrls[eComponentType_EndCar].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgBridgedriDiscount()
{
	m_DetailPricesCtrls[eComponentType_BridgeDri].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgBridgepannelDiscount()
{
	m_DetailPricesCtrls[eComponentType_BridgePannel].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgFastoonDiscount()
{
	m_DetailPricesCtrls[eComponentType_Fastoon].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgOperateunitDiscount()
{
	m_DetailPricesCtrls[eComponentType_OperateUnit].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgCranepowerDiscount()
{
	m_DetailPricesCtrls[eComponentType_CranePower].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgMaingirderDiscount()
{
	m_DetailPricesCtrls[eComponentType_MainGirder].UpdateOfferPrice();
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
void CBMOfferOptionsDlg::OnEnChangeEditOfferdlgCnt()
{
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------
