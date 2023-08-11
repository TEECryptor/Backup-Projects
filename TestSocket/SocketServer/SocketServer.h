// SocketServer.h : main header file for the SOCKETSERVER application
//

#if !defined(AFX_SOCKETSERVER_H__D7181813_8ECF_496A_93A7_0F393F05B8B9__INCLUDED_)
#define AFX_SOCKETSERVER_H__D7181813_8ECF_496A_93A7_0F393F05B8B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSocketServerApp:
// See SocketServer.cpp for the implementation of this class
//

class CSocketServerApp : public CWinApp
{
public:
	CSocketServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSocketServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETSERVER_H__D7181813_8ECF_496A_93A7_0F393F05B8B9__INCLUDED_)
