#include "surface.h"
#include "CSysFunc.h"
#include <stdio.h>
#include "tools.h"

//装载特效文件
#include "resource\FX.h"

//装载水面渲染需要的贴图
#include "resource\fresnel_water_linear.h"
#include "resource\reflection_underwater.h"
#include "resource\XZNoise.h"


/*--------------------------------------------------------------------------------------------------------------
CSurface

constructor
--------------------------------------------------------------------------------------------------------------*/

CSurface::CSurface(D3DXVECTOR3 *pos, 
				 D3DXVECTOR3 *n, 
				 int size_x, 
				 int size_y, 
				 LPDIRECT3DDEVICE9 device,
				 CCamera *renderingcamera,
				 parameterhandler *prm,
				 BOOL bSupportGPUNormal)
{
	initialized = true;
	D3DXPlaneFromPointNormal( &plane, pos, n);
	D3DXVec3Normalize(&(this->normal), n);
	m_bSupportGPUNormal = bSupportGPUNormal;

	// calculate the u and v-vectors
	// take one of two vectors (the one further away from the normal) and force it into the plane
	D3DXVECTOR3 x;
	if(fabs( D3DXVec3Dot(&D3DXVECTOR3(1,0,0),&normal)) < fabs(D3DXVec3Dot(&D3DXVECTOR3(0,0,1),&normal)))
	{
		x = D3DXVECTOR3(1,0,0);
	} 
	else 
	{
		x = D3DXVECTOR3(0,0,1);
	}
	u = x - normal*D3DXVec3Dot(&normal,&x);
	D3DXVec3Normalize(&u,&u);
	// get v (cross)
	D3DXVec3Cross(&v,&u,&normal);

	this->prm = prm;
	this->pos = *pos;
	this->device = device;
	this->gridsize_x = size_x+1;
	this->gridsize_y = size_y+1;
	this->rendering_camera = renderingcamera;
	this->projecting_camera = NULL;
	this->observing_camera = NULL;
	this->rendermode = RM_SOLID;
	this->boxfilter = false;

	surf_fresnel = NULL;
	underwater_fresnel = NULL;
	noise2D = NULL;
	sky_cubemap = NULL;


	set_displacement_amplitude(0.0f);
	//初始化海面模型的顶点缓冲
	if (!initbuffers()) initialized = false;		// init vertex & indexbuffers
	//创建顶点的波浪网格，然后拷贝数据到顶点缓冲
	software_brus = new software_noisemaker(gridsize_x,gridsize_y, prm, device, m_bSupportGPUNormal);

	//读入水面需要的几张小图片，从缓冲中，对应几个.h文件
	D3DXCreateTextureFromFileInMemory(device, fresnel_water_linear, sizeof(fresnel_water_linear), &surf_fresnel);
	D3DXCreateTextureFromFileInMemory(device, reflection_underwater, sizeof(reflection_underwater), &underwater_fresnel);
	D3DXCreateTextureFromFileInMemory(device, XZNoise, sizeof(XZNoise), &noise2D);
	

	/*
	FindMediaFile(realPath, "textures/fresnel_water_linear.bmp");
	if( FAILED( D3DXCreateTextureFromFile( device, realPath, &surf_fresnel ) ) )
	{
		MessageBox(NULL, "Could not find fresnelmap", "Textures.exe", MB_OK);
		initialized = false;		
	}
	FindMediaFile(realPath, "textures/reflection_underwater.bmp");
	if( FAILED( D3DXCreateTextureFromFile( device, realPath, &underwater_fresnel ) ) )
	{
		MessageBox(NULL, "Could not find underwater fresnelmap", "Textures.exe", MB_OK);
		initialized = false;		
	}
	FindMediaFile(realPath, "textures/XZnoise.png");
	if( FAILED( D3DXCreateTextureFromFile( device, realPath, &noise2D ) ) )
	{
		MessageBox(NULL, "Could not find noise texture", "Textures.exe", MB_OK);
		initialized = false;		
	}
			
	//if( FAILED( D3DXCreateCubeTextureFromFile( device, "textures/evening.dds", &sky_cubemap ) ) )

	
	char realPath[256];
	FindMediaFile(realPath, "textures/cubemap-evul.dds");
	if( FAILED( D3DXCreateCubeTextureFromFile( device, realPath, &sky_cubemap ) ) )
	{		
		MessageBox(NULL, "Could not find cubemap", "asdf", MB_OK);
		initialized = false;
	}*/

	//创建用于渲染水面反射和折射的贴图表面
	device->CreateTexture(reflrefrdetail,reflrefrdetail,1,D3DUSAGE_RENDERTARGET, 
							D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surf_reflection,NULL);	
	device->CreateTexture(reflrefrdetail,reflrefrdetail,1,D3DUSAGE_RENDERTARGET, 
							D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surf_refraction,NULL);		
	device->CreateDepthStencilSurface( reflrefrdetail, reflrefrdetail,D3DFMT_D24S8, 
							D3DMULTISAMPLE_NONE, 0, true, &depthstencil, NULL );

	if(!initialized)
		MessageBox(NULL, TEXT("Something went wrong in initialization of the class CSurface"), TEXT("CSurface"), MB_OK);

	this->LoadEffect();
}

