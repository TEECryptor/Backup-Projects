/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // V3D_Def.h
* 内容摘要： // V3D version3.0数据结构定义
* 其它说明： // 
* 当前版本： // V3.0
* 作    者： // 傅新波
* 完成日期： // 
* 
* 修改日期      版本号     修改人	      修改内容
* ---------------------------------------------------------------
* 
*
*********************************************************************/

#pragma once

#include "..\VGSHeader.h"
#include "V3D_Def.h"

#include <vector>
using namespace std;

#pragma pack(push, 1) // 数据结构以单字节对齐


//////////////////////////////////// V3D //////////////////////////////////////////

//V3D文件头
typedef struct V3D_HeaderV3
{
	//创图公司的名称
	char					corp[MAX_CORPNAME_LENGTH];
	//v3d文件的版本
	float					v3dVersion;
	CHAR                    Reserverd[60]; // 保留扩展
}V3D_HeaderV3;

// V3D数据目录表
typedef struct V3D_TableV3  // 96 bytes
{
	// 场景属性
	DWORD                   SceneproperyPtr;
	//材质数据的偏移量
	DWORD					mtrlDataPtr;
	// texture ptr
	DWORD                   texturePtr;
	//相机数据的偏移量
	DWORD					cameraDataPtr;
	//灯光数据的偏移量
	DWORD					lightDataPtr;
	//骨骼数据的偏移量
	DWORD					skeletonDataPtr;

	// 模型资源数据的偏移量
	DWORD					meshDataPtr;

	// 模型信息数据偏移量
	DWORD                   entityDataPtr;  // fuxb fuxb 2008-07-23

	//图形数据的偏移量
	DWORD					shapeDataPtr;
	//场景图数据的偏移量
	DWORD					sceneGraphDataPtr;

	// 文字数据的偏移量
	DWORD                   TextDataPtr;
	// 动画数据
	DWORD                   AnimationPtr;
	// overlay数据
	DWORD                   OverlayPtr;
	// button
	DWORD                   ButtonPtr;
	// 导航图
	DWORD                   NavigatotPtr;

	DWORD                   BackdropPtr;  // 背景图  fuxb 2008--7-17

	// 声音
	DWORD                   AudioPtr;
	// 控制信息
	DWORD                   ControlPtr;
	// flash
	DWORD                   FlashPtr;

	// 授权信息
	DWORD                   PrivacyDataPtr;


	CHAR                    Reserved[28];  // 保留扩展
}V3D_TableV3;

//结构体：场景的属性
typedef struct V3D_ScenePropV3
{
	INT				Width;				//窗体宽度
	INT				Height;				//窗体高度
	DWORD			BackColor;			//背景颜色
	INT				FPS;				//渲染帧率
	INT				AntiAliasLevel;		//抗拒齿级别
	//INT				FSBlurLevel;		//全屏动态模糊的级别
	//INT				FSGlowLevel;		//全屏光晕的级别

	//INT				KeyframeCnt;		//关键帧的数量
	//INT				RepeatCnt;			//循环播放的次数， -1为无限次循环
}V3D_ScenePropV3;

//结构体: Flash数据结构
typedef struct V3D_FlashV3
{
	CHAR			Name[MAX_NAMESTR_LENGTH];			//Flash对象名称
	INT             FileNameLen;        //Flash文件路径长度
	CHAR*			pFileName;		    //Flash文件全路径

	INT				OffsetOrigin;		//偏移位置的坐标原点标示
	BOOL			UseRelativePos;		//是否使用相对位置

	RECT			Rect;				//Flash 尺寸
	FLOAT			Angle;				//转角
	INT				Opacity;			//透明度
	BOOL			Transparency;		//是否抠背景

	BOOL			UseFlashSize;		//使用Flash的尺寸
}V3D_FlashV3;

//结构体: Flash列表
typedef struct V3D_FlashListV3
{		
	INT				FlashCnt;			//Flash数量
	V3D_FlashV3*	    Flash;				//Flash列表
}V3D_FlashListV3;


