// TestWinThreadDlg.h : header file
//

#if !defined(AFX_TESTWINTHREADDLG_H__1E826AB5_4CFA_4DB9_AD02_95F460D97F97__INCLUDED_)
#define AFX_TESTWINTHREADDLG_H__1E826AB5_4CFA_4DB9_AD02_95F460D97F97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyThread.h"

/////////////////////////////////////////////////////////////////////////////
// CTestWinThreadDlg dialog

class CTestWinThreadDlg : public CDialog
{
// Construction
public:
	CTestWinThreadDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestWinThreadDlg)
	enum { IDD = IDD_TESTWINTHREAD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestWinThreadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestWinThreadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CMyThread		m_Thread;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTWINTHREADDLG_H__1E826AB5_4CFA_4DB9_AD02_95F460D97F97__INCLUDED_)
