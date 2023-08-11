//--------------------------------------------------------
//	Name£ºGenericWnd.cpp
//	Introduce£ºThe base class of window, distribute windows message
//	Author£ºYYF
//	Date£º05-1-17
//	Version£º0.1v
//--------------------------------------------------------
#include "stdafx.h"
#include "GenericWnd.h"
//--------------------------------------------------------
CHWndClassMap CGenericWnd::m_Map;
BOOL CGenericWnd::m_bIsRegster = false;
//--------------------------------------------------------
//	name£ºCGenericWnd
//	intro£ºConstructor
//	param£ºvoid
//	return£ºvoid
//--------------------------------------------------------
CGenericWnd::CGenericWnd()
{
	m_hWnd = NULL;
}
//--------------------------------------------------------
//	name£º~CGenericWnd
//	intro£ºDestructor
//	param£ºvoid
//	return£ºvoid
//--------------------------------------------------------
CGenericWnd::~CGenericWnd()
{
	m_Map.Delete(m_hWnd);
	SetWindowLong(m_hWnd, DWL_DLGPROC, (LONG)m_lOldProcFunc);
}
//--------------------------------------------------------
//	name£ºWndProc
//	intro£ºThe static window procedure, windows message are distributed in this function
//	param£º
//	return£º
//--------------------------------------------------------
LRESULT CALLBACK CGenericWnd::WndProcFunc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	CGenericWnd* pWnd = (CGenericWnd*)m_Map.GetClass(hwnd);

    if(pWnd) return pWnd->WndProc(msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
//--------------------------------------------------------
//	name£ºCreate
//	intro£ºthe function of create window
//	param£ºhInstance:i:the instance of app
//		  szClassName:i:window class name£¬if is NULL£¬register a new class
//		  hWndParent:i:the handle of parent window
//		  rc:i:the rect of new window
//	return£ºif create successed then return true, otherwise return false
//--------------------------------------------------------
BOOL CGenericWnd::Create(HINSTANCE hInstance, LPCTSTR szClassName, HWND hWndParent, RECT rc, DWORD dwStyle)
{	
	m_hParent = hWndParent;
	LPCTSTR pszClass = szClassName;
	if(szClassName == NULL)
	{
		//new window class name
		pszClass = "VGS";

		//if the new window is not register, register it!
		//if(!GetClassInfo(hInstance, pszClass, &wcOld))
		if(!m_bIsRegster)
		{
			WNDCLASS wc;
			
			//window class struct
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = (WNDPROC)WndProcFunc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = NULL;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = NULL;
			wc.lpszMenuName = NULL;
			wc.lpszClassName = pszClass;

			//register window class
			if(!(m_bIsRegster = RegisterClass(&wc)))
			{
				MessageBox(NULL, "Register Wnd Error£¡", "Error", MB_ICONERROR);
				return 0;
			}
		}
	}
	
	//create the window
	m_hWnd = CreateWindow(pszClass,
						"VGS",
						dwStyle,
						rc.left,
						rc.top,
						rc.right - rc.left,
						rc.bottom - rc.top,
						hWndParent,
						NULL,
						hInstance,
						NULL);

	//save the handle of window and class pointer to map,and send WM_MY_CREATE message
	if(m_hWnd)
	{
		m_Map.Add(this, m_hWnd);

		//send the WM_MY_CREATE to the window
		SendMessage(m_hWnd, WM_MY_CREATE, 0, 0);
		return true;
	}
	return false;
}
//--------------------------------------------------------