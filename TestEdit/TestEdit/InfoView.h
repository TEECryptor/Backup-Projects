#pragma once

#define		INFOVIEW_WIDTH 300
#define		INFOVIEW_HEIGHT 200

// CInfoView view

class CInfoView : public CScrollView
{
	DECLARE_DYNCREATE(CInfoView)

public:
//protected:
	CInfoView();           // protected constructor used by dynamic creation
	virtual ~CInfoView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void	SetStrings(CStringList &lstString);
	void	SetTextFont(LOGFONT &logFont);
	void	SetTextColor(COLORREF clrText);
	void	SetBKColor(COLORREF clrBK);
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
private:
	void	DrawStrings(HDC hdc);
private:
	LOGFONT			m_logFont;
	COLORREF		m_clrText;
	COLORREF		m_clrBK;
	CStringList		m_lstString;
	//
	CSize			m_stuViewSize;
};


