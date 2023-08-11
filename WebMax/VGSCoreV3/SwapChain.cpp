// SwapChain.cpp: implementation of the CSwapChain class.
//
//////////////////////////////////////////////////////////////////////

#include "SwapChain.h"
// #include "VGSRender.h"
#include "Root.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static INT s_SwapChainIdx = 0;

//##ModelId=47EBC72C03A9
CSwapChain::CSwapChain(LPDIRECT3D9 pd3d, LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd) : CRenderWindow(hWnd)
{
	m_pd3d       = pd3d;
	m_pd3dDevice = pd3dDevice;
	m_pSwapChain = NULL;
	//m_pInput     = NULL;
// 	m_MultiSampleType = /*g_VGSRender*/GetRoot()->getVGSRender().d3dParam.MultiSampleType;

	m_hWnd = hWnd;
	m_bRender = TRUE;
	// m_pCamera = NULL;
	CameraType = 0;
	m_projMode = 0;		 //Ĭ��Ϊ͸��ͶӰģʽ
	FillMode = D3DFILL_SOLID;

	m_bActiveControl = FALSE;
	
	controlType = -1;	

	mouseInfo.X = 0;
	mouseInfo.Y = 0;
	mouseInfo.LButton = FALSE;
	mouseInfo.MButton = FALSE;
	mouseInfo.RButton = FALSE;
	mouseInfo.RollDis = 0;

	currentWalker = NULL;
	currentEditor = NULL;		
	currentFlyer = NULL;	
	selectAxisScale=1.0f;

	m_DepthStencilBuffer = NULL;
}

//##ModelId=47EBC72C03AD
CSwapChain::~CSwapChain()
{
	
}

//����SwapChain
//##ModelId=47EBC72C039A
VOID CSwapChain::Create()
{
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp)); 
	m_d3dpp.Windowed = TRUE; 
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; 

	//ʹ�õ�ǰ�������ʾģʽ 
	GetClientRect(m_hWnd, &m_clientRect);
	m_pd3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_d3dmm); 
	m_d3dpp.BackBufferFormat = m_d3dmm.Format; 
	m_d3dpp.BackBufferWidth  = m_clientRect.right - m_clientRect.left;
	m_d3dpp.BackBufferHeight = m_clientRect.bottom - m_clientRect.top;
	m_d3dpp.MultiSampleType  = m_MultiSampleType;
	m_d3dpp.hDeviceWindow    = m_hWnd;

	//m_pSwapChain��һ��IDirect3DSwapChain*���� 
	m_pd3dDevice->CreateAdditionalSwapChain(&m_d3dpp, &m_pSwapChain);

	//m_ratio = 1.0f * (m_clientRect.right - m_clientRect.left) / (m_clientRect.bottom - m_clientRect.top);

	//����CInput����
	//m_pInput = new CInput(m_hWnd);
	//m_pInput->Init();
}

//����SwapChian�ĺ󻺳�, ������ı��Сʱ
BOOL CSwapChain::UpdateBackbuffer()
{
	// �����ӿ�����
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	UpdateViewportRect(rect);  // �����ӿ�����

	//�ͷ�SwapChain
	SAFE_RELEASE_D3D(m_pSwapChain);	
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp)); 
	m_d3dpp.Windowed = TRUE; 
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; 

	//ʹ�õ�ǰ�������ʾģʽ 
	m_pd3d->GetAdapterDisplayMode(0, &m_d3dmm); 
	m_d3dpp.BackBufferFormat = m_d3dmm.Format; 
	m_d3dpp.BackBufferWidth  =  rect.right - rect.left;
	m_d3dpp.BackBufferHeight = rect.bottom - rect.top;
	m_d3dpp.MultiSampleType  = m_MultiSampleType;
	m_d3dpp.hDeviceWindow    = m_hWnd; 

	//m_pSwapChain��һ��IDirect3DSwapChain*���� 
	Invalidate();
	HRESULT hr = m_pd3dDevice->CreateAdditionalSwapChain(&m_d3dpp, &m_pSwapChain);

	////m_ratio = (float)(rect.right - rect.left) / (rect.bottom - rect.top);

	if (SUCCEEDED(hr))
	{
		return TRUE;
	}
	return FALSE;
}

//�ͷ�SwapChain
//##ModelId=47EBC72C039B
VOID CSwapChain::Invalidate()
{
	//�ͷ�SwapChain
	SAFE_RELEASE_D3D(m_pSwapChain);	
}

