#pragma once


// CInputFileNameDlg dialog

class CInputFileNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputFileNameDlg)

public:
	CInputFileNameDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputFileNameDlg();

// Dialog Data
	enum { IDD = IDD_INPUTFILENAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString		m_strFileName;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
