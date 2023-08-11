/////////////////////////////////////////////////////////////////////////
// 名称: CImportS3D--S3D[Version 3.0]S3D数据导入类， OGRE版本
// 描述: 加载3.0版本的S3D文件数据（从CGetS3DFileInfo_V3获得）到场景
// CGetS3DFileInfo_V3负责从S3D文件读取数据，而CImportS3D负责将其读取的数据加载到场景
// 作者: lixin	
// 日期: 2007-11-27
////////////////////////////////////////////////////////////////////////

#include "ImportS3D.h"
#include "..\VGSSupport.h"
#include "..\Animation\AnimationManager.h"


CImportS3D::CImportS3D(void)
{
	mS3DFileInfo = NULL;

	mPreMtrlCnt = 0;
	mPreTexCnt = 0;
	mPreCameraCnt = 0;
	mPreLightCnt = 0;
	mPreSkeletonCnt = 0;
	mPreMeshCnt = 0;
	mPreEntityCnt = 0;

	m_pScene = NULL;				//场景管理器	
	mKeyFrameStartPlayTime = 0;		//关键帧动画开始播放的时间

	m_render_cs = NULL;

	m_textureset.clear();
}

CImportS3D::~CImportS3D(void)
{
}

//加载CGetS3DFileInfo_V3对象到场景
BOOL CImportS3D::LoadS3DFileInfo(CGetS3DFileInfo* pS3DFileInfo, CScene* pVGS_SceneMgr, HWND hWnd)
{
	if (!pVGS_SceneMgr)
	{
		return FALSE;
	}

	m_hWnd = hWnd;

	//VGS场景管理器
	m_pScene = pVGS_SceneMgr;
	mS3DFileInfo = pS3DFileInfo;

	if (!mS3DFileInfo || !m_pScene) 
	{
		return FALSE;
	}

	//场景中已有的材质数量
	mPreMtrlCnt = GetSceneMtrlCount();
	//计算场景中已经存在的相机数量
	mPreCameraCnt = GetSceneCameraCount();
	//计算场景中已经存在的灯光数量
	mPreLightCnt = GetSceneLightCount();
	//计算场景中已经存在的骨骼数量
	mPreSkeletonCnt = GetSceneSkeletonCount();
	//计算场景中已经存在的Mesh数量
	mPreMeshCnt = GetSceneMeshCount();
	//计算场景中已有的Entity数量
	mPreEntityCnt = GetSceneEntityCount();

	// 进度条总字节数和当前字节数
	DWORD totalSize = 2 * mS3DFileInfo->m_totalSize;
	DWORD curSize   = mS3DFileInfo->m_totalSize;
	DWORD oneStepSize = curSize / 7;
	try
	{
		//加载材质数据
		LoadMtrlData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // 开始进度
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("导入材质数据出错！"), LPCSTR("错误"), 0);
		return FALSE;
	}

	try
	{
		//加载相机数据
		LoadCameraData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // 开始进度
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("导入相机数据出错！"), LPCSTR("错误"), 0);
		return FALSE;
	}

	try
	{
		//加载灯光数据
		LoadLightData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // 开始进度
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("导入灯光数据出错！"), LPCSTR("错误"), 0);
		return FALSE;
	}

	try
	{
		//加载骨骼数据
		LoadSkeletonData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // 开始进度
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("导入骨骼数据出错！"), LPCSTR("错误"), 0);
		return FALSE;
	}

	try
	{
		//加载Mesh数据
		LoadMeshData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // 开始进度
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("导入模型数据出错！"), LPCSTR("错误"), 0);
		return FALSE;
	}
	
	try
	{
		//加载Shape数据
		LoadShapeData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // 开始进度
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("导入图形数据出错！"), LPCSTR("错误"), 0);
		return FALSE;
	}
	try
	{
		//加载场景图数据
		LoadSceneGraphData();
		curSize = totalSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // 开始进度
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("导入场景数据出错！"), LPCSTR("错误"), 0);
		return FALSE;
	}

	// return std::string(mResourceGroupName.c_str());
	return TRUE;
}

