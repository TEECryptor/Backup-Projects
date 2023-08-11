// HandDiscInfo.h : main header file for the HANDDISCINFO application
//

#if !defined(AFX_HANDDISCINFO_H__2764A7AF_E88E_44C0_9CEA_24DF487EAA93__INCLUDED_)
#define AFX_HANDDISCINFO_H__2764A7AF_E88E_44C0_9CEA_24DF487EAA93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHandDiscInfoApp:
// See HandDiscInfo.cpp for the implementation of this class
//

class CHandDiscInfoApp : public CWinApp
{
public:
	CHandDiscInfoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHandDiscInfoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHandDiscInfoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANDDISCINFO_H__2764A7AF_E88E_44C0_9CEA_24DF487EAA93__INCLUDED_)
