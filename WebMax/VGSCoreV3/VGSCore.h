//------------------------------------------------------------------------
// VGSCore.hͷ�ļ�
// ������VGS�����ӿ�
// ����: fuxb
// ����: 2008 -3 -1
//------------------------------------------------------------------------

#pragma once

#include "VGSDef.h"
 
// #include "VGSHeader.h"
#include <stdio.h>
#include <windows.h>

// #include "root.h"
#include <vector>
#include <set>

using namespace std;

class CRoot;
class CScene;
class CMainFrameListener;
//class LuaEngine;
//class LuaState;

// ����webmax��ר�нӿ�
// #if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))

#include "lua/LuaEngine.h"
// #include "GfxManager.h"
#include "VGSRender.h"

class VGS_EXPORT CVGSCore
{
public:
	CVGSCore();
	~CVGSCore();

	CVGSRender& getVGSRender() { return g_VGSRender;}
public:
	//---------��Ⱦ�߳�ͬ������---------------
	//��;: ���߳�ͬ���������ڷ�ֹ��Ⱦ�߳�����Ⱦʱ����Ⱦ���б������߳��޸�
	CRITICAL_SECTION			m_render_cs;	
	//---------------------------------------

	//---------�����߳�ͬ������---------------
	//��;: ���߳�ͬ���������ڷ�ֹ����߳�ͬʱȡ�ö��㻺�塢��ͼ����Ŀ���Ȩ
	CRITICAL_SECTION			m_ctrl_cs;	

	//-------------------------------------------------------------------------------------------
	//***ϵͳ����
	//-------------------------------------------------------------------------------------------
	CVGSRender g_VGSRender;  // ��Ⱦ������

	CRoot*                       m_pRoot;
	//VGS�����б�
	// vector<CVGSTestWindow*>	g_vWindow; 
	//VGS�����б�						
	vector<CScene*>		g_vScene;					

	BOOL                g_bSceneDirty;     //  ����������Ҫ����

	// CRoot*              g_pRoot = NULL;   // �����root

	std::set<std::string>    g_ResourcePath;   // ��Դ������·��

	//-------------------------------------------------------------------------------------------
	//ʱ����ر���
	//-------------------------------------------------------------------------------------------
	UINT				lastFrameTimer;						//��һ֡��Ⱦ������ʱ��


	INT					frameCnt;

	//���ڼ�¼��ǰ������ʹ�õ�ͼ���ļ���.
	set<string>	g_setImage;

	//�����Ƿ��ʼ��.
	bool mInitialized;

	//for test 2010-6-1
// 	GfxMovieViewPtr m_ptrSwfView;
	//������¼�����������
	void MessageWndProc(UINT message, WPARAM wParam, LPARAM lParam);
public:

//-------------------------------------------------------------------------------------------
///�����ʼ��������VGS������صķ���
//-------------------------------------------------------------------------------------------
//����VGS��׼���壬���parentWndΪNULL���򴴽�Overlap����;���򴴽��Ӵ���
/*DLL_EXPORT*/ HWND CreateVGSWindow(RECT* rect, HWND parentWnd);

/**��ʼ��VGS���棬�����ȵ��ø÷��������ܵ���CreateScene
@param bWritelog in : �Ƿ񱣴�log�ļ��������Ϊtrue��������dllͬһĿ¼������vgs.log�ļ�
//�����ɹ�����TUR, ���򷵻�FALSE
*/
/*DLL_EXPORT*/ BOOL InitVGS(HWND hWnd, INT browserType = 0, BOOL bWritelog = FALSE);
//�˳�VGS����
/*DLL_EXPORT*/ VOID ExitVGS();

/** �������������
@param type INT : 0 IE, 1 ff, 2 google, 3 opera, 4 safari
*/
/*DLL_EXPORT*/ VOID SetWebBrowserType(INT type);

/** ��ȡ���������
@return : 0 IE, 1 ff, 2 google, 3 opera, 4 safari
*/
/*DLL_EXPORT*/ INT  GetWebBrowserType();

/** ����IE�������������
@param type INT : 0 IE, 1 maxthon, 2 tencent
*/
/*DLL_EXPORT*/ VOID SetWebBowserSubtype(INT subType);

/** ��ȡIE�������������
@return : 0 IE, 1 maxthon, 2 tencent
*/
/*DLL_EXPORT*/ INT  GetWebBowserSubtype();


/** ����VGS����
@param in name Ҫ������VGS��������
@return ��������ɹ�������VGS������ָ�룻���򷵻�0
@remarks ���Դ������VGS����
*/
/*DLL_EXPORT*/ DWORD CreateScene(CHAR* name);

//���������±��, ֻ����ɾ�����������Ҫ����
/*DLL_EXPORT*/ VOID ResetSceneIndex();

/**���VGS����
@param in sceneHandle VGS����ָ��
*/
/*DLL_EXPORT*/ VOID	 DeleteScene(DWORD sceneHandle);
/** ���VGS��������
@param in sceneHandle VGS����ָ��
*/
/*DLL_EXPORT*/ VOID	 ResetScene(DWORD sceneHandle);

/** ɾ������VGS����
*/
/*DLL_EXPORT*/ VOID	 DeleteAllScene();

/**��ó���������*/
/*DLL_EXPORT*/ INT	GetSceneCount();

/**ͨ����Ż�ó���
@param in index VGS��������������
@return VGS����������ָ��
*/
/*DLL_EXPORT*/ DWORD GetSceneHandleByIndex(INT index);
/**ͨ�����ƻ�ó���
@param in name VGS��������������
@return VGS����������ָ��
*/
/*DLL_EXPORT*/ DWORD GetSceneHandleByName(CHAR* name);

/**ͨ�����ƾ����ó�������
@param in sceneHandle VGS����������ָ��
@param in&out name VGS��������������
*/
/*DLL_EXPORT*/ VOID GetSceneNameByHandle(DWORD sceneHandle, CHAR* name);

/** ͨ����������Ż�ȡ��������
@param in index ����������
@param in/out name �������ƣ��MAX_NAMESTR_LENGTH���ֽ� 
@return �õ��������Ʒ���TRUE������FALSE
*/
/*DLL_EXPORT*/ BOOL GetSceneNameByIndex(INT index, CHAR* name);

/** ͨ�������ó���������
@param in sceneHandle VGS����������ָ��
@return VGS��������������,�����Ч����-1
*/
/*DLL_EXPORT*/ INT GetSceneIndexByHandle(DWORD sceneHandle);


//��ó����б༭״̬���������ѡ��״̬
/*DLL_EXPORT*/ UINT GetSceneEditAxisStatus(DWORD sceneHandle);

/////////////////////// ��Ⱦ���� //////////////////////////////////
/** ������Ⱦ���壬��������
@remarks һ���������԰��������Ⱦ����
*/
/*DLL_EXPORT*/ DWORD CreateTestRenderWindow(DWORD sceneHandle, HWND hRenderWnd);

/** ������Ⱦ����
@param sceneHandle in VGS����
@param hExternalWnd in ҪVGS��Ⱦ������Ƕ��Ĵ�����
@param Name in ��������Ⱦ��������
@return ��������ɹ���������Ⱦ����ָ�룬���򷵻�0
@remarks һ���������԰��������Ⱦ����
*/
/*DLL_EXPORT*/ DWORD CreateRenderWindow(DWORD sceneHandle, HWND hExternalWnd, CHAR* Name);

/**ɾ��ָ����VGS����
@param sceneHandle in VGS����������ָ��
@param windowHandle in Ҫɾ����VGS���ھ��
*/
/*DLL_EXPORT*/ VOID DeleteRenderWindow(DWORD sceneHandle, DWORD windowHandle);

/** ������Ⱦ�������ݵ�����ͼ
@param sceneHandle  in : ����ָ��
@param pRenderWnd   in : ��Ⱦ����ָ��
@param sFileName    in : Ҫ���������ͼ�ļ����ƣ�����·������չ��
*/
/*DLL_EXPORT*/ VOID CreateRenderwindowThumb(DWORD sceneHandle, DWORD pRenderWnd, const CHAR *sFileName);

//������Ⱦ�����ͶӰģʽ
///*DLL_EXPORT*/ VOID SetRenderWindowProjMode(DWORD sceneHandle, DWORD swapChainHandle, INT projMode);
//�����Ⱦ�����ͶӰģʽ
///*DLL_EXPORT*/ INT GetRenderWindowProjMode(DWORD sceneHandle, DWORD swapChainHandle);

/** �ú����Ѿ�����. ���ó�������Ⱦ�����Ƿ������Ӧ��ꡢ���̡��ֱ��Ŀ���
@param in sceneHandle : ����ָ��
@param in RenderWndHandle : ��Ⱦ����ָ��
@param in WndHandle : ��Ӧ��Ϣ�Ĵ��壬ͨ����������
*/
/*DLL_EXPORT*/ VOID CreateWndControl(DWORD WndHandle, DWORD sceneHandle, DWORD RenderWndHandle);

/** ���ó����Ŀ���״̬
@param bEnable in TRUE:������ͨ�������̵ȿ��ƣ�FALSE�����ܿ���
*/
// /*DLL_EXPORT*/ VOID SetWndControlStatus(BOOL bKeyboardEnable, BOOL bMouseEnable);
// /*DLL_EXPORT*/ VOID SetWndControlStatus(BOOL bEnable);

/** ���ó����Ƿ���Լ��̿���
*/
/*DLL_EXPORT*/ VOID SetWndControlKeyboardStatus(BOOL bEnable);

/** ���ó����Ƿ����������
*/
/*DLL_EXPORT*/ VOID SetWndControlMouseStatus(BOOL bEnable);

/** ���ó����Ŀ���״̬
@param out: �����Ƿ��ͨ�������̵ȿ��ƣ�FALSE�����ܿ���
*/
// /*DLL_EXPORT*/ VOID GetWndControlStatus(BOOL &bKeyboardEnable, BOOL &bMouseEnable);
// /*DLL_EXPORT*/ BOOL GetWndControlStatus();

/** ��ȡ�����Ƿ���Լ��̿���
*/
/*DLL_EXPORT*/ BOOL GetWndControlKeyboardStatus();

/** ��ȡ�����Ƿ����������
*/
/*DLL_EXPORT*/ BOOL GetWndControlMouseStatus();

////////////////// ��Դ·�� //////////////////////
/** �����Դ�ļ�����·��
*/ 
/*DLL_EXPORT*/ VOID AddResourcePath(const CHAR * sFilePath);

/** ɾ����Դ�ļ�����·��
*/ 
/*DLL_EXPORT*/ VOID RemoveResourcePath(const CHAR * sFilePath);

/** ɾ��������Դ�ļ�����·��
*/ 
/*DLL_EXPORT*/ VOID ClearResourcePaths();

//-------------------------------------------------------------------------------------------
//***������Ⱦ��صķ���
//-------------------------------------------------------------------------------------------

// ��Ⱦ���еĳ���
/*DLL_EXPORT*/ VOID RenderAll();

/**��Ⱦָ���ĳ��������ܰ��������Ⱦ����
@param in sceneHandle VGS����������ָ��
*/
/*DLL_EXPORT*/ VOID	RenderScene(DWORD sceneHandle);

/** �����ƶ���Ⱦ���ڵĺ󻺳���
*/
/*DLL_EXPORT*/ BOOL UpdateBackBuffer(DWORD hRenderWnd);

/**���Viewport
@param in windowHandle ��Ⱦ������
@camHandle in �ӿ����������
@param in level   �ӿڵĲ�Σ����Խ�ߵ��ӿڻḲ���ڲ�ε͵��ӿ��Ϸ�
@param in left	  �ӿڵ������λ�ã����ֵ[0-1] left = vp_left/window_width;
@param in top	  �ӿڵ����Ͻ�λ�ã����ֵ[0-1] top = vp_top/window_height;
@param in width	  �ӿڵĿ�ȣ����ֵ[0-1] width = vp_width/window_width;
@param in  height �ӿڵĸ߶ȣ����ֵ[0-1] height = vp_height/window_height;
@return ���������ӿ�ָ�룬�������ʧ�ܣ�����0
*/
/*DLL_EXPORT*/ DWORD AddViewport(DWORD windowHandle, DWORD camHandle, INT level, FLOAT left, FLOAT top, FLOAT width, FLOAT height);

/** ɾ����Ⱦ���ڵ��ӿ�
@param windowHandle in ��Ⱦ����ָ��
@param viewportHandle in �ӿ�ָ��
*/
/*DLL_EXPORT*/ VOID DeleteViewport(DWORD windowHandle, DWORD vpHandle);

/** �����ӿ�ʹ�õ����
@param viewportHandle in �ӿ�ָ��
@param camHandle in ���ָ��
*/
/*DLL_EXPORT*/ VOID SetViewportCamera(DWORD viewportHandle, DWORD camHandle);
/** ��ȡ�ӿڵ�ǰʹ�õ����
@param viewportHandle �ӿ�ָ��
@return ���ָ��
*/
/*DLL_EXPORT*/ DWORD GetViewportCamera(DWORD viewportHandle);

/**�����ӿ�λ�úʹ�С 
@param ViewPort �ӿ�ָ��
@param left,top,width,heightȡֵ��Ϊ[0,1],��ʾ��RenderTarget�е�λ��
*/
/*DLL_EXPORT*/ VOID	SetViewportRect(DWORD ViewPort, float left, float top, float width, float height);

/** �����ӿ�ʹ������Ŀ�߱�
*/
/*DLL_EXPORT*/ VOID UpdateViewportAspectRatio(DWORD viewportHandle);

/**��ȡ�ӿڴ�С 
@param ViewPort �ӿ�ָ��
@param left,top,width,heightȡֵ��Ϊ[0,1],��ʾ��RenderTarget�е�λ��
*/
/*DLL_EXPORT*/ VOID GetViewportRect(DWORD ViewPort, float &left, float &top, float &width, float &height);

/** �õ��ӿڵ���ʵ���
@return : ����ӿڲ����ڣ�����-1
*/
/*DLL_EXPORT*/ INT GetViewportActualWidth(DWORD ViewPort);

/** �õ��ӿڵ���ʵ�߶�
@return : ����ӿڲ����ڣ�����-1
*/
/*DLL_EXPORT*/ INT GetViewportActualHeight(DWORD ViewPort);

/**���õ�ǰ�����Ƿ��Զ���Ⱦ
@param sceneHandle ����ָ��
@param renderScene �Ƿ�Ӧ�ñ���Ⱦ�������Ϊ<b>��</b>�Զ���Ⱦ����ֻ�е���RenderTarget��Update����ʱ������ܱ���Ⱦ��
*/
/*DLL_EXPORT*/ VOID SetSceneRenderStatus(DWORD sceneHandle, BOOL renderScene);
/**��ȡ��ǰ�����Ƿ��Զ���Ⱦ
@param sceneHandle ����ָ��
*/
/*DLL_EXPORT*/ BOOL	GetSceneRenderStatus(DWORD sceneHandle);

/**������Ⱦ�����Ƿ���Ⱦ
@param sceneHandle ����������ָ��
@param RenderTargetHandle ��Ⱦ����ָ��
@param bAutoRender �Ƿ��Զ���Ⱦ�������Ϊ<b>��</b>�Զ���Ⱦ����ֻ�е���RenderTarget��Update����ʱ������ܱ���Ⱦ��
*/
/*DLL_EXPORT*/ VOID SetRenderTargetStatus(DWORD sceneHandle, DWORD RenderTargetHandle, BOOL bAutoRender);

/**��ȡ��Ⱦ�����Ƿ���Ⱦ
@param sceneHandle ����������ָ��
@param RenderTargetHandle ��Ⱦ����ָ��
*/
/*DLL_EXPORT*/ BOOL GetRenderTargetStatus(DWORD sceneHandle, DWORD RenderTargetHandle);

/**������Ⱦ��������ģʽ, 2Ϊ�߿�3Ϊʵ��
@param sceneHandle ����������ָ��
@param CameraHandle �ӿ�ָ��
@param fillMode ��Ⱦģʽ��1 : Only points are rendered�� 2 : Wireframe models are rendered. 3: Solid polygons are rendered.
*/
// /*DLL_EXPORT*/ VOID SetSwapRenderWndFillMode(DWORD sceneHandle, DWORD CameraHandle, INT fillMode);
/*DLL_EXPORT*/ VOID SetViewportFillMode(DWORD sceneHandle, DWORD viewport, INT fillMode);

/**�����Ⱦ��������ģʽ, 1 : Only points are rendered�� 2 : Wireframe models are rendered. 3: Solid polygons are rendered.
@param sceneHandle ����������ָ��
@param CameraHandle ���ָ��
*/
// /*DLL_EXPORT*/ INT GetSwapRenderWndFillMode(DWORD sceneHandle, DWORD CameraHandle);
/*DLL_EXPORT*/ INT GetViewportFillMode(DWORD sceneHandle, DWORD viewport);

//for debug
/*DLL_EXPORT*/ INT	FrameRenderComplete(DWORD sceneHandle);

/** ǿ����Ⱦ��ǰ֡
@param sceneHandle ����ָ��
*/
/*DLL_EXPORT*/ VOID RenderCurrentFrame(DWORD sceneHandle);

/**�����ӿ��Ƿ���Ⱦ��ά����
@param sceneHandle in ����������ָ��
@param vpHandle in : �ӿ�ָ��
*/
/*DLL_EXPORT*/ VOID SetViewportRenderGUI(DWORD sceneHandle, DWORD vpHandle, BOOL renderGUI);

/**��ȡ�ӿ��Ƿ���Ⱦ��ά����
@param sceneHandle in ������������ָ��
@param vpHandle in : �ӿ�ָ��
@renderGUI �Ƿ���Ⱦ��άԪ��
*/
/*DLL_EXPORT*/ BOOL IsViewportRenderGUI(DWORD sceneHandle, DWORD vpHandle);

//���ÿ��ܳݵļ���
/*DLL_EXPORT*/ VOID SetAntiAliasLevel(INT level);
//��ÿ��ܳݼ���
/*DLL_EXPORT*/ INT	GetAntiAliasLevel();

/**���ó����ı���
@param sceneHandle ����������ָ��
@param ViewportHandle �ӿڵ�ָ�룬��ǰ�汾��������NULL�������ó����е������ӿ�
@param color ��ɫ
*/
/*DLL_EXPORT*/ VOID SetSceneBackColor(DWORD sceneHandle, DWORD ViewportHandle, DWORD color);
/**��ó����ı���
@param sceneHandle ����������ָ��
@param ViewportHandle �ӿڵ�ָ��
*/
/*DLL_EXPORT*/ DWORD GetSceneBackColor(DWORD sceneHandle, DWORD ViewportHandle);

//���ó�����VGS�����ʼ�ߴ�
/*DLL_EXPORT*/ VOID SetSceneRect(DWORD sceneHandle, RECT* pRect);
//��ó�����VGS�����ʼ�ߴ�
/*DLL_EXPORT*/ VOID GetSceneRect(DWORD sceneHandle, RECT* pRect);

//������ά�������Ļ����
/*DLL_EXPORT*/ VOID CalcScreenCoords(DWORD sceneHandle, DWORD viewportHandle, 
								 VECTOR3* pVec3, INT* X, INT* Y);

//������ര��
///*DLL_EXPORT*/ VOID ZoomOrthoView(DWORD sceneHandle, DWORD swapChainHandle, FLOAT zoomVal);

//��ǰ�����Ƿ�Ϊ��ര��
// /*DLL_EXPORT*/ BOOL IsOrthoView(DWORD sceneHandle, DWORD swapChainHandle);

//����ǰ�����е���ά�������õ����ʵĴ�С
// /*DLL_EXPORT*/ VOID MaximizeSceneToView(DWORD sceneHandle, DWORD swapChainHandle, FLOAT fillRatio);

//ƽ����ര��
/*DLL_EXPORT*/ VOID PanOrthoView(DWORD sceneHandle, DWORD swapChainHandle, INT X, INT Y, INT offsetX, INT offsetY);

//-------------------------------------------------------------------------------------------
///�ļ���صķ���
//-------------------------------------------------------------------------------------------
/** ����S3D�ļ�
@param sceneHandle		 : �������������
@param strS3DFileName	 : s3D�ļ�������·��
@param hWnd           in : ����Ĵ����������ڽ��ս�����Ϣ
@return �ɹ�����S3D�������ľ�������򷵻�0
@remarks ͨ�����ص�S3D����������������Ի�ȡ����S3D��ص�һЩ��Ϣ���мǣ�����ʱҪ��ʱ����DeleteS3DLoader()����ɾ��������
*/
#if (!defined(_VGS_LIB) && !defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD ImportS3DFile(DWORD sceneHandle, const CHAR* strS3DFileName, HWND hWnd);
#endif


/** ɾ��S3D���ض���
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ VOID DeleteS3DLoader(DWORD sceneHandle, DWORD S3DLoaderHandle);
#endif


/** ָ������ͼ�ļ��Ƿ�S3Dʹ��
@param S3DLoaderHandle in  : S3D������ָ�룬��ImportS3DFile()����
@param ImageFileName   in  : ͼƬ�ļ����ƣ�������·��
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ BOOL IsTextureImageUsedbyS3D(DWORD S3DLoaderHandle, const CHAR* ImageFileName);
#endif
/** �õ�S3D��ʹ�õ���ͼ������
@param loaderHandle in : S3D������ָ�룬��ImportS3DFile()����
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ INT GetS3DTextureImageCnt(DWORD S3DLoaderHandle);
#endif

/** �õ�S3D��ʹ�õĵ�index����ͼ�ļ����ƣ�����·��
@param loaderHandle in  : S3D������ָ�룬��ImportS3DFile()����
@param texIndex     in  : ����
@param strTexName   out : ��ͼ�ļ�����
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ BOOL GetS3DTextureImageFile(DWORD S3DLoaderHandle, INT texIndex, CHAR* strTexName);
#endif

/** ���泡����ָ����V3D�ļ�
@param in sceneHandle    ��VGS����������ָ��
@param in strV3DFileName ��Ҫ�����V3D�ļ���
@param hWnd           in : ����Ĵ����������ڽ��ս�����Ϣ
@return�����롣0��ʾ�ɹ�������ֵ��ʾʧ�ܡ�ͨ��GetErrorStr���Եõ���ϸ����
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD SaveSceneAsV3D(DWORD sceneHandle, CHAR* strV3DFileName, HWND hWnd);
#endif

/**��V3D�ļ�, �Զ�ʶ��ͬ�汾��V3D
@param in sceneHandle VGS����������ָ��
@param in strV3DFileName Ҫ�򿪵�V3D�ļ���
@param in OT ��V3D�ķ����������滻��ʽ���Ǻϲ���ʽ��Ŀǰ��֧���滻��ʽ
@param hWnd           in : ����Ĵ����������ڽ��ս�����Ϣ
@param in&out pErrCode ���ʧ�ܣ����ش����룬ͨ��GetErrorStr���Եõ�ԭ������
@return�����롣0��ʾ�ɹ�������ֵ��ʾʧ�ܡ�ͨ��GetErrorStr���Եõ���ϸ����
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD OpenV3DFile(DWORD sceneHandle, const CHAR* strV3DFileName, V3D_OPENTYPE OT, HWND hWnd);
#endif

/**���浱ǰ����ΪN3D��ʽ�ļ�
@param in sceneHandle ��Ҫ����ĳ���
@param in strN3DFileName �� N3D�ļ���������Ŀ¼
@param hWnd           in : ����Ĵ����������ڽ��ս�����Ϣ
@return ���ؽ������ͨ��GetErrorStr���Եõ�ԭ������
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD SaveSceneAsN3D(DWORD sceneHandle, const CHAR* strN3DFileName, HWND hWnd);
#endif

/** ��buffer�ж�ȡN3D������Ϣ
@param in sceneHandle    ��Ҫ�������ݵĳ���
@param in version        : �汾��
@param in pBuffer        ��N3D����buffer
@param hWnd           in ������Ĵ����������ڽ��ս�����Ϣ
@return ���ؽ������ͨ��GetErrorStr���Եõ�ԭ������
*/
#if(defined(_VGS_LIB)||defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD LoadSceneFromBuffer(DWORD sceneHandle, DWORD version, const BYTE* pBuffer, HWND hWnd);
#endif

/** ��buffer�ж�ȡͼ����Ϣ
@param in sceneHandle    ��Ҫ����ͼ��ĳ���
@param in version        : �汾��
@param in pBuffer        ��N3D����buffer
@param hWnd           in ������Ĵ����������ڽ��ս�����Ϣ
@return ���ؽ������ͨ��GetErrorStr���Եõ�ԭ������
*/
#if(defined(_VGS_LIB)||defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD LoadImageDataFromBuffer(DWORD sceneHandle, DWORD version, const BYTE* pBuffer, HWND hWnd);
#endif

// ��һ��ͼƬ��buffer��������
/*DLL_EXPORT*/ DWORD CreateImageTextureFromBuffer(DWORD sceneHandle, const BYTE* pBuffer, DWORD len, int idx, HWND hWnd);


/** ��N3D�����ļ�, �Զ�ʶ��ͬ�汾��N3D
@param in sceneHandle ��Ҫ����ĳ���
@param in strN3DFileName �� N3D�ļ���������Ŀ¼
@return�����롣0��ʾ�ɹ�������ֵ��ʾʧ�ܡ�ͨ��GetErrorStr���Եõ���ϸ����
*/
// /*DLL_EXPORT*/ DWORD ImportN3DFile(DWORD sceneHandle, const CHAR* strN3DFileName);

/**���浱ǰ����ΪE3D��ʽ�ļ�
@param in sceneHandle    ��Ҫ����ĳ���
@param in strE3DFileName ��E3D�ļ���������Ŀ¼
@param in bLinkRes       : �������Դ�����ⲿ���ӷ�ʽ���Ǵ�����ļ��ڲ���bLinkResΪtrue����Դ�����ⲿ���ӵķ�ʽ���������E3D�ڲ�
@param in bPackData      : �Ƿ�ѹ���ļ���TRUE��ѹ����FALSE����ѹ��
@param hWnd           in : ����Ĵ����������ڽ��ս�����Ϣ
@return�����롣0��ʾ�ɹ�������ֵ��ʾʧ�ܡ�ͨ��GetErrorStr���Եõ���ϸ����
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD SaveSceneAsE3D(DWORD sceneHandle, const CHAR* strE3DFileName, BOOL bLinkRes/* = FALSE*/, BOOL bPackData/* = TRUE*/, HWND hWnd);
#endif

