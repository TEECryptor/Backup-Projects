// TestXMLDlg.h : header file
//

#if !defined(AFX_TESTXMLDLG_H__1B071E37_81DA_40A4_A217_C2E51183766D__INCLUDED_)
#define AFX_TESTXMLDLG_H__1B071E37_81DA_40A4_A217_C2E51183766D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestXMLDlg dialog

class CTestXMLDlg : public CDialog
{
// Construction
public:
	CTestXMLDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestXMLDlg)
	enum { IDD = IDD_TESTXML_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestXMLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestXMLDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTXMLDLG_H__1B071E37_81DA_40A4_A217_C2E51183766D__INCLUDED_)
