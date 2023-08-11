/////////////////////////////////////////////////////////////////////////
// 名称: CFont类型定义--字体类
// 描述: CFont类型定义了创建字体的方法，以及绘制文字的方法
// 作者: 李欣	
// 日期: 2006-05-19
////////////////////////////////////////////////////////////////////////

#ifndef _CFONT_H
#define _CFONT_H

#include "VGSHeader.h"
#include "CSysFunc.h"

//##ModelId=47EBC74E0148
class VGS_EXPORT CFont  
{
private:
	//##ModelId=47EBC74E014A
	LPDIRECT3DDEVICE9	m_pd3dDevice;					//d3d设备接口
	//##ModelId=47EBC74E014F
	LPD3DXFONT			m_pd3dFont;						//d3d字体接口
	//##ModelId=47EBC74E0154
	D3DXFONT_DESC		m_LOGFONT;						//Windows字体
	
public:
	//##ModelId=47EBC74E0158
	CHAR				name[32];						//名称
	//##ModelId=47EBC74E0159
	INT					index;							//编号

	//CHAR				m_FontName[32];					//字体名称
public:
	
	//##ModelId=47EBC74E015A
	VOID				Init(CHAR* fontName);			//初始化字体对象

	//重置
	//##ModelId=47EBC74E015C
	VOID				Restore();					

	//##ModelId=47EBC74E015D
	VOID				SetFont(CHAR* strFontName);		
	//##ModelId=47EBC74E015F
	VOID				GetFont(CHAR* strFontName);

	//##ModelId=47EBC74E0167
	VOID				SetHeight(INT height);			//设置文字高度
	//##ModelId=47EBC74E0169
	INT					GetHeight();					//得到文字高度
		
	//##ModelId=47EBC74E016A
	VOID				SetBold(BOOL isBold);			//设置是否粗体显示
	//##ModelId=47EBC74E016C
	BOOL				GetBold();						//得到是否粗体显示

	//##ModelId=47EBC74E016D
	VOID				SetItalic(BOOL isItalic);		//设置是否斜体显示
	//##ModelId=47EBC74E016F
	BOOL				GetItalic();					//得到是否斜体显示

	//##ModelId=47EBC74E0170
	VOID				SetUnderline(BOOL isUnderline); //设置是否显示下划线
	//##ModelId=47EBC74E0172
	BOOL				GetUnderline();					//得到是否显示下划线

	//##ModelId=47EBC74E0173
	VOID				DrawText(CHAR* strText, RECT* pRect, UINT uFormat, DWORD color); //绘制文字
	
	//##ModelId=47EBC74E017B
	VOID				Release();

public:
	//##ModelId=47EBC74E017C
	CFont(LPDIRECT3DDEVICE9 pDevice);
	//##ModelId=47EBC74E017E
	~CFont();

};

#endif
