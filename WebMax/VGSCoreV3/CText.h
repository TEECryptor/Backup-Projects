/////////////////////////////////////////////////////////////////////////
// 名称: CText 类型定义--文字类
// 描述: CText 类型定义了场景中的文字单元
// 作者: 李欣	
// 日期: 2006-05-19
////////////////////////////////////////////////////////////////////////

#ifndef _CTEXT_H
#define _CTEXT_H

#include "CSysFunc.h"
#include "CFont.h"

//关键帧信息
//##ModelId=47EBC72F0000
typedef struct _TextKfrInfo
{
	INT				x;
	INT				y;
	DWORD			color;
}_TextKfrInfo;

//##ModelId=47EBC72F0002
class VGS_EXPORT CText
{
public:
	//##ModelId=47EBC72F0003
	INT					index;
	//##ModelId=47EBC72F0004
	CHAR				name[32];

	BOOL				visible;

	//使用的字体对象
	//##ModelId=47EBC72F0006
	CFont*				m_pFont;	

	//##ModelId=47EBC72F000F
	CHAR*				strText;		//字符串
	//##ModelId=47EBC72F0011
	RECT				rect;			//文字的位置
	//##ModelId=47EBC72F0015
	UINT				uFormat;		//文字显示的格式
	//##ModelId=47EBC72F0017
	D3DCOLOR			color;			//文字的颜色

public:
	//关键帧动画
	//##ModelId=47EBC72F001B
	INT							m_kfrCnt;			//关键帧数量
	//##ModelId=47EBC72F001D
	_TextKfrInfo*				m_kfrInfo;			//关键帧数据

public:
	//释放资源
	//##ModelId=47EBC72F0021
	VOID				Release();

public:
	//##ModelId=47EBC72F0022
	CText(CFont* pFont);
	//##ModelId=47EBC72F0024
	~CText();

};

#endif
