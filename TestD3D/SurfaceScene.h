//-----------------------------------------
//	FileName:SurfaceScene.h
//	Introduce:
//	Creator:Youfang Yao
//	Date:11-06-06
//-----------------------------------------
#ifndef	_SURFACESCENE_H_
#define	_SURFACESCENE_H_
//-----------------------------------------
#include <d3d9.h>
#include "RectObject.h"
//-----------------------------------------
class CSurfaceScene
{
public:
	CSurfaceScene();
	virtual ~CSurfaceScene();
	//-------------------------------------
public:
	//To initliaze the scene
	HRESULT		InitScene(HWND hWnd, RECT rc);
	//To set the project size for current scene
	HRESULT		SetProjectSize(float fWidth, float fHeight);
	//To add a rect object into the scene
	HRESULT		AddRectObject(LPCTSTR lpszName, float fLeft, float fTop, float fRight, float fBottom, LPCTSTR lpszTexture, BOOL bVisible = TRUE);
	//
	HRESULT		SetProjectType(UINT uType, float fWidth, float fHeight);
	HRESULT		MoveCamera(float x, float y, float z);
	//-------------------------------------	
private:
	HWND				m_hRenderWnd;	//The render window handle
	RECT				m_stuRectOnWnd;	//The render rect on window
	//
	LPDIRECT3D9         m_pD3D;			//Used to create the D3DDevice
	LPDIRECT3DDEVICE9	m_pd3dDevice;	//Our rendering device
	//
	float				m_fCameraX;
	float				m_fCameraY;
	float				m_fCameraZ;
};
//-----------------------------------------
#endif	//_SURFACESCENE_H_
//-----------------------------------------