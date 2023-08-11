//-----------------------------------------
//	FileName:SurfaceSceneImpl.h
//	Introduce:
//	Creator:Youfang Yao
//	Date:11-06-06
//-----------------------------------------
#ifndef	_SURFACESCENEIMPL_H_
#define	_SURFACESCENEIMPL_H_
//-----------------------------------------
#include "MediaLib.h"
#include <d3d9.h>
#include <vector>
#include "RectObject.h"
#include "Texture.h"
//-----------------------------------------
using namespace std;
//-----------------------------------------
//	Name:CSurfaceSceneImpl
//	Introduce:The surface scene implement class
//-----------------------------------------
class CSurfaceSceneImpl : public CSurfaceScene
{
public:
	CSurfaceSceneImpl();
	virtual ~CSurfaceSceneImpl();
	//-------------------------------------
public:
	//To initliaze the scene
	HRESULT		InitScene(HWND hWnd, UINT uScWidth, UINT uScHeight, RECT rcViewPoint);
	//To clear or rect objects and texture objects
	void		Clear(DWORD dwClearType);
	//To set the view size for current scene
	HRESULT		SetViewSize(float fLeft, float fRight, float fTop, float fBottom);
	//Set the texture file path string
	HRESULT		SetTextureFilePath(LPCTSTR lpszPath);
	//To set the background rect object
	HRESULT		AddBackground(LPCTSTR lpszName, float fLeft, float fTop, float fRight, float fBottom, LPCTSTR lpszTexture, BOOL bTexLoop, BOOL bVisible = TRUE);
	//To add a rect object into the scene
	HRESULT		AddRectObject(LPCTSTR lpszName, float fLeft, float fTop, float fRight, float fBottom, LPCTSTR lpszNorTexture, LPCTSTR lpszOnTexture = NULL, LPCTSTR lpszDownTexture = NULL, LPCTSTR lpszDisableTexture = NULL, BOOL bTexLoop = FALSE, BOOL bVisible = TRUE);
	//To render the scene
	HRESULT		Render();
	//-------------------------------------	
	//To get a object name by mouse point
	LPCTSTR		GetObjNameByPnt(int x, int y);
	//To set a object visible or not by name string
	void		SetObjVisible(LPCTSTR lpszObjName, BOOL bVisible);
	//To create a texutre object from file and add it to the scene texture objects list
	HRESULT		LoadTextureFile(LPCTSTR lpszTextureFile);
	//To chang the background object texture
	HRESULT		ChangeBkTextrue(LPCTSTR lpszNewTextureFile, BOOL bTexLoop);
	//To chang a rect object texture
	HRESULT		ChangeObjTexture(LPCTSTR lpszObjName, UINT uState, LPCTSTR lpszNewTextureFile, BOOL bTexLoop);
	//Move the rect object by steps
	HRESULT		MoveObject(LPCTSTR lpszObjName, float fStepX, float fStepY);
	//Move a rect object to a new position
	HRESULT		MoveObjectTo(LPCTSTR lpszObjName, float fPosX, float fPosY);
	//Rotate the rect object by angle	
	HRESULT		RotateObject(LPCTSTR lpszObjName, float fAngle);
	//Set a object state based on object name
	HRESULT		SetObjectState(LPCTSTR lpszObjName, UINT uState);
	//Get a object state based on object name
	UINT		GetObjectState(LPCTSTR lpszObjName);
	//-------------------------------------	
private:
	//To find the rect object in objects list
	CRectObject*	GetRectObject(LPCTSTR lpszName);
	//To find a texture object in the textures list
	CTexture*		GetTextureObj(LPCTSTR lpszTextureFile);
	//To gen a texture object,if the texture object in the list, DO NOT load it from file again
	HRESULT			GenTexture(LPCTSTR lpszTexFile, CTexture** lpTexture, BOOL bAddToList = TRUE);
	//-------------------------------------
private:
	HWND					m_hRenderWnd;				//The render window handle
	float					m_fViewLeft;				//The current view min x for the scene
	float					m_fViewRight;				//The current view max x for the scene
	float					m_fViewTop;					//The current view max y for the scene
	float					m_fViewBottom;				//The current view min y for the scene
	//
	LPDIRECT3D9				m_pD3D;						//Used to create the D3DDevice
	LPDIRECT3DDEVICE9		m_pd3dDevice;				//Our rendering device
	//
	TCHAR					m_szTexFilePath[MAX_PATH];	//The dynamic texture files path
	CRectObject*			m_pBackground;				//The Background object
	CTexture*				m_pBKTexture;				//
	vector<CRectObject*>	m_lstRectObjects;			//The rect objects list
	vector<CTexture*>		m_lstTexture;				//The texture objects list
	//-------------------------------------	
};
//-----------------------------------------
#endif	//_SURFACESCENEIMPL_H_
//-----------------------------------------