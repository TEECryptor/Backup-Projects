//--------------------------------------------
//	FileName:ImageButton.h
//	Introduce:The implementation file for CImageButton class
//	Creator:Youfang Yao
//	Date:06/26/06
//--------------------------------------------
#include "stdafx.h"
#include "ImageButton.h"
//#include "ComDefine.h"
#include ".\imagebutton.h"
//--------------------------------------------
//	Name:CImageButton
//	Introduce:Construction
//--------------------------------------------
CImageButton::CImageButton()
 : m_nResourceID(0)
 , m_pParentWnd(NULL)
 , m_bVisible(TRUE)
 , m_bIsSelected(FALSE)
 , m_eCurStatus(eNormal)
 , m_bTracking(FALSE)
 //, m_eCurLangType(LT_CHN)
{
	memset(m_szCaption, 0, MAX_PATH);
	memset(&m_stuBtnRect, 0, sizeof(RECT));		
	for(int iCnt = 0;iCnt < eStatusCnt;iCnt++)
	{
		m_hBtnImage[iCnt] = NULL;	
		m_pBtnImage[iCnt] = NULL;
	}
}
//--------------------------------------------
//	Name:~CImageButton
//	Introduce:Distruction
//--------------------------------------------
CImageButton::~CImageButton()
{	
	for(int iCnt = 0;iCnt < eStatusCnt;iCnt++)
	{
		if(m_hBtnImage[iCnt])
		{
			DeleteObject(m_hBtnImage[iCnt]);
			m_hBtnImage[iCnt] = NULL;			
		}
		if(m_pBtnImage[iCnt])
		{
			delete m_pBtnImage[iCnt];
			m_pBtnImage[iCnt] = NULL;
		}
	}
}
//--------------------------------------------
BEGIN_MESSAGE_MAP(CImageButton, CWnd)
	//{{AFX_MSG_MAP(CImageButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//--------------------------------------------

