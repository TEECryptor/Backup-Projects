// TestMediaHostDlg.h : header file
//

#if !defined(AFX_TESTMEDIAHOSTDLG_H__96402B30_690C_46F7_B6D1_0B63237442AE__INCLUDED_)
#define AFX_TESTMEDIAHOSTDLG_H__96402B30_690C_46F7_B6D1_0B63237442AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MediaLib.h"

/////////////////////////////////////////////////////////////////////////////
// CTestMediaHostDlg dialog

class CTestMediaHostDlg : public CDialog
{
// Construction
public:
	CTestMediaHostDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestMediaHostDlg)
	enum { IDD = IDD_TESTMEDIAHOST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestMediaHostDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestMediaHostDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnOpenbk();
	afx_msg void OnBtnPlaybk();
	afx_msg void OnBtnPausebk();
	afx_msg void OnBtnStopbk();
	afx_msg void OnBtnOpenbt();
	afx_msg void OnBtnPausebt();
	afx_msg void OnBtnPlaybt();
	afx_msg void OnBtnStopbt();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CMusic*		m_pBKMusic;
	CMusic*		m_pBtnMusic;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTMEDIAHOSTDLG_H__96402B30_690C_46F7_B6D1_0B63237442AE__INCLUDED_)