// v3d mesh数据
typedef struct V3D_MeshDetailV3
{
	std::string sName;
	UINT vertextCount;
	char* pVertexBuf;
	UINT subMeshCout;
	vector3 boxNearConner;
	vector3 boxFarConner;
	float   sphereRadius;
	// float   sphereCenter;
	vector<V3D_SubMeshDetail> vSubMesh;

	// 事件响应
	BOOL bMouseOverFunc;
	CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseDBClickFunc;
	CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLDownFunc;
	CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLUpFunc;
	CHAR bMouseLUpFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRDownFunc;
	CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRUpFunc;
	CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];

	V3D_MeshDetail()
	{
		sName = "";
		vertextCount = 0;
		pVertexBuf   = 0;
		subMeshCout  = 0;
		boxFarConner = boxNearConner = vector3(0, 0, 0);
		sphereRadius = 0;
		bMouseOverFunc = bMouseDBClickFunc = bMouseLDownFunc = bMouseLUpFunc = bMouseRDownFunc = bMouseRUpFunc = FALSE;
	}

}V3D_MeshDetailV3;

//结构体: V3D_1_0授权结构体
typedef struct V3D_PrivacyV3
{
	INT				VersionType;			//版本标示(个人版本0，标准版本1，企业版本2)
	DWORD			ClientIndex;			//授权客户编号
	CHAR			SerialNum[128];			//被授权的软件序列号，已经过128位加密
	CHAR			Authorize[128];			//是否已经被授权去广告的序列号
	DWORD			ExtendInfo1;			//扩展加密信息1
	DWORD			ExtendInfo2;			//扩展加密信息2
}V3D_PrivacyV3;

//结构体: Walker对象的结构体
typedef struct V3D_WalkerV3
{
	CHAR			Name[MAX_NAMESTR_LENGTH];		//名称
	INT				CameraIndex;	//使用的相机编号

	FLOAT			walkSpeed;		//Walker摄像机的行走速度
	FLOAT			walkAcc;		//Walker摄像机的行走加速度

	FLOAT			rosSpeed;		//Walker摄像机的旋转速度
	FLOAT			rosAcc;			//Walker摄像机的旋转加速度

	FLOAT			height;			//Walker的高度

	BOOL			bCollision;		//是否有碰撞
	FLOAT			colDis;			//Walker的碰撞距离

	BOOL			bWeight;		//是否有重力
	FLOAT			liftSpeed;		//提升下降的速度，失去重力时有效
	FLOAT			liftAcc;		//提升下降的加速度，失去重力时有效

	FLOAT			upperAngle;		//抬头的最大仰角
	FLOAT			lowerAngle;		//低头的最大俯视角
}V3D_WalkerV3;

//结构体: walker对象列表
typedef struct V3D_WalkerListV3
{
	INT				WalkerCnt;			//Walker的数量
	V3D_WalkerV3* Walkers;			//Walkder对象列表
}V3D_WalkerListV3;

//结构体: Editor对象
typedef struct V3D_EditorV3
{	
	CHAR				Name[MAX_NAMESTR_LENGTH];		//名称

	INT					CameraIndex;	//使用的相机编号

	BOOL				bIsAutoRoundRoll;					//Edit模式下是否自动翻滚

	FLOAT				autoPitchSpeed;						//Edit模式下自动绕X轴旋转的速度
	FLOAT				autoRoundSpeed;						//Edit模式下自动旋转相机的速度
	FLOAT				autoRollSpeed;						//Edit模式下自动翻滚相继的速度

	FLOAT				manualPitchSpeed;					//Edit模式下手动旋转相机的速度
	FLOAT				manualRoundSpeed;					//Edit模式下手动翻滚相机的速度

	FLOAT				manualPanXSpeed;					//Edit模式下手动平移相机的速度
	FLOAT				manualPanYSpeed;					//Edit模式下手动平移相机的速度

	FLOAT				dollySpeed;							//滚鼠标滚轮时移动相机的速度
	FLOAT				dollyAcc;							//相机推拉加速度

	FLOAT				dollyNearDis;						//最近的Dolly
	FLOAT				dollyFarDis;						//最远的Dolly
}V3D_EditorV3;

//结构体：Editor对象列表
typedef struct V3D_EditorListV3
{
	INT					EditorCnt;							//Editor的数量
	V3D_EditorV3*		Editors;							//Editor列表
}V3D_EditorListV3;

//结构体: Flyer对象
typedef struct V3D_FlyerV3
{
	CHAR				Name[MAX_NAMESTR_LENGTH];		//名称

	INT					CameraIndex;					//使用的相机编号

	FLOAT				panSpeed;						//平移速度
	FLOAT				panAcc;							//平移加速度

	FLOAT				dollySpeed;						//相机推拉速度
	FLOAT				dollyAcc;						//相机推拉加速度

	FLOAT				dollyNearDis;					//最近的Dolly
	FLOAT				dollyFarDis;					//最远的Dolly

	FLOAT				rotateSpeed;					//相机旋转的速度
	FLOAT				rotateAcc;						//相机旋转的加速度

	FLOAT				liftSpeed;						//上升下降的速度，失去重力时有效
	FLOAT				liftAcc;						//上升下降的加速度，失去重力时有效

	FLOAT				upperAngle;						//抬头的最大仰角
	FLOAT				lowerAngle;						//低头的最大俯视角

	BOOL				bCollision;						//是否有碰撞
	FLOAT				colDis;							//碰撞距离
}V3D_FlyerV3;

