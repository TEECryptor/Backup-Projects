// GetN3DFileInfo_5.cpp: implementation of the CGetN3DFileInfo_5 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GetN3DFileInfo_5.h"
//#include "CAudioPlayer.h" 
#include <algorithm>
#include "..\\CsysFunc.h"
#include "..\\vgszip\vgszip.h"
#include  "..\\VGS2DLL.h"

#ifdef _DEBUG
#pragma comment(lib, ".\\vgszip\\vgszipD.lib") 
#else
#pragma comment(lib, ".\\vgszip\\vgszip.lib")
#endif 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ��Offsetλ�ÿ��������ֽ�, ͬʱOffsetָ���Ⱥ��ƶ�sizeof(value)
#define CPY_FROM_STREAM(value, Offset)   {memcpy(&value, Offset, sizeof(value)); Offset += sizeof(value);} 


CGetN3DFileInfo_5::CGetN3DFileInfo_5(/*DWORD pScene*/)
{
	// m_pScene = pScene;

	m_pSceneResource = NULL;
	pDebugFile = NULL;
	
	N3DBufSize = 0;
	N3DBuffer = NULL;
	
	//E3DBufSize = 0;						//E3D�ļ��ĳߴ�
	//E3DBuffer = NULL;					//E3D�ļ�������
	
	m_pSceneData = NULL;
	
	m_preModelCnt = 0;		
	m_preModelGroupCnt = 0;
	m_ImageIndex = 0;
	
	m_preMtrlCnt = 0;		
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
	ModelData.model = NULL;		
	MaterialData.mtrl = NULL;	
	TextData.Texts = NULL;		
	OverlayData.Overlays = NULL;	
	NavigaterData.Navigaters = NULL;	
	ButtonData.Buttons = NULL;

	ZeroMemory(&m_N3DHeader, sizeof(m_N3DHeader));
}

CGetN3DFileInfo_5::~CGetN3DFileInfo_5()
{
	Release();
}

// �����ļ�ͷ��Ϣ
VOID CGetN3DFileInfo_5::SetHeaderInfo(N3D_Header N3DHeader) 
{
	m_N3DHeader = N3DHeader;
}
//-------------------------------------------------------------------
//����: UnzipSceneData()
//����: BYTE* srcBuf, DWORD srcBufSize, DWORD destBufSize
//��;: ��ѹ�������ݣ�д���ѹ������ݵ�destBuf
//-------------------------------------------------------------------
BOOL CGetN3DFileInfo_5::UnzipSceneData(BYTE* srcBuf, DWORD srcBufSize, DWORD destBufSize)
{
	SAFE_DELETE_ARRAY(m_pSceneData);

	//��LZMA��ѹ7Zѹ������
	m_pSceneData = new BYTE[destBufSize];
	if (destBufSize == (DWORD)vgszipDeCodeFrmBuffToBuff(srcBuf, srcBufSize, m_pSceneData))
	{
		return TRUE;
	}
	return TRUE;
}

// VOID CGetN3DFileInfo_5::ReadFileTable()
// {
// 	//write the Header to pFile
// 	if (m_pSceneData == NULL) return;
// 	
// 	//init _E3DHeader
// 	memcpy(&TableData, m_pSceneData, sizeof(TableData));
// }


//-------------------------------------------------------------------
//����: ConstructScene(CHAR* sceneName)
//����: 
//��;: ���쳡��
//-------------------------------------------------------------------
BOOL CGetN3DFileInfo_5::ConstructScene(const std::string &sResourceGoupName/*CHAR* sceneName, DWORD pSceneResource*/)
{
	return m_pVGSCore->SunLoadSceneFromBuffer(m_N3DHeader.Version, m_pSceneData);
	// m_pSceneResource = pSceneResource;	
	//read the header data
	//ReadHeader();
	//read window prop
//	ReadFileTable();
    
//	ReadSceneProp();
	//read privacy Data
	//ReadPrivacyData();
	
	//read the camera data
//	ReadCameraData();
	
	//read Audio data
//	ReadAudioData();
	//read flash data
//	ReadFlashData();
	
	//read the Light Data
//	ReadLightData();
	//read the Material data
//	ReadMaterialData();	
		
	//read the mesh data;
//	ReadModelData();
	//Read ModelGroup Data
//	ReadModelGroupData();
	//�ҵ�ÿ��ģ�Ͷ�Ӧ������ģ��
//	FindReferenceModel();
	
	//��ȡ������Ϣ
//	ReadTextData();
	//��ȡOverlay��Ϣ
//	ReadOverlayData();
	//��ȡNavigater��Ϣ
//	ReadNavigaterData();
	//��ȡButton����
//	ReadButtonData();
    
	//���������Ϣ
//	ReadControlData();

	//���볡��ͼ
//	ReadSceneGraph();

	//��ͨ������Load����ʱ�����ڴ���ĳߴ粻�䣬OnSize�¼��޷����������Ҫ�ֶ����º󻺳�
	//�����ݵ�ǰ����ĳߴ��Overlay��Rect�ٷֱ�������Overlay���غ�ĳߴ�
// 	COverlay** ppOverlay = m_pScene->overlay.begin();
// 	for (int i = 0; i< m_pScene->overlay.size(); i++)
// 	{
// 		COverlay* pOverlay = *(ppOverlay + i);
// 		m_pScene->UpdateOverlayRect(pOverlay);
// 	}	
// 
// 	m_preTexCnt = m_pScene->texture.size();
    

	
    //--------------------------------------------------------------------------------------------------
	
	//�Ϸ���Ȩȥ��Logo
	/*	if (m_pScene->m_bAuthorized)
	m_pScene->m_pStartLogo->m_bVisible = FALSE;
	else
	m_pScene->m_pStartLogo->m_bVisible = TRUE;
	*/
	return TRUE;
}


//��ȡ������Ϣ
//VOID CGetN3DFileInfo_5::ReadSceneProp()
//{
// 	BYTE* pOffset = m_pSceneData  + sizeof(TableData);;
// 	memcpy(&sceneProp, pOffset, sizeof(sceneProp));
// 	
// 	m_pScene->m_oriWidth = sceneProp.Width;
// 	m_pScene->m_oriHeight = sceneProp.Height;
// 	
// 	//m_pScene->rect.left = 0;
// 	//m_pScene->rect.top = 0;
// 	//m_pScene->rect.right = m_pScene->m_oriWidth;
// 	//m_pScene->rect.bottom = m_pScene->m_oriHeight;
// 	
// 	m_pScene->background = sceneProp.BackColor;
// 	m_pScene->fps = sceneProp.FPS;
// 	m_pScene->antiLevel = sceneProp.AntiAliasLevel;
// 	
// 	//m_pScene->FSBlurLevel = SceneProp.FSBlurLevel;
// 	//m_pScene->FSGlowLevel = SceneProp.FSGlowLevel;
// 	
// 	//m_pScene->m_KfrCnt = WindowProp.KeyframeCnt;
// 	
// 	//m_pScene->m_StartFrame = 0;
// 	//m_pScene->m_EndFrame = WindowProp.KeyframeCnt;
// 	
// 	//m_pScene->m_KfrRepeatCnt = WindowProp.RepeatCnt;
//}

//-------------------------------------------------------------------
//����: ReadCameraData()
//����: 
//��;: ��ȡ�������Ϣ
//-------------------------------------------------------------------
//VOID CGetN3DFileInfo_5::ReadCameraData()
//{
// 	BYTE* pOffset = m_pSceneData + TableData.cameraDataPtr;
// 	
// 	m_preCameraCnt = m_pScene->camera.size();
// 	
// 	//�õ����������
// 	memcpy(&(CameraData.cameraCnt), pOffset, sizeof(CameraData.cameraCnt));
// 	pOffset += sizeof(CameraData.cameraCnt);
// 	
// 	CameraData.camera = new N3D_Camera[CameraData.cameraCnt];
// 	//�����ȡ�������Ϣ
// 
// 	for (UINT i =0; i< CameraData.cameraCnt; i++)
// 	{	
// 		//pCamera->name = std::string(CameraData.camera[i].name);		
// 		CCamera* pCamera = new CCamera();
// 
// 		//Edit by Baojiahui for the version 51007 nonfinite camera string length 2009/2/24 
// 		//��ȡ���������
// 		//51007�汾���������û���ַ�����
// 		if(m_pScene->m_fileVersion < 5.1007f)
// 		{
// 		    CHAR* readName =new CHAR[N3D_MAX_NAME_LENGTH];
// 			ZeroMemory(readName,sizeof(readName));
//             memcpy(readName , pOffset, N3D_MAX_NAME_LENGTH);
// 			readName[N3D_MAX_NAME_LENGTH - 1] = '\0';
// 			pOffset += N3D_MAX_NAME_LENGTH;
// 			pCamera->name = std::string(readName);
// 			SAFE_DELETE_ARRAY(readName);
// 		}else
// 		{
// 			INT strLen;
// 			memcpy(&(strLen), pOffset, sizeof(strLen));	
// 			pOffset += sizeof(strLen);	
// 			char* readName =new char[strLen+1];
// 			ZeroMemory(readName, strLen + 1);
// 			memcpy(readName , pOffset, strLen);
// 			readName[strLen] = '\0';
// 			pOffset += strLen;
// 			pCamera->name = std::string(readName);
// 			SAFE_DELETE_ARRAY(readName);		
// 		}
// 		
// 		pCamera->name = m_pScene->renameCameraIfExist(pCamera->name);  // fuxb 2009-03-20 ���������
// 
// 		pCamera->index = m_preCameraCnt + i;
// 		
// 		//��ǰ������Ļ�����Ϣ
// 		memcpy(&(CameraData.camera[i]),pOffset,sizeof(CameraData.camera[i]));
// 		
//         pOffset += sizeof(CameraData.camera[i]);
// 		
// 		pCamera->SetFov(CameraData.camera[i].BaseInfo.fov);
// 		pCamera->m_nearClip = CameraData.camera[i].BaseInfo.nearClipDis;
// 		pCamera->m_farClip = CameraData.camera[i].BaseInfo.farClipDis;
// 
// 		// fuxb 2009-02-26 ==>
// 		if (m_pScene->m_fileVersion >= 5.1006f)
// 		{
// 			int frameCnt = 0;  // ����Ĳ�������
// 			memcpy(&frameCnt, pOffset, sizeof(frameCnt));
// 			pOffset += sizeof(frameCnt);
// 			for (int iFrame = 0; iFrame < frameCnt; ++iFrame)
// 			{
// 				N3D_CAMPARAM camInfo;
// 				memcpy(&camInfo, pOffset, sizeof(camInfo));
// 				pOffset += sizeof(camInfo);
// 				// pCamera-> ����Ӧ�ñ���������������е�fov����ʱû������
// 			}
// 		}
// 		// <==
// 		
// 		m_pScene->camera.push_back(pCamera);
// 		m_pSceneResource->camera.push_back(pCamera);
// 		//for water test
// 		//pCamera->m_fov = 45 * ATOS;
// 		//pCamera->m_nearClip = 0.3f;
// 		//pCamera->m_farClip = 5000;
// 		//pCamera->m_aspect = 1.33f;
// 	}
// 	
// 	//release memory
// 	SAFE_DELETE_ARRAY(CameraData.camera);
	
//}


//��ȡ��������
//VOID CGetN3DFileInfo_5::ReadAudioData()
//{	
	//�ҵ�������Ϣ��λ��
// 	BYTE* pOffset = m_pSceneData + TableData.AudioPtr;
// 	
// 	//�õ���Ƶ����
// 	memcpy(&(AudioData.AudioCnt), pOffset, sizeof(AudioData.AudioCnt));
// 	pOffset += sizeof(AudioData.AudioCnt);
// 	
// 	if (AudioData.AudioCnt == 0)
// 		return;
// 	
// 	AudioData.Audio = new N3D_Audio[AudioData.AudioCnt];
// 	//�����ȡ��Ƶ��Ϣ
// 	for (UINT i =0; i< AudioData.AudioCnt; i++)
// 	{
// 		//���浱ǰ��Ƶ�Ļ�����Ϣ
// 		memcpy(&(AudioData.Audio[i].strLen),pOffset,sizeof(AudioData.Audio[i].strLen));
// 		char* name =new char[AudioData.Audio[i].strLen+1];
// 		ZeroMemory(name,sizeof(name));
// 		memcpy(name, pOffset, AudioData.Audio[i].strLen);
// 		pOffset += AudioData.Audio[i].strLen;
// 		SAFE_DELETE_ARRAY(name);
//         //audioFileStringLength
//         memcpy(&(AudioData.Audio[i].fileNameLen),pOffset,sizeof(AudioData.Audio[i].fileNameLen));
//         pOffset += sizeof(AudioData.Audio[i].fileNameLen);
//         //audioFileName
//         AudioData.Audio[i].fileName.reserve(AudioData.Audio[i].fileNameLen);
//         memcpy(&(AudioData.Audio[i].fileName),pOffset,AudioData.Audio[i].fileNameLen);
// 		pOffset += AudioData.Audio[i].fileNameLen;
//         //audioTpye
//         memcpy(&(AudioData.Audio[i].type),pOffset,sizeof(AudioData.Audio[i].type));
//         pOffset += sizeof(AudioData.Audio[i].type);
// 		memcpy(&(AudioData.Audio[i].repeatCnt),pOffset,sizeof(AudioData.Audio[i].repeatCnt));
//         pOffset += sizeof(AudioData.Audio[i].repeatCnt);
// 		//
// 		
// 		/*
// 		CAudioPlayer* pAudio = new CAudioPlayer;
// 		
// 		//��ǰ��Ƶ�Ļ�����Ϣ
// 		strcpy(pAudio->name, AudioData.Audio[i].name.c_str());
// 		pAudio->index = m_pScene->audio.size();
// 		pAudio->m_repeatCnt = AudioData.Audio[i].repeatCnt;
// 		pAudio->type = AudioData.Audio[i].type;
// 		
// 		if (pAudio->Create((char*)AudioData.Audio[i].fileName.c_str(), AudioData.Audio[i].repeatCnt) == false)
// 		{
// 			SAFE_RELEASE_D3D(pAudio);
// 			SAFE_DELETE(pAudio);
// 		}
// 		m_pScene->audio.push_back((DWORD)pAudio);
// 		m_pSceneResource->audio.push_back((DWORD)pAudio);
// 		*/
// 	}
// 	
// 	//release memory
// 	SAFE_DELETE_ARRAY(AudioData.Audio);
//}
//��ȡFlash����
// VOID CGetN3DFileInfo_5::ReadFlashData()
// {	
// 	//�ҵ�Flash��Ϣ��λ��
// 	BYTE* pOffset = m_pSceneData + TableData.FlashPtr;
// 	
// 	//�õ�Flash����
// 	memcpy(&(FlashData.FlashCnt), pOffset, sizeof(FlashData.FlashCnt));
// 	if (FlashData.FlashCnt == 0)
// 		return;
// }
//-------------------------------------------------------------------
//����: IsTextureExist()
//����: 
//��;: �жϲ����Ƿ��Ѿ�����, ������ڷ���texture�ڳ���texture�б��е����������򷵻�-1
//-------------------------------------------------------------------
//int CGetN3DFileInfo_5::IsTextureExist(std::string texName, INT width, INT height, D3DFORMAT format)
//{
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
//	return -1;
//}
//-------------------------------------------------------------------
//����: GetTexOP()
//����: 
//��;: ��N3D�Ļ�ɫģʽ����ת����D3D�Ļ�ɫģʽ����
//-------------------------------------------------------------------
// D3DTEXTUREOP CGetN3DFileInfo_5::GetTexOP(const N3D_BlenderOP& n3d_BlenderOP)
// {	
// 	switch(n3d_BlenderOP)
// 	{
// 	case NBM_DISABLE:
// 		return D3DTOP_DISABLE;
// 		break;
// 	case NBM_REPLACE:
// 		return D3DTOP_SELECTARG1;
// 		break;
// 	case NBM_MODULATE:
// 		return D3DTOP_MODULATE;
// 		break;
// 	case NBM_MODULATE2:
// 		return D3DTOP_MODULATE2X;
// 		break;
// 	case NBM_MODULATE4:
// 		return D3DTOP_MODULATE4X;
// 		break;
// 	case NBM_ADD:
// 		return D3DTOP_ADD;
// 		break;
// 	case NBM_ADD_SIGNED:
// 		return D3DTOP_ADDSIGNED;
// 		break;
// 	case NBM_ADD_SMOOTH:
// 		return D3DTOP_ADDSMOOTH;
// 		break;
// 	case NBM_SUBTRACT:
// 		return D3DTOP_SUBTRACT;
// 		break;
// 	case NBM_BLEND_DIFFUSE_ALPHA:
// 		return D3DTOP_BLENDDIFFUSEALPHA;
// 		break;
// 	case NBM_BLEND_TEXTURE_ALPHA:
// 		return D3DTOP_BLENDTEXTUREALPHA;
// 		break;
// 	case NBM_BLEND_CURRENT_ALPHA:
// 		return  D3DTOP_BLENDCURRENTALPHA;
// 		break;
// 	case NBM_BLEND_MANUAL:
// 		return D3DTOP_BLENDFACTORALPHA;
// 		break;
// 	case NBM_DOTPRODUCT:
// 		return D3DTOP_DOTPRODUCT3;
// 		break;
// 	case NBM_BLEND_DIFFUSE_COLOUR:
// 		return D3DTOP_SELECTARG1;
// 		break;
// 	}
// 	
// 	return D3DTOP_DISABLE;
// }

