//-----------------------------------------------------------------------
//	File Name:BMOfferCalcMainCarDlg.cpp
//	Introduce:implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcMainCarDlg.h"
#include "BMCalcParameterDlg.h"
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CBMOfferCalcMainCarDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferCalcMainCarDlg, CDialog)
CBMOfferCalcMainCarDlg::CBMOfferCalcMainCarDlg(CWnd* pParent /*=NULL*/)
	: CBMCalcOptionsDlg(CBMOfferCalcMainCarDlg::IDD, pParent)
{
	m_bShowResultImages = FALSE;
	m_bResultQ0_Pass = FALSE;
	m_bResultQ1_Pass = FALSE;
	m_bResultQ2_Pass = FALSE;
	m_bResultT_Pass = FALSE;
	m_bResultF_Pass = FALSE;
	m_bResultS_Pass = FALSE;

	for (int i = 0; i < eCraneType_Cnt; i++)
	{
		m_pGiederImages[i] = NULL;
		m_pSideImages[i] = NULL;
	}
	for (int i = 0; i < eCalcResultType_Cnt; i++)
	{
		m_pResultImages[i] = NULL;
	}
}
//-----------------------------------------------------------------------
CBMOfferCalcMainCarDlg::~CBMOfferCalcMainCarDlg()
{
	DestroyWindow();
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GIRDER_Q0, m_Static_Q0);
	DDX_Control(pDX, IDC_STATIC_GIRDER_Q1, m_Static_Q1);
	DDX_Control(pDX, IDC_STATIC_GIRDER_Q2, m_Static_Q2);
	DDX_Control(pDX, IDC_STATIC_GIRDER_T, m_Static_T);
	DDX_Control(pDX, IDC_STATIC_GIRDER_F, m_Static_F);
	DDX_Control(pDX, IDC_STATIC_GIRDER_S, m_Static_S);
	DDX_Control(pDX, IDC_STATIC_GIRDER_Q0_RES, m_Static_Q0_RES);
	DDX_Control(pDX, IDC_STATIC_GIRDER_Q1_RES, m_Static_Q1_RES);
	DDX_Control(pDX, IDC_STATIC_GIRDER_Q2_RES, m_Static_Q2_RES);
	DDX_Control(pDX, IDC_STATIC_GIRDER_T_RES, m_Static_T_RES);
	DDX_Control(pDX, IDC_STATIC_GIRDER_F_RES, m_Static_F_RES);
	DDX_Control(pDX, IDC_STATIC_GIRDER_S_RES, m_Static_S_RES);
	DDX_Control(pDX, IDC_STATIC_CRANEPRICE, m_Static_TotalPrice);	
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferCalcMainCarDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SELECT, &CBMOfferCalcMainCarDlg::OnBnClickedBtnSelect)
	ON_EN_CHANGE(IDC_EDIT_H2, &CBMOfferCalcMainCarDlg::OnEnChangeEditWebendheight)
	ON_BN_CLICKED(IDC_BTN_CALC, &CBMOfferCalcMainCarDlg::OnBnClickedBtnCalc)
	ON_BN_CLICKED(IDC_BTN_PARAM, &CBMOfferCalcMainCarDlg::OnBnClickedBtnParam)
	ON_EN_CHANGE(IDC_EDIT_COSTT, &CBMOfferCalcMainCarDlg::OnEnChangeEditCostt)
	ON_CBN_SELCHANGE(IDC_COMBO_GIRDER_T3, &CBMOfferCalcMainCarDlg::OnCbnSelchangeComboGirderT3)
	ON_EN_CHANGE(IDC_EDIT_H1, &CBMOfferCalcMainCarDlg::OnEnChangeEditH1)
	ON_CBN_SELCHANGE(IDC_COMBO_GIRDER_W1, &CBMOfferCalcMainCarDlg::OnCbnSelchangeComboGirderW1)
	ON_CBN_SELCHANGE(IDC_COMBO_GIRDER_T1, &CBMOfferCalcMainCarDlg::OnCbnSelchangeComboGirderT1)
	ON_CBN_SELCHANGE(IDC_COMBO_MATERIAL, &CBMOfferCalcMainCarDlg::OnCbnSelchangeComboMaterial)
	ON_CBN_SELCHANGE(IDC_COMBO_RAILSTYLE, &CBMOfferCalcMainCarDlg::OnCbnSelchangeComboRailstyle)
	ON_CBN_SELCHANGE(IDC_COMBO_W3, &CBMOfferCalcMainCarDlg::OnCbnSelchangeComboW3)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CBMOfferCalcMainCarDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferCalcMainCarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_rcGiederImage.left = 20;							m_rcGiederImage.top = 240;
	m_rcGiederImage.right = m_rcGiederImage.left + 180; m_rcGiederImage.bottom = m_rcGiederImage.top + 230;
	m_rcSideImage.left = 210;							m_rcSideImage.top = 240;
	m_rcSideImage.right = m_rcSideImage.left + 180;		m_rcSideImage.bottom = m_rcSideImage.top + 230;
	//
	const int iImgX = 24;
	const int iImgY = 24;
	m_rcImageDelta0.left = 664;							m_rcImageDelta0.top = 28;
	m_rcImageDelta0.right = m_rcImageDelta0.left + iImgX;m_rcImageDelta0.bottom = m_rcImageDelta0.top + iImgY;
	m_rcImageDelta1.left = 664;							m_rcImageDelta1.top = 60;
	m_rcImageDelta1.right = m_rcImageDelta1.left + iImgX;m_rcImageDelta1.bottom = m_rcImageDelta1.top + iImgY;
	m_rcImageDelta2.left = 664;							m_rcImageDelta2.top = 90;
	m_rcImageDelta2.right = m_rcImageDelta2.left + iImgX;m_rcImageDelta2.bottom = m_rcImageDelta2.top + iImgY;
	m_rcImageTuo.left = 664;							m_rcImageTuo.top = 120;
	m_rcImageTuo.right = m_rcImageTuo.left + iImgX;		m_rcImageTuo.bottom = m_rcImageTuo.top + iImgY;
	m_rcImageF.left = 664;								m_rcImageF.top = 152;
	m_rcImageF.right = m_rcImageF.left + iImgX;			m_rcImageF.bottom = m_rcImageF.top + iImgY;
	m_rcImageS.left = 664;								m_rcImageS.top = 184;
	m_rcImageS.right = m_rcImageS.left + iImgX;			m_rcImageS.bottom = m_rcImageS.top + iImgY;
	//
	for (int i = 0; i < eCraneType_Cnt; i++)
	{
		m_pGiederImages[i] = new CImage();
		m_pSideImages[i] = new CImage();
	}
	m_pGiederImages[eCraneType_GTD]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_GIEDER_D);
	m_pGiederImages[eCraneType_GTS]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_GIEDER_S);
	m_pSideImages[eCraneType_GTD]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_SIDE_D);
	m_pSideImages[eCraneType_GTS]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_SIDE_S);
	//
	m_pResultImages[eCalcResultType_Pass] = new CImage();
	m_pResultImages[eCalcResultType_Pass]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_CALCPASS);
	m_pResultImages[eCalcResultType_NotPass] = new CImage();
	m_pResultImages[eCalcResultType_NotPass]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_CALCNOTPASS);

	FillComboBoxs();
	m_Static_TotalPrice.SetColors(COLOR_PRICE);
	
	// Set initliaze data to UI
	if (FALSE == m_bDataIsInitliazed)
	{
		/* commented by Demo
		if (m_pParamCalculator->m_bIsEditData)
		{
			InitUIWithDatabaseData();
			OnBnClickedBtnCalc();
		}
		else*/
		{
			InitUIWithDefaultData();
		}
		m_bDataIsInitliazed = TRUE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	if (NULL != m_pGiederImages[m_pParamCalculator->m_eCraneType])
	{
		m_pGiederImages[m_pParamCalculator->m_eCraneType]->StretchBlt(	dc.m_hDC,	
																		m_rcGiederImage.left, 
																		m_rcGiederImage.top, 
																		m_rcGiederImage.Width(), 
																		m_rcGiederImage.Height());
	}
	if (NULL != m_pSideImages[m_pParamCalculator->m_eCraneType])
	{
		m_pSideImages[m_pParamCalculator->m_eCraneType]->StretchBlt(	dc.m_hDC,	
																		m_rcSideImage.left, 
																		m_rcSideImage.top, 
																		m_rcSideImage.Width(), 
																		m_rcSideImage.Height());
	}

	if (TRUE == m_bShowResultImages)
	{
		CImage* pImage = NULL;
		pImage = m_bResultQ0_Pass ? m_pResultImages[eCalcResultType_Pass] : m_pResultImages[eCalcResultType_NotPass];
		pImage->StretchBlt(	dc.m_hDC, m_rcImageDelta0.left, m_rcImageDelta0.top, m_rcImageDelta0.Width(), m_rcImageDelta0.Height());
		//
		pImage = m_bResultQ1_Pass ? m_pResultImages[eCalcResultType_Pass] : m_pResultImages[eCalcResultType_NotPass];
		pImage->StretchBlt(	dc.m_hDC, m_rcImageDelta1.left, m_rcImageDelta1.top, m_rcImageDelta1.Width(), m_rcImageDelta1.Height());
		//
		pImage = m_bResultQ2_Pass ? m_pResultImages[eCalcResultType_Pass] : m_pResultImages[eCalcResultType_NotPass];
		pImage->StretchBlt(	dc.m_hDC, m_rcImageDelta2.left,m_rcImageDelta2.top, m_rcImageDelta2.Width(), m_rcImageDelta2.Height());
		//
		pImage = m_bResultT_Pass ? m_pResultImages[eCalcResultType_Pass] : m_pResultImages[eCalcResultType_NotPass];
		pImage->StretchBlt(	dc.m_hDC, m_rcImageTuo.left, m_rcImageTuo.top, m_rcImageTuo.Width(), m_rcImageTuo.Height());
		//
		pImage = m_bResultF_Pass ? m_pResultImages[eCalcResultType_Pass] : m_pResultImages[eCalcResultType_NotPass];
		pImage->StretchBlt(	dc.m_hDC, m_rcImageF.left, m_rcImageF.top, m_rcImageF.Width(), m_rcImageF.Height());
		//
		pImage = m_bResultS_Pass ? m_pResultImages[eCalcResultType_Pass] : m_pResultImages[eCalcResultType_NotPass];
		pImage->StretchBlt(	dc.m_hDC, m_rcImageS.left, m_rcImageS.top,m_rcImageS.Width(), m_rcImageS.Height());
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::OnDestroy()
{
	CDialog::OnDestroy();

	for (int i = 0; i < eCraneType_Cnt; i++)
	{
		if (NULL != m_pGiederImages[i])
		{
			m_pGiederImages[i]->Destroy();
			delete m_pGiederImages[i];
			m_pGiederImages[i] = NULL;
		}
		if (NULL != m_pSideImages[i])
		{
			m_pSideImages[i]->Destroy();
			delete m_pSideImages[i];
			m_pSideImages[i] = NULL;
		}
	}
	for (int i = 0; i < eCalcResultType_Cnt; i++)
	{
		if (NULL != m_pResultImages[i])
		{
			m_pResultImages[i]->Destroy();
			delete m_pResultImages[i];
			m_pResultImages[i] = NULL;
		}
	}
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::OnBnClickedBtnSelect()
{	
	CString strCraneTypeCode;
	if (eCraneType_GTD == m_pParamCalculator->m_eCraneType)
		strCraneTypeCode = _T("D");
	else
		strCraneTypeCode = _T("S");

	BOOL bResult = TRUE;
	CADOTable tbCranesData;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE CraTyp='%s' AND HoiLoa=%d AND CraSpa>=%f ORDER BY CraSpa", g_tcTableName[eTableID_CraneData], strCraneTypeCode, m_pParamCalculator->m_iLoadOfCrane/1000, m_pParamCalculator->m_fSpan);
	bResult = tbCranesData.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult || tbCranesData.GetRecordCnt() == 0)
	{
		myMessageBox(IDS_MAINCARDLG_NOGIRDER, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		return;
	}

	TCHAR tcCraneCode[64] = {0};
	TCHAR tcMainGirder[64] = {0};	
	TCHAR tcEndCarCode[64] = {0};
	TCHAR tcDriverCode[64] = {0};
	TCHAR tcGearCode[64] = {0};
	TCHAR tcMotroCode[64] = {0};
	tbCranesData.GetRecordCell(0, L"CraCod", tcCraneCode, 64);
	tbCranesData.GetRecordCell(0, L"MaiGirDat", tcMainGirder, 64);
	tbCranesData.GetRecordCell(0, L"CodEndCar", tcEndCarCode, 64);
	tbCranesData.GetRecordCell(0, L"CodBriDri", tcDriverCode, 64);
	tbCranesData.GetRecordCell(0, L"Gear", tcGearCode, 64);
	tbCranesData.GetRecordCell(0, L"Motor", tcMotroCode, 64);
	tbCranesData.GetRecordCell(0, L"NumBriDri", (DWORD&)m_pParamCalculator->m_iNumOfMotor);

	m_pParamCalculator->m_strCraneCode = tcCraneCode;
	m_pParamCalculator->m_strMainGirderCode = tcMainGirder;
	m_pParamCalculator->m_strEndCraCode = tcEndCarCode;
	m_pParamCalculator->m_strECType = m_pParamCalculator->m_strEndCraCode.Left(1);
	m_pParamCalculator->m_strDriverCode = tcDriverCode;
	m_pParamCalculator->m_strGearCode = tcGearCode;
	m_pParamCalculator->m_strMotorCode = tcMotroCode;
	m_pParamCalculator->UpdateMainGirderData(m_pParamCalculator->m_strMainGirderCode);
	//
	CString strTmp;
	SetDlgItemText(IDC_STATIC_RESULT, m_pParamCalculator->m_strMainGirderCode);
	strTmp.Format(_T("%d"), m_pParamCalculator->m_iGirder_h1);
	SetDlgItemText(IDC_EDIT_H1, strTmp);
	strTmp.Format(_T("%d"), m_pParamCalculator->m_iGirder_t2);
	SetDlgItemText(IDC_EDIT_TT2, strTmp);
	strTmp.Format(_T("%d"), m_pParamCalculator->m_iGirder_t4);
	SetDlgItemText(IDC_EDIT_T4, strTmp);
	strTmp.Format(_T("%d"), m_pParamCalculator->GetGirder_c1());
	SetDlgItemText(IDC_EDIT_C1, strTmp);
	strTmp.Format(_T("%d"), m_pParamCalculator->m_iGirder_t5);
	SetDlgItemText(IDC_EDIT_T5, strTmp);
	strTmp.Format(_T("%d"), m_pParamCalculator->GetGirder_h2());
	SetDlgItemText(IDC_EDIT_H2, strTmp);	
	//	
	SelectComboBoxItem((CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_W1), m_pParamCalculator->m_iGirder_w1);
	SelectComboBoxItem((CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T1), m_pParamCalculator->m_iGirder_t1);
	SelectComboBoxItem((CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T3), m_pParamCalculator->m_iGirder_t3);
	//
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAILSTYLE);
	int iSelIndex = pComboBox->GetCurSel();		
	m_pParamCalculator->m_iGirder_Rw = LOWORD(pComboBox->GetItemData(iSelIndex));
	m_pParamCalculator->m_iGirder_Rh = HIWORD(pComboBox->GetItemData(iSelIndex));

	//tbCranesData.CloseTable();
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::OnEnChangeEditH1()
{
	CString strTmp;
	GetDlgItemText(IDC_EDIT_H1, strTmp);
	m_pParamCalculator->m_iGirder_h1 = _ttol(strTmp);
	m_pParamCalculator->m_iGirder_h2 = m_pParamCalculator->GetGirder_h2(TRUE);
	//
	CString strH2;
	strH2.Format(_T("%d"), m_pParamCalculator->m_iGirder_h2);
	SetDlgItemText(IDC_EDIT_H2, strH2);
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::OnCbnSelchangeComboGirderT3()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T3);
	SelectComboBoxItem(pComboBox, 6);
	/* commented by Demo
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T3);
	int iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_iGirder_t3 = pComboBox->GetItemData(iSelIndex);
	m_pParamCalculator->m_iGirder_t5 = m_pParamCalculator->m_iGirder_t3;
	//
	CString strT5;
	strT5.Format(_T("%d"), m_pParamCalculator->m_iGirder_t5);
	SetDlgItemText(IDC_EDIT_T5, strT5);*/
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::OnEnChangeEditWebendheight()
{
	int iGrider_h2 = 0;
	CString strGrider_h2;
	GetDlgItemText(IDC_EDIT_H2, strGrider_h2);
	iGrider_h2 = _ttol(strGrider_h2);
	if (iGrider_h2 <= 0 || iGrider_h2 > m_pParamCalculator->m_iGirder_h1)
	{
		myMessageBox(IDS_CALC_GIRDERH2ISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_H2)->SetFocus();		
		return;
	}

	if (eCraneType_GTS == m_pParamCalculator->m_eCraneType)
	{
		m_pSideImages[eCraneType_GTS]->Destroy();
		if (iGrider_h2 == m_pParamCalculator->m_iGirder_h1)
			m_pSideImages[eCraneType_GTS]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_SIDE_S);
		else
			m_pSideImages[eCraneType_GTS]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_SIDE_D);
		InvalidateRect(&m_rcSideImage);
	}

	m_pParamCalculator->m_iGirder_h2 = iGrider_h2;
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::OnBnClickedBtnCalc()
{
	/*	Get current girder size from UI */
	UpdateUserSelection();

	// Calculation
	BOOL bCalcRes = m_pParamCalculator->CalcMainGirder();
	if (FALSE == bCalcRes)
	{
		myMessageBox(IDS_CALC_CALCGIRDERISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		return;
	}

	float fResult;
	CString strTmp1;
	CString strTmp2;

	// Show calculation results
	fResult = (float)(m_pParamCalculator->GetMainGirder_Q0() / m_pParamCalculator->m_fGirder_StdQ);
	strTmp1.Format(_T("%0.4f"), fResult);
	if (fResult < 1.0f)
	{
		strTmp2.Format(_T("%0.4f < 1"), fResult);
		m_Static_Q0.SetColors(COLOR_PASS, RGB(0, 0, 0));
		m_Static_Q0_RES.SetColors(COLOR_PASS, RGB(0, 0, 0));
		m_bResultQ0_Pass = TRUE;
	}
	else
	{
		strTmp2.Format(_T("%0.4f > 1"), fResult);
		m_Static_Q0.SetColors(COLOR_NOTPASS, RGB(0, 0, 0));
		m_Static_Q0_RES.SetColors(COLOR_NOTPASS, RGB(0, 0, 0));
		m_bResultQ0_Pass = FALSE;
	}
	m_Static_Q0.SetText(strTmp1);
	m_Static_Q0_RES.SetText(strTmp2);
	//
	fResult = (float)(m_pParamCalculator->GetMainGirder_Q1() / m_pParamCalculator->m_fGirder_StdQ);
	strTmp1.Format(_T("%0.4f"), fResult);
	if (fResult < 1.0f)
	{
		strTmp2.Format(_T("%0.4f < 1"), fResult);
		m_Static_Q1.SetColors(COLOR_PASS, RGB(0, 0, 0));
		m_Static_Q1_RES.SetColors(COLOR_PASS, RGB(0, 0, 0));
		m_bResultQ1_Pass = TRUE;
	}
	else
	{
		strTmp2.Format(_T("%0.4f > 1"), fResult);
		m_Static_Q1.SetColors(COLOR_NOTPASS, RGB(0, 0, 0));
		m_Static_Q1_RES.SetColors(COLOR_NOTPASS, RGB(0, 0, 0));
		m_bResultQ1_Pass = FALSE;
	}
	m_Static_Q1.SetText(strTmp1);
	m_Static_Q1_RES.SetText(strTmp2);
	//
	fResult = (float)(m_pParamCalculator->GetMainGirder_Q2() / m_pParamCalculator->m_fGirder_StdQ);
	strTmp1.Format(_T("%0.4f"), fResult);
	if (fResult < 1.0f)
	{
		strTmp2.Format(_T("%0.4f < 1"), fResult);
		m_Static_Q2.SetColors(COLOR_PASS, RGB(0, 0, 0));
		m_Static_Q2_RES.SetColors(COLOR_PASS, RGB(0, 0, 0));
		m_bResultQ2_Pass = TRUE;
	}
	else
	{
		strTmp2.Format(_T("%0.4f > 1"), fResult);
		m_Static_Q2.SetColors(COLOR_NOTPASS, RGB(0, 0, 0));
		m_Static_Q2_RES.SetColors(COLOR_NOTPASS, RGB(0, 0, 0));
		m_bResultQ2_Pass = FALSE;
	}
	m_Static_Q2.SetText(strTmp1);
	m_Static_Q2_RES.SetText(strTmp2);
	//
	fResult = (float)(m_pParamCalculator->GetMainGirder_T() / m_pParamCalculator->m_fGirder_StdT0);
	strTmp1.Format(_T("%0.4f"), fResult);
	if (fResult < 1.0f)
	{
		strTmp2.Format(_T("%0.4f < 1"), fResult);
		m_Static_T.SetColors(COLOR_PASS, RGB(0, 0, 0));
		m_Static_T_RES.SetColors(COLOR_PASS, RGB(0, 0, 0));
		m_bResultT_Pass = TRUE;
	}
	else
	{
		strTmp2.Format(_T("%0.4f > 1"), fResult);
		m_Static_T.SetColors(COLOR_NOTPASS, RGB(0, 0, 0));
		m_Static_T_RES.SetColors(COLOR_NOTPASS, RGB(0, 0, 0));
		m_bResultT_Pass = FALSE;
	}
	m_Static_T.SetText(strTmp1);
	m_Static_T_RES.SetText(strTmp2);
	//
	fResult = (float)(m_pParamCalculator->GetMainGirder_F() / m_pParamCalculator->m_fGirder_Stdfmax);
	strTmp1.Format(_T("%0.4f"), fResult);
	if (fResult < 1.0f)
	{
		strTmp2.Format(_T("%0.4f < 1"), fResult);
		m_Static_F.SetColors(COLOR_PASS, RGB(0, 0, 0));
		m_Static_F_RES.SetColors(COLOR_PASS, RGB(0, 0, 0));
		m_bResultF_Pass = TRUE;
	}
	else
	{
		strTmp2.Format(_T("%0.4f > 1"), fResult);
		m_Static_F.SetColors(COLOR_NOTPASS, RGB(0, 0, 0));
		m_Static_F_RES.SetColors(COLOR_NOTPASS, RGB(0, 0, 0));
		m_bResultF_Pass = FALSE;
	}
	m_Static_F.SetText(strTmp1);
	m_Static_F_RES.SetText(strTmp2);
	//
	fResult = (float)(m_pParamCalculator->GetMainGirder_S() / m_pParamCalculator->m_fGirder_Stdsmax);
	strTmp1.Format(_T("%0.4f"), fResult);
	if (fResult < 1.0f)
	{
		strTmp2.Format(_T("%0.4f < 1"), fResult);
		m_Static_S.SetColors(COLOR_PASS, RGB(0, 0, 0));
		m_Static_S_RES.SetColors(COLOR_PASS, RGB(0, 0, 0));
		m_bResultS_Pass = TRUE;
	}
	else
	{
		strTmp2.Format(_T("%0.4f > 1"), fResult);
		m_Static_S.SetColors(COLOR_NOTPASS, RGB(0, 0, 0));
		m_Static_S_RES.SetColors(COLOR_NOTPASS, RGB(0, 0, 0));
		m_bResultS_Pass = FALSE;
	}
	m_Static_S.SetText(strTmp1);
	m_Static_S_RES.SetText(strTmp2);

	// The new main girder data code
	CString strTmp;
	strTmp.Format(_T("%d/%d/%d/%d/%d/%d"), m_pParamCalculator->m_iGirder_w1, m_pParamCalculator->m_iGirder_h1, m_pParamCalculator->m_iGirder_t1, m_pParamCalculator->m_iGirder_t2, m_pParamCalculator->m_iGirder_t3, m_pParamCalculator->m_iGirder_t4);
	m_pParamCalculator->m_strMainGirderCode = strTmp;
	SetDlgItemText(IDC_EDIT_SS_GIRDERDATA, m_pParamCalculator->m_strMainGirderCode);
	//
	strTmp.Format(_T("%d"), (int)m_pParamCalculator->m_fGirder_ms);
	SetDlgItemText(IDC_EDIT_SS_GIRDERWEIGHT, strTmp);
	//
	strTmp.Format(_T("%d"), (int)m_pParamCalculator->m_fGirder_mz);
	SetDlgItemText(IDC_EDIT_SS_PLATWEIGHT, strTmp);
	//
	strTmp.Format(_T("%d"), (int)m_pParamCalculator->m_fGirder_mr);
	SetDlgItemText(IDC_EDIT_SS_RAILWEIGHT, strTmp);
	//
	strTmp.Format(_T("%d"), (int) m_pParamCalculator->m_fGirder_mt);
	SetDlgItemText(IDC_EDIT_SS_TOTALWEIGHT, strTmp);	
	//
	SetDlgItemText(IDC_EDIT_SS_RIBSTYPE, m_pParamCalculator->m_sParam_FText);	
	//	
	strTmp.Format(_T("%d"), (int) m_pParamCalculator->m_fParam_H);
	SetDlgItemText(IDC_EDIT_SS_RIBSQTY, strTmp);	
	//
	SetDlgItemText(IDC_EDIT_SS_RIBSPOS, m_pParamCalculator->m_strParam_HText);	
	//
	m_pParamCalculator->m_fGirder_mt = 4557;
	UpdateTotalPrice();
	
	m_bShowResultImages = TRUE;
	InvalidateRect(&m_rcImageDelta0);
	InvalidateRect(&m_rcImageDelta1);
	InvalidateRect(&m_rcImageDelta2);
	InvalidateRect(&m_rcImageTuo);
	InvalidateRect(&m_rcImageF);
	InvalidateRect(&m_rcImageS);	
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::OnBnClickedBtnParam()
{
	CBMCalcParameterDlg dlgParam;
	dlgParam.DoModal();
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::OnEnChangeEditCostt()
{
	UpdateTotalPrice();
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::FillComboBoxs()
{
	//Fill (w1) combobox;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_W1);
	int iIndex = pComboBox->AddString(_T("300"));
	pComboBox->SetItemData(iIndex, 300);
	iIndex = pComboBox->AddString(_T("410"));
	pComboBox->SetItemData(iIndex, 410);
	iIndex = pComboBox->AddString(_T("490"));
	pComboBox->SetItemData(iIndex, 490);
	iIndex = pComboBox->AddString(_T("610"));
	pComboBox->SetItemData(iIndex, 610);
	iIndex = pComboBox->AddString(_T("740"));
	pComboBox->SetItemData(iIndex, 740);
	iIndex = pComboBox->AddString(_T("860"));
	pComboBox->SetItemData(iIndex, 860);
	SelectComboBoxItem(pComboBox, 2);

	// Fill (t1) combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T1);
	iIndex = pComboBox->AddString(_T("6"));
	pComboBox->SetItemData(iIndex, 6);
	iIndex = pComboBox->AddString(_T("8"));
	pComboBox->SetItemData(iIndex, 8);
	iIndex = pComboBox->AddString(_T("10"));
	pComboBox->SetItemData(iIndex, 10);
	iIndex = pComboBox->AddString(_T("12"));
	pComboBox->SetItemData(iIndex, 12);
	SelectComboBoxItem(pComboBox, 0);

	// Fill (t3) combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T3);
	iIndex = pComboBox->AddString(_T("6"));
	pComboBox->SetItemData(iIndex, 6);
	iIndex = pComboBox->AddString(_T("8"));
	pComboBox->SetItemData(iIndex, 8);
	iIndex = pComboBox->AddString(_T("10"));
	pComboBox->SetItemData(iIndex, 10);
	iIndex = pComboBox->AddString(_T("12"));
	pComboBox->SetItemData(iIndex, 12);
	SelectComboBoxItem(pComboBox, 2);

	//Fill matril combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MATERIAL);
	iIndex = pComboBox->AddString(_T("Q235"));
	pComboBox->SetItemData(iIndex, 235);
	iIndex = pComboBox->AddString(_T("Q345"));
	pComboBox->SetItemData(iIndex, 345);
	pComboBox->SetCurSel(0);

	//Fill trolley rail combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAILSTYLE);
	iIndex = pComboBox->AddString(_T("50x30"));
	pComboBox->SetItemData(iIndex, MAKELONG(50, 30));
	iIndex = pComboBox->AddString(_T("60x40"));
	pComboBox->SetItemData(iIndex, MAKELONG(60, 40));
	iIndex = pComboBox->AddString(_T("70x40"));
	pComboBox->SetItemData(iIndex, MAKELONG(70, 40));
	iIndex = pComboBox->AddString(_T("80x60"));
	pComboBox->SetItemData(iIndex, MAKELONG(80, 60));
	pComboBox->SetCurSel(0);

	//Fill platform width combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_W3);
	iIndex = pComboBox->AddString(_T("500"));
	pComboBox->SetItemData(iIndex, 500);
	iIndex = pComboBox->AddString(_T("750"));
	pComboBox->SetItemData(iIndex, 750);
	pComboBox->SetCurSel(1);
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::InitUIWithDefaultData()
{
	//Fill (w1) combobox;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_W1);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iGirder_w1);

	// Fill (t1) combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T1);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iGirder_t1);

	// Fill (t3) combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T3);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iGirder_t3);

/*
	//Fill matril combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MATERIAL);
	pComboBox->SetCurSel(0);

	//Fill trolley rail combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAILSTYLE);
	pComboBox->SetCurSel(0);

	//Fill platform width combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_W3);
	pComboBox->SetCurSel(1);
*/
	//
	SetDlgItemText(IDC_EDIT_COSTT, _T("8000"));
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::InitUIWithDatabaseData()
{
	// Set default girder data
	SetDlgItemText(IDC_STATIC_RESULT, m_pParamCalculator->m_strMainGirderCode);

	//Fill (w1) combobox;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_W1);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iGirder_w1);

	// t1
	CString strTmp;
	strTmp.Format(_T("%d"), m_pParamCalculator->m_iGirder_h1);
	SetDlgItemText(IDC_EDIT_H1, strTmp);

	// Fill (t1) combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T1);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iGirder_t1);

	// t2
	strTmp.Format(_T("%d"), m_pParamCalculator->m_iGirder_t2);
	SetDlgItemText(IDC_EDIT_TT2, strTmp);

	// Fill (t3) combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T3);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iGirder_t3);

	// t4
	strTmp.Format(_T("%d"), m_pParamCalculator->m_iGirder_t4);
	SetDlgItemText(IDC_EDIT_T4, strTmp);

	//Fill matril combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MATERIAL);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iGirder_m);

	//Fill trolley rail combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAILSTYLE);
	SelectComboBoxItem(pComboBox, MAKELONG(m_pParamCalculator->m_iGirder_Rw,m_pParamCalculator->m_iGirder_Rh));

	//Fill platform width combobox
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_W3);
	SelectComboBoxItem(pComboBox, m_pParamCalculator->m_iGirder_w3);

	// c1
	strTmp.Format(_T("%d"), m_pParamCalculator->GetGirder_c1());
	SetDlgItemText(IDC_EDIT_C1, strTmp);

	// t5
	strTmp.Format(_T("%d"), m_pParamCalculator->m_iGirder_t5);
	SetDlgItemText(IDC_EDIT_T5, strTmp);

	// h2
	strTmp.Format(_T("%d"), m_pParamCalculator->GetGirder_h2());
	SetDlgItemText(IDC_EDIT_H2, strTmp);

	strTmp.Format(_T("%d"), m_pParamCalculator->m_iMainGirderPrice);
	SetDlgItemText(IDC_EDIT_COSTT, strTmp);
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::UpdateUserSelection()
{
	CString strTmp;
	GetDlgItemText(IDC_EDIT_H1, strTmp);
	m_pParamCalculator->m_iGirder_h1 = _ttol(strTmp);
	//
	GetDlgItemText(IDC_EDIT_TT2, strTmp);
	m_pParamCalculator->m_iGirder_t2 = _ttol(strTmp);
	//
	GetDlgItemText(IDC_EDIT_T4, strTmp);
	m_pParamCalculator->m_iGirder_t4 = _ttol(strTmp);
	//
	GetDlgItemText(IDC_EDIT_C1, strTmp);
	m_pParamCalculator->m_iGirder_c1 = _ttol(strTmp);
	//
	GetDlgItemText(IDC_EDIT_T5, strTmp);
	m_pParamCalculator->m_iGirder_t5 = _ttol(strTmp);
	//
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_W1);
	int iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_iGirder_w1 = pComboBox->GetItemData(iSelIndex);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T1);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_iGirder_t1 = pComboBox->GetItemData(iSelIndex);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T3);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_iGirder_t3 = pComboBox->GetItemData(iSelIndex);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MATERIAL);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_iGirder_m = pComboBox->GetItemData(iSelIndex);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAILSTYLE);
	iSelIndex = pComboBox->GetCurSel();		
	m_pParamCalculator->m_iGirder_Rw = LOWORD(pComboBox->GetItemData(iSelIndex));
	m_pParamCalculator->m_iGirder_Rh = HIWORD(pComboBox->GetItemData(iSelIndex));
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_W3);
	iSelIndex = pComboBox->GetCurSel();
	m_pParamCalculator->m_iGirder_w3 = pComboBox->GetItemData(iSelIndex);
}
//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::UpdateTotalPrice()
{
	CString strTmp;
	GetDlgItemText(IDC_EDIT_COSTT, strTmp);
	m_pParamCalculator->m_iMainGirderPrice = _ttol(strTmp);
	int iCraTotalPri = m_pParamCalculator->m_iMainGirderPrice * m_pParamCalculator->m_fGirder_mt/1000.f;
	strTmp.Format(_T("%d RMB"), iCraTotalPri);
	m_Static_TotalPrice.SetText(strTmp);
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
BOOL CBMOfferCalcMainCarDlg::CanDoNextStep()
{
	GetDlgItemText(IDC_COMBO_RAILSTYLE, m_pParamCalculator->m_strTroRail);

	if (!m_bResultQ0_Pass || 
		!m_bResultQ1_Pass || 
		!m_bResultQ2_Pass || 
		!m_bResultF_Pass || 
		!m_bResultT_Pass || 
		!m_bResultS_Pass)
	{
		myMessageBox(IDS_CALC_GIRDERISNOTSUITABLE, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if (m_pParamCalculator->m_iMainGirderPrice < 0)
	{
		myMessageBox(IDS_CALC_GIRDERPRICEISERROR, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_COSTT)->SetFocus();		
		return FALSE;
	}
	
	return TRUE;
};
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
void CBMOfferCalcMainCarDlg::OnCbnSelchangeComboGirderW1()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_W1);
	SelectComboBoxItem(pComboBox, 410);
}

void CBMOfferCalcMainCarDlg::OnCbnSelchangeComboGirderT1()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GIRDER_T1);
	SelectComboBoxItem(pComboBox, 6);
}

void CBMOfferCalcMainCarDlg::OnCbnSelchangeComboMaterial()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAILSTYLE);
	SelectComboBoxItem(pComboBox, 235);
}

void CBMOfferCalcMainCarDlg::OnCbnSelchangeComboRailstyle()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_RAILSTYLE);
	SelectComboBoxItem(pComboBox, MAKELONG(50, 30));
}

void CBMOfferCalcMainCarDlg::OnCbnSelchangeComboW3()
{
	myMessageBox(IDS_DEMO_CANNTCHANGE, IDS_DEMO_WARNING, MB_OK | MB_ICONERROR);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_W3);
	SelectComboBoxItem(pComboBox, 750);
}
//-----------------------------------------------------------------------
