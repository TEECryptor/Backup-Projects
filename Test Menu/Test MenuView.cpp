// Test MenuView.cpp : implementation of the CTestMenuView class
//

#include "stdafx.h"
#include "Test Menu.h"

#include "Test MenuDoc.h"
#include "Test MenuView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestMenuView

IMPLEMENT_DYNCREATE(CTestMenuView, CView)

BEGIN_MESSAGE_MAP(CTestMenuView, CView)
	//{{AFX_MSG_MAP(CTestMenuView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestMenuView construction/destruction

CTestMenuView::CTestMenuView()
{
	// TODO: add construction code here

}

CTestMenuView::~CTestMenuView()
{
}

BOOL CTestMenuView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestMenuView drawing

void CTestMenuView::OnDraw(CDC* pDC)
{
	CTestMenuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTestMenuView printing

BOOL CTestMenuView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestMenuView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestMenuView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestMenuView diagnostics

#ifdef _DEBUG
void CTestMenuView::AssertValid() const
{
	CView::AssertValid();
}

void CTestMenuView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestMenuDoc* CTestMenuView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestMenuDoc)));
	return (CTestMenuDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestMenuView message handlers