//VOID CGetN3DFileInfo_5::ReadMaterialData()
//{	
//     m_preMtrlCnt = m_pScene->shader.size();
//     BYTE* pOffset = m_pSceneData + TableData.mtrlDataPtr;
//     //mtrl count
// 	memcpy(&(MaterialData.mtrlCnt), pOffset, sizeof(MaterialData.mtrlCnt));	
// 	pOffset += sizeof(MaterialData.mtrlCnt);
// 	//mtrl data
// 	MaterialData.mtrl = new N3D_MtrlInfo[MaterialData.mtrlCnt];
// 	
// 	
// 	for (UINT i= 0; i< MaterialData.mtrlCnt; i++)
// 	{
// 		
// 		CMaterial* pMtrl = new CMaterial(m_pScene->m_pd3dDevice);
// 		pMtrl->index = m_preMtrlCnt + i;
// 		
// 		//Edit by Baojiahui for the version 51007 nonfinite material string length 2009/2/24 
// 		//51007�汾����������û���ַ�����
// 		if(m_pScene->m_fileVersion < 5.1007f)
// 		{
// 		    CHAR* readName =new CHAR[N3D_MAX_NAME_LENGTH];
// 			ZeroMemory(readName,sizeof(readName));
//             memcpy(readName , pOffset, N3D_MAX_NAME_LENGTH);
// 			readName[N3D_MAX_NAME_LENGTH - 1] = '\0';
// 			pOffset += N3D_MAX_NAME_LENGTH;
// 			pMtrl->name = std::string(readName);
// 			SAFE_DELETE_ARRAY(readName);
// 			
// 
// 		}else
// 		{
// 			INT strLen;
// 			memcpy(&(strLen), pOffset, sizeof(strLen));	
// 			pOffset += sizeof(strLen);	
// 			char* readName =new char[strLen+1];
// 			ZeroMemory(readName, strLen + 1);
// 			memcpy(readName , pOffset, strLen);
// 			readName[strLen] = '\0';
// 			pOffset += strLen;
// 			pMtrl->name = std::string(readName);
// 			SAFE_DELETE_ARRAY(readName);		
// 		}
// 		memcpy(&(MaterialData.mtrl[i].BasalInfo), pOffset, sizeof(MaterialData.mtrl[i].BasalInfo));	
// 		pOffset += sizeof(MaterialData.mtrl[i].BasalInfo); 
// 		
// 		//strcpy(pMtrl->name,MaterialData.mtrl[i].BasalInfo.Name);
// 		
// 		/*#ifdef _DEBUG
// 		if (pFile)
// 		{
// 		std::string str = std::string("\n\nmaterial in atl ").append(std::string(pMtrl->name));
// 		str.append("\n");
// 		fwrite(str.c_str(), str.length(), 1, pFile);
// 		}
// #endif*/
// 		
// 		//mtrl->dif 
// 		pMtrl->d3dmtrl.Diffuse.a = ((MaterialData.mtrl[i].BasalInfo.diffuse)&0xff)/255.0f;
// 		pMtrl->d3dmtrl.Diffuse.r = ((MaterialData.mtrl[i].BasalInfo.diffuse>>24)&0xff)/255.0f;
// 		pMtrl->d3dmtrl.Diffuse.g = ((MaterialData.mtrl[i].BasalInfo.diffuse>>16)&0xff)/255.0f;
// 		pMtrl->d3dmtrl.Diffuse.b = ((MaterialData.mtrl[i].BasalInfo.diffuse>>8)&0xff)/255.0f;
//        
// 		//mtrl->amb
// 		pMtrl->d3dmtrl.Ambient.a = ((MaterialData.mtrl[i].BasalInfo.ambient)&0xff)/255.0f;
// 		pMtrl->d3dmtrl.Ambient.r = ((MaterialData.mtrl[i].BasalInfo.ambient>>24)&0xff)/255.0f;
// 		pMtrl->d3dmtrl.Ambient.g = ((MaterialData.mtrl[i].BasalInfo.ambient>>16)&0xff)/255.0f;
// 		pMtrl->d3dmtrl.Ambient.b = ((MaterialData.mtrl[i].BasalInfo.ambient>>8)&0xff)/255.0f;
// 		//mtrl->spe
// 		pMtrl->specular.r = ((MaterialData.mtrl[i].BasalInfo.specular>>24)&0xff)/(255.0f/**MaterialData.mtrl[i].BasalInfo.specularLevel*/);
// 		pMtrl->specular.g = ((MaterialData.mtrl[i].BasalInfo.specular>>16)&0xff)/(255.0f/**MaterialData.mtrl[i].BasalInfo.specularLevel*/);
// 		pMtrl->specular.b = ((MaterialData.mtrl[i].BasalInfo.specular>>8)&0xff)/(255.0f/**MaterialData.mtrl[i].BasalInfo.specularLevel*/);		
// 		pMtrl->specular.a = ((MaterialData.mtrl[i].BasalInfo.specular)&0xff)/(255.0f/**MaterialData.mtrl[i].BasalInfo.specularLevel*/);		
// 		
// 		pMtrl->specularLevel      = MaterialData.mtrl[i].BasalInfo.specularLevel; 
// 		pMtrl->d3dmtrl.Specular   = pMtrl->specularLevel / 100.0 * pMtrl->specular;
// 		pMtrl->d3dmtrl.Specular.a = 1.0f;	
// 		
// 		//power
// 		pMtrl->d3dmtrl.Power = MaterialData.mtrl[i].BasalInfo.shininess;//�߹����
// 		
// 		//mtrl->emi
// 		pMtrl->selfIllumination = (UINT)MaterialData.mtrl[i].BasalInfo.selfIllumination;
// 		//pMtrl->selfIllumination = 100.0f;
// 		pMtrl->d3dmtrl.Emissive.a = 1.0f;
// 		pMtrl->d3dmtrl.Emissive.r = pMtrl->selfIllumination / 100.0f * pMtrl->d3dmtrl.Diffuse.r;
// 		pMtrl->d3dmtrl.Emissive.g = pMtrl->selfIllumination / 100.0f * pMtrl->d3dmtrl.Diffuse.g;
// 		pMtrl->d3dmtrl.Emissive.b = pMtrl->selfIllumination / 100.0f * pMtrl->d3dmtrl.Diffuse.b;
// 		// pMtrl->type = MaterialData.mtrl[i].BasalInfo.shaderType;
// 		pMtrl->type = D3DSHADE_GOURAUD;
// 		
// 		pMtrl->opacity = MaterialData.mtrl[i].BasalInfo.opacity; //͸����
// 		
// 		if (pMtrl->opacity < 100)
// 			pMtrl->bAlphaBlend = TRUE;
// 		
// 		pMtrl->twoSide  = MaterialData.mtrl[i].BasalInfo.bTwoSide;
// 		
// 		pMtrl->bAlphaTest = MaterialData.mtrl[i].BasalInfo.alphaTest;
// 		
// // 		if(pMtrl->bAlphaTest)
// // 		{
// // 			pMtrl->bAlphaTest = TRUE;
// // 			// pMtrl->bAlphaBlend = FALSE;  // fuxb alphaBlend and alphatest��ì��
// // 		}
// 		
// 		pMtrl->AlpahaTestValue =MaterialData.mtrl[i].BasalInfo.alphaTestValue;
// 		pMtrl->fillMode = (_D3DFILLMODE)MaterialData.mtrl[i].BasalInfo.fillMode;
// 		
// 		//51001�汾����¼�Ƿ����ʹ��ģ��diffuseֵ�Ĳ���
// 		if(m_pScene->m_fileVersion >= 5.1001f)
// 		{
// 			memcpy(&(MaterialData.mtrl[i].bUseDiffuse), pOffset, sizeof(MaterialData.mtrl[i].bUseDiffuse));
// 			pOffset +=sizeof(MaterialData.mtrl[i].bUseDiffuse);
// 		}
// 		else
// 		{
// 			MaterialData.mtrl[i].bUseDiffuse =true;
// 		}
// 		pMtrl->bUseDiffuse = (BOOL) MaterialData.mtrl[i].bUseDiffuse;
// 		
// 		//Ŀǰ���ʶ���Ϊ�� kfrCntΪ0
// 		memcpy(&(MaterialData.mtrl[i].kfrCnt), pOffset, sizeof(MaterialData.mtrl[i].kfrCnt));
// 		pOffset +=sizeof(MaterialData.mtrl[i].kfrCnt);
// 		
// 		// memcpy(&(MaterialData.mtrl->kfrMtrlInfo), pOffset, sizeof(MaterialData.mtrl->kfrCnt));
// 		// pOffset +=sizeof(MaterialData.mtrl->kfrCnt);
// 		
// 		//��ȡ	 
// 		memcpy(&(MaterialData.mtrl[i].TextureLayerCount), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerCount));
// 		pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerCount);
// 		MaterialData.mtrl[i].TextureLayerList =new N3D_TextureLayer[MaterialData.mtrl[i].TextureLayerCount];
// 		for(UINT m = 0; m < MaterialData.mtrl[i].TextureLayerCount; m++)
// 		{
// 			TextureLayer* pTexLayer = new TextureLayer();
// 			//Edit by Baojiahui for the version 51007 nonfinite material string length 2009/2/24 -----------------
// 			//51007�汾��������ͼ������û���ַ�����
// 			if(m_pScene->m_fileVersion < 5.1007f)
// 			{
// 				CHAR* readName =new CHAR[N3D_MAX_NAME_LENGTH];
// 				ZeroMemory(readName,sizeof(readName));
// 				memcpy(readName , pOffset, N3D_MAX_NAME_LENGTH);
// 				readName[N3D_MAX_NAME_LENGTH - 1] = '\0';
// 				pOffset += N3D_MAX_NAME_LENGTH;
// 				pTexLayer->Name = std::string(readName);
// 				SAFE_DELETE_ARRAY(readName);			
// 			}else
// 			{
// 				INT strLen;
// 				memcpy(&(strLen), pOffset, sizeof(strLen));	
// 				pOffset += sizeof(strLen);	
// 				char* readName =new char[strLen+1];
// 				ZeroMemory(readName, strLen + 1);
// 				memcpy(readName , pOffset, strLen);
// 				readName[strLen] = '\0';
// 				pOffset += strLen;
// 				pTexLayer->Name = std::string(readName);
// 				SAFE_DELETE_ARRAY(readName);		
// 			}
// 			
// 			/*memcpy(MaterialData.mtrl[i].TextureLayerList[m].Name, pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].Name));
// 			pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].Name);
// 			 
// 
//             MaterialData.mtrl[i].TextureLayerList[m].Name[31] = '\0';*/
// 			
// 			memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].type), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].type));
// 			pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].type);
// 			
// 			memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].UVIndex), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].UVIndex));
// 			pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].UVIndex);
// 
// 			//MipMap //51004�汾���������ͼ���MipMap��Ϣ
// 			if(m_pScene->m_fileVersion >= 5.1004f)
// 			{
// 				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].Mipmap), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].Mipmap));
// 				pOffset += sizeof(MaterialData.mtrl[i].TextureLayerList[m].Mipmap);
// 			}
// 			
// 			switch(MaterialData.mtrl[i].TextureLayerList[m].type)
// 			{
// 			case TLT_BASAL:
// 				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam));
// 				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam);
// 				break;
// 
// 			case TLT_BUMPMAP:
// 				{
// 					INT type;
// 					memcpy(&(type), pOffset, sizeof(type));
// 					pOffset +=sizeof(type);
// 					BUMPMAPTYPE bumpType =(BUMPMAPTYPE)type;
// 					//������͹��ͼ
// 					CBumpMap* pBumpMap = new CBumpMap(m_pScene->m_pd3dDevice, &m_pScene->m_d3dpp, bumpType, m_pScene->d3dParam.SupportBestBumpFormat);
// 					m_pScene->bumpTexture.push_back(pBumpMap);
// 
// 					switch(bumpType)
// 					{
// 					case BUMPWAVEMAP:
// 						{//Bump������ͼ
// 							N3D_BumpwaterTextureParam bumpwaterTexParam;
// 							memcpy(&(bumpwaterTexParam), pOffset, sizeof(bumpwaterTexParam));
// 							pOffset +=sizeof(bumpwaterTexParam);
// 							pBumpMap->m_MapWidth = bumpwaterTexParam.width;
// 							pBumpMap->m_MapHeight = bumpwaterTexParam.height;
// 							if(pBumpMap->m_MapWidth ==0)
// 							{
// 							  pBumpMap->m_MapWidth = 256;
//                               pBumpMap->m_MapHeight = 256;
// 							}
// 							pBumpMap->m_xSize = bumpwaterTexParam.XScale;					//ˮ���Ƶ�X�����ܶ�
// 							pBumpMap->m_ySize = bumpwaterTexParam.YScale;					//ˮ���Ƶ�Y�����ܶ�
// 							pBumpMap->m_radius = bumpwaterTexParam.Radius;					//����
// 							pBumpMap->m_speed = bumpwaterTexParam.Speed;					//����
// 							
// 							pBumpMap->m_CenterX = bumpwaterTexParam.CenterX;	            // ���Ƶ����ĵ�����
// 							pBumpMap->m_CenterY = bumpwaterTexParam.CenterY;                // ���Ƶ����ĵ�����
// 							
// 							pBumpMap->InitBumpWaveMap(pBumpMap->m_MapWidth,
// 													  pBumpMap->m_MapHeight,
// 													  pBumpMap->m_xSize,
// 													  pBumpMap->m_ySize,
// 													  pBumpMap->m_radius,
// 													  pBumpMap->m_speed,
// 													  pBumpMap->m_CenterX,
// 													  pBumpMap->m_CenterY);
// 						}
// 						break;
// 					case BUMPNORMALMAP:     //Bump������ͼ
// 						{
// 							N3D_BasalTextureParam basalTexParam;
//                             memcpy(&(basalTexParam), pOffset, sizeof(basalTexParam));
// 							pOffset +=sizeof(basalTexParam);
//                             pBumpMap->m_MapWidth = basalTexParam.width;
// 							pBumpMap->m_MapHeight = basalTexParam.height;
// 							N3D_BumpNormalTextureParam  bumpnormalTexParam;
// 							memcpy(&(bumpnormalTexParam), pOffset, sizeof(bumpnormalTexParam));
// 							pOffset +=sizeof(bumpnormalTexParam);
// 							if(m_pScene->light.size() > 0 && bumpnormalTexParam.LightIndex < m_pScene->light.size())
// 							{
// 								CLight* pLight = *(m_pScene->light.begin()+ bumpnormalTexParam.LightIndex);
// 								pBumpMap->SetBumpMapLight(pLight);
// 							}
// 						}
// 						break;
// 					case BUMPENVMAP:       //Bump������ͼ
// 						{
// 							N3D_BumpEnvTextureParam bumpenvTexParam;
// 							memcpy(&(bumpenvTexParam), pOffset, sizeof(bumpenvTexParam));
// 							pOffset +=sizeof(bumpenvTexParam);
// 							pBumpMap->m_MapWidth = bumpenvTexParam.width;
//                             pBumpMap->m_MapHeight = bumpenvTexParam.height;
//                             pBumpMap->m_Noise1 = bumpenvTexParam.noise1;
//                             pBumpMap->m_Noise2 = bumpenvTexParam.noise2;
// 						}
// 						break;
// 					}
// 					
// 				}
// 				break;
// 
// 			case TLT_OPACITY:
// 				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_OpacityTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_OpacityTextureParam));
// 				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_OpacityTextureParam);
// 				
// 				// fuxb 2009-03-18 ==>
// 				// if(!pMtrl->bAlphaTest)
// 				pMtrl->bAlphaBlend = TRUE;
// 				// <==
// 				break;
// 				//ʵʱ������ͼ�Ķ�ȡ
// 			case TLT_RTFMAP:
// 				if(m_pScene->m_fileVersion >= 5.1006f)
// 				{
// 					memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_RTReflectionParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_RTReflectionParam));
// 					pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_RTReflectionParam);	
// 					//��ȡ���������ģ������
// 					int modelIdx;
//                     memcpy(&(modelIdx), pOffset, sizeof(modelIdx));
// 					pOffset +=sizeof(modelIdx);
// 					
//                     //����RTF����
// 					//���ģ�;��
// 					CModel** ppModel =m_pScene->model.begin();
// 					UINT modelCnt = m_pScene->model.size();
// 					BOOL rtfTexCreated = FALSE;
// 					if(modelIdx < modelCnt)
// 					{
//                         CModel* pModel = *(ppModel + modelIdx);
// 						//����һ��RTFMap
// 						CRTFMap* pRTFMap = new CRTFMap(pTexLayer->Name, m_pScene->m_pd3dDevice, &m_pScene->m_d3dpp, MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_RTReflectionParam.width, MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_RTReflectionParam.height, D3DFMT_R8G8B8,0.5f);
// 						pRTFMap->SetRefPlaneByModel((DWORD)pModel);
// 						pRTFMap->m_texProjMatrix = m_pScene->GetTextureProjMatrix();	
// 						//����RTFMap��ͼ����
// 						m_pScene->rtfTexture.push_back(pRTFMap);
// 						rtfTexCreated = TRUE;
// 					    pTexLayer->pTexture = (DWORD)pRTFMap;
// 					}
//                     //��ȡ�����������б�
// 					DWORD refModelCount;
//                     memcpy(&refModelCount, pOffset, sizeof(refModelCount));
// 					pOffset +=sizeof(refModelCount);
//                     
// 					for(UINT i =0; i<(UINT)refModelCount;i++)
// 					{
// 						int refModelIdx;
// 						memcpy(&refModelIdx, pOffset, sizeof(refModelIdx));
// 						pOffset +=sizeof(refModelIdx);
// 						//����
// 						if(rtfTexCreated && refModelIdx < modelCnt)
// 						{
// 							
// 							CRTFMap* pRTFMap = *(m_pScene->rtfTexture.end());
// 							//���ģ�;��
// 							CModel** ppModel =m_pScene->model.begin();
// 							UINT modelCnt = m_pScene->model.size();
// 							if(modelIdx < modelCnt)
// 							{
// 								CModel* pModel = *(ppModel + refModelIdx);
// 								
// 								//��ģ�ͼ��뵽������ͼ�ķ���ģ�Ͷ���
// 								pRTFMap->AddRefModel((DWORD)pModel);
// 								
// 								//��ģ����Ҫ��Ⱦ��������ͼ���棬�������Ӹ�����Ⱦ��Ϣ
// 								ADDITIONALRENDERINFO renderInfo;
// 								renderInfo.type = ADS_RTFMAP;
// 								renderInfo.pRenderToTexture =  (DWORD) pRTFMap;
// 								
// 								//����ģ�͵ĸ�����Ⱦ��Ϣ����
// 								pModel->m_additionalRenderInfo.push_back(renderInfo);
// 							}
// 						}
// 					}	
// 				}
// 				break;
// 			//ʵʱ������ͼ�Ķ�ȡ
// 			case TLT_RTTMAP:
// 				if(m_pScene->m_fileVersion >= 5.1006f)
// 				{
// 					memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_RTRefractionParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_RTRefractionParam));
// 					pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_RTRefractionParam);
// 
// 					//��ȡ���������ģ������
// 					int modelIdx;
//                     memcpy(&(modelIdx), pOffset, sizeof(modelIdx));
// 					pOffset +=sizeof(modelIdx);
// 					 //��ȡ�����������б�
// 					DWORD refModelCount;
//                     memcpy(&refModelCount, pOffset, sizeof(refModelCount));
// 					pOffset +=sizeof(refModelCount);
// 					  
// 					for(UINT i =0; i<(UINT)refModelCount;i++)
// 					{
// 						int refModelIdx;
// 						memcpy(&refModelIdx, pOffset, sizeof(refModelIdx));
// 						pOffset +=sizeof(refModelIdx);
// 					}	        
// 				}
// 				break;
// 			case TLT_REFLECT:
// 				//51002�汾��������ӷ������ຯ��
// 				if(m_pScene->m_fileVersion >= 5.1002f)
// 				{
// 					memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_ReflectTextureParam51002), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_ReflectTextureParam51002));
// 					pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_ReflectTextureParam51002);
// 					//������ͼ�ķ��䷽ʽ
// 					//ƽ�淴��
// 					if (MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_ReflectTextureParam51002.reflectionType == 0)
// 					{
// 						if (m_pScene->d3dParam.IsSupportTexGenSphereMap)
// 							pTexLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
// 						else
// 							pTexLayer->UVIndex = D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR;	
// 						pTexLayer->transformFlag = _D3DTEXTURETRANSFORMFLAGS(D3DTTFF_COUNT3 | D3DTTFF_PROJECTED);
// 					}
// 					//���淴��
// 					if (MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_ReflectTextureParam51002.reflectionType == 1)
// 					{
// 						if (m_pScene->d3dParam.IsSupportTexGenSphereMap)
// 							pTexLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
// 						else
// 							pTexLayer->UVIndex = D3DTSS_TCI_CAMERASPACENORMAL;
// 						pTexLayer->transformFlag = D3DTTFF_COUNT3;
// 					}
// 					//��Ч
// 					if (MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_ReflectTextureParam51002.reflectionType == -1)
// 					{
// 						pTexLayer->transformFlag = D3DTTFF_DISABLE;
// 						pTexLayer->UVIndex = D3DTSS_TCI_PASSTHRU;
// 					}
// 				}
// 				else
// 				{
// 					memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_ReflectTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_ReflectTextureParam));
// 					pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_ReflectTextureParam);
// 				
// 					//���淴��
// 					if (m_pScene->d3dParam.IsSupportTexGenSphereMap)
// 						pTexLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
// 					else
// 						pTexLayer->UVIndex = D3DTSS_TCI_CAMERASPACENORMAL;
// 					pTexLayer->transformFlag = D3DTTFF_COUNT3;
// 				}
// 				break;
// 
// 			case TLT_CUBEMAP:
// 				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_CubicTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_CubicTextureParam));
// 				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_CubicTextureParam);
// 				break;
// 
// 			case TLT_DYNAMIC:
// 				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_DynamicTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_DynamicTextureParam));
// 				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_DynamicTextureParam);
// 				break;
// 			/*
// 			case TLT_BUMPWATER:
// 				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BumpwaterTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BumpwaterTextureParam));
// 				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BumpwaterTextureParam);
// 				break;
// 				*/
// 				
// 			case TLT_MOVIE:
// 				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_MovieTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_MovieTextureParam));
// 				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_MovieTextureParam);
// 				break;
// 
// 			case TLT_FLASH:
// 				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_FlashTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_FlashTextureParam));
// 				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_FlashTextureParam);
// 				break;
// 			} // switch
// 			
// 			//51003�汾�������ģ�͵�UV��Ϣ
// 			if(m_pScene->m_fileVersion >= 5.1003f && MaterialData.mtrl[i].TextureLayerList[m].type != TLT_RTFMAP && MaterialData.mtrl[i].TextureLayerList[m].type != TLT_RTTMAP)
// 			{
// 				memcpy(&(pTexLayer->USpeed), pOffset, sizeof(pTexLayer->USpeed));
// 				pOffset +=sizeof(pTexLayer->USpeed);
// 				memcpy(&(pTexLayer->VSpeed), pOffset, sizeof(pTexLayer->VSpeed));
// 				pOffset +=sizeof(pTexLayer->VSpeed);
// 
// 				if (fabs(pTexLayer->USpeed) < 0.0001f) pTexLayer->USpeed = 0;
// 				if (fabs(pTexLayer->VSpeed) < 0.0001f) pTexLayer->VSpeed = 0;
// 			}
// 			pTexLayer->USpeed = - pTexLayer->USpeed/m_pScene->fps;
//             pTexLayer->VSpeed = - pTexLayer->VSpeed/m_pScene->fps;
// 
// 			if(m_pScene->m_fileVersion >= 5.1005f)
// 			{
// 				if(MaterialData.mtrl[i].TextureLayerList[m].type == TLT_CUBEMAP)
// 				{
// 					BYTE cubicType;
// 					memcpy(&(cubicType), pOffset, sizeof(cubicType));
// 					pOffset +=sizeof(cubicType);				   
// 				}			
// 			}
// 
// 			memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo));
// 			pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo);
// 			
// 			memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].ImageCount), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].ImageCount));
// 			pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].ImageCount);
// 			
// 			UINT imageCnt =MaterialData.mtrl[i].TextureLayerList[m].ImageCount;
// 			MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList = new int[imageCnt];
// 			int *pTemint = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList;
// 			memcpy(pTemint, pOffset, sizeof(int) * imageCnt);
// 			pOffset += sizeof(int) * imageCnt;
// 			
// 			//ZeroMemory(pTexLayer->Name,sizeof(pTexLayer->Name));
// 			
// 			//strcpy(pTexLayer->Name,MaterialData.mtrl[i].TextureLayerList[m].Name);
// 			pTexLayer->type =(N3D_TextureType)(MaterialData.mtrl[i].TextureLayerList[m].type);
// 			if (pTexLayer->type != TLT_REFLECT) //������ͼ�Զ�����UV
// 				pTexLayer->UVIndex = MaterialData.mtrl[i].TextureLayerList[m].UVIndex;
// 
// 			//���û�ɫģʽ
// 			pTexLayer->Blenderinfo.AlphaBlendOption=GetTexOP(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo.AlphaBlendOption);
// 			pTexLayer->Blenderinfo.ColorBlendOption=GetTexOP(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo.ColorBlendOption);
// 			
// 			//���û�ɫ����
// 			D3DCOLOR blendFactor = D3DCOLOR_COLORVALUE(0,0,0, MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo.AlphaBlendParam.BlenderOP_alpha);
// 			pTexLayer->Blenderinfo.AlphaBlendParam = blendFactor;
// 			blendFactor = D3DCOLOR_COLORVALUE(0,0,0, MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo.ColorBlendParam.BlenderOP_alpha);
// 			pTexLayer->Blenderinfo.ColorBlendParam = blendFactor;
// 			
// 			//������ʵ���ͼ�б�
// 			pMtrl->texLayersList.push_back(pTexLayer);
// 		}
// 
// 		//������ͼ---------------------------------------------------------------------------------------------------------
// 		for(m = 0; m < MaterialData.mtrl[i].TextureLayerCount; m++)
// 		{
// 			int pTexIdx;
// 			UINT n;
// 			CCubeMap* pCubeMap =NULL;
// 			CTexture* pTex =NULL;
// 			CDynamicTex* pDyTex =NULL;
// 			TextureLayer* pTexLayer = pMtrl->texLayersList[m];
//             std::string texName = std::string(pTexLayer->Name);
// 			switch(pTexLayer->type)
// 			{
// 			case TLT_BASAL:				
// 			case TLT_REFLECT:
// 				//�жϸò����Ƿ��Ѿ�������
// 				
// 				pTexIdx = IsTextureExist(texName, 
// 										MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.width, 
// 										MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.height,
// 										D3DFMT_A8R8G8B8);
// 				//�����µ���ͼ����
// 				if (pTexIdx == -1)
// 				{
// 					CTexture* pTex = new CTexture();
// 					pTex->index = m_pScene->texture.size();
// 					//Texture����������ͼ�������
// 					//strcpy(pTex->pathAndName, pTexLayer->Name.c_str());
//                     pTex->pathAndName = pTexLayer->Name;
// 					pTex->width = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.width;
// 					pTex->height = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.height;
// 					pTex->mipCount = MaterialData.mtrl[i].TextureLayerList[m].Mipmap;
// 					pTex->BP = D3DFMT_A8R8G8B8;
// 					pTex->type = BASIC_TYPE;
// 
// 					//��ɫģʽѡ��
// 					if(MaterialData.mtrl[i].BasalInfo.alphaTest)
// 						pTex->bAlphaTest= TRUE;
// 
// 					//ͼ����������
// 					pTex->imageIdx = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[0];		  
// 					m_pScene->texture.push_back(pTex);
// 					m_pSceneResource->texture.push_back(pTex);
// 					pTexLayer->pTexture = (DWORD)pTex;
// 				}
// 				else
// 				{
// 					pTexLayer->pTexture = (DWORD)m_pScene->texture[pTexIdx];
// 				}
// 				break;
// 			
// 			//͸����ͼD3DFMT_A8----------------------------------------------------------------------------------------
// 			case TLT_OPACITY:	
// 				//�жϸò����Ƿ��Ѿ�������
// 				pTexIdx = IsTextureExist(texName, 
// 										MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.width, 
// 										MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.height,
// 										D3DFMT_A8);
// 				//�����µ���ͼ����
// 				if (pTexIdx == -1)
// 				{
// 					CTexture* pTex = new CTexture();
// 					pTex->index = m_pScene->texture.size();
// 					//Texture����������ͼ�������
// 					//strcpy(pTex->pathAndName, pTexLayer->Name.c_str());
//                     pTex->pathAndName = pTexLayer->Name;
// 					pTex->width = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.width;
// 					pTex->height = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.height;
// 					pTex->mipCount = MaterialData.mtrl[i].TextureLayerList[m].Mipmap;
// 					pTex->BP = D3DFMT_A8R8G8B8;  //��ͼ����Ҫ����A8R8G8B8����
// 					pTex->type = OPACITY_TYPE;
// 
// 					//��ɫģʽѡ��
// 					if(MaterialData.mtrl[i].BasalInfo.alphaTest)
// 						pTex->bAlphaTest= TRUE;
// 
// 					//ͼ����������
// 					pTex->imageIdx = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[0];		  
// 					m_pScene->texture.push_back(pTex);
// 					m_pSceneResource->texture.push_back(pTex);
// 					pTexLayer->pTexture = (DWORD)pTex;
// 				}
// 				else
// 				{
// 					pTexLayer->pTexture = (DWORD)m_pScene->texture[pTexIdx];
// 				}
// 				break;
//             //ʵʱ����--------------------------------------------------------------------------------------------
// 			case TLT_RTFMAP:
// 
// 				break;
// 			case TLT_RTTMAP:
// 
// 
// 				break;
// 			//ʵʱ����--------------------------------------------------------------------------------------------
// 			//TLT_REALTIMESHADOW--------------------------------------------------------------------------------------
// 			case TLT_REALTIMESHADOW:
// 				break;
// 
// 			//TLT_BUMPMAP--------------------------------------------------------------------------------------
// 			case TLT_BUMPMAP:
//                 {
//                     CBumpMap* pBumpMap = m_pScene->bumpTexture[m_pScene->bumpTexture.size()-1];
// 					if (pBumpMap->m_type != BUMPWAVEMAP)
// 					{
// 						//�жϸò����Ƿ��Ѿ�������
// 						pTexIdx = IsTextureExist(texName, 
// 												pBumpMap->m_MapWidth, 
// 												pBumpMap->m_MapHeight,
// 												pBumpMap->m_BumpMapFormat);
// 						// if(!pTexIdx)
// 						if (pTexIdx == -1)
// 						{
// 							CTexture* pTex = new CTexture();
// 							pTex->index = m_pScene->texture.size();
// 							//Texture����������ͼ�������
// 							//strcpy(pTex->pathAndName, pTexLayer->Name);
//                             pTex->pathAndName = pTexLayer->Name;
// 							pTex->width = pBumpMap->m_MapWidth;//MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.width;
// 							pTex->height = pBumpMap->m_MapHeight;//MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.height;
// 							pTex->mipCount = 0;// �������0��MaterialData.mtrl[i].TextureLayerList[m].Mipmap;
// 							pTex->BP = D3DFMT_A8R8G8B8;  //��R8G8B8������Ȼ����InitBumpMap��ת��ΪU8V8����ͼ
// 							pTex->type = BUMP_TYPE;
// 
// 							//��ɫģʽѡ��
// 							if(MaterialData.mtrl[i].BasalInfo.alphaTest)
// 								pTex->bAlphaTest= TRUE;
// 							
// 							//ͼ����������
// 							pTex->imageIdx = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[0];		  
// 							m_pScene->texture.push_back(pTex);
// 							m_pSceneResource->texture.push_back(pTex);
// 							pBumpMap->m_pBumpTexture = pTex;
// 						}
// 						else
// 						{
// 							pBumpMap->m_pBumpTexture = m_pScene->texture[pTexIdx];
// 						}	
// 						pBumpMap->m_pBumpTexture->bumpIdx = m_pScene->bumpTexture.size() - 1;
// 					}
// 					pTexLayer->pTexture = (DWORD)pBumpMap;
// 				}
// 				break;
// 
// 			//CubeMap--------------------------------------------------------------------------------------
// 			case TLT_CUBEMAP:
// 				{
// 					//�ж��Ƿ�֧��CubeMap
// 					if (!m_pScene->d3dParam.IsSupportCUBEMAP) break;
// 
// 					INT width = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_CubicTextureParam.width;
// 					INT height = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_CubicTextureParam.height;
// 					INT mipCount = MaterialData.mtrl[i].TextureLayerList[m].Mipmap;
// 					
// 					pCubeMap = new CCubeMap(m_pScene->m_pd3dDevice, 0, D3DFMT_R8G8B8, width, mipCount);
// 
// 					//CubeMap�����ֺͱ��
// 					pCubeMap->index = m_pScene->cubeMap.size(); 
// 					pCubeMap->name = std::string(pTexLayer->Name);
// 
// 					for(n = 0 ; n< 6; n++)
// 					{
// 						INT imageIdx = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[n];
// 						switch(n)
// 						{
// 						case 0: //ǰ = +z
// 							pCubeMap->imagesIdx[4] = imageIdx;
// 							break;
// 						case 1: //�� = -z
// 							pCubeMap->imagesIdx[5] = imageIdx;
// 							break;
// 						case 2: //�� = -x
// 							pCubeMap->imagesIdx[1] = imageIdx;
// 							break;
// 						case 3: //�� = +x
// 							pCubeMap->imagesIdx[0] = imageIdx;
// 							break;
// 						case 4:  //�� = +y
// 							pCubeMap->imagesIdx[2] = imageIdx;
// 							break;
// 						case 5: // ��= -y
// 							pCubeMap->imagesIdx[3] = imageIdx;
// 							break;
// 						}
// 					}
// 
// 					//���볡����Դ����
// 					m_pScene->cubeMap.push_back(pCubeMap);
// 					m_pSceneResource->cubeMap.push_back(pCubeMap);
// 					pTexLayer->pTexture = DWORD(pCubeMap);
// 				}
// 				break;
// 
// 			//��̬��ͼ--------------------------------------------------------------------------------------
// 			case TLT_DYNAMIC:
// 				//�ж��Ƿ�֧�ֶ�̬��ͼ
// 				if (!m_pScene->d3dParam.IsSupportDynamicTexture) break;
// 
// 				//������̬��ͼ
// 				INT width = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_DynamicTextureParam.width;
// 				INT height = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_DynamicTextureParam.height;
// 				INT frameCnt = MaterialData.mtrl[i].TextureLayerList[m].ImageCount;
// 				INT fps = (INT)(frameCnt/MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_DynamicTextureParam.duration);
// 				pDyTex = new CDynamicTex(m_pScene->m_pd3dDevice, width, height, D3DFMT_A8R8G8B8, frameCnt, fps);
// 
// 				//��̬��ͼ�ı�ţ����ƣ���ʽ
// 				pDyTex->index = m_pScene->dynamicTexture.size();
// 				pDyTex->name = std::string(pTexLayer->Name);
// 
// 				//��ɫģʽѡ��
// 				if(MaterialData.mtrl[i].BasalInfo.alphaTest) pDyTex->bAlphaTest= TRUE;
// 
// 				//��̬��ͼÿ֡ͼƬ�ı��
// 				for(n = 0; n< frameCnt; n++)
// 					pDyTex->imagesIdx.push_back(MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[n]);
// 
// 				//���볡����Դ����
// 				m_pScene->dynamicTexture.push_back(pDyTex);
// 				m_pSceneResource->dynamicTexture.push_back(pDyTex);
// 				pTexLayer->pTexture = DWORD(pDyTex);
// 				break;
// 		  }
// 	  } // for material texture
// 	  
//       SAFE_DELETE_ARRAY(MaterialData.mtrl[i].TextureLayerList);
//       m_pScene->shader.push_back(pMtrl);
// 	  m_pSceneResource->shader.push_back(pMtrl);
// 	} // for each material
// 
// 	SAFE_DELETE_ARRAY(MaterialData.mtrl);	
//}

