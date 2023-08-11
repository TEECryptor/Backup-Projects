//	名称:Sun3D360Define.h
//	简介:Sun3D360 通用定义文件
//	创建:姚有方
//	日期:11-27-06
//	版本:0.1V
//--------------------------------------------------------------
#ifndef _SUN3D360_DEFINE_H_
#define _SUN3D360_DEFINE_H_
//--------------------------------------------------------------
#include <windows.h>
//--------------------------------------------------------------
//	The box faces define 
//--------------------------------------------------------------
enum
{
	IMAGE_F = 0,			//The front face of the box
	IMAGE_B,				//The black face of the box
	IMAGE_L,				//The left face of the box
	IMAGE_R,				//The right face of the box
	IMAGE_T,				//The top face of the box
	IMAGE_U,				//The under face of the box
	IMAGE_CNT				//The count of the box, is 6
};
//--------------------------------------------------------------
//	Name:VGSFileHeader
//	Introduce:The define for *.VGS file header
//--------------------------------------------------------------
typedef struct tagVGSFileHeader
{
	int		iImageWidth;	//The image width in the *.VGS file
	int		iImageHeight;	//The image height in the *.VGS file
	int		iImageCnt;		//The image count in the *.VGS file
}VGSFileHeader, *LPVGSFILEHEADER;
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
#endif //_SUN3D360_DEFINE_H_
//--------------------------------------------------------------
