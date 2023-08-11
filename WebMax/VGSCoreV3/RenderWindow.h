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

	// lfet, top, width, height ȡֵ[0, 1]
	virtual CViewport* AddViewport(CCamera *pCamera, INT ZOrder, FLOAT left, FLOAT top, FLOAT width, FLOAT height);
	virtual CViewport* GetViewport(INT index);
	CViewport* GetViewport(CCamera *pCamera); // ��ȡ������Ⱦ�ӿ��У�ʹ�����pCamera���ӿڣ�����ж���ӿڣ����ص�һ��
	virtual VOID       Update();

	void DestroyViewport(CViewport *pVP);
	void DestroyAllViewport();

	// �����ӿڵĴ�С����
	VOID UpdateViewportRect(const RECT &newClientRect);

	void setName(std::string sName){m_sName = sName;}
	std::string getName(){return m_sName;}

protected:
	HWND   m_hWnd;           // ���ھ��
	RECT   m_clientRect;	 //�����Rect
	vector <CViewport*> m_vViewpot;  // �ӿ�

	std::string m_sName;  


private:

};

#endif // !defined(AFX_RENDERWINDOW_H__A88F18F9_99F4_4EC5_B29B_D43200DB91FA__INCLUDED_)
