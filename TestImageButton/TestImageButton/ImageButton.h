//-------------------------------------------------------
//	Name:ImageButton.h
//	Introduce:The image button class define
//	Creator:Youfang Yao
//	Date:12/12/05
//	Version:0.1v
//-------------------------------------------------------
#ifndef _IMAGEBUTTON_H_
#define _IMAGEBUTTON_H_
//-------------------------------------------------------
#include<windows.h>
//-------------------------------------------------------
//	Name:ButtonState
//	Introduce:The button state define
//-------------------------------------------------------
enum ButtonState
{
	BS_OUT,			//The mouse out of button
	BS_IN,			//The mouse in button
	BS_STATECNT		//The button state count
};
//-------------------------------------------------------
//	Name:ButtonParam
//	Introduce:The button click param
//-------------------------------------------------------
typedef struct tagButtonParam
{
	UINT	uID;	//The button function ID
	float	fValue;	//The button function param value
}ButtonParam, *LPBUTTONPARAM;
//-------------------------------------------------------
//	Name:CImageButton
//	Introduce:The image button class
//-------------------------------------------------------
class CImageButton
{
public:
	//---------------------------------------------------
	CImageButton();
	CImageButton(HWND parent, RECT &rc, ButtonParam &param);
	~CImageButton();
	//---------------------------------------------------
public:
	//---------------------------------------------------
	//To create the button
	bool	Create(HINSTANCE hInstance, char* image_out, char* image_in);
	//To cheek a point in the button rect or not
	bool	PntInButton(POINT pt);
	//To set the button state
	void	SetButtonState(ButtonState state);
	//To get the button state
	ButtonState GetButtonState();
	//To get the button rect
	RECT	GetButtonRect();
	//To get the button click param
	void	GetButtonParam(ButtonParam &param);
	//To draw the button image on a DC
	void	DrawButton(HDC hdc);
	//---------------------------------------------------
private:
	//---------------------------------------------------
	HWND		m_pParent;		//The parent window handle
	RECT		m_stuRect;		//The button rect
	ButtonParam	m_stuParam;		//The button click param
	ButtonState	m_eCurState;	//The button current state
	char		m_strImageFile[BS_STATECNT][MAX_PATH];	//The image files
	HBITMAP		m_hImage[BS_STATECNT];	//The images handle
	//---------------------------------------------------
};
//-------------------------------------------------------
#endif //_IMAGEBUTTON_H_
//-------------------------------------------------------
