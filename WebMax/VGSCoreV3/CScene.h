/////////////////////////////////////////////////////////////////////////
// 名称: CScene类型模型定义--场景对象
// 描述: 场景的初始化，场景的渲染及控制都在这个类中实现
//       场景的渲染把静态模型、关键帧动态模型、透明模型分开渲染，提高渲染效率
// 作者: lixin
// 日期: 2006-04-30(2)
/////////////////////////////////////////////////////////////////////////

#ifndef _CSCENE_H
#define _CSCENE_H

#include "VGSHeader.h"

#include "CSysFunc.h"
#include <stdio.h>
#include "CManyFVF.h"
#include "CD3DDevice.h"
//#include "CInput.h"
#include "CFont.h"

//基本元素
#include "CCamera.h"
#include "CTexture.h"
#include "CDynamicTex.h"

#include "CMaterial.h"
#include "CModel.h"
#include "CModelGroup.h"
#include "CLight.h"
#include "CText.h"
#include "CImage.h"
//GUI
#include "GUI\CGUIManager.h"
#include "GUI\background.h"
//坐标轴
#include "Axis.h"

//剖面图类
#include "CSectionMap.h"

//特效
#include "CFullScreenMotionBlur.h"
#include "CFSGlow.h"
#include "CLensFlare.h"

//控制类
#include ".\Controller\CWalker.h"
#include ".\Controller\CEditor.h"
#include ".\Controller\CFlyer.h"

#include "RealWater.h"

#include "VGSGetRoot.h"

#include <vector>
#include <set>
using namespace std;


#include "skeleton/a3dManager.h"

// #include "GfxManager.h"

/*
//自定义消息
//相机动画播放到某一帧时发送给主窗口的消息，用于JS Func回调
#define WM_JSFUNC_PROC		(WM_USER + 119)	
#define WM_ZTRIGFUNC_PROC   (WM_USER + 1999)
*/
// material type
enum VGS_MATERIAL_TYPE
{
	VGS_MATERIAL_TYPE_3D = 0,
	VGS_MATERIAL_TYPE_2D
};


typedef struct RenderToTexture
{
	UINT                        texStage;
	LPDIRECT3DTEXTURE9			curRenderTexture;             //当前后缓冲对应的渲染到纹理(Render-to-Texture)
	LPDIRECT3DSURFACE9			curRenderSurface; 
	LPDIRECT3DSURFACE9          curRenderZSurface;
}RenderToTexture;

typedef struct ReflectInfo
{
	D3DXPLANE        refPlane;
	CModel*          refModel;
	RenderToTexture  renderToTexInfo;  
}ReflectInfo;

class VGS_EXPORT CScene : public CVGSGetRoot
{
	//D3D显示设备相关的变量------------------------------------------
public:	
 	LPDIRECT3D9				m_pd3d;							//Direct3D对象
	LPDIRECT3DDEVICE9		m_pd3dDevice;					//D3D Device对象
	// CD3DDevice*				m_DeviceInfo;					//当前系统的所有设备信息
	D3DPRESENT_PARAMETERS	m_d3dpp;						//当前的显卡参数
	D3D_PARAM				d3dParam;
	UINT					m_AvailableVideoMemSize;		//剩余的显存大小
	
	D3DDISPLAYMODE			m_d3dmm;						//当前的显存参数

    LPDIRECT3DSURFACE9		m_pBackBuffer;					//显卡后缓冲表面
	LPDIRECT3DSURFACE9		m_pZBuffer;						//显卡ZBuffer表面

	INT                     m_index;                        // 在场景队列中的索引
	std::string             m_sName;

	//CInput					InputDevice;					//输入设备对象
	
	//显示窗体
public:
	HWND					m_hWnd;							//连接D3D设备的窗体
    CHAR					windowTitle[256]; 				//窗口标题
    BOOL					m_bWindowed;					//是否以窗口模式显示

	INT						m_oriWidth;						//设置的初始宽度
	INT						m_oriHeight;					//设置的初始高度
	D3DCOLOR				m_backgroundColor;						//背景颜色

	//RECT					m_SceneRect;							//场景RECT
	//FLOAT					m_SceneRatio;							//场景窗口的宽高比width / height
	D3DVIEWPORT9			m_viewport;						//视口
	
    
    BOOL					bActive;						//当前D3D窗口是否激活

