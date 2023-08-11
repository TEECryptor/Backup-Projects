// HomepageView.cpp : implementation file
//

#include "stdafx.h"
#include "BMHoistManager.h"
#include "HomepageView.h"


// CHomepageView

IMPLEMENT_DYNCREATE(CHomepageView, CFormView)

CHomepageView::CHomepageView()
	: CFormView(CHomepageView::IDD)
{
	m_pKImage = NULL;
}

CHomepageView::~CHomepageView()
{
}

void CHomepageView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHomepageView, CFormView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CHomepageView diagnostics

#ifdef _DEBUG
void CHomepageView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CHomepageView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHomepageView message handlers

void CHomepageView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

}

BOOL CHomepageView::OnEraseBkgnd(CDC* pDC)
{
	if (!m_pKImage)
	{
		return CFormView::OnEraseBkgnd(pDC);
	}

	SCROLLINFO si;
	si.cbSize = sizeof (SCROLLINFO) ;
	si.fMask = SIF_ALL ;
	GetScrollInfo(SB_HORZ, &si);

	CRect rc;
	GetClientRect(&rc);
	//GetWindowRect(&rc);
	//this->ScreenToClient(&rc);
	int iWidth = rc.Width();
	int iHeight = rc.Height();
	HDC hDCMem = CreateCompatibleDC(pDC->m_hDC);
	int iOldDcBkMode = SetBkMode(pDC->m_hDC, TRANSPARENT);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDCMem, (HBITMAP)m_pKImage->m_hObject);

	BITMAP bm;
	m_pKImage->GetObject(sizeof(BITMAP), &bm);  
	StretchBlt(	pDC->m_hDC, 
				0, 0, iWidth, iHeight,
				hDCMem,
				GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT),
				iWidth<bm.bmWidth?iWidth:bm.bmWidth, iHeight<bm.bmHeight?iHeight:bm.bmHeight,
				SRCCOPY);
			
	SelectObject(hDCMem, hOldBitmap);
	SetBkMode(hDCMem, iOldDcBkMode);
	DeleteDC(hDCMem);

	return TRUE;

	//return CFormView::OnEraseBkgnd(pDC);
}

int CHomepageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_pKImage) delete m_pKImage;
	m_pKImage = new CBitmap();
	m_pKImage->LoadBitmap(IDB_BITMAP_HOMEPAGEBK);

	return 0;
}

void CHomepageView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	this->Invalidate();
}
