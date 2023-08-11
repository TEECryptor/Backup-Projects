/*
 * ECViewApp.cpp
 * 
 * Implementation of class "CECViewApp"
 * (application, which displays all countries of the European community)
 * 
 * Date: 15.09.2010
 * 
 * Author(s):
 * Thomas Holte
 * 
 * Copyright (c) 2010 Thomas Holte
 * All Rights Reserved
 * 
 */

#include "stdafx.h"
#include "ECView.h"
#include "ECViewApp.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*** Definition of class "CAboutDlg" *****************************************/
class CAboutDlg : public CDialog
{
  public:
  // Dialog Data
  enum {IDD = IDD_ABOUTBOX};

  CAboutDlg(): CDialog(IDD) {}

  // ClassWizard generated virtual function overrides
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
};

/*** Protected member functions **********************************************/

/*** Data exchange: controls <--> member variables ***************************/
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}

/*** Message handlers ********************************************************/
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


/*** Definition of class "CECViewApp" ****************************************/

CECViewApp theApp;    // The one and only CECViewApp object

/*** Public member functions *************************************************/

/*** Will be called once after loading this application **********************/
BOOL CECViewApp::InitInstance()
{
  if (!AfxOleInit()) return FALSE;

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.

  // Change the registry key under which our settings are stored.
  // TODO: You should modify this string to be something appropriate
  // such as the name of your company or organization.
  SetRegistryKey(_T("Local AppWizard-Generated Applications"));

  LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views.

  CSingleDocTemplate* pDocTemplate;
  pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(CECViewDoc),
    RUNTIME_CLASS(CMainFrame),       // main SDI frame window
    RUNTIME_CLASS(CECView));
  AddDocTemplate(pDocTemplate);

  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

  // Dispatch commands specified on the command line
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

  // The one and only window has been initialized, so show and update it.
  m_pMainWnd->ShowWindow(SW_SHOW);
  m_pMainWnd->UpdateWindow();

  return TRUE;
}

/*** The info dialog shall be shown ******************************************/
void CECViewApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

/*** Message handlers ********************************************************/
BEGIN_MESSAGE_MAP(CECViewApp, CWinApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  // Standard file based document commands
  ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()
