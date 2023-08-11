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
	m_projMode = 0;		 //默认为透视投影模式
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

//创建SwapChain
//##ModelId=47EBC72C039A
VOID CSwapChain::Create()
{
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp)); 
	m_d3dpp.Windowed = TRUE; 
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; 

	//使用当前窗体的显示模式 
	GetClientRect(m_hWnd, &m_clientRect);
	m_pd3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_d3dmm); 
	m_d3dpp.BackBufferFormat = m_d3dmm.Format; 
	m_d3dpp.BackBufferWidth  = m_clientRect.right - m_clientRect.left;
	m_d3dpp.BackBufferHeight = m_clientRect.bottom - m_clientRect.top;
	m_d3dpp.MultiSampleType  = m_MultiSampleType;
	m_d3dpp.hDeviceWindow    = m_hWnd;

	//m_pSwapChain是一个IDirect3DSwapChain*对象 
	m_pd3dDevice->CreateAdditionalSwapChain(&m_d3dpp, &m_pSwapChain);

	//m_ratio = 1.0f * (m_clientRect.right - m_clientRect.left) / (m_clientRect.bottom - m_clientRect.top);

	//创建CInput对象
	//m_pInput = new CInput(m_hWnd);
	//m_pInput->Init();
}

//更新SwapChian的后缓冲, 当窗体改变大小时
BOOL CSwapChain::UpdateBackbuffer()
{
	// 更新视口区域
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	UpdateViewportRect(rect);  // 更新视口区域

	//释放SwapChain
	SAFE_RELEASE_D3D(m_pSwapChain);	
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp)); 
	m_d3dpp.Windowed = TRUE; 
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; 

	//使用当前窗体的显示模式 
	m_pd3d->GetAdapterDisplayMode(0, &m_d3dmm); 
	m_d3dpp.BackBufferFormat = m_d3dmm.Format; 
	m_d3dpp.BackBufferWidth  =  rect.right - rect.left;
	m_d3dpp.BackBufferHeight = rect.bottom - rect.top;
	m_d3dpp.MultiSampleType  = m_MultiSampleType;
	m_d3dpp.hDeviceWindow    = m_hWnd; 

	//m_pSwapChain是一个IDirect3DSwapChain*对象 
	Invalidate();
	HRESULT hr = m_pd3dDevice->CreateAdditionalSwapChain(&m_d3dpp, &m_pSwapChain);

	////m_ratio = (float)(rect.right - rect.left) / (rect.bottom - rect.top);

	if (SUCCEEDED(hr))
	{
		return TRUE;
	}
	return FALSE;
}

//释放SwapChain
//##ModelId=47EBC72C039B
VOID CSwapChain::Invalidate()
{
	//释放SwapChain
	SAFE_RELEASE_D3D(m_pSwapChain);	
}

//重建SwapChain
//##ModelId=47EBC72C039C
VOID CSwapChain::Restore()
{
	Create();
// 	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp)); 
// 	m_d3dpp.Windowed = TRUE; 
// 	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; 
// 
// 	//使用当前窗体的显示模式 
// 	RECT rect;
// 	GetClientRect(m_hWnd, &rect);
// 	m_pd3d->GetAdapterDisplayMode(0, &m_d3dmm); 
// 	m_d3dpp.BackBufferFormat = m_d3dmm.Format; 
// 	m_d3dpp.BackBufferWidth =  rect.right - rect.left;
// 	m_d3dpp.BackBufferHeight = rect.bottom - rect.top;
// 	m_d3dpp.MultiSampleType = m_MultiSampleType;
// 	m_d3dpp.hDeviceWindow = m_hWnd; 
// 
// 	//m_pSwapChain是一个IDirect3DSwapChain*对象 
// 	HRESULT hr = m_pd3dDevice->CreateAdditionalSwapChain(&m_d3dpp, &m_pSwapChain);
// 	
// 	m_ratio = 1.0f * (rect.right - rect.left) / (rect.bottom - rect.top);
}

//释放SwapChain
//##ModelId=47EBC72C039E
VOID CSwapChain::Release()
{
	SAFE_RELEASE_D3D(m_DepthStencilBuffer);

	//释放SwapChain
	SAFE_RELEASE_D3D(m_pSwapChain);	
	//释放CInput对象
	//SAFE_RELEASE(m_pInput);
	//SAFE_DELETE(m_pInput);
}

//	添加一个视口
// left, FLOAT top, FLOAT width, FLOAT height 均为百分比，取值[0, 1]
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

// 移除视口
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
// // 设置视口的大小和位置 left, FLOAT top, FLOAT width, FLOAT height 均为百分比，取值[0, 1]
// VOID CSwapChain::SetViewportRect(CViewport *pVP, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
// {
// 	// 根据百分比，计算vp的实际位置和大小
// 	RECT rect;
// 	GetClientRect(m_hWnd, &rect);
// 
// 	RECT vpRect;
// 	FLOAT wid     = rect.right - rect.left;  // 实际像素数
// 	FLOAT hei     = rect.bottom - rect.top;
// 	vpRect.left   = rect.left + wid * left;
// 	vpRect.right  = vpRect.left + wid * width;
// 	vpRect.top    = rect.top + hei * top;
// 	vpRect.bottom = vpRect.top + hei * height;
// 
// 	pVP->SetRect(vpRect);
// }
// 
// // 得到视口的大小和位置
// BOOL CSwapChain::GetViewportRect(CViewport *pVP, FLOAT &left, FLOAT &top, FLOAT &width, FLOAT &height)
// {
// 	if (pVP)
// 	{
// 		RECT vpRect;
// 		pVP->GetRect(vpRect);
// 
// 		// 根据百分比，计算vp的实际位置和大小
// 		RECT rect;
// 		GetClientRect(m_hWnd, &rect);
// 
// 		FLOAT wndWid = rect.right - rect.left;  // 实际像素数
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