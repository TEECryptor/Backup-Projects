//------------------------------------------------
//	FileName:	JpgImage.h
//	Introduce:	The CJpgImage class definition file
//	Creator:	Youfang Yao
//	Date:		05/25/06
//	Version:	1.0v
//------------------------------------------------
#ifndef _JPGIMAGE_H_
#define _JPGIMAGE_H_
//------------------------------------------------
#include "Image.h"
//------------------------------------------------
class CJpgImage : public CImage
{
public:
	CJpgImage();
	virtual ~CJpgImage();
	//--------------------------------------------
public:
	//Load the Jpg image data from a jpg file
	bool	LoadDataFromFile(const char* pszFile);
	//Load the Jpg image data from a jpg image file data buffer
	bool	LoadDataFromBuff(unsigned char* pImageBuff, int iLen);
	//Save the image data in this class to a jpg file
	bool	SaveToFile(const char* pszFile);
	//--------------------------------------------
};
//------------------------------------------------
#endif	//_JPGIMAGE_H_
//------------------------------------------------
