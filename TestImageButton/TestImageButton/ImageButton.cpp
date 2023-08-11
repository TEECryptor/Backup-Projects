//-------------------------------------------------------
//	Name:ImageButton.cpp
//	Introduce:The image button class define
//	Creator:Youfang Yao
//	Date:12/12/05
//	Version:0.1v
//-------------------------------------------------------
#include "stdafx.h"
#include "ImageButton.h"
//-------------------------------------------------------
//	Constructor
//-------------------------------------------------------
CImageButton::CImageButton()
:m_pParent(NULL),m_eCurState(BS_OUT)
{
	memset(&m_stuRect, 0, sizeof(RECT));
	memset(&m_stuParam, 0, sizeof(ButtonParam));
	memset(m_strImageFile, 0, BS_STATECNT*MAX_PATH);
	for(int i = 0;i < BS_STATECNT;i++) m_hImage[i] = NULL;
}
//-------------------------------------------------------
//	Constructor, parent:The parent window handle
//				 rc:The button rect
//-------------------------------------------------------
CImageButton::CImageButton(HWND parent, RECT &rc, ButtonParam &param)
:m_pParent(parent),m_eCurState(BS_OUT), m_stuRect(rc), m_stuParam(param)
{
	memset(m_strImageFile, 0, BS_STATECNT*MAX_PATH);
	for(int i = 0;i < BS_STATECNT;i++) m_hImage[i] = NULL;
}
//-------------------------------------------------------
//	Distructor
//-------------------------------------------------------
CImageButton::~CImageButton()
{
	for(int i = 0;i < BS_STATECNT;i++)
		DeleteObject(m_hImage[i]);
}
//-------------------------------------------------------
//	Name:Create
//	Introduce:To create the button
//	Param:hInstance:i:The instance handle
//		  image_out:i:The image file name when mouse out of the button
//	      image_in:i:The image file name when mouse in the button
//	Return:If create successfuly, return true
//-------------------------------------------------------
bool CImageButton::Create(HINSTANCE hInstance, char* image_out, char* image_in)
{
	m_hImage[BS_OUT] = ::LoadBitmap(hInstance, image_out);
	m_hImage[BS_IN] = ::LoadBitmap(hInstance, image_in);
	
	if(m_hImage[BS_OUT] == NULL || m_hImage[BS_IN] == NULL) return false;
	return true;
}
//-------------------------------------------------------
//	Name:PntInButton
//	Introduce:To cheek a point in the button rect or not
//	Param:pt:i:The point in parent coord system
//	Return:If the point in the button rect, return true
//-------------------------------------------------------
bool CImageButton::PntInButton(POINT pt)
{	
	if(PtInRect(&m_stuRect, pt) != 0)
		return true;
	else 
		return false;
}
//-------------------------------------------------------
//	Name:SetButtonState
//	Introduc:To set the button state
//	Param:state:i:The button new state
//	Return:N/A
//-------------------------------------------------------
void CImageButton::SetButtonState(ButtonState state)
{
	m_eCurState = state;
}
//-------------------------------------------------------
//	Name:GetButtonState
//	Introduce:To get the button state
//	Param:N/A
//	Return:N/A
//-------------------------------------------------------
ButtonState CImageButton::GetButtonState()
{
	return m_eCurState;
}
//-------------------------------------------------------
//	Name:GetButtonRect
//	Introduce:To get the button rect
//	Param:N/A
//	Return:The button current rect
//-------------------------------------------------------
RECT CImageButton::GetButtonRect()
{
	return m_stuRect;
}
//-------------------------------------------------------
//	Name:GetButtonParam
//	Introduce:To get the button click param
//	Param:param:o:The button click param
//	Return:N/A
//-------------------------------------------------------
void CImageButton::GetButtonParam(ButtonParam &param)
{
	memcpy(&param, &m_stuParam, sizeof(ButtonParam));
}
//-------------------------------------------------------
//	Name:DrawButton
//	Introduce:To draw the button image on a DC
//	Param:hdc:i:The DC handle button will draw on it
//	Return:N/A
//-------------------------------------------------------
void CImageButton::DrawButton(HDC hdc)
{
	if(hdc == NULL) return;

	BITMAP bit;
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hOldBMP = (HBITMAP)SelectObject(hMemDC, m_hImage[m_eCurState]);	
	GetObject(m_hImage[m_eCurState], sizeof(BITMAP), &bit);

	StretchBlt(hdc, m_stuRect.left, m_stuRect.top, m_stuRect.right-m_stuRect.left, m_stuRect.bottom-m_stuRect.top,
		hMemDC, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);
	
	SelectObject(hMemDC, hOldBMP);
	DeleteDC(hMemDC);
}
//-------------------------------------------------------