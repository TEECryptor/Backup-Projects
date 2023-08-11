#ifndef _VGSDEF_H
#define _VGSDEF_H

#include <windows.h>
//#include <windef.h>
// namespace VGS {

#pragma pack(push, 4) // 数据结构以4字节对齐


#if _VGS_BUILDING_DLL
# define VGS_EXPORT __declspec (dllexport)
#else 
# define VGS_EXPORT// __declspec (dllimport)
#endif

/////////// 定义常量 ////////////////////////////////////
/// 名字最长256个字节，包括'\0'
const INT MAX_NAMESTR_LENGTH  = 64;    // 对象的名称最长不超过64
const INT MAX_ERRORSTR_LENGTH = 128;
const INT MAX_FILENAME_LENGTH = 256;   // 文件完整路径及名称，不超过256字节

/////////////////////////////////////////////////////////

/////////////////////////////// vgs 消息定义  ///////////////////////

#define WM_WEBMAX_PROGRESS (WM_USER + 300)          // 进度消息

////////////////////////////////////////////////////////////////////

/***各种枚举类型定义
*/
// 控制对象的类型
enum CONTROLTYPE
{
	CT_WALKER = 0,  // 行走漫游模式
	CT_EDITOR,      // 物件展示模式
	CT_FLYER,       // 飞行漫游模式
	CT_NONE         // 非交互模式 
};

/// 
enum MODELCONTROLSTATUS
{
	IDLE,			//正常状态
	MOVE_WORLD,		//按世界坐标系移动
	MOVE_SELF,      //按自身坐标系移动
	ROTATE_WORLD,   //按世界坐标系旋转
	ROTATE_SELF,    //按自身坐标系旋转
	SCALE_WORLD          //按参考坐标系缩放
};
///鼠标的信息结构体
typedef struct MOUSEINFO
{
	SHORT		X;
	SHORT		Y;				//坐标
	BOOL		LButton;		//按下为1，松开为0
	BOOL		MButton;		//中键按下为1松开为0
	BOOL		RButton;
	FLOAT		RollDis;		//滚轮
}MOUSEINFO;

// 鼠标按键类型
enum VGS_MOUSEKEY_TYPE
{
	VMT_NONE = -1,  // 无效
	VMT_LCLICK,     // 左键单击
	VMT_RCLICK,     // 右键单击
	VMT_MCLICK,     // 中键单击
	VMT_LDBCLICK    // 左键双击
};

///键盘的信息结构体
typedef struct KEYBOARDINFO
{
	BYTE		Key[256];
}KEYBOARDINFO;

///D3D FORMAT
enum TEXFORMAT
{
	RGBA5650,
	RGBA5551,
	RGBA4444,
	RGBA8880,
	RGBA8888
};

///对齐模式
enum ALIGNMODE
{
	ALIGN_NONE = 0,
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_TOP,
	ALIGN_BOTTOM,
	ALIGN_LEFT_TOP,
	ALIGN_MIDDLE_TOP,
	ALIGN_RIGHT_TOP,
	ALIGN_LEFT_BOTTOM,
	ALIGN_MIDDLE_BOTTOM,
	ALIGN_RIGHT_BOTTOM,
	ALIGN_CENTER
};

/// 对齐时的参考原点
enum VGSOFFSETORIGIN
{
	VOO_TOP_LEFT = 0,  // 左上角
	VOO_TOP_MIDDLE,    // 最顶部的中点
	VOO_TOP_RIGHT,     // 右上角
	VOO_MIDDLE_LEFT,   // 最左边的中点
	VOO_CENTER,        // 中心
	VOO_MIDDLE_RIGHT,  // 最右边的中点
	VOO_BOTTOM_LEFT,   // 左下角
	VOO_BOTTOM_MIDDLE, // 最下面的中点
	VOO_BOTTOM_RIGHT   // 右下角
};


///视频的格式
enum MOVIETYPE
{
	MOVIETYPE_AVI = 0,
	MOVIETYPE_MPG,
	MOVIETYPE_WMV,
	MOVIETYPE_MP4,
	MOVIETYPE_RMVB
};

///定义几个默认的相机
enum DEFAULTCAMERA_TYPE
{
	CAME_NONE = 0,  // 无效的相机
	USER_DEFINE,		//用户自定义相机
	FRONT_CAMERA,
	BACK_CAMERA,
	LEFT_CAMERA,
	RIGHT_CAMERA,
	TOP_CAMERA,
	BOTTOM_CAMERA,
	USER_CAMERA,         //默认轴侧
	PERSPECTIVE_CAMERA   //默认透视
};

