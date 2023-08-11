

#include "N3DLoaderImpV2.h"
#include <algorithm>
#include "..\\VGSErrCode.h"
#include "..\\GUI\\COverlay.h"
#include "..\\GUI\\CNavigator.h"
#include "..\\GUI\\CButton.h"
#include "..\\Root.h"

#include "..\\VGSDataStruct.h"
#include "..\VGSSupport.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CN3DLoaderImpV2::CN3DLoaderImpV2()
{
	m_pSceneResource = NULL;
	// pDebugFile = NULL;
	
	N3DBufSize = 0;
	N3DBuffer = NULL;
	
	//E3DBufSize = 0;						//E3D文件的尺寸
	//E3DBuffer = NULL;					//E3D文件的数据
	
	m_pSceneData = NULL;
	
	m_preModelCnt = 0;		
	m_preModelGroupCnt = 0;
	m_ImageIndex = 0;
	
	//m_preMtrlCnt = 0;		
	m_preTexCnt = 0;		
	m_preCubeMapCnt = 0;	
	m_preBumpWaterCnt = 0;
	m_preDyTexCnt = 0;
	m_preMovieTexCnt = 0;
	
	m_preOverlayCnt = 0;	
	m_preButtonCnt = 0;		
	m_preNavigaterCnt = 0;
	
	m_preCameraCnt = 0;	
	m_preLightCnt = 0;
	CameraData.camera = NULL;		
	LightData.light= NULL;			
	//VertexData.Vertex = NULL;		
	//VertexPosData.PosList = NULL;	
	//VertexNormalData.NormalList = NULL;  
	//ModelData.mesh = NULL;
	//UVData.UVList = NULL;			
	// ModelData.model = NULL;		
	MaterialData.mtrl = NULL;	
	TextData.Texts = NULL;		
	OverlayData.Overlays = NULL;	
	NavigaterData.Navigaters = NULL;	
	ButtonData.Buttons = NULL;

	m_pImageData = NULL;

	m_minVersion = 50000;
	m_maxVersion = 51999;

}

CN3DLoaderImpV2::~CN3DLoaderImpV2()
{
	
}

VOID CN3DLoaderImpV2::ReadHeader()
{
	//write the Header to pFile
	if (m_pSceneData == NULL) return;
	
	//init _E3DHeader
	// memcpy(&header, m_pSceneData, sizeof(TableData));
}


VOID CN3DLoaderImpV2::ReadFileTable()
{
	//write the Header to pFile
	if (m_pSceneData == NULL) return;
	
	//init _E3DHeader
	memcpy(&TableData, m_pSceneData, sizeof(TableData));
}


//-------------------------------------------------------------------
//方法: ConstructScene(CHAR* sceneName)
//参数: 
//用途: 构造场景
//-------------------------------------------------------------------
DWORD CN3DLoaderImpV2::ConstructScene(CScene *pScene, CSceneResource* pSceneResource)
{
	m_pScene = pScene;
	m_pSceneResource = pSceneResource;	

	//read window prop
	ReadFileTable(); 
    
	ReadSceneProp();

	//read privacy Data
	ReadPrivacyData();
	
	//read the camera data
	ReadCameraData();
	
	//read Audio data
	ReadAudioData();
	//read flash data
	ReadFlashData();
	
	//read the Light Data
	ReadLightData();
	//read the Material data
	ReadMaterialData();	 
		
	//read the mesh data;
	ReadModelData();
	//Read ModelGroup Data
	ReadModelGroupData();
	//找到每个模型对应的索引模型
	FindReferenceModel();
	
	//读取文字信息
	ReadTextData();
	//读取Overlay信息
	ReadOverlayData();
	//读取Navigater信息
	ReadNavigaterData();
	//读取Button数据
	ReadButtonData();

	CreateOverlay(); // 创建还没有创建的Overlay
    
	//读入控制信息
	ReadControlData();

	//读入场景图
	ReadSceneGraph();

	m_preTexCnt = m_pScene->m_vTexture.size();
	
    //--------------------------------------------------------------------------------------------------
	
	//合法授权去掉Logo
	/*	if (m_pScene->m_bAuthorized)
	m_pScene->m_pStartLogo->m_bVisible = FALSE;
	else
	m_pScene->m_pStartLogo->m_bVisible = TRUE;
	*/
	return ERR_COMMON_OK;
}


//读取窗体信息
VOID CN3DLoaderImpV2::ReadSceneProp()
{
	BYTE* pOffset = m_pSceneData  + sizeof(TableData);;
	memcpy(&sceneProp, pOffset, sizeof(sceneProp));

	//m_pScene->m_oriWidth = sceneProp.Width;
	//m_pScene->m_oriHeight = sceneProp.Height;
	m_SceneWidth  = sceneProp.Width;
	m_SceneHeight = sceneProp.Height;
	if (m_SceneWidth <= 0)
	{
		m_SceneWidth = 800;
	}
	if (m_SceneHeight <= 0)
	{
		m_SceneHeight = 600;
	}

	//m_pScene->rect.left = 0;
	//m_pScene->rect.top = 0;
	//m_pScene->rect.right = m_pScene->m_oriWidth;
	//m_pScene->rect.bottom = m_pScene->m_oriHeight;

	BYTE red   = BYTE(sceneProp.BackColor >> 16 & 0xff);
	BYTE green = BYTE(sceneProp.BackColor >> 8 & 0xff);
	BYTE blue  = BYTE(sceneProp.BackColor & 0xff);
	m_pScene->SetBackground(red, green, blue);
	m_pScene->fps = sceneProp.FPS;
	m_pScene->antiLevel = sceneProp.AntiAliasLevel;

	//m_pScene->FSBlurLevel = SceneProp.FSBlurLevel;
	//m_pScene->FSGlowLevel = SceneProp.FSGlowLevel;

	//m_pScene->m_KfrCnt = WindowProp.KeyframeCnt;

	//m_pScene->m_StartFrame = 0;
	//m_pScene->m_EndFrame = WindowProp.KeyframeCnt;

	//m_pScene->m_KfrRepeatCnt = WindowProp.RepeatCnt;
}

// 读取私有信息
VOID CN3DLoaderImpV2::ReadPrivacyData()
{
	BYTE* pOffset = m_pSceneData + TableData.PrivacyDataPtr;
	UINT i;
	memcpy(&i, pOffset, sizeof(i));
}

//-------------------------------------------------------------------
//方法: ReadCameraData()
//参数: 
//用途: 读取摄像机信息
//-------------------------------------------------------------------
VOID CN3DLoaderImpV2::ReadCameraData()
{
	BYTE* pOffset = m_pSceneData + TableData.cameraDataPtr;
	
	m_preCameraCnt = m_pScene->getCameraCount();
	
	//得到摄像机数量
	memcpy(&(CameraData.cameraCnt), pOffset, sizeof(CameraData.cameraCnt));
	pOffset += sizeof(CameraData.cameraCnt);
	
	CameraData.camera = new N3D_Camera[CameraData.cameraCnt];
	//逐个读取摄像机信息

	for (int i =0; i < CameraData.cameraCnt; i++)
	{	
		std::string sName = "";	

		//51007版本，相机名称没有字符限制
		if (m_fN3DFileVersion < 5.1007f)
		{
		    CHAR* readName =new CHAR[N3D_MAX_NAME_LENGTH];
			ZeroMemory(readName,sizeof(readName));
            memcpy(readName , pOffset, N3D_MAX_NAME_LENGTH);
			readName[N3D_MAX_NAME_LENGTH - 1] = '\0';
			pOffset += N3D_MAX_NAME_LENGTH;
			sName = std::string(readName);
			SAFE_DELETE_ARRAY(readName);
		}else
		{
			INT strLen;
			memcpy(&(strLen), pOffset, sizeof(strLen));	
			pOffset += sizeof(strLen);	
			char* readName =new char[strLen+1];
			ZeroMemory(readName, strLen + 1);
			memcpy(readName , pOffset, strLen);
			readName[strLen] = '\0';
			pOffset += strLen;
			sName = std::string(readName);
			SAFE_DELETE_ARRAY(readName);		
		}

		sName = m_pScene->renameCameraIfExist(sName);  // 检查重命名
		CCamera* pCamera = m_pScene->CreateCamera(sName);

		pCamera->index = m_preCameraCnt + i;
		
		//当前摄像机的基本信息
		memcpy(&(CameraData.camera[i]),pOffset,sizeof(CameraData.camera[i]));
		
        pOffset += sizeof(CameraData.camera[i]);
		
		pCamera->SetFov(CameraData.camera[i].BaseInfo.fov);
		pCamera->setNearClip(CameraData.camera[i].BaseInfo.nearClipDis);
		pCamera->setFarClip(CameraData.camera[i].BaseInfo.farClipDis);

		// 读入相机参数动画
		if (m_fN3DFileVersion >= 5.1006f)
		{
			int frameCnt = 0;  // 相机的参数动画
			memcpy(&frameCnt, pOffset, sizeof(frameCnt));
			pOffset += sizeof(frameCnt);
			for (int iFrame = 0; iFrame < frameCnt; ++iFrame)
			{
				N3D_CAMPARAM camInfo;
				memcpy(&camInfo, pOffset, sizeof(camInfo));
				pOffset += sizeof(camInfo);
				// pCamera-> 这里应该保存相机参数动画中的fov，暂时没有设置
			}
		}				
		m_pSceneResource->camera.push_back(pCamera);
	}
	
	//release memory
	SAFE_DELETE_ARRAY(CameraData.camera);	
}


//读取声音数据
VOID CN3DLoaderImpV2::ReadAudioData()
{	
	//找到声音信息的位置
	BYTE* pOffset = m_pSceneData + TableData.AudioPtr;
	
	//得到音频数量
	memcpy(&(AudioData.AudioCnt), pOffset, sizeof(AudioData.AudioCnt));
	pOffset += sizeof(AudioData.AudioCnt);
	
	if (AudioData.AudioCnt == 0)
		return;
	
	AudioData.Audio = new N3D_Audio[AudioData.AudioCnt];
	//逐个读取音频信息
	for (int i =0; i< AudioData.AudioCnt; i++)
	{
		//保存当前音频的基本信息
		memcpy(&(AudioData.Audio[i].strLen),pOffset,sizeof(AudioData.Audio[i].strLen));
		char* name =new char[AudioData.Audio[i].strLen+1];
		ZeroMemory(name,sizeof(name));
		memcpy(name, pOffset, AudioData.Audio[i].strLen);
		pOffset += AudioData.Audio[i].strLen;
		SAFE_DELETE_ARRAY(name);
        //audioFileStringLength
        memcpy(&(AudioData.Audio[i].fileNameLen),pOffset,sizeof(AudioData.Audio[i].fileNameLen));
        pOffset += sizeof(AudioData.Audio[i].fileNameLen);
        //audioFileName
        AudioData.Audio[i].fileName.reserve(AudioData.Audio[i].fileNameLen);
        memcpy(&(AudioData.Audio[i].fileName),pOffset,AudioData.Audio[i].fileNameLen);
		pOffset += AudioData.Audio[i].fileNameLen;
        //audioTpye
        memcpy(&(AudioData.Audio[i].type),pOffset,sizeof(AudioData.Audio[i].type));
        pOffset += sizeof(AudioData.Audio[i].type);
		memcpy(&(AudioData.Audio[i].repeatCnt),pOffset,sizeof(AudioData.Audio[i].repeatCnt));
        pOffset += sizeof(AudioData.Audio[i].repeatCnt);
		//
		
		/*
		CAudioPlayer* pAudio = new CAudioPlayer;
		
		//当前音频的基本信息
		strcpy(pAudio->name, AudioData.Audio[i].name.c_str());
		pAudio->index = m_pScene->audio.size();
		pAudio->m_repeatCnt = AudioData.Audio[i].repeatCnt;
		pAudio->type = AudioData.Audio[i].type;
		
		if (pAudio->Create((char*)AudioData.Audio[i].fileName.c_str(), AudioData.Audio[i].repeatCnt) == false)
		{
			SAFE_RELEASE_D3D(pAudio);
			SAFE_DELETE(pAudio);
		}
		m_pScene->audio.push_back((DWORD)pAudio);
		m_pSceneResource->audio.push_back((DWORD)pAudio);
		*/
	}
	
	//release memory
	SAFE_DELETE_ARRAY(AudioData.Audio);
}
//读取Flash数据
VOID CN3DLoaderImpV2::ReadFlashData()
{	
	//找到Flash信息的位置
	BYTE* pOffset = m_pSceneData + TableData.FlashPtr;
	
	//得到Flash数量
	memcpy(&(FlashData.FlashCnt), pOffset, sizeof(FlashData.FlashCnt));
	if (FlashData.FlashCnt == 0)
		return;
}
//-------------------------------------------------------------------
//方法: IsTextureExist()
//参数: 
//用途: 判断材质是否已经存在, 如果存在返回texture在场景texture列表中的索引，否则返回-1
//-------------------------------------------------------------------
int CN3DLoaderImpV2::IsTextureExist(std::string texName, INT width, INT height, D3DFORMAT format)
{
// 	UINT texCnt = m_pScene->texture.size();
// 	CTexture** ppTex = m_pScene->texture.begin();
// 	
// 	for (UINT i = 0; i< texCnt; i++)
// 	{
// 		CTexture* pTex = *(ppTex + i);
// 		std::string theTexName = pTex->pathAndName;
// 		if (theTexName == texName && 
// 			pTex->height == height && 
// 			pTex->width == width &&
// 			pTex->BP == format)
// 		{
// 			return i;
// 		}
// 	}
	return -1;
}
//-------------------------------------------------------------------
//方法: GetD3DTexOP()
//参数: 
//用途: 将N3D的混色模式参数转换成D3D的混色模式参数
//-------------------------------------------------------------------
D3DTEXTUREOP CN3DLoaderImpV2::GetD3DTexOP(const N3D_BlenderOP& n3d_BlenderOP)
{	
	switch(n3d_BlenderOP)
	{
	case NBM_DISABLE:
		return D3DTOP_DISABLE;
		break;
	case NBM_REPLACE:
		return D3DTOP_SELECTARG1;
		break;
	case NBM_MODULATE:
		return D3DTOP_MODULATE;
		break;
	case NBM_MODULATE2:
		return D3DTOP_MODULATE2X;
		break;
	case NBM_MODULATE4:
		return D3DTOP_MODULATE4X;
		break;
	case NBM_ADD:
		return D3DTOP_ADD;
		break;
	case NBM_ADD_SIGNED:
		return D3DTOP_ADDSIGNED;
		break;
	case NBM_ADD_SMOOTH:
		return D3DTOP_ADDSMOOTH;
		break;
	case NBM_SUBTRACT:
		return D3DTOP_SUBTRACT;
		break;
	case NBM_BLEND_DIFFUSE_ALPHA:
		return D3DTOP_BLENDDIFFUSEALPHA;
		break;
	case NBM_BLEND_TEXTURE_ALPHA:
		return D3DTOP_BLENDTEXTUREALPHA;
		break;
	case NBM_BLEND_CURRENT_ALPHA:
		return  D3DTOP_BLENDCURRENTALPHA;
		break;
	case NBM_BLEND_MANUAL:
		return D3DTOP_BLENDFACTORALPHA;
		break;
	case NBM_DOTPRODUCT:
		return D3DTOP_DOTPRODUCT3;
		break;
	case NBM_BLEND_DIFFUSE_COLOUR:
		return D3DTOP_SELECTARG1;
		break;
	}
	
	return D3DTOP_DISABLE;
}

