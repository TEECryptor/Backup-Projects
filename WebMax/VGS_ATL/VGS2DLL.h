//------------------------------------------------------------------------
//VGS2DLL.h CVGS2DLL类头文件
//描述：VGS2版本引擎API方法类
//作者: lixin
//日期: 2006 -3 -31
//------------------------------------------------------------------------

#ifndef _VGS2DLL_H
#define _VGS2DLL_H

#include <stdio.h>
#include <windows.h>

#include "VGSATLHeader.h"
#include "..\\VGSCoreV3\\vgscore.h"
//#include "VGSCore\\vgscore.h"

// #include "VGSSingelton.h"
#include "CInput.h"

using namespace std;
// 
// //-------------------------------------------------------------------------------------------
// //***各种枚举类型定义
// //-------------------------------------------------------------------------------------------
// enum CONTROLSTATUS
// {
// 	WALK,			//预览状态，虚拟漫游中常用的方法
// 	EDIT,			//编辑状态, 物件展示中常用的方法
// 	FLY,			//飞行状态，GIS模式
// 	NONE			//非交互状态
// };
// 
// //D3D FORMAT
// enum TEXFORMAT
// {
// 	RGBA5650,
// 	RGBA5551,
// 	RGBA4444,
// 	RGBA8880,
// 	RGBA8888
// };
// 
// //对齐模式
// enum ALIGNMODE
// {
// 	ALIGN_NONE,
// 	ALIGN_LEFT,
// 	ALIGN_RIGHT,
// 	ALIGN_BOTTOM,
// 	ALIGN_TOP,
// 	ALIGN_LEFT_TOP,
// 	ALIGN_MIDDLE_TOP,
// 	ALIGN_RIGHT_TOP,
// 	ALIGN_LEFT_BOTTOM,
// 	ALIGN_MIDDLE_BOTTOM,
// 	ALIGN_RIGHT_BOTTOM,
// 	ALIGN_CENTER
// };
// 
// //-------------------------------------------------------------------------------------------
// //***矢量定义
// //-------------------------------------------------------------------------------------------
// typedef struct VECTOR2
// {
// 	FLOAT x;
// 	FLOAT y;
// }VECTOR2;
// 
// typedef struct VECTOR3
// {
// 	FLOAT x;
// 	FLOAT y;
// 	FLOAT z;
// }VECTOR3;
// 
// 
// //-------------------------------------------------------------------------------------------
// //***颜色定义
// //-------------------------------------------------------------------------------------------
// typedef struct RGB
// {
// 	BYTE red;
// 	BYTE green;
// 	BYTE blue;
// }RGB;
// 
// typedef struct RGBA
// {
// 	BYTE red;
// 	BYTE green;
// 	BYTE blue;
// 	BYTE alpha;
// }RGBA;
// 
// //定义包围球信息结构
// typedef struct BoundSphere
// {
// 	VECTOR3 vCenter; //中心点
// 	FLOAT   fRadius; //半径
// }BoundSphere;
// 
// //定义包围和数据结构
// typedef struct BoundBox
// {
// 	VECTOR3 vMin; //左下角
// 	VECTOR3 vMax; //右上角
// }BoundBox;
// 
// //定义摄像机属性的数据结构
// typedef struct CameraInfo
// {
// 	VECTOR3 from;			//摄像机的起点
// 	VECTOR3 at;				//摄像机的终点	
// 	FLOAT	roll;			//摄像机的roll的角度
// 	FLOAT	nearClip;		//摄像机的nearClip
// 	FLOAT   farClip;		//摄像机的farClip
// 	FLOAT	fov;			//摄像机的fov
// }CameraInfo;
// 
// 
// //定义灯光属性的数据结构
// typedef struct LightInfo
// {
//     UINT			Type;
//     RGB				Diffuse;
//     RGB				Ambient;
//     FLOAT			Multiply;
//     VECTOR3			Position;
//     VECTOR3			Direction;
//     FLOAT           Range;
//     FLOAT           Falloff;
//     FLOAT           Attenuation0;
//     FLOAT           Attenuation1;
//     FLOAT           Attenuation2;
//     FLOAT           Theta;
//     FLOAT           Phi;
// }LightInfo;
// 
// //定义材质属性的数据结构
// typedef struct MtrlInfo
// {
// 	std::string			Name;		//材质名
// 	RGB				Ambient;		//环境色
//     RGB				Diffuse;		//漫色
// 	RGB				Specular;		//高光颜色
// 	FLOAT			SpecularLevel;	//高光强度
// 	FLOAT			SpecularPower;	//高亮锐度
// 	RGB				Emissive;		//自发光颜色
// 	INT				Opacity;		//透明度
// 	BOOL			bTwoSide;		//是否双面
// 	BOOL			bAlphaTest;		//是否进行Alpha测试
// }MtrlInfo;

class CVGS2DLL //: public Singleton <CVGS2DLL>
{

public:

	CVGSCore         m_VGSCore;  // vgs渲染器
	//-------------------------------------------------------------------------------------------
	//***系统变量
	//-------------------------------------------------------------------------------------------
	DWORD				m_vgsScene;					// 场景
	DWORD               m_RenderTarget;             // 当前渲染目标
	DWORD               m_Viewport;                 // 当前的视口
	vector<DWORD>	sceneResource;				//所有的场景(1个N3D文件对应一个场景)资源对象
	FLOAT				fAxisScale;					//缺省轴缩放值
	CONTROLTYPE		controlStatus;				//当前用户控制状态值

	BOOL                m_bRender;  // 是否渲染场景

	//-------------------------------------------------------------------------------------------
	//时钟相关变量
	//-------------------------------------------------------------------------------------------
	UINT				lastFrameTimer;						//上一帧渲染结束的时间
	BOOL				bShowDebugInfo;						//是否显示调试信息

	//-------------------------------------------------------------------------------------------
	//碰撞控制相关变量
	//-------------------------------------------------------------------------------------------
	BOOL				bCollision;							//碰撞是否有效
	BOOL				bIsCollision;						//当前是否有碰撞发生
	BOOL				bWeight;							//重力是否有效
	FLOAT				colDistance;						//碰撞距离

// 	CModel*				curColModel;						//当前发生碰撞的模型
// 	CModel*				curGroundModel;						//当前地面模型
// 	CModel*             preGroundModel;                     //之前的地面模型