// 相机的投影方式
enum VGS_CAMERA_TYPE
{
	VCT_ORTHO = 0,  // 0: 轴侧相机
	VCT_PROJECT     // 1：透视相机
};

//-------------------------------------------------------------------------------------------
/***矢量定义
*/
//-------------------------------------------------------------------------------------------
typedef struct VECTOR2
{
	FLOAT x;
	FLOAT y;
}VECTOR2;

typedef struct VECTOR3
{
	FLOAT x;
	FLOAT y;
	FLOAT z;
	VECTOR3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	VECTOR3(FLOAT X, FLOAT Y, FLOAT Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	VECTOR3 operator +(VECTOR3 v)
	{
		return VECTOR3(x + v.x,	y + v.y, z + v.z);
	}
	VECTOR3 operator +=(VECTOR3 v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	VECTOR3 operator -()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}
	VECTOR3 operator -(VECTOR3 v)
	{
		return VECTOR3(x - v.x,	y - v.y, z - v.z);
	}
	VECTOR3 operator -=(VECTOR3 v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	VECTOR3 operator *(float v)
	{
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}
	VECTOR3 operator *=(float v)
	{
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}
}VECTOR3;

typedef VECTOR3 vector3;

// vgs四元数，用于记录转角
typedef struct VGS_QUATERNION
{
	FLOAT x;
	FLOAT y;
	FLOAT z;
	FLOAT w;
	VGS_QUATERNION()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}
	VGS_QUATERNION(FLOAT X, FLOAT Y, FLOAT Z, FLOAT W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
}VGS_QUATERNION;

typedef VGS_QUATERNION quaternion;

//-------------------------------------------------------------------------------------------
///颜色定义
//-------------------------------------------------------------------------------------------
typedef struct VGSRGB
{
	BYTE red;
	BYTE green;
	BYTE blue;
	VGSRGB()
	{
		red = green = blue = 255;
	}
	VGSRGB(BYTE r, BYTE g, BYTE b)
	{
		red = r; green = g; blue = b;
	}
}VGSRGB;

/// RGBA数据格式
typedef struct VGSRGBA
{
	BYTE red;
	BYTE green;
	BYTE blue;
	BYTE alpha;
	VGSRGBA()
	{
		red = green = blue = alpha = 255;
	}
	VGSRGBA(BYTE r, BYTE g, BYTE b, BYTE a)
	{
		red = r; green = g; blue = b; alpha = a;
	}
}VGSRGBA;

///定义包围球信息结构
typedef struct BoundSphere
{
	VECTOR3 vCenter; //中心点
	FLOAT   fRadius; //半径
}BoundSphere;

///定义包围和数据结构
typedef struct BoundBox
{
	VECTOR3 vMin; //左下角
	VECTOR3 vMax; //右上角
}BoundBox;

///定义walker的数据结构
typedef struct WalkerInfo
{
	CHAR	Name[MAX_NAMESTR_LENGTH];		        //Walker的名字
	// INT			cameraIndex;	//使用的相机编号
	CHAR        Camera[MAX_NAMESTR_LENGTH];			//使用的相机名称

	FLOAT	walkSpeed;		//Walker摄像机的行走速度
	FLOAT	walkAcc;		//Walker摄像机的行走加速度
	FLOAT	rosSpeed;		//Walker摄像机的旋转速度
	FLOAT	rosAcc;			//Walker摄像机的旋转加速度
	FLOAT   height;			//Walker的高度
	
	BOOL	bCollision;		//是否有碰撞
	FLOAT	colDis;			//碰撞距离

	BOOL	bWeight;		//是否有重力
	FLOAT	liftSpeed;		//上升下降的速度，失去重力时有效
	FLOAT	liftAcc;		//上升下降的加速度，失去重力时有效

	FLOAT	upperAngle;		//抬头的最大仰角
	FLOAT	lowerAngle;		//低头的最大俯视角
}WalkerInfo;

///定义Editor的数据结构
typedef struct EditorInfo
{
	CHAR		Name[MAX_NAMESTR_LENGTH];							//Editor的名字
	// INT			cameraIndex;					//使用的相机编号
	CHAR        Camera[MAX_NAMESTR_LENGTH];						    //使用的相机名称

	BOOL		bIsAutoRoundRoll;					//Edit模式下是否自动翻滚

	FLOAT		autoSpeedX;						    // Edit模式下自动绕 X 轴旋转的速度，单位：周/分钟
	FLOAT		autoSpeedY;						    // Edit模式下自动绕 Y 轴旋转的速度，单位：周/分钟

	//FLOAT		autoPitchSpeed;						//Edit模式下自动绕 X 轴旋转的速度
	//FLOAT		autoRoundSpeed;						//Edit模式下自动绕 Y 轴旋转的速度
	//FLOAT		autoRollSpeed;						//Edit模式下自动绕 X 轴旋转的速度

	FLOAT		manualSpeedX;					    //Edit模式下手动翻滚相机的速度绕 X 轴
	FLOAT		manualSpeedY;					    //Edit模式下手动旋转相机的速度绕 Y 轴
	//FLOAT		manualPitchSpeed;					//Edit模式下手动旋转相机的速度 Z
	//FLOAT		manualRoundSpeed;					//Edit模式下手动翻滚相机的速度 Y

	FLOAT		manualPanXSpeed;					//平移的X方向速度 
	FLOAT		manualPanYSpeed;					//平移的Y方向速度 

	FLOAT		dollySpeed;							//滚鼠标滚轮时移动相机的速度 [1, 100]
	FLOAT		dollyAcc;							//相机推拉加速度

	FLOAT		dollyNearDis;						//最近的Dolly
	FLOAT		dollyFarDis;						//最远的Dolly

	INT			rosAxis;							// 旋转轴标志位, 0: xy, 1 : x, 2 : y
	VECTOR3     LookAtPos;                          // Editor的目标点 

	DWORD       LookAtModelIndex;                   // 如果正对某个模型，返回模型的索引。如果没有模型，该值为0xffffffff
	INT         LookAtType;                         // 0：场景中心 1：场景中的一点  2：模型3：相机的目标点

}EditorInfo;


///定义Flyer的数据结构
typedef struct FlyerInfo
{
	CHAR		Name[MAX_NAMESTR_LENGTH];							//Editor的名字
	// INT			cameraIndex;						//使用的相机编号
	CHAR        Camera[MAX_NAMESTR_LENGTH];						    //使用的相机名称

	FLOAT		panSpeed;							//平移速度
	FLOAT		panAcc;								//平移加速度

	FLOAT		dollySpeed;							//相机推拉速度
	FLOAT		dollyAcc;							//相机推拉加速度

	FLOAT		dollyNearDis;						//最近的Dolly
	FLOAT		dollyFarDis;						//最远的Dolly

	FLOAT		rotateSpeed;						//相机旋转的速度
	FLOAT		rotateAcc;							//相机旋转的加速度

	FLOAT		liftSpeed;							//上升下降的速度，失去重力时有效
	FLOAT		liftAcc;							//上升下降的加速度，失去重力时有效

	FLOAT		upperAngle;							//抬头的最大仰角
	FLOAT		lowerAngle;							//低头的最大俯视角

	BOOL		bCollision;							//是否有碰撞
	FLOAT		colDis;								//碰撞距离
}FlyerInfo;

///定义摄像机属性的数据结构
// typedef struct CameraInfo
// {
// 	VECTOR3                 from;			// 摄像机的起点
// 	VGS_QUATERNION          quat;           // 转角
// 
// 	FLOAT	                nearClip;		// 摄像机的nearClip
// 	FLOAT					farClip;		// 摄像机的farClip
// 	FLOAT					fov;			// 摄像机的fov
// 	BYTE					ProjectType;    // 相机的投影方式， 0: 轴侧相机，1：透视相机
// 
// }CameraInfo;

///定义摄像机属性的数据结构
typedef struct CameraInfoEx
{
	VECTOR3					from;			// 摄像机的起点
	VECTOR3                 at;             // 相机的looat的方向等于 at - from
	VECTOR3					Up;             // Up方向

	FLOAT                   roll;           // 绕look at方向上旋转角度

	FLOAT	                nearClip;		// 摄像机的nearClip
	FLOAT					farClip;		// 摄像机的farClip
	FLOAT					fov;			// 摄像机的fov
	VGS_CAMERA_TYPE			ProjectType;    // 相机的投影方式， 0: 轴侧相机，1：透视相机

}CameraInfoEx;

typedef CameraInfoEx CameraInfo;

// 灯光类型
enum VGS_LIGHT_TYPE
{
	VLT_OMNI = 0,
	VLT_DIR,
	VLT_SPOT
};

///定义灯光属性的数据结构
typedef struct LightInfo
{
	VGS_LIGHT_TYPE	Type;  // 0: LT_POINT, 1: LT_DIRECTIONAL, 2:LT_SPOTLIGHT
    VGSRGB			Diffuse;
	VGSRGB			Ambient;
	FLOAT			Multiply;
    VECTOR3		    Position;
    VECTOR3		    Direction;
    FLOAT           Range;
    FLOAT           Falloff;
    FLOAT           Attenuation0;
    FLOAT           Attenuation1;
    FLOAT           Attenuation2;
    FLOAT           Theta;
    FLOAT           Phi;
}LightInfo;

///定义材质属性的数据结构
typedef struct MtrlInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];		//材质名
	VGSRGB				Ambient;		//环境色
    VGSRGB				Diffuse;		//漫色
	VGSRGB				Specular;		//高光颜色
	FLOAT			SpecularLevel;	   //高光强度
	FLOAT			shininess;	       //高亮范围
	// VGSRGB				Emissive;		//自发光颜色
	FLOAT           Emissive;       // 自发光值[0, 100]，真正的自发光颜色等于diffuse * Emissive
	INT				Opacity;		//透明度 [0, 100]
	BOOL			bTwoSide;		//是否双面
	BOOL			bAlphaTest;		//是否进行Alpha测试
	INT				FillMode;		//填充模式  1: 点模式， 2：线框模式 3：实体模式，缺省是3

	INT             AlphaTestValue;  // AlphaTest的门限值，小于该门限值的点会被剔除掉

	//float           USpeed;          // 动态UV的速度
	//float           VSpeed;

	BOOL            bUseDiffuse;    // 是否使用材质的diffuse颜色，如果不使用，等效于diffuse颜色为白色。

}MtrlInfo;

