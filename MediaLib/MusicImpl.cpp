//------------------------------------------------------
//	FileName:MusicImpl.cpp
//	Introduce:The class CMusicImpl cpp file
//	Creator:Youfang yao
//	Date:10-26-06
//	Version:1.0V
//------------------------------------------------------
#include "stdafx.h"
#include "MusicImpl.h"
//------------------------------------------------------
TCHAR		g_szEventWndName[] = "EventWnd";
//------------------------------------------------------
//	Name:EventWndProc
//	Introduce:The DirectShow event handle window process function
//------------------------------------------------------
LRESULT CALLBACK EventWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	CMusicImpl* pCurMusic = (CMusicImpl*)lp;

	if(pCurMusic)
		return pCurMusic->DSEventWndProc(msg, wp, lp);
	else
		return E_NOINTERFACE;
}
//------------------------------------------------------
//	Construction
//------------------------------------------------------
CMusicImpl::CMusicImpl()
 : m_pGB(NULL),
   m_pMC(NULL),
   m_pME(NULL),
   m_bLoop(FALSE),
   m_hEventWnd(NULL)
{
	Initialize();
}
//------------------------------------------------------
//	Distruction
//------------------------------------------------------
CMusicImpl::~CMusicImpl()
{
	Release();
}
//------------------------------------------------------
//To initialize the DirectShow interfaces
//------------------------------------------------------
HRESULT CMusicImpl::Initialize()
{
	HRESULT hr = S_OK;

	//To create the event handle window
	if(RegisterEventWnd())
	{
		if(!CreateEventWnd())
		{
			return GetLastError();
		}
	}
	else
	{
		return GetLastError();
	}

	//To initialize the DirectShow interfaces
	hr = ::CoInitialize(NULL);
	if(FAILED(hr))
		return hr;

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
                        IID_IGraphBuilder, (void **)&m_pGB);
	if(FAILED(hr))
		return hr;

	hr = m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC);
	if(FAILED(hr))
		return hr;	

	hr = m_pGB->QueryInterface(IID_IMediaEventEx, (void **)&m_pME);
	if(FAILED(hr))
		return hr;	

	hr = m_pGB->QueryInterface(IID_IMediaSeeking, (void **)&m_pMS);
	if(FAILED(hr))
		return hr;	

	hr = m_pME->SetNotifyWindow((OAHWND)m_hEventWnd, WM_DSNOTIFY, (LPARAM)this);
	if(FAILED(hr))
		return hr;	

	return S_OK;
}
//------------------------------------------------------
//To release the DirectShow interfaces
//------------------------------------------------------
void CMusicImpl::Release()
{
	if(m_pMC)
		m_pMC->Stop();

	m_pMS->Release();
	m_pME->Release();
	m_pMC->Release();
	m_pGB->Release();

	DestroyWindow(m_hEventWnd);
	m_hEventWnd = NULL;
}
//------------------------------------------------------
//To register the DirectShow event handle window class
//------------------------------------------------------
BOOL CMusicImpl::RegisterEventWnd()
{
	WNDCLASSEX wcx; 

	wcx.cbSize = sizeof(wcx);			
	wcx.style = CS_HREDRAW | 
		CS_VREDRAW;						
	wcx.lpfnWndProc = EventWndProc;		
	wcx.cbClsExtra = 0;					
	wcx.cbWndExtra = 0;					
	wcx.hInstance = NULL;				
	wcx.hIcon = NULL;					
	wcx.hCursor = NULL;					
	wcx.hbrBackground = NULL;			
	wcx.lpszMenuName =  NULL;			
	wcx.lpszClassName = g_szEventWndName;	
	wcx.hIconSm = NULL; 

	if(!RegisterClassEx(&wcx))
	{
		//If the error is not "The class is registered", return FALSE;
		DWORD dwError = GetLastError();		
		if(dwError != 0x00000582)
			return FALSE;
	}

	return TRUE; 
}
//------------------------------------------------------
//To create the DirectShow event handle window 
//------------------------------------------------------
BOOL CMusicImpl::CreateEventWnd()
{
	m_hEventWnd = CreateWindow( 
        g_szEventWndName,			
        NULL,					
        WS_OVERLAPPED,			
        0,						
        0,						
        1,						
        1,						
        (HWND) NULL,			
        (HMENU) NULL,			
        NULL,					
        (LPVOID) NULL);			
 
    if(!m_hEventWnd) 
	{
		DWORD dwError = GetLastError();
        return FALSE; 
	}

	ShowWindow(m_hEventWnd, SW_HIDE);
	return TRUE;
}
//------------------------------------------------------
//The event window process function
//------------------------------------------------------
LRESULT CMusicImpl::DSEventWndProc(UINT msg, WPARAM wp, LPARAM lp)
{ 
	switch (msg)
    {
        case WM_DSNOTIFY:
			return HandleDSEvent();
	}

	return 1;
}
//------------------------------------------------------
//The WM_DSNOTIFY handle function
//------------------------------------------------------
HRESULT	CMusicImpl::HandleDSEvent()
{
    LONG evCode = 0;
	LONG evParam1 = 0;
	LONG evParam2 = 0;
	HRESULT hr = S_OK;

    if (!m_pME)
        return S_OK;
    
    while(SUCCEEDED(m_pME->GetEvent(&evCode, (LONG_PTR *) &evParam1, 
                    (LONG_PTR *) &evParam2, 0)))
	{
		if(evCode == EC_COMPLETE)
		{
            Stop();

            LONGLONG pos = 0;
            hr = m_pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
                                   NULL, AM_SEEKING_NoPositioning);     
			if(m_bLoop)
			{
				hr = m_pMC->Run();
			}
		}
	}

	return hr;
}
//------------------------------------------------------