void CSurface::LoadEffect()
{
	//HRESULT hr;
	//TCHAR sz[512];
	//DXUtil_FindMediaFileCb(sz, sizeof(sz), _T("stuff.fx"));
	//LPD3DXEFFECT l_pEffect;
	char *errortext;
	LPD3DXBUFFER errors;
	D3DXHANDLE hTechnique;

	// same for skybox.fx
	/*char realFilePath[256];
	FindMediaFile(realFilePath, "skyBox.fx");
	D3DXCreateEffectFromFile(device, realFilePath, 
		NULL, NULL, NULL, NULL, &skybox_effect, &errors );*/
	  	//从特效缓存中创建特效
	HRESULT hr1 =   D3DXCreateEffect(device, g_FXSkyBoxBuffer, 
		(sizeof(g_FXSkyBoxBuffer) - 1), NULL, NULL, 
		NULL, NULL, &skybox_effect, &errors);


	if (errors != NULL){
		errortext = (char*) errors->GetBufferPointer();
		MessageBox(NULL, (errortext), TEXT("Textures.exe"), MB_OK);		
	}

	skybox_effect->FindNextValidTechnique(NULL, &hTechnique);    
	skybox_effect->SetTechnique(hTechnique);

	
	if(m_bSupportGPUNormal)
	{
	/*	FindMediaFile(realFilePath, "water_R300.fx");
		D3DXCreateEffectFromFile(device, realFilePath, 
			NULL, NULL, 0, NULL, &surf_software_effect, &errors );*/
		//从特效缓存中创建特效
		hr1 =   D3DXCreateEffect(device, g_FXWaterR300Buffer,
			(sizeof(g_FXWaterR300Buffer) - 1), NULL, NULL, 
			D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, NULL, &surf_software_effect, &errors);
	}
	else
	{
	// same for surf_software_effect
	/*	FindMediaFile(realFilePath, "water_soft.fx");
		D3DXCreateEffectFromFile(device, realFilePath, 
			NULL, NULL, 0, NULL, &surf_software_effect, &errors );*/
		//从特效缓存中创建特效
		HRESULT /*hr1*/hr0 =   D3DXCreateEffect(device, g_FXWaterSoftBuffer, 
			(sizeof(g_FXWaterSoftBuffer) - 1), NULL, NULL, D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,
			 NULL, &surf_software_effect, &errors);
	}

	if (errors != NULL)
	{
	//	errortext = (char*) errors->GetBufferPointer();
	//	MessageBox(NULL, errortext, "Textures.exe", MB_OK);		
	}

	surf_software_effect->FindNextValidTechnique(NULL, &hTechnique);    
	surf_software_effect->SetTechnique(hTechnique);

	
	//	underwater_software_effect
/*		D3DXCreateEffectFromFile(device, "underwater_soft.fx", 
		NULL, NULL, 0, NULL, &underwater_software_effect, &errors );*/
		//从特效缓存中创建特效
	hr1 =   D3DXCreateEffect(device, g_FXUnderWaterSoftBuffer, 
		(sizeof(g_FXUnderWaterSoftBuffer) - 1), NULL, NULL, 
		D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, NULL, &underwater_software_effect, &errors);


	if (errors != NULL){
//		errortext = (char*) errors->GetBufferPointer();
//		MessageBox(NULL, errortext, "Textures.exe", MB_OK);		
	}

	underwater_software_effect->FindNextValidTechnique(NULL, &hTechnique);
	underwater_software_effect->SetTechnique(hTechnique);
}

/*--------------------------------------------------------------------------------------------------------------
initbuffers

prepare the vertex and indexbuffer with a uniform grid (dependant on the size parameter)	
--------------------------------------------------------------------------------------------------------------*/

