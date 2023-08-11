//-----------------------------------------------------------------------
//	File Name:BMOfferCalcEndCarDlg.cpp
//	Introduce:CBMOfferCalcEndCarDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcEndCarDlg.h"
#include "BMCalcParameterDlg.h"
#include "BMOfferCalcRailDlg.h"
#include "BMOfferCalcMotorDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcEndCarDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferCalcEndCarDlg, CDialog)
CBMOfferCalcEndCarDlg::CBMOfferCalcEndCarDlg(CWnd* pParent /*=NULL*/)
	: CBMCalcOptionsDlg(CBMOfferCalcEndCarDlg::IDD, pParent)
{
	for (int i = 0; i < eCraneType_Cnt; i++)
	{
		m_pEndCarImages[i] = NULL;
	}
	m_pCurEndCarImage = NULL;
	m_bShowResult = FALSE;
	m_pPassImage = NULL;
	m_pNotPassImage = NULL;
	m_iOnlyEndCarPrice = 0;
}
//-----------------------------------------------------------------------
CBMOfferCalcEndCarDlg::~CBMOfferCalcEndCarDlg()
{
	DestroyWindow();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ENDCAR_ADDITION, m_lstCtlAdditions);
	DDX_Control(pDX, IDC_STATIC_EC_Q_RES, m_staticQResult);
	DDX_Control(pDX, IDC_STATIC_EC_F_RES, m_staticFesult);
	DDX_Control(pDX, IDC_STATIC_EC_MAXSPE_RES, m_staticMaxSpeedResult);
	DDX_Control(pDX, IDC_STATIC_EC_MAXCAPACITY_RES, m_staticMaxCapacityResult);
	DDX_Control(pDX, IDC_STATIC_EC_GROOVEWIDTH_RES, m_staticGrooveWidthResult);
	DDX_Control(pDX, IDC_STATIC_EC_RAILGAU_RES, m_staticRailGauResult);
	DDX_Control(pDX, IDC_STATIC_EC_WHEELBASE_RES, m_staticWheelBaseResult);
	DDX_Control(pDX, IDC_STATIC_EC_RAILTYPE_RES, m_staticRailTypeResult);	
	DDX_Control(pDX, IDC_STATIC_ENDCAR_ECPRICE, m_staticEndCarPrice);
	DDX_Control(pDX, IDC_STATIC_ENDCAR_MOTORPRICE, m_staticMotorPrice);
	DDX_Control(pDX, IDC_STATIC_ENDCAR_ADDPRICE, m_staticAddPrice);
	DDX_Control(pDX, IDC_STATIC_ENDCAR_TOTALPRICE, m_staticTotalPrice);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferCalcEndCarDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_SELECT, &CBMOfferCalcEndCarDlg::OnBnClickedBtnSelect)
	ON_BN_CLICKED(IDC_BTN_ENDCAR_CALC, &CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarCalc)
	ON_BN_CLICKED(IDC_BTN_ENDCAR_ADDADD, &CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarAddadd)
	ON_BN_CLICKED(IDC_BTN_ENDCAR_DELADD, &CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarDeladd)
	ON_CBN_SELCHANGE(IDC_COMBO_ECTYPE, &CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEctype)
	ON_CBN_SELCHANGE(IDC_COMBO_ECCODE, &CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEccode)
	ON_CBN_SELCHANGE(IDC_COMBO_ENDCAR_DRITYPE, &CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarDritype)
	ON_CBN_SELCHANGE(IDC_COMBO_MOTORQTY, &CBMOfferCalcEndCarDlg::OnCbnSelchangeComboMotorqty)
	ON_CBN_SELCHANGE(IDC_COMBO_ENDCAR_RAILTYPE, &CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarRailtype)
	ON_BN_CLICKED(IDC_BTN_ENDCAR_PARAMETER, &CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarParameter)
	ON_BN_CLICKED(IDC_BTN_ENDCAR_MOTOR, &CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarMotor)
	ON_BN_CLICKED(IDC_BTN_ENDCAR_RAIL, &CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarRail)
	ON_CBN_SELCHANGE(IDC_COMBO_ENDCAR_MATRIAL, &CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarMatrial)
	ON_CBN_SELCHANGE(IDC_COMBO_ENDCAR_STRUCTURE, &CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarStructure)
	ON_EN_KILLFOCUS(IDC_EDIT_ENDCAR_RAILGAUAGE, &CBMOfferCalcEndCarDlg::OnEnKillfocusEditEndcarRailgauage)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcEndCarDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferCalcEndCarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Initiliaze database table struction
	InitTablesDataStruct();

	// Initiliaze lists
	m_imgNULL.Create(1, 18, TRUE | ILC_COLOR32, 1, 0); 
	InitDataLstControls();
	
	m_rcEndCar.left = 20;	m_rcEndCar.right = m_rcEndCar.left + 378;
	m_rcEndCar.top = 300;	m_rcEndCar.bottom = m_rcEndCar.top + 148;
	//
	for (int i = 0; i < eCraneType_Cnt; i++)
	{
		m_pEndCarImages[i] = new CImage();
	}
	m_pEndCarImages[eCraneType_GTD]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_ENDCAR_D);
	m_pEndCarImages[eCraneType_GTS]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_ENDCAR_S);
	m_pEndCarImages[eCraneType_GTZ]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_ENDCAR_F);
	//
	const int iImgX = 24;
	const int iImgY = 24;
	m_rcMaxStrees.left = 630;	m_rcMaxStrees.right = m_rcMaxStrees.left + iImgX;
	m_rcMaxStrees.top = 56;		m_rcMaxStrees.bottom = m_rcMaxStrees.top + iImgY;
	//
	m_rcMaxBlend.left = 630;	m_rcMaxBlend.right = m_rcMaxBlend.left + iImgX;
	m_rcMaxBlend.top = 82;		m_rcMaxBlend.bottom = m_rcMaxBlend.top + iImgY;
	//
	m_rcMaxSpeed.left = 630;	m_rcMaxSpeed.right = m_rcMaxSpeed.left + iImgX;
	m_rcMaxSpeed.top = 108;		m_rcMaxSpeed.bottom = m_rcMaxSpeed.top + iImgY;	
	//
	m_rcMaxCapacity.left = 630;	m_rcMaxCapacity.right = m_rcMaxCapacity.left + iImgX;
	m_rcMaxCapacity.top = 134;	m_rcMaxCapacity.bottom = m_rcMaxCapacity.top + iImgY;
	//
	m_rcMaxWidth.left = 630;	m_rcMaxWidth.right = m_rcMaxWidth.left + iImgX;
	m_rcMaxWidth.top = 160;		m_rcMaxWidth.bottom = m_rcMaxWidth.top + iImgY;
	//
	m_rcRaiGauage.left = 630;	m_rcRaiGauage.right = m_rcRaiGauage.left + iImgX;
	m_rcRaiGauage.top = 186;	m_rcRaiGauage.bottom = m_rcRaiGauage.top + iImgY;
	//
	m_rcMaxWheBase.left = 630;	m_rcMaxWheBase.right = m_rcMaxWheBase.left + iImgX;
	m_rcMaxWheBase.top = 212;	m_rcMaxWheBase.bottom = m_rcMaxWheBase.top + iImgY;
	//
	m_rcRailType.left = 630;	m_rcRailType.right = m_rcRailType.left + iImgX;
	m_rcRailType.top = 238;		m_rcRailType.bottom = m_rcRailType.top + iImgY;	
	//
	m_pPassImage = new CImage();
	m_pNotPassImage = new CImage();
	m_pPassImage->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_CALCPASS);
	m_pNotPassImage->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_CALCNOTPASS);

	m_staticEndCarPrice.SetColors(COLOR_PRICE);	
	m_staticMotorPrice.SetColors(COLOR_PRICE);	
	m_staticAddPrice.SetColors(COLOR_PRICE);	
	m_staticTotalPrice.SetColors(COLOR_PRICE);	

	m_bMaxStreesOK = FALSE;
	m_bMaxBlendOK = FALSE;
	m_bMaxSpeedOK = FALSE;
	m_bMaxCapacityOK = FALSE;
	m_bMaxWidthOK = FALSE;
	m_bRaiGauageOK = FALSE;
	m_bMaxWheBaseOK = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (FALSE == bShow)
		return;

	if (_T("BMU") == m_pParamCalculator->m_CalcDatalst.GA1)
	{
		GetDlgItem(IDC_BTN_ENDCAR_RAIL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_ENDCAR_MOTOR)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_BTN_ENDCAR_RAIL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_ENDCAR_MOTOR)->ShowWindow(SW_SHOW);
	}

	m_pParamCalculator->m_CalcDatalst.DA4 = m_pParamCalculator->m_CalcDatalst.GA9;
	if (m_pParamCalculator->m_CalcDatalst.DA5.GetLength() == 0) m_pParamCalculator->m_CalcDatalst.DA5= m_pParamCalculator->m_CalcDatalst.HA8;
	if (m_pParamCalculator->m_CalcDatalst.DB7.GetLength() == 0) m_pParamCalculator->m_CalcDatalst.DB7 = m_pParamCalculator->m_CalcDatalst.DA3;
	m_pParamCalculator->UpdateRailGaugeData();

	FillComboBoxes();
	
	if (_T("BMS") == m_pParamCalculator->m_CalcDatalst.GA11)
	{
		m_bRaiGauageOK = TRUE;
		SetDlgItemText(IDC_EDIT_ENDCAR_RAILGAUAGE, _T("0"));
		GetDlgItem(IDC_EDIT_ENDCAR_RAILGAUAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_EC_RAILRES)->EnableWindow(FALSE);		
	}
	else
	{
		SetDlgItemText(IDC_EDIT_ENDCAR_RAILGAUAGE, m_pParamCalculator->m_CalcDatalst.DA5);
		GetDlgItem(IDC_EDIT_ENDCAR_RAILGAUAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_EC_RAILRES)->EnableWindow(TRUE);		
	}

	// Set initliaze data to UI
