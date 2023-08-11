// TestMatrixDlg.h : header file
//

#if !defined(AFX_TESTMATRIXDLG_H__44E09E75_4D05_435D_8564_5AB7604CC551__INCLUDED_)
#define AFX_TESTMATRIXDLG_H__44E09E75_4D05_435D_8564_5AB7604CC551__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestMatrixDlg dialog

class CTestMatrixDlg : public CDialog
{
// Construction
public:
	CTestMatrixDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestMatrixDlg)
	enum { IDD = IDD_TESTMATRIX_DIALOG };
	int		m_iMaxAroundCnt;
	int		m_iColCnt;
	int		m_iIDCount;
	int		m_iMaxLineCnt;
	int		m_iRowCnt;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestMatrixDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestMatrixDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnFind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTMATRIXDLG_H__44E09E75_4D05_435D_8564_5AB7604CC551__INCLUDED_)