// 纹理贴图坐标层次
const INT TEXTURECOORD_DIFFUSE       = 0;   // diffuse
const INT TEXTURECOORD_LIGHTMAP      = 1;   // 
const INT TEXTURECOORD_REFLECT       = 2;   // 
const INT TEXTURECOORD_OPACITY       = 3;   // diffuse
const INT TEXTURECOORD_SHADOW        = 4;   // 
const INT TEXTURECOORD_BUMP          = 5;   // 
const INT TEXTURECOORD_WATER         = 6;   // diffuse
const INT TEXTURECOORD_CUBE          = 7;   // 
const INT TEXTURECOORD_DYNAMIC       = 8;   // 
const INT TEXTURECOORD_MOVIE         = 9;   // 
const INT TEXTURECOORD_FLASH         = 10;   // 

// 立方贴图的图片类型
enum VGS_CUBIC_FACE
{
	VCF_FRONT = 0,
	VCF_BACK,
	VCF_LEFT,
	VCF_RIGHT,
	VCF_TOP,
	VCF_BOTTOM
};

///定义贴图属性的数据结构
typedef struct TexInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];		//贴图名
	INT				Width;			//贴图高度
	INT				Height;			//贴图宽度
	TEXFORMAT		Format;			//格式
	INT				MipLevel;		//Mip层级			
}TexInfo;

