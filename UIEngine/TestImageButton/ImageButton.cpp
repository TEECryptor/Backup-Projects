//--------------------------------------------
//	FileName:ImageButton.h
//	Introduce:The implementation file for CImageButton class
//	Creator:Youfang Yao
//	Date:06/26/06
//
//	Fixed for BabyTree Client UI
//	Date:04/07/07
//--------------------------------------------
#include "stdafx.h"
#include "ImageButton.h"
//--------------------------------------------
//	Name:CImageButton
//	Introduce:Construction
//--------------------------------------------
CImageButton::CImageButton()
 : m_nResourceID(0),m_pParentWnd(NULL),m_bVisible(TRUE),
   m_eCurStatus(eNormal),m_bTracking(FALSE)
{
	memset(m_szCaption, 0, MAX_PATH);
	memset(&m_stuBtnRect, 0, sizeof(RECT));		
	for(int iCnt = 0;iCnt < eStatusCnt;iCnt++)
		m_pBtnImage[iCnt] = NULL;	

	m_pParentBkImg = NULL;
}
//--------------------------------------------
//	Name:~CImageButton
//	Introduce:Distruction
//--------------------------------------------
CImageButton::~CImageButton()
{	
	for(int iCnt = 0;iCnt < eStatusCnt;iCnt++)
	{
		if(m_pBtnImage[iCnt])
		{
			delete m_pBtnImage[iCnt];
			m_pBtnImage[iCnt] = NULL;

			for(int i = iCnt+1; i < eStatusCnt; i++)
			{
				if(m_pBtnImage[i])
				m_pBtnImage[i] = NULL;
			}
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

//--------------------------------------------
//Overload the Create function of CButton
//--------------------------------------------
BOOL CImageButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, 
	CWnd *pParentWnd, UINT nID, const char* pszNormal, const char* pszSelect, 
	const char* pszDown, const char* pszDisable, HCURSOR hOverHandle)
{
	ASSERT(pParentWnd);

	strcpy(m_szCaption, lpszCaption);
	m_stuBtnRect = rect;
	m_pParentWnd = pParentWnd;
	m_nResourceID = nID;


	BOOL bRest = FALSE;
	
	//Create eNormal status image
	bRest = LoadBtnImage(pszNormal, &m_pBtnImage[eNormal]);
	if(FALSE == bRest)
		return FALSE;

	//Create eSelect status image
	if(NULL == pszSelect)
		m_pBtnImage[eSelect] = m_pBtnImage[eNormal];
	else
	{
		bRest = LoadBtnImage(pszSelect, &m_pBtnImage[eSelect]);
		if(FALSE == bRest)
			return FALSE;
	}

	//Create eDown status image
	if(NULL == pszDown)
		m_pBtnImage[eDown] = m_pBtnImage[eSelect];
	else
	{
		bRest = LoadBtnImage(pszDown, &m_pBtnImage[eDown]);
		if(FALSE == bRest)
			return FALSE;
	}

	//Create eDisable status image
	if(NULL == pszDisable)
		m_pBtnImage[eDisable] = m_pBtnImage[eDown];
	else
	{
		bRest = LoadBtnImage(pszDisable, &m_pBtnImage[eDisable]);
		if(FALSE == bRest)
			return FALSE;
	}

	m_hOverHandle = hOverHandle != NULL ? hOverHandle : LoadCursor(NULL, IDC_ARROW);

	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID);
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
	if(bEnable)
		ChangeState(eNormal);
	else
		ChangeState(eDisable);
}
//--------------------------------------------
//Get the button enable attribute
//--------------------------------------------
BOOL CImageButton::GetEnable()
{
	return (m_eCurStatus == eDisable) ? FALSE : TRUE;
}
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



//--------------------------------------------
//	Load a image for button status
//--------------------------------------------
BOOL CImageButton::LoadBtnImage(const char* pszFileName, CGDIPlusImage** ppImage)
{
	if(NULL == pszFileName)
		return FALSE;

	if(*ppImage != NULL )
	{
		delete *ppImage;
		*ppImage = NULL;
	}

	char szExt[5] = {'\0'};
	char* pExt = (char*)pszFileName;
	while(*pExt != '\0') pExt++;
	while(*pExt != '.') pExt--;
	strcpy(szExt, pExt);
	strlwr(szExt);

	BOOL bRest = FALSE;
	if(strcmp(szExt, ".gif") == 0)
	{
		*ppImage = new CGIFImage();
	}
	else
	{
		*ppImage = new CStillImage();
	}
	
	if(!(*ppImage)->LoadFromFile(pszFileName))
		return FALSE;

	return TRUE;
}
//--------------------------------------------
//	Change current state to a new one
//--------------------------------------------
void CImageButton::ChangeState(eBtnStatus state)
{
	if(m_eCurStatus == state)
		return;

	if(NULL != m_pBtnImage[m_eCurStatus])
		m_pBtnImage[m_eCurStatus]->SetVisible(FALSE);

	m_eCurStatus = state;
	m_pBtnImage[m_eCurStatus]->SetVisible(TRUE);

	Invalidate(FALSE);
}
//--------------------------------------------
//	WM_PAINT
//--------------------------------------------
void CImageButton::OnPaint() 
{
	CPaintDC dc(this); 

	RECT rcBtn;
	GetClientRect(&rcBtn);

	if(NULL != m_pParentBkImg)
		m_pParentBkImg->Draw(m_hWnd, dc.m_hDC, rcBtn, m_stuBtnRect);
	if(NULL != m_pBtnImage[m_eCurStatus])
		m_pBtnImage[m_eCurStatus]->Draw(m_hWnd, dc.m_hDC, rcBtn);
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

		POINT pnt;
		pnt.x = m_stuBtnRect.right;
		pnt.y = m_stuBtnRect.top;
		::ClientToScreen(m_pParentWnd->m_hWnd, &pnt);
	}

	//Change the mouse ico image
	m_hOldHandle = SetCursor(m_hOverHandle);

	//The left button is downed, set the status is eDown
	if(nFlags == MK_LBUTTON)
	{
		ChangeState(eDown);
	}
	//The left button is not downed
	else
	{
		if(m_eCurStatus != eSelect)
		{
			ChangeState(eSelect);
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
	ChangeState(eNormal);

	POINT pnt;
	pnt.x = m_stuBtnRect.right;
	pnt.y = m_stuBtnRect.top;
	::ClientToScreen(m_pParentWnd->m_hWnd, &pnt);

	SetCursor(m_hOldHandle);

	return 0;
}
//--------------------------------------------
//	WM_LBUTTONDOWN
//--------------------------------------------
void CImageButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_pBtnImage[eNormal]->IsTransparent(point.x, point.y, 200))
		return;

	ChangeState(eDown);

	::PostMessage(m_pParentWnd->m_hWnd, WM_COMMAND, m_nResourceID, 0);

	CWnd::OnLButtonDown(nFlags, point);
}
//--------------------------------------------
//	WM_LBUTTONUP
//--------------------------------------------
void CImageButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ChangeState(eNormal);

	CWnd::OnLButtonUp(nFlags, point);
}
//--------------------------------------------

//--------------------------------------------
void CImageButton::SetParentBkImage(CGDIPlusImage* pParentBkImg)
{
	m_pParentBkImg = pParentBkImg;
}
//--------------------------------------------

