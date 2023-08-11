// TestBTRender.h : main header file for the TESTBTRENDER application
//

#if !defined(AFX_TESTBTRENDER_H__3061EFA3_3FC4_43BB_B80A_66532E294E40__INCLUDED_)
#define AFX_TESTBTRENDER_H__3061EFA3_3FC4_43BB_B80A_66532E294E40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestBTRenderApp:
// See TestBTRender.cpp for the implementation of this class
//

class CTestBTRenderApp : public CWinApp
{
public:
	CTestBTRenderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestBTRenderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestBTRenderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTBTRENDER_H__3061EFA3_3FC4_43BB_B80A_66532E294E40__INCLUDED_)
