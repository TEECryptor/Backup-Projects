#include "stdafx.h"
#include "SwfTexture.h"

#include <comdef.h> //for _bstr_t

# ifndef _VGS_ATL
	CComModule _Module;
#endif

BOOL	 bThreadTerminated = FALSE;

//Our own little thread.  This will push the Flash AcitveX Control
//to the least used Core on a multi-core system
DWORD WINAPI LoadSWF (LPVOID data) throw()
{
	SwfTexture * cont = (SwfTexture *)data;

	HRESULT hr = S_OK;

	AtlAxWinInit();

# ifndef _VGS_ATL
	extern __declspec(selectany) CAtlModule* _pAtlModule;
	_pAtlModule = &_Module;
#endif

	//create Window and Attach Flash control
	cont->windowless = CreateWindowEx(WS_EX_TOPMOST, CAxWindow::GetWndClassName(), TEXT("ShockwaveFlash.ShockwaveFlash"), 
		WS_CHILD | WS_CLIPCHILDREN, 
		0, 0, cont->m_nFlashWidth, cont->m_nFlashHeight, cont->m_hParent, 0, 0, 0);
	//::SetWindowLong(cont->windowless, GWL_WNDPROC, (LONG)WndProc);

	//	hr = CoCreateInstance(__uuidof(ShockwaveFlash),0,CLSCTX_ALL, __uuidof(IShockwaveFlash), (void **)&cont->iflash);

	AtlAxGetControl(cont->windowless, &cont->unk);  

	//挂事件          
	//     AtlAdvise(cont->unk, new TFlashEventHandle(cont->m_pClientSite), DIID__IShockwaveFlashEvents, &cont->m_dwConPointID);
	cont->unk->QueryInterface(IID_IShockwaveFlash, (void**)&cont->iflash);
	cont->unk->Release();

	if(cont->iflash == NULL)
	{
		// ::MessageBox(NULL, "VGS has not find Flash player in your system, \nPlease install Flash player first via Adobe Flash player installer website.", "Flash Player Error...", MB_ICONASTERISK | MB_OK);
		//	return;
	}


	//set up event notifications
	hr = cont->iflash->QueryInterface(IID_IConnectionPointContainer, (void**)&cont->m_lpConCont);
	if (FAILED(hr))
		return -1;

	hr = cont->m_lpConCont->FindConnectionPoint(DIID__IShockwaveFlashEvents, &cont->m_lpConPoint);
	if (FAILED(hr))
		return -1;


	//We went through all this trouble, make the little prick give us the Alpha Channel
	hr = cont->iflash->set_WMode(L"transparent");

	//get the view object
	hr = cont->iflash->QueryInterface(__uuidof(IViewObject),(void **)&cont->viewobject);

	//create stream to Marshal view object into render thread
	cont->pStream = NULL;
	hr = CoMarshalInterThreadInterfaceInStream(__uuidof(IViewObject), cont->viewobject, &cont->pStream);

	//sanity check
	cont->RTviewobject = NULL;

	//we want it to always loop
	hr = cont->iflash->set_Loop(true);

	//load the movie
	BSTR  movieFileName = SysAllocString(cont->real_name); 
	hr = cont->iflash->set_Movie(movieFileName);

	//since these are always going to be local media,  force wait until ready state is loaded
	int tryCount = 0;
	for(cont->state = -1; (!hr) && (cont->state != 4);)
	{
		hr = cont->iflash->get_ReadyState(&cont->state);
		if(cont->state == 4 || cont->state == 3)
		{
			//get the total frames of the SWF file
			hr = cont->iflash->get_TotalFrames(&cont->totalframes);
			break;
		}

		//防止卡死
		tryCount++;
		if (tryCount > 100)
			break;

		Sleep(0); //snooze
	}

	//如果当前播放状态
	switch (cont->playStatus)
	{
	case 0:
		cont->Pause();
		break;
	case 1:
		cont->Play();
		break;
	case 2:
		cont->Stop();
		break;
	}
	//调到已经指定的帧，加载需要时间
	cont->GoToFrame(cont->m_curFrame);
	cont->flashThreadStatus = SwfTexture::eRunning;

	//our own little message loop.  The ActiveX container needs this to run outside of the main proc thread.
	MSG msg;
	::memset(&msg, 0, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		::GetMessage (&msg, 0, 0, 0);
		if(bThreadTerminated)
			break;
		else
			DispatchMessage(&msg);
	}

	cont->flashThreadStatus = SwfTexture::eFinished;
	return 0;
}

