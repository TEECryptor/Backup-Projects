//------------------------------------------------------
//	FileName:MusicImpl.h
//	Introduce:The class CMusicImpl definition file
//	Creator:Youfang yao
//	Date:10-26-06
//	Version:1.0V
//------------------------------------------------------
#ifndef _MUSICIMPL_H_
#define _MUSICIMPL_H_
//------------------------------------------------------
#include "MediaLib.h"
#include <dshow.h>
//------------------------------------------------------
#define WM_DSNOTIFY		WM_APP + 1
//------------------------------------------------------
class CMusicImpl : public CMusic
{
public:
	CMusicImpl();
	virtual ~CMusicImpl();
	//--------------------------------------------------
public:
	//The event window process function
	LRESULT	DSEventWndProc(UINT msg, WPARAM wp, LPARAM lp);
	//Set the music file name for the CMusic object
	HRESULT LoadFromFile(LPCTSTR lpFileName);
	//To play the music
	HRESULT Play(BOOL bLoop = FALSE);
	//To pause the music
	HRESULT Pause();
	//To resume the music
	HRESULT Resume();
	//To stop the music
	HRESULT Stop();
	//To get the muic player current state
	UINT GetState();
	//--------------------------------------------------
private:
	//To initialize the DirectShow interfaces
	HRESULT Initialize();
	//To release the DirectShow interfaces
	void	Release();
	//To register the DirectShow event handle window class
	BOOL	RegisterEventWnd();
	//To create the DirectShow event handle window 
	BOOL	CreateEventWnd();
	//The WM_DSNOTIFY handle function
	HRESULT	HandleDSEvent();
	//--------------------------------------------------
private:
	IGraphBuilder*		m_pGB;
	IMediaControl*		m_pMC;
	IMediaEventEx*		m_pME;
	IMediaSeeking*		m_pMS;
	//
	BOOL				m_bLoop;
	HWND				m_hEventWnd;
	//--------------------------------------------------
};
//------------------------------------------------------
#endif	//_MUSICIMPL_H_
//------------------------------------------------------