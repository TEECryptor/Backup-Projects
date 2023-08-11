/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // V3D_Def.h
* 内容摘要： // V3D数据结构定义
* 其它说明： // 
* 当前版本： // V2.0
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
//#include "..\VGSDef.h"
#include "s3d_def.h"

#include <vector>
using namespace std;

#pragma pack(push, 1) // 数据结构以单字节对齐

//相机参数结构V3D，位置和转角交给Node
typedef struct VGS_V3D_CAMINFO_OLD
{
	//是否是轴侧相机
	bool					isOrtho;

	//镜头
	float					fov;

	//相机的最近最远剪切面
	float					nearClipDis;
	float					farClipDis;

	VECTOR3                 position;      // 位置  
	VGS_QUATERNION          quat;          // 转角

}VGS_V3D_CAMINFO_OLD;

typedef struct VGS_V3D_CAMINFO
{
	//是否是轴侧相机
	bool					isOrtho;
	
	//镜头
	float					fov;
	
	//相机的最近最远剪切面
	float					nearClipDis;
	float					farClipDis;
	
	VECTOR3                 position;      // 位置

	VECTOR3                 at;
	quaternion              quat;          // 转角
	
}VGS_V3D_CAMINFO;


typedef struct VGS_V3D_CAMINFO_V3
{
	//是否是轴侧相机
	bool					isOrtho;

	//镜头
	float					fov;

	//相机的最近最远剪切面
	float					nearClipDis;
	float					farClipDis;

	VECTOR3                 position;      // 位置

	VECTOR3                 at;
	VECTOR3                 up;

}VGS_V3D_CAMINFO_V3;

// V3D相机
typedef struct VGS_V3D_Camera_old
{
	//相机的名称
	int                    nameLen;
	std::string            name;

	//相机初始信息
	VGS_V3D_CAMINFO_OLD				v3dBaseInfo;			// 仅用于V3D, 相机基本信息
	std::vector<VGS_V3D_CAMINFO>	v3dAnimationInfo;		// 仅用于V3D，相机动画

}VGS_V3D_Camera_old;

typedef struct VGS_V3D_Camera
{
	//相机的名称
	int                             nameLen;
	std::string                     name;
	
	//相机初始信息
	VGS_V3D_CAMINFO_V3				v3dBaseInfo;			// 仅用于V3D, 相机基本信息
	std::vector<VGS_V3D_CAMINFO>	v3dAnimationInfo;		// 仅用于V3D，相机动画
	
}VGS_V3D_Camera;

//灯光的可变参数V3D
typedef struct VGS_V3D_OMNI
{
	//颜色
	DWORD					color;
	//强度
	float					multiply;
	
	//光线的位置
	VECTOR3                 position;

	//光线衰减范围
	float					attenuationRange;
	float					attenuation0;
	float					attenuation1;
	float					attenuation2;
}VGS_V3D_OMNI;

//灯光的可变参数V3D
typedef struct VGS_V3D_SPOTLIGHT
{
	//颜色
	DWORD					color;
	//强度
	float					multiply;
	
	//光线的位置和方向
	VECTOR3                 position;
	VECTOR3					direction;

	//光线衰减范围
	float					attenuationRange;
	float					attenuation0;
	float					attenuation1;
	float					attenuation2;

	//光圈区域的范围角
	float					innerAngle;
	float					outerAngle;
	float					fallOff;
}VGS_V3D_SPOTLIGHT;


//灯光的可变参数V3D
typedef struct VGS_V3D_DIRECTIONAL
{
	//颜色
	DWORD					color;
	//强度
	float					multiply;
	
	//光线的位置和方向
	VECTOR3					direction;

	//光线衰减范围
	float					attenuationRange;
	float					attenuation0;
	float					attenuation1;
	float					attenuation2;

}VGS_V3D_DIRECTIONAL;

