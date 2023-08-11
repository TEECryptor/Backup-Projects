//------------------------------------------------------------------
//	Name:OGLWnd.h
//	Introduce:The define file of OpenGL window class
//	Author:yaoyoufang
//	Date:05-7-2
//	Version:0.1v
//------------------------------------------------------------------
#ifndef _OGLWND_H_
#define _OGLWND_H_
//------------------------------------------------------------------
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/wglext.h>
#include "GenericWnd.h"
//------------------------------------------------------------------
//	The box faces define 
//------------------------------------------------------------------
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
//------------------------------------------------------------------
//	Name:VGSFileHeader
//	Introduce:The define for *.VGS file header
//------------------------------------------------------------------
typedef struct tagVGSFileHeader
{
	char	strType[4];		//The data format, 0:jpg,1:zlib,2:BMP
	int		iImageWidth;	//The image width in the *.VGS file
	int		iImageHeight;	//The image height in the *.VGS file
	int		iImageCnt;		//The image count in the *.VGS file
	int		iDivType;		//The division type
}VGSFileHeader, *LPVGSFILEHEADER;
//------------------------------------------------------------------
//	Name:FrameDataOffset
//	Introduce:Define the offset of frame data
//------------------------------------------------------------------
typedef struct tagFrameDataOffset
{
	DWORD			bId;				//Specifies the id of frame
	__int64			bOffset;			//Specifies the total offset of the frame data
}FrameDataOffset, *LPFRAMEDATAOFFSET;
//------------------------------------------------------------------
//	Name:Quad2D
//	Introduce:Define the quad in Opengl window
//------------------------------------------------------------------
typedef struct tagQuad2D
{
	int				iV[4][2];			//The 4 vertex of the quad
	float			fT[4][2];			//The texture coord 
}Quad2D, *LPQUAD2D;
//------------------------------------------------------------------
//	Name:COGLWnd
//	Introduce:Define the Opengl windows
//------------------------------------------------------------------
class COGLWnd : public CGenericWnd
{
public:
	COGLWnd(int id, HWND hWnd);
	~COGLWnd();
	//-------------------------------------------------------------
public:
	//-------------------------------------------------------------
	//the function of create OpenGL window
	BOOL		Create(HINSTANCE hInstance, HWND hWndParent, RECT rc, int iQuadCnt);
	//Set the multi-screen windows params
	void		SetWindowParam(float fColor, float fRatio, float fPower, int iArcHeight);
	//to set the camera param
	void		SetCameraParam(float nearpos, float farpos, float field);
	//To set the texture param
	void		SetTotalTexParam(int iWidth, int iHeight, int iAddWidth);	
	//Draw
	void		Draw(GLuint uTex);
	//Swap
	void		Swap();
	//-------------------------------------------------------------

	//-------------------------------------------------------------
private:
	//-------------------------------------------------------------
	//the procedure function of window
	LRESULT		WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	//the procedure function of WM_MY_CREATE message
	void		OnCreate();
	//the procedure function of WM_SIZE message
	void		OnSize();
	//-------------------------------------------------------------
	//Initalize OpenGL 
	void		OpenGLInit();	
	//Set the params of scene
	void		SetupRender();
	//Set the format of pixels
	BOOL		bSetPixelFormat(HDC hdc);
	//Set the OpenGL view port type
	void		SetViewport(int iViewportType, int iWidth, int iHeight);
	//To draw the over rect
	void		DrawOverFace(int ind);
	void		DrawOverFace_L();
	void		DrawOverFace_M();
	void		DrawOverFace_R();
	//-------------------------------------------------------------
	//To calucate the y coord on a ACR
	float		CalcArcCoord_Y(float fCenterX, float fCenterY, float fR, float x);
	//To create the quads, calucate the vertex and texture coord
	void		CalucateQuads();
	void		CalucateQuads2();
	//To draw the quads
	void		DrawQuads(GLuint uTex);
	//-------------------------------------------------------------
public:
	//-------------------------------------------------------------
public:	
	HDC					m_hdc;
	HGLRC				m_hrc;							//The handle of OGL render scene
	bool				m_bRenderEnd;					//
	static COGLWnd*		g_pTheFirstWnd;					
	//-------------------------------------------------------------
private:
	int					m_iID;							//窗口ID
	HWND				m_hHandWnd;
	RECT				m_stuCurRect;					//The current RECT of the window
	int					m_iWinWidth ;
	int					m_iWinHeight;
	//
	float				m_fCameraNear;
	float				m_fCameraFar;
	float				m_fCameraField;
	//
	float				m_fOverColor;					//覆盖面的颜色值
	float				m_fRatio;						//
	float				m_fPower;						//衰减指数
	//-------------------------------------------------------------
	int					m_iArcHeight;					//The height of the arc
	int					m_iQuadCnt;						//The quads count on the window
	Quad2D*				m_pQuad2D;						//The quads in the OpenGL window
	int					m_iTexWidth;
	int					m_iTexHeight;
	int					m_iAddTexWidth;					//The add texture width
	//-------------------------------------------------------------
};
//------------------------------------------------------------------
#endif	//_OGLWND_H_
//------------------------------------------------------------------
