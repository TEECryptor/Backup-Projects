// TestMediaHost.h : main header file for the TESTMEDIAHOST application
//

#if !defined(AFX_TESTMEDIAHOST_H__45679FCC_BA54_4F26_A2D3_86731702B2A6__INCLUDED_)
#define AFX_TESTMEDIAHOST_H__45679FCC_BA54_4F26_A2D3_86731702B2A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestMediaHostApp:
// See TestMediaHost.cpp for the implementation of this class
//

class CTestMediaHostApp : public CWinApp
{
public:
	CTestMediaHostApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestMediaHostApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestMediaHostApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTMEDIAHOST_H__45679FCC_BA54_4F26_A2D3_86731702B2A6__INCLUDED_)
