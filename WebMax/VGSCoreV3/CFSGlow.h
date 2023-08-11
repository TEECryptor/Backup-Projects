////////////////////////////////////////////////////////////////////////////////////////////////
// 名称: CFSGLow类型定义--全屏光晕特效
// 描述: 对整个场景进行动态模糊叠加实现，使用了DX8中的Alpha混合和渲染到纹理(Render to texture)技术
// 作者: 李欣
// 日期: 2007-04-11
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CFSGLOW_H
#define _CFSGLOW_H

#include "VGSHeader.h"
#include "CSysFunc.h"
#include <d3d9types.h>

class VGS_EXPORT CFSGlow
{
private:
	LPDIRECT3DDEVICE9			m_pd3dDevice;		
	RECT*						m_pRect;
	D3DPRESENT_PARAMETERS*		m_pd3dpp;
	BOOL						m_bSupport;			//当前显示卡是否支持FSMotionBlur

	D3DXMATRIX*					m_pMatWorld;
	D3DXMATRIX*					m_pMatView;
	D3DXMATRIX*					m_pMatProj;

	D3DXMATRIX					m_MatIdentity;
	D3DXMATRIX					m_MatView;
	D3DXMATRIX					m_MatProj;

	LPDIRECT3DSURFACE9			m_pCurrentBuffer;                    //当前后缓存表面
	LPDIRECT3DTEXTURE9			m_pCurrentRenderTexture;             //当前后缓冲对应的渲染到纹理(Render-to-Texture)
	LPDIRECT3DSURFACE9			m_pCurrentRenderSurface;             //当前纹理的渲染表面
	LPDIRECT3DSURFACE9			m_pDisplayZSurface;                  //Z缓存表面
	LPDIRECT3DSURFACE9			m_pZBuffer;						    //Z缓存
	LPDIRECT3DVERTEXBUFFER9		m_pVB;								//用来显示效果的矩形顶点缓冲
	
	//全屏光晕的强度参数
	//模糊度-纹理的Alpha通道值(0-255),数值越小，效果越强
	BYTE						m_GlowAmount;	
	//透明度
	FLOAT						m_OpacityValue;	
	//对比度


public:
	BOOL						m_bGlowFlag;                         //设置是否打开全屏高光效果
	
public:
	//创建顶点缓冲
	BOOL						CreateVB();
	//释放顶点缓冲
	VOID						Invalidate();
	//初始化特效
	BOOL						InitEffect();
	//设置渲染表面
	VOID						SetRenderTarget();
	//渲染特效
	VOID						RenderEffect();	
	//设置模糊强度
	VOID						SetGlowAmount(INT amount);    
	//设置矩形的坐标变换
	VOID						SetupNewMatrices();
	//清除资源
	VOID						Release();                                         

public:
	CFSGlow(LPDIRECT3DDEVICE9	pd3dDevice, 
			RECT* pRect,
			D3DPRESENT_PARAMETERS* pd3dpp,
			D3DXMATRIX*	pMatWorld,
			D3DXMATRIX* pMatView,
			D3DXMATRIX* pMatProj);
	~CFSGlow();
};

#endif