//灯光
typedef struct VGS_V3D_Light
{
	//Node的名称
	int						strLen;
	std::string 			name;

	//灯光类型
	VGS_LIGHT_TYPE   		type;

	//V3D保存的灯光参数帧数据
	std::vector<VGS_V3D_OMNI>				omniInfo;
	std::vector<VGS_V3D_DIRECTIONAL>		directionLightInfo;
	std::vector<VGS_V3D_SPOTLIGHT>			spotLightInfo;

}VGS_V3D_Light;

//////////////////////////////////// V3D //////////////////////////////////////////

const char SUNTOWARD_NAME[] = "SUNTOWARD\0";

/** V3D版本
1. 2008-10-28之前 5.0000
2. 2008-10-29     50001  // 将版本号改为整数，50001 = 5.00.01
3. 2008-12-08     50002  // V3D中cube map保存贴图文件名称
4. 2008-12-13     50003  // 模型信息结构修改
5. 2008-12-15     50004  // editor信息增加lookatmodelindex字段
*/
const float CURRENT_V3DVERSION = 60000; 

const int MAX_CORPNAME_LENGTH = 64;

//V3D文件头
typedef struct V3D_Header
{
	//创图公司的名称
	char					corp[MAX_CORPNAME_LENGTH];
	//v3d文件的版本
	float					v3dVersion;
	CHAR                    Reserverd[60]; // 保留扩展
}V3D_Header;

// V3D数据目录表
typedef struct V3D_Table  // 96 bytes
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
}V3D_Table;

//结构体：场景的属性
typedef struct V3D_SceneProp
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
}V3D_SceneProp;


typedef struct V3D_Animation
{
}V3D_Animation;

//结构体: 声音数据结构
typedef struct V3D_Audio
{
	CHAR			Name[32];				//音频对象名
	INT             FileNameLen;            //音频文件路径长度
	CHAR*			pFileName;		    	//音频文件全路径
	BYTE			Type;					//文件类型
	INT				RepeatCnt;				//重播次数
}V3D_Audio;

//音频列表
typedef struct V3D_AudioList
{
	INT				AudioCnt;			//音频的数量
	V3D_Audio*	    Audio;				//音频列表
}V3D_AudioList;

//结构体: Flash数据结构
typedef struct V3D_Flash
{
	CHAR			Name[32];			//Flash对象名称
	INT             FileNameLen;        //Flash文件路径长度
	CHAR*			pFileName;		    //Flash文件全路径
	
	INT				OffsetOrigin;		//偏移位置的坐标原点标示
	BOOL			UseRelativePos;		//是否使用相对位置

	RECT			Rect;				//Flash 尺寸
	FLOAT			Angle;				//转角
	INT				Opacity;			//透明度
	BOOL			Transparency;		//是否抠背景

	BOOL			UseFlashSize;		//使用Flash的尺寸
}V3D_Flash;

//结构体: Flash列表
typedef struct V3D_FlashList
{		
	INT				FlashCnt;			//Flash数量
	V3D_Flash*	    Flash;				//Flash列表
}V3D_FlashList;

//结构体：文字信息
typedef struct V3D_TextInfo
{
	DWORD	Color;
	INT			X;
	INT			Y;
}V3D_TextInfo;

//结构体: 文字基本结构
typedef struct V3D_Text
{
	INT						Length;		//字符数量
	CHAR*					StrText;	//字符串

	INT						X;
	INT						Y;			//文字的位置
	DWORD				Color;		//文字的颜色

	CHAR					FontName[32];	//字体名
	
	UINT					KfrCnt;		//关键帧数量
	V3D_TextInfo*		KfrInfo;	//关键帧数据
}V3D_Text;

//结构体: 文字列表
typedef struct V3D_TextList
{
	INT			TextCnt;	//文字数量
	V3D_Text*	Texts;		//文字列表
}V3D_TextList;

