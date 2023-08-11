#pragma once


// CUnlockPinDlg dialog

class CUnlockPinDlg : public CDialog
{
	DECLARE_DYNAMIC(CUnlockPinDlg)

public:
	CUnlockPinDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUnlockPinDlg();

// Dialog Data
	enum { IDD = IDD_UNLOCKPIN_DIALOG };
	
public:
	TCHAR	m_tcSOPin[32];
	TCHAR	m_tcNewUserPin[32];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
