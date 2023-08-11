// SocketHttpDlg.h : header file
//

#pragma once

#include <Winsock.h>
/*
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/wait.h> 
*/

// CSocketHttpDlg dialog
class CSocketHttpDlg : public CDialog
{
// Construction
public:
	CSocketHttpDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SOCKETHTTP_DIALOG };

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

private:
	BOOL	InitSocket();
	BOOL	WriteToFile(char* pBuf, int iLen);
private:
	SOCKET	m_Socket;
	FILE*	m_pFile;
};
