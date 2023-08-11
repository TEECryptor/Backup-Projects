//	名称:Sun3D360.h
//	简介:Sun3D360 dll定义文件
//	创建:姚有方
//	日期:05-4-29
//	版本:0.1V
//--------------------------------------------------------------
#ifndef _SUN3D360_H_
#define _SUN3D360_H_
//--------------------------------------------------------------
#include <windows.h>
//--------------------------------------------------------------
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LSUN3D_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LSUN3D_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//--------------------------------------------------------------
#define SUN3D360_EXPORTS
#ifdef  SUN3D360_EXPORTS
#define SUN3D360_API __declspec(dllexport)
#else
#define SUN3D360_API __declspec(dllimport)
#endif
//--------------------------------------------------------------
//	Name:S3DFileHeader
//	Introduce:Define the header struct of the .S3D file
//--------------------------------------------------------------
/*
typedef struct tagS3DFileHeader
{
	DWORD			bfType;				//Specifies the file type. It must be SD 
	DWORD			bfSize;				//Specifies the size, in bytes, of the .S3D file 
	DWORD			bfImageCnt;			//Specifies the total count of images in the .S3D file
	DWORD			bfImageWidth;		//Specifies the width of the images
	DWORD			bfImageHeight;		//Specifies the height of the images
	DWORD			bfTexFormat;		//Specifies	the format of the textures will gen by this data in the .S3D file
}S3DFileHeader, *LPS3DFILEHEADER;
//--------------------------------------------------------------
//	Name:FrameDataOffset
//	Introduce:Define the offset of frame data
//--------------------------------------------------------------
typedef struct tagFrameDataOffset
{
	DWORD			bId;				//Specifies the id of frame
	DWORD			bTotalOffset;		//Specifies the total offset of the frame data
	DWORD			bOffset[6];			//Specifies the offset form begin of the current frame`s 6 image data
}FrameDataOffset, *LPFRAMEDATAOFFSET;
//--------------------------------------------------------------
//	Name:ImageFileInfo
//	Introduce:Define the file info of image file
//--------------------------------------------------------------
typedef struct tagImageFileInfo
{
	DWORD			bfSize;				//The size of the image file
	BYTE*			pData;				//The data buffer of the image file
}ImageFileInfo, *LPIMAGEFILEINFO;
//--------------------------------------------------------------
//	Name:S3DFrameInfo
//	Introduce:Define the image data info of one frame
//--------------------------------------------------------------
typedef struct tagS3DFrameInfo
{
	DWORD			bfID;				//Specifies the id of the group image data in the .S3D file
	ImageFileInfo	stuImages[6];		//Specifies	the image data of one frame  
}S3DFrameInfo, *LPS3DFRAMEINFO;
//--------------------------------------------------------------
*/
//--------------------------------------------------------------
/* CSun3D360 接口 */
//--------------------------------------------------------------
class CSun3D360
{
public:
	virtual ~CSun3D360(){;};
	//----------------------------------------------------------
public:
	//创建CSun3D360接口，也就是创建OpenGL窗体，该接口必须先调用，然后才能调用其他接口
	virtual bool __stdcall S3D360_Create(HINSTANCE hInstance, HWND hParent, RECT rc) = 0;
	//重新设置OpenGL窗体的大小，当其父窗体的大小改变时，调用该接口
	virtual void __stdcall S3D360_Resize(RECT rc) = 0;
	//销毁OpenGL窗体，当应用程序退出时，调用该接口
	virtual bool __stdcall S3D360_Destroy() = 0;
	//----------------------------------------------------------
	//打开指定的*.S3D文件
	virtual bool __stdcall S3D360_OpenFile(const char* file) = 0;
	//设置播放的方式，0-自动播放，1-手动播放
	virtual void __stdcall S3D360_SetPlayType(int iType) = 0;
	//设置暂停,ture-暂停,false-继续播放
	virtual void __stdcall S3D360_SetPause(bool bPause) = 0;
	//设置相机属性
	virtual void __stdcall S3D360_SetCameraInfo(float ang, float nearpos, float farpos) = 0;
	//设置渲染时钟间隔
	virtual void __stdcall S3D360_SetTimerInter(int inter) = 0;
};
//--------------------------------------------------------------

//--------------------------------------------------------------
/* DLL输出函数 */
//--------------------------------------------------------------
//构造函数，应用程序先调用该函数得到一个CSun3D360指针，然后才能调用其接口
SUN3D360_API CSun3D360* Sun3D360_Create();
//销毁函数，应用程序在退出的时候调用该函数来销毁CSun3D360对象
SUN3D360_API void Sun3D360_Destroy(CSun3D360* pSun3D360);
//--------------------------------------------------------------
#endif	//_SUN3D360_H_
//--------------------------------------------------------------