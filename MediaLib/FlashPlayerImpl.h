//--------------------------------------------------
//	Name:FlashPlayerImpl.h
//	Introduce:User IShockwaveFlash interface to play flas file
//	Creator:Youfang Yao
//	Date:12-04-06
//	Version:1.0V
//--------------------------------------------------
#ifndef	_FLASHPLAYERIMPL_H_
#define	_FLASHPLAYERIMPL_H_
//--------------------------------------------------
#include "MediaLib.h"
#import "C:\WINDOWS\system32\Macromed\Flash\Flash.ocx" 
#pragma comment(lib,"atl") 
//--------------------------------------------------
class CFlashPlayerImpl : public CFlashPlayer
{
	//----------------------------------------------
public:
	CFlashPlayerImpl();
	~CFlashPlayerImpl();
	//----------------------------------------------
public:
	//To create the flash control
	HRESULT		Create(HWND hParentWnd, int iLeft, int iTop, int iWidth, int iHeight, bool bVisible, UINT uEndMsg);
	//To show or hide the flash control
	void		SetVisible(bool bVisible);
	//To check the flash is playing or not
	bool		IsPlaying();
	//To play the flash file
	HRESULT		Play(LPCTSTR lpszMovieFile, bool bLoop);
	//To stop the flash file
	HRESULT		Stop();
	//----------------------------------------------
	//To check the playing is end or not
	DWORD		TimerThreadProc();
	//----------------------------------------------
private:
	ShockwaveFlashObjects::IShockwaveFlash*	m_pFlash;
	HWND		m_hFlashWnd;
	HWND		m_hParentWnd;
	BOOL		m_bIsVisible;
	UINT		m_uEndMsgType;
	//
	bool		m_bIsPlaying;
	HANDLE		m_hEndCheckThread;
	//----------------------------------------------
};
//--------------------------------------------------
#endif	//_FLASHPLAYERIMPL_H_
//--------------------------------------------------
