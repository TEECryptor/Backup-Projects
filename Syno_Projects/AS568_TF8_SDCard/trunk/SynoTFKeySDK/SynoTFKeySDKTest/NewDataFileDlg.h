#pragma once


// CNewDataFileDlg dialog

class CNewDataFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewDataFileDlg)

public:
	CNewDataFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewDataFileDlg();

// Dialog Data
	enum { IDD = IDD_NEWDATAFILE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString		m_strFileName;
	CString		m_strFileData;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
