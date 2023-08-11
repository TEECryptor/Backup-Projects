// RTFMap.h: interface for the CRTFMap class.
// Real Time Reflection 实时反射贴图类，用于处理场景中需要实时反射的贴图
// 2009-01-02
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTFMAP_H__7F51C9E2_8B33_48B5_AEDA_A09F237DFFED__INCLUDED_)
#define AFX_RTFMAP_H__7F51C9E2_8B33_48B5_AEDA_A09F237DFFED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSHeader.h"
#include "CSysFunc.h"
#include "CTexture.h"

#include <vector>
using std::vector;

class VGS_EXPORT CRTFMap : public CTexture
{
public:
	LPDIRECT3DDEVICE9			m_pd3dDevice;	//D3D Device对象
	D3DPRESENT_PARAMETERS*		m_pd3dpp;

	//贴图的尺寸
	INT							m_width;
	INT							m_height;

	//渲染本反射贴图用到的显存表面，不直接使用贴图的表面，因为直接渲染到贴图表面抗锯齿无法实现，所以只能先渲染到指定表面，然后拷贝到贴图表面
	LPDIRECT3DSURFACE9			m_pRenderTarget;
	//渲染本反射贴图用到的ZBuffer
	LPDIRECT3DSURFACE9			m_pZBuf;

	//每隔多少帧绘制一次，默认为1
	INT							m_Interval;
	
	//反射平面				
	D3DXPLANE					m_refPlane;
	
	//反射变换矩阵
	D3DXMATRIX					m_refMatrix;
	//反射贴图的投影矩阵
	D3DXMATRIX					m_texProjMatrix;

	//该RTFMap反射的模型
	vector<DWORD>				m_refModelList;

	CModel*                     m_pRefPlaneModel;  // 确定反射平面的模型
	CMaterial*                  m_pMaterial;  // 所属的材质

public://getter/setter
	size_t	GetReflectedModelCount() const { return m_refModelList.size(); }
	DWORD	GetReflectedModelByIndex(size_t idx);
	/** 设置贴图的宽高，可能需要重新创建贴图.
	*/
	void	SetSize(int nWidth, int nHeight);
public:
	//初始化贴图对象
	HRESULT						Init();
	//通过点和方向量设置反射平面
	VOID						SetRefPlaneByModel(D3DXVECTOR3 position, D3DXVECTOR3 normal);
	//通过模型设置反射平面
	VOID						SetRefPlaneByModel(DWORD modelHandle);
	//为反射贴图增加反射模型
	VOID						AddRefModel(DWORD modelHandle);
	//将渲染表面的图像拷贝到贴图表面
	VOID						UpdateTexBuf();
	//清空表面
	VOID						Clear();
	//释放
	VOID						Invalidate();
	//重置
	HRESULT						Restore(D3DXMATRIX texProj);
	//销毁
	VOID						Release();

public:
	CRTFMap(const std::string & name, LPDIRECT3DDEVICE9 pd3dDevice, 
			D3DPRESENT_PARAMETERS* pd3dpp, INT width, INT height, D3DFORMAT format);
	virtual ~CRTFMap();

};

#endif // !defined(AFX_RTFMAP_H__7F51C9E2_8B33_48B5_AEDA_A09F237DFFED__INCLUDED_)
