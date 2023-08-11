//----------------------------------------------------
//	File Name:AFULoginDlg.h
//	Introduce:Class CAFULoginDlg intefaces definition file
//
//----------------------------------------------------
#pragma once
//----------------------------------------------------



//----------------------------------------------------
// CAFULoginDlg dialog
//----------------------------------------------------
class CAFULoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CAFULoginDlg)
	//------------------------------------------------
public:
	CAFULoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAFULoginDlg();
// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };
	//------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//------------------------------------------------
public:
	void	GetUserNameAndPassword(CString &strUserName, CString &strPassword);
	//------------------------------------------------
private:
	CString	m_strUserName;
	CString	m_strPassword;
	//------------------------------------------------
public:
	afx_msg void OnBnClickedOk();
	//------------------------------------------------
};
//----------------------------------------------------
