/*
 * MainFrm.cpp
 * 
 * Implementation of class "CMainFrame"
 * (document class of application, which displays all countries of the
 *  European community)
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
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*** Definition of class "CMainFrame" ****************************************/

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

/*** Public member functions *************************************************/

/*** Will be called before this window will be created ***********************/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if( !CFrameWnd::PreCreateWindow(cs) )
    return FALSE;
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  return TRUE;
}

/*** Diagnostic functions ****************************************************/
#ifndef NDEBUG
void CMainFrame::AssertValid() const
{
  CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
  CFrameWnd::Dump(dc);
}
#endif // NDEBUG

/*** Protected member functions **********************************************/

/*** This window will be created *********************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  return 0;
}

/*** Message handlers ********************************************************/
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
  ON_WM_CREATE()
END_MESSAGE_MAP()