VOID CN3DLoaderImpV2::ReadMaterialData()
{
/*#ifdef _DEBUG
FILE *pFile;
std::string tmpfile = std::string("D:\\StartTeam\\editmodel\\material_atl.txt");
pFile = fopen(tmpfile.c_str(), "wt");
#endif*/
	
	m_vMaterial.clear();

    BYTE* pOffset = m_pSceneData + TableData.mtrlDataPtr;

    //mtrl count
	memcpy(&(MaterialData.mtrlCnt), pOffset, sizeof(MaterialData.mtrlCnt));	
	pOffset += sizeof(MaterialData.mtrlCnt);
	//mtrl data
	MaterialData.mtrl = new N3D_MtrlInfo[MaterialData.mtrlCnt];	
	
	for (int i= 0; i< MaterialData.mtrlCnt; i++)
	{		
		std::string sName;

		//51007版本，材质名称没有字符限制
		if(m_fN3DFileVersion < 5.1007f)
		{
		    CHAR* readName =new CHAR[N3D_MAX_NAME_LENGTH];
			ZeroMemory(readName,sizeof(readName));
            memcpy(readName , pOffset, N3D_MAX_NAME_LENGTH);
			readName[N3D_MAX_NAME_LENGTH - 1] = '\0';
			pOffset += N3D_MAX_NAME_LENGTH;
			sName = std::string(readName);
			SAFE_DELETE_ARRAY(readName);	
		}
		else
		{
			INT strLen;
			memcpy(&(strLen), pOffset, sizeof(strLen));	
			pOffset += sizeof(strLen);	
			char* readName =new char[strLen+1];
			ZeroMemory(readName, strLen + 1);
			memcpy(readName , pOffset, strLen);
			readName[strLen] = '\0';
			pOffset += strLen;
			sName = std::string(readName);
			SAFE_DELETE_ARRAY(readName);		
		}

		CMaterial* pMtrl = m_pScene->CreateMaterial(sName);
		if (!pMtrl)
		{
			continue;
		}
		pMtrl->index = i;
		memcpy(&(MaterialData.mtrl[i].BasalInfo), pOffset, sizeof(MaterialData.mtrl[i].BasalInfo));	
		pOffset += sizeof(MaterialData.mtrl[i].BasalInfo); 
		
		//mtrl->dif 
		pMtrl->d3dmtrl.Diffuse.a = ((MaterialData.mtrl[i].BasalInfo.diffuse)&0xff)/255.0f;
		pMtrl->d3dmtrl.Diffuse.r = ((MaterialData.mtrl[i].BasalInfo.diffuse>>24)&0xff)/255.0f;
		pMtrl->d3dmtrl.Diffuse.g = ((MaterialData.mtrl[i].BasalInfo.diffuse>>16)&0xff)/255.0f;
		pMtrl->d3dmtrl.Diffuse.b = ((MaterialData.mtrl[i].BasalInfo.diffuse>>8)&0xff)/255.0f;
       
		//mtrl->amb
		pMtrl->d3dmtrl.Ambient.a = ((MaterialData.mtrl[i].BasalInfo.ambient)&0xff)/255.0f;
		pMtrl->d3dmtrl.Ambient.r = ((MaterialData.mtrl[i].BasalInfo.ambient>>24)&0xff)/255.0f;
		pMtrl->d3dmtrl.Ambient.g = ((MaterialData.mtrl[i].BasalInfo.ambient>>16)&0xff)/255.0f;
		pMtrl->d3dmtrl.Ambient.b = ((MaterialData.mtrl[i].BasalInfo.ambient>>8)&0xff)/255.0f;
		//mtrl->spe
		pMtrl->specular.r = ((MaterialData.mtrl[i].BasalInfo.specular>>24)&0xff)/(255.0f/**MaterialData.mtrl[i].BasalInfo.specularLevel*/);
		pMtrl->specular.g = ((MaterialData.mtrl[i].BasalInfo.specular>>16)&0xff)/(255.0f/**MaterialData.mtrl[i].BasalInfo.specularLevel*/);
		pMtrl->specular.b = ((MaterialData.mtrl[i].BasalInfo.specular>>8)&0xff)/(255.0f/**MaterialData.mtrl[i].BasalInfo.specularLevel*/);		
		pMtrl->specular.a = ((MaterialData.mtrl[i].BasalInfo.specular)&0xff)/(255.0f/**MaterialData.mtrl[i].BasalInfo.specularLevel*/);		
		
		pMtrl->specularLevel      = MaterialData.mtrl[i].BasalInfo.specularLevel; 
		pMtrl->d3dmtrl.Specular   = pMtrl->specularLevel / 100.0f * pMtrl->specular;
		pMtrl->d3dmtrl.Specular.a = 1.0f;	
		
		//power
		pMtrl->d3dmtrl.Power = MaterialData.mtrl[i].BasalInfo.shininess;//高光锐度
		
		//mtrl->emi
		pMtrl->selfIllumination = (UINT)MaterialData.mtrl[i].BasalInfo.selfIllumination;

		//pMtrl->selfIllumination = 100.0f;
		pMtrl->d3dmtrl.Emissive.a = 1.0f;
		pMtrl->d3dmtrl.Emissive.r = pMtrl->selfIllumination / 100.0f * pMtrl->d3dmtrl.Diffuse.r;
		pMtrl->d3dmtrl.Emissive.g = pMtrl->selfIllumination / 100.0f * pMtrl->d3dmtrl.Diffuse.g;
		pMtrl->d3dmtrl.Emissive.b = pMtrl->selfIllumination / 100.0f * pMtrl->d3dmtrl.Diffuse.b;
		// pMtrl->type = MaterialData.mtrl[i].BasalInfo.shaderType;
		pMtrl->type = D3DSHADE_GOURAUD;
		
		pMtrl->setOpacity((INT)MaterialData.mtrl[i].BasalInfo.opacity); //透明度
		
		if (pMtrl->getOpacity() < 100)
		{
			pMtrl->bAlphaBlend = TRUE;
		}

		pMtrl->twoSide  = MaterialData.mtrl[i].BasalInfo.bTwoSide;
		
		pMtrl->bAlphaTest = MaterialData.mtrl[i].BasalInfo.alphaTest;		
		pMtrl->AlpahaTestValue =MaterialData.mtrl[i].BasalInfo.alphaTestValue;
		pMtrl->fillMode = (_D3DFILLMODE)MaterialData.mtrl[i].BasalInfo.fillMode;
		
		//51001版本，记录是否材质使用模型diffuse值的参数
		if(m_fN3DFileVersion >= 5.1001f)
		{
			memcpy(&(MaterialData.mtrl[i].bUseDiffuse), pOffset, sizeof(MaterialData.mtrl[i].bUseDiffuse));
			pOffset +=sizeof(MaterialData.mtrl[i].bUseDiffuse);
		}
		else
		{
			MaterialData.mtrl[i].bUseDiffuse =true;
		}
		pMtrl->m_bUseDiffuse = (BOOL) MaterialData.mtrl[i].bUseDiffuse;
		
		//目前材质动画为空 kfrCnt为0
		memcpy(&(MaterialData.mtrl[i].kfrCnt), pOffset, sizeof(MaterialData.mtrl[i].kfrCnt));
		pOffset +=sizeof(MaterialData.mtrl[i].kfrCnt);
		
		// memcpy(&(MaterialData.mtrl->kfrMtrlInfo), pOffset, sizeof(MaterialData.mtrl->kfrCnt));
		// pOffset +=sizeof(MaterialData.mtrl->kfrCnt);
		
		//读取	 
		memcpy(&(MaterialData.mtrl[i].TextureLayerCount), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerCount));
		pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerCount);
		MaterialData.mtrl[i].TextureLayerList =new N3D_TextureLayer[MaterialData.mtrl[i].TextureLayerCount];
		for(UINT m = 0; m < MaterialData.mtrl[i].TextureLayerCount; m++)
		{
			CTextureLayer* pTexLayer = new CTextureLayer();
			//51007版本，材质贴图层名称没有字符限制
			if(m_fN3DFileVersion < 5.1007f)
			{
				CHAR* readName =new CHAR[N3D_MAX_NAME_LENGTH];
				ZeroMemory(readName,sizeof(readName));
				memcpy(readName , pOffset, N3D_MAX_NAME_LENGTH);
				readName[N3D_MAX_NAME_LENGTH - 1] = '\0';
				pOffset += N3D_MAX_NAME_LENGTH;
				pTexLayer->m_sName = std::string(readName);
				SAFE_DELETE_ARRAY(readName);			
			}else
			{
				INT strLen;
				memcpy(&(strLen), pOffset, sizeof(strLen));	
				pOffset += sizeof(strLen);	
				char* readName =new char[strLen+1];
				ZeroMemory(readName, strLen + 1);
				memcpy(readName , pOffset, strLen);
				readName[strLen] = '\0';
				pOffset += strLen;
				pTexLayer->m_sName = std::string(readName);
				SAFE_DELETE_ARRAY(readName); 		
			}
			
			/*memcpy(MaterialData.mtrl[i].TextureLayerList[m].Name, pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].Name));
			pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].Name);
            MaterialData.mtrl[i].TextureLayerList[m].Name[31] = '\0';*/
			
			memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].type), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].type));
			pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].type);
			
			memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].UVIndex), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].UVIndex));
			pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].UVIndex);

			//MipMap //51004版本，添加了贴图层的MipMap信息
			if (m_fN3DFileVersion >= 5.1004f)
			{
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].Mipmap), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].Mipmap));
				pOffset += sizeof(MaterialData.mtrl[i].TextureLayerList[m].Mipmap);
			}
			
			switch(MaterialData.mtrl[i].TextureLayerList[m].type)
			{
			case TLT_BASAL:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam);
				break;

			case TLT_BUMPMAP:
				{
					INT type;
					memcpy(&(type), pOffset, sizeof(type));
					pOffset += sizeof(type);
					VGS_BumpType bumpType = (VGS_BumpType)type;
					//创建凹凸贴图
					CBumpMap* pBumpMap = new CBumpMap(m_pScene->m_pd3dDevice, &m_pScene->m_d3dpp, m_pScene->d3dParam.SupportBestBumpFormat);
					m_pScene->m_vBumpTexture.push_back(pBumpMap);

					switch(bumpType)
					{
					case VBT_BUMPWATER:
						{//Bump波纹贴图
							N3D_BumpwaterTextureParam bumpwaterTexParam;
							memcpy(&(bumpwaterTexParam), pOffset, sizeof(bumpwaterTexParam));
							pOffset += sizeof(bumpwaterTexParam);
							if (pBumpMap->m_MapWidth == 0)
							{
							  bumpwaterTexParam.width = 256;
                              bumpwaterTexParam.height = 256;
							}
							pBumpMap->SetBumpAsWavemap(bumpwaterTexParam.width, bumpwaterTexParam.height, bumpwaterTexParam.XScale, bumpwaterTexParam.YScale,
								                       bumpwaterTexParam.Radius, bumpwaterTexParam.Speed, bumpwaterTexParam.CenterX, bumpwaterTexParam.CenterY);
							
						}
						break;
					case VBT_BUMPNORMARLMAP:     //Bump法线贴图
						{
							N3D_BasalTextureParam basalTexParam;
                            memcpy(&(basalTexParam), pOffset, sizeof(basalTexParam));
							pOffset += sizeof(basalTexParam);
                            pBumpMap->m_MapWidth = basalTexParam.width;
							pBumpMap->m_MapHeight = basalTexParam.height;
							if (pBumpMap->m_MapWidth == 0)
							{
								basalTexParam.width  = 256;
								basalTexParam.height = 256;
							}

							N3D_BumpNormalTextureParam  bumpnormalTexParam;
							memcpy(&(bumpnormalTexParam), pOffset, sizeof(bumpnormalTexParam));
							pOffset += sizeof(bumpnormalTexParam);
							CLight* pLight = m_pScene->GetLightByIndex(bumpnormalTexParam.LightIndex + m_preLightCnt);
							pBumpMap->SetBumpMapLight(pLight);
						}
						break;
					case VBT_BUMPENVIRONMENT:       //Bump环境贴图
						{
							N3D_BumpEnvTextureParam bumpenvTexParam;
							memcpy(&(bumpenvTexParam), pOffset, sizeof(bumpenvTexParam));
							pOffset += sizeof(bumpenvTexParam);
							pBumpMap->m_MapWidth = bumpenvTexParam.width;
                            pBumpMap->m_MapHeight = bumpenvTexParam.height;
                            pBumpMap->m_Noise1 = bumpenvTexParam.noise1;
                            pBumpMap->m_Noise2 = bumpenvTexParam.noise2;
						}
						break;
					}					
				}
				break;

			case TLT_OPACITY:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.OpacityTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.OpacityTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.OpacityTextureParam);				
				pMtrl->bAlphaBlend = TRUE;
				break;
				
			case TLT_RTFMAP:  //实时反射贴图的读取
				if (m_fN3DFileVersion >= 5.1006f)
				{
					memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTReflectionParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTReflectionParam));
					pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTReflectionParam);	
					//读取反射主体的模型索引
					int modelIdx;
                    memcpy(&(modelIdx), pOffset, sizeof(modelIdx));
					pOffset +=sizeof(modelIdx);
					
                    //创建RTF材质
					//获得模型句柄
					std::vector<CModel*>::iterator ppModel =m_pScene->model.begin();
					int modelCnt = m_pScene->model.size();
					BOOL rtfTexCreated = FALSE;
					if (modelIdx < modelCnt)
					{
                        CModel* pModel = *(ppModel + modelIdx);
						//创建一个RTFMap
						CRTFMap* pRTFMap = new CRTFMap(pTexLayer->m_sName, m_pScene->m_pd3dDevice, &m_pScene->m_d3dpp, MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTReflectionParam.width, MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTReflectionParam.height, D3DFMT_R8G8B8);
						pRTFMap->SetRefPlaneByModel((DWORD)pModel);
						pRTFMap->m_texProjMatrix = m_pScene->GetTextureProjMatrix();	
						//放入RTFMap贴图队列
						m_pScene->m_vRtfTexture.push_back(pRTFMap);
						rtfTexCreated = TRUE;
					    pTexLayer->pTexture = pRTFMap;
					}
                    //读取被反射物体列表
					DWORD refModelCount;
                    memcpy(&refModelCount, pOffset, sizeof(refModelCount));
					pOffset +=sizeof(refModelCount);
                    
					for(UINT i =0; i<(UINT)refModelCount;i++)
					{
						int refModelIdx;
						memcpy(&refModelIdx, pOffset, sizeof(refModelIdx));
						pOffset +=sizeof(refModelIdx);
						//创建
						if (rtfTexCreated && refModelIdx < modelCnt)
						{
							
							CRTFMap* pRTFMap = *(m_pScene->m_vRtfTexture.end());

							//获得模型句柄
							std::vector<CModel*>::iterator ppModel =m_pScene->model.begin();
							int modelCnt = m_pScene->model.size();
							if (modelIdx < modelCnt)
							{
								CModel* pModel = *(ppModel + refModelIdx);
								
								//将模型加入到反射贴图的反射模型队列
								pRTFMap->AddRefModel((DWORD)pModel);
								
								//该模型需要渲染到反射贴图表面，所以增加附加渲染信息
								ADDITIONALRENDERINFO renderInfo;
								renderInfo.type = ADS_RTFMAP;
								renderInfo.pRenderToTexture =  (DWORD) pRTFMap;
								
								//放入模型的附加渲染信息队列
								pModel->m_additionalRenderInfo.push_back(renderInfo);
							}
						}
					}	
				}
				break;
			//实时折射贴图的读取
			case TLT_RTTMAP:
				if (m_fN3DFileVersion >= 5.1006f)
				{
					memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTRefractionParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTRefractionParam));
					pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTRefractionParam);

					//读取折射主体的模型索引
					int modelIdx;
                    memcpy(&(modelIdx), pOffset, sizeof(modelIdx));
					pOffset +=sizeof(modelIdx);
					 //读取被折射物体列表
					DWORD refModelCount;
                    memcpy(&refModelCount, pOffset, sizeof(refModelCount));
					pOffset +=sizeof(refModelCount);
					  
					for(UINT i =0; i<(UINT)refModelCount;i++)
					{
						int refModelIdx;
						memcpy(&refModelIdx, pOffset, sizeof(refModelIdx));
						pOffset +=sizeof(refModelIdx);
					}	        
				}
				break;
			case TLT_REFLECT:
				//51002版本，反射添加反射种类函数
				if (m_fN3DFileVersion >= 5.1002f)
				{
					memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam51002), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam51002));
					pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam51002);
					//反射贴图的反射方式
					//平面反射
					if (MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam51002.reflectionType == 0)
					{
						if (m_pScene->d3dParam.IsSupportTexGenSphereMap)
							pTexLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
						else
							pTexLayer->UVIndex = D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR;	
						pTexLayer->transformFlag = _D3DTEXTURETRANSFORMFLAGS(D3DTTFF_COUNT3 | D3DTTFF_PROJECTED);
					}
					//曲面反射
					if (MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam51002.reflectionType == 1)
					{
						if (m_pScene->d3dParam.IsSupportTexGenSphereMap)
							pTexLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
						else
							pTexLayer->UVIndex = D3DTSS_TCI_CAMERASPACENORMAL;
						pTexLayer->transformFlag = D3DTTFF_COUNT3;
					}
					//无效
					if (MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam51002.reflectionType == -1)
					{
						pTexLayer->transformFlag = D3DTTFF_DISABLE;
						pTexLayer->UVIndex = D3DTSS_TCI_PASSTHRU;
					}
				}
				else
				{
					memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam));
					pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam);
				
					//曲面反射
					if (m_pScene->d3dParam.IsSupportTexGenSphereMap)
						pTexLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
					else
						pTexLayer->UVIndex = D3DTSS_TCI_CAMERASPACENORMAL;
					pTexLayer->transformFlag = D3DTTFF_COUNT3;
				}
				break;

			case TLT_CUBEMAP:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.CubicTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.CubicTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.CubicTextureParam);
				break;

			case TLT_DYNAMIC:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.DynamicTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.DynamicTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.DynamicTextureParam);
				break;
			/*
			case TLT_BUMPWATER:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BumpwaterTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BumpwaterTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BumpwaterTextureParam);
				break;
				*/
				
			case TLT_MOVIE:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.MovieTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.MovieTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.MovieTextureParam);
				break;

			case TLT_FLASH:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.FlashTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.FlashTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.FlashTextureParam);
				break;
			} // switch
			
			//51003版本，添加了模型的UV信息  
			if (m_fN3DFileVersion >= 5.1003f && MaterialData.mtrl[i].TextureLayerList[m].type != TLT_RTFMAP && MaterialData.mtrl[i].TextureLayerList[m].type != TLT_RTTMAP)
			{
				memcpy(&(pTexLayer->USpeed), pOffset, sizeof(pTexLayer->USpeed));
				pOffset += sizeof(pTexLayer->USpeed);
				memcpy(&(pTexLayer->VSpeed), pOffset, sizeof(pTexLayer->VSpeed));
				pOffset += sizeof(pTexLayer->VSpeed);

				if (fabs(pTexLayer->USpeed) < 0.0001f) pTexLayer->USpeed = 0;
				if (fabs(pTexLayer->VSpeed) < 0.0001f) pTexLayer->VSpeed = 0;
			}
			pTexLayer->USpeed = - pTexLayer->USpeed/m_pScene->fps;
            pTexLayer->VSpeed = - pTexLayer->VSpeed/m_pScene->fps;

			if (m_fN3DFileVersion >= 5.1005f)
			{
				if(MaterialData.mtrl[i].TextureLayerList[m].type == TLT_CUBEMAP)
				{
					BYTE cubicType;
					memcpy(&(cubicType), pOffset, sizeof(cubicType));
					pOffset += sizeof(cubicType);				   
				}			
			}

			// 混色方式
			memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo));
			pOffset += sizeof(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo);
			
			memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].ImageCount), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].ImageCount));
			pOffset += sizeof(MaterialData.mtrl[i].TextureLayerList[m].ImageCount);
			
			UINT imageCnt =MaterialData.mtrl[i].TextureLayerList[m].ImageCount;
			MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList = new int[imageCnt];
			int *pTemint = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList;
			memcpy(pTemint, pOffset, sizeof(int) * imageCnt);
			pOffset += sizeof(int) * imageCnt;
			
			pTexLayer->type =(N3D_TextureType)(MaterialData.mtrl[i].TextureLayerList[m].type);
			if (pTexLayer->type != TLT_REFLECT) //反射贴图自动生成UV
			{
				pTexLayer->UVIndex = MaterialData.mtrl[i].TextureLayerList[m].UVIndex;
			}
			//设置混色模式
			pTexLayer->Blenderinfo.AlphaBlendOption=GetD3DTexOP(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo.AlphaBlendOption);
			pTexLayer->Blenderinfo.ColorBlendOption=GetD3DTexOP(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo.ColorBlendOption);
			
			//设置混色参数
			D3DCOLOR blendFactor = D3DCOLOR_COLORVALUE(0,0,0, MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo.AlphaBlendParam.BlenderOP_alpha);
			pTexLayer->Blenderinfo.AlphaBlendParam = blendFactor;
			blendFactor = D3DCOLOR_COLORVALUE(0,0,0, MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo.ColorBlendParam.BlenderOP_alpha);
			pTexLayer->Blenderinfo.ColorBlendParam = blendFactor;
			
			//加入材质的贴图列表
			pMtrl->m_vpTexLayers.push_back(pTexLayer);
		}

		//创建贴图---------------------------------------------------------------------------------------------------------
		for (m = 0; m < MaterialData.mtrl[i].TextureLayerCount; m++)
		{
			CCubeMap* pCubeMap =NULL; 
			CTexture* pTex =NULL;
			CDynamicTex* pDyTex =NULL;
			CTextureLayer* pTexLayer = pMtrl->m_vpTexLayers[m];
            std::string texName = pTexLayer->m_sName;
			switch(pTexLayer->type)
			{
			case TLT_BASAL:				
			case TLT_REFLECT:
			{
				//判断该材质是否已经被创建				
				CTexture *pTex = m_pScene->getTextureByName(texName);
				//创建新的贴图对象
				if (pTex == NULL)
				{
					pTex = m_pScene->CreateEmptyTex(texName, D3DFMT_A8R8G8B8, MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam.width,
						                     MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam.height, MaterialData.mtrl[i].TextureLayerList[m].Mipmap);

					//混色模式选择
					if (MaterialData.mtrl[i].BasalInfo.alphaTest)
					{
						pTex->bAlphaTest= TRUE;
					}
					//图像数据索引
					pTex->imageIdx = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[0];		  
					m_pSceneResource->texture.push_back(pTex);
					pTexLayer->pTexture = pTex;
					pTexLayer->type = TLT_BASAL;
					pTexLayer->UVIndex = MaterialData.mtrl[i].TextureLayerList[m].UVIndex;
				}
				else
				{
					pTexLayer->pTexture = pTex; 
				}
				break;
			}
			//透明贴图D3DFMT_A8----------------------------------------------------------------------------------------
			case TLT_OPACITY:	
			{
				//判断该材质是否已经被创建				
				CTexture *pTex = m_pScene->getTextureByName(texName);
				//创建新的贴图对象
				if (pTex == NULL)
				{
					pTex = m_pScene->CreateEmptyTex(texName, D3DFMT_A8, MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam.width,
						                     MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam.height, MaterialData.mtrl[i].TextureLayerList[m].Mipmap);

					//混色模式选择
					if (MaterialData.mtrl[i].BasalInfo.alphaTest)
					{
						pTex->bAlphaTest= TRUE;
					}

					//图像数据索引
					pTex->imageIdx = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[0];	
					m_pSceneResource->texture.push_back(pTex);
					pTexLayer->pTexture = pTex;
					pTexLayer->type = TLT_OPACITY;
					pTexLayer->UVIndex = MaterialData.mtrl[i].TextureLayerList[m].UVIndex;
				}
				else
				{
					pTexLayer->pTexture = pTex;
				}
				break;
			}
            //实时反射--------------------------------------------------------------------------------------------
			case TLT_RTFMAP:
				break;
			case TLT_RTTMAP:
				break;			
			case TLT_REALTIMESHADOW: 
				break;
			case TLT_BUMPMAP: //TLT_BUMPMAP
            {
//                 CBumpMap* pBumpMap = m_pScene->bumpTexture[m_pScene->bumpTexture.size() - 1];
// 				if (pBumpMap->m_type != BUMPWAVEMAP)
// 				{
// 					//判断该材质是否已经被创建
// 					pTexIdx = IsTextureExist(texName, 
// 											pBumpMap->m_MapWidth, 
// 											pBumpMap->m_MapHeight,
// 											pBumpMap->m_BumpMapFormat);
// 					// if(!pTexIdx)
// 					if (pTexIdx == -1)
// 					{
// 						CTexture* pTex = new CTexture();
// 						pTex->index = m_pScene->texture.size();
// 						//Texture的名字用贴图层的名字
// 						//strcpy(pTex->pathAndName, pTexLayer->Name);
//                         pTex->pathAndName = pTexLayer->Name;
// 						pTex->width = pBumpMap->m_MapWidth;//MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.width;
// 						pTex->height = pBumpMap->m_MapHeight;//MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.height;
// 						pTex->mipCount = 0;// 必须等于0；MaterialData.mtrl[i].TextureLayerList[m].Mipmap;
// 						pTex->BP = D3DFMT_A8R8G8B8;  //以R8G8B8创建，然后在InitBumpMap中转化为U8V8的贴图
// 						pTex->type = BUMP_TYPE;
// 
// 						//混色模式选择
// 						if(MaterialData.mtrl[i].BasalInfo.alphaTest)
// 							pTex->bAlphaTest= TRUE;
// 						
// 						//图像数据索引
// 						pTex->imageIdx = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[0];		  
// 						m_pScene->texture.push_back(pTex);
// 						m_pSceneResource->texture.push_back(pTex);
// 						pBumpMap->m_pBumpTexture = pTex;
// 					}
// 					else
// 					{
// 						pBumpMap->m_pBumpTexture = m_pScene->texture[pTexIdx];
// 					}	
// 					pBumpMap->m_pBumpTexture->bumpIdx = m_pScene->bumpTexture.size() - 1;
// 				}
// 				pTexLayer->pTexture = (DWORD)pBumpMap;
				
				break;
			}

			//CubeMap--------------------------------------------------------------------------------------
			case TLT_CUBEMAP:
			{
				//判断是否支持CubeMap
// 				if (!m_pScene->d3dParam.IsSupportCUBEMAP) break;
// 
// 				INT width = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_CubicTextureParam.width;
// 				INT height = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_CubicTextureParam.height;
// 				INT mipCount = MaterialData.mtrl[i].TextureLayerList[m].Mipmap;
// 				
// 				pCubeMap = new CCubeMap(m_pScene->m_pd3dDevice, 0, D3DFMT_R8G8B8, width, mipCount);
// 
// 				//CubeMap的名字和编号
// 				pCubeMap->index = m_pScene->cubeMap.size(); 
// 				pCubeMap->name = std::string(pTexLayer->Name);
// 
// 				for(n = 0 ; n< 6; n++)
// 				{
// 					INT imageIdx = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[n];
// 					switch(n)
// 					{
// 					case 0: //前 = +z
// 						pCubeMap->imagesIdx[4] = imageIdx;
// 						break;
// 					case 1: //后 = -z
// 						pCubeMap->imagesIdx[5] = imageIdx;
// 						break;
// 					case 2: //左 = -x
// 						pCubeMap->imagesIdx[1] = imageIdx;
// 						break;
// 					case 3: //右 = +x
// 						pCubeMap->imagesIdx[0] = imageIdx;
// 						break;
// 					case 4:  //上 = +y
// 						pCubeMap->imagesIdx[2] = imageIdx;
// 						break;
// 					case 5: // 下= -y
// 						pCubeMap->imagesIdx[3] = imageIdx;
// 						break;
// 					}
// 				}
// 
// 				//放入场景资源队列
// 				m_pScene->cubeMap.push_back(pCubeMap);
// 				m_pSceneResource->cubeMap.push_back(pCubeMap);
// 				pTexLayer->pTexture = DWORD(pCubeMap);

				break;
			}

			//动态贴图--------------------------------------------------------------------------------------
			case TLT_DYNAMIC:
			{
				//判断是否支持动态贴图 verify by tim
 				if (!m_pScene->d3dParam.IsSupportDynamicTexture) break;
 
 				//创建动态贴图
 				int width = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.DynamicTextureParam.width;
 				int height = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.DynamicTextureParam.height;
 				int frameCnt = MaterialData.mtrl[i].TextureLayerList[m].ImageCount;
 				int fps = (INT)(frameCnt/MaterialData.mtrl[i].TextureLayerList[m].TextureParam.DynamicTextureParam.duration);
 				pDyTex = new CDynamicTex(m_pScene->m_pd3dDevice, width, height, D3DFMT_A8R8G8B8, frameCnt );

 
 				//动态贴图的编号，名称，格式
 				pDyTex->index = m_pScene->dynamicTexture.size();
 				pDyTex->name = std::string(pTexLayer->m_sName);
 
 				//混色模式选择
 				if(MaterialData.mtrl[i].BasalInfo.alphaTest) pDyTex->bAlphaTest= TRUE;
 
 				//动态贴图每帧图片的编号
 				for(int  n = 0; n< frameCnt; n++)
				{
					pDyTex->imagesIdx.push_back(MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[n]);
				}
 
 				//放入场景资源队列
 				m_pScene->dynamicTexture.push_back(pDyTex);
 				m_pSceneResource->dynamicTexture.push_back(pDyTex);
 				pTexLayer->pTexture = (CTexture*)pDyTex;
				
				break;
			}
		  }
	  } // for material texture
	  
      SAFE_DELETE_ARRAY(MaterialData.mtrl[i].TextureLayerList);
      m_vMaterial.push_back(pMtrl);//m_pScene->m_vShader.push_back(pMtrl);
	  m_pSceneResource->shader.push_back(pMtrl);
	} // for each material

	SAFE_DELETE_ARRAY(MaterialData.mtrl);	
}

