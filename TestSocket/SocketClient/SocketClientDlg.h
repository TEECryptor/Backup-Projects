//---------------------------------------------------------------
// SocketClientDlg.h : header file
//
//---------------------------------------------------------------
#if !defined(AFX_SOCKETCLIENTDLG_H__194E4C63_5546_4E53_A289_783787BD9609__INCLUDED_)
#define AFX_SOCKETCLIENTDLG_H__194E4C63_5546_4E53_A289_783787BD9609__INCLUDED_
//---------------------------------------------------------------
#include "ClientSocket.h"
#include "MsgDataDefine.h"
//---------------------------------------------------------------
// CSocketClientDlg dialog
//---------------------------------------------------------------
class CSocketClientDlg : public CDialog
{
// Construction
public:
	CSocketClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSocketClientDlg)
	enum { IDD = IDD_SOCKETCLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSocketClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnConnect();
	afx_msg void OnBtnSend();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//-----------------------------------------------------------
private:
	CClientSocket*	m_pSocket;
	//-----------------------------------------------------------
};
//---------------------------------------------------------------
#endif // !defined(AFX_SOCKETCLIENTDLG_H__194E4C63_5546_4E53_A289_783787BD9609__INCLUDED_)
//---------------------------------------------------------------