/** ����N3D�ļ�
@param strE3DFileName in : Ҫ������ļ�����
@param hWnd           in : ����Ĵ����������ڽ��ս�����Ϣ
@return�����롣0��ʾ�ɹ�������ֵ��ʾʧ�ܡ�ͨ��GetErrorStr���Եõ���ϸ����
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD LoadE3DFile(DWORD sceneHandle, const CHAR* strE3DFileName, HWND hWnd);
#endif

//-------------------------------------------------------------------------------------------
///ģ����صķ���
//-------------------------------------------------------------------------------------------

/**�õ�������ģ�͵�����
@param in sceneHandle ����������ָ��
@return ������ģ�͵�����
*/
/*DLL_EXPORT*/ INT GetModelCount(DWORD sceneHandle);

/*ɾ�������е�ģ��
@param sceneHandle ����������ָ��
@param modelHandle ģ��(entity)ָ��
@return �ɹ�ɾ������TRUE������FALSE
*/
/*DLL_EXPORT*/ BOOL DeleteModelbyHandle(DWORD sceneHandle, DWORD modelHandle);


/** ɾ�������е�ģ��
@param sceneHandle ����������ָ��
@param strModelName ģ��(entity)����
@return �ɹ�ɾ������TRUE������FALSE
*/
/*DLL_EXPORT*/ BOOL DeleteModel(DWORD sceneHandle, const CHAR* strModelName);

//����ģ�͵����Ƶõ�ģ�͵ľ��
/*DLL_EXPORT*/ DWORD GetModelHandleByName(DWORD sceneHandle, const CHAR* strModelName);
//����ģ�͵ı�ŵõ�ģ�͵ľ��
/*DLL_EXPORT*/ DWORD GetModelHandleByIndex(DWORD sceneHandle, INT index);

/**����ģ�͵ľ���õ�ģ�͵�����
@param in sceneHandle ����ָ��
@param in modelHandle ģ�͵ľ��
@param in & out strModelName ģ������
@return �õ����ַ���TRUE������FALSE
*/
/*DLL_EXPORT*/ BOOL GetModelNameByHandle(DWORD sceneHandle, DWORD modelHandle, CHAR* strModelName);

/**����ģ�͵������ŵõ�ģ�͵�����
@param in sceneHandle ����ָ��
@param in index ģ�͵�������
@param in & out strModelName ģ������
@return �õ����ַ���TRUE������FALSE
*/
/*DLL_EXPORT*/ BOOL GetModelNameByIndex(DWORD sceneHandle, INT index, CHAR* strModelName);


//����ģ�͵�����
/*DLL_EXPORT*/ VOID SetModelName(DWORD sceneHandle, DWORD modelHandle, CHAR* strModelName);

/** �ƶ�ģ�͵�λ��
@param in sceneHandle	: �������
@param in camHandle     : ������
@param in pVec          ���ƶ����������
@param in coordEnum     ���ƶ��Ĳο�����ϵ
			               0 - ����������ϵƽ��
						   1 - ����������ϵƽ��
*/
/*DLL_EXPORT*/ VOID MoveModel(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec, UINT flag);

/** �õ�ģ�͵�λ��,����Χ�����ĵ���������
@param out pVec : ģ�Ͱ�Χ�����ĵ���������λ��
*/
/*DLL_EXPORT*/ VOID GetModelPosition(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec);
//����ģ�͵�λ��
/*DLL_EXPORT*/ VOID SetModelPosition(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec);

/** ��ȡģ�͵İ�Χ��
@param in sceneHandle : ����������ָ��
@param in modelHandle : ģ��ָ��
@param out vMinCorner : ��Χ��������С�Ľǵ���������
@param out vMinCorner : ��Χ���������Ľǵ���������
@return ��Χ����Ч������TRUE, ����FALSE
*/ 
/*DLL_EXPORT*/ BOOL GetModelBoundsBox(DWORD sceneHandle, DWORD modelHandle, VECTOR3 &vMinCorner, VECTOR3 &vMaxCorner);

/** ��ȡģ�͵İ�Χ��
@param in sceneHandle : ����������ָ��
@param in modelHandle : ģ��ָ��
@param out vCenter    : ��Χ�����ĵ����������
@param out Radius     : ��Χ��뾶
@return ��Χ����Ч������TRUE, ����FALSE
*/ 
/*DLL_EXPORT*/ BOOL GetModelBoundsSphere(DWORD sceneHandle, DWORD modelHandle, VECTOR3 &vCenter, FLOAT &Radius);

/** ����ģ���Ƿ���ʾ��Χ��
@param in sceneHandle : ����������ָ��
@param in modelHandle : ģ��ָ��
@param in bvisible    : �Ƿ���ʾ��Χ��
*/
/*DLL_EXPORT*/ VOID SetModelBoundsBoxVisibility(DWORD sceneHandle, DWORD modelHandle, BOOL bVisible);

///** ����ģ���Ƿ���ʾ��Χ��
//@param in sceneHandle : ����������ָ��
//@param in modelHandle : ģ��ָ��
//@param in bvisible    : �Ƿ���ʾ��Χ��
//@param in type        : 
//						VAT_REPLACE = 0,  // ��modelHandle�İ�Χ����Ϊbvisible��ͬʱ��������ģ�͵İ�Χ������!bvisible
//						VAT_REVERSE,      // ��modelHandle�İ�Χ����Ϊ!bvisible��ͬʱ��������ģ�͵İ�Χ������bvisible
//						VAT_ADD           // ��modelHandle�İ�Χ����Ϊbvisible����������ģ�Ͳ��ı��Χ��״̬
//*/
///*DLL_EXPORT*/ VOID SetModelBoundsBoxVisibility(DWORD sceneHandle, DWORD modelHandle, BOOL bVisible, VGS_ADDTYPE type);

/** ��תģ��
@param in pVec : �ڸ�����������ת�ĽǶ�����,����
@param in coordEnum :
	//0 - �ƴ����������ģ�����������ϵƽ�е�����ϵ��ת
	//1 - ����������ϵ��תģ��
	//2 - �Ƹ�����ϵ��ת
*/
/*DLL_EXPORT*/ VOID RotateModel(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec, INT coordEnum);

//ģ����ָ��������ת
///*DLL_EXPORT*/ VOID RotateModelByAxis(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pPos, VECTOR3* pVec, FLOAT fAngle);

/** �õ�ģ�͵�ת��
@param out pRot ��ģ�͵�ǰ�����ԭʼ״̬�����������ϵ�ת�ǣ���λ�ǻ���
*/
/*DLL_EXPORT*/ BOOL GetModelRotation(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pRot);

/** ����ģ�͵�ת��
@param out pRot ��ģ�͵�ǰ�����ԭʼ״̬�����������ϵ�ת�ǣ���λ�ǻ���
*/
/*DLL_EXPORT*/ VOID SetModelRotation(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pRot);

/**-----------------------------------------------------------------------------
// ������: ScaleModelByPoint()
// ��  ��: DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale
// ��  ;: �Թ̶���Ϊ��������ģ��
//-----------------------------------------------------------------------------*/
/*DLL_EXPORT*/ VOID ScaleModelByPoint(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale);

/** ����������Ϊ���ģ��ڵ�ǰ����������ģ��
@param in pScale : ���������ϵ�����ֵ
@param in flag   : �ο�����ϵ��0����������ϵ��1����������ϵ
@reamarks: ���磺pScale����(1, 2, 0.5)���ͱ�ʾģ�ͽ��������ڵĻ����ϣ���X�������ţ�y����Ŵ�һ����z������Сһ��
*/
/*DLL_EXPORT*/ VOID ScaleModel(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pScale, int flag = 0/*��������ϵ*/);

/** ����������Ϊ���ģ���ģ��ԭʼ�ߴ����������ģ��
@reamarks: ���磺pScale����(1, 2, 0.5)���ͱ�ʾģ�������ԭʼ�ߴ磬����X�������ţ�y����Ŵ�һ����z������Сһ��
*/
/*DLL_EXPORT*/ VOID SetModelScale(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pScale);

/** �õ�ģ�͵�����
@param out pScale : ģ�;�������ֵ�������ԭʼ��С�������������һ�εġ�
@reamarks: ���磺pScale����(1, 2, 0.5)���ͱ�ʾģ�����������ԭʼ�ߴ磬��X����û�����ţ�y����Ŵ���һ����z������С��һ��
*/
/*DLL_EXPORT*/ BOOL GetModelScale(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pScale);

//�Թ̶���Ϊ��������ģ��
// /*DLL_EXPORT*/ VOID ScaleModelByPoint(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale);

//Add By Baojiahui �����ƶ�����Լ��ƶ�����ʱλ�ò�׼ȷ�ķ���----------------------------------------------------------------------------------------------------------
/** �õ���ֱ����Ļ�����¶�Ӧ��3d������ĳ��ƽ���ϵĽ�������
@param in sceneHandle      :����������ָ��
@param in vpHandle         :�ӿ�ָ��
@param in ScreenX, ScreenY :�����RenderWindow�������Ļ����
@param in planeNormal*      :�ض�ƽ��ķ�������ָ��
@param in planePoint*       :���ض�ƽ��ĵ�����ָ��
@param out vec3WorldPoint*  :���ص�ƽ�潻������ָ��
*/
/*DLL_EXPORT*/ VOID GetWorldPointUnderLoc(DWORD sceneHandle,DWORD vpHandle,UINT ScreenX, UINT ScreenY,VECTOR3* planeNormal,VECTOR3* planePoint,VECTOR3* vec3WorldPoint);
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

/** �õ���Ļ�����¶�Ӧ��3d�����е�ģ�ͣ����ж��ģ�ͣ��õ������һ�������ػ򱻶����model���ᱻѡ��
@param in sceneHandle      : ����������ָ��
@param in vpHandle         : �ӿ�ָ��
@param in ScreenX, ScreenY : �����RenderWindow�������Ļ����
@param in modeltype        : 0 : ��ѡ��ģ�ͣ� 1��ѡ�����У������ƹ�����
@return : ģ��ָ��
*/
/*DLL_EXPORT*/ DWORD GetModelUnderLoc(DWORD sceneHandle, DWORD vpHandle, UINT ScreenX, UINT ScreenY, int modeltype = 0);

  //-----------------------------------------------------------------------------
// ������: GetModelUnderLoc()
// ��  ��: VECTOR* rayVec, FLOAT* dis
// ��  ;: �õ�ʸ�������ཻ�������ģ�ͣ���Ҫ���ģ�͵�collision����Ҫ����ΪTRUE
//-----------------------------------------------------------------------------
/*DLL_EXPORT*/ DWORD GetModelUnderRay(DWORD sceneHandle, DWORD swapChainHandle, 
					   VECTOR3* rayPoint, VECTOR3* rayVec,
					   FLOAT* dis, VECTOR3* pVNormal );
/** ��ȡģ��������Ϣ
*/
/*DLL_EXPORT*/ BOOL GetModelInfo(DWORD sceneHandle, DWORD modelHandle, VGS_ModelInfo &Info);

/** ����ģ��������Ϣ
*/
/*DLL_EXPORT*/ VOID SetModelInfo(DWORD sceneHandle, DWORD modelHandle, const VGS_ModelInfo &Info);

/** ����ģ�͵�����¼���������
@param scenHandle   in : ����ָ��
@param modelHandle  in : ģ��ָ��
@param type         in  : �¼�����
@param sEventName   in : ��������
*/
/*DLL_EXPORT*/ VOID SetModelMouseEvent(DWORD sceneHandle, DWORD modelHandle, VGS_MOUSEEVENT_TYPE type, const CHAR* sEventName);

/** ��ȡģ�͵�����¼���������
@param scenHandle   in  : ����ָ��
@param modelHandle  in  : ģ��ָ��
@param type         in  : �¼�����
@param sEventName   out : ��������
@return ���ģ���Ѿ��ƶ��˶�Ӧ���¼�����������TRUE������FALSE
*/
/*DLL_EXPORT*/ BOOL GetModelMouseEvent(DWORD sceneHandle, DWORD modelHandle, VGS_MOUSEEVENT_TYPE type, CHAR* sEventName);