VOID CGetN3DFileInfo_5::ReadImageData(BYTE *pImageDataBuffer)
{
	m_pVGSCore->SunLoadImageDataFromBuffer(m_N3DHeader.Version, pImageDataBuffer);
}

//-------------------------------------------------------------------
//����: ReadTexData()
//����: 
//��;: ��ȡ��ͼ��Ϣ
//-------------------------------------------------------------------
//VOID CGetN3DFileInfo_5::ReadModelData()
//{
// 	BYTE* pOffset = m_pSceneData + TableData.meshDataPtr;
// 	
// 	//�õ�������ģ�͵�����
// 	memcpy(&(ModelData.modelCnt), pOffset, sizeof(ModelData.modelCnt));	
// 	pOffset += sizeof(ModelData.modelCnt);
// 	ModelData.model = new N3D_Model[ModelData.modelCnt];
// 
// 	//֮ǰ��ģ������
// 	m_preModelCnt = m_pScene->model.size();
//    
// 	//ģ�͵Ĳ����Ƿ���͸����Ϣ
// 	BOOL bTransModel = FALSE;
// 
// 	//��Mesh��Ϣת��Ϊģ����Ϣ
// 	for(int i= 0; i< ModelData.modelCnt; i++)
// 	{
// 		CModel* pModel = new CModel(m_pScene->m_pd3dDevice);
// 
// 		//ģ�ͱ��
// 		pModel->index = m_preModelCnt + i;
// 
// 		//ģ������
// 		memcpy(&(ModelData.model[i].strLen), pOffset, sizeof(ModelData.model[i].strLen));	
// 		pOffset += sizeof(ModelData.model[i].strLen);
// 
// 		char* name =new char[ModelData.model[i].strLen+1];
// 		ZeroMemory(name, ModelData.model[i].strLen + 1);
//         memcpy(name , pOffset, ModelData.model[i].strLen);
// 		name[ModelData.model[i].strLen] = '\0';
// 		pOffset += ModelData.model[i].strLen;
// 		pModel->name = std::string(name);
// 		SAFE_DELETE_ARRAY(name);
// 	
//         //ģ�͵Ķ�������
// 		memcpy(&(ModelData.model[i].vertexCnt), pOffset, sizeof(ModelData.model[i].vertexCnt));
// 		pOffset += sizeof(ModelData.model[i].vertexCnt);
// 		pModel->vertexCnt = ModelData.model[i].vertexCnt;
// 		
// 		//�����б�
// 		ModelData.model[i].vertex = new N3D_Vertex[ModelData.model[i].vertexCnt];
// 		pModel->vertex = new CVertex[pModel->vertexCnt];
// 		for(UINT j =0;j< ModelData.model[i].vertexCnt;j++)
// 		{
// 			pModel->vertex[j].index = j;
// 
// 			memcpy(&(ModelData.model[i].vertex[j]), pOffset, sizeof(ModelData.model[i].vertex[j]));
// 
// 			pModel->vertex[j].position.x = ModelData.model[i].vertex[j].position.x;
// 			pModel->vertex[j].position.y = ModelData.model[i].vertex[j].position.y;
// 			pModel->vertex[j].position.z = -ModelData.model[i].vertex[j].position.z;
// 			
// 			pModel->vertex[j].normal.x = ModelData.model[i].vertex[j].normal.x;
// 			pModel->vertex[j].normal.y = ModelData.model[i].vertex[j].normal.y;
// 			pModel->vertex[j].normal.z = -ModelData.model[i].vertex[j].normal.z;
// 			
// 			pModel->vertex[j].diffuse = (D3DCOLOR)ModelData.model[i].vertex[j].color;
// 			
// 			pModel->vertex[j].uv1.u =ModelData.model[i].vertex[j].uv1.u;
// 			pModel->vertex[j].uv1.v =ModelData.model[i].vertex[j].uv1.v;
// 			pModel->vertex[j].uv2.u =ModelData.model[i].vertex[j].uv2.u;
// 			pModel->vertex[j].uv2.v =ModelData.model[i].vertex[j].uv2.v;
// 			pModel->vertex[j].uv3.u =ModelData.model[i].vertex[j].uv3.u;
// 			pModel->vertex[j].uv3.v =ModelData.model[i].vertex[j].uv3.v;
// 			pModel->vertex[j].uv4.u =ModelData.model[i].vertex[j].uv4.u;
// 			pModel->vertex[j].uv4.v =ModelData.model[i].vertex[j].uv4.v;
// 			
// 			pOffset += sizeof(ModelData.model[i].vertex[j]);
// 		}
// 		SAFE_DELETE_ARRAY(ModelData.model[i].vertex);
// 		
//         //SubMesh����
// 		unsigned short curModelSubMeshCnt = 0;
//         memcpy(&curModelSubMeshCnt , pOffset, sizeof(curModelSubMeshCnt));
//         pOffset += sizeof(curModelSubMeshCnt);
//         bTransModel = FALSE;
// 		//SubMesh�б�
// 		for(j = 0; j< curModelSubMeshCnt; j++)
// 		{
// 			//����һ���µ�SubMesh
// 			N3D_SubMesh* pN3DSubMesh = new N3D_SubMesh();
// 			_subMesh subMesh;
// 			
// 			//SubMeshʹ�õĲ��ʱ��
// 			memcpy(&(pN3DSubMesh->mtrlIndex), pOffset, sizeof(pN3DSubMesh->mtrlIndex));
// 			pOffset += sizeof(pN3DSubMesh->mtrlIndex);     
// 				
// 			if (pN3DSubMesh->mtrlIndex >= 0)
// 			{
// 				//��ǰSubMeshʹ�õĲ���
// 				int mtrlIndex = m_preMtrlCnt + pN3DSubMesh->mtrlIndex;
// 				subMesh.shader = subMesh.oriShader = m_pScene->shader[mtrlIndex];
// 				if (/*subMesh.shader->bAlphaBlend||*/subMesh.shader->opacity < 100)  // fuxb 2009-03-06, alphaBlend����������
// 					bTransModel = TRUE;
// 			}
// 			else
// 				subMesh.shader =NULL;
// 			
// 			//������
// 			UINT IndexCount = 0;
// 			memcpy(&IndexCount, pOffset, sizeof(IndexCount));
// 			pOffset += sizeof(IndexCount);
// 			pN3DSubMesh->faceCnt = IndexCount/3;
// 			subMesh.faceCnt = pN3DSubMesh->faceCnt;
// 			subMesh.face = new CFace[subMesh.faceCnt];
// 
// 			//����˳��Ϊ��ʱ��
// 			if (IndexCount > 65536)  // ���ĸ��ֽڵĻ���
// 			{		
// 				//����32λ��������			
// 				pN3DSubMesh->face = new N3D_Face_32[pN3DSubMesh->faceCnt];
// 				
// 				// һ���Զ�ȡ��������
// 				memcpy(pN3DSubMesh->face, pOffset, 4 * IndexCount);
// 				pOffset += 4 * IndexCount;
// 				
// 				N3D_Face_32* pFace = (N3D_Face_32*)(pN3DSubMesh->face);
// 				for(UINT n =0; n< pN3DSubMesh->faceCnt;n++)
// 				{
// 					subMesh.face[n].index = n;
// 					subMesh.face[n].vertices0 = &(pModel->vertex[pFace[n].a]);
// 					subMesh.face[n].vertices1 = &(pModel->vertex[pFace[n].c]);
// 					subMesh.face[n].vertices2 = &(pModel->vertex[pFace[n].b]);
// 				}
// 			}
// 			else  // �������ֽڵĻ���
// 			{
// 				//����16λ��������
// 				pN3DSubMesh->face = new N3D_Face_16[pN3DSubMesh->faceCnt];		
// 				// һ���Զ�ȡ��������
// 				memcpy(pN3DSubMesh->face, pOffset, 2 * IndexCount);
// 				pOffset += 2 * IndexCount;
// 				
// 				N3D_Face_16 *pFace = (N3D_Face_16 *)(pN3DSubMesh->face);
// 				for(UINT n =0; n< pN3DSubMesh->faceCnt;n++)
// 				{
// 					subMesh.face[n].index =n;
// 					subMesh.face[n].vertices0 = &(pModel->vertex[pFace[n].a]);
// 					subMesh.face[n].vertices1 = &(pModel->vertex[pFace[n].c]);
// 					subMesh.face[n].vertices2 = &(pModel->vertex[pFace[n].b]);
// 				}				
// 			}	
// 			
// 			pModel->subMesh.push_back(subMesh);
// 
// 			SAFE_DELETE_ARRAY(pN3DSubMesh->face);
// 			SAFE_DELETE(pN3DSubMesh);
// 		}
// 			
// 		//��Χ��
// 		memcpy(&(ModelData.model[i].boundBox), pOffset, sizeof(ModelData.model[i].boundBox));
// 		pOffset += sizeof(ModelData.model[i].boundBox);
// 		pModel->boundingBox.MaxCorner.x = ModelData.model[i].boundBox.maxCorner.x;
// 		pModel->boundingBox.MaxCorner.y = ModelData.model[i].boundBox.maxCorner.y;
// 		pModel->boundingBox.MaxCorner.z = -ModelData.model[i].boundBox.maxCorner.z;
// 		
// 		pModel->boundingBox.MinCorner.x = ModelData.model[i].boundBox.minCorner.x;
// 		pModel->boundingBox.MinCorner.y = ModelData.model[i].boundBox.minCorner.y;
// 		pModel->boundingBox.MinCorner.z = -ModelData.model[i].boundBox.minCorner.z;
// 		
// 		pModel->oriBBox.MaxCorner =pModel->boundingBox.MaxCorner;
// 		pModel->oriBBox.MinCorner =pModel->boundingBox.MinCorner;
// 		
// 		pModel->pRBBox= new CRenderBoundBox(m_pScene->m_pd3dDevice);
// 		pModel->pRBBox->SetRBBoxVertex(pModel->oriBBox.MinCorner,pModel->oriBBox.MaxCorner);
// 		
// 		pModel->oriBSphere.Center = (pModel->oriBBox.MaxCorner + pModel->oriBBox.MinCorner)/2.0f;
// 		pModel->boundingSphere.Center =pModel->oriBSphere.Center;
// 		
// 		//��Χ��
// 		memcpy(&(ModelData.model[i].boundSphereRadius), pOffset, sizeof(ModelData.model[i].boundSphereRadius));
// 		pOffset +=sizeof(ModelData.model[i].boundSphereRadius);
// 		pModel->boundingSphere.Radius=ModelData.model[i].boundSphereRadius;
// 		pModel->oriBSphere.Radius = pModel->boundingSphere.Radius;
// 		
// 		//isBillboard
// 		memcpy(&(ModelData.model[i].isBillboard), pOffset, sizeof(ModelData.model[i].isBillboard));
// 		pOffset +=sizeof(ModelData.model[i].isBillboard);
// 		pModel->isBillboard =ModelData.model[i].isBillboard;
// 		if (pModel->isBillboard)
// 		{
// 			BYTE axis = 4;  // -y
// 			memcpy(&axis, pOffset, sizeof(axis));
// 			pOffset += sizeof(axis);  
// 			pModel->billboardDirectionAxis = _GetAxisDirection(axis);  // ��������ķ�����
// 		}
// 		
// 		// Collision
// 		memcpy(&(ModelData.model[i].isCollision), pOffset, sizeof(ModelData.model[i].isCollision));
// 		pOffset +=sizeof(ModelData.model[i].isCollision);  
// 		pModel->isCollision =ModelData.model[i].isCollision;
// 		
// 		//isGround
// 		memcpy(&(ModelData.model[i].isGround), pOffset, sizeof(ModelData.model[i].isGround));
// 		pOffset +=sizeof(ModelData.model[i].isGround);  
// 		pModel->isGround =ModelData.model[i].isGround;
// 
// 		// fuxb 2009-03-19 ==>
// 		if (pModel->isGround)
// 		{
// 			pModel->isCollision = TRUE;
// 		}
// 		// <==
// 		
// 		//isVisible
// 		memcpy(&(ModelData.model[i].isVisible), pOffset, sizeof(ModelData.model[i].isVisible));
// 		pOffset +=sizeof(ModelData.model[i].isVisible); 
// 		pModel->visible =ModelData.model[i].isVisible;
// 		
// 		//isClickable
// 		memcpy(&(ModelData.model[i].isClickable), pOffset, sizeof(ModelData.model[i].isClickable));
// 		pOffset +=sizeof(ModelData.model[i].isClickable); 
// 		pModel->clickable =ModelData.model[i].isClickable;
// 		
// 		// upAxis  billboradUpAxis
// 		BYTE billboradUpAxis = 2;  // local z axis
// 		memcpy(&billboradUpAxis, pOffset, sizeof(billboradUpAxis));
// 		pOffset += sizeof(billboradUpAxis);
// 		pModel->billboradUpAxis = _GetAxisDirection(billboradUpAxis);  // ģ���������ʱ��Up������
// 		
// 		// isAutoNormalise
// 		memcpy(&(ModelData.model[i].isAutoNormalise), pOffset, sizeof(ModelData.model[i].isAutoNormalise));
// 		pOffset += sizeof(ModelData.model[i].isAutoNormalise); 
// 		pModel->isAutoNormalise = ModelData.model[i].isAutoNormalise;
// 		
// 		//isSkyBox
// 		memcpy(&(ModelData.model[i].isSkyBox), pOffset, sizeof(ModelData.model[i].isSkyBox));
// 		pOffset += sizeof(ModelData.model[i].isSkyBox); 
// 		pModel->isSkyBox =ModelData.model[i].isSkyBox;   
// 		
// 		if(m_pScene->m_fileVersion >=5.1001f)
// 		{
// 			bool bReflection;
// 			memcpy(&(bReflection), pOffset, sizeof(bReflection));
// 			pOffset +=sizeof(bReflection); 
// 			
// 			bool bRefrection;
// 			memcpy(&(bRefrection), pOffset, sizeof(bRefrection));
// 			pOffset +=sizeof(bRefrection); 
// 		}
// 		
// 		int bindingSeketonIndex;
// 		memcpy(&(bindingSeketonIndex), pOffset, sizeof(bindingSeketonIndex));
// 		pOffset +=sizeof(bindingSeketonIndex); 
// 		
// 		pModel->m_FVF = FVF_T7;
// 		pModel->m_FVFSize =sizeof(T7MESHVERTEX);
// 
// 		//create vBuffer
// 		pModel->createVBuffer();
// 		//create IBuffer
// 		pModel->createIBuffer();
// 
// 		//push model
// 		m_pScene->model.push_back(pModel);
// 		m_pSceneResource->model.push_back(pModel);
// 		
// 		//push the transparent model
// 		if (bTransModel)
// 		    m_pScene->transModel.push_back(pModel);
// 
// 		//���ģ����Ҫ������Ⱦ���棬���ģ�͵�������Ⱦ���桪����͹�ϳ���ͼ
// 		BandModelToAddtionalSurface_Bump(pModel);
// 	}
// 
// 	//���¹�����ʵ�SubMeshList�б�
// 	m_pSceneResource->ConstructShaderSubMeshList();
// 
// 	//�ͷ���ʱ�ڴ�
// 	SAFE_DELETE_ARRAY(ModelData.model);
//}

