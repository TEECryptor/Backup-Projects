//-----------------------------------------------------------------------
//	File Name:BMOfferCalcOptionsDlg.cpp
//	Introduce:Class CBMOfferCalcOptionsDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcOptionsDlg.h"
#include "Localization.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcOptionsDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferCalcOptionsDlg, CDialog)
CBMOfferCalcOptionsDlg::CBMOfferCalcOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferCalcOptionsDlg::IDD, pParent)
{
	m_clrBackground = NULL;
	m_pCurDialog = NULL;
	m_pBasicDataDlg = NULL;
	m_pHoistDataDlg = NULL;
	m_pMainCarDataDlg = NULL;
	m_pEndCarDataDlg = NULL;
	m_pElectricalDlg = NULL;
	m_pTechnicalDlg = NULL;
}
//-----------------------------------------------------------------------
CBMOfferCalcOptionsDlg::~CBMOfferCalcOptionsDlg()
{
	DestroyWindow();
}
//-----------------------------------------------------------------------
void CBMOfferCalcOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_OPTIONS, m_tabOptions);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferCalcOptionsDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_OPTIONS, &CBMOfferCalcOptionsDlg::OnTcnSelchangeTabOptions)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcOptionsDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferCalcOptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	CFont* pDlgFont = GetFont();
	pDlgFont->GetLogFont(&lf);
	lf.lfWeight = FW_MEDIUM;
	m_tabFont.CreateFontIndirect(&lf);

	CRect rcClient;
	GetClientRect(&rcClient);
	m_tabOptions.InsertItem(0, myGetText(IDS_OPTIONSTAB_BASIC));
	m_tabOptions.InsertItem(1, myGetText(IDS_OPTIONSTAB_HOIST));
	if (FALSE == m_CraneCalculator.m_bOnlyHoist)
	{
		m_tabOptions.InsertItem(2, myGetText(IDS_OPTIONSTAB_MAINCAR));
		m_tabOptions.InsertItem(3, myGetText(IDS_OPTIONSTAB_ENDCAR));
		m_tabOptions.InsertItem(4, myGetText(IDS_OPTIONSTAB_ELCCTRL));
		m_tabOptions.InsertItem(5, myGetText(IDS_OPTIONSTAB_TECHNICAL));
	}
	else 
	{
		m_tabOptions.InsertItem(3, myGetText(IDS_OPTIONSTAB_TECHNICAL));
	}
	m_tabOptions.MoveWindow(&rcClient);
	m_tabOptions.UpdateWindow();
	m_tabOptions.SetCurSel(0);
	m_iLastSelTab = 0;
	
	m_pBasicDataDlg = new CBMOfferCalcBasicDataDlg(&m_tabOptions);
	m_pBasicDataDlg->SetParamCalculator(&m_CraneCalculator);
	m_pBasicDataDlg->Create(IDD_DIALOG_BASICDATA, &m_tabOptions);
	m_pBasicDataDlg->ShowWindow(SW_SHOW);
	m_pBasicDataDlg->UpdateWindow();
	m_pCurDialog = m_pBasicDataDlg;

	UpdateControlsPos();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferCalcOptionsDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (FALSE == bShow)
		return;

	LONG lResult = 0;
	OnTcnSelchangeTabOptions(NULL, &lResult);
}
//-----------------------------------------------------------------------
HBRUSH CBMOfferCalcOptionsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(m_clrBackground == NULL)
	{
		m_clrBackground = RGB(239, 235, 222);
		m_tabOptions.SetBkColor(m_clrBackground);
		m_tabOptions.SetFont(&m_tabFont);
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
//-----------------------------------------------------------------------
void CBMOfferCalcOptionsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	UpdateControlsPos();
}
//-----------------------------------------------------------------------
void CBMOfferCalcOptionsDlg::OnTcnSelchangeTabOptions(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iSelItem = m_tabOptions.GetCurSel();
	if ((iSelItem > m_iLastSelTab) && m_pCurDialog && FALSE == m_pCurDialog->CanDoNextStep())
	{
		m_tabOptions.SetCurSel(m_iLastSelTab);
		return;
	}
	//	
	m_pCurDialog->ShowWindow(SW_HIDE);
	m_pCurDialog->UpdateWindow();
	m_tabOptions.UpdateWindow();
	switch(iSelItem)
	{
	case 0:		//Basic data
		m_pBasicDataDlg->ShowWindow(SW_SHOW);
		m_pCurDialog = m_pBasicDataDlg;
		break;
	case 1:		//Hoist data
		if (NULL == m_pHoistDataDlg)
		{
			m_pHoistDataDlg =  new CBMOfferCalcHoistDlg(&m_tabOptions);
			m_pHoistDataDlg->SetParamCalculator(&m_CraneCalculator);
			m_pHoistDataDlg->Create(IDD_DIALOG_HOIST, &m_tabOptions);	
			UpdateControlsPos();
		}
		m_pHoistDataDlg->ShowWindow(SW_SHOW);
		m_pCurDialog = m_pHoistDataDlg;
		break;
	case 2:		//Main-car data		
		if (TRUE == m_CraneCalculator.m_bOnlyHoist)
		{
			if (NULL == m_pTechnicalDlg)
			{
				m_pTechnicalDlg =  new CBMOfferCalcTechnicalDlg(&m_tabOptions);
				m_pTechnicalDlg->SetParamCalculator(&m_CraneCalculator);
				m_pTechnicalDlg->Create(IDD_DIALOG_TECHNICAL, &m_tabOptions);	
				UpdateControlsPos();
			}
			m_pTechnicalDlg->ShowWindow(SW_SHOW);
			m_pCurDialog = m_pTechnicalDlg;
		}
		else
		{
			if (NULL == m_pMainCarDataDlg)
			{
				m_pMainCarDataDlg =  new CBMOfferCalcMainCarDlg(&m_tabOptions);
				m_pMainCarDataDlg->SetParamCalculator(&m_CraneCalculator);
				m_pMainCarDataDlg->Create(IDD_DIALOG_MAINCAR, &m_tabOptions);	
				UpdateControlsPos();
			}
			m_pMainCarDataDlg->ShowWindow(SW_SHOW);
			m_pCurDialog = m_pMainCarDataDlg;
		}
		break;
	case 3:		//End-car data
		if (NULL == m_pEndCarDataDlg)
		{
			m_pEndCarDataDlg =  new CBMOfferCalcEndCarDlg(&m_tabOptions);
			m_pEndCarDataDlg->SetParamCalculator(&m_CraneCalculator);
			m_pEndCarDataDlg->Create(IDD_DIALOG_ENDCAR, &m_tabOptions);		
			UpdateControlsPos();
		}
		m_pEndCarDataDlg->ShowWindow(SW_SHOW);
		m_pCurDialog = m_pEndCarDataDlg;
		break;
	case 4:		//Elc-contrl data
		if (NULL == m_pElectricalDlg)
		{
			m_pElectricalDlg =  new CBMOfferCalcElecDlg(&m_tabOptions);
			m_pElectricalDlg->SetParamCalculator(&m_CraneCalculator);
			m_pElectricalDlg->Create(IDD_DIALOG_ELECTRICAL, &m_tabOptions);		
			UpdateControlsPos();
		}
		m_pElectricalDlg->ShowWindow(SW_SHOW);
		m_pCurDialog = m_pElectricalDlg;
		break;
	case 5:		//Technical data
		if (NULL == m_pTechnicalDlg)
		{
			m_pTechnicalDlg =  new CBMOfferCalcTechnicalDlg(&m_tabOptions);
			m_pTechnicalDlg->SetParamCalculator(&m_CraneCalculator);
			m_pTechnicalDlg->Create(IDD_DIALOG_TECHNICAL, &m_tabOptions);	
			UpdateControlsPos();
		}
		m_pTechnicalDlg->ShowWindow(SW_SHOW);
		m_pCurDialog = m_pTechnicalDlg;
		break;
	}

	*pResult = 0;
	m_iLastSelTab = iSelItem;
	if (iSelItem == m_tabOptions.GetItemCount() - 1)
	{
		::SendMessage(m_hMsgParent, WM_USER_PAGECHANGED, 2, 0);
		*pResult = 1;
	}
	else if(0 == iSelItem)
	{
		::SendMessage(m_hMsgParent, WM_USER_PAGECHANGED, 1, 0);
		*pResult = 1;
	}
	else
	{
		::SendMessage(m_hMsgParent, WM_USER_PAGECHANGED, 0, 0);
		*pResult = 0;
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcOptionsDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_tabFont.Detach();
	if (NULL != m_pBasicDataDlg)
	{
		delete m_pBasicDataDlg;
		m_pBasicDataDlg = NULL;
	}
	if (NULL != m_pHoistDataDlg)
	{
		delete m_pHoistDataDlg;
		m_pHoistDataDlg = NULL;
	}
	if (NULL != m_pMainCarDataDlg)
	{
		delete m_pMainCarDataDlg;
		m_pMainCarDataDlg = NULL;
	}	
	if (NULL != m_pEndCarDataDlg)
	{
		delete m_pEndCarDataDlg;
		m_pEndCarDataDlg = NULL;
	}		
	if (NULL != m_pElectricalDlg)
	{
		delete m_pElectricalDlg;
		m_pElectricalDlg = NULL;
	}		
	if (NULL != m_pTechnicalDlg)
	{
		delete m_pTechnicalDlg;
		m_pTechnicalDlg = NULL;
	}			
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void CBMOfferCalcOptionsDlg::SetInitInformation(BOOL bIsOnlyHoist, 
												CString strOfferNo, 
												CString strComName)
{
	m_CraneCalculator.m_bOnlyHoist = bIsOnlyHoist;
	m_strOfferNo = strOfferNo;
	m_strComName = strComName;
	//
	m_CraneCalculator.LoadDataFromDatabase(strOfferNo, strComName);
}
//-----------------------------------------------------------------------
BOOL CBMOfferCalcOptionsDlg::DoBackStep()
{
	int iSelItem = m_tabOptions.GetCurSel();
	iSelItem = max(iSelItem--, 0);
	m_tabOptions.SetCurSel(iSelItem);
	//
	HRESULT hr = 0;
	OnTcnSelchangeTabOptions(NULL, &hr);

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMOfferCalcOptionsDlg::DoNextStep()
{
	int iSelItem = m_tabOptions.GetCurSel();

	// It's finished
	if (iSelItem == m_tabOptions.GetItemCount() - 1)
	{
		if (m_CraneCalculator.SaveDataToDatabase(m_strOfferNo, m_strComName))
		{
			myMessageBox(IDS_CALC_SAVESUCCESSED, IDS_MSGDLG_SUCCESS, MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			myMessageBox(IDS_CALC_SAVEFAILED, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		}
	}
	else
	{
		iSelItem = iSelItem++;
		m_tabOptions.SetCurSel(iSelItem);
		//
		HRESULT hr = 0;
		OnTcnSelchangeTabOptions(NULL, &hr);
	}

	return TRUE;
}
//-----------------------------------------------------------------------
void CBMOfferCalcOptionsDlg::UpdateControlsPos()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	if (m_tabOptions.m_hWnd != NULL)
	{
		CRect rcTab;
		rcTab = rcClient;
		m_tabOptions.MoveWindow(&rcTab);
		m_tabOptions.UpdateWindow();
		
		CRect rcTabCtrl;
		m_tabOptions.GetClientRect(&rcTabCtrl);
		rcTabCtrl.left += 5;
		rcTabCtrl.right -= 5;
		rcTabCtrl.top += 22;
		rcTabCtrl.bottom -= 5;
		//
		if (NULL != m_pBasicDataDlg) m_pBasicDataDlg->MoveWindow(&rcTabCtrl);
		if (NULL != m_pHoistDataDlg) m_pHoistDataDlg->MoveWindow(&rcTabCtrl);
		if (NULL != m_pMainCarDataDlg) m_pMainCarDataDlg->MoveWindow(&rcTabCtrl);
		if (NULL != m_pEndCarDataDlg) m_pEndCarDataDlg->MoveWindow(&rcTabCtrl);
		if (NULL != m_pElectricalDlg) m_pElectricalDlg->MoveWindow(&rcTabCtrl);
		if (NULL != m_pTechnicalDlg) m_pTechnicalDlg->MoveWindow(&rcTabCtrl);
	}
}
//-----------------------------------------------------------------------