// TestRollLine.h : main header file for the TESTROLLLINE application
//

#if !defined(AFX_TESTROLLLINE_H__F754CFB6_6141_429A_BDCC_E13663C4600B__INCLUDED_)
#define AFX_TESTROLLLINE_H__F754CFB6_6141_429A_BDCC_E13663C4600B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestRollLineApp:
// See TestRollLine.cpp for the implementation of this class
//

class CTestRollLineApp : public CWinApp
{
public:
	CTestRollLineApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestRollLineApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestRollLineApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTROLLLINE_H__F754CFB6_6141_429A_BDCC_E13663C4600B__INCLUDED_)
