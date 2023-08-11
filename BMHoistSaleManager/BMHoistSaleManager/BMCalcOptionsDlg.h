//-----------------------------------------------------------------------
//	File Name:BMCalcOptionsDlg.h
//	Introduce:header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "afxwin.h"
#include "BMParamCalculator.h"
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
class CBMCalcOptionsDlg :
	public CDialog
{
public:
	CBMCalcOptionsDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	~CBMCalcOptionsDlg(void);
	//-------------------------------------------------------------------
public:
	virtual BOOL CanDoNextStep() = 0;
	virtual void SetParamCalculator(CBMParamCalculator* pCalulator);
	//-------------------------------------------------------------------
protected:
	BOOL					m_bDataIsInitliazed;
	CBMParamCalculator*		m_pParamCalculator;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
