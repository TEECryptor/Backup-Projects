#pragma once
//things needed for flash
// #include "..\ATLFlashCtrlUnit.h"
#include <atlbase.h>
#include <atlwin.h>

//DirectX9 goodies
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include <windows.h>
#include <Atlhost.h>
#include <initguid.h>

#include <string>
#include "flash.h"
#include "CSysFunc.h"
#include "CTexture.h"


class VGS_EXPORT SwfTexture :  public IDispEventImpl<0, SwfTexture, &DIID__IShockwaveFlashEvents, &LIBID_ShockwaveFlashObjects, 0, 0>, public CTexture
{
public:
	//yes, this needs to be mutexed
	typedef enum FlashThreadStatus
	{
		eNotStarted = 0x0,
		eRunning,
		eWaitingToStop,
		eFinished,
	} FlashThreadStatus;
	
	
    HWND		m_hParent;			//父窗体句柄
	//hidden window that will hold the flash ActiveX Control
	HWND windowless;
//     IOleClientSite*     m_pClientSite;  //
	IDirect3DDevice9 *  m_pDevice;
	//interfaces to shockwave object
	IShockwaveFlash *iflash;
	IUnknown *unk;
	IViewObject *viewobject;

	//the stream interface to marshal the viewobject into the Rendering Thread
	IStream *pStream;

	//the RenderThread's version of the view object
	IViewObject *RTviewobject;

	//the ready state of the Flash Document
	long state;

	//handle to Flash's thread
	HANDLE hThread;

	//event thinga-ma-bobs
	IConnectionPointContainer *m_lpConCont;
	IConnectionPoint *m_lpConPoint;

	//Event Advise cookie (mmmmmmm cookies)
	DWORD m_dwConPointID;

	//System memory texture
	IDirect3DTexture9 * memtex;
    
    //Pool Manange texture for the render texture
    IDirect3DTexture9 * frameRenderTexSource;
    IDirect3DTexture9 * frameRenderTexDest;
    //IDirect3DTexture9 * frameRenderTexOut;
	
	//Size of ActiveX Container
	int m_nFlashWidth,m_nFlashHeight;
	//number of frames in the Flash Document
	long totalframes;
	//当前帧
	int m_curFrame;

	//path to flash file in Unicode
	//unsigned short real_name[256];
    WCHAR		real_name[256];

	//status of the flash thread
	FlashThreadStatus flashThreadStatus;

	//原始基础贴图的指针，材质的某一层贴图对象
	DWORD	oriTexHandle;
	//播放状态
	INT     playStatus;		
	//是否循环播放
	BOOL	m_bLoop;

	BOOL    m_bVisible; 
//----------------------------------------------------------
//	Flash控件消息事件映射
//----------------------------------------------------------
public:
	BEGIN_SINK_MAP(SwfTexture)
		//SINK_ENTRY_EX(0,DIID__IShockwaveFlashEvents, 0xFFFFFD9F, OnReadyStateChange)
		//SINK_ENTRY_EX(0,DIID__IShockwaveFlashEvents, 0x000007A6, OnProgress)
		//SINK_ENTRY_EX(0, DIID__IShockwaveFlashEvents, 0x00000096, OnFSCommand)
	END_SINK_MAP();	

	SwfTexture(const std::string& strName, HWND hParent, IDirect3DDevice9 * m_pDevice, /*IOleClientSite* pClientSite,*/
		const std::string& strFlashPath, int Width,int Height,int texWidth,int texHeight);
	~SwfTexture(void);

	HRESULT GetFrame();

	//创建用于渲染flash内容的贴图
	HRESULT	CreateFrameTextureBuffer();
	//释放动态贴图对象
    VOID    ReleaseFrameTextureBuffer();

	//设置是否看见
    void        SetVisible(BOOL bVisible);
    //播放
    void        Play(BOOL bLoop = false);
	//暂停
	void		Pause();
    //停止
    void        Stop();
	//跳至指定的帧
	void		SetCurFrm(long frm);
	//跳转到指定帧并播放
	void        GoToFrame(long frm);
	//得到当前Flash的总帧数
	int			GetFrameCnt();
	//得到Flash的当前帧
	int			GetCurrentFrame();
	//得到Load的百分比
	int			GetLoadPercent();
	//释放内存
	void		Delete();
    //Add Flash 和VC之间的变量通信方法
	//返回flash中的变量
//	string      GetFlashVariable(string variableName);
	//设置flash的变量
	void        SetFlashVariable(string variableName, string variable);

};
