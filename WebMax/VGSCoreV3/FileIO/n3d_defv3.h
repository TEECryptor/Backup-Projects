//----------------------------------------------------------------------
// N3D文件相关结构
//----------------------------------------------------------------------

#ifndef _N3DV3_DEF_H
#define _N3DV3_DEF_H

#include "..\VGSHeader.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include "S3D_Def.h"
#include "N3D_Def.h"

using namespace std;
using namespace N3D;

#pragma pack(push, 1) // 数据结构以单字节对齐

namespace N3DV3
{
	//模型
	typedef struct N3D_ModelV3
	{
		//Node的名称
		CHAR                    name[MAX_NAMESTR_LENGTH];
		//顶点数量
		UINT					vertexCnt;	
		//顶点数据
		N3D_Vertex*				vertex;
		//submesh 
		std::vector<N3D_SubMesh> subMesh;
		//包围盒
		N3D_BoundBox			boundBox;

		float                   boundSphereRadius;

		bool                    isBillboard;
		bool                    isCollision;
		bool                    isGround;
		bool                    isSkyBox; 
		bool                    isVisible;		
		bool                    isClickable;
		bool                    isAutoNormalise;            // 模型缩放时，是否自动更新法线 fuxb 2008-09-09

		// 事件响应
		BOOL bMouseOverFunc;
		CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseDBClickFunc;
		CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseLDownFunc;
		CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseLUpFunc;
		CHAR sMouseLUpFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseRDownFunc;
		CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseRUpFunc;
		CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];

	}N3D_ModelV3;

	//结构体: 模型组
	typedef struct N3D_ModelGroup
	{
		//Node的名称
		CHAR name[MAX_NAMESTR_LENGTH];

		INT						ModelCnt;			//组中的模型数量
		INT*					ModelIndex;			//组中的模型编号序列,对应meshlist中的索引号

		std::vector<N3D_ModelGroup*> ModelGroup;
	}N3D_ModelGroup;

	//结构体: 模型组列表 --模型组列表将作为一个scene下的全局vector使用，不再单独定义结构体、

	//模型列表
	typedef struct N3D_ModelListV3
	{
		//场景中的model数量
		UINT					modelCnt;
		// model列表
		N3D_ModelV3*				model;
	}N3D_ModelListV3;

	typedef struct N3D_ImageInfoV3
	{
		CHAR			Name[MAX_NAMESTR_LENGTH];		//名称
		DWORD			Size;			//压缩后的Image数据大小
	}N3D_ImageInfoV3;

	//下载http上的N3D文件中的Image，然后保存到内存中的Image单元数据结构
	typedef struct N3D_ZipImageDataV3
	{

		N3D_ImageInfoV3   imageInfo;      //image参数
		BYTE*			    Data;			//压缩后的Image数据
	}N3D_ZipImageDataV3;

	//结构体: Flash数据结构
	typedef struct N3D_FlashV3
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
	}N3D_FlashV3;

	//结构体: Flash列表
	typedef struct N3D_FlashListV3
	{		
		INT				FlashCnt;			//Flash数量
		N3D_FlashV3*	    Flash;				//Flash列表
	}N3D_FlashListV3;

	typedef struct N3D_OverlayBasalInfoV3
	{
		CHAR		Name[N3D_MAX_NAME_LENGTH];			// Overlay的名字

		INT			OffsetOrigin;		// 偏移位置的坐标原点
		BOOL		UseRelativePos;		// 使用相对位置
		BOOL		UseRelativeSize;	// 是否使用相对的尺寸

		RECT		Rect;				// Overlay尺寸及位置
		FLOAT		Angle;				// 转角
		INT			Opacity;			// 透明度
		BOOL		IsBackdrop;			// 是否是背景图

		INT			MaterialIndex;		// Overlay的材质Index

		BOOL        IsVisible;          // 是否可见

		INT         zOrder;    // overlay的z顺序, 值越大越靠上层

		// 事件响应
		CHAR	sMouseEventFunc[VGS_MOUSEEVENT_CNT][MAX_NAMESTR_LENGTH];
/*
		BOOL bMouseOverFunc;
		CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseDBClickFunc;
		CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseLDownFunc;
		CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseLUpFunc;
		CHAR sMouseLUpFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseRDownFunc;
		CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseRUpFunc;
		CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];*/

	}N3D_OverlayBasalInfoV3;

	//结构体: Overlay基本结构
	typedef struct N3D_OverlayV3
	{
		N3D_OverlayBasalInfoV3	BasalInfo;  // 基本信息
	}N3D_OverlayV3;

	//结构体: Overlay列表
	typedef struct N3D_OverlayListV3
	{
		INT				OverlayCnt;	//Overlay的数量
		N3D_OverlayV3*	Overlays;	//Overlay列表
	}N3D_OverlayListV3;

	//导航图指针信息
	typedef struct N3D_NavigaterPointerV3
	{
		N3D_OverlayBasalInfoV3 baseInfo;
		UINT		naviType;				//Pointer的类型，Camera-0, Model-1
		INT			ObjIndex;			//相机或模型的编号
	}N3D_NavigaterPointerV3;

	//结构体: Navigater基本结构
	typedef struct N3D_NavigaterV3
	{
		N3D_OverlayBasalInfoV3 baseInfo;  // 小地图的信息

		FLOAT						sceneLeft;					// 场景TopView的Rect
		FLOAT						sceneTop;
		FLOAT						sceneRight;
		FLOAT						sceneBottom;	

		UINT						PointerCnt;					// 导航点的数量
		N3D_NavigaterPointerV3*		NavPointer;					// 导航点的信息

	}N3D_NavigaterV3;

	//结构体: Navigater列表
	typedef struct N3D_NavigaterListV3
	{
		INT						    NavigaterCnt;	//Navigater数量
		N3D_NavigaterV3*			    Navigaters;		//Navigater列表
	}N3D_NavigaterListV3;

	// 按钮基本参数
	typedef struct N3D_ButtonBasalInfoV3
	{
		N3D_OverlayBasalInfoV3 baseInfo; 
		INT		MouseOverMtrlIndex;	//overImage的材质ID号
		INT		MouseDownMtrlIndex;	//downImage的材质ID号

	}N3D_ButtonBasalInfoV3;

	//结构体: Button列表
	typedef struct N3D_ButtonListV3
	{
		INT					ButtonCnt;			//按钮个数
		N3D_ButtonBasalInfoV3*		Buttons;			//按钮列表
	}N3D_ButtonListV3;

	//结构体: N3D_1_0授权结构体
	typedef struct N3D_PrivacyV3
	{
		INT				VersionType;			//版本标示(个人版本0，标准版本1，企业版本2)
		DWORD			ClientIndex;			//授权客户编号
		CHAR			SerialNum[128];			//被授权的软件序列号，已经过128位加密
		CHAR			Authorize[128];			//是否已经被授权去广告的序列号
		DWORD			ExtendInfo1;			//扩展加密信息1
		DWORD			ExtendInfo2;			//扩展加密信息2
	}N3D_PrivacyV3;

	//结构体: Walker对象的结构体
	typedef struct N3D_WalkerV3
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
	}N3D_WalkerV3;

	//结构体: walker对象列表
	typedef struct N3D_WalkerListV3
	{
		INT				WalkerCnt;			//Walker的数量
		N3D_WalkerV3* Walkers;			//Walkder对象列表
	}N3D_WalkerListV3;

	//结构体: Editor对象
	typedef struct N3D_EditorV3
	{	
		CHAR		Name[MAX_NAMESTR_LENGTH];		//名称

		INT			CameraIndex;					//使用的相机编号

		BOOL		bIsAutoRoundRoll;				//Edit模式下是否自动翻滚

		FLOAT		autoSpeedX;						// Edit模式下自动绕 X 轴旋转的速度
		FLOAT		autoSpeedY;						// Edit模式下自动绕 Y 轴旋转的速度

		FLOAT		manualSpeedX;					//Edit模式下手动旋转相机的速度绕 X 轴
		FLOAT		manualSpeedY;					//Edit模式下手动翻滚相机的速度绕 Y 轴

		FLOAT		manualPanXSpeed;				//平移的X方向速度
		FLOAT		manualPanYSpeed;				//平移的Y方向速度

		FLOAT		dollySpeed;						//滚鼠标滚轮时移动相机的速度
		FLOAT		dollyAcc;						//相机推拉加速度

		FLOAT		dollyNearDis;					//最近的Dolly
		FLOAT		dollyFarDis;					//最远的Dolly

		INT			rosAxis;						// 旋转轴标志位, 0: xy, 1 : x, 2 : y
		vector3     LookAtPos;                      // Editor的目标点 

	}N3D_EditorV3;

	//结构体：Editor对象列表
	typedef struct N3D_EditorListV3
	{
		INT					EditorCnt;				//Editor的数量
		N3D_EditorV3*		Editors;				//Editor列表
	}N3D_EditorListV3;

	//结构体: Flyer对象
	typedef struct N3D_FlyerV3
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
	}N3D_FlyerV3;

	//结构体: Flyer对象列表
	typedef struct N3D_FlyerListV3
	{
		INT					FlyerCnt;						//飞翔对象数量
		N3D_FlyerV3*		Flyers;							//飞翔对象列表
	}N3D_FlyerListV3;

	//结构体：控制对象
	typedef struct N3D_ControlDataV3
	{
		INT					ControlType;					//控制类型	CT_WALKER = 0, CT_EDITOR = 1, CT_FLYER = 2,	CT_NONE 非交互模式 
		INT					ControlerIndex;					//控制对象编号

		N3D_WalkerListV3	WalkerList;						//漫游对象列表
		N3D_EditorListV3	EditorList;						//编辑对象列表
		N3D_FlyerListV3	FlyerList;						//飞行对象列表
	}N3D_ControlDataV3;


	// 授权结构体 32 个字节
	typedef struct N3D_RegInfoV3
	{
		DWORD			CorpID;				// 被授权的公司标识
		DWORD			Date1;				// 授权的日期参数
		DWORD			Date2;				// 授权的日期参数
		DWORD           Reserved[5];        // 预留位置
	}N3D_RegInfoV3;

	// 结构体: N3D文件头结构 96个字节 + 后面的可变部分
	typedef struct N3D_HeaderV3
	{	
		N3D_Header		baseHeader;				// 基本头信息

		// v3新增的内容
		DWORD           imageCnt;               // 文件中保存的图片文件数目
		DWORD*          pImageIndexData;        // 每个图片数据在n3d文件中的开始位置

	}N3D_HeaderV3;

} // namespace N3D



#pragma pack(pop)

#endif