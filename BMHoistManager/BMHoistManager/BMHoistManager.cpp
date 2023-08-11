// BMHoistManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BMHoistManager.h"
#include "MainFrm.h"
#include "LoginDlg.h"
#include "./Database/ADODataBase.h"


CADODataBase*	g_pDatabase;
CString			g_strLoginName;
CString			g_strUserName;
CString			g_strUserGroup;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBMHoistManagerApp

BEGIN_MESSAGE_MAP(CBMHoistManagerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CBMHoistManagerApp::OnAppAbout)
END_MESSAGE_MAP()


// CBMHoistManagerApp construction

CBMHoistManagerApp::CBMHoistManagerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CBMHoistManagerApp object

CBMHoistManagerApp theApp;


// CBMHoistManagerApp initialization

BOOL CBMHoistManagerApp::InitInstance()
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

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	::CoInitialize(NULL);

	// 连接数据库
	if (!OpenDatabase())
	{
		return FALSE;
	}

	CLoginDlg dlgLogin;
#ifdef _DEBUG
	dlgLogin.m_strLoginName = "LBY";
	dlgLogin.m_strUserName = "刘宝玉";
	dlgLogin.m_strUserGroup = "系统管理员";
#else
	dlgLogin.m_pDatabase = g_pDatabase;
	if (dlgLogin.DoModal() != IDOK)
	{
		return TRUE;
	}
#endif	//DEBUG


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	g_strLoginName = dlgLogin.m_strLoginName;
	g_strUserName = dlgLogin.m_strUserName;
	g_strUserGroup = dlgLogin.m_strUserGroup;
	pFrame->m_strLoginName = dlgLogin.m_strLoginName;
	pFrame->m_strUserName = dlgLogin.m_strUserName;
	pFrame->m_strUserGroup = dlgLogin.m_strUserGroup;

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_NORMAL);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	return TRUE;
}


// CBMHoistManagerApp message handlers

BOOL CBMHoistManagerApp::OpenDatabase()
{
	TCHAR tcExePath[MAX_PATH];
	CString strDatabaseFile;
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	strDatabaseFile = tcExePath;
	int iPos = strDatabaseFile.ReverseFind(_T('\\'));
	strDatabaseFile = strDatabaseFile.Left(iPos + 1);
	strDatabaseFile += _T("BMDatabase.mdb");

	g_pDatabase = new CADODataBase();
	if (!g_pDatabase->OpenDatabase(strDatabaseFile))
	{
		delete g_pDatabase;
		g_pDatabase = NULL;
		MessageBox(NULL, _T("打开数据库文件错误，请确认数据库文件是否存在!"), _T("错误"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CBMHoistManagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CBMHoistManagerApp message handlers