	//-------------------------------------------------------------------------------------------
	//WALK模式变量
	//-------------------------------------------------------------------------------------------
// 	CWalker				walker;								//walker摄像机对象
// 	D3DXVECTOR3			walker_realMoveVec;					//walker的实际移动速度
// 	D3DXVECTOR3			walker_realRotateVec;				//walker的实际旋转速度
// 	FLOAT				moveStep;
// 	FLOAT				rosDis;


	//-------------------------------------------------------------------------------------------
	//EDIT模式变量
	//-------------------------------------------------------------------------------------------
// 	CEditor				editor;								//editor摄像机对象
// 	INT					mouseX;
// 	INT					mouseY;
// 
// 	//-------------------------------------------------------------------------------------------
// 	//FLY模式变量
// 	//-------------------------------------------------------------------------------------------
// 	CFlyer				flyer;								//飞行对象
// 
// 	//-------------------------------------------------------------------------------------------
// 	//外部控制相机帧移动到目标点的方法相关变量
// 	//-------------------------------------------------------------------------------------------
// 	BOOL				bFrameMoveCamera;					//渲染帧中是否移动相机
// 	D3DXVECTOR3			FrameMoveCamera_From;				//帧移动相机的目标From
// 	D3DXVECTOR3			FrameMoveCamera_At;					//帧移动相机的目标At
// 	INT					FrameMoveCamera_FrameCnt;			//移动相机的总帧数
// 	INT					FrameMoveCamera_FrameIndex;			//移动相机的当前帧
// 	INT					FrameMoveCamera_AccMode;			//加速模式
// 	D3DXVECTOR3			FrameMoveCamera_FromAcc;			//From点加速度
// 	D3DXVECTOR3			FrameMoveCamera_FromVel;			//From点速度
// 	D3DXVECTOR3			FrameMoveCamera_AtAcc;				//At点加速度
// 	D3DXVECTOR3			FrameMoveCamera_AtVel;				//At点速度
// 
// 	//-------------------------------------------------------------------------
// 	//相机控制的状态变量
// 	//-------------------------------------------------------------------------
// 	BOOL				bCanZoom;							//Home和End键作用
// 	BOOL				bCanMove;							//ASWD和方向键作用
// 	BOOL				bCanLift;							//PageUp和PageDown作用
// 	BOOL				bCanRos;							//QERF和鼠标旋转作用
// 
// 	FLOAT				fZoomSpeed;							//调用PlayerZoomIn和PlayerZoomOut方法的缩放速度
public:
	//-------------------------------------------------------------------------------------------
	//***引擎初始化相关的方法
	//-------------------------------------------------------------------------------------------
	// 初始化引擎
	BOOL SunInitVGS(HWND hWnd);

	VOID SunRelease();

	// 初始化缺省的渲染窗口
	BOOL InitDefaultRenderForm(HWND hWnd);

	// 获得缺省使用的视口
	DWORD SunGetDefaultViewport();

	// 初始化GUI状态
	VOID SunInitGUIStatus();

	//创建VGS场景
	DWORD SunCreateScene(/*HWND hWnd, RECT* rect*/);
	//清除VGS场景
	VOID SunDeleteScene();
	//Reset VGS场景
	VOID SunResetScene();
	//设置场景的背景色
	VOID SunSetBackground(VGSRGB* color);
	//创建Logo
	BOOL CreateLogo();
	//删除Logo
	VOID DeleteLogo();
	//Invalidate Scene
	VOID InvalidateScene();
	//Restore Scene
	VOID RestoreScene();
	//设置抗拒齿的级别
	VOID SetAntiAliasLevel(INT level);

	//--------------------------------------------------------------------------------------------
	//场景资源对象相关的方法，场景资源对象在LoadN3D文件时自动创建，一个N3D文件对应一个场景资源对象
	//--------------------------------------------------------------------------------------------
	//创建场景资源对象
// 	CSceneResource* CreateSceneResource(std::string sceneResourceName, D3DXMATRIX* pTransform);
// 	//删除指定场景资源对象，也删除了资源本身
// 	void	DeleteSceneResource(CSceneResource* pSceneResource);
// 	//找到指定的场景资源对象
// 	CSceneResource* GetSceneResource(std::string sceneResourceName);
// 
// 	//克隆指定的场景
// 	CSceneResource* CloneScene(std::string destSceneName, 
// 								std::string srcSceneName, 
// 								std::string parentSceneName, 
// 								D3DXMATRIX* pTransformMat);
// 

	//-------------------------------------------------------------------------------------------
	//***模型相关的方法
	//-------------------------------------------------------------------------------------------
	//得到场景中模型的数量
	UINT SunGetModelCount();
	//根据模型的句柄得到模型的名称
	VOID SunGetModelNameByHandle(DWORD modelHandle, CHAR* strModelName);
	//根据模型的名称得到模型的句柄
	DWORD SunGetModelHandleByName(const CHAR* strModelName);
	//根据模型的编号得到模型的句柄
	DWORD SunGetModelHandleByIndex(UINT index);
	//设置模型的可见性
	VOID SunSetModelVisibility(DWORD modelHandle, BOOL visible);
	//得到模型的可见性
	BOOL SunGetModelVisibility(DWORD modelHandle);
	//删除场景中的模型
	VOID SunDeleteModelByHandle(DWORD modelHandle);
	
	/** 移动模型的位置
	@param in modelHandle   : 相机句柄
	@param in pVec          ：移动的相对向量
	@param in flag          ：移动的参考坐标系
							   0 - 在世界坐标系平移
							   1 - 在自身坐标系平移
	*/
	VOID SunMoveModel(DWORD modelHandle, VECTOR3* pVec, UINT flag);

	//移动指定的模型到某个位置
	//acctype 0-匀速运动，1-匀加速运动, 2-匀减速运动
	VOID MoveModelTo(DWORD modelHandle, VECTOR3* pVec, INT frameCnt, INT accType);

