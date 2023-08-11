//----------------------------------------------------
//	File Name: AFUProductDlg.cpp
//	Introduce: Class CAFUProductDlg implement file
//
//----------------------------------------------------
#include "stdafx.h"
#include "AFUManager.h"
#include "AFUProductDlg.h"
//----------------------------------------------------


//----------------------------------------------------
//	Name:CAFUProductDlg
//	Introduce:Construction
//----------------------------------------------------
IMPLEMENT_DYNAMIC(CAFUProductDlg, CDialog)
CAFUProductDlg::CAFUProductDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAFUProductDlg::IDD, pParent)
{

}
//----------------------------------------------------
CAFUProductDlg::~CAFUProductDlg()
{
}
//----------------------------------------------------
void CAFUProductDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//----------------------------------------------------
BEGIN_MESSAGE_MAP(CAFUProductDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAFUProductDlg::OnBnClickedOk)
END_MESSAGE_MAP()
//----------------------------------------------------


//----------------------------------------------------
// CAFUProductDlg message handlers
//----------------------------------------------------
BOOL CAFUProductDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strPrice;
	CString strCount;
	strPrice.Format(_T("%.2f"), m_fPrice);
	strCount.Format(_T("%d"), m_lCount);
	SetDlgItemText(IDC_EDIT_GOODSID, m_strProductID);
	SetDlgItemText(IDC_EDIT_NAME, m_strProductName);
	SetDlgItemText(IDC_EDIT_PRICE, strPrice);
	SetDlgItemText(IDC_EDIT_COUNT, strCount);
	SetDlgItemText(IDC_EDIT_COMMENT, m_strOther);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//----------------------------------------------------
void CAFUProductDlg::OnBnClickedOk()
{	
	CString strPrice;
	CString strCount;
	GetDlgItemText(IDC_EDIT_GOODSID, m_strProductID);
	GetDlgItemText(IDC_EDIT_NAME, m_strProductName);
	GetDlgItemText(IDC_EDIT_PRICE, strPrice);
	GetDlgItemText(IDC_EDIT_COUNT, strCount);
	GetDlgItemText(IDC_EDIT_COMMENT, m_strOther);
	m_fPrice = (float)_tstof(strPrice);
	m_lCount = _ttol(strCount);

	OnOK();
}
//----------------------------------------------------


//----------------------------------------------------
void CAFUProductDlg::SetInitializeData(CString strProductID, 
									   CString strProductName, 
									   float fPrice, 
									   long lCount, 
									   CString strOther)
{
	m_strProductID = strProductID;
	m_strProductName = strProductName;
	m_fPrice = fPrice;
	m_lCount = lCount;
	m_strOther = strOther;
}		
//----------------------------------------------------
void CAFUProductDlg::GetUpdatedData(CString &strProductID, 
									CString &strProductName, 
									float &fPrice, 
									long &lCount, 
									CString &strOther)
{
	strProductID = m_strProductID;
	strProductName = m_strProductName;
	fPrice = m_fPrice;
	lCount = m_lCount;
	strOther = m_strOther;
}
//----------------------------------------------------

