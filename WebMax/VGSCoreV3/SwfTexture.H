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
	
	
    HWND		m_hParent;			//��������
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
	//��ǰ֡
	int m_curFrame;

	//path to flash file in Unicode
	//unsigned short real_name[256];
    WCHAR		real_name[256];

	//status of the flash thread
	FlashThreadStatus flashThreadStatus;

	//ԭʼ������ͼ��ָ�룬���ʵ�ĳһ����ͼ����
	DWORD	oriTexHandle;
	//����״̬
	INT     playStatus;		
	//�Ƿ�ѭ������
	BOOL	m_bLoop;

	BOOL    m_bVisible; 
//----------------------------------------------------------
//	Flash�ؼ���Ϣ�¼�ӳ��
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

	//����������Ⱦflash���ݵ���ͼ
	HRESULT	CreateFrameTextureBuffer();
	//�ͷŶ�̬��ͼ����
    VOID    ReleaseFrameTextureBuffer();

	//�����Ƿ񿴼�
    void        SetVisible(BOOL bVisible);
    //����
    void        Play(BOOL bLoop = false);
	//��ͣ
	void		Pause();
    //ֹͣ
    void        Stop();
	//����ָ����֡
	void		SetCurFrm(long frm);
	//��ת��ָ��֡������
	void        GoToFrame(long frm);
	//�õ���ǰFlash����֡��
	int			GetFrameCnt();
	//�õ�Flash�ĵ�ǰ֡
	int			GetCurrentFrame();
	//�õ�Load�İٷֱ�
	int			GetLoadPercent();
	//�ͷ��ڴ�
	void		Delete();
    //Add Flash ��VC֮��ı���ͨ�ŷ���
	//����flash�еı���
//	string      GetFlashVariable(string variableName);
	//����flash�ı���
	void        SetFlashVariable(string variableName, string variable);

};