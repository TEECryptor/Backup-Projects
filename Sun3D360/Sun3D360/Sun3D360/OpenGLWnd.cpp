//------------------------------------------------------------------
//	Name:OpenGLWnd.cpp
//	Introduce:The implement of OpenGL window class
//	Author:yaoyoufang
//	Date:05-3-2
//	Version:0.1v
//------------------------------------------------------------------
#include <windows.h>
#include <assert.h>
#include <gl/gl.h>
#include <gl/glu.h>
//------------------------------------------------------------------
#include "OpenGLWnd.h"
#include "JpgManager.h"
//------------------------------------------------------------------
const float	g_TexCoord_S = 0.001f;
const float g_TexCoord_E = 0.999f;
const float g_HalfWidth = 1.0f;
BYTE		g_KeyState[256] = {0}; 
//------------------------------------------------------------------
//	Name:COpenGLWnd
//	Introduce:Constructor
//------------------------------------------------------------------
COpenGLWnd::COpenGLWnd()
{
	m_hrc = NULL;			
	m_iWinWidth = 1;	
	m_iWinHeight = 1;	
	m_iTimerInter = 33;
	m_bBusy = false;

	m_fCameraNearPos = 0.5f;	
	m_fCameraFarPos = 4.0f;	
	m_fCameraFieldAng = 65;	

	m_bIsRoting = false;
	m_fCurRot[0] = 0;
	m_fCurRot[1] = 0;
	memset(m_fCurRotDelt, 0, 2*sizeof(float));	
	memset(m_fLastXY, 0, 2*sizeof(float));		

	m_iTexWidth = 512;
	m_iTexHeight = 512;
	m_iImageTotalCnt = 0;
	memset(m_uTex, 0, 6*sizeof(GLuint));
	memset(m_iVisible, 0, 6*sizeof(int));	

	m_iType = 0;
	m_iLastImageID = -1;
	m_iCurImageID = 0;

	m_bIsPause = false;

	m_bJoyRotateCamera = false;
	ZeroMemory(&m_stuJoyCaps, sizeof(JOYCAPS));
}
//------------------------------------------------------------------
//	Name:~COpenGLWnd
//	Introduce:Destruction
//------------------------------------------------------------------
COpenGLWnd::~COpenGLWnd()
{
	glDeleteTextures(2, m_uTex);
}
//------------------------------------------------------------------
//	Name:Create
//	Introduce:Create the OpenGL window
//------------------------------------------------------------------
BOOL COpenGLWnd::Create(HINSTANCE hInstance, HWND hWndParent, RECT rc)
{
	m_iWinWidth = rc.right - rc.left;
	m_iWinHeight = rc.bottom - rc.top;

	return CGenericWnd::Create(hInstance, NULL, hWndParent, rc, WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE);
}
//------------------------------------------------------------------
//	Name:WndProc
//	Introduce:The process function of the OpenGL window
//------------------------------------------------------------------
LRESULT COpenGLWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_MY_CREATE:
		OnCreate();
		break;
	case WM_SIZE:
		OnSize(wParam, lParam);
		break;
	case WM_TIMER:
		OnTimer();
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(wParam, lParam);
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, lParam);
		break;
	case WM_KEYDOWN:
		OnKeyDown(wParam, lParam);
		break;
	case WM_DESTROY:
		OnDestroy();
		break;
	case MM_JOY1MOVE:				//The joystrick messages
		OnMmJoyMove(wParam, lParam);
		break;
	default:
		return DefWindowProc(m_hWnd, msg, wParam, lParam);
	}

	return 0;
}
//------------------------------------------------------------------
//	Name:OnCreate
//	Introduce:the procedure function of WM_MY_CREATE message
//	Param:void
//	Return:void
//------------------------------------------------------------------
void COpenGLWnd::OnCreate()
{
	//Capture the joystrick, add by yaoyoufang 05-7-7
	if(JOYERR_NOERROR  == joySetCapture(m_hWnd, JOYSTICKID1, NULL, FALSE)) 
		joyGetDevCaps(JOYSTICKID1, &m_stuJoyCaps, sizeof(JOYCAPS));
	else
        MessageBeep(MB_ICONEXCLAMATION); 
	//end

	OpenGLInit();
	SetupRender();
}
//------------------------------------------------------------------
//	Name:OnSize
//	Introduce:the procedure function of WM_SIZE message
//	Param:
//	Return:void
//------------------------------------------------------------------
void COpenGLWnd::OnSize(WPARAM wParam, LPARAM lParam)
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_iWinWidth = (rc.right-rc.left > 0 ? rc.right-rc.left : 1); 
	m_iWinHeight = (rc.bottom-rc.top > 0 ? rc.bottom-rc.top : 1);
	
	//set the new viewport
	glViewport(0, 0, m_iWinWidth, m_iWinHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(m_fCameraFieldAng, (float)m_iWinWidth/m_iWinHeight, m_fCameraNearPos, m_fCameraFarPos);
	glMatrixMode(GL_MODELVIEW);
}
//------------------------------------------------------------------
//	Name:OnTimer
//	Introduce:the procedure function of WM_TIMER message
//	Param:void
//	Return:void
//------------------------------------------------------------------
void COpenGLWnd::OnTimer()
{
	RenderScene();
}
//------------------------------------------------------------------
//	Name:OnLButtonDown
//	Introduce:the procedure function of WM_LBUTTONDOWN message
//	Param:
//	Return:void
//------------------------------------------------------------------
void COpenGLWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
    m_fLastXY[0] = LOWORD(lParam); 
    m_fLastXY[1] = HIWORD(lParam);
	m_bIsRoting = true;

	SendMessage(m_hParent, WM_LBUTTONDOWN, wParam, lParam);
}
//------------------------------------------------------------------
//	Name:OnLButtonUp
//	Introduce:the procedure function of WM_LBUTTONUP message
//	Param:
//	Return:void
//------------------------------------------------------------------
void COpenGLWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	m_bIsRoting = false;

	SendMessage(m_hParent, WM_LBUTTONUP, wParam, lParam);
}
//------------------------------------------------------------------
//	Name:OnMouseMove
//	Introduce:the procedure function of WM_MOUSEMOVE message
//	Param:
//	Return:void
//------------------------------------------------------------------
void COpenGLWnd::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	int xPos = LOWORD(lParam); 
	int yPos = HIWORD(lParam);

	if(m_bIsRoting)
	{
		m_fCurRotDelt[1] = ((xPos - m_fLastXY[0])* 180 * 0.2f) / m_iWinWidth;
		m_fCurRotDelt[0] = ((yPos - m_fLastXY[1])* 180 * 0.2f) / m_iWinHeight;
	}

	SendMessage(m_hParent, WM_MOUSEMOVE, wParam, lParam);
}
//------------------------------------------------------------------
//	Name:OnKeyDown
//	Introduce:the procedure function of WM_KEYDOWN message
//	Param:
//	Return:void
//------------------------------------------------------------------
void COpenGLWnd::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	int nVirtKey = (int) wParam;
	
	SendMessage(m_hParent, WM_KEYDOWN, wParam, lParam);
}
//------------------------------------------------------------------
//	Name:OnMmJoyMove
//	Introduce:The procedur function of MM_JOY1MOVE message
//	Param:wParam:i:button
//		  lParam:i:position
//	Return:void
//------------------------------------------------------------------
void COpenGLWnd::OnMmJoyMove(WPARAM wParam, LPARAM lParam)
{
	int fwButtons = wParam; 
	int xPos = LOWORD(lParam); 
	int yPos = HIWORD(lParam); 

	//Turn camera
	if((xPos == 0) || (fwButtons == JOY_BUTTON1))
	{
		m_bJoyRotateCamera = true;
		// to left
		if(xPos == 0)
		{
			m_fCurRotDelt[0] = 0;
			m_fCurRotDelt[1] = -1.0f;
		}
		// to up
		else
		{
			m_fCurRotDelt[0] = -1.0f;
			m_fCurRotDelt[1] = 0;
		}
	}
	//Turn camera
	else if((xPos == m_stuJoyCaps.wXmax) || (fwButtons == JOY_BUTTON3))
	{
		m_bJoyRotateCamera = true;
		// to right
		if(xPos == m_stuJoyCaps.wXmax)
		{
			m_fCurRotDelt[0] = 0;
			m_fCurRotDelt[1] = 1.0f;
		}
		// to down
		else
		{
			m_fCurRotDelt[0] = 1.0f;
			m_fCurRotDelt[1] = 0;
		}
	}
	else 
	{		
		m_bJoyRotateCamera = false;
	}

	//根据用户的按键，调整当前要显示的图片序号，以及要删除和要加载的图片序号
	static bool bFrameIsChange = true;

	//move fornt
	if(yPos == 0)
	{
		if( (m_fCurRot[1] >= 0 && m_fCurRot[1] <= 90) || (m_fCurRot[1] >= 270 && m_fCurRot[1] <= 360) ||
			(m_fCurRot[1] >= -90 && m_fCurRot[1] <= 0) || (m_fCurRot[1] >= -360 && m_fCurRot[1] <= -270))	m_iCurImageID--;		
		else m_iCurImageID++;

		/*为什么要去掉最后两帧呢???如果不去掉,当手动到最后一帧后不能后退,估计原因是
		当文件指针读取到文件结束处时自动关闭文件了,所以再向后移动文件指针时没有反应
		*/
		if(m_iCurImageID > m_iImageTotalCnt-2) m_iCurImageID = m_iImageTotalCnt-2;
		bFrameIsChange = true;
	}
	//move back
	else if(yPos == m_stuJoyCaps.wYmax)
	{
		if((m_fCurRot[1] >= 90 && m_fCurRot[1] <= 270) || (m_fCurRot[1] >= -270 && m_fCurRot[1] <= -90))	m_iCurImageID--;
		else m_iCurImageID++;

		if(m_iCurImageID < 0) m_iCurImageID = 0;
		bFrameIsChange = true;
	}
	else
	{
		if(bFrameIsChange)
		{
			memset(m_iVisible, 1, 6*sizeof(int));
			ChangeTexDataFromFileByID(m_iCurImageID-1, m_iCurImageID);
		}
		bFrameIsChange = false;
	}
}
//------------------------------------------------------------------
//	Name:OnDestroy
//	Introduce:the procedure function of WM_DESTROY
//	Param:void
//	Return:void
//------------------------------------------------------------------
void COpenGLWnd::OnDestroy()
{
	//delete DC
	::wglMakeCurrent(NULL,  NULL);	
	if(m_hrc)
	{
		::wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}

	//release joystrick
	joyReleaseCapture(JOYSTICKID1);
}
//------------------------------------------------------------------


