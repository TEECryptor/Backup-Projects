//------------------------------------------------
//	FileName:	Image.h
//	Introduce:	The CImage class definition file
//				This class defines a image object
//				and is the base class of all format
//				image class
//	Creator:	Youfang Yao
//	Date:		05/25/06
//	Version:	1.0v
//------------------------------------------------
#ifndef _IMAGE_H_
#define _IMAGE_H_
//------------------------------------------------
class CImage
{
public:
	CImage();
	virtual ~CImage();
	//--------------------------------------------
public:	
	//Load the image data from a image file
	virtual bool LoadDataFromFile(const char* pszFile) = 0;
	//Load the image data from a image file data buffer
	virtual bool LoadDataFromBuff(unsigned char* pImageBuff, int iLen) = 0;
	//Save the image class data to a image file
	virtual bool SaveToFile(const char* pszFile) = 0;
	//--------------------------------------------
public:
	//Get the image width
	int		GetImageWidth();
	//Get the image height
	int		GetImageHeight();
	//Get the bits per pixel of this image
	int		GetBitsPerPixel();
	//Get the image data size, in byte
	int		GetImageDataSize();
	//Get the image data buffer pointer
	unsigned char* GetImageData();
	//--------------------------------------------
	//Set the image width
	void	SetImageWidth(int iWidth);
	//Set the image height
	void	SetImageHeight(int iHeight);
	//Set the image width
	void	SetBitsPerPixel(int iBitsPerPixel);
	//Set the image data size, in byte
	void	SetImageDataSize(int iImageDataSize);
	//Set the image data buffer pointer
	void	SetImageData(unsigned char* pData);
	//--------------------------------------------
	//Turn over the image
	void	TurnOverImage();
	//--------------------------------------------
	//Draw image on a hdc
	void	DrawImage(HDC hdc, int iLeft, int iTop, int iWidth, int iHeight);
	//--------------------------------------------
protected:
	//Do row arrange, the bytes count of row MUST is Nx4.
	void	DealImageRowPad();
	//--------------------------------------------
protected:
	int		m_iImageWidth;			//Image width
	int		m_iImageHeight;			//Image height
	int		m_iBitsPerPixel;		//The bits count per pixel,will be 24 or 32
	int		m_iImageDataSize;		//Image data size
	unsigned char*	m_pImageData;	//Image data buff,in BMP format
	//--------------------------------------------
	HBITMAP	m_hBitmap;				//The bitmap handle of this image
	//--------------------------------------------

};
//------------------------------------------------
#endif	//_IMAGE_H_
//------------------------------------------------
