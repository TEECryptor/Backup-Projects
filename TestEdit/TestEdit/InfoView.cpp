// InfoView.cpp : implementation file
//

#include "stdafx.h"
#include "TestEdit.h"
#include "InfoView.h"


// CInfoView

IMPLEMENT_DYNCREATE(CInfoView, CScrollView)

CInfoView::CInfoView()
{
	m_clrText = RGB(0, 0, 0);
	m_clrBK = RGB(255, 255, 255);
	//
	m_stuViewSize.cx = INFOVIEW_WIDTH;
	m_stuViewSize.cy = INFOVIEW_HEIGHT;
}

CInfoView::~CInfoView()
{
}


BEGIN_MESSAGE_MAP(CInfoView, CScrollView)
END_MESSAGE_MAP()


// CInfoView drawing

void CInfoView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CInfoView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	HDC hDCMem = CreateCompatibleDC(pDC->m_hDC);
	int iOldMemBkMode = SetBkMode(hDCMem, TRANSPARENT);	
	HBITMAP hMemBmp = CreateCompatibleBitmap(pDC->m_hDC, m_stuViewSize.cx, m_stuViewSize.cy);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDCMem, hMemBmp);	

	HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
	FillRect(hDCMem, CRect(0, 0, m_stuViewSize.cx, m_stuViewSize.cy), hBrush);
	DeleteObject(hBrush);

	DrawStrings(hDCMem);
	
	BitBlt(	pDC->m_hDC, 
			0, 0, m_stuViewSize.cx, m_stuViewSize.cy,
			hDCMem,
			0, 0,
			SRCCOPY);
			
	SelectObject(pDC->m_hDC, hOldBitmap);
	SetBkMode(hDCMem, iOldMemBkMode);
	DeleteObject(hMemBmp);
	DeleteDC(hDCMem);
}


// CInfoView diagnostics

#ifdef _DEBUG
void CInfoView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CInfoView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


void CInfoView::DrawStrings(HDC hdc)
{
	int iCurTop = 10;
	CSize sizeMax(1, 1);

	HBRUSH hBrush = CreateSolidBrush(m_clrBK);

	HFONT hFont = CreateFontIndirect(&m_logFont);
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
	COLORREF clrOld = ::SetTextColor(hdc, m_clrText);

	POSITION pos = m_lstString.GetHeadPosition();
	while(pos)
	{
		CString strLine = m_lstString.GetNext(pos);

		CRect rcText(0, 0, 0, 0);
		DrawText(hdc, strLine, strLine.GetLength(), &rcText, DT_CALCRECT|DT_LEFT|DT_TOP);
		sizeMax.cx = max(sizeMax.cx, rcText.Width()+10+10);

		CRect rcDraw(CPoint(10, iCurTop), CPoint(10+rcText.Width(), iCurTop+rcText.Height()));
		FillRect(hdc, rcDraw, hBrush);
		DrawText(hdc, strLine, strLine.GetLength(), &rcDraw, DT_LEFT|DT_TOP);
		sizeMax.cy = max(sizeMax.cy, rcDraw.bottom+10);

		iCurTop += (10+rcText.Height());
	}
	SelectObject(hdc, hOldFont);
	::SetTextColor(hdc, clrOld);
	DeleteObject(hFont);
	DeleteObject(hBrush);

	m_stuViewSize.cx = INFOVIEW_WIDTH;
	m_stuViewSize.cy = INFOVIEW_HEIGHT;

	if(sizeMax.cx > INFOVIEW_WIDTH)
	{
		m_stuViewSize.cx = sizeMax.cx;
	}
	else
	{
		sizeMax.cx = 1;
	}

	if(sizeMax.cy > INFOVIEW_HEIGHT)
	{
		m_stuViewSize.cy = sizeMax.cy;
	}
	else
	{
		sizeMax.cy = 1;
	}
	SetScrollSizes(MM_TEXT, sizeMax);
}


// CInfoView message handlers
void CInfoView::SetStrings(CStringList &lstString)
{
	m_lstString.RemoveAll();
	POSITION pos = lstString.GetHeadPosition();
	while(pos)
	{
		m_lstString.AddTail(lstString.GetNext(pos));
	}
	Invalidate();
}
void CInfoView::SetTextFont(LOGFONT &logFont)
{
	m_logFont = logFont;
	Invalidate();
}
void CInfoView::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
	Invalidate();
}
void CInfoView::SetBKColor(COLORREF clrBK)
{
	m_clrBK = clrBK;
	Invalidate();
}
