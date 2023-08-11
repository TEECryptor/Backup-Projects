// TestGDIPlusDlg.h : header file
//

#if !defined(AFX_TESTGDIPLUSDLG_H__A1E0D6DA_7193_4155_81B2_2A0E53397409__INCLUDED_)
#define AFX_TESTGDIPLUSDLG_H__A1E0D6DA_7193_4155_81B2_2A0E53397409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif
//--------------------------------------------
#include "Gdiplus.h"
using namespace Gdiplus;
//--------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// CTestGDIPlusDlg dialog

class CTestGDIPlusDlg : public CDialog
{
// Construction
public:
	CTestGDIPlusDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestGDIPlusDlg)
	enum { IDD = IDD_TESTGDIPLUS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestGDIPlusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestGDIPlusDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	Image*	pBKImage;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTGDIPLUSDLG_H__A1E0D6DA_7193_4155_81B2_2A0E53397409__INCLUDED_)
