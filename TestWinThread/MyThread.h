//-----------------------------------------------------------
#if !defined(AFX_MYTHREAD_H__331B3BFB_38BE_46F7_B966_A0D0CAEC4EBE__INCLUDED_)
#define AFX_MYTHREAD_H__331B3BFB_38BE_46F7_B966_A0D0CAEC4EBE__INCLUDED_
//-----------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-----------------------------------------------------------
#define		WM_MY_MSG			WM_USER + 1
#define		WM_MY_MSG_WAITE		WM_USER + 2
//-----------------------------------------------------------
// MyThread.h : header file
//-----------------------------------------------------------
// CMyThread thread
//-----------------------------------------------------------
class CMyThread : public CWinThread
{
	DECLARE_DYNCREATE(CMyThread)
public:
	CMyThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyThread();

	// Generated message map functions
	//{{AFX_MSG(CMyThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------
public:
	BOOL		PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT		SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT		WndProc(UINT msg, WPARAM wp, LPARAM lp);
	//-------------------------------------------------------
private:
	BOOL		RegisterMsgWnd();
	BOOL		CreateMsgWnd();
	//-------------------------------------------------------
private:
	HWND		m_hMsgWnd;
	//-------------------------------------------------------
};
//-----------------------------------------------------------
#endif // !defined(AFX_MYTHREAD_H__331B3BFB_38BE_46F7_B966_A0D0CAEC4EBE__INCLUDED_)
//-----------------------------------------------------------
