// VGS2.h : CVGS2 
#pragma once
#include "resource.h"       //
#include <atlctl.h>
#include "VGSATL2_i.h"
#include "_IVGS2Events_CP.h"



#include "VGSATLHeader.h"
#include "VGS2Window.h"
#include <wininet.h>
#include "VGS2DLL.h"
#include "VGSTypedef.h"
#include "..\\VGSCoreV3\\VGSDef.h"


// CVGS2
class ATL_NO_VTABLE CVGS2 :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IVGS2, &IID_IVGS2, &LIBID_VGSATL2Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStreamInitImpl<CVGS2>,
	public IOleControlImpl<CVGS2>,
	public IOleObjectImpl<CVGS2>,
	public IOleInPlaceActiveObjectImpl<CVGS2>,
	public IViewObjectExImpl<CVGS2>,
	public IOleInPlaceObjectWindowlessImpl<CVGS2>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CVGS2>,
	public CProxy_IVGS2Events<CVGS2>,
	public IPersistStorageImpl<CVGS2>,
	public ISpecifyPropertyPagesImpl<CVGS2>,
	public IQuickActivateImpl<CVGS2>,
	public IDataObjectImpl<CVGS2>,
	public IProvideClassInfo2Impl<&CLSID_VGS2, &__uuidof(_IVGS2Events), &LIBID_VGSATL2Lib>,
	public IPropertyNotifySinkCP<CVGS2>,
	public CComCoClass<CVGS2, &CLSID_VGS2>,
	public IObjectSafetyImpl<CVGS2, INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>,
	public IPersistPropertyBagImpl<CVGS2>,
	public CComControl<CVGS2>
{
public:


	CVGS2();
	~CVGS2();

	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
		OLEMISC_INSIDEOUT |
		OLEMISC_ACTIVATEWHENVISIBLE |
		OLEMISC_SETCLIENTSITEFIRST
		)

		DECLARE_REGISTRY_RESOURCEID(IDR_VGS2)


	BEGIN_COM_MAP(CVGS2)
		COM_INTERFACE_ENTRY(IVGS2)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IViewObjectEx)
		COM_INTERFACE_ENTRY(IViewObject2)
		COM_INTERFACE_ENTRY(IViewObject)
		COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
		COM_INTERFACE_ENTRY(IOleInPlaceObject)
		COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
		COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
		COM_INTERFACE_ENTRY(IOleControl)
		COM_INTERFACE_ENTRY(IOleObject)
		COM_INTERFACE_ENTRY(IPersistStreamInit)
		COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
		COM_INTERFACE_ENTRY(IQuickActivate)
		COM_INTERFACE_ENTRY(IPersistStorage)
		COM_INTERFACE_ENTRY(IDataObject)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo2)
		COM_INTERFACE_ENTRY(IPersistPropertyBag)
	END_COM_MAP()

	BEGIN_PROP_MAP(CVGS2)
		PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
		PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
		PROP_ENTRY("wbType", 290, CLSID_NULL)

	END_PROP_MAP()

	BEGIN_CONNECTION_POINT_MAP(CVGS2)
		CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
		CONNECTION_POINT_ENTRY(__uuidof(_IVGS2Events))
	END_CONNECTION_POINT_MAP()

	BEGIN_CATEGORY_MAP(CVGS2)  
		IMPLEMENTED_CATEGORY(CATID_SafeForScripting)  
		IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)  
	END_CATEGORY_MAP()  

	BEGIN_MSG_MAP(CVGS2)
		CHAIN_MSG_MAP(CComControl<CVGS2>)
		DEFAULT_REFLECTION_HANDLER()
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()

