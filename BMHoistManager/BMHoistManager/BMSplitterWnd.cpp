// BMSplitterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BMHoistManager.h"
#include "BMSplitterWnd.h"


// CBMSplitterWnd

IMPLEMENT_DYNAMIC(CBMSplitterWnd, CSplitterWnd)

CBMSplitterWnd::CBMSplitterWnd()
{

}

CBMSplitterWnd::~CBMSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CBMSplitterWnd, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CBMSplitterWnd message handlers



void CBMSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CSplitterWnd::OnLButtonDown(nFlags, point);
}

void CBMSplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CSplitterWnd::OnMouseMove(nFlags, point);
}
