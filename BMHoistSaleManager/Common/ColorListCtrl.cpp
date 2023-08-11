//-----------------------------------------------
//	File Name:ColorListCtrl.cpp
//	Introduce:implementation file
//	Date:
//------------------------------------------------
#include "stdafx.h"
#include "ColorListCtrl.h"
//------------------------------------------------
// CColorListCtrl
//------------------------------------------------
IMPLEMENT_DYNAMIC(CColorListCtrl, CListCtrl)
CColorListCtrl::CColorListCtrl()
{
	m_clrText = RGB(0, 0, 0);
	m_clrItem1 = RGB(255, 255, 255);
	m_clrItem2 = RGB(255, 255, 255);
}
//------------------------------------------------
CColorListCtrl::~CColorListCtrl()
{
}
//------------------------------------------------
BEGIN_MESSAGE_MAP(CColorListCtrl, CListCtrl)
END_MESSAGE_MAP()
//------------------------------------------------



//------------------------------------------------
// CColorListCtrl message handlers
//------------------------------------------------
void CColorListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	int iColumnCnt = GetHeaderCtrl()->GetItemCount();
	for (int iColumn = 0; iColumn < iColumnCnt; iColumn++)
	{
		CRect rcItem;
		CString strItem;
		DWORD dwTextAlige = DT_LEFT;
		//
		GetSubItemRect(lpDrawItemStruct->itemID, iColumn, LVIR_LABEL, rcItem);
		strItem = GetItemText(lpDrawItemStruct->itemID, iColumn);
		//
		LV_COLUMN column;
		memset(&column, 0, sizeof(LV_COLUMN));
		column.mask = LVCF_FMT|LVCF_WIDTH;
		if (GetColumn(iColumn, &column))
		{
			if (column.fmt == LVCFMT_RIGHT)
				dwTextAlige = DT_RIGHT;
			else if (column.fmt == LVCFMT_CENTER)
				dwTextAlige = DT_CENTER;
			else 
				dwTextAlige = DT_LEFT;
		}

		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			CBrush brBK;
			brBK.CreateSolidBrush(/*GetSysColor(COLOR_HIGHLIGHT)*/RGB(4, 251, 183));
			FillRect(lpDrawItemStruct->hDC, &rcItem, brBK);
		}
		else
		{
			CBrush brBK;
			COLORREF clrBK = (lpDrawItemStruct->itemID%2 == 0) ? m_clrItem1 : m_clrItem2;//m_mapItemColor[lpDrawItemStruct->itemID];
			brBK.CreateSolidBrush(clrBK);
			FillRect(lpDrawItemStruct->hDC, &rcItem, brBK);
		}

		int iItemData = GetItemData(lpDrawItemStruct->itemID);
		int iSelFlag = LOWORD(iItemData);
		int iTitleFlag = HIWORD(iItemData);
		if (1 == iSelFlag)
		{
			CRect rcCopy = lpDrawItemStruct->rcItem;
			rcCopy.left += 1;
			rcCopy.top += 1;
			rcCopy.right -= 1;
			rcCopy.bottom -= 1;
			HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			HPEN hOldPen = (HPEN)SelectObject(lpDrawItemStruct->hDC, hPen);
			MoveToEx(lpDrawItemStruct->hDC, rcCopy.left, rcCopy.top, NULL);
			LineTo(lpDrawItemStruct->hDC, rcCopy.right, rcCopy.top);
			LineTo(lpDrawItemStruct->hDC, rcCopy.right, rcCopy.bottom);
			LineTo(lpDrawItemStruct->hDC, rcCopy.left, rcCopy.bottom);
			LineTo(lpDrawItemStruct->hDC, rcCopy.left, rcCopy.top);
			SelectObject(lpDrawItemStruct->hDC, hOldPen);
			DeleteObject(hPen);
		}		

		HFONT hOldFont;
		TEXTMETRIC metric;
		CRect rcText = rcItem;
		GetTextMetrics(lpDrawItemStruct->hDC, &metric);		
		rcText.left += metric.tmAveCharWidth;
		rcText.right -= metric.tmAveCharWidth;
		::SetTextColor(lpDrawItemStruct->hDC, m_clrText);
		if (1 == iTitleFlag)
		{
			LOGFONT lf;
			CFont* pFont = GetFont();
			pFont->GetLogFont(&lf);
			lf.lfWeight = FW_BOLD;
			HFONT hBoldFont = CreateFontIndirect(&lf);
			hOldFont = (HFONT)SelectObject(lpDrawItemStruct->hDC, hBoldFont);
		}
		::DrawText(lpDrawItemStruct->hDC, strItem, strItem.GetLength(), &rcText, dwTextAlige | DT_VCENTER | DT_SINGLELINE);
		
		if (1 == iTitleFlag)
		{
			SelectObject(lpDrawItemStruct->hDC, hOldFont);
		}
	}
}
//------------------------------------------------



//------------------------------------------------
void CColorListCtrl::SetFullItemBKColor(int iItem, COLORREF clrBK)
{
	m_mapItemColor[iItem] = clrBK;
}
//------------------------------------------------
void CColorListCtrl::SetFullItemBKColor(COLORREF clrBK1, COLORREF clrBK2)
{
	m_clrItem1 = clrBK1;
	m_clrItem2 = clrBK2;
}
//------------------------------------------------
void CColorListCtrl::SetItemTextColor(COLORREF clrText)
{
	m_clrText = clrText;
}
//------------------------------------------------
void CColorListCtrl::SetItemIsTitle(int iItem)
{
	LONG lItemData = MAKELONG(0, 1);
	SetItemData(iItem, lItemData);
}
//------------------------------------------------
