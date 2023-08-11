#pragma once


// CAFUSettingDlg dialog

class CAFUSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CAFUSettingDlg)

public:
	CAFUSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAFUSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void	SetInitializeData(CString strDBFile, BOOL bIsMultiPage, LONG lRecordCntPerPage);
	void	GetSettingData(CString &strDBFile, BOOL &bIsMultiPage, LONG &lRecordCntPerPage);

private:
	CString	m_strDBFile;
	BOOL	m_bIsMultiPage;
	LONG	m_lRecordCntPerPage;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioMultipage();
	afx_msg void OnBnClickedRadioSinglepage();
	afx_msg void OnBnClickedButtonBrowse();
};
