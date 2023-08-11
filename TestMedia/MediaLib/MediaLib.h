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
public:
	CMusic(){};
	virtual ~CMusic(){};
	//--------------------------------------------------
public:
	/*
		Name:LoadFromFile
		Introduce:Set the music file name for the CMusic object
		Param:lpFileName:i:The music full file name
		Return:If successful return S_OK,otherwise return an error code
	*/
	virtual HRESULT LoadFromFile(LPCTSTR lpFileName) = 0;

	/*
		Name:Play
		Introduce:To play the music
		Param:bLoop:i:If is true, the music will play loop
		Return:If successful return S_OK,otherwise return an error code
	*/
	virtual HRESULT Play(BOOL bLoop = FALSE) = 0;
	
	/*
		Name:Pause
		Introduce:To pause the music
		Param:N/A
		Return:If successful return S_OK,otherwise return an error code
	*/
	virtual HRESULT Pause() = 0;

	/*
		Name:Resume
		Introduce:To resume the music
		Param:N/A
		Return:If successful return S_OK,otherwise return an error code
	*/
	virtual HRESULT Resume() = 0;

	/*
		Name:Stop
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
//	Name:CImage
//	Introduce:The image object interface definition.a CImage
//	          object can render a picture file on DC by using DirectShow
//------------------------------------------------------
class CImage
{
public:
	CImage(){};
	virtual ~CImage(){};
	//--------------------------------------------------
public:
	/*
		Name:LoadFormFile
		Introduce:Set the image file name for the CImage object
		Param:lpFileName:i:The image full file name
		Return:If successful return S_OK,otherwise return an error code
	*/
	virtual HRESULT LoadFormFile(LPCTSTR lpFileName) = 0;

	/*
		Name:Draw
		Introduce:To draw the image object on a DC
		Param:hdc:i:The dc handle
		      left,top,width,height:i:The draw rect on DC
		Return:If successful return S_OK,otherwise return an error code
	*/
	virtual HRESULT Draw(HDC hdc, int left, int top, int width, int height) = 0;
	//--------------------------------------------------
};
//------------------------------------------------------



//------------------------------------------------------
/*The export functions of the dll*/
//------------------------------------------------------
//To create and return a CMusic object pointer
MEDIALIB_API CMusic* MediaLib_CreateMusic();
//To destroy the specific CMusic object pointer
MEDIALIB_API void MediaLib_DestroyMusic(CMusic* pMusic);
//To create and return a CImage object pointer
MEDIALIB_API CImage* MediaLib_CreateImage();
//To destroy the specific CImage object pointer
MEDIALIB_API void MediaLib_DestroyImage(CImage* pImage);
//------------------------------------------------------
#endif	//_MEDIALIB_H_
//------------------------------------------------------

