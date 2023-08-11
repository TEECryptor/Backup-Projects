//-----------------------------------------------------------------------
//	File Name:BMCustomManagerDlg.h
//	Introduce:header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ColorListCtrl.h"
#include "BMCustomDataDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMCustomManagerDlg dialog
//-----------------------------------------------------------------------
class CBMCustomManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMCustomManagerDlg)
public:
	CBMCustomManagerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMCustomManagerDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CUSTOMER };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnBnClickedBtnReturn();
	afx_msg HRESULT OnDataRecordIsUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMClickListCustomer(NMHDR *pNMHDR, LRESULT *pResult);	
	//-------------------------------------------------------------------
private:
	void	UpdateControlsPos();
	void	InitDataListCtrls();
	void	InitTablesDataStruct();
	//
	void	OpenTableAndShowRecords(CString strTableName, CString strQuery = _T(""));
	CString	GetNewCustomerID();
	//-------------------------------------------------------------------
private:
	CWnd*				m_pParentWnd;
	CImageList			m_imgNULL;
	CColorListCtrl		m_lstCtlCustomers;
	CColorListCtrl		m_lstCtlContacts;
	CRect				m_rcCopyrightImage;
	CBitmap*			m_pCopyrightImage;
	//
	TableData			m_stuCustomerTable;
	TableData			m_stuContactTable;
	CADOTable			m_tbCustomInfo;
	CADOTable			m_tbContactInfo;
	CBMCustomDataDlg*	m_pCustomDataDlg;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
