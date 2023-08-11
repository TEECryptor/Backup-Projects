//------------------------------------------------
//	FileName:	Image.cpp
//	Introduce:	The CImage class .cpp file
//	Creator:	Youfang Yao
//	Date:		05/25/06
//	Version:	1.0v
//------------------------------------------------
#include "stdafx.h"
#include "Image.h"
//------------------------------------------------
//	Construction
//------------------------------------------------
CImage::CImage() 
 :m_iImageWidth(0),m_iImageHeight(0),
  m_iBitsPerPixel(32),m_iImageDataSize(0),
  m_pImageData(NULL),m_hBitmap(NULL)
{
}
//------------------------------------------------
//	Distruction
//------------------------------------------------
CImage::~CImage()
{
	if(m_pImageData)
	{
		delete []m_pImageData;
		m_pImageData = NULL;
	}
	if(m_hBitmap)
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
}
//------------------------------------------------
//Get the image width
//------------------------------------------------
int	CImage::GetImageWidth()
{
	return m_iImageWidth;
}
//------------------------------------------------
//Get the image height
//------------------------------------------------
int	CImage::GetImageHeight()
{
	return m_iImageHeight;
}
//------------------------------------------------
//Get the bits per pixel of this image
//------------------------------------------------
int	CImage::GetBitsPerPixel()
{
	return m_iBitsPerPixel;
}
//------------------------------------------------
//Get the image data size, in byte
//------------------------------------------------
int	CImage::GetImageDataSize()
{
	return m_iImageDataSize;
}
//------------------------------------------------
//Get the image data buffer pointer
//------------------------------------------------
unsigned char* CImage::GetImageData()
{
	return m_pImageData;
}
//------------------------------------------------
//Set the image width
//------------------------------------------------
void CImage::SetImageWidth(int iWidth)
{
	m_iImageWidth = iWidth;
}
//------------------------------------------------
//Set the image height
//------------------------------------------------
void CImage::SetImageHeight(int iHeight)
{
	m_iImageHeight = iHeight;
}
//------------------------------------------------
//Set the image width
//------------------------------------------------
void CImage::SetBitsPerPixel(int iBitsPerPixel)
{
	m_iBitsPerPixel = iBitsPerPixel;
}
//------------------------------------------------
//Set the image data size, in byte
//------------------------------------------------
void CImage::SetImageDataSize(int iImageDataSize)
{
	m_iImageDataSize = iImageDataSize;
}
//------------------------------------------------
//Set the image data buffer pointer
//------------------------------------------------
void CImage::SetImageData(unsigned char* pData)
{
	if(m_iImageDataSize <= 0)
		return;

	if(m_pImageData) delete []m_pImageData;
	m_pImageData = new unsigned char[m_iImageDataSize];
	memcpy(m_pImageData, pData, m_iImageDataSize);
}
//------------------------------------------------
//Turn over the image
//------------------------------------------------	
void CImage::TurnOverImage()
{
	if(m_pImageData == NULL || m_iImageDataSize <= 0)
		return;

	int iRowDataLen = (((m_iImageWidth*m_iBitsPerPixel) + 31) >> 5) << 2;
	unsigned char* pRowData = new unsigned char[iRowDataLen];
	for(int iRow = 0; iRow < m_iImageHeight/2; iRow++)
	{
		memcpy(pRowData, &m_pImageData[iRow*iRowDataLen], iRowDataLen);
		memcpy(&m_pImageData[iRow*iRowDataLen], &m_pImageData[((m_iImageHeight-1)-iRow-1)*iRowDataLen], iRowDataLen);
		memcpy(&m_pImageData[((m_iImageHeight-1)-iRow-1)*iRowDataLen], pRowData, iRowDataLen);

	}
	delete []pRowData;
	pRowData = NULL;
}
//------------------------------------------------	
//Draw image on a hdc
//------------------------------------------------	
void CImage::DrawImage(HDC hdc, int iLeft, int iTop, int iWidth, int iHeight)
{
	if(!hdc || m_pImageData == NULL)
		return;

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biWidth = m_iImageWidth;
	bmi.bmiHeader.biHeight = m_iImageHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = m_iBitsPerPixel;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = m_iImageDataSize;

	StretchDIBits(hdc, iLeft, iTop, iWidth, iHeight, 
				0, 0, m_iImageWidth, m_iImageHeight, 
				m_pImageData, &bmi, DIB_RGB_COLORS, SRCCOPY);

/*	if(m_hBitmap == NULL) 
		return;
	
	HDC hMemDc = CreateCompatibleDC(hdc);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDc, m_hBitmap);
	
	StretchBlt(	hdc, iLeft, iTop, iWidth, iHeight,
				hMemDc, 0, 0, m_iImageWidth, m_iImageHeight,
				SRCCOPY);

	SelectObject(hMemDc, hOldBitmap);
	DeleteDC(hMemDc);
*/
}
//------------------------------------------------	


//------------------------------------------------	
//Do row arrange, the bytes count of row MUST is Nx4.
//------------------------------------------------	
void CImage::DealImageRowPad()
{
	if(m_pImageData == NULL)
		return;

	int iBytesPerPixel = m_iBitsPerPixel / 8;
	int iRowDataLen = (((m_iImageWidth*m_iBitsPerPixel) + 31) >> 5) << 2;
	if(iRowDataLen == m_iImageWidth * iBytesPerPixel)
		return;

	unsigned char* pTmpImageData = new unsigned char[m_iImageDataSize];
	memcpy(pTmpImageData, m_pImageData, m_iImageDataSize);
	delete []m_pImageData;

	m_iImageDataSize = iRowDataLen * m_iImageHeight;
	m_pImageData = new unsigned char[m_iImageDataSize];
	memset(m_pImageData, 0, m_iImageDataSize);
	for(int iCnt = 0;iCnt < m_iImageHeight;iCnt++)
	{
		memcpy(&m_pImageData[iCnt * iRowDataLen], &pTmpImageData[iCnt * m_iImageWidth*iBytesPerPixel], m_iImageWidth*iBytesPerPixel);
	}

	delete []pTmpImageData;
	pTmpImageData = NULL;
}
//------------------------------------------------	