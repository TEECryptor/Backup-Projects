//-----------------------------------------------------------------------
//	File Name:OptionTabCtrl.cpp
//	Introduce:Class COptionTabCtrl implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "OptionTabCtrl.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
//	Name:COptionTabCtrl
//	Introduce:Customize the CTabCtrl to change the  color of tab and background
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(COptionTabCtrl, CTabCtrl)
COptionTabCtrl::COptionTabCtrl()
{
}
//-----------------------------------------------------------------------
COptionTabCtrl::~COptionTabCtrl()
{
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(COptionTabCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()
//-----------------------------------------------------------------------
void COptionTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	TC_ITEM tci;  
	WCHAR szTabText[100]; 
	memset(szTabText, '\0', sizeof(szTabText));     

	tci.mask		= TCIF_TEXT;     
	tci.pszText		= szTabText;     
	tci.cchTextMax	= sizeof(szTabText)-1;  

	GetItem(lpDrawItemStruct->itemID, &tci); 
	FillRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, m_bgBrush);  
	::SetBkColor(lpDrawItemStruct->hDC, m_bgColor); 

	SelectObject(lpDrawItemStruct->hDC, *m_TabFont);
	TextOut(lpDrawItemStruct->hDC,     
		lpDrawItemStruct->rcItem.left + 3,     
		lpDrawItemStruct->rcItem.top + 3,     
		tci.pszText,     
		lstrlen(tci.pszText));  
}
//-----------------------------------------------------------------------
BOOL COptionTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	pDC->SetBkColor(m_bgColor);    

	return TRUE;
}
//-----------------------------------------------------------------------
void COptionTabCtrl::SetBkColor(COLORREF color)
{
	m_bgColor = color;

	m_bgBrush.Detach();
	m_bgBrush.CreateSolidBrush(m_bgColor);
	Invalidate(true);
}
//-----------------------------------------------------------------------
void COptionTabCtrl::SetFont(CFont* font)
{
	m_TabFont = font;
	CTabCtrl::SetFont(m_TabFont);
}
//-----------------------------------------------------------------------
void COptionTabCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CTabCtrl::OnSetFocus(pOldWnd);
}
//-----------------------------------------------------------------------