SwfTexture::SwfTexture(const std::string& strName, HWND hParent, IDirect3DDevice9 * pDevice,/*IOleClientSite* pClientSite,*/
					   const std::string& strFlashPath,int Width,int Height,int texWidth,int texHeight)
					   :iflash(NULL)
					   ,m_hParent(hParent)
{	
	m_sName = strName;
	m_sFilePathandName = strFlashPath;
	m_textureType = TLT_FLASH;

	bThreadTerminated = FALSE;

	m_nFlashWidth = Width;
	m_nFlashHeight = Height;

	playStatus = 1;
	m_bVisible = true;
	m_bLoop = TRUE;

	//初始化原始材质句柄
    oriTexHandle = 0;

	width = texWidth;
	height = texHeight;

	m_pDevice =  pDevice;
// 	m_pClientSite = pClientSite;

	frameRenderTexSource = NULL;
	frameRenderTexDest = NULL;
	//frameRenderTexOut = NULL;
	//create the system memory texture to hold the pixel buffer
	HRESULT hr = S_OK;
	hr = m_pDevice->CreateTexture(Width,					// width
									Height,					// height
									1,						// levels
									D3DUSAGE_DYNAMIC,		// usage
									D3DFMT_X8R8G8B8,        // format
									D3DPOOL_SYSTEMMEM, 
									&memtex,
									NULL);
	if (FAILED(hr))
	{
		// ::MessageBox(NULL, "Create Swf Texture Error", "", MB_OK);
	}


	//create the system memory texture to hold the pixel buffer
	hr = CreateFrameTextureBuffer();
	if (FAILED(hr))
	{
		// ::MessageBox(NULL, "CreateFrameTextureBuffer() Error", "", MB_OK);
	}
	//convert Path to WideChar	
	MultiByteToWideChar(CP_ACP,0,strFlashPath.c_str(),-1, real_name,256);

	//change threading model
	DWORD dwID;
	hThread = CreateThread(NULL, 0, LoadSWF, (LPVOID)this, 0, &dwID);
}

SwfTexture::~SwfTexture(void)
{

}


//释放内存
VOID SwfTexture::Delete()
{
	if (iflash)
		iflash->StopPlay();

	//kill the memtexture
	SAFE_RELEASE_D3D(memtex);
    ReleaseFrameTextureBuffer();
	
	//释放pStream数据流
	CoGetInterfaceAndReleaseStream(pStream, __uuidof(IViewObject), NULL);

	//stop the flash thread
	bThreadTerminated = TRUE;
	WaitForSingleObject(hThread, 100/*INFINITE*/);
	
	//release the viewobject and it's evil twin RTviewobject
	//SAFE_RELEASE(RTviewobject);
	//SAFE_RELEASE(viewobject);

	//unadvise the event sink
	m_lpConPoint->Unadvise(m_dwConPointID);
	SAFE_RELEASE(m_lpConCont);
	SAFE_RELEASE(m_lpConPoint);
	
	SAFE_RELEASE_D3D(iflash);

	DestroyWindow(windowless);	
}