bool CSurface::initbuffers()
{
	SURFACEVERTEX* pdVertices;	
	// create the vertexbuffer used in the softwaremode (it can be empty as it'll be memcpy-ed to)
	if( FAILED( device->CreateVertexBuffer( gridsize_x*gridsize_y*sizeof(SOFTWARESURFACEVERTEX),
		D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, D3DFVF_SOFTWARESURFACEVERTEX,
		D3DPOOL_DEFAULT, &surf_software_vertices, NULL ) ) )
	{
		return false;
	}		

	// create the skybox vertexbuffer
	if( FAILED( device->CreateVertexBuffer( skyboxdetail*skyboxdetail*sizeof(SURFACEVERTEX),
		D3DUSAGE_WRITEONLY, D3DFVF_SURFACEVERTEX,
		D3DPOOL_DEFAULT, &skybox_vertices, NULL ) ) )
	{
		return false;
	}
	if( FAILED( skybox_vertices->Lock( 0, 0, (void**)&pdVertices, 0 ) ) )
		return false;
	{
		for(int v=0; v<skyboxdetail; v++)
		{
			for(int u=0; u<skyboxdetail; u++)
			{
				//水平方向，创建完整的一周2PI的顶点
				float al = -2 * 3.14159265f * ((float)u/(skyboxdetail-1.0f));
				//垂直方向，创建0.6周的顶点
				float th = 0.6f * 3.14159265f * ((float)v/(skyboxdetail-1.0f));
				//计算每个顶点的位置
				pdVertices[v*skyboxdetail+u].position.x = (float)(sin(th) * sin(al));
				pdVertices[v*skyboxdetail+u].position.y = (float)(cos(th));
				pdVertices[v*skyboxdetail+u].position.z = (float)(sin(th) * cos(al));
				pdVertices[v*skyboxdetail+u].displacement = 0.0f;
			}
		}
	}
	skybox_vertices->Unlock();

	// create/fill the indexbuffer

	if(	FAILED( device->CreateIndexBuffer(	sizeof(unsigned int) * 6 * (gridsize_x-1)*(gridsize_y-1),
		D3DUSAGE_WRITEONLY,			
		D3DFMT_INDEX32,	D3DPOOL_DEFAULT,&surf_indicies,NULL)))
	{
		return false;
	}
	unsigned int *indexbuffer;
	if( FAILED( surf_indicies->Lock(0,0,(void**)&indexbuffer,0 ) ) )
		return false;
	int i = 0;
	{
		for(int v=0; v<gridsize_y-1; v++){
			for(int u=0; u<gridsize_x-1; u++){
				// face 1 |/
				indexbuffer[i++]	= v*gridsize_x + u;
				indexbuffer[i++]	= v*gridsize_x + u + 1;
				indexbuffer[i++]	= (v+1)*gridsize_x + u;

				// face 2 /|
				indexbuffer[i++]	= (v+1)*gridsize_x + u;
				indexbuffer[i++]	= v*gridsize_x + u + 1;
				indexbuffer[i++]	= (v+1)*gridsize_x + u + 1;
			}
		}
	}
	surf_indicies->Unlock();

	// create/fill the indexbuffer

	if(	FAILED( device->CreateIndexBuffer(	sizeof(unsigned int) * 6 * (skyboxdetail-1)*(skyboxdetail-1),
		D3DUSAGE_WRITEONLY,			
		D3DFMT_INDEX32,	D3DPOOL_DEFAULT,&skybox_indicies,NULL)))
	{
		return false;
	}

	if( FAILED( skybox_indicies->Lock(0,0,(void**)&indexbuffer,0 ) ) )
		return false;
	i = 0;
	{
		for(int v=0; v<skyboxdetail-1; v++){
			for(int u=0; u<skyboxdetail-1; u++){
				// face 1 |/
				indexbuffer[i++]	= v*skyboxdetail + u;
				indexbuffer[i++]	= v*skyboxdetail + u + 1;
				indexbuffer[i++]	= (v+1)*skyboxdetail + u;

				// face 2 /|
				indexbuffer[i++]	= (v+1)*skyboxdetail + u;
				indexbuffer[i++]	= v*skyboxdetail + u + 1;
				indexbuffer[i++]	= (v+1)*skyboxdetail + u + 1;
			}
		}
	}
	skybox_indicies->Unlock();

	return true;
}

void CSurface::Invalidate()
{
	//释放水面顶点和索引缓冲
	SAFE_RELEASE_D3D(surf_software_vertices);
	SAFE_RELEASE_D3D(surf_indicies);

	//释放天空盒顶点和索引缓冲
	SAFE_RELEASE_D3D(skybox_vertices);
	SAFE_RELEASE_D3D(skybox_indicies);

	//释放反射和折射贴图表面
	SAFE_RELEASE_D3D(surf_refraction);
	SAFE_RELEASE_D3D(surf_reflection);
	//释放深度缓冲
	SAFE_RELEASE_D3D(depthstencil);

	//释放效果对象
	SAFE_RELEASE_D3D(surf_software_effect);
	SAFE_RELEASE_D3D(underwater_software_effect);
	SAFE_RELEASE_D3D(skybox_effect);

	//顶点波动器
	software_brus->Invalidate();
}