////结构体：Overlay信息
//typedef struct V3D_OverlayInfo
//{
//	RECT		Rect;			//Overlay设定的相对尺寸
//	FLOAT		Angle;			//转角
//	INT			Opacity;		//透明度
//}V3D_OverlayInfo;
//
////结构体: Overlay基本结构
//typedef struct V3D_Overlay
//{
//	CHAR		Name[32];			//Overlay的名字
//
//	INT			OffsetOrigin;		//偏移位置的坐标原点
//	BOOL		UseRelativePos;		//使用相对位置
//
//	RECT		Rect;				//Overlay尺寸
//	FLOAT		Angle;				//转角
//	INT			Opacity;			//透明度
//	BOOL		IsBackdrop;			//是否是背景图
//
//	UINT		MaterialID;			//Overlay的材质ID号
//	BOOL		UseRelativeSize;	//是否使用相对的尺寸
//	UINT		KfrCnt;				//关键帧数量
//	V3D_OverlayInfo*	KfrInfo;	//关键帧信息
//}V3D_Overlay;
//
////结构体: Overlay列表
//typedef struct V3D_OverlayList
//{
//	INT				OverlayCnt;	//Overlay的数量
//	V3D_Overlay*	Overlays;	//Overlay列表
//}V3D_OverlayList;
//
////导航图指针信息
//typedef struct V3D_NavigaterPointer
//{
//	CHAR		Name[32];			//Pointer的名字
//	UINT		OverlayIndex;		//Pointer使用的Overlay的编号
//	UINT		Type;				//Pointer的类型，Camera-0, Model-1
//	UINT		ObjIndex;			//相机或模型的编号
//}V3D_NavigaterPointer;
//
////结构体: Navigater基本结构
//typedef struct V3D_Navigater
//{
//	CHAR						Name[32];
//	RECT						Rect;						//导航Map平面区域
//
//	UINT						MapOverlayIndex;			//代表Map(Overlay)的编号
//
//	UINT						PointerCnt;					//Pointer的数量
//	V3D_NavigaterPointer*		NavPointer;					//Pointer信息
//
//	FLOAT						sceneLeft;					//场景TopView的Rect
//	FLOAT						sceneTop;
//	FLOAT						sceneRight;
//	FLOAT						sceneBottom;
//}V3D_Navigater;
//
////结构体: Navigater列表
//typedef struct V3D_NavigaterList
//{
//	INT							NavigaterCnt;	//Navigater数量
//	V3D_Navigater*			Navigaters;		//Navigater列表
//}V3D_NavigaterList;

////结构体: Button的数据
//typedef struct V3D_ButtonInfo
//{
//	RECT		Rect;				//按钮的Rect
//	INT			Opacity;			//按钮的透明度
//}V3D_ButtonInfo;
//
////结构体: Button单元结构
//typedef struct V3D_Button
//{
//	CHAR		Name[32];			//按钮的名称
//	RECT		Rect;				//按钮的Rect
//
//	INT			Opacity;			//按钮的透明度
//	CHAR		CallBackFunc[32];	//回调函数名称
//	int		MouseUpMtrlIndex;	//Button Up的ID号
//	int		MouseOverMtrlIndex;	//overImage的材质ID号
//	int		MouseDownMtrlIndex;	//downImage的材质ID号
//	
//	UINT		KfrCnt;				//关键帧数量
//	V3D_ButtonInfo*	KfrInfo;		//关键帧数据
//
//}V3D_Button;
//
////结构体: Button列表
//typedef struct V3D_ButtonList
//{
//	INT					ButtonCnt;			//按钮个数
//	V3D_Button*		Buttons;			//按钮列表
//}V3D_ButtonList;

//结构体: V3D_1_0授权结构体
typedef struct V3D_Privacy
{
	INT				VersionType;			//版本标示(个人版本0，标准版本1，企业版本2)
	DWORD			ClientIndex;			//授权客户编号
	CHAR			SerialNum[128];			//被授权的软件序列号，已经过128位加密
	CHAR			Authorize[128];			//是否已经被授权去广告的序列号
	DWORD			ExtendInfo1;			//扩展加密信息1
	DWORD			ExtendInfo2;			//扩展加密信息2
}V3D_Privacy;

