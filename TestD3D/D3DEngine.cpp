//------------------------------------------------
#include "stdafx.h"
#include ".\d3dengine.h"
//------------------------------------------------
CD3DEngine::CD3DEngine(void)
 : m_pD3D(NULL),		
   m_pd3dDevice(NULL)
{
}
//------------------------------------------------
CD3DEngine::~CD3DEngine(void)
{
	SAF_RELEASE(m_pd3dDevice);
	SAF_RELEASE(m_pD3D);
}
//------------------------------------------------
BOOL CD3DEngine::CreateD3DDevice(int iWidth, int iHeight, HWND hRenderWnd, BOOL bFullScreen)
{
	if(NULL == hRenderWnd)
		return FALSE;

	SAF_RELEASE(m_pd3dDevice);
	SAF_RELEASE(m_pD3D);
	
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if(NULL == m_pD3D)
		return FALSE;

	ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
	if(FALSE == bFullScreen)
	{
		m_d3dpp.Windowed = TRUE;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.BackBufferCount = 1;
		m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; 
		m_d3dpp.hDeviceWindow = hRenderWnd;
	}
	else
	{	
		m_d3dpp.Windowed					= FALSE;
		m_d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;
		m_d3dpp.BackBufferCount				= 1;
		m_d3dpp.BackBufferWidth				= iWidth;
		m_d3dpp.BackBufferHeight			= iHeight;
		m_d3dpp.BackBufferFormat			= D3DFMT_A8R8G8B8; 
		m_d3dpp.hDeviceWindow				= hRenderWnd;
		m_d3dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;
		m_d3dpp.MultiSampleQuality			= 0;
		m_d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;
		m_d3dpp.EnableAutoDepthStencil		= true; 
		m_d3dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;
		m_d3dpp.Flags						= 0;
		m_d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
		m_d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	HRESULT hr = S_OK;
	hr = m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT,		// primary adapter
								D3DDEVTYPE_HAL,         // device type
								hRenderWnd,             // window associated with device
								D3DCREATE_SOFTWARE_VERTEXPROCESSING,  // vertex processing
								&m_d3dpp,				// present parameters
								&m_pd3dDevice);			// return created device

	if(FAILED(hr))
	{
		// try again using a 16-bit depth buffer
		m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hRenderWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&m_d3dpp,
									&m_pd3dDevice);
		if(FAILED(hr))
		{
			m_pD3D->Release();
			::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
			return FALSE;
		}
	}

	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	
	return	TRUE;
}
//------------------------------------------------
VOID CD3DEngine::SetFullScreen(int iWidth, int iHeight, HWND hRenderWnd, BOOL bFullScreen)
{
	SAF_RELEASE(m_pd3dDevice);
	SAF_RELEASE(m_pD3D);

	CreateD3DDevice(iWidth, iHeight, hRenderWnd, bFullScreen);
}
//------------------------------------------------
VOID CD3DEngine::SetViewport(DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, float fMinZ, float fMaxZ)
{
	if(NULL != m_pd3dDevice)
	{
		D3DVIEWPORT9 vp;
		vp.X = dwX;	vp.Y = dwY;
		vp.Width = dwWidth;	vp.Height = dwHeight;
		vp.MinZ = fMinZ;	vp.MaxZ = fMaxZ;

		m_pd3dDevice->SetViewport(&vp);
	}
}
//------------------------------------------------
VOID CD3DEngine::Render()
{
	if( NULL == m_pd3dDevice )
		return;

	// Clear the backbuffer to a blue color
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	// Begin the scene
	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{    

		// End the scene
		m_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}
//------------------------------------------------

