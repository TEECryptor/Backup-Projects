//-----------------------------------------------------------------------
//	File Name:BMOfferContactsInfoDlg.cpp
//	Introduce:Class CBMOfferContactsInfoDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferContactsInfoDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferContactsInfoDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferContactsInfoDlg, CDialog)
CBMOfferContactsInfoDlg::CBMOfferContactsInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferContactsInfoDlg::IDD, pParent)
	, m_strCompany(_T(""))
	, m_strCustomer(_T(""))
	, m_strCusPhone(_T(""))
	, m_strCusMobile(_T(""))
	, m_strCusFax(_T(""))
	, m_strCusEmail(_T(""))
	, m_strOfferNo(_T(""))
	, m_strSales(_T(""))
	, m_strSalPhone(_T(""))
	, m_strSalMobile(_T(""))
	, m_strSalFax(_T(""))
	, m_strSalEmail(_T(""))
{

}
//-----------------------------------------------------------------------
CBMOfferContactsInfoDlg::~CBMOfferContactsInfoDlg()
{
	DestroyWindow();
}
//-----------------------------------------------------------------------
void CBMOfferContactsInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONTACTSDLG_COMPANY, m_strCompany);
	DDX_Text(pDX, IDC_EDIT_CONTACTSDLG_CUSTOMER, m_strCustomer);
	DDX_Text(pDX, IDC_EDIT_CONTACTSDLG_CUSPHONE, m_strCusPhone);
	DDX_Text(pDX, IDC_EDIT_CONTACTSDLG_CUSMOBILE, m_strCusMobile);
	DDX_Text(pDX, IDC_EDIT_CONTACTSDLG_CUSFAX, m_strCusFax);
	DDX_Text(pDX, IDC_EDIT_CONTACTSDLG_CUSEMAIL, m_strCusEmail);
	DDX_Text(pDX, IDC_EDIT_CONTACTSDLG_OFFERNO, m_strOfferNo);
	DDX_Text(pDX, IDC_EDIT_CONTACTSDLG_SALES, m_strSales);
	DDX_Text(pDX, IDC_EDIT_CONTACTSDLG_SALPHONE, m_strSalPhone);
	DDX_Text(pDX, IDC_EDIT_CONTACTSDLG_SALMOBILE, m_strSalMobile);
	DDX_Text(pDX, IDC_EDIT_CONTACTSDLG_SALFAX, m_strSalFax);
	DDX_Text(pDX, IDC_EDIT_CONTACTSDLG_SALEMAIL, m_strSalEmail);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferContactsInfoDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferContactsInfoDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferContactsInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rcClient;
	GetClientRect(&rcClient);
	m_iStdWndHeight = rcClient.Height();

	GetCustomerAndSales();
	GetCustomerContacts();
	GetSalesContacts();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferContactsInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
}
//-----------------------------------------------------------------------
void CBMOfferContactsInfoDlg::OnSize(UINT nType, int cx, int cy)
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
void CBMOfferContactsInfoDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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


//-----------------------------------------------------------------------
void CBMOfferContactsInfoDlg::SetOfferNo(CString strOfferNo)
{
	m_strOfferNo = strOfferNo;
}
//-----------------------------------------------------------------------
void CBMOfferContactsInfoDlg::UpdateSales(CString strSalesName)
{
	m_strSales = strSalesName;
	GetSalesContacts();
	UpdateData(FALSE);
}
//-----------------------------------------------------------------------
void CBMOfferContactsInfoDlg::GetCustomerAndSales()
{
	CString strOperations;
	CADOTable tbOfferInfo;
	strOperations.Format(_T("SELECT * FROM %s WHERE OfferNo='%s'"), g_tcTableName[eTableID_OffersInfo], m_strOfferNo);
	if (!tbOfferInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOperations) || tbOfferInfo.GetRecordCnt() == 0)
		return;

	TCHAR tcTemp[128] = {0};
	tbOfferInfo.GetRecordCell(0, L"CompanyShortName", tcTemp, 128);	m_strCompanyAlias = tcTemp;
	tbOfferInfo.GetRecordCell(0, L"ContactName", tcTemp, 128);		m_strCustomer = tcTemp;	
	tbOfferInfo.GetRecordCell(0, L"Employee", tcTemp, 128);			m_strSales = tcTemp;

	tbOfferInfo.CloseTable();
}
//-----------------------------------------------------------------------
void CBMOfferContactsInfoDlg::GetCustomerContacts()
{
	CString strOperations;
	CADOTable tbCustomerInfo;
	strOperations.Format(_T("SELECT * FROM %s WHERE CompanyShortName='%s'"), g_tcTableName[eTableID_CustomerInfo], m_strCompanyAlias);
	if (!tbCustomerInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOperations) || tbCustomerInfo.GetRecordCnt() == 0)
		return;

	TCHAR tcTemp[128] = {0};
	CString strCompanyID;
	tbCustomerInfo.GetRecordCell(0, L"CompanyNo", tcTemp, 128);	strCompanyID = tcTemp;
	tbCustomerInfo.GetRecordCell(0, L"FullName", tcTemp, 128);	m_strCompany = tcTemp;
	tbCustomerInfo.GetRecordCell(0, L"Fox", tcTemp, 128);		m_strCusFax = tcTemp;
	tbCustomerInfo.CloseTable();

	
	CADOTable tbContactsInfo;
	strOperations.Format(_T("SELECT * FROM %s WHERE CompanyNo='%s' AND ContactName='%s'"), g_tcTableName[eTableID_ContactsInfo], strCompanyID, m_strCustomer);
	if (!tbContactsInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOperations) || tbContactsInfo.GetRecordCnt() == 0)
		return;

	tbContactsInfo.GetRecordCell(0, L"Tel", tcTemp, 128);		m_strCusPhone = tcTemp;
	tbContactsInfo.GetRecordCell(0, L"CellPhone", tcTemp, 128);	m_strCusMobile = tcTemp;
	tbContactsInfo.GetRecordCell(0, L"Email", tcTemp, 128);		m_strCusEmail = tcTemp;
	tbContactsInfo.CloseTable();
}
//-----------------------------------------------------------------------
void CBMOfferContactsInfoDlg::GetSalesContacts()
{
	CString strOperations;
	CADOTable tbEmployeeInfo;
	strOperations.Format(_T("SELECT * FROM %s WHERE UserName='%s'"), g_tcTableName[eTableID_EmployeeInfo], m_strSales);
	if (!tbEmployeeInfo.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOperations) || tbEmployeeInfo.GetRecordCnt() == 0)
		return;

	TCHAR tcTemp[128] = {0};
	tbEmployeeInfo.GetRecordCell(0, L"Phone", tcTemp, 128);		m_strSalPhone = tcTemp;
	tbEmployeeInfo.GetRecordCell(0, L"Mobile", tcTemp, 128);	m_strSalMobile = tcTemp;
	tbEmployeeInfo.GetRecordCell(0, L"Email", tcTemp, 128);		m_strSalEmail = tcTemp;
	tbEmployeeInfo.GetRecordCell(0, L"Fax", tcTemp, 128);		m_strSalFax = tcTemp;
	tbEmployeeInfo.CloseTable();
}
//-----------------------------------------------------------------------
