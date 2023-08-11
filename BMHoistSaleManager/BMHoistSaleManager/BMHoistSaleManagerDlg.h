//-----------------------------------------------------------------------
//	File Name:BMHoistSaleManagerDlg.h
//	Introduce:header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "Localization.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMHoistSaleManagerDlg dialog
//-----------------------------------------------------------------------
class CBMHoistSaleManagerDlg : public CDialog
{
// Construction
public:
	CBMHoistSaleManagerDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	enum { IDD = IDD_BMHOISTSALEMANAGER_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//-------------------------------------------------------------------
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg HRESULT OnCustomerManage(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnOfferInfoManage(WPARAM wParam, LPARAM lParam);
	//-------------------------------------------------------------------
private:
	CDialog*	m_pOfferManagerDlg;
	CDialog*	m_pCustomManagerDlg;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