//Draw the contents of the SFW hiden window onto a Render Target in the GPU (m_pOutTexture)
HRESULT SwfTexture::GetFrame()
{
	HRESULT hr = S_OK;

	if (!memtex)
	{
		return E_FAIL;
	}

	//Don't bother if it's not fully loaded
	if(state != 4 && state != 3) return hr;

	//do we need to un-marshall our version of the view object?
	if(!RTviewobject)
	{
		CoGetInterfaceAndReleaseStream (pStream, __uuidof(IViewObject), (void**) &RTviewobject);
	}

	RECT rc={0,0,0,0};
	rc.right = m_nFlashWidth;
	rc.bottom = m_nFlashHeight;
	
	BOOL isplay;
	long fn = -1;

	iflash->IsPlaying(&isplay);
	iflash->CurrentFrame(&fn);

	//clear out the memtexture
	D3DLOCKED_RECT d3dlr;
	hr = memtex->LockRect(0,&d3dlr,0,D3DLOCK_DONOTWAIT);
	_int32 * buffer = (_int32 *)d3dlr.pBits;
	DWORD buffersize = m_nFlashWidth * m_nFlashHeight;
	/*
	for(int i = 1; i < buffersize; i++)
	{
		buffer[i] = (_int32)0x00000000;
	}
	*/
	ZeroMemory(buffer, buffersize * sizeof(_int32));
	memtex->UnlockRect(0);

	//draw frame onto surface of memtexture
	IDirect3DSurface9 * memsurface = NULL;
	memtex->GetSurfaceLevel(0,&memsurface);
	//Draw contents of hidden SWF window directly onto surface of in-memory texture
	HDC hdcCompatible;
	hr = memsurface->GetDC(&hdcCompatible);
	//Sanity check
	SetMapMode(hdcCompatible,MM_TEXT);
	if(!hr)
	{
		//draw the frame
		try
		{			
			hr = OleDraw(RTviewobject,DVASPECT_CONTENT,hdcCompatible,&rc);
		}
		catch(HRESULT hr1)
		{
			hr = hr1;
		}
		memsurface->ReleaseDC(hdcCompatible);
	}
	SAFE_RELEASE_D3D(memsurface);
    //upload mem-texture to render target on GPU
	memtex->AddDirtyRect(NULL);
    hr = m_pDevice->UpdateTexture(memtex,frameRenderTexSource);
	
	if (frameRenderTexSource && frameRenderTexDest)
	{
		IDirect3DSurface9 * pOutSurfaceSource = NULL;
		frameRenderTexSource->GetSurfaceLevel(0,&pOutSurfaceSource);
		//D3DXSaveSurfaceToFile("C:\\Surface1.jpg", D3DXIFF_JPG, pOutSurfaceSource, NULL, NULL);
		IDirect3DSurface9 * pOutSurfaceDest = NULL;
		frameRenderTexDest->GetSurfaceLevel(0,&pOutSurfaceDest);
		hr = m_pDevice->StretchRect(pOutSurfaceSource, NULL, pOutSurfaceDest, NULL, D3DTEXF_LINEAR);
		//IDirect3DSurface9 * pOutSurfaceOut = NULL;
		//frameRenderTexOut->GetSurfaceLevel(0,&pOutSurfaceOut);
		//hr = D3DXLoadSurfaceFromSurface(pOutSurfaceOut, NULL, NULL, pOutSurfaceDest, NULL, NULL, D3DX_FILTER_NONE, 0);
		SAFE_RELEASE_D3D(pOutSurfaceSource);
		SAFE_RELEASE_D3D(pOutSurfaceDest);
		//SAFE_RELEASE_D3D(pOutSurfaceOut);
	}

	return hr;
}


