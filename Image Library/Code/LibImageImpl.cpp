//------------------------------------------------
//	FileName:	LibImageImpl.cpp
//	Introduce:	The CLibImageImpl class .cpp file
//	Creator:	Youfang Yao
//	Date:		05/24/06
//	Version:	1.0v
//------------------------------------------------
#include "stdafx.h"
#include "LibImageImpl.h"
#include "BmpImage.h"
#include "TgaImage.h"
#include "JpgImage.h"
//------------------------------------------------
//	Name:CLibImageImpl
//	Introduce:Construction
//------------------------------------------------
CLibImageImpl::CLibImageImpl()
 :m_ImageFormat(eUnknow),m_pImage(NULL)
{
}
//------------------------------------------------
//	Name:~CLibImageImpl
//	Introduce:Distruction
//------------------------------------------------
CLibImageImpl::~CLibImageImpl()
{
	if(m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}
//------------------------------------------------
//	Name:LoadImageFromFile
//	Introduce:Load an image data form file and transform it into bitmap format.
//	Param:pszFile:i:The image file name
//	Return:Return true if sucessful, otherwise return false
//------------------------------------------------
bool CLibImageImpl::LoadImageFromFile(const char* pszFile)
{
	if(pszFile == NULL)
		return false;

	m_ImageFormat = GetImageFormat(pszFile);

	if(m_pImage) delete m_pImage;
	switch(m_ImageFormat)
	{
	case eBitmap:
		m_pImage = new CBmpImage();
		break;
	case eTga:
		m_pImage = new CTgaImage();
		break;
	case eJpg:
		m_pImage = new CJpgImage();
		break;
	default:
		return false;
	}

	return m_pImage->LoadDataFromFile(pszFile);
}
//------------------------------------------------
//	Name:LoadImageFromBuff
//		Introduce:Load an image data form buffer and transform it into bitmap format.
//	Param:pImageBuff:i:The image data buffer
//	      eFormat:i:The image format in the image data buffer
//		  iLen:i:Buffer length,in byte
//	Return:Return true if sucessful, otherwise return false
//------------------------------------------------
bool CLibImageImpl::LoadImageFromBuff(unsigned char* pImageBuff, int iLen, eImageFormat eFormat)
{
	if(pImageBuff == NULL || iLen <= 0)
		return false;

	m_ImageFormat = eFormat;

	if(m_pImage) delete m_pImage;
	switch(m_ImageFormat)
	{
	case eBitmap:
		m_pImage = new CBmpImage();
		break;
	case eTga:
		m_pImage = new CTgaImage();
		break;
	case eJpg:
		m_pImage = new CJpgImage();
		break;
	default:
		return false;
	}

	return m_pImage->LoadDataFromBuff(pImageBuff, iLen);
}
//------------------------------------------------
//	Name:SaveImageAs
//	Introduce:Transform the bitmap data to an image format and save to the file.
//	Param:eFormat:i:The image format will to be saved
//		 pszFile:i:The image file name will to be saved
//	Return:Return true if sucessful, otherwise return false
//------------------------------------------------
bool CLibImageImpl::SaveImageAs(eImageFormat eFormat, const char* pszFile)
{	
	if(pszFile == NULL)
		return false;

	if(m_pImage == NULL)
		return false;

	//The new image format and the old are the same
	if(m_ImageFormat == eFormat)
	{
		return m_pImage->SaveToFile(pszFile);
	}
	//The new image format and the old are different
	else
	{
		bool bResult = false;
		CImage* pSaveImage = NULL;
		switch(eFormat)
		{
		case eBitmap:
			pSaveImage = new CBmpImage();
			break;
		case eTga:
			pSaveImage = new CTgaImage();
			break;
		case eJpg:
			pSaveImage = new CJpgImage();
			break;
		default:
			return false;
		}

		pSaveImage->SetImageWidth(m_pImage->GetImageWidth());
		pSaveImage->SetImageHeight(m_pImage->GetImageHeight());
		pSaveImage->SetBitsPerPixel(m_pImage->GetBitsPerPixel());
		pSaveImage->SetImageDataSize(m_pImage->GetImageDataSize());
		pSaveImage->SetImageData(m_pImage->GetImageData());

		bResult = pSaveImage->SaveToFile(pszFile);

		delete m_pImage;
		m_pImage = pSaveImage;
		m_ImageFormat = eFormat;

		return bResult;
	}

	return false;
}
//------------------------------------------------
//Get the image width
//------------------------------------------------
int CLibImageImpl::GetImageWidth()
{
	if(m_pImage)
		return m_pImage->GetImageWidth();
	else
		return 0;
}
//------------------------------------------------
//Get the image Height
//------------------------------------------------
int CLibImageImpl::GetImageHeight()
{
	if(m_pImage)
		return m_pImage->GetImageHeight();
	else
		return 0;
}
//------------------------------------------------
//Get the image bit count of pixel,will be 24 or 32
//------------------------------------------------
int CLibImageImpl::GetBitsPerPixel()
{
	if(m_pImage)
		return m_pImage->GetBitsPerPixel();
	else
		return 0;
}
//------------------------------------------------
//Get the image data size, in byte
//------------------------------------------------
int CLibImageImpl::GetImageDataSize()
{
	if(m_pImage)
		return m_pImage->GetImageDataSize();
	else
		return 0;
}
//------------------------------------------------
//Get the image data buffer, in bitmap format
//------------------------------------------------
unsigned char* CLibImageImpl::GetImageData()
{
	if(m_pImage)
		return m_pImage->GetImageData();
	else
		return NULL;
}
//------------------------------------------------
//	Name:Draw
//	Introduce:Draw the image on a DC
//	Param:hdc:i:The dest DC handle
//		  left,top,width,height:i:The dest rest on the DC
//	return:N/A
//------------------------------------------------
void CLibImageImpl::Draw(HDC hdc, int left, int top, int width, int height)
{
	if(m_pImage)
		m_pImage->DrawImage(hdc, left, top, width, height);
}
//------------------------------------------------


//------------------------------------------------
//Get the image file format form file name
//------------------------------------------------
eImageFormat CLibImageImpl::GetImageFormat(const char* pszFile)
{
	if(pszFile == NULL)
		return eUnknow;

	char* pTmp = (char*)pszFile;
	while(*pTmp != '\0') pTmp++;
	while((*pTmp != '.') && (pTmp != pszFile)) pTmp--;

	char FileExt[MAX_PATH] = {0};
	strcpy(FileExt, pTmp);
	strlwr(FileExt);
	if(strcmp(FileExt, ".bmp") == 0)
		return eBitmap;
	else if(strcmp(FileExt, ".tga") == 0)
		return eTga;
	else if(strcmp(FileExt, ".jpg")==0 || strcmp(FileExt, ".jpeg")==0)
		return eJpg;
	else 
		return eUnknow;
}
//------------------------------------------------