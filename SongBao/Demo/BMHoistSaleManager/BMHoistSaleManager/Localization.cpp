//-----------------------------------------------------------------------
// Localization.cpp : implement file for localization functions
//			
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "Localization.h"
#include "BMHoistSaleManager.h"
#include "BMMessageDlg.h"
//-----------------------------------------------------------------------
HINSTANCE	g_hInst = 0;
HINSTANCE	g_hInstLoc = 0;	
DWORD		g_nLanguageId = 2052;
//-----------------------------------------------------------------------
//	Name:InitLocalization
//	Introduce:Load localization file
//-----------------------------------------------------------------------
void InitLocalization()
{
	if (!g_hInst)
		g_hInst = AfxGetResourceHandle();

	CString csLocfile;
	switch(g_nLanguageId)
	{
	case 2052:
		csLocfile = _T("_Chn.dll");
		break;
	case 1033:
	default:
		csLocfile = _T("_Enu.dll");
		break;
	}

	TCHAR buf[MAX_PATH];
	GetModuleFileName(g_hInst, buf, MAX_PATH);
	CString csName = buf;
	int i = csName.ReverseFind('.');
	csName = csName.Left(i);
	csName += csLocfile;
	g_hInstLoc =  LoadLibrary(csName);
	if(g_hInstLoc != NULL)
	{
		AfxSetResourceHandle(g_hInstLoc);
	}
}
//-----------------------------------------------------------------------
//	Name:myGetText
//	Introduce:Return text string based on id
//-----------------------------------------------------------------------
LPTSTR myGetText(int id, LPTSTR buf)
{
	static TCHAR textBuf[MAX_PATH*5];
	memset(textBuf, 0, sizeof(TCHAR)*MAX_PATH*5);

	if (!buf)
		buf = textBuf;

	if (g_hInstLoc)
		if (::LoadString(g_hInstLoc, id, buf, MAX_PATH*5))
			return buf;
		
	if (!::LoadString(g_hInst, id, buf, MAX_PATH*5))
	{
#ifdef _DEBUG
		wsprintf(buf, _T("## String (%u) not found ##"), id);
#else
		*buf = 0;
#endif	//_DEBUG
	}
	return buf;
}
//-----------------------------------------------------------------------
//	Name:myDoModal
//	Introduce:Show a dialog in modal mode
//-----------------------------------------------------------------------
INT_PTR myDoModal(CDialog *dlg, CWnd *cwndparent, int resId)
{
	HGLOBAL hDialogTemplate = 0;
	HRSRC hResource = ::FindResource(g_hInstLoc, MAKEINTRESOURCE(resId), RT_DIALOG);
	if (hResource)
		hDialogTemplate = LoadResource(g_hInstLoc, hResource);
	else
	{
		hResource = ::FindResource(g_hInst, MAKEINTRESOURCE(resId), RT_DIALOG);
		if (hResource)
			hDialogTemplate = LoadResource(g_hInst, hResource);
	}

	dlg->InitModalIndirect(hDialogTemplate, cwndparent);

	INT_PTR erg = dlg->DoModal();
	FreeResource(hDialogTemplate);

	return erg;
}
//-----------------------------------------------------------------------
int	myMessageBox(int iTextID, int iCaptionID, UINT nFlag)
{
	CString strText = myGetText(iTextID);
	CString strCaption = myGetText(iCaptionID);

	CBMMessageDlg dlg;
	dlg.SetMsgTextAndIcon(strCaption, strText, nFlag);
	UINT nResult = dlg.DoModal();

	return nResult;
}
//-----------------------------------------------------------------------