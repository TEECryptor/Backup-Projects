//-----------------------------------------------------------------------
//	File Name:BMOfferCalcTechnicalDlg.h
//	Introduce:Class CBMOfferCalcTechnicalDlg define header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ColorListCtrl.h"
#include "BMCalcOptionsDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferCalcTechnicalDlg dialog
//-----------------------------------------------------------------------
class CBMOfferCalcTechnicalDlg : public CBMCalcOptionsDlg
{
	DECLARE_DYNAMIC(CBMOfferCalcTechnicalDlg)

public:
	CBMOfferCalcTechnicalDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferCalcTechnicalDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TECHNICAL };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//-------------------------------------------------------------------
public:
	BOOL CanDoNextStep();
	//-------------------------------------------------------------------
private:
	void	FillCompomentListData();
	void	FillHoistData();
	void	FillHoistStdAdditionsData();
	void	FillHoistSelAdditionsData();
	void	FillCranesData();
	void	FillCraneSelAdditionsData();
	//-------------------------------------------------------------------
private:
	CImageList			m_imgNULL;
	CColorListCtrl		m_lstCtlTechnicalData;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
