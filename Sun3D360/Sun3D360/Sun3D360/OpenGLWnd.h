//------------------------------------------------------------------
//	Name:OpenGLWnd.h
//	Introduce:The define file of OpenGL window class
//	Author:yaoyoufang
//	Date:05-3-2
//	Version:0.1v
//
//	Version:0.2v
//	Introduce:Add network function
//------------------------------------------------------------------
#ifndef _OPENGLWND_H_
#define _OPENGLWND_H_
//------------------------------------------------------------------
#include <Mmsystem.h>
#include <gl/gl.h>
#include <string>
#include <vector>   
#include <fstream>
//------------------------------------------------------------------
#include "GenericWnd.h"
#include "Sun3D360.h"
#include "Sun3D360Define.h"
#include "Camera.h"
//------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------
#ifndef M_PI
#define M_PI 3.14159f
#endif
//------------------------------------------------------------------
class COpenGLWnd : public CGenericWnd
{
public:
	COpenGLWnd();
	~COpenGLWnd();
	//-------------------------------------------------------
public:
	//-------------------------------------------------------
	//Create the OpenGL window
	BOOL		Create(HINSTANCE hInstance, HWND hWndParent, RECT rc);
	//To open the *.S3d file
	BOOL		OpenFile(const char* file);
	//Set the type of player
	void		SetPlayType(int iType);
	//To pause the player
	void		SetPause(bool bPause);
	//Set the player`s information 
	void		SetCameraInfo(float ang, float nearpos, float farpos);
	//Set the inter time of the timer
	void		SetTimerInter(int inter);
	//-------------------------------------------------------
private:
	//-------------------------------------------------------
	//the procedure function of window
	LRESULT		WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	//the procedure function of WM_MY_CREATE message
	void		OnCreate();
	//the procedure function of WM_SIZE message
	void		OnSize(WPARAM wParam, LPARAM lParam);
	//the procedure function of WM_TIMER message
	void		OnTimer();
	//the procedure function of WM_LBUTTONDOWN message
	void		OnLButtonDown(WPARAM wParam, LPARAM lParam);
	//the procedure function of WM_LBUTTONUP message
	void		OnLButtonUp(WPARAM wParam, LPARAM lParam);
	//the procedure function of WM_MOUSEMOVE message
	void		OnMouseMove(WPARAM wParam, LPARAM lParam);
	//the procedure function of WM_KEYDOWN message
	void		OnKeyDown(WPARAM wParam, LPARAM lParam);
	//the procedure function of WM_DESTROY message
	void		OnDestroy();
	//the procedur function of MM_JOY1MOVE message
	void		OnMmJoyMove(WPARAM wParam, LPARAM lParam);
	//-------------------------------------------------------	
	//Initalize OpenGL 
	void		OpenGLInit();	
	//Set the params of scene
	void		SetupRender();
	//Set the format of pixels
	BOOL		bSetPixelFormat(HDC hdc);
	//The render function
	void		RenderScene(); 
	//To Generate a texture object by data buffer
	GLuint		GenTexture(GLuint tex, GLint width, GLint height, GLenum formate, GLubyte* data);
	//To change a texture data from a new file data
	void		ChangeTexDataFromFileByID(int iLastID, int iCurID);
	//Read some data from stream to data buffer	
	void		ReadImageData(fstream& inf, BYTE*& data, int &cnt);
	//Load a jpg image from data buffer
	int			LoadJpgImageFromBuff(BYTE* srcdata, int size, BYTE*& data);
	//To chang the data of a texture object 
	void		ChangTexData(GLuint tex, GLint width, GLint height, GLenum format, GLubyte* data);
	//To move the file pointer
	void		MoveFilePointer(fstream& inf, int dir, int len);
	//To cheek the visible of 6 faces
	void		CheckFaceIsVisible();
	//To calculate the current camera rotate
	void		CalcCurCameraRot();
	//To calculate the current image ID base on user input
	void		CalcCurImageID();
	//-------------------------------------------------------	   
private:
	HGLRC			m_hrc;							//The handle of OGL render scene
	int				m_iWinWidth;					//The width of OGL window
	int				m_iWinHeight;					//The height of OGL window
	int				m_iTimerInter;					//The interal of render timer
	bool			m_bBusy;						//The render function is busy or not
	//-------------------------------------------------------	
    float           m_fCameraNearPos;				//The near posotion of camera
    float           m_fCameraFarPos;				//The far posotion of camera
    float           m_fCameraFieldAng;				//The field ang of camera
	//-------------------------------------------------------	
	int				m_iVisible[6];					//The faces are visible or not
	GLuint			m_uTex[6];						//The texture objects of 6 faces
	bool			m_bIsRoting;					//The camera is roted by mouse or not
	float			m_fCurRot[2];					//The current rotate of camera
	float			m_fCurRotDelt[2];				//The delt ang of the camera roting
	float			m_fLastXY[2];					//The last position of mouse
	GLint			m_iTexWidth;					//
	GLint			m_iTexHeight;					//
	GLint			m_iImageTotalCnt;				//
	fstream			m_fS3dFile;						//
	//-------------------------------------------------------
	int				m_iType;						//The type of play,0 is auto ane 1 is by hand
	bool			m_bDrawAll;						//Draw 6 faces or not
	CCamera			m_Camera;						//The current camera object
	int				m_iLastImageID;					//The last image ID
	int				m_iCurImageID;					//The current image ID
	vector<LPFRAMEDATAOFFSET>	m_lstFramDataOffset;
	//-------------------------------------------------------
	bool			m_bIsPause;						//The player is paused or not
	//-------------------------------------------------------
	bool			m_bJoyRotateCamera;				//Rotate the camera by joy or not
	JOYCAPS			m_stuJoyCaps;					//The state of joy
	//-------------------------------------------------------


	//-------------------------------------------------------	
};
//------------------------------------------------------------------
#endif	//_OPENGLWND_H_
//------------------------------------------------------------------