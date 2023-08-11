//-----------------------------------------------------------------------
//	File Name:BMOfferCalcTechnicalDlg.cpp
//	Introduce:Class CBMOfferCalcTechnicalDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferCalcTechnicalDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcTechnicalDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferCalcTechnicalDlg, CDialog)
CBMOfferCalcTechnicalDlg::CBMOfferCalcTechnicalDlg(CWnd* pParent /*=NULL*/)
	: CBMCalcOptionsDlg(CBMOfferCalcTechnicalDlg::IDD, pParent)
{

}
//-----------------------------------------------------------------------
CBMOfferCalcTechnicalDlg::~CBMOfferCalcTechnicalDlg()
{
	DestroyWindow();
}
//-----------------------------------------------------------------------
void CBMOfferCalcTechnicalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TECHDATA, m_lstCtlTechnicalData);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferCalcTechnicalDlg, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcTechnicalDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferCalcTechnicalDlg::OnInitDialog()
{
	CBMCalcOptionsDlg::OnInitDialog();

	// Initiliaze lists
	m_imgNULL.Create(1, 20, TRUE | ILC_COLOR32, 1, 0); 
	m_lstCtlTechnicalData.SetExtendedStyle(m_lstCtlTechnicalData.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	//m_lstCtlTechnicalData.SetFullItemBKColor(RGB(255, 255, 255), RGB(255, 255, 255));
	m_lstCtlTechnicalData.SetImageList(&m_imgNULL, LVSIL_SMALL);
	m_lstCtlTechnicalData.InsertColumn(0, _T(""), 0, 300);
	m_lstCtlTechnicalData.InsertColumn(1, _T(""), 0, 200);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferCalcTechnicalDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBMCalcOptionsDlg::OnShowWindow(bShow, nStatus);
	if (FALSE == bShow)
		return;

	m_pParamCalculator->ConstructTechData();

	m_lstCtlTechnicalData.DeleteAllItems();
	if (!m_pParamCalculator->m_bOnlyHoist)
	{
		FillCompomentListData();
	}
	FillHoistData();
	FillHoistStdAdditionsData();
	FillHoistSelAdditionsData();
	if (!m_pParamCalculator->m_bOnlyHoist)
	{
		FillCranesData();
		FillCranesHoistData();
		FillCraneSelAdditionsData();
	}
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
BOOL CBMOfferCalcTechnicalDlg::CanDoNextStep()
{
	return TRUE;
};
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferCalcTechnicalDlg::FillCompomentListData()
{
	TechDataList list;
	m_pParamCalculator->GetCompomentTechData(list);
	
	int iIndex = m_lstCtlTechnicalData.GetItemCount();
	if (iIndex > 0) m_lstCtlTechnicalData.InsertItem(iIndex++, _T(""));
	m_lstCtlTechnicalData.InsertItem(iIndex++, _T("Component List"));
	m_lstCtlTechnicalData.SetItemIsTitle(iIndex-1);
	//
	for (TechDataList::iterator it = list.begin(); it != list.end(); it++)
	{
		TechData data = *it;
		m_lstCtlTechnicalData.InsertItem(iIndex++, data.strName);
		m_lstCtlTechnicalData.SetItemText(iIndex-1, 1, data.strValue);
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcTechnicalDlg::FillHoistData()
{
	TechDataList list;
	if (_T("BMZ") == m_pParamCalculator->m_CalcDatalst.GA1) m_pParamCalculator->GetHoistTechDataEx(list);
	else m_pParamCalculator->GetHoistTechData(list);

	int iIndex = m_lstCtlTechnicalData.GetItemCount();
	if (iIndex > 0) m_lstCtlTechnicalData.InsertItem(iIndex++, _T(""));
	m_lstCtlTechnicalData.InsertItem(iIndex++, _T("Hoist Data"));
	m_lstCtlTechnicalData.SetItemIsTitle(iIndex-1);
	//
	for (TechDataList::iterator it = list.begin(); it != list.end(); it++)
	{
		TechData data = *it;
		m_lstCtlTechnicalData.InsertItem(iIndex++, data.strName);
		m_lstCtlTechnicalData.SetItemText(iIndex-1, 1, data.strValue);
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcTechnicalDlg::FillHoistStdAdditionsData()
{
	TechDataList list;
	m_pParamCalculator->GetHoistStdAddTechData(list);

	int iIndex = m_lstCtlTechnicalData.GetItemCount();
	if (iIndex > 0) m_lstCtlTechnicalData.InsertItem(iIndex++, _T(""));
	m_lstCtlTechnicalData.InsertItem(iIndex++, _T("Standard Additional features (Price included in the hoist price)"));
	m_lstCtlTechnicalData.SetItemIsTitle(iIndex-1);
	//
	for (TechDataList::iterator it = list.begin(); it != list.end(); it++)
	{
		TechData data = *it;
		m_lstCtlTechnicalData.InsertItem(iIndex++, data.strName);
		m_lstCtlTechnicalData.SetItemText(iIndex-1, 1, data.strValue);
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcTechnicalDlg::FillHoistSelAdditionsData()
{
	TechDataList list;
	m_pParamCalculator->GetHoistSelAddTechData(list);

	int iIndex = m_lstCtlTechnicalData.GetItemCount();
	if (iIndex > 0) m_lstCtlTechnicalData.InsertItem(iIndex++, _T(""));
	m_lstCtlTechnicalData.InsertItem(iIndex++, _T("Added Additional features"));
	m_lstCtlTechnicalData.SetItemIsTitle(iIndex-1);
	//
	for (TechDataList::iterator it = list.begin(); it != list.end(); it++)
	{
		TechData data = *it;
		m_lstCtlTechnicalData.InsertItem(iIndex++, data.strName);
		m_lstCtlTechnicalData.SetItemText(iIndex-1, 1, data.strValue);
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcTechnicalDlg::FillCranesData()
{
	int iIndex = m_lstCtlTechnicalData.GetItemCount();
	if (iIndex > 0) m_lstCtlTechnicalData.InsertItem(iIndex++, _T(""));
	m_lstCtlTechnicalData.InsertItem(iIndex++, _T("Cranes Data"));
	m_lstCtlTechnicalData.SetItemIsTitle(iIndex-1);

	TechDataList list;
	m_pParamCalculator->GetCranesTechData(list);
	for (TechDataList::iterator it = list.begin(); it != list.end(); it++)
	{
		TechData data = *it;
		m_lstCtlTechnicalData.InsertItem(iIndex++, data.strName);
		m_lstCtlTechnicalData.SetItemText(iIndex-1, 1, data.strValue);
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcTechnicalDlg::FillCranesHoistData()
{
	TechDataList list;
	if (_T("BMZ") == m_pParamCalculator->m_CalcDatalst.GA1) m_pParamCalculator->GetCranesTechHoistDataEx(list);
	else m_pParamCalculator->GetCranesTechHoistData(list);

	int iIndex = m_lstCtlTechnicalData.GetItemCount();
	for (TechDataList::iterator it = list.begin(); it != list.end(); it++)
	{
		TechData data = *it;
		m_lstCtlTechnicalData.InsertItem(iIndex++, data.strName);
		m_lstCtlTechnicalData.SetItemText(iIndex-1, 1, data.strValue);
	}
}
//-----------------------------------------------------------------------
void CBMOfferCalcTechnicalDlg::FillCraneSelAdditionsData()
{
	TechDataList list;
	m_pParamCalculator->GetCraneSelAddTechData(list);

	int iIndex = m_lstCtlTechnicalData.GetItemCount();
	if (iIndex > 0) m_lstCtlTechnicalData.InsertItem(iIndex++, _T(""));
	m_lstCtlTechnicalData.InsertItem(iIndex++, _T("Added Additional features"));
	m_lstCtlTechnicalData.SetItemIsTitle(iIndex-1);
	//
	for (TechDataList::iterator it = list.begin(); it != list.end(); it++)
	{
		TechData data = *it;
		m_lstCtlTechnicalData.InsertItem(iIndex++, data.strName);
		m_lstCtlTechnicalData.SetItemText(iIndex-1, 1, data.strValue);
	}
}
//-----------------------------------------------------------------------
