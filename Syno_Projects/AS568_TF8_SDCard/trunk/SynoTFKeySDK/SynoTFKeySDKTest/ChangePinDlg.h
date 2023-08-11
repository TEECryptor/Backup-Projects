#pragma once


// CChangePinDlg dialog

class CChangePinDlg : public CDialog
{
	DECLARE_DYNAMIC(CChangePinDlg)

public:
	CChangePinDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChangePinDlg();

// Dialog Data
	enum { IDD = IDD_CHANGEPIN_DIALOG };
	
public:
	TCHAR	m_tcOldPin[32];
	TCHAR	m_tcNewPin[32];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