//------------------------------------------------------------------
//	Name:OpenGLInit
//	Introduce:Initalize OpenGL 
//	Param:void
//	Return:void
//------------------------------------------------------------------
void COpenGLWnd::OpenGLInit()
{	
	PIXELFORMATDESCRIPTOR pfd;
	int n;

	HDC hdc = GetDC(m_hWnd);

	assert(hdc != NULL);						
	
	if(!bSetPixelFormat(hdc))					
		return;

	n = ::GetPixelFormat(hdc);					
												
	::DescribePixelFormat(hdc, n, sizeof(pfd), &pfd);
												
	m_hrc = wglCreateContext(hdc);
												
	wglMakeCurrent(hdc, m_hrc);
}
//------------------------------------------------------------------
//	Name:SetupRender
//	Introduce:Set the params of scene
//	Param:void
//	Return:void
//------------------------------------------------------------------
void COpenGLWnd::SetupRender()
{   
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_TEXTURE_2D);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}
//------------------------------------------------------------------
//	Name:bSetPixelFormat
//	Introduce:Set the format of pixels
//	Param:hdc:the display device context 
//	Return: return TRUE if successfully,otherwise return FALSE
//------------------------------------------------------------------
BOOL COpenGLWnd::bSetPixelFormat(HDC hdc)
{	
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),     
		1,                                 
		PFD_DRAW_TO_WINDOW|                
		PFD_SUPPORT_OPENGL|                
		PFD_DOUBLEBUFFER,                  
		PFD_TYPE_RGBA,                     
		24,                                
		0,0,0,0,0,0,                       
		0,                                 
		0,                                 
		0,                                 
		0,0,0,0,                           
		32,                                
		8,                                 
		0,                                 
		PFD_MAIN_PLANE,                    
		0,                                 
		0,0,0                              
	};
	int pixelFormat;                       
                                           
	if((pixelFormat = ChoosePixelFormat(hdc,&pfd)) == 0)
	{
		return FALSE;
	}
                                            
	if(SetPixelFormat(hdc, pixelFormat, &pfd) == FALSE)
	{
		assert(FALSE);
		return FALSE;
	}

	return TRUE;
}
//------------------------------------------------------------------
//	Name:RenderScene
//	Introduce:The render function
//	Param:void
//	Return:void
//------------------------------------------------------------------
void COpenGLWnd::RenderScene()
{
	if(m_bBusy) return;
	m_bBusy = true;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	//The new camera rotate 
	CalcCurCameraRot();
	glRotatef(m_fCurRot[0], 1, 0, 0);
	glRotatef(m_fCurRot[1], 0, 1, 0);
	
	//To cheek the faces is visible or not
	CheckFaceIsVisible();
	
	//Play by auto 
	if(m_iType == 0)
	{
		if(!m_bIsPause)
		{
			ChangeTexDataFromFileByID(m_iLastImageID, m_iCurImageID);
			m_iLastImageID = m_iCurImageID;
			if(++m_iCurImageID >= m_iImageTotalCnt-1) m_iCurImageID = 0;
		}
	}
	//Play by hand
	else
	{
		//Chang the image ID by user inputing
		CalcCurImageID();
		ChangeTexDataFromFileByID(m_iLastImageID, m_iCurImageID);
		m_iLastImageID = m_iCurImageID;
	}

	//前
	if(m_iVisible[0])
	{
		glBindTexture(GL_TEXTURE_2D, m_uTex[IMAGE_F]);  
		glBegin(GL_QUADS);
			glTexCoord2f(g_TexCoord_S, g_TexCoord_E);	glVertex3f(-g_HalfWidth, g_HalfWidth, -g_HalfWidth);
			glTexCoord2f(g_TexCoord_S, g_TexCoord_S);	glVertex3f(-g_HalfWidth, -g_HalfWidth, -g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_S);	glVertex3f(g_HalfWidth, -g_HalfWidth, -g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_E);	glVertex3f(g_HalfWidth, g_HalfWidth, -g_HalfWidth);
		glEnd();
	}
	//后
	if(m_iVisible[1])
	{
		glBindTexture(GL_TEXTURE_2D, m_uTex[IMAGE_B]);  
		glBegin(GL_QUADS);
			glTexCoord2f(g_TexCoord_S, g_TexCoord_E);	glVertex3f(g_HalfWidth, g_HalfWidth, g_HalfWidth);
			glTexCoord2f(g_TexCoord_S, g_TexCoord_S);	glVertex3f(g_HalfWidth, -g_HalfWidth, g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_S);	glVertex3f(-g_HalfWidth, -g_HalfWidth, g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_E);	glVertex3f(-g_HalfWidth, g_HalfWidth, g_HalfWidth);
		glEnd();
	}
	//左
	if(m_iVisible[2])
	{
		glBindTexture(GL_TEXTURE_2D, m_uTex[IMAGE_L]);  
		glBegin(GL_QUADS);
			glTexCoord2f(g_TexCoord_S, g_TexCoord_E);	glVertex3f(-g_HalfWidth, g_HalfWidth, g_HalfWidth);
			glTexCoord2f(g_TexCoord_S, g_TexCoord_S);	glVertex3f(-g_HalfWidth, -g_HalfWidth, g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_S);	glVertex3f(-g_HalfWidth, -g_HalfWidth, -g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_E);	glVertex3f(-g_HalfWidth, g_HalfWidth, -g_HalfWidth);
		glEnd();
	}
	//右
	if(m_iVisible[3])
	{
		glBindTexture(GL_TEXTURE_2D, m_uTex[IMAGE_R]); 
		glBegin(GL_QUADS); 
			glTexCoord2f(g_TexCoord_S, g_TexCoord_E);	glVertex3f(g_HalfWidth, g_HalfWidth, -g_HalfWidth);
			glTexCoord2f(g_TexCoord_S, g_TexCoord_S);	glVertex3f(g_HalfWidth, -g_HalfWidth, -g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_S);	glVertex3f(g_HalfWidth, -g_HalfWidth, g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_E);	glVertex3f(g_HalfWidth, g_HalfWidth, g_HalfWidth);
		glEnd();
	}
	//顶
	if(m_iVisible[4])
	{
		glBindTexture(GL_TEXTURE_2D, m_uTex[IMAGE_T]); 
		glBegin(GL_QUADS); 		
/*			glTexCoord2f(g_TexCoord_S, g_TexCoord_E);	glVertex3f(-g_HalfWidth, g_HalfWidth, -g_HalfWidth);
			glTexCoord2f(g_TexCoord_S, g_TexCoord_S);	glVertex3f(g_HalfWidth, g_HalfWidth, -g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_S);	glVertex3f(g_HalfWidth, g_HalfWidth, g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_E);	glVertex3f(-g_HalfWidth, g_HalfWidth, g_HalfWidth);
*/
			glTexCoord2f(g_TexCoord_S, g_TexCoord_E);	glVertex3f(-g_HalfWidth, g_HalfWidth, -g_HalfWidth);
			glTexCoord2f(g_TexCoord_S, g_TexCoord_S);	glVertex3f(-g_HalfWidth, g_HalfWidth, g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_S);	glVertex3f(g_HalfWidth, g_HalfWidth, g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_E);	glVertex3f(g_HalfWidth, g_HalfWidth, -g_HalfWidth);
		glEnd();
	}
	//底
	if(m_iVisible[5])
	{
		glBindTexture(GL_TEXTURE_2D, m_uTex[IMAGE_U]);  
		glBegin(GL_QUADS);
/*			glTexCoord2f(g_TexCoord_S, g_TexCoord_E);	glVertex3f(g_HalfWidth, -g_HalfWidth, -g_HalfWidth);
			glTexCoord2f(g_TexCoord_S, g_TexCoord_S);	glVertex3f(-g_HalfWidth, -g_HalfWidth, -g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_S);	glVertex3f(-g_HalfWidth, -g_HalfWidth, g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_E);	glVertex3f(g_HalfWidth, -g_HalfWidth, g_HalfWidth);
*/
			glTexCoord2f(g_TexCoord_S, g_TexCoord_E);	glVertex3f(-g_HalfWidth, -g_HalfWidth, -g_HalfWidth);
			glTexCoord2f(g_TexCoord_S, g_TexCoord_S);	glVertex3f(-g_HalfWidth, -g_HalfWidth, g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_S);	glVertex3f(g_HalfWidth, -g_HalfWidth, g_HalfWidth);
			glTexCoord2f(g_TexCoord_E, g_TexCoord_E);	glVertex3f(g_HalfWidth, -g_HalfWidth, -g_HalfWidth);

		glEnd();
	}

	SwapBuffers(wglGetCurrentDC());
	
	m_bBusy = false;
} 
//------------------------------------------------------------------
//	To Generate a texture object by data buffer
//------------------------------------------------------------------
GLuint COpenGLWnd::GenTexture(GLuint tex, GLint width, GLint height, GLenum formate, GLubyte* data)
{
	if(data == NULL) return 0;

    glEnable(GL_TEXTURE_2D);
	if(tex == 0)
	{
		glGenTextures(1, &tex);
	}

	glBindTexture(GL_TEXTURE_2D, tex);    
	if(formate == GL_RGBA)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
/*
	if(formate == GL_RGB)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	else 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return tex;
}
//------------------------------------------------------------------
//	To change a texture data from a new file data
//------------------------------------------------------------------
void COpenGLWnd::ChangeTexDataFromFileByID(int iLastID, int iCurID)
{	
	//If the image ID is same, return
	if(iCurID == iLastID) return;

	DWORD id;
	m_fS3dFile.read(reinterpret_cast<char*>(&id), sizeof(DWORD));
	if(id != iCurID)
	{
		if(iCurID < 0 || iCurID > m_lstFramDataOffset.size()-1) return;
		else
		{
			m_fS3dFile.seekg(m_lstFramDataOffset[iCurID]->bTotalOffset, ios::beg);
			m_fS3dFile.read(reinterpret_cast<char*>(&id), sizeof(DWORD));

			if(id != iCurID) return;
		}
	}

	int iDataSize = 0;
	BYTE* pSrcData = NULL;
	BYTE* pDesData = NULL;
	if(iCurID >=0 && iCurID < m_iImageTotalCnt)
	{
		for(int tex = 0;tex < 6;tex++)
		{
			if(m_iVisible[tex] != 0)
			{
				ReadImageData(m_fS3dFile, pSrcData, iDataSize);
				if(LoadJpgImageFromBuff(pSrcData, iDataSize, pDesData))
				{
					ChangTexData(m_uTex[tex], m_iTexWidth, m_iTexHeight, GL_BGR_EXT, pDesData);
				}	
			}
			else
			{
				int len = 0;
				m_fS3dFile.read(reinterpret_cast<char*>(&len), sizeof(DWORD));
				MoveFilePointer(m_fS3dFile, 1, len);
			}
		}
	}

	if(pSrcData)
	{
		delete[] pSrcData;
		pSrcData = NULL;
	}
	if(pDesData)
	{
		delete[] pDesData;
		pDesData = NULL;
	}
}
//------------------------------------------------------------------
//	Read some data from stream to data buffer	
//------------------------------------------------------------------
void  COpenGLWnd::ReadImageData(fstream& inf, BYTE*& data, int &cnt)
{
	inf.read(reinterpret_cast<char*>(&cnt), sizeof(DWORD));

	if(data) delete[] data;
	data = new BYTE[cnt];
	inf.read(reinterpret_cast<char*>(data), cnt);
}
//------------------------------------------------------------------
//	Load a jpg image from data buffer
//------------------------------------------------------------------
int COpenGLWnd::LoadJpgImageFromBuff(BYTE* srcdata, int size, BYTE*& data)
{
	if(srcdata == NULL) return 0;

	CJpgManager jpgLoader;
	if(jpgLoader.jpgReadBuff(srcdata, size))
	{
		int width = jpgLoader.jpgGetImageParams(_JPGIMAGEWIDTH);
		int height = jpgLoader.jpgGetImageParams(_JPGIMAGEHEIGHT);
		BYTE* pData = jpgLoader.jpgGetImageData();

		if(data) delete[] data;
		data = new unsigned char[width*height*3];		
		for(int i = 0;i < height;i++)
			memcpy(&data[i*width*3], &pData[(height-i-1)*width*3], width*3);
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------
//	To chang the data of a texture object 
//------------------------------------------------------------------
void COpenGLWnd::ChangTexData(GLuint tex, GLint width, GLint height, GLenum format, GLubyte* data)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);    
	if(tex == 0)
	{
		glGenTextures(1, &tex);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	}
}
//------------------------------------------------------------------
//	To move the file pointer
//------------------------------------------------------------------
void COpenGLWnd::MoveFilePointer(fstream& inf, int dir, int len)
{
	if(dir == 1)
	{
		inf.seekg(len, ios::cur);
	}
	else
	{
		inf.seekg(-1 * len, ios::cur);
	}
}
//------------------------------------------------------------------
//	To cheek the visible of 6 faces
//------------------------------------------------------------------
void COpenGLWnd::CheckFaceIsVisible()
{
	//To calculate the current camera view frustum
	m_Camera.CalculateViewFrustum();
	
	//前面
	m_iVisible[0] = m_Camera.CuboidFrustumTest(-g_HalfWidth, g_HalfWidth, -g_HalfWidth, g_HalfWidth, -g_HalfWidth, -g_HalfWidth);
	//后面
	m_iVisible[1] = m_Camera.CuboidFrustumTest(-g_HalfWidth, g_HalfWidth, -g_HalfWidth, g_HalfWidth, g_HalfWidth, g_HalfWidth);
	//左面
	m_iVisible[2] = m_Camera.CuboidFrustumTest(-g_HalfWidth, -g_HalfWidth, -g_HalfWidth, g_HalfWidth, -g_HalfWidth, g_HalfWidth);
	//右面
	m_iVisible[3] = m_Camera.CuboidFrustumTest(g_HalfWidth, g_HalfWidth, -g_HalfWidth, g_HalfWidth, -g_HalfWidth, g_HalfWidth);
	//顶面
	m_iVisible[4] = m_Camera.CuboidFrustumTest(-g_HalfWidth, g_HalfWidth, g_HalfWidth, g_HalfWidth, -g_HalfWidth, g_HalfWidth);
	//底面
	m_iVisible[5] = m_Camera.CuboidFrustumTest(-g_HalfWidth, g_HalfWidth, -g_HalfWidth, -g_HalfWidth, -g_HalfWidth, g_HalfWidth);
}
//------------------------------------------------------------------
//	To calculate the current camera rotate
//------------------------------------------------------------------
void COpenGLWnd::CalcCurCameraRot()
{
	//Get the current state of keyboard
	GetKeyboardState(g_KeyState);
	if(g_KeyState['A'] > 64 || g_KeyState['a'] > 64 || g_KeyState[VK_LEFT] > 64)
	{
		m_fCurRot[1] -= 1;
	}
	else if(g_KeyState['D'] > 64 || g_KeyState['d'] > 64 || g_KeyState[VK_RIGHT] > 64)
	{
		m_fCurRot[1] += 1;
	}

	//Rotate the camera by mouse or joy
	if(m_bIsRoting || m_bJoyRotateCamera)
	{
		m_fCurRot[0] += m_fCurRotDelt[0];
		m_fCurRot[1] += m_fCurRotDelt[1];
	}
	
	//
	if(m_fCurRot[0] > 90) m_fCurRot[0] = 90;
	else if(m_fCurRot[0] < -90) m_fCurRot[0] = -90;

	if(m_fCurRot[1] > 360) m_fCurRot[1] -= 360;
	else if(m_fCurRot[1] < -360) m_fCurRot[1] += 360;
}
//------------------------------------------------------------------
//	To calculate the current image ID base on user input
//------------------------------------------------------------------
void COpenGLWnd::CalcCurImageID()
{
	GetKeyboardState(g_KeyState);

	//根据用户的按键，调整当前要显示的图片序号，以及要删除和要加载的图片序号
	static bool bFrameIsChange = true;
	if(g_KeyState['W'] > 64 || g_KeyState['w'] > 64 || g_KeyState[VK_UP] > 64)
	{
		if( (m_fCurRot[1] >= 0 && m_fCurRot[1] <= 90) || (m_fCurRot[1] >= 270 && m_fCurRot[1] <= 360) ||
			(m_fCurRot[1] >= -90 && m_fCurRot[1] <= 0) || (m_fCurRot[1] >= -360 && m_fCurRot[1] <= -270))	m_iCurImageID--;		
		else m_iCurImageID++;

		/*为什么要去掉最后两帧呢???如果不去掉,当手动到最后一帧后不能后退,估计原因是
		当文件指针读取到文件结束处时自动关闭文件了,所以再向后移动文件指针时没有反应
		*/
		if(m_iCurImageID > m_iImageTotalCnt-2) m_iCurImageID = m_iImageTotalCnt-2;
		bFrameIsChange = true;
	}
	else if(g_KeyState['S'] > 64 || g_KeyState['s'] > 64 || g_KeyState[VK_DOWN] > 64)
	{
		if((m_fCurRot[1] >= 90 && m_fCurRot[1] <= 270) || (m_fCurRot[1] >= -270 && m_fCurRot[1] <= -90))	m_iCurImageID--;
		else m_iCurImageID++;

		if(m_iCurImageID < 0) m_iCurImageID = 0;
		bFrameIsChange = true;
	}
	else
	{
		if(bFrameIsChange)
		{
			memset(m_iVisible, 1, 6*sizeof(int));
			ChangeTexDataFromFileByID(m_iCurImageID-1, m_iCurImageID);
		}
		bFrameIsChange = false;
	}
}
//------------------------------------------------------------------