VOID CN3DLoaderImpV2::ReadImageData()
{
	if (!m_pImageData)
	{
		return;
	}

	BYTE *pOffset = m_pImageData;

	DWORD count;
	CPY_FROM_STREAM(count, pOffset);

	std::vector<N3D_ImageInfo> vImageInfo;
	for (DWORD i = 0; i < count; ++i)
	{
		N3D_ImageInfo ImageInfo;

		CPY_FROM_STREAM(ImageInfo.nameLen, pOffset);
		//ImageInfo.name = new char[ImageInfo.nameLen + 1];
		//ZeroMemory(ImageInfo.name, ImageInfo.nameLen + 1);
		//CPY_FROM_STREAM(ImageInfo.name, pOffset);
		ImageInfo.name = (char*)pOffset;
		pOffset += ImageInfo.nameLen;

		CPY_FROM_STREAM(ImageInfo.DataLen, pOffset);
		ImageInfo.Data = (char*)pOffset;
		pOffset += ImageInfo.DataLen;
		//ImageInfo.Data = new char[ImageInfo.DataLen + 1];
		//ZeroMemory(ImageInfo.Data, ImageInfo.DataLen + 1);
		//CPY_FROM_STREAM(m_pImageData, ImageInfo.Data);

		vImageInfo.push_back(ImageInfo);
	}

	// 创建texture 
	for (UINT iImage = 0; iImage < vImageInfo.size(); ++iImage)
	{
		// 遍历所有的texture
		UINT textureCount = m_pRoot->GetTextureManager().getResourceCount();
		for (UINT iTexture = 0; iTexture < textureCount; ++iTexture)
		{
			CTexture *pTexture = (CTexture*)m_pRoot->GetTextureManager().getResource(iTexture);
			if (pTexture && !pTexture->bCreated && pTexture->imageIdx == iImage)
			{
				N3D_ImageInfo &imageInfo = vImageInfo[iImage];
				m_pRoot->GetTextureManager().createTextureFromBuffer(pTexture, imageInfo.Data, imageInfo.DataLen, pTexture->isOpacity);
			}
		}
	}

	vImageInfo.clear();
}