	//得到模型的位置
	VOID SunGetModelPosition(DWORD modelHandle, VECTOR3* pVec);
	//设置模型的位置
	VOID SunSetModelPosition(DWORD modelHandle, VECTOR3* pVec);
	//旋转模型, flag = 0-> 绕穿过自身Privot并与世界坐标系平行的坐标系旋转, flag =1-> 自身坐标系, flag =2->父坐标系
	VOID SunRotateModel(DWORD modelHandle, VECTOR3* pVec, UINT flag);
	//模型绕指定的轴旋转
	VOID SunRotateModelByAxis(DWORD modelHandle, VECTOR3* pPos, VECTOR3* pVec, FLOAT fAngle);
	//得到模型的转角
	VOID SunGetModelRotation(DWORD modelHandle, VECTOR3* pRot);
	//设置模型的转角
	VOID SunSetModelRotation(DWORD modelHandle, VECTOR3* pRot);
	//缩放模型, flag = 0-> GCS , flag =1->LCS
	VOID SunScaleModel(DWORD modelHandle, VECTOR3* pScale, UINT flag);
	//以固定点为中心缩放模型
	VOID SunScaleModelByPoint(DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale);
	//得到模型的缩放
	VOID SunGetModelScale(DWORD modelHandle, VECTOR3* pScale);
	//设置模型缩放
	VOID SunSetModelScale(DWORD modelHandle, VECTOR3* pScale);
	//得到光标下方的模型句柄
	DWORD SunGetModelUnderLoc(UINT x, UINT y);
	//得到光标下方的有鼠标响应的模型句柄
	DWORD SunGetMouseHintModelUnderLoc(UINT x, UINT y);
	//设置模型的引用模型
	VOID SunSetModelReference(DWORD curModelHandle, DWORD refModelHandle);
	//重新计算模型的法线，让模型看起来非常光滑Smooth,计算量较大
	VOID SunSmoothModelNormal(DWORD modelHandle); 
	//设置模型正对Billboard状态
	VOID SunSetBillBoardStatus(DWORD modelHandle, BOOL status);
	//设置模型正对Billboard的Up向量
	VOID SunSetBillboardUp(DWORD modelHandle, VECTOR3* pUp);
	//设置场景碰撞的有效状态
	VOID SunEnableCollision(BOOL status);
	//设置碰撞发生的有效距离, 默认为1.0
	VOID SunSetColDistance(FLOAT distance);
	//设置模型的碰撞
	VOID SunSetCollisionModel(DWORD modelHandle, BOOL status);
	//设置重力有效状态
	VOID SunEnableWeight(BOOL status);
	//设置模型为地面模型
	VOID SunSetGroundModel(DWORD modelHandle, BOOL status);
	//转动模型Diffuse贴图的UV
	VOID SunMoveModelUV(DWORD modelHandle, FLOAT u, FLOAT v);
	//设置模型是否使用自动转动UV
	VOID SunSetDynamicUVStatus(DWORD modelHandle, BOOL status);
	//设置模型的自动转动UV的参数
	VOID SunSetDynamicUVProp(DWORD modelHandle, FLOAT UStep, FLOAT VStep, FLOAT speed);
	//设置模型是否可以点击
	VOID SunSetModelClickable(DWORD modelHandle, BOOL status);

	//设置模型的opacity
	VOID SunSetModelOpacity(DWORD modelHandle, UINT opacity);
	//设置模型的顶点颜色
	VOID SunSetModelColor(DWORD modelHandle, BYTE red, BYTE green, BYTE blue, BYTE alpha);

	//-------------------------------------------------------------------------------------------
	//***材质相关的方法
	//-------------------------------------------------------------------------------------------
	//得到场景中材质的数量
	UINT SunGetMaterialCount();
	//根据材质的句柄得到材质的名称
	VOID SunGetMaterialNameByHandle(DWORD mtrlHandle, CHAR* strMtrlName);
	//根据材质的名称得到材质的句柄
	DWORD SunGetMaterialHandleByName(const CHAR* strMtrlName);
	//根据材质的编号得到材质的句柄
	DWORD SunGetMaterialHandleByIndex(UINT index);
	//得到材质的Diffuse贴图
// 	DWORD SunGetDifTexture(DWORD mtrlHandle);
// 	//设置材质的Diffuse贴图
// 	VOID SunSetDifTexture(DWORD mtrlHandle, DWORD texHandle);
// 
// 	//得到材质的Opacity贴图
// 	DWORD SunGetOpacityTexture(DWORD mtrlHandle);
// 	//设置材质的Opacity贴图
// 	VOID SunSetOpacityTexture(DWORD mtrlHandle, DWORD texHandle);
// 	//得到材质的Lightmap贴图
// 	DWORD SunGetLightTexture(DWORD mtrlHandle);
// 	//设置材质的Lightmap贴图
// 	VOID SunSetLightTexture(DWORD mtrlHandle, DWORD texHandle);
// 	//得到材质的Reflection反射贴图
// 	DWORD SunGetRefTexture(DWORD mtrlHandle);
// 	//设置材质的Reflection反射贴图
// 	VOID	SunSetRefTexture(DWORD mtrlHandle, DWORD texHandle);
// 	//设置材质的反射贴图的呈现程度
// 	VOID SunSetRefLevel(DWORD mtrlHandle, UINT level);
	//创建材质
	DWORD SunCreateEmptyMtrl(CHAR* strMtrlName);
	//删除材质
	VOID SunDeleteMtrl(DWORD mtrlHandle);
	//通过模型得到材质
	DWORD SunGetMtrlByModel(DWORD modelHandle);
	//设置某个模型的材质
	VOID SunSetModelMtrl(DWORD modelHandle, INT subMeshID, DWORD mtrlHandle);
	//设置材质的opacity
	VOID SetMtrlOpacity(DWORD mtrlHandle, UINT opacity);
	//设置材质的颜色
	VOID SunSetMtrlColor(DWORD mtrlHandle, INT red, INT green, INT blue, INT alpha);

	//设置模型为SkyBox模型
	VOID SunSetSkyBox(DWORD modelHandle, BOOL status);
	//得到材质的CubeMap
	DWORD SunGetCubeMap(DWORD mtrlHandle);
	//设置材质的CubeMap
	VOID SunSetMtrlCubeMap(DWORD mtrllHandle, DWORD cubeMapHandle, FLOAT power);
	//设置材质的高光CubeMap
	VOID SetMtrlSpecularCubeMap(DWORD mtrlHandle, DWORD cubeMapHandle);
	//通过CubeMap的名称得到CubeMap
	DWORD GetCubeMapHandleByName(CHAR* cubeMapName);
	//通过CubeMap的index得到CubeMap
	DWORD GetCubeMapHandleByIndex(INT index);
	
	//获得材质信息
	VOID SunGetMtrlInfo(DWORD mtrlHandle, MtrlInfo* pMtrlInfo);
	//设置材质信息
	VOID SunSetMtrlInfo(DWORD mtrlhandle, MtrlInfo* pMtrlInfo);


	//创建Bump贴图
	DWORD CreateBumpTexture(CHAR* name, VGS_BumpType type, INT width, INT height, INT xSize, INT ySize, 
							FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY);
	//删除BumpWater
	VOID DeleteBumpTexture(DWORD bumpWaterHandle);
	//通过Bumpwater的名字获得指针
	DWORD	GetBumpTextureHandleByName(CHAR* strName);

