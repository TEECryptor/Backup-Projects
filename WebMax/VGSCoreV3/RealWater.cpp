// RealWater.cpp: implementation of the CRealWater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RealWater.h"
#include "resource\FX.h"

enum rs_mode 
{
	rsm_normal=0,
	rsm_refraction,
	rsm_reflection,
	rsm_reflection_backface,
};

//--------------------------------------------------------------


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRealWater::CRealWater(CHAR* name)
{
	m_Name = std::string(name);

	g_depthstencil = NULL;
	m_terrain_effect = NULL;

	m_pCameraView = NULL;
	m_RTTModels.clear();
	m_RTFModels.clear();
	
	m_bSupportGPUNormal = FALSE;
}

CRealWater::~CRealWater()
{
	
}

//初始化
HRESULT CRealWater::Init(LPDIRECT3DDEVICE9 pd3dDevice, CCamera* pCamera, D3DXVECTOR3* position, int gridsize_x, int gridsize_y, BOOL bSupportGPUNormal)
{
	m_pd3dDevice = pd3dDevice;
	m_bSupportGPUNormal = bSupportGPUNormal;

	if (m_bSupportGPUNormal)
	{
		m_gridsize_x = gridsize_x;
		m_gridsize_y = gridsize_y;
	}
	else //因为没有高度图和法线图，所以CPU近似模拟水面的效果，要提高水面的顶点数量
	{
		m_gridsize_x = gridsize_x * 2;
		m_gridsize_y = gridsize_y * 2;
	}
	
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW);		
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pd3dDevice->GetDepthStencilSurface(&g_depthstencil);
	
	// initialize camera objects
	m_pCameraView = pCamera;
	
	// create parameter handler
	m_prm = new parameterhandler();
	m_prm->set_float( p_fStrength,	1.9f );
	m_prm->set_bool(	p_bDisplace,	true );
	m_prm->set_int(	p_iOctaves,		8 );
	m_prm->set_float( p_fScale,		0.38f );
	m_prm->set_float( p_fFalloff,		0.607f );
	m_prm->set_float( p_fAnimspeed,	1.4f );
	m_prm->set_float( p_fTimemulti,	1.27f );
	m_prm->set_bool(	p_bPaused,		false );
	
	m_prm->set_float(	p_fLODbias,		0.0f);
	m_prm->set_bool(	p_bDisplayTargets, false );
	m_prm->set_float(	p_fElevation,	7.0f );
	
	//m_prm->set_float( p_fSunPosAlpha,		2.7f );
	//m_prm->set_float( p_fSunPosTheta,		0.1f );
	m_prm->set_float( p_fSunPosAlpha,		1.38f );
	m_prm->set_float( p_fSunPosTheta,		1.09f );
	m_prm->set_float( p_fSunShininess,	84.0f );
	m_prm->set_float( p_fSunStrength,		12.0f );

	if(m_bSupportGPUNormal)
		m_prm->set_bool(	p_bSmooth,			true );
	else
		m_prm->set_bool(	p_bSmooth,			false );
	
	m_prm->set_float(	p_bReflRefrStrength,0.1f );
	
	m_prm->set_float( p_fWaterColourR,	0.17f );
	m_prm->set_float( p_fWaterColourG,	0.27f );
	m_prm->set_float( p_fWaterColourB,	0.26f );
	
	m_prm->set_bool(	p_bAsPoints,		false );
	m_prm->set_bool(	p_bDrawDuckie,		true );
	m_prm->set_bool(	p_bDrawIsland,		false );
	m_prm->set_bool(	p_bDiffuseRefl,		false );
	
	m_prm->set_active_parameter(p_fStrength);
	
	
	m_prm->set_float( p_fScale,		0.38f );
	m_prm->set_float( p_fStrength,	11.3f );
	m_prm->set_float( p_fFalloff,		0.56f );
	m_prm->set_float( p_fWaterColourR,	0.17f );
	m_prm->set_float( p_fWaterColourG,	0.27f );
	m_prm->set_float( p_fWaterColourB,	0.26f );
	m_prm->set_float(	p_fLODbias,			0.0f);
	m_prm->set_bool(	p_bAsPoints,		false );
	m_prm->set_bool(	p_bDrawDuckie,		true );
	m_prm->set_bool(	p_bDrawIsland,		true );
	m_prm->set_float( p_fSunPosAlpha,		7.85f );
	m_prm->set_float( p_fSunPosTheta,		0.30f );
	m_prm->set_float( p_fSunShininess,	1263.0f );
	m_prm->set_float( p_fSunStrength,		5270.0f );
	m_prm->set_bool(	p_bDiffuseRefl,		true );
	
	
	// create sea object
	m_pSea = new CSurface(&D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0), m_gridsize_x, m_gridsize_y,
						m_pd3dDevice, m_pCameraView, m_prm, m_bSupportGPUNormal);
	
	//创建特效
	HRESULT hr = InitEffect();
	
	return hr;
}

