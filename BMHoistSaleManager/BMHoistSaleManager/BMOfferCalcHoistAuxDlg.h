//-----------------------------------------------------------------------
//	File Name:BMOfferCalcHoistAuxDlg.h
//	Introduce:Class CBMOfferCalcHoistAuxDlg define header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ColorListCtrl.h"
#include "ColorStatic.h"
#include "BMParamCalculator.h"
#include "BMSelectAdditionsDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcHoistAuxDlg dialog
//-----------------------------------------------------------------------
class CBMOfferCalcHoistAuxDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferCalcHoistAuxDlg)
public:
	CBMOfferCalcHoistAuxDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferCalcHoistAuxDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_HOISTAUX };
	//-------------------------------------------------------------------

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboAuxhoistLoad();
	afx_msg void OnCbnSelchangeComboAuxhoistBrand();
	afx_msg void OnBnClickedBtnAuxhoistAdd();
	afx_msg void OnBnClickedBtnAuxhoistFind();
	afx_msg void OnBnClickedBtnAuxhoistDel();
	afx_msg void OnBnClickedBtnAuxhoistParameter();
	afx_msg void OnNMClickListAuxhoist(NMHDR *pNMHDR, LRESULT *pResult);
	//-------------------------------------------------------------------
public:
	void	SetParamCalculator(CBMParamCalculator* pCalculator);
	//-------------------------------------------------------------------
private:
	void	InitLoadComobox();
	void	InitBrandComobox();
	void	InitTablesDataStruct();
	void	InitDataLstControls();	
	void	ParserSelHoistData();
	void	UpdatePrices();
	//-------------------------------------------------------------------
private:
	CImageList			m_imgNULL;
	TableData			m_stuHoistTable;
	TableData			m_stuAdditionTable;
	CADOTable			m_tbHoistSel;
	CColorListCtrl		m_lstCtlHoists;
	CColorListCtrl		m_lstCtlAdditions;
	CColorStatic		m_staticAuxHoistCode;
	CColorStatic		m_staticAuxHoistPrice;
	CColorStatic		m_staticAuxHoistAddPrice;
	CColorStatic		m_staticAuxHoistTotalPrice;
	//
	CBMParamCalculator*		m_pParamCalculator;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
