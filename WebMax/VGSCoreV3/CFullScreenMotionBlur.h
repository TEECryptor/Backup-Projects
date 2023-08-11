////////////////////////////////////////////////////////////////////////////////////////////////
// 名称: CFullScreenMotionBlur类型定义--全屏动态模糊特效
// 描述: 对整个场景进行动态模糊的实现，使用了DX8中的Alpha混合和渲染到纹理(Render to texture)技术
// 作者: zyx
// 日期: 2006-12-11
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CGLOBALMOTIONBLUR_H
#define _CGLOBALMOTIONBLUR_H

#include "VGSHeader.h"
#include "CSysFunc.h"

//##ModelId=47EBC74E003E
class VGS_EXPORT CFullScreenMotionBlur
{
private:
	//##ModelId=47EBC74E0040
	LPDIRECT3DDEVICE9	m_pd3dDevice;		
	//##ModelId=47EBC74E004F
	RECT*				m_pRect;
	//##ModelId=47EBC74E0054
	D3DPRESENT_PARAMETERS*	m_pd3dpp;
	//##ModelId=47EBC74E0058
	BOOL				m_bSupport;			//当前显示卡是否支持FSMotionBlur

	//##ModelId=47EBC74E005A
	D3DXMATRIX*			m_pMatWorld;
	//##ModelId=47EBC74E005F
	D3DXMATRIX*			m_pMatView;
	//##ModelId=47EBC74E0064
	D3DXMATRIX*			m_pMatProj;

	//##ModelId=47EBC74E0069
	D3DXMATRIX			m_MatIdentity;
	//##ModelId=47EBC74E006E
	D3DXMATRIX			m_MatView;
	//##ModelId=47EBC74E0073
	D3DXMATRIX			m_MatProj;

	//##ModelId=47EBC74E0078
	LPDIRECT3DSURFACE9	m_pCurrentBuffer;                    //当前后缓存表面
	//##ModelId=47EBC74E007D
	LPDIRECT3DTEXTURE9	m_pCurrentRenderTexture;             //当前后缓冲对应的渲染到纹理(Render-to-Texture)
	//##ModelId=47EBC74E0082
	LPDIRECT3DSURFACE9	m_pCurrentRenderSurface;             //当前纹理的渲染表面
	//##ModelId=47EBC74E0087
	LPDIRECT3DSURFACE9	m_pDisplayZSurface;                  //Z缓存表面
	//##ModelId=47EBC74E008C
	LPDIRECT3DSURFACE9	m_pZBuffer;						    //Z缓存
	//##ModelId=47EBC74E0091
	LPDIRECT3DVERTEXBUFFER9 m_pVB;                          //用来显示效果的矩形顶点缓冲
	//##ModelId=47EBC74E0095
	BYTE m_Amount;											//动态模糊的程度,即纹理的Alpha通道值(0-255),数值越小，效果越强


public:
	//##ModelId=47EBC74E0096
	BOOL m_bBlurFlag;                                       //设置是否打开动态模糊
	
public:
	//创建顶点缓冲
	//##ModelId=47EBC74E0097
	BOOL	CreateVB();
	//##ModelId=47EBC74E0098
	VOID	Invalidate();
	//##ModelId=47EBC74E0099
	BOOL InitEffect();								//初始化特效
	//##ModelId=47EBC74E009A
	VOID SetRenderTarget();
	//##ModelId=47EBC74E009B
	VOID RenderEffect();								//渲染特效
	//##ModelId=47EBC74E009C
	VOID SetBlurAmount(BYTE);                               //设置模糊强度
	//##ModelId=47EBC74E009E
	VOID SetupNewMatrices();							//设置矩形的坐标变换
	//##ModelId=47EBC74E009F
	VOID Release();                                         //清除资源

public:
	//##ModelId=47EBC74E00A0
	CFullScreenMotionBlur(LPDIRECT3DDEVICE9	pd3dDevice, 
							RECT* pRect,
							D3DPRESENT_PARAMETERS* pd3dpp,
							D3DXMATRIX*	pMatWorld,
							D3DXMATRIX* pMatView,
							D3DXMATRIX* pMatProj);
	//##ModelId=47EBC74E00AF
	~CFullScreenMotionBlur();
};

#endif
