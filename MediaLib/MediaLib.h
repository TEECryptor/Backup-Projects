//------------------------------------------------------
//	FileName:MediaLib.h
//	Introduce:The MediaLib definition file
//	Creator:Youfang yao
//	Date:10-26-06
//	Version:1.0V
//------------------------------------------------------
#ifndef _MEDIALIB_H_
#define _MEDIALIB_H_
//------------------------------------------------------
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MEDIALIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MEDIALIB_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//------------------------------------------------------
#define MEDIALIB_EXPORTS

#ifdef MEDIALIB_EXPORTS
#define MEDIALIB_API __declspec(dllexport)
#else
#define MEDIALIB_API __declspec(dllimport)
#endif
//------------------------------------------------------


//------------------------------------------------------
//	Name:MUSIC_PLAY_STATE
//	Introduce:Music play state definition
//------------------------------------------------------
enum MUSIC_PLAY_STATE
{		
	MPS_STOPPED	= 0,
	MPS_PAUSED	= 1,
	MPS_RUNNING	= 2
};
//------------------------------------------------------
//	Name:CMusic
//	Introduce:The music object interface definition.A CMusic
//		      object can render a music file use DirectShow interfaces
//------------------------------------------------------
class CMusic
{
	//--------------------------------------------------
public:
	CMusic(){};
	virtual ~CMusic(){};
	//--------------------------------------------------
public:
	/*	Name:LoadFromFile
		Introduce:Set the music file name for the CMusic object
		Param:lpFileName:i:The music full file name
		Return:If successful return S_OK,otherwise return an error code
	*/
	virtual HRESULT LoadFromFile(LPCTSTR lpFileName) = 0;

	/*	Name:Play
		Introduce:To play the music
		Param:bLoop:i:If is true, the music will play loop
		Return:If successful return S_OK,otherwise return an error code
	*/
	virtual HRESULT Play(BOOL bLoop = FALSE) = 0;
	
	/*	Name:Pause
		Introduce:To pause the music
		Param:N/A
		Return:If successful return S_OK,otherwise return an error code
	*/
	virtual HRESULT Pause() = 0;

	/*	Name:Resume
		Introduce:To resume the music
		Param:N/A
		Return:If successful return S_OK,otherwise return an error code
	*/
	virtual HRESULT Resume() = 0;

	/*	Name:Stop
		Introduce:To stop the music
		Param:N/A
		Return:If successful return S_OK,otherwise return an error code
	*/
	virtual HRESULT Stop() = 0;

	/*	Name:GetState
		Introduce:To get the muic player current state
		Param:N/A
		Return:If successful return a state,otherwise return an error code
	*/
	virtual UINT GetState() = 0;
	//--------------------------------------------------		
};
//------------------------------------------------------


//------------------------------------------------------
//	Name:SCENE_CLEAR_TYPE
//	Introduce:Scene clear type definiton
//------------------------------------------------------
enum SCENE_CLEAR_TYPE
{
	SCT_BK			= 0x00000001,
	SCT_OBJECT		= 0x00000010,
	SCT_TEXTURE		= 0x00000100
};
//------------------------------------------------------
//	Name:SCENE_OBJECT_STATE
//	Introduce:Scene object state definiton
//------------------------------------------------------
enum SCENE_OBJECT_STATE
{
	SOS_NORMAL		= 0,
	SOS_MOUSEON		= 1,
	SOS_MOUSEDOWN	= 2,
	SOS_DISABLE		= 3,
	SOS_COUNT
};
//------------------------------------------------------
//	Name:CSurfaceScene
//	Introduce:The suface scene object interface definition.A surface scene
//		      object can render a 2D graphic scene on a window
//------------------------------------------------------
class CSurfaceScene
{
	//--------------------------------------------------
public:
	CSurfaceScene(){};
	virtual ~CSurfaceScene(){};
	//--------------------------------------------------
public:
	/*	Name:InitScene
		Introduce:To initialize the scene
		Param:hWnd:i:The render window
		      uScWidth,uScHeight:i:Scene size
			  rcViewPoint:i:Scene position on render window
		Return:Return S_OK if successful, otherwise return an error code
	*/
	virtual HRESULT	InitScene(HWND hWnd, UINT uScWidth, UINT uScHeight, RECT rcViewPoint) = 0;

	/*	Name:Clear
		Introduce:To clear all rect objects and texture objects
		Param:bClearBackground:i:Clear the background object or not
		Return:N/A
	*/
	virtual void Clear(DWORD dwClearType) = 0;