//-------------------------------------------------------------------
//方法: ReadTexData()
//参数: 
//用途: 读取贴图信息
//-------------------------------------------------------------------
VOID CN3DLoaderImpV2::ReadModelData()
{
//#if _DEBUG
//	FILE *pTmpFile = fopen("d:\\indexBufferVS9.txt", "wt");
//#endif

	BYTE* pOffset = m_pSceneData + TableData.meshDataPtr;

	//之前的模型数量
	m_preModelCnt = m_pScene->model.size();	

	// 得到场景中模型的数量
	UINT modelCount = 0;
	CPY_FROM_STREAM(modelCount, pOffset);

	std::vector<CVGSModelData> vModelData(modelCount);
   
	//模型的材质是否有透明信息
	BOOL bTransModel = FALSE; 


	//将Mesh信息转换为模型信息
	for (UINT i = 0; i < modelCount; i++)
	{
		CVGSModelData &curModelData = vModelData[i];
		curModelData.mesh.faceCnt = 0;

		//模型名称
		UINT nameLen = 0;
		CPY_FROM_STREAM(nameLen, pOffset);

		char* name = new char[nameLen + 1];
		ZeroMemory(name, nameLen + 1);
        memcpy(name , pOffset, nameLen);
		pOffset += nameLen;
		CModel *pModel = m_pScene->CreateModel(std::string(name));
		SAFE_DELETE_ARRAY(name);

		if (!pModel)
		{
			continue;
		}

		// 模型编号
		pModel->index = m_preModelCnt + i;

		// fvf
		/*if (m_fN3DFileVersion >= _N3D_FLOATVERSION(52000))
		{
			CPY_FROM_STREAM(curModelData.mesh.fvf, pOffset);
		}
		else*/
		{
			curModelData.mesh.fvf = FVF_POSNORMALCOLORCOORD4;
		}

        // 模型的顶点数量
		CPY_FROM_STREAM(curModelData.mesh.vertexCnt, pOffset);
		switch (curModelData.mesh.fvf)
		{
		case FVF_POSNORMALCOLORCOORD1:
			{
				CVertex_POSNORMALCOLORCOORD1 *pVertexBuffer = new CVertex_POSNORMALCOLORCOORD1[curModelData.mesh.vertexCnt];
				curModelData.mesh.pVertex = (void*)pVertexBuffer;
				for (UINT j = 0; j < curModelData.mesh.vertexCnt; j++)
				{
					CPY_FROM_STREAM(pVertexBuffer[j].position, pOffset);
					pVertexBuffer[j].position.z *= -1;
					CPY_FROM_STREAM(pVertexBuffer[j].normal, pOffset);
					pVertexBuffer[j].normal.z *= -1;
					CPY_FROM_STREAM(pVertexBuffer[j].color, pOffset);
					CPY_FROM_STREAM(pVertexBuffer[j].uv1, pOffset);
				}
				break;
			}
		case FVF_POSNORMALCOLORCOORD2:
			{
				CVertex_POSNORMALCOLORCOORD2 *pVertexBuffer = new CVertex_POSNORMALCOLORCOORD2[curModelData.mesh.vertexCnt];
				curModelData.mesh.pVertex = (void*)pVertexBuffer;
				for (UINT j = 0; j < curModelData.mesh.vertexCnt; j++)
				{
					CPY_FROM_STREAM(pVertexBuffer[j].position, pOffset);
					pVertexBuffer[j].position.z *= -1;
					CPY_FROM_STREAM(pVertexBuffer[j].normal, pOffset);
					pVertexBuffer[j].normal.z *= -1;
					CPY_FROM_STREAM(pVertexBuffer[j].color, pOffset);
					CPY_FROM_STREAM(pVertexBuffer[j].uv1, pOffset);
					CPY_FROM_STREAM(pVertexBuffer[j].uv2, pOffset);
				}
				break;
			}
		case FVF_POSNORMALCOLORCOORD3:
			{
				CVertex_POSNORMALCOLORCOORD3 *pVertexBuffer = new CVertex_POSNORMALCOLORCOORD3[curModelData.mesh.vertexCnt];
				curModelData.mesh.pVertex = (void*)pVertexBuffer;
				for (UINT j = 0; j < curModelData.mesh.vertexCnt; j++)
				{
					CPY_FROM_STREAM(pVertexBuffer[j].position, pOffset);
					pVertexBuffer[j].position.z *= -1;
					CPY_FROM_STREAM(pVertexBuffer[j].normal, pOffset);
					pVertexBuffer[j].normal.z *= -1;
					CPY_FROM_STREAM(pVertexBuffer[j].color, pOffset);
					CPY_FROM_STREAM(pVertexBuffer[j].uv1, pOffset);
					CPY_FROM_STREAM(pVertexBuffer[j].uv2, pOffset);
					CPY_FROM_STREAM(pVertexBuffer[j].uv3, pOffset);
				}
				break;
			}
		case FVF_POSNORMALCOLORCOORD4:
			{
				CVertex_POSNORMALCOLORCOORD4 *pVertexBuffer = new CVertex_POSNORMALCOLORCOORD4[curModelData.mesh.vertexCnt];
				curModelData.mesh.pVertex = (void*)pVertexBuffer;
				for (UINT j = 0; j < curModelData.mesh.vertexCnt; j++)
				{
					CPY_FROM_STREAM(pVertexBuffer[j].position, pOffset);
					pVertexBuffer[j].position.z *= -1;
					CPY_FROM_STREAM(pVertexBuffer[j].normal, pOffset);
					pVertexBuffer[j].normal.z *= -1;
					CPY_FROM_STREAM(pVertexBuffer[j].color, pOffset);
					CPY_FROM_STREAM(pVertexBuffer[j].uv1, pOffset);
					CPY_FROM_STREAM(pVertexBuffer[j].uv2, pOffset);
					CPY_FROM_STREAM(pVertexBuffer[j].uv3, pOffset);
					CPY_FROM_STREAM(pVertexBuffer[j].uv4, pOffset);
				}
				break;
			}
		default:
			break;
		} // switch
		
        //SubMesh数量
		unsigned short curModelSubMeshCnt = 0;
		CPY_FROM_STREAM(curModelSubMeshCnt, pOffset);
		curModelData.mesh.subMeshCount = curModelSubMeshCnt;
		CVGSSubMesh *pVGSSubmesh = new CVGSSubMesh[curModelData.mesh.subMeshCount];
		curModelData.mesh.pSubMesh = (void*)pVGSSubmesh;


        bTransModel = FALSE;
		//SubMesh列表
		for (UINT j = 0; j < curModelData.mesh.subMeshCount; j++)
		{
			CVGSSubMesh &subMesh = pVGSSubmesh[j];  // 当前的submesh

			CPY_FROM_STREAM(subMesh.mtrIndex, pOffset);  //SubMesh使用的材质编号			
				
			if (subMesh.mtrIndex >= 0)
			{
				//当前SubMesh使用的材质
				int mtrlIndex = subMesh.mtrIndex;
				CMaterial *pMaterial = _GetHandleByIndex(m_vMaterial, mtrlIndex);//subMesh.oriShader = m_pScene->shader[mtrlIndex];
				if (/*subMesh.shader->bAlphaBlend||*/pMaterial->getOpacity() < 100)  // fuxb 2009-03-06, alphaBlend不参与排序
				{
					bTransModel = TRUE;
				}
				m_pScene->AddMaterial(VGS_MATERIAL_TYPE_3D, pMaterial);
			}

			// 面索引
			UINT IndexCount = 0;
			CPY_FROM_STREAM(IndexCount, pOffset);
			// curModelData.mesh.faceCnt = IndexCount / 3;
			subMesh.faceCount = IndexCount / 3;
			curModelData.mesh.faceCnt += subMesh.faceCount;  // 总的面数
			subMesh.pFace = new CVGSFace[subMesh.faceCount];  // face buffer

			// 顶点顺序为逆时针
			if (curModelData.mesh.vertexCnt > 65535)  // 用四个字节的缓冲
			{
				// 一次性读取缓冲数据
				UINT *pBuffer = new UINT[IndexCount];
				memcpy(pBuffer, pOffset, sizeof(UINT) * IndexCount);
				// memcpy((void*)subMesh.pFace, pOffset, sizeof(UINT) * IndexCount);
				pOffset += sizeof(UINT) * IndexCount;

				UINT iVertexIndex = 0;
				for (UINT n = 0; n < subMesh.faceCount; n++)
				{
					subMesh.pFace[n].a = pBuffer[iVertexIndex++];
					subMesh.pFace[n].b = pBuffer[iVertexIndex++];
					subMesh.pFace[n].c = pBuffer[iVertexIndex++];
				}
			}
			else  // 用两个字节的缓冲
			{
				// 一次性读取缓冲数据
				unsigned short *pBuffer = new unsigned short[IndexCount];
				memcpy(pBuffer, pOffset, sizeof(unsigned short) * IndexCount);
				pOffset += sizeof(unsigned short) * IndexCount;

//#if _DEBUG
//				unsigned short tmp[20000];
//				memcpy((void*)tmp, (void*)pBuffer, min(20000 * sizeof(unsigned short), sizeof(unsigned short) * IndexCount));
//#endif
				
				UINT iVertexIndex = 0;  
				for (UINT n = 0; n < subMesh.faceCount; n++) 
				{
					subMesh.pFace[n].a = pBuffer[iVertexIndex++];
					subMesh.pFace[n].b = pBuffer[iVertexIndex++];
					subMesh.pFace[n].c = pBuffer[iVertexIndex++];

					/*if (subMesh.pFace[n].a > IndexCount || subMesh.pFace[n].b > IndexCount || subMesh.pFace[n].c > IndexCount)
					{
						::MessageBox(0, TEXT("index buffer 越界"), TEXT(""), 0);
					}*/
				}	

//#if _DEBUG
//				DWORD tmp2[20000];
//				ZeroMemory(tmp2, 20000 * sizeof(DWORD));
//				memcpy((void*)tmp2, (void*)(&subMesh.pFace[0]), min(20000 * sizeof(DWORD), sizeof(DWORD) * IndexCount));
//
//
//				fwrite(pModel->getName().c_str(), pModel->getName().length(), 1, pTmpFile);	
//				fwrite("\\r\\n", sizeof("\\r\\n"), 1, pTmpFile);
//				std::string sSpace = " ";
//				for (UINT iIndex = 0; iIndex < IndexCount; iIndex++)
//				{
//					fwrite(&tmp2[iIndex], sizeof(DWORD), 1, pTmpFile);
//					fwrite(sSpace.c_str(), sSpace.length(), 1, pTmpFile);					
//				}
//				fwrite("\\r\\n", sizeof("\\r\\n"), 1, pTmpFile);
//#endif
				SAFE_DELETE_ARRAY(pBuffer);
			} // if indexcount
		} // for submesh

		// 创建模型
		std::vector<CMaterial*> vMaterial;
		m_pScene->getMaterialList(vMaterial);
		pModel->Create(&curModelData.mesh, vMaterial);

		// SAFE_DELETE_ARRAY(pVertex);
			
		if (m_fN3DFileVersion < _N3D_FLOATVERSION(52000))
		{
			// 包围盒
			N3D_BoundBox boundBox;
			memcpy(&(boundBox), pOffset, sizeof(boundBox));
			pOffset += sizeof(boundBox);
			//pModel->boundingBox.maxCorner.x = boundBox.maxCorner.x;
			//pModel->boundingBox.maxCorner.y = boundBox.maxCorner.y;
			//pModel->boundingBox.maxCorner.z = -boundBox.maxCorner.z;
			
			//pModel->boundingBox.minCorner.x = boundBox.minCorner.x;
			//pModel->boundingBox.minCorner.y = boundBox.minCorner.y;
			//pModel->boundingBox.minCorner.z = -boundBox.minCorner.z;
			
			//pModel->oriBBox.maxCorner = pModel->boundingBox.maxCorner;
			//pModel->oriBBox.minCorner = pModel->boundingBox.minCorner;
			
			//pModel->pRBBox= new CRenderBoundBox(m_pScene->m_pd3dDevice);
			//pModel->pRBBox->SetRBBoxVertex(pModel->oriBBox.minCorner,pModel->oriBBox.maxCorner);
			
			//pModel->oriBSphere.Center = (pModel->oriBBox.maxCorner + pModel->oriBBox.minCorner)/2.0f;
			//pModel->boundingSphere.Center =pModel->oriBSphere.Center;
			
			//包围球
			float boundSphereRadius;
			CPY_FROM_STREAM(boundSphereRadius, pOffset);
			//pModel->boundingSphere.Radius = boundSphereRadius;
			//pModel->oriBSphere.Radius = pModel->boundingSphere.Radius;
		}		

		//isBillboard
		CPY_FROM_STREAM(curModelData.isBillboard, pOffset);
		pModel->isBillboard = curModelData.isBillboard;
		if (pModel->isBillboard)
		{
			BYTE axis = 4;  // -y
			memcpy(&axis, pOffset, sizeof(axis));
			pOffset += sizeof(axis);  
			VECTOR3 vDir = _GetAxisDirection((VGS_AXIS_TYPE)axis);
			pModel->billboardDirectionAxis = D3DXVECTOR3(vDir.x, vDir.y, vDir.z);  // 朝向相机的方向轴
		}
		
		// Collision
		CPY_FROM_STREAM(curModelData.isCollision, pOffset);
		pModel->isCollision = curModelData.isCollision;
		
		//isGround
		CPY_FROM_STREAM(curModelData.isGround, pOffset); 
		pModel->isGround = curModelData.isGround;
		if (pModel->isGround)
		{
			pModel->isCollision = TRUE;
		}
		
		//isVisible
		CPY_FROM_STREAM(curModelData.isVisible, pOffset);
		pModel->visible = curModelData.isVisible;
		
		//isClickable
		CPY_FROM_STREAM(curModelData.isClickable, pOffset);
		pModel->clickable = curModelData.isClickable;
		
		// upAxis  billboradUpAxis
		BYTE billboradUpAxis = 2;  // local z axis
		CPY_FROM_STREAM(billboradUpAxis, pOffset);
		VECTOR3 vDir = _GetAxisDirection((VGS_AXIS_TYPE)billboradUpAxis);
		pModel->billboradUpAxis = D3DXVECTOR3(vDir.x, vDir.y, vDir.z);  // 模型正对相机时的Up方向轴
		
		// isAutoNormalise
		CPY_FROM_STREAM(curModelData.isAutoNormalise, pOffset);
		pModel->isAutoNormalise = curModelData.isAutoNormalise;
		
		//isSkyBox
		CPY_FROM_STREAM(curModelData.isSkyBox, pOffset);
		pModel->isSkyBox = curModelData.isSkyBox;   
		
		if (m_fN3DFileVersion >= 5.1001f)
		{
			bool bReflection;
			CPY_FROM_STREAM(bReflection, pOffset);
			
			bool bRefrection;
			CPY_FROM_STREAM(bRefrection, pOffset);
		}
		
		int bindingSeketonIndex;
		CPY_FROM_STREAM(bindingSeketonIndex, pOffset);

		//push model
		m_pSceneResource->model.push_back(pModel);
		
		//push the transparent model
		if (bTransModel)
		{
			m_pScene->AddModelToTransparentList(pModel);
			/*if (find(m_pScene->m_vTransModel.begin(), m_pScene->m_vTransModel.end(), pModel) == m_pScene->m_vTransModel.end())
			{
				m_pScene->m_vTransModel.push_back(pModel);
			}*/
		}

		//如果模型需要辅助渲染表面，则绑定模型到辅助渲染表面——凹凸合成贴图
		BandModelToAddtionalSurface_Bump(pModel);
	}


	//重新构造材质的SubMeshList列表
	m_pSceneResource->ConstructShaderSubMeshList();

	//释放临时内存
	// SAFE_DELETE_ARRAY(ModelData.model);

//#if _DEBUG
//
//	fclose(pTmpFile);
//#endif
}

//如果模型需要辅助渲染表面，则绑定模型到辅助水波渲染表面
VOID CN3DLoaderImpV2::BandModelToAddtionalSurface_Bump(CModel* pModel)
{
// 	INT subMeshCnt = pModel->subMesh.size();
// 	_subMesh* _pSubMesh = pModel->subMesh.begin();
// 	for (INT i = 0; i< subMeshCnt; i++)
// 	{
// 		_subMesh* pSubMesh = &(*(_pSubMesh + i));
// 		TextureLayer* pTexLayer = pSubMesh->shader->IsContainBumpWaveMap();
// 		if (pTexLayer)
// 		{
// 			//判断辅助渲染表面是否已经加入到模型辅助渲染列表，防止重复加入
// 			BOOL isExist = false;
// 			INT addRenderInfoModelCnt = pModel->m_additionalRenderInfo.size();
// 			for (INT j = 0; j < addRenderInfoModelCnt; j++)
// 			{
// 				if (pModel->m_additionalRenderInfo[j].pRenderToTexture == (DWORD)(pTexLayer->pTexture))
// 				{
// 					isExist = true;
// 					break;
// 				}
// 			}
// 			//该模型需要渲染到凹凸合成贴图表面，所以增加附加渲染信息
// 			if (isExist == false)
// 			{
// 				ADDITIONALRENDERINFO renderInfo;
// 				renderInfo.type = ADS_BUMPWAVEMAP;
// 				renderInfo.pRenderToTexture = pTexLayer->pTexture;
// 				pModel->m_additionalRenderInfo.push_back(renderInfo);
// 			}
// 		}
// 	}
}

