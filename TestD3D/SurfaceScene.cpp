//-----------------------------------------
//	FileName:SurfaceScene.cpp
//	Introduce:
//	Creator:Youfang Yao
//	Date:11-06-06
//-----------------------------------------
#include "stdafx.h"
#include "SurfaceScene.h"
//-----------------------------------------
//Constructor
//-----------------------------------------
CSurfaceScene::CSurfaceScene()
 : m_pD3D(NULL),
   m_pd3dDevice(NULL)
{
	m_fCameraX = 0.0f;
	m_fCameraY = 0.0f;
	m_fCameraZ = 0.0f;
};
//-----------------------------------------
//Distructor
//-----------------------------------------
CSurfaceScene::~CSurfaceScene()
{
	if(m_pd3dDevice)
		m_pd3dDevice->Release();
	if(m_pD3D)
		m_pD3D->Release();
};
//-----------------------------------------	



//-----------------------------------------	
//To initliaze the scene
//-----------------------------------------	
HRESULT	CSurfaceScene::InitScene(HWND hWnd, RECT rc)
{ 
	if(hWnd == NULL)
		return E_POINTER;

	m_hRenderWnd = hWnd;
	m_stuRectOnWnd = rc;

	HRESULT hr = S_OK;   
	
	//To create the D3D interface
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D)
        return E_FAIL;

	//To create the D3D device interface
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	//
	hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                               D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                               &d3dpp, &m_pd3dDevice);
    if(FAILED(hr))
        return hr;

	//To set the scence states
    //m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	return S_OK;
}
//-----------------------------------------	
//To set the project size for current scene
//-----------------------------------------	
HRESULT	CSurfaceScene::SetProjectSize(float fWidth, float fHeight)
{
	if(NULL == m_pd3dDevice)
        return E_POINTER;

	//To set the world matrix
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity( &matWorld );
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	//To set the project matrix
    D3DXMATRIXA16 matProj;
	D3DXMatrixOrthoLH(&matProj, fWidth, fHeight, 0, 1.0f);
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj);

	return S_OK;
}
//-----------------------------------------	
//To add a rect object into the scene
//-----------------------------------------	
HRESULT	CSurfaceScene::AddRectObject(LPCTSTR lpszName, 
		float fLeft, float fTop, float fRight, float fBottom, 
		LPCTSTR lpszTexture, BOOL bVisible)
{
	HRESULT hr = S_OK;

	return S_OK;
}
//-----------------------------------------	
HRESULT	CSurfaceScene::SetProjectType(UINT uType, float fWidth, float fHeight)
{
	//To set the world matrix
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity( &matWorld );
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	//For perspective
	if(uType == 0)
	{	
		//Look at
		D3DXVECTOR3 vEyePt( m_fCameraX, m_fCameraY,m_fCameraZ );
		D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
		m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

		//To set the project matrix
		D3DXMATRIXA16 matProj;
		D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
		m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj);
	}
	//For Ortho
	else
	{
		D3DXMATRIXA16 matView;
		D3DXMatrixIdentity( &matView );
		m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
		//
		//To set the project matrix
		D3DXMATRIXA16 matProj;
		D3DXMatrixOrthoLH(&matProj, fWidth, fHeight, 0, 1.0f);
		m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj);
	}

	return S_OK;
}
//-----------------------------------------	
HRESULT	CSurfaceScene::MoveCamera(float x, float y, float z)
{
	m_fCameraX += x;
	m_fCameraY += y;
	m_fCameraZ += z;

	return S_OK;
}
//-----------------------------------------	
	