///定义CubeMap贴图属性的数据结构
typedef struct CubeMapInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];
	INT				Size;				//贴图尺寸
	TEXFORMAT		Format;				//格式
}CubeMapInfo;

///定义BumpWater贴图属性的数据结构
typedef struct BumpWaterInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];			
	INT				Width;					//贴图的宽度和高度
	INT				Height;
	FLOAT			XScale;					//水波纹的X方向密度
	FLOAT			YScale;					//水波纹的Y方向密度
	FLOAT			Radius;					//波长
	FLOAT			Speed;					//波速
}BumpWaterInfo;

///定义MovieTex贴图属性的数据结构
typedef struct MovieTexInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];				//对象名称
	MOVIETYPE		Type;					//文件类型
	BOOL			WithAudio;				//是否播放音频
	INT				RepeatCnt;				//循环播放的次数， -1为无限次循环
}MovieTexInfo;

///定义Image的信息
typedef struct ImageInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];
	INT				FileSize;
	INT				Width;
	INT				Height;
}ImageInfo;

// 前景图、背景图属性
typedef struct OverlayInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];  // 名称
	CHAR            BaseImg[256];  // 基本图片名称
	CHAR            OpacImg[256];  // 透明图片名称

	BOOL            bRelativeSize; // 是否使用相对尺寸
	BOOL            bRelativePos;  // 是否使用相对位置
	BOOL            bVisible;      // 是否可见

	FLOAT           Opacity;       // 透明度
	FLOAT			Rotation;      // 旋转角度

	FLOAT           ActualWidth;   // 图像真实的宽度
	FLOAT           ActualHeight;  // 图像真实的高度

	VGSOFFSETORIGIN             Origin;  // 对齐时的参考原点，VGSOFFSETORIGIN值 
	INT				Width;   // 目前的宽度
	INT				Height;  // 目前的高度
	INT             Left;    // 左坐标，相对于视口左上角 
	INT             Top;     // 上坐标，相对于视口左上角 

}OverlayInfo;