//���ģ����Ҫ������Ⱦ���棬���ģ�͵�����ˮ����Ⱦ����
//VOID CGetN3DFileInfo_5::BandModelToAddtionalSurface_Bump(CModel* pModel)
//{
// 	INT subMeshCnt = pModel->subMesh.size();
// 	_subMesh* _pSubMesh = pModel->subMesh.begin();
// 	for (INT i = 0; i< subMeshCnt; i++)
// 	{
// 		_subMesh* pSubMesh = &(*(_pSubMesh + i));
// 		TextureLayer* pTexLayer = pSubMesh->shader->IsContainBumpWaveMap();
// 		if (pTexLayer)
// 		{
// 			//�жϸ�����Ⱦ�����Ƿ��Ѿ����뵽ģ�͸�����Ⱦ�б���ֹ�ظ�����
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
// 			//��ģ����Ҫ��Ⱦ����͹�ϳ���ͼ���棬�������Ӹ�����Ⱦ��Ϣ
// 			if (isExist == false)
// 			{
// 				ADDITIONALRENDERINFO renderInfo;
// 				renderInfo.type = ADS_BUMPWAVEMAP;
// 				renderInfo.pRenderToTexture = pTexLayer->pTexture;
// 				pModel->m_additionalRenderInfo.push_back(renderInfo);
// 			}
// 		}
// 	}
//}

