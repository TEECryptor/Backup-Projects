//--------------------------------------------
//	File Name:TaskSettingDlg.h
//	Introduce:Class CTaskSettingDlg
//	Date:2010-07-02
//--------------------------------------------
#pragma once
//--------------------------------------------
// CTaskSettingDlg dialog
//--------------------------------------------
class CTaskSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CTaskSettingDlg)

public:
	CTaskSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTaskSettingDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SETTING };

public:
	DWORD	m_dwMaxThreadCnt;
	DWORD	m_dwSettingThreadCnt;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboThreadcnt();
};
//--------------------------------------------
