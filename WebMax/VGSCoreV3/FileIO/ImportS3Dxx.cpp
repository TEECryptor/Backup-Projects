/////////////////////////////////////////////////////////////////////////
// ����: CImportS3D--S3D[Version 3.0]S3D���ݵ����࣬ OGRE�汾
// ����: ����3.0�汾��S3D�ļ����ݣ���CGetS3DFileInfo_V3��ã�������
// CGetS3DFileInfo_V3�����S3D�ļ���ȡ���ݣ���CImportS3D�������ȡ�����ݼ��ص�����
// ����: lixin	
// ����: 2007-11-27
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

	m_pScene = NULL;				//����������	
	mKeyFrameStartPlayTime = 0;		//�ؼ�֡������ʼ���ŵ�ʱ��

	m_render_cs = NULL;

	m_textureset.clear();
}

CImportS3D::~CImportS3D(void)
{
}

//����CGetS3DFileInfo_V3���󵽳���
BOOL CImportS3D::LoadS3DFileInfo(CGetS3DFileInfo* pS3DFileInfo, CScene* pVGS_SceneMgr, HWND hWnd)
{
	if (!pVGS_SceneMgr)
	{
		return FALSE;
	}

	m_hWnd = hWnd;

	//VGS����������
	m_pScene = pVGS_SceneMgr;
	mS3DFileInfo = pS3DFileInfo;

	if (!mS3DFileInfo || !m_pScene) 
	{
		return FALSE;
	}

	//���������еĲ�������
	mPreMtrlCnt = GetSceneMtrlCount();
	//���㳡�����Ѿ����ڵ��������
	mPreCameraCnt = GetSceneCameraCount();
	//���㳡�����Ѿ����ڵĵƹ�����
	mPreLightCnt = GetSceneLightCount();
	//���㳡�����Ѿ����ڵĹ�������
	mPreSkeletonCnt = GetSceneSkeletonCount();
	//���㳡�����Ѿ����ڵ�Mesh����
	mPreMeshCnt = GetSceneMeshCount();
	//���㳡�������е�Entity����
	mPreEntityCnt = GetSceneEntityCount();

	// ���������ֽ����͵�ǰ�ֽ���
	DWORD totalSize = 2 * mS3DFileInfo->m_totalSize;
	DWORD curSize   = mS3DFileInfo->m_totalSize;
	DWORD oneStepSize = curSize / 7;
	try
	{
		//���ز�������
		LoadMtrlData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // ��ʼ����
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("����������ݳ���"), LPCSTR("����"), 0);
		return FALSE;
	}

	try
	{
		//�����������
		LoadCameraData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // ��ʼ����
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("����������ݳ���"), LPCSTR("����"), 0);
		return FALSE;
	}

	try
	{
		//���صƹ�����
		LoadLightData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // ��ʼ����
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("����ƹ����ݳ���"), LPCSTR("����"), 0);
		return FALSE;
	}

	try
	{
		//���ع�������
		LoadSkeletonData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // ��ʼ����
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("����������ݳ���"), LPCSTR("����"), 0);
		return FALSE;
	}

	try
	{
		//����Mesh����
		LoadMeshData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // ��ʼ����
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("����ģ�����ݳ���"), LPCSTR("����"), 0);
		return FALSE;
	}
	
	try
	{
		//����Shape����
		LoadShapeData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // ��ʼ����
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("����ͼ�����ݳ���"), LPCSTR("����"), 0);
		return FALSE;
	}
	try
	{
		//���س���ͼ����
		LoadSceneGraphData();
		curSize = totalSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // ��ʼ����
	}
	catch (...)
	{
		MessageBox(NULL, LPCSTR("���볡�����ݳ���"), LPCSTR("����"), 0);
		return FALSE;
	}

	// return std::string(mResourceGroupName.c_str());
	return TRUE;
}

