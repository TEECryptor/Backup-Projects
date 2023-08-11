//-----------------------------------------------------------------------
//	File Name:BMOfferCalcMotorDlg.h
//	Introduce:Class CBMOfferCalcMotorDlg define header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "BMParamCalculator.h"
#include "BMOfferCalcMotorParamTabDlg.h"
#include "..\GridCtrl\GridCtrl.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcMotorDlg dialog
//-----------------------------------------------------------------------
class CBMOfferCalcMotorDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferCalcMotorDlg)

public:
	CBMOfferCalcMotorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferCalcMotorDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CALCMOTOR };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnMotorcalcReset();
	afx_msg void OnBnClickedBtnMotorcalcCalc();
	//-------------------------------------------------------------------
public:
	void	SetParamCalculator(CBMParamCalculator* pCalculator);
	//-------------------------------------------------------------------
private:
	void	InitDataWithDefaultSetting();
	float	GetMO10();
	float	GetMO12();
	float	GetMO15();
	float	GetMO17();
	float	GetMO18();
	//-------------------------------------------------------------------
private:
	CBMParamCalculator*		m_pParamCalculator;
	CGridCtrl*				m_pGridCtrl;
	CBMOfferCalcMotorParamTabDlg*	m_pParamTabDlg;
	//-------------------------------------------------------------------
public:
	afx_msg void OnTcnSelchangeTabParams(NMHDR *pNMHDR, LRESULT *pResult);
};
//-----------------------------------------------------------------------
