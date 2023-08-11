// TestFileDlg.h : header file
//

#if !defined(AFX_TESTFILEDLG_H__FE05D24D_AA0F_4B8C_BD67_FCC51201951D__INCLUDED_)
#define AFX_TESTFILEDLG_H__FE05D24D_AA0F_4B8C_BD67_FCC51201951D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestFileDlg dialog

class CTestFileDlg : public CDialog
{
// Construction
public:
	CTestFileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestFileDlg)
	enum { IDD = IDD_TESTFILE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTFILEDLG_H__FE05D24D_AA0F_4B8C_BD67_FCC51201951D__INCLUDED_)
