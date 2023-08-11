//-----------------------------------------------------------------------
//	File Name:CBMHoistLayotsDlg.h
//	Introduce:header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "ColorListCtrl.h"
#include "BMParamCalculator.h"
#include <atlimage.h>
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMHoistLayotsDlg dialog
//-----------------------------------------------------------------------
class CBMHoistLayotsDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMHoistLayotsDlg)
public:
	CBMHoistLayotsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMHoistLayotsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_HOISTLAYOT };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//-------------------------------------------------------------------
public:
	void	SetCraneParamObject(CBMParamCalculator* pCraneParam);
	void	SetCurrentViewType(eLayoutViewType eViewType);
	//-------------------------------------------------------------------
private:
	void	InitTableDataStruct();
	//-------------------------------------------------------------------
private:
//	CBitmap*				m_pLayoutImage;
	CImageList				m_imgNULL;
	CColorListCtrl			m_ctrlDimHoistLst;
	CBMParamCalculator*		m_pCraneParam;
	//
	eHoistDrawType			m_eCurDrawType;
	eLayoutViewType			m_eCurViewType;
	CImage*					m_pHoistImage[eHoistDrawType_Cnt][eLayoutViewType_Cnt];
	TableData				m_stuDimHoistTables[eLayoutViewType_Cnt][4];
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
