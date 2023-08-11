#pragma once


// CBMSplitterWnd

class CBMSplitterWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CBMSplitterWnd)

public:
	CBMSplitterWnd();
	virtual ~CBMSplitterWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