public:

	// CDownloadN3DFileManager*   m_pDownloadN3DFileManager; // n3d下载任务管理器

	CVGS2DLL                  m_VGSEngine;              // vgs渲染引擎接口

	//---------VGS2窗口对象指针---------------
	//用途: 该窗体对象是IE窗体的子窗体，用于显示VGS场景
	CVGS2Window*				m_pVGS2Wnd;			
	//---------------------------------------				

	//---------渲染线程同步对象---------------
	//用途: 该线程同步对象用于防止渲染线程在渲染时，渲染队列被其它线程修改
	CRITICAL_SECTION			m_render_cs;	
	//---------------------------------------

	//---------控制线程同步对象---------------
	//用途: 该线程同步对象用于防止多个线程同时取得顶点缓冲、贴图表面的控制权
	CRITICAL_SECTION			m_ctrl_cs;	
	//---------------------------------------

	//----------网络相关变量-----------------
	BOOL						m_IsWebFile;					//是否是网络文件
	CHAR						m_webServer[512];				//服务器地址
	CHAR						m_localPath[512];				//本地路径
	HINTERNET					m_hInternetSession;		        //网络会话对象
	//----------js交互外部属性-----------------------------------------------------------------------
	DWORD                       m_theTextHandle;                  //创建的text的句柄
	DWORD                       m_theModelHandle;                 //指定模型的句柄
	DWORD                       m_theCamHandle;                   //指定的相机的句柄
	DWORD                       m_theSceneResHandle;              //指定的场景资源句柄
	string                      m_strflashVar;                    //指定的flash变量

	// 这个成员变量，是 IPersistXXXImpl 所必须的
	BOOL m_bRequiresSave;	// 表示属性数据是否已经改变而需要保存

	WEBBROWSERTYPE  m_BrowserType;     // 浏览器类型，0 ：ie 1：ff 2：google

	//-------------------------MSG CALL BACK-------------------------------------------------------------
	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		return S_OK;
	}
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	//创建网络会话对象
	BOOL	CreateInternetSession();
	//删除网络会话对象
	VOID	ReleaseSession();

	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] =
		{
			&IID_IVGS2,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

	// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

	// IVGS2
public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	void FinalRelease()
	{
	}

public:
	STDMETHOD(SetAddtionalKeys)(/*[in]*/BYTE left, /*[in]*/BYTE right, /*[in]*/BYTE up, /*[in]*/BYTE down);
	STDMETHOD(SetAddtionalKeysStatus)(/*[in]*/BOOL bEnable);
	STDMETHOD(SetButtonRect)(BSTR buttonName, INT x, INT y, INT width, INT height);
	STDMETHOD(SetSceneResColor)(BSTR SceneName,INT red,INT green,INT blue,INT alpha);
	STDMETHOD(get_theSceneScaleZ)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theSceneScaleY)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theSceneScaleX)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theSceneRotZ)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theSceneRotY)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theSceneRotX)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theScenePosZ)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theScenePosY)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theScenePosX)(/*[out, retval]*/ float *pVal);
	STDMETHOD(GetInfoFromSceneResource)(BSTR SceneName);
	STDMETHOD(LoadN3DScene)(BSTR SceneName, BSTR ParentSceneName, BSTR N3DFileURL,FLOAT posX,FLOAT posY,FLOAT posZ,FLOAT rotX,FLOAT rotY,FLOAT rotZ,FLOAT scaleX,FLOAT scaleY,FLOAT scaleZ);
	STDMETHOD(SetFlashVarible)(BSTR flashName,BSTR varibleName,BSTR varibleValue);
	STDMETHOD(GetFlashVaribleInfo)(BSTR flashName,BSTR varibleName);
	STDMETHOD(get_theFlashVar)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(DeleteFlashCtrlSemaphore)(BSTR flashName, BSTR SemaphoreName);
	STDMETHOD(AddFlashCtrlSemaphore)(BSTR flashName,BSTR SemaphoreName,BSTR SemaphoreFunc);
	STDMETHOD(SetMtrlTexture)(BSTR mtrlName, INT textureLayerIndex, INT textureType, BSTR textureName);
	STDMETHOD(SetEditCtrlMaxUpDownAngle)(FLOAT upAngle, FLOAT downAngle);
	STDMETHOD(SetTextureImageFromURL)(BSTR mtrlName, INT texLayerIdx, BSTR imageURL);
	STDMETHOD(SetColorEffect)(FLOAT brightness, FLOAT contrast, FLOAT red, FLOAT green, FLOAT blue);
	STDMETHOD(CreateColorEffect)();
	STDMETHOD(SetRealWaterCubeMap)(BSTR RealWaterName, BSTR CubeMapName);
	STDMETHOD(AddRealWaterRTFModel)(BSTR RealWaterName, BSTR ModelName);
	STDMETHOD(AddRealWaterRTTModel)(BSTR RealWaterName, BSTR ModelName);
	STDMETHOD(CreateRealWater)(BSTR name, FLOAT x, FLOAT y, FLOAT z, INT gridX, INT gridY);
	STDMETHOD(DeleteDynamicCubeMapReflectionModel)(BSTR cubeMapName, BSTR modelName);
	STDMETHOD(AddDynamicCubeMapReflectionModel)(BSTR cubeMapName, BSTR ModelName);
	STDMETHOD(CreateDynamicCubeMap)(BSTR cubeMapame, BSTR centerModelName, INT size, INT power);
	STDMETHOD(AddModelToRTFTexture)(BSTR RTFTextureName, BSTR ModelName);
	STDMETHOD(CreateRTFTexture)(BSTR TextureName, BSTR refPlaneName, INT height, INT width, FLOAT power);
	STDMETHOD(CloneScene)(BSTR destSceneName, BSTR srcSceneName, BSTR ParentSceneName,  FLOAT m11, FLOAT m12, FLOAT m13, FLOAT m14, FLOAT m21, FLOAT m22, FLOAT m23, FLOAT m24, FLOAT m31, FLOAT m32, FLOAT m33, FLOAT m34, FLOAT m41, FLOAT m42, FLOAT m43, FLOAT m44);
	//删除场景
	STDMETHOD(DeleteScene)(BSTR SceneName);
	//加载一个子场景到指定父场景
	STDMETHOD(LoadScene)(BSTR SceneName, BSTR ParentSceneName, BSTR N3DFileURL, 
		FLOAT m11, FLOAT m12, FLOAT m13, FLOAT m14, 
		FLOAT m21, FLOAT m22, FLOAT m23, FLOAT m24, 
		FLOAT m31, FLOAT m32, FLOAT m33, FLOAT m34, 
		FLOAT m41, FLOAT m42, FLOAT m43, FLOAT m44);

	STDMETHOD(SetTextString)(long textHandle,BSTR textString);
	STDMETHOD(get_theCamAtPosZ)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theCamAtPosY)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theCamAtPosX)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theCamFromPosZ)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theCamFromPosY)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theCamFromPosX)(/*[out, retval]*/ float *pVal);
	STDMETHOD(GetInfoFromCamera)(BSTR cameraName);
	STDMETHOD(DeleteText)(long textHandle);
	STDMETHOD(SetTextColor)(long textHandle,INT red,INT green,INT blue,INT alpha);
	STDMETHOD(SetTextPosition)(long textHandle,UINT x,UINT y);
	STDMETHOD(get_theTextHandle)(/*[out, retval]*/ long *pVal);
	STDMETHOD(CreateNewText)(BSTR textString);
	STDMETHOD(GetInfoFromModel)(BSTR modelName);
	STDMETHOD(get_theModelScreenPosY)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_theModelScreenPosX)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_theModelPosZ)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theModelPosY)(/*[out, retval]*/ float *pVal);
	STDMETHOD(get_theModelPosX)(/*[out, retval]*/ float *pVal);
	STDMETHOD(CloneModel)(BSTR oriModelName,BSTR desModelName,INT ID1,INT ID2,FLOAT  posX,FLOAT posY,FLOAT posZ);
	STDMETHOD(SetSceneEditMode)(INT EditMode);
	STDMETHOD(CurCamGotoFrame)(INT frame);
	STDMETHOD(SetButtonOpacity)(BSTR buttonName,INT opacity);
	STDMETHOD(SetAlphaTestLevel)(INT level);
	STDMETHOD(SetZoneTriggerModelCallBackFunc)(BSTR modelName,BSTR funcName);
	STDMETHOD(SetOverlayMtrl)(BSTR overlayName,BSTR mtrlName);
	STDMETHOD(MoveFlash)(BSTR flashName, INT x , INT y);
	STDMETHOD(SetDynamicUVStatus)(BSTR modelName, BOOL status);
	STDMETHOD(SetModelPositionByPivot)(BSTR modelName, FLOAT x, FLOAT y, FLOAT z);
	STDMETHOD(SetDynamicTexPlaySpeed)(BSTR UseDyTexMtrlName,INT texLayerIdx, INT fps);
	STDMETHOD(SetAllCameraKfrRepeatCnt)(INT repeatCnt);
	STDMETHOD(SetCameraKfrRepeatCnt)(BSTR camName, INT repeatCnt);
	STDMETHOD(SetAllCameraKfrPlayStatus)(BOOL status);
	STDMETHOD(SetAllModelVisibility)(BOOL status);
	STDMETHOD(SetCameraKfrPlayStatus)(BSTR camName, BOOL status);
	STDMETHOD(PlayKeyFrame1)(INT startFrame, INT endFrame);
	STDMETHOD(SetMtrlSpecularPower)(BSTR mtrlName, INT power);
	STDMETHOD(SetMtrlSpecularLevel)(BSTR mtrlName, INT level);
	STDMETHOD(SetMtrlAmbient)(BSTR mtrlName, INT red, INT green, INT blue);
	STDMETHOD(SetMtrlSpecular)(BSTR mtrlName, INT red, INT green, INT blue);
	STDMETHOD(EndFSGlow)();
	STDMETHOD(StartFSGlow)();
	STDMETHOD(SetFSGlowValue)(FLOAT BloomValue, FLOAT BlurValue);
	STDMETHOD(CreateFSGlow)(FLOAT BloomValue, FLOAT BlurValue);
	STDMETHOD(SetKeyFramePlaySpeed)(INT fps);
	STDMETHOD(GotoFrame)(INT frame);
	STDMETHOD(SetKeyFrameRepeatCount)(INT repeatCnt);
	STDMETHOD(StopKeyFrame)();
	STDMETHOD(PauseKeyFrame)();
	STDMETHOD(PlayKeyFrame)();
	STDMETHOD(DeleteAllModel)();
	STDMETHOD(PushOverlayBack)(BSTR overlayName);
	STDMETHOD(PushOverlayFront)(BSTR OverlayName);
	STDMETHOD(SetOverlayFullScreen)(BSTR OverlayName, BOOL xFull, BOOL yFull);
	STDMETHOD(SetPlayerZoomSpeed)(FLOAT speed);
	STDMETHOD(SetNavigaterPointerObject)(BSTR navName, BSTR navPointerName, INT objType, BSTR objName);
	STDMETHOD(ResumeCameraKfr)(BSTR cameraName);
	STDMETHOD(PauseCameraKfr)(BSTR camName);
	STDMETHOD(DeleteNavigaterPointer)(BSTR navigaterName, BSTR navPointerName);
	STDMETHOD(SetNavigaterPointerVisibility)(BSTR navigaterName, BSTR navPointerName, BOOL status);
	STDMETHOD(AddNavigaterPointer)(BSTR navigaterName,
		BSTR navPointerName,  
		INT	 navPointerType,
		BSTR camOrModelName,
		BSTR navPointerOriImage, 
		BSTR navPointerAlphaImage, 
		INT  width, INT height);
	STDMETHOD(SetModelRotation)(BSTR modelName, FLOAT x, FLOAT y, FLOAT z);
	STDMETHOD(SetModelPosition)(BSTR modelName, FLOAT x, FLOAT y, FLOAT z);
	STDMETHOD(SetAllModelKfrPlayStatus)(BOOL kfrPlay);
	STDMETHOD(SetAllLightKfrRepeatCnt)(INT repeatCnt);
	STDMETHOD(SetLightKfrRepeatCnt)(BSTR lightName, INT repeatCnt);
	STDMETHOD(SetAllLightKfrPlayStatus)(BOOL bFlag);
	STDMETHOD(SetLightKfrPlayStatus)(BSTR lightName, BOOL bFlag);
	STDMETHOD(SetModelMouseHintType)(BSTR modelName,  INT type);
	STDMETHOD(ClearModelGroup)(BSTR modelGroupName);
	STDMETHOD(DeleteModelFromModelGroup)(BSTR modelGroupName, BSTR modelName);
	STDMETHOD(AddModelToModelGroup)(BSTR modelGroupName, BSTR modelName);
	STDMETHOD(DeleteAllModelGroup)();
	STDMETHOD(DeleteModelGroupByIndex)(INT index);
	STDMETHOD(DeleteModelGroup)(BSTR name);
	STDMETHOD(RewindAudio)(BSTR audioName);
	STDMETHOD(StopAudio)(BSTR audioName);
	STDMETHOD(PauseAudio)(BSTR audioName);
	STDMETHOD(PlayAudio)(BSTR audioName);
	STDMETHOD(SetAudioRepeatCount)(BSTR audioName, INT repeatCnt);
	STDMETHOD(SetAudioFile)(BSTR audioName, BSTR audioFile);
	STDMETHOD(DeleteAllAudio)();
	STDMETHOD(DeleteAudioByIndex)(INT index);
	STDMETHOD(DeleteAudioByName)(BSTR name);
	STDMETHOD(CreateAudio)(BSTR name, BSTR audioFile, INT repeatCnt);
	STDMETHOD(SetOverlayVisibility)(BSTR overlayName, BOOL visible);
	STDMETHOD(ExecuteSlice)(BSTR sectionMapName);
	STDMETHOD(CreateSectionMap)(BSTR sectioMapName, 
		INT width, INT height, 
		BSTR sliceModelName, 
		FLOAT leftTopX, FLOAT leftTopY, FLOAT leftTopZ, 
		FLOAT rightTopX, FLOAT rightTopY, FLOAT rightTopZ, 
		FLOAT leftBottomX, FLOAT leftBottomY, FLOAT leftBottomZ, 
		FLOAT rightBottomX, FLOAT rightBottomY, FLOAT rightBottomZ, 
		BSTR imageName );
	STDMETHOD(SetButtonActive)(BSTR name, BOOL bActive);
	STDMETHOD(SetMtrlSpecularCubeMap)(BSTR mtrlName, BSTR specularCubeMapName);
	STDMETHOD(SetAntiAliasLevel)(INT level);
	STDMETHOD(SetButtonVisibility)(BSTR btnName, BOOL visible);
	STDMETHOD(SetModelEventOnMouseMove)(BOOL status);
	STDMETHOD(SetAllModelsKfrPlayStatus)(BOOL bKfrPlay);
	STDMETHOD(PlayModelKfr)(BSTR modelName, INT startFrame, INT endFrame, INT repeatCnt);
	STDMETHOD(SetModelKfrPlayStatus)(BSTR name, BOOL kfrPlay);
	STDMETHOD(DeleteMovieTexture)(BSTR mtrlName, BSTR movieTexName);
	STDMETHOD(SetPlayerCanLift)(BOOL status);
	STDMETHOD(SetPlayerCanZoom)(BOOL status);
	STDMETHOD(SetPlayerCanRos)(BOOL status);
	STDMETHOD(SetPlayerCanMove)(BOOL status);
	STDMETHOD(SetCanNavigateByPointer)(BSTR navName, BOOL status);
	STDMETHOD(DeleteModel)(BSTR modelName);
	STDMETHOD(CreateCamera)(BSTR name, FLOAT fromX, FLOAT fromY, FLOAT fromZ, FLOAT atX, FLOAT atY, FLOAT atZ, FLOAT fov, FLOAT nearClip, FLOAT farClip);
	STDMETHOD(SetSpecularMapMtrl)(BSTR mtrlName, BSTR specularMapName);
	STDMETHOD(CreateSpecularMap)(BSTR name, BSTR imageName, INT width, INT height, INT mipLevel);
	STDMETHOD(CreateBackdropFromImage)(BSTR backdropName, BSTR oriImageName, BSTR alphaImageName, INT alignMode, INT left, INT top, INT width, INT height);
	STDMETHOD(SetAllModelKfrRepeatCnt)(INT kfrRepeatCnt);
	STDMETHOD(SetModelKfrRepeatCnt)(BSTR modelName, INT kfrRepeatCnt);
	STDMETHOD(SetMovieMtrl)(BSTR mtrlName, BSTR movieTexName);
	STDMETHOD(CreateMovieTexture)(BSTR texName, BSTR moviePath, INT movieType, BOOL withAudio, INT repeatCnt, BSTR mtrlName);
	STDMETHOD(SetLightStatus)(BSTR lightName, BOOL status);
	STDMETHOD(SetLightAttenuation)(BSTR lightName, FLOAT attenuation0, FLOAT attenuation1, FLOAT attenuation2);
	STDMETHOD(SetLightRange)(BSTR lightName, FLOAT range);
	STDMETHOD(SetLightPosition)(BSTR lightName, FLOAT x , FLOAT y, FLOAT z);
	STDMETHOD(SetLightAmbient)(BSTR lightName, INT red, INT green, INT blue);
	STDMETHOD(SetLightPower)(BSTR lightName, FLOAT power);
	STDMETHOD(SetLightColor)(BSTR lightName, INT red, INT green, INT blue);
	STDMETHOD(SetMtrlCubeMap)(BSTR mtrlName, BSTR cubeMapName, FLOAT power);
	STDMETHOD(CreateStaticCubeMap)(BSTR name, BSTR frontImage, BSTR backImage, BSTR leftImage, BSTR rightImage, BSTR topImage, BSTR bottomImge, INT size);
	STDMETHOD(SetCollisionDistance)(FLOAT distance);
	STDMETHOD(SetLensFlareVisibility)(BOOL status);
	STDMETHOD(SetZOneModel)(BSTR modelName, BOOL status);
	STDMETHOD(SetAlphaTestStatus)(BSTR mtrlName, BOOL status);
	STDMETHOD(RestoreAllModelsColor)();
	STDMETHOD(RestoreModelColor)(BSTR modelName);
	STDMETHOD(SetNavigaterVisibility)(BSTR navName, BOOL visible);
	STDMETHOD(SetMouseMoveEventTimer)(INT timeStep);
	STDMETHOD(SetModelMouseColor)(BSTR modelName, 
		INT mouseOver_Red, 
		INT mouseOver_Green, 
		INT mouseOver_Blue,
		INT mouseOver_Alpha,
		INT mouseClick_Red,
		INT mouseClick_Green,
		INT mouseClick_Blue,
		INT mouseClick_Alpha);
	STDMETHOD(SetFilter)(BSTR overlayName, BOOL status, INT opacity);
	STDMETHOD(SetFogRenderStatus)(BOOL status);
	STDMETHOD(SetFogField)(FLOAT start, FLOAT end);
	STDMETHOD(SetFogDensity)(FLOAT density);
	STDMETHOD(SetFogColor)(INT red, INT blue, INT green);
	STDMETHOD(SetFogMode)(INT mode);
	STDMETHOD(CreateFog)(INT fogMode, INT red, INT green, INT blue, FLOAT density, FLOAT start, FLOAT end);
	STDMETHOD(CreateLensFlare)(FLOAT x, FLOAT y, FLOAT z, BOOL coverFlag);
	STDMETHOD(SetFlashCurrentFrame)(BSTR flashName, INT frameIndex);
	STDMETHOD(SetCameraKfrCallBackFunc)(BSTR camName, INT frameIndex, BSTR funcName);
	STDMETHOD(SetCameraKFRFSMotionBlur)(BSTR camName, INT startFrame, INT endFrame, INT power, BOOL status);
	STDMETHOD(StopFSMotionBlur)();
	STDMETHOD(StartFSMotionBlur)();
	STDMETHOD(SetFSMotionBlurPower)(INT value);
	STDMETHOD(CreateFSMotionBlur)(INT blurCount);
	STDMETHOD(SetCurrentCameraFov)(FLOAT fov);
	STDMETHOD(SetCameraFov)(BSTR cameraName, FLOAT fov);
	STDMETHOD(SetCameraAt)(BSTR cameraName, FLOAT x, FLOAT y, FLOAT z);
	STDMETHOD(SetCameraFrom)(BSTR cameraName, FLOAT x, FLOAT y, FLOAT z);
	STDMETHOD(SetSkyBoxModel)(BSTR modelName, BOOL status);
	STDMETHOD(SetBumpWaterRadius)(BSTR waterName, FLOAT radius);
	STDMETHOD(SetBumpWaterSpeed)(BSTR waterName, FLOAT speed);
	STDMETHOD(SetModelFBLStatus)(BSTR modelName, BOOL status);
	STDMETHOD(SetModelColor)(BSTR modelName, INT red, INT green, INT blue, INT alpha);
	STDMETHOD(PlayerGoDown)();
	STDMETHOD(PlayerGoUp)();
	STDMETHOD(PlayerZoomOut)();
	STDMETHOD(PlayerZoomIn)();
	STDMETHOD(PlayerTurnDown)();
	STDMETHOD(PlayerTurnUp)();
	STDMETHOD(PlayerTurnRight)();
	STDMETHOD(PlayerTurnLeft)();
	STDMETHOD(PlayerStop)();
	STDMETHOD(PlayerGoRightward)();
	STDMETHOD(PlayerGoLeftward)();
	STDMETHOD(PlayerGoBackward)();
	STDMETHOD(PlayerGoForward)();
	STDMETHOD(RestoreAllModelsOpacity)();
	STDMETHOD(RestoreModelOpacity)(BSTR modelName);
	STDMETHOD(MoveModelTo)(BSTR modelName, FLOAT x, FLOAT y, FLOAT z, INT frameCnt, INT accModel);
	STDMETHOD(MoveModel)(BSTR modelName, FLOAT x, FLOAT y, FLOAT z, INT coordEnum);
	STDMETHOD(SetMtrlIllumination)(BSTR mtrlName, INT illumination);
	STDMETHOD(DeleteMtrl)(BSTR mtrlName);
	STDMETHOD(SetMtrlColor)(BSTR mtrlName, INT red, INT green, INT blue, INT alpha);
	STDMETHOD(CreateMtrl)(BSTR mtrlName, INT red, INT green, INT blue, INT alpha, INT selfIllu);
	STDMETHOD(RestoreAllModelsMtrl)();
	STDMETHOD(RestoreModelMtrl)(BSTR modelName);
	STDMETHOD(GetModelHandleByName)(BSTR modelName, VARIANT* modelHandle);
	STDMETHOD(ShowAllModels)();
	STDMETHOD(HideAllModels)();
	STDMETHOD(FocusCurrentCameraTo)(BSTR model1Name, BSTR model2Name, FLOAT x, FLOAT y, FLOAT z, FLOAT distance, INT stepCnt);
	STDMETHOD(RotateCamera)(BSTR cameraName, FLOAT x, FLOAT y, FLOAT z, INT stepCnt, INT coordEnum);
	STDMETHOD(RotateCurrentCamera)(FLOAT x, FLOAT y, FLOAT z, INT stepCnt, INT coordEnum);
	STDMETHOD(CreateBumpWaterTexture)(BSTR bumpWaterName, INT width, INT height, 
		INT xSize, INT ySize, FLOAT radius, FLOAT speed, 
		FLOAT centerX, FLOAT centerY);
	STDMETHOD(CreateModelGroup)(BSTR modelGroupName);
	STDMETHOD(SetModelOpacity)(BSTR modelName, INT opacity);
	STDMETHOD(SetModelMouseMtrl)(BSTR modelName, BSTR mouseOverMtrl, BSTR mouseDownMtrl);
	STDMETHOD(DisableMouseMoveCallback)();
	STDMETHOD(EnableMouseMoveCallback)();
	STDMETHOD(DisableMouseDBClickCallback)();
	STDMETHOD(EnableMouseDBClickCallback)();
	STDMETHOD(DisableMouseUpCallback)();
	STDMETHOD(EnableMouseUpCallback)();
	STDMETHOD(DisableMouseDownCallback)();
	STDMETHOD(EnableMouseDownCallback)();
	STDMETHOD(SetModelMouseHint)(BSTR modelName, BOOL bFlag);
	//walk ctrl method
	STDMETHOD(SetWalkCtrl_ManualParam)(FLOAT moveSpeed, FLOAT moveAcc, FLOAT rotateSpeed, FLOAT rotateAcc, FLOAT height);
	STDMETHOD(SetCurrentCameraByIndex)(INT camIndex);
	STDMETHOD(SetMtrlOpacity)(BSTR mtrlName, INT opacity);
	STDMETHOD(SetModelMtrl)(BSTR modelName, INT subMeshID, BSTR mtrlName);
	STDMETHOD(ResetScene)();
	STDMETHOD(LoadN3DFile)(BSTR strN3DFileName, BSTR N3DName);
	STDMETHOD(MoveCurrentCameraAt)(FLOAT x, FLOAT y, FLOAT z);
	STDMETHOD(MoveCurrentCameraFrom)(FLOAT x, FLOAT y, FLOAT z);
	STDMETHOD(MoveCurrentCameraAtTo)(FLOAT x, FLOAT y, FLOAT z);
	STDMETHOD(MoveCurrentCameraFromTo)(FLOAT x, FLOAT y, FLOAT z);
	STDMETHOD(MoveOverlay)(BSTR overlayName, INT x, INT y);
	STDMETHOD(SetOverlayRect)(BSTR overlayName, INT left, INT top, INT width, INT height);
	STDMETHOD(SetOverlayAlignMode)(BSTR overlayName, INT alignMode);
	STDMETHOD(SetOverlayHeight)(BSTR overlayName, INT height);
	STDMETHOD(SetOverlayWidth)(BSTR overlayName, INT width);
	STDMETHOD(SetFBL)(FLOAT fblValue);
	STDMETHOD(MoveCurrentCameraTo)(FLOAT from_x, FLOAT from_y, FLOAT from_z, FLOAT at_x, FLOAT at_y, FLOAT at_z, INT frameCnt, INT accType);
	//edit ctrl model method-------------------------------------
	STDMETHOD(SetEditCtrl_ManualParam)(FLOAT RoundSpeed, FLOAT RollSpeed);
	STDMETHOD(SetEditCtrl_AutoPitchRoundRoll)(BOOL status);
	STDMETHOD(SetEditCtrl_AutoParam)(FLOAT PitchSpeedValue, FLOAT RoundSpeedValue, FLOAT RollSpeedValue);
	STDMETHOD(SetEditCtrl_DollyDistance)(FLOAT nearDis, FLOAT farDis);
	STDMETHOD(SetEditCtrl_RosAxis)(INT rosFlag);
	STDMETHOD(SetEditCtrl_DollySpeed)(FLOAT dollySpeed);
	//-----------------------------------------------------------
	STDMETHOD(TranslateModel)(BSTR modelName, FLOAT x, FLOAT y, FLOAT z, INT coord);
	STDMETHOD(DeleteButton)(BSTR btnName);
	STDMETHOD(CreateButtonFromImage)(BSTR btnName, BSTR mouseUpImage, BSTR mouseUpImage_alpha, BSTR mouseOverImage, BSTR mouseOverImage_alpha, BSTR mouseDownImage, BSTR mouseDownImage_alpha, INT alignMode, INT x, INT y, INT width, INT height, BSTR callBackFunc);
	STDMETHOD(SetOverlayOpacity)(BSTR overlayName, INT opacity);
	STDMETHOD(DeleteOverlay)(BSTR overlayName);
	STDMETHOD(CreateOverlayFromImage)(BSTR overlayName, BSTR oriImageName, BSTR alphaImageName, INT alignMode, INT left, INT top, INT width, INT height);
	STDMETHOD(SetModelClickable)(BSTR modelName, BOOL clickable);
	STDMETHOD(SetServer)(BSTR strServer);
	STDMETHOD(StopCameraKfr)(BSTR camName);
	STDMETHOD(SetCurrentCameraAt)(FLOAT x, FLOAT y, FLOAT z);
	STDMETHOD(SetCurrentCameraFrom)(FLOAT x, FLOAT y, FLOAT z);
	STDMETHOD(PlayCameraKfr)(BSTR camName, INT repeatCnt);
	STDMETHOD(SetCurrentCameraByName)(BSTR camName);
	STDMETHOD(GotoN3D)(BSTR N3DName);
	STDMETHOD(PreloadN3DFile)(BSTR strN3DFileName, BSTR N3DName);
	STDMETHOD(SetBackgroundColor)(INT red, INT green, INT blue);
	STDMETHOD(DollyCamera)(BSTR cameraName, FLOAT value, INT stepCnt);
	STDMETHOD(DollyCurrentCamera)(FLOAT value, INT stepCnt);
	STDMETHOD(SetCurrentCameraClip)(FLOAT nearClip, FLOAT farClip);
	STDMETHOD(SetCameraClip)(BSTR cameraName, FLOAT nearClip, FLOAT farClip);
	STDMETHOD(SetControlStatus)(INT status);
	STDMETHOD(ShowDefaultLogo)(BOOL status);
	STDMETHOD(MoveCurrentCamera)(FLOAT x, FLOAT y, FLOAT z, INT stepCnt, INT coordEnum);
	STDMETHOD(MoveCamera)(BSTR cameraName, FLOAT x, FLOAT y, FLOAT z, INT stepCnt, INT coordEnum);
	STDMETHOD(DeleteFlashCtrl)(BSTR flashName);
	STDMETHOD(SetFlashPlayStatus)(BSTR flashName, INT status);
	STDMETHOD(SetRenderStatus)(BOOL status);
	STDMETHOD(SetFlashCtrlVisibility)(BSTR flashName, BOOL status);
	STDMETHOD(SetFlashRect)(BSTR flashName, INT alignMode, INT left, INT top, INT width, INT height);
	STDMETHOD(AddFlashCtrl)(BSTR flashName, BSTR flashFile, INT alignMode, INT left, INT top, INT width, INT height, BOOL loop);
	STDMETHOD(SetDynamicUV)(BSTR modelName,  FLOAT u, FLOAT v, FLOAT speed);
	STDMETHOD(SetModelVisibility)(BSTR modelName, BOOL visible);
	STDMETHOD(SetSubBrowserType)(LONG type);


	CComPtr<IFontDisp> m_pFont;
	CComPtr<IPictureDisp> m_pMouseIcon;
	CComPtr<IPictureDisp> m_pPicture;
	CComBSTR m_bstrText;


	STDMETHOD(get_wbType)(LONG* pVal);
	STDMETHOD(put_wbType)(LONG newVal);
};

OBJECT_ENTRY_AUTO(__uuidof(VGS2), CVGS2)
extern CVGS2* g_pVGS2;