/** ���������Ϊbillboard���
*/
/*DLL_EXPORT*/ VOID SetBillboardCamera(DWORD sceneHandle, DWORD camHanlde);

/** ��ȡbillboard���
*/
/*DLL_EXPORT*/ DWORD GetBillboardCamera(DWORD sceneHandle); 

/** ����ģ������Billboard״̬
@param axis in : ����ģ�͵��ĸ������ᣨ����ָ�����
*/
/*DLL_EXPORT*/ VOID SetModelBillboardStatus(DWORD sceneHandle, DWORD modelHandle, BOOL status, VGS_AXIS_TYPE axis = VGS_LAXIS_NEGATIVEZ);

/** �ж�ģ���Ƿ�ΪBillboardģ��
@param axis in : ģ�͵��ĸ������ᣨ����ָ�����
*/
/*DLL_EXPORT*/ BOOL GetModelBillboardStatus(DWORD sceneHandle, DWORD modelHandle, VGS_AXIS_TYPE &axis);

//-----------------------------------------------------------------------------
// ������: SetBillboardUp()
// ��  ��: DWORD modelHandle, VECTOR3* pModelRay, VECTOR3* pAt, VECTOR3* pUp
//modelHandle ->ģ�͵�ָ��
//pUp ->ģ�͵��Ϸ���ʲô����Ʃ��Tree��Ҫ��Z��������Ŀ��㣬��up����ʼ��Ϊ(0,1,0), ����͵���
// ��  ;: ����ģ������Billboard
//-----------------------------------------------------------------------------
/*DLL_EXPORT*/ VOID SetBillboardUp(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pUp);

//����ģ������Billboard��Up����
///*DLL_EXPORT*/ VOID SetBillboardUp(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pUp);

////���ó�����ײ����Ч״̬
///*DLL_EXPORT*/ VOID EnableCollision(DWORD sceneHandle, BOOL status);

////������ײ��������Ч����, Ĭ��Ϊ1.0
///*DLL_EXPORT*/ VOID SetColDistance(DWORD sceneHandle, FLOAT distance);

/** ����ģ��Ϊˮƽ��ײģ��
*/
/*DLL_EXPORT*/ VOID SetModelCollisionStatus(DWORD sceneHandle, DWORD modelHandle, BOOL status);

/** �ж�ģ���Ƿ�Ϊˮƽ��ײģ��
*/
/*DLL_EXPORT*/ BOOL	GetModelCollisionStatus(DWORD sceneHandle, DWORD modelHandle);

/** ����ģ��Ϊ����ģ��
*/
/*DLL_EXPORT*/ VOID SetModelAsGround(DWORD sceneHandle, DWORD modelHandle, BOOL status);

/** �ж�ģ���Ƿ�Ϊ����ģ��
*/
/*DLL_EXPORT*/ BOOL GetModelAsGround(DWORD sceneHandle, DWORD modelHandle);

////�ƶ�ģ��Diffuse��ͼ��UV
///*DLL_EXPORT*/ VOID MoveModelUV(DWORD sceneHandle, DWORD modelHandle, FLOAT x, FLOAT y);
//
////����ģ�Ͷ�̬UV��״̬
///*DLL_EXPORT*/ VOID SetModelDynamicUVStatus(DWORD sceneHandle, DWORD modelHandle, BOOL status);
//
////���ģ�Ͷ�̬UV��״̬
///*DLL_EXPORT*/ BOOL GetModelDynamicUVStatus(DWORD sceneHandle, DWORD modelHandle);
//
////����ģ�͵Ķ�̬UV
///*DLL_EXPORT*/ VOID SetModelDynamicUV(DWORD sceneHandle, DWORD modelHandle, FLOAT u, FLOAT v);
////���ģ�͵Ķ�̬UV
///*DLL_EXPORT*/ VOID GetModelDynamicUV(DWORD sceneHandle, DWORD modelHandle, FLOAT* u, FLOAT* v);


/** ����ģ���Ƿ�ɼ�
*/
/*DLL_EXPORT*/ VOID SetModelVisibility(DWORD sceneHandle, DWORD modelHandle, BOOL status);

/** ��ȡģ���Ƿ�ɼ�
*/
/*DLL_EXPORT*/ BOOL GetModelVisibility(DWORD sceneHandle, DWORD modelHandle);

/** ����ģ���ڲ���״̬���Ƿ���Ե��
*/
/*DLL_EXPORT*/ VOID SetModelClickable(DWORD sceneHandle, DWORD modelHandle, BOOL status);

/** ��ȡģ���ڲ���״̬���Ƿ���Ե��
*/
/*DLL_EXPORT*/ BOOL GetModelClickabel(DWORD sceneHandle, DWORD modelHandle);

//����ģ��ΪSkyBoxģ��
/*DLL_EXPORT*/ VOID SetSkyBox(DWORD sceneHandle, DWORD modelHandle, BOOL status);

//�ж�ģ���Ƿ�ΪSkyBox
/*DLL_EXPORT*/ BOOL IsSkyBox(DWORD sceneHandle, DWORD modelHandle);

/** ����ģ�͵���ɫ
*/
/*DLL_EXPORT*/ VOID SetModelColor(DWORD sceneHandle, DWORD modelHandle, BYTE red, BYTE green, BYTE blue, BYTE alpha);
///*DLL_EXPORT*/ VOID GetModelColor(DWORD sceneHandle, DWORD modelHandle, BYTE &red, BYTE &green, BYTE &blue, BYTE *alpha);

/** ����ģ��͸��ͼ
*/
/*DLL_EXPORT*/ VOID SetModelOpacity(DWORD sceneHandle, DWORD modelHandle, INT opacity);
///*DLL_EXPORT*/ INT GetModelOpacity(DWORD sceneHandle, DWORD modelHandle);

/** ģ���Ƿ�����ؼ�֡����
*/
/*DLL_EXPORT*/ BOOL IsModelHasKeyframeAnimation(DWORD sceneHandle, DWORD modelHandle);

/** ģ���Ƿ������������
*/
/*DLL_EXPORT*/ BOOL IsModelHasSkeletonAnimation(DWORD sceneHandle, DWORD modelHandle);

////����ģ�͵���ɫ
///*DLL_EXPORT*/ VOID SetModelColor(DWORD sceneHandle, DWORD modelHandle, INT red, INT green, INT blue, INT alpha);
//
////�õ�ģ�͵�͸����
///*DLL_EXPORT*/ INT GetModelOpacity(DWORD sceneHandle, DWORD modelHandle);
//
////����ģ�͵�͸����
///*DLL_EXPORT*/ VOID SetModelOpacity(DWORD sceneHandle, DWORD modelHandle, INT opacity);

// ��ģ�ͷ���
/** ��ȡģ����ģ��ʹ�õĲ������ƣ�ÿ����ģ��ֻ����һ������
@param in sceneHandle �� ����������
@param in modelHandle ��ģ����ָ��
@return ��ģ����Ŀ
**/
/*DLL_EXPORT*/ INT GetSubModelCount(DWORD sceneHandle, DWORD modelHandle);

// ��ȡģ����ģ��ʹ�õĲ������ƣ�ÿ����ģ��ֻ����һ������
/*DLL_EXPORT*/ BOOL GetSubModelMaterialName(DWORD modelHandle, int index, CHAR *sName);

// ��ȡģ����ģ��ʹ�õĲ���ָ�룬ÿ����ģ��ֻ����һ������
/*DLL_EXPORT*/ DWORD GetSubModelMaterialHandle(DWORD modelHandle, int index);

/** Ϊģ���еĵ�index����ģ��ָ������
@param in modelHandle : ����������ָ��
@param in index : ��ģ������
@param in pMtrName : ��������
*/
/*DLL_EXPORT*/ VOID SetSubModelMaterial(DWORD sceneHandle, DWORD modelHandle, int index, const CHAR* pMtrName);

/**Ϊģ���ƶ����ʣ����ģ�Ͱ��������ģ�ͣ���ô������ģ�Ͷ���ʹ���������
@param in modelHandle : ����������ָ��
@param in pMtrName : ��������
*/
/*DLL_EXPORT*/ VOID SetModelMaterial(DWORD sceneHandle, DWORD modelHandle, const CHAR* pMtrName);

//-------------------------------------------------------------------------------------------
///������صķ���
//-------------------------------------------------------------------------------------------
/** ��������
@param strMtrlName in : Ҫ�����Ĳ�������
*/
/*DLL_EXPORT*/ DWORD CreateMaterial(DWORD sceneHandle, const CHAR* strMtrlName);

/** ɾ������
@param mtrlHandle in : Ҫɾ���Ĳ���ָ��
*/
/*DLL_EXPORT*/ VOID	DeleteMaterialByHandle(DWORD sceneHandle, DWORD mtrlHandle);

/** ɾ������
@param pName in : Ҫɾ���Ĳ�������
*/
/*DLL_EXPORT*/ VOID DeleteMaterialByName(DWORD sceneHandle, const CHAR* pName);

/** �õ������в��ʵ�����
*/
/*DLL_EXPORT*/ INT GetMaterialCount(DWORD sceneHandle);

/** ���ݲ��ʵ����Ƶõ����ʵľ��
*/
/*DLL_EXPORT*/ DWORD GetMaterialHandleByName(DWORD sceneHandle, const CHAR* strMtrlName);

/** ���ݲ��ʵı�ŵõ����ʵľ��
*/
/*DLL_EXPORT*/ DWORD GetMaterialHandleByIndex(DWORD sceneHandle, INT index);

/** ���ݲ��ʵľ���õ����ʵ�����
*/
/*DLL_EXPORT*/ VOID GetMaterialNameByHandle(DWORD sceneHandle, DWORD mtrlHandle, CHAR* strMtrlName);

/** ���ò��ʵĶ�̬UV�ٶ�
*/
// /*DLL_EXPORT*/ VOID SetMaterialUVSpeed(DWORD sceneHandle, DWORD mtrlHandle, float USpeed, float VSpeed);
/*DLL_EXPORT*/ VOID SetTextureUVSpeed(DWORD sceneHandle, DWORD textureHandle, float USpeed, float VSpeed);

/** ��ȡ���ʵĶ�̬UV�ٶ�
*/
// /*DLL_EXPORT*/ VOID GetMaterialUVSpeed(DWORD sceneHandle, DWORD mtrlHandle, float &USpeed, float &VSpeed);
/*DLL_EXPORT*/ VOID GetTextureUVSpeed(DWORD sceneHandle, DWORD textureHandle, float &USpeed, float &VSpeed);


///////////// texture ///////////////////////////////////

// ��Image buffer����ʹ�ø�Image������texture
/*DLL_EXPORT*/ BOOL FillTexturesFromBuffer(UINT len, CHAR* pBuffer, INT imgIndex);

/** Ϊ��������һ�������ͼ�������ӵ���ͼ���������ϲ�
@param sceneHandle   in : VGS����ָ��
@param pMaterailName in : ��������
@param pTextureFile  in : textureʹ�õ�ͼƬ���ƣ��������·��
@param pTextureName  in : texture������ƣ�����NULL��ʾ�Զ�����
@param UVIndex		 in : textureʹ����һ��UV���꣬ĿǰVGS֧��4��UV����
@return �´�����texture���
*/
/*DLL_EXPORT*/ DWORD AddMaterialBasalTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* pTextureFile, 
										 const CHAR* pTextureName = NULL, int UVIndex = 0);

/** Ϊ��������һ��͸����ͼ�������ӵ���ͼ���������ϲ�
@param sceneHandle   in : VGS����ָ��
@param pMaterailName in : ��������
@param pTextureFile  in : textureʹ�õ�ͼƬ���ƣ���������·��
@param pTextureName  in : texture������ƣ�����NULL��ʾ�Զ�����
@param UVIndex		 in : textureʹ����һ��UV���꣬ĿǰVGS֧��4��UV����
@return �´�����texture���
*/
/*DLL_EXPORT*/ DWORD AddOpacityTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* pTextureFile, const CHAR* pTextureName = NULL, int UVIndex = 0);

/** Ϊ��������һ�㷴����ͼ
@param hScene   in : VGS����ָ��
@param szMtrl in : ��������
@param szTextureFile  in : textureʹ�õ�ͼƬ���ƣ���������·��
@param szTexture  in : texture������ƣ�����NULL��ʾ�Զ�����
@param nUVindex	 in : textureʹ����һ��UV���꣬ĿǰVGS֧��4��UV����
@param vrt	 in : ��������
@return �´�����texture���
*/
/*DLL_EXPORT*/ DWORD AddReflectTexture(DWORD hScene, const char* szMtrl, const char* szTextureFile, const char* szTexture = NULL, int nUVindex = 0, VGS_REFLECT_TYPE vrt = VRT_CURVED);
/** ���÷�����ͼ������--added by Linxq
@param hScene   in : VGS����ָ��
@param hTexture in : ��ͼ���
@param vrt	 in : ��������
@return:�����Ƿ�ɹ�
*/
/*DLL_EXPORT*/ BOOL SetReflectTextureType(DWORD hScene, DWORD hTexture, VGS_REFLECT_TYPE vrt);
/** ��ȡ������ͼ������--added by Linxq
@param hScene   in : VGS����ָ��
@param hTexture in : ��ͼ���
@return:��������
*/
/*DLL_EXPORT*/ VGS_REFLECT_TYPE GetReflectTextureType(DWORD hScene, DWORD hTexture);

/** ��ȡ������ͼ��������--added by linxq --2009-6-3
@param hScene       in : VGS����ָ��
@param hTexture in : ��ͼ����
@return:������ͼ����.
*/
/*DLL_EXPORT*/ VGS_CubemapType GetCubeTextureType(DWORD hScene, DWORD hTexture);

/** Ϊ��������һ�㾲̬������ͼ�������ӵ���ͼ���������ϲ�
@param sceneHandle   in : VGS����ָ��
@param pMaterailName in : ��������
@param pTextureFile  in : textureʹ�õ�ͼƬ���У��������·����cubic�������6��ͼƬ��ͼƬ��Ҫ����ǰ���������µ�˳���ṩ
@param pMaterailName in : �������ƣ����Ϊ�գ����Զ�����
@return �´�����texture���
*/
/*DLL_EXPORT*/ DWORD AddMaterialCubicStaticTexture(DWORD sceneHandle, const CHAR* pMaterailName, DWORD* pTextureFile, 
    									       const CHAR* pTextureName = NULL);

/** Ϊ��������һ�㶯̬������ͼ�������ӵ���ͼ���������ϲ�
@param sceneHandle   in : VGS����ָ��
@param pMaterailName in : �������ƣ����Ϊ�գ����Զ�����
@param pTextureName  in : ������texture����
@return �´�����texture���
*/
/*DLL_EXPORT*/ DWORD AddMaterialCubicDynamicTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* pTextureName = NULL);

/** Ϊ��̬������ͼ���һ������ģ��
@param sceneHandle       in : VGS����ָ��
@param DynamicCubeHandle in : ��̬cubemapָ��
@param modelHandle       in : ������ģ�;��
*/
// �Ƴ���̬cubemap�ķ���ģ��
/*DLL_EXPORT*/ VOID AddDynamicReflectedModel(DWORD sceneHandle, DWORD DynamicCubeHandle, DWORD modelHandle);

/** Ϊ��̬������ͼ�Ƴ�һ������ģ��
@param sceneHandle       in : VGS����ָ��
@param DynamicCubeHandle in : ��̬cubemapָ��
@param modelHandle       in : ���Ƴ���ģ�;��
*/
// �Ƴ���̬cubemap�ķ���ģ��
/*DLL_EXPORT*/ VOID RemoveDynamicReflectedModel(DWORD sceneHandle, DWORD DynamicCubeHandle, DWORD modelHandle);
/** �Ƴ���̬cubemap��ȫ������ģ��.--added by linxq --2009-6-3
@param hScene       in : VGS����ָ��
@param hDynamicCubeMap in : ��̬cubemapָ��
@return : ӳ���ģ������.
*/
/*DLL_EXPORT*/ VOID RemoveDynamicReflectedAllModel(DWORD hScene, DWORD hDynamicCubeMap);
/** ��ȡ��̬������ͼ��ģ������.--added by linxq --2009-6-3
@param hScene       in : VGS����ָ��
@param hDynamicCubeMap in : ��̬cubemapָ��
@return : ӳ���ģ������.
*/
/*DLL_EXPORT*/ int GetDynamicReflectedModelCount(DWORD hScene, DWORD hDynamicCubeMap);
/** ��������ȡ��̬������ͼ��ģ�;��.--added by linxq --2009-6-3
@param hScene       in : VGS����ָ��
@param hDynamicCubeMap in : ��̬cubemapָ��
@param nIdx in: ��Ч��������.
@return : ģ�;��.
*/
/*DLL_EXPORT*/ DWORD GetDynamicReflectedModelHandle(DWORD hScene, DWORD hDynamicCubeMap, int nIdx);


/** Ϊ��������һ��bump��ͼ�������ӵ���ͼ���������ϲ�
@param sceneHandle   in : VGS����ָ��
@param pMaterailName in : ��������
@param pTextureFile  in : textureʹ�õ�ͼƬ����������·���������ָ��ͼƬ������NULL��һ�����ַ�����
@param UVIndex		 in : textureʹ����һ��UV���꣬ĿǰVGS֧��4��UV����
@return �´�����texture���
@remark:ȱʡ����VBT_ENVIRONMENT���͵�bump��ͼ
*/
/*DLL_EXPORT*/ DWORD AddMaterialBumpTexture(DWORD sceneHandle, const char* pMaterailName, const CHAR* pTextureFile, VGS_BumpType type, int UVIndex = 0);


/*DLL_EXPORT*/ DWORD AddMaterialBumpwaveTexture(DWORD sceneHandle, const char* pMaterailName, INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY);

/*DLL_EXPORT*/ DWORD AddMaterialBumpEnviromentTexture(DWORD sceneHandle, const char* pMaterailName, const char* pTextureFile, float noise1, float noise2);

/*DLL_EXPORT*/ DWORD AddMaterialBumpNormalTexture(DWORD sceneHandle, const char* pMaterailName, const char* pTextureFile, DWORD lightHandle);
/** ��ȡbump��ͼ������.--added by linxq --2009-6-8
@param hScene in : VGS����ָ��
@param hBump in: bump��ͼ���.
@return : bump��ͼ������
*/
/*DLL_EXPORT*/ VGS_BumpType GetBumpTextureType(DWORD hScene, DWORD hBumpTexture);

/** ����bump water�Ĳ���
@param sceneHandle      in : VGS����ָ��
@param pTexture         in : texture���
@param dwWidth/dwHeight in : ��ͼ�ĳߴ�
@param xSize/ySize      in :
@param radius:
@param speed:
@param centerX/centerY
*/
/*DLL_EXPORT*/ VOID SetBumpwaveTextureParam(DWORD sceneHandle, DWORD pTexture, 
							  INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY);

