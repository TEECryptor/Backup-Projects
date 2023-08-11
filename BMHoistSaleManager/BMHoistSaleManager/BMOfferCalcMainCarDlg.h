//-----------------------------------------------------------------------
//	File Name:BMOfferCalcMainCarDlg.h
//	Introduce:Class CBMOfferCalcMainCarDlg define header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ColorStatic.h"
#include "BMCalcOptionsDlg.h"
#include <atlimage.h>
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcMainCarDlg dialog
//-----------------------------------------------------------------------
class CBMOfferCalcMainCarDlg : public CBMCalcOptionsDlg
{
	DECLARE_DYNAMIC(CBMOfferCalcMainCarDlg)

public:
	CBMOfferCalcMainCarDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferCalcMainCarDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MAINCAR };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnSelect();
	afx_msg void OnEnKillfocusEditH1();
	afx_msg void OnCbnSelchangeComboGirderT3();
	afx_msg void OnEnKillfocusEditH2();
	afx_msg void OnBnClickedBtnCalc();
	afx_msg void OnBnClickedBtnParam();
	afx_msg void OnEnChangeEditCostt();
	//-------------------------------------------------------------------
public:
	BOOL CanDoNextStep();
	//-------------------------------------------------------------------
private:	
	void	FillComboBoxs();
	void	InitUIWithDefaultData();
	void	InitUIWithDatabaseData();
	void	UpdateUserSelection();
	void	UpdateTotalPrice();
	//-------------------------------------------------------------------
private:	
	CRect		m_rcGiederImage;
	CRect		m_rcSideImage;
	CImage*		m_pGiederImages[eCraneType_Cnt];
	CImage*		m_pSideImages[eCraneType_Cnt];
	CImage*		m_pResultImages[eCalcResultType_Cnt];
	//
	BOOL		m_bShowResultImages;
	CRect		m_rcImageDelta0;
	CRect		m_rcImageDelta1;
	CRect		m_rcImageDelta2;
	CRect		m_rcImageTuo;
	CRect		m_rcImageF;
	CRect		m_rcImageS;
	BOOL		m_bResultQ0_Pass;
	BOOL		m_bResultQ1_Pass;
	BOOL		m_bResultQ2_Pass;
	BOOL		m_bResultT_Pass;
	BOOL		m_bResultF_Pass;
	BOOL		m_bResultS_Pass;
	CColorStatic	m_Static_Q0;
	CColorStatic	m_Static_Q1;
	CColorStatic	m_Static_Q2;
	CColorStatic	m_Static_T;
	CColorStatic	m_Static_F;
	CColorStatic	m_Static_S;
	CColorStatic	m_Static_Q0_RES;
	CColorStatic	m_Static_Q1_RES;
	CColorStatic	m_Static_Q2_RES;
	CColorStatic	m_Static_T_RES;
	CColorStatic	m_Static_F_RES;
	CColorStatic	m_Static_S_RES;
	CColorStatic	m_Static_TotalPrice;
	//-------------------------------------------------------------------
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelchangeComboRailstyle();
	afx_msg void OnCbnSelchangeComboMaterial();
};
//-----------------------------------------------------------------------
