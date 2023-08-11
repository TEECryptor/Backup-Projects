// RussiaServer.h : main header file for the RUSSIASERVER application
//

#if !defined(AFX_RUSSIASERVER_H__88907D5E_6B83_4984_9380_782E4E42EE0A__INCLUDED_)
#define AFX_RUSSIASERVER_H__88907D5E_6B83_4984_9380_782E4E42EE0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRussiaServerApp:
// See RussiaServer.cpp for the implementation of this class
//

class CRussiaServerApp : public CWinApp
{
public:
	CRussiaServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRussiaServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRussiaServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUSSIASERVER_H__88907D5E_6B83_4984_9380_782E4E42EE0A__INCLUDED_)
