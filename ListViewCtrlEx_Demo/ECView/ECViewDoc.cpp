/*
 * ECViewDoc.cpp
 * 
 * Implementation of class "CECViewDoc"
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
#include "ECViewDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*** Definition of class "CECViewDoc" ****************************************/

IMPLEMENT_DYNCREATE(CECViewDoc, CDocument)

/*** Public member functions *************************************************/

/*** A new document will be created ******************************************/
BOOL CECViewDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  // TODO: add reinitialization code here
  // (SDI documents will reuse this document)

  return TRUE;
}

/*** (De-)serialize document *************************************************/

void CECViewDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    // TODO: add storing code here
  }
  else
  {
    // TODO: add loading code here
  }
}

/*** Diagnostic functions ****************************************************/
#ifndef NDEBUG
void CECViewDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CECViewDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/*** Message handlers ********************************************************/
BEGIN_MESSAGE_MAP(CECViewDoc, CDocument)
END_MESSAGE_MAP()