	//变换矩阵有关的成员变量---------------------------------------------------
public:
	INT						m_projMode;						//投影模式，透视模式为0，轴侧模式为1
	D3DXMATRIX				m_MatWorld;						//world transform Matrix
	D3DXMATRIX				m_MatView;						//当前View变换矩阵	
	D3DXMATRIX				m_MatProj;						//Projection Transform Matrix
	D3DXMATRIX				m_MatViewProj;					//视角和投影矩阵的乘积

	D3DXMATRIX				m_MatViewOri;					//起始View变换矩阵	
	D3DXMATRIX				m_MatViewInverse;				//当前View变换矩阵的逆矩阵

	D3DXMATRIX				m_MatIdentity;					//单位矩阵
	D3DXMATRIX				m_RollMatrix;					//摄像机旋转矩阵
	D3DXMATRIX				m_SkyBoxMatView;				//SKYBox的view变换矩阵
	D3DXMATRIX				m_SkyBoxMatProj;				//SkyBox的proj变换矩阵

	
	//用户输入有关的成员变量-------------------------------------------------
public:
	BOOL					userInput;						//用户控制状态

	POINT					lastClickPos;					//最后一次按下鼠标时光标的位置
	D3DXVECTOR3				m_pickRayDir;					//当前光标的转化为三维空间后的射线方向
	D3DXVECTOR3				m_rayDir;						//当前帧的视觉方向矢量
    D3DXVECTOR3				m_rayOrig;						//当前视觉射线的起始点位置，即视点
	
	CModel*					m_CursorModel;					//光标下方的模型
	CButton*				pMouseOverButton;				//当前光标下方的按钮

	COverlay*				pCtrlOl;						//方向控制箭头Overlay
	COverlay*				pRosOl;							//旋转罗盘Overlay
	BOOL					softKey[12];					//软件盘-方向键
	
	INT						controlType;					//当前的控制类型0walk,1Editor, 2Flyers, eles None
	CWalker*				currentWalker;					//当前Walker对象
	CEditor*				currentEditor;					//当前Editor对象
	CFlyer*					currentFlyer;					//当前Flyer对象
   
	INT                     sceneEditMode;                  //当前场景的编辑状态
	BOOL                    m_ModelEdit;                    //模型是否被编辑过状态
	BOOL                    pLButton;                       //上一个鼠标左键的状态
	D3DXVECTOR3             mouseWorldPos;                 //鼠标射线在世界坐标中的位置

	//渲染辅助变量-----------------------------------------------------
public:
	UINT					renderStatus;					//渲染状态
	BOOL					renderScene;					//是否渲染
	INT						m_FrameCnt;						//到目前为止渲染的总帧数
	UINT					fps;							//渲染帧率
	INT						antiLevel;						//抗拒齿界别

	CModel*					m_CurModel;						//显卡当前渲染的模型
	CMaterial*				m_CurMtrl;						//显示卡当前使用的材质	
	CTexture*				m_CurDifTex;					//显示卡当前使用的DifTexture
	CTexture*				m_CurLightTex;					//显示卡当前使用的LightTexture
	CMaterial*				m_CurCubeMtrl;					//渲染当前CubeMap的材质，渲染完CubeMap后设置回来
	CCubeMap*				m_CurCubeMap;					//当前渲染的CubeMap
	CTexture				m_DefaultTex;					//默认的贴图
	INT						m_CurTexStage;					//当前的贴图Stage

	CCamera*                m_pCurCamera;                   // 当前使用的相机
	CViewport*              m_CurViewport;                  // 当前要渲染的视口
	CSwapChain*             m_pCurSwapChain;                // 当前渲染的swapchain

	FLOAT					fbl;							//显示模型的包围球半径和模型到当前相机的距离的比值设定

	BOOL					m_bRenderScene;					//渲染场景
	BOOL					m_bRenderGUI;					//渲染界面

	CHAR					m_err[256];						//错误消息字符串
	
	BOOL					m_bAuthorized;					//所有N3D是否取得合法授权

	//场景中的资源---------------------------------------------------
public:
	// FLOAT					m_fileVersion;					//当前渲染得场景资源文件的版本号

	//模型有关的成员变量----------------------------------------------
	vector<CModel*>			model;							//所有模型渲染队列
	vector<CModel*>			m_vTransModel;						//透明模型渲染队列
	// std::set<CModel*>            m_TransModel; // 透明模型队列