//创建效果
HRESULT CRealWater::InitEffect()
{
	// load terrain underwater effect
	char *errortext;
	LPD3DXBUFFER errors;
	D3DXHANDLE hTechnique;
	/*
	char RealFilePath[256];
	FindMediaFile(RealFilePath, "terrain_underwater.fx");
	D3DXCreateEffectFromFile(m_pd3dDevice, RealFilePath, 
		NULL, NULL, 0, NULL, &terrain_effect, &errors );*/

    //从特效缓存中创建特效
	HRESULT hr1 =   D3DXCreateEffect(m_pd3dDevice, g_FXTerUnderWaterBuffer, 
		(sizeof(g_FXTerUnderWaterBuffer) - 1), NULL, NULL, NULL,
		NULL, &m_terrain_effect, &errors);

	if (errors != NULL)
	{
		errortext = (char*) errors->GetBufferPointer();
		MessageBox(NULL, (errortext), TEXT("lalala.exe"), MB_OK);	
		return E_FAIL;
	}
	
	m_terrain_effect->FindNextValidTechnique(NULL, &hTechnique);    
	m_terrain_effect->SetTechnique(hTechnique);
	
	return S_OK;
}

//增加RTT折射模型，作为水面渲染得反射和折射物体
void CRealWater::addRTTModel(CModel* pModel)
{
	m_RTTModels.push_back(pModel);
}

//移除RTT折射模型
void CRealWater::eraseRTTModel(CModel* pModel)
{
	int RTTModelCnt = m_RTTModels.size();
	std::vector<CModel*>::iterator ppModel = m_RTTModels.begin();
	for (int i = 0; i< RTTModelCnt; i++)
	{
		CModel* _pModel = *(ppModel + i);
		if (_pModel == pModel)
		{
			m_RTTModels.erase(ppModel + i);
			return;
		}
	}
}

//增加RTF反射模型
void CRealWater::addRTFModel(CModel* pModel)
{
	m_RTFModels.push_back(pModel);
}

void CRealWater::eraseRTFModel(CModel* pModel)
{
	int RTFModelCnt = m_RTFModels.size();
	std::vector<CModel*>::iterator ppModel = m_RTFModels.begin();
	for (int i = 0; i< RTFModelCnt; i++)
	{
		CModel* _pModel = *(ppModel + i);
		if (_pModel == pModel)
		{
			m_RTFModels.erase(ppModel + i);
			return;
		}
	}
}

//设置相机
VOID CRealWater::SetViewCamera(CCamera* pCamera)
{
	m_pCameraView = pCamera;
	m_pSea->rendering_camera = pCamera;
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws everything
//-----------------------------------------------------------------------------
VOID CRealWater::Render()
{	
	static DWORD it=0;
	static DWORD time_prepare, time_total, time_total_start;
	it++;
	// measure fps
	if (it>63)
	{
		time_total = timeGetTime() - time_total_start;
		time_total_start = timeGetTime();
		it = 0;
	}
	
	// update mouse & kbd
	DWORD time_prepare_start = timeGetTime();
	m_pSea->prepare(m_pCameraView);
	time_prepare = timeGetTime() - time_prepare_start;

	// Setup the world, view, and projection matrices (this is used to render the frustum)
	SetupMatrices();
	
	// render reflections/refractions into textures
	//#ifdef REFRACTION
	//渲染折射效果
	render_refracted_scene();
	//#endif
	//渲染反射效果
	render_reflected_scene();		
	
	// set rendering states		
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,true);
	m_pd3dDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,false);		
	
	//渲染天空盒
	if (!m_prm->get_bool(p_bAsPoints))	
		 m_pSea->render_skybox();
	
	//渲染海面
	m_pSea->render();	

	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW  );	
}