	/*	Name:SetViewSize
		Introduce:To set the view size for current scene
		Param:fLeft,fRight,fTop,fBottom:i:The scene view size
		Return:Return S_OK if successful, otherwise return an error code
	*/
	virtual HRESULT SetViewSize(float fLeft, float fRight, float fTop, float fBottom) = 0;

	/*	Name:SetTextureFilePath
		Introduce:Set the texture file path string
		Param:lpszPath:i:The texture object files path string
		Return:Return S_OK if successful, otherwise return an error code
	*/
	virtual HRESULT	SetTextureFilePath(LPCTSTR lpszPath) = 0;

	/*	Name:AddBackground
		Introduce:Add the background rect object, the background object will be renderd at first for all time
		Param:lpszName:i:Background object name string
		      fLeft, fTop, fRight, fBottom:i:The background object position in scene
			  lpszTexture:i:Texture file name strings
		      bTexLoop:i:If is dynimate texture, show it as loop or not
			  bVisible:i:Is visible or not
		Return:Return S_OK if successful, otherwise return an error code
	*/
	virtual HRESULT	AddBackground(LPCTSTR lpszName, float fLeft, float fTop, float fRight, float fBottom, LPCTSTR lpszTexture, BOOL bTexLoop, BOOL bVisible = TRUE) = 0;
	
	/*	Name:AddRectObject
		Introduce:Add a rect object in the scene
		Param:lpszName:i:Rect object name string
		      fLeft, fTop, fRight, fBottom:i:The rect object position in scene
			  lpszTexture:i:Texture file name strings
		      bTexLoop:i:If is dynimate texture, show it as loop or not
			  bVisible:i:Is visible or not
		Return:Return S_OK if successful, otherwise return an error code
	*/
	virtual HRESULT	AddRectObject(	LPCTSTR lpszName, 
									float fLeft, float fTop, float fRight, float fBottom, 
									LPCTSTR lpszNorTexture, 
									LPCTSTR lpszOnTexture = NULL,
									LPCTSTR lpszDownTexture = NULL,
									LPCTSTR lpszDisableTexture = NULL,
									BOOL bTexLoop = FALSE, 
									BOOL bVisible = TRUE) = 0;

	/*	Name:Render
		Introduce:Render the scene
		Param:N/A
		Return:Return S_OK if successful, otherwise return an error code
	*/
	virtual HRESULT	Render() = 0;

	/*	Name:GetObjNameByPnt
		Introduce:To get a object name by mouse point
		Param:x, y:i:The point position based on the render window
		Return:The selected object name, return NULL when no any object is selected
	*/
	virtual LPCTSTR	GetObjNameByPnt(int x, int y) = 0;

	/*	Name:SetObjVisible
		Introduce:To set a object visible or not by name string
		Param:lpszObjName:i:The object name string
		      bVisible:i:Set TRUE to show the object
		Return:N/A
	*/
	virtual void SetObjVisible(LPCTSTR lpszObjName, BOOL bVisible) = 0;

	/*	Name:LoadTextureFile
		Introduce:To create a texutre object from file and add it to the scene texture objects list
		Param:lpszTextureFile:i:The texture file name strings
		Return:Return S_OK if successful, otherwise return an error code
	*/
	virtual HRESULT	LoadTextureFile(LPCTSTR lpszTextureFile) = 0;

	/*	Name:ChangeBkTextrue
		Introduce:To chang the background object texture
		Param:lpszNewTextureFile:i:The new texture object file name string
		      bTexLoop:i:If is dynimate texture, show it as loop or not
		Return:Return S_OK if successful, otherwise return a error code
	*/
	virtual HRESULT ChangeBkTextrue(LPCTSTR lpszNewTextureFile, BOOL bTexLoop) = 0;

	/*	Name:ChangeObjTexture
		Introduce:To chang a rect object texture
		Param:lpszObjName:i:The rect object name string
		      lpszNewTextureFile:i:The new texture object file name string
		      bTexLoop:i:If is dynimate texture, show it as loop or not
		Return:Return S_OK if successful, otherwise return a error code
	*/
	virtual HRESULT	ChangeObjTexture(LPCTSTR lpszObjName, UINT uState, LPCTSTR lpszNewTextureFile, BOOL bTexLoop) = 0;

