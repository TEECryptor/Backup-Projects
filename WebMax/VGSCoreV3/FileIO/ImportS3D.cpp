/////////////////////////////////////////////////////////////////////////
// 名称: CImportS3D--S3D[Version 3.0]S3D数据导入类， OGRE版本
// 描述: 加载3.0版本的S3D文件数据（从CGetS3DFileInfo_V3获得）到场景
// CGetS3DFileInfo_V3负责从S3D文件读取数据，而CImportS3D负责将其读取的数据加载到场景
// 作者: lixin	
// 日期: 2007-11-27
////////////////////////////////////////////////////////////////////////

#include "ImportS3D.h"
#include "..\VGSSupport.h"
#include "..\Root.h"
#include "..\Animation\Skeleton.h"


CImportS3D::CImportS3D(void)
{
	mS3DFileInfo = NULL;

	mPreMtrlCnt = 0;
	mPreTexCnt = 0;
	mPreCameraCnt = 0;
	mPreLightCnt = 0;
	mPreSkeletonCnt = 0;
	// mPreMeshCnt = 0;
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

	SetRoot(pVGS_SceneMgr->GetRoot());

	//VGS场景管理器
	m_pScene = pVGS_SceneMgr;
	mS3DFileInfo = pS3DFileInfo;

	if (!mS3DFileInfo || !m_pScene) 
	{
		return FALSE;
	}

	//场景中已有的材质数量
	mPreMtrlCnt = m_pScene->getMaterialCount(); // GetSceneMtrlCount();
	//计算场景中已经存在的相机数量
	mPreCameraCnt = m_pScene->getCameraCount(); // GetSceneCameraCount();
	//计算场景中已经存在的灯光数量
	mPreLightCnt = m_pScene->getLightCount();
	//计算场景中已经存在的骨骼数量
	mPreSkeletonCnt = 0; // GetSceneSkeletonCount();
	//计算场景中已经存在的Mesh数量
//	mPreMeshCnt = GetSceneMeshCount();
	m_preModelCnt = m_pScene->GetModelCount();
	//计算场景中已有的Entity数量
	// mPreEntityCnt = GetSceneEntityCount();

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
		MessageBox(NULL, TEXT("导入材质数据出错！"), TEXT("错误"), 0);
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
		MessageBox(NULL, TEXT("导入相机数据出错！"), TEXT("错误"), 0);
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
		MessageBox(NULL, TEXT("导入灯光数据出错！"), TEXT("错误"), 0);
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
		MessageBox(NULL, TEXT("导入模型数据出错！"), TEXT("错误"), 0);
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
		MessageBox(NULL, TEXT("导入图形数据出错！"), TEXT("错误"), 0);
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
		MessageBox(NULL, TEXT("导入场景数据出错！"), TEXT("错误"), 0);
		return FALSE;
	}

	try
	{
		//加载骨骼数据
		LoadSkeletonAnimationData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // 开始进度
	}
	catch (...)
	{
		MessageBox(NULL, TEXT("导入骨骼数据出错！"), TEXT("错误"), 0);
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
		pMtrl->setOpacity((int)pVGSMtrl->opacity);
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
		pMtrl->selfIllumination = (UINT)pVGSMtrl->selfIllumination;
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
		pMtrl->m_FVF = FVF_POSNORMALCOLORCOORD4;
		pMtrl->m_FVFSize = sizeof(VERTEXDECL_POSNORMALCOLORCOORD4);

		// 是否diffuse带透明通道
		BOOL bIsAlphaDiffuse = FALSE;

		// bump放在第一层
		if (pVGSMtrl->texture.bumpTexture_strLen > 0) 
		{	
			std::string sFileName = mS3DFileInfo->mStrPurePath;
			sFileName.append(string("\\texture\\"));
			sFileName.append(string(pVGSMtrl->texture.bumpTexture.c_str()));
			std::string sName = _GetPureFileNameStr(sFileName);
			CTextureLayer *pLayer = CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.bumpTexture_uvIndex, 0);
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
			CTextureLayer *pLayer = CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.diffuseTexture_uvIndex);
			if (pLayer)
			{
				pMtrl->m_vpTexLayers.push_back(pLayer);
				pMtrl->m_bUseDiffuse = false;

				//如果是透明通道的图像
				if (_IsTransparentFormat(((CTexture*)pLayer->pTexture)->m_format))
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
				CTextureLayer *pLayer = CreateTextureLayerFromFile(m_pScene, sName, sFileName, TRUE, pVGSMtrl->texture.opacityTexture_uvIndex);
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
			CTextureLayer *pLayer = CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.selfIlluminationTexture_uvIndex);
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
			CTextureLayer *pLayer = CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.displacementTexture_uvIndex);
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
			CTextureLayer *pLayer = CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.reflectionTexture_uvIndex);
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
	// mPreCameraCnt = m_pScene->getCameraCount();  // 场景中已有的相机数目

	m_vCamS3DTransform.clear();

	//逐个相机创建
	int cameraCnt = mS3DFileInfo->cameraManager.cameraCnt;
	for (int i = 0; i< cameraCnt; i++)
	{
		VGS_Camera* pVGSCamera = &(mS3DFileInfo->cameraManager.camera[i]);

		//相机名称
		std::string RealName = std::string(pVGSCamera->name);
		RealName = m_pScene->RenameCameraIfExist(RealName);

		//创建相机
		CCamera* pCamera = m_pScene->CreateCamera(RealName);

		//是否是轴侧
		if (pVGSCamera->isOrtho)
			pCamera->m_camType = VCT_ORTHO;
		else
			pCamera->m_camType = VCT_PROJECT;

		//相机的From点
		D3DXVECTOR3 from;
		from.x = pVGSCamera->camInfo[0].from.x;
		from.y = pVGSCamera->camInfo[0].from.z;
		from.z = pVGSCamera->camInfo[0].from.y;
		//pCamera->SetPosition(&from, VTS_WORLD);
		pCamera->setOriginPosition(from);

		//相机的at点
		D3DXVECTOR3 at;
		at.x = pVGSCamera->camInfo[0].at.x;
		at.y = pVGSCamera->camInfo[0].at.z;
		at.z = pVGSCamera->camInfo[0].at.y;
		//pCamera->Lookat(&at, VTS_WORLD);
		pCamera->setOriginLookatPos(at);

		//相机的Roll
		//pCamera->SetRoll(pVGSCamera->camInfo[0].roll);
		pCamera->setOriginRoll(pVGSCamera->camInfo[0].roll);

		CCameraS3DTransform s3dTrans;
		s3dTrans.v3Pos = from;
		s3dTrans.v3At  = at;
		s3dTrans.roll  = -pVGSCamera->camInfo[0].roll;  // 右手系转左手系，roll相反
		s3dTrans.sName = pCamera->getName();

		m_vCamS3DTransform.push_back(s3dTrans);  // 保存相机参数

		//Fov//实际上max中默认的是水平方向的fov，而d3d使用的是竖直方向的fov,这里可能需要转化一下
		double tan_half_fovy = 0.75f * tan(pVGSCamera->camInfo[0].fov/2.0f);
		pCamera->SetFov(2.0f * (float)atan(tan_half_fovy));

		//near far clip
		pCamera->setNearClip(g_CamDefaultNearClip);
		pCamera->setFarClip(g_CamDefaultfarClip);

		//创建相机的参数动画
		// CreateCameraParamAnimation(pCamera, pVGSCamera);

	}
	return 1;
}

