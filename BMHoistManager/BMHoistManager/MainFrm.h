// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "MainView.h"
#include "BMSplitterWnd.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar		m_wndStatusBar;
	CToolBar		m_wndToolBar;
	//CBMSplitterWnd	m_wndLeftSplitter;
	CSplitterWnd	m_wndLeftSplitter;
	BOOL			m_bMenuSystemOptionsChecked;	
	eTableID		m_eCurTableID;
public:	
	CString			m_strLoginName;
	CString			m_strUserName;
	CString			m_strUserGroup;
private:
	void	SwitchViewInSplitter(CSplitterWnd* pSplitter, int row, int col, CRuntimeClass* pViewClass);
	CString	GetUserGroupText(DWORD dwGroup);
	void 	OnLeftViewBtnCustom();
	void 	OnLeftViewBtnSale();
	void 	OnLeftViewBtnHetong();
	void 	OnLeftViewBtnProjects();
	void 	OnLeftViewBtnBuy();
	void 	OnLeftViewBtnSave();
	void 	OnLeftViewBtnEmployee();
	void 	OnLeftViewBtnUser();
	void 	OnLeftViewBtnSystemSetting();
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnMenuSystemHome();
	afx_msg void OnMenuSystemOptions();
	afx_msg void OnMenuUserSettingSet();
	afx_msg void OnMenuUserChangePassword();
	afx_msg void OnMenuUserLogout();
	afx_msg void OnMenuToolCalculator();
	afx_msg void OnMenuToolCalendar();
	afx_msg void OnUpdateMenuSystemHome(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuSystemOptions(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuSettingSet(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuSettingChangePsw(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolbtnFind(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolbtnNew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolbtnModify(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolbtnDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolbtnExport(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolbtnCount(CCmdUI *pCmdUI);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

public:
	void	DoLeftViewActions(ULONG uBtnID, BOOL bShowHomepage = FALSE);
	void	ToShowTable(eTableID tableId);
};