///定义FRect
typedef struct FRect
{
	FLOAT			Left;
	FLOAT			Top;
	FLOAT			Right;
	FLOAT			Bottom;
	FRect(){Left = Top = Right = Bottom = 0.0f;}
	FRect(FLOAT l, FLOAT t, FLOAT r, FLOAT b){Left = l; Top = t; Right = r; Bottom = b;}
}FRect;

// operation type of open v3d
enum V3D_OPENTYPE
{
	VOP_REPLACE = 0,
	VOP_MERGE
};

// 添加包围盒的方式
enum VGS_ADDTYPE
{
	VAT_ADDTYPE_REPLACE = 0,  // 增加一个包围盒，同时取消其他的包围盒
	VAT_ADDTYPE_REVERSE,      // 本身不现实包围盒，但显示其他所有包围盒
	VAT_ADDTYPE_ADD           // 增加一个包围盒
};
// 动画类型
enum VGS_ANIMATIONTYPE
{
	VAT_ANIMATIONTYPE_MODEL = 0,        // 模型关键帧动画
	VAT_ANIMATIONTYPE_CAMERA,           // 相机关键帧动画
	VAT_ANIMATIONTYPE_LIGHT,            // 灯光动画
	VAT_ANIMATIONTYPE_SKELETON,          // 骨骼动画
	VAT_ANIMATIONTYPE_NONE
};

// 动画播放状态
enum VGS_ANIMATIONSTATUS
{
	VAS_PLAYING = 0,  // 播放
	VAS_PAUSED,       // 暂停
	VAS_STOP,          // 停止
	VAS_NONE
};

// gui类型
enum VGS_GUIOBJ_TYPE
{
	VGS_GUIOBJ_TYPE_NONE = 0,  
	VGS_GUIOBJ_TYPE_OVERLAY,      // 前景图
	VGS_GUIOBJ_TYPE_BACKDROP,     // 背景图
	VGS_GUIOBJ_TYPE_BUTTON,       // 按钮 
	VGS_GUIOBJ_TYPE_NAVIGATION,    // 导航图
	VGS_GUIOBJ_TYPE_NAVIPOINTOR,    // 导航图指针
	VGS_GUIOBJ_TYPE_FLASH			//Flash控件GUI.
};

// 坐标轴类型
enum VGS_AXIS_TYPE
{
	VGS_AXIS_TYPE_NONE = 0,  // 不设置正对的轴

	// 自身坐标系坐标
	VGS_LAXIS_X,
	VGS_LAXIS_Y,
	VGS_LAXIS_Z,
	VGS_LAXIS_NEGATIVEX,  // -x
	VGS_LAXIS_NEGATIVEY,  // -y
	VGS_LAXIS_NEGATIVEZ,  // -z

	// 世界坐标系坐标
	VGS_WAXIS_X,
	VGS_WAXIS_Y,
	VGS_WAXIS_Z,
	VGS_WAXIS_NEGATIVEX,
	VGS_WAXIS_NEGATIVEY,
	VGS_WAXIS_NEGATIVEZ,

	// 父节点坐标系坐标
	VGS_PAXIS_X,
	VGS_PAXIS_Y,
	VGS_PAXIS_Z,
	VGS_PAXIS_NEGATIVEX,
	VGS_PAXIS_NEGATIVEY,
	VGS_PAXIS_NEGATIVEZ
};