//���ز�������
int	CImportS3D::LoadMtrlData()
{
	//�����������
	int mtrlCnt = mS3DFileInfo->mtrlManager.mtrlCnt;
	for (int i = 0; i< mtrlCnt; i++)
	{
		VGS_Mtrl* pVGSMtrl = &(mS3DFileInfo->mtrlManager.mtrl[i]);

		//������
		std::string RealName = std::string(pVGSMtrl->name.c_str());
		RealName = m_pScene->RenameMaterialIfExist(RealName); // _RenameMaterialIfExist(RealName);

		//����������Դ��ָ����Դ��
		CMaterial *pMtrl = m_pScene->CreateMaterial(RealName);

		if (!pMtrl)
		{
			continue;
		}		

		//͸���� 
		pMtrl->setOpacity(pVGSMtrl->opacity);
		float opacity = pVGSMtrl->opacity / 100.0f;
       
		//���ò��ʵ�Diffse���� - ��ɫ
		pMtrl->d3dmtrl.Diffuse.r = (((pVGSMtrl->diffuse) >> 16) & 0xff) / 255.0f;
		pMtrl->d3dmtrl.Diffuse.g = (((pVGSMtrl->diffuse) >> 8) & 0xff) / 255.0f;
		pMtrl->d3dmtrl.Diffuse.b = (((pVGSMtrl->diffuse)) & 0xff) / 255.0f;
		pMtrl->d3dmtrl.Diffuse.a = opacity;

		
		//���ò��ʵ�Ambient���� - ����ɫ������Ϊ0�����ɣ�ģ����������
		pMtrl->d3dmtrl.Ambient.r = 0;
		pMtrl->d3dmtrl.Ambient.g = 0;
		pMtrl->d3dmtrl.Ambient.b = 0;
		pMtrl->d3dmtrl.Ambient.a = opacity;
		
		//���ò��ʵ�Specular���� - �߹�ɫ
		pMtrl->specular.r = (((pVGSMtrl->specular)>>16) & 0xff)/255.0f;
		pMtrl->specular.g = (((pVGSMtrl->specular)>>8) & 0xff)/255.0f;
		pMtrl->specular.b = (((pVGSMtrl->specular)) & 0xff)/255.0f;
		pMtrl->specular.a = opacity;
		pMtrl->specularLevel = pVGSMtrl->specularLevel;

		pMtrl->d3dmtrl.Specular.r = pMtrl->specular.r * (pVGSMtrl->specularLevel / 100.0f);
		pMtrl->d3dmtrl.Specular.g = pMtrl->specular.g * (pVGSMtrl->specularLevel / 100.0f);
		pMtrl->d3dmtrl.Specular.b = pMtrl->specular.b * (pVGSMtrl->specularLevel / 100.0f);
		pMtrl->d3dmtrl.Specular.a = opacity;
		pMtrl->d3dmtrl.Power      = pVGSMtrl->shininess;  // �߹����
		
		//�Է�������
		pMtrl->selfIllumination = pVGSMtrl->selfIllumination;
		if (pMtrl->selfIllumination >= 100) 
		{
			pMtrl->selfIllumination = 99; // ��Ϊ100�Է���ʱ����������߹⣬���������ȫ�Է��⣬�����Ϊ99��
		}

		pMtrl->d3dmtrl.Emissive.r = pMtrl->d3dmtrl.Diffuse.r * pMtrl->selfIllumination / 100.0f;
		pMtrl->d3dmtrl.Emissive.g = pMtrl->d3dmtrl.Diffuse.g * pMtrl->selfIllumination / 100.0f;
		pMtrl->d3dmtrl.Emissive.b = pMtrl->d3dmtrl.Diffuse.b * pMtrl->selfIllumination / 100.0f;
		pMtrl->d3dmtrl.Emissive.a = opacity;

		//Shader����//��������Ż�,ֻ�й��ղ��ʺ��ж���ɫ��Ҫ��ɫ�Ĳ���Ҫ��Gouraudģ��
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

		//˫����Ⱦ����
		if (pVGSMtrl->bTwoSide) 
			pMtrl->twoSide = TRUE;
		else
			pMtrl->twoSide = FALSE;

		
		// ȱʡ��ʹ��alphatest
		pMtrl->bAlphaTest = FALSE;
		pMtrl->AlpahaTestValue = 64;

		//͸��������, ģ�ͱ����͸��
		if(opacity < 1.0f)
		{
			pMtrl->bAlphaBlend = TRUE;
		}
		else
		{
			//ģ�Ͳ�͸��������͸����ͼ
			if (pVGSMtrl->texture.opacityTexture_strLen)
			{
				pMtrl->bAlphaBlend = TRUE;
			}
			else//ģ�Ͳ�͸����Ҳû��͸����ͼ
			{
				pMtrl->bAlphaBlend = FALSE;
			}
		}

		// ��������λ�á����ߡ���ɫ��4��UV
		pMtrl->m_FVF = FVF_T7;
		pMtrl->m_FVFSize = sizeof(T7MESHVERTEX);

		// �Ƿ�diffuse��͸��ͨ��
		BOOL bIsAlphaDiffuse = FALSE;

		// bump���ڵ�һ��
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
		
		//�����Ӧ��ŵ���ͼ��û�д�������Ϊ���ʴ����ñ�ŵ���ͼ
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

				//�����͸��ͨ����ͼ��
				if (_IsTransparentFormat(((CTexture*)pLayer->pTexture)->BP))
				{
					pMtrl->bAlphaTest = true;
					pMtrl->AlpahaTestValue = DEFAULT_ALPHATESTVALUE; // 64
					//pMtrl->bAlphaBlend = true;
					bIsAlphaDiffuse = TRUE;
				}
			}
		}	// if diffuse				

		// ���diffuse��͸��ͨ��������opacityͨ����ͼ��diffuse��ͬ���Ͷ���opactity�㡣���ǿ���ֱ��͸����
		// ͨ������3ds max�У�������ϰ����������ͬ��tgaͼƬ����͸��Ч��		
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
					pLayer->Blenderinfo.ColorBlendOption = D3DTOP_ADD; //D3DTOP_DISABLE;  // ͸����ͼû����ɫ�����Ա����Ǽӻ�����Ч
					pLayer->Blenderinfo.AlphaBlendOption = D3DTOP_MODULATE;
 					pMtrl->bAlphaTest = true;
 					pMtrl->AlpahaTestValue = DEFAULT_ALPHATESTVALUE; // 64
 					//pMtrl->bAlphaBlend = true;
					pMtrl->m_vpTexLayers.push_back(pLayer);
					pMtrl->m_bUseDiffuse = false;
				}								
			} // if bIsAlphaDiffuse
		} // if opacity			

		//��������ͼ
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

				pMtrl->selfIllumination = 99; // ���˹���ͼ���Է�����Ϊ99��
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
		
		//����������ͼ
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

		// ���������diffuse�����ò�����ɫ
		if (!pMtrl->m_bUseDiffuse)
		{
			pMtrl->d3dmtrl.Diffuse.r = 1.0f;
			pMtrl->d3dmtrl.Diffuse.g = 1.0f;
			pMtrl->d3dmtrl.Diffuse.b = 1.0f;
			pMtrl->d3dmtrl.Diffuse.a = 1.0f;
		}
		//��pMtrl���볡��material�б�
		m_pScene->m_vShader.push_back(pMtrl);

		// ���μ��صĲ���
		m_vLoadedShader.push_back(pMtrl);

	} // for material

	return 1;
}

