// TestDlg.h : header file
//
#if !defined(AFX_TESTDLG_H__12A7C63C_9C24_4F37_884A_368C26EE56E1__INCLUDED_)
#define AFX_TESTDLG_H__12A7C63C_9C24_4F37_884A_368C26EE56E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//----------------------------------------------
#include "MSGManager.h"
//----------------------------------------------
#define		MSG_STC_GETCLIENTINFO		1
#define		MSG_CTS_CLIENTINFO			2
//----------------------------------------------
struct ClientInfo
{
	UINT	uMsgType;
	UINT	uClientID;
	TCHAR	szClientName[64];
};
//----------------------------------------------
// CTestDlg dialog
//----------------------------------------------
class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_TEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnConnect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//------------------------------------------
public:
	DWORD ServerCallbackFunc(DWORD dwCode, DWORD dwParam, LPVOID lpParam);
	DWORD ClientCallbackFunc(DWORD dwCode, DWORD dwParam, LPVOID lpParam);
	//------------------------------------------
private:
	void	OnClientConnected(DWORD dwClientID);
	DWORD	OnServerRecvData(BYTE* lpData, DWORD dwSize);
	DWORD	OnClientRecvData(BYTE* lpData, DWORD dwSize);
	//------------------------------------------
private:
	CServer*	m_pServer;
	CClient*	m_pClient;
	//------------------------------------------
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__12A7C63C_9C24_4F37_884A_368C26EE56E1__INCLUDED_)
