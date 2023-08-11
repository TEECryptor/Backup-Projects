//----------------------------------------------------
//	File Name:AFUEmployeeDlg.h
//	Introduce:Dialog class CAFUEmployeeDlg interfaces definition
//----------------------------------------------------
#pragma once
//----------------------------------------------------
#include "DatabaseDefine.h"
//----------------------------------------------------


//----------------------------------------------------
//	Name:CAFUEmployeeDlg
//	Introduce:Class CAFUEmployeeDlg
//----------------------------------------------------
class CAFUEmployeeDlg : public CDialog
{
	DECLARE_DYNAMIC(CAFUEmployeeDlg)
	//------------------------------------------------
public:
	CAFUEmployeeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAFUEmployeeDlg();
	//------------------------------------------------
// Dialog Data
	enum { IDD = IDD_DIALOG_EMPLOYEE };
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
	void	SetInitializeData(DWORD dwEmployeeID, CString strUserName, CString strEmployeeName, eEmployeeGroup eGroup, CString strTel, CString strAddress);
	void	GetUpdatedData(DWORD &dwEmployeeID, CString &strUserName, CString &strEmployeeName, eEmployeeGroup &eGroup, CString &strTel, CString &strAddress);
	//------------------------------------------------
private:
	DWORD			m_dwEmployeeID;
	CString			m_strUserName;
	CString			m_strEmployeeName;
	eEmployeeGroup	m_eUserGroup;
	CString			m_strTel;
	CString			m_strAddress;
	//------------------------------------------------
};
//----------------------------------------------------
