//--------------------------------------------
//	FileName:GDIPlusImage.h
//	Introduce:Define a image used GDI+
//	Creator:Youfang Yao
//	Date:04/05/07
//--------------------------------------------
#ifndef _GDIPLUSIMAGE_H_
#define _GDIPLUSIMAGE_H_
//--------------------------------------------
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif
//--------------------------------------------
#include "ImageEx.h"
#include "Gdiplus.h"
using namespace Gdiplus;
//--------------------------------------------
#include <vector>
#include <fstream>
using namespace std;
//--------------------------------------------
class CGDIPlusImage
{
public:
	CGDIPlusImage();
	~CGDIPlusImage();
	//----------------------------------------
public:
	virtual BOOL LoadFromFile(const char* lpszFile) = 0;
	virtual void Draw(HWND hwnd, HDC hdc, RECT rc) = 0;
	virtual void Draw(HWND hwnd, HDC hdc, RECT &rcDes, RECT &rcSrc) = 0;
	virtual void SetVisible(BOOL bVisible = TRUE) = 0;
	virtual	BOOL IsTransparent(int iPosX, int iPosY, float fTranshAphla) = 0;
	//----------------------------------------
};
//--------------------------------------------


//--------------------------------------------
class CStillImage : public CGDIPlusImage
{
public:
	CStillImage();
	~CStillImage();
	//----------------------------------------
public:
	BOOL	LoadFromFile(const char* lpszFile);
	void	Draw(HWND hwnd, HDC hdc, RECT rc);
	void	Draw(HWND hwnd, HDC hdc, RECT &rcDes, RECT &rcSrc);
	void	SetVisible(BOOL bVisible = TRUE);
	BOOL	IsTransparent(int iPosX, int iPosY, float fTranshAphla);
	//----------------------------------------
private:
	Bitmap*	m_pImage;
	BOOL	m_bVisible;
	//----------------------------------------
};
//--------------------------------------------


//--------------------------------------------
class CGIFImage : public CGDIPlusImage
{
public:
	CGIFImage();
	~CGIFImage();
	//----------------------------------------
public:
	BOOL	LoadFromFile(const char* lpszFile);
	void	Draw(HWND hwnd, HDC hdc, RECT rc);
	void	Draw(HWND hwnd, HDC hdc, RECT &rcDes, RECT &rcSrc);
	void	SetVisible(BOOL bVisible = TRUE);
	BOOL	IsTransparent(int iPosX, int iPosY, float fTranshAphla);
	//----------------------------------------
private:
	ImageEx*	m_pImage;
	BOOL		m_bVisible;
	//
	vector<Bitmap*>	m_lstImages;
	//----------------------------------------
};
//--------------------------------------------


//--------------------------------------------
class CAnimateImage : public CGDIPlusImage
{
public:
	CAnimateImage();
	~CAnimateImage();
	//----------------------------------------
public:
	BOOL	LoadFromFile(const char* lpszFile);
	void	Draw(HWND hwnd, HDC hdc, RECT rc);
	void	Draw(HWND hwnd, HDC hdc, RECT &rcDes, RECT &rcSrc);
	void	SetVisible(BOOL bVisible = TRUE);
	BOOL	IsTransparent(int iPosX, int iPosY, float fTranshAphla);
	//----------------------------------------
private:
	int				m_iCurFrameID;
	vector<Bitmap*>	m_lstImages;
	vector<long>	m_lstImgLen;
	BOOL			m_bVisible;
	//----------------------------------------
};
//--------------------------------------------
#endif	//_GDIPLUSIMAGE_H_
//--------------------------------------------