    vector<CModel*>			m_SelectedModel;				//选中的模型列表
	//坐标轴
	Axis*				    m_pAxis;

    //Edit by Baojiahui     2007/9/27                       
	vector<TriggerCallBack*> triggerModel;                    //触发碰撞物体列表

	//Add by Baojiahui      2008/10/21
	vector<ReflectInfo>     reflectInfoList;
    BOOL                    beReflect;

	//模型组有关的变量------------------------------------------------
	vector<CModelGroup*>	modelGroup;						//模型组

	//剖面图----------------------------------------------------------
	vector<CSectionMap*>    sectionMap;						//剖面图

	//材质有关的成员变量----------------------------------------------
	vector<CMaterial*>		m_vShader;							// 3D models 材质资源队列
	vector<CMaterial*>      m_v2DMaterial;                      // GUI 材质资源队列

	//贴图有关的成员变量----------------------------------------------
	vector<CTexture*>		m_vTexture;						//贴图资源队列
	// vector<CBumpWater*>		bumpWater;						//BumpWater队列
	vector<CCubeMap*>		cubeMap;						//CubeMap对列
	//vector<DWORD>			movieTexture;					//movieTex队列
	vector<CDynamicTex*>	dynamicTexture;					//动态贴图

	vector<CBumpMap*>       m_vBumpTexture;
	vector<CRTFMap*>		m_vRtfTexture;					//实时反射贴图列表

	//摄像机有关的成员变量--------------------------------------------
	vector<CCamera*>		m_vCamera;							//摄像机列表

	//灯光有关的成员变量----------------------------------------------
	vector<CLight*>			m_vLight;							//灯光列表

	//Overlay有关的成员变量-------------------------------------------
	vector<COverlay*>		m_vOverlay;						   //Overlay列表

	BackGround*             m_pBackGround;                      // 背景图

	//Image有关的成员变量----------------------------------------------
	vector<CImage*>			m_vImage;							//image列表

	//Navigater有关的成员变量------------------------------------------
	vector<CNavigator*>		m_vNavigater;						//navigaters列表

	//Button有关的成员变量---------------------------------------------
	vector<CButton*>		m_vButton;							//button列表

	//文字相关的成员变量-----------------------------------------------
	vector<CFont*>			m_vFont;							//字体对象
	vector<CText*>			m_vText;							//文字列表

	//音频相关的成员变量-----------------------------------------------
	// vector<DWORD>			m_vAudio;							//音频队列

	//控制类对象-----------------------------------------------------
	//vector<CWalker*>		m_vWalker;							//行走漫游对象
	//vector<CEditor*>		m_vEditor;							//物件展示控制对象
	//vector<CFlyer*>			m_vFlyer;							//飞翔漫游对象

	//alphaTest 相关
	DWORD                  alphaTestLevel;

	vector <CSwapChain*>   m_vSwapChain;                   // 场景中的swapchain

	//##ModelId=47EBC730013D
	INT						m_sceneType;					//标准场景为0，材质球场景为1

	// 相机相关
	UINT getCameraCount();
	CCamera*  CreateCamera(const std::string &sName);
	CCamera*  GetCameraByName(const std::string &sName);
	CCamera*  GetCameraByIndex(INT index);
	std::string  GetCameraNameByIndex(INT index);
	INT GetCameraIndex(CCamera* pCamera);
	VOID DeleteCamera(CCamera* pCamera);
	VOID DeleteCamera(INT index);
	BOOL IsCameraExist(const std::string &sName);
	// 如果相机重名，重新命名
	std::string renameCameraIfExist(std::string &sName);

	// 灯光相关
	UINT getLightCount();
	CLight*  CreateLight(const std::string &sName);
	CLight*  GetLightByName(const std::string &sName);
	CLight*  GetLightByIndex(INT index);
	std::string  GetLightNameByIndex(INT index);
	INT GetLightIndex(CLight* pLight);
	VOID DeleteLight(CLight* pLight);
	VOID DeleteLight(INT index);
	BOOL IsLightExist(const std::string &sName);

	CSceneNode*             m_pRootNode;                    // 场景的根节点


	// 模型相关
	// 创建模型
	CModel *CreateModel(const std::string &sName);
	UINT GetModelCount();
	CModel* GetModelByName(const std::string &sName);
	CModel* GetModelByIndex(INT index);
	std::string GetModelNameByIndex(INT index);
	INT GetModelIndex(CModel* pModel);
	VOID DeleteModel(CModel* pModel);
	VOID DeleteModel(INT index);
	BOOL IsModelExist(const std::string &sName);
	// 将模型添加到场景
	void AddModel(CModel *pModel);

