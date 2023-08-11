// Sun3D360 Tool.h : main header file for the SUN3D360 TOOL application
//

#if !defined(AFX_SUN3D360TOOL_H__879AFB07_F746_4D5F_891C_64FB0C5F4622__INCLUDED_)
#define AFX_SUN3D360TOOL_H__879AFB07_F746_4D5F_891C_64FB0C5F4622__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSun3D360ToolApp:
// See Sun3D360 Tool.cpp for the implementation of this class
//

class CSun3D360ToolApp : public CWinApp
{
public:
	CSun3D360ToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSun3D360ToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSun3D360ToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUN3D360TOOL_H__879AFB07_F746_4D5F_891C_64FB0C5F4622__INCLUDED_)