//构造场景图
VOID CN3DLoaderImpV2::ConstructSceneGraph(CSceneNode* parentNode, N3D_Node* pNode, BYTE** offset)
{
	//创建场景中的一个子节点信息----------------------------------------
	//节点的类型
	memcpy(&(pNode->type), *offset, sizeof(pNode->type));
	*offset +=sizeof(pNode->type);
	memcpy(&(pNode->index), *offset, sizeof(pNode->index));
	*offset +=sizeof(pNode->index);
	//5.1004版本添加了节点的名称
	if (m_fN3DFileVersion >= 5.1004f)
	{
		UINT len;
		memcpy(&len, *offset, sizeof(len));
		*offset +=sizeof(len);

		// char* name =new char[len + 1];
		char tmp[MAX_FILENAME_LENGTH];
		ZeroMemory(tmp, MAX_FILENAME_LENGTH);
		memcpy(tmp , *offset, len);
		*offset += len;
		//pNode-> = std::string(name);
		//SAFE_DELETE_ARRAY(name);
	}

	//数据库标记
	DWORD ID1,ID2;
	memcpy(&(ID1), *offset, sizeof(ID1));
	(*offset) +=sizeof(ID1);
	memcpy(&(ID2), *offset, sizeof(ID2));
	(*offset) +=sizeof(ID2);

	N3D_NodeTransform pNodeTransform;
	memcpy(&(pNodeTransform), *offset, sizeof(pNodeTransform));
	(*offset) +=sizeof(pNodeTransform);

	//创建节点对象
	CSceneNode* pSceneNode = new CSceneNode();
	//节点的类型
	//pSceneNode-> =(N3D_NODETYPE)pNode->type;

	//pSceneNode->name = pNode->name;

	//如果有父节点
	if(parentNode)
	{
		/*(parentNode)->m_child.push_back(pSceneNode);
		pSceneNode->m_parent =(parentNode);*/
		pSceneNode = parentNode->CreateChild();
	}

	//如果是根节点
	if (pNode->type == NNT_SCENEROOT)
	{
		m_pSceneResource->m_pRootNode = pSceneNode;
	}


	//计算节点的变换矩阵-------------------------------------------------------
	//这部分比较复杂，N3D文件保存的是右手坐标系，要转化为左手坐标系
	//-------------------------------------------------------------------------
	//位置, Z取反
	D3DXVECTOR3 pos = D3DXVECTOR3(pNodeTransform.position.x,
									pNodeTransform.position.y, 
									-pNodeTransform.position.z);
	//转角, XY取反
	D3DXQUATERNION quat = D3DXQUATERNION(-pNodeTransform.quat.x, 
										-pNodeTransform.quat.y, 
										pNodeTransform.quat.z, 
										pNodeTransform.quat.w);

	//缩放
	D3DXVECTOR3 scale = D3DXVECTOR3(1, 1, 1);
	if (pNode->type == NNT_MODEL)
	{
		scale = D3DXVECTOR3(pNodeTransform.scale.x, pNodeTransform.scale.y, pNodeTransform.scale.z);
	}

	// 变换矩阵
	D3DXMATRIX transMat;
	//获得变换矩阵
	D3DXMatrixTransformation(&transMat, &D3DXVECTOR3(0,0,0), &D3DXQUATERNION(0, 0, 0, 0), 
							&scale, &D3DXVECTOR3(0,0,0), &quat, &pos);

	pSceneNode->setMatrix(transMat, VTS_PARENT);

	//根节点
	//if (pNode->type == NNT_SCENEROOT)
	//{
	//	/*pSceneNode->curTransform.matrix      = m_pSceneResource->m_transform;
	//	pSceneNode->curTransform.worldMatrix = m_pSceneResource->m_transform;
	//	pSceneNode->updateTransform();*/
	//	pSceneNode->setMatrix(m_pSceneResource->m_transform, VTS_PARENT);
	//}
	//else //其他节点
	//{
	//	/*pSceneNode->curTransform.matrix = transMat;
	//	pSceneNode->updateTransform();*/
	//	pSceneNode->setMatrix(m_pSceneResource->m_transform, VTS_PARENT);
	//}

	//是否有动画
	float animationLength = 0.0f;
	memcpy(&(animationLength), (*offset), sizeof(animationLength));
	(*offset) +=sizeof(animationLength);

	//存在关键帧动画
	int keyFrameCnt = 0;
	std::vector<CNodeTransform> vTransformKeyframe;
	//std::vector<CVGS_CAMERA_ANIMINFO> vCamInfo;
	if (animationLength > 0)
	{
		CPY_FROM_STREAM(keyFrameCnt, *offset);
		vTransformKeyframe.resize(keyFrameCnt);  // 相机节点变换
		//vCamInfo.resize(keyFrameCnt);            // 相机参数变化

		for (int i = 0 ; i < keyFrameCnt; ++i)
		{
			N3D_NodeTransform pNodeTransform;
			CPY_FROM_STREAM(pNodeTransform, (*offset));

			CNodeTransform &transform = vTransformKeyframe[i]; // 关键帧信息
			transform.position.x = pNodeTransform.position.x;
			transform.position.y = pNodeTransform.position.y;
			transform.position.z = -pNodeTransform.position.z;
			transform.quat.x     = -pNodeTransform.quat.x;
			transform.quat.y     = -pNodeTransform.quat.y;
			transform.quat.z     = pNodeTransform.quat.z;
			transform.quat.w     = pNodeTransform.quat.w;
			transform.scale      = pNodeTransform.scale;
			if (pNode->type != NNT_MODEL)
			{
				transform.scale = vector3(1, 1, 1);
			}
		} // for each keyframe  
		// if 存在关键帧动画
	}
	else
	{
		// pSceneNode->keyFrameCnt =0;
		//pSceneNode->kfrtransform = NULL;
	}


	switch (pNode->type)
	{
	case NNT_SCENEROOT:
		break;
	case NNT_CAMERANODE:
		{
			// 相机参数动画
			//std::vector<CVGS_CAMERA_ANIMINFO> vCamInfo;
			//vCamInfo.resize(keyFrameCnt);
			if (pNode->index >=0 && (pNode->index + m_preCameraCnt) < m_pScene->getCameraCount())
			{
				CCamera* pCamera = m_pScene->GetCameraByIndex(pNode->index + m_preCameraCnt);

				pSceneNode->attachObject(pCamera);

				//将Camera的pos,Quat,rot信息转换成老版本的from,At,Roll
				//相机的from点是相机的WorldPosition
				//相机的At点是(0,0,-1)*相机的旋转矩阵*固定长+from点
				//相机的Roll是相机旋转矩阵的Z方向
				D3DXVECTOR3 Origin_from;
				D3DXVECTOR4 transVec;
				D3DXVECTOR3 Origin_at,Origin_up;

				Origin_from   = pSceneNode->getPosition(VTS_WORLD);

				D3DXVECTOR3 dir = D3DXVECTOR3(0, 0, 100);
				D3DXVec3TransformNormal(&Origin_at, &dir, &pSceneNode->getMatrix(VTS_WORLD));
				D3DXVec3Add(&Origin_at, &Origin_from, &Origin_at);

				D3DXMATRIX mat = pSceneNode->getMatrix(VTS_WORLD);
				Origin_up = D3DXVECTOR3(mat._21, mat._22, mat._23); 

				pCamera->SetPosition(&Origin_from, VTS_WORLD);
				pCamera->Lookat(&Origin_at, VTS_WORLD);
				pCamera->SetRoll(0);
				
				// 相机参数动画
				//for (UINT i = 0; i < keyFrameCnt; i++)
				//{
				//	CVGS_CAMERA_ANIMINFO &camInfo = vCamInfo[i];
				//	// CNodeTransform &transform = vTransformKeyframe[i];

				//	camInfo.fov  = pCamera->GetFov();
				//	camInfo.nearClip = pCamera->getNearClip();
				//	camInfo.farClip  = pCamera->getFarClip();
				//}

				//set the camera Keyframe data
				//pCamera->kfrCnt = 0;

			} // if pNode->index	

			std::vector<CVGS_CAMERA_ANIMINFO> vCamInfo;
			m_pRoot->GetAnimManager().CreateCameraAnimation(pSceneNode, vTransformKeyframe, vCamInfo);

			//pCamera->pNode = pSceneNode;
		} 
		break;
	case NNT_LIGHTNODE:
		{
			if (pNode->index >= 0 && (pNode->index + m_preLightCnt) < m_pScene->getLightCount())
			{
				CLight* pLight = m_pScene->GetLightByIndex(pNode->index + m_preLightCnt);
				pSceneNode->attachObject(pLight);
				pSceneNode->name = std::string(pLight->getName());

				//pLight->m_pNode  = pSceneNode;
				// pLight->kfrCnt = pSceneNode->keyFrameCnt;  // 关键帧数目
				//pLight-> = 0;
				//pLight->endFrame   = pLight->kfrCnt - 1;
				// pLight->kfrCurFrame = 0;

				//更新灯光数据，设置灯光的位置
				pLight->param.Position = pLight->m_pNode->getPosition(VTS_WORLD);
				m_pScene->m_pd3dDevice->SetLight(pNode->index, &(pLight->param));		
				m_pScene->m_pd3dDevice->LightEnable(pNode->index, TRUE);	

				// light 关键帧
				std::vector<CVGS_LIGHT_INFO> vLightInfo;
				vLightInfo.resize(keyFrameCnt);
				N3DLightParamData lightParamData = getLightParamFrameData(pLight); 
				bool bHasParamAnim = true;
				for (UINT iFrame = 0; iFrame < keyFrameCnt; ++iFrame)
				{
					CVGS_LIGHT_INFO &lightParamInfo = vLightInfo[iFrame];
					if (pLight->getType() == VLT_OMNI)
					{
						if (lightParamData.vOmni.size() != keyFrameCnt)
						{
							bHasParamAnim = false;
							break;
							/*lightParamInfo.color    = D3DCOLOR_COLORVALUE(pLight->diffuse.a, pLight->diffuse.r, pLight->diffuse.g, pLight->diffuse.b);
							lightParamInfo.multiply = pLight->multiply;
							lightParamInfo.range2   = pLight->param.Range;*/
						}
						else
						{
							lightParamInfo.color = lightParamData.vOmni[iFrame].color;
							lightParamInfo.multiply = lightParamData.vOmni[iFrame].multiply;
							lightParamInfo.range2 = lightParamData.vOmni[iFrame].attenuationRange;
						}
					}
					else if (pLight->getType() == VLT_SPOT)
					{
						if (lightParamData.vSpot.size() != keyFrameCnt)
						{
							bHasParamAnim = false;
							break;
							/*lightParamInfo.color    = D3DCOLOR_COLORVALUE(pLight->diffuse.a, pLight->diffuse.r, pLight->diffuse.g, pLight->diffuse.b);
							lightParamInfo.multiply = pLight->multiply;
							lightParamInfo.range2   = pLight->param.Range;
							lightParamInfo.dir = VECTOR3(pLight->direction.x, pLight->direction.y, pLight->direction.z);
							lightParamInfo.theta = pLight->param.Theta;
							lightParamInfo.phi = pLight->param.Phi;*/
						}
						else
						{
							lightParamInfo.color = lightParamData.vSpot[iFrame].color;
							lightParamInfo.multiply = lightParamData.vSpot[iFrame].multiply;
							lightParamInfo.range2 = lightParamData.vSpot[iFrame].attenuationRange;
							lightParamInfo.dir = lightParamData.vSpot[iFrame].direction;
							lightParamInfo.theta = lightParamData.vSpot[iFrame].innerAngle;
							lightParamInfo.phi = lightParamData.vSpot[iFrame].outerAngle;
						}

					}
					else if (pLight->getType() == VLT_DIR)
					{
						if (lightParamData.vDir.size() != keyFrameCnt)
						{
							bHasParamAnim = false;
							break;
							/*lightParamInfo.color    = D3DCOLOR_COLORVALUE(pLight->diffuse.a, pLight->diffuse.r, pLight->diffuse.g, pLight->diffuse.b);
							lightParamInfo.multiply = pLight->multiply;
							lightParamInfo.range2   = pLight->param.Range;
							lightParamInfo.dir      = VECTOR3(pLight->direction.x, pLight->direction.y, pLight->direction.z);*/
						}
						else
						{
							lightParamInfo.color = lightParamData.vDir[iFrame].color;
							lightParamInfo.multiply = lightParamData.vDir[iFrame].multiply;
							lightParamInfo.range2 = lightParamData.vDir[iFrame].attenuationRange;
							lightParamInfo.dir = lightParamData.vDir[iFrame].direction;
						}

					}
				}

				// 创建灯光动画
				if (bHasParamAnim)
				{
					vLightInfo.clear();
				}
				m_pRoot->GetAnimManager().CreateLightAnimation(pSceneNode, vTransformKeyframe, pLight->getType(), vLightInfo);
			}
		}

		break;
	case NNT_MODEL:
		{
			if ((pNode->index + m_preModelCnt)< m_pScene->GetModelCount())
			{
				CModel* pModel = m_pScene->GetModelByIndex(pNode->index + m_preModelCnt);
				if (pModel)
				{
					pSceneNode->attachObject(pModel);
				}
				
				// 创建模型关键帧动画
				m_pRoot->GetAnimManager().CreateModelAnimation(pSceneNode, vTransformKeyframe);
			}
		}
		break;
	}

	// 创建子节点
	UINT childCnt =0;
	memcpy(&(childCnt), (*offset), sizeof(childCnt));
	(*offset) +=sizeof(childCnt);
	for (UINT i =0 ;i< childCnt;i++)
	{
		ConstructSceneGraph(pSceneNode,pNode, offset);	      
	}  
}


VOID CN3DLoaderImpV2::ReadSceneGraph()
{
 	BYTE* pOffset = m_pSceneData + TableData.sceneGraphDataPtr;
	N3D_Node* pRootNode = new N3D_Node;
	CSceneNode *pNode = m_pScene->GetRootSceneNode();
 	ConstructSceneGraph(pNode, pRootNode, &pOffset);
 	SAFE_DELETE(pRootNode);
}

//-------------------------------------------------------------------
//方法: Release()
//参数: 
//用途: 释放过渡数据
//-------------------------------------------------------------------
VOID CN3DLoaderImpV2::Release()
{	
	SAFE_DELETE_ARRAY(N3DBuffer);
// 	SAFE_DELETE_ARRAY(m_pSceneData);
// 	if (pDebugFile)
// 		fclose(pDebugFile);
}