void CSurface::RestoreDeviceObject()
{	
	//创建水面和天空盒的顶点和索引缓冲
	initbuffers();
	//创建深度缓冲
	//创建用于渲染水面反射和折射的贴图表面
	device->CreateTexture(reflrefrdetail,reflrefrdetail,1,D3DUSAGE_RENDERTARGET, 
							D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surf_reflection,NULL);	
	device->CreateTexture(reflrefrdetail,reflrefrdetail,1,D3DUSAGE_RENDERTARGET, 
							D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surf_refraction,NULL);		
	device->CreateDepthStencilSurface( reflrefrdetail, reflrefrdetail,D3DFMT_D24S8, 
							D3DMULTISAMPLE_NONE, 0, true, &depthstencil, NULL );
	//创建效果对象
	LoadEffect();
	
	//顶点波动器
	software_brus->RestoreDeviceObject();
}

void CSurface::Release()
{
	Invalidate();
	
	//释放顶点波动器对象
	SAFE_RELEASE(software_brus);

	//释放贴图对象
	SAFE_RELEASE_D3D(surf_fresnel);
	SAFE_RELEASE_D3D(underwater_fresnel);
	SAFE_RELEASE_D3D(noise2D);
}

bool CSurface::within_frustum(D3DXVECTOR3 *pos)
{
	D3DXVECTOR3 test;
	D3DXMATRIX matView;
	rendering_camera->getViewMatrix(&matView);
	D3DXVec3TransformCoord(&test, pos, &matView);
	if((fabs(test.x) < 1.00001f)&&(fabs(test.y) < 1.00001f)&&(fabs(test.z) < 1.00001f))
		return true;
	return false;
}

float dispmulti(float dist)
{
	return max(0, min(1, dist-1));
}

/*--------------------------------------------------------------------------------------------------------------
render

render CSurface
--------------------------------------------------------------------------------------------------------------*/

bool CSurface::prepare(CCamera *cam)
{
	if(!initialized) return false;
	
	SAFE_RELEASE(observing_camera);
	SAFE_DELETE(observing_camera);
	observing_camera = new CCamera(*cam);
	this->SetupMatrices(this->observing_camera);		// obsolete with vertexshaders
	
	plane_within_frustum = this->getMinMax(&range);
	
	if (plane_within_frustum)
	{
		
		software_brus->render_geometry(&range);
		
		D3DVERTEXBUFFER_DESC pDesc;
		SOFTWARESURFACEVERTEX *vertices;
		HRESULT hr = surf_software_vertices->GetDesc(&pDesc );
		
		if( FAILED(surf_software_vertices->Lock( 0, 0, (void**) &vertices, D3DLOCK_DISCARD)))	
		{
			MessageBox(NULL, TEXT("Could not lock vertexbuffer"), TEXT("Textures.exe"), MB_OK);
		}
		else
		{
			int size = pDesc.Size;
			memcpy(vertices, software_brus->vertices, size);
			surf_software_vertices->Unlock();		
		}
	}
	

	return true;
}

void CSurface::render_skybox()
{
	/*
	g_pd3dDevice->SetMaterial((D3DMATERIAL9*)duck_Materials->GetBufferPointer());
	device->SetRenderState( D3DRS_LIGHTING, true);

	device->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
	device->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL );
	device->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL );
	device->SetTexture(0, sky_cubemap);
	*/

	device->SetStreamSource( 0, skybox_vertices, 0, sizeof(SURFACEVERTEX) );
	device->SetFVF( D3DFVF_SURFACEVERTEX);
	device->SetIndices(skybox_indicies);

	skybox_effect->Begin(NULL,NULL);
	skybox_effect->BeginPass(0);
	

	// build the 'fake' viweproj with the distance vector set to 0,0,0
	D3DXMATRIXA16 matView;
	observing_camera->getViewMatrix(&matView);
	D3DXMATRIXA16 fvproj(matView);
	D3DXMATRIXA16 matProj;
	observing_camera->getProjectMatrix(&matProj);
	fvproj._41 = 0;
	fvproj._42 = 0;
	fvproj._43 = 0;
	fvproj = fvproj * matProj;

	skybox_effect->SetMatrix("mViewProj",&fvproj);
	skybox_effect->SetMatrix("mInvViewProj",&(observing_camera->m_invviewproj));	
	skybox_effect->SetMatrix("mInvView",&(observing_camera->m_invview));	
	skybox_effect->SetFloat("sun_alfa", prm->params[p_fSunPosAlpha].fData);
	skybox_effect->SetFloat("sun_theta", prm->params[p_fSunPosTheta].fData);
	skybox_effect->SetFloat("sun_shininess", 4*prm->params[p_fSunShininess].fData);
	skybox_effect->SetFloat("sun_strength", prm->params[p_fSunStrength].fData);	

	D3DXVECTOR3 camPos = observing_camera->GetPosition(VTS_WORLD);
	skybox_effect->SetVector("view_position", &D3DXVECTOR4(camPos.x, camPos.y, camPos.z, 1));	
	skybox_effect->SetTexture("EnvironmentMap",sky_cubemap);	

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,	0,	0, skyboxdetail*skyboxdetail, 0, 2*(skyboxdetail-1)*(skyboxdetail-1) );
	skybox_effect->EndPass();
	skybox_effect->End();
}

