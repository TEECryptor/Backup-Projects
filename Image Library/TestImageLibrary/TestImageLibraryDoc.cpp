// TestImageLibraryDoc.cpp : implementation of the CTestImageLibraryDoc class
//

#include "stdafx.h"
#include "TestImageLibrary.h"

#include "TestImageLibraryDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestImageLibraryDoc

IMPLEMENT_DYNCREATE(CTestImageLibraryDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestImageLibraryDoc, CDocument)
	//{{AFX_MSG_MAP(CTestImageLibraryDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestImageLibraryDoc construction/destruction

CTestImageLibraryDoc::CTestImageLibraryDoc()
{
	// TODO: add one-time construction code here

}

CTestImageLibraryDoc::~CTestImageLibraryDoc()
{
}

BOOL CTestImageLibraryDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestImageLibraryDoc serialization

void CTestImageLibraryDoc::Serialize(CArchive& ar)
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
// CTestImageLibraryDoc diagnostics

#ifdef _DEBUG
void CTestImageLibraryDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestImageLibraryDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestImageLibraryDoc commands
