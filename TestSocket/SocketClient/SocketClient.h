// SocketClient.h : main header file for the SOCKETCLIENT application
//

#if !defined(AFX_SOCKETCLIENT_H__9EC734C8_DAFA_406E_BA3B_34E9E5BBE9CE__INCLUDED_)
#define AFX_SOCKETCLIENT_H__9EC734C8_DAFA_406E_BA3B_34E9E5BBE9CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSocketClientApp:
// See SocketClient.cpp for the implementation of this class
//

class CSocketClientApp : public CWinApp
{
public:
	CSocketClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSocketClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETCLIENT_H__9EC734C8_DAFA_406E_BA3B_34E9E5BBE9CE__INCLUDED_)
