/////////////////////////////////////////////////////////////////////////////////////////
// 名  称: CLensFlare
// 描  述: 镜头光斑特效类, 光斑对象用COverlay对象来实现
//			实际上镜头光斑类是一组Overlay对象按照一定的前后关系和平面位置放置好，
//			然后根据视角动态改变Overlay的位置即可。
//			Overlay没有放入场景Overlay列表
// 作  者: zhangfan
// 日  期: 2006-12-15
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CLENSFLARE_H
#define _ClENSFLARE_H

#include ".\GUI\COverlay.h"
#include <vector>
using std::vector;

//##ModelId=47EBC74900AB
class VGS_EXPORT CLensFlare  
{
private:
	//D3D对象变量--------------------------------------------------------
	//##ModelId=47EBC74900AD
	LPDIRECT3DDEVICE9			m_pd3dDevice;			//d3d设备
	//##ModelId=47EBC74900B2
	RECT*						m_pRect;				//场景Rect指针

	//贴图变量--------------------------------------------------------
	//##ModelId=47EBC74900BC
	LPDIRECT3DTEXTURE9			m_pSunTex;				//光源的贴图
	//##ModelId=47EBC74900C1
	LPDIRECT3DTEXTURE9			m_pTex1;				//第一种Spot的贴图
	//##ModelId=47EBC74900C6
	LPDIRECT3DTEXTURE9			m_pTex2;				//第二种Spot的贴图
	//##ModelId=47EBC74900CB
	LPDIRECT3DTEXTURE9			m_pTex3;				//第三种Spot的贴图

	//##ModelId=47EBC74900D0
	RECT						m_formatRect;			//标准Rect

public:
	//##ModelId=47EBC74900D5
	vector<COverlay*>			m_Spot;					//存放Spot Overlay列表

	//太阳的位置--三维世界坐标系中的位置
	//##ModelId=47EBC74900DA
	D3DXVECTOR3                 m_vSunPos;				//光源位置
	//投影到屏幕坐标中的位置
	//##ModelId=47EBC74900DE
	int                         m_fSunScreenPosX;		//光源的屏幕坐标的x值
	//##ModelId=47EBC74900DF
	int                         m_fSunScreenPosY;		//光源的屏幕坐标的y值
	//##ModelId=47EBC74900E0
	int                         m_fSunScreenPosZ;		//光源的屏幕坐标的z值

private:
	//创建Spot
	//##ModelId=47EBC74900E1
	COverlay*	CreateSpot(FLOAT relativeSize,  
						FLOAT refLineY,
						LPDIRECT3DTEXTURE9 pTex,
						D3DCOLOR color);
	//创建LensFlare
	//##ModelId=47EBC74900EC
	HRESULT	Create(BYTE* pSunTex, 
					   INT	sunTexSize,
					   BYTE* pTex1,
					   INT tex1Size,
					   BYTE* pTex2, 
					   INT tex2Size,
					   BYTE* pTex3,
					   INT tex3Size);

public:
	//##ModelId=47EBC74900FD
	CLensFlare(LPDIRECT3DDEVICE9 pD3DDevice,
					   RECT* pRect,
					   BYTE* pSunTex, 
					   INT	sunTexSize,
					   BYTE* pTex1,
					   INT tex1Size,
					   BYTE* pTex2, 
					   INT tex2Size,
					   BYTE* pTex3,
					   INT tex3Size,
					   D3DXVECTOR3* pSunPos);
	//##ModelId=47EBC7490110
	~CLensFlare();

	//释放内存
	//##ModelId=47EBC7490119
	VOID		Release();
};

#endif











