//���쳡��ͼ
// VOID CGetN3DFileInfo_5::ConstructSceneGraph(CSceneNode** parentNode, N3D_Node* pNode, BYTE** offset)
// {
// 	//���������е�һ���ӽڵ���Ϣ----------------------------------------
// 	//�ڵ������
// 	memcpy(&(pNode->type), *offset, sizeof(pNode->type));
// 	*offset +=sizeof(pNode->type);
// 	memcpy(&(pNode->index), *offset, sizeof(pNode->index));
//     *offset +=sizeof(pNode->index);
//     //5.1004�汾����˽ڵ������
//     if(m_pScene->m_fileVersion >= 5.1004f)
// 	{
//       memcpy(&(pNode->strLen), *offset, sizeof(pNode->strLen));
// 	  *offset +=sizeof(pNode->strLen);
// 
// 	  char* name =new char[pNode->strLen + 1];
// 	  ZeroMemory(name, pNode->strLen + 1);
//       memcpy(name , *offset, pNode->strLen);
// 	  name[pNode->strLen] = '\0';
// 	  *offset += pNode->strLen;
// 	  pNode->name = std::string(name);
// 	  SAFE_DELETE_ARRAY(name);
// 	}
// 
// 	//���ݿ���
// 	DWORD ID1,ID2;
//     memcpy(&(ID1), *offset, sizeof(ID1));
// 	(*offset) +=sizeof(ID1);
// 	memcpy(&(ID2), *offset, sizeof(ID2));
//     (*offset) +=sizeof(ID2);
// 	
// 	N3D_NodeTransform pNodeTransform;
// 	memcpy(&(pNodeTransform), *offset, sizeof(pNodeTransform));
//     (*offset) +=sizeof(pNodeTransform);
// 
//     //�����ڵ����
// 	CSceneNode* pSceneNode = new CSceneNode();
//     //�ڵ������
// 	pSceneNode->type =(INT)pNode->type;
// 
// 	pSceneNode->name = pNode->name;
// 	
// 	//����Ǹ��ڵ�
// 	if (pSceneNode->type == NNT_SCENEROOT)
// 		m_pSceneResource->m_pRootNode = pSceneNode;
// 	
// 	//����и��ڵ�
// 	if(parentNode)
// 	{
// 		(*parentNode)->m_child.push_back(pSceneNode);
// 		pSceneNode->m_parent =(*parentNode);
// 	}
// 
// 
// 	//����ڵ�ı任����-------------------------------------------------------
// 	//�ⲿ�ֱȽϸ��ӣ�N3D�ļ����������������ϵ��Ҫת��Ϊ��������ϵ
// 	//-------------------------------------------------------------------------
// 	//λ��, Zȡ��
// 	D3DXVECTOR3 pos = D3DXVECTOR3(pNodeTransform.position.x,
// 									pNodeTransform.position.y, 
// 									-pNodeTransform.position.z);
// 	//ת��, XYȡ��
// 	D3DXQUATERNION quat = D3DXQUATERNION(-pNodeTransform.quat.x, 
// 											-pNodeTransform.quat.y, 
// 											pNodeTransform.quat.z, 
// 											pNodeTransform.quat.w);
// 	
// 	//����
// 	D3DXVECTOR3 scale;
// 	if (pNode->type != NNT_MODEL)
// 		scale = D3DXVECTOR3(1,1,1);
// 	else
// 		scale = D3DXVECTOR3(pNodeTransform.scale.x, pNodeTransform.scale.y, pNodeTransform.scale.z);
// 
// 	//��������
// 	if(pNode->type == NNT_MODEL)
// 	{
// 		if (pNode->index >= 0 && (pNode->index+ m_preModelCnt) < m_pScene->model.size())
// 		{
// 			CModel* pModel = *(m_pScene->model.begin() + (pNode->index+ m_preModelCnt));
// 			pModel->pNode = pSceneNode;
// 		}
//     }
// 	
// 	//ԭ��������ϵ�ı任����
// 	D3DXMATRIX transMat;
//     //��ñ任����
//     D3DXMatrixTransformation(&transMat, &D3DXVECTOR3(0,0,0), &D3DXQUATERNION(0, 0, 0, 0), 
// 								&scale, &D3DXVECTOR3(0,0,0), &quat, &pos);
// 
// 
// 	
//     D3DXVECTOR3 dir = D3DXVECTOR3(0, 0, 1);
// 
// 	//���ڵ�
// 	if (pNode->type == NNT_SCENEROOT)
// 	{
// 		pSceneNode->curTransform.matrix = m_pSceneResource->m_transform;
// 		pSceneNode->curTransform.worldMatrix = m_pSceneResource->m_transform;
// 		pSceneNode->updateTransform();
// 	}
// 	else //�����ڵ�
// 	{
// 		pSceneNode->curTransform.matrix = transMat;
// 		pSceneNode->updateTransform();
// 	}
// 	
// 	//�Ƿ��ж���
// 	float animationLength =0.0f;
// 	
// 	memcpy(&(animationLength), (*offset), sizeof(animationLength));
// 	(*offset) +=sizeof(animationLength);
// 	//���ڹؼ�֡����
// 	if(animationLength>0)
// 	{
// 		UINT keyFrameCnt;
// 		memcpy(&(keyFrameCnt), (*offset), sizeof(keyFrameCnt));
// 		(*offset) +=sizeof(keyFrameCnt);
// 		pSceneNode->keyFrameCnt =keyFrameCnt;
// 		pSceneNode->kfrtransform = new D3DXMATRIX[keyFrameCnt];
// 		
// 		for(UINT i =0 ; i< keyFrameCnt; i++)
// 		{
// 			N3D_NodeTransform pNodeTransform;
// 			memcpy(&(pNodeTransform), (*offset), sizeof(pNodeTransform));
// 			(*offset) +=sizeof(pNodeTransform);
// 			//����ڵ�ı任����
// 			D3DXMATRIX transMat;
// 			//pos
// 			D3DXVECTOR3 pos = D3DXVECTOR3(pNodeTransform.position.x, 
// 											pNodeTransform.position.y, 
// 											-pNodeTransform.position.z);
// 			//quat
// 			D3DXQUATERNION quat = D3DXQUATERNION(-pNodeTransform.quat.x, 
// 												-pNodeTransform.quat.y, 
// 												pNodeTransform.quat.z, 
// 												pNodeTransform.quat.w);
// 			//scale
// 			D3DXVECTOR3 scale;
// 			if (pNode->type != NNT_MODEL)
// 				scale =D3DXVECTOR3(1,1,1);
// 			else
// 				scale =D3DXVECTOR3(pNodeTransform.scale.x,pNodeTransform.scale.y,pNodeTransform.scale.z);
// 
// 			//D3DXVECTOR3 scale =D3DXVECTOR3(pNodeTransform.scale.x,pNodeTransform.scale.y,pNodeTransform.scale.z);
// 			//��ñ任����
// 			D3DXMatrixTransformation(&transMat,NULL,NULL,&scale,NULL,&quat,&pos);
// 			
// 			//д��ÿ֡�ı任����
// 			pSceneNode->kfrtransform[i]=transMat;
// 		} // for each keyframe  
// 		
// 		// if ���ڹؼ�֡����
// 	}
// 	else
// 	{
// 		pSceneNode->keyFrameCnt =0;
// 		pSceneNode->kfrtransform = NULL;
// 	}
// 
// 
// 	switch (pNode->type)
// 	{
// 	case NNT_SCENEROOT:
// 		break;
// 	case NNT_CAMERANODE:
// 		{
// 			if (pNode->index >=0 && (pNode->index + m_preCameraCnt) < m_pScene->camera.size())
// 			{
// 				CCamera* pCamera = *(m_pScene->camera.begin() + (pNode->index + m_preCameraCnt));
// 				
// 				pSceneNode->m_object = (DWORD)pCamera;
// 				pSceneNode->name = std::string(pCamera->name);
// 				pCamera->pNode = pSceneNode;
// 				
// 				//��Camera��pos,Quat,rot��Ϣת�����ϰ汾��from,At,Roll
// 				//�����from���������WorldPosition
// 				//�����At����(0,0,-1)*�������ת����*�̶���+from��
// 				//�����Roll�������ת�����Z����
// 				D3DXVECTOR3 Origin_from;
// 				D3DXVECTOR4 transVec;
// 				D3DXVECTOR3 Origin_at,Origin_up;				
// 				//float Origin_roll;
// 				
// 				Origin_from   = pSceneNode->worldPosition;
// 				//Origin_from.z = -Origin_from.z;
// 				dir    = D3DXVECTOR3(0, 0, 100);
// 				
// 				D3DXVec3TransformNormal(&Origin_at, &dir, &pSceneNode->curTransform.worldMatrix);
//                 //Origin_at.z = Origin_at.z;
//                 
// 				D3DXVec3Add(&Origin_at, &Origin_from, &Origin_at);
// 			    
// 				dir = D3DXVECTOR3(0,1,0);
//                 D3DXVec3TransformNormal(&Origin_up, &dir, &pSceneNode->curTransform.worldMatrix);
// 				//���Roll����������
// 				//Origin_roll = pSceneNode->worldRotation.z;
// 				//if (Origin_roll == PI) Origin_roll = 0;
// 
// 				pCamera->m_from = Origin_from;
// 				pCamera->m_at = Origin_at;
// 				pCamera->m_up = D3DXVECTOR3(pSceneNode->curTransform.worldMatrix._21,
// 							pSceneNode->curTransform.worldMatrix._22,
// 							pSceneNode->curTransform.worldMatrix._23);
// 							//D3DXVECTOR3(0,1,0);
// 				//pCamera->m_roll = Origin_roll;
// 				
// 				//set for transform
// 				pCamera->Update();
// 				
// 				//set the camera Keyframe data
// 				pCamera->kfrCnt = pSceneNode->keyFrameCnt;
// 				if (pCamera->kfrCnt > m_pScene->m_KfrCnt)
// 				{
// 					m_pScene->m_KfrCnt   = pCamera->kfrCnt;
// 					m_pScene->m_EndFrame = pCamera->kfrCnt;
// 				}
// 				if (pSceneNode->keyFrameCnt > 1)
// 				{
// 					pCamera->kfrInfo = new CAMINFO[pSceneNode->keyFrameCnt];
// 					
// 					//�ؼ�֡���������еĶ�̬ģ��Ч��������ÿһ֡�����Բ�ͬ
// 					pCamera->kfrFSMotionBlur = new BOOL[pSceneNode->keyFrameCnt];
// 					ZeroMemory(pCamera->kfrFSMotionBlur, sizeof(BOOL) * pSceneNode->keyFrameCnt);
// 					pCamera->kfrFSMotionBlurValue = new INT[pSceneNode->keyFrameCnt];		
// 					ZeroMemory(pCamera->kfrFSMotionBlurValue, sizeof(INT) * pSceneNode->keyFrameCnt);
// 					
// 					//�ؼ�֡���������е�JS Function���ã�����ÿһ֡�����Բ�ͬ
// 					pCamera->InvokeFuncName = new _NAME[pSceneNode->keyFrameCnt];
// 					ZeroMemory(pCamera->InvokeFuncName, sizeof(_NAME) * pSceneNode->keyFrameCnt);
// 					
// 					for (UINT i = 0; i < pSceneNode->keyFrameCnt; i++)
// 					{
// 						pSceneNode->curTransform.matrix = pSceneNode->kfrtransform[i];
// 						pSceneNode->updateTransform();
// 						
// 						D3DXVECTOR3 from = pSceneNode->worldPosition;
// 						//from.z = -from.z;
// 						
// 						dir = D3DXVECTOR3(0, 0, 1);
// 						
// 						D3DXVECTOR3 at;
//                         D3DXVec3TransformNormal(&at, &dir, &pSceneNode->curTransform.worldMatrix);
// 						//at.z = -at.z;
// 						D3DXVec3Add(&at, &from, &at);	
// 
// 						pCamera->kfrInfo[i].From = from;
// 						pCamera->kfrInfo[i].At   = at;
// 						//pCamera->kfrInfo[i].Rol  = pSceneNode->worldRotation.z;
// 						pCamera->kfrInfo[i].Up = D3DXVECTOR3(pSceneNode->curTransform.worldMatrix._21,
// 							pSceneNode->curTransform.worldMatrix._22,
// 							pSceneNode->curTransform.worldMatrix._23);
// 						
// 					} // for					
// 				} // if keyFrameCnt
// 				
// 				//for water test
// 				//pCamera->m_from = D3DXVECTOR3(-27, 8, 0);
// 				//pCamera->m_at = D3DXVECTOR3(-27, 8, 1);
// 				//pCamera->m_up = D3DXVECTOR3(0, 1, 0);
// 				//pCamera->Update();
// 			} // if pNode->index			
//             
// 			//pCamera->pNode = pSceneNode;
// 		} 
// 		break;
// 	case NNT_LIGHTNODE:
// 		{
//             if (pNode->index >= 0 && (pNode->index + m_preLightCnt) < m_pScene->light.size())
// 			{
// 				CLight* pLight = *(m_pScene->light.begin() + (pNode->index + m_preLightCnt));
// 				pSceneNode->m_object = (DWORD)pLight;
// 				pSceneNode->name = std::string(pLight->name);
// 				
// 				pLight->pNode  = pSceneNode;
// 				pLight->kfrCnt = pSceneNode->keyFrameCnt;  // �ؼ�֡��Ŀ
// 				
// 				//���µƹ����ݣ����õƹ��λ��
// 				pLight->param.Position.x = pLight->pNode->worldPosition.x;
// 				pLight->param.Position.y = pLight->pNode->worldPosition.y;
// 				pLight->param.Position.z = -pLight->pNode->worldPosition.z;
// 				m_pScene->m_pd3dDevice->SetLight(pNode->index, &(pLight->param));		
// 				m_pScene->m_pd3dDevice->LightEnable(pNode->index, TRUE);				
// 			}
// 		}
// 		
// 		break;
// 	case NNT_MODEL:
// 		{
// 			if ((pNode->index + m_preModelCnt)< m_pScene->model.size())
// 			{
// 				CModel* pModel = *(m_pScene->model.begin() + (pNode->index + m_preModelCnt));
//                 pSceneNode->m_object =(DWORD)pModel;
// 				pSceneNode->name = std::string(pModel->name);
// 				pModel->pNode = pSceneNode;
// 				pModel->update();
// 				if(pModel->pNode->keyFrameCnt>0)
// 				{
// 					pModel->kfrCnt =pModel->pNode->keyFrameCnt;
// 					if (pModel->kfrCnt > m_pScene->m_KfrCnt)
// 					{
// 						m_pScene->m_KfrCnt = pModel->kfrCnt;
// 						m_pScene->m_EndFrame = pModel->kfrCnt;
// 					}
// 				}
// 				//����ģ�͵İ�Χ����Ϣ
// 				pModel->pRBBox->SetWorldMatrix(&(pModel->pNode->curTransform.worldMatrix));
// 				
// 			}
// 		}
// 		break;
// 	}
// 	UINT childCnt =0;
// 	memcpy(&(childCnt), (*offset), sizeof(childCnt));
// 	(*offset) +=sizeof(childCnt);
// 	for (UINT i =0 ;i< childCnt;i++)
// 	{
// 		ConstructSceneGraph(&pSceneNode,pNode, offset);	      
// 	}   
// }
// 
// 
// VOID CGetN3DFileInfo_5::ReadSceneGraph()
// {
// 	BYTE* pOffset = m_pSceneData + TableData.sceneGraphDataPtr;
// 	N3D_Node* pRootNode = new N3D_Node;
// 	ConstructSceneGraph(NULL, pRootNode, &pOffset);
// 	SAFE_DELETE(pRootNode);
// }

