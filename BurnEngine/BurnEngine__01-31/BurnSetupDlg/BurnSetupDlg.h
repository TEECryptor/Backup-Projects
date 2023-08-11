//-----------------------------------------------
//	BurnSetupDlg.h
//	Copyright (c) 2001 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------
#include "BurnEngine.h"
//-----------------------------------------------
class CWriteSetupDlg
{
public:
	CWriteSetupDlg(){};
	virtual ~CWriteSetupDlg(){};
	//-------------------------------------------
public:
	virtual SetBurnEngine(CBurnEngine* pBurnEngine) = 0;
	virtual SetImageFile(LPCTSTR lpszImageFile) = 0;
	//-------------------------------------------
};
//-----------------------------------------------
//-----------------------------------------------