/** Ϊ��������һ�㶯̬��ͼ�������ӵ���ͼ���������ϲ�
@param sceneHandle   in : VGS����ָ��
@param pMaterailName in : ��������
@param pTextureFile  in : textureʹ�õ�ͼƬ���У��������·��
@param Duration      in : ��������ͼƬ��ʱ�䳤�ȣ���λ�롣���磬���һ����5��ͼƬ��Duration = 1����ôƽ��200���벥��һ��ͼƬ
@param UVIndex		 in : textureʹ����һ��UV���꣬ĿǰVGS֧��4��UV����
@return �´�����texture���
*/
/*DLL_EXPORT*/ DWORD AddMaterialDynamicTexture(DWORD sceneHandle, const CHAR* pMaterailName, int FrameCount, DWORD* pTextureFile, int Duration, int UVIndex = 0);

/**@param pMaterailName     in : ��������
@param sTextureLayerName in : ͼ������
@param width/height      in : ������ͼ�ĳߴ�
*/
/*DLL_EXPORT*/ DWORD AddMaterialRTReflectionTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* sTextureLayerName, INT width = 256, INT height = 256);

/** ������ͼ��Ϊʵʱ����
*/
/*DLL_EXPORT*/ VOID setTextureAsRTReflection(DWORD sceneHandle, DWORD textureHandle);

/** ����ʵʱ����ģ���Ի�ȡ����ƽ��
@param textureHandle    in : ͼ����
@param hReflectionModel in : ָ����ģ��ָ�룬ͨ����ģ�ͻ�ȡ����ƽ��
*/
/*DLL_EXPORT*/ VOID setRTReflectionModel(DWORD sceneHandle, DWORD textureHandle, DWORD hReflectionModel);

/** ��ȡʵʱ�����ģ��
*/
/*DLL_EXPORT*/ DWORD getRTReflectionModel(DWORD sceneHandle, DWORD textureHandle);

/** ��ȡ��ʵʱ�����ģ����Ŀ
*/
/*DLL_EXPORT*/ DWORD getRTReflectedModelCount(DWORD sceneHandle, DWORD textureHandle);

/** ����������ȡ�������ģ��
*/
/*DLL_EXPORT*/ DWORD getRTReflectedModelByIndex(DWORD sceneHandle, DWORD textureHandle, INT index);

/** ���ñ�ʵʱ�����ģ���б� 
@param textureHandle     in : ͼ����
@param count             in : ������ģ�͵ĸ���
@param phReflectedModels in : �������ģ��ָ���б�
*/
/*DLL_EXPORT*/ VOID setRTReflectedModel(DWORD sceneHandle, DWORD textureHandle, DWORD count, DWORD* phReflectedModels);

/** ��ȡ������ͼ�ĳߴ�
*/
/*DLL_EXPORT*/ BOOL getRTReflectionDimension(DWORD sceneHandle, DWORD textureHandle, INT &width, INT &height);

/** ���÷�����ͼ�ĳߴ�
*/
/*DLL_EXPORT*/ VOID setRTReflectionDimension(DWORD sceneHandle, DWORD textureHandle, INT width, INT height);

/** Ϊ�ƶ��Ĳ�������һ��ʵʱ�����
@param sceneHandle       in : ����ָ��
@param pMaterailName     in : ��������
@param sTextureLayerName in : ͼ������
@param width/height      in : ������ͼ�ĳߴ�
*/
/*DLL_EXPORT*/ DWORD AddMaterialRTRefractionTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* sTextureLayerName, INT width = 256, INT height = 256);

/** ����ͼ��Ϊʵʱ����
*/
/*DLL_EXPORT*/ VOID setTextureAsRTRefraction(DWORD sceneHandle, DWORD textureHandle);

/** ����ʵʱ����ģ���Ի�ȡ����ƽ��
@param textureHandle    in : ͼ����
@param hReflectionModel in : ָ����ģ��ָ�룬ͨ����ģ�ͻ�ȡ����ƽ��
*/
/*DLL_EXPORT*/ VOID setRTRefractionModel(DWORD sceneHandle, DWORD textureHandle, DWORD hRefractionModel);

/** ��ȡ����ģ��
*/
/*DLL_EXPORT*/ DWORD getRTRefractionModel(DWORD sceneHandle, DWORD textureHandle);

/** ��ȡ�������ģ����Ŀ
*/
/*DLL_EXPORT*/ DWORD getRTRefractedModelCount(DWORD sceneHandle, DWORD textureHandle);

/** ��ȡ�������ģ��
*/
/*DLL_EXPORT*/ DWORD getRTRefractedModelByIndex(DWORD sceneHandle, DWORD textureHandle, INT index);

/** ���ñ�ʵʱ�����ģ���б� 
@param textureHandle     in : ͼ����
@param count             in : ������ģ�͵ĸ�����Ϊ0��phReflectedModelsΪ��ʱ���������õ�ģ���б�������.
@param phReflectedModels in : �������ģ��ָ���б�
*/
/*DLL_EXPORT*/ VOID setRTRefractedModel(DWORD sceneHandle, DWORD textureHandle, DWORD count, DWORD* phRefractedModels);

/** ��ȡ������ͼ�ĳߴ�
*/
/*DLL_EXPORT*/ BOOL getRTRefractionDimension(DWORD sceneHandle, DWORD textureHandle, INT &width, INT &height);

/** ����������ͼ�ĳߴ�
*/
/*DLL_EXPORT*/ VOID setRTRefractionDimension(DWORD sceneHandle, DWORD textureHandle, INT width, INT height);

// /*DLL_EXPORT*/ VOID SetTextureAsProjective(DWORD sceneHandle, DWORD textureHandle, BOOL enabled, DWORD cameraHandle);

/** ������ͼΪbump
@param type : ����bump֮һ
*/
/*DLL_EXPORT*/ VOID SetTextureAsBump(DWORD sceneHandle, DWORD textureHandle, VGS_BumpType type);

/** ����bump��ͼ�Ĳ���
@param type  in : bump������
@param param in : ����
*/
/*DLL_EXPORT*/ VOID SetBumpTextureParam(DWORD sceneHandle, DWORD textureHandle, const VGS_BumpParam &param);

/** ��ȡbump��ͼ�Ĳ���
@param type  out : bump������
@param param out : ����
*/
/*DLL_EXPORT*/ BOOL GetBumpTextureParam(DWORD sceneHandle, DWORD textureHandle, VGS_BumpParam &param);

/** �ж�һ��texture�Ƿ�����bump
@param type out : �����bump��type����bump������
@return �����bump������TRUE������FALSE
*/
/*DLL_EXPORT*/ BOOL GetTextureAsBump(DWORD sceneHandle, DWORD textureHandle, VGS_BumpType &type);

/** ��ȡ���ʵ�texture��Ŀ
@param sceneHandle   in : VGS����ָ��
@param pMaterailName in : ��������
@return texture����Ŀ 
*/
/*DLL_EXPORT*/ DWORD GetMaterialTextureCount(DWORD sceneHandle, const CHAR* pMaterailName);

/** ��ȡ���ʵĵ�textureIndex��texture���
@param sceneHandle   in : VGS����ָ��
@param pMaterailName in : ��������
@param textureIndex  in : texture����
@return texture�ľ��
*/
/*DLL_EXPORT*/ DWORD GetMaterialTextureHandle(DWORD sceneHandle, const CHAR* pMaterailName, int textureIndex);

/** ��ղ��ʵ�һ��texture����, ��պ�texture��ָ����Ȼ��Ч
@param sceneHandle   in : VGS����ָ��
@param pMaterailName in : ��������
@param textureHandle in : texture���
*/
/*DLL_EXPORT*/ VOID ClearMaterialTexture(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle);

/** ɾ�����ʵ�һ��texture��ɾ���󣬸ò�texture֮�������texture��������ı�, <b>ʹ��ʱһ��Ҫע��</b>��
@param sceneHandle   in : VGS����ָ��
@param pMaterailName in : ��������
@param textureHandle in : texture���
*/
/*DLL_EXPORT*/ VOID RemoveMaterialTexture(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle);

/** ����textureIndex�����͡�<b>ע�⣺���texture�����͸ı䣬texture�����ݽ��ᱻ���</b>��
@param sceneHandle   in : VGS����ָ��
@param pMaterailName in : ��������
@param textureHandle in : texture���
@param type          in : texture ������
*/
///*DLL_EXPORT*/ VOID SetTextureType(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle, VGS_TextureType type);

/** ��ȡtextureIndex������
@param sceneHandle   in : VGS����ָ��
@param pMaterailName in : ��������
@param textureHandle  in : texture���
@return texture������
*/
/*DLL_EXPORT*/ VGS_TextureType GetTextureType(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle);

/** ��ȡtextureIndexʹ�õ�UV������
@param sceneHandle   in : VGS����ָ��
@param textureHandle  in : texture���
@return texture��UV������
*/
/*DLL_EXPORT*/ INT  GetTextureUVIndex(DWORD sceneHandle, DWORD textureHandle);

/** ����textureIndexʹ�õ�UV������
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param UVIndex		 in : textureʹ�õ�UV������
*/
/*DLL_EXPORT*/ VOID SetTextureUVIndex(DWORD sceneHandle, DWORD textureHandle, INT UVIndex);

/** ���û�����ͼʹ�õ�ͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param pTextureFile	 in : textureʹ�õ�ͼ���ļ�����������·��
*/
// /*DLL_EXPORT*/ VOID SetTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile);
/*DLL_EXPORT*/ VOID SetBasalTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile);
/*DLL_EXPORT*/ VOID SetTransparentTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile);

/** ����cubic��ͼʹ�õ�ͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param pTextureFile	 in : textureʹ�õ�ͼ���ļ����У���������·����������6��ͼƬ�����Ұ���ǰ���������µ�˳���ṩ
*/
/*DLL_EXPORT*/ VOID SetCubicTextureImages(DWORD sceneHandle, DWORD textureHandle, DWORD* pTextureFile);

/** Ϊ��̬��ͼ������������ʹ�õ�ͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param FrameCount    in : textureʹ�õ�ͼƬ��Ŀ
@param pTextureFile  in : textureʹ�õ�ͼƬ�б�
*/
/*DLL_EXPORT*/ VOID SetDynamicTextureImages(DWORD sceneHandle, DWORD textureHandle, int FrameCount, DWORD* pTextureFile);

/** ����Bump��ͼʹ�õ�ͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param pTextureFile	 in : textureʹ�õ�ͼ���ļ�����������·��. �������ҪͼƬ������ΪNULL����ַ�����
*/
/*DLL_EXPORT*/ VOID SetBumpTextureImage(DWORD sceneHandle, DWORD textureHandle, const char* pTextureFile);

/** ���ö�̬��ͼʹ�õ�ͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param index		 in : ��̬��ͼ�����е�ͼƬ����
@param pTextureFile	 in : textureʹ�õ�ͼ���ļ��� ��������·��
*/
/*DLL_EXPORT*/ VOID SetDynamicTextureImageByIndex(DWORD sceneHandle, DWORD textureHandle, INT index, const CHAR* pTextureFile);

/** ���һ�Ŷ�̬��ͼͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param pTextureFile	 in : Ҫ��ӵ�textureͼ���ļ��� ��������·��
*/
/*DLL_EXPORT*/ VOID AddDynamicTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile);

/** ɾ����̬��ͼʹ�õ�ͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param index		 in : ��̬��ͼ�����е�ͼƬ����
@return ɾ���ɹ�����TRUE������FALSE
*/
/*DLL_EXPORT*/ BOOL DeleteDynamicTextureImageByIndex(DWORD sceneHandle, DWORD textureHandle, INT FrameIdx);

/** ���ö�̬��ͼ�Ĳ������ڳ��ȣ���λ�롣 ע�⣺���ͼƬ����Ϊ�գ����ø÷�������Ч�ġ�
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param Duration      in : ��̬��ͼ���в���һ�����������ʱ�䳤��,��λ��
*/
/*DLL_EXPORT*/ VOID SetDynamicTextureDuration(DWORD sceneHandle, DWORD textureHandle, INT Duration);


/** ��ȡ������ͼʹ�õ�ͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param pTextureFile	 out : textureʹ�õ�ͼ���ļ��� �� ����·��
@return �õ���ͼ������TRUE,����FALSE
*/
// /*DLL_EXPORT*/ BOOL GetTextureImage(DWORD sceneHandle, DWORD textureHandle, CHAR* pTextureFile);

/** ��ȡ������ͼʹ�õ�ͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param pTextureFile	 out : textureʹ�õ�ͼ���ļ��� �� ����·��
@return �õ���ͼ������TRUE,����FALSE
*/
/*DLL_EXPORT*/ BOOL GetBasalTextureImage(DWORD sceneHandle, DWORD textureHandle, char* pTextureFile);

/** ��ȡbump��ͼʹ�õ�ͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param pTextureFile	 out : textureʹ�õ�ͼ���ļ��� �� ����·��
*/
/*DLL_EXPORT*/ BOOL GetBumpTextureImage(DWORD sceneHandle, DWORD textureHandle, char* pTextureFile);

/** ��ȡ��̬��ͼ���в���ʱ�䳤��
@param sceneHandle	   in  : ����������
@param textureHandle   in  : textureָ��  
@return ��̬��ͼ���в���ʱ�䳤��
*/
/*DLL_EXPORT*/ INT GetDynamicDuration(DWORD sceneHandle, DWORD textureHandle);

/** ��ȡ������ͼʹ�õ�ͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param faceType      in : ָ��һ����
@param pTextureFile	 out : textureʹ�õ�ͼ���ļ��� �� ����·��
@return �õ���ͼ������TRUE,����FALSE
*/
/*DLL_EXPORT*/ BOOL GetCubicTextureImages(DWORD sceneHandle, DWORD textureHandle, VGS_CUBIC_FACE faceType, CHAR* pTextureFile);

/** ��ȡ��̬��ͼʹ�õ�ͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param pTextureFile	 out : textureʹ�õ�ͼ���ļ����У� �� ����·��
*/
/*DLL_EXPORT*/ INT GetDynamicTextureImageCount(DWORD sceneHandle, DWORD textureHandle);

/** ��ȡ��̬��ͼʹ�õ�ͼƬ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param index		 in : ��̬��ͼ�����е�ͼƬ����
@param pTextureFile	 out : textureʹ�õ�ͼ���ļ��� �� ����·��
@return �õ�ͼƬ������TRUE������FALSE
*/
/*DLL_EXPORT*/ BOOL GetDynamicTextureImageByIndex(DWORD sceneHandle, DWORD textureHandle, INT index, CHAR* pTextureFile);

/** ����texture�Ļ�ɫ��Ϣ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param BlenderInfo	 in : texture�Ļ�ɫ����
*/
/*DLL_EXPORT*/ VOID SetTextureBlenderInfo(DWORD sceneHandle, DWORD textureHandle, VGS_BlenderType type, const VGS_BlenderInfo &BlenderInfo);

/** ��ȡtexture�Ļ�ɫ��Ϣ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : texture���
@param BlenderInfo	 out : texture�Ļ�ɫ����
*/
/*DLL_EXPORT*/ VOID GetTextureBlenderInfo(DWORD sceneHandle, DWORD textureHandle, VGS_BlenderType type, VGS_BlenderInfo &BlenderInfo);

/** ��ȡ��ͼ��Mipmaps��Ŀ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : textureָ��
@return ��ͼ��Mipmaps��Ŀ
*/
/*DLL_EXPORT*/ INT GetTextureMipmaps(DWORD sceneHandle, DWORD textureHandle);

/** ������ͼ��Mipmaps��Ŀ
@param sceneHandle   in : VGS����ָ��
@param textureHandle in : textureָ��
@param Mipmaps       in : ��ͼ��Mipmaps��Ŀ
*/
/*DLL_EXPORT*/ VOID SetTextureMipmaps(DWORD sceneHandle, DWORD textureHandle, INT Mipmaps);

/** ��������Ĵ�С
@param sceneHandle  in  : ����������
@param mtrlHandle   in  : ����ָ��  
@param mapChanel    in  : ��ͼͨ�� 
@param width/height out : ͼƬ�Ĵ�С
*/
///*DLL_EXPORT*/ VIOD SetTextureSize(DWORD sceneHandle, DWORD textureHandle, INT width, INT height);

/** ��ȡ����Ĵ�С
@param sceneHandle  in  : ����������
@param mtrlHandle   in  : ����ָ��  
@param mapChanel    in  : ��ͼͨ�� 
@param width/height out : ͼƬ�Ĵ�С
@return TRUE�����ص�width/heigh��Ч��FALSE:���ص�width/heigh��Ч
*/
/*DLL_EXPORT*/ BOOL GetTextureSize(DWORD sceneHandle, DWORD textureHandle, INT &width, INT &height, INT iFrmIndex = 0);
//---------Flash Texture Start----linxq 2009-12-15
/** ��ָ���Ĳ������һ��FlashTexture.
@param hScene in:����.
@param pMtrlName in:�������ƣ�����Ϊ��.
@param pTxtrName in:Ҫ��������ͼ���ƣ����ɿ�.
@param pFlashFile in: Flash�ļ���·������.
@param flashWidth in: Flash�Ŀ��.
@param flashHeight in: Flash�߶�.
@param nWidth in:Ҫ��������ͼ�Ŀ��.
@param nHeight in:Ҫ��������ͼ�ĸ߶�.
@return:������ͼ.
*/
/*DLL_EXPORT*/ DWORD AddMaterialFlashTexture(DWORD hScene, const char* pMtrlName, const char* pTxtrName, const char* pFlashPath,
										 int flashWidth, int flashHeight, int nWidth, int nHeight); 
/** ���ø������Ƶ�Flash��ͼ�Ĳ���״̬.
@param hScene in:����.
@param textureHandle in:��ͼ.
@param nStatur in:����״̬����0:��ͣ,1:����,2:ֹͣ.
*/
/*DLL_EXPORT*/ VOID SetFlashTexturePlayStatus(DWORD hScene, DWORD textureHandle, int nStatus); 
/** ����Flash��ͼ�ƶ�ָ��֡.
@param hScene in:����.
@param textureHandle in:��ͼ.
@param nFrameIndex in: Ҫ�ƶ���Flash����.
*/
/*DLL_EXPORT*/ VOID SetFlashTextureGoToFrame(DWORD hScene, DWORD textureHandle, int nFrameIndex); 

//---------Flash Texture end----

///** ��ȡ��ͼͼ��Ĵ�С
//@param sceneHandle  in  : ����������
//@param mtrlHandle   in  : ����ָ��  
//@param mapChanel    in  : ��ͼͨ�� 
//@param width/height out : ͼƬ�Ĵ�С
//@return TRUE�����ص�width/heigh��Ч��FALSE:���ص�width/heigh��Ч
//*/
///*DLL_EXPORT*/ BOOL GetTextureSize(DWORD sceneHandle, DWORD mtrlHandle, INT mapChanel, INT &width, INT &height);

//���ò��ʵ���ɫ
/*DLL_EXPORT*/ VOID SetMaterialColor(DWORD sceneHandle, DWORD mtrlHandle, INT red, INT green, INT blue, INT alpha);

//��ò�����Ϣ
/*DLL_EXPORT*/ VOID GetMaterialInfo(DWORD sceneHandle, DWORD mtrlHandle, MtrlInfo* pMtrlInfo);
//���ò�����Ϣ
/*DLL_EXPORT*/ VOID SetMaterialInfo(DWORD sceneHandle, DWORD mtrlHandle, MtrlInfo* pMtrlInfo);