//加载材质数据
int	CImportS3D::LoadMtrlData()
{
	//逐个创建材质
	int mtrlCnt = mS3DFileInfo->mtrlManager.mtrlCnt;
	for (int i = 0; i< mtrlCnt; i++)
	{
		VGS_Mtrl* pVGSMtrl = &(mS3DFileInfo->mtrlManager.mtrl[i]);

		//材质名
		std::string RealName = std::string(pVGSMtrl->name.c_str());
		RealName = m_pScene->RenameMaterialIfExist(RealName); // _RenameMaterialIfExist(RealName);

		//创建材质资源到指定资源组
		CMaterial *pMtrl = m_pScene->CreateMaterial(RealName);

		if (!pMtrl)
		{
			continue;
		}		

		//透明度 
		pMtrl->setOpacity(pVGSMtrl->opacity);
		float opacity = pVGSMtrl->opacity / 100.0f;
       
		//设置材质的Diffse属性 - 本色
		pMtrl->d3dmtrl.Diffuse.r = (((pVGSMtrl->diffuse) >> 16) & 0xff) / 255.0f;
		pMtrl->d3dmtrl.Diffuse.g = (((pVGSMtrl->diffuse) >> 8) & 0xff) / 255.0f;
		pMtrl->d3dmtrl.Diffuse.b = (((pVGSMtrl->diffuse)) & 0xff) / 255.0f;
		pMtrl->d3dmtrl.Diffuse.a = opacity;

		
		//设置材质的Ambient属性 - 环境色，必须为0，理由：模型自身不发光
		pMtrl->d3dmtrl.Ambient.r = 0;
		pMtrl->d3dmtrl.Ambient.g = 0;
		pMtrl->d3dmtrl.Ambient.b = 0;
		pMtrl->d3dmtrl.Ambient.a = opacity;
		
		//设置材质的Specular属性 - 高光色
		pMtrl->specular.r = (((pVGSMtrl->specular)>>16) & 0xff)/255.0f;
		pMtrl->specular.g = (((pVGSMtrl->specular)>>8) & 0xff)/255.0f;
		pMtrl->specular.b = (((pVGSMtrl->specular)) & 0xff)/255.0f;
		pMtrl->specular.a = opacity;
		pMtrl->specularLevel = pVGSMtrl->specularLevel;

		pMtrl->d3dmtrl.Specular.r = pMtrl->specular.r * (pVGSMtrl->specularLevel / 100.0f);
		pMtrl->d3dmtrl.Specular.g = pMtrl->specular.g * (pVGSMtrl->specularLevel / 100.0f);
		pMtrl->d3dmtrl.Specular.b = pMtrl->specular.b * (pVGSMtrl->specularLevel / 100.0f);
		pMtrl->d3dmtrl.Specular.a = opacity;
		pMtrl->d3dmtrl.Power      = pVGSMtrl->shininess;  // 高光锐度
		
		//自发光属性
		pMtrl->selfIllumination = pVGSMtrl->selfIllumination;
		if (pMtrl->selfIllumination >= 100) 
		{
			pMtrl->selfIllumination = 99; // 因为100自发光时，不会产生高光，所以如果完全自发光，程序改为99。
		}

		pMtrl->d3dmtrl.Emissive.r = pMtrl->d3dmtrl.Diffuse.r * pMtrl->selfIllumination / 100.0f;
		pMtrl->d3dmtrl.Emissive.g = pMtrl->d3dmtrl.Diffuse.g * pMtrl->selfIllumination / 100.0f;
		pMtrl->d3dmtrl.Emissive.b = pMtrl->d3dmtrl.Diffuse.b * pMtrl->selfIllumination / 100.0f;
		pMtrl->d3dmtrl.Emissive.a = opacity;

		//Shader类型//这里可以优化,只有光照材质和有顶点色需要混色的才需要作Gouraud模型
		switch(pVGSMtrl->shaderType)
		{
		case 0x00:
			pMtrl->type = D3DSHADE_FLAT;
			break;
		case 0x08:
			pMtrl->type = D3DSHADE_GOURAUD;
			break;
		case 0x20:
			pMtrl->type = D3DSHADE_PHONG;
			break;
		default:
			pMtrl->type = D3DSHADE_GOURAUD;
			break;
		}

		//双面渲染属性
		if (pVGSMtrl->bTwoSide) 
			pMtrl->twoSide = TRUE;
		else
			pMtrl->twoSide = FALSE;

		
		// 缺省不使用alphatest
		pMtrl->bAlphaTest = FALSE;
		pMtrl->AlpahaTestValue = 64;

		//透明度属性, 模型本身半透明
		if(opacity < 1.0f)
		{
			pMtrl->bAlphaBlend = TRUE;
		}
		else
		{
			//模型不透明，但有透明贴图
			if (pVGSMtrl->texture.opacityTexture_strLen)
			{
				pMtrl->bAlphaBlend = TRUE;
			}
			else//模型不透明，也没有透明贴图
			{
				pMtrl->bAlphaBlend = FALSE;
			}
		}

		// 包含顶点位置、法线、颜色、4层UV
		pMtrl->m_FVF = FVF_T7;
		pMtrl->m_FVFSize = sizeof(T7MESHVERTEX);

		// 是否diffuse带透明通道
		BOOL bIsAlphaDiffuse = FALSE;

		// bump放在第一层
		if (pVGSMtrl->texture.bumpTexture_strLen > 0) 
		{	
			std::string sFileName = mS3DFileInfo->mStrPurePath;
			sFileName.append(string("\\texture\\"));
			sFileName.append(string(pVGSMtrl->texture.bumpTexture.c_str()));
			std::string sName = _GetPureFileNameStr(sFileName);
			CTextureLayer *pLayer = _CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.bumpTexture_uvIndex, 0);
			if (pLayer)
			{
				pLayer->type = TLT_BUMPMAP;  // bump
				pMtrl->m_vpTexLayers.push_back(pLayer);
				pMtrl->m_bUseDiffuse = false;
			}
		}
		
		//如果相应编号的贴图还没有创建，则为材质创建该编号的贴图
		if (pVGSMtrl->texture.diffuseTexture_strLen > 0)
		{
			std::string sFileName = mS3DFileInfo->mStrPurePath;
			sFileName.append(string("\\texture\\"));
			sFileName.append(string(pVGSMtrl->texture.diffuseTexture.c_str()));
			std::string sName = _GetPureFileNameStr(sFileName);
			CTextureLayer *pLayer = _CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.diffuseTexture_uvIndex);
			if (pLayer)
			{
				pMtrl->m_vpTexLayers.push_back(pLayer);
				pMtrl->m_bUseDiffuse = false;

				//如果是透明通道的图像
				if (_IsTransparentFormat(((CTexture*)pLayer->pTexture)->BP))
				{
					pMtrl->bAlphaTest = true;
					pMtrl->AlpahaTestValue = DEFAULT_ALPHATESTVALUE; // 64
					//pMtrl->bAlphaBlend = true;
					bIsAlphaDiffuse = TRUE;
				}
			}
		}	// if diffuse				

		// 如果diffuse带透明通道，而且opacity通道贴图跟diffuse相同，就丢弃opactity层。我们可以直接透明。
		// 通常，在3ds max中，制作者习惯用两张相同的tga图片制作透明效果		
		if (pVGSMtrl->texture.opacityTexture_strLen > 0)
		{
			std::string sFileName = mS3DFileInfo->mStrPurePath;
			sFileName.append(string("\\texture\\"));
			sFileName.append(string(pVGSMtrl->texture.opacityTexture.c_str()));

			if (!bIsAlphaDiffuse || strcmp(pVGSMtrl->texture.diffuseTexture.c_str(), sFileName.c_str()) != 0)
			{
				std::string sName = _GetPureFileNameStr(sFileName);
				CTextureLayer *pLayer = _CreateTextureLayerFromFile(m_pScene, sName, sFileName, TRUE, pVGSMtrl->texture.opacityTexture_uvIndex);
				if (pLayer)
				{
					pLayer->type = TLT_OPACITY;
					pLayer->Blenderinfo.ColorBlendOption = D3DTOP_ADD; //D3DTOP_DISABLE;  // 透明贴图没有颜色，所以必须是加或者无效
					pLayer->Blenderinfo.AlphaBlendOption = D3DTOP_MODULATE;
 					pMtrl->bAlphaTest = true;
 					pMtrl->AlpahaTestValue = DEFAULT_ALPHATESTVALUE; // 64
 					//pMtrl->bAlphaBlend = true;
					pMtrl->m_vpTexLayers.push_back(pLayer);
					pMtrl->m_bUseDiffuse = false;
				}								
			} // if bIsAlphaDiffuse
		} // if opacity			

		//创建光贴图
		if (pVGSMtrl->texture.selfIlluminationTexture_strLen > 0)
		{
			std::string sFileName = mS3DFileInfo->mStrPurePath;
			sFileName.append(string("\\texture\\"));
			sFileName.append(string(pVGSMtrl->texture.selfIlluminationTexture.c_str()));

			std::string sName = _GetPureFileNameStr(sFileName);
			CTextureLayer *pLayer = _CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.selfIlluminationTexture_uvIndex);
			if (pLayer)
			{
				pMtrl->m_vpTexLayers.push_back(pLayer);
				pMtrl->m_bUseDiffuse = false;

				pMtrl->selfIllumination = 99; // 有了光贴图，自发光设为99。
			}					
		}

		// displacement
		if (pVGSMtrl->texture.displacementTexture_strLen > 0) 
		{	
			std::string sFileName = mS3DFileInfo->mStrPurePath;
			sFileName.append(string("\\texture\\"));
			sFileName.append(string(pVGSMtrl->texture.displacementTexture.c_str()));
			std::string sName = _GetPureFileNameStr(sFileName);
			CTextureLayer *pLayer = _CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.displacementTexture_uvIndex);
			if (pLayer)
			{
				pMtrl->m_vpTexLayers.push_back(pLayer);
				pMtrl->m_bUseDiffuse = false;
			}
		}
		
		//创建反射贴图
		if (pVGSMtrl->texture.reflectionTexture_strLen > 0)
		{
			std::string sFileName = mS3DFileInfo->mStrPurePath;
			sFileName.append(string("\\texture\\"));
			sFileName.append(string(pVGSMtrl->texture.reflectionTexture.c_str()));
			std::string sName = _GetPureFileNameStr(sFileName);
			CTextureLayer *pLayer = _CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.reflectionTexture_uvIndex);
			if (pLayer)
			{
				pLayer->type = TLT_REFLECT;
				pMtrl->m_vpTexLayers.push_back(pLayer);
				pMtrl->m_bUseDiffuse = false;
			}
		}

		// 如果不是用diffuse，设置材质颜色
		if (!pMtrl->m_bUseDiffuse)
		{
			pMtrl->d3dmtrl.Diffuse.r = 1.0f;
			pMtrl->d3dmtrl.Diffuse.g = 1.0f;
			pMtrl->d3dmtrl.Diffuse.b = 1.0f;
			pMtrl->d3dmtrl.Diffuse.a = 1.0f;
		}
		//将pMtrl放入场景material列表
		m_pScene->m_vShader.push_back(pMtrl);

		// 本次加载的材质
		m_vLoadedShader.push_back(pMtrl);

	} // for material

	return 1;
}

