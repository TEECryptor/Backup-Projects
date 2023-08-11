//----------------------------------------------------
//	File Name:AFUMemberDlg.h
//	Introduce:Dialog class CAFUMemberDlg interfaces definition
//----------------------------------------------------
#pragma once
//----------------------------------------------------


//----------------------------------------------------
//	Name:CAFUMemberDlg
//	Introduce:Class CAFUMemberDlg
//----------------------------------------------------
class CAFUMemberDlg : public CDialog
{
	DECLARE_DYNAMIC(CAFUMemberDlg)
public:
	CAFUMemberDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAFUMemberDlg();
	//------------------------------------------------
// Dialog Data
	enum { IDD = IDD_DIALOG_VIPMEMBER };
	//------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	//------------------------------------------------
public:
	void	SetInitializeData(CString strName, DWORD dwVIPID, BOOL bSexIsFemale, CString strTel, CTime& dateBirth, CString strEmail, CString strAddress, CString strOther);
	void	GetUpdatedData(CString &strName, DWORD &dwVIPID, BOOL &bSexIsFemale, CString &strTel, CTime& dateBirth, CString &strEmail, CString &strAddress, CString &strOther);
	//------------------------------------------------
private:
	CString		m_strName;
	CString		m_strVIPID;
	BOOL		m_bSexIsFemale;
	CString		m_strTel;
	CTime		m_dateBirth;
	CString		m_strEmail;
	CString		m_strAddress;
	CString		m_strOtherInfo;
	//------------------------------------------------
};
//----------------------------------------------------
