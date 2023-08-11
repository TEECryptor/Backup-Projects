// BumpMap.h: interface for the CBumpMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUMPMAP_H__AD3EF571_366D_46D4_AF05_17574CE7C6CC__INCLUDED_)
#define AFX_BUMPMAP_H__AD3EF571_366D_46D4_AF05_17574CE7C6CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VGSDef.h"
#include "CTexture.h"
#include "CLight.h"

inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
//#define _T(x)       __T(x)
//#define __T(x)      x

//Bump贴图的类型
typedef VGS_BumpType BUMPMAPTYPE;

class VGS_EXPORT CBumpMap : public CTexture
{
public:
	LPDIRECT3DDEVICE9			m_pd3dDevice;
	D3DPRESENT_PARAMETERS*		m_pd3dpp;

    LPDIRECT3DTEXTURE9			m_pBumpEnvMap;        // The 环境凹凸贴图 bumpmap
    LPDIRECT3DTEXTURE9			m_pNormalMap;        // The actual bumpmap

	BUMPMAPTYPE					m_type;				//Bump贴图的类型
    D3DFORMAT					m_BumpMapFormat;    // Bumpmap texture format

	CLight*						m_Light;			//法线贴图使用的灯光
	D3DXVECTOR3					m_LightDir;			//灯光的方向
	DWORD						m_dwFactor;			//法线贴图用到的Factor值

	CMaterial*                  m_pMaterial;  // 所属的材质
	//--------------------------------------------------------------------------
	//BumpWave的相关参数
    LPDIRECT3DTEXTURE9			m_pBumpWaveMap;        // The actual bumpmap
	INT							m_MapWidth;				//波纹凹凸贴图的宽度
	INT							m_MapHeight;			//波纹凹凸贴图的宽度
	INT							m_xSize;			//波纹的密度
	INT							m_ySize;
	FLOAT						m_radius;			//波长
	FLOAT						m_speed;			//波速

	FLOAT                       m_CenterX;          //波中心点坐标
    FLOAT                       m_CenterY;          //波中心点坐标

	//BumpEnv的相关参数
	FLOAT                       m_Noise1;
	FLOAT                       m_Noise2;

	//不直接使用贴图的表面，因为直接渲染到贴图表面抗锯齿无法实现，所以只能先渲染到指定表面，然后拷贝到贴图表面
	LPDIRECT3DSURFACE9			m_pRenderTarget;
	LPDIRECT3DTEXTURE9			m_compositeTexForBump;	//为凹凸贴图合成的贴图，如果贴图层中有凹凸，则先将其它层渲染到一个贴图，再将该贴图与凹凸混色
	D3DXMATRIX					m_compositeTexPorjMatrix;	//合成后的贴图的投影矩阵

	
	//初始化BumpEnv贴图
	HRESULT						InitBumpEnvMap(const std::string &sFileName, float noise1, float noise2);
	
	//初始化Bump法线贴图
	HRESULT						InitBumpNormalMap(const std::string &sFileName, CLight* pLight);
	
	//初始化Bump波纹贴图
	HRESULT						InitBumpWaveMap(INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY);
	
	//创建用于合成其它图层的表面和贴图
	HRESULT						InitCompositeRenderTarget();

	// 计算环境贴图
	VOID ComputeBumpEnviromentmap();

	VOID ComputeBumpNormalmap();

	// 计算bump wave
	VOID ComputeBumpWavemap();
public:
	HRESULT SetBumpAsWavemap(INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY);
	HRESULT SetBumpAsEnviromentmap(const std::string &sFileName, float noise1, float noise2);
	HRESULT SetBumpAsNormalmap(const std::string &sFileName, CLight* pLight);
	
	/** 用一张图像创建bump的基本贴图缓冲区.本应放在CTexture类中，因改变较大，当前先放这里.--added by linxq --2009-6-9
	@param strFileName in: 基本图像的文件名.
	@return : 返回E_FAIL，则创建失败.
	*/
	HRESULT CreateBumpBaseTexture(const std::string& strFileName);
	/** 改变Bump环境贴图的图像文件.
	@param strFileName in: 基本图像的文件名.
	@param nNoise1 in: 噪声1.
	@param nNoise2 in: 噪声2.
	@return : S_OK，则设置成功.
	*/
	HRESULT ChangeEnvironmentMapImageFile(const std::string &sFileName, float noise1, float noise2);
	/** 设置Bump环境贴图的噪声参数.
	@param nNoise1 in: 噪声1.
	@param nNoise2 in: 噪声2.
	@return : S_OK，则设置成功.
	*/
	HRESULT SetEnvironmentMapNoise(float fNoise1, float fNoise2);

	
	// 设置bump wave的参数
	HRESULT  SetBumpwaveParam(INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY);
	HRESULT  SetBumpNormalParam(const std::string &sFileName, CLight* pLight);
	
	//更新波纹贴图
	VOID						UpdateBumpWaveMap(INT stageIndex);	

	//设置计算法线贴图凹凸效果使用的光源
	VOID						SetBumpMapLight(CLight* pLight);
	//光源的位置转化为颜色
	DWORD						VectortoRGBA(FLOAT fHeight);
	//将渲染表面的图像拷贝到贴图表面
	VOID						UpdateTexBuf();
	//清空表面
	VOID						Clear();
	//释放 
	VOID						Invalidate();
	//重置
	VOID						Restore(D3DXMATRIX matrix);
	//释放贴图资源
	VOID						Release();

	LPDIRECT3DTEXTURE9          getBumpWavemap(){return m_pBumpWaveMap;}
	LPDIRECT3DTEXTURE9          getBumpEnviromentmap(){return m_pBumpEnvMap;}
	LPDIRECT3DTEXTURE9          getBumpNormalmap(){return m_pNormalMap;}


public:
	CBumpMap(LPDIRECT3DDEVICE9 pd3dDevice, D3DPRESENT_PARAMETERS* pd3dpp, D3DFORMAT format);//, INT width);
	~CBumpMap();


};

#endif // !defined(AFX_BUMPMAP_H__AD3EF571_366D_46D4_AF05_17574CE7C6CC__INCLUDED_)
