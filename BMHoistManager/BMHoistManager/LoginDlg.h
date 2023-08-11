#pragma once

#include "./Database/ADODataBase.h"
#include "./Database/ADOTable.h"


// CLoginDlg dialog

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CBitmap*		m_pKImage;
public:
	CADODataBase*	m_pDatabase;
	//
	CString			m_strLoginName;
	CString			m_strUserName;
	CString			m_strUserGroup;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
};
