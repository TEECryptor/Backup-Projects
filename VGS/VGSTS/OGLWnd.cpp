//------------------------------------------------------------------
//	Name:OGLWnd.cpp
//	Introduce:The define file of OpenGL window class
//	Author:yaoyoufang
//	Date:05-7-2
//	Version:0.1v
//------------------------------------------------------------------
#include "stdafx.h"
#include <assert.h>
#include <math.h>
#include "OGLWnd.h"
//------------------------------------------------------------------
COGLWnd* COGLWnd::g_pTheFirstWnd = NULL;
//------------------------------------------------------------------
COGLWnd::COGLWnd(int id, HWND hWnd)
 : m_iWinWidth(1024), m_iWinHeight(768),
   m_fCameraNear(0.5f), m_fCameraFar(4.0f), m_fCameraField(64),
   m_fOverColor(0), m_fRatio(0), m_fPower(1),
   m_bRenderEnd(false), m_iArcHeight(0), m_pQuad2D(NULL),   
   m_iTexWidth(3096), m_iTexHeight(768), m_iAddTexWidth(0)
{
	m_iID = id;
	m_hHandWnd = hWnd;
}
//------------------------------------------------------------------
COGLWnd::~COGLWnd()
{
	if(m_pQuad2D)
	{
		delete []m_pQuad2D;
		m_pQuad2D = NULL;
	}

	ReleaseDC(m_hWnd, m_hdc);	
}
//------------------------------------------------------------------
//the function of create OpenGL window
//------------------------------------------------------------------
BOOL COGLWnd::Create(HINSTANCE hInstance, HWND hWndParent, RECT rc, int iQuadCnt)
{	
	memcpy(&m_stuCurRect, &rc, sizeof(RECT));
	m_iWinWidth = m_stuCurRect.right - m_stuCurRect.left;
	m_iWinHeight = m_stuCurRect.bottom - m_stuCurRect.top;

	//To calucate the quads 
	m_iQuadCnt = iQuadCnt <= 0 ? 20 : iQuadCnt;
	m_pQuad2D = new Quad2D[m_iQuadCnt];
	//CalucateQuads();	
	CalucateQuads2();

	return CGenericWnd::Create(hInstance, NULL, hWndParent, rc, WS_POPUP | WS_VISIBLE);
}
//------------------------------------------------------------------
//To calucate the quads, calucate the vertex and texture coord
//------------------------------------------------------------------
void COGLWnd::CalucateQuads2()
{	
	float fTmpR = (m_iWinWidth*m_iWinWidth/4.0f + m_iArcHeight*m_iArcHeight)/(2.0f * m_iArcHeight);	

	int N = 3*m_iQuadCnt/2;
	float fBaseDeltTex = (3.0f*m_iWinWidth/(3.0f*m_iWinWidth+2.0f*m_iAddTexWidth-2.0f*m_fRatio*m_iWinWidth))/(3.0f*m_iQuadCnt);
	float fDeltTex = (m_iAddTexWidth/(1.5f*m_iWinWidth+m_iAddTexWidth-m_fRatio*m_iWinWidth)) / (N*(N-1));

	int iDeltX = m_iWinWidth / m_iQuadCnt;

	
	int iTmp = 1;
	for(int i = 0;i < m_iQuadCnt;i++)
	{
		m_pQuad2D[i].iV[0][0] = i * iDeltX;
		m_pQuad2D[i].iV[1][0] = (i+1) * iDeltX;
		m_pQuad2D[i].iV[2][0] = (i+1) * iDeltX;
		m_pQuad2D[i].iV[3][0] = i * iDeltX;
		//
		m_pQuad2D[i].iV[0][1] = (int)CalcArcCoord_Y(m_iWinWidth/2.0f, -(fTmpR-m_iArcHeight), fTmpR, (float)m_pQuad2D[i].iV[0][0]);
		m_pQuad2D[i].iV[1][1] = (int)CalcArcCoord_Y(m_iWinWidth/2.0f, -(fTmpR-m_iArcHeight), fTmpR, (float)m_pQuad2D[i].iV[1][0]);
		m_pQuad2D[i].iV[2][1] = m_iWinHeight;
		m_pQuad2D[i].iV[3][1] = m_iWinHeight;

		m_pQuad2D[i].fT[0][1] = 0.0f;
		m_pQuad2D[i].fT[1][1] = 0.0f;
		m_pQuad2D[i].fT[2][1] = 1.0f;
		m_pQuad2D[i].fT[3][1] = 1.0f;

		//The first window
		if(m_iID == 0)
		{
			if(i == 0)
			{
				m_pQuad2D[i].fT[0][0] = 0 ;
				m_pQuad2D[i].fT[3][0] = m_pQuad2D[i].fT[0][0];
			}
			else
			{
				m_pQuad2D[i].fT[0][0] = m_pQuad2D[i-1].fT[1][0];
				m_pQuad2D[i].fT[3][0] = m_pQuad2D[i].fT[0][0];
			}
			m_pQuad2D[i].fT[1][0] = m_pQuad2D[i].fT[0][0] + fBaseDeltTex + (N-iTmp)*fDeltTex;
			m_pQuad2D[i].fT[2][0] = m_pQuad2D[i].fT[1][0];
			iTmp++;
		}
	}

	//The seccod window
	if(m_iID == 1)
	{
		iTmp = 0;
		for(int i = m_iQuadCnt/2-1;i >= 0;i--)
		{
			if(i == m_iQuadCnt/2 - 1)
			{
				m_pQuad2D[i].fT[1][0] = 0.5f ;
				m_pQuad2D[i].fT[2][0] = m_pQuad2D[i].fT[1][0];				
			}
			else
			{
				m_pQuad2D[i].fT[1][0] = m_pQuad2D[i+1].fT[0][0];
				m_pQuad2D[i].fT[2][0] = m_pQuad2D[i].fT[1][0];	
			}
			m_pQuad2D[i].fT[0][0] = m_pQuad2D[i].fT[1][0] - fBaseDeltTex - iTmp*fDeltTex;
			m_pQuad2D[i].fT[3][0] = m_pQuad2D[i].fT[0][0];
			iTmp++;
		}

		iTmp = 0;
		for(i = m_iQuadCnt/2;i < m_iQuadCnt;i++)
		{
			if(i == m_iQuadCnt/2)
			{
				m_pQuad2D[i].fT[0][0] = 0.5f ;
				m_pQuad2D[i].fT[3][0] = m_pQuad2D[i].fT[0][0];				
			}
			else
			{
				m_pQuad2D[i].fT[0][0] = m_pQuad2D[i-1].fT[1][0];
				m_pQuad2D[i].fT[3][0] = m_pQuad2D[i].fT[0][0];	
			}
			m_pQuad2D[i].fT[1][0] = m_pQuad2D[i].fT[0][0] + fBaseDeltTex + iTmp*fDeltTex;
			m_pQuad2D[i].fT[2][0] = m_pQuad2D[i].fT[1][0];
			iTmp++;
		}
	}
	//The thirds window
	else if(m_iID == 2)
	{
		int iTmp = 1;
		for(int i = m_iQuadCnt-1;i >= 0;i--)
		{
			if(i == m_iQuadCnt-1)
			{
				m_pQuad2D[i].fT[1][0] = 1.0f ;
				m_pQuad2D[i].fT[2][0] = m_pQuad2D[i].fT[1][0];	
			}
			else
			{
				m_pQuad2D[i].fT[1][0] = m_pQuad2D[i+1].fT[0][0];
				m_pQuad2D[i].fT[2][0] = m_pQuad2D[i].fT[1][0];	
			}
			m_pQuad2D[i].fT[0][0] = m_pQuad2D[i].fT[1][0] - fBaseDeltTex - (N - iTmp)*fDeltTex;
			m_pQuad2D[i].fT[3][0] = m_pQuad2D[i].fT[0][0];
			iTmp++;
		}
	}
}
//------------------------------------------------------------------
//To calucate the quads, calucate the vertex and texture coord
//------------------------------------------------------------------
void COGLWnd::CalucateQuads()
{
	float fTmpR = (m_iWinWidth*m_iWinWidth/4.0f + m_iArcHeight*m_iArcHeight)/(2.0f * m_iArcHeight);

	int iDeltX = m_iWinWidth / m_iQuadCnt;
	float fTexCoord_X0, fTexCoord_X1;
	switch(m_iID)
	{
	case 0:
		fTexCoord_X0 = 0.0f;
		fTexCoord_X1 = 1.0f/(3.0f - 2*m_fRatio);
		break;
	case 1:
		fTexCoord_X0 = (1.0f - m_fRatio) / (3.0f - 2*m_fRatio);
		fTexCoord_X1 = (2.0f - m_fRatio) / (3.0f - 2*m_fRatio);
		break;
	case 2:
		fTexCoord_X0 = (2.0f - 2*m_fRatio) / (3.0f - 2*m_fRatio);
		fTexCoord_X1 = 1.0f;
		break;
	}

	for(int i = 0;i < m_iQuadCnt;i++)
	{
		m_pQuad2D[i].iV[0][0] = i * iDeltX;
		m_pQuad2D[i].iV[1][0] = (i+1) * iDeltX;
		m_pQuad2D[i].iV[2][0] = (i+1) * iDeltX;
		m_pQuad2D[i].iV[3][0] = i * iDeltX;
		//
		m_pQuad2D[i].iV[0][1] = (int)CalcArcCoord_Y(m_iWinWidth/2.0f, -(fTmpR-m_iArcHeight), fTmpR, (float)m_pQuad2D[i].iV[0][0]);
		m_pQuad2D[i].iV[1][1] = (int)CalcArcCoord_Y(m_iWinWidth/2.0f, -(fTmpR-m_iArcHeight), fTmpR, (float)m_pQuad2D[i].iV[1][0]);
		m_pQuad2D[i].iV[2][1] = m_iWinHeight;
		m_pQuad2D[i].iV[3][1] = m_iWinHeight;

		m_pQuad2D[i].fT[0][0] = fTexCoord_X0 + i*(fTexCoord_X1-fTexCoord_X0)/m_iQuadCnt;
		m_pQuad2D[i].fT[0][1] = 0.0f;
		m_pQuad2D[i].fT[1][0] = fTexCoord_X0 + (i+1)*(fTexCoord_X1-fTexCoord_X0)/m_iQuadCnt;
		m_pQuad2D[i].fT[1][1] = 0.0f;
		m_pQuad2D[i].fT[2][0] = fTexCoord_X0 + (i+1)*(fTexCoord_X1-fTexCoord_X0)/m_iQuadCnt;
		m_pQuad2D[i].fT[2][1] = 1.0f;
		m_pQuad2D[i].fT[3][0] = fTexCoord_X0 + i*(fTexCoord_X1-fTexCoord_X0)/m_iQuadCnt;
		m_pQuad2D[i].fT[3][1] = 1.0f;
	}
}
//------------------------------------------------------------------
//To calucate the y coord on a ACR
//------------------------------------------------------------------
float COGLWnd::CalcArcCoord_Y(float fCenterX, float fCenterY, float fR, float x)
{
	float fTmp = fR*fR - (x-fCenterX)*(x-fCenterX);
	if(fTmp <= 0) 
		return 0.0f;
	else
		return fCenterY + sqrtf(fTmp);
}
//------------------------------------------------------------------
//To draw the quads
//------------------------------------------------------------------
void COGLWnd::DrawQuads(GLuint uTex)
{	
	glBindTexture(GL_TEXTURE_2D, uTex);
	for(int i = 0;i < m_iQuadCnt;i++)
	{
		glBegin(GL_QUADS);
		for(int j = 0;j < 4;j++)
		{
			glTexCoord2f(m_pQuad2D[i].fT[j][0], m_pQuad2D[i].fT[j][1]);
			glVertex2i(m_pQuad2D[i].iV[j][0], m_pQuad2D[i].iV[j][1]);
		}
		glEnd();
	}
}
//------------------------------------------------------------------
//the procedure function of window
//------------------------------------------------------------------
LRESULT COGLWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_MY_CREATE:
		OnCreate();
		break;
	case WM_SIZE:
		OnSize();
		break;
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE) SendMessage(m_hHandWnd, WM_QUIT, 0, 0);//exit(0);
		break;
	case WM_QUIT:
		exit(0);
		break;
	default:
		return DefWindowProc(m_hWnd, msg, wParam, lParam);
	}

	return 0;
}
//------------------------------------------------------------------
//the procedure function of WM_MY_CREATE message
//------------------------------------------------------------------
void COGLWnd::OnCreate()
{
	//Init OpenGL 
	OpenGLInit();
	//
	SetupRender();
	//
	OnSize();
}
//------------------------------------------------------------------
//the procedure function of WM_SIZE message
//------------------------------------------------------------------
void COGLWnd::OnSize()
{
	GetClientRect(m_hWnd, &m_stuCurRect);
	m_iWinWidth = m_stuCurRect.right - m_stuCurRect.left;
	m_iWinHeight = m_stuCurRect.bottom - m_stuCurRect.top;
}
//------------------------------------------------------------------
//	Set the OpenGL view port type
//------------------------------------------------------------------
void COGLWnd::SetViewport(int iViewportType, int iWidth, int iHeight)
{
	glViewport(0, 0, iWidth, iHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(iViewportType == 0)
		gluPerspective(m_fCameraField, (float)iWidth/iHeight, m_fCameraNear, m_fCameraFar);
	else		
		gluOrtho2D(0, iWidth, 0, iHeight);
	glMatrixMode(GL_MODELVIEW);
}
//------------------------------------------------------------------


//------------------------------------------------------------------
//Initalize OpenGL 
//------------------------------------------------------------------
void COGLWnd::OpenGLInit()
{	
	PIXELFORMATDESCRIPTOR pfd;
	int n;

	m_hdc = GetDC(m_hWnd);

	assert(m_hdc != NULL);						
	
	if(!bSetPixelFormat(m_hdc))					
		return;

	n = ::GetPixelFormat(m_hdc);					
												
	::DescribePixelFormat(m_hdc, n, sizeof(pfd), &pfd);
												
	m_hrc = wglCreateContext(m_hdc);	
	
	if(g_pTheFirstWnd == NULL)
		g_pTheFirstWnd = this;
	else
		wglShareLists(g_pTheFirstWnd->m_hrc, m_hrc);

	wglMakeCurrent(m_hdc, m_hrc);
}
//------------------------------------------------------------------
//Set the params of scene
//------------------------------------------------------------------
void COGLWnd::SetupRender()
{   
    //Alpha测试
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5f);
    //深度测试
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //2D纹理
    glEnable(GL_TEXTURE_2D);
    //绘制双单面
	glDisable(GL_CULL_FACE);
    //清空颜色
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
//------------------------------------------------------------------
//Set the format of pixels
//------------------------------------------------------------------
BOOL COGLWnd::bSetPixelFormat(HDC hdc)
{	
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),      //结构体的大小
		1,                                  //版本，必须为1
		PFD_DRAW_TO_WINDOW|                 //像素格式  绘制到窗体上
		PFD_SUPPORT_OPENGL|                 //          支持OpenGl
		PFD_DOUBLEBUFFER,                   //          采用双缓冲
		PFD_TYPE_RGBA,                      //为RGBA类型
		16,                                 //所需要的颜色索引位
		0,0,0,0,0,0,                        //RBG颜色的Bits和Shift值
		0,                                  //没有透明度alpha
		0,                                  //忽略透明度的变化
		0,                                  //没有累积索引位
		0,0,0,0,                            //忽略RGBA的累积
		32,                                 //深度缓冲区位数
		8,                                  //模板缓冲区位数
		0,                                  //无辅助缓冲区位数
		PFD_MAIN_PLANE,                     //层次类型  主层次
		0,                                  //保留变量
		0,0,0                               //忽略层次标记
	};

	int pixelFormat;                        //像素格式索引号
                                            //选取适当的像素格式，并获取该格式的索引号
	if((pixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		assert(FALSE);
		return FALSE;
	}
                                           //设置像素格式
	if(SetPixelFormat(hdc, pixelFormat, &pfd) == FALSE)
	{
		assert(FALSE);
		return FALSE;
	}

	return TRUE;
}
//------------------------------------------------------------------
void COGLWnd::Draw(GLuint uTex)
{	
	wglMakeCurrent(m_hdc, m_hrc);	
	SetViewport(1, m_iWinWidth, m_iWinHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	//To draw the total texture on window
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	DrawQuads(uTex);

	DrawOverFace(m_iID);

	m_bRenderEnd = true;
}
//------------------------------------------------------------------
/* 绘制覆盖在上面的平面-1号窗体 */
//------------------------------------------------------------------
void COGLWnd::DrawOverFace(int ind)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_SRC_ALPHA);

	//To set the Ortho2D