//结构体: Walker对象的结构体
typedef struct V3D_Walker
{
	CHAR			Name[32];		//名称
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
}V3D_Walker;

//结构体: walker对象列表
typedef struct V3D_WalkerList
{
	INT				WalkerCnt;			//Walker的数量
	V3D_Walker* Walkers;			//Walkder对象列表
}V3D_WalkerList;

//结构体: Editor对象
typedef struct V3D_Editor
{	
	CHAR				Name[32];		//名称

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
}V3D_Editor;

//结构体：Editor对象列表
typedef struct V3D_EditorList
{
	INT					EditorCnt;							//Editor的数量
	V3D_Editor*		Editors;							//Editor列表
}V3D_EditorList;

//结构体: Flyer对象
typedef struct V3D_Flyer
{
	CHAR				Name[32];		//名称

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
}V3D_Flyer;

//结构体: Flyer对象列表
typedef struct V3D_FlyerList
{
	INT					FlyerCnt;						//飞翔对象数量
	V3D_Flyer*		Flyers;							//飞翔对象列表
}V3D_FlyerList;

//结构体：控制对象
typedef struct V3D_ControlData
{
	INT					ControlType;					//控制类型
	INT					ControlerIndex;					//控制对象编号

	V3D_WalkerList	WalkerList;						//漫游对象列表
	V3D_EditorList	EditorList;						//编辑对象列表
	V3D_FlyerList	FlyerList;						//飞行对象列表
}V3D_ControlData;

//V3D文件结构
typedef struct V3D_FileStruct
{
	//文件头
	V3D_Header				header;

}V3D_FileStruct;

//////////////////// 以下数据结构是为了提高写入V3D时的效率而定义的 ///////////////////////////////////

// v3d submesh数据
typedef struct V3D_SubMeshDetail
{
	std::string sName;
	UINT indexCount;
	UINT indexSize;
	char* pIndexBuf;
	V3D_SubMeshDetail()
	{
		sName = "";
		indexSize = 0;
		indexCount = 0;
		pIndexBuf = 0;
	}
}V3D_SubMeshDetail;

// v3d mesh数据
typedef struct V3D_MeshDetail
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

	V3D_MeshDetail()
	{
		sName = "";
		vertextCount = 0;
		pVertexBuf   = 0;
		subMeshCout  = 0;
		boxFarConner = boxNearConner = vector3(0, 0, 0);
		sphereRadius = 0;
	}

}V3D_MeshDetail;

typedef struct V3D_MaterialDetail
{
}V3D_MaterialDetail;

typedef struct V3D_PassDetail
{
	BYTE ShadingMode;
	UINT Amb_RGBA;
	UINT Dif_RGBA;
	UINT Spe_RGBA;
	float shininess;
	UINT sel_RGBA;
	BYTE sBlendFactor;
	BYTE dBlendFactor;
	BYTE CullMode;
	bool bDepthCheck;
	bool bLighting;
	BYTE PolygonMode;
	bool bDepthWrite;
	BYTE RejFunc;
	BYTE AlphaValue;
	float Spe_Level;  // [0, 100] add by fuxb 2008-8-31
	float selfIllumination; // [0, 100]
	float USpeed;  // fuxb 2008-12-17 废弃不用了
	float VSpeed;  // fuxb 2008-12-17 废弃不用了

	bool  bUseDiffuse;  // fuxb 2008-10-28, 是否使用材质的颜色

}V3D_PassDetail;


// fuxb 2008-10-28, 增加标识是否使用材质的颜色
typedef struct V3D_PassDetail_Ex
{	
	BYTE ShadingMode;
	UINT Amb_ARGB;
	UINT Dif_ARGB;
	UINT Spe_ARGB;
	float Spe_Level;  // [0, 100] add by fuxb 2008-8-31
	float shininess;
	float selfIllumination; // [0, 100]
	UINT sel_ARGB;
	BYTE bTwoside;
	BYTE PolygonMode;
	bool bAlphaBlend;
	bool bAlphaTest;
	BYTE AlphaTestValue;
	INT Opacity;		//透明度 [0, 100]
	bool bLighting;
	bool bDepthWrite;
	BYTE fillMode;

	bool  bUseDiffuse;  // fuxb 2008-10-28, 是否使用材质的颜色
}V3D_PassDetail_Ex;

