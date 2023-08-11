//------------------------------------------------
//	FileName:	LibImageImpl.h
//	Introduce:	The CLibImageImpl class header file
//	Creator:	Youfang Yao
//	Date:		05/24/06
//	Version:	1.0v
//------------------------------------------------
#ifndef _LIBIMAGEIMPL_H_
#define _LIBIMAGEIMPL_H_
//------------------------------------------------
#include "LibImage.h"
#include "Image.h"
//------------------------------------------------
class CLibImageImpl : public CLibImage
{
public:
	CLibImageImpl();
	virtual ~CLibImageImpl();
	//--------------------------------------------
public:
	//--------------------------------------------
	//Load an image data form file and transform it into bitmap format
	bool	LoadImageFromFile(const char* pszFile);
	//Load an image data form buffer and transform it into bitmap format
	bool	LoadImageFromBuff(unsigned char* pImageBuff, int iLen, eImageFormat eFormat);
	//Transform the bitmap data to an image format and save to the file
	bool	SaveImageAs(eImageFormat eFormat, const char* pszFile);
	//--------------------------------------------
	//Get the image width
	int		GetImageWidth();
	//Get the image Height
	int		GetImageHeight();
	//Get the image bit count of pixel,will be 24 or 32
	int		GetBitsPerPixel();
	//Get the image data size, in byte
	int		GetImageDataSize();
	//Get the image data buffer, in bitmap format
	unsigned char* GetImageData();
	//--------------------------------------------	
	//Draw the image on a DC
	void	Draw(HDC hdc, int left, int top, int width, int height);
	//--------------------------------------------
private:
	//Get the image file format form file name
	eImageFormat GetImageFormat(const char* pszFile);
	//--------------------------------------------
private:
	eImageFormat	m_ImageFormat;		//The current image format
	CImage*			m_pImage;			//The current image object pointer
	//--------------------------------------------
};
//------------------------------------------------
#endif  //_LIBIMAGEIMPL_H_
//------------------------------------------------