//�����������
int	CImportS3D::LoadCameraData()
{
	//����������
	int cameraCnt = mS3DFileInfo->cameraManager.cameraCnt;
	for (int i = 0; i< cameraCnt; i++)
	{
		VGS_Camera* pVGSCamera = &(mS3DFileInfo->cameraManager.camera[i]);

		//�������
		std::string RealName = std::string(pVGSCamera->name.c_str());
		RealName = m_pScene->RenameCameraIfExist(RealName);

		//�������
		CCamera* pCamera = m_pScene->CreateCamera(RealName);

		//�Ƿ������
		if (pVGSCamera->isOrtho)
			pCamera->m_Type = VCT_ORTHO;
		else
			pCamera->m_Type = VCT_PROJECT;

		//�����From��
		D3DXVECTOR3 from;
		from.x = pVGSCamera->camInfo[0].from.x;
		from.y = pVGSCamera->camInfo[0].from.z;
		from.z = pVGSCamera->camInfo[0].from.y;
		pCamera->SetFrom(&from);

		//�����at��
		D3DXVECTOR3 at;
		at.x = pVGSCamera->camInfo[0].at.x;
		at.y = pVGSCamera->camInfo[0].at.z;
		at.z = pVGSCamera->camInfo[0].at.y;
		pCamera->SetAt(&at);

		//�����Roll
		pCamera->SetRoll(pVGSCamera->camInfo[0].roll);

		//Fov//ʵ����max��Ĭ�ϵ���ˮƽ�����fov����d3dʹ�õ�����ֱ�����fov,���������Ҫת��һ��
		double tan_half_fovy = 0.75 * tan(pVGSCamera->camInfo[0].fov/2.0f);
		pCamera->m_fov = 2 * atan(tan_half_fovy);

		//near far clip
		pCamera->nearClip = g_CamDefaultNearClip;
		pCamera->farClip  = g_CamDefaultfarClip;

		//��������Ĳ�������
		// CreateCameraParamAnimation(pCamera, pVGSCamera);

	}
	return 1;
}

//���صƹ�����
int	CImportS3D::LoadLightData()
{
	//����ƹⴴ��
	int lightCnt = mS3DFileInfo->lightManager.lightCnt;
	for (int i = 0; i< lightCnt; i++)
	{
		VGS_Light* pVGSLight = &(mS3DFileInfo->lightManager.light[i]);
		
		//�ƹ�����
		std::string RealName =std::string(pVGSLight->name.c_str());
		std::string RenameLog;
		BOOL bRename = RenameLightIfExist(RealName, RenameLog);
		if (bRename) //����Ĺ�����
		{
			pVGSLight->name = std::string(RealName);
		}

		//��Ⱦͬ������
		//EnterCriticalSection(m_render_cs);

		//�����ƹ�
		CLight* pLight = m_pScene->CreateLight(RealName);

		//��ɫ
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

		//�����⣬����������ģ��ȫ�ֹ�
		pLight->ambient   = pLight->diffuse;
		pLight->ambient.a = 1.0f;
		pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb �Ѳ���0,05��Ϊ��0.0f
		pLight->param.Ambient.a = 1.0f;

		//�߹�
		pLight->param.Specular   = pLight->multiply * pLight->diffuse;  // fuxb 2007-10-16 ��3ds max������ͬ�Ĵ���ʽ
		pLight->param.Specular.a = 1.0f;

		//Ĭ�ϵƹⷶΧ
		if (pVGSLight->lightInfo[0].range2 == -1) 
		{
			pVGSLight->lightInfo[0].range2 = (float)MAXDWORD;
		}

		//���ߵ������Զ����
		pLight->param.Range        = pVGSLight->lightInfo[0].range2;
		pLight->param.Attenuation0 = 1.0f;
		pLight->param.Attenuation1 = 0.0f;
		pLight->param.Attenuation2 = 0.0f;

		pLight->param.Type= D3DLIGHT_POINT;
		D3DXVECTOR3 vPos(pVGSLight->lightInfo[0].pos.x, pVGSLight->lightInfo[0].pos.z, pVGSLight->lightInfo[0].pos.y);  // z �� y����
		pLight->setPosition(&vPos);		

		//�ƹ�����,��ͬ�ĵƹ����ͱ����趨��ͬ�Ĳ���--------------------------		
		if (pVGSLight->type == OMINI)    //omni light
		{	
			pLight->param.Type= D3DLIGHT_POINT;
		}
		else if (pVGSLight->type == SPOTLIGHT)  //spot light
		{
			pLight->param.Type= D3DLIGHT_SPOT;
			
            //��������ķ�Χ��
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
	
			//����Spot����
			pLight->param.Falloff = 1.0f;

			D3DXVECTOR3 vDir(pVGSLight->lightInfo[0].dir.x, pVGSLight->lightInfo[0].dir.z, pVGSLight->lightInfo[0].dir.y);  // z �� y����
			pLight->setDirection(&vDir);
		}

		//��Ⱦͬ������
		//LeaveCriticalSection(m_render_cs);

		//�ƹ��������-----------------------------------------------------------------
		// CreateLightParamAnimation(pLight, pVGSLight);

	} // for each light

	return 1;
}