	//设置材质的某层贴图
	VOID SetMtrlTexture(CHAR* mtrlName, INT layerIndex, VGS_TextureType type, CHAR* textureName);

	//-------------------------------------------------------------------------------------------
	//***贴图相关的方法
	//-------------------------------------------------------------------------------------------
	//得到场景中贴图的数量
	UINT SunGetTextureCount();
	//根据贴图的句柄得到贴图的名称
	VOID SunGetTextureNameByHandle(DWORD texHandle, CHAR* strTexName);
	//根据贴图的名称得到贴图的句柄
	DWORD SunGetTextureHandleByName(CHAR* strTexName);
	//根据贴图的编号得到贴图的句柄
	DWORD SunGetTextureHandleByIndex(UINT index);

	//Add by Baojiahui 2008/12/8-----------------------------------------------------------------
	//-------------------------------------------------------------------------------------------

	//从本地文件创建贴图
	DWORD SunCreateTexture(CHAR* texName, CHAR* strImageFile, TEXFORMAT format);
	//从Image对象创建贴图
	DWORD SunCreateTextureFromImage(CHAR* texName, CHAR* strImageName, TEXFORMAT format, INT mipLevel);
	// 从Image buffer更新使用该Image的所有texture
	VOID SunFillTexturesFromBuffer(UINT len, CHAR* pBuffer, INT imgIndex);
	//删除贴图
	VOID SunDeleteTexture(DWORD texHandle);
	//设置贴图的FORMAT
	VOID SunSetTextureFormat(DWORD texHandle, TEXFORMAT format);
	//判断贴图是否被场景所用到
	BOOL IsTextureInScene(DWORD pTex);
	//判断CubeMap对象是否被场景用到
	BOOL IsCubeMapInScene(DWORD pCubeMap);
	//判断动态贴图对象是否被场景用到
	BOOL IsDyTexInScene(DWORD pDyTex);
	//判断电影贴图是否被场景用到
	BOOL IsMovieTexInScene(DWORD pMovieTex);
	//删除动态贴图
	VOID DeleteDynamicTexture(DWORD dynamicTexHandle);
	//-------------------------------------------------------------------------------------------
	//***摄像机相关的方法
	//-------------------------------------------------------------------------------------------
	//得到场景中摄像机的数量
	UINT SunGetCameraCount();
	//根据像机的句柄得到像机的名称
	VOID SunGetCameraNameByHandle(DWORD camHandle, CHAR* strCameraName);
	//根据像机的名称得到像机的句柄
	DWORD SunGetCameraHandleByName(CHAR* strCameraName);
	//根据摄像机的编号得到像机的句柄
	DWORD SunGetCameraHandleByIndex(UINT index);
	//设置当前摄像机
	VOID SunSetCurrentCamera(DWORD camHandle);
	//得到当前摄像机句柄
	DWORD SunGetCurrentCamera();

	/** 相机目标点不动，沿相机方向前后推拉相机。
	@param in sceneHandle : 场景指针
	@param in camHandle : 相机指针
	@param dollyValue  in   : 相机推拉的距离，> 0 向前， < 1向后
	*/
	VOID SunDollyCamera(DWORD handle, FLOAT value);

	//平移摄像机
	VOID SunMoveCamera(DWORD camHandle, VECTOR3* pVec, UINT coordEnum);
	//旋转摄像机
	VOID SunRotateCamera(DWORD camHandle, VECTOR3* pVec, UINT coordEnum);
	//以目标点为中心，以pAxisVec为轴，旋转From点fAngle弧度
	VOID SunRoundCamera(DWORD camHandle, VECTOR3* pAxisVec, FLOAT fAngle); 
	//摄像机正对
	VOID SunSetCameraLookAt(DWORD camHandle, DWORD modelHandle);
	
	//创建像机
	DWORD SunCreateCamera(const CHAR* name, D3DXVECTOR3* pFrom, D3DXVECTOR3* pAt);
	// 创建一个缺省相机
	DWORD SunCreatePredefinedCamera(DEFAULTCAMERA_TYPE type);

	//设置摄像机的起始点
	VOID SunSetCameraFrom(DWORD camHandle, VECTOR3* pPos);
	//设置摄像机的目标点
	VOID SunSetCameraAt(DWORD camHandle, VECTOR3* pPos);
	VOID SunGetCameraAt(DWORD camHandle, VECTOR3* pPos);
	//设置摄像机的FOV
	VOID SunSetCameraFov(DWORD camHandle, FLOAT fov);
	//设置摄像机的nearClip
	VOID SunSetCameraNearClip(DWORD camHandle, FLOAT nearClip);
	//设置摄像机的farClip
	VOID SunSetCameraFarClip(DWORD camHandle, FLOAT farClip);
	//设置摄像机的Roll
	VOID SunSetCameraRoll(DWORD camHandle, FLOAT rollAngle);
	//一次性设置摄像机的属性
	VOID SunSetCameraInfo(DWORD camHandle, CameraInfo* pCamInfo);
	//播放摄像机关键帧动画
	VOID SunPlayCameraKeyFrame(DWORD camHandle);
	//暂停摄像机关键帧动画
	VOID SunPauseCameraKeyFrame(DWORD camHandle);
	//回退摄像机关键帧动画
	VOID SunStopCameraKeyFrame(DWORD camHandle);
	//调到摄像机动画的某一帧
	VOID SunGotoCameraFrame(DWORD camHandle, INT frame);

	//循环调用方法, 需要在帧循环中调用。
	//frameCnt为到达目标点的总帧数，accType为运动过程中的加速方法
	VOID MoveCurrentCameraTo(D3DXVECTOR3* vFrom, D3DXVECTOR3* vAt, INT frameCnt, BOOL accType);
	//渲染帧自动调用的移动相机的方法，每帧调用一次(内部调用)
	VOID FrameMoveCurrentCamera();

	//-------------------------------------------------------------------------------------------
	//***关键帧动画相关的方法
	//-------------------------------------------------------------------------------------------
	//播放关键帧动画
	VOID PlayKeyframe();
	//暂停关键帧动画
	VOID PauseKeyframe();
	//停止关键帧动画
	VOID StopKeyframe();

	//得到关键帧动画的帧数
	INT GetKeyframeCount();
	//跳到某一帧
	VOID GotoFrame(INT index);
	//得到当前播放的帧
	INT GetCurrentFrame();

