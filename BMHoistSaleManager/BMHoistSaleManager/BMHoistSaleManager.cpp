//-----------------------------------------------------------------------
//	File Name:BMHoistSaleManager.cpp
//	Introduce:Defines the class behaviors for the application.
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMHoistSaleManagerDlg.h"
#include "BMLoginDlg.h"
#include "PasswordHelper.h"
#include <shlwapi.h>
//-----------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMHoistSaleManagerApp
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMHoistSaleManagerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------
// CBMHoistSaleManagerApp construction
//-----------------------------------------------------------------------
CBMHoistSaleManagerApp::CBMHoistSaleManagerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
//-----------------------------------------------------------------------
// The one and only CBMHoistSaleManagerApp object
//-----------------------------------------------------------------------
CBMHoistSaleManagerApp theApp;
//-----------------------------------------------------------------------
LPCWSTR GetDatabasePassword(CString strExePath)
{
	CString strPswDataFile = strExePath;
	strPswDataFile += _T("BMCert.dat");

	static TCHAR tcPassword[256] = {0};
	memset(tcPassword, 0, sizeof(TCHAR)*256);
	FILE* file = _tfopen(strPswDataFile, _T("rb"));
	if (file)
	{
		TCHAR tcData[256] = {0};
		fread(tcData, sizeof(TCHAR), 256, file);
		fclose(file);

		LPCWSTR lpResult = CPasswordHelper::DecodePasswordString(tcData);
		_tcscpy(tcPassword, lpResult);
	}

	return tcPassword;
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CBMHoistSaleManagerApp initialization
//-----------------------------------------------------------------------
BOOL CBMHoistSaleManagerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CString strExePath;
	TCHAR tcExeFile[MAX_PATH] = {_T("\0")};	
	GetModuleFileName(NULL, tcExeFile, MAX_PATH);
	strExePath = tcExeFile;
	int iPos = strExePath.ReverseFind(_T('\\'));
	strExePath = strExePath.Left(iPos + 1);

	// Get database password
	LPCWSTR lpwzDBPassword = GetDatabasePassword(strExePath);
	LPCWSTR lpwzOfferPsw = CPasswordHelper::DecodePasswordString(STR_OFFER_PSW);

	// Try to open databases
	CString strSettingDBFile = strExePath;
	CString strOfferDBFile = strExePath;
	strSettingDBFile += _T("BMHoistSetting.mdb");
	strOfferDBFile += _T("BMHoistOffer.mdb");
	if (!PathFileExists(strOfferDBFile) || !PathFileExists(strSettingDBFile))
	{		
		MessageBox(NULL, _T("Try to open database files failed, application can't find the necessary database files!\nPlease make sure database files 'BMHoistSetting.mdb' and 'BMHoistOffer.mdb' are in current folder."), _T("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	//
	g_pSettingDatabase = new CADODataBase();
	g_pOfferDatabase = new CADODataBase();
	if (!g_pSettingDatabase->OpenDatabase(strSettingDBFile, lpwzDBPassword) ||
		!g_pOfferDatabase->OpenDatabase(strOfferDBFile, lpwzOfferPsw))
	{
		MessageBox(NULL, _T("Try to open database files failed, maybe you have an incorrect password file!\nPlease contact your administrator to get correct password file."), _T("Error"), MB_OK | MB_ICONERROR);
		delete g_pSettingDatabase;
		delete g_pOfferDatabase;
		g_pOfferDatabase = NULL;
		g_pSettingDatabase = NULL;
		return FALSE;
	}	

	CBMLoginDlg dlgLogin;
	if (IDOK != dlgLogin.DoModal())
		return FALSE;

	// Show main dialog
	g_nLanguageId = 2052;
	InitLocalization();

	CBMHoistSaleManagerDlg dlg;
	m_pMainWnd = &dlg;
	//INT_PTR nResponse = dlg.DoModal();
	INT_PTR nResponse = myDoModal(&dlg, NULL, IDD_BMHOISTSALEMANAGER_DIALOG);
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	delete g_pSettingDatabase;
	delete g_pOfferDatabase;
	g_pOfferDatabase = NULL;
	g_pSettingDatabase = NULL;
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
//-----------------------------------------------------------------------
