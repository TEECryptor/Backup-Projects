#pragma once


// CPlainTextDlg dialog

class CPlainTextDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlainTextDlg)

public:
	CPlainTextDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlainTextDlg();

// Dialog Data
	enum { IDD = IDD_PLAINTEXT_DIALOG };

	ULONG		m_ulTestType;
	CString		m_strPlainText;
	ULONG		m_ulAlgId;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
