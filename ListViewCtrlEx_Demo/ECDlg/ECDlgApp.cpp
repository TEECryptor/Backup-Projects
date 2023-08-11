/*
 * ECDlgApp.cpp
 * 
 * Implementation of class "CECDlgApp"
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
#include "ECDlgApp.h"
#include "ECDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*** Definition of class "CECDlgApp" *****************************************/

/*** The one and only CECDlgApp object ***************************************/
CECDlgApp theApp;

/*** Public member functions *************************************************/

/*** Will be called at application start *************************************/
BOOL CECDlgApp::InitInstance()
{
  if (!AfxOleInit()) return FALSE;

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.

  CECDlg dlg;
  m_pMainWnd = &dlg;
  INT_PTR nResponse = dlg.DoModal();
  if (nResponse == IDOK)
  {
    // TODO: Place code here to handle when the dialog is
    //  dismissed with OK
  }
  else if (nResponse == IDCANCEL)
  {
    // TODO: Place code here to handle when the dialog is
    //  dismissed with Cancel
  }

  // Since the dialog has been closed, return FALSE so that we exit the
  //  application, rather than start the application's message pump.
  return FALSE;
}

/*** Message handler table ***************************************************/
BEGIN_MESSAGE_MAP(CECDlgApp, CWinApp)
  ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()