//------------------------------------------------------
//Set the music file name for the CMusic object
//------------------------------------------------------
HRESULT CMusicImpl::LoadFromFile(LPCTSTR lpFileName)
{
	HRESULT hr = S_OK;

	WCHAR wcFileName[MAX_PATH] = {0};
	MultiByteToWideChar(CP_ACP, 0, lpFileName, -1, wcFileName, MAX_PATH);

	hr = m_pGB->RenderFile(wcFileName, NULL);
	if(FAILED(hr))
		return hr;

	return S_OK;
}
//------------------------------------------------------
//To play the music
//------------------------------------------------------
HRESULT CMusicImpl::Play(BOOL bLoop)
{
	HRESULT hr = S_OK;

	if(m_pMC == NULL)
		return E_NOINTERFACE;

	m_bLoop = bLoop;

	OAFilterState  fs;
	hr = m_pMC->GetState(INFINITE, &fs);

	if(fs == State_Running)
	{
		hr = m_pMC->Stop();

        LONGLONG pos = 0;
        hr = m_pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
										NULL, AM_SEEKING_NoPositioning);
		if(FAILED(hr))
			return hr;
	}

	hr = m_pMC->Run();
	if(FAILED(hr))
		return hr;

	return S_OK;
}
//------------------------------------------------------
//To pause the music
//------------------------------------------------------
HRESULT CMusicImpl::Pause()
{
	HRESULT hr = S_OK;

	if(m_pMC == NULL)
		return E_NOINTERFACE;

	hr = m_pMC->Pause();
	if(FAILED(hr))
		return hr;

	return S_OK;
}
//------------------------------------------------------
//To resume the music
//------------------------------------------------------
HRESULT CMusicImpl::Resume()
{
	HRESULT hr = S_OK;

	if(m_pMC == NULL)
		return E_NOINTERFACE;

	hr = m_pMC->Run();
	if(FAILED(hr))
		return hr;

	return S_OK;
}
//------------------------------------------------------
//To stop the music
//------------------------------------------------------
HRESULT CMusicImpl::Stop()
{
	HRESULT hr = S_OK;

	if(m_pMC == NULL)
		return E_NOINTERFACE;

	hr = m_pMC->Stop();
	if(FAILED(hr))
		return hr;

	return S_OK;
}
//------------------------------------------------------
//To get the muic player current state
//------------------------------------------------------
UINT CMusicImpl::GetState()
{
	HRESULT hr = S_OK;

	if(m_pMC == NULL)
		return E_NOINTERFACE;

	OAFilterState fs;
	hr = m_pMC->GetState(500, &fs);
	if(FAILED(hr))
		return hr;

	return fs;
}
//------------------------------------------------------