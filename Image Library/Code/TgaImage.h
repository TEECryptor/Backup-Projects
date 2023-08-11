//------------------------------------------------
//	FileName:	TgaImage.h
//	Introduce:	The CTgaImage class definition file
//	Creator:	Youfang Yao
//	Date:		05/25/06
//	Version:	1.0v
//------------------------------------------------
#ifndef _TGAIMAGE_H_
#define _TGAIMAGE_H_
//------------------------------------------------
#include "Image.h"
#include <fstream>
//------------------------------------------------
using namespace std;
//------------------------------------------------
//	Name:TgaHeader
//	Introduce:The tga header information in file
//------------------------------------------------
typedef struct tagTgaHeader
{
	BYTE	ubImageInfoByteCnt;		//The image information length, in byte
	BYTE	ubColorTableExist;		//0-have not color table,1-have 
	BYTE	ubImageType;			//Image type,2-uncompare RGB image,10-compare RGB image
	BYTE	ubColorTableInfo[5];	//Color table information
	BYTE	ubImageInfo[10];		//Image information
}TgaHeader, *LPTGAHEADER;
//------------------------------------------------
//	Name:CTgaImage
//	Introduce:The CTgaImage class definition
//------------------------------------------------
class CTgaImage : public CImage
{
public:
	CTgaImage();
	virtual ~CTgaImage();
	//--------------------------------------------
public:
	//Load the Tga image data from a tga file
	bool	LoadDataFromFile(const char* pszFile);
	//Load the Tga image data from a tga image file data buffer
	bool	LoadDataFromBuff(unsigned char* pImageBuff, int iLen);
	//Save the image data in this class to a tga file
	bool	SaveToFile(const char* pszFile);
	//--------------------------------------------
private:
	//Load the Tga image data from a stream
	bool	LoadDataFromStream(istream &is);
	//Load the uncompare tga image data from file
	bool	LoadUnCompareData(istream &is, bool bNeedTurnOver);
	//Load the compare tga image data from file
	bool	LoadCompareData(istream &is, bool bNeedTurnOver);
	//--------------------------------------------
};
//------------------------------------------------
#endif	//_TGAIMAGE_H_
//------------------------------------------------
