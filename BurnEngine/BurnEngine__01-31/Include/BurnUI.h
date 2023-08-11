//-----------------------------------------------
//	BurnUI.h
//	Copyright (c) 2001 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------
#ifndef _BURNUI_H_
#define _BURNUI_H_
//-----------------------------------------------
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
//-----------------------------------------------
#include "resource.h"		// main symbols
//-----------------------------------------------    
#define DllExport __declspec( dllexport )
//-----------------------------------------------
// CBurnUIApp
// See BurnUI.cpp for the implementation of this class
//-----------------------------------------------
class Cburn_uiApp : public CWinApp
{
public:
	Cburn_uiApp();
    virtual ~Cburn_uiApp()
    {
        CoUninitialize();
    }
// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
//-----------------------------------------------


//-----------------------------------------------
//	Name:PartialInfo
//	Introduce:Define partial information for progress dialog
//-----------------------------------------------
class PartialInfo
{
	float fStartPos;
	float fEndPos;
};
//-----------------------------------------------


//-----------------------------------------------
//	Name:IWriteSetupDlg
//	Introduce:Write setup dialog interface definition
//-----------------------------------------------
class IWriteSetupDlg
{
public:
	virtual void SetBurnEngine(void*) = 0;
    // return IDOK or IDCANCEL
	virtual INT_PTR Show() = 0;

    virtual void SetClientName(LPCTSTR lpName) = 0;
    virtual void UsePreSelectedTargetDevice(bool bUse = true) = 0;
    virtual void UsePreSelectedWriteSpeed(bool bUse = true) = 0;
    virtual void UsePreSelectedNumberOfCopies(bool bUse = true) = 0;
    virtual void EstimateProjectSizeInBlocks(ULONG uDiscSize) = 0;
    virtual void SetImageFileName(LPCTSTR lpFileName) = 0;

};
//-----------------------------------------------
//	Name:IProgressDlg
//	Introduce:Progress dialog interface definition
//-----------------------------------------------
class IProgressDlg
{
public:
	virtual bool SetBurnEngine(void*) = 0;
    // return IDOK or IDCANCEL
	virtual INT_PTR Show() = 0;

    virtual void SetPartialCnt(int iNumber) = 0;
    virtual void SetPartialInfo(int iID, PartialInfo&) = 0;
    virtual void SetCurrentPartial(int iID) = 0;
    virtual void SetCurrentPos(float fPos = -1) = 0;
    virtual void SetThumbnail(LPVOID pBitmap);
};
//-----------------------------------------------



//-----------------------------------------------
DllExport IWriteSetupDlg* CreateInstanceWriteSetup(CWnd* pParent = NULL);
DllExport void RemoveInstanceWriteSetup(IWriteSetupDlg*);
DllExport IProgressDlg* CreateInstanceProgress(CWnd* pParent = NULL);
DllExport void RemoveInstanceProgress(IProgressDlg*);
//-----------------------------------------------