//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
VOID CRealWater::SetupMatrices()
{
	// D3DXMATRIXA16 matWorld,matProj;
	
	D3DXMatrixIdentity(&m_MatWorld);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_MatWorld);
	m_pCameraView->getViewMatrix(&m_MatView);
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_MatView);
	m_pCameraView->getProjectMatrix(&m_MatProj);
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_MatProj);
}


void CRealWater::render_refracted_scene()
{
	// set rendertarget
	LPDIRECT3DSURFACE9 target,bb;
	m_pd3dDevice->GetRenderTarget(0, &bb );
	m_pSea->surf_refraction->GetSurfaceLevel( 0,&target );
	m_pd3dDevice->SetRenderTarget(0, target);
	m_pd3dDevice->SetDepthStencilSurface( m_pSea->depthstencil );
	
	m_pd3dDevice->Clear( 0, NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
		D3DCOLOR_XRGB((int)(255*m_prm->params[p_fWaterColourR].fData),
		(int)(255*m_prm->params[p_fWaterColourG].fData),
		(int)(255*m_prm->params[p_fWaterColourB].fData)), 1.0f, 0 );

	//设置折射的剪切面
	float plane[4];
	plane[0] = 0;
	plane[1] = -1;
	plane[2] = 0;
	plane[3] = 1.7 * m_prm->params[p_fStrength].fData;	// a slight offset to avoid seams
	m_pd3dDevice->SetClipPlane(0, plane);
	m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, 1);
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW  );

	D3DXMATRIXA16 store;	
	m_pd3dDevice->GetTransform(D3DTS_WORLD,&store);

	//渲染折射效果
	render_scene(rsm_refraction);
	
	// restore
	m_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
	m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, 0);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &store);	
	m_pd3dDevice->SetRenderTarget(0, bb);
	m_pd3dDevice->SetDepthStencilSurface( g_depthstencil );

	SAFE_RELEASE_D3D(target);
	SAFE_RELEASE_D3D(bb);
}

void CRealWater::render_reflected_scene()
{
	// set rendertarget
	LPDIRECT3DSURFACE9 target,bb;
	m_pd3dDevice->GetRenderTarget(0, &bb );
	m_pSea->surf_reflection->GetSurfaceLevel( 0,&target );
	m_pd3dDevice->SetRenderTarget(0, target);
	m_pd3dDevice->SetDepthStencilSurface( m_pSea->depthstencil );
	
	// alpha & z must be cleared
	m_pd3dDevice->Clear( 0, NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW  );
	
	// add a clip-plane as well	
	float plane[4];
	plane[0] = 0;
	plane[1] = -1;
	plane[2] = 0;
	plane[3] = 0;
	m_pd3dDevice->SetClipPlane(0, plane);
	m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, 1);
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, true);

	D3DXMATRIXA16 store;	
	m_pd3dDevice->GetTransform(D3DTS_WORLD,&store);

	render_scene(rsm_reflection);

	// restore
	m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, 0);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &store);	
	m_pd3dDevice->SetRenderTarget(0, bb);
	m_pd3dDevice->SetDepthStencilSurface( g_depthstencil );

	SAFE_RELEASE_D3D(target);
	SAFE_RELEASE_D3D(bb);
}