//�жϵ�ǰ�����Ƿ�ģ�͹���
/*DLL_EXPORT*/ DWORD IsMtrlUsingByModel(DWORD sceneHandle, DWORD mtrlHandle);

//�����Ƿ���ʾ���ʵ���ͼ
/*DLL_EXPORT*/ VOID SetShowMtrlTexture(DWORD sceneHandle, DWORD mtrlHandle, BOOL showTexture);
//����Ƿ���ʾ��ͼ��״̬
/*DLL_EXPORT*/ BOOL GetShowMtrlTexture(DWORD sceneHandle, DWORD mtrlHandle);

/** ��ȡָ���Ĳ����Ƿ�ʹ������ͼ
*/
/*DLL_EXPORT*/ BOOL IsMaterialHasTexture(DWORD sceneHandle, DWORD mtrlHandle);

//-------------------------------------------------------------------------------------------
///��ͼ��صķ���
//-------------------------------------------------------------------------------------------
//�ӱ����ļ�������ͼ
///*DLL_EXPORT*/ DWORD CreateTextureFromFile(DWORD sceneHandle, CHAR* texName, 
//									   CHAR* strImageFile, INT mipLevel);
////ɾ����ͼ
///*DLL_EXPORT*/ VOID	DeleteTexture(DWORD sceneHandle, DWORD texHandle);

/** �õ���������ͼ������, ����2D��3Dʹ�õ���ͼ
*/ 
// /*DLL_EXPORT*/ UINT GetTextureCount(DWORD sceneHandle);
/*DLL_EXPORT*/ UINT GetTextureImageCount(DWORD sceneHandle);

/** ���������õ�������ļ����ƣ�������·����
@param in sceneHandle : ����ָ��
@param in index : �����ڶ����е�����
@param out TextureName : ��������
*/
// /*DLL_EXPORT*/ BOOL GetTextureNamebyIndex(DWORD sceneHandle, INT index, CHAR* TextureName);
/*DLL_EXPORT*/ BOOL GetTextureImageNamebyIndex(DWORD sceneHandle, INT index, CHAR* TextureName);

/** ͼ���Ƿ�����ʹ��
@param ImagePureName in : ͼ���ļ����ƣ�������·��
*/
/*DLL_EXPORT*/ BOOL IsImageInused(DWORD sceneHandle, const CHAR* ImagePureName);

// �õ��������ٸ�����ʹ��
// /*DLL_EXPORT*/ DWORD GetTextureReferencedCount(DWORD sceneHandle, const CHAR* pTextureName);

/** �õ���index������ʹ��ָ������Ĳ�����Ϣ
@param in sceneHandle : ����ָ��
@param in pTextureName:�����ļ����ƣ�������·��
@param in iMaterialIndex:ʹ��ָ������Ĳ������
@param out pMaterailName : ��������
@param out mapChannel : ͨ��
@return �ɹ��õ�������Ϣ����true������false
*/
// /*DLL_EXPORT*/ BOOL GetMaterialInfoofTexture(DWORD sceneHandle, const CHAR* pTextureName, INT iMaterialIndex, CHAR* pMaterailName/*, INT &mapChannel*/);

////������ͼ�����Ƶõ���ͼ�ľ��
///*DLL_EXPORT*/ DWORD GetTextureHandleByName(DWORD sceneHandle, CHAR* strTexName);
////������ͼ�ı�ŵõ���ͼ�ľ��
///*DLL_EXPORT*/ DWORD GetTextureHandleByIndex(DWORD sceneHandle, INT index);
//
////������ͼ�ľ���õ���ͼ������
///*DLL_EXPORT*/ VOID GetTextureNameByHandle(DWORD sceneHandle, DWORD texHandle, CHAR* strTexName);
////������ͼ�ľ���õ�����
///*DLL_EXPORT*/ INT GetTextureIndexByHandle(DWORD sceneHandle, DWORD mtrlHandle);

////������ͼ���õ�ͼ���ļ�
///*DLL_EXPORT*/ VOID	SetTextureSource(DWORD sceneHandle, DWORD texHandle, CHAR* strImageFile);
////�����ͼ���õ�ͼ��Դ�ļ�
///*DLL_EXPORT*/ VOID GetTextureSource(DWORD sceneHandle, DWORD texHandle, CHAR* strImageFile);

//�����ͼ�Ĳ���
///*DLL_EXPORT*/ VOID GetTextureInfo(DWORD sceneHandle, DWORD texHandle, TexInfo* pTexInfo);
////������ͼ����
///*DLL_EXPORT*/ VOID SetTextureInfo(DWORD sceneHandle, DWORD texHandle, TexInfo* pTexInfo);

//������ͼ�����ȺͶԱȶ�
//brightness (-100, 100);
//contrast (-100, 100);
///*DLL_EXPORT*/ VOID SetTextureBriAndCon(DWORD sceneHandle, DWORD texHandle, INT brightness, INT contrast);
//
////�ж���ͼ�Ƿ񱻲���������, ���ع������ʾ��
///*DLL_EXPORT*/ DWORD IsTextureUsingByMaterial(DWORD sceneHandle, DWORD texHandle);


/** ������պ�
*/
/*DLL_EXPORT*/ VOID CreateSkyBox(DWORD sceneHandle, const CHAR* sTextureName);

/** ������պ�ʹ�õ���ͼ
*/
/*DLL_EXPORT*/ VOID ChangeSetSkyBoxTexture(DWORD sceneHandle, const CHAR* sTextureName);

/** ������պеľ���
*/
/*DLL_EXPORT*/ VOID SetSkyBoxDistance(DWORD sceneHandle, FLOAT dist);

/** ������պ��Ƿ���Ч
*/
/*DLL_EXPORT*/ VOID SetSkyBoxEnabled(DWORD sceneHandle, BOOL bEnabled);

/** ʹ��ָ��ͼƬ��������ͼ,ÿ������ֻ����һ�ű���ͼ���ظ����ñ���ͼ�����֮ǰ�Ѿ������ı���ͼ��
*/
/*DLL_EXPORT*/ DWORD SetBackground(DWORD sceneHandle, const CHAR* sImageFile);

// ��ȡ����ͼ�����ÿ������ֻ����һ�ű���ͼ
/*DLL_EXPORT*/ DWORD GetBackgroundHandle(DWORD sceneHandle);

/** ɾ����������ͼ
*/
/*DLL_EXPORT*/ VOID RemoveBackground(DWORD sceneHandle);

/** ��ȡ����ͼʹ�õ�ͼ�����ƣ�������·��
@return �ɹ���ȡ����ͼ����TRUE,����FALSE
*/
/*DLL_EXPORT*/ BOOL GetBackgroundImage(DWORD sceneHandle, CHAR *pImageFile);  

/** ���ó����ı���ͼ�Ƿ�ɼ�
*/
/*DLL_EXPORT*/ VOID SetBackgroundVisibility(DWORD sceneHandle, BOOL bVisible);

/** ��ȡ�����ı���ͼ�Ƿ�ɼ�
*/
/*DLL_EXPORT*/ BOOL IsBackgroundVisible(DWORD sceneHandle);

/** ���ñ���ͼ��������ӿڵ���Դ�С�����λ�ã�left/top/width/heightȡֵ��Χ[0, 1]������ԭ�����ӿڵ����Ͻ�
*/
/*DLL_EXPORT*/ VOID SetBackgroundRect(DWORD sceneHandle, FLOAT left, FLOAT top, FLOAT width, FLOAT height);  

/** ��ȡ����ͼ��������ӿڵ���Դ�С�����λ�ã�left/top/width/heightȡֵ��Χ[0, 1]������ԭ�����ӿڵ����Ͻ�
*/
/*DLL_EXPORT*/ VOID GetBackgroundRect(DWORD sceneHandle, FLOAT &left, FLOAT &top, FLOAT &width, FLOAT &height);

/** ����ͼȫ����ʾ
*/
/*DLL_EXPORT*/ VOID FullScreenBackground(DWORD sceneHandle);  


//-------------------------------------------------------------------------------------------
///�������صķ���
//-------------------------------------------------------------------------------------------
/** ʹ��ȱʡ��������һ�����
*/
/*DLL_EXPORT*/ DWORD CreateCamera(DWORD sceneHandle, const CHAR* strCameraName);

/** ��������Ԥ�������
*/
/*DLL_EXPORT*/ DWORD CreateFrontCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD CreateBackCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD CreateTopCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD CreateBottomCamera(DWORD sceneHandle); 
/*DLL_EXPORT*/ DWORD CreateLeftCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD CreateRightCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD CreatePerspectiveCamera(DWORD sceneHandle);

/** ��ȡԤ���������ָ�룬�������Ѿ����ڣ����������ָ�룬���򷵻�NULL
*/
/*DLL_EXPORT*/ DWORD GetFrontCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD GetBackCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD GetTopCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD GetBottomCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD GetLeftCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD GetRightCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD GetPerspectiveCamera(DWORD sceneHandle);

//�õ������������������
/*DLL_EXPORT*/ INT GetCameraCount(DWORD sceneHandle);

//������������Ƶõ�����ľ��
/*DLL_EXPORT*/ DWORD GetCameraHandleByName(DWORD sceneHandle, CHAR* strCameraName);
//����������ı�ŵõ�����ľ��
/*DLL_EXPORT*/ DWORD GetCameraHandleByIndex(DWORD sceneHandle, INT index);

//��������ľ���õ����������
/*DLL_EXPORT*/ VOID GetCameraNameByHandle(DWORD sceneHandle, DWORD camHandle, CHAR* strCameraName);
//�������������
/*DLL_EXPORT*/ VOID SetCameraNamebyHandle(DWORD sceneHandle, DWORD camHandle, CHAR* strCameraName);
/*DLL_EXPORT*/ VOID SetCameraNamebyName(DWORD sceneHandle, CHAR* strOldCameraName, CHAR* strNewCameraName);

/** ��������������õ����������
*/
/*DLL_EXPORT*/ BOOL GetCameraNameByIndex(DWORD sceneHandle, INT index, CHAR* pName);

/**ɾ�����
*/
/*DLL_EXPORT*/ VOID DeleteCamerabyHandle(DWORD sceneHandle, DWORD camHandle);
/**ɾ�����
*/
/*DLL_EXPORT*/ VOID DeleteCamerabyName(DWORD sceneHandle, CHAR* camName);

//���õ�ǰ�����
/*DLL_EXPORT*/ VOID SetCurrentCamera(DWORD sceneHandle, DWORD swapChainHandle, DWORD camHandle);
//�õ���ǰ��������
/*DLL_EXPORT*/ DWORD GetCurrentCamera(DWORD sceneHandle, DWORD swapChainHandle);

//���õ�ǰ���ΪϵͳԤ�����
// /*DLL_EXPORT*/ VOID SetCurrentCameraToPreConfig(DWORD sceneHandle, DWORD swapChainHandle, CAMERATYPE cameraType);

//��õ�ǰ������Ƿ�ΪԤ�������������Ƿ���NONE
// /*DLL_EXPORT*/ CAMERATYPE GetCurrentPreConfigCamera(DWORD sceneHandle, DWORD swapChainHandle);


/** ƽ�������
@param in sceneHandle ����������
@param in camHandle ������
@param in pVec ����ƶ����������
@param in coordEnum �ο�����ϵ
	//0 - ����������ϵƽ�������
	//1 - ����������ϵƽ��
	//2 - walk�����ܷɵ�����ȥ
*/
/*DLL_EXPORT*/ VOID MoveCamera(DWORD sceneHandle, DWORD camHandle, VECTOR3* pVec, UINT coordEnum);

/** ��ת�����
@param in pVec : ���������ϵ���ת�Ƕ�
@param in coordEnum : �ο�����ϵ
//0 - ����������ϵ��ת�����
//1 - ����������ϵ��ת���
//2 - �ƴ���from���ģ�����������ϵƽ�е�����ϵ��ת
*/
/*DLL_EXPORT*/ VOID RotateCamera(DWORD sceneHandle, DWORD camHandle, VECTOR3* pVec, UINT coordEnum);

/** ��pTargetPos��Ϊ����,��ת���fAngle����
@param in sceneHandle   : ����������
@param in camHandle     : ������
@param in pTargetPos    : Χ����ת�����ĵ�(��������ϵ)
@param in axistype      : Χ����ת����(��������ϵ)�� 0��world X�� 1��world y��2��world z, 3: local X, 4: local Y, 5: local Z
@param in fAngle        : ��ת�ĽǶȣ��� ���� Ϊ��λ
*/  
/*DLL_EXPORT*/ VOID RoundCamera(DWORD sceneHandle, DWORD camHandle, VECTOR3 *pTargetPos, INT axistype, FLOAT fAngle);

/** ��������Ŀ�߱�
*/
// /*DLL_EXPORT*/ VOID SetCameraAspect(DWORD camHandle, const FLOAT fAspectRatio);

//�Զ���Ϊ���ģ�ָ�������ᣬ��תfAngle����
///*DLL_EXPORT*/ VOID RoundCamera1(DWORD sceneHandle, DWORD camHandle, VECTOR3* pAxisCenter, VECTOR3* pAxisVec, FLOAT fAngle);

/**�Զ���Ϊ����, ���������ϵ����ת���
@param pAxisCenter in : ���Χ����ת��Ŀ���
@param pXYZAangle  in : �����ת�ĽǶȣ����� 
*/
/*DLL_EXPORT*/ VOID RoundInCameraSpace(DWORD sceneHandle, DWORD camHandle, VECTOR3* pAxisCenter, VECTOR3* pXYZAngle);

/** ���Ŀ��㲻�������������ǰ�����������
@param in sceneHandle : ����ָ��
@param in camHandle : ���ָ��
@param dollyValue  in   : ��������ľ��룬> 0 ��ǰ�� < 1���
*/
/*DLL_EXPORT*/ VOID DollyCamera(DWORD sceneHandle, DWORD camHandle, FLOAT dollyValue);

/** ���������ģ��
@param in sceneHandle : ����ָ��
@param in camHandle : ���ָ��
@param in modelHandle : ģ��ָ��
*/
/*DLL_EXPORT*/ VOID SetCameraLookAtModel(DWORD sceneHandle, DWORD camHandle, DWORD modelHandle);

/*�������������������ϵλ��
@param in sceneHandle : ����ָ��
@param in camHandle   : ���ָ��
@param in pPos        : ��������ϵ�е���λ��
*/
/*DLL_EXPORT*/ VOID SetCameraPosition(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos);

/*��ȡ���������������ϵλ��
@param in sceneHandle : ����ָ��
@param in camHandle   : ���ָ��
@param in pPos        : ��������ϵ�е���λ��
*/
/*DLL_EXPORT*/ VOID GetCameraPosition(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos);

//�����������Ŀ���
/*DLL_EXPORT*/ VOID SetCameraAt(DWORD sceneHandle, DWORD camHandle, const VECTOR3* pPos);
/*DLL_EXPORT*/ VOID GetCameraAt(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos);

/** �����������FOV
@param in fov : �����fov����y������ӽ�
*/
/*DLL_EXPORT*/ VOID SetCameraFov(DWORD sceneHandle, DWORD camHandle, FLOAT fov);

/** �����������nearClip�����������С��nearClip�����彫���ᱻ����
*/
/*DLL_EXPORT*/ VOID SetCameraNearClip(DWORD sceneHandle, DWORD camHandle, FLOAT nearClip);

/** �����������farClip��������������farClip�����彫���ᱻ����
*/
/*DLL_EXPORT*/ VOID SetCameraFarClip(DWORD sceneHandle, DWORD camHandle, FLOAT farClip);

//�����������Roll
// /*DLL_EXPORT*/ VOID SetCameraRoll(DWORD sceneHandle, DWORD camHandle, FLOAT rollAngle);

/** һ���Ի�����������
*/
/*DLL_EXPORT*/ BOOL GetCameraInfo(DWORD sceneHandle, DWORD camHandle, CameraInfoEx* pCamInfo);

/** һ�����������������
*/
/*DLL_EXPORT*/ VOID SetCameraInfo(DWORD sceneHandle, DWORD camHandle, CameraInfoEx* pCamInfo);

/** ��ȡ����ķ���
@param vDirection out : �������������ϵ�еķ��򣬼�lookat�ķ��������ȡ����ʧ�ܣ�vDirection = (0, 0, 0)
*/
/*DLL_EXPORT*/ VOID GetCameraDirection(DWORD sceneHandle, DWORD camHandle, VECTOR3 &vDirection);

/** ��������ķ���
@param vDirection in : �������������ϵ�еķ��򣬼�lookat����
*/
/*DLL_EXPORT*/ VOID SetCameraDirection(DWORD sceneHandle, DWORD camHandle, const VECTOR3 &vDirection);

// ��ȡ�����up����
/*DLL_EXPORT*/ VOID GetCameraUp(DWORD sceneHandle, DWORD camHandle, VECTOR3 &vUp);

// ��ȡ�����right����
/*DLL_EXPORT*/ VOID GetCameraRight(DWORD sceneHandle, DWORD camHandle, VECTOR3 &vRight);

// ��ȡ�����near/far clip
/*DLL_EXPORT*/ VOID GetCameraClip(DWORD sceneHandle, DWORD camHandle, float &nearClip, float &farClip);
//-------------------------------------------------------------------------------------------
///�ƹ���صķ���
//-------------------------------------------------------------------------------------------
/** �õ������еƹ������
@return �����е����еƹ���Ŀ
*/
/*DLL_EXPORT*/ INT GetLightCount(DWORD sceneHandle);

/** ɾ���ƹ�
*/
/*DLL_EXPORT*/ VOID	DeleteLight(DWORD sceneHandle, DWORD lightHandle);

/** ���ݵƹ�����Ƶõ��ƹ�ľ��
*/
/*DLL_EXPORT*/ DWORD GetLightHandleByName(DWORD sceneHandle, CHAR* strLightName);

/** ���ݵƹ�ı�ŵõ��ƹ�ľ��
*/
/*DLL_EXPORT*/ DWORD GetLightHandleByIndex(DWORD sceneHandle, INT index);

/** ���ݵƹ�ľ���õ��ƹ������
*/
/*DLL_EXPORT*/ VOID GetLightNameByHandle(DWORD sceneHandle, DWORD lightHandle, CHAR* strLightName);

/*���õƹ������
@return ���óɹ�����TRUE������FALSE��ʧ�ܿ���������������ͻ���µġ�
*/
/*DLL_EXPORT*/ BOOL SetLightName(DWORD sceneHandle, DWORD lightHandle, CHAR* strLightName);

/** ���ݵƹ�ı�ŵõ��ƹ������
*/
/*DLL_EXPORT*/ BOOL GetLightNameByIndex(DWORD sceneHandle, INT index, CHAR* strLightName);

/** ����������ϵ���ƶ��ƹ�
@param in pVec : �ƶ���Ŀ����������꣬����ֵ
*/
/*DLL_EXPORT*/ VOID MoveLight(DWORD sceneHandle, DWORD lightHandle, VECTOR3* pVec);

//��ת�ƹ�
// /*DLL_EXPORT*/ VOID RotateLight(DWORD sceneHandle, DWORD lightHandle, VECTOR3* pRos);

/** ���õƹ��λ��
@param in vPos : �ƹ�Ҫ�ƶ�������λ�ã��ο�����ϵ����������ϵ
*/
/*DLL_EXPORT*/ VOID SetLightPosition(DWORD sceneHandle, DWORD lightHandle, VECTOR3* vPos);