//加载相机数据
int	CImportS3D::LoadCameraData()
{
	//逐个相机创建
	int cameraCnt = mS3DFileInfo->cameraManager.cameraCnt;
	for (int i = 0; i< cameraCnt; i++)
	{
		VGS_Camera* pVGSCamera = &(mS3DFileInfo->cameraManager.camera[i]);

		//相机名称
		std::string RealName = std::string(pVGSCamera->name.c_str());
		RealName = m_pScene->RenameCameraIfExist(RealName);

		//创建相机
		CCamera* pCamera = m_pScene->CreateCamera(RealName);

		//是否是轴侧
		if (pVGSCamera->isOrtho)
			pCamera->m_Type = VCT_ORTHO;
		else
			pCamera->m_Type = VCT_PROJECT;

		//相机的From点
		D3DXVECTOR3 from;
		from.x = pVGSCamera->camInfo[0].from.x;
		from.y = pVGSCamera->camInfo[0].from.z;
		from.z = pVGSCamera->camInfo[0].from.y;
		pCamera->SetFrom(&from);

		//相机的at点
		D3DXVECTOR3 at;
		at.x = pVGSCamera->camInfo[0].at.x;
		at.y = pVGSCamera->camInfo[0].at.z;
		at.z = pVGSCamera->camInfo[0].at.y;
		pCamera->SetAt(&at);

		//相机的Roll
		pCamera->SetRoll(pVGSCamera->camInfo[0].roll);

		//Fov//实际上max中默认的是水平方向的fov，而d3d使用的是竖直方向的fov,这里可能需要转化一下
		double tan_half_fovy = 0.75 * tan(pVGSCamera->camInfo[0].fov/2.0f);
		pCamera->m_fov = 2 * atan(tan_half_fovy);

		//near far clip
		pCamera->nearClip = g_CamDefaultNearClip;
		pCamera->farClip  = g_CamDefaultfarClip;

		//创建相机的参数动画
		// CreateCameraParamAnimation(pCamera, pVGSCamera);

	}
	return 1;
}

//加载灯光数据
int	CImportS3D::LoadLightData()
{
	//逐个灯光创建
	int lightCnt = mS3DFileInfo->lightManager.lightCnt;
	for (int i = 0; i< lightCnt; i++)
	{
		VGS_Light* pVGSLight = &(mS3DFileInfo->lightManager.light[i]);
		
		//灯光名称
		std::string RealName =std::string(pVGSLight->name.c_str());
		std::string RenameLog;
		BOOL bRename = RenameLightIfExist(RealName, RenameLog);
		if (bRename) //保存改过的名
		{
			pVGSLight->name = std::string(RealName);
		}

		//渲染同步对象
		//EnterCriticalSection(m_render_cs);

		//创建灯光
		CLight* pLight = m_pScene->CreateLight(RealName);

		//颜色
		D3DCOLORVALUE diffuse;
		diffuse.r = ((pVGSLight->lightInfo[0].color>>16) & 0xff)/255.0f;
		diffuse.g = ((pVGSLight->lightInfo[0].color>>8) & 0xff)/255.0f;
		diffuse.b = ((pVGSLight->lightInfo[0].color) & 0xff)/255.0f;
		diffuse.a = 1.0f;
		pLight->diffuse  = diffuse;
		pLight->multiply = pVGSLight->lightInfo[0].multiply;

		pLight->param.Diffuse.r = diffuse.r * pLight->multiply;
		pLight->param.Diffuse.g = diffuse.g * pLight->multiply;
		pLight->param.Diffuse.b = diffuse.b * pLight->multiply;
		pLight->param.Diffuse.a = 1.0f;

		//环境光，用它来近似模拟全局光
		pLight->ambient   = pLight->diffuse;
		pLight->ambient.a = 1.0f;
		pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
		pLight->param.Ambient.a = 1.0f;

		//高光
		pLight->param.Specular   = pLight->multiply * pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式
		pLight->param.Specular.a = 1.0f;

		//默认灯光范围
		if (pVGSLight->lightInfo[0].range2 == -1) 
		{
			pVGSLight->lightInfo[0].range2 = (float)MAXDWORD;
		}

		//光线到达的最远距离
		pLight->param.Range        = pVGSLight->lightInfo[0].range2;
		pLight->param.Attenuation0 = 1.0f;
		pLight->param.Attenuation1 = 0.0f;
		pLight->param.Attenuation2 = 0.0f;

		pLight->param.Type= D3DLIGHT_POINT;
		D3DXVECTOR3 vPos(pVGSLight->lightInfo[0].pos.x, pVGSLight->lightInfo[0].pos.z, pVGSLight->lightInfo[0].pos.y);  // z 和 y互换
		pLight->setPosition(&vPos);		

		//灯光类型,不同的灯光类型必须设定不同的参数--------------------------		
		if (pVGSLight->type == OMINI)    //omni light
		{	
			pLight->param.Type= D3DLIGHT_POINT;
		}
		else if (pVGSLight->type == SPOTLIGHT)  //spot light
		{
			pLight->param.Type= D3DLIGHT_SPOT;
			
            //光线区域的范围角
			pLight->param.Theta   = pVGSLight->lightInfo[0].theta;
			pLight->param.Phi     = pVGSLight->lightInfo[0].phi;
			pLight->param.Falloff = 1.0f;
			
			D3DXVECTOR3 vDir(pVGSLight->lightInfo[0].dir.x, pVGSLight->lightInfo[0].dir.z, pVGSLight->lightInfo[0].dir.y);		
			pLight->setDirection(&vDir);
		}

		//directional light
		else if (pVGSLight->type == DIRECTIONLIGHT)
		{
			pLight->param.Type= D3DLIGHT_DIRECTIONAL;
	
			//光线Spot区域
			pLight->param.Falloff = 1.0f;

			D3DXVECTOR3 vDir(pVGSLight->lightInfo[0].dir.x, pVGSLight->lightInfo[0].dir.z, pVGSLight->lightInfo[0].dir.y);  // z 和 y互换
			pLight->setDirection(&vDir);
		}

		//渲染同步对象
		//LeaveCriticalSection(m_render_cs);

		//灯光参数动画-----------------------------------------------------------------
		// CreateLightParamAnimation(pLight, pVGSLight);

	} // for each light

	return 1;
}

//加载骨骼数据
int	CImportS3D::LoadSkeletonData()
{
	/*//逐个骨骼加载
	int skeletonCnt = mS3DFileInfo->skeletonManager.skeletonCnt;
	for (int i = 0; i< skeletonCnt; i++)
	{
		VGS_Skeleton* pVGSSkeleton = &(mS3DFileInfo->skeletonManager.skeleton[i]);

		//骨骼名称
		std::string RealName = std::string(pVGSSkeleton->name.c_str());

		if (Ogre::SkeletonManager::getSingleton().getByName(RealName).get() != NULL)
		{
			RealName = RealName + std::string("_ReN") + Ogre::StringConverter::toString(gRenamedSkeletonCnt);
			gRenamedSkeletonCnt++;
			pVGSSkeleton->name = std::string(RealName);
		}
		
		//渲染同步对象
		//EnterCriticalSection(m_render_cs);
		//创建骨骼
		Ogre::SkeletonPtr pSkeleton = Ogre::SkeletonManager::getSingleton().create(RealName, mResourceGroupName);
		//渲染同步对象
		//LeaveCriticalSection(m_render_cs);

		//创建骨骼动画，指定骨骼动画的长度
		int animationCnt = pVGSSkeleton->animationCnt;
		for (int j = 0; j< animationCnt; j++)
		{
			VGS_SkeletonAnimation* pAnimation = pVGSSkeleton->animationList[j];

			std::string AnimationName = std::string(pAnimation->name.c_str());
			float animationLen = mKeyFrameStartPlayTime + 1.0f * pAnimation->frameCnt / gkfrPlaySpeed;
			Animation* pSkeletonAni = pSkeleton->createAnimation(AnimationName, animationLen);
			
			// 设置关键帧之间的插值方法为样条插值
			pSkeletonAni->setDefaultInterpolationMode(Animation::IM_LINEAR);
			
			//递归创建骨骼中的骨头, 骨头只能创建一次
			if (j == 0)
			{
				int boneIndex = 0;
				//渲染同步对象
				//EnterCriticalSection(m_render_cs);
				CreateBone(pSkeleton, pSkeletonAni, &(pAnimation->bone), boneIndex); 
				//保存绑定设置
				pSkeleton->setBindingPose();
				//渲染同步对象
				//LeaveCriticalSection(m_render_cs);
			}
			else //向已有的骨头中增加动画
			{
				//创建新的骨头动画
				int boneIndex = 0;
				CreateBoneAnimation(pSkeleton, pSkeletonAni, &(pAnimation->bone), boneIndex);
			}
		}
		
		//渲染同步对象
		//EnterCriticalSection(m_render_cs);
		//优化所有的动画
		pSkeleton->optimiseAllAnimations();
		//渲染同步对象
		//LeaveCriticalSection(m_render_cs);
	}
*/
	return 1;
}

