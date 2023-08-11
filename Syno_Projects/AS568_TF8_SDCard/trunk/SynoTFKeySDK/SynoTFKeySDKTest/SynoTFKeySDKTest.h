// SynoTFKeySDKTest.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSynoTFKeySDKTestApp:
// See SynoTFKeySDKTest.cpp for the implementation of this class
//

class CSynoTFKeySDKTestApp : public CWinApp
{
public:
	CSynoTFKeySDKTestApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSynoTFKeySDKTestApp theApp;