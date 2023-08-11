//-----------------------------------------------------------------------
//	File Name:ColorStatic.cpp
//	Introduce:Class CColorStatic implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "ColorStatic.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CColorStatic
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CColorStatic, CStatic)
CColorStatic::CColorStatic()
{
	m_clrBackground = RGB(239, 235, 222);
	m_clrText = RGB(0, 0, 0);
}
//-----------------------------------------------------------------------
CColorStatic::~CColorStatic()
{
	m_Brush.DeleteObject(); 
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CColorStatic message handlers
//-----------------------------------------------------------------------
void CColorStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	LOGFONT lf;
	CRect rcClient;
	CString strText;
	GetWindowText(strText);
	GetClientRect(&rcClient);
	GetFont()->GetLogFont(&lf);

	UINT nTextAlign = DT_LEFT;
	LONG lTextAlign = GetWindowLong(m_hWnd, GWL_STYLE);
	if (lTextAlign & SS_LEFT)
		nTextAlign = DT_LEFT;
	else if (lTextAlign & SS_RIGHT)
		nTextAlign = DT_RIGHT;
	else if (lTextAlign & SS_CENTER)
		nTextAlign = DT_CENTER;
	nTextAlign |= (DT_VCENTER | DT_SINGLELINE);
	
	int iOldDcBkMode = SetBkMode(dc.m_hDC, TRANSPARENT);

	CBrush brBK;
	brBK.CreateSolidBrush(m_clrBackground);
	FillRect(dc.m_hDC, &rcClient, brBK);
	brBK.DeleteObject(); 
	
	HFONT hFont = CreateFontIndirect(&lf);
	HFONT hOldFont = (HFONT)SelectObject(dc.m_hDC, hFont);
	COLORREF oldColor = SetTextColor(dc.m_hDC, m_clrText);
	DrawText(dc.m_hDC, strText, strText.GetLength(), &rcClient, nTextAlign);
	SetTextColor(dc.m_hDC, oldColor);
	SelectObject(dc.m_hDC, hOldFont);
	DeleteObject(hFont);

	SetBkMode(dc.m_hDC, iOldDcBkMode);
}
//-----------------------------------------------------------------------
void CColorStatic::SetColors(COLORREF clrBK, COLORREF clrText)
{
	m_clrBackground = clrBK;
	m_clrText = clrText;
}
//-----------------------------------------------------------------------
void CColorStatic::SetText(CString strText)
{
	SetWindowText(strText);
	Invalidate();
}
//-----------------------------------------------------------------------
