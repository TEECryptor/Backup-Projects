// TestFile.h : main header file for the TESTFILE application
//

#if !defined(AFX_TESTFILE_H__4FD2490E_C946_4D25_BC59_91B32B0D8EF3__INCLUDED_)
#define AFX_TESTFILE_H__4FD2490E_C946_4D25_BC59_91B32B0D8EF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestFileApp:
// See TestFile.cpp for the implementation of this class
//

class CTestFileApp : public CWinApp
{
public:
	CTestFileApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestFileApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestFileApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTFILE_H__4FD2490E_C946_4D25_BC59_91B32B0D8EF3__INCLUDED_)
