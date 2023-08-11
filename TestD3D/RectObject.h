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
	CRectObject(LPDIRECT3DDEVICE9 lpd3dDevice);
	virtual ~CRectObject();
	//-------------------------------------
public:
	//To set the DirectD3DDevice interface
	void		SetD3DDevice(LPDIRECT3DDEVICE9 lpd3dDevice);
	//To set the rect object position and size
	HRESULT		SetRect(float fLeft, float fTop, float fRight, float fBottom);
	//To set the texture object file name for the rect object
	HRESULT		SetTextureFile(LPCTSTR lpszTexture);
	//To set the texture object for the rect object
	HRESULT		SetTextureObject(LPDIRECT3DTEXTURE9	lpTexture);
	//To render the rect object
	void		Render();
	//-------------------------------------
public:
	LPDIRECT3DDEVICE9		m_lpd3dDevice;	//The D3DDevice interface
	LPDIRECT3DVERTEXBUFFER9 m_pVB;			//Buffer to hold vertices
	LPDIRECT3DTEXTURE9      m_pTexture;		//Our texture
	//
	int						m_iLeft;
	int						m_iTop;
	int						m_iRight;
	int						m_iBottom;
	//-------------------------------------
};
//-----------------------------------------
#endif	//_RECTOBJECT_H_
//-----------------------------------------