//	SetViewport(1, m_iWinWidth, m_iWinHeight);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	switch(ind)
	{
	case 0:
		DrawOverFace_L();
		break;
	case 1:
		DrawOverFace_M();
		break;
	case 2:
		DrawOverFace_R();
		break;
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}//------------------------------------------------------------------
//	To draw the over rect on the LEFT window
//------------------------------------------------------------------
void COGLWnd::DrawOverFace_L()
{	
	int iOverPnt_X = (int)(m_iWinWidth * m_fRatio);
	glColor4f(m_fOverColor, m_fOverColor, m_fOverColor, 1.0f - m_fOverColor);
	glBegin(GL_TRIANGLES);
		glVertex2i(0, m_iWinHeight); 
		glVertex2i(0, 0);
		glVertex2i(m_iWinWidth - iOverPnt_X, 0);
		//
		glVertex2i(0, m_iWinHeight); 
		glVertex2i(m_iWinWidth - iOverPnt_X, 0);
		glVertex2i(m_iWinWidth - iOverPnt_X, m_iWinHeight);
	glEnd();

	for(int iCnt = 0; iCnt < iOverPnt_X; iCnt++)
	{
		float fAlpha = (float)pow(1.0f - (float)iCnt / iOverPnt_X, m_fPower) - ((float)iCnt / iOverPnt_X)*((float)iCnt / iOverPnt_X);
		glBegin(GL_TRIANGLES);
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(m_iWinWidth - (iOverPnt_X - iCnt), m_iWinHeight); 
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(m_iWinWidth - (iOverPnt_X - iCnt), 0);
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(m_iWinWidth - (iOverPnt_X - iCnt) + 1, 0);
			//
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(m_iWinWidth - (iOverPnt_X - iCnt), m_iWinHeight); 
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(m_iWinWidth - (iOverPnt_X - iCnt) + 1, 0);
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(m_iWinWidth - (iOverPnt_X - iCnt) + 1, m_iWinHeight);
		glEnd();
	}
}
//------------------------------------------------------------------
//	To draw the over rect on the MINDDLE window
//------------------------------------------------------------------
void COGLWnd::DrawOverFace_M()
{
	int iOverPnt_X = (int)(m_iWinWidth * m_fRatio);
	//Between the LEFT window
	for(int iCnt = 0; iCnt < iOverPnt_X; iCnt++)
	{
		float fAlpha = (float)pow((float)iCnt / iOverPnt_X, m_fPower) - (1.0f- (float)iCnt / iOverPnt_X)*(1.0f -(float)iCnt / iOverPnt_X)*(1.0f -(float)iCnt / iOverPnt_X) ;
		glBegin(GL_TRIANGLES);
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(iCnt, m_iWinHeight); 
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(iCnt, 0);
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(iCnt + 1, 0);
			//
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(iCnt, m_iWinHeight); 
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(iCnt + 1, 0);
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(iCnt + 1, m_iWinHeight);
		glEnd();
	}

	glColor4f(m_fOverColor, m_fOverColor, m_fOverColor, 1.0f - m_fOverColor);
	glBegin(GL_TRIANGLES);
		glVertex2i(iOverPnt_X, m_iWinHeight); 
		glVertex2i(iOverPnt_X, 0);
		glVertex2i(m_iWinWidth - iOverPnt_X, 0);
		//
		glVertex2i(iOverPnt_X, m_iWinHeight); 
		glVertex2i(m_iWinWidth - iOverPnt_X, 0);
		glVertex2i(m_iWinWidth - iOverPnt_X, m_iWinHeight);
	glEnd();

	//Between the RIGHT window
	for(iCnt = 0; iCnt < iOverPnt_X; iCnt++)
	{
		float fAlpha = (float)pow(1.0f - (float)iCnt / iOverPnt_X, m_fPower) - ((float)iCnt / iOverPnt_X)*((float)iCnt / iOverPnt_X)*((float)iCnt / iOverPnt_X);
		glBegin(GL_TRIANGLES);
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(m_iWinWidth - (iOverPnt_X-iCnt), m_iWinHeight); 
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(m_iWinWidth - (iOverPnt_X-iCnt), 0);
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(m_iWinWidth - (iOverPnt_X-iCnt) + 1, 0);
			//
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(m_iWinWidth - (iOverPnt_X-iCnt), m_iWinHeight); 
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(m_iWinWidth - (iOverPnt_X-iCnt) + 1, 0);
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(m_iWinWidth - (iOverPnt_X-iCnt) + 1, m_iWinHeight);
		glEnd();
	}
}
//------------------------------------------------------------------
//	To draw the over rect on the RIGHT window
//------------------------------------------------------------------
void COGLWnd::DrawOverFace_R()
{	
	int iOverPnt_X = (int)(m_iWinWidth * m_fRatio);
	glColor4f(m_fOverColor, m_fOverColor, m_fOverColor, 1.0f - m_fOverColor);
	glBegin(GL_TRIANGLES);
		glVertex2i(iOverPnt_X, m_iWinHeight); 
		glVertex2i(iOverPnt_X, 0);
		glVertex2i(m_iWinWidth, 0);
		//
		glVertex2i(iOverPnt_X, m_iWinHeight); 
		glVertex2i(m_iWinWidth, 0);
		glVertex2i(m_iWinWidth, m_iWinHeight);
	glEnd();

	for(int iCnt = 0; iCnt < iOverPnt_X; iCnt++)
	{
		float fAlpha = (float)pow((float)iCnt / iOverPnt_X, m_fPower) - (1.0f- (float)iCnt / iOverPnt_X)*(1.0f -(float)iCnt / iOverPnt_X)*(1.0f -(float)iCnt / iOverPnt_X);
		glBegin(GL_TRIANGLES);
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(iCnt, m_iWinHeight); 
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(iCnt, 0);
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(iCnt + 1, 0);
			//
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(iCnt, m_iWinHeight); 
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(iCnt + 1, 0);
			glColor4f(0, 0, 0, fAlpha);
			glVertex2i(iCnt + 1, m_iWinHeight);
		glEnd();
	}
}
//------------------------------------------------------------------
/* Swap*/
//------------------------------------------------------------------
void COGLWnd::Swap()
{
	wglMakeCurrent(m_hdc, m_hrc);
	SwapBuffers(wglGetCurrentDC());
	m_bRenderEnd = false;
}
//------------------------------------------------------------------
//	To set the multi-screen params
//------------------------------------------------------------------
void  COGLWnd::SetWindowParam(float fColor, float fRatio, float fPower, int iArcHeight)
{
	//color
	m_fOverColor = fColor;

	//the power value
	m_fPower = fPower;

	if(m_iArcHeight != iArcHeight || m_fRatio != fRatio)
	{
		m_fRatio = fRatio;
		m_iArcHeight = iArcHeight;
		//CalucateQuads();	
		CalucateQuads2();
	}
}
//------------------------------------------------------------------
//	to set the camera param
//------------------------------------------------------------------
void COGLWnd::SetCameraParam(float nearpos, float farpos, float field)
{
	m_fCameraNear = nearpos;
	m_fCameraFar = farpos;
	m_fCameraField = field;

	OnSize();
}
//------------------------------------------------------------------
//	To set the texture param
//------------------------------------------------------------------
void COGLWnd::SetTotalTexParam(int iWidth, int iHeight, int iAddWidth)
{
	if(m_iTexWidth != iWidth ||	m_iTexHeight != iHeight || m_iAddTexWidth != iAddWidth)
	{
		m_iTexWidth = iWidth;
		m_iTexHeight = iHeight;
		m_iAddTexWidth = iAddWidth;

		//CalucateQuads();		
		CalucateQuads2();
	}
}
//------------------------------------------------------------------
