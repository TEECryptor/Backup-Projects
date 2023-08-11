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
// ��ȡ��������
//----------------------------------------------------------
 DWORD CGetV3DImpV3::ReadAllData()
 {	
	fseek(m_pFile, m_V3DTable.PrivacyDataPtr, SEEK_SET); // �ƶ�����ʼλ��

	// ��Ȩ��Ϣ
	UINT PrivacyDataSize = m_V3DTable.cameraDataPtr - m_V3DTable.PrivacyDataPtr;
	m_pPrivacyBuf = new char[PrivacyDataSize];
	if (1 != fread(m_pPrivacyBuf, PrivacyDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// �������
	UINT CameDataSize = m_V3DTable.lightDataPtr - m_V3DTable.cameraDataPtr;
	m_pCameraBuf = new char[CameDataSize];
	if (1 != fread(m_pCameraBuf, CameDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// �ƹ�
	UINT LightDataSize = m_V3DTable.texturePtr - m_V3DTable.lightDataPtr;
	m_pLightBuf = new char[LightDataSize];
	if (1 != fread(m_pLightBuf, LightDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ��ͼ��������
	UINT TextureDataSize = m_V3DTable.mtrlDataPtr - m_V3DTable.texturePtr;
	m_pTextureBuf = new char[TextureDataSize];
	if (1 != fread(m_pTextureBuf, TextureDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ��������
	UINT MtrDataSize = m_V3DTable.meshDataPtr - m_V3DTable.mtrlDataPtr;	
	m_pMaterailBuf = new char[MtrDataSize];
	if (1 != fread(m_pMaterailBuf, MtrDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// entity(Model)
	UINT EntityDataSize = m_V3DTable.TextDataPtr - m_V3DTable.meshDataPtr;
	m_pEntityBuf = new char[EntityDataSize];
	if (1 != fread(m_pEntityBuf, EntityDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ����
	UINT TextDataSize = m_V3DTable.OverlayPtr - m_V3DTable.TextDataPtr;
	m_pTextBuf = new char[TextDataSize];
	if (1 != fread(m_pTextBuf, TextDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// overlay
	UINT OverlayDataSize = m_V3DTable.NavigatotPtr - m_V3DTable.OverlayPtr;
	m_pOverlayBuf = new char[OverlayDataSize];
	if (1 != fread(m_pOverlayBuf, OverlayDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ����ͼ
	UINT NaviSize = m_V3DTable.ButtonPtr - m_V3DTable.NavigatotPtr;
	m_pNaviBuf = new char[NaviSize];
	int ReadSize = fread(m_pNaviBuf, NaviSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ��ť
	UINT ButtonSize = m_V3DTable.BackdropPtr - m_V3DTable.ButtonPtr;
	m_pButtonBuf = new char[ButtonSize];
	ReadSize = fread(m_pButtonBuf, ButtonSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ����ͼ
	UINT BackdropSize = m_V3DTable.AudioPtr - m_V3DTable.BackdropPtr;
	m_pBackdropBuf = new char[BackdropSize];
	ReadSize = fread(m_pBackdropBuf, BackdropSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ����
	UINT AudioSize = m_V3DTable.FlashPtr - m_V3DTable.AudioPtr;
	m_pAudioBuf = new char[AudioSize];
	ReadSize = fread(m_pAudioBuf, AudioSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// flash
	UINT FlashDataSize = m_V3DTable.AnimationPtr - m_V3DTable.FlashPtr;
	m_pFlashBuf = new char[FlashDataSize];
	ReadSize = fread(m_pFlashBuf, FlashDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ����
	UINT AnimationDataSize = m_V3DTable.ControlPtr - m_V3DTable.AnimationPtr;
	m_pAnimationBuf = new char[AnimationDataSize];
	ReadSize = fread(m_pAnimationBuf, AnimationDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ������Ϣ
	UINT ControlDataSize = m_V3DTable.sceneGraphDataPtr - m_V3DTable.ControlPtr;
	m_pControlBuf = new char[ControlDataSize];
	fseek(m_pFile, m_V3DTable.ControlPtr, SEEK_SET);
	ReadSize = fread(m_pControlBuf, ControlDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ����ͼ
	fseek(m_pFile, 0, SEEK_END); // �ļ���β
	UINT FileSize = ftell(m_pFile);
	UINT DataSize = FileSize - m_V3DTable.sceneGraphDataPtr;
	m_pSceneGraphBuf = new char[DataSize];
	fseek(m_pFile, m_V3DTable.sceneGraphDataPtr, SEEK_SET);
	ReadSize = fread(m_pSceneGraphBuf, DataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	return ERR_COMMON_OK;
 }
//----------------------------------------------------------
 DWORD CGetV3DImpV3::ReadMaterialData()
{
	if (!m_pFile)
	{
		// throw Exception::exception("���ļ���������(03)���޷���ȡ�ļ�");
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pMaterailBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	m_vMaterial.clear();

	// ��ȡ������Ϣ
	char* Offset = m_pMaterailBuf;          // �ֽ��������ݿ�ʼ��λ��
	DWORD MtrCount = 0;                     // ������Ŀ
	CPY_FROM_STREAM(MtrCount, Offset)       // ��ȡ������Ŀ
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
			// ����������Դ��ָ����Դ��, Ogre���Զ�����һ��technique��һ��pass
			// pMtrl = MaterialManager::getSingleton().create(pName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			MessageBox(NULL, TEXT("������û��Ϊ����ָ�����ƣ����ܻᵼ�����⣡"), TEXT("����"), 0);
			continue;
		}

		//����������Դ��ָ����Դ��
		CMaterial *pMtrl = m_pVGSSceneMgr->CreateMaterial(sMtlName);
		if (!pMtrl)
		{
			continue;
		}

		//pass��ϸ����
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
			color.a = (PassDetail.Dif_ARGB & 0x000000ff) / 255.0f; // ��ʾ��rgba��ʽ
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
		pMtrl->setOpacity((float)(pMtrl->d3dmtrl.Diffuse.a * 100/*/ 2.55f*/ + 0.5));  // ��[0, 255]�����[0, 100]

		// ������ͼ����Ŀ
		UINT texLayerCount = 0;
		CPY_FROM_STREAM(texLayerCount, Offset);

		// ÿ����ͼ��
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

			// �������
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

			// ������
			VGS_TextureType texType = TLT_INVALID;
			CPY_FROM_STREAM(texType, Offset);
			pLayer->type = (VGS_TextureType)texType;

			// ֡��
			UINT FrameCount = 0;
			CPY_FROM_STREAM(FrameCount, Offset);

			// ͼ���֡�����Լ�ÿ֡��ͼ�����ƣ�������·��
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
			
			// ������ͼ��					
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
			case TLT_RTFMAP: // ʵʱ����
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
			case TLT_RTTMAP:  // ʵʱ����
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

			// ��ɫ�Ļ�ɫģʽ
			VGS_BlenderInfo colorBlendInfo;
			CPY_FROM_STREAM(colorBlendInfo, Offset);  
			pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(colorBlendInfo.option);
			pLayer->Blenderinfo.ColorBlendParam  = D3DCOLOR_COLORVALUE(0, 0, 0, colorBlendInfo.param.BlenderOP_alpha);					

			// alpahͨ���Ļ�ɫģʽ
			VGS_BlenderInfo alphaBlendInfo;
			CPY_FROM_STREAM(alphaBlendInfo, Offset);  
			pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(alphaBlendInfo.option);
			pLayer->Blenderinfo.AlphaBlendParam  = D3DCOLOR_COLORVALUE(0, 0, 0, alphaBlendInfo.param.BlenderOP_alpha);

			pMtrl->m_vpTexLayers.push_back(pLayer);
		}

		m_vMaterial.push_back(pMtrl);

	} // Material 

	DELETE_ARRAYPTR(m_pMaterailBuf);  // ɾ���ֽ���

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

	// �ֽ��������ݿ�ʼ��λ��
	char* Offset = m_pCameraBuf;   
	
	m_vpCamera.clear();  // �������б�

	//�����������
	UINT cameraCnt = 0;                         
	CPY_FROM_STREAM(cameraCnt, Offset)       

	//��������������
	for (UINT i = 0; i < cameraCnt; i++)
	{
		//�������
		UINT len = 0;
		CPY_FROM_STREAM(len, Offset); 
		char *pName = new char[len + 1];
		ZeroMemory(pName, len + 1);
		CPY_PTR_FROM_STREAM(pName, len, Offset);
		std::string sName = std::string(pName);
		delete pName;
		pName = NULL;

		// �������
		CCamera *pCamera = m_pVGSSceneMgr->CreateCamera(sName);
		if (!pCamera)
		{
			return ERR_SCENE_CREATE;
		}

		// ����Ļ�������
		VGS_V3D_CAMINFO_V3 camInfo;
		CPY_FROM_STREAM(camInfo.isOrtho, Offset);
		CPY_FROM_STREAM(camInfo.fov, Offset);
		CPY_FROM_STREAM(camInfo.nearClipDis, Offset);
		CPY_FROM_STREAM(camInfo.farClipDis, Offset);
		CPY_FROM_STREAM(camInfo.position, Offset);
		CPY_FROM_STREAM(camInfo.at, Offset);
		CPY_FROM_STREAM(camInfo.up, Offset);

		// ͶӰ����
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

		m_vpCamera.push_back(pCamera);   // ����������������ܻ�ʹ��
	}

	DELETE_ARRAYPTR(m_pCameraBuf);  // ɾ���ֽ���

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

	//��ȡ�ƹ������
	char* Offset = m_pLightBuf; 
	int LightCount = 0; 
	CPY_FROM_STREAM(LightCount, Offset);

	m_vpLight.clear();
	
	//����ƹ��ȡ����
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

		//�ƹ�����
		VGS_LIGHT_TYPE type;
		CPY_FROM_STREAM(type, Offset)

		switch (type)
		{
		case VLT_OMNI:
			{
				//������Դ��Ϣ
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
				
				//�����⣬����������ģ��ȫ�ֹ�
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb �Ѳ���0,05��Ϊ��0.0f
				pLight->param.Ambient.a = 1.0f;
				
				//�߹�
				pLight->param.Specular   = omni.multiply * pLight->diffuse;  // fuxb 2007-10-16 ��3ds max������ͬ�Ĵ���ʽ
				pLight->param.Specular.a = 1.0f;
				
				//���ߵ������Զ����
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
				//������Դ��Ϣ
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
				
				//�����⣬����������ģ��ȫ�ֹ�
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb �Ѳ���0,05��Ϊ��0.0f
				pLight->param.Ambient.a = 1.0f;
				
				//�߹�
				pLight->param.Specular   = directionLight.multiply * pLight->diffuse;  // fuxb 2007-10-16 ��3ds max������ͬ�Ĵ���ʽ
				pLight->param.Specular.a = 1.0f;
				
				//���ߵ������Զ����
				pLight->param.Range        = directionLight.attenuationRange;
				pLight->param.Attenuation0 = directionLight.attenuation0;
				pLight->param.Attenuation1 = directionLight.attenuation1;
				pLight->param.Attenuation2 = directionLight.attenuation2;
				
				pLight->param.Type = D3DLIGHT_DIRECTIONAL;

				//����
				D3DXVECTOR3 dir(directionLight.direction.x, directionLight.direction.y, directionLight.direction.z);
				pLight->setDirection(&dir);
			}
			break;

		case VLT_SPOT:
			{
				//������Դ��Ϣ
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
				
				//�����⣬����������ģ��ȫ�ֹ�
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb �Ѳ���0,05��Ϊ��0.0f
				pLight->param.Ambient.a = 1.0f;
				
				//�߹�
				pLight->param.Specular   = spotLight.multiply * pLight->diffuse;  // fuxb 2007-10-16 ��3ds max������ͬ�Ĵ���ʽ
				pLight->param.Specular.a = 1.0f;
				
				//���ߵ������Զ����
				pLight->param.Range        = spotLight.attenuationRange;
				pLight->param.Attenuation0 = spotLight.attenuation0;
				pLight->param.Attenuation1 = spotLight.attenuation1;
				pLight->param.Attenuation2 = spotLight.attenuation2;
				
				pLight->param.Type = D3DLIGHT_SPOT;
				
				//����
				D3DXVECTOR3 dir(spotLight.direction.x, spotLight.direction.y, spotLight.direction.z);
				pLight->setDirection(&dir);

				//�Ž�
				pLight->param.Theta = spotLight.innerAngle;
				pLight->param.Phi = spotLight.outerAngle;
				pLight->param.Falloff = spotLight.fallOff;

				// λ��
				D3DXVECTOR3 vPos(spotLight.position.x, spotLight.position.y, spotLight.position.z);  
				pLight->setPosition(&vPos);
			}
			break;
		}

		m_vpLight.push_back(pLight);  // �����Լ����صĵƹ�
	}

	DELETE_ARRAYPTR(m_pLightBuf);  // ɾ���ֽ���

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
		// throw Exception::exception("���ļ���������(03)���޷���ȡ�ļ�");
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pEntityBuf)
	{
		return ERR_FILE_V3D_NODATA; 
	}

	// ��ȡentity��Ϣ
	char* Offset = m_pEntityBuf;              // �ֽ��������ݿ�ʼ��λ��
	DWORD EntityCount;                        // entity��Ŀ
	CPY_FROM_STREAM(EntityCount, Offset)      // ��ȡentity��Ŀ
	
	// ��ȡÿ��ģ��
	for (UINT idx = 0; idx < EntityCount; ++idx)
	{
		int namelen = 0;
		char sName[MAX_FILENAME_LENGTH];
		ZeroMemory(sName, MAX_FILENAME_LENGTH);
		CPY_FROM_STREAM(namelen, Offset)      // entity����		
		memcpy(sName, Offset, namelen);       // ��ȡ����
		Offset += namelen;

		CModel *pModel = m_pVGSSceneMgr->CreateModel(std::string(sName));

		// ģ�͵Ķ����ʽ
		DWORD fvf = 0;
		CPY_FROM_STREAM(fvf, Offset)
		pModel->m_FVF = fvf;
		pModel->m_FVFSize = D3DXGetFVFVertexSize(pModel->m_FVF);
		
		// �������
		UINT vertexCount = 0; 
		CPY_FROM_STREAM(vertexCount, Offset)
		pModel->m_vertexCount = vertexCount;

		// mesh �Ķ�����Ϣ
		BYTE *pVertextBuffer = new BYTE[vertexCount * pModel->m_FVFSize];
		memset(pVertextBuffer, 0, vertexCount * pModel->m_FVFSize);
		CPY_PTR_FROM_STREAM(pVertextBuffer, vertexCount * pModel->m_FVFSize, Offset);
	
		// �����
		UINT faceCount = 0;
		CPY_FROM_STREAM(faceCount, Offset)
		pModel->m_faceCount = faceCount;

		// �涥��index buffer����		
		UINT indexSize = pModel->m_faceCount * 3 * sizeof(UINT); 
		BYTE *pIndexBuffer = new BYTE[indexSize];
		CPY_PTR_FROM_STREAM(pIndexBuffer, indexSize, Offset);

		// submesh ��Ŀ
		unsigned short SubMeshCount = 0;
		CPY_FROM_STREAM(SubMeshCount, Offset)
		
		//  ���ÿ��submesh����Ϣ
		vector<CSubModel> vSubModel;
		for (UINT iSubMesh = 0; iSubMesh < SubMeshCount; ++iSubMesh)
		{
			CSubModel subModel;

			// submeshʹ�õĲ�������
			int indexMtrl = 0;
			CPY_FROM_STREAM(indexMtrl, Offset)
			subModel.m_pMaterial = _GetHandleByIndex(m_vMaterial, indexMtrl);
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_3D, subModel.m_pMaterial);

			// ÿ����ģ�͵Ķ������С���������
			CPY_FROM_STREAM(subModel.m_vertexStart, Offset)
			CPY_FROM_STREAM(subModel.m_vertexCount, Offset)
			CPY_FROM_STREAM(subModel.m_faceStart, Offset)
			CPY_FROM_STREAM(subModel.m_faceCount, Offset)
			
			vSubModel.push_back(subModel);
		} 

		// ����ģ��
		HRESULT hr = pModel->Create(fvf, vertexCount, faceCount, pVertextBuffer, pIndexBuffer, vSubModel);

		// ģ�͵�������Ϣ
		bool bBillBoard = false;
		CPY_FROM_STREAM(bBillBoard, Offset)
		pModel->isBillboard = bBillBoard;
		if (pModel->isBillboard)
		{
			// ���������������
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

		// ������Ϣ
		int bindingSkeletonIndex  = -1;
		CPY_FROM_STREAM(bindingSkeletonIndex, Offset)
		
		m_vModel.push_back(pModel);

		delete []pVertextBuffer;
		delete []pIndexBuffer;
		pVertextBuffer = NULL;
		pIndexBuffer = NULL;
	}

	m_vMesh.clear();
	DELETE_ARRAYPTR(m_pEntityBuf);  // ɾ���ֽ���
	DELETE_ARRAYPTR(m_pMeshBuf);    // ɾ���ֽ���

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
 
 	// ��ȡOverlay������
 	int iOverlayCnt = 0; 
 	CPY_FROM_STREAM(iOverlayCnt, Offset);

	// ��ȡOverlay���Բ�����Overlay����
	for (int iIndex = 0; iIndex < iOverlayCnt; iIndex++)
	{
 		V3D_OverlayInfoV3 overlayInfo;
		CPY_FROM_STREAM(overlayInfo, Offset);

		CGUIObject* pOverlay = GUIMngr.CreateOverLayer(overlayInfo.Name, "", "");
		if (NULL != pOverlay)
		{
			// ����
			int iMtrlIndex = overlayInfo.materialIndex;
			CMaterial* pMtrl = _GetHandleByIndex(m_vMaterial, iMtrlIndex);
			pOverlay->SetMaterial(pMtrl);
			
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);

 			// �ӿڴ�С
 			int iViewWidth = 0;
			int iViewHight = 0;
			GUIMngr.GetViewportSize(iViewWidth, iViewHight);
	 
			// overlay��ʵ�����꼰��С
 			float fLeft = 0.0f;
			float fTop = 0.0f;
			float fWidth = 0.0f;
			float fHeight = 0.0f;
			// ת���ɾ�������
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
			// ת���ɾ��Գߴ�
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
			pOverlay->SetOffsetOrigin(VGSOFFSETORIGIN::VOO_TOP_LEFT);	 //������Ϊ���ϽǱ�������λ��
			pOverlay->SetUsedRelativePos(overlayInfo.bRelativePos);
			pOverlay->SetUsedRelativeSize(overlayInfo.bRelativeSize);
			pOverlay->SetSize(fWidth, fHeight);
			pOverlay->SetOffsetOrigin(VGSOFFSETORIGIN(overlayInfo.Origin));	//����offset origin
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

 	// ��ȡ����ͼ������
 	int iNavCount = 0; 
 	CPY_FROM_STREAM(iNavCount, Offset);
 
	// ��ȡ����ͼ���Բ���������ͼ����
 	for (int iIndex = 0; iIndex < iNavCount; iIndex++)
 	{
		FRect mapRect;
		V3D_NavigatorInfoV3 naviInfo; 
		ZeroMemory(&mapRect, sizeof(FRect));
		ZeroMemory(&naviInfo, sizeof(V3D_NavigatorInfoV3));
 
		// ��������
 		CPY_FROM_STREAM(naviInfo.NaviBaseInfo, Offset); 
 
		// map��������	
 		CPY_FROM_STREAM(mapRect, Offset); 
 
 		// �ӿڴ�С
		int iViewWidth = 0;
		int iViewHight = 0;
		GUIMngr.GetViewportSize(iViewWidth, iViewHight);
 
		// ת���ɾ�������
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
		// ת���ɾ�������
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
 
		// ���������
 		CPY_FROM_STREAM(naviInfo.PointerCount, Offset); 
		if (naviInfo.PointerCount > 0)
		{
			// ��������Ϣ
			naviInfo.PointerList = new V3D_NaviPointerInfoV3[naviInfo.PointerCount];
			ZeroMemory(naviInfo.PointerList, naviInfo.PointerCount*sizeof(V3D_NaviPointerInfoV3));
 			for (int iPointer = 0; iPointer < naviInfo.PointerCount; iPointer++)
 			{
				CPY_FROM_STREAM(naviInfo.PointerList[iPointer], Offset);
 			}
		}
 
 		// ��������ͼ
		CNavigator* pNavigator = (CNavigator*)GUIMngr.CreateNavigator(naviInfo.NaviBaseInfo.Name, "");
		if (NULL != pNavigator)
		{
			int iMtrlIndex = naviInfo.NaviBaseInfo.MapMaterialIndex;
			CMaterial* pMtrl = _GetHandleByIndex(m_vMaterial, iMtrlIndex);
			pNavigator->SetMaterial(pMtrl);
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);
			//
			pNavigator->SetOffsetOrigin(VGSOFFSETORIGIN::VOO_TOP_LEFT);					// ������Ϊ���ϽǱ�������λ��
			pNavigator->SetUsedRelativePos(naviInfo.NaviBaseInfo.bRelativePos);
			pNavigator->SetUsedRelativeSize(naviInfo.NaviBaseInfo.bRelativeSize);
			pNavigator->SetSize(rect.Right - rect.Left, rect.Bottom - rect.Top);
			pNavigator->SetOffsetOrigin(VGSOFFSETORIGIN(naviInfo.NaviBaseInfo.Origin));  // ����offset origin
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
						naviInfo.PointerList[iPointer].PointedObjIndex < m_vModel.size())		// ģ��
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
			
		// �ͷ��ڴ�
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

 	// ��ȡbutton������
 	int iBtnCount = 0; 
 	CPY_FROM_STREAM(iBtnCount, Offset);
 
	// ��ȡbutton���Բ�����button����
 	for (int iIndex = 0; iIndex < iBtnCount; iIndex++)
 	{
 		V3D_ButtonInfoV3 BtnInfo;
 		CPY_FROM_STREAM(BtnInfo, Offset); 

 		// ����button
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

			// �ӿڴ�С
			int iViewWidth = 0;
			int iViewHight = 0;
			GUIMngr.GetViewportSize(iViewWidth, iViewHight);

 			float fLeft = 0.0f;
			float fTop = 0.0f;
			float fWidth = 0.0f;
			float fHeight = 0.0f; 

			// ת���ɾ�������
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
	 
			// ת���ɾ�������
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
			pButton->SetOffsetOrigin(VGSOFFSETORIGIN::VOO_TOP_LEFT);   // ������Ϊ���ϽǱ�������λ��
			pButton->SetUsedRelativePos(BtnInfo.bRelativePos);
			pButton->SetUsedRelativeSize(BtnInfo.bRelativeSize);
			pButton->SetSize(fWidth, fHeight);
			pButton->SetOffsetOrigin(VGSOFFSETORIGIN(BtnInfo.Origin));  // ����offset origin
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

 	// ��ȡ����ͼƬ����
 	int iBkgndCount = 0; 
 	CPY_FROM_STREAM(iBkgndCount, Offset);

	// ��ȡ����ͼƬ���Բ���������ͼƬ����
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
		
 	// ��ȡ�ܵĿ�������Ŀ
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
 			((CEditor*)(m_pVGSSceneMgr->GetEditor(controlInfo.ControlerIndex)))->SetCamera(m_pVGSSceneMgr->GetEditor(controlInfo.ControlerIndex)->GetCamera()); // ������������������Ŀ���
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
//�ݹ鴴������
//----------------------------------------------------------
void CGetV3DImpV3::ConstructSceneGraph(CSceneNode* pNode, char* offset)
{
	CSceneNode* pChildNode = NULL;
	bool	bHasSkeleton = false;
	VGS_Node node;
	string RealName;

	//����node������
	CPY_FROM_STREAM(node.type, offset);
	
	//����node�ı��
	CPY_FROM_STREAM(node.index, offset);

	//����node������
	CPY_FROM_STREAM(node.strLen, offset);
	node.name.reserve(node.strLen);
	memcpy((void*)(node.name.c_str()), offset, node.strLen);
	offset += node.strLen;

	//���벢��������\ת��\λ��
	D3DXVECTOR3 scale;	
	CPY_VECTOR3_FROM_STRRAM(scale, offset);
	D3DXQUATERNION quat;
	CPY_FROM_STREAM(quat.x, offset);
	CPY_FROM_STREAM(quat.y, offset);
	CPY_FROM_STREAM(quat.z, offset);
	CPY_FROM_STREAM(quat.w, offset);
	D3DXVECTOR3 pos;
	CPY_VECTOR3_FROM_STRRAM(pos, offset);

	//����ýڵ�Ķ�������
	ReadANodeAnimation(pNode, &node, offset);

	//Node�ı任, ����ı任�������ﴦ��---------------------------------------------------------
	//λ��//ת������ϵ
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
			//����//ת������ϵ
			pChildNode->setScale(&D3DXVECTOR3(scale.x, scale.y, scale.z), VTS_PARENT);
			//pChildNode->updateTransform();
			//��ת//ת������ϵ
			pChildNode->setRotationByQuat(&quat, VTS_PARENT);
			//pChildNode->updateTransform();
			//λ��
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

	m_Offset = offset - m_pSceneGraphBuf; // �ض�λoffset

	//�����ӽڵ�����
	int childNodeCnt = 0;
	CPY_FROM_STREAM(childNodeCnt, offset);
	for (int i = 0; i< childNodeCnt; i++)
	{
		ConstructSceneGraph(pChildNode, m_pSceneGraphBuf + m_Offset);
	}
}
//----------------------------------------------------------
//����ýڵ�Ķ�������
//----------------------------------------------------------
void CGetV3DImpV3::ReadANodeAnimation(CSceneNode* pSceneNode, VGS_Node* pNode, char* offset)
{
	if (NULL == pSceneNode || NULL == pNode)
		return;

	// �ؼ�֡��Ŀ
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
	std::vector<CVGS_CAMERA_ANIMINFO> vCamParamInfo;	// �����������
	std::vector<CVGS_LIGHT_INFO> vLightParamInfo;		// �ƹ��������
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

		// ��ȡÿһ֡����
		for (UINT iFrame = 0; iFrame < keyFrameCnt; ++iFrame)
		{
			CNodeTransform &nodeTrans = vTransformKeyframe[iFrame];
			N3D_NodeTransform n3dTrans;
			CPY_FROM_STREAM(n3dTrans, offset);
			nodeTrans.time	   = n3dTrans.time;
			nodeTrans.position = n3dTrans.position;
			nodeTrans.quat     = n3dTrans.quat;
			nodeTrans.scale    = n3dTrans.scale;

			// ����������ƹ�ڵ㣬���ܻ��в�������
			if (pNode->type == NNT_CAMERANODE)
			{
				bool bHasParamAnimation;
				CPY_FROM_STREAM(bHasParamAnimation, offset); // �Ƿ������������
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
				CPY_FROM_STREAM(bHasParamAnimation, offset); // �Ƿ������������

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
			// ��������ؼ�֡����
			if (NULL != pCamera)
			{
				pSceneNode->attachObject(pCamera);
				m_pRoot->GetAnimManager().CreateCameraAnimation(pSceneNode, vTransformKeyframe, vCamParamInfo);
			}
		} 
		break;
	case NNT_LIGHTNODE:
		{
			// �����ƹ⶯��
			if (NULL != pLight)
			{
				pSceneNode->attachObject(pLight);
				pSceneNode->name = std::string(pLight->getName());

				//���µƹ����ݣ����õƹ��λ��
				pLight->param.Position = pLight->m_pNode->getPosition(VTS_WORLD);
				m_pVGSSceneMgr->m_pd3dDevice->SetLight(pNode->index, &(pLight->param));		
				m_pVGSSceneMgr->m_pd3dDevice->LightEnable(pNode->index, TRUE);

				m_pRoot->GetAnimManager().CreateLightAnimation(pSceneNode, vTransformKeyframe, pLight->getType(), vLightParamInfo);
			}
		}
		break;
	case NNT_MODEL:
		{
			// ����ģ�͹ؼ�֡����
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