//Overload the Create function of CButton
BOOL CImageButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const CRect& rect, CWnd *pParentWnd, UINT nID, 
			LPCTSTR lpNormal, LPCTSTR lpSelect, LPCTSTR lpDown, LPCTSTR lpDisable, HCURSOR hOverHandle)
{
	ASSERT(pParentWnd);

	_tcscpy(m_szCaption, lpszCaption);
	m_stuBtnRect = rect;
	m_pParentWnd = pParentWnd;
	m_nResourceID = nID;

	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	m_pBtnImage[eNormal] = new CGDIPlusImage();
	m_pBtnImage[eNormal]->LoadFromFile(lpNormal);
	//
	m_pBtnImage[eSelect] = new CGDIPlusImage();
	lpSelect = ((NULL == lpSelect || _tcslen(lpSelect) == 0) ? lpNormal : lpSelect);
	m_pBtnImage[eNormal]->LoadFromFile(lpSelect);
	//
	m_pBtnImage[eDown] = new CGDIPlusImage();
	lpDown = ((NULL == lpDown || _tcslen(lpDown) == 0) ? lpSelect : lpDown);
	m_pBtnImage[eNormal]->LoadFromFile(lpDown);
	//
	m_pBtnImage[eDisable] = new CGDIPlusImage();
	lpDisable = ((NULL == lpDisable || _tcslen(lpDisable) == 0) ? lpDown : lpDisable);
	m_pBtnImage[eNormal]->LoadFromFile(lpDisable);

	m_hOverHandle = hOverHandle != NULL ? hOverHandle : LoadCursor(NULL, IDC_ARROW);

	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID);
}
//--------------------------------------------
//Overload the Create function of CButton
//--------------------------------------------
BOOL CImageButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const CRect& rect, CWnd *pParentWnd, UINT nID, 
			HBITMAP hNormal, HBITMAP hSelect, HBITMAP hDown, HBITMAP hDisable, HCURSOR hOverHandle)
{
	ASSERT(pParentWnd);

	_tcscpy(m_szCaption, lpszCaption);
	m_stuBtnRect = rect;
	m_pParentWnd = pParentWnd;
	m_nResourceID = nID;

	m_hBtnImage[eNormal] = hNormal;
	m_hBtnImage[eSelect] = hSelect != NULL ? hSelect : hNormal;
	m_hBtnImage[eDown] = hDown != NULL ? hDown : hNormal;
	m_hBtnImage[eDisable] = hDisable != NULL ? hDisable : hNormal;
	
	m_hOverHandle = hOverHandle != NULL ? hOverHandle : LoadCursor(NULL, IDC_ARROW);

	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID);
}	
//--------------------------------------------	
//Overload the create function of CButton
//--------------------------------------------	
BOOL CImageButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const CRect& rect, CWnd *pParentWnd, UINT nID, 
			UINT nNormalImage, UINT nSelectImage, UINT nDownImage, UINT nDisableImage, HCURSOR hOverHandle)
{
	ASSERT(pParentWnd);

	_tcscpy(m_szCaption, lpszCaption);
	m_stuBtnRect = rect;
	m_pParentWnd = pParentWnd;
	m_nResourceID = nID;

	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	m_pBtnImage[eNormal] = new CGDIPlusImage();
	m_pBtnImage[eNormal]->LoadFromResource(hInstance, _T("PNG"), nNormalImage);
	//
	m_pBtnImage[eSelect] = new CGDIPlusImage();
	nSelectImage = (nSelectImage == 0 ? nNormalImage : nSelectImage);
	m_pBtnImage[eSelect]->LoadFromResource(hInstance, _T("PNG"), nSelectImage);
	//
	m_pBtnImage[eDown] = new CGDIPlusImage();
	nDownImage = (nDownImage == 0 ? nSelectImage : nDownImage);
	m_pBtnImage[eDown]->LoadFromResource(hInstance, _T("PNG"), nDownImage);
	//
	m_pBtnImage[eDisable] = new CGDIPlusImage();
	nDisableImage = (nDisableImage == 0 ? nDownImage : nDisableImage);
	m_pBtnImage[eDisable]->LoadFromResource(hInstance, _T("PNG"), nSelectImage);

	m_hOverHandle = hOverHandle != NULL ? hOverHandle : LoadCursor(NULL, IDC_ARROW);

	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID);
}
//--------------------------------------------		
//	Name:SetImages
//	Introduce:Set the new images
//--------------------------------------------		
void CImageButton::SetImages(LPCTSTR lpNormal, LPCTSTR lpSelect, LPCTSTR lpDown, LPCTSTR lpDisable)
{
	for(int iIndex = 0; iIndex < eStatusCnt; iIndex++)
		SAF_DELETE(m_pBtnImage[iIndex]);

	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	m_pBtnImage[eNormal] = new CGDIPlusImage();
	m_pBtnImage[eNormal]->LoadFromFile(lpNormal);
	//
	m_pBtnImage[eSelect] = new CGDIPlusImage();
	lpSelect = ((NULL == lpSelect || _tcslen(lpSelect) == 0) ? lpNormal : lpSelect);
	m_pBtnImage[eNormal]->LoadFromFile(lpSelect);
	//
	m_pBtnImage[eDown] = new CGDIPlusImage();
	lpDown = ((NULL == lpDown || _tcslen(lpDown) == 0) ? lpSelect : lpDown);
	m_pBtnImage[eNormal]->LoadFromFile(lpDown);
	//
	m_pBtnImage[eDisable] = new CGDIPlusImage();
	lpDisable = ((NULL == lpDisable || _tcslen(lpDisable) == 0) ? lpDown : lpDisable);
	m_pBtnImage[eNormal]->LoadFromFile(lpDisable);

	Invalidate(FALSE);
	if(m_pParentWnd) m_pParentWnd->InvalidateRect(&m_stuBtnRect, FALSE);
}
//--------------------------------------------		
//	Name:SetImages
//	Introduce:Set the new images
//--------------------------------------------	
void CImageButton::SetImages(HBITMAP hNormal, HBITMAP hSelect, HBITMAP hDown, HBITMAP hDisable)
{
	//Delete the old image objects
	for(int iCnt = 0;iCnt < eStatusCnt;iCnt++)
	{
		if(m_hBtnImage[iCnt])
		{
			DeleteObject(m_hBtnImage[iCnt]);
			m_hBtnImage[iCnt] = NULL;
		}
	}

	//Set the new image objects	
	m_hBtnImage[eNormal] = hNormal;
	m_hBtnImage[eSelect] = hSelect != NULL ? hSelect : hNormal;
	m_hBtnImage[eDown] = hDown != NULL ? hDown : hNormal;
	m_hBtnImage[eDisable] = hDisable != NULL ? hDisable : hNormal;
}
//--------------------------------------------	
//	Name:SetImages
//	Introduce:Set the new images
//--------------------------------------------	
void CImageButton::SetImages(UINT nNormalImage, UINT nSelectImage, UINT nDownImage, UINT nDisableImage)
{
	for(int iIndex = 0; iIndex < eStatusCnt; iIndex++)
		SAF_DELETE(m_pBtnImage[iIndex]);

	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	m_pBtnImage[eNormal] = new CGDIPlusImage();
	m_pBtnImage[eNormal]->LoadFromResource(hInstance, _T("PNG"), nNormalImage);
	//
	m_pBtnImage[eSelect] = new CGDIPlusImage();
	nSelectImage = (nSelectImage == 0 ? nNormalImage : nSelectImage);
	m_pBtnImage[eSelect]->LoadFromResource(hInstance, _T("PNG"), nSelectImage);
	//
	m_pBtnImage[eDown] = new CGDIPlusImage();
	nDownImage = (nDownImage == 0 ? nSelectImage : nDownImage);
	m_pBtnImage[eDown]->LoadFromResource(hInstance, _T("PNG"), nDownImage);
	//
	m_pBtnImage[eDisable] = new CGDIPlusImage();
	nDisableImage = (nDisableImage == 0 ? nDownImage : nDisableImage);
	m_pBtnImage[eDisable]->LoadFromResource(hInstance, _T("PNG"), nSelectImage);

	Invalidate(FALSE);
	if(m_pParentWnd) m_pParentWnd->InvalidateRect(&m_stuBtnRect, FALSE);
}
//--------------------------------------------	
//	Name:SetBtnSelected
//	Introduce:Set this button is selected or not
//--------------------------------------------	
void CImageButton::SetBtnSelected(BOOL bSelected)
{
	m_bIsSelected = bSelected;
	Invalidate(FALSE);
}
//--------------------------------------------	
//Set the button visible attribute
//--------------------------------------------	
void CImageButton::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	ShowWindow((bVisible == TRUE) ? SW_SHOW : SW_HIDE);
}
//--------------------------------------------
//Get the button visible attribute	
//--------------------------------------------
BOOL CImageButton::GetVisible()
{
	return m_bVisible;
}
//--------------------------------------------
//Set the button enable attribute
//--------------------------------------------
void CImageButton::SetEnable(BOOL bEnable)
{
	m_eCurStatus = (bEnable == TRUE) ? eNormal : eDisable;
}
//--------------------------------------------
//Get the button enable attribute
//--------------------------------------------
BOOL CImageButton::GetEnable()
{
	return (m_eCurStatus == eDisable) ? FALSE : TRUE;
}
//--------------------------------------------
//Set the button tip string
//--------------------------------------------
//void CImageButton::SetTipString(eLangType eLang, LPCTSTR lpszTip)
//{
//	/*
//	if(lpszTip)
//	{
//		if((m_TipWnd.GetSafeHwnd() == NULL) && (m_pParentWnd != NULL))
//		{
//			POINT pnt;
//			pnt.x = m_stuBtnRect.right;
//			pnt.y = m_stuBtnRect.top;
//			::ClientToScreen(m_pParentWnd->m_hWnd, &pnt);
//
//			RECT tipRect;
//			tipRect.left = pnt.x;
//			tipRect.top = pnt.y;
//			tipRect.right = tipRect.left + TIPWND_WIDTH;
//			tipRect.bottom = tipRect.top + 10;
//			m_TipWnd.Create(tipRect);  
//		}
//		m_TipWnd.SetTipString(eLang, lpszTip);
//	}
//	else
//	{
//		m_TipWnd.SetTipString(eLang, "");
//	}
//*/
//}
////--------------------------------------------
////Get the button tip string
////--------------------------------------------
//void CImageButton::GetTipString(eLangType eLang, LPTSTR lpszTip)
//{
//	/*
//	if(lpszTip)
//	{
//		CString strTip = m_TipWnd.GetTipString(eLang);
//		_tcscpy(lpszTip, strTip);
//	}
//	*/
//}
//--------------------------------------------
//Set the button rect on parent window
//--------------------------------------------
void CImageButton::SetBtnRect(RECT &rc)
{
	m_stuBtnRect = rc;
	if(m_hWnd) MoveWindow(&rc);
}
//--------------------------------------------
//Get the button rect on parent window
//--------------------------------------------
void CImageButton::GetBtnRect(RECT &rc)
{
	rc = m_stuBtnRect;
}
//--------------------------------------------
//Set the language version
//--------------------------------------------
//void CImageButton::SetLangVersion(eLangType eLang)
//{
//	m_eCurLangType = eLang;
//	//m_TipWnd.SetLangVersion(m_eLangVersion);
//}
//--------------------------------------------


