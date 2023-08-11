//--------------------------------------------------
//	Name:FlashPlayerImpl.cpp
//	Introduce:The cpp file for class CFlashPlayerImpl
//	Creator:Youfang Yao
//	Date:12-04-06
//	Version:1.0V
//--------------------------------------------------
#include "stdafx.h"
#include "FlashPlayerImpl.h"
#include <atlbase.h> 
CComModule _Module; 
#include <atlwin.h> 
#include <atlcom.h>
//--------------------------------------------------
//--------------------------------------------------
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	CFlashPlayerImpl* pPlayer = (CFlashPlayerImpl*)lpParameter;

	if(pPlayer)
		return pPlayer->TimerThreadProc();
	else
		return 0;
}
//--------------------------------------------------
//	Name:CFlashPlayerImpl
//	Introduce:Constructor
//--------------------------------------------------
CFlashPlayerImpl::CFlashPlayerImpl()
 : m_pFlash(NULL),
   m_hParentWnd(NULL),
   m_hFlashWnd(NULL),
   m_bIsVisible(false),
   m_uEndMsgType(0),
   m_bIsPlaying(FALSE),
   m_hEndCheckThread(NULL)
{
}
//--------------------------------------------------
//	Name:~CFlashPlayerImpl
//	Introduce:Distructor
//--------------------------------------------------
CFlashPlayerImpl::~CFlashPlayerImpl()
{	
	if(NULL != m_hEndCheckThread)
	{
		SuspendThread(m_hEndCheckThread);
		CloseHandle(m_hEndCheckThread);
		m_hEndCheckThread = NULL;
	}

    if(m_pFlash) 
	{
		m_pFlash->StopPlay();
		m_pFlash->Release();
	}
	if(m_hFlashWnd)
	{
		DestroyWindow(m_hFlashWnd);
		m_hFlashWnd = NULL;
	}
}
//--------------------------------------------------
//	Name:Create
//	Introduce:To create the flash control
//--------------------------------------------------
HRESULT	CFlashPlayerImpl::Create(HWND hParentWnd, int iLeft, int iTop, int iWidth, int iHeight, bool bVisible, UINT uEndMsg)
{   
	if(NULL == hParentWnd)
		return FALSE;

	m_hParentWnd = hParentWnd;
	m_uEndMsgType = uEndMsg;

    AtlAxWinInit();
	m_hFlashWnd = CreateWindowEx(WS_EX_TOPMOST, _T("AtlAxWin"), _T("ShockwaveFlash.ShockwaveFlash"),
        WS_CHILD | WS_VISIBLE, iLeft, iTop, iWidth, iHeight, hParentWnd, 0, 0, 0);
	if(NULL == m_hFlashWnd)
		return FALSE;

	m_bIsVisible = bVisible;
    ShowWindow(m_hFlashWnd, m_bIsVisible ? SW_SHOW : SW_HIDE);

	IUnknown *iUnk;
    HRESULT hr = AtlAxGetControl(m_hFlashWnd, &iUnk);    
	if(NULL == iUnk)
		return FALSE;

	iUnk->QueryInterface(__uuidof(ShockwaveFlashObjects::IShockwaveFlash), (void**)&m_pFlash);
	iUnk->Release();

	if(NULL == m_pFlash)
		return FALSE;

	m_hEndCheckThread = CreateThread(NULL, 0, ThreadProc, this, CREATE_SUSPENDED, 0);
	if(NULL == m_hEndCheckThread)
		return FALSE;

	return TRUE;
}
//--------------------------------------------------
//	Name:SetVisible
//	Introduce:To show or hide the flash control
//--------------------------------------------------
void CFlashPlayerImpl::SetVisible(bool bVisible)
{
	if(NULL == m_hFlashWnd)
		return;

	m_bIsVisible = bVisible;
    ShowWindow(m_hFlashWnd, m_bIsVisible ? SW_SHOW : SW_HIDE);
}
//--------------------------------------------------	
//	Name:IsPlaying
//	Introduce:To check the flash is playing or not
//--------------------------------------------------
bool CFlashPlayerImpl::IsPlaying()
{
	if(NULL == m_pFlash)
		return false;

	return m_bIsPlaying;
}
//--------------------------------------------------
//	Name:Play
//	Introduce:To play the flash file
//--------------------------------------------------
HRESULT CFlashPlayerImpl::Play(LPCTSTR lpszMovieFile, bool bLoop)
{
	if(NULL == m_pFlash || NULL == lpszMovieFile)
		return E_POINTER;

	HRESULT hr = S_OK;

	m_pFlash->Movie = lpszMovieFile;
    m_pFlash->Loop = bLoop;
    hr = m_pFlash->Play();	
	if(FAILED(hr))
		return hr;

	m_bIsPlaying = true;
	
	ResumeThread(m_hEndCheckThread);

	return S_OK;
}
//--------------------------------------------------
//	Name:Stop
//	Introduce:To stop the flash file
//--------------------------------------------------
HRESULT	CFlashPlayerImpl::Stop()
{
	if(NULL == m_pFlash)
		return E_POINTER;

	if(!m_bIsPlaying)
		return S_OK;

	HRESULT hr = S_OK;
    hr = m_pFlash->StopPlay();
	if(FAILED(hr))
		return hr;

     hr = m_pFlash->Rewind();
	if(FAILED(hr))
		return hr;

	m_bIsPlaying = false;

	return S_OK;
}
//--------------------------------------------------
//	Name:TimerThreadProc
//	Introduce:To check the playing is end or not
//--------------------------------------------------
DWORD CFlashPlayerImpl::TimerThreadProc()
{
	while(true)
	{
		if(m_pFlash)
		{
			long iTotalFrm = m_pFlash->GetTotalFrames();
			long iCurFrm = m_pFlash->CurrentFrame();

			if(iCurFrm >= iTotalFrm-1)
			{
				PostMessage(m_hParentWnd, m_uEndMsgType, 0, 0);
				SuspendThread(m_hEndCheckThread);
			}
		}		

		Sleep(1000);
	};

	return 0;
}
//--------------------------------------------------

