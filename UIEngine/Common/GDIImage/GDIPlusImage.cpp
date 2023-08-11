//--------------------------------------------
//	FileName:GDIPlusImage.cpp
//	Introduce:The cpp file for GDIPlusImage class
//	Creator:Youfang Yao
//	Date:04/05/07
//--------------------------------------------
#include "stdafx.h"
#include "GDIPlusImage.h"
//--------------------------------------------
CGDIPlusImage::CGDIPlusImage()
{
}
//--------------------------------------------
CGDIPlusImage::~CGDIPlusImage()
{
}
//--------------------------------------------


//--------------------------------------------
CStillImage::CStillImage()
 : m_pImage(NULL),
   m_bVisible(TRUE)
{
}
//--------------------------------------------
CStillImage::~CStillImage()
{
	if(NULL != m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}
//--------------------------------------------
BOOL CStillImage::LoadFromFile(const char* lpszFile)
{
	if(NULL == lpszFile)
		return FALSE;

	WCHAR wFileName[MAX_PATH];
	int iLen = MultiByteToWideChar(CP_ACP, 0, lpszFile, strlen(lpszFile), wFileName, MAX_PATH);
	wFileName[iLen] = L'\0';

	if(NULL != m_pImage)
		delete m_pImage;
	m_pImage = Bitmap::FromFile(wFileName, FALSE);	

	if(NULL == m_pImage)
		return FALSE;

	if(m_pImage->GetLastStatus() != Ok)
		return FALSE;

	return TRUE;
}
//--------------------------------------------
void CStillImage::Draw(HWND hwnd, HDC hdc, RECT rc)
{
	if(FALSE == m_bVisible)
		return;

	if(NULL == hdc)
		return;

	if(NULL == m_pImage)
		return;

	Graphics graphics(hdc);
	graphics.DrawImage(m_pImage, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
}
//--------------------------------------------
void CStillImage::Draw(HWND hwnd, HDC hdc, RECT &rcDes, RECT &rcSrc)
{
	if(FALSE == m_bVisible)
		return;

	if(NULL == hdc)
		return;

	if(NULL == m_pImage)
		return;

	int iWidth = m_pImage->GetWidth();
	int iHeight = m_pImage->GetHeight();
	Graphics graphics(hdc);
/*	Status sR = graphics.DrawImage(	m_pImage, rcDes.left, rcDes.top, 
						rcSrc.left, rcSrc.top, 
						int(rcSrc.right-rcSrc.left), 
						int(rcSrc.bottom-rcSrc.top), 
						UnitPixel);*/
	
	RectF rcF(rcDes.left, rcDes.top, rcDes.right-rcDes.left, rcDes.bottom-rcDes.top);
	graphics.DrawImage(m_pImage, rcF, rcSrc.left, rcSrc.top, rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top, UnitPixel, NULL, NULL, NULL);
}
//--------------------------------------------
void CStillImage::SetVisible(BOOL bVisile)
{
	m_bVisible = bVisile;
}
//--------------------------------------------
BOOL CStillImage::IsTransparent(int iPosX, int iPosY, float fTranshAphla)
{
	if(NULL == m_pImage)
		return TRUE;

	Color color(0, 0, 0);
	m_pImage->GetPixel(iPosX, iPosY, &color);

	BYTE bRed = color.GetR();
	BYTE bGreen = color.GetG();
	BYTE bBlue = color.GetB();
	BYTE bAphla = color.GetA();
	if(bAphla >= fTranshAphla)
		return TRUE;

	return FALSE;
}
//--------------------------------------------



//--------------------------------------------
CGIFImage::CGIFImage()
 : m_pImage(NULL),
   m_bVisible(TRUE)
{
}
//--------------------------------------------
CGIFImage::~CGIFImage()
{
}
//--------------------------------------------
BOOL CGIFImage::LoadFromFile(const char* lpszFile)
{
	if(NULL == lpszFile)
		return FALSE;

	WCHAR wFileName[MAX_PATH];
	int iLen = MultiByteToWideChar(CP_ACP, 0, lpszFile, strlen(lpszFile), wFileName, MAX_PATH);
	wFileName[iLen] = L'\0';
	wcslwr(wFileName);

	WCHAR* pExt = wFileName;
	while(*pExt != '\0') pExt++;
	while(*pExt != '.') pExt--;
	if(wcscmp(pExt, L".gif") != 0)
		return FALSE;

	if(NULL != m_pImage)
		delete m_pImage;
	m_pImage = new ImageEx(wFileName);

	if(NULL == m_pImage)
		return FALSE;

	return TRUE;
}
//--------------------------------------------
void CGIFImage::Draw(HWND hwnd, HDC hdc, RECT rc)
{
	if(NULL == hwnd)
		return;

	if(NULL == m_pImage)
		return;

	m_pImage->InitAnimation(hwnd, rc);
}
//--------------------------------------------
void CGIFImage::Draw(HWND hwnd, HDC hdc, RECT &rcDes, RECT &rcSrc)
{
	//Noting
}
//--------------------------------------------
void CGIFImage::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;

	if(NULL != m_pImage)
		m_pImage->SetPause(bVisible ? FALSE : TRUE);
}
//--------------------------------------------
BOOL CGIFImage::IsTransparent(int iPosX, int iPosY, float fTranshAphla)
{
	return FALSE;
}
//--------------------------------------------




//--------------------------------------------
CAnimateImage::CAnimateImage()
 : m_bVisible(TRUE),
   m_iCurFrameID(0)
{
}
//--------------------------------------------
CAnimateImage::~CAnimateImage()
{
}
//--------------------------------------------
BOOL CAnimateImage::LoadFromFile(const char* lpszFile)
{
	if(NULL == lpszFile)
		return FALSE;
	
	char szAnimateImg[MAX_PATH] = {0};
	strcpy(szAnimateImg, lpszFile);

	char szImgPath[MAX_PATH] = {0};
	char* lpExt = szAnimateImg;
	while(*lpExt != '\0') lpExt++;
	while(*lpExt != '\\') lpExt--;
	memcpy(szImgPath, lpszFile, (lpExt-szAnimateImg)*sizeof(char));

	char szFileName[MAX_PATH] = {0};
	ifstream ilkFile(szAnimateImg);

	while(true)
	{
		//Read a frame texture file name
		ilkFile.getline(szFileName, MAX_PATH);
		if(lstrlen(szFileName) == 0)
		{
			break;
		}    
		
		//The full image file name
		char szFullFileName[MAX_PATH] = {0};
		strcpy(szFullFileName, szImgPath);
		strcat(szFullFileName, szFileName);

		//Read the time lenght for current frame
		TCHAR szTime[32] = {_T("\0")};
		ilkFile.getline(szTime, 32);
		long lTimeLen = _ttol(szTime);

		//Load this frame image
		WCHAR wFileName[MAX_PATH];
		int iLen = MultiByteToWideChar(CP_ACP, 0, lpszFile, strlen(szFullFileName), wFileName, MAX_PATH);
		wFileName[iLen] = L'\0';
		Bitmap* pImage = Bitmap::FromFile(wFileName, FALSE);	
		//
		if(NULL == pImage)
			return FALSE;
		//
		if(pImage->GetLastStatus() != Ok)
		{
			delete pImage;
			return FALSE;
		}

		//Save
		m_lstImages.push_back(pImage);
		m_lstImgLen.push_back(lTimeLen);
	}

	return TRUE;
}
//--------------------------------------------
void CAnimateImage::Draw(HWND hwnd, HDC hdc, RECT rc)
{
	if(NULL == hwnd)
		return;
	
	Bitmap* pImage = m_lstImages[m_iCurFrameID];
	if(NULL == pImage)
		return;

	Graphics graphics(hdc);
	graphics.DrawImage(pImage, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);

	m_iCurFrameID++;
	if(m_iCurFrameID >= m_lstImages.size())
		m_iCurFrameID = 0;
}
//--------------------------------------------
void CAnimateImage::Draw(HWND hwnd, HDC hdc, RECT &rcDes, RECT &rcSrc)
{
	//Noting
}
//--------------------------------------------
void CAnimateImage::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
}
//--------------------------------------------
BOOL CAnimateImage::IsTransparent(int iPosX, int iPosY, float fTranshAphla)
{
	return FALSE;
}
//--------------------------------------------
