//-----------------------------------------------------------------------
//	File Name:BMOfferComponentsInfoDlg.cpp
//	Introduce:Class CBMOfferComponentsInfoDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferComponentsInfoDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferComponentsInfoDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferComponentsInfoDlg, CDialog)
CBMOfferComponentsInfoDlg::CBMOfferComponentsInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferComponentsInfoDlg::IDD, pParent)
{

}
//-----------------------------------------------------------------------
CBMOfferComponentsInfoDlg::~CBMOfferComponentsInfoDlg()
{
	DestroyWindow();
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferComponentsInfoDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK_COMPONENTDLG_CHECK1, &CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck1)
	ON_BN_CLICKED(IDC_CHECK_COMPONENTDLG_CHECK2, &CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck2)
	ON_BN_CLICKED(IDC_CHECK_COMPONENTDLG_CHECK3, &CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck3)
	ON_BN_CLICKED(IDC_CHECK_COMPONENTDLG_CHECK4, &CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck4)
	ON_BN_CLICKED(IDC_CHECK_COMPONENTDLG_CHECK5, &CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck5)
	ON_BN_CLICKED(IDC_CHECK_COMPONENTDLG_CHECK6, &CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck6)
	ON_BN_CLICKED(IDC_CHECK_COMPONENTDLG_CHECK7, &CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck7)
	ON_BN_CLICKED(IDC_CHECK_COMPONENTDLG_CHECK8, &CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck8)
	ON_BN_CLICKED(IDC_CHECK_COMPONENTDLG_CHECK9, &CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck9)
	ON_BN_CLICKED(IDC_CHECK_COMPONENTDLG_CHECK10, &CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck10)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferComponentsInfoDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferComponentsInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ComPriceCtrls[0].nCheckBox = IDC_CHECK_COMPONENTDLG_CHECK1;
	m_ComPriceCtrls[0].nNameEdit = IDC_EDIT_COMPONENTDLG_NAME1;
	m_ComPriceCtrls[0].nQtyEdit = IDC_EDIT_COMPONENTDLG_QTY1;
	m_ComPriceCtrls[0].nUnitPriceEdit = IDC_EDIT_COMPONENTDLG_UNITPRICE1;
	m_ComPriceCtrls[0].nUnitUnitStatic = IDC_EDIT_COMPONENTDLG_UNITUNIT1;
	m_ComPriceCtrls[0].nTotalPriceEdit = IDC_EDIT_COMPONENTDLG_TOTALPRICE1;
	m_ComPriceCtrls[0].nTotalUnitStatic = IDC_EDIT_COMPONENTDLG_TOTALUNIT1;
	//
	m_ComPriceCtrls[1].nCheckBox = IDC_CHECK_COMPONENTDLG_CHECK2;
	m_ComPriceCtrls[1].nNameEdit = IDC_EDIT_COMPONENTDLG_NAME2;
	m_ComPriceCtrls[1].nQtyEdit = IDC_EDIT_COMPONENTDLG_QTY2;
	m_ComPriceCtrls[1].nUnitPriceEdit = IDC_EDIT_COMPONENTDLG_UNITPRICE2;
	m_ComPriceCtrls[1].nUnitUnitStatic = IDC_EDIT_COMPONENTDLG_UNITUNIT2;
	m_ComPriceCtrls[1].nTotalPriceEdit = IDC_EDIT_COMPONENTDLG_TOTALPRICE2;
	m_ComPriceCtrls[1].nTotalUnitStatic = IDC_EDIT_COMPONENTDLG_TOTALUNIT2;
	//
	m_ComPriceCtrls[2].nCheckBox = IDC_CHECK_COMPONENTDLG_CHECK3;
	m_ComPriceCtrls[2].nNameEdit = IDC_EDIT_COMPONENTDLG_NAME3;
	m_ComPriceCtrls[2].nQtyEdit = IDC_EDIT_COMPONENTDLG_QTY3;
	m_ComPriceCtrls[2].nUnitPriceEdit = IDC_EDIT_COMPONENTDLG_UNITPRICE3;
	m_ComPriceCtrls[2].nUnitUnitStatic = IDC_EDIT_COMPONENTDLG_UNITUNIT3;
	m_ComPriceCtrls[2].nTotalPriceEdit = IDC_EDIT_COMPONENTDLG_TOTALPRICE3;
	m_ComPriceCtrls[2].nTotalUnitStatic = IDC_EDIT_COMPONENTDLG_TOTALUNIT3;
	//
	m_ComPriceCtrls[3].nCheckBox = IDC_CHECK_COMPONENTDLG_CHECK4;
	m_ComPriceCtrls[3].nNameEdit = IDC_EDIT_COMPONENTDLG_NAME4;
	m_ComPriceCtrls[3].nQtyEdit = IDC_EDIT_COMPONENTDLG_QTY4;
	m_ComPriceCtrls[3].nUnitPriceEdit = IDC_EDIT_COMPONENTDLG_UNITPRICE4;
	m_ComPriceCtrls[3].nUnitUnitStatic = IDC_EDIT_COMPONENTDLG_UNITUNIT4;
	m_ComPriceCtrls[3].nTotalPriceEdit = IDC_EDIT_COMPONENTDLG_TOTALPRICE4;
	m_ComPriceCtrls[3].nTotalUnitStatic = IDC_EDIT_COMPONENTDLG_TOTALUNIT4;
	//
	m_ComPriceCtrls[4].nCheckBox = IDC_CHECK_COMPONENTDLG_CHECK5;
	m_ComPriceCtrls[4].nNameEdit = IDC_EDIT_COMPONENTDLG_NAME5;
	m_ComPriceCtrls[4].nQtyEdit = IDC_EDIT_COMPONENTDLG_QTY5;
	m_ComPriceCtrls[4].nUnitPriceEdit = IDC_EDIT_COMPONENTDLG_UNITPRICE5;
	m_ComPriceCtrls[4].nUnitUnitStatic = IDC_EDIT_COMPONENTDLG_UNITUNIT5;
	m_ComPriceCtrls[4].nTotalPriceEdit = IDC_EDIT_COMPONENTDLG_TOTALPRICE5;
	m_ComPriceCtrls[4].nTotalUnitStatic = IDC_EDIT_COMPONENTDLG_TOTALUNIT5;
	//
	m_ComPriceCtrls[5].nCheckBox = IDC_CHECK_COMPONENTDLG_CHECK6;
	m_ComPriceCtrls[5].nNameEdit = IDC_EDIT_COMPONENTDLG_NAME6;
	m_ComPriceCtrls[5].nQtyEdit = IDC_EDIT_COMPONENTDLG_QTY6;
	m_ComPriceCtrls[5].nUnitPriceEdit = IDC_EDIT_COMPONENTDLG_UNITPRICE6;
	m_ComPriceCtrls[5].nUnitUnitStatic = IDC_EDIT_COMPONENTDLG_UNITUNIT6;
	m_ComPriceCtrls[5].nTotalPriceEdit = IDC_EDIT_COMPONENTDLG_TOTALPRICE6;
	m_ComPriceCtrls[5].nTotalUnitStatic = IDC_EDIT_COMPONENTDLG_TOTALUNIT6;
	//
	m_ComPriceCtrls[6].nCheckBox = IDC_CHECK_COMPONENTDLG_CHECK7;
	m_ComPriceCtrls[6].nNameEdit = IDC_EDIT_COMPONENTDLG_NAME7;
	m_ComPriceCtrls[6].nQtyEdit = IDC_EDIT_COMPONENTDLG_QTY7;
	m_ComPriceCtrls[6].nUnitPriceEdit = IDC_EDIT_COMPONENTDLG_UNITPRICE7;
	m_ComPriceCtrls[6].nUnitUnitStatic = IDC_EDIT_COMPONENTDLG_UNITUNIT7;
	m_ComPriceCtrls[6].nTotalPriceEdit = IDC_EDIT_COMPONENTDLG_TOTALPRICE7;
	m_ComPriceCtrls[6].nTotalUnitStatic = IDC_EDIT_COMPONENTDLG_TOTALUNIT7;
	//
	m_ComPriceCtrls[7].nCheckBox = IDC_CHECK_COMPONENTDLG_CHECK8;
	m_ComPriceCtrls[7].nNameEdit = IDC_EDIT_COMPONENTDLG_NAME8;
	m_ComPriceCtrls[7].nQtyEdit = IDC_EDIT_COMPONENTDLG_QTY8;
	m_ComPriceCtrls[7].nUnitPriceEdit = IDC_EDIT_COMPONENTDLG_UNITPRICE8;
	m_ComPriceCtrls[7].nUnitUnitStatic = IDC_EDIT_COMPONENTDLG_UNITUNIT8;
	m_ComPriceCtrls[7].nTotalPriceEdit = IDC_EDIT_COMPONENTDLG_TOTALPRICE8;
	m_ComPriceCtrls[7].nTotalUnitStatic = IDC_EDIT_COMPONENTDLG_TOTALUNIT8;
	//
	m_ComPriceCtrls[8].nCheckBox = IDC_CHECK_COMPONENTDLG_CHECK9;
	m_ComPriceCtrls[8].nNameEdit = IDC_EDIT_COMPONENTDLG_NAME9;
	m_ComPriceCtrls[8].nQtyEdit = IDC_EDIT_COMPONENTDLG_QTY9;
	m_ComPriceCtrls[8].nUnitPriceEdit = IDC_EDIT_COMPONENTDLG_UNITPRICE9;
	m_ComPriceCtrls[8].nUnitUnitStatic = IDC_EDIT_COMPONENTDLG_UNITUNIT9;
	m_ComPriceCtrls[8].nTotalPriceEdit = IDC_EDIT_COMPONENTDLG_TOTALPRICE9;
	m_ComPriceCtrls[8].nTotalUnitStatic = IDC_EDIT_COMPONENTDLG_TOTALUNIT9;
	//
	m_ComPriceCtrls[9].nCheckBox = IDC_CHECK_COMPONENTDLG_CHECK10;
	m_ComPriceCtrls[9].nNameEdit = IDC_EDIT_COMPONENTDLG_NAME10;
	m_ComPriceCtrls[9].nQtyEdit = IDC_EDIT_COMPONENTDLG_QTY10;
	m_ComPriceCtrls[9].nUnitPriceEdit = IDC_EDIT_COMPONENTDLG_UNITPRICE10;
	m_ComPriceCtrls[9].nUnitUnitStatic = IDC_EDIT_COMPONENTDLG_UNITUNIT10;
	m_ComPriceCtrls[9].nTotalPriceEdit = IDC_EDIT_COMPONENTDLG_TOTALPRICE10;
	m_ComPriceCtrls[9].nTotalUnitStatic = IDC_EDIT_COMPONENTDLG_TOTALUNIT10;

	int iComponentCnt = m_lstComponentPrices.size();
	for (int i = 0; i < MAX_COMPONENT_CNT; i++)
	{		
		((CButton*)GetDlgItem(m_ComPriceCtrls[i].nCheckBox))->SetCheck(BST_CHECKED);
		SetDlgItemText(m_ComPriceCtrls[i].nUnitUnitStatic, m_strCurrency);
		SetDlgItemText(m_ComPriceCtrls[i].nTotalUnitStatic, m_strCurrency);
		if (i >= iComponentCnt)
		{
			GetDlgItem(m_ComPriceCtrls[i].nCheckBox)->ShowWindow(SW_HIDE);
			GetDlgItem(m_ComPriceCtrls[i].nNameEdit)->ShowWindow(SW_HIDE);
			GetDlgItem(m_ComPriceCtrls[i].nQtyEdit)->ShowWindow(SW_HIDE);
			GetDlgItem(m_ComPriceCtrls[i].nUnitPriceEdit)->ShowWindow(SW_HIDE);
			GetDlgItem(m_ComPriceCtrls[i].nUnitUnitStatic)->ShowWindow(SW_HIDE);
			GetDlgItem(m_ComPriceCtrls[i].nTotalPriceEdit)->ShowWindow(SW_HIDE);
			GetDlgItem(m_ComPriceCtrls[i].nTotalUnitStatic)->ShowWindow(SW_HIDE);
		}
	}
	UpdateTotalPrices();

	int iIndex = 0;
	for (ComponentPriceList::iterator it = m_lstComponentPrices.begin();
		it != m_lstComponentPrices.end();
		it++, iIndex++)
	{
		ComPriceInfo PriceInfo = *it;
		CString strCount;
		CString strUnitPrice;
		CString strTotalPrice;
		strCount.Format(_T("%d"), PriceInfo.iComCount);
		strUnitPrice.Format(_T("%d"), PriceInfo.iOfferPrice);
		strTotalPrice.Format(_T("%d"), PriceInfo.iComCount * PriceInfo.iOfferPrice);
		SetDlgItemText(m_ComPriceCtrls[iIndex].nNameEdit, PriceInfo.strComName);
		SetDlgItemText(m_ComPriceCtrls[iIndex].nQtyEdit, strCount);
		SetDlgItemText(m_ComPriceCtrls[iIndex].nUnitPriceEdit, strUnitPrice);
		SetDlgItemText(m_ComPriceCtrls[iIndex].nTotalPriceEdit, strTotalPrice);
	}

	CRect rcClient;
	CRect rcBottomCtrl;
	GetClientRect(&rcClient);
	int iBottomCtrlIndex = (iComponentCnt >= 2) ? iComponentCnt-1 : 1;
	GetDlgItem(m_ComPriceCtrls[iBottomCtrlIndex].nCheckBox)->GetWindowRect(&rcBottomCtrl);
	ScreenToClient(&rcBottomCtrl);
	rcClient.bottom = rcBottomCtrl.bottom + 5;
	MoveWindow(&rcClient);
	m_iStdWndHeight = rcClient.Height();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);
	int iScrollLen = m_iStdWndHeight-rcClient.Height();
	if (iScrollLen > 0)	
	{
		SetScrollRange(SB_VERT, 0, iScrollLen);
		EnableScrollBarCtrl(SB_VERT, TRUE);
	}
	else 
	{
		EnableScrollBarCtrl(SB_VERT, FALSE);
	}
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
    switch (nSBCode) 
	{ 
        case SB_BOTTOM: 
            ScrollWindow(0, (scrollinfo.nPos-scrollinfo.nMax)); 
            scrollinfo.nPos = scrollinfo.nMax; 
            SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);	
            break; 
        case SB_TOP: 
            ScrollWindow(0, (scrollinfo.nPos-scrollinfo.nMin)); 
            scrollinfo.nPos = scrollinfo.nMin; 
            SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
            break; 
		case SB_LINEUP: 
            scrollinfo.nPos -= 5; 
			if (scrollinfo.nPos <scrollinfo.nMin) 
			{ 
				scrollinfo.nPos = scrollinfo.nMin; 
				break; 
			} 
			SetScrollInfo(SB_VERT, &scrollinfo,SIF_ALL); 
			ScrollWindow(0, 5);  
			break; 
		case SB_LINEDOWN: 
			scrollinfo.nPos += 5; 
			if (scrollinfo.nPos > scrollinfo.nMax) 
			{ 
				scrollinfo.nPos = scrollinfo.nMax; 
				break; 
			} 
			SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL); 
			ScrollWindow(0, -5); 
			break; 
		case SB_PAGEUP: 
			scrollinfo.nPos -= 10; 
			if (scrollinfo.nPos < scrollinfo.nMin) 
			{ 
				scrollinfo.nPos = scrollinfo.nMin; 
				break; 
			} 
			SetScrollInfo(SB_VERT, &scrollinfo,SIF_ALL); 
			ScrollWindow(0,10); 
			break; 
		case SB_PAGEDOWN: 
			scrollinfo.nPos += 10; 
			if (scrollinfo.nPos > scrollinfo.nMax) 
			{ 
				scrollinfo.nPos = scrollinfo.nMax; 
				break; 
			} 
			SetScrollInfo(SB_VERT, &scrollinfo,SIF_ALL); 
			ScrollWindow(0, -10); 
			break; 
		case SB_ENDSCROLL: 
			break; 
		case SB_THUMBPOSITION: 
			break; 
		case SB_THUMBTRACK: 
			ScrollWindow(0, (scrollinfo.nPos-nPos)); 
			scrollinfo.nPos = nPos; 
			SetScrollInfo(SB_VERT, &scrollinfo,SIF_ALL); 
		break; 
	} 

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck1()
{
	UpdateTotalPrices();
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck2()
{
	UpdateTotalPrices();
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck3()
{
	UpdateTotalPrices();
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck4()
{
	UpdateTotalPrices();
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck5()
{
	UpdateTotalPrices();
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck6()
{
	UpdateTotalPrices();
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck7()
{
	UpdateTotalPrices();
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck8()
{
	UpdateTotalPrices();
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck9()
{
	UpdateTotalPrices();
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::OnBnClickedCheckComponentdlgCheck10()
{
	UpdateTotalPrices();
}
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::SetComponentList(ComponentPriceList lstPriceInfo, CString strCurrency)
{
	m_lstComponentPrices = lstPriceInfo;
	m_strCurrency = strCurrency;
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::GetSelectedComponents(ComponentPriceList &lstSelectedPrice)
{
	lstSelectedPrice.clear();

	int iIndex = 0;
	for (ComponentPriceList::iterator it = m_lstComponentPrices.begin();
		it != m_lstComponentPrices.end();
		it++, iIndex++)
	{
		ComPriceInfo PriceInfo = *it;

		CButton* pBtn = (CButton*)GetDlgItem(m_ComPriceCtrls[iIndex].nCheckBox);
		if (pBtn->GetCheck() == BST_UNCHECKED)
			continue;

		lstSelectedPrice.push_back(PriceInfo);
	}
}
//-----------------------------------------------------------------------
void CBMOfferComponentsInfoDlg::UpdateTotalPrices()
{
	int iTotalPrices = 0;

	int iIndex = 0;
	for (ComponentPriceList::iterator it = m_lstComponentPrices.begin();
		it != m_lstComponentPrices.end();
		it++, iIndex++)
	{
		ComPriceInfo PriceInfo = *it;


		CButton* pBtn = (CButton*)GetDlgItem(m_ComPriceCtrls[iIndex].nCheckBox);
		if (pBtn->GetCheck() == BST_CHECKED)
		{
			iTotalPrices += (PriceInfo.iComCount * PriceInfo.iOfferPrice);
		}
	}
	::PostMessage(m_pParentWnd->m_hWnd, WM_USER_UPDATETOTALPRICE, iTotalPrices, 0);
}
//-----------------------------------------------------------------------