	// 将模型添加/删除透明模型队列
	void AddModelToTransparentList(CModel *pModel);
	void RemoveModelFormTransparentList(CModel *pModel);

	//特效类变量-----------------------------------------------------
public:
	//动态模糊对象
	CFullScreenMotionBlur*		m_pFSMotionBlur;			//动态模糊对象
	BOOL						m_bFSMotionBlur;			//是否打开动态模糊
	INT							FSBlurLevel;				//全屏动态模糊的级别

	BOOL						m_bLastFSMotionBlurStatus;	//在播放相机关键帧动画时，保存动态模糊的状态值
	BOOL						m_bRenderCurrentFrame;		//是否渲染当前帧
	BOOL						m_bLastFrameBlur;			//上一帧是否动态模糊

	//全屏光晕对象
	CFSGlow*					m_pFSGlow;					//全屏光晕对象
	BOOL						m_bFSGlow;					//是否打开全屏光晕
	INT							FSGlowLevel;				//全屏光晕的强度

	//镜头光斑对象
	CLensFlare*					m_pLensFlare;
	BOOL						m_bLensFlare;				//是否打开镜头光斑特效
	BOOL						m_bLensFlareCoverd;			//是否计算遮挡
	BOOL						m_bLensFlare_CalCover;		//是否计算遮挡
	BOOL						m_bLensFlare_Inverse;		//是否在视角背后

	//雾对象
	BOOL						m_bFog;						//是否使用雾
	D3DCOLOR					m_FogColor;					//雾的颜色
	FLOAT						m_FogStart;					//雾的起止点
	FLOAT						m_FogEnd;
	FLOAT						m_FogDensity;				//雾的浓度
	D3DFOGMODE					m_FogMode;					//雾的衰减模式


	//关键帧动画变量------------------------------------------------------
public:
	INT							m_KfrCnt;					//关键帧总数
	INT							m_KfrRepeatCnt;				//关键帧动画播放的次数
	INT							m_KfrRepeatIndex;			//关键帧动画的重复次数
	INT							m_CurrentFrame;				//当前帧
	BOOL						m_bKfrPlaying;				//关键帧动画是否在播放
	INT							m_kfrPlayspeed;				//关键帧动画播放的速度

	INT							m_StartFrame;				//关键帧动画播放的起始帧
	INT							m_EndFrame;					//关键帧动画播放的终止帧
	
	INT							m_TotalFrameCnt;			//总的帧数
	//INT						m_bRenderComplete;			//当前帧是否渲染完成
	DWORD						m_lastTimer;				//系统时间


	//场景默认的对象-----------------------------------------------
public:
	//默认的几个相机
	/*CCamera						frontCamera;
	CCamera						backCamera;
	CCamera						leftCamera;
	CCamera						rightCamera;
	CCamera						topCamera;
	CCamera						bottomCamera;
	CCamera						userCamera;
	CCamera						perspectiveCamera;
	
	CCamera						defaultCam;						//默认的摄像机
	BOOL						useDefCam;						//是否使用默认的像机模式
	CCamera*					currentCam;						//当前使用的摄像机
*/
	CFont*						pDefaultFont;
	
	COverlay*					m_pStartLogo;					//公司Logo Overlay
	COverlay*					m_pUnitedLogo;					//联盟公司的Logo Overlay
	COverlay*                   m_pLoadingBg;                   //loading background Image
	COverlay*                   m_pLoadingBar;                  //loading progress bar Image


//-----------------------------------------------------------------------------
// Name: 内部调用函数
//-----------------------------------------------------------------------------
private:

	//更新BackBuffer的长宽比
	// VOID UpdateRatio();
	//视角变换
	VOID ViewTransform(); 
	//投影变换
	VOID ProjTransform(); 
	//设置默认的摄像机
	// VOID UseDefaultCam();
	//渲染场景子对象-不透明模型
	VOID RenderOpacModels();
	// VOID DrawOpacModel();
	//渲染场景子对象-透明模型
	VOID RenderTransModels();

	//渲染角色动画
	VOID RenderA3D();

