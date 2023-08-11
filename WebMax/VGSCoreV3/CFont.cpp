/////////////////////////////////////////////////////////////////////////
// 名称: CFont类型定义--字体类
// 描述: CFont类型定义了创建字体的方法，以及绘制文字的方法
// 作者: 李欣	
// 日期: 2006-05-19
////////////////////////////////////////////////////////////////////////

#include "CFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74E017C
CFont::CFont(LPDIRECT3DDEVICE9 pDevice)
{
	index = -1;
	m_pd3dDevice = pDevice;
	m_pd3dFont = NULL;
}

//##ModelId=47EBC74E017E
CFont::~CFont()
{
}

//-------------------------------------------------------------------
//方法: Init()
//参数: 
//用途: 初始化字体对象
//-------------------------------------------------------------------
//##ModelId=47EBC74E015A
VOID CFont::Init(CHAR* fontName)
{
	//创建d3dFont接口指针对象
	m_LOGFONT.Height = 13;
	m_LOGFONT.Width = 0;
	m_LOGFONT.Weight = 0;
	m_LOGFONT.Italic = 0;
	m_LOGFONT.CharSet = DEFAULT_CHARSET;
	m_LOGFONT.OutputPrecision = OUT_DEFAULT_PRECIS;
	m_LOGFONT.Quality = DEFAULT_QUALITY;
	//m_LOGFONT.FaceName = DEFAULT_PITCH;
	strcpy(m_LOGFONT.FaceName, fontName);

	D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
}

//重置
//##ModelId=47EBC74E015C
VOID CFont::Restore()
{
	SAFE_RELEASE_D3D(m_pd3dFont);
	D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
}

//-------------------------------------------------------------------
//方法: SetFont()
//参数: 
//用途: 设置字体
//-------------------------------------------------------------------
//##ModelId=47EBC74E015D
VOID CFont::SetFont(CHAR* strFontName)
{
	if (m_pd3dFont)
	{
		m_pd3dFont->GetDesc(&m_LOGFONT);
		strcpy(m_LOGFONT.FaceName, strFontName);

		SAFE_RELEASE_D3D(m_pd3dFont);
		D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
	}
}

//获得字体
//##ModelId=47EBC74E015F
VOID CFont::GetFont(CHAR* strFontName)
{
	if (m_pd3dFont)
	{
		m_pd3dFont->GetDesc(&m_LOGFONT);
		strcpy(strFontName, m_LOGFONT.FaceName);
	}
}	

//-------------------------------------------------------------------
//方法: SetHeight()
//参数: 
//用途: 设置文字高度
//-------------------------------------------------------------------
//##ModelId=47EBC74E0167
VOID CFont::SetHeight(INT height)
{
	if (m_pd3dFont)
	{
		m_pd3dFont->GetDesc(&m_LOGFONT);
		m_LOGFONT.Height = height;

		SAFE_RELEASE_D3D(m_pd3dFont);
		D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
	}
}

//得到文字高度
//##ModelId=47EBC74E0169
INT CFont::GetHeight()
{
	if (m_pd3dFont)
		m_pd3dFont->GetDesc(&m_LOGFONT);
	return m_LOGFONT.Height;
}
			
//-------------------------------------------------------------------
//方法: SetBold()
//参数: 
//用途: 设置是否粗体显示
//-------------------------------------------------------------------
//##ModelId=47EBC74E016A
VOID CFont::SetBold(BOOL isBold)
{
	if (m_pd3dFont)
	{
		m_pd3dFont->GetDesc(&m_LOGFONT);

		if (isBold)
			m_LOGFONT.Weight = FW_BOLD;
		else
			m_LOGFONT.Weight = FW_NORMAL;

		SAFE_RELEASE_D3D(m_pd3dFont);
		D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
	}
}

//得到是否粗体显示
//##ModelId=47EBC74E016C
BOOL CFont::GetBold()
{
	if (m_pd3dFont)
		m_pd3dFont->GetDesc(&m_LOGFONT);
	
	if (m_LOGFONT.Weight == FW_BOLD)
		return TRUE;
	else
		return FALSE;
}
					
//-------------------------------------------------------------------
//方法: SetItalic()
//参数: 
//用途: 设置是否斜体显示
//-------------------------------------------------------------------
//##ModelId=47EBC74E016D
VOID CFont::SetItalic(BOOL isItalic)
{
	if (m_pd3dFont)
	{
		m_pd3dFont->GetDesc(&m_LOGFONT);
		m_LOGFONT.Italic = TRUE;

		SAFE_RELEASE_D3D(m_pd3dFont);
		D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
	}
}

//获得是否斜体
//##ModelId=47EBC74E016F
BOOL CFont::GetItalic()
{
	if (m_pd3dFont)
		m_pd3dFont->GetDesc(&m_LOGFONT);
	return m_LOGFONT.Italic;
}

//-------------------------------------------------------------------
//方法: SetUnderline()
//参数: 
//用途: 设置是否显示下划线
//-------------------------------------------------------------------
//##ModelId=47EBC74E0170
VOID CFont::SetUnderline(BOOL isUnderline)
{
// 	if (m_pd3dFont)
// 	{
// 		m_pd3dFont->GetDesc(&m_LOGFONT);
// 		m_LOGFONT.Underline = TRUE;
// 
// 		SAFE_RELEASE_D3D(m_pd3dFont);
// 		D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
// 	}
}
//##ModelId=47EBC74E0172
BOOL CFont::GetUnderline()
{
// 	if (m_pd3dFont)
// 		m_pd3dFont->GetDesc(&m_LOGFONT);
// 	return m_LOGFONT.Underline;
	return FALSE;
}

//-------------------------------------------------------------------
//方法: DrawText()
//参数: 
//用途: 绘制文字
//-------------------------------------------------------------------
//##ModelId=47EBC74E0173
VOID CFont::DrawText(CHAR* strText, RECT* pRect, UINT uFormat, DWORD color)
{	
	if (m_pd3dFont)
	{
// 		m_pd3dFont->Begin();
		m_pd3dFont->DrawText(NULL,strText, -1, pRect, uFormat, color);
//		m_pd3dFont->End();
	}
} 

//-------------------------------------------------------------------
//方法: Release()
//参数: 
//用途: 释放内存
//-------------------------------------------------------------------
//##ModelId=47EBC74E017B
VOID CFont::Release()
{
	SAFE_RELEASE_D3D(m_pd3dFont);
}
