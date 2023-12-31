//-------------------------------------------------------------------------
// SunMaxScreenClientDlg.h : header file
//
//-------------------------------------------------------------------------
#pragma once
//-------------------------------------------------------------------------
#include "SunMsgSender.h"
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
// CSunMaxScreenClientDlg dialog
//-------------------------------------------------------------------------
class CSunMaxScreenClientDlg : public CDialog
{
// Construction
public:
	CSunMaxScreenClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SUNMAXSCREENCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//---------------------------------------------------------------------
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	//---------------------------------------------------------------------
public:
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnProject();
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnBnClickedOk();
	//---------------------------------------------------------------------
private:
	CString m_strCfgFileName;
	BOOL	m_bConnectIsShown;
	BOOL	m_bIsConnected;
	//---------------------------------------------------------------------
public:
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnNMClickSyslinkConnect(NMHDR *pNMHDR, LRESULT *pResult);
};
//-------------------------------------------------------------------------
