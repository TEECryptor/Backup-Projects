//--------------------------------------------
//	File Name:NewThreadDlg.h
//	Introduce:Class CNewThreadDlg
//	Date:2010-06-06
//--------------------------------------------
#pragma once
//--------------------------------------------
struct StreamTaskThreadInfo
{
	DWORD dwThreadID;
	DWORD dwStartPos;
	DWORD dwDataLength;
};
//--------------------------------------------
// CNewThreadDlg dialog
//--------------------------------------------
class CNewThreadDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewThreadDlg)

public:
	CNewThreadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewThreadDlg();

// Dialog Data
	enum { IDD = IDD_DLG_NEWTHREAD };

public:
	DWORD	m_dwThreadID;
	DWORD	m_dwStartPos;
	DWORD	m_dwLength;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
//--------------------------------------------
