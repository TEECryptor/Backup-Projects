// TestD3D.h : main header file for the TESTD3D application
//

#if !defined(AFX_TESTD3D_H__2D4AA0A9_8053_4483_AFFB_FC91B4D6CCE7__INCLUDED_)
#define AFX_TESTD3D_H__2D4AA0A9_8053_4483_AFFB_FC91B4D6CCE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestD3DApp:
// See TestD3D.cpp for the implementation of this class
//

class CTestD3DApp : public CWinApp
{
public:
	CTestD3DApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestD3DApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestD3DApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTD3D_H__2D4AA0A9_8053_4483_AFFB_FC91B4D6CCE7__INCLUDED_)