//���ع�������
int	CImportS3D::LoadSkeletonData()
{
	/*//�����������
	int skeletonCnt = mS3DFileInfo->skeletonManager.skeletonCnt;
	for (int i = 0; i< skeletonCnt; i++)
	{
		VGS_Skeleton* pVGSSkeleton = &(mS3DFileInfo->skeletonManager.skeleton[i]);

		//��������
		std::string RealName = std::string(pVGSSkeleton->name.c_str());

		if (Ogre::SkeletonManager::getSingleton().getByName(RealName).get() != NULL)
		{
			RealName = RealName + std::string("_ReN") + Ogre::StringConverter::toString(gRenamedSkeletonCnt);
			gRenamedSkeletonCnt++;
			pVGSSkeleton->name = std::string(RealName);
		}
		
		//��Ⱦͬ������
		//EnterCriticalSection(m_render_cs);
		//��������
		Ogre::SkeletonPtr pSkeleton = Ogre::SkeletonManager::getSingleton().create(RealName, mResourceGroupName);
		//��Ⱦͬ������
		//LeaveCriticalSection(m_render_cs);

		//��������������ָ�����������ĳ���
		int animationCnt = pVGSSkeleton->animationCnt;
		for (int j = 0; j< animationCnt; j++)
		{
			VGS_SkeletonAnimation* pAnimation = pVGSSkeleton->animationList[j];

			std::string AnimationName = std::string(pAnimation->name.c_str());
			float animationLen = mKeyFrameStartPlayTime + 1.0f * pAnimation->frameCnt / gkfrPlaySpeed;
			Animation* pSkeletonAni = pSkeleton->createAnimation(AnimationName, animationLen);
			
			// ���ùؼ�֮֡��Ĳ�ֵ����Ϊ������ֵ
			pSkeletonAni->setDefaultInterpolationMode(Animation::IM_LINEAR);
			
			//�ݹ鴴�������еĹ�ͷ, ��ͷֻ�ܴ���һ��
			if (j == 0)
			{
				int boneIndex = 0;
				//��Ⱦͬ������
				//EnterCriticalSection(m_render_cs);
				CreateBone(pSkeleton, pSkeletonAni, &(pAnimation->bone), boneIndex); 
				//���������
				pSkeleton->setBindingPose();
				//��Ⱦͬ������
				//LeaveCriticalSection(m_render_cs);
			}
			else //�����еĹ�ͷ�����Ӷ���
			{
				//�����µĹ�ͷ����
				int boneIndex = 0;
				CreateBoneAnimation(pSkeleton, pSkeletonAni, &(pAnimation->bone), boneIndex);
			}
		}
		
		//��Ⱦͬ������
		//EnterCriticalSection(m_render_cs);
		//�Ż����еĶ���
		pSkeleton->optimiseAllAnimations();
		//��Ⱦͬ������
		//LeaveCriticalSection(m_render_cs);
	}
*/
	return 1;
}

//�ݹ鴴����ͷ
/*Bone* CImportS3D::CreateBone(Ogre::SkeletonPtr pSkeleton, Animation* pSkeletonAni, VGS_Bone* pVGSBone, int& boneIndex)
{
	//������ͷ
	Bone* pBone = pSkeleton->createBone(std::string(pVGSBone->name.c_str()));
	
	//������ͷ����̬--------------------------------------------------------------
	//λ��//ת������ϵ
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

	//����//ת������ϵ
	Vector3 scale;
	scale.x = pVGSBone->pose.scale.x;
	scale.y = pVGSBone->pose.scale.z;
	scale.z = pVGSBone->pose.scale.y;
	pBone->setScale(scale);

	//������ͷ�Ķ���-------------------------------------------------------------------
	//����֡��
	int frameCnt = pVGSBone->transform.size();
	if (frameCnt > 1)
	{
		NodeAnimationTrack* pNodeAniTrack = pSkeletonAni->createNodeTrack(boneIndex, pBone);
		for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
		{
			TransformKeyFrame* pTransKfr =  pNodeAniTrack->createNodeKeyFrame(mKeyFrameStartPlayTime + (1.0f * frameIndex) / gkfrPlaySpeed);
					
			//��ͷ�ؼ�֡������Ϣ��
			//1. setTranslate: �ڸ�������ϵ�У��ù�ͷ��ǰ֡��Գ�ʼ��̬��λ��
			//2. setRotation: �ڸ�������ϵ�У��ù�ͷ��ǰ֡��Գ�ʼ��̬��ת��
			//3. setScale: �ù�ͷ��ǰ֡��Գ�ʼ��̬������
			
			//λ�ñ任
			Vector3 pos;
			pos.x = pVGSBone->transform[frameIndex].position.x;
			pos.y = pVGSBone->transform[frameIndex].position.z;
			pos.z = -pVGSBone->transform[frameIndex].position.y;
			pTransKfr->setTranslate(pos);

			//ת�Ǳ任
			Quaternion quat;
			quat.x = -pVGSBone->transform[frameIndex].quat.x;
			quat.y = -pVGSBone->transform[frameIndex].quat.z;
			quat.z = pVGSBone->transform[frameIndex].quat.y;
			quat.w = pVGSBone->transform[frameIndex].quat.w;
			pTransKfr->setRotation(quat);

			//���ű任 
			Vector3 scale;
			scale.x = pVGSBone->transform[frameIndex].scale.x;
			scale.y = pVGSBone->transform[frameIndex].scale.z;
			scale.z = pVGSBone->transform[frameIndex].scale.y;
			pTransKfr->setScale(scale);	
		}
	}
	boneIndex++;

	//�����ӹ�ͷ
	int childCnt = pVGSBone->childBone.size();
	for (int i = 0; i< childCnt; i++)
	{
		VGS_Bone* pChildVGSBone = &(pVGSBone->childBone[i]);
		Bone* pChildBone = CreateBone(pSkeleton, pSkeletonAni, pChildVGSBone, boneIndex);
		pBone->addChild(pChildBone);
	}

	return pBone;
}*/