//-------------------------------------------------------------------
//����: Release()
//����: 
//��;: �ͷŹ�������
//-------------------------------------------------------------------
VOID CGetN3DFileInfo_5::Release()
{	
	SAFE_DELETE_ARRAY(N3DBuffer);
	SAFE_DELETE_ARRAY(m_pSceneData);
	if (pDebugFile)
		fclose(pDebugFile);
}

// VOID CGetN3DFileInfo_5::ReadButtonData()
// {
// 	BYTE* pOffset = m_pSceneData +  TableData.ButtonPtr;
// 	
// 	m_preButtonCnt = m_pScene->button.size();
// 	
// 	// ����Button��������
// 	UINT btnCount = 0;
// 	CPY_FROM_STREAM(btnCount, pOffset);
// 	if (btnCount <= 0)
// 	{
// 		return;
// 	}
// 	
// 	// ��������button��Ϣ
// 	for (UINT iBtn = 0; iBtn < btnCount; ++iBtn)
// 	{
// 		N3D_Button Info;
// 		// ��ȡbutton������Ϣ
// 		CPY_FROM_STREAM(Info.BasalInfo, pOffset);
// 		
// 		CButton* pButton = new CButton();
// 		
// 		pButton->index = m_preButtonCnt + iBtn;
// 		
// 		//����pbutton������
// 		strcpy(pButton->name, Info.BasalInfo.Name);
// 		
// 		//����button��Rect
// 		RECT btnRect = Info.BasalInfo.Rect;
// 		
// 		//����Button��͸����
// 		INT opacity = Info.BasalInfo.Opacity;
// 		
// 		//����button�Ļص���������
// 		strcpy(pButton->callBackFunc, Info.BasalInfo.CallBackFunc);			
// 		
// 		//����button��Overlay��Index
// 		if (Info.BasalInfo.BtnOverlayIndex >= 0 
// 			&& Info.BasalInfo.BtnOverlayIndex < m_pScene->overlay.size())
// 		{
// 			pButton->btnOverlay = m_pScene->overlay[Info.BasalInfo.BtnOverlayIndex];
// 		}
// 		pButton->btnOverlay->SetRect(&btnRect);
// 		pButton->btnOverlay->SetOpacity(opacity);
// 		
// 		//button MouseUp Mtrl 
// 		pButton->pMtrl_MouseUp = pButton->btnOverlay->m_pMtrl;
// 		
// 		//����button��mouseOver Mtrl��Index
// 		INT MouseOverMtrlIndex = Info.BasalInfo.MouseOverMtrlIndex; 
// 		// if (MouseOverMtrlIndex != -1)
// 		if (MouseOverMtrlIndex >= 0 && MouseOverMtrlIndex < m_pScene->shader.size())
// 		{
// 			pButton->pMtrl_MouseOver = m_pScene->shader[MouseOverMtrlIndex];
// 		}
// 		else
// 		{
// 			pButton->pMtrl_MouseOver= pButton->pMtrl_MouseUp;
// 		}
// 		
// 		//����button��mouseDown Mtrl��Index
// 		INT MouseDownMtrlIndex = Info.BasalInfo.MouseDownMtrlIndex;
// 		// if (MouseDownMtrlIndex != -1)
// 		if (MouseDownMtrlIndex >= 0 && MouseDownMtrlIndex < m_pScene->shader.size())
// 		{
// 			pButton->pMtrl_MouseDown = m_pScene->shader[MouseDownMtrlIndex];
// 		}else
// 		{
// 			pButton->pMtrl_MouseDown= pButton->pMtrl_MouseUp;
// 		}
// 		
// 		//����Button�Ĺؼ�֡����
// 		CPY_FROM_STREAM(Info.KfrCnt, pOffset);
// 		pButton->m_kfrCnt = Info.KfrCnt;
// 		pButton->m_kfrInfo = NULL;
// 		
// 		//����Button�Ĺؼ�֡����
// 		if (pButton->m_kfrCnt > 0)
// 		{
// 			pButton->m_kfrInfo = new ButtonKfrInfo[pButton->m_kfrCnt];  // ���䰴ťkeyframe���ڴ�
// 			
// 			// Info.BasalInfo.KfrInfo = new E3D_3_1_ButtonInfo[pButton->m_kfrCnt];
// 			for (INT j = 0; j < pButton->m_kfrCnt; j++)
// 			{
// 				N3D_ButtonFrameInfo FrameInfo;
// 				CPY_FROM_STREAM(FrameInfo, pOffset);
// 				pButton->m_kfrInfo[j].Rect    = FrameInfo.Rect;
// 				pButton->m_kfrInfo[j].Opacity = FrameInfo.Opacity;
// 			}
// 		}
// 		
// 		//��pButton Push��image�б�
// 		m_pScene->button.push_back(pButton);
// 		m_pSceneResource->button.push_back(pButton);
// 	}
// }
// VOID CGetN3DFileInfo_5::ReadNavigaterData()
// {
// 	BYTE* pOffset = m_pSceneData +  TableData.NavigatotPtr;
// 	
// 	// m_preNaviCnt = m_pScene->button.size();  // �������Ѿ����ڵĵ���ͼ��Ŀ
// 	
// 	// ����Navigator��������
// 	UINT naviCount = 0;
// 	CPY_FROM_STREAM(naviCount, pOffset);
// 	if (naviCount <= 0)
// 	{
// 		return;
// 	}
// 	
// 	// ��������Navigator��Ϣ
// 	for (UINT iNavi = 0; iNavi < naviCount; ++iNavi)
// 	{
// 		CNavigater* pNavigater = new CNavigater();
// 		if (pNavigater == NULL) 
// 		{
// 			return;
// 		}
// 		
// 		N3D_Navigater Info;
// 		
// 		// ��ȡnavigator�Ļ�����Ϣ
// 		CPY_FROM_STREAM(Info.BasalInfo, pOffset);
// 		
// 		//����pNavigater������
// 		strcpy(pNavigater->name, Info.BasalInfo.Name);
// 		
// 		//����navigate��navRect
// 		pNavigater->navRect = Info.BasalInfo.Rect;
//         pNavigater->navRect.right =pNavigater->navRect.right-pNavigater->navRect.left;
// 		pNavigater->navRect.bottom = pNavigater->navRect.bottom-pNavigater->navRect.top;
// 		
//         pNavigater->navRect.top =0;
//         pNavigater->navRect.left =0;
// 		
// 		//����navigate��mapOverlay��Index
// 		INT overlayIndex = Info.BasalInfo.MapOverlayIndex;
// 		if (overlayIndex >= 0 && overlayIndex < m_pScene->overlay.size())
// 		{
// 			pNavigater->pNavigateMap = m_pScene->overlay[overlayIndex];
// 		}
// 		else
// 		{
// 			pNavigater->pNavigateMap = NULL;
// 		}
// 		
// 		//����Pointer������
// 		CPY_FROM_STREAM(Info.PointerCnt, pOffset);
// 		
// 		//����Pointer����
// 		INT pointerCnt = Info.PointerCnt;	
// 		if (pointerCnt > 0)
// 		{
// 			for (INT j = 0; j < pointerCnt; j++)
// 			{
// 				N3D_NavigaterPointer PointInfo;
// 				
// 				CPY_FROM_STREAM(PointInfo, pOffset);
// 				
// 				NavPointer* pNavPointer = new NavPointer;
// 				
// 				//Pointer����
// 				strcpy(pNavPointer->Name, PointInfo.Name);
// 				
// 				//Pointer Overlay���
// 				INT OLIndex = PointInfo.OverlayIndex;
// 				pNavPointer->pPointerOverlay = NULL;
// 				if (OLIndex >= 0 && OLIndex < m_pScene->overlay.size())
// 				{
// 					pNavPointer->pPointerOverlay = m_pScene->overlay[OLIndex];
// 					pNavPointer->pPointerOverlay->useRelativeSize = FALSE;
// 				}
// 				
// 				//Pointer����Ķ�������
// 				pNavPointer->NavPointerType = PointInfo.Type;
// 				
// 				//Pointer����Ķ���ı��
// 				CModel* pModel = NULL;
// 				CCamera* pCamera = NULL;
// 				BOOL addPointer = TRUE;
// 				if (pNavPointer->NavPointerType == 0)
// 				{
// 					INT index = PointInfo.ObjIndex;
// 					if (index >= 0 && index <= m_pScene->camera.size())
// 					{
// 						CCamera* pCamera = m_pScene->camera[index];
// 						pNavPointer->NavPointerHandle = (DWORD)pCamera;
// 					}else
// 					{
// 						MessageBox(NULL, "���������Ϊ��", "δָ�����������",MB_OK|MB_ICONEXCLAMATION);
// 						addPointer =FALSE;
// 						pNavPointer->NavPointerHandle =NULL;
// 					}
// 				}
// 				if (pNavPointer->NavPointerType == 1)
// 				{
// 					INT index = PointInfo.ObjIndex;
// 					if(index> 0 && index <= m_pScene->model.size())
// 					{
// 						CModel* pModel = m_pScene->model[index];
// 						pNavPointer->NavPointerHandle = (DWORD)pModel;
// 					}else
// 					{
// 						MessageBox(NULL, "������ģ��Ϊ��", "δָ��������ģ��",MB_OK|MB_ICONEXCLAMATION);
// 						addPointer =FALSE;
// 						pNavPointer->NavPointerHandle =NULL;
// 					}
// 				}
//                 if(addPointer)
// 				{
// 					//������ָ����뵼��ͼ����
// 					pNavigater->AddNavPointer(pNavPointer);	
// 				}else
// 				{
// 					SAFE_DELETE(pNavPointer);
// 				}
// 				
// 			}  // for pointerCnt
// 			
// 		} // if pointerCnt
// 		
// 		//����navigate��sceneLeft
// 		pNavigater->sceneLeft = Info.BasalInfo.sceneLeft;
// 		
// 		//����navigate��sceneTop
// 		pNavigater->sceneTop = Info.BasalInfo.sceneTop;
// 								
// 		//����navigate��sceneRight
// 		pNavigater->sceneRight = Info.BasalInfo.sceneRight;
// 		
// 		//����navigate��sceneBottom
// 		pNavigater->sceneBottom = Info.BasalInfo.sceneBottom;
// 		
// 		//�� navigater Push�� navigater �б�
// 		m_pScene->navigater.push_back(pNavigater);
// 		m_pSceneResource->navigater.push_back(pNavigater);
// }
//	}
	
