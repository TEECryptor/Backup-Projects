//--------------------------------------------
//	FileName:ImageButton.h
//	Introduce:The CImageButton class definition file
//	Creator:Youfang Yao
//	Date:06/26/06
//--------------------------------------------
#ifndef _IMAGEBUTTON_H_
#define _IMAGEBUTTON_H_
//--------------------------------------------
//#include "ComDefine.h"
#include "GDIPlusImage.h"

#define SAF_DELETE(x){if(x){delete x;x=NULL;}}
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
	BOOL	Create(LPCTSTR lpszCaption, DWORD dwStyle, const CRect& rect, CWnd *pParentWnd, UINT nID, LPCTSTR lpNormal, LPCTSTR lpSelect = NULL, LPCTSTR lpDown = NULL, LPCTSTR lpDisable = NULL, HCURSOR hOverHandle = NULL);
	//Overload the Create function of CButton
	BOOL	Create(LPCTSTR lpszCaption, DWORD dwStyle, const CRect& rect, CWnd *pParentWnd, UINT nID, HBITMAP hNormal, HBITMAP hSelect = NULL, HBITMAP hDown = NULL, HBITMAP hDisable = NULL, HCURSOR hOverHandle = NULL);
	//Overload the create function of CButton
	BOOL	Create(LPCTSTR lpszCaption, DWORD dwStyle, const CRect& rect, CWnd *pParentWnd, UINT nID, UINT nNormalImage, UINT nSelectImage = 0, UINT nDownImage = 0, UINT nDisableImage = 0, HCURSOR hOverHandle = NULL);
	//Set the new images
	void	SetImages(LPCTSTR lpNormal, LPCTSTR lpSelect = NULL, LPCTSTR lpDown = NULL, LPCTSTR lpDisable = NULL);
	//Set the new images
	void	SetImages(HBITMAP hNormal, HBITMAP hSelect = NULL, HBITMAP hDown = NULL, HBITMAP hDisable = NULL);
	//Set the new images
	void	SetImages(UINT nNormalImage, UINT nSelectImage = 0, UINT nDownImage = 0, UINT nDisableImage = 0);
	//Set this button is selected or not
	void	SetBtnSelected(BOOL bSelected = TRUE);
	//Set/Get the button visible attribute
	void	SetVisible(BOOL bVisible);
	BOOL	GetVisible();
	//Set/Get the button enable attribute
	void	SetEnable(BOOL bEnable);
	BOOL	GetEnable();
	//Set/Get the button tip string
//	void	SetTipString(eLangType eLang, LPCTSTR lpszTip);
//	void	GetTipString(eLangType eLang, LPTSTR lpszTip);
	//Set/Get the button rect
	void	SetBtnRect(RECT &rc);
	void	GetBtnRect(RECT &rc);
	//Set the language version
//	void	SetLangVersion(eLangType eLang);
	//----------------------------------------
private:
	TCHAR			m_szCaption[MAX_PATH];	//The caption for the button
	UINT			m_nResourceID;			//The resource ID for the button
	CWnd*			m_pParentWnd;			//The parent wnd for the button
	CRect			m_stuBtnRect;			//The rect for the button
	HBITMAP			m_hBtnImage[eStatusCnt];//The status images for the button
	CGDIPlusImage*	m_pBtnImage[eStatusCnt];//The status images for the button
	//
//	eLangType		m_eCurLangType;
	BOOL			m_bVisible;				//The visible attribute for the button
	eBtnStatus		m_eCurStatus;			//The current status for the button
	BOOL			m_bIsSelected;			//This button is selected or not
	BOOL			m_bTracking;			//The WM_MOUSELEAVE message track flag
	//
	//CTipWnd		m_TipWnd;				//The tip tool control for the button
	HCURSOR			m_hOverHandle;			//The mouse ico handle when mouse over this button
	HCURSOR			m_hOldHandle;			//The old mouse ico handle
};
//--------------------------------------------
#endif //_IMAGEBUTTON_H_
//--------------------------------------------
