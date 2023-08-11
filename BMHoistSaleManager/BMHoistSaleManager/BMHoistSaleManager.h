//-----------------------------------------------------------------------
//	File Name:BMHoistSaleManager.h
//	Introduce:main header file for the PROJECT_NAME application
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif
//-----------------------------------------------------------------------
#include "resource.h"		// main symbols
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMHoistSaleManagerApp:
// See BMHoistSaleManager.cpp for the implementation of this class
//
//-----------------------------------------------------------------------
class CBMHoistSaleManagerApp : public CWinApp
{
public:
	CBMHoistSaleManagerApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};
//-----------------------------------------------------------------------
extern CBMHoistSaleManagerApp theApp;
//-----------------------------------------------------------------------