//结构体: Flyer对象列表
typedef struct V3D_FlyerListV3
{
	INT				FlyerCnt;						//飞翔对象数量
	V3D_FlyerV3*		Flyers;							//飞翔对象列表
}V3D_FlyerListV3;

//结构体：控制对象
typedef struct V3D_ControlDataV3
{
	INT					ControlType;					//控制类型
	INT					ControlerIndex;					//控制对象编号

	V3D_WalkerListV3	WalkerList;						//漫游对象列表
	V3D_EditorListV3	EditorList;						//编辑对象列表
	V3D_FlyerListV3	FlyerList;						//飞行对象列表
}V3D_ControlDataV3;


//////////////////// 以下数据结构是为了提高写入V3D时的效率而定义的 ///////////////////////////////////

// v3版本的信息
typedef struct V3D_OverlayInfoV3   
{
	CHAR			Name[MAX_NAMESTR_LENGTH];  // 名称
	INT				materialIndex;  // 使用的材质索引

	BOOL            bRelativeSize; // 是否使用相对尺寸
	BOOL            bRelativePos;  // 是否使用相对位置
	BOOL            bVisible;      // 是否可见

	INT             Origin;  // 对齐时的参考原点，VGSOFFSETORIGIN值 
	FLOAT			Width;   // 目前的宽度, 如果使用相对大小，这里保存百分比 
	FLOAT			Height;  // 目前的高度
	FLOAT           Left;    // 左坐标，相对于视口左上角, 如果使用相对位置，这里保存百分比
	FLOAT           Top;     // 上坐标，相对于视口左上角 

	int             ZOrder;  // 上下的层次顺序，越大越靠上

	// 事件响应
	BOOL bMouseOverFunc;
	CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseDBClickFunc;
	CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLDownFunc;
	CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLUpFunc;
	CHAR bMouseLUpFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRDownFunc;
	CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRUpFunc;
	CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];
}V3D_OverlayInfoV3;


// V3D中保存的button属性
typedef struct V3D_OverlayListV3
{
	UINT					count;
	V3D_OverlayInfoV3*		OverlayList;	
}V3D_OverlayListV3;


// V3D中保存的导航点属性
typedef struct V3D_NaviPointerInfoV3  
{
	CHAR        sName[MAX_NAMESTR_LENGTH];  // 导航点的名称
	int			NavType;		// 0表示以相机导航，1表示用模型导航
	int		    PointedObjIndex;		// 相机或模型对象的索引

	int        materialIndex;   // 导航点使用的材质索引
	BOOL            bRelativeSize; // 是否使用相对尺寸
	BOOL            bRelativePos;  // 是否使用相对位置
	BOOL            bVisible;      // 是否可见

	int             ZOrder;  // 所有导航点之间的层次关系，会始终在导航图上，越大越靠上

	// 事件响应
	BOOL bMouseOverFunc;
	CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseDBClickFunc;
	CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLDownFunc;
	CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLUpFunc;
	CHAR bMouseLUpFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRDownFunc;
	CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRUpFunc;
	CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];

}V3D_NaviPointerInfoV3;

typedef struct V3D_NavigatorBaseInfoV3
{
	CHAR			Name[MAX_NAMESTR_LENGTH];    // 名称
	int             MapMaterialIndex;   // 地图用到的材质索引

	BOOL            bRelativeSize; // 是否使用相对尺寸
	BOOL            bRelativePos;  // 是否使用相对位置
	BOOL            bVisible;      // 是否可见

	FLOAT           Opacity;       // 透明度

	INT             Origin;  // 对齐时的参考原点，VGSOFFSETORIGIN值 
	float			Width;   // 目前的宽度, 如果使用相对大小，这里保存百分比
	float			Height;  // 目前的高度
	float			Left;    // 左坐标，相对于视口左上角 , 如果使用相对位置，这里保存百分比
	float           Top;     // 上坐标，相对于视口左上角

	int             ZOrder;  // 上下的层次顺序，越大越靠上, 对于导航图，设置ZOrder无效

}V3D_NavigatorBaseInfoV3;