	/*	Name:MoveObject
		Introduce:Move the rect object by steps
		Param:lpszObjName:i:The object name
		      fStepX:i:The step in X coordinate
		      fStepY:i:The step in X coordinate
		Return:Return S_OK if successful, otherwise return a error code
	*/
	virtual HRESULT MoveObject(LPCTSTR lpszObjName, float fStepX, float fStepY) = 0;

	
	/*	Name:MoveObjectTo
		Introduce:Move a rect object to a new position
		Param:lpszObjName:i:The object name
		      fPosX:i:The new position in X coordinate
		      fPosY:i:The new position in X coordinate
		Return:Return S_OK if successful, otherwise return a error code
	*/
	virtual HRESULT	MoveObjectTo(LPCTSTR lpszObjName, float fPosX, float fPosY) = 0;

	/*
		Name:RotateObject
		Introduce:Rotate the rect object by angle
		Param:lpszObjName:i:The object name
		      fAngle:i:The rotate angle
		Return:Return S_OK if successful, otherwise return a error code
	*/
	virtual HRESULT RotateObject(LPCTSTR lpszObjName, float fAngle) = 0;

	/*	Name:SetObjectState
		Introduce:Set a object state based on object name
		Param:lpszObjName:i:Object name string
			  uState:i:The state type
		Return:Return S_OK if successful, otherwise return a error code
	*/
	virtual HRESULT SetObjectState(LPCTSTR lpszObjName, UINT uState) = 0;

	/*	Name:GetObjectState
		Introduce:Get a object state based on object name
		Param:lpszObjName:i:Object name string
		Return:Return the current state for the object
	*/
	virtual UINT GetObjectState(LPCTSTR lpszObjName) = 0;
	//--------------------------------------------------
};
//------------------------------------------------------



//------------------------------------------------------
//	Name:CFlashPlayer
//	Introduce:The Flash Player interface object use IShockwaveFlash
//			  interface to play a flash movie file
//------------------------------------------------------
class CFlashPlayer
{
public:
	CFlashPlayer(){};
	virtual ~CFlashPlayer(){};
	//--------------------------------------------------
public:
	/*	Name:Create
		Introduce:To create a CFlashPlayer object
		Param:hParentWnd:i:The Flash parent window
		      iLeft,iTop,iWidth,iHeight:i:The flash position on parent window
			  bVisible:i:Is visible or not
			  uEndMsg:i:When flash playing is end, send the message to parent window
		Return:Return S_OK if create successful, otherwise return an error code
	*/
	virtual HRESULT Create(HWND hParentWnd, int iLeft, int iTop, int iWidth, int iHeight, bool bVisible, UINT uEndMsg) = 0;

	/*	Name:SetVisible
		Introduce:Set the flash control is visible or not
		Param:bVisible:i:bVisible = true to show the flash control
		Return:N/A
	*/
	virtual void SetVisible(bool bVisible) = 0;

	/*	Name:IsPlaying
		Introduce:To check the flash control is playing or not now
		Param:N/A
		Return:If the flash control is playing return true, otherwise return false
	*/
	virtual bool IsPlaying() = 0;

	/*	Name:Play
		Introduce:To play the flash control
		Param:lpMovieFile:i:The flash movie file name
              bLoop:i:Loop or not
		Return:Return S_OK if successful, otherwise return an error code
	*/
	virtual HRESULT Play(LPCTSTR lpMovieFile, bool bLoop) = 0;

	/*	Name:Stop
		Introduce:To play the flash control
		Param:N/A
		Return:Return S_OK if successful, otherwise return an error code
	*/
	virtual HRESULT Stop() = 0;
};
//------------------------------------------------------



//------------------------------------------------------
/*The export functions of the dll*/
//------------------------------------------------------
//To create and return a CMusic object pointer
MEDIALIB_API CMusic* MediaLib_CreateMusic();
//To destroy the specific CMusic object pointer
MEDIALIB_API void MediaLib_DestroyMusic(CMusic* pMusic);
//To create and return a CSurfaceScene object pointer
MEDIALIB_API CSurfaceScene* MediaLib_CreateScene();
//To destroy the specific CSurfaceScene object pointer
MEDIALIB_API void MediaLib_DestroyScene(CSurfaceScene* pScene);
//To create and return a CFlashPlayer object pointer
MEDIALIB_API CFlashPlayer* MediaLib_CreateFlash();
//To destroy the specific CFlashPlayer object pointer
MEDIALIB_API void MediaLib_DestroyFlash(CFlashPlayer* pFlashPlayer);
//------------------------------------------------------
#endif	//_MEDIALIB_H_
//------------------------------------------------------

