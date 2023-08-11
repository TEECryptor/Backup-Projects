//-----------------------------------------------------------------------
//	File Name:BMOfferCalcHoistDlg.h
//	Introduce:Class CBMOfferCalcHoistDlg define header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ColorListCtrl.h"
#include "ColorStatic.h"
#include "OptionTabCtrl.h"
#include "BMCalcOptionsDlg.h"
#include "BMHoistLayotsDlg.h"
#include "BMSelectAdditionsDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcHoistDlg dialog
//-----------------------------------------------------------------------
class CBMOfferCalcHoistDlg : public CBMCalcOptionsDlg
{
	DECLARE_DYNAMIC(CBMOfferCalcHoistDlg)
public:
	CBMOfferCalcHoistDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferCalcHoistDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_HOIST };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelchangeComboHoistBrand();
	afx_msg void OnCbnSelchangeComboReevingtype();
	afx_msg void OnCbnSelchangeComboStock();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnNMClickListHoist(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnTcnSelchangeTabLayout(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnEnChangeEditAmount();
	afx_msg void OnBnClickedBtnAuxhoist();
	afx_msg void OnBnClickedBtnParameter();
	//-------------------------------------------------------------------
private:	
	void	InitTablesDataStruct();
	void	InitDataLstControls();
	void	InitUIWithDefaultData();
	void	InitUIWithDatabaseData();
	void	OpenAndShowHoistTable(CString strQuery = _T(""));
	CString	GetQuerySQLText(BOOL bIncludeRailGau = FALSE, int iRailGua = 0);
	void	FillDefaultStdAddtions();
	CString	GetCurHoistTableName();
	void	ParserSelHoistData();
	void	UpdatePrices();
	void	UpdateAdditionTechData();
	void	UpdateRailGauageWithAdd(CString strSelHoistGauage, int iAddGauageLevel = 0);
	//-------------------------------------------------------------------
public:
	BOOL	CanDoNextStep();
	//-------------------------------------------------------------------
private:	
	CImageList			m_imgNULL;
	TableData			m_stuHoistTable;
	TableData			m_stuAdditionTable;
	CADOTable			m_tbHoistSel;
	CColorListCtrl		m_lstCtlHoists;
	CColorListCtrl		m_lstCtlAdditions;
	CColorStatic		m_staticHoistCode;
	CColorStatic		m_staticHoistPrice;
	CColorStatic		m_staticAuxHoistCode;
	CColorStatic		m_staticAuxHoistPrice;
	CColorStatic		m_staticTotalPrice;
	CColorStatic		m_staticAddPrice;
	CColorStatic		m_staticCount;
	CColorStatic		m_staticUnitPrice;
	//COptionTabCtrl		m_tabHoistLayout;
	//CBMHoistLayotsDlg*	m_pHoistLayoutDlg;
	//
	eCraneType			m_eHoistType;
	CString				m_strLastCraneType;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