//递归创建骨头
/*Bone* CImportS3D::CreateBone(Ogre::SkeletonPtr pSkeleton, Animation* pSkeletonAni, VGS_Bone* pVGSBone, int& boneIndex)
{
	//创建骨头
	Bone* pBone = pSkeleton->createBone(std::string(pVGSBone->name.c_str()));
	
	//创建骨头的姿态--------------------------------------------------------------
	//位置//转化坐标系
	Vector3 pos; 
	pos.x = pVGSBone->pose.position.x;
	pos.y = pVGSBone->pose.position.z;
	pos.z = -pVGSBone->pose.position.y;
	pBone->setPosition(pos);

	Ogre::Quaternion quat;
	quat.x = -pVGSBone->pose.quat.x;
	quat.y = -pVGSBone->pose.quat.z;
	quat.z = pVGSBone->pose.quat.y;
	quat.w = pVGSBone->pose.quat.w;
	pBone->setOrientation(quat);

	//缩放//转化坐标系
	Vector3 scale;
	scale.x = pVGSBone->pose.scale.x;
	scale.y = pVGSBone->pose.scale.z;
	scale.z = pVGSBone->pose.scale.y;
	pBone->setScale(scale);

	//创建骨头的动画-------------------------------------------------------------------
	//动画帧数
	int frameCnt = pVGSBone->transform.size();
	if (frameCnt > 1)
	{
		NodeAnimationTrack* pNodeAniTrack = pSkeletonAni->createNodeTrack(boneIndex, pBone);
		for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
		{
			TransformKeyFrame* pTransKfr =  pNodeAniTrack->createNodeKeyFrame(mKeyFrameStartPlayTime + (1.0f * frameIndex) / gkfrPlaySpeed);
					
			//骨头关键帧动画信息：
			//1. setTranslate: 在父级坐标系中，该骨头当前帧相对初始姿态的位移
			//2. setRotation: 在父级坐标系中，该骨头当前帧相对初始姿态的转角
			//3. setScale: 该骨头当前帧相对初始姿态的缩放
			
			//位置变换
			Vector3 pos;
			pos.x = pVGSBone->transform[frameIndex].position.x;
			pos.y = pVGSBone->transform[frameIndex].position.z;
			pos.z = -pVGSBone->transform[frameIndex].position.y;
			pTransKfr->setTranslate(pos);

			//转角变换
			Quaternion quat;
			quat.x = -pVGSBone->transform[frameIndex].quat.x;
			quat.y = -pVGSBone->transform[frameIndex].quat.z;
			quat.z = pVGSBone->transform[frameIndex].quat.y;
			quat.w = pVGSBone->transform[frameIndex].quat.w;
			pTransKfr->setRotation(quat);

			//缩放变换 
			Vector3 scale;
			scale.x = pVGSBone->transform[frameIndex].scale.x;
			scale.y = pVGSBone->transform[frameIndex].scale.z;
			scale.z = pVGSBone->transform[frameIndex].scale.y;
			pTransKfr->setScale(scale);	
		}
	}
	boneIndex++;

	//创建子骨头
	int childCnt = pVGSBone->childBone.size();
	for (int i = 0; i< childCnt; i++)
	{
		VGS_Bone* pChildVGSBone = &(pVGSBone->childBone[i]);
		Bone* pChildBone = CreateBone(pSkeleton, pSkeletonAni, pChildVGSBone, boneIndex);
		pBone->addChild(pChildBone);
	}

	return pBone;
}*/

//递归创建骨头动画
/*Bone* CImportS3D::CreateBoneAnimation(Ogre::SkeletonPtr pSkeleton, Animation* pSkeletonAni, VGS_Bone* pVGSBone, int& boneIndex)
{
	//找到骨头
	//Bone* pBone = pSkeleton->getBone(std::string(pVGSBone->name.c_str()));
	Bone* pBone = pSkeleton->getBone(boneIndex);
	
	//创建骨头的动画-------------------------------------------------------------------
	//动画帧数
	int frameCnt = pVGSBone->transform.size();
	if (frameCnt > 1)
	{
		NodeAnimationTrack* pNodeAniTrack = pSkeletonAni->createNodeTrack(boneIndex, pBone);
		for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
		{
			TransformKeyFrame* pTransKfr =  pNodeAniTrack->createNodeKeyFrame(1.0f * frameIndex / gkfrPlaySpeed);
					
			//骨头关键帧动画信息：
			//1. setTranslate: 在父级坐标系中，该骨头当前帧相对初始姿态的位移
			//2. setRotation: 在父级坐标系中，该骨头当前帧相对初始姿态的转角
			//3. setScale: 该骨头当前帧相对初始姿态的缩放
			
			//位置变换
			Vector3 pos;
			pos.x = pVGSBone->transform[frameIndex].position.x;
			pos.y = pVGSBone->transform[frameIndex].position.z;
			pos.z = -pVGSBone->transform[frameIndex].position.y;

			//转角变换
			Quaternion quat;
			quat.x = -pVGSBone->transform[frameIndex].quat.x;
			quat.y = -pVGSBone->transform[frameIndex].quat.z;
			quat.z = pVGSBone->transform[frameIndex].quat.y;
			quat.w = pVGSBone->transform[frameIndex].quat.w;

			//缩放变换 
			Vector3 scale;
			scale.x = pVGSBone->transform[frameIndex].scale.x;
			scale.y = pVGSBone->transform[frameIndex].scale.z;
			scale.z = pVGSBone->transform[frameIndex].scale.y;

			pTransKfr->setTranslate(pos);
			pTransKfr->setRotation(quat);
			pTransKfr->setScale(scale);	
		}
	}
	boneIndex++;

	//创建子骨头动画
	int childCnt = pVGSBone->childBone.size();
	for (int i = 0; i< childCnt; i++)
	{
		VGS_Bone* pChildVGSBone = &(pVGSBone->childBone[i]);
		Bone* pChildBone = CreateBoneAnimation(pSkeleton, pSkeletonAni, pChildVGSBone, boneIndex);
	}

	return pBone;
}
*/

