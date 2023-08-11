// TestHTTPDlg.h : header file
//

#pragma once

#include "HTTPDownloadLib.h"

#define	WM_MSG_TASKSTATUSCHANGED	WM_USER + 1
#define	WM_MSG_TASKPROGRESS			WM_USER + 2
#define	WM_MSG_TASKSERROR			WM_USER + 3

// CTestHTTPDlg dialog
class CTestHTTPDlg : public CDialog
{
// Construction
public:
	CTestHTTPDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTHTTP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnDownload();
	afx_msg void OnBnClickedBtnSavelog();
	afx_msg void OnBnClickedRadioSingle();
	afx_msg void OnBnClickedRadioMulti();

public:
	long	OnHttpDownloadEvent(LONG lTaskID, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2);
	LONG	OnTaskStatusChange(WPARAM wParam, LPARAM lParam);
	LONG	OnTaskProgress(WPARAM wParam, LPARAM lParam);
	LONG	OnTaskError(WPARAM wParam, LPARAM lParam);
private:
	void	UpdateSettingControls(BOOL bEnable = TRUE);
	void	UpdateTaskProgress(DWORD dwCurFinished);
private:
	TCHAR	m_tcNetFileURL[1024];
	TCHAR	m_tcTargetFile[MAX_PATH];
	TCHAR	m_tcLogFile[MAX_PATH];
	DWORD	m_dwThreadCnt;
	LONG	m_lFileSize;
	LONG	m_lTaksID;
	IHttpDownloadIF*	m_pHttpDownloader;
public:
	afx_msg void OnCbnSelchangeComboThreadcnt();
};
