// Test_Sun3D360.h : main header file for the TEST_SUN3D360 application
//

#if !defined(AFX_TEST_SUN3D360_H__BB52B500_A82B_43D8_BF83_1410FADF15A8__INCLUDED_)
#define AFX_TEST_SUN3D360_H__BB52B500_A82B_43D8_BF83_1410FADF15A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTest_Sun3D360App:
// See Test_Sun3D360.cpp for the implementation of this class
//

class CTest_Sun3D360App : public CWinApp
{
public:
	CTest_Sun3D360App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_Sun3D360App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTest_Sun3D360App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_SUN3D360_H__BB52B500_A82B_43D8_BF83_1410FADF15A8__INCLUDED_)
