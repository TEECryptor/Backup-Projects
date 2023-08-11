//--------------------------------------------------------------
//	名称:Sun3D.h
//	简介:类Sun3D定义文件
//	创建:姚有方
//	日期:05-4-29
//	版本:0.1V
//--------------------------------------------------------------
#ifndef _SUN3D_H_
#define _SUN3D_H_
//--------------------------------------------------------------
#include "../Common/Sun3D360.h"
#include "OpenGLWnd.h"
//--------------------------------------------------------------
class CSun3D : public CSun3D360
{
public:
	CSun3D();
	~CSun3D();
	//----------------------------------------------------------
public:
	//创建CSun3D360接口，也就是创建OpenGL窗体，该接口必须先调用，然后才能调用其他接口
	bool __stdcall S3D360_Create(HINSTANCE hInstance, HWND hParent, RECT rc);
	//重新设置OpenGL窗体的大小，当其父窗体的大小改变时，调用该接口
	void __stdcall S3D360_Resize(RECT rc);
	//销毁OpenGL窗体，当应用程序退出时，调用该接口
	bool __stdcall S3D360_Destroy();
	//----------------------------------------------------------
	//打开指定的*.S3D文件
	bool __stdcall S3D360_OpenFile(const char* file);
	//设置播放的方式，0-自动播放，1-手动播放
	void __stdcall S3D360_SetPlayType(int iType);
	//设置暂停,ture-暂停,false-继续播放
	void __stdcall S3D360_SetPause(bool bPause);
	//设置相机属性
	void __stdcall S3D360_SetCameraInfo(float ang, float nearpos, float farpos);	
	//设置渲染时钟间隔
	void __stdcall S3D360_SetTimerInter(int inter);
	//----------------------------------------------------------
private:
	COpenGLWnd*			m_pOpenGLWnd;		//OpenGL渲染窗体
};
//--------------------------------------------------------------
#endif	//_SUN3D_H_
//--------------------------------------------------------------