/**-----------------------------------------------------------------------------
// ������: EnableLight()
// ��  ��: DWORD lightHandle, BOOL isEnable
// ��  ;: ���õƹ���Ƿ���Ч
//-----------------------------------------------------------------------------*/
/*DLL_EXPORT*/ VOID EnableLight(DWORD sceneHandle, DWORD lightHandle, BOOL isEnable);

//���õƹ�ķ���
// /*DLL_EXPORT*/ VOID SetLightDirection(DWORD sceneHandle, DWORD lightHandle, VECTOR3* pDir);

/** �õ��ƹ������
*/
/*DLL_EXPORT*/ VOID GetLightInfo(DWORD sceneHandle, DWORD lightHandle, LightInfo* pLightInfo);

/** ���õƹ������
*/
/*DLL_EXPORT*/ VOID SetLightInfo(DWORD sceneHandle, DWORD lihgtHandle, LightInfo* pLightInfo);

/** ���õƹ��Ƿ���Ч
*/
/*DLL_EXPORT*/ VOID SetLightEnableStatus(DWORD sceneHandle, DWORD lightHandle, BOOL bEnable);

/////////////////////// ��άԪ�ش������� /////////////////////////////////////////

/** ��ʼ��GUI״̬���´����ĳ������Ҫʹ��GUI���������ȵ��ø÷���
// ����Ѿ���ʼ�����������κβ�����ֱ�ӷ���
// �����ջ����ٳ����󣬿����ֳ�ʼ��GUI״̬
*/
/*DLL_EXPORT*/ VOID InitGUIStatus(DWORD sceneHandle, DWORD renderWnd, DWORD viewport);

/** �õ�����·���GUI������������,����ж�����󣬷������ϲ㣨ZOrder��󣩵�һ��GUI����
@param sceneHandle in : ����ָ��
@param x/y  : �ӿ��е�����
@param type out �� GUI��������ͣ��ο�VGS_GUIOBJ_TYPE����
@return GUI�����ָ��
*/
/*DLL_EXPORT*/ DWORD GetGUIObjectUnderLoc(DWORD sceneHandle, INT x, INT y, VGS_GUIOBJ_TYPE &type);


/** ��ȡGUI���������
@param GUIObjectHandle in : GUI����ľ��
@return GUI��������ͣ��ο�VGS_GUIOBJ_TYPE����
*/
/*DLL_EXPORT*/ VGS_GUIOBJ_TYPE GetGUIObjectType(DWORD sceneHandle, DWORD GUIObjectHandle);

/** ����GUI�����ZOrder��Խ���ʾGUI������Խ�ϲ�
*/
/*DLL_EXPORT*/ INT GetGUIObjectZOrder(DWORD sceneHandle, DWORD GUIObjectHandle);

/** ����GUI�����ZOrder��Խ���ʾGUI������Խ�ϲ�
*/
///*DLL_EXPORT*/ VOID SetObjectZOrder(DWORD GUIObjectHandle, int offset);

// GUI��������һ��
/*DLL_EXPORT*/ VOID MoveGUIObjectUp(DWORD sceneHandle, DWORD GUIObjectHandle);

/** GUI��������һ��
*/
/*DLL_EXPORT*/ VOID MoveGUIObjectDown(DWORD sceneHandle, DWORD GUIObjectHandle);

/** GUI�����ƶ������
*/
/*DLL_EXPORT*/ VOID GotoGUIObjectsTop(DWORD sceneHandle, DWORD GUIObjectHandle);

/** GUI�����ƶ�����ײ�
*/
/*DLL_EXPORT*/ VOID GotoGUIObjectsBottom(DWORD sceneHandle, DWORD GUIObjectHandle);

/** ��ȡ���ж�άԪ�صľ���б�
// vHandle����ҪԤ�ȷ����㹻�Ŀռ�
*/ 
/*DLL_EXPORT*/ VOID GetAllGUIObjectsList(DWORD* vHandle);

/** ��ȡ���ж�άԪ�ص���Ŀ
*/
/*DLL_EXPORT*/ UINT GetAllGUIObjectsCount(DWORD sceneHandle);

/**�õ�Overlay������
*/
/*DLL_EXPORT*/ INT GetGUIObjectCount(DWORD sceneHandle, VGS_GUIOBJ_TYPE guitype);

/** ��ȡָ������GUI����ľ���б�
*/
/*DLL_EXPORT*/ VOID GetGUIObjectList(VGS_GUIOBJ_TYPE guitype, DWORD* vHandle);

/** ͨ�������õ�ĳ��gui����ľ��
*/
/*DLL_EXPORT*/ DWORD GetGUIObjetHandleByIndex(DWORD sceneHandle, VGS_GUIOBJ_TYPE guitype, INT index);

/** ����GUI����λ�Ĳο�ԭ��
*/
/*DLL_EXPORT*/ VOID SetGUIObjOffsetOrigin(DWORD sceneHandle, DWORD overlayHandle, VGSOFFSETORIGIN offsetOrigin);

/** ���GUI����λ�Ĳο�ԭ��
*/
/*DLL_EXPORT*/ VGSOFFSETORIGIN GetGUIObjOffsetOrigin(DWORD sceneHandle, DWORD overlayHandle);

/**����GUI�����Ƿ�ʹ���������(%)����λ��
*/
/*DLL_EXPORT*/ VOID SetGUIObjUseRelativePos(DWORD sceneHandle, DWORD guiHandle, BOOL bRelativePos);

/**��ȡGUI�����Ƿ�ʹ���������(%)����λ��
*/
/*DLL_EXPORT*/ BOOL GetGUIObjUseRelativePos(DWORD sceneHandle, DWORD guiHandle);

/** ����GUI�����Ƿ�ʹ����Գߴ�(%)����ߴ�
*/
/*DLL_EXPORT*/ VOID SetGUIObjUseRelativeSize(DWORD sceneHandle, DWORD guiHandle, BOOL bRelativeSize);

/** ��ȡGUI�����Ƿ�ʹ����Գߴ�(%)����ߴ�
*/
/*DLL_EXPORT*/ BOOL GetGUIObjUseRelativeSize(DWORD sceneHandle, DWORD guiHandle);

/** ɾ�� GUI
@param sceneHandle   in : ����������
@param GUIHandle in : GUI���
*/
/*DLL_EXPORT*/ VOID	DeleteGUIObject(DWORD sceneHandle, DWORD GUIHandle);


/** �õ� GUI���� ��Ļ����ľ���Rect
@param rect out : rect�ı�ʾ��λ����GUI����������ӿ�offsetOrigin��λ��,��λΪ����
*/
/*DLL_EXPORT*/ VOID GetGUIObjectRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, BOOL &bRelativeSize, BOOL &bRelativePos);

/** �õ� GUI���� ��Ļ����ľ���Rect
@param rect out : rect�ı�ʾ��λ����GUI����������ӿ����Ͻǵ�λ��,Ҳ����GUI��������Ļ��ʵ��λ�á���λΪ����
*/
/*DLL_EXPORT*/ VOID GetGUIObjectAbsoluteRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, BOOL &bRelativeSize, BOOL &bRelativePos);

/** ���� GUI���� �������Ļ����ľ���Rect
@param rect in : rect�ı�ʾ��λ����GUI����������ӿ�offsetOrigin��λ��,��λΪ����
*/
/*DLL_EXPORT*/ VOID SetGUIObjectRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, BOOL bRelativeSize = TRUE, BOOL bRelativePos = TRUE);

/** ���� GUI���� �������Ļ����ľ���Rect
@param rect in : rect�ı�ʾ��λ����GUI����������ӿ����Ͻǵ�λ��,Ҳ����GUI�����ʵ��λ�á���λΪ����
*/
/*DLL_EXPORT*/ VOID SetGUIObjectAbsoluteRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, BOOL bRelativeSize = TRUE, BOOL bRelativePos = TRUE);

/** ����GUI�����λ��
@param sceneHandle   in : ����������
@param guiHandle in : gui���
@param x, y : ������ӿ�offsetOrigin������,��λΪ����
@param bRelative : true�������λ�ã�false���þ���λ�á�����������λ�ã�ʵ���ϼ�¼��overlay���Ͻ�λ�������Ŀǰ�ӿڴ�С�İٷֱȣ�
�ӿ�����ʱ��overlay�ᱣ�������������
*/
/*DLL_EXPORT*/ VOID SetGUIObjectPosition(DWORD sceneHandle, DWORD guiHandle, INT x, INT y, BOOL bRelative = true);

/** ����gui����ĳߴ�
@param sceneHandle   in : ����������
@param guiHandle in : overlay���
@param width, height    : Overlay�Ŀ��
@param bRelative : true������Դ�С��false���þ��Դ�С�����������Դ�С��ʵ���ϼ�¼��overlay�ߴ������Ŀǰ�ӿڳߴ�İٷֱȣ�
�ӿ�����ʱ��gui����ᱣ�������������
*/
/*DLL_EXPORT*/ VOID SetGUIObjectSize(DWORD sceneHandle, DWORD guiHandle, INT width, INT height, BOOL bRelative = true);
/**�õ� gui���� ��͸����
@return gui����Ĳ�͸���� [0, 100]
*/
/*DLL_EXPORT*/ INT GetGUIObjectOpacity(DWORD sceneHandle, DWORD guiHandle); 

/**���� gui���� ��͸����
@opacity in : gui����Ĳ�͸���� [0, 100]
*/
/*DLL_EXPORT*/ VOID SetGUIObjectOpacity(DWORD sceneHandle, DWORD guiHandle, INT opacity); 

/** ����gui�����Ƿ�ɼ�
*/
/*DLL_EXPORT*/ VOID SetGUIObjectVisibility(DWORD sceneHandle, DWORD guiHandle, BOOL visible);

/** ��ȡgui�����Ƿ�ɼ�
*/
/*DLL_EXPORT*/ BOOL GetGUIObjectVisibility(DWORD sceneHandle, DWORD guiHandle);

/** ͨ�����Ƶõ�gui����
*/
/*DLL_EXPORT*/ DWORD GetGUIObjectHandleByName(DWORD sceneHandle, CHAR* name);

/** �õ�gui���������
*/
/*DLL_EXPORT*/ VOID	GetGUIObjectNameByHandle(DWORD sceneHandle, DWORD guiHandle, CHAR* name);

/** ����gui���������
*/
/*DLL_EXPORT*/ BOOL SetGUIObjectName(DWORD sceneHandle, DWORD guiHandle, CHAR* NewName);

/** ����gui����ѡ����Ƿ�ɼ�
*/
/*DLL_EXPORT*/ VOID ShowGUIObjectSelcetionBox(DWORD sceneHandle, DWORD guiHandle, BOOL bVisible);

/** ��ȡ��ά����ѡ����Ƿ���ʾ
*/
/*DLL_EXPORT*/ BOOL GetGUISelectionBoxVisible(DWORD sceneHandle, DWORD GUIObjectHandle);

/** ����GUI���������¼���������
@param scenHandle   in : ����ָ��
@param guiHandle  in : GUI����ָ��
@param type         in  : �¼�����
@param sEventName   in : ��������
*/
/*DLL_EXPORT*/ VOID SetGuiMouseEvent(DWORD sceneHandle, DWORD guiHandle, VGS_MOUSEEVENT_TYPE type, const CHAR* sEventName);

/** ��ȡGUI���������¼���������
@param scenHandle   in  : ����ָ��
@param guiHandle  in  : GUI����ָ��
@param type         in  : �¼�����
@param sEventName   out : ��������
@return ���GUI�����Ѿ��ƶ��˶�Ӧ���¼�����������TRUE������FALSE
*/
/*DLL_EXPORT*/ BOOL GetGuiMouseEvent(DWORD sceneHandle, DWORD guiHandle, VGS_MOUSEEVENT_TYPE type, CHAR* sEventName);

//------------------Flash GUI Start-------------------------------------------------------------------------
/** ��Flash�ļ�����һ��GUI.
@param scenHandle   in  : ����ָ��
@param hParent		in	:	��������.
@param sName        in : GUI����
@param sFile       in : Flash�ļ����ƣ�����·��
@param rect        in : GUI�ĸ��ǵ�����
@return : ����Flash GUI��ָ��
*/
/*DLL_EXPORT*/ DWORD CreateFlashGUIFromFile(DWORD sceneHandle, HWND hParent, const char* sName, const char* sFile, const RECT &rect);
/** ����ƶ�Flash GUI.
@param scenHandle   in  : ����ָ��
@param guiHandle  in  : GUI����ָ��
@param x in: 
@param y in: 
*/
/*DLL_EXPORT*/ VOID MoveFlashGUI(DWORD sceneHandle, DWORD guiHandle,INT x, INT y);
/** ����Flash����״̬.
@param scenHandle   in  : ����ָ��
@param guiHandle  in  : GUI����ָ��
@param status in: 0Ϊ��ͣ���ţ�1Ϊ����ͣ�㿪ʼ���ţ�2Ϊֹͣ����ת����һ֡
*/
/*DLL_EXPORT*/ VOID SetFlashGUIPlayStatus(DWORD sceneHandle, DWORD guiHandle,INT status);
/** ��Flash GUI�������һ���ź���.
@param scenHandle   in  : ����ָ��
@param guiHandle  in  : GUI����ָ��
@param SemaphoreName in : FLash���ź���������.
@param SemaphoreFunc in : �ź�Ҫ������JS�ص��ú���.
*/
/*DLL_EXPORT*/ VOID AddFlashGUISemaphore(DWORD sceneHandle, DWORD guiHandle,const char*  SemaphoreName, const char*  SemaphoreFunc);

//DeleteFlashCtrl������DeleteGUIObjectʵ�֡�SetFlashRect������SetGUIObjectRect��SetGUIObjectAbsoluteRectʵ�֡�SetFlashCtrlVisibility������SetGUIObjectVisibilityʵ��
//------------------Flash GUI End-------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
///ǰ��ͼ��صķ���
//-------------------------------------------------------------------------------------------
/**
��ͼƬ�ļ�����Overlay
@param sceneHandle in : ����������
@param name        in : Overlay����
@param sFile       in : ͼƬ�ļ����ƣ�����·��
@param sOpcatityImageFile in : ͸��ͼƬ�ļ����ƣ�����·��, ���û��͸����ͼ��sOpcatityImageFile���ÿռ���
@param rect        in : Overlay�ĸ��ǵ�����
@return ����overlay��ָ��
*/
/*DLL_EXPORT*/ DWORD CreateOverlayFromFile(DWORD sceneHandle, const CHAR* name, const CHAR* sFile, const CHAR* sOpcatityImageFile, RECT &rect);

/** ��ȡoverlay������
@param sceneHandle   in  : ����������
@param overlayHandle in  : overlay���
@param info          out : Overlay������
*/
// /*DLL_EXPORT*/ BOOL GetOverlayInfo(DWORD sceneHandle, DWORD overlayHandle, OverlayInfo &info);

/**�õ�Overlay������
*/
/*DLL_EXPORT*/ INT GetOverlayCount(DWORD sceneHandle);

// ��ȡ����Overlay�ľ���б�
/*DLL_EXPORT*/ VOID GetOverlayList(DWORD sceneHandle, DWORD* vHandle);

/**���Overlayʹ�õ�ͼ���ļ�
@param sBaseFile in : ������ͼ�ļ�
@param sOpacFile in : ͸����ͼ�ļ�
*/
/*DLL_EXPORT*/ VOID GetOverlayImageFile(DWORD sceneHandle, DWORD overlayHandle, CHAR* sBaseFile, CHAR* sOpacFile);

/**����Overlayʹ�õ�ͼ���ļ�
@param sBaseFile in : ������ͼ�ļ�
@param sOpacFile in : ͸����ͼ�ļ�
*/
/*DLL_EXPORT*/ VOID SetOverlayImageFile(DWORD sceneHandle, DWORD overlayHandle, const CHAR* sBaseFile, const CHAR* sOpacFile = NULL);

//-------------------------------------------------------------------------------------------
///Button��صķ���
//-------------------------------------------------------------------------------------------
/** ����Button
@param  sName             in ����ť������
@param rect               in ����ť����������ĸ����꣬���ư�ť��λ�úʹ�С
@param mouseUpImageFile   in ������ɿ���Դͼ���ļ�
@param mouseOverImageFile in ��������ϵ�Դͼ���ļ�
@param mouseDownImageFile in ����갴�µ�Դͼ���ļ�
@return ����ɹ����������ذ�ťָ�룬����0
*/
/*DLL_EXPORT*/ DWORD CreateButtonFromFile(DWORD sceneHandle, const CHAR* sName, const RECT &rect, 
									  const CHAR* mouseUpImageFile,
									  const CHAR* mouseOverImageFile = NULL,
									  const CHAR* mouseDownImageFile = NULL);

/** ��ð�ť�Ļص���������, ���Ȳ�����MAX_NAMESTR_LENGTH
*/
/*DLL_EXPORT*/ VOID GetButtonCallBack(DWORD sceneHandle, DWORD buttonHandle, CHAR* callBackFunc);

/** ���ð�ť�Ļص���������[��ť���£��ص�����], ���Ȳ�����MAX_NAMESTR_LENGTH
*/
/*DLL_EXPORT*/ VOID SetButtonCallBack(DWORD sceneHandle, DWORD buttonHandle, const CHAR* callBackFunc);

/** ��ð�ťʹ�õ�MouseUpͼ���ļ�,Ҳ��������״̬����ʾ��ͼ���ļ�
*/
/*DLL_EXPORT*/ VOID GetButtonMouseUpImageFile(DWORD sceneHandle, DWORD buttonHandle, CHAR* strMouseUpImageFile);

/** ���ð�ťʹ�õ�MouseUpͼ���ļ�
*/
/*DLL_EXPORT*/ VOID SetButtonMouseUpImageFile(DWORD sceneHandle, DWORD buttonHandle, const CHAR* strMouseUpImageFile);

/** ��ð�ťʹ�õ�Mouse over(�����ͣ�ڰ�ť����ʱ)ͼ���ļ�
*/
/*DLL_EXPORT*/ VOID GetButtonMouseOverImageFile(DWORD sceneHandle, DWORD buttonHandle, CHAR* strMouseOverImageFile);

/** ���ð�ťʹ�õ�Mouse over(�����ͣ�ڰ�ť����ʱ)ͼ���ļ�
*/
/*DLL_EXPORT*/ VOID SetButtonMouseOverImageFile(DWORD sceneHandle, DWORD buttonHandle, const CHAR* strMouseOverImageFile);

/** ��ð�ťʹ�õ�MouseDown(��ť������)ͼ���ļ�
*/
/*DLL_EXPORT*/ VOID GetButtonMouseDownImageFile(DWORD sceneHandle, DWORD buttonHandle, CHAR* strMouseDownImageFile);

/** ���ð�ťʹ�õ�MouseDown(��ť������)ͼ���ļ�
*/
/*DLL_EXPORT*/ VOID SetButtonMouseDownImageFile(DWORD sceneHandle, DWORD buttonHandle, const CHAR* strMouseDownImageFile);

/** ����Button�Ƿ񼤻�
*/
/*DLL_EXPORT*/ VOID SetButtonEnable(DWORD sceneHandle, DWORD buttonHandle, BOOL enable);

