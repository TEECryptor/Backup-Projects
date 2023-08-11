//------------------------------------------------
//	FileName:	BmpImage.h
//	Introduce:	The CBmpImage class definition file
//	Creator:	Youfang Yao
//	Date:		05/24/06
//	Version:	1.0v
//------------------------------------------------
#ifndef _BMPIMAGE_H_
#define _BMPIMAGE_H_
//------------------------------------------------
#include "Image.h"
#include <fstream>
//------------------------------------------------
using namespace std;
//------------------------------------------------
class CBmpImage : public CImage
{
public:
	CBmpImage();
	virtual ~CBmpImage();
	//--------------------------------------------
public:
	//Load the BMP image data from a bitmap file
	bool	LoadDataFromFile(const char* pszFile);
	//Load the BMP image data from a bitmap buffer
	bool	LoadDataFromBuff(unsigned char* pImageBuff, int iLen);
	//Save the image data in this class to a bmp file
	bool	SaveToFile(const char* pszFile);
	//--------------------------------------------
};
//------------------------------------------------
#endif	//_BMPIMAGE_H_
//------------------------------------------------