	//渲染水面材质
	VOID RenderBumpWater(CMaterial* pMtrl);
	//渲染LensFlare
	VOID RenderLensFlare();
	//渲染CubeMap
	VOID RenderCubeMap(CMaterial* pMtrl, INT stageIndex);
	//渲染Logo
	VOID RenderLogo();
	//渲染不透明反射物体
	VOID RenderOpacReflect(const VGS_RenderSubmesh& subMesh);
	//渲染透明反射物体
    //VOID RenderTransReflect(const UINT& subMeshIdx);

	//渲染场景子对象-文字
	VOID RenderText();
	//渲染场景子对象-Overlay
	VOID RenderOverlay();
    //渲染特定的overlay
	VOID RenderOverlay(COverlay* pOverlay);
	//渲染backdrop
	// VOID RenderBackdrop();
	//渲染轴对象
	VOID RenderAttach();
	//清空贴图内存
	VOID ClearTextureMem();

	//找到当前编号模型对应的动态模型编号
	//INT GetDUnitIndex(GetS3DFileInfo* pS3DFile, UINT unitIndex);
	//播放模型动画
	VOID PlayModelKeyFrame();
	VOID UpdateParentFrame(CModel** curModel);


	//场景编辑状态下调用函数
	//使用鼠标移动特定的物体
	VOID MoveModel();
	//使用鼠标旋转特定物体
	VOID RotateModel();
    //计算旋转的角度
    //-----------------------------------------------------------------------------
    FLOAT GetRotateAngle(D3DXVECTOR3* oriVector, D3DXVECTOR3* desVector, D3DXVECTOR3* axisVector);
	//设置物体编辑状态
	VOID setModelCtrl();
    
public:

    //将屏幕坐标转换成目标平面的世界坐标
	VOID GetCoordScreenToWorldFromPlane(CViewport *pViewport/*DWORD pSwapChain*/, UINT x, UINT y, 
		                                 const VECTOR3 &planePoint, const VECTOR3 &planeNormal,
										 VECTOR3 &OutVec);
    // VOID GetCoordScreenToWorldFromPlane(CSwapChain* pSwapChain, UINT x,UINT y,D3DXVECTOR3* planePoint,D3DXVECTOR3* planeNormal,D3DXVECTOR3* pOutVec);
	//将屏幕坐标转换成目标平面的世界坐标
    VOID GetCoordScreenToWorldFromPlaneAndRay(CViewport *pViewport/*CSwapChain* pSwapChain*/, UINT x,UINT y,D3DXVECTOR3* planePoint,D3DXVECTOR3* planeNormal,D3DXVECTOR3* dipartRaydir,D3DXVECTOR3* pOutVec);

	//射线求交
	BOOL IntersectModel(D3DXVECTOR3* orig, 
						D3DXVECTOR3* dir, 
						CModel* pModel, 
						D3DXVECTOR3* pNormal, 
						INT*   faceIndex,
						FLOAT* pSU,
						FLOAT* pSV,
						FLOAT* distance);
	BOOL IntersectModelBound(D3DXVECTOR3* orig, D3DXVECTOR3* dir, CModel* pModel);
	BOOL IntersectModelMesh(D3DXVECTOR3* orig, D3DXVECTOR3* dir, CModel* pModel, 
									 INT* FaceIndex, FLOAT* pU, FLOAT* pV, 
									 FLOAT* pDist, D3DXVECTOR3* vColNormal);
	BOOL IntersectTriangle(D3DXVECTOR3* orig, D3DXVECTOR3* dir, 
							   D3DXVECTOR3* p0, D3DXVECTOR3* p1, 
							   D3DXVECTOR3* p2, //三个顶点
							   float *pU, float *pV, //UV坐标
							   float *pDist); //距离
	//检测模型是否在视锥体内
	BOOL IsInFrustrum(CModel* pModel);
	//检测顶点是否在视锥体内
	BOOL IsInFrustrum(D3DXVECTOR3* pos);
	//剪裁计算
	VOID FrustrumTest();

	//用于LensFlare的屏幕坐标计算方法
	VOID CalcScreenCoords(CViewport* pViewport, D3DXVECTOR3* pos, INT* ScreenX, INT* ScreenY);
	//动态灯光
	VOID	LightFrame();

	//更新导航器Pointer
	VOID	UpdateNavigaterPointer();
	//更新导航器Map
	VOID	UpdateNavigaterMap();

