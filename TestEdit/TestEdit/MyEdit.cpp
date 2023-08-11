// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "TestEdit.h"
#include "MyEdit.h"
#include ".\myedit.h"


// CMyEdit

IMPLEMENT_DYNAMIC(CMyEdit, CEdit)
CMyEdit::CMyEdit()
{
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
//	ON_WM_CTLCOLOR()
//	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_VSCROLL()
	ON_CONTROL_REFLECT(EN_VSCROLL, OnEnVscroll)
	ON_CONTROL_REFLECT(EN_HSCROLL, OnEnHscroll)
	ON_WM_HSCROLL()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CMyEdit message handlers

void CMyEdit::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(&rcClient);
	//rcClient.left += 1;	rcClient.right -= 1;
	//rcClient.top += 1; rcClient.bottom -= 1;

	HDC hDCMem = CreateCompatibleDC(dc.m_hDC);
	int iOldMemBkMode = SetBkMode(hDCMem, TRANSPARENT);	
	HBITMAP hMemBmp = CreateCompatibleBitmap(dc.m_hDC, rcClient.Width(), rcClient.Height());
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDCMem, hMemBmp);	
	CBrush brush(RGB(0, 255, 255));
	FillRect(hDCMem, &rcClient, (HBRUSH)brush);
	
	DrawString(hDCMem);

	//Rectangle(hDCMem, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

	BitBlt(	dc.m_hDC, 
			0, 0, rcClient.Width(), rcClient.Height(),
			hDCMem,
			0, 0,
			SRCCOPY);
			
	SelectObject(dc.m_hDC, hOldBitmap);
	SetBkMode(hDCMem, iOldMemBkMode);
	DeleteObject(hMemBmp);
	DeleteDC(hDCMem);
}

void CMyEdit::DrawString(HDC hdc)
{
	TRACE(_T("DrawString\n"));

	int iCurTop = 10-GetScrollPos(SB_VERT);

	DWORD dwLineCnt = GetLineCount();
	for(DWORD dwIndex = 0; dwIndex < dwLineCnt; dwIndex++)
	{
		TCHAR szLine[512] = {_T("")};
		GetLine(dwIndex, szLine, 512);
		CString strLine = szLine;

		CRect rcText(0, 0, 0, 0);
		DrawText(hdc, strLine, strLine.GetLength(), &rcText, DT_CALCRECT|DT_LEFT|DT_TOP);

		CRect rcDraw(CPoint(10, iCurTop), CPoint(10+rcText.Width(), iCurTop+rcText.Height()));
		DrawText(hdc, strLine, strLine.GetLength(), &rcDraw, DT_LEFT|DT_TOP);

		iCurTop += (10+rcText.Height());
	}
	
	SetScrollRange(SB_VERT, 0, iCurTop);
	/*
	CString strText;
	GetWindowText(strText);

	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.left += 1;	rcClient.right -= 1;
	rcClient.top += 1; rcClient.bottom -= 1;

	DrawText(hdc, strText, strText.GetLength(), &rcClient, DT_LEFT | DT_TOP);
	*/
}

void CMyEdit::OnEnChange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
/*
	SCROLLINFO info;
	GetScrollInfo(SB_VERT, &info);

	int iPos = GetScrollPos(SB_VERT);
*/

	//Invalidate(TRUE);
	//UpdateWindow();
	//SendMessage(WM_NCPAINT);
	//UpdateWindow();
}

//HBRUSH CMyEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	return hbr;
//}

//BOOL CMyEdit::OnEraseBkgnd(CDC* pDC)
//{
//	CRect rcClient;
//	GetClientRect(&rcClient);
//	//rcClient.left += 1;	rcClient.right -= 1;
//	//rcClient.top += 1; rcClient.bottom -= 1;
//
//	HDC hDCMem = CreateCompatibleDC(pDC->m_hDC);
//	int iOldMemBkMode = SetBkMode(hDCMem, TRANSPARENT);	
//	HBITMAP hMemBmp = CreateCompatibleBitmap(pDC->m_hDC, rcClient.Width(), rcClient.Height());
//	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDCMem, hMemBmp);	
//	CBrush brush(RGB(0, 255, 255));
//	FillRect(hDCMem, &rcClient, (HBRUSH)brush);
//	
//	//DrawString(hDCMem);
//
//	//Rectangle(hDCMem, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
//
//	BitBlt(	pDC->m_hDC, 
//			0, 0, rcClient.Width(), rcClient.Height(),
//			hDCMem,
//			0, 0,
//			SRCCOPY);
//			
//	SelectObject(pDC->m_hDC, hOldBitmap);
//	SetBkMode(hDCMem, iOldMemBkMode);
//	DeleteObject(hMemBmp);
//	DeleteDC(hDCMem);
//
//
//	//return CEdit::OnEraseBkgnd(pDC);
//	return TRUE;
//}

HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	//pDC->SetBkMode(TRANSPARENT);			///选择透明背景模式
	//pDC->SetTextColor(RGB(0xff,0xff,0xff)); ///设置文字颜色为白色

	return NULL;
}

void CMyEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//Invalidate(TRUE);
	//UpdateWindow();

	CEdit::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMyEdit::OnEnVscroll()
{
	//Invalidate(TRUE);
	//UpdateWindow();
}

void CMyEdit::OnEnHscroll()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_SCROLL flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	//Invalidate(TRUE);
}

void CMyEdit::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//Invalidate(TRUE);

	CEdit::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMyEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
}
