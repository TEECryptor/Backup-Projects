//-----------------------------------------------------------------------
//	File Name:BMOfferCalcEndCarDlg.cpp
//	Introduce:CBMOfferCalcEndCarDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcEndCarDlg.h"
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
	m_bShowResult = FALSE;
	m_pPassImage = NULL;
	m_pNotPassImage = NULL;
	m_iEndCarPrice = 0;
	m_iMotorPrice = 0;
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
	ON_CBN_SELCHANGE(IDC_COMBO_JOINTPLATE, &CBMOfferCalcEndCarDlg::OnCbnSelchangeComboJointplate)
	ON_CBN_SELCHANGE(IDC_COMBO_ENDCAR_MATRIAL, &CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarMatrial)
	ON_CBN_SELCHANGE(IDC_COMBO_ENDCAR_STRUCTURE, &CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarStructure)
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
	m_pEndCarImages[eCraneType_GTF]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_ENDCAR_F);
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

	FillComboBoxes();
	
	// Set initliaze data to UI
	if (FALSE == m_bDataIsInitliazed)
	{
		/* commented by Demo
		if (m_pParamCalculator->m_bIsEditData)
		{
			InitUIWithDatabaseData();
			OnBnClickedBtnEndcarCalc();
		}
		else*/
		{
			InitUIWithDefaultData();
		}
		m_bDataIsInitliazed = TRUE;
	}
	//
	if (eCraneType_GTS == m_pParamCalculator->m_eCraneType)
	{
		m_bRaiGauageOK = TRUE;
		SetDlgItemText(IDC_EDIT_ENDCAR_RAILGAUAGE, _T("0"));
		GetDlgItem(IDC_EDIT_ENDCAR_RAILGAUAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_EC_RAILRES)->EnableWindow(FALSE);		
	}
	else
	{
		SetDlgItemText(IDC_EDIT_ENDCAR_RAILGAUAGE, m_pParamCalculator->m_strRaiGauge);
		GetDlgItem(IDC_EDIT_ENDCAR_RAILGAUAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_EC_RAILRES)->EnableWindow(TRUE);		
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (NULL != m_pEndCarImages[m_pParamCalculator->m_eCraneType])
	{
		m_pEndCarImages[m_pParamCalculator->m_eCraneType]->StretchBlt(	dc.m_hDC,	
																		m_rcEndCar.left, 
																		m_rcEndCar.top, 
																		m_rcEndCar.Width(), 
																		m_rcEndCar.Height());
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
		if (_T("S") != m_pParamCalculator->m_strECType)
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
	int iDefaultMotorCnt;
	CString strDefaultECType;
	CString strDefaultECCode;
	CString strDefaultDriCode;
	GetDefaultEndCarData(strDefaultECCode, strDefaultDriCode, iDefaultMotorCnt);
	strDefaultECType = strDefaultECCode.GetAt(0);
	//
	CString strEndCarData;
	strEndCarData.Format(_T("%s-%s"), strDefaultECCode, strDefaultDriCode);
	SetDlgItemText(IDC_STATIC_ENDCARDATA, strEndCarData);
	//
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE CodEndCar='%s'"), g_tcTableName[eTableID_EndCarSteMat], strDefaultECCode);
	CString strSteelStruct = GetStringValueFromTable(strSQL, _T("EndStrSec"));
	SetDlgItemText(IDC_EDIT_ENDCAR_STEELSTRUCT, strSteelStruct);
	//
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ECTYPE);
	SelectComboBoxItem(pComboBox, int(strDefaultECType.GetAt(0)));
	SelectedECTypeIsChanged();	
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ECCODE);
	SelectComboBoxItem(pComboBox, strDefaultECCode);
	SelectedECCodeIsChanged();
	//	
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_DRITYPE);
	SelectComboBoxItem(pComboBox, strDefaultDriCode);
	SelectedECDriverIsChanged();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOTORQTY);
	SelectComboBoxItem(pComboBox, iDefaultMotorCnt);
	SelectedMotorQtyIsChanged();
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_MATRIAL);
	SelectComboBoxItem(pComboBox, 235);

	UpdateEndCarDataText();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnBnClickedBtnEndcarCalc()
{
	/*	Get user setting data */
	UpdateEndcarStructure();
	//
	CString strWheelBase;
	GetDlgItemText(IDC_EDIT_ENDCAR_WHEELBASE, strWheelBase);
	m_pParamCalculator->m_iWheBase = _ttol(strWheelBase);

	if (!m_pParamCalculator->CalcEndCar())
		return;	

	CString strResult;
	strResult.Format(_T("%0.2f"), m_pParamCalculator->m_fMaxStress);
	SetDlgItemText(IDC_EDIT_ENDCAR_MAXSTRESS, strResult);
	//
	strResult.Format(_T("%0.2f"), m_pParamCalculator->m_fMaxBlend);
	SetDlgItemText(IDC_EDIT_ENDCAR_MAXBLEND, strResult);

	CString strTmp;
	CComboBox* pMatrialCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_MATRIAL);
	int iSelIndex = pMatrialCombo->GetCurSel();
	m_pParamCalculator->m_iEndCar_Matrial = pMatrialCombo->GetItemData(iSelIndex);
	float fQ_Result = m_pParamCalculator->m_fMaxStress / (m_pParamCalculator->m_iEndCar_Matrial / m_pParamCalculator->m_fParam_n);
	if (fQ_Result <= 1.0f)
	{
		m_bMaxStreesOK = TRUE;
		strTmp.Format(_T("%0.4f <= 1.0f"), fQ_Result);
	}
	else
	{
		m_bMaxStreesOK = FALSE;
		strTmp.Format(_T("%0.4f > 1.0f"), fQ_Result);
	}
	m_staticQResult.SetColors(m_bMaxStreesOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticQResult.SetText(strTmp);
	//
	int iEndCarLen = 0;
	CString strEndCarLen;
	strEndCarLen.Format(_T("%c%c"), m_pParamCalculator->m_strEndCraCode.GetAt(3), m_pParamCalculator->m_strEndCraCode.GetAt(4));
	iEndCarLen = _ttol(strEndCarLen);
	float fF_Result = m_pParamCalculator->m_fMaxBlend / (iEndCarLen * 100 / m_pParamCalculator->m_fParam_EC_Stiffness);
	if (fF_Result <= 1.0f)
	{
		m_bMaxBlendOK =  TRUE;
		strTmp.Format(_T("%0.4f <= 1.0f"), fF_Result);
	}
	else
	{
		m_bMaxBlendOK =  FALSE;
		strTmp.Format(_T("%0.4f > 1.0f"), fF_Result);
	}
	m_staticFesult.SetColors(m_bMaxBlendOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticFesult.SetText(strTmp);
	//
	float fV3_Result = (float)m_pParamCalculator->m_iHoist_V3 / m_pParamCalculator->m_iMaxSpeed;
	if (fV3_Result <= 1.0f) 
	{
		m_bMaxSpeedOK = TRUE;
		strTmp.Format(_T("%0.4f <= 1.0f"), fV3_Result);
	}
	else
	{
		m_bMaxSpeedOK = FALSE;
		strTmp.Format(_T("%0.4f > 1.0f"), fV3_Result);
	}
	m_staticMaxSpeedResult.SetColors(m_bMaxSpeedOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticMaxSpeedResult.SetText(strTmp);
	//
	float fCapacity = m_pParamCalculator->GetCapacityFactor();
	if (fCapacity <= 1.0f)
	{
		m_bMaxCapacityOK = TRUE;
		strTmp.Format(_T("%0.4f <= 1.0f"), fCapacity);
	}
	else
	{
		m_bMaxCapacityOK = FALSE;
		strTmp.Format(_T("%0.4f > 1.0f"), fCapacity);
	}
	m_staticMaxCapacityResult.SetColors(m_bMaxCapacityOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticMaxCapacityResult.SetText(strTmp);
	//
	int iRailMaxWidth = GetRailMaxWidth();
	iRailMaxWidth = max(iRailMaxWidth, 1);
	float fGrooveWid_Result = float(m_pParamCalculator->m_iRaiWidTop + 20) / iRailMaxWidth;
	if (fGrooveWid_Result <= 1.0f)
	{
		m_bMaxWidthOK = TRUE;
		strTmp.Format(_T("%0.4f <= 1.0f"), fGrooveWid_Result);
	}
	else
	{
		m_bMaxWidthOK = FALSE;
		strTmp.Format(_T("%0.4f > 1.0f"), fGrooveWid_Result);
	}
	m_staticGrooveWidthResult.SetColors(m_bMaxWidthOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticGrooveWidthResult.SetText(strTmp);
	//
	if (_T("S") != m_pParamCalculator->m_strECType)
	{
		float fRaiGau_Result = 0.0f;
		int iRailGauage = _ttol(m_pParamCalculator->m_strRaiGauge);
		if (_T("D") == m_pParamCalculator->m_strECType)
			fRaiGau_Result = (float)iRailGauage / (m_pParamCalculator->GetWheelBase() - GetRailMaxGauage());
		else if (_T("B") == m_pParamCalculator->m_strECType)
			fRaiGau_Result = (float)GetRailMaxGauage() / iRailGauage;
		if (fRaiGau_Result <= 1.0f)
		{
			m_bRaiGauageOK = TRUE;
			strTmp.Format(_T("%0.4f <= 1.0f"), fRaiGau_Result);
		}
		else
		{
			m_bRaiGauageOK = FALSE;
			strTmp.Format(_T("%0.4f > 1.0f"), fRaiGau_Result);
		}
		m_staticRailGauResult.SetColors(m_bRaiGauageOK ? COLOR_PASS : COLOR_NOTPASS);
		m_staticRailGauResult.SetText(strTmp);
	}
	//
	float fWheBase_Result = m_pParamCalculator->m_fSpan * 1000 * m_pParamCalculator->m_fParam_ld / m_pParamCalculator->GetWheelBase();
	if (fWheBase_Result <= 1.0f)
	{
		m_bMaxWheBaseOK = TRUE;
		strTmp.Format(_T("%0.4f <= 1.0f"), fWheBase_Result);
	}
	else
	{
		m_bMaxWheBaseOK = FALSE;
		strTmp.Format(_T("%0.4f > 1.0f"), fWheBase_Result);
	}
	m_staticWheelBaseResult.SetColors(m_bMaxWheBaseOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticWheelBaseResult.SetText(strTmp);
	//
	m_bRailTypeOK = m_pParamCalculator->m_bRailTypeIsOK;
	m_staticRailTypeResult.SetColors(m_bRailTypeOK ? COLOR_PASS : COLOR_NOTPASS);
	m_staticRailTypeResult.SetText(m_bRailTypeOK ? _T("OK") : _T("Rail not suitable"));
	//
	CString strBuffExt;
	int iBuffExt = (int)m_pParamCalculator->GetBufferExtend();
	if (iBuffExt <= 0) strBuffExt = _T("No need");
	else strBuffExt.Format(_T("%d"), iBuffExt);
	SetDlgItemText(IDC_EDIT_ENDCAR_BUFFEXT, strBuffExt);
	UpdateBufferType();
	UpdateEndCarDataText();
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
	/* commented by Demo
	SelectedECTypeIsChanged();
	*/
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ECTYPE);
	SelectComboBoxItem(pComboBox, int('S'));
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEccode()
{
	/* commented by Demo
	SelectedECCodeIsChanged();
	*/
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ECCODE);
	SelectComboBoxItem(pComboBox, _T("S2032"));
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarDritype()
{
	/* commented by Demo
	SelectedECDriverIsChanged();
	*/	
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_DRITYPE);
	SelectComboBoxItem(pComboBox, _T("EFB2/FGA423/4D71C-4"));
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboMotorqty()
{
	/* commented by Demo
	SelectedMotorQtyIsChanged();
	*/	
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOTORQTY);
	SelectComboBoxItem(pComboBox, 2);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarRailtype()
{
	/* commented by Demo
	SelectedRailTypeIsChanged();
	*/	
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_RAILTYPE);
	SelectComboBoxItem(pComboBox, _T("P22"));
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
	pComboBox->AddString(_T("P22"));
	pComboBox->AddString(_T("P24"));
	pComboBox->AddString(_T("P30"));
	pComboBox->AddString(_T("P38"));
	pComboBox->AddString(_T("P43"));
	pComboBox->AddString(_T("QU70"));
	pComboBox->AddString(_T("QU80"));
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_strRailType);
	SelectedRailTypeIsChanged();

	int iIndex = 0;
	pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ECTYPE);	
	pComboBox->ResetContent();
	if (m_pParamCalculator->m_eCraneType)
	{
		iIndex = pComboBox->AddString(_T("S-EC"));
		pComboBox->SetItemData(iIndex, int('S'));
	}
	else
	{
		iIndex = pComboBox->AddString(_T("D-EC"));
		pComboBox->SetItemData(iIndex, int('D'));
		iIndex = pComboBox->AddString(_T("B-EC"));
		pComboBox->SetItemData(iIndex, int('B'));
	}
	SelectComboBoxItem(pComboBox, (int)m_pParamCalculator->m_strECType.GetAt(0));
	SelectedECTypeIsChanged();
	//
	FillDriCodeComboBox();
	// 
	pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_MOTORQTY);
	pComboBox->ResetContent();
	iIndex = pComboBox->AddString(_T("2"));
	pComboBox->SetItemData(iIndex, 2);
	iIndex = pComboBox->AddString(_T("4"));
	pComboBox->SetItemData(iIndex, 4);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iNumOfMotor);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_MATRIAL);
	pComboBox->ResetContent();
	iIndex = pComboBox->AddString(_T("Q235"));
	pComboBox->SetItemData(iIndex, 235);
	iIndex = pComboBox->AddString(_T("Q345"));
	pComboBox->SetItemData(iIndex, 345);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iEndCar_Matrial);
	// 
	FillJoiPlaTypeComboBox();
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
	strSQL.Format(_T("SELECT CodEndCar FROM %s WHERE CodEndCar LIKE '%s%s'"), g_tcTableName[eTableID_EndCarSteMat], m_pParamCalculator->m_strECType, _T("%"));
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
			if (_tcsicmp(tcEndCarCode, m_pParamCalculator->m_strEndCraCode) == 0)
			{
				iDefaultSel = dwIndex;
			}
		}

		tbEndCarCode.CloseTable();
	}
	pComboBox->SetCurSel(iDefaultSel);
	SelectedECCodeIsChanged();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::FillDriCodeComboBox()
{
	int iDefaultSel = 0;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_DRITYPE);
	pComboBox->ResetContent();

	CString strSQL;
	CADOTable tbCraneData;
	strSQL.Format(_T("SELECT * FROM %s"), g_tcTableName[eTableID_TroDriDat]);
	if (tbCraneData.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL))
	{
		DWORD dwTotalCnt = tbCraneData.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwTotalCnt; dwIndex++)
		{
			float fPowTraDri = 0.0f;
			TCHAR tcDriverCode[32] = {0};
			tbCraneData.GetRecordCell(dwIndex, _T("TroDriTyp"), tcDriverCode, 32);
			tbCraneData.GetRecordCell(dwIndex, _T("PowTraDri"), fPowTraDri);

			// Only list "EFBxxxxxx" driver
			CString strDriverCode = tcDriverCode;
			strDriverCode.MakeUpper();
			if (strDriverCode.Left(3) != _T("EFB"))
				continue;

			int iIndex = pComboBox->AddString(tcDriverCode);
			pComboBox->SetItemData(iIndex, int(fPowTraDri*100 + 0.5f));
			if (_tcsicmp(tcDriverCode, m_pParamCalculator->m_strDriverCode) == 0)
			{
				iDefaultSel = iIndex;
			}
		}

		tbCraneData.CloseTable();
	}
	pComboBox->SetCurSel(iDefaultSel);
	SelectedECDriverIsChanged();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::FillJoiPlaTypeComboBox()
{
	int iDefaultSel = 0;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_JOINTPLATE);
	pComboBox->ResetContent();

	CString strSQL;
	CADOTable tbJoiPlaType;
	strSQL.Format(_T("SELECT JoiPlaTyp FROM %s"), g_tcTableName[eTableID_JoiPlaTyp]);
	if (tbJoiPlaType.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL))
	{
		DWORD dwTotalCnt = tbJoiPlaType.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwTotalCnt; dwIndex++)
		{
			TCHAR tcJoiPlaType[32] = {0};
			tbJoiPlaType.GetRecordCell(dwIndex, _T("JoiPlaTyp"), tcJoiPlaType, 32);
			pComboBox->AddString(tcJoiPlaType);
			//
			if (_tcsicmp(tcJoiPlaType, GetJoiPlaTypeString()) == 0)
			{
				iDefaultSel = dwIndex;
			}
		}

		tbJoiPlaType.CloseTable();
	}
	pComboBox->SetCurSel(iDefaultSel);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::InitUIWithDefaultData()
{
	//All default data are set in initliaze
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::InitUIWithDatabaseData()
{
	// The default EC code
	int iDefaultMotorCnt;
	CString strDefaultECCode;
	CString strDefaultDriCode;
	CString strDefaultEndCarData;
	GetDefaultEndCarData(strDefaultECCode, strDefaultDriCode, iDefaultMotorCnt);
	strDefaultEndCarData.Format(_T("%s-%s"), strDefaultECCode, strDefaultDriCode);
	SetDlgItemText(IDC_STATIC_ENDCARDATA, strDefaultEndCarData);

	// Steel structure
	CString strSQL;
	CString strSteelStruct;
	strSQL.Format(_T("SELECT * FROM %s WHERE CodEndCar='%s'"), g_tcTableName[eTableID_EndCarSteMat], strDefaultECCode);
	strSteelStruct = GetStringValueFromTable(strSQL, _T("EndStrSec"));
	strSteelStruct = strSteelStruct.IsEmpty() ? m_pParamCalculator->m_strECStructure : strSteelStruct;
	SetDlgItemText(IDC_EDIT_ENDCAR_STEELSTRUCT, strSteelStruct);

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
	CString strCraneTypeCode;
	strCraneTypeCode = (eCraneType_GTD == m_pParamCalculator->m_eCraneType) ? _T("D") : _T("S");
	strOpenOperation.Format(L"SELECT * FROM %s WHERE CraTyp='%s' AND HoiLoa=%d AND CraSpa>=%f ORDER BY CraSpa", g_tcTableName[eTableID_CraneData], strCraneTypeCode, m_pParamCalculator->m_iLoadOfCrane/1000, m_pParamCalculator->m_fSpan);
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
		strECCode = m_pParamCalculator->m_strEndCraCode;
		strDriCode = m_pParamCalculator->m_strDriverCode;
		iMotorCnt = m_pParamCalculator->m_iNumOfMotor;
	}
	tbCranesData.CloseTable();
}
//-----------------------------------------------------------------------
CString	CBMOfferCalcEndCarDlg::GetJoiPlaTypeString()
{
	CString strJoiPlaType = _T("0");
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ECCODE);
	int iSelIndex = pComboBox->GetCurSel();
	int iWheDim = pComboBox->GetItemData(iSelIndex);
	strJoiPlaType = m_pParamCalculator->GetJoiPlaTypeString(iWheDim);
	return strJoiPlaType;
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
			tbEndCarMatPar.GetRecordCell(dwIndex, _T("EcIx"), dwEcIx);
			tbEndCarMatPar.GetRecordCell(dwIndex, _T("EcW"), dwEcW);
			int iIndex = pComboBox->AddString(tcEndCarSec);
			pComboBox->SetItemData(iIndex, MAKELONG(dwEcIx, dwEcW));
			//
			if (_tcsicmp(tcEndCarSec, m_pParamCalculator->m_strECStructure) == 0)
			{
				iDefaultSel = dwIndex;
			}
		}

		tbEndCarMatPar.CloseTable();
	}
	pComboBox->SetCurSel(iDefaultSel);
}
//-----------------------------------------------------------------------
int CBMOfferCalcEndCarDlg::GetRailMaxWidth()
{
	int iWheMaxWidth = 0;
	
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ECCODE);
	int iSelIndex = pComboBox->GetCurSel();
	int iWheDim = pComboBox->GetItemData(iSelIndex);

	CString strWheBloType;
	strWheBloType.Format(_T("%c%d"), m_pParamCalculator->m_strGearCode.GetAt(0), iWheDim);

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE WheBloTyp='%s'"), g_tcTableName[eTableID_WheWidDat], strWheBloType);
	iWheMaxWidth = GetLONGValueFromTable(strSQL, _T("WheWidMax"));

	return iWheMaxWidth;
}
//-----------------------------------------------------------------------
int CBMOfferCalcEndCarDlg::GetRailMaxGauage()
{
	int iMaxGauage = 0;
	
	CString strECCode;
	if (_T("D") == m_pParamCalculator->m_strECType)
		strECCode = m_pParamCalculator->m_strEndCraCode.Left(3);
	else if (_T("B") == m_pParamCalculator->m_strECType)
		strECCode = m_pParamCalculator->m_strEndCraCode;
	else
		return iMaxGauage;

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE EndCarTyp='%s' AND CodConPlt='%s'"), g_tcTableName[eTableID_RaiGauDat], strECCode, GetJoiPlaTypeString());
	iMaxGauage = GetLONGValueFromTable(strSQL, _T("EndCarDat2"));

	return iMaxGauage;
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateEndcarStructure()
{
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_STRUCTURE);
	pComboBox->GetWindowText(m_pParamCalculator->m_strECStructure);
	int iCurSel = pComboBox->GetCurSel();
	DWORD dwItemData = pComboBox->GetItemData(iCurSel);
	m_pParamCalculator->m_iEndCar_EcW = HIWORD(dwItemData);
	m_pParamCalculator->m_iEncCar_EcIx = LOWORD(dwItemData);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateDataBasedOnCodeAndDriver()
{
	// Get Max Capacity and Max Speed
	CString strSQL;
	CString strTmp;
	strSQL.Format(_T("SELECT * FROM %s WHERE CodEndCarStr='%s' AND NumBriDri=%d AND CodBriDri='%s'"), g_tcTableName[eTableID_EndCarPri], m_pParamCalculator->m_strEndCraCode, m_pParamCalculator->m_iNumOfMotor, m_pParamCalculator->m_strDriverCode);	
	m_pParamCalculator->m_fDriverLoad = GetFLOATValueFromTable(strSQL, _T("DriLoa"));
	m_pParamCalculator->m_iMaxSpeed = GetLONGValueFromTable(strSQL, _T("SpeTraMax"));
	strTmp.Format(_T("%0.2f"), m_pParamCalculator->m_fDriverLoad);
	SetDlgItemText(IDC_EDIT_ENDCAR_CAPACITY, strTmp);
	strTmp.Format(_T("%d"), m_pParamCalculator->m_iMaxSpeed);
	SetDlgItemText(IDC_EDIT_ENDCAR_MAXSPE, strTmp);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateWheelBase()
{
	CString strWheelBase;
	strWheelBase.Format(_T("%d"), m_pParamCalculator->GetWheelBase(TRUE));
	SetDlgItemText(IDC_EDIT_ENDCAR_WHEELBASE, strWheelBase);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateBufferType()
{
	CString strBufTyp = m_pParamCalculator->GetBufferType();	
	SetDlgItemText(IDC_EDIT_ENDCAR_BUFFTYPE, strBufTyp);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateEndCarDataText()
{
	CString strJoiPlaType = _T("0");
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ECCODE);
	int iSelIndex = pComboBox->GetCurSel();
	int iWheDim = pComboBox->GetItemData(iSelIndex);

	CString strECText = m_pParamCalculator->GetEndCarDataText(iWheDim);
	SetDlgItemText(IDC_EDIT_ENDCAR_ENDCARRIAGE, strECText);
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdateMotorPrice()
{
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE TroDriTyp='%s'"), g_tcTableName[eTableID_TroDriDat], m_pParamCalculator->m_strDriverCode);	
	m_iMotorPrice = GetFLOATValueFromTable(strSQL, _T("PriTraDriRmb"));
	m_iMotorPrice *= m_pParamCalculator->m_iNumOfMotor;
	//
	CString strPrice;
	strPrice.Format(_T("%d RMB"), m_iMotorPrice);
	m_staticMotorPrice.SetText(strPrice);
	//
	UpdatePrices();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::UpdatePrices()
{
	// Get all additions price
	DWORD dwTotalAddPrice = 0;
	int iSelAddCnt = m_lstCtlAdditions.GetItemCount();
	for (int iIndex = 0; iIndex < iSelAddCnt; iIndex++)
	{
		CString strPrice;
		strPrice = m_lstCtlAdditions.GetItemText(iIndex, 2);
		dwTotalAddPrice += _ttol(strPrice);
	}
	//
	CString strAddsPrice;
	strAddsPrice.Format(_T("%d RMB"), dwTotalAddPrice);
	m_staticAddPrice.SetText(strAddsPrice);
	//
	CString strTotalPrice;
	m_pParamCalculator->m_iEndCarPrice = dwTotalAddPrice + m_iEndCarPrice;
	strTotalPrice.Format(_T("%d RMB"), m_pParamCalculator->m_iEndCarPrice + m_iMotorPrice);
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





//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboJointplate()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_JOINTPLATE);
	SelectComboBoxItem(pComboBox, _T("L4"));
}

void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarMatrial()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_MATRIAL);
	SelectComboBoxItem(pComboBox, 235);
}

void CBMOfferCalcEndCarDlg::OnCbnSelchangeComboEndcarStructure()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ENDCAR_STRUCTURE);
	SelectComboBoxItem(pComboBox, _T("350x200x8"));
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::SelectedECTypeIsChanged()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ECTYPE);
	int iSelIndex = pComboBox->GetCurSel();
	int iItemData = pComboBox->GetItemData(iSelIndex);
	m_pParamCalculator->m_strECType = (TCHAR)iItemData;
	if (m_pParamCalculator->m_strECType == _T("D"))
		SetDlgItemText(IDC_STATIC_EC_RAILRES, _T("R/Rmax="));
	else if (m_pParamCalculator->m_strECType == _T("B"))
		SetDlgItemText(IDC_STATIC_EC_RAILRES, _T("Rmin/R="));
	//
	FillECCodeComboBox();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::SelectedECCodeIsChanged()
{
	CString strTmp;

	// Get EndCar weigth from database at first
	CString strSQL;
	GetDlgItemText(IDC_COMBO_ECCODE, m_pParamCalculator->m_strEndCraCode);	
	strSQL.Format(_T("SELECT * FROM %s WHERE EndCarTyp='%s'"), g_tcTableName[eTableID_EndCarWei], m_pParamCalculator->m_strEndCraCode);	
	m_pParamCalculator->m_iEndCar_Wgt = GetLONGValueFromTable(strSQL, _T("ECWei"));
	//
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_JOINTPLATE);
	SelectComboBoxItem(pComboBox, GetJoiPlaTypeString());
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE CodEndCar='%s'"), g_tcTableName[eTableID_EndCarSteMat], m_pParamCalculator->m_strEndCraCode);
	CString strECStructure = GetStringValueFromTable(strSQL, _T("EndStrSec"));
	pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ENDCAR_STRUCTURE);
	SelectComboBoxItem(pComboBox, strECStructure);
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE CodEndCarStr='%s'"), g_tcTableName[eTableID_EndCarStePri], m_pParamCalculator->m_strEndCraCode);	
	m_iEndCarPrice = GetLONGValueFromTable(strSQL, _T("PriEndCarStr"));
	strTmp.Format(_T("%d RMB"), m_iEndCarPrice);
	m_staticEndCarPrice.SetText(strTmp);
	UpdatePrices();
	//
	UpdateDataBasedOnCodeAndDriver();
	UpdateWheelBase();
	UpdateBufferType();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::SelectedECDriverIsChanged()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_ENDCAR_DRITYPE);
	int iSelIndex = pComboBox->GetCurSel();
	int iItemData = pComboBox->GetItemData(iSelIndex);
	m_pParamCalculator->m_fEndCar_DriPow = iItemData / 100.0f;	
	pComboBox->GetWindowText(m_pParamCalculator->m_strDriverCode);
	//
	// Update gear code
	m_pParamCalculator->m_strGearCode = m_pParamCalculator->GetGearCodeFromDriver(m_pParamCalculator->m_strDriverCode);
	//
	UpdateDataBasedOnCodeAndDriver();
	UpdateMotorPrice();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::SelectedMotorQtyIsChanged()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_MOTORQTY);
	int iSelIndex = pComboBox->GetCurSel();
	int iItemData = pComboBox->GetItemData(iSelIndex);
	m_pParamCalculator->m_iNumOfMotor = iItemData;	
	//
	UpdateDataBasedOnCodeAndDriver();
	UpdateMotorPrice();
}
//-----------------------------------------------------------------------
void CBMOfferCalcEndCarDlg::SelectedRailTypeIsChanged()
{
	GetDlgItemText(IDC_COMBO_ENDCAR_RAILTYPE, m_pParamCalculator->m_strRailType);
	
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE RaiTyp='%s'"), g_tcTableName[eTableID_DatRai], m_pParamCalculator->m_strRailType);
	m_pParamCalculator->m_iRaiWidTop = (int)GetFLOATValueFromTable(strSQL, _T("RaiWidTop"));
	
	// Update groove width
	CString strGrooveWidth;
	strGrooveWidth.Format(_T("%d"), m_pParamCalculator->m_iRaiWidTop + 20);
	SetDlgItemText(IDC_EDIT_ENDCAR_GROOVEWIDTH, strGrooveWidth);
}
//-----------------------------------------------------------------------
