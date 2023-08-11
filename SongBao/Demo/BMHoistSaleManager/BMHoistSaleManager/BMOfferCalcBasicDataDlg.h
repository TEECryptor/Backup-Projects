//-----------------------------------------------------------------------
//	File Name:BMOfferCalcBasicDataDlg.h
//	Introduce:Class CBMOfferCalcBasicDataDlg header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "BMCalcOptionsDlg.h"
#include <atlimage.h>
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcBasicDataDlg dialog
//-----------------------------------------------------------------------
class CBMOfferCalcBasicDataDlg : public CBMCalcOptionsDlg
{
	DECLARE_DYNAMIC(CBMOfferCalcBasicDataDlg)

public:
	CBMOfferCalcBasicDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferCalcBasicDataDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BASICDATA };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnCbnSelchangeComboCtrltype();
	afx_msg void OnCbnSelchangeComboGaitype();
	afx_msg void OnEnKillfocusEditCraneload();
	afx_msg void OnEnKillfocusEditHeigth();
	afx_msg void OnEnKillfocusEditLength();
	afx_msg void OnEnKillfocusEditToptofloor();
	afx_msg void OnEnKillfocusEditRooftotop();
	//-------------------------------------------------------------------
private:
	void	FillComboBoxes();
	void	FillHoistTypeComboBox();
	void	FillHoistSpeedComboBox();
	void	FillCraneCtrlTypeComboBox();
	void	InitUIWithDefaultData();
	void	InitUIWithDatabaseData();
	void	UpdateControlsStatus(eCraneType eCraneType);
	//-------------------------------------------------------------------
public:
	BOOL	CanDoNextStep();
	//-------------------------------------------------------------------
private:
	eCraneType		m_eCurCraneType;
	eCraneCtrlType	m_eCurCtrlType;
	BOOL		m_bIsClosing;
	int			m_iClearance;
	CImage*		m_pHoistImages[eCraneType_Cnt];
	CImage*		m_pCtrlTypeImages[eCraneCtrlType_Cnt];
	int			m_iHoistParams[5];
	CRect		m_rcHoistImage;
	CRect		m_rcCtrlTypes[eCraneCtrlType_Cnt];
	//
	CString		m_strRail;
	CString		m_strMin;
	CString		m_strSpan;
	CString		m_strLoad;
	CString		m_strHOL;
	CString		m_strClearance;
	CString		m_strTopToFloor;
	CString		m_strTopToRoof;
	CString		m_strTopToCraneTop;
	//
	LOGFONT		m_lFont;
	CRect		m_rcRail;
	CRect		m_rcMin;
	CRect		m_rcSpan;
	CRect		m_rcLoad;
	CRect		m_rcHOL;
	CRect		m_rcClearance;
	CRect		m_rcTopToFloor;
	CRect		m_rcTopToRoof;
	CRect		m_rcTopToCraneTop;
	//-------------------------------------------------------------------
public:
	afx_msg void OnCbnSelchangeComboHoisttype();
	afx_msg void OnCbnSelchangeComboHoistload();
	afx_msg void OnCbnSelchangeComboLevel();
	afx_msg void OnCbnSelchangeComboHoistlevel();
	afx_msg void OnCbnSelchangeComboPower();
	afx_msg void OnCbnSelchangeComboCtrlvol();
	afx_msg void OnCbnSelchangeComboWorkingmode();
	afx_msg void OnCbnSelchangeComboSpeed();
	afx_msg void OnCbnSelchangeComboRaiseCtltype();
	afx_msg void OnCbnSelchangeComboMovespeed();
	afx_msg void OnCbnSelchangeComboMoveCtltype();
	afx_msg void OnCbnSelchangeComboMaincarspeed();
	afx_msg void OnCbnSelchangeComboMaincarCtltype();
};
//-----------------------------------------------------------------------
