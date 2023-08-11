//-----------------------------------------------------------------------
//	File Name:BMOfferContactsInfoDlg.h
//	Introduce:Class CBMOfferContactsInfoDlg header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
struct ComponentPriceCtrls
{
	UINT	nCheckBox;
	UINT	nNameEdit;
	UINT	nQtyEdit;
	UINT	nUnitPriceEdit;
	UINT	nUnitUnitStatic;
	UINT	nTotalPriceEdit;
	UINT	nTotalUnitStatic;
};
//-----------------------------------------------------------------------
// CBMOfferComponentsInfoDlg dialog
//-----------------------------------------------------------------------
class CBMOfferComponentsInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferComponentsInfoDlg)

public:
	CBMOfferComponentsInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferComponentsInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OFFERCOMPONENTS };
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
	void	SetComponentList(ComponentPriceList lstPriceInfo, CString strCurrency);
	void	GetSelectedComponents(ComponentPriceList &lstSelectedPrice);
	//-------------------------------------------------------------------
private:
	void	UpdateTotalPrices();
	//-------------------------------------------------------------------
private:
	int		m_iStdWndHeight;
	CString	m_strCurrency;
	ComponentPriceList	m_lstComponentPrices;
	ComponentPriceCtrls	m_ComPriceCtrls[MAX_COMPONENT_CNT];
	//-------------------------------------------------------------------
public:
	afx_msg void OnBnClickedCheckComponentdlgCheck1();
	afx_msg void OnBnClickedCheckComponentdlgCheck2();
	afx_msg void OnBnClickedCheckComponentdlgCheck3();
	afx_msg void OnBnClickedCheckComponentdlgCheck4();
	afx_msg void OnBnClickedCheckComponentdlgCheck5();
	afx_msg void OnBnClickedCheckComponentdlgCheck6();
	afx_msg void OnBnClickedCheckComponentdlgCheck7();
	afx_msg void OnBnClickedCheckComponentdlgCheck8();
	afx_msg void OnBnClickedCheckComponentdlgCheck9();
	afx_msg void OnBnClickedCheckComponentdlgCheck10();
};
//-----------------------------------------------------------------------
