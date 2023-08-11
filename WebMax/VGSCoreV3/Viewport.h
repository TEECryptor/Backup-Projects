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

	// ��������ĺ���
	VOID UpdateAspectRatio();

	// ��ȡ��߱�
	float getAspectRatio();

	// ��ȡD3Dviewport����
	D3DVIEWPORT9* GetD3DViewport();

	VOID Update();  // �����ӿ�,��Ⱦһ֡ͼ��

	INT GetFillMode();  // ��ȡ�ӿڵ����ģʽ
	VOID SetFillMode(INT FillMode);  // �����ӿڵ����ģʽ

	// �����ӿ��Ƿ���Ҫ��Ⱦ
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

	INT             m_FillMode;   // ���ģʽ

	// CSwapChain*     m_pRenderWnd;
	CRenderWindow*     m_pRenderWnd;     

	BOOL            m_bEnable;  // �Ƿ���Ⱦ

	float           m_Ratio; // �ӿڵĿ�߱�
};

#endif // !defined(AFX_VIEWPORT_H__CF3894A1_4339_4F48_AAD0_0BB6D5C493BB__INCLUDED_)
