//--------------------------------------------------
// SocketServerDlg.h : header file
//--------------------------------------------------
#if !defined(AFX_SOCKETSERVERDLG_H__5FA9DDE3_7DAD_458C_88E2_832170CDB56F__INCLUDED_)
#define AFX_SOCKETSERVERDLG_H__5FA9DDE3_7DAD_458C_88E2_832170CDB56F__INCLUDED_
//--------------------------------------------------
#include "ServerSocket.h"
#include "MsgDataDefine.h"
//--------------------------------------------------
// CSocketServerDlg dialog
//--------------------------------------------------
class CSocketServerDlg : public CDialog
{
// Construction
public:
	CSocketServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSocketServerDlg)
	enum { IDD = IDD_SOCKETSERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSocketServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//----------------------------------------------
public:
	//SKT_MSG_CLIENTCONNECTED message handler
	HRESULT		OnClientConnected(WPARAM wParam, LPARAM lParam);
	//SKT_MSG_RECEIVEDMSG message handler
	HRESULT		OnRecievedMsg(WPARAM wParam, LPARAM lParam);
	//----------------------------------------------
private:
	CServerSocket*	m_pServerSocket;		//The server socket pointer
	//----------------------------------------------
};
//--------------------------------------------------
#endif // !defined(AFX_SOCKETSERVERDLG_H__5FA9DDE3_7DAD_458C_88E2_832170CDB56F__INCLUDED_)
//--------------------------------------------------
