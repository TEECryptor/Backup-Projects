// TestXMLLoaderDlg.h : header file
//

#if !defined(AFX_TESTXMLLOADERDLG_H__A260A873_E995_461E_A988_E896681F775C__INCLUDED_)
#define AFX_TESTXMLLOADERDLG_H__A260A873_E995_461E_A988_E896681F775C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------
// CTestXMLLoaderDlg dialog
//-------------------------------------------------------
class CTestXMLLoaderDlg : public CDialog
{
// Construction
public:
	CTestXMLLoaderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestXMLLoaderDlg)
	enum { IDD = IDD_TESTXMLLOADER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestXMLLoaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestXMLLoaderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnLoad();
	afx_msg void OnBtnOpenSkin();
	afx_msg void OnBtnOpenGame();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//---------------------------------------------------
private:
	TCHAR		m_szSkinXMLFile[MAX_PATH];
	TCHAR		m_szGameXMLFile[MAX_PATH];
	//---------------------------------------------------
};
//-------------------------------------------------------
#endif // !defined(AFX_TESTXMLLOADERDLG_H__A260A873_E995_461E_A988_E896681F775C__INCLUDED_)
//-------------------------------------------------------