// VOID CGetN3DFileInfo_5::ReadControlData()
// {
// 	BYTE* pOffset = m_pSceneData +  TableData.ControlPtr;
// 	
// 	// ������ƶ�������
// 	UINT controlCount = 0;
// 	CPY_FROM_STREAM(controlCount, pOffset);
// 	if (controlCount <= 0)
// 	{
// 		return;
// 	}
// 	
// 	// Walker����Ŀ���б�
// 	UINT WalkerCount = 0;
// 	CPY_FROM_STREAM(WalkerCount, pOffset);
// 	for (UINT iWalker = 0; iWalker < WalkerCount; ++iWalker)
// 	{
// 		N3D_Walker WalkerInfo;
// 		CPY_FROM_STREAM(WalkerInfo, pOffset);
// 		
// 		CWalker* pWalker = new CWalker;
// 		
// 		strcpy(pWalker->name, WalkerInfo.Name);
// 		
// 		// ���cameraIndex�ĺ�����		
// 		if (WalkerInfo.CameraIndex < m_pScene->camera.size())
// 		{
// 			pWalker->m_pCamera = *(m_pScene->camera.begin() + WalkerInfo.CameraIndex);
// 		}
// 		else
// 		{
// 			pWalker->m_pCamera = &(m_pScene->defaultCam);
// 			//MessageBox(NULL, "Walker�޷��ҵ�������Զ��л���ȱʡ���", "�޷��ҵ����",MB_OK|MB_ICONEXCLAMATION);
// 		}
// 		
// 		pWalker->walkSpeed  = WalkerInfo.walkSpeed;
// 		pWalker->walkAcc    = WalkerInfo.walkAcc;
// 		pWalker->rosSpeed   = WalkerInfo.rosSpeed;
// 		pWalker->rosAcc     = WalkerInfo.rosAcc;
// 		pWalker->height     = WalkerInfo.height;
// 		pWalker->bCollision = WalkerInfo.bCollision;
// 		pWalker->colDis     = WalkerInfo.colDis;
// 		pWalker->bWeight    = WalkerInfo.bWeight;
// 		pWalker->liftSpeed  = WalkerInfo.liftSpeed;
// 		pWalker->liftAcc    = WalkerInfo.liftAcc;
// 		pWalker->upperAngle = WalkerInfo.upperAngle;
// 		pWalker->lowerAngle = WalkerInfo.lowerAngle;
// 		
// 		m_pScene->walker.push_back(pWalker);
// 		m_pSceneResource->walker.push_back(pWalker);
// 	}
// 	
// 	// Editor����Ŀ���б�
// 	UINT EditorCount = 0;
// 	CPY_FROM_STREAM(EditorCount, pOffset);
// 	for (UINT iEditor = 0; iEditor < EditorCount; ++iEditor)
// 	{
// 		// ���뱣���Editor��Ϣ
// 		N3D_Editor EditorInfo;
// 		CPY_FROM_STREAM(EditorInfo, pOffset);
// 		
// 		// ����Editor����
// 		CEditor* pEditor = new CEditor;
// 		
// 		strcpy(pEditor->name, EditorInfo.Name);
// 		
// 		
// 		pEditor->bIsAutoRoundRoll = EditorInfo.bIsAutoRoundRoll;
// 		
// 		pEditor->autoPitchSpeed   = EditorInfo.autoSpeedX;
// 		pEditor->autoRoundSpeed   = EditorInfo.autoSpeedY;
// 		pEditor->autoRollSpeed    = 0;//EditorInfo.autoRollSpeed;
// 		
// 		pEditor->manualPitchSpeed = EditorInfo.manualSpeedX;
// 		pEditor->manualRoundSpeed = EditorInfo.manualSpeedY;
// 		
// 		pEditor->manualPanXSpeed  = EditorInfo.manualPanXSpeed;
// 		pEditor->manualPanYSpeed  = EditorInfo.manualPanYSpeed;
// 		
// 		pEditor->dollySpeed       = EditorInfo.dollySpeed;
// 		pEditor->dollyAcc         = EditorInfo.dollyAcc;
// 		
// 		pEditor->dollyNearDis     = EditorInfo.dollyNearDis;
// 		pEditor->dollyFarDis      = EditorInfo.dollyFarDis;
// 		// ���cameraIndex�ĺ�����
// 		if (EditorInfo.CameraIndex < m_pScene->camera.size()) // Խ����
// 		{
// 			pEditor->m_pCamera = *(m_pScene->camera.begin() + EditorInfo.CameraIndex);
// 		}
// 		else
// 		{
// 			pEditor->m_pCamera = &(m_pScene->defaultCam);
// 			//MessageBox(NULL, "Editor�޷��ҵ�������Զ��л���ȱʡ���", "�޷��ҵ����",MB_OK|MB_ICONEXCLAMATION);
// 		}
// 		D3DXVECTOR3 camAt(EditorInfo.LookAtPos.x, EditorInfo.LookAtPos.y, -EditorInfo.LookAtPos.z);
// 		pEditor->m_pCamera->m_at = camAt;
// 		pEditor->lookatPos = camAt;
// 		
// 		m_pScene->editor.push_back(pEditor);
// 		m_pSceneResource->editor.push_back(pEditor);
// 	}
// 	
// 	// Flyer����Ŀ���б�
// 	UINT FlyerCount = 0;
// 	CPY_FROM_STREAM(FlyerCount, pOffset);
// 	for (UINT iFlyer = 0; iFlyer < FlyerCount; ++iFlyer)
// 	{
// 		// ���뱣���Flyer��Ϣ
// 		N3D_Flyer FlyerInfo;
// 		CPY_FROM_STREAM(FlyerInfo, pOffset);
// 		
// 		// ����Flyer����
// 		CFlyer* pFlyer = new CFlyer;			
// 		
// 		strcpy(pFlyer->name, FlyerInfo.Name);
// 		
// 		// ���cameraIndex�ĺ�����
// 		if (FlyerInfo.CameraIndex < m_pScene->camera.size()) // Խ����
// 		{
// 			pFlyer->m_pCamera = *(m_pScene->camera.begin() + FlyerInfo.CameraIndex);				
// 		}
// 		else
// 		{
// 			pFlyer->m_pCamera = &(m_pScene->defaultCam);
// 			//MessageBox(NULL, "Flyer�޷��ҵ�������Զ��л���ȱʡ���", "�޷��ҵ����",MB_OK|MB_ICONEXCLAMATION);
// 		}
// 		
// 		pFlyer->panSpeed     = FlyerInfo.panSpeed;
// 		pFlyer->panAcc       = FlyerInfo.panAcc;
// 		pFlyer->dollySpeed   = FlyerInfo.dollySpeed;
// 		pFlyer->dollyAcc     = FlyerInfo.dollyAcc;
// 		pFlyer->dollyNearDis = FlyerInfo.dollyNearDis;
// 		pFlyer->dollyFarDis  = FlyerInfo.dollyFarDis;
// 		pFlyer->dollySpeed   = FlyerInfo.dollySpeed;
// 		pFlyer->dollyAcc     = FlyerInfo.dollyAcc;
// 		pFlyer->rotateSpeed  = FlyerInfo.rotateSpeed;
// 		pFlyer->rotateAcc    = FlyerInfo.rotateAcc;
// 		pFlyer->liftSpeed    = FlyerInfo.liftSpeed;
// 		pFlyer->liftAcc      = FlyerInfo.liftAcc;
// 		pFlyer->upperAngle   = FlyerInfo.upperAngle;
// 		pFlyer->lowerAngle   = FlyerInfo.lowerAngle;
// 		pFlyer->bCollision   = FlyerInfo.bCollision;
// 		pFlyer->colDis       = FlyerInfo.colDis;
// 		
// 		m_pScene->flyer.push_back(pFlyer);
// 		m_pSceneResource->flyer.push_back(pFlyer);
// 	}
// 	
// 	// ��ǰ���ƶ��������
// 	N3D_ControlData ControlData;
// 	CPY_FROM_STREAM(ControlData.ControlType, pOffset);
// 	CPY_FROM_STREAM(ControlData.ControlerIndex, pOffset);
// 	m_pScene->controlType = ControlData.ControlType;
// 	switch (ControlData.ControlType)
// 	{
// 	case 0:
// 		if (!m_pScene->currentWalker)
// 		{
// 			m_pScene->currentWalker = *(m_pScene->walker.begin() + ControlData.ControlerIndex);
// 		}
// 		break;
// 	case 1:
// 		if (!m_pScene->currentEditor)
// 		{
// 			m_pScene->currentEditor = *(m_pScene->editor.begin() + ControlData.ControlerIndex);
// 		}
// 		break;
// 	case 2:
// 		if (!m_pScene->currentFlyer)
// 		{
// 			m_pScene->currentFlyer = *(m_pScene->flyer.begin() + ControlData.ControlerIndex);
// 		}
// 		break;
// 	}
// }


//Ϊoverlay����
// bool SortOverlayByOrder(COverlay* p1,COverlay* p2)
// {
// 	if(p1->ZOrder< p2->ZOrder)
// 		return true;	
// 	else
// 		return false;
// }
// VOID CGetN3DFileInfo_5::ReadOverlayData()
// {
// 	BYTE* pOffset = m_pSceneData +  TableData.OverlayPtr;
// 	
// 	m_preOverlayCnt = m_pScene->overlay.size();
// 	
// 	// ����Overlay��������
// 	UINT overlayCount = 0;
// 	CPY_FROM_STREAM(overlayCount, pOffset);
// 	if (overlayCount <= 0)
// 	{
// 		return;
// 	}
// 	
// 	// ��������overlay��Ϣ
// 	for (UINT iOverlay = 0; iOverlay < overlayCount; ++iOverlay)
// 	{
// 		N3D_Overlay Info;
// 		
// 		// ��ȡoverlay������Ϣ
// 		CPY_FROM_STREAM(Info.BasalInfo, pOffset);
// 		
// 		COverlay* pOverlay = new COverlay(m_pScene->m_pd3dDevice);
// 		
// 		pOverlay->index = m_preOverlayCnt + iOverlay;
// 		
// 		//����Overlay������
// 		strcpy(pOverlay->name, Info.BasalInfo.Name);
// 		
// 		//д��Overlay��ƫ������ԭ���ʾ
// 		pOverlay->offsetOrigin = Info.BasalInfo.OffsetOrigin;
// 		
// 		//����Overlay��λ�ñ�ʾ(������껹�Ǿ�������)
// 		pOverlay->useRelativePos = Info.BasalInfo.UseRelativePos;
// 		
// 		//����Overlay�ߴ�
// 		RECT Rect = Info.BasalInfo.Rect;
// 		
// 		//������Գߴ�
// 		pOverlay->m_x1Percent = (FLOAT)Rect.left / m_pScene->m_oriWidth;
// 		pOverlay->m_y1Percent = (FLOAT)Rect.top / m_pScene->m_oriHeight;
// 		pOverlay->m_x2Percent = (FLOAT)Rect.right / m_pScene->m_oriWidth;
// 		pOverlay->m_y2Percent = (FLOAT)Rect.bottom / m_pScene->m_oriHeight;
// 		
// 		//����Overlayת��
// 		FLOAT angle = Info.BasalInfo.Angle;
// 		pOverlay->SetRotation(angle);
// 		
// 		//�Ƿ��Ǳ���ͼ
// 		pOverlay->isBackdrop = Info.BasalInfo.IsBackdrop;
// 		pOverlay->m_bVisible = Info.BasalInfo.IsVisible;
//         //51003�汾�������overlay��Zorder
// 		if(m_pScene->m_fileVersion >= 5.1003f)
//             CPY_FROM_STREAM(pOverlay->ZOrder, pOffset);
// 		//����Overlay�Ĳ���ID
// 		if (Info.BasalInfo.MaterialIndex != -1)
// 		{
// 			UINT index = Info.BasalInfo.MaterialIndex;
// 			pOverlay->m_pMtrl = *(m_pScene->shader.begin() + index);
// 			pOverlay->m_pMtrl->bIsOverlayMtrl = TRUE;
// 		
// 			//����Overlay�õ�����ͼ��MipMapΪ1
// 			int texLayerCnt = pOverlay->m_pMtrl->texLayersList.size();
// 			//��������ͼ���������ͼ���б���㰴������ͼ����ɫ
// 			if (texLayerCnt > 0)
// 			{
// 				TextureLayer** ppTexLayer = pOverlay->m_pMtrl->texLayersList.begin();
// 				for(int i = 0; i< texLayerCnt; i++)
// 				{
// 					TextureLayer* pTexLayer = *(ppTexLayer+i);
// 					CTexture* pTex = (CTexture*)pTexLayer->pTexture;
// 					pTex->mipCount = 1;
// 				}
// 			}
// 
// 			pOverlay->m_pMtrl->selfIllumination = 100;
// 			pOverlay->m_imageWidth = Rect.right - Rect.left;
// 			pOverlay->m_imageHeight = Rect.bottom - Rect.top;
// 		}
// 
// 		//����Overlay��͸����
// 		INT opacity = Info.BasalInfo.Opacity;
// 		pOverlay->SetOpacity(opacity);
// 		
// 		//����Overlay�Ƿ�ʹ���趨�ľ��Գߴ�
// 		pOverlay->useRelativeSize = Info.BasalInfo.UseRelativeSize;
// 		
// 		if (!pOverlay->useRelativeSize)
// 		{
// 			pOverlay->SetRect(&Rect);
// 			pOverlay->offsetRect = Rect;
// 		}
// 		else
// 		{
// 			INT width = m_pScene->rect.right - m_pScene->rect.left;
// 			INT height = m_pScene->rect.bottom - m_pScene->rect.top;
// 			if (pOverlay->useRelativePos)
// 			{
// 				Rect.left = pOverlay->m_x1Percent * width;
// 				Rect.top  = pOverlay->m_y1Percent * height;
// 			}
// 			Rect.right  = pOverlay->m_x2Percent * width;
// 			Rect.bottom = pOverlay->m_y2Percent * height;
// 			pOverlay->SetRect(&Rect);
// 			pOverlay->offsetRect = Rect;
// 		}
// 		
// 		//����Overlay�Ĺؼ�֡����
// 		CPY_FROM_STREAM(Info.KfrCnt, pOffset);
// 		pOverlay->m_kfrCnt = Info.KfrCnt;
// 		pOverlay->m_kfrInfo = NULL;
// 		
// 		//д��ؼ�֡����
// 		if (pOverlay->m_kfrCnt > 0)
// 		{
// 			pOverlay->m_kfrInfo = new OverlayKfrInfo[pOverlay->m_kfrCnt];
// 			for (INT j = 0; j< pOverlay->m_kfrCnt; j++)
// 			{
// 				N3D_OverlayFrameInfo FrameInfo;
// 				CPY_FROM_STREAM(FrameInfo, pOffset);
// 				pOverlay->m_kfrInfo[j].Rect    = FrameInfo.Rect;
// 				pOverlay->m_kfrInfo[j].Angle   = FrameInfo.Angle;
// 				pOverlay->m_kfrInfo[j].Opacity = FrameInfo.Opacity;
// 			}
// 		}
// 		
// 		// ����Overlay
// 		m_pScene->overlay.push_back(pOverlay);
// 		m_pSceneResource->overlay.push_back(pOverlay);
// 		
// 	} // for
// 
// 	//Ϊoverlay��������
//     if(m_pScene->m_fileVersion >= 5.1003f)
// 	{
//      sort(m_pScene->overlay.begin(), m_pScene->overlay.end(), SortOverlayByOrder);
// 	 sort(m_pSceneResource->overlay.begin(), m_pSceneResource->overlay.end(), SortOverlayByOrder);
// 	}
// 	
// }
// 
// VOID CGetN3DFileInfo_5::ReadTextData()
// {
// 	
// }
// 
// VOID CGetN3DFileInfo_5::FindReferenceModel()
// {
// 	
// }
// 
// VOID CGetN3DFileInfo_5::ReadModelGroupData()
// {
// 	
// }
// 
// VOID CGetN3DFileInfo_5::ReadLightData()
// {
// 	BYTE* pOffset = m_pSceneData +  TableData.lightDataPtr;
//     
// 	//����ƹ�����
// 	memcpy(&(LightData.lightCnt), pOffset, sizeof(LightData.lightCnt));
// 	pOffset += sizeof(LightData.lightCnt);
//     m_preLightCnt = m_pScene->light.size();
// 	//����ƹ���ϸ����
// 	LightData.light = new N3D_Light[LightData.lightCnt];
//     
// 	for (UINT i = 0; i< LightData.lightCnt; i++)
// 	{
// 		CLight* pLight = new CLight(m_pScene->m_pd3dDevice);
// 		
// 		//����light����
// 		//memcpy(&(LightData.light[i].name), pOffset, sizeof(LightData.light[i].name));
// 		//pOffset += sizeof(LightData.light[i].name);
// 
// 		//Edit by Baojiahui for the version 51007 nonfinite camera string length 2009/2/24 
// 		//��ȡ���������
// 		//51007�汾���������û���ַ�����
// 		if(m_pScene->m_fileVersion < 5.1007f)
// 		{
// 		    CHAR* readName =new CHAR[N3D_MAX_NAME_LENGTH];
// 			ZeroMemory(readName,sizeof(readName));
//             memcpy(readName , pOffset, N3D_MAX_NAME_LENGTH);
// 			readName[N3D_MAX_NAME_LENGTH - 1] = '\0';
// 			pOffset += N3D_MAX_NAME_LENGTH;
// 			pLight->name = std::string(readName);
// 			SAFE_DELETE_ARRAY(readName);
// 		}else
// 		{
// 			INT strLen;
// 			memcpy(&(strLen), pOffset, sizeof(strLen));	
// 			pOffset += sizeof(strLen);	
// 			char* readName =new char[strLen+1];
// 			ZeroMemory(readName, strLen + 1);
// 			memcpy(readName , pOffset, strLen);
// 			readName[strLen] = '\0';
// 			pOffset += strLen;
// 			pLight->name = std::string(readName);
// 			SAFE_DELETE_ARRAY(readName);		
// 		}
// 
// 		memcpy(&(LightData.light[i].type), pOffset, sizeof(LightData.light[i].type));
// 		pOffset += sizeof(LightData.light[i].type);
// 		
// 		//�ƹ�����,��ͬ�ĵƹ����ͱ����趨��ͬ�Ĳ���--------------------------
// 		//pLight->name = std::string(LightData.light[i].name);
// 		
// 		//	pLight->param.Type = (_D3DLIGHTTYPE)LightData.light[i].type;
// 		
// 		switch (LightData.light[i].type)
// 		{
// 		case NLT_OMINI:
// 			pLight->param.Type= D3DLIGHT_POINT;
// 			LightData.light[i].pOmniLight =new N3D_OMNI();
// 			memcpy(LightData.light[i].pOmniLight, pOffset, sizeof(*(LightData.light[i].pOmniLight)));
// 			pOffset += sizeof(*(LightData.light[i].pOmniLight));
// 			
// 			//ǿ��
// 			pLight->multiply = LightData.light[i].pOmniLight->multiply;
// 			
// 			//��ɫ
// 			pLight->diffuse.r = ((LightData.light[i].pOmniLight->color>>24) &0xff)/255.0f;
// 			pLight->diffuse.g = ((LightData.light[i].pOmniLight->color>>16) &0xff)/255.0f;
// 			pLight->diffuse.b = ((LightData.light[i].pOmniLight->color>>8) &0xff)/255.0f;
//             pLight->diffuse.a =1.0f;
// 			pLight->param.Diffuse.r = pLight->diffuse.r * pLight->multiply;
// 			pLight->param.Diffuse.g = pLight->diffuse.g * pLight->multiply;
// 			pLight->param.Diffuse.b = pLight->diffuse.b * pLight->multiply;
// 			pLight->param.Diffuse.a = 1.0f;
// 			
// 			//�����⣬����������ģ��ȫ�ֹ�
// 			pLight->ambient   = pLight->diffuse;
// 			pLight->ambient.a = 1.0f;
// 			pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb �Ѳ���0,05��Ϊ��0.0f
// 			pLight->param.Ambient.a = 1.0f;
// 			
// 			//�߹�
// 			// pLight->specular = 0.0 * pLight->diffuse;
// 			pLight->param.Specular   = pLight->multiply * pLight->diffuse;  // fuxb 2007-10-16 ��3ds max������ͬ�Ĵ���ʽ
// 			pLight->param.Specular.a = 1.0f;
// 			
//             //���ߵ������Զ����
// 			pLight->param.Range        = LightData.light[i].pOmniLight->attenuationRange;
// 			pLight->param.Attenuation0 = LightData.light[i].pOmniLight->attenuation0;
// 			pLight->param.Attenuation1 = LightData.light[i].pOmniLight->attenuation1;
// 			pLight->param.Attenuation2 = LightData.light[i].pOmniLight->attenuation2;			
//             
// 			memcpy(&(LightData.light[i].FrameCount), pOffset, sizeof(LightData.light[i].FrameCount));
// 			pOffset += sizeof(LightData.light[i].FrameCount);
// 			pLight->kfrCnt = LightData.light[i].FrameCount;
// 			if (LightData.light[i].FrameCount > 0)
// 			{
// 				pLight->pAnimOmniLight = new N3D_OMNI[LightData.light[i].FrameCount];
// 				memcpy(&(pLight->pAnimOmniLight[0]), pOffset, LightData.light[i].FrameCount* sizeof(N3D_OMNI));
// 				pOffset += LightData.light[i].FrameCount * sizeof(N3D_OMNI);
// 			}  
// 			break;
// 			
// 		case NLT_SPOTLIGHT:
// 			pLight->param.Type= D3DLIGHT_SPOT;
// 			LightData.light[i].pSpotLightInfo = new N3D_SPOTLIGHT();
// 			memcpy(LightData.light[i].pSpotLightInfo, pOffset, sizeof(*(LightData.light[i].pSpotLightInfo)));
// 			pOffset += sizeof(*(LightData.light[i].pSpotLightInfo));
// 			//ǿ��
// 			pLight->multiply = LightData.light[i].pSpotLightInfo->multiply;
// 			//��ɫ
// 			pLight->diffuse.r = ((LightData.light[i].pSpotLightInfo->color >> 24) & 0xff)/255.0f;
// 			pLight->diffuse.g = ((LightData.light[i].pSpotLightInfo->color >> 16) & 0xff)/255.0f;
// 			pLight->diffuse.b = ((LightData.light[i].pSpotLightInfo->color >> 8) & 0xff)/255.0f;
// 			pLight->ambient = 0.0f * pLight->diffuse;
// 			pLight->param.Diffuse.r = pLight->diffuse.r * pLight->multiply;
// 			pLight->param.Diffuse.g = pLight->diffuse.g * pLight->multiply;
// 			pLight->param.Diffuse.b = pLight->diffuse.b * pLight->multiply;
// 			pLight->param.Diffuse.a = 1.0f;
// 			
// 			//�����⣬����������ģ��ȫ�ֹ�
// 			pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; // fuxb �Ѳ���0,05��Ϊ��0.0f
// 			pLight->param.Ambient.a = 1.0f;
// 			
// 			//�߹�
// 			pLight->param.Specular = pLight->diffuse;  // fuxb 2007-10-16 ��3ds max������ͬ�Ĵ���ʽ
// 			
//             //���ߵ������Զ����
// 			pLight->param.Range        = LightData.light[i].pSpotLightInfo->attenuationRange;
// 			pLight->param.Attenuation0 = LightData.light[i].pSpotLightInfo->attenuation0;
// 			pLight->param.Attenuation1 = LightData.light[i].pSpotLightInfo->attenuation1;
// 			pLight->param.Attenuation2 = LightData.light[i].pSpotLightInfo->attenuation2;
// 			
// 			// ����������ϵ�еķ���
// 			pLight->param.Direction.x = LightData.light[i].pSpotLightInfo->direction.x;
// 			pLight->param.Direction.y = LightData.light[i].pSpotLightInfo->direction.y;
// 			pLight->param.Direction.z = -LightData.light[i].pSpotLightInfo->direction.z;
// 			
//             //��������ķ�Χ��
// 			pLight->param.Theta   = LightData.light[i].pSpotLightInfo->innerAngle;
// 			pLight->param.Phi     = LightData.light[i].pSpotLightInfo->outerAngle;
// 			pLight->param.Falloff = LightData.light[i].pSpotLightInfo->fallOff;
//             
// 			memcpy(&(LightData.light[i].FrameCount), pOffset, sizeof(LightData.light[i].FrameCount));
// 			pOffset += sizeof(LightData.light[i].FrameCount);
// 			pLight->kfrCnt = LightData.light[i].FrameCount;
// 			if(LightData.light[i].FrameCount > 0)
// 			{
// 				pLight->pAnimSpotLight = new N3D_SPOTLIGHT[LightData.light[i].FrameCount];
// 				for(UINT i = 0; i<LightData.light[i].FrameCount;i++)
// 				{
// 					memcpy(pLight->pAnimSpotLight, pOffset, sizeof(*(LightData.light[i].pSpotLightInfo)));
// 					pOffset += sizeof(*(LightData.light[i].pSpotLightInfo));
//                     pLight->pAnimSpotLight[i].direction.z = -pLight->pAnimSpotLight[i].direction.z;
// 				}			    
// 			} 
// 			
// 			break;
// 		case NLT_DIRECTIONLIGHT:
// 			pLight->param.Type = D3DLIGHT_DIRECTIONAL; 
// 			LightData.light[i].pDirectionalLight =new N3D_DIRECTIONAL();
// 			memcpy(LightData.light[i].pDirectionalLight, pOffset, sizeof(N3D_DIRECTIONAL));
// 			pOffset += sizeof(N3D_DIRECTIONAL);
// 			//ǿ��
// 			pLight->multiply = LightData.light[i].pDirectionalLight->multiply;
// 			//��ɫ
// 			pLight->diffuse.r = ((LightData.light[i].pDirectionalLight->color >> 24) &0xff)/255.0f;
// 			pLight->diffuse.g = ((LightData.light[i].pDirectionalLight->color >> 16) &0xff)/255.0f;
// 			pLight->diffuse.b = ((LightData.light[i].pDirectionalLight->color >> 8) &0xff)/255.0f;
// 			pLight->ambient   = 0.0f * pLight->diffuse;
// 			pLight->param.Diffuse.r = pLight->diffuse.r * pLight->multiply;
// 			pLight->param.Diffuse.g = pLight->diffuse.g * pLight->multiply;
// 			pLight->param.Diffuse.b = pLight->diffuse.b * pLight->multiply;
// 			pLight->param.Diffuse.a = 1.0f;
// 			//�����⣬����������ģ��ȫ�ֹ�
// 			pLight->param.Ambient = 0.0f * pLight->param.Diffuse; // fuxb �Ѳ���0,05��Ϊ��0.0f
// 			pLight->param.Ambient.a = 1.0f;
// 			//�߹�
// 			pLight->param.Specular = pLight->diffuse;  // fuxb 2007-10-16 ��3ds max������ͬ�Ĵ���ʽ
//             //���ߵ������Զ����
// 			pLight->param.Range = LightData.light[i].pDirectionalLight->attenuationRange;
// 			pLight->param.Attenuation0 = LightData.light[i].pDirectionalLight->attenuation0;
// 			pLight->param.Attenuation1 = LightData.light[i].pDirectionalLight->attenuation1;
// 			pLight->param.Attenuation2 = LightData.light[i].pDirectionalLight->attenuation2;
// 			
// 			//����
// 			pLight->param.Direction.x = LightData.light[i].pDirectionalLight->direction.x;
// 			pLight->param.Direction.y = LightData.light[i].pDirectionalLight->direction.y;
// 			pLight->param.Direction.z = -LightData.light[i].pDirectionalLight->direction.z;
//             
// 			memcpy(&(LightData.light[i].FrameCount), pOffset, sizeof(LightData.light[i].FrameCount));
// 			pOffset += sizeof(LightData.light[i].FrameCount);
// 			pLight->kfrCnt = LightData.light[i].FrameCount;
// 			if(LightData.light[i].FrameCount > 0)
// 			{
// 				pLight->pAnimDirectionalLight = new N3D_DIRECTIONAL[LightData.light[i].FrameCount];
// 				for(UINT i = 0; i<LightData.light[i].FrameCount;i++)
// 				{
// 					memcpy(pLight->pAnimDirectionalLight, pOffset, sizeof(*(LightData.light[i].pDirectionalLight)));
// 					pOffset += sizeof(*(LightData.light[i].pDirectionalLight));
//                     pLight->pAnimDirectionalLight[i].direction.z = -pLight->pAnimDirectionalLight[i].direction.z;
// 				}
// 			} 			
// 			break;	
// 		}
// 		
// 		
// 		//�ؼ�֡������Ϣ
// 		pLight->playKfr   = FALSE;
// 		pLight->isPlaying = FALSE;
// 		pLight->kfrCnt = LightData.light[i].FrameCount;
// 		
// 		if (pLight->kfrCnt > m_pScene->m_KfrCnt)
// 		{
// 			m_pScene->m_KfrCnt = pLight->kfrCnt;
// 			m_pScene->m_EndFrame = pLight->kfrCnt;
// 		}
// 		pLight->index = i;
// 		m_pScene->m_pd3dDevice->SetLight(i, &pLight->param);
// 		m_pScene->m_pd3dDevice->LightEnable(i, TRUE);
// 		
// 		m_pScene->light.push_back(pLight);	
// 		m_pSceneResource->light.push_back(pLight);
// 	}
// 	SAFE_DELETE_ARRAY(LightData.light);
// }