//�ݹ鴴����ͷ����
/*Bone* CImportS3D::CreateBoneAnimation(Ogre::SkeletonPtr pSkeleton, Animation* pSkeletonAni, VGS_Bone* pVGSBone, int& boneIndex)
{
	//�ҵ���ͷ
	//Bone* pBone = pSkeleton->getBone(std::string(pVGSBone->name.c_str()));
	Bone* pBone = pSkeleton->getBone(boneIndex);
	
	//������ͷ�Ķ���-------------------------------------------------------------------
	//����֡��
	int frameCnt = pVGSBone->transform.size();
	if (frameCnt > 1)
	{
		NodeAnimationTrack* pNodeAniTrack = pSkeletonAni->createNodeTrack(boneIndex, pBone);
		for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
		{
			TransformKeyFrame* pTransKfr =  pNodeAniTrack->createNodeKeyFrame(1.0f * frameIndex / gkfrPlaySpeed);
					
			//��ͷ�ؼ�֡������Ϣ��
			//1. setTranslate: �ڸ�������ϵ�У��ù�ͷ��ǰ֡��Գ�ʼ��̬��λ��
			//2. setRotation: �ڸ�������ϵ�У��ù�ͷ��ǰ֡��Գ�ʼ��̬��ת��
			//3. setScale: �ù�ͷ��ǰ֡��Գ�ʼ��̬������
			
			//λ�ñ任
			Vector3 pos;
			pos.x = pVGSBone->transform[frameIndex].position.x;
			pos.y = pVGSBone->transform[frameIndex].position.z;
			pos.z = -pVGSBone->transform[frameIndex].position.y;

			//ת�Ǳ任
			Quaternion quat;
			quat.x = -pVGSBone->transform[frameIndex].quat.x;
			quat.y = -pVGSBone->transform[frameIndex].quat.z;
			quat.z = pVGSBone->transform[frameIndex].quat.y;
			quat.w = pVGSBone->transform[frameIndex].quat.w;

			//���ű任 
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

	//�����ӹ�ͷ����
	int childCnt = pVGSBone->childBone.size();
	for (int i = 0; i< childCnt; i++)
	{
		VGS_Bone* pChildVGSBone = &(pVGSBone->childBone[i]);
		Bone* pChildBone = CreateBoneAnimation(pSkeleton, pSkeletonAni, pChildVGSBone, boneIndex);
	}

	return pBone;
}
*/

