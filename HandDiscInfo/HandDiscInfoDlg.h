// HandDiscInfoDlg.h : header file
//

#if !defined(AFX_HANDDISCINFODLG_H__78A63471_86AB_4CF1_9C44_D417D1F02621__INCLUDED_)
#define AFX_HANDDISCINFODLG_H__78A63471_86AB_4CF1_9C44_D417D1F02621__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHandDiscInfoDlg dialog

class CHandDiscInfoDlg : public CDialog
{
// Construction
public:
	CHandDiscInfoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHandDiscInfoDlg)
	enum { IDD = IDD_HANDDISCINFO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHandDiscInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHandDiscInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int		GetHDInfo(LPTSTR lpszInfo, int iDriver);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANDDISCINFODLG_H__78A63471_86AB_4CF1_9C44_D417D1F02621__INCLUDED_)
