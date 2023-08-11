//-----------------------------------------
//	FileName:RectObject.cpp
//	Introduce:
//	Creator:Youfang Yao
//	Date:11-06-06
//-----------------------------------------
#include "stdafx.h"
#include <TCHAR.h>
#include "RectObject.h"
//-----------------------------------------
//	Constructor
//-----------------------------------------
CRectObject::CRectObject()
 : m_bVisible(TRUE),
   m_fWidth(0),
   m_fHeight(0),
   m_fRotAngle(0),
   m_fCenterPosX(0),
   m_fCenterPosY(0),
   m_eState(SOS_NORMAL),
   m_lpd3dDevice(NULL),
   m_pVB(NULL),
   m_bTexLoop(true),
   m_lLastTexTime(0),
   m_iCurTexIndex(0)
{
	lstrcpy(m_szName, _T("\0"));
	//
	for(int iCnt = 0; iCnt < SOS_COUNT; iCnt++)
		m_pTexture[iCnt] = NULL;
}
//-----------------------------------------
//	Distructor
//-----------------------------------------
CRectObject::~CRectObject()
{
	if( m_pVB != NULL )
		m_pVB->Release();
}
//-----------------------------------------


//-----------------------------------------
//To create the rect object
//-----------------------------------------
HRESULT	 CRectObject::Create(LPDIRECT3DDEVICE9 lpd3dDevice, LPCTSTR lpszName, 
		float fLeft, float fTop, float fRight, float fBottom, BOOL bVisible)
{
	if((m_lpd3dDevice == NULL) && (lpd3dDevice == NULL))
		return E_POINTER;

	m_lpd3dDevice = lpd3dDevice;

	//The object name
	if(lpszName != NULL)
		lstrcpy(m_szName, lpszName);

	//The object position
	SetRect(fLeft, fTop, fRight, fBottom);

	//The visible
	m_bVisible = bVisible;

	return S_OK;
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
	
	m_fWidth = fRight - fLeft;
	m_fHeight = fTop - fBottom;
	m_fCenterPosX = fLeft + m_fWidth / 2.0f;
	m_fCenterPosY = fBottom + m_fHeight / 2.0f;
	pVertex[0].x = -m_fWidth/2.0f;	pVertex[0].y = -m_fHeight/2.0f;	pVertex[0].z = 1.0f;
	pVertex[1].x = m_fWidth/2.0f;	pVertex[1].y = -m_fHeight/2.0f;	pVertex[1].z = 1.0f;
	pVertex[2].x = -m_fWidth/2.0f;	pVertex[2].y = m_fHeight/2.0f;	pVertex[2].z = 1.0f;
	pVertex[3].x = m_fWidth/2.0f;	pVertex[3].y = m_fHeight/2.0f;	pVertex[3].z = 1.0f;
	//
	pVertex[0].tu = TEX_COORDINATE_DELTA;		pVertex[0].tv = 1 - TEX_COORDINATE_DELTA;
	pVertex[1].tu = 1 - TEX_COORDINATE_DELTA;	pVertex[1].tv = 1 - TEX_COORDINATE_DELTA;
	pVertex[2].tu = TEX_COORDINATE_DELTA;		pVertex[2].tv = TEX_COORDINATE_DELTA;
	pVertex[3].tu = 1 - TEX_COORDINATE_DELTA;	pVertex[3].tv = TEX_COORDINATE_DELTA;

	m_pVB->Unlock();

	return S_OK;
}
//-----------------------------------------
//To set the texture object for the rect object
//-----------------------------------------
HRESULT CRectObject::SetTextureObject(CTexture* lpNorTex, CTexture* lpOnTex, CTexture* lpDownTex, CTexture* lpDisableTex, BOOL bLoop)
{
	if(m_lpd3dDevice == NULL)
		return E_POINTER;

	//The normal state texture object
	m_pTexture[SOS_NORMAL] = lpNorTex;

	//The mouse on state texture object
	if(NULL != lpOnTex)
		m_pTexture[SOS_MOUSEON] = lpOnTex;
	else
		m_pTexture[SOS_MOUSEON] = m_pTexture[SOS_NORMAL];

	//The mouse on state texture object
	if(NULL != lpDownTex)
		m_pTexture[SOS_MOUSEDOWN] = lpDownTex;
	else
		m_pTexture[SOS_MOUSEDOWN] = m_pTexture[SOS_MOUSEON];

	//The disable state texture object
	m_pTexture[SOS_DISABLE] = lpDisableTex;

	m_bTexLoop = bLoop;

	return S_OK;
}
//-----------------------------------------
//To change the texture object for the rect object based on state tpye
//-----------------------------------------
HRESULT CRectObject::ChangeTextureObject(UINT uState, CTexture* lpNewTex, BOOL bLoop)
{
	if(uState < SOS_NORMAL || uState > SOS_DISABLE)
		return S_FALSE;
	
	m_pTexture[uState] = lpNewTex;
	m_iCurTexIndex = 0;
	m_bTexLoop = bLoop;

	return S_OK;
}
//-----------------------------------------
//To render the rect object
//-----------------------------------------
void CRectObject::Render()
{
	if(!m_bVisible || m_lpd3dDevice == NULL)
		return;

    D3DXMATRIXA16 matWorld, matRot;
    D3DXMatrixIdentity( &matWorld );
	D3DXMatrixTranslation( &matWorld, m_fCenterPosX, m_fCenterPosY, 0);
	//
    D3DXMatrixIdentity( &matRot );
    D3DXMatrixRotationZ( &matRot, m_fRotAngle );
	D3DXMatrixMultiply( &matWorld, &matRot, &matWorld);
	m_lpd3dDevice->SetTransform( D3DTS_WORLD, &matWorld);
	
	long iTexTimeLen = 0;
	LPDIRECT3DTEXTURE9 lpTexture = NULL;
	if(m_pTexture[m_eState])
	{
		lpTexture = m_pTexture[m_eState]->GetTexturePointer(m_iCurTexIndex, iTexTimeLen, m_bTexLoop);

		if(lpTexture)
		{
			D3DSURFACE_DESC sd;
			lpTexture->GetLevelDesc(0, &sd);
			if ((sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4))
			{
				m_lpd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			}
			else
			{
				m_lpd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			}
			m_lpd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			m_lpd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	
		}
	}
	m_lpd3dDevice->SetTexture(0, lpTexture);

	m_lpd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
    m_lpd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

    m_lpd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	DWORD dwCurTime = GetTickCount();
	if((dwCurTime - m_lLastTexTime) >= iTexTimeLen)
	{
		m_iCurTexIndex++;
		m_lLastTexTime = dwCurTime;
	}

    D3DXMatrixIdentity( &matWorld );
    m_lpd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
}
//-----------------------------------------
//To get the object name string
//-----------------------------------------
LPCTSTR CRectObject::GetName()
{
	return m_szName;
}
//-----------------------------------------
//To set the visible attribute
//-----------------------------------------
void CRectObject::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
}
//-----------------------------------------
//To get the visible attribute
//-----------------------------------------
BOOL CRectObject::GetVisible()
{
	return m_bVisible;
}
//-----------------------------------------
//To check a point is in the object rect or not
//-----------------------------------------
BOOL CRectObject::PntInObject(float fX, float fY)
{
	float fLeft = m_fCenterPosX - m_fWidth/2.0f;
	float fRight = m_fCenterPosX + m_fWidth/2.0f;
	float fTop = m_fCenterPosY + m_fHeight/2.0f;
	float fBottom = m_fCenterPosY - m_fHeight/2.0f;

	if( (fX > fLeft) && (fX < fRight) &&
		(fY > fBottom) && (fY < fTop))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//-----------------------------------------
//To move the object by step
//-----------------------------------------
void CRectObject::MoveByStep(float fStepX, float fStepY)
{
	m_fCenterPosX += fStepX;
	m_fCenterPosY += fStepY;
}
//-----------------------------------------
//Move the object to a new position
//-----------------------------------------
void CRectObject::MoveTo(float fPosX, float fPosY)
{
	m_fCenterPosX = fPosX;
	m_fCenterPosY = fPosY;
}
//-----------------------------------------
//To rotate the object by angle
//-----------------------------------------
void CRectObject::RotateByStep(float fAngle)
{
	m_fRotAngle  = fAngle;
}
//-----------------------------------------
//Set the rect object state type
//-----------------------------------------
void CRectObject::SetState(UINT uState)
{
	if(uState < SOS_NORMAL || uState >= SOS_COUNT)
		return;

	m_eState = (SCENE_OBJECT_STATE)uState;
}
//-----------------------------------------
//Get the rect object current state type
//-----------------------------------------
UINT CRectObject::GetState()
{
	return m_eState;
}
//-----------------------------------------