//加载灯光数据
int	CImportS3D::LoadLightData()
{
	// mPreLightCnt = m_pScene->getLightCount();  // 场景中已有的灯光数目

	//逐个灯光创建
	int lightCnt = mS3DFileInfo->lightManager.lightCnt;
	for (int i = 0; i < lightCnt; i++)
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
void CImportS3D::ReadNodeAnimation( VGS_Node* pVGSNode,FILE* pStream )
{
	VGS_Node node;
	fread(&node.type,1,sizeof(node.type),pStream);
	fread(&node.index,1,sizeof(node.index),pStream);
	
	int strLen = 0;
	fread(&strLen,1,4,pStream);

	char str[MAX_PATH];
	fread(str,1,strLen,pStream);

	int frameCnt = 0;
	fread(&frameCnt,1,4,pStream);

	node.name = str;

	for( int i = 0;i<frameCnt;i++ )
	{
		NodeTransform xfm;
		fread(&xfm,1,sizeof(xfm),pStream);
		node.keyframeTransform.push_back((xfm));
	}

	int nId = pVGSNode->childNode.size();
	pVGSNode->childNode.push_back(node);

	int nChilds = 0;
	fread(&nChilds,1,4,pStream);

	for( i = 0;i<nChilds;i++ )
	{
		ReadNodeAnimation( &pVGSNode->childNode[nId],pStream );
	}
}

//加载骨骼数据
int	CImportS3D::LoadSkeletonAnimationData()
{
 	int skeletonAnimationCnt = mS3DFileInfo->skeletonAnimationManager.skeletonAnimationCnt;
	
	for (int i = 0; i< skeletonAnimationCnt; i++)
	{
		VGS_SkeletonAnimation skeletonAnimation = mS3DFileInfo->skeletonAnimationManager.skeletonAnimation[i];

	}
	return 1;
}


//加载Mesh模型数据
int	CImportS3D::LoadMeshData()
{
	// mPreMeshCnt = m_pScene->GetModelCount();

	//逐个加载Mesh的数据
	int meshCnt = mS3DFileInfo->m_vMeshData.size();

	for (int i = 0; i < meshCnt; i++)
	{
		CVGSMesh* pVGSMesh = &(mS3DFileInfo->m_vMeshData[i]);

		//模型名
		CModel* pModel = new CModel(m_pScene->m_pd3dDevice);
		pModel->Create(pVGSMesh, m_vLoadedShader);

		//模型名
		std::string RealName = m_pScene->RenameModelIfExist(pVGSMesh->sName);
		pModel->setName(RealName);

		//放入场景的模型列表
		// m_pScene->model.push_back(pModel);  // 保存到模型列表
		m_pScene->AddModel(pModel);
	} 

	return 1;
}

//加载Shape数据
int	CImportS3D::LoadShapeData()
{
	return 1;
}

//获得场景的子节点
void CImportS3D::GetSceneNodePtrList( vector<CSceneNode*>& vNodeList, CSceneNode* pRoot )
{
	vNodeList.push_back( pRoot );

	int c_childs = pRoot->m_child.size();
	for( int i = 0;i<c_childs;i++ )
	{
		GetSceneNodePtrList( vNodeList,pRoot->m_child[i] );
	}
}

//加载场景图
int	CImportS3D::LoadSceneGraphData()
{
	int nodeCnt = mS3DFileInfo->sceneNodeGraph.nodeCnt;

	VGS_Node* pVGSNode = &(mS3DFileInfo->sceneNodeGraph.node);
	
	//创建场景节点
	CSceneNode* pRootNode = m_pScene->GetRootSceneNode();
	CreateSceneNode(pRootNode, pVGSNode);

	vector<CSceneNode*>nodevec ;
	GetSceneNodePtrList(nodevec,pRootNode);

	return 1;
}

//创建场景图
void CImportS3D::CreateSceneNode(CSceneNode* pNode, VGS_Node* pVGSNode)
{	
	CSceneNode* pChildNode = NULL;
	std::string RealName = "";

	bool	bHasSkeleton = pVGSNode->bHasSkeleton; 

	CLight*  pLight = NULL;
	CModel*  pModel = NULL;
	CCamera* pCamera = NULL;

	//渲染同步对象
	//EnterCriticalSection(m_render_cs);

	//Node的类型
	NodeType type = pVGSNode->type;

	//找到对应编号的对象
	switch (type)
	{
	//ogre已经创建场景根节点
	case NNT_SCENEROOT:
		pChildNode = pNode;
		break;

	case NNT_CAMERANODE:
		{
			// RealName = std::string(mS3DFileInfo->cameraManager.camera[pVGSNode->index].name.c_str());
			pChildNode = pNode->CreateChild();
			//pChildNode->attachObject(NNT_CAMERANODE, (DWORD)m_pScene->GetCameraByName(RealName));
			
			pCamera = m_pScene->GetCameraByIndex(pVGSNode->index + mPreCameraCnt);
			pChildNode->attachObject(pCamera);
			// pCamera->m_pNode = pChildNode;
			//pCamera->m_pNode->updateTransform();
		}
		break; 

	case NNT_LIGHTNODE:
		{
			// RealName = std::string(mS3DFileInfo->lightManager.light[pVGSNode->index].name.c_str());
			pChildNode = pNode->CreateChild();
			
			pLight = m_pScene->GetLightByIndex(pVGSNode->index + mPreLightCnt);
			pChildNode->attachObject(pLight);
			//pLight->m_pNode = pChildNode;
			//pLight->pNode->updateTransform();
			
		}
		break;

	case NNT_MODEL:
		{
			int count = mS3DFileInfo->m_vMeshData.size();
			int index = pVGSNode->index;
			// RealName = std::string(mS3DFileInfo->meshManager.mesh[index].name.c_str());
			pChildNode = pNode->CreateChild();

			pModel = m_pScene->GetModelByIndex(pVGSNode->index + m_preModelCnt);
			pChildNode->attachObject(pModel);

			/*if (pModel)
			{
				pModel->m_pNode = pChildNode;
			}*/

		}
		break;

	case NNT_NONE:
		pChildNode->name = pVGSNode->name.c_str();	
		return;
		
	}
	
	pChildNode->name = pVGSNode->name.c_str();	
	
	//渲染同步对象
	//LeaveCriticalSection(m_render_cs);

	//Node的变换, 相机的变换不在这里处理---------------------------------------------------------
	//位置//转化坐标系
	if (type == NNT_SCENEROOT)
	{
		pChildNode->setPosition(&(D3DXVECTOR3(0, 0, 0)), VTS_WORLD);
		//pChildNode->updateTransform();
	}
	else
	{		
		//缩放//转化坐标系
		D3DXVECTOR3 scale;
		scale.x = pVGSNode->keyframeTransform[0].scale.x;
		scale.y = pVGSNode->keyframeTransform[0].scale.z;
		scale.z = pVGSNode->keyframeTransform[0].scale.y;
		//pChildNode->setScale(&scale, VTS_PARENT);
		
		//旋转//转化坐标系
		D3DXQUATERNION quat; 
		quat.x = pVGSNode->keyframeTransform[0].quat.x;
		quat.y = pVGSNode->keyframeTransform[0].quat.z;
		quat.z = pVGSNode->keyframeTransform[0].quat.y;
		quat.w = pVGSNode->keyframeTransform[0].quat.w;
		//pChildNode->setRotationByQuat(&quat, VTS_PARENT);

		D3DXVECTOR3 pos;
		pos.x = pVGSNode->keyframeTransform[0].position.x;
		pos.y = pVGSNode->keyframeTransform[0].position.z;
		pos.z = pVGSNode->keyframeTransform[0].position.y;
		//pChildNode->setPosition(&pos, VTS_PARENT);

		D3DXMATRIX transMat;
		//获得变换矩阵
		D3DXMatrixTransformation(&transMat, &D3DXVECTOR3(0,0,0), 
								 &D3DXQUATERNION(0, 0, 0, 0), 
								 &scale, 
								 &D3DXVECTOR3(0,0,0), 
								 &quat, 
								 &pos);
		pChildNode->setMatrix(transMat, VTS_PARENT);

		if (type == NNT_CAMERANODE)
		{
			if (pCamera)
			{
				// pCamera->restore();  // 设置为origin参数
				CCameraS3DTransform s3dTrans = getCameraParamByName(pCamera->getName());
				pCamera->SetPosition(&s3dTrans.v3Pos, VTS_WORLD);
				pCamera->Lookat(&s3dTrans.v3At, VTS_WORLD);
				pCamera->SetRoll(s3dTrans.roll);

				//将S3D文件中相机动画数据，转化为Node的动画变换
				VGS_Camera* pVGSCamera = &(mS3DFileInfo->cameraManager.camera[pVGSNode->index]);
				for (int frameIndex = 0; frameIndex < (int)pVGSCamera->camInfo.size(); frameIndex++)
				{
					ConvCamTransformToNodeTransform(&(pVGSNode->keyframeTransform[frameIndex]), &(pVGSCamera->camInfo[frameIndex]));
				}
			}
		}		
		else
		{			
			if (type == NNT_MODEL && pModel)
			{				
				// 如果模型有缩放,将模型设置为自动计算发现，避免光照计算错误
				if (D3DXVec3Length(&(pChildNode->getScale(VTS_WORLD))) > 0) // (pChildNode->getDerivedScaled())
				{					
					pModel->isAutoNormalise = true;
				}	
				pModel->UpdateFromNode(); // 更新包围盒
				
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
	std::string strAnimName = m_pRoot->GetAnimManager().GetNodeAnimationName(pNode);

	CNodeAnimation *pAnim = new CNodeAnimation(pNode, strAnimName);  // 创建节点的关键帧动画

	if (!pAnim)
	{
		return;
	}

	VGS_Camera*      pVGSCamera = NULL;   // 相机的参数动画信息
	VGS_CAMERA_INFO* pCamInfo = NULL;

	VGS_Light*      pVGSLight = NULL;    // 灯光的参数动画信息
	VGS_LIGHT_INFO* pLightInfo = NULL;
	if (pNode->getMovableType() == VGS_MOVABLETYPE_CAMERA)
	{
		int index = ((CCamera*)pNode->getMovableObject())->index;
		if (index >= 0 && index < mS3DFileInfo->cameraManager.cameraCnt)
		{
			pVGSCamera = &(mS3DFileInfo->cameraManager.camera[index]);
		}
	}
	else if (pNode->getMovableType() == VGS_MOVABLETYPE_LIGHT)
	{
		int index = ((CLight*)pNode->getMovableObject())->index;
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
		if (pNode->getMovableType() == VGS_MOVABLETYPE_CAMERA)
		{			
			if (pVGSCamera && frameIndex >= 0 && frameIndex < (int)pVGSCamera->camInfo.size())
			{
				pCamInfo = &pVGSCamera->camInfo[frameIndex];
			}			
			pKeyFrame = CreateCameraKeyFrame(pNode, pTransform, pCamInfo);
		}
		else if (pNode->getMovableType() == VGS_MOVABLETYPE_LIGHT)
		{			
			if (pVGSLight && frameIndex >= 0 && frameIndex < (int)pVGSLight->lightInfo.size())
			{
				pLightInfo = &pVGSLight->lightInfo[frameIndex];
			}
			pKeyFrame = CreateLightKeyFrame(pNode, pTransform, pLightInfo);
		}
		else if (pNode->getMovableType() == VGS_MOVABLETYPE_MODEL)
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
	m_pRoot->GetAnimManager().AddNodeKeyfrmAnimation(pAnim);
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
		pCamKfr->setFov((float)fovy);
		pCamKfr->setRoll(pCamParam->roll);
		pCamKfr->setClip(10, 100000);
		pCamKfr->setFrom(VECTOR3(pCamParam->from.x, pCamParam->from.z, pCamParam->from.y));
		pCamKfr->setAt(VECTOR3(pCamParam->at.x, pCamParam->at.z, pCamParam->at.y));
		pCamKfr->m_bHasParamAnimation = true;
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
		pLightKfr->m_bHasParamAnimation = true;

		pLightKfr->setPosition(VECTOR3(pLightParam->pos.x, pLightParam->pos.y, pLightParam->pos.z));

		//颜色
		float r = ((pLightParam->color>>16) & 0xff) / 255.0f * pLightParam->multiply;
		float g = ((pLightParam->color>>8) & 0xff) / 255.0f * pLightParam->multiply;
		float b = ((pLightParam->color) & 0xff) / 255.0f * pLightParam->multiply;
		pLightKfr->setColor(r, g, b, 1.0f);
		pLightKfr->setRange(pLightParam->range2);		
		pLightKfr->setAttenuation();
		
		CLight *pLight = (CLight*)pNode->getMovableObject();
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

//
//
////计算场景中已有的材质数量
//int CImportS3D::GetSceneMtrlCount()
//{
//	return m_pScene->m_vShader.size();
//}
//
////计算场景中已有的相机数量
//int CImportS3D::GetSceneCameraCount()
//{
//	return m_pScene->m_vCamera.size();
//}
//
////计算场景中已有的灯光数量
//int CImportS3D::GetSceneLightCount()
//{
//	return m_pScene->m_vLight.size();
//}
//
////计算场景中已经有的骨骼数量
//int CImportS3D::GetSceneSkeletonCount()
//{
//	int count = 0;
//
//	return count;
//}
//
////计算场景中已经有的Mesh数量
//int CImportS3D::GetSceneMeshCount()
//{
//	int count = 0;
//	return count;
//}
//
////计算场景中已有的Entity数量
//int	CImportS3D::GetSceneEntityCount()
//{
//	return m_pScene->model.size();
//}

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

CCameraS3DTransform CImportS3D::getCameraParamByName(const std::string &sName)
{
	CCameraS3DTransform trans;
	UINT count = m_vCamS3DTransform.size();
	for (UINT i = 0; i < count; ++i)
	{
		if (m_vCamS3DTransform[i].sName == sName)
		{
			return m_vCamS3DTransform[i];
		}
	}
	return trans;
}


// 从图像文件创建一个texturelayer
CTextureLayer* CImportS3D::CreateTextureLayerFromFile(CScene *pScene, const std::string &sLayerName, const std::string &sTextureFileName, 
														   BOOL bAlphaOnly/* = FALSE*/, INT UVIndex/* = 0*/, INT mipMap/* = DEFAULT_MIPMAP*/)
{
	if (!_FileExist(sTextureFileName))
	{
		return 0;
	}

	CTexture *pTexture = m_pRoot->GetTextureManager().createTexture(sTextureFileName, bAlphaOnly, mipMap, "");
	// CTexture *pTexture = pScene->CreateTexFromFile(sTextureFileName.c_str(), sLayerName.c_str(), mipMap);
	if (pTexture)
	{
		CTextureLayer *pLayer = new CTextureLayer();
		if (pLayer)
		{
			// ZeroMemory(pLayer, sizeof(CTextureLayer));
			pLayer->m_sName = sLayerName;
			pLayer->pTexture = /*(DWORD)*/pTexture;
			pLayer->type = TLT_BASAL;
			pLayer->UVIndex = UVIndex;
			pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(VBO_MODULATE);
			pLayer->Blenderinfo.AlphaBlendParam = 0;
			pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(VBO_MODULATE);
			pLayer->Blenderinfo.ColorBlendParam = 0;
		}
		return pLayer;
	}
	return 0;
}
