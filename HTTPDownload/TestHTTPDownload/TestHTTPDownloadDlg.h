//--------------------------------------------
//	File Name:TestHTTPDownloadDlg.h
//	Introduce:Class CTestHTTPDownloadDlg
//	Date:2010-05-10
//--------------------------------------------
#pragma once
//--------------------------------------------
//#define	_USE_DLL_

#ifdef _USE_DLL_
#include "HTTPDownloadDll.h"
#else
#include "HTTPDownloadLib.h"
#endif	//_USE_DLL_
//--------------------------------------------
// CTestHTTPDownloadDlg dialog
//--------------------------------------------
class CTestHTTPDownloadDlg : public CDialog
{
// Construction
public:
	CTestHTTPDownloadDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTHTTPDOWNLOAD_DIALOG };

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
	//----------------------------------------
public:
	long	OnHttpDownloadEvent(LONG lTaskID, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2);
	//----------------------------------------
private:
	void	AddNewTaskToList(LONG lTaskID, BOOL bIsStreamTask);
	void	AddNewThreadToList(LONG lTaskID, LONG lThreadID);
	void	DelTaskFromList(LONG lTaskID, LONG lThreadID);
	void	UpdateTaskStatus(LONG lTaskID, LONG lThreadID, DWORD dwNewStatus);
	void	UpdateTaskProgress(LONG lTaskID, LONG lThreadID, DWORD dwFinishedBytes, DWORD dwCurrentSpeed);
	void	UpdateTaskThreadCnt(LONG lTaskID, DWORD dwNewThreadCnt);
	LPCTSTR	GetStatusText(LONG lStatus);
	LPCTSTR	GetBytesSizeText(DWORD dwBytesSize);
	//----------------------------------------
private:
	IHttpDownloadIF*			m_pHttpDownloader;
	DWORD						m_dwMaxThreadCnt;
	//----------------------------------------
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnNew();
	afx_msg void OnBnClickedBtnSetting();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnOption();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnReadfile();
};
//--------------------------------------------
