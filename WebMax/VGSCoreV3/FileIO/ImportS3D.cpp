/////////////////////////////////////////////////////////////////////////
// ����: CImportS3D--S3D[Version 3.0]S3D���ݵ����࣬ OGRE�汾
// ����: ����3.0�汾��S3D�ļ����ݣ���CGetS3DFileInfo_V3��ã�������
// CGetS3DFileInfo_V3�����S3D�ļ���ȡ���ݣ���CImportS3D�������ȡ�����ݼ��ص�����
// ����: lixin	
// ����: 2007-11-27
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

	SetRoot(pVGS_SceneMgr->GetRoot());

	//VGS����������
	m_pScene = pVGS_SceneMgr;
	mS3DFileInfo = pS3DFileInfo;

	if (!mS3DFileInfo || !m_pScene) 
	{
		return FALSE;
	}

	//���������еĲ�������
	mPreMtrlCnt = m_pScene->getMaterialCount(); // GetSceneMtrlCount();
	//���㳡�����Ѿ����ڵ��������
	mPreCameraCnt = m_pScene->getCameraCount(); // GetSceneCameraCount();
	//���㳡�����Ѿ����ڵĵƹ�����
	mPreLightCnt = m_pScene->getLightCount();
	//���㳡�����Ѿ����ڵĹ�������
	mPreSkeletonCnt = 0; // GetSceneSkeletonCount();
	//���㳡�����Ѿ����ڵ�Mesh����
//	mPreMeshCnt = GetSceneMeshCount();
	m_preModelCnt = m_pScene->GetModelCount();
	//���㳡�������е�Entity����
	// mPreEntityCnt = GetSceneEntityCount();

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
		MessageBox(NULL, TEXT("����������ݳ���"), TEXT("����"), 0);
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
		MessageBox(NULL, TEXT("����������ݳ���"), TEXT("����"), 0);
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
		MessageBox(NULL, TEXT("����ƹ����ݳ���"), TEXT("����"), 0);
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
		MessageBox(NULL, TEXT("����ģ�����ݳ���"), TEXT("����"), 0);
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
		MessageBox(NULL, TEXT("����ͼ�����ݳ���"), TEXT("����"), 0);
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
		MessageBox(NULL, TEXT("���볡�����ݳ���"), TEXT("����"), 0);
		return FALSE;
	}

	try
	{
		//���ع�������
		LoadSkeletonAnimationData();
		curSize += oneStepSize;
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize, totalSize);  // ��ʼ����
	}
	catch (...)
	{
		MessageBox(NULL, TEXT("����������ݳ���"), TEXT("����"), 0);
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
		pMtrl->setOpacity((int)pVGSMtrl->opacity);
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
		pMtrl->selfIllumination = (UINT)pVGSMtrl->selfIllumination;
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
		pMtrl->m_FVF = FVF_POSNORMALCOLORCOORD4;
		pMtrl->m_FVFSize = sizeof(VERTEXDECL_POSNORMALCOLORCOORD4);

		// �Ƿ�diffuse��͸��ͨ��
		BOOL bIsAlphaDiffuse = FALSE;

		// bump���ڵ�һ��
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
		
		//�����Ӧ��ŵ���ͼ��û�д�������Ϊ���ʴ����ñ�ŵ���ͼ
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

				//�����͸��ͨ����ͼ��
				if (_IsTransparentFormat(((CTexture*)pLayer->pTexture)->m_format))
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
				CTextureLayer *pLayer = CreateTextureLayerFromFile(m_pScene, sName, sFileName, TRUE, pVGSMtrl->texture.opacityTexture_uvIndex);
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
			CTextureLayer *pLayer = CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.selfIlluminationTexture_uvIndex);
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
			CTextureLayer *pLayer = CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.displacementTexture_uvIndex);
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
			CTextureLayer *pLayer = CreateTextureLayerFromFile(m_pScene, sName, sFileName, FALSE, pVGSMtrl->texture.reflectionTexture_uvIndex);
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
	// mPreCameraCnt = m_pScene->getCameraCount();  // ���������е������Ŀ

	m_vCamS3DTransform.clear();

	//����������
	int cameraCnt = mS3DFileInfo->cameraManager.cameraCnt;
	for (int i = 0; i< cameraCnt; i++)
	{
		VGS_Camera* pVGSCamera = &(mS3DFileInfo->cameraManager.camera[i]);

		//�������
		std::string RealName = std::string(pVGSCamera->name);
		RealName = m_pScene->RenameCameraIfExist(RealName);

		//�������
		CCamera* pCamera = m_pScene->CreateCamera(RealName);

		//�Ƿ������
		if (pVGSCamera->isOrtho)
			pCamera->m_camType = VCT_ORTHO;
		else
			pCamera->m_camType = VCT_PROJECT;

		//�����From��
		D3DXVECTOR3 from;
		from.x = pVGSCamera->camInfo[0].from.x;
		from.y = pVGSCamera->camInfo[0].from.z;
		from.z = pVGSCamera->camInfo[0].from.y;
		//pCamera->SetPosition(&from, VTS_WORLD);
		pCamera->setOriginPosition(from);

		//�����at��
		D3DXVECTOR3 at;
		at.x = pVGSCamera->camInfo[0].at.x;
		at.y = pVGSCamera->camInfo[0].at.z;
		at.z = pVGSCamera->camInfo[0].at.y;
		//pCamera->Lookat(&at, VTS_WORLD);
		pCamera->setOriginLookatPos(at);

		//�����Roll
		//pCamera->SetRoll(pVGSCamera->camInfo[0].roll);
		pCamera->setOriginRoll(pVGSCamera->camInfo[0].roll);

		CCameraS3DTransform s3dTrans;
		s3dTrans.v3Pos = from;
		s3dTrans.v3At  = at;
		s3dTrans.roll  = -pVGSCamera->camInfo[0].roll;  // ����ϵת����ϵ��roll�෴
		s3dTrans.sName = pCamera->getName();

		m_vCamS3DTransform.push_back(s3dTrans);  // �����������

		//Fov//ʵ����max��Ĭ�ϵ���ˮƽ�����fov����d3dʹ�õ�����ֱ�����fov,���������Ҫת��һ��
		double tan_half_fovy = 0.75f * tan(pVGSCamera->camInfo[0].fov/2.0f);
		pCamera->SetFov(2.0f * (float)atan(tan_half_fovy));

		//near far clip
		pCamera->setNearClip(g_CamDefaultNearClip);
		pCamera->setFarClip(g_CamDefaultfarClip);

		//��������Ĳ�������
		// CreateCameraParamAnimation(pCamera, pVGSCamera);

	}
	return 1;
}

