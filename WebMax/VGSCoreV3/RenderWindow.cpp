// RenderWindow.cpp: implementation of the CRenderWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "RenderWindow.h"
#include <algorithm>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRenderWindow::CRenderWindow(HWND wndHandle)
{
	m_sName = "";
	m_hWnd = wndHandle;

	ZeroMemory(&m_clientRect, sizeof(m_clientRect));
	GetClientRect(m_hWnd, &m_clientRect);
}

CRenderWindow::~CRenderWindow()
{

}

VOID CRenderWindow::Release()
{	
	for (UINT i = 0; i < m_vViewpot.size(); ++i)
	{
		SAFE_RELEASE(m_vViewpot[i]);
		SAFE_DELETE(m_vViewpot[i]);
	}

	m_vViewpot.clear();
}

// lfet, top, width, height ȡֵ[0, 1]
CViewport* CRenderWindow::AddViewport(CCamera *pCamera, INT ZOrder, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
	CViewport *pVP = new CViewport(pCamera);
	if (pVP)
	{
		// �������ֵ�������ӿ�����Ⱦ�����е�����λ��--����
		GetClientRect(m_hWnd, &m_clientRect);
		INT WndWidth  = m_clientRect.right - m_clientRect.left;  // render window�Ŀ�Ⱥ͸߶�
		INT WndHeight = m_clientRect.bottom - m_clientRect.top;

		INT vpLeft   = m_clientRect.left + left * WndWidth;
		INT vpTop    = m_clientRect.top + top * WndHeight;
		INT vpWidth  = width * WndWidth;
		INT vpHeight = height * WndHeight;

		pVP->SetRect(vpLeft, vpTop, vpWidth, vpHeight);  
		pVP->SetZOrder(ZOrder);
		pVP->SetRenderWindow(this);
		
		m_vViewpot.push_back(pVP);

		sort(m_vViewpot.begin(), m_vViewpot.end());  // ��zorder����
	}

	return pVP;
}
CViewport* CRenderWindow::GetViewport(INT index)
{
	if (index >= 0 && index < (INT)m_vViewpot.size())
	{
		return m_vViewpot[index];
	}
	return 0;
}

// ��ȡ������Ⱦ�ӿ��У�ʹ�����pCamera���ӿڣ�����ж���ӿڣ����ص�һ��
CViewport* CRenderWindow::GetViewport(CCamera *pCamera)
{
	for (UINT i = 0; i < m_vViewpot.size(); ++i)
	{
		if (m_vViewpot[i] && m_vViewpot[i]->GetCamera() == pCamera)
		{
			return m_vViewpot[i];
		}
	}
	return 0;
}
void CRenderWindow::DestroyViewport(CViewport *pVP)
{
	if (pVP)
	{
		for (UINT i = 0; i < m_vViewpot.size(); ++i)
		{
			if (m_vViewpot[i] == pVP)
			{
				SAFE_RELEASE(pVP);
				SAFE_DELETE(pVP);
				m_vViewpot.erase(m_vViewpot.begin() + i);
				return;
			}
		}
	}
}
void CRenderWindow::DestroyAllViewport()
{
	for (UINT i = 0; i < m_vViewpot.size(); ++i)
	{
		if (m_vViewpot[i])
		{
			SAFE_RELEASE(m_vViewpot[i]);
			SAFE_DELETE(m_vViewpot[i]);
		}
	}
	m_vViewpot.clear();
}

VOID CRenderWindow::Update()
{
	for (UINT i = 0; i < m_vViewpot.size(); ++i)
	{
		if (m_vViewpot[i] && m_vViewpot[i]->GetEnable())
		{
			m_vViewpot[i]->Update();
		}
	}
}

// �����ӿڵĴ�С����
VOID CRenderWindow::UpdateViewportRect( const RECT &newClientRect)
{
	// �µ���Ⱦ���ڴ�С
	INT newWidth  = newClientRect.right - newClientRect.left;  // render window�Ŀ�Ⱥ͸߶�
	INT newHeight = newClientRect.bottom - newClientRect.top;

	if ((newClientRect.right == 0 && newClientRect.right == newClientRect.left)
		|| (newClientRect.top == 0 && newClientRect.top == newClientRect.bottom))
	{
		return;
	}

	// �ɵ���Ⱦ���ڴ�С
	INT oldWidth  = m_clientRect.right - m_clientRect.left;
	INT oldHeight = m_clientRect.bottom - m_clientRect.top;
	
	if (oldWidth > 0 && oldHeight > 0)
	{
		float widthScale  = newWidth / (float)oldWidth;    // ��Ⱦ���ڵ����ű���
		float heigthScale = newHeight / (float)oldHeight;
		
		// ���ݴ��ڵ����ű���������ÿ���ӿڵ�����
		for (UINT i = 0; i < m_vViewpot.size(); ++i)
		{
			CViewport *pVP = m_vViewpot[i];
			if (pVP)
			{	
				RECT rect;
				pVP->GetRect(rect);
				rect.left = rect.left * widthScale + 0.5f;
				rect.right = rect.right * widthScale + 0.5f;
				rect.top = rect.top * heigthScale + 0.5f;
				rect.bottom = rect.bottom * heigthScale + 0.5f;

				pVP->SetRect(rect);			
			}
		}
	}

	// ������º���ӿ�����
	m_clientRect = newClientRect;

}
