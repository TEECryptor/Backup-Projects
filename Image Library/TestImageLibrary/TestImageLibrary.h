// TestImageLibrary.h : main header file for the TESTIMAGELIBRARY application
//

#if !defined(AFX_TESTIMAGELIBRARY_H__E20D49CF_078B_47A3_A3E5_CDBA506150A1__INCLUDED_)
#define AFX_TESTIMAGELIBRARY_H__E20D49CF_078B_47A3_A3E5_CDBA506150A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestImageLibraryApp:
// See TestImageLibrary.cpp for the implementation of this class
//

class CTestImageLibraryApp : public CWinApp
{
public:
	CTestImageLibraryApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestImageLibraryApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTestImageLibraryApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTIMAGELIBRARY_H__E20D49CF_078B_47A3_A3E5_CDBA506150A1__INCLUDED_)