	//播放moveModelTo动画的内部方法
	VOID	PlayUserMoveModelKFR();

//-----------------------------------------------------------------------------
// Name: 供外部调用的函数接口
//-----------------------------------------------------------------------------
public:
	//初始化变量
	VOID	InitVar();
	//清空当前场景，并释放场景中所有对象的内存
	VOID    Reset();
	//删除Scene对象，释放内存
	VOID	Release();
    //删除image队列
	VOID    ClearImageList();
	//清理场景中的贴图资源,没有用到的删除
	VOID	CleanTextureSource();

	// 获取场景中使用的图片名称列表
	void getImageNameList(bool includePath, vector<std::string> &vNameList);
//------------------------------------------------------------------------------
// 窗体相关的方法
//------------------------------------------------------------------------------
	//设置背景色
	VOID    SetBackground(BYTE red, BYTE green, BYTE blue);
	//更新Overlay的Rect
	VOID	UpdateOverlayRect(COverlay* pOverlay);

//------------------------------------------------------------------------------
// 渲染相关的方法
//------------------------------------------------------------------------------
	//渲染场景方法
	VOID Render(CCamera *pCamera, CViewport *pVP);

	//VOID RenderGrapha();

	//检查音频的播放状态
	VOID CheckAudioStatus();

	//设置当前的材质状态
	VOID SetMaterial(CMaterial* pMtrl);

	//设置贴图层
	VOID SetTextureLayer(CTextureLayer* pTexLayer);
	//设置bumpwater的材质;
	//VOID SetRenderWaterMaterial(CMaterial* pMtrl);
    //设置Overlay贴图
	VOID SetOverlayTexture();
/*	//设置反射贴图
	VOID SetReflectionTexture();
	//设置CubeMap环境贴图
	VOID SetCubeTexture();
	//设置高光反射贴图
	VOID SetSpecularCubeTexture();
	//设置高亮贴图
	VOID SetSpecularTexture();*/

	//更新后缓冲
	//BOOL UpdateBackBuf(RECT* pRect);

//-----------------------------------------------------------------------------
// 模型相关设定
//-----------------------------------------------------------------------------	
	//通过光标的位置的获得指定的射线
// 	VOID GetRayUnderPoint(CSwapChain* pSwapChain, UINT x, UINT y, 
// 						  D3DXVECTOR3* pOrig, D3DXVECTOR3* pDir);
	VOID GetRayUnderPoint(CViewport* pViewport, UINT x, UINT y, 
						  D3DXVECTOR3* pOrig, D3DXVECTOR3* pDir);
	//得到RayHit模型
	CModel* GetModelUnderRay(CSwapChain* pSwapChain, D3DXVECTOR3* orig, D3DXVECTOR3* dir, 
							 FLOAT* pU, FLOAT* pV,
							 FLOAT* pDist, D3DXVECTOR3* vColNormal);
	//得到光标下方的模型
	// CModel* GetModelUnderCursor(CSwapChain* pSwapChain, UINT x, UINT y);
	CModel* GetModelUnderCursor(CViewport* pViewport, UINT x, UINT y);
	//得到光标下方的MouseHint模型
	CModel* GetMouseHintModelUnderCursor(CSwapChain* pSwapChain, UINT x, UINT y);
	//光滑模型表面的顶点法线
	HRESULT SmoothModelNormal(CModel* pModel);
	//设置模型Billboard
	VOID SetModelBillboard(CModel* pModel);
	//获得碰撞模型和碰撞法线
	BOOL DetectCollision(D3DXVECTOR3* pOri, D3DXVECTOR3* pRay, 
						 CModel* pModel, D3DXVECTOR3* pNormal, 
						 FLOAT* distance);
	//获得Walker下方地面的碰撞模型和法线
	BOOL DetectGround(D3DXVECTOR3* pOri, D3DXVECTOR3* pRay, 
					  DWORD* pModelHandle, D3DXVECTOR3* pNormal, 
					  FLOAT* distance);
	
	//创建反射
	HRESULT CreateReflectEffect(CModel* pRefModel,UINT width, UINT height, float power);
    //设置反射物体
	VOID SetModelReflected(CModel* pRefModel, CModel* pRefedModel,BOOL checkFlag);

