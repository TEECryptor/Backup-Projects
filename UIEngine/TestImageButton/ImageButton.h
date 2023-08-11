//--------------------------------------------
//	FileName:ImageButton.h
//	Introduce:The CImageButton class definition file
//	Creator:Youfang Yao
//	Date:06/26/06
//
//	Fixed for BabyTree Client UI
//	Date:04/07/07
//--------------------------------------------
#ifndef _IMAGEBUTTON_H_
#define _IMAGEBUTTON_H_
//--------------------------------------------
#include "GDIPlusImage.h"
//--------------------------------------------
//	Name:eBtnStatus
//	Introduce:The all status definition for CImageButton
//--------------------------------------------
typedef enum eBtnStatus
{
	eNormal = 0,
	eSelect,
	eDown,
	eDisable,
	eStatusCnt
};
//--------------------------------------------
//	Name:CImageButton
//	Introduce:The CImageButton class, has four status images
//--------------------------------------------
class CImageButton : public CWnd
{
public:
	CImageButton();
	virtual ~CImageButton();
	//----------------------------------------
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageButton)
	public:
	//}}AFX_VIRTUAL
	//----------------------------------------
	// Generated message map functions
protected:
	//{{AFX_MSG(CImageButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	//----------------------------------------
public:
	//Overload the Create function of CButton
	BOOL	Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd *pParentWnd, UINT nID, const char* pszNormal, const char* pszSelect = NULL, const char* pszDown = NULL, const char* pszDisable = NULL, HCURSOR hOverHandle = NULL);
	//Set the new images
	void	SetImages(HBITMAP hNormal, HBITMAP hSelect = NULL, HBITMAP hDown = NULL, HBITMAP hDisable = NULL);
	//Set/Get the button visible attribute
	void	SetVisible(BOOL bVisible);
	BOOL	GetVisible();
	//Set/Get the button enable attribute
	void	SetEnable(BOOL bEnable);
	BOOL	GetEnable();
	//Set/Get the button rect
	void	SetBtnRect(RECT &rc);
	void	GetBtnRect(RECT &rc);
	//
	void	SetParentBkImage(CGDIPlusImage* pParentBkImg);
	//----------------------------------------
private:
	//Load a image for button status
	BOOL	LoadBtnImage(const char* pszFileName, CGDIPlusImage** ppImage);
	//Change current state to a new one
	void	ChangeState(eBtnStatus state);
	//----------------------------------------
private:
	TCHAR			m_szCaption[MAX_PATH];	//The caption for the button
	UINT			m_nResourceID;			//The resource ID for the button
	CWnd*			m_pParentWnd;			//The parent wnd for the button
	RECT			m_stuBtnRect;			//The rect for the button
	CGDIPlusImage*	m_pBtnImage[eStatusCnt];//The status images for the button
	//
	BOOL			m_bVisible;				//The visible attribute for the button
	eBtnStatus		m_eCurStatus;			//The current status for the button
	BOOL			m_bTracking;			//The WM_MOUSELEAVE message track flag
	//
	HCURSOR			m_hOverHandle;			//The mouse ico handle when mouse over this button
	HCURSOR			m_hOldHandle;			//The old mouse ico handle
	//
	CGDIPlusImage*	m_pParentBkImg;
};
//--------------------------------------------
#endif //_IMAGEBUTTON_H_
//--------------------------------------------