	//设置关键帧动画播放的帧率
	VOID SetKfrPlaySpeed(INT index);
	//获得关键帧动画播放的帧率
	INT GetKfrPlaySpeed(); 
	//设置关键帧动画的播放次数
	VOID SetKfrRepeatCount(INT index);

	//-------------------------------------------------------------------------------------------
	//***灯光相关的方法
	//-------------------------------------------------------------------------------------------
	//得到场景中灯光的数量
	UINT SunGetLightCount();

	//根据灯光的名称得到灯光的句柄
	DWORD SunGetLightHandleByName(CHAR* strLightName);
	//根据灯光的编号得到灯光的句柄
	DWORD SunGetLightHandleByIndex(UINT index);

	VOID	ResetLightIndex();
	//根据灯光的句柄得到灯光的名称
	VOID SunGetLightNameByHandle(DWORD lightHandle, CHAR* strLightName);
	//得到灯光的编号
	INT SunGetLightIndexByHandle(DWORD lightHandle);

	//设置灯光变是否有效
	VOID SunEnableLight(DWORD lightHandle, BOOL isEnalbe);
	//移动灯光
	VOID SunMoveLight(DWORD lightHandle, VECTOR3* pVec);
	//旋转灯光
	VOID SunRotateLight(DWORD lightHandle, VECTOR3* pRos);
	//设置灯光的位置
	VOID SunSetLightPosition(DWORD lightHandle, VECTOR3* pPos);
	//设置灯光的方向
	VOID SunSetLightDirection(DWORD lightHandle, VECTOR3* pDir);
	//得到灯光的属性
	VOID SunGetLightInfo(DWORD lightHandle, LightInfo* pLightInfo);
	//设置灯光的属性
	VOID SunSetLightInfo(DWORD lightHandle, LightInfo* pLightInfo);
	/** 设置灯光是否有效
	*/
	VOID SunSetLightEnableStatus(DWORD lightHandle, BOOL bEnable);

	//设置是否播放灯光的关键帧动画
	VOID SetLightKfrPlayStatus(DWORD lightHandle, BOOL bFlag);
	VOID SetAllLightKfrPlayStatus(BOOL bFlag);

	//设置灯光关键帧动画的播放次数
	VOID SetLightKfrRepeatCnt(DWORD lightHandle, INT repeatCnt);
	VOID SetAllLightKfrRepeatCnt(INT repeatCnt);

	//-------------------------------------------------------------------------------------------
	//***文字相关的方法
	//-------------------------------------------------------------------------------------------
	//新建文字对象
	DWORD SunNewText(CHAR* strText);
	//删除文字对象
	VOID SunDelText(DWORD textHandle);
	//得到场景中文字对象的数量
	UINT SunGetTextCount();
	//通过文字对象的编号得到文字对象的句柄
	DWORD SunGetTextHandleByIndex(UINT index);
	//设置文字的颜色
	VOID SunSetTextColor(DWORD textHandle, DWORD color);
	//设置文字对象的位置
	VOID SunSetTextPosition(DWORD textHandle, UINT x, UINT y);
	//设置文字的内容
	VOID SunSetTextString(DWORD textHandle, CHAR* strText);


	//-------------------------------------------------------------------------------------------
	//***Overlay相关的方法
	//-------------------------------------------------------------------------------------------
	DWORD CreateOverlayFromMemory(RECT* pRect, const CHAR* overlayName, const CHAR* overlayTexName, const BYTE* pData ,UINT dataSize);
	//新建 Overlay, name为Overlay的名称， strOriImageFile为原图，strAlphaImageFile为Alpha贴图, alignMode为对齐模式， pRect为Overlay的Rect
	DWORD SunNewOverlay(CHAR* name, CHAR* strOriImageFile, CHAR* strAlphaImageFile, ALIGNMODE alignMode, RECT* pRect);
	//从Buffer创建 Overlay
	DWORD SunNewOverlayFromBuf(CHAR* name, BYTE* pOriBuf, DWORD oriDataSize, BYTE* pAlphaBuf, DWORD alphaDataSize, ALIGNMODE alignMode, RECT* pRect);
	//从Image对象新建Overlay
	DWORD SunNewOverlayFromImage(CHAR* overlayName, CHAR* pOriImageName, CHAR* pAlphaImageName, ALIGNMODE alignMode, RECT* pRect);
	//删除 Overlay
	VOID SunDelOverlay(DWORD overlayHandle);

	//通过名称得到Ovelay的句柄
	DWORD SunGetOverlayHandleByName(CHAR* strName);
	//通过编号得到Overlay的句柄
	DWORD SunGetOverlayHandleByIndex(INT index);
	//得到Overlay的index
	INT SunGetOverlayIndex(DWORD overlayHandle);
	//得到Overlay的名字
	VOID SunGetOverlayName(DWORD overlayHandle, CHAR* strName);
	//得到 Overlay 的文件名
	VOID SunGetOverlayImage(DWORD overlayHandle, CHAR* strImageFile);
	//设置 Overlay 的文件名
	VOID SunSetOverlayImage(DWORD overlayHandle, CHAR* strImageFile);
	//得到 Overlay 的Rect
	VOID SunGetOverlayRect(DWORD overlayHandle, RECT* rect);
	//设置 Overlay 的Rect
	VOID SunSetOverlayRect(DWORD overlayHandle, RECT* rect);
	//得到 Overlay 的透明度
	UINT SunGetOverlayOpacity(DWORD overlayHandle); 
	//设置 Overlay 的透明度
	VOID SunSetOverlayOpacity(DWORD overlayHandle, UINT opacity);
	//得到 Overlay 的渲染模式
	VOID SunGetOverlayFormat(DWORD overlayHandle, UINT enumStatus);
	//设置 Overlay 的渲染模式
	UINT SunSetOverlayFormat(DWORD overlayHandle);
	//移动 Overlay
	VOID SunMoveOverlay(DWORD overlayHandle, INT x, INT y);
	//旋转 Overlay
	VOID SunRotateOverlay(DWORD overlayHandle, FLOAT angle);
	//得到 Overlay旋转的角度
	FLOAT SunGetOverlayRotation(DWORD overlayHandle);
	//设置 Overlay旋转的角度
	VOID SunSetOverlayRotation(DWORD overlayHandle, FLOAT angle);
	//得到 鼠标点击的Overlay
	DWORD SunGetOverlayUnderLoc(INT x, INT y);
	//设置Overlay的对齐模式
	VOID SetOverlayAlignMode(DWORD overlayHandle, INT alignMode);
	
