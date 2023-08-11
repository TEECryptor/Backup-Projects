//-----------------------------------------------------------------------
//	File Name:BMCalcOptionsDlg.cpp
//	Introduce:Class CBMCalcOptionsDlg implementation file
//-----------------------------------------------------------------------
#include "StdAfx.h"
#include "BMCalcOptionsDlg.h"
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
CBMCalcOptionsDlg::CBMCalcOptionsDlg(UINT nIDTemplate, CWnd* pParentWnd)
	: CDialog(nIDTemplate, pParentWnd)
	, m_pParamCalculator(NULL)
	, m_bDataIsInitliazed(FALSE)
{
}
//-----------------------------------------------------------------------
CBMCalcOptionsDlg::~CBMCalcOptionsDlg(void)
{
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMCalcOptionsDlg::SetParamCalculator(CBMParamCalculator* pCalulator)
{
	 m_pParamCalculator = pCalulator;
}
//-----------------------------------------------------------------------