// transform(平移、旋转、缩放)操作的参考坐标系
enum VGS_TRANSFORMSPACE
{
	VTS_WORLD = 0,
	VTS_LOCAL,
	VTS_PARENT
};

// 旋转的参考类型
enum VGS_ROTATIONSPACE
{
	VRS_WORLD = 0,
	VRS_WORLD2,
	VRS_LOCAL
};

// 反射类型
enum VGS_REFLECT_TYPE
{
	VRT_INVALID = -1,
	VRT_PLANAR,
	VRT_CURVED,
	VRT_REFLECTION,
	VRT_NORMAL
};

// 动画播放速度无效值
#define VGS_ANIMSPEED_NONE (-10000)


///////////// texture的混色方式 /////////////////////

// texture的类型
// enum VGS_TextureType
// {
// 	VTT_INVALID = 0,  // 无效贴图
// 	VTT_BASAL,		  // 基本贴图
// 	VTT_CUBIC,
// 	VTT_DYNAMIC,
// 	VTT_MOVIE,
// 	VTT_FLASH,
// };
// texture的类型
enum VGS_TextureType
{
	//VTT_INVALID = 0,  // 无效贴图
	//VTT_BASAL,		  // 基本贴图
	//VTT_CUBIC,
	//VTT_DYNAMIC,
	//VTT_MOVIE,
	//VTT_FLASH,
	//VTT_BUMP
	TLT_INVALID = 0,
	TLT_BASAL,						// 基础贴图
	TLT_OPACITY,		            // 透明贴图
	TLT_REFLECT,                    // 反射贴图
	TLT_REALTIMESHADOW,             // 实时阴影,暂时不支持
	TLT_CUBEMAP,                    // 立方图，包括静态立方贴图和实时反射立方贴图
	TLT_DYNAMIC,                    // 动态贴图
	TLT_RTFMAP,						// 实时反射,暂时不支持
	TLT_MOVIE,                      // 电影贴图,暂时不支持
	TLT_FLASH,						// Flash贴图,暂时不支持
	TLT_BUMPMAP,					// 凹凸贴图，包括凹凸波纹，凹凸环境反射，法线贴图等
	TLT_HEIGHTMAP,					// 高度贴图,暂时不支持
	TLT_RTTMAP						// 实时折射,暂时不支持
};

// bump类型
enum VGS_BumpType
{
	VBT_BUMPINVALID = 0,
	VBT_BUMPWATER,           // bump 水特效
	VBT_BUMPENVIRONMENT,     // bump 环境反射
	VBT_BUMPNORMARLMAP       // 法线贴图
};

// cube map类型
enum VGS_CubemapType
{
	VCT_INVALID = -1,
	VCT_STATIC,      // 静态贴图
	VCT_REALTIME     // 实时反射 
};

// bumpwater参数
typedef struct VGS_BumpwaterTextureParam
{
	INT             width;
	INT			    height;
	FLOAT			XScale;					//水波纹的X方向密度
	FLOAT			YScale;					//水波纹的Y方向密度
	FLOAT			Radius;					//波长
	FLOAT			Speed;					//波速
	
	FLOAT           CenterX;	// 波纹的中心点坐标
	FLOAT			CenterY;   
}VGS_BumpwaterTextureParam;

// bump环境反射
typedef struct VGS_BumpEnvTextureParam
{
	INT width;
	INT height;
	float   noise1;
	float   noise2;
}VGS_BumpEnvTextureParam;

// 法线贴图
typedef struct VGS_BumpNormalTextureParam
{
	DWORD   LightHandle;
}VGS_BumpNormalTextureParam;

typedef union VGS_BumpParam
{
	VGS_BumpwaterTextureParam    waterParam;
	VGS_BumpEnvTextureParam      envParam;
	VGS_BumpNormalTextureParam   normalParam;
	
}VGS_BumpParam;

// 混色类型
enum VGS_BlenderType
{
	VBT_COLOR = 0,
	VBT_ALPHA
};

