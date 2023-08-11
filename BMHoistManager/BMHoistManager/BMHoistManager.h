// BMHoistManager.h : main header file for the BMHoistManager application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBMHoistManagerApp:
// See BMHoistManager.cpp for the implementation of this class
//

class CBMHoistManagerApp : public CWinApp
{
public:
	CBMHoistManagerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
private:	
	BOOL		OpenDatabase();

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBMHoistManagerApp theApp;