VOID CN3DLoaderImpV2::ReadButtonData()
{
	BYTE* pOffset = m_pSceneData +  TableData.ButtonPtr;

	m_preButtonCnt = m_pScene->m_vButton.size();

	// 读入Button对象数量
	UINT btnCount = 0;
	CPY_FROM_STREAM(btnCount, pOffset);
	if (btnCount <= 0)
	{
		return;
	}

	// 读入所有button信息
	for (UINT iBtn = 0; iBtn < btnCount; ++iBtn)
	{
		N3D_Button Info;
		// 读取button基本信息
		CPY_FROM_STREAM(Info.BasalInfo, pOffset);

		//读入pbutton的名字	
		std::string tmp = std::string(Info.BasalInfo.Name);
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), toupper); // 转换成大写

		RECT rect;
		rect.left = rect.top = 0;
		rect.right = rect.bottom = 100;
		CButton *pButton = (CButton*)m_pScene->GetGuiManagerRef().CreateButton(tmp.c_str(), rect);

		//读入navigate的mapOverlay的Index
		if (Info.BasalInfo.BtnOverlayIndex != -1 && Info.BasalInfo.BtnOverlayIndex < m_vN3DOverlayData.size())
		{
			N3DOverlayData &Overlaydata = m_vN3DOverlayData[Info.BasalInfo.BtnOverlayIndex];  // overlay 信息
			N3D_Overlay  &overlayInfo = Overlaydata.overlayInfo;

			CopyOverlayInfo(pButton, overlayInfo);  // 设置navigator的属性
			Overlaydata.bCreated = TRUE;  // 已经创建
		}

		//读入button的回调函数名称
		pButton->SetMouseLUpFuncName(std::string(Info.BasalInfo.CallBackFunc));

		//button MouseUp Mtrl 
		pButton->SetMaterial(0, ((COverlay*)pButton)->GetMaterial());

		//读入button的mouseOver Mtrl的Index
		pButton->SetMaterial(2, pButton->GetMaterial(0));
		if (Info.BasalInfo.MouseOverMtrlIndex != -1)
		{
			INT MouseOverMtrlIndex = Info.BasalInfo.MouseOverMtrlIndex;
			if (MouseOverMtrlIndex >= 0 && MouseOverMtrlIndex < m_vMaterial.size())
			{
				CMaterial* pMtrl = _GetHandleByIndex(m_vMaterial, MouseOverMtrlIndex);
				pButton->SetMaterial(2, pMtrl);
				m_pScene->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);
			}
		}

		//读入button的mouseDown Mtrl的Index
		pButton->SetMaterial(1, pButton->GetMaterial(0)); 
		if (Info.BasalInfo.MouseDownMtrlIndex != -1)
		{
			INT MouseDownMtrlIndex = Info.BasalInfo.MouseDownMtrlIndex;
			if (MouseDownMtrlIndex >= 0 && MouseDownMtrlIndex < m_pScene->getMaterialCount())
			{
				CMaterial* pMtrl = _GetHandleByIndex(m_vMaterial, MouseDownMtrlIndex);
				pButton->SetMaterial(1, pMtrl);
				m_pScene->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);
			}
		}

		//读入Button的关键帧数量
		CPY_FROM_STREAM(Info.KfrCnt, pOffset);

		//将pButton Push到image列表
		m_pScene->m_vButton.push_back(pButton);
		m_pSceneResource->button.push_back(pButton);
	}
}
VOID CN3DLoaderImpV2::ReadNavigaterData()
{
	BYTE* pOffset = m_pSceneData +  TableData.NavigatotPtr;

	// m_preNaviCnt = m_pScene->button.size();  // 场景中已经存在的导航图数目

	// 读入Navigator对象数量
	UINT naviCount = 0;
	CPY_FROM_STREAM(naviCount, pOffset);
	if (naviCount <= 0)
	{
		return;
	}

	// 读入所有Navigator信息
	for (UINT iNavi = 0; iNavi < naviCount; ++iNavi)
	{
		N3D_Navigater Info;

		// 读取navigator的基本信息
		CPY_FROM_STREAM(Info.BasalInfo, pOffset);

		//读入pNavigater的名字
		std::string tmp = std::string(Info.BasalInfo.Name);
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), toupper); // 转换成大写
		// strncpy(pNavigater->name, tmp.c_str(), tmp.length());
		
		CNavigator *pNavigator = (CNavigator*)m_pScene->GetGuiManagerRef().CreateNavigator(tmp.c_str()); // 创建navigator		

		//读入navigate的mapOverlay的Index
		if (Info.BasalInfo.MapOverlayIndex != -1 && Info.BasalInfo.MapOverlayIndex < m_vN3DOverlayData.size())
		{
			N3DOverlayData &Overlaydata = m_vN3DOverlayData[Info.BasalInfo.MapOverlayIndex];  // overlay 信息
			N3D_Overlay  &overlayInfo = Overlaydata.overlayInfo;

			CopyOverlayInfo(pNavigator, overlayInfo);  // 设置navigator的属性
			Overlaydata.bCreated = TRUE;
		}

		//读入Pointer的数量
		CPY_FROM_STREAM(Info.PointerCnt, pOffset);

		//读入Pointer数据
		INT pointerCnt = Info.PointerCnt;	
		if (pointerCnt > 0)
		{
			for (INT j = 0; j < pointerCnt; j++)
			{
				N3D_NavigaterPointer PointInfo;

				CPY_FROM_STREAM(PointInfo, pOffset);

				//Pointer名称
				std::string tmp = std::string(PointInfo.Name);
				std::transform(tmp.begin(), tmp.end(), tmp.begin(), toupper); // 转换成大写

				CNavPointer* pNavPointer = pNavigator->AddNavPointer(tmp.c_str());

				//Pointer Overlay编号
				if (PointInfo.OverlayIndex != -1 && PointInfo.OverlayIndex < m_vN3DOverlayData.size())
				{
					N3DOverlayData &OverlayData = m_vN3DOverlayData[PointInfo.OverlayIndex];
					N3D_Overlay &overlayInfo = OverlayData.overlayInfo;

					CopyOverlayInfo(pNavPointer, overlayInfo); // 设置导航点的属性
					OverlayData.bCreated = TRUE;  // 已经创建
				}

				//Pointer代表的对象类型
				pNavPointer->SetNavType(PointInfo.Type);

				//Pointer代表的对象的编号
				CModel* pModel = NULL;
				CCamera* pCamera = NULL;
				BOOL addPointer = TRUE;
				if (pNavPointer->GetNavType() == 0 && PointInfo.ObjIndex != -1)
				{
					INT index = PointInfo.ObjIndex + m_preCameraCnt;
					if (index >= 0 && index < m_pScene->getCameraCount())
					{
						CCamera* pCamera = m_pScene->GetCameraByIndex(index);
						pNavPointer->SetPointerHandle((DWORD)pCamera);
					}
					else
					{
						MessageBox(NULL, TEXT("导航点相机为空"), TEXT("未指定导航点相机"),MB_OK|MB_ICONEXCLAMATION);
						addPointer = FALSE;
						pNavPointer->SetPointerHandle(0);
					}
				}
				else if (pNavPointer->GetNavType() == 1 && PointInfo.ObjIndex != -1)
				{
					INT index = PointInfo.ObjIndex + m_preModelCnt;
					if (index >= 0 && index < m_pScene->GetModelCount())
					{
						CModel* pModel = m_pScene->model[index];
						pNavPointer->SetPointerHandle((DWORD)pModel);
					}
					else
					{
						MessageBox(NULL, TEXT("导航点模型为空"), TEXT("未指定导航点模型"),MB_OK|MB_ICONEXCLAMATION);
						addPointer = FALSE;
						pNavPointer->SetPointerHandle(0);
					}
				}

			}  // for pointerCnt

		} // if pointerCnt

		//读入navigate的navRect
		pNavigator->SetNavRange(FRect(Info.BasalInfo.sceneLeft, Info.BasalInfo.sceneTop, 
			                          Info.BasalInfo.sceneRight, Info.BasalInfo.sceneBottom));

		//将 navigater Push到 navigater 列表
		m_pScene->m_vNavigater.push_back(pNavigator);
		m_pSceneResource->navigater.push_back(pNavigator);
	}
}

// 创建还没有创建的Overlay
VOID CN3DLoaderImpV2::CreateOverlay()
{
	for (UINT i = 0; i < m_vN3DOverlayData.size(); ++i)
	{		
		if (!m_vN3DOverlayData[i].bCreated)
		{
			N3D_Overlay &OverlayInfo = m_vN3DOverlayData[i].overlayInfo;
			COverlay *pOverlay = NULL;
			if (OverlayInfo.BasalInfo.IsBackdrop)
			{
				pOverlay = (COverlay*)m_pScene->GetGuiManagerRef().CreateBackGroudLayer();
			}
			else
			{
				pOverlay = (COverlay*)m_pScene->GetGuiManagerRef().CreateOverLayer(OverlayInfo.BasalInfo.Name);
			}
			if (pOverlay)
			{
				CopyOverlayInfo(pOverlay, OverlayInfo);
			}
			m_vN3DOverlayData[i].bCreated = TRUE;
		}
	}
}

// 从N3D_Overlay中获取Overlay属性
VOID CN3DLoaderImpV2::CopyOverlayInfo(COverlay *pOverlay, const N3D_Overlay &overlayInfo)
{
	//写入Overlay的偏移坐标原点标示
	pOverlay->SetOffsetOrigin((VGSOFFSETORIGIN)overlayInfo.BasalInfo.OffsetOrigin);

	//读入Overlay的位置标示(相对坐标还是绝对坐标)
	pOverlay->SetUsedRelativePos((bool)overlayInfo.BasalInfo.UseRelativePos);

	//读入Overlay是否使用设定的绝对尺寸
	pOverlay->SetUsedRelativeSize((bool)overlayInfo.BasalInfo.UseRelativeSize);

	//读入Overlay尺寸
	RECT Rect = overlayInfo.BasalInfo.Rect; // 保存的始终是绝对位置和大小		
	pOverlay->SetRect(overlayInfo.BasalInfo.Rect);   // 计算相对尺寸

	//读入Overlay转角
	FLOAT angle = overlayInfo.BasalInfo.Angle;
	pOverlay->SetRotation(angle);

	//是否是背景图
	pOverlay->SetVisibility((bool)overlayInfo.BasalInfo.IsVisible);

	//51003版本后添加了overlay的Zorder
	if (m_fN3DFileVersion >= 5.1003f)
	{
		//CPY_FROM_STREAM(pOverlay->m_bRelativeSize, pOffset);
	}
	//读入Overlay的材质ID
	if (overlayInfo.BasalInfo.MaterialIndex != -1)
	{
		UINT index = overlayInfo.BasalInfo.MaterialIndex;
		pOverlay->SetMaterial(m_pScene->GetMaterialByIndex(index)); //*(m_pScene->shader.begin() + index);
		// pOverlay->m_pMtrl->bIsOverlayMtrl = TRUE;

		//设置Overlay用到的贴图的MipMap为1
		int texLayerCnt = pOverlay->GetMaterial()->getTexureLayerCount();
		//材质有贴图，则遍历贴图层列表逐层按类型贴图并混色
		if (texLayerCnt > 0)
		{
			for(int i = 0; i< texLayerCnt; i++)
			{
				CTextureLayer* pTexLayer = pOverlay->GetMaterial()->getTextureLayerByIndex(i);
				CTexture* pTex = (CTexture*)(pTexLayer->pTexture);
				pTex->MipCount = 1;
			}
		}

		pOverlay->GetMaterial()->selfIllumination = 100;
		//pOverlay->m_imageWidth = Rect.right - Rect.left;
		//pOverlay->m_imageHeight = Rect.bottom - Rect.top;
	}

	//读入Overlay的透明度
	pOverlay->SetOpacity(overlayInfo.BasalInfo.Opacity);
}

VOID CN3DLoaderImpV2::ReadControlData()
{
	BYTE* pOffset = m_pSceneData +  TableData.ControlPtr;

	// 读入控制对象数量
	UINT controlCount = 0;
	CPY_FROM_STREAM(controlCount, pOffset);
	if (controlCount <= 0)
	{
		return;
	}

	INT preWalkerCnt = m_pScene->GetWalkerCount();
	INT preEditorCnt = m_pScene->GetEditorCount();
	INT preFlyerCnt  = m_pScene->GetFlyerCount();

	// Walker的数目及列表
	UINT WalkerCount = 0;
	CPY_FROM_STREAM(WalkerCount, pOffset);
	for (UINT iWalker = 0; iWalker < WalkerCount; ++iWalker)
	{
		N3D_Walker Info;
		CPY_FROM_STREAM(Info, pOffset);

		// CWalker* pWalker = new CWalker;

		// pWalker->SetName(Info.Name);

		// 检查cameraIndex的合理性
		INT camIndx = Info.CameraIndex + preWalkerCnt;
		std::string sCamName = m_pScene->GetCameraNameByIndex(camIndx);
		// pWalker->SetCamera(m_pScene->GetCameraByIndex(camIndx));

		WalkerInfo walkerInfo;
		ZeroMemory(&walkerInfo, sizeof(walkerInfo));
		strncpy_s(walkerInfo.Name, sizeof(walkerInfo.Name), Info.Name, sizeof(Info.Name));
		walkerInfo.bCollision = Info.bCollision;
		walkerInfo.bWeight    = Info.bWeight;
		walkerInfo.colDis     = Info.colDis;
		strncpy_s(walkerInfo.Camera, sizeof(walkerInfo.Camera), sCamName.c_str(), sCamName.length());
		walkerInfo.height     = Info.height;
		walkerInfo.liftAcc    = Info.liftAcc;
		walkerInfo.liftSpeed  = Info.liftSpeed;
		walkerInfo.rosAcc     = Info.rosAcc;
		walkerInfo.rosSpeed   = Info.rosSpeed;
		walkerInfo.upperAngle = Info.upperAngle;
		walkerInfo.walkAcc    = Info.walkAcc;
		walkerInfo.walkSpeed  = Info.walkSpeed;

		// pWalker->SetWalkerInfo(walkerInfo);  // 设置walker属性
		CWalker* pWalker = new CWalker(walkerInfo);
		pWalker->SetCamera(m_pScene->GetCameraByName(std::string(walkerInfo.Camera)));

		m_pScene->AddWalker(pWalker); // m_vWalker.push_back(pWalker);
		m_pSceneResource->walker.push_back(pWalker);
	}

	// Editor的数目及列表
	UINT EditorCount = 0;
	CPY_FROM_STREAM(EditorCount, pOffset);
	for (UINT iEditor = 0; iEditor < EditorCount; ++iEditor)
	{
		// 读入保存的Editor信息
		N3D_Editor Info;
		CPY_FROM_STREAM(Info, pOffset);

		// 创建Editor对象
		//CEditor* pEditor = new CEditor();

		//pEditor->SetName(std::string(Info.Name));
		// 检查cameraIndex的合理性
		INT camIndx = Info.CameraIndex + preEditorCnt;
		std::string sCamName = m_pScene->GetCameraNameByIndex(camIndx);

		EditorInfo editorInfo;
		ZeroMemory(&editorInfo, sizeof(editorInfo));
		strncpy_s(editorInfo.Name, sizeof(editorInfo.Name), Info.Name, sizeof(Info.Name));
		editorInfo.autoSpeedY      = Info.autoSpeedX;
		editorInfo.autoSpeedY      = Info.autoSpeedY;
		editorInfo.bIsAutoRoundRoll = Info.bIsAutoRoundRoll;
		strncpy_s(editorInfo.Camera, sizeof(editorInfo.Camera), sCamName.c_str(), sCamName.length());
		editorInfo.dollyAcc = Info.dollyAcc;
		editorInfo.dollyFarDis    = Info.dollyFarDis;
		editorInfo.dollyNearDis   = Info.dollyNearDis;
		editorInfo.dollySpeed     = Info.dollySpeed;
		editorInfo.LookAtModelIndex = -1;
		editorInfo.LookAtPos        = VECTOR3(Info.LookAtPos.x, Info.LookAtPos.y, Info.LookAtPos.z);
		editorInfo.LookAtType       = 1;
		editorInfo.manualPanXSpeed  = Info.manualPanXSpeed;
		editorInfo.manualPanYSpeed  = Info.manualPanYSpeed;
		editorInfo.manualSpeedX = Info.manualSpeedX;
		editorInfo.manualSpeedY = Info.manualSpeedY;
		editorInfo.rosAxis       = Info.rosAxis;

		// pEditor->SetEditorInfo(editorInfo);
		CEditor* pEditor = new CEditor(m_pScene, editorInfo);
		pEditor->SetCamera(m_pScene->GetCameraByName(std::string(editorInfo.Camera)));

		//VECTOR3 camAt(Info.LookAtPos.x, Info.LookAtPos.y, -Info.LookAtPos.z);
		//pEditor->LookAt(camAt, 1);

		m_pScene->AddEditor(pEditor);//m_vEditor.push_back(pEditor);
		m_pSceneResource->editor.push_back(pEditor);
	}

	// Flyer的数目及列表
	UINT FlyerCount = 0;
	CPY_FROM_STREAM(FlyerCount, pOffset);
	for (UINT iFlyer = 0; iFlyer < FlyerCount; ++iFlyer)
	{
		// 读入保存的Flyer信息
		N3D_Flyer Info;
		CPY_FROM_STREAM(Info, pOffset);

		// 检查cameraIndex的合理性
		INT camIndx = Info.CameraIndex + preFlyerCnt;
		std::string sCamName = m_pScene->GetCameraNameByIndex(camIndx);

		// 创建Flyer对象
		//CFlyer* pFlyer = new CFlyer;	
		//pFlyer->SetName(Info.Name);

		FlyerInfo flyerInfo;
		ZeroMemory(&flyerInfo, sizeof(flyerInfo));
		strncpy_s(flyerInfo.Name, sizeof(flyerInfo.Name), Info.Name, sizeof(Info.Name));
		flyerInfo.bCollision   = Info.bCollision;
		flyerInfo.colDis       = Info.colDis;
		strncpy_s(flyerInfo.Camera, sizeof(flyerInfo.Camera), sCamName.c_str(), sCamName.length());
		flyerInfo.dollySpeed   = Info.dollySpeed;
		flyerInfo.dollyAcc     = Info.dollyAcc;
		flyerInfo.dollyNearDis = Info.dollyNearDis;
		flyerInfo.dollyFarDis  = Info.dollyFarDis;
		flyerInfo.dollySpeed   = Info.dollySpeed;
		flyerInfo.dollyAcc     = Info.dollyAcc;
		flyerInfo.liftSpeed    = Info.liftSpeed;
		flyerInfo.liftAcc      = Info.liftAcc;
		flyerInfo.lowerAngle   = Info.lowerAngle;
		flyerInfo.panSpeed     = Info.panSpeed;
		flyerInfo.panAcc       = Info.panAcc;
		flyerInfo.rotateSpeed  = Info.rotateSpeed;
		flyerInfo.rotateAcc    = Info.rotateAcc;
		flyerInfo.upperAngle   = Info.upperAngle;
		// pFlyer->SetFlyerInfo(flyerInfo);

		CFlyer* pFlyer = new CFlyer(flyerInfo);
		pFlyer->SetCamera(m_pScene->GetCameraByName(std::string(flyerInfo.Camera)));

		m_pScene->AddFlyer(pFlyer);//m_vFlyer.push_back(pFlyer);
		m_pSceneResource->flyer.push_back(pFlyer);
	}

	// 当前控制对象的类型
	N3D_ControlData ControlData;
	CPY_FROM_STREAM(ControlData.ControlType, pOffset); 
	CPY_FROM_STREAM(ControlData.ControlerIndex, pOffset);

	IController *pCurController = m_pScene->GetCurrentController();
	if (!pCurController)  // 没有当前控制对象
	{
		switch (ControlData.ControlType)
		{
		case 0:
			m_pScene->SetCurrentController(m_pScene->GetWalker(ControlData.ControlerIndex + preWalkerCnt));
			break;
		case 1:
			m_pScene->SetCurrentController(m_pScene->GetEditor(ControlData.ControlerIndex + preEditorCnt));
			break;
		case 2:
			m_pScene->SetCurrentController(m_pScene->GetFlyer(ControlData.ControlerIndex + preFlyerCnt));
			break;
		} // switch
	} // end if
}