//����Meshģ������
int	CImportS3D::LoadMeshData()
{
	int preModelCnt = m_pScene->GetModelCount();

	//�������Mesh������
	int meshCnt = mS3DFileInfo->meshManager.meshCnt;

	for (int i = 0; i < meshCnt; i++)
	{
		VGS_Mesh* pVGSMesh = &(mS3DFileInfo->meshManager.mesh[i]);

		//ģ����
		std::string RealName = m_pScene->RenameModelIfExist(pVGSMesh->name.c_str());
		pVGSMesh->name = std::string(RealName);

		//��Ⱦͬ������
		//EnterCriticalSection(m_render_cs);

		CModel* pModel = new CModel(m_pScene->m_pd3dDevice);

		if (!pModel)
		{
			continue;
		}

		//ģ�ͱ��
		pModel->index = preModelCnt + i;
		pModel->setName(RealName);
		
		//��������
		pModel->vertexCnt = pVGSMesh->vertexCnt;
		pModel->vertex = new CVertex[pModel->vertexCnt];
		CVertex *pVertex = pModel->vertex;
		for (UINT j = 0; j < pModel->vertexCnt; ++j)
		{
			VGS_Vertex* pVGSVertex = &(pVGSMesh->vertex[j]);

			pVertex[j].index = j;

			//���붥��λ��//�任����ϵ
			pVertex[j].position.x = pVGSVertex->position.x;
			pVertex[j].position.y = pVGSVertex->position.z;
			pVertex[j].position.z = pVGSVertex->position.y;
			
			//���붥�㷨��//�任����ϵ
			pVertex[j].normal.x = pVGSVertex->normal.x;
			pVertex[j].normal.y = pVGSVertex->normal.z;
			pVertex[j].normal.z = pVGSVertex->normal.y;

			//���붥�����ɫ
			pVertex[j].diffuse = (D3DCOLOR)pVGSVertex->color;

			//���붥���Diffuse ͨ��UV
			pVertex[j].uv1.u = pVGSVertex->uv1.u;
			pVertex[j].uv1.v = 1.0f - pVGSVertex->uv1.v;

			//���붥���LightMap ͨ��WZ
			pVertex[j].uv2.u = pVGSVertex->uv2.u;
			pVertex[j].uv2.v = 1.0f - pVGSVertex->uv2.v;

			//���붥���LightMap ͨ��WZ
			pVertex[j].uv3.u = pVGSVertex->uv3.u;
			pVertex[j].uv3.v = 1.0f - pVGSVertex->uv3.v;

			//���붥���LightMap ͨ��WZ
			pVertex[j].uv4.u = pVGSVertex->uv4.u;
			pVertex[j].uv4.v = 1.0f - pVGSVertex->uv4.v;

		} // for j


		int subMeshCnt = pVGSMesh->subMesh.size();
		for (int subMeshIndex = 0; subMeshIndex < subMeshCnt; ++subMeshIndex)
		{
			VGS_SubMesh* pVGS_SubMesh = &(pVGSMesh->subMesh[subMeshIndex]);

			//����SubMesh����
			_subMesh pSubMesh;

			pSubMesh.faceCnt = pVGS_SubMesh->face.size();
			pSubMesh.face = new CFace[pSubMesh.faceCnt];
			
			//���������������������
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
			
			//�ҵ���ǰsubMesh�Ĳ���
			int mtrlIndex = pVGS_SubMesh->mtrlIndex;
			if (mtrlIndex >= 0 && mtrlIndex < m_vLoadedShader.size())
			{
				pSubMesh.shader = m_vLoadedShader[mtrlIndex];
			}
			else  // ʹ��ȱʡ����
			{
				CMaterial *pMaterial = new CMaterial();
				pSubMesh.shader = pMaterial;
			}
			
			pModel->subMesh.push_back(pSubMesh);                // ���浽ģ��

			RenderSubMesh rsm;
			rsm.modelPointer = (DWORD)pModel;
			rsm.subMeshID = pModel->subMesh.size() - 1;
			pSubMesh.shader->subMeshList.push_back(rsm);   // ʹ�øò��ʵ���ģ���б�

		} // for subMeshIndex
		
		//ģ�͵İ�Χ��, ע��任����ϵ
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

		// ģ�ͱ�ѡ��ʱ��ʾ�İ�Χ��
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

		// ����ģ�ͻ���
		pModel->createVBuffer();
		pModel->createIBuffer();

		m_pScene->model.push_back(pModel);  // ���浽ģ���б�		

	} // for i

	return 1;
}

//����Entity�Ĺ���
/*bool CImportS3D::ShareEntitySkeleton(Entity* pEntity)
{
	//������S3D�Ѿ����ص�Entity����
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
//�Mesh�Ĺ���
/*void CImportS3D::BindMeshToSkeleton(MeshPtr pMesh, VGS_Mesh* pVGSMesh, SkeletonPtr pSkeleton)
{
	int boneAssCnt = pVGSMesh->bindingData.size();
	for (int i = 0; i< boneAssCnt; i++)
	{
		BoneAss* pBoneAss = &(pVGSMesh->bindingData[i]);
		//����Ȩ��
		Ogre::VertexBoneAssignment_s vBoneAss;
		vBoneAss.vertexIndex = pBoneAss->vertexIndex;
		vBoneAss.boneIndex = pBoneAss->bindingBoneIndex;
		vBoneAss.weight = pBoneAss->bindingWeight;
		pMesh->addBoneAssignment(vBoneAss);
	}

	//����ͷ�ı任
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
	
	//���������ṹ��Mesh֮�������
	pMesh->_notifySkeleton(pSkeleton);

	//���²���
	pMesh->updateMaterialForAllSubMeshes();
	
}*/

//����Shape����
int	CImportS3D::LoadShapeData()
{
	return 1;
}

//���س���ͼ
int	CImportS3D::LoadSceneGraphData()
{
	int nodeCnt = mS3DFileInfo->sceneNodeGraph.nodeCnt;

	VGS_Node* pVGSNode = &(mS3DFileInfo->sceneNodeGraph.node);
	
	//���������ڵ�
	CSceneNode* pRootNode = m_pScene->GetRootSceneNode();
	CreateSceneNode(pRootNode, pVGSNode);

	return 1;
}