	//将Overlay移到下一层[Overlay有前后关系，后面的会被前面的挡住]
	VOID PushOverlayBack(DWORD overlayHandle);
	//将Overlay上移一层
	VOID PushOverlayFront(DWORD overlayHandle);

	//-------------------------------------------------------------------------------------------
	//***Button相关的方法, 新建的Button对象将作为资源保存在E3D文件中，可以动态调用
	//-------------------------------------------------------------------------------------------
	//从本地文件创建Button
	//name-Button的名称
	//mouseUpImageFile-鼠标松开的源图像文件
	//mouseUpAlphaImageFile-鼠标松开的Alpha图像文件
	//mouseOverImageFile-鼠标移上的源图像文件
	//mouseOverAlphaImageFile-鼠标移上的Alpha图像文件
	//mouseDownImageFile-鼠标按下的源图像文件
	//mouseDownAlphaImageFile-鼠标按下的Alpha图像文件
	//alignMode-对齐模式，当为NONE时pButtonRect->top和pButtonRect->top自动失效
	//pButtonRect-按钮的Rect
	//callBackFunc-按下按钮的回调函数
	DWORD SunNewButton(CHAR* name, 
								 CHAR* mouseUpImageFile, CHAR* mouseUpAlphaImageFile,
								 CHAR* mouseOverImageFile, CHAR* mouseOverAlphaImageFile,
								 CHAR* mouseDownImageFile, CHAR* mouseDownAlphaImageFile,
								 ALIGNMODE alignMode,
								 RECT* pButtonRect,
								 CHAR* callBackFunc);

	//从Image对象创建Button
	//name-Button的名称
	//mouseUpImage-鼠标松开的源图像对象
	//mouseUpImage_alpha-鼠标松开的Alpha图像对象
	//mouseOverImage-鼠标移上的源图像对象
	//mouseOverImage_alpha-鼠标移上的Alpha图像对象
	//mouseDownImage-鼠标按下的源图像对象
	//mouseDownImage_alpha-鼠标按下的Alpha图像对象
	//ALIGNMODE alignMode-对齐模式
	//pButtonRect-Button的Rect，当对齐align模式设置为相应模式时，相应参数是小
	DWORD SunNewButtonFromImage(CHAR* name, 
										  CHAR* mouseUpImage, CHAR* mouseUpImage_alpha,
										  CHAR* mouseOverImage, CHAR* mouseOverImage_alpha,
										  CHAR* mouseDownImage, CHAR* mouseDownImage_alpha,
										  ALIGNMODE alignMode,
										  RECT* pButtonRect,
										  CHAR* callBackFunc);

	//通过名称得到Button
	DWORD SunGetButtonByName(CHAR* name);
	//通过编号得到Button
	DWORD SunGetButtonByIndex(UINT index);
	//删除按钮
	VOID SunDeleteButton(DWORD buttonHandle);

	//-------------------------------------------------------------------------------------------
	//***Image相关的方法, 新建的Image对象将作为资源保存在E3D文件中，可以动态调用
	//-------------------------------------------------------------------------------------------
	//新建Image资源
	DWORD SunNewImage(CHAR* name, CHAR* fileName);
	//通过编号得到Image对象
	DWORD SunGetImageHandleByIndex(INT index);
	//通过图像名称得到Image对象
	DWORD SunGetImageHandleByName(CHAR* name);
	//通过图像编号删除Image对象
	VOID SunDeleteImageByIndex(INT index);
	//通过图像名称删除Image资源
	VOID SunDeleteImageByName(CHAR* name);


	//-------------------------------------------------------------------------------------------
	//***导航图相关的方法
	//-------------------------------------------------------------------------------------------
	//删除Navigater
	VOID SunDeleteNavigater(DWORD navigterHandle);
	//通过Navigater的名称得到句柄
	DWORD SunGetNavigaterByName(CHAR* name);
	
	//增加导航指针对象
	//navigaterHandle - 导航图对象的句柄
	//navPointerName - 导航图指针对象的名称
	//pointerType - 导航图指针的类型，0为相机导航，1为模型导航
	//pointerHandle - 导航图指针的句柄，如果导航图指针类型为相机，则为相机句柄，否则为模型句柄
	//oriPointerFile - 导航图指针图片
	//alphaPointerFile - 导航图指针alpha图片
	//pointerWidth, pointerHeight - 导航图指针的尺寸
	//返回导航图指针对向的句柄
	DWORD AddNavigaterPointer(DWORD navigaterHandle, 
										CHAR* navPointerName, 
										INT	  pointerType,	//camera or model 
										DWORD pointerHandle, //camera or model handle
										CHAR* oriPointerImageName, 
										CHAR* alphaPointerImageName, 
										INT pointerWidth, 
										INT pointerHeight);

	//-------------------------------------------------------------------------------------------
	//***剖面图相关的方法
	//-------------------------------------------------------------------------------------------
	//创建剖面图对象
	//sectionMapName-剖面图对象名称
	//sliceModelName-切片模型
	//leftTopX, leftTopY, leftTopZ 侧面左上角坐标
	//rightTopX, rightTopY, rightTopZ 侧面右上角坐标
	//leftBottomX, leftBottomY, leftBottom 侧面左下角坐标
	//rightBottomX, rightBottomY, rightBottomZ 侧面右下角坐标
	//image对象名称
	DWORD	CreateSectionMap(CHAR* sectioMapName, 
							INT width, INT height,
							DWORD sliceModelHandle, 
							 FLOAT leftTopX, FLOAT leftTopY, FLOAT leftTopZ, 
							 FLOAT rightTopX, FLOAT rightTopY, FLOAT rightTopZ, 
							 FLOAT leftBottomX, FLOAT leftBottomY, FLOAT leftBottomZ, 
							 FLOAT rightBottomX, FLOAT rightBottomY, FLOAT rightBottomZ, 
							 DWORD imageHandle);

	//执行切片
	VOID	ExecuteSlice(DWORD sectionMapHandle);

	//通过SectionMap的名称得到句柄
	DWORD	GetSectionMapHandleByName(CHAR* sectionMapName);

	//-------------------------------------------------------------------------------------------
	//***场景渲染相关的方法
	//-------------------------------------------------------------------------------------------
	//渲染整个场景
	VOID SunRenderScene();
	//设置背景色
	VOID SunSetBackColor(BYTE red, BYTE green, BYTE blue);
	//设置渲染帧率
	VOID SunSetRenderFPS(UINT fps);
	//设置视口大小
	VOID SunSetViewPort(RECT* rect);
	//得到视口大小
	VOID SunGetViewPort(RECT* rect);

