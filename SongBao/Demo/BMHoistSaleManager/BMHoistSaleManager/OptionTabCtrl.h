//-----------------------------------------------------------------------
//	File Name:OptionTabCtrl.h
//	Introduce:header file
//-----------------------------------------------------------------------
#ifndef	_OPTION_TABCTRL_H_
#define	_OPTION_TABCTRL_H_
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//	Name:COptionTabCtrl
//	Introduce:Customize the CTabCtrl
//-----------------------------------------------------------------------
class COptionTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(COptionTabCtrl)

public:
	COptionTabCtrl();
	virtual ~COptionTabCtrl();
	//-------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
public:
	//-------------------------------------------------------------------
	void SetBkColor(COLORREF color);
	void SetFont(CFont* font);
	//-------------------------------------------------------------------
private:
	COLORREF			m_bgColor;
	CBrush				m_bgBrush; 
	CFont*				m_TabFont;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
#endif	//_OPTION_TABCTRL_H_
//-----------------------------------------------------------------------