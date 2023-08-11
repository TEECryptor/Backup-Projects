//-----------------------------------------------------------------------
//	File Name:BMOfferContactsInfoDlg.h
//	Introduce:Class CBMOfferContactsInfoDlg header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferContactsInfoDlg dialog
//-----------------------------------------------------------------------
class CBMOfferContactsInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferContactsInfoDlg)

public:
	CBMOfferContactsInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferContactsInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OFFERCONTACTS };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//-------------------------------------------------------------------
public:
	void	SetOfferNo(CString strOfferNo);
	void	UpdateSales(CString strSalesName);
	//-------------------------------------------------------------------
private:
	void	GetCustomerAndSales();
	void	GetCustomerContacts();
	void	GetSalesContacts();
	//-------------------------------------------------------------------
public:
	CString m_strCompany;
	CString m_strCustomer;
	CString m_strCusPhone;
	CString m_strCusMobile;
	CString m_strCusFax;
	CString m_strCusEmail;
	CString	m_strOfferNo;
	CString m_strSales;
	CString m_strSalPhone;
	CString m_strSalMobile;
	CString m_strSalFax;
	CString m_strSalEmail;
	//-------------------------------------------------------------------
private:
	int		m_iStdWndHeight;
	CString m_strCompanyAlias;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
