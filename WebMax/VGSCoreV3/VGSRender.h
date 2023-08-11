// VGSRender.h: interface for the CVGSRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VGSRENDER_H__545DE559_E9EC_4D27_9E4D_350A4CC38046__INCLUDED_)
#define AFX_VGSRENDER_H__545DE559_E9EC_4D27_9E4D_350A4CC38046__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSHeader.h"
#include "CD3DDevice.h"
#include "CFont.h"
#include "Viewport.h"
//#include "lua/VgsLua.h"
#include "vgsdef.h"
class VGS_EXPORT CVGSRender  
{
public:
	//D3D对象变量----------------------------------------------------------------------------------
	//##ModelId=47EBC7290178
 	LPDIRECT3D9				m_pd3d;							//Direct3D对象
	//##ModelId=47EBC729017D
	LPDIRECT3DDEVICE9		m_pd3dDevice;					//D3D Device对象
	
	//##ModelId=47EBC7290181
	HWND					m_hWnd;							//主窗体

public:
	//D3D显示设备相关的变量------------------------------------------------------------------------
	//##ModelId=47EBC7290183
	CD3DDevice*				m_DeviceInfo;					//当前系统的所有设备信息

	//##ModelId=47EBC729018A
	D3DPRESENT_PARAMETERS	m_d3dpp;						//当前的显卡参数
	//##ModelId=47EBC729018F
	D3DDISPLAYMODE			m_d3dmm;						//当前的显存参数
	//##ModelId=47EBC7290194
	D3D_PARAM				d3dParam;

	//##ModelId=47EBC7290198
	INT						m_antiLevel;					//当前的抗拒齿级别

	//##ModelId=47EBC729019A
    LPDIRECT3DSURFACE9		m_pBackBuffer;					//显卡后缓冲表面
	//##ModelId=47EBC729019F
	LPDIRECT3DSURFACE9		m_pZBuffer;						//显卡ZBuffer表面

	//##ModelId=47EBC72901A4
	CFont*					m_pFont;							//字体对象

public:
	//初始化VGS引擎
	//##ModelId=47EBC72901A8
	HRESULT					Init(HWND hWnd, WEBBROWSERTYPE type = WBT_IE);
	//初始化d3d设备
	//##ModelId=47EBC72901AA
	HRESULT					InitD3DDevice();
	//更新
	//##ModelId=47EBC72901AB
	HRESULT					Update(CViewport *pViewport);
	//释放d3d设备和资源
	//##ModelId=47EBC72901AC
	VOID					Release();

	// 是否支持cubemap
	BOOL IsSupportCUBEMAP();

	/** 设置浏览器类型
	@param type INT : 0 IE, 1 ff, 2 google, 3 opera, 4 safari
	*/
	VOID SetWebBrowserType(WEBBROWSERTYPE type){m_BrowserType = type;}

	/** 获取浏览器类型
	@return : 0 IE, 1 ff, 2 google, 3 opera, 4 safari
	*/
	WEBBROWSERTYPE  GetWebBrowserType(){return m_BrowserType;}

	/** 设置IE浏览器的子类型
	@param type INT : 0 IE, 1 maxthon, 2 tencent
	*/
	VOID SetWebBowserSubtype(WEBBROWSER_SUBTYPE subType){m_BrowserSubtype = subType;}

	/** 获取IE浏览器的子类型
	@return : 0 IE, 1 maxthon, 2 tencent
	*/
	WEBBROWSER_SUBTYPE  GetWebBowserSubtype(){return m_BrowserSubtype;}

public:
	//##ModelId=47EBC72901AD
	CVGSRender();
	//##ModelId=47EBC72901AE
	~CVGSRender();

private:
	LPDIRECT3DSURFACE9 m_DepthStencilBuffer;

	WEBBROWSERTYPE          m_BrowserType;                  // 浏览器类型
	WEBBROWSER_SUBTYPE      m_BrowserSubtype;               // 如果是IE浏览器，表示何种IE浏览器

};

// extern CVGSRender g_VGSRender;  // 全局的渲染器对象

#endif // !defined(AFX_VGSRENDER_H__545DE559_E9EC_4D27_9E4D_350A4CC38046__INCLUDED_)