// V3D中保存的导航图属性
typedef struct V3D_NavigatorInfoV3   
{
	V3D_NavigatorBaseInfoV3		NaviBaseInfo;     // 导航图的基本属性

	float						mapRectMinX;	  // 导航图映射的3D空间范围，从正上方的俯瞰区域
	float                       mapRectMaxX;
	float                       mapRectMinZ;
	float                       mapRectMaxZ;

	INT							PointerCount;     // 导航点的数目
	V3D_NaviPointerInfoV3*		PointerList;      // 导航点列表

	// 事件响应
	BOOL bMouseOverFunc;
	CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseDBClickFunc;
	CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLDownFunc;
	CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLUpFunc;
	CHAR bMouseLUpFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRDownFunc;
	CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRUpFunc;
	CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];

}V3D_NavigatorInfoV3;

// V3D中保存的导航图属性
typedef struct V3D_NavigatorListV3   
{
	UINT				count;
	V3D_NavigatorInfoV3*  NaviList;	
}V3D_NavigatorListV3;

typedef struct V3D_ButtonInfoV3
{
	CHAR			Name[MAX_NAMESTR_LENGTH];    // 名称

	int             UpMaterialIndex;     // 鼠标弹起时，即正常状态时用到的材质索引
	int             DownMaterialIndex;   // 鼠标按下时使用的材质索引
	int             OverMaterialIndex;   // 鼠标在按钮上时使用的材质索引
	CHAR            CallbackFunName[MAX_NAMESTR_LENGTH];  // 按钮的回调函数名称

	BOOL            bRelativeSize; // 是否使用相对尺寸
	BOOL            bRelativePos;  // 是否使用相对位置
	BOOL            bVisible;      // 是否可见

	FLOAT           Opacity;       // 透明度

	INT             Origin;  // 对齐时的参考原点，VGSOFFSETORIGIN值 
	FLOAT			Width;   // 目前的宽度, 如果使用相对大小，这里保存百分比
	FLOAT			Height;  // 目前的高度, 如果使用相对大小，这里保存百分比
	FLOAT           Left;    // 左坐标，相对于视口左上角 , 如果使用相对位置，这里保存百分比
	FLOAT           Top;     // 上坐标，相对于视口左上角 , 如果使用相对位置，这里保存百分比 

	int             ZOrder;  // 上下的层次顺序，越大越靠上

	// 事件响应
	BOOL bMouseOverFunc;
	CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseDBClickFunc;
	CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLDownFunc;
	CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLUpFunc;
	CHAR bMouseLUpFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRDownFunc;
	CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRUpFunc;
	CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];

}V3D_ButtonInfoV3;

// V3D中保存的button属性
typedef struct V3D_ButtonListV3
{
	UINT				count;
	V3D_ButtonInfoV3*		ButtonList;	
}V3D_ButtonListV3;

// V3D中保存的前景图属性
typedef struct V3D_BackdropInfoV3   
{
	CHAR			Name[MAX_NAMESTR_LENGTH];  // 名称

	int             materialIndex;     // 材质索引

	BOOL            bRelativeSize; // 是否使用相对尺寸
	BOOL            bRelativePos;  // 是否使用相对位置
	BOOL            bVisible;      // 是否可见

	INT             Origin;  // 对齐时的参考原点，VGSOFFSETORIGIN值 
	FLOAT			Width;   // 目前的宽度, 如果使用相对大小，这里保存百分比
	FLOAT			Height;  // 目前的高度, 如果使用相对大小，这里保存百分比
	FLOAT           Left;    // 左坐标，相对于视口左上角 , 如果使用相对位置，这里保存百分比
	FLOAT           Top;     // 上坐标，相对于视口左上角 , 如果使用相对位置，这里保存百分比 

	int             ZOrder;  // 上下的层次顺序，越大越靠上

	// 事件响应
	BOOL bMouseOverFunc;
	CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseDBClickFunc;
	CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLDownFunc;
	CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLUpFunc;
	CHAR bMouseLUpFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRDownFunc;
	CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRUpFunc;
	CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];

}V3D_BackdropInfoV3;

// V3D中保存的背景图属性
typedef struct V3D_BackdropListV3
{
	UINT					count;
	V3D_BackdropInfoV3*		BackdropList;	
}V3D_BackdropListV3;

//////////////////////////////////// End of V3D //////////////////////////////////////////

#pragma pack(pop)
