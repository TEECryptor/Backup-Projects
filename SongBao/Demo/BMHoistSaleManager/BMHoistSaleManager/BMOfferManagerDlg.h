//-----------------------------------------------------------------------
//	File Name:BMOfferManagerDlg.h
//	Introduce:Class BMOfferManagerDlg define header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ColorListCtrl.h"
#include "BMOfferDataDlg.h"
#include "BMOfferCalcDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferManagerDlg dialog
//-----------------------------------------------------------------------
class CBMOfferManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferManagerDlg)
public:
	CBMOfferManagerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferManagerDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OFFERMANAGER };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnBnClickedBtnFindall();
	afx_msg void OnBnClickedBtnCustom();
	afx_msg void OnBnClickedBtnNew();
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnCalc();
	afx_msg void OnBnClickedBtnReportprice();
	afx_msg void OnBnClickedBtnOrders();
	afx_msg void OnBnClickedBtnCopy();
	afx_msg void OnBnClickedBtnPaste();
	afx_msg void OnBnClickedBtnImport();
	afx_msg void OnBnClickedBtnExist();
	afx_msg HRESULT OnDataRecordIsUpdated(WPARAM wParam, LPARAM lParam);	
	afx_msg void OnNMClickListOffer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboFromyear();
	afx_msg void OnCbnSelchangeComboFrommonth();
	afx_msg void OnCbnSelchangeComboToyear();
	afx_msg void OnCbnSelchangeComboTomonth();
	//-------------------------------------------------------------------
private:
	void	UpdateControlsPos();
	void	InitDataListCtrls();
	void	InitTablesDataStruct();
	void	InitFindOptionsCtrls();
	void	UpdatePasteBtnState(BOOL bIsInCopying = TRUE);
	void	FillDateDayComboBox(int iYear, int iMonth, CComboBox* pDayComboBox);
	//
	void	OpenTableAndShowRecords(CString strTableName, CString strQuery = _T(""));
	CString	GetNewOfferID();
	int 	CopyOfferDataInTable(int iCopyItem);
	BOOL	CopyDataInTables(CADOTable* pSrcTable, int iSrcIndex, CADOTable* pDestTable, int iDestIndex, TableData &tb, int iFromPos);
	//-------------------------------------------------------------------
private:
	CWnd*				m_pParentWnd;
	CImageList			m_imgNULL;
	CColorListCtrl		m_lstCtlOffers;
	CColorListCtrl		m_lstCtlDetails;
	CRect				m_rcCopyrightImage;
	CBitmap*			m_pCopyrightImage;
	//
	TableData			m_stuOfferInfoTable;
	TableData			m_stuDetailInfoTable;
	CADOTable			m_tbOffersInfo;
	CADOTable			m_tbComponentInfo;
	CBMOfferDataDlg*	m_pOfferDataDlg;
	//
	BOOL				m_bIsInCopyPaste;
	int					m_iCopiedItem;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
