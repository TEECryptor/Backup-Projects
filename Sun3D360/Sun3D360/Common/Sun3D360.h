//	����:Sun3D360.h
//	���:Sun3D360 dll�����ļ�
//	����:Ҧ�з�
//	����:05-4-29
//	�汾:0.1V
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
/* CSun3D360 �ӿ� */
//--------------------------------------------------------------
class CSun3D360
{
public:
	virtual ~CSun3D360(){;};
	//----------------------------------------------------------
public:
	//����CSun3D360�ӿڣ�Ҳ���Ǵ���OpenGL���壬�ýӿڱ����ȵ��ã�Ȼ����ܵ��������ӿ�
	virtual bool __stdcall S3D360_Create(HINSTANCE hInstance, HWND hParent, RECT rc) = 0;
	//��������OpenGL����Ĵ�С�����丸����Ĵ�С�ı�ʱ�����øýӿ�
	virtual void __stdcall S3D360_Resize(RECT rc) = 0;
	//����OpenGL���壬��Ӧ�ó����˳�ʱ�����øýӿ�
	virtual bool __stdcall S3D360_Destroy() = 0;
	//----------------------------------------------------------
	//��ָ����*.S3D�ļ�
	virtual bool __stdcall S3D360_OpenFile(const char* file) = 0;
	//���ò��ŵķ�ʽ��0-�Զ����ţ�1-�ֶ�����
	virtual void __stdcall S3D360_SetPlayType(int iType) = 0;
	//������ͣ,ture-��ͣ,false-��������
	virtual void __stdcall S3D360_SetPause(bool bPause) = 0;
	//�����������
	virtual void __stdcall S3D360_SetCameraInfo(float ang, float nearpos, float farpos) = 0;
	//������Ⱦʱ�Ӽ��
	virtual void __stdcall S3D360_SetTimerInter(int inter) = 0;
};
//--------------------------------------------------------------

//--------------------------------------------------------------
/* DLL������� */
//--------------------------------------------------------------
//���캯����Ӧ�ó����ȵ��øú����õ�һ��CSun3D360ָ�룬Ȼ����ܵ�����ӿ�
SUN3D360_API CSun3D360* Sun3D360_Create();
//���ٺ�����Ӧ�ó������˳���ʱ����øú���������CSun3D360����
SUN3D360_API void Sun3D360_Destroy(CSun3D360* pSun3D360);
//--------------------------------------------------------------
#endif	//_SUN3D360_H_
//--------------------------------------------------------------