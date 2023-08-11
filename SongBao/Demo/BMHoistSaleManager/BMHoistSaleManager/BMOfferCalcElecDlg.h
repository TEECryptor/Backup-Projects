//-----------------------------------------------------------------------
//	File Name:BMOfferCalcElecDlg.h
//	Introduce:Class CBMOfferCalcElecDlg define header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ColorListCtrl.h"
#include "ColorStatic.h"
#include "BMCalcOptionsDlg.h"
#include "BMSelectAdditionsDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcElecDlg dialog
//-----------------------------------------------------------------------
class CBMOfferCalcElecDlg : public CBMCalcOptionsDlg
{
	DECLARE_DYNAMIC(CBMOfferCalcElecDlg)

public:
	CBMOfferCalcElecDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferCalcElecDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ELECTRICAL };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnElecAddadd();
	afx_msg void OnBnClickedBtnElecDeladd();
	afx_msg void OnBnClickedBtnElecCalc();
	//-------------------------------------------------------------------
public:
	BOOL CanDoNextStep();
	//-------------------------------------------------------------------
private:	
	void	InitTablesDataStruct();
	void	InitDataLstControls();
	void	FillComboBoxes();
	void	FillPannelSizeComboBox();
	void	InitUIWithDefaultData();
	void	InitUIWithDatabaseData();
	void	UpdatePrices();
	//
	CString	GetCranePannelCode();
	CString	GetFastoonCode();
	CString	GetOperterUnitCode();
	CString	GetCranePowSupplyCode();
	//
	int		GetCranePannelPrice();
	int		GetFastoonPrice();
	int		GetOperterUnitPrice();
	int		GetCranePowSupplyPrice();
	//-------------------------------------------------------------------
private:	
	CImageList			m_imgNULL;
	TableData			m_stuAdditionTable;
	CColorListCtrl		m_lstCtlAdditions;
	//
	CColorStatic		m_staticAddPriceEL;	
	CColorStatic		m_staticAddPriceEFS;	
	CColorStatic		m_staticAddPriceEOP;	
	CColorStatic		m_staticAddPriceECP;	
	CColorStatic		m_staticPannelPrice;	
	CColorStatic		m_staticFastoonPrice;		
	CColorStatic		m_staticOperterUnitPrice;	
	CColorStatic		m_staticPowerSupplyPrice;	
	CColorStatic		m_staticTotalPrice;	
	//
	int					m_iAddPriceEL;
	int					m_iAddPriceEFS;
	int					m_iAddPriceEOP;
	int					m_iAddPriceECP;
	//-------------------------------------------------------------------
public:
	afx_msg void OnCbnSelchangeComboElecPannelinverter();
	afx_msg void OnCbnSelchangeComboElecPannelsize();
	afx_msg void OnCbnSelchangeComboElecPannelselection();
	afx_msg void OnCbnSelchangeComboElecPanneltransformer();
	afx_msg void OnCbnSelchangeComboElecFtnpwrtype();
	afx_msg void OnCbnSelchangeComboElecFtnctrltype();
	afx_msg void OnCbnSelchangeComboElecFtnselection();
	afx_msg void OnCbnSelchangeComboElecOuctrltype();
	afx_msg void OnCbnSelchangeComboElecOubtnnumber();
	afx_msg void OnCbnSelchangeComboElecHornbtn();
	afx_msg void OnCbnSelchangeComboElecOuselection();
	afx_msg void OnCbnSelchangeComboElecOuconnecttype();
	afx_msg void OnCbnSelchangeComboElecPowpowtype();
	afx_msg void OnCbnSelchangeComboElecPowbracket();
	afx_msg void OnCbnSelchangeComboElecPowrailbeamwdt();
};
//-----------------------------------------------------------------------
