// SunMaxScreenClient.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSunMaxScreenClientApp:
// See SunMaxScreenClient.cpp for the implementation of this class
//

class CSunMaxScreenClientApp : public CWinApp
{
public:
	CSunMaxScreenClientApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSunMaxScreenClientApp theApp;