//��������ͼ
void CImportS3D::CreateSceneNode(CSceneNode* pNode, VGS_Node* pVGSNode)
{	
	CSceneNode* pChildNode = NULL;
	std::string RealName = "";
	bool	bHasSkeleton = false; 

	CLight* pLight = NULL;
	CModel* pModel = NULL;

	//��Ⱦͬ������
	//EnterCriticalSection(m_render_cs);

	//Node������
	NodeType type = pVGSNode->type;

	pNode->name = pVGSNode->name.c_str();

	//�ҵ���Ӧ��ŵĶ���
	switch (type)
	{
	//ogre�Ѿ������������ڵ�
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
			//pModel->update();//��ģ�͵�λ�ñ任��RBBox

			
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

			//��ģ�͵�λ�ñ任��RBBox
			// pModel->pRBBox->SetTransformMatrix(&pChildNode->curTransform.worldMatrix);
		}
		break;

	case NNT_NONE:
		return;
		
	}

	//��Ⱦͬ������
	//LeaveCriticalSection(m_render_cs);

	//Node�ı任, ����ı任�������ﴦ��---------------------------------------------------------
	//λ��//ת������ϵ
	if (type == NNT_SCENEROOT)
	{
		pChildNode->setPosition(&(D3DXVECTOR3(0, 0, 0)));
		pChildNode->updateTransform();
	}
	else
	{		
		//����//ת������ϵ
		D3DXVECTOR3 scale;
		scale.x = pVGSNode->keyframeTransform[0].scale.x;
		scale.y = pVGSNode->keyframeTransform[0].scale.z;
		scale.z = pVGSNode->keyframeTransform[0].scale.y;
		pChildNode->setScale(&scale, 2);
		pChildNode->updateTransform();	
		
		//��ת//ת������ϵ
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

			//��S3D�ļ�������������ݣ�ת��ΪNode�Ķ����任
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
				// ���ģ��������,��ģ������Ϊ�Զ����㷢�֣�������ռ������
				if (D3DXVec3Length(&(pChildNode->worldScale)) > 0) // (pChildNode->getDerivedScaled())
				{					
					pModel->isAutoNormalise = true;
					// �����Ƿ����¼���ģ�ͷ��ߣ���֤����ģ����������ʾ�߹�
					// m_pScene->m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
				}	
				pModel->update(); // ���°�Χ��
				
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

	//��Ⱦͬ������
	//EnterCriticalSection(m_render_cs);
	//Node�Ĺؼ�֡����----------------------------------------------------
	if (pVGSNode->keyframeTransform.size() > 1 && !bHasSkeleton) //�й�����������Թؼ�֡����
	{
		CreateNodeAnimation(pChildNode, pVGSNode, type);
	}
	//��Ⱦͬ������
	//LeaveCriticalSection(m_render_cs);
	

	int childNodeCnt = pVGSNode->childNode.size();
	for (int i = 0; i< childNodeCnt; i++)
	{
		VGS_Node* pChildVGSNode = &(pVGSNode->childNode[i]);
		CreateSceneNode(pChildNode, pChildVGSNode);
	}
}

//����Node�Ķ���
void CImportS3D::CreateNodeAnimation(CSceneNode* pNode, VGS_Node* pVGSNode, NodeType type)
{
	if (!pNode || !pVGSNode)
	{
		return;
	}

	//������������
	int frameCnt = pVGSNode->keyframeTransform.size();
	std::string strAnimName = _GetNodeAnimationName(pNode);

	CNodeAnimation *pAnim = new CNodeAnimation(pNode, strAnimName);  // �����ڵ�Ĺؼ�֡����

	if (!pAnim)
	{
		return;
	}

	VGS_Camera*      pVGSCamera = NULL;   // ����Ĳ���������Ϣ
	VGS_CAMERA_INFO* pCamInfo = NULL;

	VGS_Light*      pVGSLight = NULL;    // �ƹ�Ĳ���������Ϣ
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

	//���嶯���켣�����Ĺؼ�֡�任����������ÿһ֡���ǹؼ�֡
	for (INT frameIndex = 0; frameIndex < frameCnt; frameIndex++)
	{
		NodeTransform *pTransform = &(pVGSNode->keyframeTransform[frameIndex]);           // ����Ľڵ㶯��

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

		pAnim->addKeyframe(pKeyFrame); // ���һ֡����
		pAnim->setStartPlayTime(mKeyFrameStartPlayTime);
		pAnim->setLength(mKeyFrameStartPlayTime + (1.0f * frameCnt) / gkfrPlaySpeed);

	}

	//��������״̬����
	pAnim->setEnabled(true);
	pAnim->setLoop(true);

	// ����һ���ڵ㶯��
	CAnimationManager::getSingleton().AddNodeKeyfrmAnimation(pAnim);
}