void CRealWater::render_scene(int mode)
{
	// set lightning-----------------------------------------------------------------------------
	sun.Direction.x = -cos(m_prm->get_float(p_fSunPosTheta))*sin(m_prm->get_float(p_fSunPosAlpha));
	sun.Direction.y = -sin(m_prm->get_float(p_fSunPosTheta));
	sun.Direction.z = -cos(m_prm->get_float(p_fSunPosTheta))*cos(m_prm->get_float(p_fSunPosAlpha));
	if(mode==rsm_reflection)
		sun.Direction.y = -sun.Direction.y;
	sun.Diffuse.r = 2.0f;
	sun.Diffuse.g = 2.0f;
	sun.Diffuse.b = 2.0f;
	sun.Diffuse.a = 1.0f;
	sun.Ambient.a = 1.0f;
	sun.Ambient.r = 0.2f;
	sun.Ambient.g = 0.3f;
	sun.Ambient.b = 0.3f;
	sun.Specular.r = 1.0f;
	sun.Specular.g = 1.0f;
	sun.Specular.b = 1.0f;
	sun.Specular.a = 1.0f;
	
	sun.Attenuation0 = 1.0f;
	sun.Type = D3DLIGHT_DIRECTIONAL;
	m_pd3dDevice->SetLight(2, &sun);
	m_pd3dDevice->LightEnable(2, true);		

	sun.Diffuse.r = 1.0f;
	sun.Diffuse.g = 1.0f;
	sun.Diffuse.b = 1.0f;
	m_pd3dDevice->SetLight(2, &sun);
	// set lightning OK -----------------------------------------------------------------------------

	//折射特效---------------------------------------------------------------------------------------
	if (mode == rsm_refraction)
	{															
		D3DXMATRIXA16 scale;
		D3DXMatrixScaling( &scale, 1, 0.75, 1 );
	
		m_terrain_effect->Begin(NULL,NULL);
		m_terrain_effect->BeginPass(0);		
		
		float sa = m_prm->params[p_fSunPosAlpha].fData;
		float st = m_prm->params[p_fSunPosTheta].fData;
		m_terrain_effect->SetVector("sun_vec",&D3DXVECTOR4(cos(st)*sin(sa), sin(st), cos(st)*cos(sa),0));
		m_terrain_effect->SetVector("watercolour", 
									&D3DXVECTOR4(m_prm->params[p_fWaterColourR].fData,
									m_prm->params[p_fWaterColourG].fData,
									m_prm->params[p_fWaterColourB].fData,1));

		//渲染折射模型
		int RTTModelCnt = m_RTTModels.size();
		std::vector<CModel*>::iterator ppModel = m_RTTModels.begin();
		for (int i = 0; i< RTTModelCnt; i++)
		{
			CModel* pModel = *(ppModel + i);

			//设置变换矩阵
			D3DXMATRIX matWorldViewProj = pModel->m_pNode->getPosition(VTS_WORLD);
			D3DXMatrixMultiply(&matWorldViewProj, &matWorldViewProj, &m_MatProj);
			m_terrain_effect->SetMatrix("mWorldViewProj", &matWorldViewProj);

			pModel->render();
		}

		m_terrain_effect->EndPass();		
		m_terrain_effect->End();
	} 

	//反射
	if (mode == rsm_reflection)
	{					
		// mirror the scene
		D3DXMATRIX scale;
		D3DXMatrixScaling( &scale, 1, -1, 1 );

		//渲染反射模型
		int RTFModelCnt = m_RTFModels.size();
		std::vector<CModel*>::iterator ppModel = m_RTFModels.begin();
		for (int i = 0; i< RTFModelCnt; i++)
		{
			CModel* pModel = *(ppModel + i);
			D3DXMATRIX currentModelWorldMat = pModel->m_pNode->getPosition(VTS_WORLD);
			D3DXMatrixMultiply(&currentModelWorldMat, &currentModelWorldMat, &scale);
			m_pd3dDevice->SetTransform(D3DTS_WORLD, &currentModelWorldMat); 

			pModel->render();
		}
	}
	
	// restore stuff
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, false);		
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW  );
}


void CRealWater::set_clipplane_height(float height)
{
	float plane[4];
	
	m_pd3dDevice->GetClipPlane(0,plane);
	
	plane[3] = height;
	
	m_pd3dDevice->SetClipPlane(0,plane);
}

void CRealWater::Invalidate()
{	
	SAFE_RELEASE_D3D(g_depthstencil);
	SAFE_RELEASE_D3D(m_terrain_effect);
	m_pSea->Invalidate();
}


void CRealWater::RestoreDeviceObject()
{
	m_pd3dDevice->GetDepthStencilSurface(&g_depthstencil);
	InitEffect();
	m_pSea->RestoreDeviceObject();
}

void CRealWater::Release()
{
	Invalidate();

	SAFE_DELETE(m_prm);
	
	SAFE_RELEASE(m_pSea);
	SAFE_DELETE(m_pSea);
}