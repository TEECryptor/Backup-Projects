// Viewport.cpp: implementation of the CViewport class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "Viewport.h"
#include "CScene.h"
#include "CCamera.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CViewport::CViewport(CCamera* pCamera)
{
	m_pRenderWnd = NULL;
	m_pCamera = NULL;
	m_D3DViewport.MinZ = 0.0;
	m_D3DViewport.MaxZ = 1.0;
	m_sName = "";

	m_bEnable = TRUE;

	SetCamera(pCamera);
	if (pCamera)
	{
		pCamera->addViewport((DWORD)this);
	}

	m_Ratio = 1.3f;  // ��߱�
}
CViewport::~CViewport()
{
	m_pCamera = NULL;
	m_pRenderWnd = NULL;
}

VOID CViewport::Release()
{	
	if (m_pCamera)
	{
		m_pCamera->removeViewport((DWORD)this);
		m_pCamera = NULL;
	}
	m_pRenderWnd = NULL;
}

// �����ӿ�,��Ⱦһ֡ͼ��
VOID CViewport::Update()
{
	if (/*m_bEnable && */m_pCamera)
	{
		CScene *pScene = m_pCamera->GetScene();
		if (pScene)
		{
			pScene->Render(m_pCamera, this);
		}
	}
}
VOID CViewport::SetCamera(CCamera* pCamera)
{
	m_pCamera = pCamera;
	if (pCamera)
	{
		pCamera->addViewport((DWORD)this);
	}
}
CCamera* CViewport::GetCamera()
{
	return m_pCamera;
}

VOID CViewport::SetName(const std::string &sName)
{
	m_sName = sName;
}
std::string CViewport::GetName()
{
	return m_sName;
}

VOID CViewport::GetRect(RECT &rc)
{	
	rc.left   = m_D3DViewport.X;
	rc.right  = m_D3DViewport.X + m_D3DViewport.Width;
	rc.top    = m_D3DViewport.Y;
	rc.bottom = m_D3DViewport.Y + m_D3DViewport.Height;

}
VOID CViewport::SetRect(const RECT &rc)
{
	m_D3DViewport.X      = rc.left;
    m_D3DViewport.Y      = rc.top;
    m_D3DViewport.Width  = rc.right - rc.left;
    m_D3DViewport.Height = rc.bottom - rc.top;

	UpdateAspectRatio();  // ���¿�߱�

	// ����GUI
	if (m_pCamera && m_pCamera->GetScene())
	{
		m_pCamera->GetScene()->GetGuiManagerRef().SetViewportSize(m_D3DViewport.Width, m_D3DViewport.Height);
	}
}

VOID CViewport::SetRect(INT left, INT top, INT width, INT height)
{
	m_D3DViewport.X      = left;
    m_D3DViewport.Y      = top;
    m_D3DViewport.Width  = width;
    m_D3DViewport.Height = height;

	UpdateAspectRatio();  // ���¿�߱�

	// ����GUI
	if (m_pCamera && m_pCamera->GetScene())
	{
		m_pCamera->GetScene()->GetGuiManagerRef().SetViewportSize(m_D3DViewport.Width, m_D3DViewport.Height);
	}
}
VOID CViewport::SetZOrder(INT ZOrder)
{
	m_ZOrder = ZOrder;
}
INT CViewport::GetZOrder()
{
	return m_ZOrder;
}

CRenderWindow* CViewport::GetRenderWindow()
{
	return m_pRenderWnd;
}

VOID CViewport::SetRenderWindow(CRenderWindow* pRenderWnd)
{
	m_pRenderWnd = pRenderWnd;
}
// ��������ĺ���
VOID CViewport::UpdateAspectRatio()
{
	if (m_D3DViewport.Height > 0)
	{
		m_Ratio = m_D3DViewport.Width / (float)m_D3DViewport.Height;	
	}
}
// ��ȡ��߱�
float CViewport::getAspectRatio()
{
	return m_Ratio;
}

// ��ȡD3Dviewport����
D3DVIEWPORT9* CViewport::GetD3DViewport()
{
	return &m_D3DViewport;

}
// ��ȡ�ӿڵ����ģʽ
INT CViewport::GetFillMode() 
{
	return m_FillMode;
} 

// �����ӿڵ����ģʽ
VOID CViewport::SetFillMode(INT FillMode)
{
	m_FillMode = FillMode;
} 

// �����ӿ��Ƿ���Ҫ��Ⱦ
VOID CViewport::SetEnable(BOOL enable)
{
	m_bEnable = enable;
}

BOOL CViewport::GetEnable()
{
	return m_bEnable;
}