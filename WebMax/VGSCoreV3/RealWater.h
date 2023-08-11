// RealWater.h: interface for the CRealWater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALWATER_H__2FBBE04B_55AE_456E_896C_79381783A409__INCLUDED_)
#define AFX_REALWATER_H__2FBBE04B_55AE_456E_896C_79381783A409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSHeader.h"

#include "CSysFunc.h"
#include "CCamera.h"
#include "CModel.h"

#include "surface.h"
#include "parameterhandler.h"
#include "software_noisemaker.h"

class VGS_EXPORT CRealWater  
{
public:
	std::string				m_Name;					//水面对象的名字

	LPDIRECT3DSURFACE9		g_depthstencil;			//主场景深度缓冲
	LPD3DXEFFECT			m_terrain_effect;		//折射特效对象
	CCamera*				m_pCameraView;			//当前相机
	vector<CModel*>			m_RTTModels;			//需要进行折射的模型
	vector<CModel*>			m_RTFModels;			//需要反射的模型


	int						m_gridsize_x;			//水面X方向的网格数
	int						m_gridsize_y;			//水面Y方向的网格数	
	CSurface*				m_pSea;					//海面Surface对象
	parameterhandler*		m_prm;					//海面调节的参数
	D3DLIGHT9				sun;					//太阳的光照

	BOOL                    m_bSupportGPUNormal;		//是否支持64位浮点贴图


public:
	//增加RTT折射模型，作为水面渲染得反射和折射物体
	void					addRTTModel(CModel* pModel);
	//移除RTT折射模型
	void					eraseRTTModel(CModel* pModel);
	
	//增加RTF反射模型
	void					addRTFModel(CModel* pModel);
	void					eraseRTFModel(CModel* pModel);

public:
	LPDIRECT3DDEVICE9		m_pd3dDevice;

public:
	//初始化
	HRESULT Init(LPDIRECT3DDEVICE9 pd3dDevice, CCamera* pCamera, D3DXVECTOR3* position, int gridsize_x, int gridsize_y, BOOL bSupportTex64);
	//创建效果
	HRESULT CRealWater::InitEffect();
	//设置相机
	VOID	SetViewCamera(CCamera* pCamera);

	VOID	Render();
	VOID	SetupMatrices();
	void	render_refracted_scene();
	void	render_reflected_scene();
	void	render_scene(int mode);
	void	set_clipplane_height(float height);

	void	Invalidate();
	void	RestoreDeviceObject();
	void	Release();


public:
	CRealWater(CHAR* name);
	virtual ~CRealWater();

private:

	D3DXMATRIX m_MatWorld, m_MatView, m_MatProj;

};

#endif // !defined(AFX_REALWATER_H__2FBBE04B_55AE_456E_896C_79381783A409__INCLUDED_)
