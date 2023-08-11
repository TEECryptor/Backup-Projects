//--------------------------------------------------------
//	name:GenericWnd.h
//	Intro:the define of window base class
//	Author:YYF
//	date:05-1-17
//  version:0.1v
//
//	Date:05-3-2
//	Introduce:transplant form eVC to VC
//	Version:0.2v
//--------------------------------------------------------
#ifndef _GENERICWND_H_
#define _GENERICWND_H_
//--------------------------------------------------------
#include <windows.h>
//--------------------------------------------------------
#define  WM_MY_CREATE  (WM_USER + 100)		//create message of custom defined
#define  MAX_MAP_CNT	15					//the max mapping count of handle,class
//--------------------------------------------------------
//	name£ºCHWndClassMap
//	Intro£ºthe stable map of custom defined£¬don`t use std::map
//--------------------------------------------------------
class CHWndClassMap
{
	//-----------------------------------------------------
public:
	//-----------------------------------------------------
	//	name£ºCHWndClassMap
	//	Intro£ºConstructor
	//-----------------------------------------------------
	CHWndClassMap()
	{
		m_iCurMapCnt = 0;
		for(int i = 0;i < MAX_MAP_CNT;i++)
		{
			m_pClass[i] = NULL;
			m_hWnd[i] = NULL;
		}
	}
	//-----------------------------------------------------
	//	Name£º~CHWndClassMap
	//	Intro£ºDestructor
	//-----------------------------------------------------
	~CHWndClassMap(){;}
	//-----------------------------------------------------
public:
	//-----------------------------------------------------
	//	name£ºAdd
	//	Intro£ºadd mapping count
	//	Param£ºpClass:i:class pointer
	//	      hWnd:i:the handle of window class
	//	return£ºif successed then return TRUE£¬otherwise return FALSE
	//-----------------------------------------------------
	BOOL Add(LPVOID pClass, HWND hWnd)
	{
		if(m_iCurMapCnt >= MAX_MAP_CNT) return FALSE;
		
		m_pClass[m_iCurMapCnt] = pClass;
		m_hWnd[m_iCurMapCnt] = hWnd;
		
		m_iCurMapCnt++;
		return TRUE;
	}
	//-----------------------------------------------------
	//	name£ºDelete
	//	Intro£ºDelete the mapping
	//	Param£ºhWnd:i:the specified window handle
	//	return£ºif deleted successed then return ture, otherwise return false
	//-----------------------------------------------------
	BOOL Delete(HWND hWnd)
	{
		for(int i = 0;i < MAX_MAP_CNT;i++)
		{
			if(m_hWnd[i] == hWnd)
			{
				m_pClass[i] = NULL;
				m_hWnd[i] = NULL;
				return TRUE;
			}
		}
		return FALSE;
	}
	//-----------------------------------------------------
	//	name£ºGetClass
	//	Intro£ºreturn class from hWnd
	//	Param£ºhWnd:i:the specified window handle
	//	Return£ºthe pionter of the handle
	//-----------------------------------------------------
	LPVOID GetClass(HWND hWnd)
	{		
		for(int i = 0;i < MAX_MAP_CNT;i++)
		{
			if(m_hWnd[i] == hWnd) return m_pClass[i];
		}
		return NULL;
	}
	//-----------------------------------------------------
private:
	int				m_iCurMapCnt;				//current map count
	LPVOID			m_pClass[MAX_MAP_CNT];		//class
	HWND			m_hWnd[MAX_MAP_CNT];		//HWND
};
//--------------------------------------------------------
//	name£ºCGenericWnd
//	Intro£ºthe window base class
//--------------------------------------------------------
class CGenericWnd  
{
public:
	CGenericWnd();
	virtual ~CGenericWnd();
	//-----------------------------------------------------
public:
	//create function
	BOOL	Create(HINSTANCE hInstance, LPCTSTR szClassName, HWND hWndParent, RECT rc, DWORD dwStyle);
public:
	//-----------------------------------------------------	
	//the procedure of static window£¬which distribute the window message
    static LRESULT CALLBACK WndProcFunc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	//the message callback function of sub class 
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) = 0;
	//-----------------------------------------------------
public:
	static BOOL	m_bIsRegster;	
	HWND	m_hParent;			//The parent window
	HWND	m_hWnd;				//window handle
	WNDPROC	m_lOldProcFunc;		//the old window procrudce
	//-----------------------------------------------------
private:	
	static CHWndClassMap m_Map;	//mapping of HWND,Class 
};
//--------------------------------------------------------
#endif // _GENERICWND_H_
//--------------------------------------------------------