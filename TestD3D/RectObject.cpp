//-----------------------------------------
//	FileName:RectObject.cpp
//	Introduce:
//	Creator:Youfang Yao
//	Date:11-06-06
//-----------------------------------------
#include "stdafx.h"
#include "RectObject.h"
//-----------------------------------------
//	Constructor
//-----------------------------------------
CRectObject::CRectObject()
 : m_iLeft(0),
   m_iTop(0),
   m_iRight(0),
   m_iBottom(0),
   m_lpd3dDevice(NULL),
   m_pVB(NULL),
   m_pTexture(NULL)
{
}
//-----------------------------------------
//	Constructor
//-----------------------------------------
CRectObject::CRectObject(LPDIRECT3DDEVICE9 lpd3dDevice)
 : m_iLeft(0),
   m_iTop(0),
   m_iRight(0),
   m_iBottom(0),
   m_pVB(NULL),
   m_pTexture(NULL)
{
	m_lpd3dDevice = lpd3dDevice;
}
//-----------------------------------------
//	Distructor
//-----------------------------------------
CRectObject::~CRectObject()
{
	if( m_pTexture != NULL )
		m_pTexture->Release();

	if( m_pVB != NULL )
		m_pVB->Release();
}
//-----------------------------------------


//-----------------------------------------
//To set the DirectD3DDevice interface
//-----------------------------------------
void CRectObject::SetD3DDevice(LPDIRECT3DDEVICE9 lpd3dDevice)
{
	m_lpd3dDevice = lpd3dDevice;
}
//-----------------------------------------
//	To set the rect object position and size
//-----------------------------------------
HRESULT CRectObject::SetRect(float fLeft, float fTop, float fRight, float fBottom)
{
	if(m_lpd3dDevice == NULL)
		return E_POINTER;

	HRESULT hr = S_OK;

	//To create the vertex buffer
	if(m_pVB)
		m_pVB->Release();
	hr = m_lpd3dDevice->CreateVertexBuffer( 4* sizeof(CUSTOMVERTEX),
											0, D3DFVF_CUSTOMVERTEX,
											D3DPOOL_DEFAULT, &m_pVB, NULL);
	if(FAILED(hr))
		return hr;

	//To fill the vertex buffer
	CUSTOMVERTEX* pVertex = NULL;
	hr = m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	if(FAILED(hr))
		return hr;

	pVertex[0].x = fLeft;	pVertex[0].y = fBottom;	pVertex[0].z = 1.0f;
	pVertex[1].x = fRight;	pVertex[1].y = fBottom; pVertex[1].z = 1.0f;
	pVertex[2].x = fLeft;	pVertex[2].y = fTop;	pVertex[2].z = 1.0f;
	pVertex[3].x = fRight;	pVertex[3].y = fTop;	pVertex[3].z = 1.0f;
	//
	pVertex[0].tu = 0;		pVertex[0].tv = 1;
	pVertex[1].tu = 1;		pVertex[1].tv = 1;
	pVertex[2].tu = 0;		pVertex[2].tv = 0;
	pVertex[3].tu = 1;		pVertex[3].tv = 0;

	m_pVB->Unlock();

	return S_OK;
}
//-----------------------------------------
//To set the texture object for the rect object
//-----------------------------------------
HRESULT CRectObject::SetTextureFile(LPCTSTR lpszTexture)
{
	if(m_lpd3dDevice == NULL)
		return E_POINTER;

	HRESULT hr = S_OK;

	//To load the texture from file
	if(m_pTexture)
		m_pTexture->Release();

	hr = D3DXCreateTextureFromFile(m_lpd3dDevice, lpszTexture, &m_pTexture);
	if(FAILED(hr))
		return hr;

	return S_OK;
}
//-----------------------------------------
//To set the texture object for the rect object
//-----------------------------------------
HRESULT CRectObject::SetTextureObject(LPDIRECT3DTEXTURE9 lpTexture)
{
	if(m_lpd3dDevice == NULL || lpTexture == NULL)
		return E_POINTER;

	if(m_pTexture)
		m_pTexture->Release();
	m_pTexture = lpTexture;

	return S_OK;
}
//-----------------------------------------
//To render the rect object
//-----------------------------------------
void CRectObject::Render()
{
	if(m_lpd3dDevice == NULL)
		return;

	if(m_pTexture)
	{
		m_lpd3dDevice->SetTexture(0, m_pTexture);
	}

	m_lpd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
    m_lpd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_lpd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//-----------------------------------------