// 用于实现 块 写入v3d文件
typedef struct V3D_CameraDetail 
{
	BYTE       PrjType;
	float      FromX;
	float      FromY;
	float      FromZ;
	float      OrientW;
	float      OrientX;
	float      OrientY;
	float      OrientZ;
	float      DirectionX;
	float      DirectionY;
	float      DirectionZ;
	BYTE       PolygonMode;
	float      fFOVY;
	float      nearClip; 
	float      farClip; 
}V3D_CameraDetail;

// 用于实现 块 写入v3d文件
typedef struct V3D_LightDetail
{
	BYTE  type;
	DWORD Dif_RGBA;
	DWORD Spe_RGBA;
	float fAttenuationRange;
	float fAttenuationConst;
	float fAttenuationLinear;
	float fAttenuationQuadric;
	float PositionX;
	float PositionY;
	float PositionZ;
	float DirectionX;
	float DirectionY;
	float DirectionZ;

}V3D_LightDetail;

// V3D中保存的前景图属性
typedef struct V3D_OverlayInfo   
{
	CHAR			Name[MAX_NAMESTR_LENGTH];  // 名称
	CHAR            BaseImg[MAX_NAMESTR_LENGTH];  // 基本图片名称
	CHAR            OpacImg[MAX_NAMESTR_LENGTH];  // 透明图片名称
	// INT				materialIndex;  // 使用的材质索引
	//CHAR            materialName[MAX_NAMESTR_LENGTH];   // 地图用到的材质名称

	BOOL            bRelativeSize; // 是否使用相对尺寸
	BOOL            bRelativePos;  // 是否使用相对位置
	BOOL            bVisible;      // 是否可见

	FLOAT           Opacity;       // 透明度

	INT             Origin;  // 对齐时的参考原点，VGSOFFSETORIGIN值 

	FLOAT			Width;   // 目前的宽度, 如果使用相对大小，这里保存百分比 
	FLOAT			Height;  // 目前的高度
	FLOAT           Left;    // 左坐标，相对于视口左上角, 如果使用相对位置，这里保存百分比
	FLOAT           Top;     // 上坐标，相对于视口左上角 

	int             ZOrder;  // 上下的层次顺序，越大越靠上

}V3D_OverlayInfo;

// V3D中保存的button属性
typedef struct V3D_OverlayList
{
	UINT					count;
	V3D_OverlayInfo*		OverlayList;	
}V3D_OverlayList;


// V3D中保存的导航点属性
typedef struct V3D_NaviPointerInfo  
{
	CHAR        sName[MAX_NAMESTR_LENGTH];  // 导航点的名称
	int			NavType;		// 0表示以相机导航，1表示用模型导航
	// DWORD		PointedHandle;	// 相机或模型的句柄
	DWORD		PointedIndex;		// 相机或模型对象的索引

	CHAR        PointerImg[MAX_NAMESTR_LENGTH];  // 使用的图片名称
	// int 		materialIndex;	// 导航点用到的材质索引
	// CHAR            materialName[MAX_NAMESTR_LENGTH];   // 地图用到的材质名称

	int         width;
	int         height;
	int         left;
	int         top;

	bool        bVisible;
	int         Opacity; // [0, 100]

}V3D_NaviPointerInfo;

typedef struct V3D_NavigatorBaseInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];    // 名称
	CHAR            MapImg[MAX_NAMESTR_LENGTH];  // 地图图片名称
	// int             MapMaterialIndex;   // 地图用到的材质索引
	// CHAR            materialName[MAX_NAMESTR_LENGTH];   // 地图用到的材质名称

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

}V3D_NavigatorBaseInfo;