void CSurface::render_cutter()
{
	if (plane_within_frustum)
	{		
		//device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW  );
		device->SetStreamSource( 0, surf_software_vertices, 0, sizeof(SOFTWARESURFACEVERTEX) );
		device->SetFVF( D3DFVF_SOFTWARESURFACEVERTEX);			
		device->SetIndices(surf_indicies);
		device->DrawIndexedPrimitive(	D3DPT_TRIANGLELIST, 0,	0, gridsize_x*gridsize_y, 0, 2*(gridsize_x-1)*(gridsize_y-1) );			
	}
}

bool CSurface::render()
{
	if (plane_within_frustum)
	{		
		{			
			//HRESULT hr;
			/*// underwater pass
			device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW  );
			device->SetStreamSource( 0, surf_software_vertices, 0, sizeof(SOFTWARESURFACEVERTEX) );
			device->SetFVF( D3DFVF_SOFTWARESURFACEVERTEX);			
			device->SetIndices(surf_indicies);

			underwater_software_effect->Begin(NULL,NULL);
			underwater_software_effect->Pass(0);			
			underwater_software_effect->SetMatrix("mViewProj",&(observing_camera->viewproj));
			underwater_software_effect->SetFloat("sun_alfa", prm->params[p_fSunPosAlpha].fData);
			underwater_software_effect->SetFloat("sun_theta", prm->params[p_fSunPosTheta].fData);
			underwater_software_effect->SetFloat("sun_shininess", prm->params[p_fSunShininess].fData);
			underwater_software_effect->SetFloat("sun_strength", prm->params[p_fSunStrength].fData);
			underwater_software_effect->SetVector("watercolour", &D3DXVECTOR4(prm->params[p_fWaterColourR].fData,prm->params[p_fWaterColourG].fData,prm->params[p_fWaterColourB].fData,1));

			underwater_software_effect->SetFloat("LODbias", prm->get_float(p_fLODbias) );
			underwater_software_effect->SetVector("view_position", &D3DXVECTOR4(observing_camera->position.x,observing_camera->position.y,observing_camera->position.z,1));
			if (sky_cubemap)
				underwater_software_effect->SetTexture("EnvironmentMap",sky_cubemap);
			underwater_software_effect->SetTexture("FresnelMap",underwater_fresnel);
			underwater_software_effect->SetTexture("Normalmap",software_brus->normalmap);

			
			if ( prm->params[p_bAsPoints].bData )
				device->DrawPrimitive( D3DPT_POINTLIST, 0, gridsize_x*gridsize_y );
			else
				device->DrawIndexedPrimitive(	D3DPT_TRIANGLELIST, 0,	0, gridsize_x*gridsize_y, 0, 2*(gridsize_x-1)*(gridsize_y-1) );				
			
			underwater_software_effect->End();
			*/
			// above water pass
			device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE  );	
			device->SetStreamSource( 0, surf_software_vertices, 0, sizeof(SOFTWARESURFACEVERTEX) );
			device->SetFVF( D3DFVF_SOFTWARESURFACEVERTEX);			
			device->SetIndices(surf_indicies);

			if(m_bSupportGPUNormal)
				software_brus->generate_normalmap();

			//device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW  );

			surf_software_effect->Begin(NULL,NULL);
			surf_software_effect->BeginPass(0);			

			surf_software_effect->SetMatrix("mViewProj",&(observing_camera->m_viewproj));
			surf_software_effect->SetMatrix("mView",&(observing_camera->m_view));
			//surf_software_effect->SetFloat("sun_alfa", prm->params[p_fSunPosAlpha].fData);
			//surf_software_effect->SetFloat("sun_theta", prm->params[p_fSunPosTheta].fData);
			float sa = prm->params[p_fSunPosAlpha].fData, st = prm->params[p_fSunPosTheta].fData;
			surf_software_effect->SetVector("sun_vec",&D3DXVECTOR4((float)(cos(st) * sin(sa)), (float)sin(st), (float)(cos(st) * cos(sa)), 0));
			surf_software_effect->SetFloat("sun_shininess", prm->params[p_fSunShininess].fData);			
			surf_software_effect->SetFloat("sun_strength", prm->params[p_fSunStrength].fData);
			surf_software_effect->SetFloat("reflrefr_offset", prm->params[p_bReflRefrStrength].fData);			
			surf_software_effect->SetBool("diffuseSkyRef", prm->params[p_bDiffuseRefl].bData);

			surf_software_effect->SetVector("watercolour", &D3DXVECTOR4(prm->params[p_fWaterColourR].fData,prm->params[p_fWaterColourG].fData,prm->params[p_fWaterColourB].fData,1));
			surf_software_effect->SetFloat("LODbias", prm->get_float(p_fLODbias) );

			D3DXVECTOR3 camPos = observing_camera->GetPosition(VTS_WORLD);
			surf_software_effect->SetVector("view_position", 
											&D3DXVECTOR4(camPos.x, camPos.y, camPos.z, 1));
			if (sky_cubemap)			
				surf_software_effect->SetTexture("EnvironmentMap",sky_cubemap);
			surf_software_effect->SetTexture("FresnelMap",surf_fresnel);
			
			if(m_bSupportGPUNormal)
			{
				surf_software_effect->SetTexture("Heightmap",software_brus->heightmap);
				surf_software_effect->SetTexture("Normalmap",software_brus->normalmap);
			}

			surf_software_effect->SetTexture("Refractionmap",surf_refraction);
			surf_software_effect->SetTexture("Reflectionmap",surf_reflection);

			//surf_software_effect->SetTexture("noiseXZ",noise2D);

			 if ( prm->params[p_bAsPoints].bData )
				device->DrawPrimitive( D3DPT_POINTLIST, 0, gridsize_x*gridsize_y );
			else
				device->DrawIndexedPrimitive(	D3DPT_TRIANGLELIST, 0,	0, gridsize_x*gridsize_y, 0, 2*(gridsize_x-1)*(gridsize_y-1) );				
			
			surf_software_effect->EndPass();
			surf_software_effect->End();
		} 
	}
	return true;
}

