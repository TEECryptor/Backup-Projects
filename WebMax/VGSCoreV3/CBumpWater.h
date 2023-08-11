/////////////////////////////////////////////////////////////////////////
// 名  称: CBumpWater
// 描  述: 水特效类，通过创建CBumpWater类对象，实现类似于水面的特效
// 作  者: lixin
// 日  期: 2006-11-20
/////////////////////////////////////////////////////////////////////////

#ifndef _CBUMPWATER_H
#define _CBUMPWATER_H

#include "VGSHeader.h"
#include "CSysFunc.h"


inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
#define _T(x)       __T(x)
#define __T(x)      x

#define D3DAPPERR_MEDIANOTFOUND       0x8200000b


class VGS_EXPORT CBumpWater  
{
private:
	LPDIRECT3DDEVICE9			m_pd3dDevice;			//d3d设备

public:
	BOOL CreateBumpMap(INT dwWidth, 
					   INT dwHeight,
					   INT xSize,
					   INT ySize,
					   FLOAT radius,
					   FLOAT speed);
	//创建多层材质混合的渲染表面
    BOOL                        CreateMultiLayerRenderTarget(D3DPRESENT_PARAMETERS	d3dpp, UINT width, UINT height);
	VOID                        ReleaseMultiLayerRenderTarget();
	VOID						Release();

public:
	CHAR						name[32];
	INT							index;

	INT							width;					//bumpmap的尺寸
	INT							height;
	D3DMATRIX					bumpMat;
    UINT						m_n;               // Number of vertices in the ground grid along X
    UINT						m_m;               // Number of vertices in the ground grid along Z
	UINT						m_nTriangles;      // Number of triangles in the ground grid
	float                       m_fTime;
	LPDIRECT3DTEXTURE9			pBumpTex;            //BumpMap贴图 
	INT							m_xSize;
	INT							m_ySize;			//波纹密度
	FLOAT						m_speed;			//波纹运动的速度
	FLOAT						m_radius;			//波纹振幅
	BOOL                        multiLayer;         //是否是多层纹理
    //如果是多层纹理的bumpwater效果，则将多层纹理先渲染成一幅纹理后再与bumpmap进行混色
	UINT                        renderWidth;               //创建的渲染表面的宽度
	UINT                        renderHeight;              //创建的渲染表面的高度
    
	LPDIRECT3DTEXTURE9			curRenderTexture;             //当前后缓冲对应的渲染到纹理(Render-to-Texture)
	LPDIRECT3DSURFACE9			curRenderSurface; 
	LPDIRECT3DSURFACE9          curRenderZSurface;

	std::string                 getTextrueFileName(){return std::string(name);}           

public:
	CBumpWater(LPDIRECT3DDEVICE9 pD3DDevice);
	~CBumpWater();

};

#endif




