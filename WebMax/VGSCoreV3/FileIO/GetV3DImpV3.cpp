//----------------------------------------------------------
#include "GetV3DImpV3.h"
#include "..\Root.h"
#include "..\VGSErrCode.h"
//----------------------------------------------------------
CGetV3DImpV3::CGetV3DImpV3(void)
{
}
//----------------------------------------------------------
CGetV3DImpV3::~CGetV3DImpV3(void)
{
}
//----------------------------------------------------------



//----------------------------------------------------------
// 读取所有数据
//----------------------------------------------------------
 DWORD CGetV3DImpV3::ReadAllData()
 {	
	fseek(m_pFile, m_V3DTable.PrivacyDataPtr, SEEK_SET); // 移动到开始位置

	// 授权信息
	UINT PrivacyDataSize = m_V3DTable.cameraDataPtr - m_V3DTable.PrivacyDataPtr;
	m_pPrivacyBuf = new char[PrivacyDataSize];
	if (1 != fread(m_pPrivacyBuf, PrivacyDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 相机数据
	UINT CameDataSize = m_V3DTable.lightDataPtr - m_V3DTable.cameraDataPtr;
	m_pCameraBuf = new char[CameDataSize];
	if (1 != fread(m_pCameraBuf, CameDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 灯光
	UINT LightDataSize = m_V3DTable.texturePtr - m_V3DTable.lightDataPtr;
	m_pLightBuf = new char[LightDataSize];
	if (1 != fread(m_pLightBuf, LightDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 贴图纹理数据
	UINT TextureDataSize = m_V3DTable.mtrlDataPtr - m_V3DTable.texturePtr;
	m_pTextureBuf = new char[TextureDataSize];
	if (1 != fread(m_pTextureBuf, TextureDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 材质数据
	UINT MtrDataSize = m_V3DTable.meshDataPtr - m_V3DTable.mtrlDataPtr;	
	m_pMaterailBuf = new char[MtrDataSize];
	if (1 != fread(m_pMaterailBuf, MtrDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// entity(Model)
	UINT EntityDataSize = m_V3DTable.TextDataPtr - m_V3DTable.meshDataPtr;
	m_pEntityBuf = new char[EntityDataSize];
	if (1 != fread(m_pEntityBuf, EntityDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 文字
	UINT TextDataSize = m_V3DTable.OverlayPtr - m_V3DTable.TextDataPtr;
	m_pTextBuf = new char[TextDataSize];
	if (1 != fread(m_pTextBuf, TextDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// overlay
	UINT OverlayDataSize = m_V3DTable.NavigatotPtr - m_V3DTable.OverlayPtr;
	m_pOverlayBuf = new char[OverlayDataSize];
	if (1 != fread(m_pOverlayBuf, OverlayDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 导航图
	UINT NaviSize = m_V3DTable.ButtonPtr - m_V3DTable.NavigatotPtr;
	m_pNaviBuf = new char[NaviSize];
	int ReadSize = fread(m_pNaviBuf, NaviSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 按钮
	UINT ButtonSize = m_V3DTable.BackdropPtr - m_V3DTable.ButtonPtr;
	m_pButtonBuf = new char[ButtonSize];
	ReadSize = fread(m_pButtonBuf, ButtonSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 背景图
	UINT BackdropSize = m_V3DTable.AudioPtr - m_V3DTable.BackdropPtr;
	m_pBackdropBuf = new char[BackdropSize];
	ReadSize = fread(m_pBackdropBuf, BackdropSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 声音
	UINT AudioSize = m_V3DTable.FlashPtr - m_V3DTable.AudioPtr;
	m_pAudioBuf = new char[AudioSize];
	ReadSize = fread(m_pAudioBuf, AudioSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// flash
	UINT FlashDataSize = m_V3DTable.AnimationPtr - m_V3DTable.FlashPtr;
	m_pFlashBuf = new char[FlashDataSize];
	ReadSize = fread(m_pFlashBuf, FlashDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 动画
	UINT AnimationDataSize = m_V3DTable.ControlPtr - m_V3DTable.AnimationPtr;
	m_pAnimationBuf = new char[AnimationDataSize];
	ReadSize = fread(m_pAnimationBuf, AnimationDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 控制信息
	UINT ControlDataSize = m_V3DTable.sceneGraphDataPtr - m_V3DTable.ControlPtr;
	m_pControlBuf = new char[ControlDataSize];
	fseek(m_pFile, m_V3DTable.ControlPtr, SEEK_SET);
	ReadSize = fread(m_pControlBuf, ControlDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 场景图
	fseek(m_pFile, 0, SEEK_END); // 文件结尾
	UINT FileSize = ftell(m_pFile);
	UINT DataSize = FileSize - m_V3DTable.sceneGraphDataPtr;
	m_pSceneGraphBuf = new char[DataSize];
	fseek(m_pFile, m_V3DTable.sceneGraphDataPtr, SEEK_SET);
	ReadSize = fread(m_pSceneGraphBuf, DataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	return ERR_COMMON_OK;
 }
//----------------------------------------------------------
 DWORD CGetV3DImpV3::ReadMaterialData()
{
	if (!m_pFile)
	{
		// throw Exception::exception("读文件发生错误(03)，无法读取文件");
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pMaterailBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	m_vMaterial.clear();

	// 读取材质信息
	char* Offset = m_pMaterailBuf;          // 字节流中数据开始的位置
	DWORD MtrCount = 0;                     // 材质数目
	CPY_FROM_STREAM(MtrCount, Offset)       // 读取材质数目
	for (DWORD MtrIdx = 0; MtrIdx < MtrCount; ++MtrIdx)
	{
		DWORD len = 0;
		CPY_FROM_STREAM(len, Offset)        // Mtrl Name length
		std::string sMtlName = "";
		if (len > 0)
		{
			char *pName = new char[len + 1];
			ZeroMemory(pName, len + 1);
			memcpy(pName, Offset, len);         // Mtrl Name
			Offset += len;
			sMtlName = std::string(pName);			
			delete [] pName;
			pName = NULL;
		}
		else
		{
			// 创建材质资源到指定资源组, Ogre会自动创建一个technique和一个pass
			// pMtrl = MaterialManager::getSingleton().create(pName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			MessageBox(NULL, TEXT("数据中没有为材质指定名称，可能会导致问题！"), TEXT("错误"), 0);
			continue;
		}

		//创建材质资源到指定资源组
		CMaterial *pMtrl = m_pVGSSceneMgr->CreateMaterial(sMtlName);
		if (!pMtrl)
		{
			continue;
		}

		//pass详细参数
		V3D_PassDetail_Ex PassDetail;
		CPY_FROM_STREAM(PassDetail, Offset);
		
		pMtrl->m_bUseDiffuse     = PassDetail.bUseDiffuse;
		if (pMtrl->m_bUseDiffuse)
		{
			pMtrl->d3dmtrl.Diffuse = _Get_D3DCOLORVALUE_ARGB(PassDetail.Dif_ARGB);
		}
		else
		{
			D3DCOLORVALUE color;
			color.r = color.g = color.b = 1.0f;
			color.a = (PassDetail.Dif_ARGB & 0x000000ff) / 255.0f; // 演示是rgba格式
			pMtrl->d3dmtrl.Diffuse = color;
		}
		pMtrl->d3dmtrl.Ambient = _Get_D3DCOLORVALUE_ARGB(PassDetail.Dif_ARGB);

		pMtrl->specular = _Get_D3DCOLORVALUE_ARGB(PassDetail.Spe_ARGB);
		pMtrl->specularLevel = PassDetail.Spe_Level;
		float factor = pMtrl->specularLevel / 100.0f;
		pMtrl->d3dmtrl.Specular = _Multiply_D3DCOLORVALUE(pMtrl->specular, factor);
		pMtrl->d3dmtrl.Power    = PassDetail.shininess;
		
		pMtrl->selfIllumination = (UINT)PassDetail.selfIllumination;
		factor = pMtrl->selfIllumination / 100.0f;
		pMtrl->d3dmtrl.Emissive = _Multiply_D3DCOLORVALUE(pMtrl->d3dmtrl.Diffuse, factor);
		
		pMtrl->bAlphaTest = PassDetail.bAlphaTest;
		pMtrl->AlpahaTestValue = PassDetail.AlphaTestValue;

		pMtrl->twoSide = (PassDetail.bTwoside == 1) ? true : false;

		pMtrl->fillMode = (_D3DFILLMODE)PassDetail.fillMode;
		pMtrl->setOpacity((float)(pMtrl->d3dmtrl.Diffuse.a * 100/*/ 2.55f*/ + 0.5));  // 由[0, 255]换算成[0, 100]

		// 纹理贴图的数目
		UINT texLayerCount = 0;
		CPY_FROM_STREAM(texLayerCount, Offset);

		// 每个帖图层
		for (UINT iLayer = 0; iLayer < texLayerCount; ++iLayer)
		{
			CTextureLayer *pLayer = new CTextureLayer;

			// Unit Name
			UINT nameLen;
			CPY_FROM_STREAM(nameLen, Offset);
			char *pName = new char[nameLen + 1];
			ZeroMemory(pName, nameLen + 1);
			CPY_PTR_FROM_STREAM(pName, nameLen, Offset);
			pLayer->m_sName = std::string(pName);
			delete [] pName;
			pName = 0;

			// 坐标序号
			UINT UVIndex = 0;	
			CPY_FROM_STREAM(UVIndex, Offset);
			pLayer->UVIndex = UVIndex;

			// mipMap
			BYTE mipMap  = 0;	
			CPY_FROM_STREAM(mipMap, Offset);
			pLayer->mipMap = mipMap;

			FLOAT USpeed = 0;
			CPY_FROM_STREAM(USpeed, Offset);
			pLayer->USpeed = USpeed;

			FLOAT VSpeed = 0;
			CPY_FROM_STREAM(VSpeed, Offset);
			pLayer->VSpeed = VSpeed;

			// 层类型
			VGS_TextureType texType = TLT_INVALID;
			CPY_FROM_STREAM(texType, Offset);
			pLayer->type = (VGS_TextureType)texType;

			// 帧数
			UINT FrameCount = 0;
			CPY_FROM_STREAM(FrameCount, Offset);

			// 图像的帧数，以及每帧的图像名称，不包括路径
			vector <std::string> vImage;
			for (UINT iFrame = 0; iFrame < FrameCount; ++iFrame)
			{
				UINT nameLen;
				CPY_FROM_STREAM(nameLen, Offset);
				char *pName = new char[nameLen + 1];
				ZeroMemory(pName, nameLen + 1);
				CPY_PTR_FROM_STREAM(pName, nameLen, Offset);
				std::string strFileName(pName);
				strFileName = AddPathtoTextureFile(strFileName);
				vImage.push_back(strFileName);
				delete [] pName;
				pName = 0;
			}
			
			// 创建贴图层					
			pLayer->pTexture = 0;
			switch (pLayer->type)
			{
			case TLT_DYNAMIC:
				{
					int duration = 0;
					CPY_FROM_STREAM(duration, Offset);    
					break;
				}				
			case TLT_REFLECT:
				{
					char type = 0;
					CPY_FROM_STREAM(type, Offset);   
					if (vImage.size() > 0)
					{
						pLayer->pTexture = m_pRoot->GetTextureManager().createTexture(vImage[0], TRUE, mipMap);
					}
					break;
				}
			case TLT_OPACITY:
				if (vImage.size() > 0)
				{
					pLayer->pTexture = m_pRoot->GetTextureManager().createTexture(vImage[0], TRUE, mipMap);
				}
				break;
			case TLT_BUMPMAP:
				{
					N3D_BumpwaterTextureParam bumpWaterParam;
					CPY_FROM_STREAM(bumpWaterParam, Offset);   

					CBumpMap *pBump = ((CBumpMap*)pLayer->pTexture);
					pBump->SetBumpwaveParam(bumpWaterParam.width, bumpWaterParam.height, bumpWaterParam.XScale, bumpWaterParam.YScale,
							                    bumpWaterParam.Radius, bumpWaterParam.Speed, bumpWaterParam.CenterX, bumpWaterParam.CenterY);

					/*std::string strName = pLayer->m_sName;
					if (vImage.size() > 0)
					{
						strName = vImage[0];
					}
					pLayer->pTexture = m_pRoot->GetTextureManager().CreateBumpTexture(strName);
					CBumpMap *pBump = ((CBumpMap*)pLayer->pTexture);
					if (pBump && bumpType == VBT_BUMPWATER)
					{
						pBump->SetBumpAsWavemap(bumpWaterParam.width, bumpWaterParam.height, bumpWaterParam.XScale, bumpWaterParam.YScale,
							                    bumpWaterParam.Radius, bumpWaterParam.Speed, bumpWaterParam.CenterX, bumpWaterParam.CenterY);
					}
					else if (pBump && bumpType == VBT_BUMPENVIRONMENT)
					{
						pBump->SetBumpAsEnviromentmap(strName, bumpEnvParam.noise1, bumpEnvParam.noise2);
					}
					else if (pBump && bumpType == VBT_BUMPNORMARLMAP)
					{
						pBump->SetBumpAsNormalmap(strName, (CLight*)bumpNormalParam.LightHandle);
					}*/
					break;
				}
			case TLT_RTFMAP: // 实时反射
				{
					/*std::string strName = pLayer->m_sName;
					if (vImage.size() > 0)
					{
						strName = vImage[0];
					}
					pLayer->pTexture = m_pRoot->GetTextureManager().CreateRTreflectTexture(strName, refrationParam.width, reflectionParam.height);
					CRTFMap* pRTFMap = (CRTFMap*)pLayer->pTexture;
					if (pRTFMap)
					{
						pRTFMap->SetRefPlaneByModel(reflectionParam.refectionModel);
						for (UINT iModel = 0; iModel < reflectionParam.vrefectedModels.size(); ++iModel)
						{
							pRTFMap->AddRefModel(reflectionParam.vrefectedModels[iModel]);
						}
					}*/
				}
				break;
			case TLT_RTTMAP:  // 实时折射
				{
					/*std::string strName = pLayer->m_sName;
					if (vImage.size() > 0)
					{
						strName = vImage[0];
					}
					pLayer->pTexture = CTextureManager::getSingleton().CreateRTreflectTexture(strName, refrationParam.width, reflectionParam.height);
					CRTFMap* pRTFMap = (CRTFMap*)pLayer->pTexture;
					if (pRTFMap)
					{
						pRTFMap->SetRefPlaneByModel(reflectionParam.refectionModel);
						for (UINT iModel = 0; iModel < reflectionParam.vrefectedModels.size(); ++iModel)
						{
							pRTFMap->AddRefModel(reflectionParam.vrefectedModels[iModel]);
						}
					}*/
				}
				break;
			case TLT_BASAL:
			default:
				if (vImage.size() > 0)
				{
					pLayer->pTexture = m_pRoot->GetTextureManager().createTexture(vImage[0], FALSE, mipMap);
				}
				break;
			}

			// 颜色的混色模式
			VGS_BlenderInfo colorBlendInfo;
			CPY_FROM_STREAM(colorBlendInfo, Offset);  
			pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(colorBlendInfo.option);
			pLayer->Blenderinfo.ColorBlendParam  = D3DCOLOR_COLORVALUE(0, 0, 0, colorBlendInfo.param.BlenderOP_alpha);					

			// alpah通道的混色模式
			VGS_BlenderInfo alphaBlendInfo;
			CPY_FROM_STREAM(alphaBlendInfo, Offset);  
			pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(alphaBlendInfo.option);
			pLayer->Blenderinfo.AlphaBlendParam  = D3DCOLOR_COLORVALUE(0, 0, 0, alphaBlendInfo.param.BlenderOP_alpha);

			pMtrl->m_vpTexLayers.push_back(pLayer);
		}

		m_vMaterial.push_back(pMtrl);

	} // Material 

	DELETE_ARRAYPTR(m_pMaterailBuf);  // 删除字节流

	return ERR_COMMON_OK;
}	
//----------------------------------------------------------
DWORD CGetV3DImpV3::ReadCameraData()
 {	
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}
	if (!m_pCameraBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// 字节流中数据开始的位置
	char* Offset = m_pCameraBuf;   
	
	m_vpCamera.clear();  // 情况相机列表

	//读入相机数量
	UINT cameraCnt = 0;                         
	CPY_FROM_STREAM(cameraCnt, Offset)       

	//逐个相机读入数据
	for (UINT i = 0; i < cameraCnt; i++)
	{
		//相机名称
		UINT len = 0;
		CPY_FROM_STREAM(len, Offset); 
		char *pName = new char[len + 1];
		ZeroMemory(pName, len + 1);
		CPY_PTR_FROM_STREAM(pName, len, Offset);
		std::string sName = std::string(pName);
		delete pName;
		pName = NULL;

		// 创建相机
		CCamera *pCamera = m_pVGSSceneMgr->CreateCamera(sName);
		if (!pCamera)
		{
			return ERR_SCENE_CREATE;
		}

		// 相机的基本参数
		VGS_V3D_CAMINFO_V3 camInfo;
		CPY_FROM_STREAM(camInfo.isOrtho, Offset);
		CPY_FROM_STREAM(camInfo.fov, Offset);
		CPY_FROM_STREAM(camInfo.nearClipDis, Offset);
		CPY_FROM_STREAM(camInfo.farClipDis, Offset);
		CPY_FROM_STREAM(camInfo.position, Offset);
		CPY_FROM_STREAM(camInfo.at, Offset);
		CPY_FROM_STREAM(camInfo.up, Offset);

		// 投影类型
		if (camInfo.isOrtho)
		{
			pCamera->m_camType = VCT_ORTHO;
		}
		else
		{
			pCamera->m_camType = VCT_PROJECT;
		}
		
		// Fov
		pCamera->SetFov(camInfo.fov);

		//near far clip
		pCamera->setNearClip(camInfo.nearClipDis);
		pCamera->setFarClip(camInfo.farClipDis);

		pCamera->SetPosition(&D3DXVECTOR3(camInfo.position.x, camInfo.position.y, camInfo.position.z), VTS_PARENT);
		pCamera->Lookat(&D3DXVECTOR3(camInfo.at.x, camInfo.at.y, camInfo.at.z), VTS_PARENT);
		pCamera->SetUp(D3DXVECTOR3(camInfo.up.x, camInfo.up.y, camInfo.up.z), VTS_PARENT);

		m_vpCamera.push_back(pCamera);   // 保存起来，后面可能会使用
	}

	DELETE_ARRAYPTR(m_pCameraBuf);  // 删除字节流

	return ERR_COMMON_OK;
 }
//----------------------------------------------------------
DWORD CGetV3DImpV3::ReadLightData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pLightBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}
	if (!m_pVGSSceneMgr)
	{
		return ERR_SCENE_NOSCENE;
	}

	//读取灯光的数量
	char* Offset = m_pLightBuf; 
	int LightCount = 0; 
	CPY_FROM_STREAM(LightCount, Offset);

	m_vpLight.clear();
	
	//逐个灯光读取数据
	for (int i = 0; i < LightCount; i++)
	{
		UINT len = 0;
		CPY_FROM_STREAM(len, Offset);
		char pName[256];
		ZeroMemory(pName, 256);
		memcpy(pName, Offset, len);
		Offset += len;		
		
		CLight *pLight = m_pVGSSceneMgr->CreateLight(std::string(pName));
		if (!pLight)
		{
			return ERR_SCENE_CREATE;
		}

		//灯光类型
		VGS_LIGHT_TYPE type;
		CPY_FROM_STREAM(type, Offset)

		switch (type)
		{
		case VLT_OMNI:
			{
				//读入点光源信息
				VGS_V3D_OMNI omni;
				CPY_FROM_STREAM(omni, Offset);
				
				D3DCOLORVALUE diffuse;
				diffuse.r = ((omni.color>>16) & 0xff)/255.0f;
				diffuse.g = ((omni.color>>8) & 0xff)/255.0f;
				diffuse.b = ((omni.color) & 0xff)/255.0f;
				diffuse.a = 1.0f;
				pLight->diffuse  = diffuse;
				pLight->multiply = omni.multiply;
				
				pLight->param.Diffuse.r = diffuse.r * omni.multiply;
				pLight->param.Diffuse.g = diffuse.g * omni.multiply;
				pLight->param.Diffuse.b = diffuse.b * omni.multiply;
				pLight->param.Diffuse.a = 1.0f;
				
				//环境光，用它来近似模拟全局光
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
				pLight->param.Ambient.a = 1.0f;
				
				//高光
				pLight->param.Specular   = omni.multiply * pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式
				pLight->param.Specular.a = 1.0f;
				
				//光线到达的最远距离
				pLight->param.Range        = omni.attenuationRange;
				pLight->param.Attenuation0 = omni.attenuation0;
				pLight->param.Attenuation1 = omni.attenuation1;
				pLight->param.Attenuation2 = omni.attenuation2;
				
				pLight->param.Type = D3DLIGHT_POINT;
				D3DXVECTOR3 vPos(omni.position.x, omni.position.y, omni.position.z);  
				pLight->setPosition(&vPos);	
			}
			break;

		case VLT_DIR:
			{				
				//读入点光源信息
				VGS_V3D_DIRECTIONAL directionLight;
				CPY_FROM_STREAM(directionLight, Offset);
				
				D3DCOLORVALUE diffuse;
				diffuse.r = ((directionLight.color>>16) & 0xff)/255.0f;
				diffuse.g = ((directionLight.color>>8) & 0xff)/255.0f;
				diffuse.b = ((directionLight.color) & 0xff)/255.0f;
				diffuse.a = 1.0f;
				pLight->diffuse  = diffuse;
				pLight->multiply = directionLight.multiply;
				
				pLight->param.Diffuse.r = diffuse.r * directionLight.multiply;
				pLight->param.Diffuse.g = diffuse.g * directionLight.multiply;
				pLight->param.Diffuse.b = diffuse.b * directionLight.multiply;
				pLight->param.Diffuse.a = 1.0f;
				
				//环境光，用它来近似模拟全局光
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
				pLight->param.Ambient.a = 1.0f;
				
				//高光
				pLight->param.Specular   = directionLight.multiply * pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式
				pLight->param.Specular.a = 1.0f;
				
				//光线到达的最远距离
				pLight->param.Range        = directionLight.attenuationRange;
				pLight->param.Attenuation0 = directionLight.attenuation0;
				pLight->param.Attenuation1 = directionLight.attenuation1;
				pLight->param.Attenuation2 = directionLight.attenuation2;
				
				pLight->param.Type = D3DLIGHT_DIRECTIONAL;

				//方向
				D3DXVECTOR3 dir(directionLight.direction.x, directionLight.direction.y, directionLight.direction.z);
				pLight->setDirection(&dir);
			}
			break;

		case VLT_SPOT:
			{
				//读入点光源信息
				VGS_V3D_SPOTLIGHT spotLight;
				CPY_FROM_STREAM(spotLight, Offset);
				
				D3DCOLORVALUE diffuse;
				diffuse.r = ((spotLight.color>>16) & 0xff)/255.0f;
				diffuse.g = ((spotLight.color>>8) & 0xff)/255.0f;
				diffuse.b = ((spotLight.color) & 0xff)/255.0f;
				diffuse.a = 1.0f;
				pLight->diffuse  = diffuse;
				pLight->multiply = spotLight.multiply;
				
				pLight->param.Diffuse.r = diffuse.r * spotLight.multiply;
				pLight->param.Diffuse.g = diffuse.g * spotLight.multiply;
				pLight->param.Diffuse.b = diffuse.b * spotLight.multiply;
				pLight->param.Diffuse.a = 1.0f;
				
				//环境光，用它来近似模拟全局光
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
				pLight->param.Ambient.a = 1.0f;
				
				//高光
				pLight->param.Specular   = spotLight.multiply * pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式
				pLight->param.Specular.a = 1.0f;
				
				//光线到达的最远距离
				pLight->param.Range        = spotLight.attenuationRange;
				pLight->param.Attenuation0 = spotLight.attenuation0;
				pLight->param.Attenuation1 = spotLight.attenuation1;
				pLight->param.Attenuation2 = spotLight.attenuation2;
				
				pLight->param.Type = D3DLIGHT_SPOT;
				
				//方向
				D3DXVECTOR3 dir(spotLight.direction.x, spotLight.direction.y, spotLight.direction.z);
				pLight->setDirection(&dir);

				//张角
				pLight->param.Theta = spotLight.innerAngle;
				pLight->param.Phi = spotLight.outerAngle;
				pLight->param.Falloff = spotLight.fallOff;

				// 位置
				D3DXVECTOR3 vPos(spotLight.position.x, spotLight.position.y, spotLight.position.z);  
				pLight->setPosition(&vPos);
			}
			break;
		}

		m_vpLight.push_back(pLight);  // 保存以及加载的灯光
	}

	DELETE_ARRAYPTR(m_pLightBuf);  // 删除字节流

	return ERR_COMMON_OK;
}
//----------------------------------------------------------
DWORD CGetV3DImpV3::ReadMeshData()
{
	// Only Model and no Mesh in 3.0, check it in file SaveV3D.cpp
	return ERR_COMMON_OK;
}
//----------------------------------------------------------
DWORD CGetV3DImpV3::ReadModelData()
{	
	if (!m_pFile)
	{
		// throw Exception::exception("读文件发生错误(03)，无法读取文件");
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pEntityBuf)
	{
		return ERR_FILE_V3D_NODATA; 
	}

	// 读取entity信息
	char* Offset = m_pEntityBuf;              // 字节流中数据开始的位置
	DWORD EntityCount;                        // entity数目
	CPY_FROM_STREAM(EntityCount, Offset)      // 读取entity数目
	
	// 读取每个模型
	for (UINT idx = 0; idx < EntityCount; ++idx)
	{
		int namelen = 0;
		char sName[MAX_FILENAME_LENGTH];
		ZeroMemory(sName, MAX_FILENAME_LENGTH);
		CPY_FROM_STREAM(namelen, Offset)      // entity名称		
		memcpy(sName, Offset, namelen);       // 读取名称
		Offset += namelen;

		CModel *pModel = m_pVGSSceneMgr->CreateModel(std::string(sName));

		// 模型的顶点格式
		DWORD fvf = 0;
		CPY_FROM_STREAM(fvf, Offset)
		pModel->m_FVF = fvf;
		pModel->m_FVFSize = D3DXGetFVFVertexSize(pModel->m_FVF);
		
		// 顶点个数
		UINT vertexCount = 0; 
		CPY_FROM_STREAM(vertexCount, Offset)
		pModel->m_vertexCount = vertexCount;

		// mesh 的顶点信息
		BYTE *pVertextBuffer = new BYTE[vertexCount * pModel->m_FVFSize];
		memset(pVertextBuffer, 0, vertexCount * pModel->m_FVFSize);
		CPY_PTR_FROM_STREAM(pVertextBuffer, vertexCount * pModel->m_FVFSize, Offset);
	
		// 面个数
		UINT faceCount = 0;
		CPY_FROM_STREAM(faceCount, Offset)
		pModel->m_faceCount = faceCount;

		// 面顶点index buffer数据		
		UINT indexSize = pModel->m_faceCount * 3 * sizeof(UINT); 
		BYTE *pIndexBuffer = new BYTE[indexSize];
		CPY_PTR_FROM_STREAM(pIndexBuffer, indexSize, Offset);

		// submesh 数目
		unsigned short SubMeshCount = 0;
		CPY_FROM_STREAM(SubMeshCount, Offset)
		
		//  添加每个submesh的信息
		vector<CSubModel> vSubModel;
		for (UINT iSubMesh = 0; iSubMesh < SubMeshCount; ++iSubMesh)
		{
			CSubModel subModel;

			// submesh使用的材质索引
			int indexMtrl = 0;
			CPY_FROM_STREAM(indexMtrl, Offset)
			subModel.m_pMaterial = _GetHandleByIndex(m_vMaterial, indexMtrl);
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_3D, subModel.m_pMaterial);

			// 每个字模型的顶点序列、索引序列
			CPY_FROM_STREAM(subModel.m_vertexStart, Offset)
			CPY_FROM_STREAM(subModel.m_vertexCount, Offset)
			CPY_FROM_STREAM(subModel.m_faceStart, Offset)
			CPY_FROM_STREAM(subModel.m_faceCount, Offset)
			
			vSubModel.push_back(subModel);
		} 

		// 创建模型
		HRESULT hr = pModel->Create(fvf, vertexCount, faceCount, pVertextBuffer, pIndexBuffer, vSubModel);

		// 模型的属性信息
		bool bBillBoard = false;
		CPY_FROM_STREAM(bBillBoard, Offset)
		pModel->isBillboard = bBillBoard;
		if (pModel->isBillboard)
		{
			// 正对相机的坐标轴
			BYTE axis;
			CPY_FROM_STREAM(axis, Offset)
			//pModel->billboardDirectionAxis = axis;
		}

		bool bCollision = false;
		CPY_FROM_STREAM(bCollision, Offset)
		pModel->isCollision = bCollision;

		bool bGround = false;
		CPY_FROM_STREAM(bGround, Offset)
		pModel->isGround = bGround;

		bool bVisible = false;
		CPY_FROM_STREAM(bVisible, Offset)
		pModel->visible = bVisible;

		bool bClickable = false;
		CPY_FROM_STREAM(bClickable, Offset)
		pModel->clickable = bClickable;

		BYTE upAxis = 0; 
		CPY_FROM_STREAM(upAxis, Offset)
		//pModel->billboradUpAxis = upAxis;

		bool bIsAutoNormalise = false;
		CPY_FROM_STREAM(bIsAutoNormalise, Offset)
		pModel->isAutoNormalise = bIsAutoNormalise;

		bool bSkybox = false;
		CPY_FROM_STREAM(bSkybox, Offset)
		pModel->isSkyBox = bSkybox;

		// 骨骼信息
		int bindingSkeletonIndex  = -1;
		CPY_FROM_STREAM(bindingSkeletonIndex, Offset)
		
		m_vModel.push_back(pModel);

		delete []pVertextBuffer;
		delete []pIndexBuffer;
		pVertextBuffer = NULL;
		pIndexBuffer = NULL;
	}

	m_vMesh.clear();
	DELETE_ARRAYPTR(m_pEntityBuf);  // 删除字节流
	DELETE_ARRAYPTR(m_pMeshBuf);    // 删除字节流

	return ERR_COMMON_OK;
}
//----------------------------------------------------------
DWORD CGetV3DImpV3::ReadOverlayData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pOverlayBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}

 	char* Offset = m_pOverlayBuf;
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();
 
 	// 读取Overlay的数量
 	int iOverlayCnt = 0; 
 	CPY_FROM_STREAM(iOverlayCnt, Offset);

	// 读取Overlay属性并创建Overlay对象
	for (int iIndex = 0; iIndex < iOverlayCnt; iIndex++)
	{
 		V3D_OverlayInfoV3 overlayInfo;
		CPY_FROM_STREAM(overlayInfo, Offset);

		CGUIObject* pOverlay = GUIMngr.CreateOverLayer(overlayInfo.Name, "", "");
		if (NULL != pOverlay)
		{
			// 材质
			int iMtrlIndex = overlayInfo.materialIndex;
			CMaterial* pMtrl = _GetHandleByIndex(m_vMaterial, iMtrlIndex);
			pOverlay->SetMaterial(pMtrl);
			
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);

 			// 视口大小
 			int iViewWidth = 0;
			int iViewHight = 0;
			GUIMngr.GetViewportSize(iViewWidth, iViewHight);
	 
			// overlay的实际坐标及大小
 			float fLeft = 0.0f;
			float fTop = 0.0f;
			float fWidth = 0.0f;
			float fHeight = 0.0f;
			// 转换成绝对坐标
 			if (overlayInfo.bRelativePos)
 			{
 				fLeft = iViewWidth * overlayInfo.Left;
 				fTop  = iViewHight * overlayInfo.Top;
 			}
 			else
 			{
 				fLeft = overlayInfo.Left;
 				fTop  = overlayInfo.Top;
 			}
			// 转换成绝对尺寸
 			if (overlayInfo.bRelativeSize)  
 			{
 				fWidth  = iViewWidth * overlayInfo.Width;
 				fHeight = iViewHight * overlayInfo.Height;
 			}
 			else
 			{
 				fWidth  = overlayInfo.Width;
 				fHeight = overlayInfo.Height;
 			}
			pOverlay->SetOffsetOrigin(VGSOFFSETORIGIN::VOO_TOP_LEFT);	 //先设置为左上角便于设置位置
			pOverlay->SetUsedRelativePos(overlayInfo.bRelativePos);
			pOverlay->SetUsedRelativeSize(overlayInfo.bRelativeSize);
			pOverlay->SetSize(fWidth, fHeight);
			pOverlay->SetOffsetOrigin(VGSOFFSETORIGIN(overlayInfo.Origin));	//设置offset origin
			pOverlay->SetPosition(fLeft, fTop);
			pOverlay->SetOpacity(overlayInfo.Opacity);
			pOverlay->SetVisibility(overlayInfo.bVisible);
			//			
			for (int iFunc = 0; iFunc < VGS_MOUSEEVENT_CNT; iFunc++)
			{
				pOverlay->SetEventFuncName(VGS_MOUSEEVENT_TYPE(iFunc), overlayInfo.sMouseEventFunc[iFunc]);
			}
		}
	}
 
	return ERR_COMMON_OK;
}
//----------------------------------------------------------
DWORD CGetV3DImpV3::ReadNavigaterData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pNaviBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}
 
 	char* Offset = m_pNaviBuf; 
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

 	// 读取导航图的数量
 	int iNavCount = 0; 
 	CPY_FROM_STREAM(iNavCount, Offset);
 
	// 读取导航图属性并创建导航图对象
 	for (int iIndex = 0; iIndex < iNavCount; iIndex++)
 	{
		FRect mapRect;
		V3D_NavigatorInfoV3 naviInfo; 
		ZeroMemory(&mapRect, sizeof(FRect));
		ZeroMemory(&naviInfo, sizeof(V3D_NavigatorInfoV3));
 
		// 基本属性
 		CPY_FROM_STREAM(naviInfo.NaviBaseInfo, Offset); 
 
		// map导航区域	
 		CPY_FROM_STREAM(mapRect, Offset); 
 
 		// 视口大小
		int iViewWidth = 0;
		int iViewHight = 0;
		GUIMngr.GetViewportSize(iViewWidth, iViewHight);
 
		// 转换成绝对坐标
 		FRect rect;
 		if (naviInfo.NaviBaseInfo.bRelativePos)  
 		{
 			rect.Left = iViewWidth * naviInfo.NaviBaseInfo.Left;
 			rect.Top  = iViewHight * naviInfo.NaviBaseInfo.Top;
 		}
 		else
 		{
 			rect.Left = naviInfo.NaviBaseInfo.Left;
 			rect.Top  = naviInfo.NaviBaseInfo.Top;
 		} 
		// 转换成绝对坐标
 		if (naviInfo.NaviBaseInfo.bRelativeSize)  
 		{
 			rect.Right  = rect.Left + iViewWidth * naviInfo.NaviBaseInfo.Width;
 			rect.Bottom = rect.Top + iViewHight * naviInfo.NaviBaseInfo.Height;
 		}
 		else
 		{
 			rect.Right  = rect.Left + naviInfo.NaviBaseInfo.Width;
 			rect.Bottom = rect.Top +naviInfo.NaviBaseInfo.Height;
 		}		
 
		// 导航点个数
 		CPY_FROM_STREAM(naviInfo.PointerCount, Offset); 
		if (naviInfo.PointerCount > 0)
		{
			// 导航点信息
			naviInfo.PointerList = new V3D_NaviPointerInfoV3[naviInfo.PointerCount];
			ZeroMemory(naviInfo.PointerList, naviInfo.PointerCount*sizeof(V3D_NaviPointerInfoV3));
 			for (int iPointer = 0; iPointer < naviInfo.PointerCount; iPointer++)
 			{
				CPY_FROM_STREAM(naviInfo.PointerList[iPointer], Offset);
 			}
		}
 
 		// 创建导航图
		CNavigator* pNavigator = (CNavigator*)GUIMngr.CreateNavigator(naviInfo.NaviBaseInfo.Name, "");
		if (NULL != pNavigator)
		{
			int iMtrlIndex = naviInfo.NaviBaseInfo.MapMaterialIndex;
			CMaterial* pMtrl = _GetHandleByIndex(m_vMaterial, iMtrlIndex);
			pNavigator->SetMaterial(pMtrl);
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);
			//
			pNavigator->SetOffsetOrigin(VGSOFFSETORIGIN::VOO_TOP_LEFT);					// 先设置为左上角便于设置位置
			pNavigator->SetUsedRelativePos(naviInfo.NaviBaseInfo.bRelativePos);
			pNavigator->SetUsedRelativeSize(naviInfo.NaviBaseInfo.bRelativeSize);
			pNavigator->SetSize(rect.Right - rect.Left, rect.Bottom - rect.Top);
			pNavigator->SetOffsetOrigin(VGSOFFSETORIGIN(naviInfo.NaviBaseInfo.Origin));  // 设置offset origin
			pNavigator->SetPosition(rect.Left, rect.Top);
			pNavigator->SetOpacity(naviInfo.NaviBaseInfo.Opacity);
			pNavigator->SetVisibility(naviInfo.NaviBaseInfo.bVisible);
			//
			pNavigator->SetNavRange(mapRect);
			//
			for (int iFunc = 0; iFunc < VGS_MOUSEEVENT_CNT; iFunc++)
			{
				pNavigator->SetEventFuncName(VGS_MOUSEEVENT_TYPE(iFunc), naviInfo.sMouseEventFunc[iFunc]);
			}
			//
			for (int iPointer = 0; iPointer < naviInfo.PointerCount; iPointer++)
			{
				CNavPointer* pPointer = (CNavPointer*)GUIMngr.CreateNaviPointer(naviInfo.PointerList[iPointer].sName, "");
				if (NULL != pPointer)
				{					
					int iMtrlIndex = naviInfo.PointerList[iPointer].materialIndex;
					CMaterial* pMtrl = _GetHandleByIndex(m_vMaterial, iMtrlIndex);
					pPointer->SetMaterial(pMtrl);
					m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);
					//
 					DWORD dwPointedHandle = 0;
					if (0 == naviInfo.PointerList[iPointer].NavType &&
						naviInfo.PointerList[iPointer].PointedObjIndex >= 0 &&
						naviInfo.PointerList[iPointer].PointedObjIndex < m_vpCamera.size())	 // camera
					{
						dwPointedHandle = (DWORD)m_vpCamera[naviInfo.PointerList[iPointer].PointedObjIndex];
					}
					else if (1 == naviInfo.PointerList[iPointer].NavType &&
						naviInfo.PointerList[iPointer].PointedObjIndex >= 0 &&
						naviInfo.PointerList[iPointer].PointedObjIndex < m_vModel.size())		// 模型
					{
						dwPointedHandle = (DWORD)m_vModel[naviInfo.PointerList[iPointer].PointedObjIndex];
					}
					pPointer->SetPointerHandle(dwPointedHandle);
					pPointer->SetNavType(naviInfo.PointerList[iPointer].NavType);
					pPointer->SetVisibility(naviInfo.PointerList[iPointer].bVisible);
					pPointer->SetOpacity(naviInfo.PointerList[iPointer].Opacity);
					pPointer->SetSize(naviInfo.PointerList[iPointer].Width, naviInfo.PointerList[iPointer].Height);
					pPointer->SetPosition(rect.Left + naviInfo.PointerList[iPointer].Left, rect.Top + naviInfo.PointerList[iPointer].Top);
					//
					for (int iFunc = 0; iFunc < VGS_MOUSEEVENT_CNT; iFunc++)
					{
						pPointer->SetEventFuncName(VGS_MOUSEEVENT_TYPE(iFunc), naviInfo.PointerList[iPointer].sMouseEventFunc[iFunc]);
					}
					//
					pNavigator->AddNavPointer(pPointer);
				}
			}
		}
			
		// 释放内存
		if (NULL != naviInfo.PointerList)
		{
			delete []naviInfo.PointerList;
			naviInfo.PointerList = NULL;
		}
 	}

	return ERR_COMMON_OK;
}
//----------------------------------------------------------
DWORD CGetV3DImpV3::ReadButtonData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pButtonBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}
 
 	char* Offset = m_pButtonBuf; 
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

 	// 读取button的数量
 	int iBtnCount = 0; 
 	CPY_FROM_STREAM(iBtnCount, Offset);
 
	// 读取button属性并创建button对象
 	for (int iIndex = 0; iIndex < iBtnCount; iIndex++)
 	{
 		V3D_ButtonInfoV3 BtnInfo;
 		CPY_FROM_STREAM(BtnInfo, Offset); 

 		// 创建button
		RECT rcBtn;
		memset(&rcBtn, 0, sizeof(RECT));
		CButton* pButton = (CButton*)GUIMngr.CreateButton(BtnInfo.Name, rcBtn, "", "", "");
		if (NULL != pButton)
		{	
			int iMtrlIndex = BtnInfo.UpMaterialIndex;
			CMaterial* pMtrl = _GetHandleByIndex(m_vMaterial, iMtrlIndex);
			pButton->SetMaterial(0, pMtrl);			
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);
			//
			iMtrlIndex = BtnInfo.OverMaterialIndex;
			pMtrl = _GetHandleByIndex(m_vMaterial, iMtrlIndex);
			pButton->SetMaterial(1, pMtrl);
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);
			//
			iMtrlIndex = BtnInfo.DownMaterialIndex;
			pMtrl = _GetHandleByIndex(m_vMaterial, iMtrlIndex);
			pButton->SetMaterial(2, pMtrl);
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);

			// 视口大小
			int iViewWidth = 0;
			int iViewHight = 0;
			GUIMngr.GetViewportSize(iViewWidth, iViewHight);

 			float fLeft = 0.0f;
			float fTop = 0.0f;
			float fWidth = 0.0f;
			float fHeight = 0.0f; 

			// 转换成绝对坐标
 			if (BtnInfo.bRelativePos)  
 			{
 				fLeft = iViewWidth * BtnInfo.Left;
 				fTop  = iViewHight * BtnInfo.Top;
 			}
 			else
 			{
 				fLeft = BtnInfo.Left;
 				fTop  = BtnInfo.Top;
 			}
	 
			// 转换成绝对坐标
 			if (BtnInfo.bRelativeSize)  			
			{
 				fWidth  = iViewWidth * BtnInfo.Width;
 				fHeight = iViewHight * BtnInfo.Height;
 			}
 			else
 			{
 				fWidth  = BtnInfo.Width;
 				fHeight = BtnInfo.Height;
 			}
			//
			pButton->SetOffsetOrigin(VGSOFFSETORIGIN::VOO_TOP_LEFT);   // 先设置为左上角便于设置位置
			pButton->SetUsedRelativePos(BtnInfo.bRelativePos);
			pButton->SetUsedRelativeSize(BtnInfo.bRelativeSize);
			pButton->SetSize(fWidth, fHeight);
			pButton->SetOffsetOrigin(VGSOFFSETORIGIN(BtnInfo.Origin));  // 设置offset origin
			pButton->SetPosition(fLeft, fTop);
			pButton->SetOpacity(BtnInfo.Opacity);
			pButton->SetVisibility(BtnInfo.bVisible);
			pButton->SetJsCallbackFunc(BtnInfo.CallbackFunName);
			//
			for (int iFunc = 0; iFunc < VGS_MOUSEEVENT_CNT; iFunc++)
			{
				pButton->SetEventFuncName(VGS_MOUSEEVENT_TYPE(iFunc), BtnInfo.sMouseEventFunc[iFunc]);
			}
		} 		
 	}
	
	return ERR_COMMON_OK;
}
//----------------------------------------------------------
DWORD CGetV3DImpV3::ReadBackdropData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pBackdropBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}

 	char* Offset = m_pBackdropBuf; 
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

 	// 读取背景图片个数
 	int iBkgndCount = 0; 
 	CPY_FROM_STREAM(iBkgndCount, Offset);

	// 读取背景图片属性并创建背景图片对象
 	for (int iIndex = 0; iIndex < iBkgndCount; iIndex++)
 	{
 		V3D_BackdropInfoV3 bkgndInfo;
 		CPY_FROM_STREAM(bkgndInfo, Offset); 
		//
		BackGround* pBkgrnd = GUIMngr.CreateBackGroudLayer("");
		if (NULL != pBkgrnd)
		{
			int iMtrlIndex = bkgndInfo.materialIndex;
			CMaterial* pMtrl = _GetHandleByIndex(m_vMaterial, iMtrlIndex);
			pBkgrnd->SetMaterial(pMtrl);
			pBkgrnd->SetVisibility(bkgndInfo.bVisible);
			pBkgrnd->SetRect(bkgndInfo.Left, bkgndInfo.Top, bkgndInfo.Width, bkgndInfo.Height);
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);
		}
	}
 
	return ERR_COMMON_OK;
}
//----------------------------------------------------------
DWORD CGetV3DImpV3::ReadControlData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pVGSSceneMgr || !m_pControlBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}	
 
 	char* Offset = m_pControlBuf; 
		
 	// 读取总的控制器数目
 	DWORD TotalCount = 0;
 	CPY_FROM_STREAM(TotalCount, Offset)	
 	if (TotalCount == 0)
 	{
 		return ERR_COMMON_OK;
 	}
	
	V3D_ControlDataV3 controlInfo;
	ZeroMemory(&controlInfo, sizeof(V3D_ControlDataV3));
	CPY_FROM_STREAM(controlInfo.ControlType, Offset)	
	CPY_FROM_STREAM(controlInfo.ControlerIndex, Offset)	
	//
	CPY_FROM_STREAM(controlInfo.WalkerList.WalkerCnt, Offset)	
	if (controlInfo.WalkerList.WalkerCnt > 0)
	{
		controlInfo.WalkerList.Walkers = new V3D_WalkerV3[controlInfo.WalkerList.WalkerCnt];
		for (int iIndex = 0; iIndex < controlInfo.WalkerList.WalkerCnt; iIndex++)
		{
			CPY_FROM_STREAM(controlInfo.WalkerList.Walkers[iIndex], Offset);
			//			
 			WalkerInfo Info;
 			ZeroMemory(&Info, sizeof(Info));
			strcpy(Info.Name, controlInfo.WalkerList.Walkers[iIndex].Name);
			strcpy(Info.Camera, _GetNameByIndex(m_vpCamera, controlInfo.WalkerList.Walkers[iIndex].CameraIndex).c_str());
			Info.walkSpeed = controlInfo.WalkerList.Walkers[iIndex].walkSpeed;
			Info.walkAcc = controlInfo.WalkerList.Walkers[iIndex].walkAcc;
			Info.rosSpeed = controlInfo.WalkerList.Walkers[iIndex].rosSpeed;
			Info.rosSpeed = controlInfo.WalkerList.Walkers[iIndex].rosAcc;
			Info.rosSpeed = controlInfo.WalkerList.Walkers[iIndex].height;
			Info.bCollision = controlInfo.WalkerList.Walkers[iIndex].bCollision;
			Info.colDis = controlInfo.WalkerList.Walkers[iIndex].colDis;
			Info.bWeight = controlInfo.WalkerList.Walkers[iIndex].bWeight;
			Info.liftSpeed = controlInfo.WalkerList.Walkers[iIndex].liftSpeed;
			Info.liftAcc = controlInfo.WalkerList.Walkers[iIndex].liftAcc;
			Info.upperAngle = controlInfo.WalkerList.Walkers[iIndex].upperAngle;
			Info.lowerAngle = controlInfo.WalkerList.Walkers[iIndex].lowerAngle;
			m_pVGSSceneMgr->CreateWalker(Info);
		}
		delete []controlInfo.WalkerList.Walkers;
		controlInfo.WalkerList.Walkers = NULL;
	}
	//
	CPY_FROM_STREAM(controlInfo.FlyerList.FlyerCnt, Offset)	
	if (controlInfo.FlyerList.FlyerCnt > 0)
	{
		controlInfo.FlyerList.Flyers = new V3D_FlyerV3[controlInfo.FlyerList.FlyerCnt];
		for (int iIndex = 0; iIndex < controlInfo.FlyerList.FlyerCnt; iIndex++)
		{
			CPY_FROM_STREAM(controlInfo.FlyerList.Flyers[iIndex], Offset);
			//			
			FlyerInfo Info;
 			ZeroMemory(&Info, sizeof(Info));
			strcpy(Info.Name, controlInfo.FlyerList.Flyers[iIndex].Name);
			strcpy(Info.Camera, _GetNameByIndex(m_vpCamera, controlInfo.FlyerList.Flyers[iIndex].CameraIndex).c_str());
			Info.panSpeed = controlInfo.FlyerList.Flyers[iIndex].panSpeed;
			Info.panAcc = controlInfo.FlyerList.Flyers[iIndex].panAcc;
			Info.dollySpeed = controlInfo.FlyerList.Flyers[iIndex].dollySpeed;
			Info.dollyAcc = controlInfo.FlyerList.Flyers[iIndex].dollyAcc;
			Info.dollyFarDis = controlInfo.FlyerList.Flyers[iIndex].dollyFarDis;
			Info.dollyNearDis = controlInfo.FlyerList.Flyers[iIndex].dollyNearDis;
			Info.rotateSpeed = controlInfo.FlyerList.Flyers[iIndex].rotateSpeed;
			Info.rotateAcc = controlInfo.FlyerList.Flyers[iIndex].rotateAcc;
			Info.liftSpeed = controlInfo.FlyerList.Flyers[iIndex].liftSpeed;
			Info.liftAcc = controlInfo.FlyerList.Flyers[iIndex].liftAcc;
			Info.upperAngle = controlInfo.FlyerList.Flyers[iIndex].upperAngle;
			Info.lowerAngle = controlInfo.FlyerList.Flyers[iIndex].lowerAngle;
			Info.bCollision = controlInfo.FlyerList.Flyers[iIndex].bCollision;
			Info.colDis = controlInfo.FlyerList.Flyers[iIndex].colDis;
			//
			m_pVGSSceneMgr->CreateFlyer(Info);
		}
		delete []controlInfo.FlyerList.Flyers;
		controlInfo.FlyerList.Flyers = NULL;
	}
	//
	CPY_FROM_STREAM(controlInfo.EditorList.EditorCnt, Offset)	
	if (controlInfo.EditorList.EditorCnt > 0)
	{
		controlInfo.EditorList.Editors = new V3D_EditorV3[controlInfo.EditorList.EditorCnt];
		for (int iIndex = 0; iIndex < controlInfo.EditorList.EditorCnt; iIndex++)
		{
			CPY_FROM_STREAM(controlInfo.EditorList.Editors[iIndex], Offset);
			//
			EditorInfo Info;
 			ZeroMemory(&Info, sizeof(Info));
			strcpy(Info.Name, controlInfo.EditorList.Editors[iIndex].Name);
			strcpy(Info.Camera, _GetNameByIndex(m_vpCamera, controlInfo.EditorList.Editors[iIndex].CameraIndex).c_str());
			Info.bIsAutoRoundRoll = controlInfo.EditorList.Editors[iIndex].bIsAutoRoundRoll;
			Info.autoSpeedX = 0;
			Info.autoSpeedY = 0;
			Info.manualSpeedX = controlInfo.EditorList.Editors[iIndex].manualRoundSpeed;
			Info.manualPanXSpeed = controlInfo.EditorList.Editors[iIndex].manualPanXSpeed;
			Info.manualPanYSpeed = controlInfo.EditorList.Editors[iIndex].manualPanYSpeed;
			Info.dollySpeed = controlInfo.EditorList.Editors[iIndex].dollySpeed;
			Info.dollyAcc = controlInfo.EditorList.Editors[iIndex].dollyAcc;
			Info.dollyFarDis = controlInfo.EditorList.Editors[iIndex].dollyFarDis;
			Info.dollyNearDis = controlInfo.EditorList.Editors[iIndex].dollyNearDis;
			Info.LookAtModelIndex = 0;
			//			
 			m_pVGSSceneMgr->CreateEditor(Info);			
		}
		delete []controlInfo.EditorList.Editors;
		controlInfo.EditorList.Editors = NULL;
	}

 	m_pVGSSceneMgr->SetCurrentController(NULL); 
	if ((CONTROLTYPE)controlInfo.ControlType == CT_EDITOR)
 	{
		if (controlInfo.ControlerIndex >= 0 && controlInfo.ControlerIndex < m_pVGSSceneMgr->GetEditorCount())
 		{
 			m_pVGSSceneMgr->SetCurrentController(m_pVGSSceneMgr->GetEditor(controlInfo.ControlerIndex));
 			((CEditor*)(m_pVGSSceneMgr->GetEditor(controlInfo.ControlerIndex)))->SetCamera(m_pVGSSceneMgr->GetEditor(controlInfo.ControlerIndex)->GetCamera()); // 重设相机，让相机朝向目标点
 		}
 	}
 	else if ((CONTROLTYPE)controlInfo.ControlType == CT_WALKER)
 	{
 		if (controlInfo.ControlerIndex >= 0 && controlInfo.ControlerIndex < m_pVGSSceneMgr->GetWalkerCount())
 		{
 			m_pVGSSceneMgr->SetCurrentController(m_pVGSSceneMgr->GetWalker(controlInfo.ControlerIndex));
 		}
 	}
 	else if ((CONTROLTYPE)controlInfo.ControlType == CT_FLYER)
 	{
 		if (controlInfo.ControlerIndex >= 0 && controlInfo.ControlerIndex < m_pVGSSceneMgr->GetFlyerCount())
 		{
 			m_pVGSSceneMgr->SetCurrentController(m_pVGSSceneMgr->GetFlyer(controlInfo.ControlerIndex));
 		}
 	}
	
	return ERR_COMMON_OK;
}
//----------------------------------------------------------
//递归创建场景
//----------------------------------------------------------
void CGetV3DImpV3::ConstructSceneGraph(CSceneNode* pNode, char* offset)
{
	CSceneNode* pChildNode = NULL;
	bool	bHasSkeleton = false;
	VGS_Node node;
	string RealName;

	//读入node的类型
	CPY_FROM_STREAM(node.type, offset);
	
	//读入node的编号
	CPY_FROM_STREAM(node.index, offset);

	//读入node的名称
	CPY_FROM_STREAM(node.strLen, offset);
	node.name.reserve(node.strLen);
	memcpy((void*)(node.name.c_str()), offset, node.strLen);
	offset += node.strLen;

	//读入并设置缩放\转角\位置
	D3DXVECTOR3 scale;	
	CPY_VECTOR3_FROM_STRRAM(scale, offset);
	D3DXQUATERNION quat;
	CPY_FROM_STREAM(quat.x, offset);
	CPY_FROM_STREAM(quat.y, offset);
	CPY_FROM_STREAM(quat.z, offset);
	CPY_FROM_STREAM(quat.w, offset);
	D3DXVECTOR3 pos;
	CPY_VECTOR3_FROM_STRRAM(pos, offset);

	//读入该节点的动画数据
	ReadANodeAnimation(pNode, &node, offset);

	//Node的变换, 相机的变换不在这里处理---------------------------------------------------------
	//位置//转化坐标系
	if (node.type == SCENEROOT)
	{		
		pChildNode = pNode;
		pChildNode->setScale(&D3DXVECTOR3(scale.x, scale.y, scale.z), VTS_WORLD);
		pChildNode->setRotationByQuat(&quat, VTS_WORLD);
		pChildNode->setPosition(&D3DXVECTOR3(pos.x, pos.y, pos.z), VTS_WORLD);
	}
	else
	{
		pChildNode = pNode->CreateChild();
		if (pChildNode)
		{
			//缩放//转化坐标系
			pChildNode->setScale(&D3DXVECTOR3(scale.x, scale.y, scale.z), VTS_PARENT);
			//pChildNode->updateTransform();
			//旋转//转化坐标系
			pChildNode->setRotationByQuat(&quat, VTS_PARENT);
			//pChildNode->updateTransform();
			//位置
			pChildNode->setPosition(&D3DXVECTOR3(pos.x, pos.y, pos.z), VTS_PARENT);
			//pChildNode->updateTransform();
			
			if (node.type == CAMERANODE)
			{
				if (node.index >= 0 && node.index < (int)m_vpCamera.size())
				{				
					CCamera *pCamera = m_vpCamera[node.index];
					if (pCamera)
					{
						D3DXVECTOR3 v3Pos = pCamera->GetPosition(VTS_PARENT);
						D3DXVECTOR3 v3At  = pCamera->GetLookat(VTS_PARENT);
						pChildNode->attachObject(pCamera);
						pCamera->m_pNode = pChildNode;
						pCamera->SetPosition(&v3Pos, VTS_PARENT);
					}
				}
			}
			else if (node.type == LIGHTNODE)
			{			
				if (node.index >= 0 && node.index < (int)m_vpLight.size())
				{
					pChildNode->attachObject(m_vpLight[node.index]);
					m_vpLight[node.index]->m_pNode = pChildNode;
				}
			}
			else if (node.type == MESHNODE)
			{			
				if (node.index >= 0 && node.index < (int)m_vModel.size())
				{
					CModel *pModel = m_vModel[node.index];
					if (pModel)
					{
						pChildNode->attachObject(pModel);
						pModel->m_pNode = pChildNode;
						pModel->UpdateFromNode();
					}				
				}
			} // else if
		} // if pChildNode
			
	}  // node type

	m_Offset = offset - m_pSceneGraphBuf; // 重定位offset

	//读入子节点数量
	int childNodeCnt = 0;
	CPY_FROM_STREAM(childNodeCnt, offset);
	for (int i = 0; i< childNodeCnt; i++)
	{
		ConstructSceneGraph(pChildNode, m_pSceneGraphBuf + m_Offset);
	}
}
//----------------------------------------------------------
//读入该节点的动画数据
//----------------------------------------------------------
void CGetV3DImpV3::ReadANodeAnimation(CSceneNode* pSceneNode, VGS_Node* pNode, char* offset)
{
	if (NULL == pSceneNode || NULL == pNode)
		return;

	// 关键帧数目
	UINT keyFrameCnt = 0;
	CPY_FROM_STREAM(keyFrameCnt, offset); 

	CCamera *pCamera = NULL;
	CLight  *pLight  = NULL;
	CModel *pModel = NULL;
	switch(pNode->type)
	{
	case NNT_CAMERANODE:
		if (pNode->index >=0 && pNode->index < m_pVGSSceneMgr->getCameraCount())
		{
			pCamera = m_pVGSSceneMgr->GetCameraByIndex(pNode->index);
		}
		break;
	case NNT_LIGHTNODE:
		if (pNode->index >= 0 && pNode->index < m_pVGSSceneMgr->getLightCount())
		{
			pLight = m_pVGSSceneMgr->GetLightByIndex(pNode->index);
		}
		break;
	case NNT_MODEL:
		if (pNode->index >= 0 && pNode->index < m_pVGSSceneMgr->GetModelCount())
		{
			pModel = m_pVGSSceneMgr->GetModelByIndex(pNode->index);
		}
		break;
	}

	std::vector<CNodeTransform> vTransformKeyframe;
	std::vector<CVGS_CAMERA_ANIMINFO> vCamParamInfo;	// 相机参数动画
	std::vector<CVGS_LIGHT_INFO> vLightParamInfo;		// 灯光参数动画
	if (keyFrameCnt > 1)
	{
		vTransformKeyframe.resize(keyFrameCnt);
		if (pNode->type == NNT_CAMERANODE)
		{
			vCamParamInfo.resize(keyFrameCnt);
		}
		else if (pNode->type == NNT_LIGHTNODE)
		{
			vLightParamInfo.resize(keyFrameCnt);
		}

		// 读取每一帧数据
		for (UINT iFrame = 0; iFrame < keyFrameCnt; ++iFrame)
		{
			CNodeTransform &nodeTrans = vTransformKeyframe[iFrame];
			N3D_NodeTransform n3dTrans;
			CPY_FROM_STREAM(n3dTrans, offset);
			nodeTrans.time	   = n3dTrans.time;
			nodeTrans.position = n3dTrans.position;
			nodeTrans.quat     = n3dTrans.quat;
			nodeTrans.scale    = n3dTrans.scale;

			// 如果是相机或灯光节点，可能还有参数动画
			if (pNode->type == NNT_CAMERANODE)
			{
				bool bHasParamAnimation;
				CPY_FROM_STREAM(bHasParamAnimation, offset); // 是否包含参数动画
				if (bHasParamAnimation)
				{
					float fov, nearClip, farClip, roll;
					CPY_FROM_STREAM(fov, offset);
					CPY_FROM_STREAM(nearClip, offset);
					CPY_FROM_STREAM(farClip, offset);
					CPY_FROM_STREAM(roll, offset);
					vCamParamInfo[iFrame].fov = fov;
					vCamParamInfo[iFrame].nearClip = nearClip;
					vCamParamInfo[iFrame].farClip = farClip;
				}
			}
			else if (pNode->type == NNT_LIGHTNODE)
			{
				bool bHasParamAnimation;
				CPY_FROM_STREAM(bHasParamAnimation, offset); // 是否包含参数动画

				DWORD color;
				float range;
				CPY_FROM_STREAM(color, offset); 
				CPY_FROM_STREAM(range, offset); 
				vLightParamInfo[iFrame].color = color;
				vLightParamInfo[iFrame].range2 = range;
				vLightParamInfo[iFrame].multiply = 1.0f;
				if (pLight && pLight->getType() == VLT_DIR)
				{
					VECTOR3 v3Dir;
					CPY_FROM_STREAM(v3Dir.x, offset); 
					CPY_FROM_STREAM(v3Dir.y, offset); 
					CPY_FROM_STREAM(v3Dir.z, offset); 
					vLightParamInfo[iFrame].dir = v3Dir;
				}
				else if (pLight && pLight->getType() == VLT_SPOT)
				{
					VECTOR3 v3Dir;
					float theta, phi, fallof;
					CPY_FROM_STREAM(v3Dir.x, offset); 
					CPY_FROM_STREAM(v3Dir.y, offset); 
					CPY_FROM_STREAM(v3Dir.z, offset); 
					CPY_FROM_STREAM(theta, offset); 
					CPY_FROM_STREAM(phi, offset); 
					CPY_FROM_STREAM(fallof, offset); 
					vLightParamInfo[iFrame].dir = v3Dir;
					vLightParamInfo[iFrame].theta = theta;
					vLightParamInfo[iFrame].phi   = phi;
				}
			}
		}
	}
	//
	switch (pNode->type)
	{
	case NNT_SCENEROOT:
		break;
	case NNT_CAMERANODE:
		{
			// 创建相机关键帧动画
			if (NULL != pCamera)
			{
				pSceneNode->attachObject(pCamera);
				m_pRoot->GetAnimManager().CreateCameraAnimation(pSceneNode, vTransformKeyframe, vCamParamInfo);
			}
		} 
		break;
	case NNT_LIGHTNODE:
		{
			// 创建灯光动画
			if (NULL != pLight)
			{
				pSceneNode->attachObject(pLight);
				pSceneNode->name = std::string(pLight->getName());

				//更新灯光数据，设置灯光的位置
				pLight->param.Position = pLight->m_pNode->getPosition(VTS_WORLD);
				m_pVGSSceneMgr->m_pd3dDevice->SetLight(pNode->index, &(pLight->param));		
				m_pVGSSceneMgr->m_pd3dDevice->LightEnable(pNode->index, TRUE);

				m_pRoot->GetAnimManager().CreateLightAnimation(pSceneNode, vTransformKeyframe, pLight->getType(), vLightParamInfo);
			}
		}
		break;
	case NNT_MODEL:
		{
			// 创建模型关键帧动画
			if (NULL != pModel)
			{
				pSceneNode->attachObject(pModel);
				m_pRoot->GetAnimManager().CreateModelAnimation(pSceneNode, vTransformKeyframe);
			}
		}
		break;
	}
}
//----------------------------------------------------------