/*--------------------------------------------------------------------------------------------------------------
getMinMax

get the matrix that defines the minimum rectangle in which the frustum is located
--------------------------------------------------------------------------------------------------------------*/
bool CSurface::getMinMax(D3DXMATRIXA16 *range)
{
	set_displacement_amplitude(prm->params[p_fStrength].fData);
	float		x_min,y_min,x_max,y_max;
	D3DXVECTOR3 frustum[8],proj_points[24];		// frustum to check the camera against

	int n_points=0;
	int cube[] = {	0,1,	0,2,	2,3,	1,3,
		0,4,	2,6,	3,7,	1,5,
		4,6,	4,5,	5,7,	6,7};	// which frustum points are connected together?

	// transform frustum points to worldspace (should be done to the rendering_camera because it's the interesting one)
	D3DXVec3TransformCoord(&frustum[0], &D3DXVECTOR3(-1,-1,-1), &(rendering_camera->m_invviewproj));
	D3DXVec3TransformCoord(&frustum[1], &D3DXVECTOR3(+1,-1,-1), &(rendering_camera->m_invviewproj));
	D3DXVec3TransformCoord(&frustum[2], &D3DXVECTOR3(-1,+1,-1), &(rendering_camera->m_invviewproj));
	D3DXVec3TransformCoord(&frustum[3], &D3DXVECTOR3(+1,+1,-1), &(rendering_camera->m_invviewproj));
	D3DXVec3TransformCoord(&frustum[4], &D3DXVECTOR3(-1,-1,+1), &(rendering_camera->m_invviewproj));
	D3DXVec3TransformCoord(&frustum[5], &D3DXVECTOR3(+1,-1,+1), &(rendering_camera->m_invviewproj));
	D3DXVec3TransformCoord(&frustum[6], &D3DXVECTOR3(-1,+1,+1), &(rendering_camera->m_invviewproj));
	D3DXVec3TransformCoord(&frustum[7], &D3DXVECTOR3(+1,+1,+1), &(rendering_camera->m_invviewproj));	


	// check intersections with upper_bound and lower_bound	
	for(int i=0; i<12; i++)
	{
		int src=cube[i*2], dst=cube[i*2+1];
		if ((upper_bound.a*frustum[src].x + upper_bound.b*frustum[src].y + upper_bound.c*frustum[src].z + upper_bound.d*1)/(upper_bound.a*frustum[dst].x + upper_bound.b*frustum[dst].y + upper_bound.c*frustum[dst].z + upper_bound.d*1)<0)
		{			
			D3DXPlaneIntersectLine( &proj_points[n_points++], &upper_bound, &frustum[src], &frustum[dst]);			
		}
		if ((lower_bound.a*frustum[src].x + lower_bound.b*frustum[src].y + lower_bound.c*frustum[src].z + lower_bound.d*1)/(lower_bound.a*frustum[dst].x + lower_bound.b*frustum[dst].y + lower_bound.c*frustum[dst].z + lower_bound.d*1)<0)
		{			
			D3DXPlaneIntersectLine( &proj_points[n_points++], &lower_bound, &frustum[src], &frustum[dst]);			
		}
	}
	// check if any of the frustums vertices lie between the upper_bound and lower_bound planes
	{
		for(int i=0; i<8; i++)
		{		
			if ((upper_bound.a*frustum[i].x + upper_bound.b*frustum[i].y + upper_bound.c*frustum[i].z + upper_bound.d*1)/(lower_bound.a*frustum[i].x + lower_bound.b*frustum[i].y + lower_bound.c*frustum[i].z + lower_bound.d*1)<0)
			{			
				proj_points[n_points++] = frustum[i];
			}		
		}	
	}

	//
	// create the camera the grid will be projected from
	//
	SAFE_RELEASE(projecting_camera);
	SAFE_DELETE(projecting_camera);
	projecting_camera = new CCamera(*rendering_camera);

	D3DXVECTOR3 projCamPos = projecting_camera->GetPosition(VTS_WORLD);
	D3DXVECTOR3 projCamDir = projecting_camera->GetDirection(VTS_WORLD);
	D3DXVECTOR3 renderCamPos = rendering_camera->GetPosition(VTS_WORLD);
	D3DXVECTOR3 renderCamDir = rendering_camera->GetDirection(VTS_WORLD);

	// make sure the camera isn't too close to the plane
	float height_in_plane = (lower_bound.a * projCamPos.x + lower_bound.b * projCamPos.y + lower_bound.c * projCamPos.z);

	bool keep_it_simple = false;
	bool underwater=false;

	if (height_in_plane < 0.0f) underwater = true;

	if(keep_it_simple)
	{
		//projecting_camera->m_forward = rendering_camera->m_forward;
		//projecting_camera->UpdateLookAt();
		projecting_camera->Lookat(&(projCamPos + renderCamDir), VTS_WORLD);
		projecting_camera->UpdateAddtionalMatrix();
	}
	else
	{
		D3DXVECTOR3 aimpoint, aimpoint2;		

		if (height_in_plane < (prm->params[p_fStrength].fData+prm->get_float(p_fElevation)))
		{					
			if(underwater)
				projecting_camera->Move(D3DXVECTOR3(lower_bound.a,lower_bound.b,lower_bound.c)*(prm->params[p_fStrength].fData + prm->get_float(p_fElevation) - 2*height_in_plane), VTS_WORLD);															
			else
				projecting_camera->Move(D3DXVECTOR3(lower_bound.a,lower_bound.b,lower_bound.c)*(prm->params[p_fStrength].fData + prm->get_float(p_fElevation) - height_in_plane), VTS_WORLD);
		} 
		
		// aim the projector at the point where the camera view-vector intersects the plane
		// if the camera is aimed away from the plane, mirror it's view-vector against the plane
		if( (D3DXPlaneDotNormal(&plane, &(renderCamDir)) < 0.0f) log_xor (D3DXPlaneDotCoord(&plane, &(renderCamPos)) < 0.0f ) )
		{				
			D3DXPlaneIntersectLine( &aimpoint, &plane, &renderCamDir, &(renderCamPos + renderCamDir) );			
		}
		else
		{
			D3DXVECTOR3 flipped;
			flipped = renderCamDir - 2*normal*D3DXVec3Dot(&renderCamDir,&normal);
			D3DXPlaneIntersectLine( &aimpoint, &plane, &(renderCamPos), &(renderCamPos + flipped) );			
		}

			// force the point the camera is looking at in a plane, and have the projector look at it
			// works well against horizon, even when camera is looking upwards
			// doesn't work straight down/up
			float af = (float)fabs(D3DXPlaneDotNormal(&plane, &renderCamDir));
			//af = 1 - (1-af)*(1-af)*(1-af)*(1-af)*(1-af);
			//aimpoint2 = (rendering_camera->position + rendering_camera->zfar * rendering_camera->forward);
			aimpoint2 = (renderCamPos + 10.0f * renderCamDir);
			aimpoint2 = aimpoint2 - normal*D3DXVec3Dot(&aimpoint2,&normal);
		
			// fade between aimpoint & aimpoint2 depending on view angle
			
			aimpoint = aimpoint*af + aimpoint2*(1.0f-af);
			//aimpoint = aimpoint2;
			
			//projecting_camera->m_forward = aimpoint-projecting_camera->m_from;
			//projecting_camera->UpdateLookAt();
			projecting_camera->Lookat(&(aimpoint), VTS_WORLD);
			projecting_camera->UpdateAddtionalMatrix();
	}

	sprintf( debugdata, "n_points %i\n",n_points);
	{
		for(int i=0; i<n_points; i++)
		{
			// project the point onto the CSurface plane
			proj_points[i] = proj_points[i] - normal*D3DXVec3Dot(&proj_points[i],&normal);	
		}
	}

	{
		for(int i=0; i<n_points; i++)
		{
			D3DXVec3TransformCoord( &proj_points[i], &proj_points[i], &(projecting_camera->m_view));	 
			sprintf( debugdata, "%s%f  %f  %f\n",debugdata,proj_points[i].x,proj_points[i].y,proj_points[i].z);
			D3DXVec3TransformCoord( &proj_points[i], &proj_points[i], &(projecting_camera->m_proj));
		}
	}

	// debughonk

	/*	for(int i=0; i<n_points; i++){
	sprintf( debugdata, "%s%f  %f  %f\n",debugdata,proj_points[i].x,proj_points[i].y,proj_points[i].z);
	}*/

	// get max/min x & y-values to determine how big the "projection window" must be
	if (n_points > 0)
	{
		x_min = proj_points[0].x;
		x_max = proj_points[0].x;
		y_min = proj_points[0].y;
		y_max = proj_points[0].y;
		for(int i=1; i<n_points; i++)
		{
			if (proj_points[i].x > x_max) x_max = proj_points[i].x;
			if (proj_points[i].x < x_min) x_min = proj_points[i].x;
			if (proj_points[i].y > y_max) y_max = proj_points[i].y;
			if (proj_points[i].y < y_min) y_min = proj_points[i].y;
		}		
		
		sprintf( debugdata, "%sx = [%f..%f] y = [%f..%f]\n",debugdata,x_min,x_max,y_min,y_max);
		sprintf( debugdata, "%sheight_in_plane: %f\n",debugdata,height_in_plane);
		
		//sprintf( debugdata,	"%slimit_y_upper = %f\n",debugdata,limit_y_upper);
		//		sprintf( debugdata, "%sy1 = [%f] y2 = [%f]\n",debugdata,y1,y2);

		// build the packing matrix that spreads the grid across the "projection window"
		D3DXMATRIXA16 pack(	x_max-x_min,	0,				0,		x_min,
							0,				y_max-y_min,	0,		y_min,
							0,				0,				1,		0,	
							0,				0,				0,		1);
		D3DXMatrixTranspose(&pack,&pack);
		*range = pack*projecting_camera->m_invviewproj;

		return true;
	}
	return false;
}