	// 获取场景根基点
	CSceneNode* GetRootSceneNode();

//-----------------------------------------------------------------------------
// 材质相关设定
//-----------------------------------------------------------------------------	
	//HRESULT CreateTransparentTexture(CTexture* pTex);
    //获得材质投影矩阵
	D3DXMATRIXA16 GetMatriceProjectionTexture();
	//构造材质的SubMesh列表
	void ConstructShaderSubMeshList();
	//将使用pMtrl材质的SubMesh规整到pMtrl->subMeshList中
	void PushSubMeshToShader(CMaterial* pMtrl);

	BOOL IsMaterialExist(const std::string &sMatName);
	CMaterial* CreateMaterial(const std::string &sMatName);
	VOID DeleteMaterialIfNoUse(CMaterial *pMaterial);

	UINT getMaterialCount();
	VOID getMaterialList(vector <CMaterial*> &vMaterial);
	CMaterial* GetMaterialByName(const std::string &sName);
	CMaterial* GetMaterialByIndex(INT index);
	INT getMaterialIndex(CMaterial* pMaterial);
	// add a meterial to list
	CMaterial* AddMaterial(VGS_MATERIAL_TYPE type, CMaterial *pMaterial);

	// 资源重命名
    std::string RenameMaterialIfExist(const std::string &sName);
	std::string RenameCameraIfExist(const std::string &sName);
	std::string RenameLightIfExist(const std::string &sName);
	std::string RenameModelIfExist(const std::string &sName);
	

//-----------------------------------------------------------------------------
// 贴图相关设定
//-----------------------------------------------------------------------------	
	//从Data缓冲创建贴图,数据默认的尺寸
	//HRESULT CreateTexFromBuffer(CTexture* pTex, INT mipLevel);
	//从Data缓冲创建贴图，指定尺寸
	//HRESULT	CreateTexFromBuffer(CTexture* pTex, UINT width, UINT height, INT mipLevel);
	//创建空贴图
	//HRESULT	CreateEmptyTex(CTexture* pTex, INT mipLevel);
	CTexture*	CreateEmptyTex(const std::string &sName, D3DFORMAT format, INT width, INT height, INT mipLevel);
	//从RGB缓冲创建贴图
	// HRESULT	CreateTexFromRGBBuffer(CTexture* pTex, INT mipLevel);
	//从文件创建贴图
	// HRESULT	CreateTexFromFile(CHAR* strFileName, CTexture* pTex, INT mipLevel);
	// 从文件创建一个Texture对象
	CTexture* CreateTexFromFile(const CHAR* strFileName, BOOL bOpacityOnly, const CHAR *strTextureName, INT mipLevel);

	// HRESULT	CreateOpacityTexFromFile(CHAR* strFileName, CTexture* pTex, INT mipLevel);
	
	//从内存创建动态贴图
	//HRESULT CreateDyTexFromFileInMemory(CTexture* pTex);

	//VOID CreateDyTexFrameImageBuffer();
    //创建cubemap
	//VOID CreateCubeMapBuffer();

	//设置贴图的亮度
	HRESULT SetTexBrightness(CTexture* pTex, UINT brightness);
	//得到CubeMap的贴图Maxtrix
	D3DXMATRIX GetCubeMapViewMatrix( DWORD dwFace );

	// texture相关
	UINT getTextureCount();
	CTexture* getTextureByIndex(INT index);
	CTexture* getTextureByName(const std::string &sName);

	// 根据索引获取图像
	UINT getImageCount();
	CImage* getImageByIndex(INT index);



	//创建SwapChain
	CSwapChain*  CreateSwapChain(HWND hWnd, D3DMULTISAMPLE_TYPE multiSampleType);

	// 准备渲染 
	VOID InitRenderSatusFromSwapchain(CSwapChain* pSwapChain);

	// 窗口缩放是调用
	HRESULT InvalidateScene();
	HRESULT RestoreScene();

public://GUI操作区
	CGUIManager          m_GUIManager;   // GUI对象管理器
	//获取GUI管理器引用--added by linxq 09-4-9
	inline CGUIManager& GetGuiManagerRef() { return m_GUIManager;}

public:
	// 控制对象相关
	IController *CreateWalker(const WalkerInfo &walkinfo);
	IController *CreateFlyer(const FlyerInfo &flyerinfo);
	IController *CreateEditor(const EditorInfo &editorinfo);
	UINT GetControlCount();
	UINT GetWalkerCount();
	UINT GetFlyerCount();
	UINT GetEditorCount();
	VOID AddWalker(IController *pWalker);
	VOID AddEditor(IController *pEditor);
	VOID AddFlyer(IController *pFlyer);
    IController* GetControl(UINT idx);
    IController* GetWalker(UINT idx);
    IController* GetFlyer(UINT idx);
    IController* GetEditor(UINT idx);
    IController* GetWalker(const string sName);
    IController* GetFlyer(const string sName);
    IController* GetEditor(const string sName);
	INT GetWalkerIndex(IController *pWalker);
	INT GetEditorIndex(IController *pEditor);
	INT GetFlyerIndex(IController *pFlyer);
	void DeleteWalker(const string &sName);
	void DeleteEditor(const string &sName);
	void DeleteFlyer(const string &sName);
	void DeleteWalker(DWORD handle);
	void DeleteEditor(DWORD handle);
	void DeleteFlyer(DWORD handle);	
	