//  ��������ؼ�֡����
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

	// ��������Ľڵ㶯��
	if (pNodeTransform)
	{
		// pCamKfr->setFrom(VECTOR3(pNodeTransform->position.x, pNodeTransform->position.y, pNodeTransform->position.z));
		// pCamKfr->setAt(VECTOR3(pNodeTransform->))
	}

	// ��������Ĳ�������
	if (pCamParam)
	{
		//Fov//ʵ����max��Ĭ�ϵ���ˮƽ�����fov����d3dʹ�õ�����ֱ�����fov,���������Ҫת��һ��
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
//  �����ƹ��֡����
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
	
	// ��������Ľڵ㶯��
	if (pNodeTransform)
	{
		
	}
	
	// ��������Ĳ�������
	if (pLightParam)
	{
		pLightKfr->setPosition(VECTOR3(pLightParam->pos.x, pLightParam->pos.y, pLightParam->pos.z));

		//��ɫ
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
//  ����ģ�͹ؼ�֡����
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

//ת��S3D�����������V3D�����������ʽ
void CImportS3D::ConvCamTransformToNodeTransform(NodeTransform* pNodeTransform, VGS_CAMERA_INFO* pS3DCamInfo)
{

}



//���㳡�������еĲ�������
int CImportS3D::GetSceneMtrlCount()
{
	return m_pScene->m_vShader.size();
}

//���㳡�������е��������
int CImportS3D::GetSceneCameraCount()
{
	return m_pScene->m_vCamera.size();
}

//���㳡�������еĵƹ�����
int CImportS3D::GetSceneLightCount()
{
	return m_pScene->m_vLight.size();
}

//���㳡�����Ѿ��еĹ�������
int CImportS3D::GetSceneSkeletonCount()
{
	int count = 0;

	return count;
}

//���㳡�����Ѿ��е�Mesh����
int CImportS3D::GetSceneMeshCount()
{
	int count = 0;
	return count;
}

//���㳡�������е�Entity����
int	CImportS3D::GetSceneEntityCount()
{
	return m_pScene->model.size();
}

/**********************************************************************
* �������ƣ� // RenameMaterialIfExist
* ���������� // �����������ͬ�����ʣ����Զ�����
* ��������� // 
				RealName		: ������
				ChangeNameLog	: ��������Ϣ��¼
* ��������� // �ޡ�
* �� �� ֵ�� // �������Ĳ��ʷ���TRUE, ���򷵻�FALSE�� 
* ����˵���� // �ޡ�
*
* �޸�����		�汾��      �޸���	      �޸�����
* ----------------------------------------------	-
* 
***********************************************************************/
bool CImportS3D::RenameMaterialIfExist(std::string& RealName, std::string& RenameLog)
{
	RealName = m_pScene->RenameMaterialIfExist(RealName);	
	return true;
	
}


/**********************************************************************
* �������ƣ� // RenameCameraIfExist
* ���������� // �����������ͬ����������Զ�����
* ��������� // 
				RealName		: �����
				ChangeNameLog	: ��������Ϣ��¼
* ��������� // �ޡ�
* �� �� ֵ�� // ���������������TRUE, ���򷵻�FALSE�� 
* ����˵���� // �ޡ�
*
* �޸�����		�汾��      �޸���	      �޸�����
* ----------------------------------------------	-
* 
***********************************************************************/
bool CImportS3D::RenameCameraIfExist(std::string& RealName, std::string& RenameLog)
{
	RealName = m_pScene->RenameCameraIfExist(RealName);	
	return true;

}

/**********************************************************************
* �������ƣ� // RenameLightIfExist
* ���������� // �����������ͬ���ƹ⣬���Զ�����
* ��������� // 
				RealName		: �ƹ���
				ChangeNameLog	: ��������Ϣ��¼
* ��������� // �ޡ�
* �� �� ֵ�� // �������ĵƹⷵ��TRUE, ���򷵻�FALSE�� 
* ����˵���� // �ޡ�
*
* �޸�����		�汾��      �޸���	      �޸�����
* ----------------------------------------------	-
* 
***********************************************************************/
bool CImportS3D::RenameLightIfExist(std::string& RealName, std::string& RenameLog)
{
	RealName = m_pScene->RenameLightIfExist(RealName);	
	return true;

}


/**********************************************************************
* �������ƣ� // RenameSkeletonIfExist
* ���������� // �����������ͬ�����������Զ�����
* ��������� // 
				RealName		: ������
				ChangeNameLog	: ��������Ϣ��¼
* ��������� // �ޡ�
* �� �� ֵ�� // ��������������TRUE, ���򷵻�FALSE�� 
* ����˵���� // �ޡ�
*
* �޸�����		�汾��      �޸���	      �޸�����
* ----------------------------------------------	-
* 
***********************************************************************/
bool CImportS3D::RenameSkeletonIfExist(std::string& RealName, std::string& RenameLog)
{
	bool result = FALSE;
	/*std::string oriName = RealName;

	while (true)
	{
		bool bExist = false; //  �Ƿ��Ѿ�����
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
* �������ƣ� // RenameMeshIfExist
* ���������� // �����������ͬ��ģ�ͣ����Զ�����
* ��������� // 
				RealName	: ģ����
				RenameLog	: ��������Ϣ��¼
* ��������� // �ޡ�
* �� �� ֵ�� // ��������ģ�ͷ���TRUE, ���򷵻�FALSE�� 
* ����˵���� // �ޡ�
*
* �޸�����		�汾��      �޸���	      �޸�����
* ----------------------------------------------	-
* 
***********************************************************************/
bool CImportS3D::RenameMeshIfExist(std::string& RealName, std::string& RenameLog)
{
	RealName = m_pScene->RenameModelIfExist(RealName);	
	return true;
}

//�жϵ�ǰ��ͼ�Ƿ��Ѿ���ʹ��
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

//�ͷ���Դ
void CImportS3D::Release()
{
	//ɾ����ͼ���ʹ����б�
	// ReleaseTextureMtrlDev();
	//ɾ��ģ�͹��������б�
	// ReleaseSkeletonModelDev();
}

//ɾ����ͼ
void CImportS3D::ReleaseTextureMtrlDev()
{
	// m_textureset.clear();
}

//ɾ��ģ�͹��������б�
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

//�ж�����Ƿ��Ѿ�����
BOOL CImportS3D::IsCameraInScene(std::string strName)
{
	CCamera* pCamera = m_pScene->GetCameraByName(strName);
	return (pCamera != NULL);
}

//�жϵƹ��Ƿ��Ѿ�����
BOOL CImportS3D::IsLightInScene(std::string strName)
{
	CLight* pLight = m_pScene->GetLightByName(strName);
	return (pLight != NULL);
}

