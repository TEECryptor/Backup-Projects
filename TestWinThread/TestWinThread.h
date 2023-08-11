// TestWinThread.h : main header file for the TESTWINTHREAD application
//

#if !defined(AFX_TESTWINTHREAD_H__F163A286_21D3_497C_814F_D83DCB7F7F16__INCLUDED_)
#define AFX_TESTWINTHREAD_H__F163A286_21D3_497C_814F_D83DCB7F7F16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestWinThreadApp:
// See TestWinThread.cpp for the implementation of this class
//

class CTestWinThreadApp : public CWinApp
{
public:
	CTestWinThreadApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestWinThreadApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestWinThreadApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTWINTHREAD_H__F163A286_21D3_497C_814F_D83DCB7F7F16__INCLUDED_)
