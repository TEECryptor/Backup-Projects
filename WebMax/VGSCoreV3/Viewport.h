// Viewport.h: interface for the CViewport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWPORT_H__CF3894A1_4339_4F48_AAD0_0BB6D5C493BB__INCLUDED_)
#define AFX_VIEWPORT_H__CF3894A1_4339_4F48_AAD0_0BB6D5C493BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSHeader.h"
#include "RenderWindow.h"

class VGS_EXPORT CViewport
{
public:
	CViewport       (CCamera* pCamera);
	virtual         ~CViewport();

	virtual         VOID Release();

	VOID            SetCamera(CCamera* pCamera);
	CCamera*        GetCamera();

	VOID            SetName(const std::string &sName);
	std::string     GetName();

	VOID            GetRect(RECT &rc);
	VOID            SetRect(const RECT &rc);
	VOID            SetRect(INT left, INT top, INT width, INT height);

	VOID            SetZOrder(INT ZOrder);
	INT             GetZOrder();

	CRenderWindow*  GetRenderWindow();
	VOID            SetRenderWindow(CRenderWindow* pRenderWnd);

	// 更新相机的横宽比
	VOID UpdateAspectRatio();

	// 获取宽高比
	float getAspectRatio();

	// 获取D3Dviewport参数
	D3DVIEWPORT9* GetD3DViewport();

	VOID Update();  // 更新视口,渲染一帧图像

	INT GetFillMode();  // 获取视口的填充模式
	VOID SetFillMode(INT FillMode);  // 设置视口的填充模式

	// 设置视口是否需要渲染
	VOID SetEnable(BOOL enable);	
	BOOL GetEnable();

	inline bool operator < (CViewport* pVP)
	{
		return (m_ZOrder < pVP->m_ZOrder);
	}

private:
	std::string     m_sName;
	CCamera*        m_pCamera;
	D3DVIEWPORT9    m_D3DViewport;

	INT             m_ZOrder;

	INT             m_FillMode;   // 填充模式

	// CSwapChain*     m_pRenderWnd;
	CRenderWindow*     m_pRenderWnd;     

	BOOL            m_bEnable;  // 是否渲染

	float           m_Ratio; // 视口的宽高比
};

#endif // !defined(AFX_VIEWPORT_H__CF3894A1_4339_4F48_AAD0_0BB6D5C493BB__INCLUDED_)
