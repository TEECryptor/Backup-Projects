//-----------------------------------------------------------------------
//	File Name:BMOfferCalcRailDlg.h
//	Introduce:Class CBMOfferCalcRailDlg define header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "BMParamCalculator.h"
#include "ColorStatic.h"
#include <atlimage.h>
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcRailDlg dialog
//-----------------------------------------------------------------------
class CBMOfferCalcRailDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferCalcRailDlg)

public:
	CBMOfferCalcRailDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferCalcRailDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CACLRAIL };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeComboCalcrailRailtype();
	afx_msg void OnCbnSelchangeComboCalcrailRaildir();
	afx_msg void OnCbnSelchangeComboCalcrailSpeed();
	afx_msg void OnEnKillfocusEditCalcrailMaxwhepress();
	afx_msg void OnEnKillfocusEditCalcrailMinwhepress();
	afx_msg void OnBnClickedBtnRailcalcReset();
	afx_msg void OnBnClickedBtnRailcalcCalc();
	//-------------------------------------------------------------------
public:
	void	SetParamCalculator(CBMParamCalculator* pCalculator);
	//-------------------------------------------------------------------
private:
	void	FillRailTypeCombobox();
	void	FillRailDirCombobox();
	void	FillRailSpeedCombobox();
	void	InitDataWithDefaultSetting();
	float	GetRA10();
	float	GetRA11();
	float	GetRA13();
	void	DoRailCalculation();
	//-------------------------------------------------------------------
private:
	BOOL					m_bMaxPressOK;
	BOOL					m_bAvgPressOK;
	CImage*					m_pPassImage;
	CImage*					m_pNotPassImage;
	CRect					m_rcMaxPress;
	CRect					m_rcAvgPress;
	CColorStatic			m_staticMaxPressResult;
	CColorStatic			m_staticAvgPressResult;
	CBMParamCalculator*		m_pParamCalculator;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------