/** �ж�Button�Ƿ񼤻�
*/
/*DLL_EXPORT*/ BOOL IsButtonEnable(DWORD sceneHandle, DWORD buttonHandle);

//-------------------------------------------------------------------------------------------
/** ����ͼ��صķ��� */
//-------------------------------------------------------------------------------------------
/**��������ͼ, 
@param name        in : ����ͼʹ�õ�����
@param strMapFile  in : ����ͼʹ�õĵ�ͼ�ļ����ƣ���������·��
@param pRect       in : ������Χ����������ͼ�п����ĳ�������left < right(x����), top < bottom(z����)
@return ���ص���ͼ����ľ����0
*/
/*DLL_EXPORT*/ DWORD CreateNavigater(DWORD sceneHandle, const CHAR* name, const CHAR* strMapFile, const FRect &pRect);

/** ��õ���ͼ��ͼ
@param strMapFile out : ����ͼ����ͼʹ�õ�ͼƬ�ļ���������·��
*/
/*DLL_EXPORT*/ VOID GetNavigaterMap(DWORD sceneHandle, DWORD navHandle, CHAR* strMapFile);

/** ���õ���ͼ��ͼ
@param strMapFile in : ����ͼ����ͼʹ�õ�ͼƬ�ļ�����Ҫ��������·��
*/
/*DLL_EXPORT*/ VOID SetNavigaterMap(DWORD sceneHandle, DWORD navHandle, const CHAR* strMapFile);

/** ��õ���ͼ�ĵ�����Χ����
@param pRect out : ��������ͼ�п����ĳ�������left < right(x����), top < bottom(z����)
*/
/*DLL_EXPORT*/ VOID GetNavigaterSceneRect(DWORD sceneHandle, DWORD navHandle, FRect &pRect);

/** ���õ���ͼ�ĵ�����Χ����
@param pRect in : ��������ͼ�п����ĳ�������left < right(x����), top < bottom(z����)
*/
/*DLL_EXPORT*/ VOID SetNavigaterSceneRect(DWORD sceneHandle, DWORD navHandle, const FRect &pRect);


/**���ӵ��������
@param  navigaterHandle - ����ͼ����ľ��
@param  navPointerName - ����ͼ�������
@param  pointerType - ����ͼ������ͣ�0Ϊ���������1Ϊģ�͵���
@param  pointerHandle - �������������Ķ���ľ�����������ͼ������Ϊ�������Ϊ������������Ϊģ�;��
@return �ɹ����������㷵��TRUE������FALSE
*/
/*DLL_EXPORT*/ BOOL AddNavigaterPointer(DWORD sceneHandle, DWORD navigaterHandle,
									const CHAR* navPointerName,
									const CHAR* strPointerFile, 
									INT	  pointerType   = 0,	// 0 - camera or 1 - model 
									DWORD pointerHandle = 0     //camera or model handle
									);

/** ʹ�����еĵ���ͼ�㴴��һ���µ�����
@param  navPointerName   in : Ҫ�����ĵ���������
@param  sSrcPointerName   in : �Ѵ��ڵĵ��������ƣ��´����ĵ��ʹ����ͼƬ�����Ҿ�����ͬ�ĵ�������
@return �ɹ����������㷵��TRUE������FALSE
*/
/*DLL_EXPORT*/ BOOL CloneNavigaterPointer(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* navPointerName, 
									  const CHAR* sSrcPointerName);

/** ɾ��������
@param navigaterHandle in : ����ͼ
@param sName           in : ����������
*/
/*DLL_EXPORT*/ VOID DeleteNavigaterPointer(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName);

/** �õ�Navigater�ĵ���������
@param navigaterHandle in : ����ͼ����ָ��
@return ����ͼ�еĵ�������Ŀ
*/
/*DLL_EXPORT*/ INT GetNavPointerCount(DWORD sceneHandle, DWORD navigaterHandle);

/** ������������ȡ����ͼ�ĵ���������
*/
/*DLL_EXPORT*/ VOID GetNavPointerNameByIndex(DWORD sceneHandle, DWORD navigaterHandle, INT index, CHAR* sName);

/** ��õ���ͼ����ʹ�õ�ͼƬ���ƣ�����·��
@param navigaterHandle in : ����ͼ����ָ��
@param sName           in : ����������
@param strPointerFile  out : ������ʹ�õ�ͼƬ���ƣ�������·��
*/
/*DLL_EXPORT*/ VOID GetNavPointerImage(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, CHAR* strPointerFile);

/** ���õ���ͼ����ʹ�õ�ͼƬ����
@param navigaterHandle in : ����ͼ����ָ��
@param sName           in : ����������
@param strPointerFile  in : ������ʹ�õ�ͼƬ���ƣ���Ҫ��������·��
*/
/*DLL_EXPORT*/ VOID SetNavPointerImage(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, const CHAR* strPointerFile);

/** ��õ�����ĵ������ͺ͵�������
*/
/*DLL_EXPORT*/ BOOL GetNavPointerObj(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT &pointerType, DWORD &pointerHandle);

/** ���õ�����ĵ������ͺ͵�������
*/
/*DLL_EXPORT*/ VOID SetNavPointerObj(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT pointerType, DWORD pointerHandle);

/** ���õ������Ƿ�ɼ�
*/
/*DLL_EXPORT*/ VOID SetNavPointerVisiblilty(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, BOOL bVisible);

/** ��ȡ�������Ƿ�ɼ�
*/
/*DLL_EXPORT*/ BOOL GetNavPointerVisiblilty(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName);

/** ���õ�����Ĳ�͸���� [0, 100]
*/
/*DLL_EXPORT*/ VOID SetNavPointerOpacity(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT opacity);

/** ��ȡ������Ĳ�͸���� [0, 100]
*/
/*DLL_EXPORT*/ INT  GetNavPointerOpacity(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName);

/** ���õ�����Ŀ�Ⱥ͸߶ȣ�������Ŀ
*/
/*DLL_EXPORT*/ VOID SetNavPointerSize(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT width, INT height);

/** ��ȡ������Ŀ�Ⱥ͸߶ȣ�������Ŀ
*/
/*DLL_EXPORT*/ BOOL GetNavPointerSize(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT &width, INT &height);

/** ���õ������λ�ã�������Ŀ
*/
/*DLL_EXPORT*/ VOID SetPointerPosition(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT left, INT top);

/** ��ȡ�������λ�ã�������Ŀ
*/
/*DLL_EXPORT*/ BOOL GetPointerPosition(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT &left, INT &top);

/** ���������
*/
/*DLL_EXPORT*/ VOID RenamePointer(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sOldName, const CHAR* sNewName);

//-------------------------------------------------------------------------------------------
///������صķ���
//-------------------------------------------------------------------------------------------
//�½��������
/*DLL_EXPORT*/ DWORD NewVGSFont(DWORD sceneHandle, CHAR* name, CHAR* fontName);
//ɾ���������
/*DLL_EXPORT*/ VOID DeleteVGSFont(DWORD sceneHandle, DWORD fontHandle);

//�õ�������������
/*DLL_EXPORT*/ INT GetVGSFontCout(DWORD sceneHandle);
 
//ͨ���������ı�ŵõ��������ľ��
/*DLL_EXPORT*/ DWORD GetVGSFontHandleByIndex(DWORD sceneHandle, INT index);
//ͨ����������Ƶõ��������
/*DLL_EXPORT*/ DWORD GetVGSFontHandleByName(DWORD sceneHandle, CHAR* name);

//ͨ���������ľ���������
/*DLL_EXPORT*/ VOID GetVGSFontNameByHandle(DWORD sceneHandle, DWORD fontHandle, CHAR* name);
//ͨ���������ľ����ñ��
/*DLL_EXPORT*/ INT	GetVGSFontIndexByHandle(DWORD sceneHandle, DWORD fontHandle);

//�����������ɫ
/*DLL_EXPORT*/ VOID SetVGSFontColor(DWORD sceneHandle, DWORD fontHandle, DWORD color);
//����������ɫ
/*DLL_EXPORT*/ VOID GetVGSFontColor(DWORD sceneHandle, DWORD fontHandle, VGSRGB* pColor);

//��������Ĵ�С
/*DLL_EXPORT*/ VOID SetVGSFontSize(DWORD sceneHandle, DWORD fontHandle, INT size);
//�������Ĵ�С
/*DLL_EXPORT*/ INT GetVGSFontSize(DWORD sceneHandle, DWORD fontHandle);

//��������Ĵ���״̬
/*DLL_EXPORT*/ VOID SetVGSFontBold(DWORD sceneHandle, DWORD fontHandle, BOOL bBold);
//�������Ĵ���״̬
/*DLL_EXPORT*/ BOOL GetVGSFontBold(DWORD sceneHandle, DWORD fontHandle);

//��������б��״̬
/*DLL_EXPORT*/ VOID SetVGSFontItalic(DWORD sceneHandle, DWORD fontHandle, BOOL bItalic);
//��������б��״̬
/*DLL_EXPORT*/ BOOL GetVGSFontItalic(DWORD sceneHandle, DWORD fontHandle);

//���������»���״̬
/*DLL_EXPORT*/ VOID SetVGSFontUnderline(DWORD sceneHandle, DWORD fontHandle, BOOL bUnderline);
//���������»���״̬
/*DLL_EXPORT*/ BOOL GetVGSFontUnderline(DWORD sceneHandle, DWORD fontHandle);


//-------------------------------------------------------------------------------------------
///������صķ���
//-------------------------------------------------------------------------------------------
//�½����ֶ���
// /*DLL_EXPORT*/ DWORD NewText(DWORD sceneHandle, CHAR* textName, DWORD fontHandle, CHAR* strText);
/*DLL_EXPORT*/ DWORD NewText(DWORD sceneHandle, const CHAR* textName, const CHAR* fontName, const CHAR* strText);

//ɾ�����ֶ���
/*DLL_EXPORT*/ VOID DeleteText(DWORD sceneHandle, DWORD textHandle);

//�õ����������ֶ��������
/*DLL_EXPORT*/ INT	 GetTextCount(DWORD sceneHandle);

//ͨ�����ֶ���ı�ŵõ����ֶ���ľ��
/*DLL_EXPORT*/ DWORD GetTextHandleByIndex(DWORD sceneHandle, INT index);
//ͨ�����ֵ����Ƶõ����ֶ���
/*DLL_EXPORT*/ DWORD GetTextHandleByName(DWORD sceneHandle, CHAR* textName);

//ͨ�����ֶ���ľ���������
/*DLL_EXPORT*/ VOID GetTextNameByHandle(DWORD sceneHandle, DWORD textHandle, CHAR* name);
//ͨ�����ֶ���ľ����ñ��
/*DLL_EXPORT*/ INT	GetTextIndexByHandle(DWORD sceneHandle, DWORD textHandle);

//�������ֶ����λ��
/*DLL_EXPORT*/ VOID SetTextPosition(DWORD sceneHandle, DWORD textHandle, UINT x, UINT y);
//������ֶ����λ��
/*DLL_EXPORT*/ VOID GetTextPosition(DWORD sceneHandle, DWORD textHandle, VECTOR2* pVec2);

//�������ֶ������ɫ
/*DLL_EXPORT*/ VOID SetTextRGB(DWORD sceneHandle, DWORD textHandle, DWORD color);
//������ֶ������ɫ
/*DLL_EXPORT*/ DWORD GetTextRGB(DWORD sceneHandle, DWORD textHandle);

//�������ֵ�����
/*DLL_EXPORT*/ VOID SetTextString(DWORD sceneHandle, DWORD textHandle, CHAR* strText);
//������ֶ�����ַ���
/*DLL_EXPORT*/ VOID GetTextString(DWORD sceneHandle, DWORD textHandle, CHAR* strText);

//-------------------------------------------------------------------------------------------
///Image��صķ���, �½���Image������Ϊ��Դ������E3D�ļ��У����Զ�̬����
//-------------------------------------------------------------------------------------------
//�½�Image��Դ
/*DLL_EXPORT*/ DWORD NewImage(DWORD sceneHandle, CHAR* name, CHAR* fileName);
//ɾ��Image����
/*DLL_EXPORT*/ VOID DeleteImage(DWORD sceneHandle, DWORD imageHandle);

//��Image�������±��
/*DLL_EXPORT*/ VOID ResetImageIndex(DWORD sceneHandle);
//�õ�Image������
/*DLL_EXPORT*/ INT GetImageCount(DWORD sceneHandle);

//ͨ����ŵõ�Image����
/*DLL_EXPORT*/ DWORD GetImageHandleByIndex(DWORD sceneHandle, INT index);
//ͨ��ͼ�����Ƶõ�Image����
/*DLL_EXPORT*/ DWORD GetImageHandleByName(DWORD sceneHandle, CHAR* name);

//ͨ��Image��Handle�õ�����
/*DLL_EXPORT*/ VOID GetImageNameByHandle(DWORD sceneHandle, DWORD imageHandle, CHAR* name);
//����Image���������
/*DLL_EXPORT*/ VOID SetImageName(DWORD sceneHandle, DWORD imageHandle, CHAR* strImageName);
//ͨ��Image�����Handle�õ�Index
/*DLL_EXPORT*/ INT GetImageIndexByHandle(DWORD sceneHandle, DWORD imageHandle);

//����Image���õ�ͼ���ļ�
/*DLL_EXPORT*/ VOID	SetImageSource(DWORD sceneHandle, DWORD imageHandle, CHAR* strImageFile);
//���Image���õ�ͼ��Դ�ļ�
/*DLL_EXPORT*/ VOID GetImageSource(DWORD sceneHandle, DWORD imageHandle, CHAR* strImageFile);

//�õ�ͼ���ļ�����Ϣ
/*DLL_EXPORT*/ VOID GetImageInfoByFile(DWORD sceneHandle, CHAR* imageFileName, ImageInfo* pImageInfo);
//�õ�ͼ��������Ϣ
/*DLL_EXPORT*/ VOID GetImageInfoByHandle(DWORD sceneHandle, DWORD imageHandle, ImageInfo* pImageInfo);


//-------------------------------------------------------------------------------------------
///ģ�ͳ�����صķ���
//-------------------------------------------------------------------------------------------
/*DLL_EXPORT*/ DWORD CreateModelGroup(DWORD sceneHandle, CHAR* groupName);

/*DLL_EXPORT*/ VOID	DeleteModelGroup(DWORD sceneHandle, DWORD ModelGroupHandle);
/*DLL_EXPORT*/ VOID	DeleteModelGroupByName(DWORD sceneHandle, CHAR* groupName);
/*DLL_EXPORT*/ VOID	DeleteModelGroupByIndex(DWORD sceneHandle, INT index);
/*DLL_EXPORT*/ VOID	DeleteAllModelGroup(DWORD sceneHandle);

/*DLL_EXPORT*/ INT	GetModelGroupCount(DWORD sceneHandle);
/*DLL_EXPORT*/ VOID	ResetModelGroupIndex(DWORD sceneHandle);

/*DLL_EXPORT*/ DWORD GetModelGroupHandleByName(DWORD sceneHandle, CHAR* groupName);
/*DLL_EXPORT*/ DWORD GetModelGroupHandleByIndex(DWORD sceneHandle, INT index);

/*DLL_EXPORT*/ VOID	GetModelGroupNameByHandle(DWORD sceneHandle, DWORD ModelGroupHandle, CHAR* groupName);
/*DLL_EXPORT*/ INT	GetModelGroupIndexByHandle(DWORD sceneHandle, DWORD ModelGroupHandle);
	
/*DLL_EXPORT*/ VOID	AddModelToModelGroup(DWORD sceneHandle, DWORD modelGroupHandle, DWORD modelHandle);
/*DLL_EXPORT*/ VOID	DeleteModelFromModelGroup(DWORD sceneHandle, DWORD modelGroupHandle, DWORD modelHandle);
/*DLL_EXPORT*/ VOID	ClearModelGroup(DWORD sceneHandle, DWORD modelGroupHandle);

//���ModelGroup�е�ģ������
/*DLL_EXPORT*/ INT GetModelCountInModelGroup(DWORD sceneHandle, DWORD modelGroupHandle);
//���ModelGroup�е�ģ��
/*DLL_EXPORT*/ DWORD GetModelHandleInModelGroupByIndex(DWORD sceneHandle, DWORD modelGroupHandle, INT index);

//���ModelGroup�е�ģ��������
/*DLL_EXPORT*/ INT GetModelGroupCountInModelGroup(DWORD sceneHandle, DWORD modelGroupHandle);
//���ModelGroup�е�ģ����
/*DLL_EXPORT*/ DWORD GetModelGroupHandleInModelGroupByIndex(DWORD sceneHandle, DWORD modelGroupHandle, INT index);

//����ModelGroup��ModelGroup
/*DLL_EXPORT*/ VOID AddModelGroupToModelGroup(DWORD sceneHandle, DWORD parentModelGroupHandle, DWORD modelGroupHandle);
//ɾ��ModelGroup��ModelGroup
/*DLL_EXPORT*/ VOID DeleteModelGroupFromModelGroup(DWORD sceneHandle, DWORD parentModelGroupHandle, DWORD modelGroupHandle);

//�õ�ModelGroup��Parent��Handle, ����ModelGroup�ĸ���ModelGroup, ��߼�ModelGroup����0
/*DLL_EXPORT*/ DWORD GetModelGroupParent(DWORD sceneHandle, DWORD modelGroupHandle);

//�ı�ModelGroup������
/*DLL_EXPORT*/ VOID ChangeModelGroupName(DWORD sceneHandle, DWORD modelGroupHandle, CHAR* groupName);

//-------------------------------------------------------------------------------------------
///������Ч�������ط���
//-------------------------------------------------------------------------------------------
/*DLL_EXPORT*/ VOID CreateBloomEffect(DWORD viewportHandle);
/*DLL_EXPORT*/ VOID EnableBloomEffect(DWORD viewportHandle, BOOL enabled);
/*DLL_EXPORT*/ VOID DestroyBloomEffect(DWORD viewportHandle);

/*DLL_EXPORT*/ VOID CreateHDREffect(DWORD viewportHandle);
/*DLL_EXPORT*/ VOID EnableHDREffect(DWORD viewportHandle, BOOL enabled);
/*DLL_EXPORT*/ VOID DestroyHDREffect(DWORD viewportHandle);

//������̬ģ����Ч����
/*DLL_EXPORT*/ VOID CreateFSMotionBlur(DWORD sceneHandle);
//�򿪶�̬ģ����Ч
/*DLL_EXPORT*/ VOID	OpenFSMotionBlur(DWORD sceneHandle);
//�رն�̬ģ����Ч
/*DLL_EXPORT*/ VOID    CloseFSMotionBlur(DWORD sceneHandle);
//���ö�̬ģ����ǿ��, 0-100
/*DLL_EXPORT*/ VOID	SetFSMotionBlurPower(DWORD sceneHandle, INT value);

//---------------------------------------------------------------------------------
//����ȫ�����ζ���
/*DLL_EXPORT*/ VOID CreateFSGlow(DWORD sceneHandle);
//��ȫ��������Ч
/*DLL_EXPORT*/ VOID OpenFSGlow(DWORD sceneHandle);
//�ر�ȫ��������Ч
/*DLL_EXPORT*/ VOID CloseFSGlow(DWORD sceneHandle);
//����ȫ������ǿ�� 0-100
/*DLL_EXPORT*/ VOID SetFSGlowPower(DWORD sceneHandle, INT value);