//加载Mesh模型数据
int	CImportS3D::LoadMeshData()
{
	int preModelCnt = m_pScene->GetModelCount();

	//逐个加载Mesh的数据
	int meshCnt = mS3DFileInfo->meshManager.meshCnt;

	for (int i = 0; i < meshCnt; i++)
	{
		VGS_Mesh* pVGSMesh = &(mS3DFileInfo->meshManager.mesh[i]);

		//模型名
		std::string RealName = m_pScene->RenameModelIfExist(pVGSMesh->name.c_str());
		pVGSMesh->name = std::string(RealName);

		//渲染同步对象
		//EnterCriticalSection(m_render_cs);

		CModel* pModel = new CModel(m_pScene->m_pd3dDevice);

		if (!pModel)
		{
			continue;
		}

		//模型编号
		pModel->index = preModelCnt + i;
		pModel->setName(RealName);
		
		//顶点数量
		pModel->vertexCnt = pVGSMesh->vertexCnt;
		pModel->vertex = new CVertex[pModel->vertexCnt];
		CVertex *pVertex = pModel->vertex;
		for (UINT j = 0; j < pModel->vertexCnt; ++j)
		{
			VGS_Vertex* pVGSVertex = &(pVGSMesh->vertex[j]);

			pVertex[j].index = j;

			//填入顶点位置//变换坐标系
			pVertex[j].position.x = pVGSVertex->position.x;
			pVertex[j].position.y = pVGSVertex->position.z;
			pVertex[j].position.z = pVGSVertex->position.y;
			
			//填入顶点法线//变换坐标系
			pVertex[j].normal.x = pVGSVertex->normal.x;
			pVertex[j].normal.y = pVGSVertex->normal.z;
			pVertex[j].normal.z = pVGSVertex->normal.y;

			//填入顶点的颜色
			pVertex[j].diffuse = (D3DCOLOR)pVGSVertex->color;

			//填入顶点的Diffuse 通道UV
			pVertex[j].uv1.u = pVGSVertex->uv1.u;
			pVertex[j].uv1.v = 1.0f - pVGSVertex->uv1.v;

			//填入顶点的LightMap 通道WZ
			pVertex[j].uv2.u = pVGSVertex->uv2.u;
			pVertex[j].uv2.v = 1.0f - pVGSVertex->uv2.v;

			//填入顶点的LightMap 通道WZ
			pVertex[j].uv3.u = pVGSVertex->uv3.u;
			pVertex[j].uv3.v = 1.0f - pVGSVertex->uv3.v;

			//填入顶点的LightMap 通道WZ
			pVertex[j].uv4.u = pVGSVertex->uv4.u;
			pVertex[j].uv4.v = 1.0f - pVGSVertex->uv4.v;

		} // for j


		int subMeshCnt = pVGSMesh->subMesh.size();
		for (int subMeshIndex = 0; subMeshIndex < subMeshCnt; ++subMeshIndex)
		{
			VGS_SubMesh* pVGS_SubMesh = &(pVGSMesh->subMesh[subMeshIndex]);

			//创建SubMesh对象
			_subMesh pSubMesh;

			pSubMesh.faceCnt = pVGS_SubMesh->face.size();
			pSubMesh.face = new CFace[pSubMesh.faceCnt];
			
			//逐个面填入索引缓冲数据
			for (int k = 0; k < pSubMesh.faceCnt; ++k)
			{
				pSubMesh.face[k].index = k;
				int a = pVGS_SubMesh->face[k].a;
				int b = pVGS_SubMesh->face[k].b;
				int c = pVGS_SubMesh->face[k].c;
				pSubMesh.face[k].vertices0 = &(pModel->vertex[a]);
				pSubMesh.face[k].vertices1 = &(pModel->vertex[b]);
				pSubMesh.face[k].vertices2 = &(pModel->vertex[c]);
			}
			
			//找到当前subMesh的材质
			int mtrlIndex = pVGS_SubMesh->mtrlIndex;
			if (mtrlIndex >= 0 && mtrlIndex < m_vLoadedShader.size())
			{
				pSubMesh.shader = m_vLoadedShader[mtrlIndex];
			}
			else  // 使用缺省材质
			{
				CMaterial *pMaterial = new CMaterial();
				pSubMesh.shader = pMaterial;
			}
			
			pModel->subMesh.push_back(pSubMesh);                // 保存到模型

			RenderSubMesh rsm;
			rsm.modelPointer = (DWORD)pModel;
			rsm.subMeshID = pModel->subMesh.size() - 1;
			pSubMesh.shader->subMeshList.push_back(rsm);   // 使用该材质的子模型列表

		} // for subMeshIndex
		
		//模型的包围盒, 注意变换坐标系
		D3DXVECTOR3 minCorner, maxCorner;
		minCorner.x = pVGSMesh->boundBox.minCorner.x;
		minCorner.y = pVGSMesh->boundBox.minCorner.z;
		minCorner.z = pVGSMesh->boundBox.minCorner.y;
		maxCorner.x = pVGSMesh->boundBox.maxCorner.x;
		maxCorner.y = pVGSMesh->boundBox.maxCorner.z;
		maxCorner.z = pVGSMesh->boundBox.maxCorner.y;
		pModel->boundingBox.minCorner = pVGSMesh->boundBox.minCorner;
		pModel->boundingBox.maxCorner = pVGSMesh->boundBox.maxCorner;

		pModel->oriBBox.maxCorner = pModel->boundingBox.maxCorner;
		pModel->oriBBox.minCorner = pModel->boundingBox.minCorner;

		// 模型被选中时显示的包围盒
		pModel->pRBBox= new CRenderBoundBox(m_pScene->m_pd3dDevice);
		pModel->pRBBox->SetRBBoxVertex(minCorner, maxCorner);
		pModel->showRBBox = false;
		
		D3DXVECTOR3 center;
		D3DXVec3Add(&center, &minCorner, &maxCorner);
		D3DXVECTOR3 vLen = maxCorner - minCorner;		
		pModel->oriBSphere.Center     = center / 2.0f;
		pModel->oriBSphere.Radius     = D3DXVec3Length(&vLen);
		pModel->boundingSphere.Center = pModel->oriBSphere.Center;
		pModel->boundingSphere.Radius = D3DXVec3Length(&vLen);

		pModel->bDynamicUV = false;
		pModel->bReflect = false;
		pModel->bSelected = false;
		pModel->isCollision = false;
		pModel->isGround = false;
		pModel->visible = true;
		
		pModel->m_FVF = FVF_T7;
		pModel->m_FVFSize = sizeof(T7MESHVERTEX);

		// 创建模型缓冲
		pModel->createVBuffer();
		pModel->createIBuffer();

		m_pScene->model.push_back(pModel);  // 保存到模型列表		

	} // for i

	return 1;
}

//共享Entity的骨骼
/*bool CImportS3D::ShareEntitySkeleton(Entity* pEntity)
{
	//遍历该S3D已经加载的Entity对象
	Ogre::SceneManager::MovableObjectIterator RSit = mSceneMgr->getMovableObjectIterator(Ogre::EntityFactory::FACTORY_TYPE_NAME);
	while (RSit.hasMoreElements())
	{
		Entity* sEntity = (Entity*)RSit.getNext();
		if (sEntity != pEntity)
		{
			SkeletonInstance* sEntityInst = sEntity->getSkeleton();
			SkeletonInstance* pEntityInst = pEntity->getSkeleton();
			
			if (sEntityInst && pEntityInst && (sEntityInst->getName() == pEntityInst->getName()))
			{
				pEntity->shareSkeletonInstanceWith(sEntity);
				return true;
			}
		}
	}
	return false;
}
*/
//邦定Mesh的骨骼
/*void CImportS3D::BindMeshToSkeleton(MeshPtr pMesh, VGS_Mesh* pVGSMesh, SkeletonPtr pSkeleton)
{
	int boneAssCnt = pVGSMesh->bindingData.size();
	for (int i = 0; i< boneAssCnt; i++)
	{
		BoneAss* pBoneAss = &(pVGSMesh->bindingData[i]);
		//顶点权重
		Ogre::VertexBoneAssignment_s vBoneAss;
		vBoneAss.vertexIndex = pBoneAss->vertexIndex;
		vBoneAss.boneIndex = pBoneAss->bindingBoneIndex;
		vBoneAss.weight = pBoneAss->bindingWeight;
		pMesh->addBoneAssignment(vBoneAss);
	}

	//根骨头的变换
	Bone* pRootBone = pSkeleton->getRootBone();
	Vector3 pos;
	pos.x = pVGSMesh->skeletontransform.position.x;
	pos.y = pVGSMesh->skeletontransform.position.z;
	pos.z = -pVGSMesh->skeletontransform.position.y;
	pRootBone->setPosition(pos);

	Quaternion quat;
	quat.x = -pVGSMesh->skeletontransform.quat.x;
	quat.y = -pVGSMesh->skeletontransform.quat.z;
	quat.z = pVGSMesh->skeletontransform.quat.y;
	quat.w = pVGSMesh->skeletontransform.quat.w;
	pRootBone->setOrientation(quat);
	
	Vector3 scale;
	scale.x = pVGSMesh->skeletontransform.scale.x;
	scale.y = pVGSMesh->skeletontransform.scale.z;
	scale.z = pVGSMesh->skeletontransform.scale.y;
	pRootBone->setScale(scale);
	
	//建立骨骼结构和Mesh之间的连接
	pMesh->_notifySkeleton(pSkeleton);

	//更新材质
	pMesh->updateMaterialForAllSubMeshes();
	
}*/

