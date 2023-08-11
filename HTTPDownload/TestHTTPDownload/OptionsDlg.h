//--------------------------------------------
//	File Name:OptionsDlg.h
//	Introduce:Class COptionsDlg
//	Date:2010-05-10
//--------------------------------------------
#pragma once
//--------------------------------------------
// COptionsDlg dialog
//--------------------------------------------
class COptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsDlg)

public:
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionsDlg();

// Dialog Data
	enum { IDD = IDD_DLG_OPTIONS };
	//----------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//----------------------------------------
public:
	DWORD	m_dwMaxTaskCnt;
	DWORD	m_dwMaxThreadCnt;
	//----------------------------------------
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
//--------------------------------------------
