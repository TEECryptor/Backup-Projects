// MyThread.cpp : implementation file
//
//-----------------------------------------------------------
#include "stdafx.h"
#include "MyThread.h"
//-----------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-----------------------------------------------------------
CMyThread*		g_pCurrentThread = NULL;
//-----------------------------------------------------------
//	The message window process function
//-----------------------------------------------------------
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	return g_pCurrentThread->WndProc(msg, wp, lp);
}
//----------------------------------------------------------- 
// CMyThread
//-----------------------------------------------------------
IMPLEMENT_DYNCREATE(CMyThread, CWinThread)
//-----------------------------------------------------------
CMyThread::CMyThread()
{
	g_pCurrentThread = this;
	m_hMsgWnd = 0;
}
//-----------------------------------------------------------
CMyThread::~CMyThread()
{
}
//-----------------------------------------------------------
BOOL CMyThread::InitInstance()
{
	//To create the msg window
	if(RegisterMsgWnd())
	{
		if(!CreateMsgWnd())
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
//-----------------------------------------------------------
int CMyThread::ExitInstance()
{
	DestroyWindow(m_hMsgWnd);
	m_hMsgWnd = NULL;

	return CWinThread::ExitInstance();
}
//-----------------------------------------------------------
BEGIN_MESSAGE_MAP(CMyThread, CWinThread)
	//{{AFX_MSG_MAP(CMyThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-----------------------------------------------------------

//-----------------------------------------------------------
BOOL CMyThread::PostMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	PostThreadMessage(message, wParam, lParam);
	
	return TRUE;
}
//-----------------------------------------------------------
LRESULT CMyThread::SendMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_hMsgWnd, message, wParam, lParam);

	return S_OK;
}
//-----------------------------------------------------------
BOOL CMyThread::RegisterMsgWnd()
{   
	WNDCLASSEX wcx; 
  
    wcx.cbSize = sizeof(wcx);			
    wcx.style = CS_HREDRAW | 
        CS_VREDRAW;						
    wcx.lpfnWndProc = MainWndProc;		
    wcx.cbClsExtra = 0;					
    wcx.cbWndExtra = 0;					
    wcx.hInstance = NULL;				
    wcx.hIcon = NULL;					
    wcx.hCursor = NULL;					
    wcx.hbrBackground = NULL;			
    wcx.lpszMenuName =  NULL;			
    wcx.lpszClassName = "MainWClass";	
    wcx.hIconSm = NULL; 
  
    return RegisterClassEx(&wcx); 
}
//-----------------------------------------------------------
BOOL CMyThread::CreateMsgWnd()
{    
	m_hMsgWnd = CreateWindow( 
        "MainWClass",			
        NULL,					
        WS_OVERLAPPED,			
        0,						
        0,						
        1,						
        1,						
        (HWND) NULL,			
        (HMENU) NULL,			
        NULL,					
        (LPVOID) NULL);			
 
    if (!m_hMsgWnd) 
	{
		DWORD dwError = GetLastError();
        return FALSE; 
	}

	ShowWindow(m_hMsgWnd, SW_HIDE);
	return TRUE;
}
//-----------------------------------------------------------
BOOL CMyThread::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_MY_MSG)
	{
		char szString[32] = {0};
		sprintf(szString, "Param = %d", pMsg->wParam);
		MessageBox(NULL, szString, "Message", MB_OK);

		return 0;
	}
	
	return CWinThread::PreTranslateMessage(pMsg);
}
//-----------------------------------------------------------
LRESULT CMyThread::WndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
	case WM_MY_MSG_WAITE:
		{
			Sleep(5000);
		}
		return 1;
	}

	return 1;
}
//-----------------------------------------------------------