// 混色方式
enum VGS_BlenderOP
{
	VBO_DISABLE = 0,  // 使用source2
	VBO_REPLACE,      // 用source1替换source2
	VBO_MODULATE,     // source2 * source1
	VBO_MODULATE2,    // VBO_MODULATE * 2
	VBO_MODULATE4,    // VBO_MODULATE * 4	
	VBO_ADD,          // source2 + source1        
    VBO_ADD_SIGNED,	/// as LBX_ADD, but subtract 0.5 from the result        
    VBO_ADD_SMOOTH,	/// as LBX_ADD, but subtract product from the sum
    VBO_SUBTRACT,	  // source1 - source2
	VBO_BLEND_DIFFUSE_ALPHA,		/// use interpolated alpha value from vertices to scale source1, then add source2 scaled by (1-alpha)
    VBO_BLEND_TEXTURE_ALPHA,		/// as LBX_BLEND_DIFFUSE_ALPHA, but use alpha from texture        
    VBO_BLEND_CURRENT_ALPHA,		/// as LBX_BLEND_DIFFUSE_ALPHA, but use current alpha from previous stages       
    VBO_BLEND_MANUAL,			/// as LBX_BLEND_DIFFUSE_ALPHA but use a constant manual blend value (0.0-1.0)        
    VBO_DOTPRODUCT,					/// dotproduct of color1 and color2         
    VBO_BLEND_DIFFUSE_COLOUR	/// use interpolated color values from vertices to scale source1, then add source2 scaled by (1-color)        
    
};

 enum VGS_LayerBlendSource
{
    /// the colour as built up from previous stages
    VBS_CURRENT,
    /// the colour derived from the texture assigned to this layer
    VBS_TEXTURE,
    /// the interpolated diffuse colour from the vertices
    VBS_DIFFUSE,
    /// the interpolated specular colour from the vertices
    VBS_SPECULAR,
    /// a colour supplied manually as a separate argument
    VBS_MANUAL
};

// 混色参数
typedef struct VGS_BlenderParam
{
	//VGS_LayerBlendSource source1;
	//VGS_LayerBlendSource source2;
	float BlenderOP_alpha;  // 当VGS_BlenderOP == VBO_BLEND_MANUAL时有效，混色模式中用到的参数alpha,

}VGS_BlenderParam;

// texturelayer的混色信息
typedef struct VGS_BlenderInfo
{
	VGS_BlenderOP       option;       // 
	VGS_BlenderParam    param;        // 混色参数
}VGS_BlenderInfo;

// 模型属性信息
typedef struct VGS_ModelInfo
{
	// CHAR  sName[MAX_NAMESTR_LENGTH];
	BOOL  isBillboard;
	BOOL  isCollision;
	BOOL  isGround;
	BOOL  isVisible;
	BOOL  isClickable;                // 播放状态下是否可点击
	VGS_AXIS_TYPE   dirctionAxis;     // 模型正对相机的方向轴
}VGS_ModelInfo;

// 3d对象类型
enum VGS_MOVABLETYPE
{
	VGS_MOVABLETYPE_NONE = 0,
	VGS_MOVABLETYPE_MODEL,
	VGS_MOVABLETYPE_CAMERA,
	VGS_MOVABLETYPE_LIGHT
};

// 鼠标事件类型
enum VGS_MOUSEEVENT_TYPE
{
	VGS_MOUSEEVENT_TYPE_NONE = 0,
	VGS_MOUSEEVENT_TYPE_OVER,         // 
	VGS_MOUSEEVENT_TYPE_DBCLICK,      
	VGS_MOUSEEVENT_TYPE_LEFTDOWN,
	VGS_MOUSEEVENT_TYPE_LEFTUP,
	VGS_MOUSEEVENT_TYPE_RIGHTDOWN,
	VGS_MOUSEEVENT_TYPE_RIGHTUP,
	VGS_MOUSEEVENT_TYPE_MOVE,         // 移动
	VGS_MOUSEEVENT_TYPE_LEAVE,        // 离开 
	VGS_MOUSEEVENT_CNT
};


// 浏览器类型
typedef enum WEBBROWSERTYPE
{
	WBT_IE = 0,
	WBT_FF,
	WBT_GOOGLE,
	WBT_OPERA,
	WBT_SAFARI
}WEBBROWSERTYPE;

// 浏览器类型
typedef enum WEBBROWSER_SUBTYPE
{
	WBST_IE      = 0,
	WBST_MAXTHON,
	WBST_TENCENT
}WEBBROWSER_SUBTYPE;

extern WEBBROWSERTYPE  g_BrowserType/* = WBT_IE*/;        // 浏览器类型，0 ：ie 1：ff 2：google
extern WEBBROWSER_SUBTYPE g_BrowserSubType/* = WBST_IE*/;


#pragma pack(pop)

//}
//
//using namespace VGS;

#endif