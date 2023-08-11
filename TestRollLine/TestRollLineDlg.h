// TestRollLineDlg.h : header file
//

#if !defined(AFX_TESTROLLLINEDLG_H__938319E3_0CAE_4223_A042_3110A8E5DFFD__INCLUDED_)
#define AFX_TESTROLLLINEDLG_H__938319E3_0CAE_4223_A042_3110A8E5DFFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestRollLineDlg dialog

class CTestRollLineDlg : public CDialog
{
// Construction
public:
	CTestRollLineDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestRollLineDlg)
	enum { IDD = IDD_TESTROLLLINE_DIALOG };
	int		m_iBigR;
	int		m_iMiniR;
	int		m_iStartAngle;
	int		m_iEndAngle;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestRollLineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestRollLineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void	DrawCoordinate(HDC hdc, int iWidth, int iHeight); 
	void	DrawCircle(HDC hdc, int iPosX, int iPosY, int iRadius);
	void	DrawArcLine(HDC hdc, int iPosX, int iPosY, int iStartR, int iEndR);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTROLLLINEDLG_H__938319E3_0CAE_4223_A042_3110A8E5DFFD__INCLUDED_)
