//-------------------------------------------------------------------------
// SunMaxScreenServerDlg.h : header file
//
//-------------------------------------------------------------------------
#pragma once
//-------------------------------------------------------------------------
#include "ServerSocket.h"
#include "WebBrowser2.h"
//-------------------------------------------------------------------------
#define	WM_SHOWTASK	WM_USER + 100
#define	LOG(x) {if(m_pLogFile){_ftprintf(m_pLogFile, x);_ftprintf(m_pLogFile, _T("\r\n"));fflush(m_pLogFile);}};
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
// CSunMaxScreenServerDlg dialog
//-------------------------------------------------------------------------
class CSunMaxScreenServerDlg : public CDialog
{
// Construction
public:
	CSunMaxScreenServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SUNMAXSCREENSERVER_DIALOG };
	CWebBrowser2	m_MainIEWindow;

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
	afx_msg LRESULT OnRecieveData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()
	//---------------------------------------------------------------------
private:
	void	MinsizeToTray();
	void	DeleteTray();
	void	ExecuteCmdProject();
	void	ExecuteCmdRefresh();
	void	ExecuteCmdShow3DStore(LPCTSTR lpszURL);
	void	ExecuteCmdShowProjectInfo(LPCTSTR lpszURL);
	void	ExecuteCmdShowFieldInfo(LPCTSTR lpszURL);
	//---------------------------------------------------------------------
private:
	FILE*			m_pLogFile;
	CServerSocket*	m_pServerSocket;
	//---------------------------------------------------------------------
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//---------------------------------------------------------------------
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
//-------------------------------------------------------------------------
