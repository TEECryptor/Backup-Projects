
#pragma once

#include "d3dx9math.h"

/////////// 定义常量 ////////////////////////////////////
/// 名字最长256个字节，包括'\0'
const INT _MAX_NAMESTR_LENGTH  = 64;    // 对象的名称最长不超过64
const INT _MAX_ERRORSTR_LENGTH = 128;
const INT _MAX_FILENAME_LENGTH = 256;   // 文件完整路径及名称，不超过256字节

// 预定义相机的名称
const char CAMERA_NAME_FRONT[]       = "VGS_FrontCamera";
const char CAMERA_NAME_BACK[]        = "VGS_BackCamera";
const char CAMERA_NAME_LEFT[]        = "VGS_leftCamera";
const char CAMERA_NAME_RIGHT[]       = "VGS_RightCamera";
const char CAMERA_NAME_TOP[]         = "VGS_TopCamera";
const char CAMERA_NAME_BOTTOM[]      = "VGS_BottomCamera";
const char CAMERA_NAME_PERSPECTIVE[] = "VGS_PerspectiveCamera";

// 缺省的相机远近剪切面
const INT g_CamDefaultNearClip = 5;
const INT g_CamDefaultfarClip  = 10000 * g_CamDefaultNearClip;

//关键帧动画播放的帧率
const int	gkfrPlaySpeed = 25;

// 二维控件相对于窗口的位置关系
enum OFFSETORIGIN
{
	TOP_LEFT = 0,  // 左上角
	TOP_MIDDLE,    // 最顶部的中点
	TOP_RIGHT,     // 右上角
	MIDDLE_LEFT,   // 最左边的中点
	CENTER,        // 中心
	MIDDLE_RIGHT,  // 最右边的中点
	BOTTOM_LEFT,   // 左下角
	BOTTOM_MIDDLE, // 最下面的中点
	BOTTOM_RIGHT   // 右下角
};

#pragma pack(push, 1) // 数据结构以单字节对齐

// GUI 对象的基本参数
struct GUI_CommonProperty
{
	char            sName[_MAX_NAMESTR_LENGTH];
	float			left;   // 如果使用相对位置，保存的是百分比，如果使用的是绝对位置，保存的是绝对坐标
	float			top;  
	float			width;	// 如果使用相对大小，保存的是百分比，如果使用的是绝对大小，保存的是绝对大小
	float			height;
	bool			bIsRelativePos;
	bool			bIsRelativeSize;
	OFFSETORIGIN	OffsetOrigin;
	int             Opacity;   // [0, 100]
	bool            bVisible;
	int             ZOrder;  // 上下的层次顺序，越大越靠上
};

#pragma pack(pop)


#define WM_GUI_SENDTEXT	            (WM_USER + 0x130) // ChatInputBox发送文字
#define WM_GUI_COMBOBOXCHANGED	    (WM_USER + 0x131) // combbobox选择内容变化
#define WM_GUI_BUTTONCLICK   	    (WM_USER + 0x132) // 按钮click
#define WM_GUI_NAVIGATORCLICK       (WM_USER + 0x133) // 导航图click
#define WM_GUI_SCROLLBARPOS    	    (WM_USER + 0x134) // 滚动条滚动
#define WM_GUI_PROGRESSBARPOS  	    (WM_USER + 0x135) // 进度条进度更新
#define WM_GUI_TEXTBOX       	    (WM_USER + 0x136) // textbox按下回车


const INT  DEFAULT_MIPMAP         = 3;    // 缺省的mipmap
const INT  DEFAULT_ALPHATESTVALUE = 64;   // 缺省的alphaTest值

// 向量常量
#define  VGS_ZEROVECTOR3			D3DXVECTOR3(0, 0, 0)
#define  VGS_XVECTOR3				D3DXVECTOR3(1, 0, 0)
#define  VGS_YVECTOR3				D3DXVECTOR3(0, 1, 0)
#define  VGS_ZVECTOR3				D3DXVECTOR3(0, 0, 1)
#define  VGS_ONEVECTOR3				D3DXVECTOR3(1, 1, 1)
#define  VGS_ZEROQUATERNION			D3DXQUATERNION(0, 0, 0, 1)