//���صƹ�����
int	CImportS3D::LoadLightData()
{
	// mPreLightCnt = m_pScene->getLightCount();  // ���������еĵƹ���Ŀ

	//����ƹⴴ��
	int lightCnt = mS3DFileInfo->lightManager.lightCnt;
	for (int i = 0; i < lightCnt; i++)
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

//���ع�������
int	CImportS3D::LoadSkeletonAnimationData()
{
 	int skeletonAnimationCnt = mS3DFileInfo->skeletonAnimationManager.skeletonAnimationCnt;
	
	for (int i = 0; i< skeletonAnimationCnt; i++)
	{
		VGS_SkeletonAnimation skeletonAnimation = mS3DFileInfo->skeletonAnimationManager.skeletonAnimation[i];

	}
	return 1;
}


//����Meshģ������
int	CImportS3D::LoadMeshData()
{
	// mPreMeshCnt = m_pScene->GetModelCount();

	//�������Mesh������
	int meshCnt = mS3DFileInfo->m_vMeshData.size();

	for (int i = 0; i < meshCnt; i++)
	{
		CVGSMesh* pVGSMesh = &(mS3DFileInfo->m_vMeshData[i]);

		//ģ����
		CModel* pModel = new CModel(m_pScene->m_pd3dDevice);
		pModel->Create(pVGSMesh, m_vLoadedShader);

		//ģ����
		std::string RealName = m_pScene->RenameModelIfExist(pVGSMesh->sName);
		pModel->setName(RealName);

		//���볡����ģ���б�
		// m_pScene->model.push_back(pModel);  // ���浽ģ���б�
		m_pScene->AddModel(pModel);
	} 

	return 1;
}

//����Shape����
int	CImportS3D::LoadShapeData()
{
	return 1;
}

//��ó������ӽڵ�
void CImportS3D::GetSceneNodePtrList( vector<CSceneNode*>& vNodeList, CSceneNode* pRoot )
{
	vNodeList.push_back( pRoot );

	int c_childs = pRoot->m_child.size();
	for( int i = 0;i<c_childs;i++ )
	{
		GetSceneNodePtrList( vNodeList,pRoot->m_child[i] );
	}
}

//���س���ͼ
int	CImportS3D::LoadSceneGraphData()
{
	int nodeCnt = mS3DFileInfo->sceneNodeGraph.nodeCnt;

	VGS_Node* pVGSNode = &(mS3DFileInfo->sceneNodeGraph.node);
	
	//���������ڵ�
	CSceneNode* pRootNode = m_pScene->GetRootSceneNode();
	CreateSceneNode(pRootNode, pVGSNode);

	vector<CSceneNode*>nodevec ;
	GetSceneNodePtrList(nodevec,pRootNode);

	return 1;
}

//��������ͼ
void CImportS3D::CreateSceneNode(CSceneNode* pNode, VGS_Node* pVGSNode)
{	
	CSceneNode* pChildNode = NULL;
	std::string RealName = "";

	bool	bHasSkeleton = pVGSNode->bHasSkeleton; 

	CLight*  pLight = NULL;
	CModel*  pModel = NULL;
	CCamera* pCamera = NULL;

	//��Ⱦͬ������
	//EnterCriticalSection(m_render_cs);

	//Node������
	NodeType type = pVGSNode->type;

	//�ҵ���Ӧ��ŵĶ���
	switch (type)
	{
	//ogre�Ѿ������������ڵ�
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
	
	//��Ⱦͬ������
	//LeaveCriticalSection(m_render_cs);

	//Node�ı任, ����ı任�������ﴦ��---------------------------------------------------------
	//λ��//ת������ϵ
	if (type == NNT_SCENEROOT)
	{
		pChildNode->setPosition(&(D3DXVECTOR3(0, 0, 0)), VTS_WORLD);
		//pChildNode->updateTransform();
	}
	else
	{		
		//����//ת������ϵ
		D3DXVECTOR3 scale;
		scale.x = pVGSNode->keyframeTransform[0].scale.x;
		scale.y = pVGSNode->keyframeTransform[0].scale.z;
		scale.z = pVGSNode->keyframeTransform[0].scale.y;
		//pChildNode->setScale(&scale, VTS_PARENT);
		
		//��ת//ת������ϵ
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
		//��ñ任����
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
				// pCamera->restore();  // ����Ϊorigin����
				CCameraS3DTransform s3dTrans = getCameraParamByName(pCamera->getName());
				pCamera->SetPosition(&s3dTrans.v3Pos, VTS_WORLD);
				pCamera->Lookat(&s3dTrans.v3At, VTS_WORLD);
				pCamera->SetRoll(s3dTrans.roll);

				//��S3D�ļ�������������ݣ�ת��ΪNode�Ķ����任
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
				// ���ģ��������,��ģ������Ϊ�Զ����㷢�֣�������ռ������
				if (D3DXVec3Length(&(pChildNode->getScale(VTS_WORLD))) > 0) // (pChildNode->getDerivedScaled())
				{					
					pModel->isAutoNormalise = true;
				}	
				pModel->UpdateFromNode(); // ���°�Χ��
				
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
	std::string strAnimName = m_pRoot->GetAnimManager().GetNodeAnimationName(pNode);

	CNodeAnimation *pAnim = new CNodeAnimation(pNode, strAnimName);  // �����ڵ�Ĺؼ�֡����

	if (!pAnim)
	{
		return;
	}

	VGS_Camera*      pVGSCamera = NULL;   // ����Ĳ���������Ϣ
	VGS_CAMERA_INFO* pCamInfo = NULL;

	VGS_Light*      pVGSLight = NULL;    // �ƹ�Ĳ���������Ϣ
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

	//���嶯���켣�����Ĺؼ�֡�任����������ÿһ֡���ǹؼ�֡
	for (INT frameIndex = 0; frameIndex < frameCnt; frameIndex++)
	{
		NodeTransform *pTransform = &(pVGSNode->keyframeTransform[frameIndex]);           // ����Ľڵ㶯��

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

		pAnim->addKeyframe(pKeyFrame); // ���һ֡����
		pAnim->setStartPlayTime(mKeyFrameStartPlayTime);
		pAnim->setLength(mKeyFrameStartPlayTime + (1.0f * frameCnt) / gkfrPlaySpeed);

	}

	//��������״̬����
	pAnim->setEnabled(true);
	pAnim->setLoop(true);

	// ����һ���ڵ㶯��
	m_pRoot->GetAnimManager().AddNodeKeyfrmAnimation(pAnim);
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
		pCamKfr->setFov((float)fovy);
		pCamKfr->setRoll(pCamParam->roll);
		pCamKfr->setClip(10, 100000);
		pCamKfr->setFrom(VECTOR3(pCamParam->from.x, pCamParam->from.z, pCamParam->from.y));
		pCamKfr->setAt(VECTOR3(pCamParam->at.x, pCamParam->at.z, pCamParam->at.y));
		pCamKfr->m_bHasParamAnimation = true;
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
		pLightKfr->m_bHasParamAnimation = true;

		pLightKfr->setPosition(VECTOR3(pLightParam->pos.x, pLightParam->pos.y, pLightParam->pos.z));

		//��ɫ
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

//
//
////���㳡�������еĲ�������
//int CImportS3D::GetSceneMtrlCount()
//{
//	return m_pScene->m_vShader.size();
//}
//
////���㳡�������е��������
//int CImportS3D::GetSceneCameraCount()
//{
//	return m_pScene->m_vCamera.size();
//}
//
////���㳡�������еĵƹ�����
//int CImportS3D::GetSceneLightCount()
//{
//	return m_pScene->m_vLight.size();
//}
//
////���㳡�����Ѿ��еĹ�������
//int CImportS3D::GetSceneSkeletonCount()
//{
//	int count = 0;
//
//	return count;
//}
//
////���㳡�����Ѿ��е�Mesh����
//int CImportS3D::GetSceneMeshCount()
//{
//	int count = 0;
//	return count;
//}
//
////���㳡�������е�Entity����
//int	CImportS3D::GetSceneEntityCount()
//{
//	return m_pScene->model.size();
//}

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


// ��ͼ���ļ�����һ��texturelayer
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
