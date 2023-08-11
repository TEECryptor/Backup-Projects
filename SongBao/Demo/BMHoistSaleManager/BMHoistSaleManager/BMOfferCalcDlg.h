//-----------------------------------------------------------------------
//	File Name:BMOfferCalcDlg.h
//	Introduce:Class CBMOfferCalcDlg header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "OptionTabCtrl.h"
#include "BMOfferCalcOptionsDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
//	Name:BMOfferCalcDlg
//	Inroduce:CBMOfferCalcDlg dialog
//-----------------------------------------------------------------------
class CBMOfferCalcDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferCalcDlg)

public:
	CBMOfferCalcDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferCalcDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OFFERCALC };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnBack();
	afx_msg void OnBnClickedBtnNext();
	afx_msg HRESULT OnOpterationPageChanged(WPARAM wParam, LPARAM lParam);		
	//-------------------------------------------------------------------
public:
	void SetOfferNo(CString strOfferNo);
	//-------------------------------------------------------------------
private:
	void InitCaptionText();
	void InitComponentTabCtrl();
	//-------------------------------------------------------------------
private:
	COptionTabCtrl	m_tabComponents;
	CFont			m_tabFont;
	COLORREF		m_clrBackground;
	CString			m_strOfferNo;
	CADOTable		m_tbComponentInfo;
	//
	int							m_iCurTabIndex;
	BOOL						m_bComponentIsOnlyHoist[MAX_COMPONENT_CNT];
	CString						m_strComponentName[MAX_COMPONENT_CNT];
	CBMOfferCalcOptionsDlg*		m_pCalcOptionsDlg[MAX_COMPONENT_CNT];
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
