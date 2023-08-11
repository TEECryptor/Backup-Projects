// TestImageLibraryView.cpp : implementation of the CTestImageLibraryView class
//

#include "stdafx.h"
#include "TestImageLibrary.h"

#include "TestImageLibraryDoc.h"
#include "TestImageLibraryView.h"
#include <iostream>
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestImageLibraryView

IMPLEMENT_DYNCREATE(CTestImageLibraryView, CView)

BEGIN_MESSAGE_MAP(CTestImageLibraryView, CView)
	//{{AFX_MSG_MAP(CTestImageLibraryView)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestImageLibraryView construction/destruction

CTestImageLibraryView::CTestImageLibraryView()
{
	// TODO: add construction code here
	m_pImage = NULL;

}

CTestImageLibraryView::~CTestImageLibraryView()
{
	if(m_pImage)
	{
		LibImage_Destroy(m_pImage);
		m_pImage = NULL;
	}
}

BOOL CTestImageLibraryView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestImageLibraryView drawing

void CTestImageLibraryView::OnDraw(CDC* pDC)
{
	CTestImageLibraryDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	RECT rect;
	GetClientRect(&rect);
	
	HDC hDCMem = CreateCompatibleDC(pDC->m_hDC);
	HBITMAP hTmpMemBmp = CreateCompatibleBitmap(pDC->m_hDC, rect.right - rect.left, rect.bottom - rect.top);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDCMem, hTmpMemBmp);	
	
	if(m_pImage)
	{
		int iLeft = ((rect.right - rect.left) - m_pImage->GetImageWidth()) / 2;
		int iTop = ((rect.bottom - rect.top) - m_pImage->GetImageHeight()) / 2;

		m_pImage->Draw(hDCMem, iLeft, iTop, m_pImage->GetImageWidth(), m_pImage->GetImageHeight());
	}

	BitBlt(	pDC->m_hDC, 
			rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
			hDCMem,
			rect.left, rect.top,
			SRCCOPY);

	SelectObject(pDC->m_hDC, hOldBitmap);
	DeleteObject(hTmpMemBmp);
	DeleteDC(hDCMem);
}

/////////////////////////////////////////////////////////////////////////////
// CTestImageLibraryView printing

BOOL CTestImageLibraryView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestImageLibraryView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestImageLibraryView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestImageLibraryView diagnostics

#ifdef _DEBUG
void CTestImageLibraryView::AssertValid() const
{
	CView::AssertValid();
}

void CTestImageLibraryView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestImageLibraryDoc* CTestImageLibraryView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestImageLibraryDoc)));
	return (CTestImageLibraryDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestImageLibraryView message handlers

void CTestImageLibraryView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}




void CTestImageLibraryView::OnFileOpen() 
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, "Bitmap Image Files (*.bmp)|*.bmp|Tga Image Files (*.tga)|*.tga||", this);   

	if( dlg.DoModal ()==IDOK )
	{
		CString pathName = dlg.GetPathName(); 
	
		if(m_pImage) LibImage_Destroy(m_pImage);
		m_pImage = LibImage_Create();
		if(m_pImage)
		{
			m_pImage->LoadImageFromFile(pathName.GetBuffer(pathName.GetLength()));
			/*	
			ifstream is;
			is.open (pathName.GetBuffer(pathName.GetLength()), ios::binary);

			// get length of file:
			is.seekg (0, ios::end);
			int length = is.tellg();
			is.seekg (0, ios::beg);

			// allocate memory:
			char* buffer = new char[length];
			is.read(&buffer[0], length);
			is.close();

			m_pImage->LoadImageFromBuff((UCHAR*)buffer, length, eTga);
			delete []buffer;
			buffer = NULL;
*/
			pathName.ReleaseBuffer();
			Invalidate();
		}
	}
}

void CTestImageLibraryView::OnFileSaveAs() 
{
	if(!m_pImage) 
		return;

	CFileDialog dlg(FALSE, "BMP", NULL, OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, "Bitmap Image Files (*.bmp)|*.bmp|Tga Image Files (*.tga)|*.tga||", this);   

	if( dlg.DoModal ()==IDOK )
	{
		CString pathName = dlg.GetPathName();
		pathName.MakeLower();
		
		eImageFormat type = eUnknow;
		if(pathName.Find(".bmp") != -1)
			type = eBitmap;
		else if(pathName.Find(".tga") != -1)
			type = eTga;
		else if(pathName.Find(".jpg") != -1)
			type = eJpg;

		m_pImage->SaveImageAs(type, pathName.GetBuffer(pathName.GetLength()));

		Invalidate();
	}
}

BOOL CTestImageLibraryView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return false;//CView::OnEraseBkgnd(pDC);
}
