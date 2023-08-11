//-----------------------------------------------------------------------
//	File Name:BMOfferExportDlg.h
//	Introduce:Class CBMOfferExportDlg header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ExcelExporter.h"
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
// CBMOfferExportDlg dialog
//-----------------------------------------------------------------------
class CBMOfferExportDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferExportDlg)

public:
	CBMOfferExportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferExportDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OFFEREXPORT };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRadioExportall();
	afx_msg void OnBnClickedRadioExportsel();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnExport();
	//-------------------------------------------------------------------
public:
	void	SetCurSelOfferNo(CString strSelOfferNo);
	//-------------------------------------------------------------------
private:
	void	InitTablesDataStruct();
	//-------------------------------------------------------------------
private:
	TableData			m_stuOfferInfoTable;
	TableData			m_stuDetailInfoTable;
	CString				m_strSelOfferNo;
	BOOL				m_bExportAllOffers;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