//创建动态贴图帧缓冲
HRESULT	SwfTexture::CreateFrameTextureBuffer()
{
    if ( FAILED( D3DXCreateTexture(m_pDevice,
									m_nFlashWidth, m_nFlashHeight,
									1, D3DUSAGE_RENDERTARGET, 
									D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &frameRenderTexSource ) ) 
		 || !frameRenderTexSource)
	{
		// ::MessageBox(NULL, "CreateFrameTextureBuffer-1", "Error", MB_OK);
		return E_FAIL;
	}

    if( FAILED( D3DXCreateTexture(m_pDevice,
								width, height,
								1, D3DUSAGE_RENDERTARGET, 
								D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &frameRenderTexDest ) ) 
		|| !frameRenderTexDest)
    {
		// ::MessageBox(NULL, "CreateFrameTextureBuffer-2", "Error", MB_OK);
		return E_FAIL;
	}
/*    if( FAILED( D3DXCreateTexture(m_pDevice,
                    _texWidth, _texHeight,
                    1, D3DUSAGE_DYNAMIC, 
                    D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &frameRenderTexOut ) ) )
        return E_FAIL;*/
	return S_OK;
}

//释放动态贴图对象
VOID SwfTexture::ReleaseFrameTextureBuffer()
{
	SAFE_RELEASE_D3D(frameRenderTexSource);
	SAFE_RELEASE_D3D(frameRenderTexDest);
	//SAFE_RELEASE_D3D(frameRenderTexOut);
}
//flash控制
//--------------------------------------------------------------------------
//  设置是否看见     
//--------------------------------------------------------------------------
void SwfTexture::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible; 
}

//--------------------------------------------------------------------------
//  播放
//--------------------------------------------------------------------------
void SwfTexture::Play(BOOL bLoop)
{
	playStatus = 1;
    if(iflash == NULL) return;
    
	//m_atlFlash->set_Movie(m_strMovieName);
	m_bLoop = bLoop;
    iflash->set_Loop(m_bLoop);

    iflash->Play();

	//CComBSTR mode("transparent");
    //m_atlFlash->set_WMode(mode);
}

//--------------------------------------------------------------------------
//  暂停
//--------------------------------------------------------------------------
void SwfTexture::Pause()
{
	playStatus = 0;
    if(iflash == NULL) return;
    iflash->StopPlay();
}


//--------------------------------------------------------------------------
//  停止
//--------------------------------------------------------------------------
void SwfTexture::Stop()
{                  
	playStatus = 2;
	
    if(iflash == NULL) return;
    iflash->Rewind();
    iflash->StopPlay();
}

//--------------------------------------------------------------------------
//	跳至指定的帧
//--------------------------------------------------------------------------
void SwfTexture::SetCurFrm(long frm)
{
	m_curFrame = frm;
    if(iflash == NULL) return;
	iflash->set_FrameNum(frm);
}

void SwfTexture::GoToFrame(long frm)
{
	m_curFrame = frm;
    if(iflash == NULL) return;
	iflash->GotoFrame(frm);
}

//得到当前Flash的总帧数
int	SwfTexture::GetFrameCnt()
{
	if (iflash == NULL)
		return 0;

	long frameCnt = 0;
	iflash->get_TotalFrames(&frameCnt);
	return frameCnt;
}

//得到Flash的当前帧
int	SwfTexture::GetCurrentFrame()
{
	if (iflash == NULL)
		return 0;

	long currentFrame = 0;
	iflash->CurrentFrame(&currentFrame);
	return currentFrame;
}

//得到Load的百分比
int	SwfTexture::GetLoadPercent()
{
	if (iflash == NULL)
		return 0;

	long percent = 0;
	iflash->PercentLoaded(&percent);
	return percent;
}
/*//获得flash的变量
string SwfTexture::GetFlashVariable(string variableName)
{
	BSTR d = NULL;
	_bstr_t bst(variableName.c_str());
	iflash->GetVariable(bst,&d);
	string c;
    if(d!=NULL)
		c = string(_bstr_t(d));
	return c;
}*/

//设置flash中的变量
void SwfTexture::SetFlashVariable(string variableName, string variable)
{
	iflash->SetVariable(_bstr_t(variableName.c_str()), _bstr_t(variable.c_str()));
}