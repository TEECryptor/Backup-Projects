// VGSThreeScreen.cpp : Defines the entry point for the application.
//
//-------------------------------------------------
#include "stdafx.h"
#include "resource.h"
#include <stdio.h>
#include <vector>
#include "OGLWnd.h"
#include "JpgManager.h"
#include "Camera.h"
#include "Mp3Player.h"
#include "OGLExtension.h"
#include "zlib.h"
#include "BoxFace.h"
//-------------------------------------------------
using namespace std;
//-------------------------------------------------
#define MAX_LOADSTRING 100
//-------------------------------------------------
// Global Variables:
HINSTANCE hInst;									// current instance
TCHAR szTitle[MAX_LOADSTRING];						// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];				// The title bar text
//-------------------------------------------------
// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
//-------------------------------------------------
extern PFNWGLCHOOSEPIXELFORMATARBPROC  wglChoosePixelFormatARB;
extern PFNWGLCREATEPBUFFERARBPROC    wglCreatePbufferARB;
extern PFNWGLGETPBUFFERDCARBPROC     wglGetPbufferDCARB;
extern PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
extern PFNWGLDESTROYPBUFFERARBPROC   wglDestroyPbufferARB;
extern PFNWGLQUERYPBUFFERARBPROC     wglQueryPbufferARB;
extern PFNWGLBINDTEXIMAGEARBPROC     wglBindTexImageARB;
extern PFNWGLRELEASETEXIMAGEARBPROC  wglReleaseTexImageARB;
//-------------------------------------------------
HWND				g_hWnd = NULL;
CBoxFace*			g_pBoxFace[IMAGE_CNT];
COGLWnd*			g_pOGLWnd[3];
CCamera				g_Camera;
CMp3Player			g_Mp3Player;
CMp3Player			g_FrontMusicPlayer;
char				g_strAppPath[MAX_PATH];
char				g_strIniFile[MAX_PATH];	
char				g_strBKMusicFile[MAX_PATH];			
HANDLE				g_hVGSFile = NULL;
char				g_strVGSFile[MAX_PATH];				//Be used When g_iSceneFile = 0
vector<LPFRAMEDATAOFFSET>	g_lstFramDataOffset;		//Be used When g_iSceneFile = 0
//-------------------------------------------------
const float			g_HalfWidth = 1.0f;
//-------------------------------------------------
int					g_iQuadCnt = 20;				//The Quads count on one OpenGL window
int					g_iWinWidth = 0;
int					g_iWinHeight = 0;
//
float				g_fCameraNear = 0.5f;
float				g_fCameraFar  = 4.0f;
float				g_fCameraField= 60.0f;
//
float				g_fColor		= 1.0f;
float				g_fRatio		= 0;
float				g_fPower		= 1.0f;
int					g_iArcHeight	= 0;		//The arc height
//
int					g_iDataFmt		= 0;		//The data format, 0:jpg,1:zlib,2:BMP
int					g_iImageCnt		= 0;
int					g_iImageWidth	= 1;
int					g_iImageHeight	= 1; 
int					g_iImageSize	= 0;
int					g_iDivType		= DIS4_4;
int					g_iSmallImageW	= 1;
int					g_iSmallImageH	= 1;
int					g_iSmallImageS	= 0;
//-------------------------------------------------
unsigned char		g_uKeyState[256];
int					g_iPlayType	   = 0;
int					g_iCurImageID  = 0;
int					g_iLastImageID = -1;
float				g_fCurCameraRot[2];
BYTE*				g_pSrcDataBuff = NULL;//new BYTE[DATABUFF_SIZE];
BYTE*				g_pDesDataBuff = NULL;//new BYTE[DATABUFF_SIZE];
//-------------------------------------------------
//	MusicInfo
//-------------------------------------------------
typedef struct tagMusicInfo
{
	int		iStartFrm;
	char	szFileName[MAX_PATH];
}MusicInfo, *LPMUSICINFO;
//
vector<LPMUSICINFO> g_lstMusicInfo;
//-------------------------------------------------
//To read the ini file
void	ReadSysParam(char* file);
//To write the ini file
void	WriteSysParam(char* file);
//To create the texture objects
void	CreateBoxTextureObjects();
//To open the VGS file
bool	OpenVGSFile(char* file);
//The WM_TIMER message process function
void	OnTimer();
//The render function
void	RenderScene();
//To change the textures data
void	ChangeTextureImage();
//Load the JPG format data and to decode
int		LoadJpgImageFromBuff(BYTE* srcdata, int size, BYTE*& data);
//To cheek the visible of 6 faces
void	CheckFaceIsVisible();
//To process the key state change
void	ProcKeyStateChange();
//To calculate the current camera rotate
void	CalcCurCameraRot();
//When play by hand, to calculate the new image id
void	CalcCurImageID();
void	OnDestroy();
//-------------------------------------------------
void		InitOpenGL();
HPBUFFERARB CreatePBuffer(HDC dc, GLuint iWidth, GLuint iHeight);
void		CreateTotalTex();
void		SetViewport(int iViewportType, int iWidth, int iHeight);
void		RenderToTexture();
//-------------------------------------------------
HDC			g_hMemDC = NULL;
HGLRC		g_hMemRc = NULL;
HPBUFFERARB	g_pPBuffer = NULL;
GLuint		g_uBoxTex[IMAGE_CNT];
int			g_iTotalTexW = 2048;
int			g_iTotalTexH = 1024;
int			g_iAddTexW = 0;
GLuint		g_uTotalTex = 0;
//-------------------------------------------------
__int64 myFileSeek (HANDLE hf, __int64 distance, DWORD MoveMethod)
{
   LARGE_INTEGER li;

   li.QuadPart = distance;

   li.LowPart = SetFilePointer (hf, li.LowPart, &li.HighPart, MoveMethod);

   if (li.LowPart == 0xFFFFFFFF && GetLastError() != NO_ERROR)
   {
      li.QuadPart = -1;
   }

   return li.QuadPart;
}
//-------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_VGSTHREESCREEN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_VGSTHREESCREEN);

	//To init to the values
	memset(g_fCurCameraRot, 0x00, sizeof(float)*2);

	//To get the path string of the Client.exe file
	GetModuleFileName(NULL, g_strAppPath, MAX_PATH);
	char* p = g_strAppPath;
	while(*p != '\0') p++;
	while(*p != '\\') {*p = '\0'; p--;}
	strcpy(g_strVGSFile, g_strAppPath);	
	strcpy(g_strBKMusicFile, g_strAppPath);	
	
	//The ini file name
	strcpy(g_strIniFile, g_strAppPath);
	strcat(g_strIniFile, "Config.ini");
	ReadSysParam(g_strIniFile);

	RECT rect[3];
	rect[0].left = 0;
	rect[0].right = g_iWinWidth;
	rect[0].top = 0;
	rect[0].bottom = g_iWinHeight;
	rect[1].left = rect[0].right;
	rect[1].right = rect[0].right + g_iWinWidth;
	rect[1].top = 0;
	rect[1].bottom = g_iWinHeight;
	rect[2].left = rect[1].right;
	rect[2].right = rect[1].right + g_iWinWidth;
	rect[2].top = 0;
	rect[2].bottom = g_iWinHeight;
	for(int i = 0;i < 3;i++)
	{
		//创建OGL窗体
		g_pOGLWnd[i] = new COGLWnd(i, g_hWnd);
		if(!g_pOGLWnd[i]->Create(hInstance, NULL, rect[i], g_iQuadCnt)) return false;
	
		g_pOGLWnd[i]->SetCameraParam(g_fCameraNear, g_fCameraFar, g_fCameraField);
		g_pOGLWnd[i]->SetTotalTexParam(g_iTotalTexW, g_iTotalTexH, g_iAddTexW);
		g_pOGLWnd[i]->SetWindowParam(g_fColor, g_fRatio, g_fPower, g_iArcHeight);
		ShowWindow(g_pOGLWnd[i]->m_hWnd, SW_SHOW); 
		UpdateWindow(g_pOGLWnd[i]->m_hWnd); 
	}

	//Added by Youfang Yao
	InitOGLExtension();
	InitOpenGL();
	CreateTotalTex();
	//End add

	//To open the *.VGS file 
	if(!OpenVGSFile(g_strVGSFile))
	{
		char tmp[MAX_PATH];
		sprintf(tmp, "Open file %s error!", g_strVGSFile);
		MessageBox(NULL, tmp, "Error", MB_OK);
		PostQuitMessage(0);
	}
	else
	{
		//New the space
		if(g_pSrcDataBuff) delete []g_pSrcDataBuff;
		g_pSrcDataBuff = new BYTE[g_iSmallImageS];
		if(g_pDesDataBuff) delete []g_pDesDataBuff;
		g_pDesDataBuff = new BYTE[g_iSmallImageS];

		//To create the box face objects
		for(int iFace = IMAGE_F;iFace < IMAGE_CNT;iFace++)
		{
			g_pBoxFace[iFace] = new CBoxFace;
			g_pBoxFace[iFace]->Create(iFace, g_iDivType, 2*g_HalfWidth);
		}

		//To create the texture objects;
		CreateBoxTextureObjects();
		//To start render		
		::SetTimer(g_hWnd, 0, 20, NULL);

		//To play the back ground music
		if(strcmp(g_strBKMusicFile, g_strAppPath) != 0)
			g_Mp3Player.PlayMusic(g_strBKMusicFile, true);
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}
//-------------------------------------------------
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
//-------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_VGSTHREESCREEN);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_VGSTHREESCREEN;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}
//-------------------------------------------------
//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
//-------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }

//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);

   return TRUE;
}
//-------------------------------------------------
//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//-------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_TIMER:
			OnTimer();
			break;
		case WM_QUIT:
			PostQuitMessage(0);
		case WM_DESTROY: 
			//OnDestroy();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}
//-------------------------------------------------
// Mesage handler for about box.
//-------------------------------------------------
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
//-------------------------------------------------


//-------------------------------------------------
//	To read the ini file
//-------------------------------------------------
void ReadSysParam(char* file)
{	
	char tmp[64]= {0};

	GetPrivateProfileString("SceneFile", "File", "", tmp, 64, file);
	strcat(g_strVGSFile, tmp);

	//The bk music
	GetPrivateProfileString("Musics", "BKMusic", "", tmp, 64, file);
	strcat(g_strBKMusicFile, tmp);	
	
	//The introduce musics
	int iMusicCnt = GetPrivateProfileInt("Musics", "Count", 0, file);
	for(int iCnt = 0; iCnt < iMusicCnt;iCnt++)
	{
		LPMUSICINFO pMusicInfo = new MusicInfo;
		strcpy(pMusicInfo->szFileName, g_strAppPath);

		//The start frame
		char szStartFrm[32] = {0};
		sprintf(szStartFrm, "Start%d", iCnt);
		pMusicInfo->iStartFrm = GetPrivateProfileInt("Musics", szStartFrm, 0, file);

		//The music file name
		char szFileName[MAX_PATH] = {0};
		char szMusic[32] = {0};
		sprintf(szMusic, "Music%d", iCnt);
		GetPrivateProfileString("Musics", szMusic, "", szFileName, MAX_PATH, file);
		strcat(pMusicInfo->szFileName, szFileName);

		g_lstMusicInfo.push_back(pMusicInfo);
	}

	g_iQuadCnt = GetPrivateProfileInt("WindowParam", "QuadCnt", 20, file);
	g_iWinWidth = GetPrivateProfileInt("WindowParam", "Width", 1024, file);
	g_iWinHeight = GetPrivateProfileInt("WindowParam", "Height",768, file);
	//
	g_iTotalTexW = GetPrivateProfileInt("WindowParam", "TexWidth", 2048, file);
	g_iTotalTexH = GetPrivateProfileInt("WindowParam", "TexHeight",1024, file);
	g_iAddTexW = GetPrivateProfileInt("WindowParam", "AddWidth", 0, file);

	GetPrivateProfileString("CameraParam", "NearPos", "0.5", tmp, 64, file);
	g_fCameraNear = (float)atof(tmp);
	GetPrivateProfileString("CameraParam", "FarPos", "4", tmp, 64, file);
	g_fCameraFar = (float)atof(tmp);
	GetPrivateProfileString("CameraParam", "Field", "64", tmp, 64, file);
	g_fCameraField = (float)atof(tmp);

	GetPrivateProfileString("MultiParam", "Color", "0", tmp, 64, file);
	g_fColor = (float)atof(tmp);
	GetPrivateProfileString("MultiParam", "Ratio", "0", tmp, 64, file);
	g_fRatio = (float)atof(tmp);
	GetPrivateProfileString("MultiParam", "Power", "1", tmp, 64, file);
	g_fPower = (float)atof(tmp);
	g_iArcHeight = GetPrivateProfileInt("MultiParam", "ArcHeight", 0, file);
}
//-------------------------------------------------
//	To write the ini file
//-------------------------------------------------
void WriteSysParam(char* file)
{	
	char tmp[32]= {0};
	sprintf(tmp, "%f", g_fColor);
	WritePrivateProfileString("MultiParam", "Color", tmp, file);

	sprintf(tmp, "%f", g_fRatio);
	WritePrivateProfileString("MultiParam", "Ratio", tmp, file);

	sprintf(tmp, "%f", g_fPower);
	WritePrivateProfileString("MultiParam", "Power", tmp, file);

	sprintf(tmp, "%d", g_iArcHeight);
	WritePrivateProfileString("MultiParam", "ArcHeight", tmp, file);
}
//-------------------------------------------------
//	To create the texture objects
//-------------------------------------------------
void CreateBoxTextureObjects()
{	
	if(COGLWnd::g_pTheFirstWnd == NULL) return;
	wglMakeCurrent(g_hMemDC, g_hMemRc);					
	glEnable(GL_TEXTURE_2D);
		
	GLubyte *pTmpData = new GLubyte[g_iSmallImageS];
	memset(pTmpData, 0x00, g_iSmallImageS);
	for(int i = 0;i < IMAGE_CNT;i++)
	{
		if(g_pBoxFace[i])
		{
			for(int iCnt = 0;iCnt < g_pBoxFace[i]->m_iSmallCnt;iCnt++)
			{
				glGenTextures(1, &g_pBoxFace[i]->m_uTex[iCnt]);

				glBindTexture(GL_TEXTURE_2D, g_pBoxFace[i]->m_uTex[iCnt]);    
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, g_iSmallImageW, g_iSmallImageH, GL_BGR_EXT, GL_UNSIGNED_BYTE, pTmpData);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
		}
	}
	delete []pTmpData;
	pTmpData = NULL;

	wglMakeCurrent(NULL, NULL);
}
//-------------------------------------------------
//	To open the VGS file
//-------------------------------------------------
bool OpenVGSFile(char* file)
{
	g_hVGSFile = CreateFile(g_strVGSFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(g_hVGSFile == NULL)
	{
		MessageBox(NULL, "打开文件错误！","Error", MB_OK);
		return false;
	}

	DWORD dwReadSize = 0;

	//Read the file header of *.VGS
	VGSFileHeader header;
	ReadFile(g_hVGSFile, &header, sizeof(header), &dwReadSize, NULL);

	//To cheek the VGS file formate
	if(strcmp(header.strType, "JPG") != 0)
	{
		return false;
	}
	g_iImageWidth = header.iImageWidth;
	g_iImageHeight = header.iImageHeight;
	g_iImageCnt = header.iImageCnt;	
	g_iImageSize = g_iImageWidth * g_iImageHeight * 3;
	g_iDivType = header.iDivType;

	//To caluc the small images size
	g_iSmallImageW = g_iImageWidth/pow(2, g_iDivType);
	g_iSmallImageH = g_iImageHeight/pow(2, g_iDivType);
	g_iSmallImageS = 3 * g_iSmallImageW * g_iSmallImageH;

	//The offset information of frame data
	int iSize = sizeof(FrameDataOffset);
	__int64 offset = -1 * ((__int64)g_iImageCnt*sizeof(FrameDataOffset));
	myFileSeek(g_hVGSFile, offset, FILE_END);
	for(int iCnt = 0;iCnt < g_iImageCnt;iCnt++)
	{
		LPFRAMEDATAOFFSET pOffset = new FrameDataOffset;		
		ReadFile(g_hVGSFile, pOffset, sizeof(FrameDataOffset), &dwReadSize, NULL);
		g_lstFramDataOffset.push_back(pOffset);
	}

	//Set the file pointer to the first frame data start positoon	
	myFileSeek(g_hVGSFile, sizeof(header), FILE_BEGIN);

	return true;
}
//-------------------------------------------------
//	The timer function
//-------------------------------------------------
void OnTimer()
{
	//To play the music
	for(int iMusic = 0; iMusic < g_lstMusicInfo.size(); iMusic++)
	{
		if(g_lstMusicInfo[iMusic]->iStartFrm == g_iCurImageID)
		{
			g_FrontMusicPlayer.StopMusic();
			g_FrontMusicPlayer.PlayMusic(g_lstMusicInfo[iMusic]->szFileName, FALSE);
		}
	}

	//Render
	RenderScene();
}
//-------------------------------------------------
//	The render function
//-------------------------------------------------
void RenderScene()
{
	//To process the keyboard state change 
	ProcKeyStateChange();

	//To rotate the camera
	CalcCurCameraRot();

	//To cheek the visible of 6 faces
	CheckFaceIsVisible();

	//To change the texture object data to current image
	if(g_iCurImageID != g_iLastImageID)	
	{
		ChangeTextureImage();
		g_iLastImageID = g_iCurImageID;
	}

	//Get the total texture
	RenderToTexture();

	//To calculate the new image ID
	if(g_iPlayType == 0)
	{
		if(++g_iCurImageID >= g_iImageCnt - 1) g_iCurImageID = 0;
	}
	else
	{
		CalcCurImageID();
	}

	//Draw the total texture on OGL windows
	if(g_pOGLWnd[0] && g_pOGLWnd[1] && g_pOGLWnd[2])
	{
		//To draw
		g_pOGLWnd[0]->Draw(g_uTotalTex);
		g_pOGLWnd[1]->Draw(g_uTotalTex);
		g_pOGLWnd[2]->Draw(g_uTotalTex);

		//To swap
		g_pOGLWnd[0]->Swap();
		g_pOGLWnd[1]->Swap();
		g_pOGLWnd[2]->Swap();
	}
}
//-------------------------------------------------
void InitOpenGL()
{		
	g_pPBuffer = CreatePBuffer(g_pOGLWnd[0]->m_hdc, g_iTotalTexW+2*g_iAddTexW, g_iTotalTexH);
	if(g_pPBuffer == NULL) return;
	
	g_hMemDC = wglGetPbufferDCARB(g_pPBuffer);
	
	g_hMemRc = wglCreateContext(g_hMemDC);

	wglShareLists(g_pOGLWnd[0]->m_hrc, g_hMemRc);

	wglMakeCurrent(g_hMemDC, g_hMemRc);
}
//-------------------------------------------------
void CreateTotalTex()
{					
	wglMakeCurrent(g_pOGLWnd[0]->m_hdc, g_pOGLWnd[0]->m_hrc);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &g_uTotalTex);
	glBindTexture(GL_TEXTURE_2D, g_uTotalTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			
	wglMakeCurrent(g_hMemDC, g_hMemRc);
}
//-------------------------------------------------
HPBUFFERARB CreatePBuffer(HDC dc, GLuint iWidth, GLuint iHeight)
{	
	GLint   pixelFormats[1];
	GLuint  numFormat;
	GLint   iAttributes[40];
	GLfloat fattributes[40] = {0, 0};
	GLint   attributeIndex = 0;

	iAttributes[attributeIndex++] = WGL_DRAW_TO_PBUFFER_ARB;
	iAttributes[attributeIndex++] = GL_TRUE;
	iAttributes[attributeIndex++] = WGL_BIND_TO_TEXTURE_RGBA_ARB;
	iAttributes[attributeIndex++] = GL_TRUE;
	iAttributes[attributeIndex++] = WGL_SUPPORT_OPENGL_ARB;
	iAttributes[attributeIndex++] = GL_TRUE;
	iAttributes[attributeIndex++] = WGL_ACCELERATION_ARB;
	iAttributes[attributeIndex++] = WGL_FULL_ACCELERATION_ARB;
	iAttributes[attributeIndex++] = 0;
	iAttributes[attributeIndex++] = 0;

	if(!wglChoosePixelFormatARB) return NULL;

	if(!wglChoosePixelFormatARB(dc,
								iAttributes,
								fattributes,
								1,
								pixelFormats,
								&numFormat)
	   && numFormat <= 0)
	{
		//throw aeCreateAdapterFailed("无法选择PixelFormat。");
	}
	attributeIndex = 0;
	//RGBA
	iAttributes[attributeIndex++] = WGL_TEXTURE_FORMAT_ARB;
	iAttributes[attributeIndex++] = WGL_TEXTURE_RGBA_ARB;

	//
	iAttributes[attributeIndex++] = WGL_TEXTURE_TARGET_ARB;
	iAttributes[attributeIndex++] = WGL_TEXTURE_2D_ARB;
	//Mipmap
	iAttributes[attributeIndex++] = WGL_MIPMAP_TEXTURE_ARB;
	iAttributes[attributeIndex++] = 0;
	//
	iAttributes[attributeIndex++] = WGL_PBUFFER_LARGEST_ARB;
	iAttributes[attributeIndex++] = 1;

	iAttributes[attributeIndex++] = 0;
	iAttributes[attributeIndex++] = 0;
	//Pbuffer
	HPBUFFERARB pbuffer = wglCreatePbufferARB(dc,
									 pixelFormats[0],
									 iWidth, iHeight,
									 iAttributes);
	if(NULL != pbuffer)
	{
		GLint var;
		wglQueryPbufferARB(pbuffer, WGL_PBUFFER_WIDTH_ARB,  &var);
		iWidth = var;
		wglQueryPbufferARB(pbuffer, WGL_PBUFFER_HEIGHT_ARB, &var);
		iHeight = var;
	}
	return pbuffer;
}
//-------------------------------------------------
void SetViewport(int iViewportType, int iWidth, int iHeight)
{
	glViewport(0, 0, iWidth, iHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(iViewportType == 0)
		gluPerspective(g_fCameraField, (float)(iWidth-2.0f*g_iAddTexW)/iHeight, g_fCameraNear, g_fCameraFar);
	else		
		gluOrtho2D(0, iWidth, 0, iHeight);
	glMatrixMode(GL_MODELVIEW);
}
//-------------------------------------------------
void RenderToTexture()
{	
	//For render to buffer bitmap
	wglMakeCurrent(g_pOGLWnd[0]->m_hdc, g_pOGLWnd[0]->m_hrc);
	glBindTexture(GL_TEXTURE_2D, g_uTotalTex);
	wglReleaseTexImageARB(g_pPBuffer, WGL_FRONT_LEFT_ARB);
	wglMakeCurrent(g_hMemDC, g_hMemRc);

	SetViewport(0, g_iTotalTexW+2*g_iAddTexW, g_iTotalTexH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();		

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();
		//The new camera rotate 
		glRotatef(g_fCurCameraRot[0], 1, 0, 0);
		glRotatef(g_fCurCameraRot[1], 0, 1, 0);

		for(int iFace = IMAGE_F;iFace < IMAGE_CNT;iFace++)
			g_pBoxFace[iFace]->Draw();

	glPopMatrix();

	//For render to buffer bitmap
	wglMakeCurrent(g_pOGLWnd[0]->m_hdc, g_pOGLWnd[0]->m_hrc);
	wglBindTexImageARB(g_pPBuffer, WGL_FRONT_LEFT_ARB);
}
//-------------------------------------------------
//	To chang the texture image data
//-------------------------------------------------
void ChangeTextureImage()
{
	DWORD dwReadSize = 0;

	//The frame id
	int iImageID = 0;
	ReadFile(g_hVGSFile, &iImageID, sizeof(int), &dwReadSize, NULL);	
	
	if(iImageID != g_iCurImageID)
	{
		if(g_iCurImageID < 0 || g_iCurImageID > g_lstFramDataOffset.size()-1) return;
		else
		{
			__int64 dwOffSet = g_lstFramDataOffset[g_iCurImageID]->bOffset;
			myFileSeek(g_hVGSFile, dwOffSet, FILE_BEGIN);
			ReadFile(g_hVGSFile, &iImageID, sizeof(int), &dwReadSize, NULL);	

			if(iImageID != g_iCurImageID) 
			{
				return;
			}
		}
	}

	if(iImageID >= 0 && iImageID < g_iImageCnt)
	{
		for(int i = 0;i < IMAGE_CNT;i++)
		{
			//The total size of this face
			int iFaceSize = 0;
			ReadFile(g_hVGSFile, &iFaceSize, sizeof(int), &dwReadSize, NULL);

			//Can see the face, to cheek the child faces
			if(g_pBoxFace[i]->m_stuVisible.iTotalVisible != 0)
			{
				for(int iCnt = 0;iCnt < g_pBoxFace[i]->m_iSmallCnt;iCnt++)
				{
					//The child image size
					int iChildSize = 0;
					ReadFile(g_hVGSFile, &iChildSize, sizeof(int), &dwReadSize, NULL);

					//The child rect can be see
					if(g_pBoxFace[i]->m_stuVisible.iChildVisible[iCnt] != 0)
					{	
						//Read the child image file data
						ReadFile(g_hVGSFile, g_pSrcDataBuff, iChildSize, &dwReadSize, NULL);
						
						//To decode the child image data from JPG to BMP
						if(LoadJpgImageFromBuff(g_pSrcDataBuff, iChildSize, g_pDesDataBuff))
						{
							glBindTexture(GL_TEXTURE_2D, g_pBoxFace[i]->m_uTex[iCnt]);  		
							glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g_iSmallImageW, g_iSmallImageH, GL_BGR_EXT, GL_UNSIGNED_BYTE, g_pDesDataBuff);							
						}
					}
					//The child rect can not be see
					else
					{
						myFileSeek(g_hVGSFile, iChildSize, FILE_CURRENT);
					}
				}
			}
			//Can not see the face
			else
			{
				myFileSeek(g_hVGSFile, iFaceSize, FILE_CURRENT);
			}
		}
	}
}
//-------------------------------------------------
//	Load the JPG format data and to decode
//-------------------------------------------------
int	LoadJpgImageFromBuff(BYTE* srcdata, int size, BYTE*& data)
{
	if(srcdata == NULL) return 0;

	CJpgManager jpgLoader;
	if(jpgLoader.jpgReadBuff(srcdata, size))
	{
		int width = jpgLoader.jpgGetImageParams(_JPGIMAGEWIDTH);
		int height = jpgLoader.jpgGetImageParams(_JPGIMAGEHEIGHT);
		BYTE* pData = jpgLoader.jpgGetImageData();
	
		for(int i = 0;i < height;i++)
			memcpy(&data[i*width*3], &pData[(height-i-1)*width*3], width*3);
		return 1;
	}
	return 0;
}
//-------------------------------------------------
//	To cheek the visible of 6 faces
//-------------------------------------------------
void CheckFaceIsVisible()
{
	wglMakeCurrent(g_hMemDC, g_hMemRc);

	//To set the current view matrix
	glLoadIdentity();
	glPushMatrix();
		//The new camera rotate 
		glRotatef(g_fCurCameraRot[0], 1, 0, 0);
		glRotatef(g_fCurCameraRot[1], 0, 1, 0);

		//To calculate the current camera view frustum
		g_Camera.CalculateViewFrustum();
		
		for(int iFace = IMAGE_F;iFace < IMAGE_CNT;iFace++)
		{
			if(g_pBoxFace[iFace])
			{
				float fMinX = g_pBoxFace[iFace]->m_fMinX;
				float fMaxX = g_pBoxFace[iFace]->m_fMaxX;
				float fMinY = g_pBoxFace[iFace]->m_fMinY;
				float fMaxY = g_pBoxFace[iFace]->m_fMaxY;
				float fMinZ = g_pBoxFace[iFace]->m_fMinZ;
				float fMaxZ = g_pBoxFace[iFace]->m_fMaxZ;
	
				//Cheek the whole image at first
				int iVisible = g_Camera.CuboidFrustumTest(fMinX, fMaxX, fMinY, fMaxY, fMinZ, fMaxZ);
				g_pBoxFace[iFace]->m_stuVisible.iTotalVisible = iVisible;

				//To cheek the child images
				if(iVisible != 0)
				{
					float fSmallMinX = 0;
					float fSmallMaxX = 0;
					float fSmallMinY = 0;
					float fSmallMaxY = 0;
					float fSmallMinZ = 0;
					float fSmallMaxZ = 0;
					
					for(int iCnt = 0;iCnt < g_pBoxFace[iFace]->m_iSmallCnt;iCnt++)
					{
						g_pBoxFace[iFace]->GetChildMinMax(iCnt, fSmallMinX, fSmallMaxX, fSmallMinY, fSmallMaxY, fSmallMinZ, fSmallMaxZ);
						g_pBoxFace[iFace]->m_stuVisible.iChildVisible[iCnt] = g_Camera.CuboidFrustumTest(fSmallMinX, fSmallMaxX, fSmallMinY, fSmallMaxY, fSmallMinZ, fSmallMaxZ);						
					}
				}
			}
		}	
	glPopMatrix();
}
//-------------------------------------------------
//	To process the key state change
//-------------------------------------------------
void ProcKeyStateChange()
{
	//Get the current state of keyboard
	GetKeyboardState(g_uKeyState);

	//To chang the play type
	if(g_uKeyState['C'] & 0x80 || g_uKeyState['C'] & 0x80)
	{
		if(g_iPlayType == 1) g_iPlayType = 0;
		else g_iPlayType = 1;
	}

	bool bMultiScreenParamChange = false;
	//To change the over rect color to up
	if(g_uKeyState['Q'] & 0x80 || g_uKeyState['q'] & 0x80)
	{
		g_fColor += 1.0f/256;
		if(g_fColor > 1.0f) g_fColor = 1.0f;
		bMultiScreenParamChange = true;
	}
	else if(g_uKeyState['A'] & 0x80 || g_uKeyState['a'] & 0x80)
	{
		g_fColor -= 1.0f/256;
		if(g_fColor < 0.0f) g_fColor = 0.0f;
		bMultiScreenParamChange = true;
	}

	//To change the ratio of over width on OpenGL window
	if(g_uKeyState['W'] & 0x80 || g_uKeyState['w'] & 0x80)
	{
		g_fRatio += 0.0005f;
		if(g_fRatio > 1.0f) g_fRatio = 1.0f;
		bMultiScreenParamChange = true;
	}
	else if(g_uKeyState['S'] & 0x80 || g_uKeyState['s'] & 0x80)
	{
		g_fRatio -= 0.0005f;
		if(g_fRatio < 0) g_fRatio = 0;
		bMultiScreenParamChange = true;
	}
	
	//To change the power value to up
	if(g_uKeyState['R'] & 0x80 || g_uKeyState['r'] & 0x80)
	{
		g_fPower *= 1.01f;
		bMultiScreenParamChange = true;
	}
	else if(g_uKeyState['F'] & 0x80 || g_uKeyState['f'] & 0x80)
	{
		g_fPower /= 1.01f;
		bMultiScreenParamChange = true;
	}	
	//To change the arc height to up
	if(g_uKeyState['U'] & 0x80 || g_uKeyState['u'] & 0x80)
	{
		g_iArcHeight += 1;
		if(g_iArcHeight > g_iWinHeight) g_iArcHeight = g_iWinHeight;
		bMultiScreenParamChange = true;
	}
	else if(g_uKeyState['J'] & 0x80 || g_uKeyState['j'] & 0x80)
	{
		g_iArcHeight -= 1;
		if(g_iArcHeight < 0) g_iArcHeight = 0;
		bMultiScreenParamChange = true;
	}

	//If the multi-screen params have been change, send the new params to OGL windons
	if(bMultiScreenParamChange)
	{
		if(g_pOGLWnd[0] && g_pOGLWnd[1] && g_pOGLWnd[2])
		{
			g_pOGLWnd[0]->SetWindowParam(g_fColor, g_fRatio, g_fPower, g_iArcHeight);
			g_pOGLWnd[1]->SetWindowParam(g_fColor, g_fRatio, g_fPower, g_iArcHeight);
			g_pOGLWnd[2]->SetWindowParam(g_fColor, g_fRatio, g_fPower, g_iArcHeight);
		}
	}
}
//-------------------------------------------------
//	To calculate the current camera rotate
//-------------------------------------------------
void CalcCurCameraRot()
{
	//Get the current state of keyboard
	GetKeyboardState(g_uKeyState);

	//The rotate in Y dirction
	if(g_uKeyState[VK_LEFT] & 0x80)
	{
		g_fCurCameraRot[1] -= 1;
	}
	else if(g_uKeyState[VK_RIGHT] & 0x80)
	{
		g_fCurCameraRot[1] += 1;
	}

	//The rotate in X dirction
	if(g_uKeyState['p'] & 0x80 || g_uKeyState['P'] & 0x80)
	{
		g_fCurCameraRot[0] -= 1;
	}
	else if(g_uKeyState['L'] & 0x80 || g_uKeyState['l'] & 0x80)
	{
		g_fCurCameraRot[0] += 1;
	}
	
	if(g_fCurCameraRot[0] > 90) g_fCurCameraRot[0] = 90;
	else if(g_fCurCameraRot[0] < -90) g_fCurCameraRot[0] = -90;

	if(g_fCurCameraRot[1] > 360) g_fCurCameraRot[1] -= 360;
	else if(g_fCurCameraRot[1] < -360) g_fCurCameraRot[1] += 360;
}
//-------------------------------------------------
//	When play by hand, to calculate the new image id
//-------------------------------------------------
void CalcCurImageID()
{	
	GetKeyboardState(g_uKeyState);

	//Chang the image ID base on user input
	static bool bFrameIsChange = true;
	if(g_uKeyState[VK_UP] & 0x80)
	{
		if( (g_fCurCameraRot[1] >= 0 && g_fCurCameraRot[1] <= 90) || (g_fCurCameraRot[1] >= 270 && g_fCurCameraRot[1] <= 360) ||
			(g_fCurCameraRot[1] >= -90 && g_fCurCameraRot[1] <= 0) || (g_fCurCameraRot[1] >= -360 && g_fCurCameraRot[1] <= -270))	g_iCurImageID--;		
		else g_iCurImageID++;

		/*
		I donnt know why must -2
		*/
		if(g_iCurImageID > g_iImageCnt-2) g_iCurImageID = g_iImageCnt-2;
		bFrameIsChange = true;
	}
	else if(g_uKeyState[VK_DOWN] & 0x80)
	{
		if((g_fCurCameraRot[1] >= 90 && g_fCurCameraRot[1] <= 270) || (g_fCurCameraRot[1] >= -270 && g_fCurCameraRot[1] <= -90))	g_iCurImageID--;
		else g_iCurImageID++;

		if(g_iCurImageID < 0) g_iCurImageID = 0;
		bFrameIsChange = true;
	}
	else
	{
		if(bFrameIsChange)
		{
			//To set all image can be see
			for(int iFace = IMAGE_F;iFace < IMAGE_CNT;iFace++)
			{
				if(g_pBoxFace[iFace])
				{		
					g_pBoxFace[iFace]->m_stuVisible.iTotalVisible = 1;						
					for(int iCnt = 0;iCnt < g_pBoxFace[iFace]->m_iSmallCnt;iCnt++)
					{
						g_pBoxFace[iFace]->m_stuVisible.iChildVisible[iCnt] = 1;
					}
				}
			}
			//Update the image data
			ChangeTextureImage();
		}
		bFrameIsChange = false;
	}
}
//-------------------------------------------------
//	The WM_DESTROY message process function
//-------------------------------------------------
void OnDestroy()
{	
	DestroyWindow(g_hWnd);

	wglReleasePbufferDCARB(g_pPBuffer, g_hMemDC);
	wglDestroyPbufferARB(g_pPBuffer);

	wglMakeCurrent(0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &g_uTotalTex);
	glDeleteTextures(6, g_uBoxTex);

	WriteSysParam(g_strIniFile);

	if(g_pSrcDataBuff)
	{
		delete []g_pSrcDataBuff;
		g_pSrcDataBuff = NULL;
	}
	if(g_pDesDataBuff)
	{
		delete []g_pDesDataBuff;
		g_pDesDataBuff = NULL;
	}

	g_Mp3Player.StopMusic();

	CloseHandle(g_hVGSFile);

	for(int i = 0;i < 3;i++)
	{
		if(g_pOGLWnd[i])
		{
			delete g_pOGLWnd[i];
			g_pOGLWnd[i] = NULL;
		}
	}

	LPFRAMEDATAOFFSET pOffSet = NULL;
	for(int j = 0;j < g_lstFramDataOffset.size();j++)
	{
		pOffSet = g_lstFramDataOffset[j];
		if(pOffSet)
		{
			delete pOffSet;
			pOffSet = NULL;
		}
	}

	for(int iFace = IMAGE_F;iFace < IMAGE_CNT;iFace++)
	{
		if(g_pBoxFace[iFace])
		{
			delete g_pBoxFace[iFace];
			g_pBoxFace[iFace] = NULL;
		}
	}

	for(int iMusic = 0; iMusic < g_lstMusicInfo.size(); iMusic++)
	{
		LPMUSICINFO pMusic = g_lstMusicInfo[iMusic];
		if(pMusic)
		{
			delete pMusic;
			pMusic = NULL;
		}
	}
}
//-------------------------------------------------

