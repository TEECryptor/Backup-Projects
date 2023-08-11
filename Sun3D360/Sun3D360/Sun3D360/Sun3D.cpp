//--------------------------------------------------------------
//	名称:Sun3D.cpp
//	简介:类Sun3D实现文件
//	创建:姚有方
//	日期:05-4-29
//	版本:0.1V
//--------------------------------------------------------------
#include "Sun3D.h"
//--------------------------------------------------------------
//	名称：CSun3D
//	简介：构造函数
//--------------------------------------------------------------
CSun3D::CSun3D()
{
	m_pOpenGLWnd = NULL;
}
//--------------------------------------------------------------
//	名称：~CSun3D
//	简介：析构函数
//--------------------------------------------------------------
CSun3D::~CSun3D()
{
}
//--------------------------------------------------------------
//	名称：S3D360_Create
//	简介：创建OpenGL窗体，并初始化OpenGL
//	参数：hInstance:i:应用程序实例
//	      hParent:i:父窗口句柄
//		  rc:i:OpenGL窗体的在父窗体上的位置
//	返回：创建成功返回true，否则返回false
//--------------------------------------------------------------
bool CSun3D::S3D360_Create(HINSTANCE hInstance, HWND hParent, RECT rc)
{	
	if(m_pOpenGLWnd) S3D360_Destroy();

	m_pOpenGLWnd = new COpenGLWnd();
	if(!m_pOpenGLWnd->Create(hInstance, hParent, rc)) return false;

	ShowWindow(m_pOpenGLWnd->m_hWnd, SW_SHOW);
	UpdateWindow(m_pOpenGLWnd->m_hWnd);
	return true;
}
//--------------------------------------------------------------
//	名称：S3D360_Resize
//	简介：设置OpenGL窗体的位置
//	参数：rc:i:OpenGL窗体在父窗体上的新位置
//	返回：无
//--------------------------------------------------------------
void CSun3D::S3D360_Resize(RECT rc)
{
	if(m_pOpenGLWnd)
	{
		::SetWindowPos(	m_pOpenGLWnd->m_hWnd, NULL, rc.left, rc.top, 
					rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
	}
}
//--------------------------------------------------------------
//	名称：S3D360_Destroy
//	简介：销毁OpenGL窗体
//	参数：无
//	返回：销毁成功返回true，否则返回false
//--------------------------------------------------------------
bool CSun3D::S3D360_Destroy()
{
	if(m_pOpenGLWnd)
	{
		::DestroyWindow(m_pOpenGLWnd->m_hWnd);
		delete m_pOpenGLWnd;
		m_pOpenGLWnd = NULL;
	}

	return true;
}
//--------------------------------------------------------------


//--------------------------------------------------------------
/*  以下是接口函数区域 */
//--------------------------------------------------------------
//	名称：S3D360_OpenFile
//	简介：打开指定的*.S3D文件，打开成功后播放第0帧
//	参数：file:i:指定的*.S3D文件名
//	返回：打开成功返回true，否则返回false
//--------------------------------------------------------------
bool CSun3D::S3D360_OpenFile(const char* file)
{
	if(m_pOpenGLWnd) m_pOpenGLWnd->OpenFile(file);
	return true;
}
//--------------------------------------------------------------
//	名称：S3D360_SetPlayType
//	简介：设置播放的方式
//	参数：iType:i:播放方式，0-自动播放，1-手动播放
//	返回：无
//--------------------------------------------------------------
void CSun3D::S3D360_SetPlayType(int iType)
{
	if(m_pOpenGLWnd) m_pOpenGLWnd->SetPlayType(iType);
}
//--------------------------------------------------------------
//	名称：S3D360_SetPause
//	简介：设置暂停
//	参数：iType:i:设置暂停，ture-暂停,false-继续播放
//	返回：无
//--------------------------------------------------------------
void CSun3D::S3D360_SetPause(bool bPause)
{
	if(m_pOpenGLWnd) m_pOpenGLWnd->SetPause(bPause);
}
//--------------------------------------------------------------
//	名称：S3D360_SetCameraInfo
//	简介：设置相机属性
//	参数：ang:i:夹角
//		  near:i:近裁剪面
//		  far:i:远裁剪面
//	返回：无
//--------------------------------------------------------------
void CSun3D::S3D360_SetCameraInfo(float ang, float nearpos, float farpos)
{
	if(m_pOpenGLWnd) m_pOpenGLWnd->SetCameraInfo(ang, nearpos, farpos);
}
//--------------------------------------------------------------
//	名称：S3D360_SetTimerInter
//	简介：设置渲染时钟间隔
//	参数：inter:i:时钟间隔，单位：毫秒
//	返回：无
//--------------------------------------------------------------
void CSun3D::S3D360_SetTimerInter(int inter)
{
	if(m_pOpenGLWnd) m_pOpenGLWnd->SetTimerInter(inter);
}
//--------------------------------------------------------------
