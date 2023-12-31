//-----------------------------------------------------------------------
//	File Name:BMOfferCalcDlg.cpp
//	Introduce:Class CBMOfferCalcDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
//	Name:CBMOfferCalcDlg
//	Introduce:CBMOfferCalcDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferCalcDlg, CDialog)
CBMOfferCalcDlg::CBMOfferCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferCalcDlg::IDD, pParent)
{
	m_iCurTabIndex = 0;
	m_clrBackground = NULL;
	for (int i = 0; i < MAX_COMPONENT_CNT; i++)
	{
		m_bComponentIsOnlyHoist[i] = FALSE;
		m_strComponentName[i] = _T("\0");
		m_pCalcOptionsDlg[i] = NULL;
	}
}
//-----------------------------------------------------------------------
CBMOfferCalcDlg::~CBMOfferCalcDlg()
{
}
//-----------------------------------------------------------------------
void CBMOfferCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabComponents);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferCalcDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CBMOfferCalcDlg::OnTcnSelchangeTabMain)
	ON_BN_CLICKED(IDC_BTN_BACK, &CBMOfferCalcDlg::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CBMOfferCalcDlg::OnBnClickedBtnNext)
	ON_MESSAGE(WM_USER_PAGECHANGED, OnOpterationPageChanged)		
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferCalcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	CFont* pDlgFont = GetFont();
	pDlgFont->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	lf.lfHeight = -MulDiv(8, GetDeviceCaps(CPaintDC(this).GetSafeHdc(), LOGPIXELSY), 72);
	m_tabFont.CreateFontIndirect(&lf);

	InitCaptionText();
	InitComponentTabCtrl();

	HRESULT hr = 0;
	m_tabComponents.SetCurSel(0);
	OnTcnSelchangeTabMain(NULL, &hr);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
HBRUSH CBMOfferCalcDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(m_clrBackground == NULL)
	{
		m_clrBackground = RGB(247, 243, 239);
		m_tabComponents.SetBkColor(m_clrBackground);
		m_tabComponents.SetFont(&m_tabFont);
	}

	return hbr;
}
//-----------------------------------------------------------------------
void CBMOfferCalcDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_tbComponentInfo.CloseTable();
	m_tabFont.Detach();
	for (int i = 0; i < MAX_COMPONENT_CNT; i++)
	{
		if (NULL != m_pCalcOptionsDlg[i])
		{
			delete m_pCalcOptionsDlg[i];
			m_pCalcOptionsDlg[i] = NULL;
		}
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	CRect rcClient;
	m_tabComponents.GetClientRect(&rcClient);
	rcClient.left += 5;
	rcClient.right -= 5;
	rcClient.top += 22;
	rcClient.bottom -= 5;

	m_iCurTabIndex = m_tabComponents.GetCurSel();
	int iTotalItem = m_tabComponents.GetItemCount();
	for (int iIndex = 0; iIndex < iTotalItem; iIndex++)
	{
		if (iIndex == m_iCurTabIndex)
		{
			if (NULL == m_pCalcOptionsDlg[iIndex])
			{
				m_pCalcOptionsDlg[iIndex] = new CBMOfferCalcOptionsDlg();
				m_pCalcOptionsDlg[iIndex]->SetInitInformation(m_bComponentIsOnlyHoist[iIndex], m_strOfferNo, m_strComponentName[iIndex]);
				m_pCalcOptionsDlg[iIndex]->Create(IDD_DIALOG_CALCOPTIONS, &m_tabComponents);
				m_pCalcOptionsDlg[iIndex]->MoveWindow(&rcClient);
				m_pCalcOptionsDlg[iIndex]->m_hMsgParent = m_hWnd;
			}
			m_pCalcOptionsDlg[iIndex]->ShowWindow(SW_SHOW);
			m_pCalcOptionsDlg[iIndex]->UpdateWindow();
		}
		else
		{
			if (NULL != m_pCalcOptionsDlg[iIndex])
			{
				m_pCalcOptionsDlg[iIndex]->ShowWindow(SW_HIDE);
			}
		}
	}

	*pResult = 0;
}
//-----------------------------------------------------------------------
void CBMOfferCalcDlg::OnBnClickedBtnBack()
{
	m_pCalcOptionsDlg[m_iCurTabIndex]->DoBackStep();
}
//-----------------------------------------------------------------------
void CBMOfferCalcDlg::OnBnClickedBtnNext()
{
	m_pCalcOptionsDlg[m_iCurTabIndex]->DoNextStep();
}
//-----------------------------------------------------------------------
HRESULT CBMOfferCalcDlg::OnOpterationPageChanged(WPARAM wParam, LPARAM lParam)
{
	if (0 == wParam)
	{
		GetDlgItem(IDC_BTN_BACK)->EnableWindow(TRUE);
		SetDlgItemText(IDC_BTN_NEXT, _T("Next"));
	}
	else if (1 == wParam)
	{
		GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);
		SetDlgItemText(IDC_BTN_NEXT, _T("Next"));
	}
	else if (2 == wParam)
	{
		GetDlgItem(IDC_BTN_BACK)->EnableWindow(TRUE);
		SetDlgItemText(IDC_BTN_NEXT, _T("Save"));
	}

	return S_OK;
}
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
void CBMOfferCalcDlg::SetOfferNo(CString strOfferNo)
{
	m_strOfferNo = strOfferNo;
}
//-----------------------------------------------------------------------
void CBMOfferCalcDlg::InitCaptionText()
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

	CString strCaption;
	strCaption.Format(_T("%s + %s"), strCustomer, strProjectName);
	SetWindowText(strCaption);
}
//-----------------------------------------------------------------------
void CBMOfferCalcDlg::InitComponentTabCtrl()
{
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE OfferNo='%s'", g_tcTableName[eTableID_ComponentInfo], m_strOfferNo);

	BOOL bResult = TRUE;
	m_tbComponentInfo.CloseTable();
	bResult = m_tbComponentInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult || m_tbComponentInfo.GetRecordCnt() == 0)
		return;

	DWORD dwComponentCnt = m_tbComponentInfo.GetRecordCnt();
	for (DWORD dwIndex = 0; dwIndex < dwComponentCnt; dwIndex++)
	{
		CString strComName;
		CString strTabText;
		CString strComType;
		m_tbComponentInfo.GetRecordCell(dwIndex, L"ComponentName", strComName.GetBuffer(64), 64);
		m_tbComponentInfo.GetRecordCell(dwIndex, L"ComponentType", strComType.GetBuffer(64), 64);
		strTabText.Format(_T("%s: %s"), g_tcTabIndex[dwIndex], strComName);
		m_tabComponents.InsertItem(dwIndex, strTabText);
		//
		m_bComponentIsOnlyHoist[dwIndex] = _tcsicmp(strComType, STR_COMTYPE_HOIST)==0 ? TRUE : FALSE;
		m_strComponentName[dwIndex] = strComName;
		m_pCalcOptionsDlg[dwIndex] = NULL;
	}
}
//-----------------------------------------------------------------------