//----------------------------------------------------------------------------------
//����LensFlare��ͷ��߶���
/*DLL_EXPORT*/ VOID	CreateLensFlare(DWORD sceneHandle, VECTOR3* sunPos);
//ɾ����ͷ��߶���
/*DLL_EXPORT*/ VOID		DeleteLensFlare(DWORD sceneHandle);

/** ������Ч
@param   fogMode         in : �������
@param   red/green/blue  in : �����ɫ
@param   density         in : ����ܶ�
@param   start/end       in : ��Ŀ�ʼ�������루������ľ��룩
*/
 /*DLL_EXPORT*/ VOID CreateFog(DWORD sceneHandle, INT fogMode, INT red, INT green, INT blue, FLOAT density, FLOAT start, FLOAT end);

//-------------------------------------------------------------------------------------------
///����������صķ���
//-------------------------------------------------------------------------------------------
/*DLL_EXPORT*/ DWORD CreateAudio(DWORD sceneHandle, CHAR* name, CHAR* AudioFilePath, INT repeatCnt);

/*DLL_EXPORT*/ VOID DeleteAudio(DWORD sceneHandle, DWORD audioHandle);
/*DLL_EXPORT*/ VOID DeleteAudioByName(DWORD sceneHandle, CHAR* name);
/*DLL_EXPORT*/ VOID DeleteAudioByIndex(DWORD sceneHandle, INT index);
/*DLL_EXPORT*/ VOID DeleteAllAudio(DWORD sceneHandle);

/*DLL_EXPORT*/ INT GetAudioCount(DWORD sceneHandle);
/*DLL_EXPORT*/ VOID SetAudioName(DWORD sceneHandle, DWORD audioHandle, CHAR* name);

/*DLL_EXPORT*/ DWORD GetAudioHandleByName(DWORD sceneHandle, CHAR* name);
/*DLL_EXPORT*/ DWORD GetAudioHandleByIndex(DWORD sceneHandle, INT index);

/*DLL_EXPORT*/ VOID GetAudioNameByHandle(DWORD sceneHandle, DWORD audioHandle, CHAR* name);
/*DLL_EXPORT*/ INT GetAudioIndexByHandle(DWORD sceneHandle, DWORD audioHandle);

/*DLL_EXPORT*/ VOID SetAudioFilePath(DWORD sceneHandle, DWORD audioHandle, CHAR* AudioFilePath);
/*DLL_EXPORT*/ VOID GetAudioFilePath(DWORD sceneHandle, DWORD audioHandle, CHAR* AudioFilePath);

/*DLL_EXPORT*/ VOID SetAudioRepeatCount(DWORD sceneHandle, DWORD audioHandle, INT repeatCount);
/*DLL_EXPORT*/ INT	GetAudioRepeatCount(DWORD sceneHandle, DWORD audioHandle);

/*DLL_EXPORT*/ VOID PlayAudio(DWORD sceneHandle, DWORD audioHandle);
/*DLL_EXPORT*/ VOID PauseAudio(DWORD sceneHandle, DWORD audioHandle);
/*DLL_EXPORT*/ VOID StopAudio(DWORD sceneHandle, DWORD audioHandle);
/*DLL_EXPORT*/ VOID RewindAudio(DWORD sceneHandle, DWORD audioHandle);

//-------------------------------------------------------------------------------------------
///�û�������صķ���
//-------------------------------------------------------------------------------------------

///// ���ƶ��󷽷� ////////////////////////////////
// ���õ�ǰ������
/*DLL_EXPORT*/ VOID SetCurrentController(DWORD sceneHandle, DWORD ControllerHandle);

// ��ȡ��ǰ������
/*DLL_EXPORT*/ DWORD GetCurrentController(DWORD sceneHandle);

// ��ȡ���ƶ��������
/*DLL_EXPORT*/ CONTROLTYPE GetControllerType(DWORD ControllerHandle);

// ��ȡ��ǰ������ʹ�õ����
/*DLL_EXPORT*/ DWORD GetCurrentControllerCamera(DWORD sceneHandle);

// ���ÿ����������
/*DLL_EXPORT*/ VOID SetControllerCamera(DWORD sceneHandle, DWORD ControllerHandle, DWORD CameraHandle);

// ��ȡ��ǰ�����������
/*DLL_EXPORT*/ DWORD GetControllerCamera(DWORD sceneHandle, DWORD ControllerHandle);

//-------------------------------------------------------------------------------------------
///Walker���ζ�����صķ���
//-------------------------------------------------------------------------------------------
/** ����Walker���ζ���
param in scneneHandle : ����ָ��
param in wInfo : walker�Ĳ���
return ����ɹ�����walker��ָ�룬���򷵻�NULL
*/
/*DLL_EXPORT*/ DWORD CreateWalker(DWORD sceneHandle, WalkerInfo* wInfo);
//ɾ��Walker���ζ���
/*DLL_EXPORT*/ VOID DeleteWalkerByHandle(DWORD sceneHandle, DWORD walkerHandle);
/*DLL_EXPORT*/ VOID DeleteWalkerByName(DWORD sceneHandle, const CHAR* pName);
//�õ�Walker������
/*DLL_EXPORT*/ INT GetWalkerCount(DWORD sceneHandle);

//ͨ����Ż��Walker
/*DLL_EXPORT*/ DWORD GetWalkerHandleByIndex(DWORD sceneHandle, INT index);

//ͨ��name���Walker
/*DLL_EXPORT*/ DWORD GetWalkerHandleByName(DWORD sceneHandle, const CHAR* sName);

//ͨ������������
/*DLL_EXPORT*/ VOID GetWalkerNameByHandle(DWORD sceneHandle, DWORD walkerHandle, CHAR* name);

//����Ԥ��״̬ʱ���ߵ�walker������
/*DLL_EXPORT*/ VOID SetWalkerProp(DWORD sceneHandle, DWORD walkerHandle, WalkerInfo* wInfo);
//��������õ�Walker������
/*DLL_EXPORT*/ VOID GetWalkerProp(DWORD sceneHandle, DWORD walkerHandle, WalkerInfo* wInfo);


//-------------------------------------------------------------------------------------------
///Editor���ƶ�����صķ���
//-------------------------------------------------------------------------------------------
//����Editor����
/*DLL_EXPORT*/ DWORD CreateEditor(DWORD sceneHandle, EditorInfo* esInfo);
//ɾ��Editor����
/*DLL_EXPORT*/ VOID DeleteEditorbyHandle(DWORD sceneHandle, DWORD editorHandle);
//ɾ��Editor����
/*DLL_EXPORT*/ VOID DeleteEditorbyName(DWORD sceneHandle, const CHAR* pName);

//�õ�Editor������
/*DLL_EXPORT*/ INT GetEditorCount(DWORD sceneHandle);

//ͨ����Ż��Editor
/*DLL_EXPORT*/ DWORD GetEditorHandleByIndex(DWORD sceneHandle, INT index);
//ͨ�����ƻ��Editor
/*DLL_EXPORT*/ DWORD GetEditorHandleByName(DWORD sceneHandle, CHAR* name);

//ͨ������������
/*DLL_EXPORT*/ VOID GetEditorNameByHandle(DWORD sceneHandle, DWORD editorHandle, CHAR* name);

//����Ԥ��״̬ʱ���ߵ�Editor������
/*DLL_EXPORT*/ VOID SetEditorProp(DWORD sceneHandle, DWORD editorHandle, EditorInfo* eInfo);
//��������õ�Editor������
/*DLL_EXPORT*/ VOID GetEditorProp(DWORD sceneHandle, DWORD editorHandle, EditorInfo* eInfo);

/** ����editor���Ե�Ŀ��㣬ʵ����ҪΧ�Ƹõ���ת
@param lookatType in : looat�����ͣ�0���������� 1�������е�һ��
*/
/*DLL_EXPORT*/ VOID SetEditorLookAt(DWORD sceneHandle, DWORD editorHandle,	const VECTOR3 &v3, INT lookatType = 0);

/** ����editor���Ե�ģ�ͣ�ʵ����ҪΧ��ģ�Ͱ�Χ�е�������ת
*/
/*DLL_EXPORT*/ VOID SetEditorLookAtModel(DWORD sceneHandle, DWORD editorHandle,	DWORD modelHandle);

// ��ȡeditor���ƶ������Ե�ģ�����ƣ����û������ģ�ͣ�����false
/*DLL_EXPORT*/ BOOL GetEditorLookAtModelName(DWORD sceneHandle, DWORD editorHandle, CHAR* sName);

//-------------------------------------------------------------------------------------------
///Flyer���ƶ�����صķ���
//-------------------------------------------------------------------------------------------
//����Flyer����
/*DLL_EXPORT*/ DWORD CreateFlyer(DWORD sceneHandle, FlyerInfo* fInfo);
//ɾ��Flyer����
/*DLL_EXPORT*/ VOID DeleteFlyerbyHandle(DWORD sceneHandle, DWORD flyerHandle);
//ɾ��Flyer����
/*DLL_EXPORT*/ VOID DeleteFlyerbyName(DWORD sceneHandle, const CHAR* pName);

//�õ�Flyer������
/*DLL_EXPORT*/ INT GetFlyerCount(DWORD sceneHandle);

//ͨ����Ż��Flyer
/*DLL_EXPORT*/ DWORD GetFlyerHandleByIndex(DWORD sceneHandle, INT index);
//ͨ�����ƻ��Flyer
/*DLL_EXPORT*/ DWORD GetFlyerHandleByName(DWORD sceneHandle, CHAR* name);

//ͨ������������
/*DLL_EXPORT*/ VOID GetFlyerNameByHandle(DWORD sceneHandle, DWORD flyerHandle, CHAR* name);

//����Ԥ��״̬ʱ���ߵ�Flyer������
/*DLL_EXPORT*/ VOID SetFlyerProp(DWORD sceneHandle, DWORD flyerHandle, FlyerInfo* fInfo);
//��������õ�Editor������
/*DLL_EXPORT*/ VOID GetFlyerProp(DWORD sceneHandle, DWORD flyerHandle, FlyerInfo* fInfo);


//-------------------------------------------------------------------------------------------
///�ؼ�֡������صķ���
//-------------------------------------------------------------------------------------------
/** ��ȡָ�����Ͷ�������Ŀ
@param in type : Ŀǰ������
	VAT_KEYFRM = 0,       // �ؼ�֡����
	VAT_CAMERA,           // �������
	VAT_LIGHT,            // �ƹ⶯��
	VAT_SKELETON          // ��������
*/
/*DLL_EXPORT*/ DWORD GetAnimationCount(DWORD sceneHandle, VGS_ANIMATIONTYPE type);

/** ���������õ�����������
@param in type  : VAT_KEYFRM���ؼ�֡������VAT_CAMERA�����������VAT_LIGHT���ƹ⶯��, VAT_SKELETON:��������
@param in index : ��������
@param out sName : ����
@return �õ����ƣ�����true������false
*/
/*DLL_EXPORT*/ BOOL GetAnimationNameByIndex(DWORD sceneHandle, VGS_ANIMATIONTYPE type, int index, CHAR* sName);

/** ����ָ�����ƵĶ���
*/
/*DLL_EXPORT*/ VOID PlayAnimation(DWORD sceneHandle, const CHAR* sAnimName);

/** �������ж���
*/
/*DLL_EXPORT*/ VOID PlayAllAnimation(DWORD sceneHandle);

/** ��ָͣ�����ƵĶ���
*/
/*DLL_EXPORT*/ VOID PauseAnimation(DWORD sceneHandle, const CHAR* sAnimName);
/** ��ͣ���ж���
*/
/*DLL_EXPORT*/ VOID PauseAllAnimation(DWORD sceneHandle);

/** ָֹͣ�����ƵĶ���
*/
/*DLL_EXPORT*/ VOID StopAnimation(DWORD sceneHandle, const CHAR* sAnimName);
/** ֹͣ���ж���
*/
/*DLL_EXPORT*/ VOID StopAllAnimation(DWORD sceneHandle);

/** �õ��ؼ�֡������֡��
*/
/*DLL_EXPORT*/ INT GetKeyframeCount(DWORD sceneHandle, const CHAR* sAnimName);

/** ����ĳһ֡
*/
/*DLL_EXPORT*/ VOID GotoFrame(DWORD sceneHandle, const CHAR* sAnimName, INT FrameIndex);

/** �õ���ǰ���ڲ��ŵ�֡
*/
/*DLL_EXPORT*/ INT GetCurrentFrame(DWORD sceneHandle, const CHAR* sAnimName);

/** �������йؼ�֡�������ŵ�֡��
*/
/*DLL_EXPORT*/ VOID SetAllKfrPlaySpeed(DWORD sceneHandle, INT FPS);

/** ����ָ���ؼ�֡�������ŵ�֡��
*/
/*DLL_EXPORT*/ VOID SetKfrPlaySpeed(DWORD sceneHandle, const CHAR* sAnimName, INT FPS);

/** ��ùؼ�֡�������ŵ�֡��
*/ 
/*DLL_EXPORT*/ INT GetKfrPlaySpeed(DWORD sceneHandle, const CHAR* sAnimName); 

/** ��ȡ����FPS��ȱʡֵ
*/
/*DLL_EXPORT*/ INT GetDefaultFPS();

/** ��ȡ�������Ƿ�ѭ������
*/
/*DLL_EXPORT*/ BOOL GetAnimationLoop(DWORD sceneHandle, const CHAR* sAnimName); 

/** ���ö������Ƿ�ѭ������
*/
/*DLL_EXPORT*/ VOID SetAnimationLoop(DWORD sceneHandle, const CHAR* sAnimName, BOOL bLoop); 


///////////////////////// math functions /////////////////////////////////
/** ���ݷ��ߺ͵㴴��һ��ƽ��
@param vNormal in ����Ļ�ķ���
@param point   in : ƽ��������һ��
@return : ƽ���ָ��
*/
/*DLL_EXPORT*/ DWORD CreatePlane(const VECTOR3 &vNormal, const VECTOR3 &point);

/** ����һ��ƽ��
*/ 
/*DLL_EXPORT*/ VOID DestroyPlane(const DWORD hPlane);

/** ����һ�㵽һ��ƽ��ľ���
@param hPlane  in ��ƽ�����ָ��
@param point   in : �ռ��е�����һ��
@param dist    out����point��ƽ��hPlane�ľ��룬���pointλ��hPlane�ķ��������򣬷���ֵ>0,����ڸ������ϣ�����ֵ<0,�������ƽ���ϣ�����ֵ=0
@return : ��ȷ�õ����������true������false
*/
/*DLL_EXPORT*/ BOOL GetDistance(const DWORD hPlane, const VECTOR3 &point, FLOAT &dist);

/** ����һ���᷽�򣬾���һ����Ԫ�����ú���µķ���
@param quat     in  : ��ת��Ԫ��
@param vOrigDir in  : ��ʼ�ķ���
@param vNewDir  out : vOrigDir����quat���ú���µķ���
*/
/*DLL_EXPORT*/ VOID RotationAxis(const VGS_QUATERNION &quat, const VECTOR3 &vOrigDir, VECTOR3 &vNewDir); 

////////////////////////////////////////////////////////////////////////////

//////////////////// ��Ϣ��Ӧ ///////////////////////////////

/** ��Ϣ��Ӧ
@param sceneHandle in : �������
@param hWnd in : ��Ⱦ���ھ������ΪNULL����ʾ��������Ⱦ���ڡ�
@param uMsg in : ��ϢID���ɲο�windows��׼��Ϣ˵��
@param wParam/lParam  in : ��Ϣ�������ɲο�windows��׼��Ϣ˵��
*/
/*DLL_EXPORT*/ VOID VGSMsgProc(DWORD sceneHandle, DWORD hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//////////////////// ���� ////////////////////////////////
/// ��ȡ���һ�δ�����
/*DLL_EXPORT*/ DWORD GetVGSLastErrCode();

/// ��ȡ���һ�δ�������
/*DLL_EXPORT*/ VOID GetVGSLastErrStr(CHAR *sErrStr);

/// �õ��������Ӧ����ϸ����
/*DLL_EXPORT*/ VOID GetVGSErrStr(DWORD Code, CHAR *sErrStr);

///////////////////��������ء���Start/////////////////////////////////////////////////////////
/**����ģ������ϵ��ѡ�����
@param sceneHandle in : �������
@param modelHandle in : ģ�;��
@param selectStatus in :0-��ѡ���κ���,1-ѡ��x�ᣬ2-ѡ��y�ᣬ3-ѡ��z�ᣬ4-XYƽ��, 5-YZƽ��, 6-XZƽ��.
*/
/*DLL_EXPORT*/ VOID SelectModelAxis(DWORD sceneHandle, DWORD modelHandle, UINT selectStatus);
/**�����Ƿ���ʾģ������ϵ
@param sceneHandle in : �������
@param modelHandle in : ģ�;��
@param modelHandle in :true-��ʾ.
*/
/*DLL_EXPORT*/ VOID SetShowModelAxis(DWORD sceneHandle, DWORD modelHandle, BOOL bShow);
/**����ģ������ϵ��ѡ���״̬.
@param sceneHandle in : �������
@param modelHandle in : ģ�;��
@param nStatus in :0-��ʾ��������ϵ��1����ʾƽ������ϵ��2����ʾ��ת����ϵ��3����ʾ��������ϵ������ֵ����ʾ.
*/
/*DLL_EXPORT*/ VOID SetModelAxisStatus(DWORD sceneHandle, DWORD modelHandle, int nStatus);
//��ѡ���Χ��
int GetModelAxisStatusUnderCursor(DWORD sceneHandle, DWORD modelHandle, DWORD hViewport, UINT nScreenX, UINT nScreenY);

///////////////////��������ء���End/////////////////////////////////////////////////////////

protected:
	LuaEngine*	m_pLuaEngine;
	//��¼N3d�ļ����ڵ�Ŀ¼�����ڲ���
	std::string	m_strN3dDir;
public:
	void setN3dDir(const char* n3dDir);
	const std::string& getN3dDir()
	{
		return m_strN3dDir;
	}
public://Luaר����
	LuaEngine* getLuaEnginePtr() { return m_pLuaEngine;}
	int Lua_GetViewPortRect(LuaState * state);
	int Lua_GetModelNameByIndex(LuaState * state);
	int Lua_GetModelNameByHandle(LuaState * state);
	int Lua_GetModelPosition(LuaState * state);
	int Lua_SetModelPosition(LuaState * state);
	int Lua_MoveModel(LuaState * state);
	int Lua_GetModelRotation(LuaState * state);
	int Lua_SetModelRotation(LuaState * state);
	int Lua_RotateModel(LuaState * state);
	int Lua_GetModelScale(LuaState * state);
	int Lua_SetModelScale(LuaState * state);
	int Lua_ScaleModel(LuaState * state);

public://������ʾ�������ʱʹ��
	int Lua_createFlashOverlay(LuaState* state);
	int Lua_createRenderTexture(LuaState* state);
	int Lua_createFlashRTT(LuaState* state);
	int Lua_setFlashRenderState(LuaState* state);

private:
	CMainFrameListener* m_pMainFrameListener;

};