//------------------------------------------------------------------
/*  The following functions are called by interface functions  */
//------------------------------------------------------------------
//	Name:OpenFile
//	Introduce:To open the *.S3D file
//	Param:file:i:The *.S3D file name
//	Return:True if successful; otherwise false.
//------------------------------------------------------------------
BOOL COpenGLWnd::OpenFile(const char* file)
{
	m_fS3dFile.open(file, ios::in | ios::binary);
	if(!m_fS3dFile.is_open()) return false;

	//Read the file header of *.S3D
	VGSFileHeader header;
	m_fS3dFile.read(reinterpret_cast<char*>(&header), sizeof(VGSFileHeader));
	m_iTexWidth = header.iImageWidth;
	m_iTexHeight = header.iImageHeight;
	m_iImageTotalCnt = header.iImageCnt;

	//Read the images offet of every frame
	for(int iCnt = 0;iCnt < m_iImageTotalCnt;iCnt++)
	{
		LPFRAMEDATAOFFSET pOffset = new FrameDataOffset;
		m_fS3dFile.read(reinterpret_cast<char*>(pOffset), sizeof(FrameDataOffset));
		m_lstFramDataOffset.push_back(pOffset);
	}
	
	//Generate 6 empty texture objects
	unsigned char* ptmpImage = new unsigned char[m_iTexWidth*m_iTexHeight*3];
	memset(ptmpImage, 0, m_iTexWidth*m_iTexHeight*3);
	for(int tex = 0; tex < 6;tex++)
	{
		m_uTex[tex] = GenTexture(m_uTex[tex], m_iTexWidth, m_iTexHeight, GL_RGB, ptmpImage);
	}
	delete[] ptmpImage;
	ptmpImage = NULL;
	
	OnSize(0, 0);
	SetTimer(m_hWnd, 0, m_iTimerInter, NULL);

	return true;
}
//------------------------------------------------------------------
//	Name:SetPlayType
//	Intruduce:To set the type of player
//	Param:Type:i:0-auto, 1-by hand
//	Return:N/A
//------------------------------------------------------------------
void COpenGLWnd::SetPlayType(int iType)
{
	m_iType = iType;
}
//------------------------------------------------------------------
//	Name:SetPause
//	Intruduce:Pause or resume the player
//	Param:bPause:i:true-pause,false-resume
//	Return:N/A
//------------------------------------------------------------------
void COpenGLWnd::SetPause(bool bPause)
{
	m_bIsPause = bPause;

	if(m_bIsPause)
	{
		memset(m_iVisible, 1, 6*sizeof(int));
		ChangeTexDataFromFileByID(m_iCurImageID-1, m_iCurImageID);
	}
}
//------------------------------------------------------------------
//	Name:SetCameraInfo
//	Intruduce:To set the camera information
//	Param:ang:i:The camera field ang
//		  nearpos:i:The camera near position
//		  farpos:i:The camera far position
//	Return:N/A
//------------------------------------------------------------------
void COpenGLWnd::SetCameraInfo(float ang, float nearpos, float farpos)
{
	m_fCameraFieldAng = ang;
	m_fCameraNearPos = nearpos;
	m_fCameraFarPos = farpos;
	
	OnSize(0, 0);
}
//------------------------------------------------------------------
//	Name:SetTimerInter
//	Intruduce:To set the inter time of the timer
//	Param:inter:i:The inter time of the timer
//	Return:N/A
//------------------------------------------------------------------
void COpenGLWnd::SetTimerInter(int inter)
{
	m_iTimerInter = inter;
	KillTimer(m_hWnd, 0);
	SetTimer(m_hWnd, 0, m_iTimerInter, NULL);
}
//------------------------------------------------------------------