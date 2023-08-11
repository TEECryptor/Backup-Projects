#pragma once


#include "GridControl/GridCtrl.h"
#include "ColorListCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"


// CMainView form view

class CMainView : public CFormView
{
	DECLARE_DYNCREATE(CMainView)

public:
	CMainView();           // protected constructor used by dynamic creation
	virtual ~CMainView();

public:
	enum { IDD = IDD_DIALOG_MAIN };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnToolBarFind();
	afx_msg void OnToolBarNew();
	afx_msg void OnToolBarUpdate();
	afx_msg void OnToolBarDelete();
	afx_msg void OnToolBarExport();
	afx_msg void OnToolBarCount();
	DECLARE_MESSAGE_MAP()

private:
	CToolBar		m_wndToolBar;
	CStatic			m_FindOptionStatic;
	CComboBox		m_FindOptionCombox;
	CColorListCtrl	m_MainListCtrl;
	CStatic			m_StaticBottom;
	CStatic			m_StaticPageNum;
	CLinkCtrl		m_LinkCtrlFirst;
	CLinkCtrl		m_LinkCtrlUp;
	CLinkCtrl		m_LinkCtrlDown;
	CLinkCtrl		m_LinkCtrlLast;
	CImageList		m_ToolBarEnableImages;
	CImageList		m_ToolBarDisableImages;
	//	
	eTableID		m_eCurTableID;
	CADOTable		m_TableObj;
	BOOL			m_bShowFindOption;
	BOOL			m_bIsMultiPage;
	LONG			m_lCountPerPage;
	LONG			m_lTotalPage;
	LONG			m_lCurPage;

private:
	void		CreateToolBar();
	void		InitUIControls();
	CRect		SetFindOptionsCtrlPos(int iTop);
	void		InitTableStruct();
	void		ShowFindOptions(BOOL bShow = TRUE);
	BOOL		OpenTable(eTableID tableID, CString strOperation);
	void		ShowCurrentTable(BOOL bIsMultiPage, LONG lCountPerPage, LONG lCurPage);
public:
	void		ToShowTable(eTableID curTable, CADODataBase* pDatabase);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnInitialUpdate();
	afx_msg void OnNMDblclkListMainviewMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnMainviewFind();
};