	// 设置、获取当前控制器
	void SetCurrentController(IController* pController);
	IController *GetCurrentController();

	// 添加一个动态cubemap对象到列表
	VOID addDynamicCubemap(CCubeMap* pCubemap);
	// 添加一个动态bump map对象到列表
	VOID addBumpmap(CBumpMap* pBumpmap);

public:
	// 文件相关 
	// 导入s3d数据
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
	DWORD ImportS3DFile(const std::string &strS3DFileName, HWND hWnd);
	DWORD SaveV3DFile(const std::string &sFile, HWND hWnd);
	DWORD OpenV3DFile(const std::string &sFileName, HWND hWnd);
#endif

public:
	CScene(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pD3DDevice);
	~CScene();

private:

	// 控制对象列表
	IController *m_pCurController;   // 当前控制器
	std::vector<IController*> m_vWalker;
	std::vector<IController*> m_vEditor;
	std::vector<IController*> m_vFlyer;
	void DeleteAllController();

	// 水面对象
	vector<CRealWater*>		m_vRealWater;

	vector<CCubeMap*>       m_vpDynamicCubemap;  // 动态cubemap

	//渲染前的设置
	VOID StartRender();

	// 渲染特殊材质
	VOID RenderadditionalTexture();
	
	//渲染backdrop
	VOID RenderBackDrop();
	
	//渲染模型
	VOID RenderModels();
	
	//结束本帧的渲染
	VOID EndRender();
	
	//渲染其它的对象
	VOID RenderOthers();

	VOID ClearAllAdditionalSurface();  // 清空场景中所有的辅助渲染表面

	VOID UpdateAllAdditionalSurface();	//更新所有辅助渲染表面

	//渲染SubMesh到实时贴图表面
	VOID RenderSubMeshToRTFTexture(CRTFMap* pRTFMap, VGS_RenderSubmesh* pSubMesh/*, CMaterial* pMtrl*/);
	//渲染一个模型到实时贴图表面----linxq-当前用于测试
	void RenderMeshToRTFTexture(CRTFMap* pRTFMap);
	//渲染SubMesh到凹凸水面混合贴图表面
	VOID RenderSubMeshToBumpWaveCompositeTexture(CBumpMap* pBumpMap, VGS_RenderSubmesh* pSubMesh, CMaterial* pMtrl);
	VOID RenderModelToDynamicCubeMapSurface(CModel* pModel, VGS_RenderSubmesh* pSubMesh, CCubeMap* pCubeMap);

	//渲染场景中的真实水面
	VOID RenderRealWater();

	//渲染使用当前材质的模型
	VOID RenderModelsByMtrl(/*CMaterial* pMtrl*/);

	//渲染一个制定物体
	VOID RenderOneModel(CModel* pModel);

	//清除所有动态CubeMap的表面
	VOID ClearDynamicCubeMapSurface();
	//切换CubeMap的表面
	VOID SwtichDynamicCubeMapSurface();

private:

	// 将submesh渲染到bump texture
	VOID RenderSubmeshToBumptexture(CBumpMap* pBumpmap, const vector<VGS_RenderSubmesh> &vSubmeshList);

public:
// 	DWORD LoadSceneFromBuffer(DWORD version, const BYTE* pBuffer, HWND hWnd);
	// 	DWORD LoadImageDataFromBuffer(DWORD version, const BYTE* pBuffer, HWND hWnd);

	D3DXMATRIX GetTextureProjMatrix();  //计算RenderToTexture的二次渲染贴图的投影矩阵

	VOID   SetRoot(CRoot *pRoot);


public:
	CA3DManager m_A3DManager;  // 骨骼动画文件管理器


};

#endif