/*--------------------------------------------------------------------------------------------------------------
~CSurface

destructor
--------------------------------------------------------------------------------------------------------------*/

CSurface::~CSurface()
{
	/*
#ifdef inc_hwpath
	this->surf_indicies->Release();
	this->surf_vertices->Release();
#endif
	this->skybox_indicies->Release();
	*/
}

/*--------------------------------------------------------------------------------------------------------------
SetupMatrices

sets the matrices according to the camera
--------------------------------------------------------------------------------------------------------------*/
void CSurface::SetupMatrices(CCamera *camera_view)
{
	D3DXMATRIXA16 matWorld,matProj;
	D3DXMatrixIdentity(&matWorld);
	device->SetTransform( D3DTS_WORLD, &matWorld );
	device->SetTransform( D3DTS_VIEW, &(camera_view->m_view) );
	device->SetTransform( D3DTS_PROJECTION, &(camera_view->m_proj) );
}

void CSurface::set_grid_size(int size_x, 
							int size_y)
{
	this->gridsize_x = size_x+1;
	this->gridsize_y = size_y+1;
	this->surf_indicies->Release();
	this->surf_software_vertices->Release();
	this->skybox_indicies->Release();
	this->skybox_vertices->Release();
	this->initbuffers();
	
	software_brus->resize(gridsize_x, gridsize_y);
}

void CSurface::set_render_mode(int rendermode)
{
	this->rendermode = rendermode;
}

void CSurface::set_displacement_amplitude(float amplitude)
{
	D3DXPlaneFromPointNormal( &(this->upper_bound), &(this->pos + amplitude * this->normal), &(this->normal));
	D3DXPlaneFromPointNormal( &(this->lower_bound), &(this->pos - amplitude * this->normal), &(this->normal));	
}

float CSurface::get_height_at( float x, float z )
{
	if (software_brus)
		return software_brus->get_height_at(x,z);
	return 0.0f;
}

void CSurface::calc_efficiency()
{
	efficiency = 0;
	for(int i=0; i<(gridsize_x*gridsize_y); i++)
	{
		D3DXVECTOR3 pos;
		pos.x = software_brus->vertices[i].x;
		pos.y = software_brus->vertices[i].y;
		pos.z = software_brus->vertices[i].z;
		if (this->within_frustum(&pos))
			efficiency += 1.0f;
	}
	efficiency /= gridsize_x*gridsize_y;
}