	// 获取/设置视口的相机
	DWORD SunGetViewportCamera(DWORD vp);
	VOID SunSetViewportCamera(DWORD vp, DWORD cam);

	//更新后缓冲区
	BOOL SunUpdateBackBuffer();
	//设置当前场景是否渲染
	VOID SunSetRenderStatus(BOOL renderIt);

	//-------------------------------------------------------------------------------------------
	//***特效相关的方法
	//-------------------------------------------------------------------------------------------
	//创建动态模糊特效对象
	DWORD CreateFSMotionBlur();
	//打开动态模糊特效
	VOID	OpenFSMotionBlur();
	//关闭动态模糊特效
	VOID    CloseFSMotionBlur();
	//设置动态模糊的强度, 0-100
	VOID	SetFSMotionBlurPower(INT value);


	//创建全屏光晕对象
	DWORD CreateFSGlow();
	//打开全屏光晕对象
	VOID OpenFSGlow();
	//关闭全屏光晕对象
	VOID CloseFSGlow();
	//设置全屏光晕的强度
	VOID SetFSGlowValue(FLOAT BloomValue, FLOAT BlurValue);


	//创建LensFlare镜头光斑对象
	DWORD	CreateLensFlare(VECTOR3* sunDir);

	//fogMode, 0-LINEAR, 1-EXP, 2-EXP2
	VOID SunCreateFog(INT fogMode, BYTE red, BYTE green, BYTE blue, FLOAT density, FLOAT start, FLOAT end);
	VOID SunSetFogMode(int mode);
	VOID SunSetFogDensity(FLOAT density);
	VOID SunSetFogFiled(FLOAT start, FLOAT end);
	VOID SunSetFogColor(BYTE red, BYTE green, INT blue);
	VOID SunSetFogEnableStatus(BOOL enable);

	//创建静态CubeMap, 
	//6个Image对象
	//size为cubeMap的尺寸，
	//mode为生成CubeMap的模式
	//返回CubeMap对象的句柄
	DWORD CreateStaticCubeMap(CHAR* name, 
								DWORD frontImage, DWORD backImage, 
								DWORD leftImage, DWORD rightImage, 
								DWORD topImage, DWORD bottomImage,
								INT size);

	//删除CubeMap
	VOID DeleteStaticCubeMap(DWORD cubeMapHandle);
	//创建动态CubeMap
	//size为cubeMap的尺寸，
	//mode为生成CubeMap的模式
	//返回CubeMap对象的句柄
	DWORD CreateDynamicCubeMap(CHAR* cubeMapName, CHAR* centerModelName, UINT size, FLOAT power);
	//增加生成动态CubeMap需要渲染的模型，只有当CreateCubeMap的mode参数设置为1时有效
	VOID AddDyCubeMapReflectionModel(DWORD cubeMapHandle, DWORD modelHandle);
	//删除生成动态CubeMap需要渲染的模型，只有当CreateCubeMap的mode参数设置为1时有效
	VOID DeleteDyCubeMapReflectionModel(DWORD cubeMapHandle, DWORD modelHandle);
	
	//创建电影贴图
	DWORD CreateMovieTexture(CHAR* name, 
							CHAR* movieFilePath, 
							INT mediaType, 
							BOOL withAudio, 
							INT	repeatCnt, 
							CHAR* mtrlName,
							CRITICAL_SECTION* pRenderCS,
							CRITICAL_SECTION* pCtrlCS);
	//通过名称得到电影贴图对象
	DWORD	GetMovieTextureHandleByName(CHAR* name);
	//通过编号得到电影贴图对象
	DWORD	GetMovieTextureHandleByIndex(INT index);
	//设置材质为电影贴图材质
	VOID	SetMovieMtrl(DWORD mtrlHandle, DWORD movieTexHandle);
	//删除电影贴图
	VOID	DeleteMovieTexture(DWORD movieTexHandle);
	//删除电影贴图
	VOID	DeleteMovieTextureByName(CHAR* name);
	//删除电影贴图
	VOID	DeleteMovieTextureByIndex(INT index);
	//删除所有的电影贴图
	VOID	DeleteAllMovieTexture();

//-------------------------------------------------------------------------------------------
//***播放声音相关的方法
//-------------------------------------------------------------------------------------------
	DWORD CreateAudio(CHAR* name, CHAR* AudioFilePath, INT repeatCnt);

	VOID DeleteAudio(DWORD audioHandle);
	VOID DeleteAudioByName(CHAR* name);
	VOID DeleteAudioByIndex(INT index);
	VOID DeleteAllAudio();

	INT GetAudioCount();

	DWORD GetAudioHandleByName(CHAR* name);
	DWORD GetAudioHandleByIndex(INT index);

	VOID GetAudioNameByHandle(DWORD audioHandle, CHAR* name);
	INT GetAudioIndexByHandle(DWORD audioHandle);

	VOID SetAudioFilePath(DWORD audioHandle, CHAR* AudioFilePath);
	VOID GetAudioFilePath(DWORD audioHandle, CHAR* AudioFilePath);

	VOID SetAudioRepeatCount(DWORD audioHandle, INT repeatCount);
	INT	GetAudioRepeatCount(DWORD audioHandle);

	VOID PlayAudio(DWORD audioHandle);
	VOID PauseAudio(DWORD audioHandle);
	VOID StopAudio(DWORD audioHandle);
	VOID RewindAudio(DWORD audioHandle);


//-------------------------------------------------------------------------------------------
//***模型成组相关的方法
//-------------------------------------------------------------------------------------------
	DWORD	CreateModelGroup(CHAR* groupName);

	VOID	DeleteModelGroup(DWORD ModelGroupHandle);
	VOID	DeleteModelGroupByName(CHAR* groupName);
	VOID	DeleteModelGroupByIndex(INT index);
	VOID	DeleteAllModelGroup();

	INT		GetModelGroupCount();
	VOID	ResetModelGroupIndex();

	DWORD	GetModelGroupHandleByName(CHAR* groupName);
	DWORD	GetModelGroupHandleByIndex(INT index);

	VOID	GetModelGroupNameByHandle(DWORD ModelGroupHandle, CHAR* groupName);
	INT		GetModelGroupIndexByHandle(DWORD ModelGroupHandle);
	
	VOID	AddModelToModelGroup(DWORD modelGroupHandle, DWORD modelHandle);
	VOID	DeleteModelFromModelGroup(DWORD modelGroupHandle, DWORD modelHandle);
	VOID	ClearModelGroup(DWORD modelGroupHandle);

