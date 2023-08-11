// TestComm.h : main header file for the TESTCOMM application
//

#if !defined(AFX_TESTCOMM_H__4E6942EF_AA4C_495A_A002_07F029069D9F__INCLUDED_)
#define AFX_TESTCOMM_H__4E6942EF_AA4C_495A_A002_07F029069D9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestCommApp:
// See TestComm.cpp for the implementation of this class
//

class CTestCommApp : public CWinApp
{
public:
	CTestCommApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestCommApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestCommApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCOMM_H__4E6942EF_AA4C_495A_A002_07F029069D9F__INCLUDED_)
