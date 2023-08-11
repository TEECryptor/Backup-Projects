#pragma once
#include "afxcmn.h"



// CLeftView form view

class CLeftView : public CFormView
{
	DECLARE_DYNCREATE(CLeftView)

protected:
	CLeftView();           // protected constructor used by dynamic creation
	virtual ~CLeftView();

public:
	enum { IDD = IDD_DIALOG_LEFT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CTreeCtrl m_OptionsTree;
public:
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnCustom();
	afx_msg void OnBnClickedBtnSale();
	afx_msg void OnBnClickedBtnHetong();
	afx_msg void OnBnClickedBtnProjects();
	afx_msg void OnBnClickedBtnBuy();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnFinance();
	afx_msg void OnBnClickedBtnHr();
	afx_msg void OnBnClickedBtnAdmin();
	afx_msg void OnBnClickedBtnStatistics();
	//afx_msg void OnBnClickedBtnEmployee();
	//afx_msg void OnBnClickedBtnUser();
	//afx_msg void OnBnClickedBtnSystem();
	virtual void OnInitialUpdate();
	afx_msg void OnTvnSelchangedTreeOptions(NMHDR *pNMHDR, LRESULT *pResult);
};