//	if (FALSE == m_bDataIsInitliazed)
	{
		if (m_pParamCalculator->m_bIsEditData)
		{
			InitUIWithDatabaseData();
			OnBnClickedBtnEndcarCalc();
		}
		else
		{
			InitUIWithDefaultData();
		}
		m_bDataIsInitliazed = TRUE;
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CImage* pCurEndCarImage = NULL;
	if (_T("S") == m_pParamCalculator->m_CalcDatalst.DB23)
		pCurEndCarImage = m_pEndCarImages[eCraneType_GTS];
	else if (_T("D") == m_pParamCalculator->m_CalcDatalst.DB23)
		pCurEndCarImage = m_pEndCarImages[eCraneType_GTD];
	else
		pCurEndCarImage = m_pEndCarImages[eCraneType_GTZ];
	if (NULL != pCurEndCarImage)
	{
		pCurEndCarImage->StretchBlt(dc.m_hDC, m_rcEndCar.left, m_rcEndCar.top, m_rcEndCar.Width(), m_rcEndCar.Height());
	}

	if (m_bShowResult)
	{		
		if (m_bMaxStreesOK) m_pPassImage->StretchBlt(dc.m_hDC, m_rcMaxStrees.left, m_rcMaxStrees.top, m_rcMaxStrees.Width(), m_rcMaxStrees.Height());
		else m_pNotPassImage->StretchBlt(dc.m_hDC, m_rcMaxStrees.left, m_rcMaxStrees.top, m_rcMaxStrees.Width(), m_rcMaxStrees.Height());
		//
		if (m_bMaxBlendOK) m_pPassImage->StretchBlt(dc.m_hDC, m_rcMaxBlend.left, m_rcMaxBlend.top, m_rcMaxBlend.Width(), m_rcMaxBlend.Height());
		else m_pNotPassImage->StretchBlt(dc.m_hDC, m_rcMaxBlend.left, m_rcMaxBlend.top, m_rcMaxBlend.Width(), m_rcMaxBlend.Height());
		//
		if (m_bMaxSpeedOK) m_pPassImage->StretchBlt(dc.m_hDC, m_rcMaxSpeed.left, m_rcMaxSpeed.top, m_rcMaxSpeed.Width(), m_rcMaxSpeed.Height());
		else m_pNotPassImage->StretchBlt(dc.m_hDC, m_rcMaxSpeed.left, m_rcMaxSpeed.top, m_rcMaxSpeed.Width(), m_rcMaxSpeed.Height());
		//
		if (m_bMaxCapacityOK) m_pPassImage->StretchBlt(dc.m_hDC, m_rcMaxCapacity.left, m_rcMaxCapacity.top, m_rcMaxCapacity.Width(), m_rcMaxCapacity.Height());
		else m_pNotPassImage->StretchBlt(dc.m_hDC, m_rcMaxCapacity.left, m_rcMaxCapacity.top, m_rcMaxCapacity.Width(), m_rcMaxCapacity.Height());
		//
		if (m_bMaxWidthOK) m_pPassImage->StretchBlt(dc.m_hDC, m_rcMaxWidth.left, m_rcMaxWidth.top, m_rcMaxWidth.Width(), m_rcMaxWidth.Height());
		else m_pNotPassImage->StretchBlt(dc.m_hDC, m_rcMaxWidth.left, m_rcMaxWidth.top, m_rcMaxWidth.Width(), m_rcMaxWidth.Height());
		//
		if (_T("S") != m_pParamCalculator->m_CalcDatalst.DB12)
		{
			if (m_bRaiGauageOK) m_pPassImage->StretchBlt(dc.m_hDC, m_rcRaiGauage.left, m_rcRaiGauage.top, m_rcRaiGauage.Width(), m_rcRaiGauage.Height());
			else m_pNotPassImage->StretchBlt(dc.m_hDC, m_rcRaiGauage.left, m_rcRaiGauage.top, m_rcRaiGauage.Width(), m_rcRaiGauage.Height());
		}
		//
		if (m_bMaxWheBaseOK) m_pPassImage->StretchBlt(dc.m_hDC, m_rcMaxWheBase.left, m_rcMaxWheBase.top, m_rcMaxWheBase.Width(), m_rcMaxWheBase.Height());
		else m_pNotPassImage->StretchBlt(dc.m_hDC, m_rcMaxWheBase.left, m_rcMaxWheBase.top, m_rcMaxWheBase.Width(), m_rcMaxWheBase.Height());
		//
		if (m_bRailTypeOK) m_pPassImage->StretchBlt(dc.m_hDC, m_rcRailType.left, m_rcRailType.top, m_rcRailType.Width(), m_rcRailType.Height());
		else m_pNotPassImage->StretchBlt(dc.m_hDC, m_rcRailType.left, m_rcRailType.top, m_rcRailType.Width(), m_rcRailType.Height());
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnDestroy()
{
	CDialog::OnDestroy();

	for (int i = 0; i < eCraneType_Cnt; i++)
	{
		if (NULL != m_pEndCarImages[i])
		{
			m_pEndCarImages[i]->Destroy();
			delete m_pEndCarImages[i];
			m_pEndCarImages[i] = NULL;
		}
	}
	if (NULL != m_pPassImage)
	{
		delete m_pPassImage;
		m_pPassImage = NULL;
	}
	if (NULL != m_pNotPassImage)
	{
		delete m_pNotPassImage;
		m_pNotPassImage = NULL;
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnBnClickedBtnSelect()
{
	// Restore EC code and driver code to default (select by main girder)
	GetDefaultEndCarData(m_pParamCalculator->m_CalcDatalst.DA11, m_pParamCalculator->m_CalcDatalst.DA12, (int&)m_pParamCalculator->m_CalcDatalst.DA16);
	m_pParamCalculator->m_CalcDatalst.DB2 = m_pParamCalculator->m_CalcDatalst.DA11;
	m_pParamCalculator->m_CalcDatalst.DB3 = m_pParamCalculator->m_CalcDatalst.DA12;
	m_pParamCalculator->m_CalcDatalst.DB4 = m_pParamCalculator->m_CalcDatalst.DA16;
	//
	m_pParamCalculator->m_CalcDatalst.DA13 = m_pParamCalculator->m_CalcDatalst.DA12.GetAt(5);
	m_pParamCalculator->m_CalcDatalst.DA14 = m_pParamCalculator->m_CalcDatalst.DA11 + m_pParamCalculator->m_CalcDatalst.DA13;
	m_pParamCalculator->m_CalcDatalst.DA15 = m_pParamCalculator->m_CalcDatalst.DA11.GetAt(0);
	//
	m_pParamCalculator->m_CalcDatalst.DA2.Format(_T("%s-%s"), m_pParamCalculator->m_CalcDatalst.DA11, m_pParamCalculator->m_CalcDatalst.DA12);
	SetDlgItemText(IDC_STATIC_ENDCARDATA, m_pParamCalculator->m_CalcDatalst.DA2);
	//
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE CodEndGer='%s'"), g_tcTableName[eTableID_EndCarSteMat], m_pParamCalculator->m_CalcDatalst.DA14);
	m_pParamCalculator->m_CalcDatalst.DA3 = GetStringValueFromTable(strSQL, _T("EndStrSec"));
	SetDlgItemText(IDC_EDIT_ENDCAR_STEELSTRUCT, m_pParamCalculator->m_CalcDatalst.DA3);
	//
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ECTYPE);
	SelectComboBoxItem(pComboBox, int(m_pParamCalculator->m_CalcDatalst.DA15.GetAt(0)));
	OnCbnSelchangeComboEctype();	
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ECCODE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.DB2);
	OnCbnSelchangeComboEccode();
	//	
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_DRITYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.DB3);
	OnCbnSelchangeComboEndcarDritype();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOTORQTY);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.DB4);
	OnCbnSelchangeComboMotorqty();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_MATRIAL);
	SelectComboBoxItem(pComboBox, 235);
	OnCbnSelchangeComboEndcarMatrial();

	m_pParamCalculator->UpdateEndCarDataText();
	SetDlgItemText(IDC_EDIT_ENDCAR_ENDCARRIAGE, m_pParamCalculator->m_CalcDatalst.DC9);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarCalc()
{	
	CString strWheelBase;
	GetDlgItemText(IDC_EDIT_ENDCAR_WHEELBASE, strWheelBase);
	m_pParamCalculator->m_CalcDatalst.DB8 = _ttol(strWheelBase);

	if (!m_pParamCalculator->CalcEndCar())
		return;	

	CString strResult;
	strResult.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.DC1);
	SetDlgItemText(IDC_EDIT_ENDCAR_MAXSTRESS, strResult);
	//
	strResult.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.DC2);
	SetDlgItemText(IDC_EDIT_ENDCAR_MAXBLEND, strResult);

	CString strTmp;
	if (m_pParamCalculator->m_CalcDatalst.DD1 <= 1.0f)
	{
		m_bMaxStreesOK = TRUE;
		strTmp.Format(_T("%0.4f <= 1.0f"), m_pParamCalculator->m_CalcDatalst.DD1);
	}
	else
	{
		m_bMaxStreesOK = FALSE;
		strTmp.Format(_T("%0.4f > 1.0f"), m_pParamCalculator->m_CalcDatalst.DD1);
	}
	m_staticQResult.SetColors(m_bMaxStreesOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticQResult.SetText(strTmp);
	//
	if (m_pParamCalculator->m_CalcDatalst.DD2 <= 1.0f)
	{
		m_bMaxBlendOK =  TRUE;
		strTmp.Format(_T("%0.4f <= 1.0f"), m_pParamCalculator->m_CalcDatalst.DD2);
	}
	else
	{
		m_bMaxBlendOK =  FALSE;
		strTmp.Format(_T("%0.4f > 1.0f"), m_pParamCalculator->m_CalcDatalst.DD2);
	}
	m_staticFesult.SetColors(m_bMaxBlendOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticFesult.SetText(strTmp);
	//
	if (m_pParamCalculator->m_CalcDatalst.DD3 <= 1.0f) 
	{
		m_bMaxSpeedOK = TRUE;
		strTmp.Format(_T("%0.4f <= 1.0f"), m_pParamCalculator->m_CalcDatalst.DD3);
	}
	else
	{
		m_bMaxSpeedOK = FALSE;
		strTmp.Format(_T("%0.4f > 1.0f"), m_pParamCalculator->m_CalcDatalst.DD3);
	}
	m_staticMaxSpeedResult.SetColors(m_bMaxSpeedOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticMaxSpeedResult.SetText(strTmp);
	//
	if (m_pParamCalculator->m_CalcDatalst.DD4 <= 1.0f)
	{
		m_bMaxCapacityOK = TRUE;
		strTmp.Format(_T("%0.4f <= 1.0f"), m_pParamCalculator->m_CalcDatalst.DD4);
	}
	else
	{
		m_bMaxCapacityOK = FALSE;
		strTmp.Format(_T("%0.4f > 1.0f"), m_pParamCalculator->m_CalcDatalst.DD4);
	}
	m_staticMaxCapacityResult.SetColors(m_bMaxCapacityOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticMaxCapacityResult.SetText(strTmp);
	//
	if (m_pParamCalculator->m_CalcDatalst.DD5 <= 1.0f)
	{
		m_bMaxWidthOK = TRUE;
		strTmp.Format(_T("%0.4f <= 1.0f"), m_pParamCalculator->m_CalcDatalst.DD5);
	}
	else
	{
		m_bMaxWidthOK = FALSE;
		strTmp.Format(_T("%0.4f > 1.0f"), m_pParamCalculator->m_CalcDatalst.DD5);
	}
	m_staticGrooveWidthResult.SetColors(m_bMaxWidthOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticGrooveWidthResult.SetText(strTmp);
	//
	if (_T("S") != m_pParamCalculator->m_CalcDatalst.DB12 &&
		_T("U") != m_pParamCalculator->m_CalcDatalst.DB12 )
	{
		if (_T("B") == m_pParamCalculator->m_CalcDatalst.DB12)
		{
			if (m_pParamCalculator->m_CalcDatalst.DD63 <= 1 && m_pParamCalculator->m_CalcDatalst.DD64 <= 1)
			{
				SetDlgItemText(IDC_STATIC_EC_RAILRES, _T("Rmin/R="));
				strTmp.Format(_T("%0.4f <= 1.0f"), m_pParamCalculator->m_CalcDatalst.DD6);
				m_bRaiGauageOK = TRUE;
			}
			else if (m_pParamCalculator->m_CalcDatalst.DD63 > 1 && m_pParamCalculator->m_CalcDatalst.DD64 <= 1)
			{
				SetDlgItemText(IDC_STATIC_EC_RAILRES, _T("Rmin/R="));
				strTmp.Format(_T("%0.4f > 1.0f"), m_pParamCalculator->m_CalcDatalst.DD6);
				m_bRaiGauageOK = FALSE;
			}
			else
			{
				SetDlgItemText(IDC_STATIC_EC_RAILRES, _T("R/Rmax="));
				strTmp.Format(_T("%0.4f > 1.0f"), m_pParamCalculator->m_CalcDatalst.DD6);
				m_bRaiGauageOK = FALSE;
			}
		}
		else
		{
			if (m_pParamCalculator->m_CalcDatalst.DD6 <= 1)
			{
				strTmp.Format(_T("%0.4f <= 1.0f"), m_pParamCalculator->m_CalcDatalst.DD6);
				m_bRaiGauageOK = TRUE;
			}
			else
			{
				strTmp.Format(_T("%0.4f > 1.0f"), m_pParamCalculator->m_CalcDatalst.DD6);
				m_bRaiGauageOK = FALSE;
			}
		}
		m_staticRailGauResult.SetColors(m_bRaiGauageOK ? COLOR_PASS : COLOR_NOTPASS);
		m_staticRailGauResult.SetText(strTmp);
	}
	//
	if (m_pParamCalculator->m_CalcDatalst.DD7 <= 1.0f)
	{
		m_bMaxWheBaseOK = TRUE;
		strTmp.Format(_T("%0.4f <= 1.0f"), m_pParamCalculator->m_CalcDatalst.DD7);
	}
	else
	{
		m_bMaxWheBaseOK = FALSE;
		strTmp.Format(_T("%0.4f > 1.0f"), m_pParamCalculator->m_CalcDatalst.DD7);
	}
	m_staticWheelBaseResult.SetColors(m_bMaxWheBaseOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticWheelBaseResult.SetText(strTmp);
	//
	m_bRailTypeOK = m_pParamCalculator->m_bRailTypeIsOK;
	m_staticRailTypeResult.SetColors(m_bRailTypeOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticRailTypeResult.SetText(m_bRailTypeOK ? _T("OK") : _T("Rail not suitable"));
	//
	UpdateBufferExtend();
	UpdateBufferType();
	m_pParamCalculator->UpdateEndCarDataText();
	SetDlgItemText(IDC_EDIT_ENDCAR_ENDCARRIAGE, m_pParamCalculator->m_CalcDatalst.DC9);
	UpdatePrices();

	m_bShowResult = TRUE;
	InvalidateRect(&m_rcMaxStrees);
	InvalidateRect(&m_rcMaxBlend);
	InvalidateRect(&m_rcMaxSpeed);
	InvalidateRect(&m_rcMaxCapacity);
	InvalidateRect(&m_rcMaxWidth);
	InvalidateRect(&m_rcRaiGauage);
	InvalidateRect(&m_rcMaxWheBase);
	InvalidateRect(&m_rcRailType);	
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarParameter()
{
	CBMCalcParameterDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarMotor()
{
	CBMOfferCalcMotorDlg dlg;
	dlg.SetParamCalculator(m_pParamCalculator);
	if (dlg.DoModal() == IDOK)
	{
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarRail()
{
	CBMOfferCalcRailDlg dlg;
	dlg.SetParamCalculator(m_pParamCalculator);
	if (dlg.DoModal() == IDOK)
	{
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarAddadd()
{
	StringList lstSelAddNames;
	int iSelAddCnt = m_lstCtlAdditions.GetItemCount();
	for (int iIndex = 0; iIndex < iSelAddCnt; iIndex++)
	{
		CString strAddCode;
		strAddCode = m_lstCtlAdditions.GetItemText(iIndex, 1);
		lstSelAddNames.push_back(strAddCode);
	}

	m_pParamCalculator->m_CalcDatalst.DE3 = 0;

	StringList lstNewAddNames;
	CBMSelectAdditionsDlg dlg(eAdditionType_EndCar);
	dlg.SetSelAdditions(lstSelAddNames);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetSelAdditions(lstNewAddNames);
		m_lstCtlAdditions.DeleteAllItems();
		m_pParamCalculator->m_lstEndCarAdditions.clear();
		
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
						m_pParamCalculator->m_lstEndCarAdditions.push_back(strAddCode);
						m_pParamCalculator->m_CalcDatalst.DE3 += dwAddPrice;
						dwSelIndex++;
						break;
					}
				}
			}
		}
		tbAdditions.CloseTable();
	}
	UpdatePrices();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarDeladd()
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
void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEctype()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ECTYPE);
	pComboBox->GetWindowText(m_pParamCalculator->m_CalcDatalst.DB1);
	int iSelIndex = pComboBox->GetCurSel();
	int iItemData = pComboBox->GetItemData(iSelIndex);
	m_pParamCalculator->m_CalcDatalst.DB12 = (TCHAR)iItemData;
	if (m_pParamCalculator->m_CalcDatalst.DB12 == _T("D"))
	{
		//SetDlgItemText(IDC_STATIC_EC_RAILRES, _T("R/Rmax="));
		m_pParamCalculator->m_CalcDatalst.DB11 = 2;
		m_pCurEndCarImage = m_pEndCarImages[eCraneType_GTD];
	}
	else if (m_pParamCalculator->m_CalcDatalst.DB12 == _T("B"))
	{
		//SetDlgItemText(IDC_STATIC_EC_RAILRES, _T("Rmin/R="));
		m_pParamCalculator->m_CalcDatalst.DB11 = 4;
		m_pCurEndCarImage = m_pEndCarImages[eCraneType_GTZ];
	}
	else //"S" or "U"
	{
		m_pParamCalculator->m_CalcDatalst.DB11 = 2;
		m_pCurEndCarImage = m_pEndCarImages[eCraneType_GTS];
	}
	InvalidateRect(&m_rcEndCar);
	//
	FillECCodeComboBox();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEccode()
{
	CString strDB22;
	GetDlgItemText(IDC_COMBO_ECCODE, m_pParamCalculator->m_CalcDatalst.DB2);	
	m_pParamCalculator->m_CalcDatalst.DB21.Format(_T("%c%c"), m_pParamCalculator->m_CalcDatalst.DB2.GetAt(1), m_pParamCalculator->m_CalcDatalst.DB2.GetAt(2));
	m_pParamCalculator->m_CalcDatalst.DB23.Format(_T("%c"), m_pParamCalculator->m_CalcDatalst.DB2.GetAt(0));
	m_pParamCalculator->m_CalcDatalst.DB24.Format(_T("%c%c%c"), m_pParamCalculator->m_CalcDatalst.DB2.GetAt(0), m_pParamCalculator->m_CalcDatalst.DB2.GetAt(1), m_pParamCalculator->m_CalcDatalst.DB2.GetAt(2));
	strDB22.Format(_T("%c%c"), m_pParamCalculator->m_CalcDatalst.DB2.GetAt(3), m_pParamCalculator->m_CalcDatalst.DB2.GetAt(4));
	m_pParamCalculator->m_CalcDatalst.DB22 = _ttol(strDB22);
	//
	FillDriCodeComboBox();
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_DRITYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.DB3);
	OnCbnSelchangeComboEndcarDritype();
	//
	CString strSQL;
	CString strTmp;
	strSQL.Format(_T("SELECT * FROM %s WHERE CodEndCarStr='%s'"), g_tcTableName[eTableID_EndCarStePri], m_pParamCalculator->m_CalcDatalst.DB2);	
	m_iOnlyEndCarPrice = GetLONGValueFromTable(strSQL, _T("PriEndCarStr"));
	strTmp.Format(_T("%d RMB"), m_iOnlyEndCarPrice);
	m_staticEndCarPrice.SetText(strTmp);
	UpdatePrices();
	//
	//Has called in OnCbnSelchangeComboEndcarDritype()
	//UpdateDataBasedOnCodeAndDriver();
	UpdateWheelBase();
	UpdateBufferExtend();
	UpdateBufferType();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarDritype()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ENDCAR_DRITYPE);
	int iSelIndex = pComboBox->GetCurSel();
	int iItemData = pComboBox->GetItemData(iSelIndex);
	m_pParamCalculator->m_CalcDatalst.DB35 = iItemData / 100.0f;	
	pComboBox->GetWindowText(m_pParamCalculator->m_CalcDatalst.DB3);

	m_pParamCalculator->UpdateDriverTypeData();

	UpdateEndcarStructure();
	UpdateJointPlateData();
	UpdateRailData();
	UpdateDataBasedOnCodeAndDriver();
	UpdateMotorPrice();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnEnKillfocusEditEndcarRailgauage()
{
	GetDlgItemText(IDC_EDIT_ENDCAR_RAILGAUAGE, m_pParamCalculator->m_CalcDatalst.DA5);
	m_pParamCalculator->UpdateRailGaugeData();

	UpdateWheelBase();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboMotorqty()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_MOTORQTY);
	int iSelIndex = pComboBox->GetCurSel();
	int iItemData = pComboBox->GetItemData(iSelIndex);
	m_pParamCalculator->m_CalcDatalst.DB4 = iItemData;	
	if (4 == m_pParamCalculator->m_CalcDatalst.DB4) 
		m_pParamCalculator->m_CalcDatalst.DB41 = _T("=");
	else 
		m_pParamCalculator->m_CalcDatalst.DB41 = _T("-");
	//
	UpdateDataBasedOnCodeAndDriver();
	UpdateMotorPrice();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarMatrial()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ENDCAR_MATRIAL);
	int iSelIndex = pComboBox->GetCurSel();
	int iItemData = pComboBox->GetItemData(iSelIndex);
	m_pParamCalculator->m_CalcDatalst.DB61 = iItemData;	
	pComboBox->GetWindowText(m_pParamCalculator->m_CalcDatalst.DB6);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarStructure()
{
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_STRUCTURE);
	pComboBox->GetWindowText(m_pParamCalculator->m_CalcDatalst.DB7);
	m_pParamCalculator->UpdateECStructureData();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarRailtype()
{
	GetDlgItemText(IDC_COMBO_ENDCAR_RAILTYPE, m_pParamCalculator->m_CalcDatalst.DA4);
	m_pParamCalculator->m_CalcDatalst.GA9 = m_pParamCalculator->m_CalcDatalst.DA4;
	m_pParamCalculator->UpdateRailTypeData(TRUE);
	
	// Update groove width
	CString strDC5;
	m_pParamCalculator->m_CalcDatalst.DC5 = m_pParamCalculator->m_CalcDatalst.DA41 + m_pParamCalculator->m_CalcDatalst.P23;
	strDC5.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.DC5);
	SetDlgItemText(IDC_EDIT_ENDCAR_GROOVEWIDTH, strDC5);
}
//-----------------------------------------------------------------------






//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::InitTablesDataStruct()
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
void CBMOfferCalcEndCarDlg::InitDataLstControls()
{
	m_lstCtlAdditions.SetExtendedStyle(m_lstCtlAdditions.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_lstCtlAdditions.SetFullItemBKColor(RGB(214, 231, 255), RGB(94, 231, 221));
	m_lstCtlAdditions.SetImageList(&m_imgNULL, LVSIL_SMALL);
	InitDataListCtrl(&m_lstCtlAdditions, m_stuAdditionTable);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::FillComboBoxes()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ENDCAR_RAILTYPE);	
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

	int iIndex = 0;
	pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ECTYPE);	
	pComboBox->ResetContent();
	if (_T("BMD") == m_pParamCalculator->m_CalcDatalst.GA1 ||
		_T("BMZ") == m_pParamCalculator->m_CalcDatalst.GA1)
	{
		iIndex = pComboBox->AddString(_T("D-EC"));
		pComboBox->SetItemData(iIndex, int('D'));
		iIndex = pComboBox->AddString(_T("B-EC"));
		pComboBox->SetItemData(iIndex, int('B'));
	}
	if (_T("BMS") == m_pParamCalculator->m_CalcDatalst.GA1)
	{
		iIndex = pComboBox->AddString(_T("S-EC"));
		pComboBox->SetItemData(iIndex, int('S'));
	}
	else
	{
		iIndex = pComboBox->AddString(_T("U-EC"));
		pComboBox->SetItemData(iIndex, int('U'));
	}
	//
	FillDriCodeComboBox();
	// 
	pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_MOTORQTY);
	pComboBox->ResetContent();
	iIndex = pComboBox->AddString(_T("2"));
	pComboBox->SetItemData(iIndex, 2);
	iIndex = pComboBox->AddString(_T("4"));
	pComboBox->SetItemData(iIndex, 4);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_MATRIAL);
	pComboBox->ResetContent();
	iIndex = pComboBox->AddString(_T("Q235"));
	pComboBox->SetItemData(iIndex, 235);
	iIndex = pComboBox->AddString(_T("Q345"));
	pComboBox->SetItemData(iIndex, 345);
	pComboBox->SetCurSel(0);
	// 
	FillECStructureComboBox();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::FillECCodeComboBox()
{
	int iDefaultSel = 0;
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ECCODE);
	pComboBox->ResetContent();

	CString strSQL;
	CADOTable tbEndCarCode;
	strSQL.Format(_T("SELECT DISTINCT CodEndCar FROM %s WHERE CodEndCar LIKE '%s%s' "), g_tcTableName[eTableID_EndCarSteMat], m_pParamCalculator->m_CalcDatalst.DB12, _T("%"));
	if (tbEndCarCode.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL))
	{
		DWORD dwTotalCnt = tbEndCarCode.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwTotalCnt; dwIndex++)
		{
			CString strEndCarCode;
			TCHAR tcEndCarCode[32] = {0};
			tbEndCarCode.GetRecordCell(dwIndex, _T("CodEndCar"), tcEndCarCode, 32);
			strEndCarCode = tcEndCarCode;
			//
			int iWheDim = 0;
			CString strWheDim;
			strWheDim.Format(_T("%c%c"), strEndCarCode.GetAt(1), strEndCarCode.GetAt(2));
			iWheDim = _ttol(strWheDim);
			int iIndex = pComboBox->AddString(tcEndCarCode);
			pComboBox->SetItemData(iIndex, iWheDim);
			//
			if (_tcsicmp(tcEndCarCode, m_pParamCalculator->m_CalcDatalst.DA11) == 0)
			{
				iDefaultSel = dwIndex;
			}
		}

		tbEndCarCode.CloseTable();
	}
	pComboBox->SetCurSel(iDefaultSel);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::FillDriCodeComboBox()
{
	int iDefaultSel = 0;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_DRITYPE);
	pComboBox->ResetContent();

	CString strSQL;
	CADOTable tbCraneData;
	strSQL.Format(_T("SELECT * FROM %s WHERE WheCode = '%s'"), g_tcTableName[eTableID_WheMot], m_pParamCalculator->m_CalcDatalst.DB21);
	if (tbCraneData.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL))
	{
		DWORD dwTotalCnt = tbCraneData.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwTotalCnt; dwIndex++)
		{
			TCHAR tcDriverCode[32] = {0};
			tbCraneData.GetRecordCell(dwIndex, _T("TraDriTyp"), tcDriverCode, 32);
			int iIndex = pComboBox->AddString(tcDriverCode);
		}

		tbCraneData.CloseTable();
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::InitUIWithDefaultData()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ENDCAR_RAILTYPE);	
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.DA4);
	OnCbnSelchangeComboEndcarRailtype();
	//
	pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ECTYPE);	
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboEctype();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_DRITYPE);
	pComboBox->SetCurSel(0);
	OnCbnSelchangeComboEndcarDritype();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOTORQTY);
	SelectComboBoxItem(pComboBox, 2);
	OnCbnSelchangeComboMotorqty();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_MATRIAL);
	SelectComboBoxItem(pComboBox, _T("235"));
	OnCbnSelchangeComboEndcarMatrial();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::InitUIWithDatabaseData()
{
	SetDlgItemText(IDC_STATIC_ENDCARDATA, m_pParamCalculator->m_CalcDatalst.DA2);
	//
	SetDlgItemText(IDC_EDIT_ENDCAR_STEELSTRUCT, m_pParamCalculator->m_CalcDatalst.DA3);
	//
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ENDCAR_RAILTYPE);	
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.DA4);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ECTYPE);
	SelectComboBoxItem(pComboBox, (int)m_pParamCalculator->m_CalcDatalst.DB12.GetAt(0));
	//	
	FillECCodeComboBox();
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ECCODE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.DB2);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_DRITYPE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.DB3);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOTORQTY);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.DB4);
	//
	SetDlgItemText(IDC_EDIT_ENDCAR_JOINTLATE, m_pParamCalculator->m_CalcDatalst.DB5);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_MATRIAL);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.DB6);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_STRUCTURE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.DB7);
	//	
	CString strWheelBase;
	strWheelBase.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.DB8);
	SetDlgItemText(IDC_EDIT_ENDCAR_WHEELBASE, strWheelBase);
	//
	CString strMaxSpeed;
	strMaxSpeed.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.DC3);
	SetDlgItemText(IDC_EDIT_ENDCAR_MAXSPE, strMaxSpeed);
	//
	CString strCapacity;
	strCapacity.Format(_T("%.2f"), m_pParamCalculator->m_CalcDatalst.DC4);
	SetDlgItemText(IDC_EDIT_ENDCAR_CAPACITY, strCapacity);
	//
	CString strDC5;
	strDC5.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.DC5);
	SetDlgItemText(IDC_EDIT_ENDCAR_GROOVEWIDTH, strDC5);
	//
	CString strBoggieW;
	strBoggieW.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.DC6);
	SetDlgItemText(IDC_EDIT_ENDCAR_BOGGIEW, strBoggieW);
	//
	CString strBuffExt;
	if (m_pParamCalculator->m_CalcDatalst.DC7 <= 0) strBuffExt = _T("No need");
	else strBuffExt.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.DC7);
	SetDlgItemText(IDC_EDIT_ENDCAR_BUFFEXT, strBuffExt);
	//
	SetDlgItemText(IDC_EDIT_ENDCAR_BUFFTYPE, m_pParamCalculator->m_CalcDatalst.DC8);
	//
	SetDlgItemText(IDC_EDIT_ENDCAR_ENDCARRIAGE, m_pParamCalculator->m_CalcDatalst.DC9);
	//
	CString strSQL;
	CString strTmp;
	strSQL.Format(_T("SELECT * FROM %s WHERE CodEndCarStr='%s'"), g_tcTableName[eTableID_EndCarStePri], m_pParamCalculator->m_CalcDatalst.DB2);	
	m_iOnlyEndCarPrice = GetLONGValueFromTable(strSQL, _T("PriEndCarStr"));
	strTmp.Format(_T("%d RMB"), m_iOnlyEndCarPrice);
	m_staticEndCarPrice.SetText(strTmp);
	//
	UpdateMotorPrice();

	// List additions	
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
			for (list<CString>::iterator it = m_pParamCalculator->m_lstEndCarAdditions.begin();
				it != m_pParamCalculator->m_lstEndCarAdditions.end();
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
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::GetDefaultEndCarData(CString &strECCode, 
												 CString &strDriCode, 
												 int &iMotorCnt)
{
	BOOL bResult = TRUE;
	CADOTable tbCranesData;
	CStringW strOpenOperation;
	//strOpenOperation.Format(L"SELECT * FROM %s WHERE CraTyp='%s' AND HoiLoa=%d AND CraSpa>=%f ORDER BY CraSpa", g_tcTableName[eTableID_CraneData], m_pParamCalculator->m_CalcDatalst.GA12, m_pParamCalculator->m_CalcDatalst.GA3/1000, m_pParamCalculator->m_CalcDatalst.GA5);
	strOpenOperation.Format(_T("SELECT * FROM %s WHERE CraCod='%s'"), g_tcTableName[eTableID_CraneData], m_pParamCalculator->m_CalcDatalst.MA11);
	bResult = tbCranesData.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
	if (TRUE == bResult && tbCranesData.GetRecordCnt() > 0)
	{
		TCHAR tcEndCarCode[64] = {0};
		TCHAR tcDriverCode[64] = {0};
		tbCranesData.GetRecordCell(0, L"CodEndCar", tcEndCarCode, 64);
		tbCranesData.GetRecordCell(0, L"CodBriDri", tcDriverCode, 64);
		tbCranesData.GetRecordCell(0, L"NumBriDri", (DWORD&)iMotorCnt);
		strECCode = tcEndCarCode;
		strDriCode = tcDriverCode;
	}
	else
	{
		strECCode = m_pParamCalculator->m_CalcDatalst.DA11;
		strDriCode = m_pParamCalculator->m_CalcDatalst.DA12;
		iMotorCnt = m_pParamCalculator->m_CalcDatalst.DA16;
	}
	tbCranesData.CloseTable();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::FillECStructureComboBox()
{
	int iDefaultSel = 0;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_STRUCTURE);
	pComboBox->ResetContent();

	CString strSQL;
	CADOTable tbEndCarMatPar;
	strSQL.Format(_T("SELECT * FROM %s"), g_tcTableName[eTableID_EndCarMatPar]);
	if (tbEndCarMatPar.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL))
	{
		DWORD dwTotalCnt = tbEndCarMatPar.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwTotalCnt; dwIndex++)
		{
			DWORD dwEcIx = 0;
			DWORD dwEcW = 0;
			TCHAR tcEndCarSec[32] = {0};
			tbEndCarMatPar.GetRecordCell(dwIndex, _T("EndCarSec"), tcEndCarSec, 32);
			int iIndex = pComboBox->AddString(tcEndCarSec);
		}

		tbEndCarMatPar.CloseTable();
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateRailData()
{
	m_pParamCalculator->UpdateRailData();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateEndcarStructure()
{	
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE CodEndGer='%s'"), g_tcTableName[eTableID_EndCarSteMat], m_pParamCalculator->m_CalcDatalst.DA14);
	m_pParamCalculator->m_CalcDatalst.DB7 = GetStringValueFromTable(strSQL, _T("EndStrSec"));
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ENDCAR_STRUCTURE);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_CalcDatalst.DB7);	
	m_pParamCalculator->UpdateECStructureData();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateJointPlateData()
{	
	m_pParamCalculator->UpdateJointPlateData();
	SetDlgItemText(IDC_EDIT_ENDCAR_JOINTLATE, m_pParamCalculator->m_CalcDatalst.DB5);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateDataBasedOnCodeAndDriver()
{
	// Get Max Capacity and Max Speed
	CString strTmp;
	m_pParamCalculator->m_CalcDatalst.DC3 = m_pParamCalculator->m_CalcDatalst.DB38;
	m_pParamCalculator->m_CalcDatalst.DC4 = m_pParamCalculator->m_CalcDatalst.DB37 * m_pParamCalculator->m_CalcDatalst.DB4 / 2;
	strTmp.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.DC3);
	SetDlgItemText(IDC_EDIT_ENDCAR_MAXSPE, strTmp);
	strTmp.Format(_T("%0.2f"), m_pParamCalculator->m_CalcDatalst.DC4);
	SetDlgItemText(IDC_EDIT_ENDCAR_CAPACITY, strTmp);

	// Endcar total power KW
	m_pParamCalculator->m_CalcDatalst.DB42 = m_pParamCalculator->m_CalcDatalst.DB35 * m_pParamCalculator->m_CalcDatalst.DB4;
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateWheelBase()
{
	m_pParamCalculator->UpdateWheelBaseData();

	CString strWheelBase;
	strWheelBase.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.DB8);
	SetDlgItemText(IDC_EDIT_ENDCAR_WHEELBASE, strWheelBase);
	if (_T("B") == m_pParamCalculator->m_CalcDatalst.DB12)
		GetDlgItem(IDC_EDIT_ENDCAR_WHEELBASE)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_EDIT_ENDCAR_WHEELBASE)->EnableWindow(TRUE);

	CString strBoggieW;
	strBoggieW.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.DC6);
	SetDlgItemText(IDC_EDIT_ENDCAR_BOGGIEW, strBoggieW);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateBufferExtend()
{
	m_pParamCalculator->UpdateBufferExtendData();

	CString strBuffExt;
	if (m_pParamCalculator->m_CalcDatalst.DC7 <= 0) strBuffExt = _T("No need");
	else strBuffExt.Format(_T("%d"), m_pParamCalculator->m_CalcDatalst.DC7);
	SetDlgItemText(IDC_EDIT_ENDCAR_BUFFEXT, strBuffExt);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateBufferType()
{
	m_pParamCalculator->UpdateBufferTypeData();
	SetDlgItemText(IDC_EDIT_ENDCAR_BUFFTYPE, m_pParamCalculator->m_CalcDatalst.DC8);
}
//-----------------------------------------------------------------------
/*
void CBMOfferCalcEndCarDlg::UpdateEndCarDataText()
{
	CString strFirst5;	
	if (_T("B") == m_pParamCalculator->m_CalcDatalst.DB12)
	{
		strFirst5.Format(_T("%s"), m_pParamCalculator->m_CalcDatalst.DB2);
	}
	else
	{
		strFirst5.Format(_T("%s%02d"), m_pParamCalculator->m_CalcDatalst.DB24, int(m_pParamCalculator->m_CalcDatalst.DB8/100.0 + 0.5f));
	}

	m_pParamCalculator->m_CalcDatalst.DC9.Format(_T("%s/%d%s%s%s/%s/%s/%s%s"),
												 strFirst5,
												 m_pParamCalculator->m_CalcDatalst.DC5,
												 m_pParamCalculator->m_CalcDatalst.DB41,
												 m_pParamCalculator->m_CalcDatalst.DA51,
												 m_pParamCalculator->m_CalcDatalst.DB5,
												 m_pParamCalculator->m_CalcDatalst.DB36,
												 m_pParamCalculator->m_CalcDatalst.DC61,
												 m_pParamCalculator->m_CalcDatalst.DC78,
												 m_pParamCalculator->m_CalcDatalst.DC8);

	SetDlgItemText(IDC_EDIT_ENDCAR_ENDCARRIAGE, m_pParamCalculator->m_CalcDatalst.DC9);
}*/
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateMotorPrice()
{
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE TroDriTyp='%s'"), g_tcTableName[eTableID_TroDriDat], m_pParamCalculator->m_CalcDatalst.DB3);	
	m_pParamCalculator->m_CalcDatalst.DE5 = GetLONGValueFromTable(strSQL, _T("PriTraDriRmb"));
	m_pParamCalculator->m_CalcDatalst.DE5 *= m_pParamCalculator->m_CalcDatalst.DB4;
	//
	CString strPrice;
	strPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.DE5);
	m_staticMotorPrice.SetText(strPrice);
	//
	UpdatePrices();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdatePrices()
{
	CString strAddsPrice;
	strAddsPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.DE3);
	m_staticAddPrice.SetText(strAddsPrice);
	//
	CString strTotalPrice;
	m_pParamCalculator->m_CalcDatalst.DE4 = m_iOnlyEndCarPrice + m_pParamCalculator->m_CalcDatalst.DE3;
	m_pParamCalculator->m_CalcDatalst.DE41 = m_pParamCalculator->m_CalcDatalst.DE4 + m_pParamCalculator->m_CalcDatalst.DC81 * 4;
	m_pParamCalculator->m_CalcDatalst.DE6 = m_pParamCalculator->m_CalcDatalst.DE4 + m_pParamCalculator->m_CalcDatalst.DE5;
	strTotalPrice.Format(_T("%d RMB"), m_pParamCalculator->m_CalcDatalst.DE6);
	m_staticTotalPrice.SetText(strTotalPrice);
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
BOOL CBMOfferCalcEndCarDlg::CanDoNextStep()
{
	if (!m_bMaxStreesOK ||
		!m_bMaxBlendOK ||
		!m_bMaxSpeedOK ||
		!m_bMaxCapacityOK ||
		!m_bMaxWidthOK ||
		!m_bRaiGauageOK ||
		!m_bMaxWheBaseOK ||
		!m_bRailTypeOK)
	{
		myMessageBox(IDS_CALC_ENDCARISNOTSUITABLE, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}
//-----------------------------------------------------------------------