//--------------------------------------------
//	WM_PAINT
//--------------------------------------------
void CImageButton::OnPaint() 
{
	CPaintDC dc(this); 

	CRect rcBtn;
	GetClientRect(&rcBtn);

	int iImageIndex = m_bIsSelected ? eDown : m_eCurStatus;

	if(m_pBtnImage[eNormal] == NULL)
	{
		HDC hMemDc = CreateCompatibleDC(dc.m_hDC);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDc, m_hBtnImage[iImageIndex]);

		BITMAP bm;
		bm.bmBits = NULL;
		GetObject(m_hBtnImage[iImageIndex], sizeof(BITMAP), &bm);

		StretchBlt(dc.m_hDC, 0, 0, m_stuBtnRect.Width(), m_stuBtnRect.Height(),
					hMemDc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

		SelectObject(hMemDc, hOldBmp);
		DeleteDC(hMemDc);
	}
	else
	{
		m_pBtnImage[iImageIndex]->Draw(dc.m_hDC, rcBtn, CRect(0, 0, m_pBtnImage[iImageIndex]->GetWidth(), m_pBtnImage[iImageIndex]->GetHeight()));
	}

}
//--------------------------------------------
//	WM_MOUSEMOVE
//--------------------------------------------
void CImageButton::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
/*
		POINT pnt;
		pnt.x = m_stuBtnRect.right;
		pnt.y = m_stuBtnRect.top;
		::ClientToScreen(m_pParentWnd->m_hWnd, &pnt);
		m_TipWnd.SetVisible(TRUE, pnt);
*/
	}

	//Change the mouse ico image
	m_hOldHandle = SetCursor(m_hOverHandle);

	//The left button is downed, set the status is eDown
	if(nFlags == MK_LBUTTON)
	{
		m_eCurStatus = eDown;
		Invalidate(FALSE);
	}
	//The left button is not downed
	else
	{
		if(m_eCurStatus != eSelect)
		{
			m_eCurStatus = eSelect;
			Invalidate(FALSE);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}
//--------------------------------------------
//	WM_MOUSELEAVE
//--------------------------------------------
LRESULT CImageButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	m_eCurStatus = eNormal;
	Invalidate(FALSE);
/*
	POINT pnt;
	pnt.x = m_stuBtnRect.right;
	pnt.y = m_stuBtnRect.top;
	::ClientToScreen(m_pParentWnd->m_hWnd, &pnt);
	m_TipWnd.SetVisible(FALSE, pnt);
*/
	SetCursor(m_hOldHandle);

	return 0;
}
//--------------------------------------------
//	WM_LBUTTONDOWN
//--------------------------------------------
void CImageButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(FALSE == m_bIsSelected)
	{
		m_eCurStatus = eDown;
		Invalidate(FALSE);
	}

	CWnd::OnLButtonDown(nFlags, point);
}
//--------------------------------------------
//	WM_LBUTTONUP
//--------------------------------------------
void CImageButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(FALSE == m_bIsSelected)
	{
		m_eCurStatus = eSelect;
		Invalidate(FALSE);

		::PostMessage(m_pParentWnd->m_hWnd, WM_COMMAND, m_nResourceID, 0);
	}

	CWnd::OnLButtonUp(nFlags, point);
}
//--------------------------------------------