//-----------------------------------------------------------------------
//	File Name:BMOfferCalcEndCarDlg.h
//	Introduce:Class CBMOfferCalcEndCarDlg define header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ColorListCtrl.h"
#include "ColorStatic.h"
#include "BMCalcOptionsDlg.h"
#include "BMSelectAdditionsDlg.h"
#include <atlimage.h>
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcEndCarDlg dialog
//-----------------------------------------------------------------------
class CBMOfferCalcEndCarDlg : public CBMCalcOptionsDlg
{
	DECLARE_DYNAMIC(CBMOfferCalcEndCarDlg)

public:
	CBMOfferCalcEndCarDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferCalcEndCarDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ENDCAR };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnSelect();
	afx_msg void OnBnClickedBtnEndcarCalc();
	afx_msg void OnBnClickedBtnEndcarAddadd();
	afx_msg void OnBnClickedBtnEndcarDeladd();
	afx_msg void OnCbnSelchangeComboEctype();
	afx_msg void OnCbnSelchangeComboEccode();
	afx_msg void OnCbnSelchangeComboEndcarDritype();
	afx_msg void OnCbnSelchangeComboMotorqty();
	//-------------------------------------------------------------------
public:
	BOOL CanDoNextStep();
	//-------------------------------------------------------------------
private:	
	void	InitTablesDataStruct();
	void	InitDataLstControls();
	void	FillComboBoxes();
	void	FillECCodeComboBox();
	void	FillDriCodeComboBox();
	void	FillJoiPlaTypeComboBox();
	void	FillECStructureComboBox();
	void	InitUIWithDefaultData();
	void	InitUIWithDatabaseData();
	void	GetDefaultEndCarData(CString &strECCode, CString &strDriCode, int &iMotorCnt);
	CString	GetJoiPlaTypeString();
	int		GetRailMaxWidth();
	int		GetRailMaxGauage();
	void	UpdateEndcarStructure();
	void	UpdateWheelBase();
	void	UpdateDataBasedOnCodeAndDriver();
	void	UpdateBufferType();
	void	UpdateEndCarDataText();
	void	UpdateMotorPrice();
	void	UpdatePrices();
	//-------------------------------------------------------------------
private:	
	CRect				m_rcEndCar;
	CImage*				m_pEndCarImages[eCraneType_Cnt];
	//
	CImageList			m_imgNULL;
	TableData			m_stuAdditionTable;
	CColorListCtrl		m_lstCtlAdditions;
	CColorStatic		m_staticQResult;
	CColorStatic		m_staticFesult;
	CColorStatic		m_staticMaxSpeedResult;
	CColorStatic		m_staticMaxCapacityResult;
	CColorStatic		m_staticGrooveWidthResult;
	CColorStatic		m_staticRailGauResult;
	CColorStatic		m_staticWheelBaseResult;
	CColorStatic		m_staticRailTypeResult;
	//
	CColorStatic		m_staticEndCarPrice;	
	CColorStatic		m_staticMotorPrice;	
	CColorStatic		m_staticAddPrice;	
	CColorStatic		m_staticTotalPrice;	
	//
	BOOL				m_bShowResult;
	CImage*				m_pPassImage;
	CImage*				m_pNotPassImage;
	BOOL				m_bMaxStreesOK;
	BOOL				m_bMaxBlendOK;
	BOOL				m_bMaxSpeedOK;
	BOOL				m_bMaxCapacityOK;
	BOOL				m_bMaxWidthOK;
	BOOL				m_bRaiGauageOK;
	BOOL				m_bMaxWheBaseOK;
	BOOL				m_bRailTypeOK;
	CRect				m_rcMaxStrees;
	CRect				m_rcMaxBlend;
	CRect				m_rcMaxSpeed;
	CRect				m_rcMaxCapacity;
	CRect				m_rcMaxWidth;
	CRect				m_rcRaiGauage;
	CRect				m_rcMaxWheBase;
	CRect				m_rcRailType;
	int					m_iEndCarPrice;
	int					m_iMotorPrice;
	//-------------------------------------------------------------------
public:
	afx_msg void OnCbnSelchangeComboEndcarRailtype();
	afx_msg void OnCbnSelchangeComboJointplate();
	afx_msg void OnCbnSelchangeComboEndcarMatrial();
	afx_msg void OnCbnSelchangeComboEndcarStructure();
	void SelectedECTypeIsChanged();
	void SelectedECCodeIsChanged();
	void SelectedECDriverIsChanged();
	void SelectedMotorQtyIsChanged();
	void SelectedRailTypeIsChanged();
};
//-----------------------------------------------------------------------
