//-----------------------------------------------------------------------
//	File Name:BMOfferSelectDlg.cpp
//	Introduce:Class CBMOfferSelectDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferSelectDlg.h"
#include "BMOfferMakerDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferSelectDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferSelectDlg, CDialog)

CBMOfferSelectDlg::CBMOfferSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferSelectDlg::IDD, pParent)
{
	for (int i = 0; i < MAX_COMPONENT_CNT; i++)
		m_pOfferOptionsDlg[i] = NULL;
}
//-----------------------------------------------------------------------
CBMOfferSelectDlg::~CBMOfferSelectDlg()
{
}
//-----------------------------------------------------------------------
void CBMOfferSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_OFFEROPTIONS, m_tabOptions);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferSelectDlg, CDialog)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_OFFEROPTIONS, &CBMOfferSelectDlg::OnTcnSelchangeTabOfferoptions)
	ON_CBN_SELCHANGE(IDC_COMBO_CURRENCY, &CBMOfferSelectDlg::OnCbnSelchangeComboCurrency)
	ON_EN_CHANGE(IDC_EDIT_RATE, &CBMOfferSelectDlg::OnEnChangeEditRate)
	ON_EN_CHANGE(IDC_EDIT_YAT, &CBMOfferSelectDlg::OnEnChangeEditYat)
	ON_BN_CLICKED(IDC_BTN_OFFER, &CBMOfferSelectDlg::OnBnClickedBtnOffer)
	ON_MESSAGE(WM_USER_UPDATETOTALPRICE, OnUpdateTotalPrice)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CBMOfferSelectDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*LOGFONT lf;
	CFont* pDlgFont = GetFont();
	pDlgFont->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	lf.lfHeight = -MulDiv(8, GetDeviceCaps(CPaintDC(this).GetSafeHdc(), LOGPIXELSY), 72);
	CFont tabFont;
	tabFont.CreateFontIndirect(&lf);
	m_tabOptions.SetFont(&tabFont);
	m_tabOptions.SetBkColor(RGB(239, 235, 222));*/

	InitCaptionText();
	InitComponentTabCtrl();
	if (m_tabOptions.GetItemCount() > 0)
	{
		m_pOfferOptionsDlg[0]->ShowWindow(SW_SHOW);
		m_pOfferOptionsDlg[0]->UpdateWindow();
	}

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CURRENCY);
	int iIndex = pComboBox->AddString(_T("RMB"));
	pComboBox->SetItemData(iIndex, eCurrencyType_RMB);
	iIndex = pComboBox->AddString(_T("EUR"));
	pComboBox->SetItemData(iIndex, eCurrencyType_EUR);
	iIndex = pComboBox->AddString(_T("USD"));
	pComboBox->SetItemData(iIndex, eCurrencyType_USD);
	pComboBox->SetCurSel(0);
	//
	SetDlgItemText(IDC_EDIT_RATE, _T("8.9"));
	SetDlgItemText(IDC_EDIT_YAT, _T("17"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferSelectDlg::OnDestroy()
{
	CDialog::OnDestroy();

	for (int i = 0; i < MAX_COMPONENT_CNT; i++)
	{
		if (NULL != m_pOfferOptionsDlg[i])
		{
			delete m_pOfferOptionsDlg[i];
			m_pOfferOptionsDlg[i] = NULL;
		}
	}
}
//-----------------------------------------------------------------------
void CBMOfferSelectDlg::OnTcnSelchangeTabOfferoptions(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iCurTabIndex = m_tabOptions.GetCurSel();
	int iTotalItem = m_tabOptions.GetItemCount();
	for (int iIndex = 0; iIndex < iTotalItem; iIndex++)
	{
		if (NULL == m_pOfferOptionsDlg[iIndex])
			continue;

		if (iIndex == iCurTabIndex)
		{
			m_pOfferOptionsDlg[iIndex]->ShowWindow(SW_SHOW);
			m_pOfferOptionsDlg[iIndex]->UpdateWindow();
		}
		else
		{
			m_pOfferOptionsDlg[iIndex]->ShowWindow(SW_HIDE);
		}
	}
	*pResult = 0;
}
//-----------------------------------------------------------------------
void CBMOfferSelectDlg::OnCbnSelchangeComboCurrency()
{
	CString strCurrency;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CURRENCY);
	int iSel = pComboBox->GetCurSel();
	int iItemData = pComboBox->GetItemData(iSel);

	CString strRate;
	float fRate = 0.0f;
	GetDlgItemText(IDC_EDIT_RATE, strRate);
	fRate = _tstof(strRate);
	int iTotalItem = m_tabOptions.GetItemCount();
	for (int iIndex = 0; iIndex < iTotalItem; iIndex++)
	{
		if (NULL == m_pOfferOptionsDlg[iIndex])
			continue;

		m_pOfferOptionsDlg[iIndex]->SetCurrentCurrency((eCurrencyType)iItemData, fRate);
	}

	SetDlgItemText(IDC_STATIC_OFFERDLG_PRICENUIT, strCurrency);
}
//-----------------------------------------------------------------------
void CBMOfferSelectDlg::OnEnChangeEditRate()
{
	CString strCurrency;
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CURRENCY);
	int iSel = pComboBox->GetCurSel();
	int iItemData = pComboBox->GetItemData(iSel);

	CString strRate;
	float fRate = 0.0f;
	GetDlgItemText(IDC_EDIT_RATE, strRate);
	fRate = _tstof(strRate);

	int iTotalItem = m_tabOptions.GetItemCount();
	for (int iIndex = 0; iIndex < iTotalItem; iIndex++)
	{
		if (NULL == m_pOfferOptionsDlg[iIndex])
			continue;
		m_pOfferOptionsDlg[iIndex]->SetCurrentCurrency((eCurrencyType)iItemData, fRate);
	}
}
//-----------------------------------------------------------------------
void CBMOfferSelectDlg::OnEnChangeEditYat()
{
	CString strVATRate;
	int iVATRate = 0;
	GetDlgItemText(IDC_EDIT_YAT, strVATRate);
	iVATRate = _ttol(strVATRate);

	int iTotalItem = m_tabOptions.GetItemCount();
	for (int iIndex = 0; iIndex < iTotalItem; iIndex++)
	{
		if (NULL == m_pOfferOptionsDlg[iIndex])
			continue;
		m_pOfferOptionsDlg[iIndex]->SetVATRate(iVATRate/100.0f);
	}
}
//-----------------------------------------------------------------------
void CBMOfferSelectDlg::OnBnClickedBtnOffer()
{	
	ComponentPriceList lstPrices;
	int iTotalItem = m_tabOptions.GetItemCount();
	for (int iIndex = 0; iIndex < iTotalItem; iIndex++)
	{
		if (m_pOfferOptionsDlg[iIndex] && 
			m_pOfferOptionsDlg[iIndex]->m_iComCount > 0 && 
			m_pOfferOptionsDlg[iIndex]->m_iComToatlPrice > 0)
		{
			DetailPriceList lstDetail;
			m_pOfferOptionsDlg[iIndex]->GetDetialPriceInfo(lstDetail);
			lstPrices.push_back(ComPriceInfo(m_pOfferOptionsDlg[iIndex]->m_strComName,
											 m_pOfferOptionsDlg[iIndex]->m_iComCount,
											 m_pOfferOptionsDlg[iIndex]->m_iComToatlPrice/m_pOfferOptionsDlg[iIndex]->m_iComCount,
											 m_pOfferOptionsDlg[iIndex]->m_pCraneCalculator,
											 lstDetail));
		}
	}

	CString strCurrency;
	GetDlgItemText(IDC_COMBO_CURRENCY, strCurrency);

	CBMOfferMakerDlg dlg;
	dlg.SetComponentPrices(m_strDlgCaption, m_strOfferNo, m_strProjectname, lstPrices, strCurrency);
	dlg.DoModal();
}
//-----------------------------------------------------------------------
HRESULT CBMOfferSelectDlg::OnUpdateTotalPrice(WPARAM wParam, LPARAM lParam)
{
	int iTotalPrices = 0;
	int iTotalItem = m_tabOptions.GetItemCount();
	for (int iIndex = 0; iIndex < iTotalItem; iIndex++)
	{
		if (NULL != m_pOfferOptionsDlg[iIndex])
		{
			iTotalPrices += m_pOfferOptionsDlg[iIndex]->m_iComToatlPrice;
		}
	}

	CString strTotalPrice;
	strTotalPrice.Format(_T("%d"), iTotalPrices);
	SetDlgItemText(IDC_EDIT_OFFERDLG_TOTLPRICE, strTotalPrice);

	return 1;
}
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
void CBMOfferSelectDlg::SetOfferNo(CString strOfferNo)
{
	m_strOfferNo = strOfferNo;
}
//-----------------------------------------------------------------------
void CBMOfferSelectDlg::InitCaptionText()
{
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE OfferNo='%s'", g_tcTableName[eTableID_OffersInfo], m_strOfferNo);

	BOOL bResult = TRUE;
	CADOTable tbOfferInfo;
	bResult = tbOfferInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult || tbOfferInfo.GetRecordCnt() == 0)
		return;

	CString strCustomer;
	CString strProjectName;
	tbOfferInfo.GetRecordCell(0, L"CompanyShortName", strCustomer.GetBuffer(64), 64);
	tbOfferInfo.GetRecordCell(0, L"ProjectName", strProjectName.GetBuffer(64), 64);
	tbOfferInfo.CloseTable();

	m_strProjectname = strProjectName;
	m_strDlgCaption.Format(_T("%s + %s"), strCustomer, strProjectName);
	SetWindowText(m_strDlgCaption);
}
//-----------------------------------------------------------------------
void CBMOfferSelectDlg::InitComponentTabCtrl()
{
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE OfferNo='%s'", g_tcTableName[eTableID_ComponentInfo], m_strOfferNo);

	BOOL bResult = TRUE;
	CADOTable tbComponent;
	bResult = tbComponent.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult || tbComponent.GetRecordCnt() == 0)
		return;

	CRect rcClient;
	m_tabOptions.GetClientRect(&rcClient);
	rcClient.left += 4;
	rcClient.right -= 4;
	rcClient.top += 22;
	rcClient.bottom -= 4;

	int iRealIndex = 0;
	DWORD dwComponentCnt = tbComponent.GetRecordCnt();
	for (DWORD dwIndex = 0; dwIndex < dwComponentCnt; dwIndex++)
	{
		TCHAR tcComName[64] = _T("\0");
		TCHAR tcHoistCode[64] = _T("\0");
		DWORD dwLoadOfCrane = 0;
		DWORD dwLoadOfHoist = 0;
		tbComponent.GetRecordCell(dwIndex, L"ComponentName", tcComName, 64);
		tbComponent.GetRecordCell(dwIndex, L"HoistCode", tcHoistCode, 64);
		tbComponent.GetRecordCell(dwIndex, L"CraneLoad", dwLoadOfCrane);
		tbComponent.GetRecordCell(dwIndex, L"HoistLoad", dwLoadOfHoist);
		if (_tcslen(tcHoistCode) == 0 || 0 == dwLoadOfCrane || 0 == dwLoadOfHoist)
			continue;

		CString strTabText;
		strTabText.Format(_T("%s: %s "), g_tcTabIndex[dwIndex], tcComName);
		m_tabOptions.InsertItem(iRealIndex, strTabText);
		//
		m_pOfferOptionsDlg[iRealIndex] = new CBMOfferOptionsDlg();
		m_pOfferOptionsDlg[iRealIndex]->m_hMsgParent = m_hWnd;
		m_pOfferOptionsDlg[iRealIndex]->SetOfferNoAndComName(m_strOfferNo, tcComName);
		m_pOfferOptionsDlg[iRealIndex]->Create(IDD_DIALOG_OFFEROPTIONS, &m_tabOptions);
		m_pOfferOptionsDlg[iRealIndex]->MoveWindow(&rcClient);
		iRealIndex++;
	}
	tbComponent.CloseTable();
}
//-----------------------------------------------------------------------
