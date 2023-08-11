//-----------------------------------------------------------------------
//	File Name:BMOfferCalcOptionsDlg.h
//	Introduce:Class CBMOfferCalcOptionsDlg header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "OptionTabCtrl.h"
#include "BMOfferCalcBasicDataDlg.h"
#include "BMOfferCalcHoistDlg.h"
#include "BMOfferCalcMainCarDlg.h"
#include "BMOfferCalcEndCarDlg.h"
#include "BMOfferCalcElecDlg.h"
#include "BMOfferCalcTechnicalDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcOptionsDlg dialog
//-----------------------------------------------------------------------
class CBMOfferCalcOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferCalcOptionsDlg)

public:
	CBMOfferCalcOptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferCalcOptionsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CALCOPTIONS };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTabOptions(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	//-------------------------------------------------------------------
public:
	void	SetInitInformation(BOOL	bIsOnlyHoist, CString strOfferNo, CString strComName);
	BOOL	DoBackStep();
	BOOL	DoNextStep();
	//-------------------------------------------------------------------
private:
	void	UpdateControlsPos();
	//-------------------------------------------------------------------
public:
	HWND	m_hMsgParent;
	//-------------------------------------------------------------------
private:
	COptionTabCtrl		m_tabOptions;
	COLORREF			m_clrBackground;
	CFont				m_tabFont;
	CString				m_strOfferNo;
	CString				m_strComName;
	//
	int					m_iLastSelTab;
	CBMCalcOptionsDlg*	m_pCurDialog;
	CBMCalcOptionsDlg*	m_pBasicDataDlg;
	CBMCalcOptionsDlg*	m_pHoistDataDlg;
	CBMCalcOptionsDlg*	m_pMainCarDataDlg;
	CBMCalcOptionsDlg*	m_pEndCarDataDlg;
	CBMCalcOptionsDlg*	m_pElectricalDlg;
	CBMCalcOptionsDlg*	m_pTechnicalDlg;
	CBMParamCalculator	m_CraneCalculator;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