//�ؽ�SwapChain
//##ModelId=47EBC72C039C
VOID CSwapChain::Restore()
{
	Create();
// 	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp)); 
// 	m_d3dpp.Windowed = TRUE; 
// 	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; 
// 
// 	//ʹ�õ�ǰ�������ʾģʽ 
// 	RECT rect;
// 	GetClientRect(m_hWnd, &rect);
// 	m_pd3d->GetAdapterDisplayMode(0, &m_d3dmm); 
// 	m_d3dpp.BackBufferFormat = m_d3dmm.Format; 
// 	m_d3dpp.BackBufferWidth =  rect.right - rect.left;
// 	m_d3dpp.BackBufferHeight = rect.bottom - rect.top;
// 	m_d3dpp.MultiSampleType = m_MultiSampleType;
// 	m_d3dpp.hDeviceWindow = m_hWnd; 
// 
// 	//m_pSwapChain��һ��IDirect3DSwapChain*���� 
// 	HRESULT hr = m_pd3dDevice->CreateAdditionalSwapChain(&m_d3dpp, &m_pSwapChain);
// 	
// 	m_ratio = 1.0f * (rect.right - rect.left) / (rect.bottom - rect.top);
}

//�ͷ�SwapChain
//##ModelId=47EBC72C039E
VOID CSwapChain::Release()
{
	SAFE_RELEASE_D3D(m_DepthStencilBuffer);

	//�ͷ�SwapChain
	SAFE_RELEASE_D3D(m_pSwapChain);	
	//�ͷ�CInput����
	//SAFE_RELEASE(m_pInput);
	//SAFE_DELETE(m_pInput);
}

//	���һ���ӿ�
// left, FLOAT top, FLOAT width, FLOAT height ��Ϊ�ٷֱȣ�ȡֵ[0, 1]
// CViewport* CSwapChain::AddViewport(CCamera* pCamera, FLOAT left, FLOAT top, FLOAT width, FLOAT height, INT ZOrder)
// {
// 	CViewport *pVP = new CViewport(pCamera);
// 	SetViewportRect(pVP, left, top, width, height);
// 	pVP->SetZOrder(ZOrder);
// 	pVP->SetCamera(pCamera);	
// 
// 	m_vViewpot.push_back(pVP);	
// 
// 	return pVP;
// }

// �Ƴ��ӿ�
// BOOL CSwapChain::RemoveViewport(CViewport *pViewport)
// {
// 	for (int i = 0; i < m_vViewpot.size(); ++i)
// 	{
// 		if (pViewport == m_vViewpot[i])
// 		{
// 			m_vViewpot.erase(m_vViewpot.begin() + i);
// 			return TRUE;
// 		}
// 	}
// 	return FALSE;
// }
// 
// CViewport* CSwapChain::GetViewport(INT index)
// {
// 	int Count = m_vViewpot.size();
// 	if (index >= 0 && index < Count)
// 	{
// 		return m_vViewpot[index];
// 	}
// 	return NULL;
// }
// 
// // �����ӿڵĴ�С��λ�� left, FLOAT top, FLOAT width, FLOAT height ��Ϊ�ٷֱȣ�ȡֵ[0, 1]
// VOID CSwapChain::SetViewportRect(CViewport *pVP, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
// {
// 	// ���ݰٷֱȣ�����vp��ʵ��λ�úʹ�С
// 	RECT rect;
// 	GetClientRect(m_hWnd, &rect);
// 
// 	RECT vpRect;
// 	FLOAT wid     = rect.right - rect.left;  // ʵ��������
// 	FLOAT hei     = rect.bottom - rect.top;
// 	vpRect.left   = rect.left + wid * left;
// 	vpRect.right  = vpRect.left + wid * width;
// 	vpRect.top    = rect.top + hei * top;
// 	vpRect.bottom = vpRect.top + hei * height;
// 
// 	pVP->SetRect(vpRect);
// }
// 
// // �õ��ӿڵĴ�С��λ��
// BOOL CSwapChain::GetViewportRect(CViewport *pVP, FLOAT &left, FLOAT &top, FLOAT &width, FLOAT &height)
// {
// 	if (pVP)
// 	{
// 		RECT vpRect;
// 		pVP->GetRect(vpRect);
// 
// 		// ���ݰٷֱȣ�����vp��ʵ��λ�úʹ�С
// 		RECT rect;
// 		GetClientRect(m_hWnd, &rect);
// 
// 		FLOAT wndWid = rect.right - rect.left;  // ʵ��������
// 		FLOAT wndHei = rect.bottom - rect.top;
// 		FLOAT vpWid   = vpRect.right - vpRect.left;
// 		FLOAT vpHei   = vpRect.bottom - vpRect.top;
// 
// 		if (fabs(wndWid) > VGS_ZERO_VALUE_5 && fabs(wndHei) < VGS_ZERO_VALUE_5)
// 		{
// 			left = (vpRect.left - rect.left) / (float)wndWid;
// 			top  = (vpRect.top - rect.top) / (float)wndHei;
// 			width = vpWid / wndWid;
// 			height = vpHei / wndHei;
// 
// 			return TRUE;
// 		}
// 	}
// 
// 	return FALSE;
// }