//-------------------------------------------------------------------
//����: CreateBasicTextureUsedThisImage()
//����: _N3DZipImageData* pImageData
//��;: �����õ���ͼ������л�����ͼ����
//-------------------------------------------------------------------
// VOID CGetN3DFileInfo_5::CreateBasicTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData)
// {
// 	if (!pImageData)
// 	{
// 		return;
// 	}
// 
//    //������N3D�ļ��ĵ�֡��ͼ�б�
// 	CTexture** ppTex = m_pSceneResource->texture.begin();
// 	INT texCnt = m_pSceneResource->texture.size();
// 	
// 	//������ͼ�б������ͼ�õ���ͼ�����ø�ͼ�񴴽���ͼ
//     for(INT i =0; i< texCnt; i++)
// 	{
// 		CTexture* pTex = *(ppTex + i);
// 		//�Ѿ�����
// 		if (pTex->bCreated) continue;
// 
// 		//�����ͼʹ�õ�ͼ�����Ƿ����m_ImageIndex
// 		if (pTex->imageIdx == m_ImageIndex)
// 		{
// 			//��ͨ��ͼ
// 			if (pTex->type == BASIC_TYPE)
// 				m_pScene->CreateTexFromBuffer(pTex, pImageData->imageInfo.Size, pImageData->Data);
// 
// 			//͸����ͼ
// 			if(pTex->type == OPACITY_TYPE)
// 			{
// 				m_pScene->CreateTransparentTexture(pTex, pImageData->imageInfo.Size, pImageData->Data);
// 				pTex->bAlphaBlend = TRUE;
// 			}
// 
// 			//��͹��ͼ
// 			if (pTex->type == BUMP_TYPE)
// 			{
// 				//��ʼ����͹��ͼ
// 				CBumpMap* pBumpMap = m_pScene->bumpTexture[pTex->bumpIdx];
// 				m_pScene->CreateTexFromBuffer(pTex, pImageData->imageInfo.Size, pImageData->Data);
// 
// 				switch (pBumpMap->m_type)
// 				{
// 				case BUMPWAVEMAP: //����Ҫ��ͼ
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
// 			// fuxb 2009-03-18 ==> �����͸����ͼ������textureΪalphaBlend
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
// 			//�ж�ʹ�ø���ͼ�Ĳ����Ƿ���AlphaBlend--------------------------------------------------------
// 			if (pTex->bAlphaBlend)
// 			{
// 				UpdateAllMtrlAlphaBlendStatus((DWORD)pTex);
// 			}
// 		}
// 	}
// }
// 
// //-------------------------------------------------------------------
// //����: CreateDynamicTextureUsedThisImage()
// //����: _N3DZipImageData* pImageData
// //��;: �����õ���ͼ������ж�̬��ͼ����
// //-------------------------------------------------------------------
// VOID CGetN3DFileInfo_5::CreateDynamicTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData)
// {
// 	if (!pImageData)
// 	{
// 		return;
// 	}
// 
// 	//������N3D�ļ��Ķ�̬����֡����ͼ�б�
// 	CDynamicTex** ppDyTex = m_pSceneResource->dynamicTexture.begin();
// 	INT dyTexCnt = m_pSceneResource->dynamicTexture.size();
// 	
// 	//������ͼ�б������ͼ�õ���ͼ�����ø�ͼ�񴴽���ͼ
//     for(INT i =0; i< dyTexCnt; i++)
// 	{
// 		CDynamicTex* pDyTex = *(ppDyTex + i);
// 		//�Ѿ�����
// 		if (pDyTex->bCreated) continue;
// 			
// 		//�����ö�̬��ͼ�����ͼ��Դ��, �ж��Ƿ��õ��˵�ǰͼ��
// 		int imageCnt = pDyTex->imagesIdx.size();
// 		for (int j = 0; j< imageCnt; j++)
// 		{
// 			INT imageIndex = pDyTex->imagesIdx[j];
// 			if (imageIndex == m_ImageIndex)
// 			{
// 				//��ͼ�������ͼ����
// 				pDyTex->FillImageToTextureBuffer(j, pImageData->imageInfo.Size, pImageData->Data);
// 
// 				// fuxb 2009-03-18 ==> �����͸����ͼ������textureΪalphaBlend
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
// }
// 
// 
// //-------------------------------------------------------------------
// //����: CreateCubeTextureUsedThisImage()
// //����: _N3DZipImageData* pImageData
// //��;: �����õ���ͼ�������CubeMap��ͼ����
// //-------------------------------------------------------------------
// VOID CGetN3DFileInfo_5::CreateCubeTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData)
// {
// 	//������N3D�ļ���CubeMap���ั����ͼ�б�
// 	CCubeMap** ppCubeMap = m_pSceneResource->cubeMap.begin();
// 	INT cubeMapCnt = m_pSceneResource->cubeMap.size();
// 	
// 	//����CubeMap��ͼ�б������ͼ�õ���ͼ�����ø�ͼ�񴴽���ͼ
//     for(INT i =0; i< cubeMapCnt; i++)
// 	{
// 		CCubeMap* pCubeMap = *(ppCubeMap + i);
// 		//�Ѿ�����
// 		if (pCubeMap->bCreated) continue;
// 		
// 		//����CubeMap��6����ͼ
// 		for (INT faceIndex = 0; faceIndex< 6; faceIndex++)
// 		{
// 			if(m_ImageIndex == pCubeMap->imagesIdx[faceIndex])
// 			{
// 				//��ͼ�������ͼ����
// 				pCubeMap->FillImageToCubeTexBuffer(faceIndex, pImageData->imageInfo.Size, pImageData->Data);
// 
// 				// fuxb 2009-03-18 ==> �����͸����ͼ������textureΪalphaBlend
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
// }
// 
// //-------------------------------------------------------------------
// //����: UpdateAllMtrlAlphaBlendStatus()
// //����: CTexture* pTex
// //��;: �ж�ʹ�ø���ͼ�Ĳ����Ƿ���AlphaBlend
// //-------------------------------------------------------------------
// VOID CGetN3DFileInfo_5::UpdateAllMtrlAlphaBlendStatus(DWORD pTex)
// {
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
// 				// fuxb 2009-03-06 ==> alphablend��ģ�Ͳ��������� ==>
// 				//�������ʵ�subMesh�б�,���ʹ�õ��ò��ʵ�ģ���Ƿ���͸��ģ���б���
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
// }
// 
// //-------------------------------------------------------------------
// //����: ConstructTextureFromImage()
// //����: _N3DZipImageData* pImageData
// //��;: ������һ����ͼ�����ݺ󣬴����õ���ͼ���������ͼ����
// //-------------------------------------------------------------------
 BOOL CGetN3DFileInfo_5::ConstructTextureFromImage(N3D_5_1_ZipImageData* pImageData, INT imgIndex)
 { 
	 if (!pImageData)
	 {
		 return FALSE;
	 }
	m_pVGSCore->SunFillTexturesFromBuffer(pImageData->imageInfo.Size, (CHAR*)pImageData->Data, imgIndex);
 	return TRUE;
 }