//为overlay排序
bool SortOverlayByOrder(COverlay* p1,COverlay* p2)
{
// 	if(p1->ZOrder< p2->ZOrder)
// 		return true;	
// 	else
		return false;
}
VOID CN3DLoaderImpV2::ReadOverlayData()
{
	m_vN3DOverlayData.clear();

	BYTE* pOffset = m_pSceneData +  TableData.OverlayPtr;

	m_preOverlayCnt = m_pScene->m_vOverlay.size();

	// 读入Overlay对象数量
	UINT overlayCount = 0;
	CPY_FROM_STREAM(overlayCount, pOffset);
	if (overlayCount <= 0)
	{
		return;
	}

	// 读入所有overlay信息
	for (UINT iOverlay = 0; iOverlay < overlayCount; ++iOverlay)
	{
		N3D_Overlay Info;

		ZeroMemory(&Info, sizeof(Info));

		// 读取overlay基本信息
		CPY_FROM_STREAM(Info.BasalInfo, pOffset);

		//读入Overlay的关键帧数量
		CPY_FROM_STREAM(Info.KfrCnt, pOffset);

		//51003版本后添加了overlay的Zorder
		if (m_fN3DFileVersion >= 5.1003f)
		{
			CPY_FROM_STREAM(Info.zOrder, pOffset);
		}

		//pOverlay->m_k = Info.KfrCnt;
		//pOverlay->m_kfrInfo = NULL;
		//写入关键帧数据
		/*if (pOverlay->m_kfrCnt > 0)
		{
		pOverlay->m_kfrInfo = new OverlayKfrInfo[pOverlay->m_kfrCnt];
		for (INT j = 0; j< pOverlay->m_kfrCnt; j++)
		{
		N3D_OverlayFrameInfo FrameInfo;
		CPY_FROM_STREAM(FrameInfo, pOffset);
		pOverlay->m_kfrInfo[j].Rect    = FrameInfo.Rect;
		pOverlay->m_kfrInfo[j].Angle   = FrameInfo.Angle;
		pOverlay->m_kfrInfo[j].Opacity = FrameInfo.Opacity;
		}
		}*/

		m_vN3DOverlayData.push_back(N3DOverlayData(FALSE, Info)); // 先保存起来

		continue;  // overlay放到最后创建

		// 判断Overlay的类型
		// ==> fuxb 2009-04-14 暂时解决webmax发布出来后，自动为按钮、导航图、导航点添加了后缀的问题
		VGS_GUIOBJ_TYPE guiType = VGS_GUIOBJ_TYPE_OVERLAY;
		std::string strName = std::string(Info.BasalInfo.Name);
		{
			std::string strSuffix = "_btnOly";
			if (strName.find(strSuffix) != std::string::npos)  // button
			{
				int len = strName.length() - std::string(strSuffix).length();
				if (len > 0)
				{
					Info.BasalInfo.Name[len] = '\0';
					strName = std::string(Info.BasalInfo.Name);
					std::transform(strName.begin(), strName.end(), strName.begin(), toupper); // 转换成大写
				}	
				guiType = VGS_GUIOBJ_TYPE_BUTTON;
			}
			else 
			{
				strSuffix = "_NavOly";
				if (strName.find(strSuffix) != std::string::npos)  // navigator
				{
					int len = strName.length() - std::string(strSuffix).length();
					if (len > 0)
					{
						Info.BasalInfo.Name[len] = '\0';
						strName = std::string(Info.BasalInfo.Name);
						std::transform(strName.begin(), strName.end(), strName.begin(), toupper); // 转换成大写
					}
					guiType = VGS_GUIOBJ_TYPE_NAVIGATION;
				}
				else
				{
					strSuffix = "_PtrOly";
					if (strName.find(strSuffix) != std::string::npos) // navigator pointer
					{
						int len = strName.length() - std::string(strSuffix).length();
						if (len > 0)
						{
							Info.BasalInfo.Name[len] = '\0';
							strName = std::string(Info.BasalInfo.Name);
							std::transform(strName.begin(), strName.end(), strName.begin(), toupper); // 转换成大写
						}
						guiType = VGS_GUIOBJ_TYPE_NAVIPOINTOR;
					} // 是否是导航点
					else
					{
						//读入Overlay的名字
						strName = std::string(Info.BasalInfo.Name);
						std::transform(strName.begin(), strName.end(), strName.begin(), toupper); // 转换成大写
					}
				} // 是否是导航图
			} // 是否是按钮
		}
		// <==

		COverlay *pOverlay = NULL;
		if (Info.BasalInfo.IsBackdrop)  // 背景图
		{
			pOverlay = (COverlay *)m_pScene->GetGuiManagerRef().CreateBackGroudLayer();
		}
		else
		{
			if (guiType == VGS_GUIOBJ_TYPE_OVERLAY) // 普通overlay
			{
				pOverlay = (COverlay *)m_pScene->GetGuiManagerRef().CreateOverLayer(strName.c_str());
			}
		}
		
		if (pOverlay)
		{
			//写入Overlay的偏移坐标原点标示
			pOverlay->SetOffsetOrigin((VGSOFFSETORIGIN)Info.BasalInfo.OffsetOrigin);

			//读入Overlay的位置标示(相对坐标还是绝对坐标)
			pOverlay->SetUsedRelativePos((bool)Info.BasalInfo.UseRelativePos);

			//读入Overlay是否使用设定的绝对尺寸
			pOverlay->SetUsedRelativeSize((bool)Info.BasalInfo.UseRelativeSize);

			//读入Overlay尺寸
			RECT Rect = Info.BasalInfo.Rect; // 保存的始终是绝对位置和大小		
			pOverlay->SetRect(Info.BasalInfo.Rect);   // 计算相对尺寸

			//读入Overlay转角
			FLOAT angle = Info.BasalInfo.Angle;
			pOverlay->SetRotation(angle);

			//是否是背景图
			pOverlay->SetVisibility((bool)Info.BasalInfo.IsVisible);

			//51003版本后添加了overlay的Zorder
			if (m_fN3DFileVersion >= 5.1003f)
			{
				//CPY_FROM_STREAM(pOverlay->m_bRelativeSize, pOffset);
			}
			//读入Overlay的材质ID
			if (Info.BasalInfo.MaterialIndex != -1)
			{
				UINT index = Info.BasalInfo.MaterialIndex;
				CMaterial* pMtrl = _GetHandleByIndex(m_vMaterial, index);
				pOverlay->SetMaterial(pMtrl); 
				m_pScene->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);
				// pOverlay->m_pMtrl->bIsOverlayMtrl = TRUE;

				//设置Overlay用到的贴图的MipMap为1
				int texLayerCnt = pOverlay->GetMaterial()->getTexureLayerCount();
				//材质有贴图，则遍历贴图层列表逐层按类型贴图并混色
				if (texLayerCnt > 0)
				{
					for(int i = 0; i< texLayerCnt; i++)
					{
						CTextureLayer* pTexLayer = pOverlay->GetMaterial()->getTextureLayerByIndex(i);
						CTexture* pTex = (CTexture*)(pTexLayer->pTexture);
						pTex->MipCount = 1;
					}
				}

				pOverlay->GetMaterial()->selfIllumination = 100;
				//pOverlay->m_imageWidth = Rect.right - Rect.left;
				//pOverlay->m_imageHeight = Rect.bottom - Rect.top;
			}

			//读入Overlay的透明度
			INT opacity = Info.BasalInfo.Opacity;
			pOverlay->SetOpacity(opacity);

			// 保存到场景列表
			//m_pScene->m_vOverlay.push_back(pOverlay);
			//m_pSceneResource->overlay.push_back(pOverlay);
		}				

	} // for
}

VOID CN3DLoaderImpV2::ReadTextData()
{
	
}

VOID CN3DLoaderImpV2::FindReferenceModel()
{
	
}

VOID CN3DLoaderImpV2::ReadModelGroupData()
{
	
}

VOID CN3DLoaderImpV2::ReadLightData()
{
	BYTE* pOffset = m_pSceneData +  TableData.lightDataPtr;

	m_vLightParam.clear();
    
	//读入灯光数量
// 	memcpy(&(LightData.lightCnt), pOffset, sizeof(LightData.lightCnt));
// 	pOffset += sizeof(LightData.lightCnt);
	CPY_FROM_STREAM(LightData.lightCnt, pOffset);

    m_preLightCnt = m_pScene->getLightCount();

	//读入灯光详细数据
	LightData.light = new N3D_Light[LightData.lightCnt];
    
	for (int i = 0; i< LightData.lightCnt; i++)
	{
		std::string sName = "";	
		//51007版本，相机名称没有字符限制
		if (m_fN3DFileVersion < 5.1007f)
		{
		    CHAR* readName =new CHAR[N3D_MAX_NAME_LENGTH];
			ZeroMemory(readName,sizeof(readName));
            memcpy(readName , pOffset, N3D_MAX_NAME_LENGTH);
			readName[N3D_MAX_NAME_LENGTH - 1] = '\0';
			pOffset += N3D_MAX_NAME_LENGTH;
			sName = std::string(readName);
			SAFE_DELETE_ARRAY(readName);
		}
		else
		{
			INT strLen;
			memcpy(&(strLen), pOffset, sizeof(strLen));	
			pOffset += sizeof(strLen);	
			char* readName =new char[strLen+1];
			ZeroMemory(readName, strLen + 1);
			memcpy(readName , pOffset, strLen);
			readName[strLen] = '\0';
			pOffset += strLen;
			sName = std::string(readName);
			SAFE_DELETE_ARRAY(readName);		
		}
		sName = m_pScene->RenameLightIfExist(sName);
		CLight *pLight = m_pScene->CreateLight(sName);

		memcpy(&(LightData.light[i].type), pOffset, sizeof(LightData.light[i].type));
		pOffset += sizeof(LightData.light[i].type);
		
		//灯光类型,不同的灯光类型必须设定不同的参数--------------------------	
		switch (LightData.light[i].type)
		{
		case NLT_OMINI:
			pLight->param.Type = D3DLIGHT_POINT;
			LightData.light[i].pOmniLight =new N3D_OMNI();
			memcpy(LightData.light[i].pOmniLight, pOffset, sizeof(*(LightData.light[i].pOmniLight)));
			pOffset += sizeof(*(LightData.light[i].pOmniLight));
			
			//强度
			pLight->multiply = LightData.light[i].pOmniLight->multiply;
			
			//颜色
			pLight->diffuse.r = ((LightData.light[i].pOmniLight->color>>24) &0xff)/255.0f;
			pLight->diffuse.g = ((LightData.light[i].pOmniLight->color>>16) &0xff)/255.0f;
			pLight->diffuse.b = ((LightData.light[i].pOmniLight->color>>8) &0xff)/255.0f;
            pLight->diffuse.a =1.0f;
			pLight->param.Diffuse.r = pLight->diffuse.r * pLight->multiply;
			pLight->param.Diffuse.g = pLight->diffuse.g * pLight->multiply;
			pLight->param.Diffuse.b = pLight->diffuse.b * pLight->multiply;
			pLight->param.Diffuse.a = 1.0f;
			
			//环境光，用它来近似模拟全局光
			pLight->ambient   = pLight->diffuse;
			pLight->ambient.a = 1.0f;
			pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
			pLight->param.Ambient.a = 1.0f;
			
			//高光
			// pLight->specular = 0.0 * pLight->diffuse;
			pLight->param.Specular   = pLight->multiply * pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式
			pLight->param.Specular.a = 1.0f;
			
            //光线到达的最远距离
			pLight->param.Range        = LightData.light[i].pOmniLight->attenuationRange;
			pLight->param.Attenuation0 = LightData.light[i].pOmniLight->attenuation0;
			pLight->param.Attenuation1 = LightData.light[i].pOmniLight->attenuation1;
			pLight->param.Attenuation2 = LightData.light[i].pOmniLight->attenuation2;			
            
			memcpy(&(LightData.light[i].FrameCount), pOffset, sizeof(LightData.light[i].FrameCount));
			pOffset += sizeof(LightData.light[i].FrameCount);
			pLight->kfrCnt = LightData.light[i].FrameCount;
			if (LightData.light[i].FrameCount > 0)
			{
				N3DLightParamData lightParam;
				lightParam.vOmni.resize(LightData.light[i].FrameCount);
				lightParam.pLight = pLight;
				for (int i = 0; i<LightData.light[i].FrameCount;i++)
				{
					memcpy(&lightParam.vOmni[i], pOffset, sizeof(N3D_OMNI));
					pOffset += sizeof(N3D_OMNI);
				}	
				m_vLightParam.push_back(lightParam);
			}  
			break;
			
		case NLT_SPOTLIGHT:
			pLight->param.Type= D3DLIGHT_SPOT;
			LightData.light[i].pSpotLightInfo = new N3D_SPOTLIGHT();
			memcpy(LightData.light[i].pSpotLightInfo, pOffset, sizeof(*(LightData.light[i].pSpotLightInfo)));
			pOffset += sizeof(*(LightData.light[i].pSpotLightInfo));
			//强度
			pLight->multiply = LightData.light[i].pSpotLightInfo->multiply;
			//颜色
			pLight->diffuse.r = ((LightData.light[i].pSpotLightInfo->color >> 24) & 0xff)/255.0f;
			pLight->diffuse.g = ((LightData.light[i].pSpotLightInfo->color >> 16) & 0xff)/255.0f;
			pLight->diffuse.b = ((LightData.light[i].pSpotLightInfo->color >> 8) & 0xff)/255.0f;
			pLight->ambient = 0.0f * pLight->diffuse;
			pLight->param.Diffuse.r = pLight->diffuse.r * pLight->multiply;
			pLight->param.Diffuse.g = pLight->diffuse.g * pLight->multiply;
			pLight->param.Diffuse.b = pLight->diffuse.b * pLight->multiply;
			pLight->param.Diffuse.a = 1.0f;
			
			//环境光，用它来近似模拟全局光
			pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
			pLight->param.Ambient.a = 1.0f;
			
			//高光
			pLight->param.Specular = pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式
			
            //光线到达的最远距离
			pLight->param.Range        = LightData.light[i].pSpotLightInfo->attenuationRange;
			pLight->param.Attenuation0 = LightData.light[i].pSpotLightInfo->attenuation0;
			pLight->param.Attenuation1 = LightData.light[i].pSpotLightInfo->attenuation1;
			pLight->param.Attenuation2 = LightData.light[i].pSpotLightInfo->attenuation2;
			
			// 在世界坐标系中的方向
			pLight->param.Direction.x = LightData.light[i].pSpotLightInfo->direction.x;
			pLight->param.Direction.y = LightData.light[i].pSpotLightInfo->direction.y;
			pLight->param.Direction.z = -LightData.light[i].pSpotLightInfo->direction.z;
			
            //光线区域的范围角
			pLight->param.Theta   = LightData.light[i].pSpotLightInfo->innerAngle;
			pLight->param.Phi     = LightData.light[i].pSpotLightInfo->outerAngle;
			pLight->param.Falloff = LightData.light[i].pSpotLightInfo->fallOff;
            
			memcpy(&(LightData.light[i].FrameCount), pOffset, sizeof(LightData.light[i].FrameCount));
			pOffset += sizeof(LightData.light[i].FrameCount);
			pLight->kfrCnt = LightData.light[i].FrameCount;
			if(LightData.light[i].FrameCount > 0)
			{
				N3DLightParamData lightParam;
				lightParam.vSpot.resize(LightData.light[i].FrameCount);
				lightParam.pLight = pLight;
				// pLight->pAnimSpotLight = new N3D_SPOTLIGHT[LightData.light[i].FrameCount];
				for (int i = 0; i<LightData.light[i].FrameCount;i++)
				{
					memcpy(&lightParam.vSpot[i], pOffset, sizeof(N3D_SPOTLIGHT));
					pOffset += sizeof(N3D_SPOTLIGHT);
					lightParam.vSpot[i].direction.z *= -1;

					//memcpy(pLight->pAnimSpotLight, pOffset, sizeof(*(LightData.light[i].pSpotLightInfo)));
					//pOffset += sizeof(*(LightData.light[i].pSpotLightInfo));
                    //pLight->pAnimSpotLight[i].direction.z = -pLight->pAnimSpotLight[i].direction.z;
				}		
				m_vLightParam.push_back(lightParam);	    
			} 
			
			break;
		case NLT_DIRECTIONLIGHT:
			pLight->param.Type = D3DLIGHT_DIRECTIONAL; 
			LightData.light[i].pDirectionalLight =new N3D_DIRECTIONAL();
			memcpy(LightData.light[i].pDirectionalLight, pOffset, sizeof(N3D_DIRECTIONAL));
			pOffset += sizeof(N3D_DIRECTIONAL);
			//强度
			pLight->multiply = LightData.light[i].pDirectionalLight->multiply;
			//颜色
			pLight->diffuse.r = ((LightData.light[i].pDirectionalLight->color >> 24) &0xff)/255.0f;
			pLight->diffuse.g = ((LightData.light[i].pDirectionalLight->color >> 16) &0xff)/255.0f;
			pLight->diffuse.b = ((LightData.light[i].pDirectionalLight->color >> 8) &0xff)/255.0f;
			pLight->ambient   = 0.0f * pLight->diffuse;
			pLight->param.Diffuse.r = pLight->diffuse.r * pLight->multiply;
			pLight->param.Diffuse.g = pLight->diffuse.g * pLight->multiply;
			pLight->param.Diffuse.b = pLight->diffuse.b * pLight->multiply;
			pLight->param.Diffuse.a = 1.0f;
			//环境光，用它来近似模拟全局光
			pLight->param.Ambient = 0.0f * pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
			pLight->param.Ambient.a = 1.0f;
			//高光
			pLight->param.Specular = pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式
            //光线到达的最远距离
			pLight->param.Range = LightData.light[i].pDirectionalLight->attenuationRange;
			pLight->param.Attenuation0 = LightData.light[i].pDirectionalLight->attenuation0;
			pLight->param.Attenuation1 = LightData.light[i].pDirectionalLight->attenuation1;
			pLight->param.Attenuation2 = LightData.light[i].pDirectionalLight->attenuation2;
			
			//方向
			pLight->param.Direction.x = LightData.light[i].pDirectionalLight->direction.x;
			pLight->param.Direction.y = LightData.light[i].pDirectionalLight->direction.y;
			pLight->param.Direction.z = -LightData.light[i].pDirectionalLight->direction.z;
            
			memcpy(&(LightData.light[i].FrameCount), pOffset, sizeof(LightData.light[i].FrameCount));
			pOffset += sizeof(LightData.light[i].FrameCount);
			pLight->kfrCnt = LightData.light[i].FrameCount;
			if(LightData.light[i].FrameCount > 0)
			{
				N3DLightParamData lightParam;
				lightParam.vDir.resize(LightData.light[i].FrameCount);
				lightParam.pLight = pLight;
				// pLight->pAnimDirectionalLight = new N3D_DIRECTIONAL[LightData.light[i].FrameCount];
				for (int i = 0; i<LightData.light[i].FrameCount;i++)
				{
					memcpy(&lightParam.vDir[i], pOffset, sizeof(N3D_DIRECTIONAL));
					pOffset += sizeof(N3D_DIRECTIONAL);
					lightParam.vDir[i].direction.z *= -1;
					//memcpy(pLight->pAnimDirectionalLight, pOffset, sizeof(*(LightData.light[i].pDirectionalLight)));
					//pOffset += sizeof(*(LightData.light[i].pDirectionalLight));
                    //pLight->pAnimDirectionalLight[i].direction.z = -pLight->pAnimDirectionalLight[i].direction.z;
				}
				m_vLightParam.push_back(lightParam);
			} 			
			break;	
		}
		
		// 关键帧
		//int keyFrameCount = 0;
		//CPY_FROM_STREAM(keyFrameCount, pOffset);
		//
		//if (keyFrameCount > 0)
		//{
		//	N3DLightParamData lightParam;
		//	lightParam.pLight = pLight;
		//	if (pLight->getType() == VLT_OMNI)
		//	{
		//		for (UINT iFrame = 0; iFrame < keyFrameCount; ++iFrame)
		//		{
		//			N3D_OMNI omni;
		//			CPY_FROM_STREAM(omni, pOffset);
		//			lightParam.vOmni.push_back(omni);
		//		}
		//	}
		//	else if (pLight->getType() == VLT_SPOT)
		//	{
		//		for (UINT iFrame = 0; iFrame < keyFrameCount; ++iFrame)
		//		{
		//			N3D_SPOTLIGHT spot;
		//			CPY_FROM_STREAM(spot, pOffset);
		//			lightParam.vSpot.push_back(spot);
		//		}
		//	}
		//	else if (pLight->getType() == VLT_DIR)
		//	{
		//		for (UINT iFrame = 0; iFrame < keyFrameCount; ++iFrame)
		//		{
		//			N3D_DIRECTIONAL dir;
		//			CPY_FROM_STREAM(dir, pOffset);
		//			lightParam.vDir.push_back(dir);
		//		}
		//	}

		//	// 保存相机动画
		//	m_vLightParam.push_back(lightParam);
		//}

		
		//关键帧动画信息
		/*pLight->playKfr   = FALSE;
		pLight->isPlaying = FALSE;
		pLight->kfrCnt = LightData.light[i].FrameCount;
		
		if ((int)pLight->kfrCnt > m_pScene->m_KfrCnt)
		{
			m_pScene->m_KfrCnt = pLight->kfrCnt;
			m_pScene->m_EndFrame = pLight->kfrCnt;
		}*/

		pLight->index = i;
		m_pScene->m_pd3dDevice->SetLight(i, &pLight->param);
		m_pScene->m_pd3dDevice->LightEnable(i, TRUE);
		
		m_pSceneResource->light.push_back(pLight);
	}
	SAFE_DELETE_ARRAY(LightData.light);
}

