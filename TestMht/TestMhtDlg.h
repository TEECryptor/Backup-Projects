// TestMhtDlg.h : header file
//

#pragma once

#include <comutil.h>
/*
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace raw_interfaces_only
#import "C:\WINDOWS\system32\cdosys.dll" no_namespace raw_interfaces_only
#include "C:\VistaSDK\v6.0\Include\cdosysstr.h"
#include "C:\VistaSDK\v6.0\Include\cdosyserr.h"
*/

// CTestMhtDlg dialog
class CTestMhtDlg : public CDialog
{
// Construction
public:
	CTestMhtDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTMHT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
