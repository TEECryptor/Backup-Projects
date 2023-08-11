/////////////////////////////////////////////////////////////////////////
// 名称: CInput.h--CInput class
// 描述: 非DX输入设备的处理类，包含鼠标键盘的输入的读取
// 作者: lixin	
// 日期: 2006-07-22
////////////////////////////////////////////////////////////////////////

#ifndef _CINPUT_H
#define _CINPUT_H

//-----------------------------------------------------------------------
#include "..\\VGSCoreV3\\VGSDef.h"
#include "CSysFunc.h"

//鼠标的信息结构体
//typedef struct MOUSEINFO
//{
//	SHORT		X;
//	SHORT		Y;				//坐标
//	BOOL		LButton;		//按下为1，松开为0
//	BOOL		RButton;
//	FLOAT		RollDis;		//滚轮
//}MOUSEINFO;

//-----------------------------------------------------------------------
class CInput  
{
public:
	//保存键盘数据的缓冲
	BYTE					keyBoardBuf[256];
	//保存鼠标数据的缓冲
	MOUSEINFO				mouseInfo;
	//释放所有资源
	VOID					Release();

public:
	CInput();
	~CInput();

};

#endif