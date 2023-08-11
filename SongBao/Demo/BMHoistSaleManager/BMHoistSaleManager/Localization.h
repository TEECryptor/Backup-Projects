//-----------------------------------------------------------------------
//	File Name:Localization.h
//	Introduce:Define public functions for localization
//-----------------------------------------------------------------------
#ifndef	_LOCALIZATION_H_
#define	_LOCALIZATION_H_
//-----------------------------------------------------------------------
extern HINSTANCE	g_hInst;
extern HINSTANCE	g_hInstLoc;
extern DWORD		g_nLanguageId;
//-----------------------------------------------------------------------
void	InitLocalization();
LPTSTR	myGetText(int id, LPTSTR buf = NULL);
INT_PTR myDoModal(CDialog *dlg, CWnd *cwndparent, int resId);
int		myMessageBox(int iTextID, int iCaptionID, UINT nFlag);
//-----------------------------------------------------------------------
#endif	//_LOCALIZATION_H_
//-----------------------------------------------------------------------