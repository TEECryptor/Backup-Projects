// Test MenuDoc.cpp : implementation of the CTestMenuDoc class
//

#include "stdafx.h"
#include "Test Menu.h"

#include "Test MenuDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestMenuDoc

IMPLEMENT_DYNCREATE(CTestMenuDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestMenuDoc, CDocument)
	//{{AFX_MSG_MAP(CTestMenuDoc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestMenuDoc construction/destruction

CTestMenuDoc::CTestMenuDoc()
{
	// TODO: add one-time construction code here

}

CTestMenuDoc::~CTestMenuDoc()
{
}

BOOL CTestMenuDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestMenuDoc serialization

void CTestMenuDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CTestMenuDoc diagnostics

#ifdef _DEBUG
void CTestMenuDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestMenuDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestMenuDoc commands

void CTestMenuDoc::OnFileNew() 
{
	// TODO: Add your command handler code here
	
}
