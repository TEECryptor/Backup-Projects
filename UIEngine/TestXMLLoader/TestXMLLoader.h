// TestXMLLoader.h : main header file for the TESTXMLLOADER application
//

#if !defined(AFX_TESTXMLLOADER_H__9F35E603_7508_4871_BF06_CA1473C6283E__INCLUDED_)
#define AFX_TESTXMLLOADER_H__9F35E603_7508_4871_BF06_CA1473C6283E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestXMLLoaderApp:
// See TestXMLLoader.cpp for the implementation of this class
//

class CTestXMLLoaderApp : public CWinApp
{
public:
	CTestXMLLoaderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestXMLLoaderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestXMLLoaderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTXMLLOADER_H__9F35E603_7508_4871_BF06_CA1473C6283E__INCLUDED_)
