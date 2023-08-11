//---------------------------------------------------------------------
//	FileName:	LibImage.h
//	Introduce:	The .h file of the images library, define the interfaces
//				of this library
//	Creator:	Youfang Yao
//	Date:		05/24/06
//	Version:	1.0v 
//---------------------------------------------------------------------
#ifndef	_LIBIMAGE_H_
#define	_LIBIMAGE_H_
//---------------------------------------------------------------------
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBIMAGE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBIMAGE_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//---------------------------------------------------------------------
#define LIBIMAGE_EXPORTS

#ifdef LIBIMAGE_EXPORTS
#define LIBIMAGE_API __declspec(dllexport)
#else
#define LIBIMAGE_API __declspec(dllimport)
#endif
//---------------------------------------------------------------------
//	Name:eImageFormat
//	Introduce:The images format definition
//---------------------------------------------------------------------
enum eImageFormat
{
	eBitmap = 0,
	eTga,
	eJpg,
	eUnknow
};
//---------------------------------------------------------------------
//	Name:CLibImage
//	Introduce:The interface definition of the image library
//---------------------------------------------------------------------
class CLibImage
{
public:
	CLibImage(){};
	virtual ~CLibImage(){};
public:	
	/*	Name:LoadImageFromFile
		Introduce:Load an image data form image file and transform it into bitmap format.
		Param:pszFile:i:The image file name
		Return:Return true if sucessful, otherwise return false
	*/
	virtual bool LoadImageFromFile(const char* pszFile) = 0;

	/*	Name:LoadImageFromBuff
		Introduce:Load an image data form image buffer and transform it into bitmap format.
		Param:pImageBuff:i:The image data buffer
		      eFormat:i:The image format in the image data buffer
			  iLen:i:Buffer length, in byte
		Return:Return true if sucessful, otherwise return false
	*/
	virtual bool LoadImageFromBuff(unsigned char* pImageBuff, int iLen, eImageFormat eFormat) = 0;
	
	/*	Name:SaveImageAs
		Introduce:Transform the bitmap data to an image format and save to the file.
		Param:eFormat:i:The image format will to be saved
			  pszFile:i:The image file name will to be saved
		Return:Return true if sucessful, otherwise return false
	*/
	virtual bool SaveImageAs(eImageFormat eFormat, const char* pszFile) = 0;


	/*	Name:GetImageWidth
		Introduce:Get the image width
	*/
	virtual int GetImageWidth() = 0;

	/*	Name:GetImageHeight
		Introduce:Get the image height
	*/
	virtual int GetImageHeight() = 0;

	/*	Name:GetBitsPerPixel
		Introduce:Get the image bit count of pixel,will be 8,16,24 or 32
	*/
	virtual int GetBitsPerPixel() = 0;

	/*	Name:GetImageDataSize
		Introduce:Get the image data size, in byte
	*/
	virtual int GetImageDataSize() = 0;

	/*	Name:GetImageData
		Introduce:Get the image data buffer, in bitmap format
	*/
	virtual unsigned char* GetImageData() = 0;

	/*	Name:Draw
		Introduce:Draw the image on a DC
		Param:hdc:i:The dest DC handle
			  left,top,width,height:i:The dest rest on the DC
	*/
	virtual void Draw(HDC hdc, int left, int top, int width, int height) = 0;
};
//---------------------------------------------------------------------


//---------------------------------------------------------------------
/*The export functions of the dll*/
//---------------------------------------------------------------------
//To create an image library interface pointer, user apps do some image
//operations by this pointer
LIBIMAGE_API CLibImage* LibImage_Create();
//To destroy an image library interface pointer
LIBIMAGE_API bool LibImage_Destroy(CLibImage* pLibImage);
//---------------------------------------------------------------------
#endif	//_LIBIMAGE_H_
//---------------------------------------------------------------------

