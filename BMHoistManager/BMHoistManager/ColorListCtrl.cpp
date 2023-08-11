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
			brBK.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
			FillRect(lpDrawItemStruct->hDC, &rcItem, brBK);
		}
		else
		{
			CBrush brBK;
			COLORREF clrBK = m_mapItemColor[lpDrawItemStruct->itemID];
			brBK.CreateSolidBrush(clrBK);
			FillRect(lpDrawItemStruct->hDC, &rcItem, brBK);
		}
		
		TEXTMETRIC metric;
		CRect rcText = rcItem;
		GetTextMetrics(lpDrawItemStruct->hDC, &metric);		
		rcText.left += metric.tmAveCharWidth;
		rcText.right -= metric.tmAveCharWidth;
		::SetTextColor(lpDrawItemStruct->hDC, RGB(255,255,255));
		::DrawText(lpDrawItemStruct->hDC, strItem, strItem.GetLength(), &rcText, dwTextAlige);
	}
}
//------------------------------------------------



//------------------------------------------------
void CColorListCtrl::SetFullItemBKColor(int iItem, COLORREF clrBK)
{
	m_mapItemColor[iItem] = clrBK;
}
//------------------------------------------------
