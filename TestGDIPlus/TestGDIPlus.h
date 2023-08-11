// TestGDIPlus.h : main header file for the TESTGDIPLUS application
//

#if !defined(AFX_TESTGDIPLUS_H__02579DB0_DB70_4D18_BFAD_CA7FB05E7700__INCLUDED_)
#define AFX_TESTGDIPLUS_H__02579DB0_DB70_4D18_BFAD_CA7FB05E7700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestGDIPlusApp:
// See TestGDIPlus.cpp for the implementation of this class
//

class CTestGDIPlusApp : public CWinApp
{
public:
	CTestGDIPlusApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestGDIPlusApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestGDIPlusApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTGDIPLUS_H__02579DB0_DB70_4D18_BFAD_CA7FB05E7700__INCLUDED_)