//加载Shape数据
int	CImportS3D::LoadShapeData()
{
	return 1;
}

//加载场景图
int	CImportS3D::LoadSceneGraphData()
{
	int nodeCnt = mS3DFileInfo->sceneNodeGraph.nodeCnt;

	VGS_Node* pVGSNode = &(mS3DFileInfo->sceneNodeGraph.node);
	
	//创建场景节点
	CSceneNode* pRootNode = m_pScene->GetRootSceneNode();
	CreateSceneNode(pRootNode, pVGSNode);

	return 1;
}

//创建场景图
void CImportS3D::CreateSceneNode(CSceneNode* pNode, VGS_Node* pVGSNode)
{	
	CSceneNode* pChildNode = NULL;
	std::string RealName = "";
	bool	bHasSkeleton = false; 

	CLight* pLight = NULL;
	CModel* pModel = NULL;

	//渲染同步对象
	//EnterCriticalSection(m_render_cs);

	//Node的类型
	NodeType type = pVGSNode->type;

	pNode->name = pVGSNode->name.c_str();

	//找到对应编号的对象
	switch (type)
	{
	//ogre已经创建场景根节点
	case NNT_SCENEROOT:
		pChildNode = pNode;
		break;

	case NNT_CAMERANODE:
		{
			RealName = std::string(mS3DFileInfo->cameraManager.camera[pVGSNode->index].name.c_str());
			pChildNode = pNode->CreateChild();
			//pChildNode->attachObject(NNT_CAMERANODE, (DWORD)m_pScene->GetCameraByName(RealName));
			
			CCamera* pCamera = m_pScene->GetCameraByName(RealName);
			pChildNode->attachObject(NNT_CAMERANODE, (DWORD)pCamera);
			pCamera->m_pNode = pChildNode;
			//pCamera->m_pNode->updateTransform();
		}
		break; 

	case NNT_LIGHTNODE:
		{
			RealName = std::string(mS3DFileInfo->lightManager.light[pVGSNode->index].name.c_str());
			pChildNode = pNode->CreateChild();
			
			pLight = m_pScene->GetLightByName(RealName);
			pChildNode->attachObject(NNT_LIGHTNODE, (DWORD)pLight);
			pLight->pNode = pChildNode;
			//pLight->pNode->updateTransform();
			
		}
		break;

	case NNT_MODEL:
		{
			int count = mS3DFileInfo->meshManager.meshCnt;
			int index = pVGSNode->index;
			RealName = mS3DFileInfo->meshManager.mesh[index].name;
			pChildNode = pNode->CreateChild();

			pModel = m_pScene->GetModelByName(RealName);
			pChildNode->attachObject(NNT_MODEL, (DWORD)pModel);
			pModel->pNode = pChildNode;
			//pModel->pNode->updateTransform();
			//pModel->update();//把模型的位置变换给RBBox

			
			if(pModel->pNode->keyFrameCnt > 0)
			{
				pModel->kfrCnt = pModel->pNode->keyFrameCnt;
				pModel->startFrame = 0;
				pModel->endFrame = pModel->kfrCnt - 1;
				pModel->kfrCurFrame = 0;
				if (pModel->kfrCnt > m_pScene->m_KfrCnt)
				{
					m_pScene->m_KfrCnt = pModel->kfrCnt;
					m_pScene->m_EndFrame = pModel->kfrCnt - 1;
				}
			}

			//把模型的位置变换给RBBox
			// pModel->pRBBox->SetTransformMatrix(&pChildNode->curTransform.worldMatrix);
		}
		break;

	case NNT_NONE:
		return;
		
	}

	//渲染同步对象
	//LeaveCriticalSection(m_render_cs);

	//Node的变换, 相机的变换不在这里处理---------------------------------------------------------
	//位置//转化坐标系
	if (type == NNT_SCENEROOT)
	{
		pChildNode->setPosition(&(D3DXVECTOR3(0, 0, 0)));
		pChildNode->updateTransform();
	}
	else
	{		
		//缩放//转化坐标系
		D3DXVECTOR3 scale;
		scale.x = pVGSNode->keyframeTransform[0].scale.x;
		scale.y = pVGSNode->keyframeTransform[0].scale.z;
		scale.z = pVGSNode->keyframeTransform[0].scale.y;
		pChildNode->setScale(&scale, 2);
		pChildNode->updateTransform();	
		
		//旋转//转化坐标系
		D3DXQUATERNION quat;
		quat.x = pVGSNode->keyframeTransform[0].quat.x;
		quat.y = pVGSNode->keyframeTransform[0].quat.z;
		quat.z = pVGSNode->keyframeTransform[0].quat.y;
		quat.w = pVGSNode->keyframeTransform[0].quat.w;
		pChildNode->setRotationByQuat(&quat);
		pChildNode->updateTransform();

		D3DXVECTOR3 pos;
		pos.x = pVGSNode->keyframeTransform[0].position.x;
		pos.y = pVGSNode->keyframeTransform[0].position.z;
		pos.z = pVGSNode->keyframeTransform[0].position.y;
		pChildNode->setPosition(&pos);
		pChildNode->updateTransform();


		if (type == NNT_CAMERANODE)
		{
			CCamera* pCamera = m_pScene->GetCameraByName(RealName);

			//将S3D文件中相机动画数据，转化为Node的动画变换
			VGS_Camera* pVGSCamera = &(mS3DFileInfo->cameraManager.camera[pVGSNode->index]);
			for (int frameIndex = 0; frameIndex< pVGSCamera->camInfo.size(); frameIndex++)
			{
				ConvCamTransformToNodeTransform(&(pVGSNode->keyframeTransform[frameIndex]), &(pVGSCamera->camInfo[frameIndex]));
			}
		}		
		else
		{			
			if (type == NNT_MODEL && pModel)
			{				
				// 如果模型有缩放,将模型设置为自动计算发现，避免光照计算错误
				if (D3DXVec3Length(&(pChildNode->worldScale)) > 0) // (pChildNode->getDerivedScaled())
				{					
					pModel->isAutoNormalise = true;
					// 设置是否重新计算模型法线，保证缩放模型能正常显示高光
					// m_pScene->m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
				}	
				pModel->update(); // 更新包围盒
				
			}
			else if (type == NNT_LIGHTNODE)
			{
				if (pLight)
				{
					pLight->setPosition(&pos);
				}
// 				if (mS3DFileInfo->header.s3dVersion >= 3.02f)
// 				{
// 					D3DXVECTOR3 pos(0, 0, 0);
// 					pChildNode->setPosition(&pos);
// 					D3DXQUATERNION quat(1, 0, 0, 0);
// 					pChildNode->setRotationByQuat(&quat);
// 					D3DXVECTOR3 sc(1, 1, 1);
// 					pChildNode->setScale(&sc, 1);
// 				}
			}
		
		}
	}

	//渲染同步对象
	//EnterCriticalSection(m_render_cs);
	//Node的关键帧动画----------------------------------------------------
	if (pVGSNode->keyframeTransform.size() > 1 && !bHasSkeleton) //有骨骼动画则忽略关键帧动画
	{
		CreateNodeAnimation(pChildNode, pVGSNode, type);
	}
	//渲染同步对象
	//LeaveCriticalSection(m_render_cs);
	

	int childNodeCnt = pVGSNode->childNode.size();
	for (int i = 0; i< childNodeCnt; i++)
	{
		VGS_Node* pChildVGSNode = &(pVGSNode->childNode[i]);
		CreateSceneNode(pChildNode, pChildVGSNode);
	}
}