//-------------------------------------------------------------------
//方法: CreateBasicTextureUsedThisImage()
//参数: _N3DZipImageData* pImageData
//用途: 创建用到该图像的所有基本贴图对象
//-------------------------------------------------------------------
VOID CN3DLoaderImpV2::CreateBasicTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData)
{
// 	if (!pImageData)
// 	{
// 		return;
// 	}
// 
//    //遍历该N3D文件的单帧贴图列表
// 	CTexture** ppTex = m_pSceneResource->texture.begin();
// 	INT texCnt = m_pSceneResource->texture.size();
// 	
// 	//遍历贴图列表，如果贴图用到该图像，则用该图像创建贴图
//     for(INT i =0; i< texCnt; i++)
// 	{
// 		CTexture* pTex = *(ppTex + i);
// 		//已经创建
// 		if (pTex->bCreated) continue;
// 
// 		//检查贴图使用的图像编号是否等于m_ImageIndex
// 		if (pTex->imageIdx == m_ImageIndex)
// 		{
// 			//普通贴图
// 			if (pTex->type == BASIC_TYPE)
// 				m_pScene->CreateTexFromBuffer(pTex, pImageData->imageInfo.Size, pImageData->Data);
// 
// 			//透明贴图
// 			if(pTex->type == OPACITY_TYPE)
// 			{
// 				m_pScene->CreateTransparentTexture(pTex, pImageData->imageInfo.Size, pImageData->Data);
// 				pTex->bAlphaBlend = TRUE;
// 			}
// 
// 			//凹凸贴图
// 			if (pTex->type == BUMP_TYPE)
// 			{
// 				//初始化凹凸贴图
// 				CBumpMap* pBumpMap = m_pScene->bumpTexture[pTex->bumpIdx];
// 				m_pScene->CreateTexFromBuffer(pTex, pImageData->imageInfo.Size, pImageData->Data);
// 
// 				switch (pBumpMap->m_type)
// 				{
// 				case BUMPWAVEMAP: //不需要贴图
// 					break;
// 				case BUMPENVMAP:
// 					pBumpMap->InitBumpEnvMap();
// 					break;
// 				case BUMPNORMALMAP:
// 					pBumpMap->InitBumpNormalMap();
// 					break;
// 				}
// 			}
// 
// 			// fuxb 2009-03-18 ==> 如果有透明贴图，设置texture为alphaBlend
// 			char tmp[64];
// 			ZeroMemory(tmp, 64); 
// 			strncpy(tmp, pImageData->imageInfo.Name, sizeof(pImageData->imageInfo.Name));			
// 			std::string str = string(tmp);
// 			transform(str.begin(), str.end(), str.begin(), tolower);
// 			if (str.find(".png") != std::string::npos || str.find(".tga") != std::string::npos)
// 			{
// 				pTex->bAlphaBlend = TRUE;
// 			}
// 			// <==
// 
// 			//判断使用该贴图的材质是否有AlphaBlend--------------------------------------------------------
// 			if (pTex->bAlphaBlend)
// 			{
// 				UpdateAllMtrlAlphaBlendStatus((DWORD)pTex);
// 			}
// 		}
// 	}
}

//-------------------------------------------------------------------
//方法: CreateDynamicTextureUsedThisImage()
//参数: _N3DZipImageData* pImageData
//用途: 创建用到该图像的所有动态贴图对象
//-------------------------------------------------------------------
VOID CN3DLoaderImpV2::CreateDynamicTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData)
{
// 	if (!pImageData)
// 	{
// 		return;
// 	}
// 
// 	//遍历该N3D文件的动态（多帧）贴图列表
// 	CDynamicTex** ppDyTex = m_pSceneResource->dynamicTexture.begin();
// 	INT dyTexCnt = m_pSceneResource->dynamicTexture.size();
// 	
// 	//遍历贴图列表，如果贴图用到该图像，则用该图像创建贴图
//     for(INT i =0; i< dyTexCnt; i++)
// 	{
// 		CDynamicTex* pDyTex = *(ppDyTex + i);
// 		//已经创建
// 		if (pDyTex->bCreated) continue;
// 			
// 		//遍历该动态贴图对象的图像源表, 判断是否用到了当前图像
// 		int imageCnt = pDyTex->imagesIdx.size();
// 		for (int j = 0; j< imageCnt; j++)
// 		{
// 			INT imageIndex = pDyTex->imagesIdx[j];
// 			if (imageIndex == m_ImageIndex)
// 			{
// 				//用图像填充贴图缓冲
// 				pDyTex->FillImageToTextureBuffer(j, pImageData->imageInfo.Size, pImageData->Data);
// 
// 				// fuxb 2009-03-18 ==> 如果有透明贴图，设置texture为alphaBlend
// 				char tmp[64];
// 				ZeroMemory(tmp, 64); 
// 				strncpy(tmp, pImageData->imageInfo.Name, sizeof(pImageData->imageInfo.Name));			
// 				std::string str = string(tmp);
// 				transform(str.begin(), str.end(), str.begin(), tolower);
// 				if (str.find(".png") != std::string::npos || str.find(".tga") != std::string::npos)
// 				{
// 					pDyTex->bAlphaBlend = TRUE;
// 				}
// 				// <==
// 
// 				break;
// 			}
// 		}
// 
// 		// fuxb 2009-03-18 ==>
// 		if (pDyTex->bAlphaBlend)
// 		{
// 			UpdateAllMtrlAlphaBlendStatus((DWORD)pDyTex);
// 		}
// 		// <==
// 	}
}


//-------------------------------------------------------------------
//方法: CreateCubeTextureUsedThisImage()
//参数: _N3DZipImageData* pImageData
//用途: 创建用到该图像的所有CubeMap贴图对象
//-------------------------------------------------------------------
VOID CN3DLoaderImpV2::CreateCubeTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData)
{
	//遍历该N3D文件的CubeMap（多副）贴图列表
// 	CCubeMap** ppCubeMap = m_pSceneResource->cubeMap.begin();
// 	INT cubeMapCnt = m_pSceneResource->cubeMap.size();
// 	
// 	//遍历CubeMap贴图列表，如果贴图用到该图像，则用该图像创建贴图
//     for(INT i =0; i< cubeMapCnt; i++)
// 	{
// 		CCubeMap* pCubeMap = *(ppCubeMap + i);
// 		//已经创建
// 		if (pCubeMap->bCreated) continue;
// 		
// 		//遍历CubeMap的6个贴图
// 		for (INT faceIndex = 0; faceIndex< 6; faceIndex++)
// 		{
// 			if(m_ImageIndex == pCubeMap->imagesIdx[faceIndex])
// 			{
// 				//用图像填充贴图缓冲
// 				pCubeMap->FillImageToCubeTexBuffer(faceIndex, pImageData->imageInfo.Size, pImageData->Data);
// 
// 				// fuxb 2009-03-18 ==> 如果有透明贴图，设置texture为alphaBlend
// 				char tmp[64];
// 				ZeroMemory(tmp, 64); 
// 				strncpy(tmp, pImageData->imageInfo.Name, sizeof(pImageData->imageInfo.Name));			
// 				std::string str = string(tmp);
// 				transform(str.begin(), str.end(), str.begin(), tolower);
// 				if (str.find(".png") != std::string::npos || str.find(".tga") != std::string::npos)
// 				{
// 					pCubeMap->bAlphaBlend = TRUE;
// 				}
// 				// <==
// 
// 				break;
// 			}
// 		} // for
// 
// 		// fuxb 2009-03-18 ==>
// 		if (pCubeMap->bAlphaBlend)
// 		{
// 			UpdateAllMtrlAlphaBlendStatus((DWORD)pCubeMap);
// 		}
// 		// <==
// 	}
}

//-------------------------------------------------------------------
//方法: UpdateAllMtrlAlphaBlendStatus()
//参数: CTexture* pTex
//用途: 判断使用该贴图的材质是否有AlphaBlend
//-------------------------------------------------------------------
VOID CN3DLoaderImpV2::UpdateAllMtrlAlphaBlendStatus(DWORD pTex)
{
// 	int mtrlCnt = m_pSceneResource->shader.size();
// 	CMaterial** ppMtrl = m_pSceneResource->shader.begin();
// 	for (int j = 0; j< mtrlCnt; j++)
// 	{
// 		CMaterial* pMtrl = *(ppMtrl + j);
// 		int texLayerCnt = pMtrl->texLayersList.size();
// 		TextureLayer** ppTextureLayer = pMtrl->texLayersList.begin();
// 		for (int k = 0; k< texLayerCnt; k++)
// 		{
// 			TextureLayer* pTexLayer = *(ppTextureLayer + k);
// 			if (pTexLayer->pTexture == (DWORD)pTex)
// 			{
// 				pMtrl->bAlphaBlend = true;
// 			    
// 				// fuxb 2009-03-06 ==> alphablend的模型不参与排序 ==>
// 				//遍历材质的subMesh列表,检查使用到该材质的模型是否在透明模型列表中
// // 			    int subMeshCnt = pMtrl->subMeshList.size();
// // 				RenderSubMesh* pSubMesh = pMtrl->subMeshList.begin();
// // 				for (INT m = 0; m< subMeshCnt; m++)
// // 				{
// // 					RenderSubMesh subMesh = *(pSubMesh + m);
// // 					//CModel* pModel = *(ppModel + m);
// // 					CModel* pTheModel =(CModel*)subMesh.modelPointer;
// // 					BOOL bInTrans = FALSE;
// // 					CModel** ppModel = m_pScene->transModel.begin();
// // 					for (INT l = 0; l < m_pScene->transModel.size(); l++)
// // 					{
// // 						
// // 						CModel* pModel = *(ppModel + l);
// // 						if(pTheModel == pModel)
// // 						{
// // 							bInTrans = TRUE;
// // 						    break;
// // 						}
// // 					}
// // 					if(bInTrans == FALSE)
// // 					{
// // 					   m_pScene->transModel.push_back(pTheModel);
// // 					}
// // 				}
// 				// <==
// 			}
// 		}
// 	}
}

//-------------------------------------------------------------------
//方法: ConstructTextureFromImage()
//参数: _N3DZipImageData* pImageData
//用途: 下载完一幅贴图的数据后，创建用到该图像的所有贴图对象
//-------------------------------------------------------------------
BOOL CN3DLoaderImpV2::ConstructTextureFromImage(N3D_5_1_ZipImageData* pImageData)
{
	//创建该N3D中用到该Image的单帧贴图
// 	CreateBasicTextureUsedThisImage(pImageData);
// 
// 	//创建该N3D中用到该Image的动态贴图
// 	CreateDynamicTextureUsedThisImage(pImageData);
// 
// 	//创建该N3D中用到该贴图的CubeMap贴图
// 	CreateCubeTextureUsedThisImage(pImageData);
// 
// 	//图像的编号
// 	m_ImageIndex++;

	return true;
}
// 加载一个N3D文件
DWORD CN3DLoaderImpV2::LoadN3DFile(CScene *pScene, const std::string &sFileName, HWND hWnd)
{
	return 0;
}
DWORD CN3DLoaderImpV2::LoadSceneFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd)
{
	if (!pScene || !pBuffer)
	{
		return ERR_FILE_N3D_SCENELOAD;
	}

	DWORD errCode;
	if (!IsSupportThisVersion(version, errCode))
	{
		return errCode;
	}

	SetRoot(pScene->GetRoot());

	m_fN3DFileVersion = _N3D_FLOATVERSION(version); 
	m_pSceneData = (BYTE*)pBuffer;
	CSceneResource *pRes = new CSceneResource("", pScene);
	INT re = ConstructScene(pScene, pRes);
	return re;
}

DWORD CN3DLoaderImpV2::LoadImageDataFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd)
{
	if (!pScene || !pBuffer)
	{
		return ERR_FILE_N3D_SCENELOAD;
	}

	DWORD errCode;
	if (!IsSupportThisVersion(version, errCode))
	{
		return errCode;
	}

	SetRoot(pScene->GetRoot());

	m_fN3DFileVersion = _N3D_FLOATVERSION(version);
	m_pImageData = (BYTE*)pBuffer;
	
	ReadImageData();
	return 0;
}
