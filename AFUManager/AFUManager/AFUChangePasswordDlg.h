//----------------------------------------------------
//	File Name:AFUChangePasswordDlg.h
//	Introduce:Class CAFUChangePasswordDlg intefaces definition file
//
//----------------------------------------------------
#pragma once
//----------------------------------------------------



//----------------------------------------------------
// CAFUChangePasswordDlg dialog
//----------------------------------------------------
class CAFUChangePasswordDlg : public CDialog
{
	DECLARE_DYNAMIC(CAFUChangePasswordDlg)
public:
	CAFUChangePasswordDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAFUChangePasswordDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PASSWORD };
	//------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//------------------------------------------------
public:
	void	SetOldPassword(CString strOldPassword);
	void	GetNewPassword(CString &strNewPassword);
	//------------------------------------------------
private:
	CString	m_strOldPassword;
	CString m_strNewPassword;
	//------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
//----------------------------------------------------
