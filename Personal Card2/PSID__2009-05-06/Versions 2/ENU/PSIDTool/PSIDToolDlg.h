// PSIDToolDlg.h : header file
//

#pragma once


// CPSIDToolDlg dialog
class CPSIDToolDlg : public CDialog
{
// Construction
public:
	CPSIDToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PSIDTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpensetting();
	afx_msg void OnBnClickedBtnOpensetting2();
	afx_msg void OnBnClickedBtnOpensetting3();
	afx_msg void OnBnClickedBtnSave();
};
