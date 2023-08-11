//-----------------------------------------
//	FileName:RectObject.h
//	Introduce:
//	Creator:Youfang Yao
//	Date:11-06-06
//-----------------------------------------
#ifndef	_RECTOBJECT_H_
#define	_RECTOBJECT_H_
//-----------------------------------------
#include <d3dx9.h>
#include "Texture.h"
#include "MediaLib.h"
//-----------------------------------------
#define	MAX_NAME_LEN			64
#define	TEX_COORDINATE_DELTA	0.001f
//-----------------------------------------
struct CUSTOMVERTEX
{
	float x, y, z;		//The vertex coordinate
	float tu, tv;		//The texture coordinate
};
//-----------------------------------------
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)
//-----------------------------------------
class CRectObject
{
public:
	CRectObject();
	virtual ~CRectObject();
	//-------------------------------------
public:
	//To create the rect object
	virtual HRESULT	Create(LPDIRECT3DDEVICE9 lpd3dDevice, LPCTSTR lpszName, float fLeft, float fTop, float fRight, float fBottom, BOOL bVisible = TRUE);
	//To set the rect object position and size
	virtual HRESULT	SetRect(float fLeft, float fTop, float fRight, float fBottom);
	//To set the texture object for the rect object
	virtual HRESULT	SetTextureObject(CTexture* lpNorTex, CTexture* lpOnTex, CTexture* lpDownTex = NULL, CTexture* lpDisableTex = NULL, BOOL bLoop = true);
	//To chage texture object for the rect object based on state tpye
	virtual HRESULT	ChangeTextureObject(UINT uState, CTexture* lpNewTex, BOOL bLoop);
	//To render the rect object
	virtual void Render();
	//To get the object name string
	virtual LPCTSTR GetName();
	//To set the visible attribute
	virtual void SetVisible(BOOL bVisible);
	//To get the visible attribute
	virtual BOOL GetVisible();
	//To check a point is in the object rect or not
	virtual BOOL PntInObject(float fX, float fY);
	//To move the object by step
	virtual void MoveByStep(float fStepX, float fStepY);
	//Move the object to a new position
	virtual void MoveTo(float fPosX, float fPosY);
	//To rotate the object by step
	virtual void RotateByStep(float fAngle);
	//Set the rect object state type
	virtual void SetState(UINT uState);
	//Get the rect object current state type
	virtual UINT GetState();
	//-------------------------------------
protected:
	LPDIRECT3DDEVICE9		m_lpd3dDevice;			//The D3DDevice interface
	LPDIRECT3DVERTEXBUFFER9 m_pVB;					//Buffer to hold vertices
	CTexture*				m_pTexture[SOS_COUNT];	//Our texture
	BOOL					m_bTexLoop;				//Loop to show the texture or not, use for dynimate texture
	//
	TCHAR					m_szName[MAX_NAME_LEN];
	BOOL					m_bVisible;				//The visible attribute
	float					m_fWidth;				//The rect object width
	float					m_fHeight;				//The rect object height
	float					m_fRotAngle;			//The current rotate angle for this rect object
	float					m_fCenterPosX;			//The current center point for this rect object
	float					m_fCenterPosY;			//The current center point for this rect object
	SCENE_OBJECT_STATE		m_eState;				//The object state
	//
	long					m_lLastTexTime;			//For dynamic texture
	int						m_iCurTexIndex;			//For dynamic texture
	//-------------------------------------
};
//-----------------------------------------
#endif	//_RECTOBJECT_H_
//-----------------------------------------