// TestCommDlg.h : header file
//

#if !defined(AFX_TESTCOMMDLG_H__6425718D_22FA_49E0_9BE2_E7C186F4163B__INCLUDED_)
#define AFX_TESTCOMMDLG_H__6425718D_22FA_49E0_9BE2_E7C186F4163B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestCommDlg dialog

class CTestCommDlg : public CDialog
{
// Construction
public:
	CTestCommDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestCommDlg)
	enum { IDD = IDD_TESTCOMM_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestCommDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestCommDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnConnect();
	afx_msg void OnBtnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()		
	//------------------------------------------
private:
	HANDLE		m_hPrinterCom;
	//------------------------------------------
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCOMMDLG_H__6425718D_22FA_49E0_9BE2_E7C186F4163B__INCLUDED_)
