#pragma once

#include "./TFKeySDKTester/Defs.h"

// CSelectSDKDlg dialog

class CSelectSDKDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectSDKDlg)

public:
	CSelectSDKDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectSDKDlg();

// Dialog Data
	enum { IDD = IDD_SELECTSDK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	SDK_TYPE	m_eSelSDKType;
	CString		m_strDllFile;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeSelectsdkComboSdk();
	afx_msg void OnBnClickedSelectsdkdlgBtnOpenfile();
	afx_msg void OnBnClickedOk();
};