// V3D中保存的导航图属性
typedef struct V3D_NavigatorInfo   
{
	V3D_NavigatorBaseInfo		NaviBaseInfo;     // 导航图的基本属性

	float						mapRectMinX;	  // 导航图映射的3D空间范围，从正上方的俯瞰区域
	float                       mapRectMaxX;
	float                       mapRectMinZ;
	float                       mapRectMaxZ;

	INT							PointerCount;     // 导航点的数目
	V3D_NaviPointerInfo*		PointerList;      // 导航点列表

}V3D_NavigatorInfo;

// V3D中保存的导航图属性
typedef struct V3D_NavigatorList   
{
	UINT				count;
	V3D_NavigatorInfo*  NaviList;	
}V3D_NavigatorList;

typedef struct V3D_ButtonInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];    // 名称

	//int             UpMaterialIndex;     // 鼠标弹起时，即正常状态时用到的材质索引
	//int             DownMaterialIndex;   // 鼠标按下时使用的材质索引
	//int             OverMaterialIndex;   // 鼠标在按钮上时使用的材质索引
	CHAR             UpImageName[MAX_NAMESTR_LENGTH];     // 鼠标弹起时，即正常状态时用到的图片名称
	CHAR             DownImageName[MAX_NAMESTR_LENGTH];   // 鼠标按下时使用的图片名称
	CHAR             OverImageName[MAX_NAMESTR_LENGTH];   // 鼠标在按钮上时使用的图片名称

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

}V3D_ButtonInfo;

// V3D中保存的button属性
typedef struct V3D_ButtonList
{
	UINT				count;
	V3D_ButtonInfo*		ButtonList;	
}V3D_ButtonList;

// V3D中保存的前景图属性
typedef struct V3D_BackdropInfo   
{
	CHAR			Name[MAX_NAMESTR_LENGTH];  // 名称

	CHAR            ImgFile[MAX_NAMESTR_LENGTH];  // 基本图片名称


	BOOL            bVisible;      // 是否可见

	FLOAT			Width;   // 百分比 
	FLOAT			Height;  // 
	FLOAT           Left;    // 百分比
	FLOAT           Top;     // 

}V3D_BackdropInfo;

// V3D中保存的背景图属性
typedef struct V3D_BackdropList
{
	UINT					count;
	V3D_BackdropInfo*		BackdropList;	
}V3D_BackdropList;



/*// node所附着对象的类型
enum NODEOBJECT_TYPE
{
	NBT_NONE = 0,
	NBT_CAMERA,
	NBT_LIGHT,
	NBT_ENTITY
};

// node所附着的对象
typedef struct NodeObject
{
	NODEOBJECT_TYPE	type;   // 对象类型
	UINT            Index;  // 对象在各自存储队列中的索引
}NodeObject;

typedef struct SceneGraphNode
{
	int            ParentIdx;         // 父节点在节点队列中的索引 
	float          PosX;              // positon
	float          PosY;
	float          PosZ;
	float          ScaleX;            // scale
	float          ScaleY;
	float          ScaleZ;
	float          OrientW;           // rotation
	float          OrientX;
	float          OrientY;
	float          OrientZ;
	std::vector<NodeObject>   vObject;     // 节点下附着的对象
}SceneGraphNode;

// used to fast write and read v3d file
typedef struct NodeSaveParam
{
	int            ParentIdx;
	float          PosX;
	float          PosY;
	float          PosZ;
	float          ScaleX;
	float          ScaleY;
	float          ScaleZ;
	float          OrientW;
	float          OrientX;
	float          OrientY;
	float          OrientZ;
}NodeSaveParam;
*/


// 实时反射参数
typedef struct VGS_RTF_PARAM
{
	int width, height;
	DWORD refectionModel;          // 反射模型指针
	std::vector<DWORD> vrefectedModels; // 被反射模型指针
}VGS_RTF_PARAM;


//////////////////// end of 以上数据结构是为了提高写入V3D时的效率而定义的 ///////////////////////////////////


//////////////////////////////////// End of V3D //////////////////////////////////////////

#pragma pack(pop)
