#pragma once


// CDevInitDlg dialog

class CDevInitDlg : public CDialog
{
	DECLARE_DYNAMIC(CDevInitDlg)

public:
	CDevInitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDevInitDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DEVINIT };

public:
	CString		m_strLabel;
	CString		m_strSOPin;
	CString		m_strUserPin;
	ULONG		m_ulSOPinRetryTimes;
	ULONG		m_ulUserPinRetryTimes;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
};
