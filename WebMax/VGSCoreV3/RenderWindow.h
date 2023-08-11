// RenderWindow.h: interface for the CRenderWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENDERWINDOW_H__A88F18F9_99F4_4EC5_B29B_D43200DB91FA__INCLUDED_)
#define AFX_RENDERWINDOW_H__A88F18F9_99F4_4EC5_B29B_D43200DB91FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSHeader.h"
#include "Viewport.h"
#include "CCamera.h"
#include <vector>
// #include "VGSGetRoot.h"

using namespace std;

class VGS_EXPORT CRenderWindow//:public CVGSGetRoot  
{
public:
	CRenderWindow(HWND wndHandle);
	virtual ~CRenderWindow();

	virtual VOID Release();

public:

	// lfet, top, width, height 取值[0, 1]
	virtual CViewport* AddViewport(CCamera *pCamera, INT ZOrder, FLOAT left, FLOAT top, FLOAT width, FLOAT height);
	virtual CViewport* GetViewport(INT index);
	CViewport* GetViewport(CCamera *pCamera); // 获取所有渲染视口中，使用相机pCamera的视口，如果有多个视口，返回第一个
	virtual VOID       Update();

	void DestroyViewport(CViewport *pVP);
	void DestroyAllViewport();

	// 更新视口的大小区域
	VOID UpdateViewportRect(const RECT &newClientRect);

	void setName(std::string sName){m_sName = sName;}
	std::string getName(){return m_sName;}

protected:
	HWND   m_hWnd;           // 窗口句柄
	RECT   m_clientRect;	 //窗体的Rect
	vector <CViewport*> m_vViewpot;  // 视口

	std::string m_sName;  


private:

};

#endif // !defined(AFX_RENDERWINDOW_H__A88F18F9_99F4_4EC5_B29B_D43200DB91FA__INCLUDED_)