	//-------------------------------------------------------------------------------------------
	//***用户输入相关的方法
	//-------------------------------------------------------------------------------------------

	// 获得当前使用的控制对象
	DWORD SunGetCurrentController();

	// 设置当前使用的控制对象
	VOID SunSetCurrentController(DWORD controllerHandle);

	// 设置控制器的相机
	VOID SunSetControllerCamera(DWORD ControllerHandle, DWORD CameraHandle);

	// 获取当前控制对象使用的相机
	DWORD SunGetControllerCamera(DWORD ControllerHandle);

	// 获取当前控制对象使用的相机
	DWORD SunGetCurrentControllerCamera();

	//设置当前用户交互的状态值
	VOID SunSetControlStatus(CONTROLTYPE controlEnum);
	CONTROLTYPE SunGetControllerType(DWORD controller);

	//行走状态控制------------------------------------------------------
	VOID WalkControl();
	//设置行走的速度
	VOID SetWalkCtrl_MoveParam(FLOAT speed, FLOAT acc);
	//设置旋转的速度
	VOID SetWalkCtrl_RotateParam(FLOAT speed, FLOAT acc);
	//------------------------------------------------------------------

	//编辑状态控制------------------------------------------------------
	VOID EditControl();
	//设置EDIT模式下当前摄像机自动旋转的速度，只有在EDIT模式下生效
	VOID SetAutoPitchRoundRollSpeed(FLOAT fPitchValue, FLOAT fRoundValue, FLOAT fRollValue);
	//设置EDIT模式下当前摄像机手动旋转的速度，只有在EDIT模式下生效
	VOID SetManualPitchRoundSpeed(FLOAT fPitchValue, FLOAT fRoundValue);
	//------------------------------------------------------------------

	//飞行状态控制------------------------------------------------------
	VOID FlyControl();
	//设置相机平移的平移速度
	VOID SetFlyCtrl_PanParam(FLOAT speed, FLOAT acc);
	//设置相机推拉速度
	VOID SetFlyCtrl_DollyParam(FLOAT speed, FLOAT acc);
	//------------------------------------------------------------------
    //获得键盘状态
    BOOL GetShiftStatus();


	//---------------------------------------------------------------------------
	//最新的水特效
	//---------------------------------------------------------------------------
	//创建水面对象，x,y,z为Position, gridx/gridy为水面模型的网格数
	VOID CreateRealWater(CHAR* name, FLOAT x, FLOAT y, FLOAT z, INT gridX, INT gridY);
	//通过水面名称获得对象
	DWORD GetRealWaterHandleByName(CHAR* name);
	//增加水面的折射物体
	VOID AddRealWaterRTTModel(DWORD realWaterHandle, DWORD modelHandle);
	//增加水面的反射物体
	VOID AddRealWaterRTFModel(DWORD realWaterHandle, DWORD modelHandle);
	//设置水面用到的CubeMap反射图
	VOID SetRealWaterCubeMap(DWORD realWaterHandle, DWORD cubeMapHandle);

	//---------------------------------------------------------------------------
	//实时反射和折射
	//---------------------------------------------------------------------------
	//创建RTF贴图对象
	DWORD CreateRTFTexture(CHAR* RTFTextureName, CHAR* refPlaneModel, INT width, INT height, FLOAT power);
	//通过RTF贴图的名字获得对象
	DWORD GetRTFTextureHandleByName(CHAR* RTFTextureName);
	//为RTF贴图对象增加需要反射的模型
	VOID AddModelToRTFTexture(CHAR* RTFTextureName, CHAR* modelName);
	
	

	//---------------------------------------------------------------------------
	//最新的画面后期特效
	//---------------------------------------------------------------------------
	//创建颜色编辑器
	VOID CreateColorEffect();
	//设置画面的亮度，对比度，颜色调整
	VOID SetColorEffect(FLOAT brightness, FLOAT contrast, FLOAT red, FLOAT green, FLOAT blue);



    //---------------------------------------------------------------------------
	//粒子系统
	//---------------------------------------------------------------------------
	//创建粒子系统
	DWORD CreateParticles(CHAR* name);
	//设置粒子系统的参数

	//////////////// 数据相关 ///////////////////////
	BOOL SunLoadSceneFromBuffer(DWORD version, const BYTE* pBuffer);       // 从buffer中读取场景信息	
	BOOL SunLoadImageDataFromBuffer(DWORD version, const BYTE* pBuffer);   // 从buffer中读取图像信息	
	BOOL SunCreateImageTextureFromBuffer(const BYTE* pBuffer, DWORD len, int imgIdx);  // 由一张图片的buffer创建纹理

public:
	CVGS2DLL();
	~CVGS2DLL();
	
public:	
	VOID SetAddtionalKeysStatus(BOOL bEnable);                         // 设置VGS响应的第二套按键（除了方向键）是否有效
	VOID SetAddtionalKeys(BYTE left, BYTE right, BYTE up, BYTE down);  // 设置VGS响应的第二套按键值，需要传入按键的ascii码

	DWORD GetDefaultRenderWnd(){return m_defaultRenderWnd;}
	DWORD GetDefaultViewport(){return m_defaultViewport;}

private:
	BOOL     m_bAddtionalKeysEnable;        // 第二套控制键是否可用
	BYTE     m_AddKeyLeft;                  // 第二套控制键
	BYTE     m_AddKeyRight;
	BYTE     m_AddKeyUp;
	BYTE     m_AddKeyDown;

	HWND     m_hwnd;  // 窗体句柄

	// 缺省创建的渲染窗口和视口
	DWORD    m_defaultRenderWnd;
	DWORD    m_defaultViewport;

	CInput   m_inputDevice;   // 输入设备

	// 当前的控制信息
	WalkerInfo  m_curWalkerInfo;
	EditorInfo  m_curEditorInfo;
	FlyerInfo   m_curFlyerInfo;

	VECTOR3  walker_realMoveVec;
	VECTOR3  walker_realRotateVec;

	int m_mouseX, m_mouseY;  // 鼠标的屏幕位置

	// 某个字符键是否被触发
	BOOL IsAlphabetKeyBuffered(BYTE key, BOOL caseSensitive = FALSE);

	// 某个字符键是否被触发
	BOOL IsAlphaKeyBuffered(BYTE key, BOOL caseSensitive = FALSE);

	// 获取当前控制对象信息
	VOID GetCurrentControllerInfo();
};

#endif