//创建Node的动画
void CImportS3D::CreateNodeAnimation(CSceneNode* pNode, VGS_Node* pVGSNode, NodeType type)
{
	if (!pNode || !pVGSNode)
	{
		return;
	}

	//创建动画对象
	int frameCnt = pVGSNode->keyframeTransform.size();
	std::string strAnimName = _GetNodeAnimationName(pNode);

	CNodeAnimation *pAnim = new CNodeAnimation(pNode, strAnimName);  // 创建节点的关键帧动画

	if (!pAnim)
	{
		return;
	}

	VGS_Camera*      pVGSCamera = NULL;   // 相机的参数动画信息
	VGS_CAMERA_INFO* pCamInfo = NULL;

	VGS_Light*      pVGSLight = NULL;    // 灯光的参数动画信息
	VGS_LIGHT_INFO* pLightInfo = NULL;
	if (pNode->m_Type == NNT_CAMERANODE && pNode->m_object != 0)
	{
		int index = ((CCamera*)pNode->m_object)->index;
		if (index >= 0 && index < mS3DFileInfo->cameraManager.cameraCnt)
		{
			pVGSCamera = &(mS3DFileInfo->cameraManager.camera[index]);
		}
	}
	else if (pNode->m_Type == NNT_LIGHTNODE && pNode->m_object != 0)
	{
		int index = ((CLight*)pNode->m_object)->index;
		if (index >= 0 && index < mS3DFileInfo->lightManager.lightCnt)
		{
			pVGSLight = &(mS3DFileInfo->lightManager.light[index]);
		}
	}

	//定义动画轨迹包含的关键帧变换对象，在这里每一帧都是关键帧
	for (INT frameIndex = 0; frameIndex < frameCnt; frameIndex++)
	{
		NodeTransform *pTransform = &(pVGSNode->keyframeTransform[frameIndex]);           // 相机的节点动画

		CNodeKeyFrame *pKeyFrame = NULL;
		if (pNode->m_Type == NNT_CAMERANODE)
		{			
			if (pVGSCamera && frameIndex >= 0 && frameIndex < pVGSCamera->camInfo.size())
			{
				pCamInfo = &pVGSCamera->camInfo[frameIndex];
			}			
			pKeyFrame = CreateCameraKeyFrame(pNode, pTransform, pCamInfo);
		}
		else if (pNode->m_Type == NNT_LIGHTNODE)
		{			
			if (pVGSLight && frameIndex >= 0 && frameIndex < pVGSLight->lightInfo.size())
			{
				pLightInfo = &pVGSLight->lightInfo[frameIndex];
			}
			pKeyFrame = CreateLightKeyFrame(pNode, pTransform, pLightInfo);
		}
		else if (pNode->m_Type == NNT_MODEL)
		{
			pKeyFrame = CreateModelKeyFrame(pNode, pTransform);
		}

		pAnim->addKeyframe(pKeyFrame); // 添加一帧动画
		pAnim->setStartPlayTime(mKeyFrameStartPlayTime);
		pAnim->setLength(mKeyFrameStartPlayTime + (1.0f * frameCnt) / gkfrPlaySpeed);

	}

	//创建动画状态对象
	pAnim->setEnabled(true);
	pAnim->setLoop(true);

	// 增加一个节点动画
	CAnimationManager::getSingleton().AddNodeKeyfrmAnimation(pAnim);
}

//  创建相机关键帧动画
CNodeKeyFrame* CImportS3D::CreateCameraKeyFrame(CSceneNode *pNode, NodeTransform* pNodeTransform, VGS_CAMERA_INFO* pCamParam)
{
	if ( !pNode || (!pNodeTransform && !pCamParam) )
	{
		return 0;
	}

	CCameraKeyFrame *pCamKfr = new CCameraKeyFrame(pNode);
	if (!pCamKfr)
	{
		return 0;
	}

	// 设置相机的节点动画
	if (pNodeTransform)
	{
		// pCamKfr->setFrom(VECTOR3(pNodeTransform->position.x, pNodeTransform->position.y, pNodeTransform->position.z));
		// pCamKfr->setAt(VECTOR3(pNodeTransform->))
	}

	// 设置相机的参数动画
	if (pCamParam)
	{
		//Fov//实际上max中默认的是水平方向的fov，而d3d使用的是竖直方向的fov,这里可能需要转化一下
		double tan_half_fovy = 0.75 * tan(pCamParam->fov/2.0f);
		double fovy = 2 * atan(tan_half_fovy);
		pCamKfr->setFov(fovy);
		pCamKfr->setRoll(pCamParam->roll);
		pCamKfr->setClip(10, 100000);
		pCamKfr->setFrom(VECTOR3(pCamParam->from.x, pCamParam->from.z, pCamParam->from.y));
		pCamKfr->setAt(VECTOR3(pCamParam->at.x, pCamParam->at.z, pCamParam->at.y));
	}

	return pCamKfr;
}
//  创建灯光键帧动画
CNodeKeyFrame* CImportS3D::CreateLightKeyFrame(CSceneNode *pNode, NodeTransform* pNodeTransform, VGS_LIGHT_INFO* pLightParam)
{
	if ( !pNode || (!pNodeTransform && !pLightParam) )
	{
		return 0;
	}
	
	CLightKeyFrame *pLightKfr = new CLightKeyFrame(pNode);
	if (!pLightKfr)
	{
		return 0;
	}
	
	// 设置相机的节点动画
	if (pNodeTransform)
	{
		
	}
	
	// 设置相机的参数动画
	if (pLightParam)
	{
		pLightKfr->setPosition(VECTOR3(pLightParam->pos.x, pLightParam->pos.y, pLightParam->pos.z));

		//颜色
		float r = ((pLightParam->color>>16) & 0xff) / 255.0f * pLightParam->multiply;
		float g = ((pLightParam->color>>8) & 0xff) / 255.0f * pLightParam->multiply;
		float b = ((pLightParam->color) & 0xff) / 255.0f * pLightParam->multiply;
		pLightKfr->setColor(r, g, b, 1.0f);
		pLightKfr->setRange(pLightParam->range2);		
		pLightKfr->setAttenuation();
		
		CLight *pLight = (CLight*)pNode->m_object;
		if (pLight && pLight->getType() == VLT_DIR)
		{
			pLightKfr->setDirection(VECTOR3(pLightParam->dir.x, pLightParam->dir.y, pLightParam->dir.z));
		}
		if (pLight && pLight->getType() == VLT_SPOT)
		{
			pLightKfr->setDirection(VECTOR3(pLightParam->dir.x, pLightParam->dir.y, pLightParam->dir.z));
			pLightKfr->setAngle(pLightParam->theta, pLightParam->phi);
		}
	}
	
	return pLightKfr;
}
//  创建模型关键帧动画
CNodeKeyFrame* CImportS3D::CreateModelKeyFrame(CSceneNode *pNode, NodeTransform* pNodeTransform)
{
	CModelKeyFrame* pModelAnim = new CModelKeyFrame(pNode);
	if (pModelAnim && pNodeTransform)
	{
		pModelAnim->setScale(VECTOR3(pNodeTransform->scale.x, pNodeTransform->scale.z, pNodeTransform->scale.y));
		pModelAnim->setTranslate(VECTOR3(pNodeTransform->position.x, pNodeTransform->position.z, pNodeTransform->position.y));
		pModelAnim->setRotation(VGS_QUATERNION(pNodeTransform->quat.x, pNodeTransform->quat.z, pNodeTransform->quat.y, pNodeTransform->quat.w));
		return pModelAnim;
	}
	return 0;
}

