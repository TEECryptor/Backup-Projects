//-----------------------------------------------------------------------
//	File Name:BMOfferSelectDlg.h
//	Introduce:Class CBMOfferSelectDlg header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "OptionTabCtrl.h"
#include "BMOfferOptionsDlg.h"
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CBMOfferSelectDlg dialog
//-----------------------------------------------------------------------
class CBMOfferSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferSelectDlg)

public:
	CBMOfferSelectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferSelectDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OFFERSELECT };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeTabOfferoptions(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboCurrency();
	afx_msg void OnEnChangeEditRate();
	afx_msg void OnEnChangeEditYat();
	afx_msg void OnBnClickedBtnOffer();
	HRESULT OnUpdateTotalPrice(WPARAM wParam, LPARAM lParam);
	//-------------------------------------------------------------------
public:
	void SetOfferNo(CString strOfferNo);
	//-------------------------------------------------------------------
private:
	void InitCaptionText();
	void InitComponentTabCtrl();
	//-------------------------------------------------------------------
private:
	CString					m_strOfferNo;
	CString					m_strProjectname;
	CString					m_strDlgCaption;
	COptionTabCtrl			m_tabOptions;
	CBMOfferOptionsDlg*		m_pOfferOptionsDlg[MAX_COMPONENT_CNT];
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