//转化S3D的相机参数到V3D的相机参数格式
void CImportS3D::ConvCamTransformToNodeTransform(NodeTransform* pNodeTransform, VGS_CAMERA_INFO* pS3DCamInfo)
{

}



//计算场景中已有的材质数量
int CImportS3D::GetSceneMtrlCount()
{
	return m_pScene->m_vShader.size();
}

//计算场景中已有的相机数量
int CImportS3D::GetSceneCameraCount()
{
	return m_pScene->m_vCamera.size();
}

//计算场景中已有的灯光数量
int CImportS3D::GetSceneLightCount()
{
	return m_pScene->m_vLight.size();
}

//计算场景中已经有的骨骼数量
int CImportS3D::GetSceneSkeletonCount()
{
	int count = 0;

	return count;
}

//计算场景中已经有的Mesh数量
int CImportS3D::GetSceneMeshCount()
{
	int count = 0;
	return count;
}

//计算场景中已有的Entity数量
int	CImportS3D::GetSceneEntityCount()
{
	return m_pScene->model.size();
}

/**********************************************************************
* 函数名称： // RenameMaterialIfExist
* 功能描述： // 如果场景中有同名材质，则自动更名
* 输入参数： // 
				RealName		: 材质名
				ChangeNameLog	: 更名的信息记录
* 输出参数： // 无。
* 返 回 值： // 有重名的材质返回TRUE, 否则返回FALSE。 
* 其它说明： // 无。
*
* 修改日期		版本号      修改人	      修改内容
* ----------------------------------------------	-
* 
***********************************************************************/
bool CImportS3D::RenameMaterialIfExist(std::string& RealName, std::string& RenameLog)
{
	RealName = m_pScene->RenameMaterialIfExist(RealName);	
	return true;
	
}


/**********************************************************************
* 函数名称： // RenameCameraIfExist
* 功能描述： // 如果场景中有同名相机，则自动更名
* 输入参数： // 
				RealName		: 相机名
				ChangeNameLog	: 更名的信息记录
* 输出参数： // 无。
* 返 回 值： // 有重名的相机返回TRUE, 否则返回FALSE。 
* 其它说明： // 无。
*
* 修改日期		版本号      修改人	      修改内容
* ----------------------------------------------	-
* 
***********************************************************************/
bool CImportS3D::RenameCameraIfExist(std::string& RealName, std::string& RenameLog)
{
	RealName = m_pScene->RenameCameraIfExist(RealName);	
	return true;

}

/**********************************************************************
* 函数名称： // RenameLightIfExist
* 功能描述： // 如果场景中有同名灯光，则自动更名
* 输入参数： // 
				RealName		: 灯光名
				ChangeNameLog	: 更名的信息记录
* 输出参数： // 无。
* 返 回 值： // 有重名的灯光返回TRUE, 否则返回FALSE。 
* 其它说明： // 无。
*
* 修改日期		版本号      修改人	      修改内容
* ----------------------------------------------	-
* 
***********************************************************************/
bool CImportS3D::RenameLightIfExist(std::string& RealName, std::string& RenameLog)
{
	RealName = m_pScene->RenameLightIfExist(RealName);	
	return true;

}


/**********************************************************************
* 函数名称： // RenameSkeletonIfExist
* 功能描述： // 如果场景中有同名骨骼，则自动更名
* 输入参数： // 
				RealName		: 骨骼名
				ChangeNameLog	: 更名的信息记录
* 输出参数： // 无。
* 返 回 值： // 有重名骨骼返回TRUE, 否则返回FALSE。 
* 其它说明： // 无。
*
* 修改日期		版本号      修改人	      修改内容
* ----------------------------------------------	-
* 
***********************************************************************/
bool CImportS3D::RenameSkeletonIfExist(std::string& RealName, std::string& RenameLog)
{
	bool result = FALSE;
	/*std::string oriName = RealName;

	while (true)
	{
		bool bExist = false; //  是否已经存在
		Ogre::SkeletonManager::ResourceMapIterator RSit = Ogre::SkeletonManager::getSingleton().getResourceIterator();
		while (RSit.hasMoreElements())
		{
			SkeletonPtr pSkeleton = RSit.getNext();
			if (pSkeleton->getName() == RealName)
			{
				RealName = oriName + "_ReN" + Ogre::StringConverter::toString(gRenamedSkeletonCnt);
				gRenamedSkeletonCnt++;
				// result = TRUE;
				bExist = true;
				break;
			}
		}
		if (!bExist)
		{
			result = true;
			break;
		}
	}

	if (result)
		RenameLog = "VGS Find a same name skeleton [" + oriName + "] in the scene, it " + "has been renamed to [" + RealName + "].";
	*/
	return result;
}


/**********************************************************************
* 函数名称： // RenameMeshIfExist
* 功能描述： // 如果场景中有同名模型，则自动更名
* 输入参数： // 
				RealName	: 模型名
				RenameLog	: 更名的信息记录
* 输出参数： // 无。
* 返 回 值： // 有重名的模型返回TRUE, 否则返回FALSE。 
* 其它说明： // 无。
*
* 修改日期		版本号      修改人	      修改内容
* ----------------------------------------------	-
* 
***********************************************************************/
bool CImportS3D::RenameMeshIfExist(std::string& RealName, std::string& RenameLog)
{
	RealName = m_pScene->RenameModelIfExist(RealName);	
	return true;
}

//判断当前贴图是否已经被使用
bool CImportS3D::IsTextureImageInused(const std::string &ImageFileName)
{
	return false;
	//return (m_textureset.find(ImageFileName) != m_textureset.end());
}

UINT CImportS3D::GetTextureImageCnt()
{
	// return m_textureset.size();
	return 0;
}

//释放资源
void CImportS3D::Release()
{
	//删除贴图材质关联列表
	// ReleaseTextureMtrlDev();
	//删除模型骨骼关联列表
	// ReleaseSkeletonModelDev();
}

//删除贴图
void CImportS3D::ReleaseTextureMtrlDev()
{
	// m_textureset.clear();
}

//删除模型骨骼关联列表
void CImportS3D::ReleaseSkeletonModelDev()
{/*
	int skeModelDevCnt = m_skeletonModelDev.size();
	for (int i = 0; i< skeModelDevCnt; i++)
	{
		VGS_SkeletonModelDev* pSkeletonModelDev = &(m_skeletonModelDev[i]);
		pSkeletonModelDev->modelName.clear();
		pSkeletonModelDev->skeletonName.clear();
	}
	m_skeletonModelDev.clear();*/
}

//判断相机是否已经存在
BOOL CImportS3D::IsCameraInScene(std::string strName)
{
	CCamera* pCamera = m_pScene->GetCameraByName(strName);
	return (pCamera != NULL);
}

//判断灯光是否已经存在
BOOL CImportS3D::IsLightInScene(std::string strName)
{
	CLight* pLight = m_pScene->GetLightByName(strName);
	return (pLight != NULL);
}

