/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // SaveN3D.h
* ����ժҪ�� // ���N3D����ʵ��
* ����˵���� // 
* ��ǰ�汾�� // V2.0
* ��    �ߣ� // ���²�
* ������ڣ� // 
* 
* �޸�����      �汾��     �޸���	      �޸�����
* ---------------------------------------------------------------
* 
*
*********************************************************************/


#include "SaveN3D.h"

#include "..\\Controller\\Controller.h"
#include "..\\Controller\\CEditor.h"
#include "..\\Controller\\CFlyer.h"
#include "..\\Controller\\CWalker.h" 

//#include "..\\VGSCommonDef.h"
#include "..\\Root.h"

#include "..\\CSysFunc.h"

#include "..\\CModel.h"
#include "..\\vgsConstDef.h"
#include "..\\VGSSupport.h"
#include "..\\CSysFunc.h"

#include "..\\gui\\CNavigator.h"
#include "..\\gui\\CButton.h"
#include "..\\gui\\background.h"

using namespace N3D;

CSaveN3D::CSaveN3D()
{
	m_pVGSSceneMgr = NULL;
	m_imageCount= 0;

	m_PureOverlayCount = 0;
	m_BackdropCount    = 0;
}
CSaveN3D::~CSaveN3D()
{	
	m_pVGSSceneMgr = NULL;
}

VOID CSaveN3D::Release()
{
	m_pVGSSceneMgr = NULL;
}

HRESULT CSaveN3D::SaveSceneAsN3D(const CScene* pVGSSceneMgr, const CHAR* strFileName, BOOL zipStatus /*= true*/, DWORD fVersion/* = N3D_CURRENT_V3DVERSION*/)
{
	if (pVGSSceneMgr == NULL)
	{
		::MessageBox(NULL, TEXT("�Ҳ�������"), TEXT("����"), MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	m_pVGSSceneMgr = (CScene*)pVGSSceneMgr;

	// �����ļ�
	errno_t et = fopen_s(&m_pFile, strFileName, "wb");
	if (et != 0 || NULL == m_pFile)
	{
		::MessageBox(NULL, TEXT("�����ļ�ʧ��"), TEXT("����"), MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
    
	ZeroMemory(m_FileName, sizeof(m_FileName));
	strncpy_s(m_FileName, sizeof(m_FileName), strFileName, strlen(strFileName));

	//-------------------------------------------------------
	//  **** ע�⣺
	//  1. ���ĳ�����͵�����û�У���д��һ�����ֽڵ�0
	//  2. д���ļ�������˳���ܱ�
	//-------------------------------------------------------

	// ������ͼ�б�
	InitImageList();

	// ��ʼ���ļ�ͷ	
	ZeroMemory(&m_Header, sizeof(m_Header));
	m_Header.imageCnt = m_vImageList.size();
	if (m_Header.imageCnt > 0)
	{
		m_Header.pImageIndexData = new DWORD[m_Header.imageCnt];
		ZeroMemory(m_Header.pImageIndexData, m_Header.imageCnt*sizeof(DWORD));
	}

	//write the header data
	WriteHeader();

	// �������ݿ�ʼλ��
	m_Header.baseHeader.SceneDataPtr = ftell(m_pFile);   
 
	// �����������ݣ�ѹ��
	CreateSceneDataBuffer();   

	 // д��ͼ�����ݣ���ѹ��
	WriteImageData(); 

	// ����д���ļ�ͷ
    WriteHeader();  

	//close the pFile
	int re = fclose(m_pFile);

	// Release
	SAFE_DELETE_ARRAY(m_Header.pImageIndexData);
   
	return S_OK;
}
//������ʱ�ĳ���buffer�ļ�����player��Ҫ�ĳ�����Ϣ���浽��ʱ�ļ��У���ѹ���������ѹ��
bool CSaveN3D::CreateSceneDataBuffer()
{
    CHAR OriSceneFile[256];
	strncpy_s(OriSceneFile, sizeof(OriSceneFile), m_FileName, sizeof(m_FileName));
	ChangeFileEx(OriSceneFile, "SCE");

    // ����δѹ������ʱ�ļ������泡������
	errno_t et = fopen_s(&m_OriSceneBufferFile, OriSceneFile, "wb");
	if (et != 0 || NULL == m_OriSceneBufferFile)
	{
		::MessageBox(NULL, TEXT("���������ļ�ʧ��"), TEXT("����"), MB_OK | MB_ICONERROR);
		return false;
	}

	CHAR ImgBufFile[256];
	ZeroMemory(ImgBufFile, 256);
	strncpy_s(ImgBufFile, sizeof(ImgBufFile), m_FileName, sizeof(m_FileName));
	ChangeFileEx(ImgBufFile, "IMG");   // ����ͼ�����ݵ��ļ�����
	
	// дӳ���
	WriteFileTable();

	//write the window prop
	WriteWindowProp();
	
	//write privacy data
	WritePrivacyData();

	//write the  camera data
	WriteCameraData();

	//write the light data
	WriteLightData();

	// write texture
	//WriteTextureData();

	//write the Material data
	WriteMaterialData();

	//write the Model data
	// WriteMeshData();
	WriteModelData();

	//write the Text data
	WriteTextData();

	//write the overlay data
	WriteOverlayData();

	// д�뱳��ͼ
	//WriteBackGround();

	//write the navigater data
	WriteNavigaterData();

	//write the button data
	WriteButtonData();

	//write AudioData
	WriteAudioData();

	//write FlashData
	WriteFlashData();

	// ����
	//WriteAnimation();

	// ����
	WriteControlData();

	// ����ͼ, �����ؼ�֡����
	WriteSceneGraph();

	m_Header.baseHeader.OriginSceneDataLen = ftell(m_OriSceneBufferFile);  // ѹ��ǰ�������ݵ��ֽڳ���

	// ����д�볡�����ݣ�����ӳ���
	WriteFileTable();    
	
	int ret = fclose(m_OriSceneBufferFile);

	//ѹ����Ŀ���ļ�
	CHAR ZipSceneFile[256];
	ZeroMemory(ZipSceneFile, 256);
	strncpy_s(ZipSceneFile, sizeof(ZipSceneFile), OriSceneFile, sizeof(OriSceneFile));
	ChangeFileEx(ZipSceneFile, "ZSC");
	//
	HMODULE hModule = ::LoadLibrary(TEXT(".\\PackFileDLL.dll"));
	if (!hModule)
	{
		MessageBox(NULL, TEXT("û���ҵ���̬���ļ� PackFileDLL.dll "), TEXT("����"), 0);
		return false;
	}
	//
	typedef BOOL (*FuncPackFile)(const CHAR*, const CHAR*);
	FuncPackFile pFuncPackFile = (FuncPackFile)::GetProcAddress(hModule, "PackFile");
	if (pFuncPackFile && pFuncPackFile(OriSceneFile, ZipSceneFile) == FALSE) 
	{
		::MessageBox(NULL, TEXT("ѹ���������ݲ���ԭ��ʧ�ܡ����´������ԭ�������:\n   1. ���Լ�鳡�����Ƿ��п�ģ��?  \n   2. ����ļ��Ƿ񳬹�2GB?"), 
			TEXT("�������WebMax�ļ����Ϻ���ͼVGS"), MB_OK | MB_ICONERROR);
		return false;
	}

	//��ѹ����ĳ������ݶ�����
	FILE* pZipSceneFile = NULL;
	errno_t et1 = fopen_s(&pZipSceneFile, ZipSceneFile, "rb");
	if (et1 != 0 || pZipSceneFile == NULL)
	{
		::MessageBox(NULL, TEXT("д�����ݹ��̴���"), TEXT("����"), 0);
		return false;
	}
	fseek(pZipSceneFile, 0, SEEK_END);
	DWORD zippedSceneFileLen = ftell(pZipSceneFile);
	fseek(pZipSceneFile, 0, SEEK_SET);

	//����ѹ����ĳ������ݴ�С�Ļ���
	BYTE* destBuf = new BYTE[zippedSceneFileLen];
	fread(destBuf, zippedSceneFileLen, 1, pZipSceneFile);
	fclose(pZipSceneFile);

	//д��ѹ����ĳ���
	fwrite(destBuf, zippedSceneFileLen, 1, m_pFile); 
	m_Header.baseHeader.SceneZipDataLen = zippedSceneFileLen;   // ѹ���󳡾����ݵĴ�С

	//ɾ����ʱ�ļ�
	DeleteFile(OriSceneFile);
	DeleteFile(ZipSceneFile);
		
	return true;
}

VOID CSaveN3D::WriteHeader()
{
	if (!m_pFile)
	{
		throw exception("д���ļ�ͷ���������޷�д���ļ�");
	}
	fseek(m_pFile, 0, SEEK_SET);

	// д�����ļ�ͷ��Ϣ
	m_Header.baseHeader.Version = N3D_CURRENT_V3DVERSION;  
	fwrite(&m_Header.baseHeader, sizeof(m_Header.baseHeader), 1, m_pFile); 

	// д3.0��չ��Ϣ
	fwrite(&m_Header.imageCnt, sizeof(DWORD), 1, m_pFile); 
	fwrite(m_Header.pImageIndexData, m_Header.imageCnt * sizeof(DWORD), 1, m_pFile);
}

void CSaveN3D::WriteFileTable()
{	
	if (m_OriSceneBufferFile)
	{
		fseek(m_OriSceneBufferFile, 0, SEEK_SET);
		fwrite(&m_Table, sizeof(N3D_5_1_Table), 1, m_OriSceneBufferFile);
	}
}

// 
VOID CSaveN3D::WriteWindowProp()
{	
	int width, height;
	m_pVGSSceneMgr->GetGuiManagerRef().GetViewportSize(width, height);    // ��ά�ӿڵĴ�С
	if ((int)width == 0 || (int)height == 0)
	{
		m_SceneProp.Width  = 800;
		m_SceneProp.Height = 600;
	}
	else
	{
		m_SceneProp.Width = (int)width;
		m_SceneProp.Height = (int)height;
	}

	m_SceneProp.BackColor      = (DWORD)m_pVGSSceneMgr->m_backgroundColor;
	m_SceneProp.FPS            = gkfrPlaySpeed;
	m_SceneProp.AntiAliasLevel = 1;
	
	if (m_OriSceneBufferFile)
	{
		m_Table.SceneproperyPtr = ftell(m_OriSceneBufferFile); // ��¼WindowProp�������ļ��Ŀ�ʼλ��
		fwrite(&m_SceneProp, sizeof(m_SceneProp), 1, m_OriSceneBufferFile);
	}
}

//д��ģ����Ϣ
// VOID CSaveN3D::WriteMeshData()
VOID CSaveN3D::WriteModelData()
{
	m_vEntityList.clear();

	m_Table.meshDataPtr = ftell(m_OriSceneBufferFile); // ��¼mesh�������ļ��Ŀ�ʼλ��
	
	DWORD ModelCount = m_pVGSSceneMgr->GetModelCount();
	fwrite(&ModelCount, sizeof(ModelCount), 1, m_OriSceneBufferFile); // Model����Ŀ
	
	m_vEntityList.clear();   // ���entity�б�

	for (DWORD i = 0; i < ModelCount; ++i)
	{
		CModel *pModel = m_pVGSSceneMgr->GetModelByIndex(i);

		m_vEntityList.push_back((DWORD)pModel);  // ����д���entity�б�

		WriteAModel(pModel);
	}
}

VOID CSaveN3D::WriteAModel(CModel *pModel)
{
	if (pModel && m_OriSceneBufferFile)
	{
		// // Mesh��entity��ogre�е�������ͬ��entity���⻹��һ��vgs���֣��û������޸�VGS���֡�
		string sVGSName = pModel->getName();
		UINT len = sVGSName.length(); 
		WRITE_VALUE(len, m_OriSceneBufferFile);          // mesh name string length
		WRITE_PTR(sVGSName.c_str(), len, m_OriSceneBufferFile);            // mesh name

		DWORD fvf = pModel->m_FVF;
		WRITE_VALUE(fvf, m_OriSceneBufferFile);  // ģ�͵Ķ����ʽ

		//д�붥�����
		UINT vertexCount = pModel->m_vertexCount;  // mesh���õ��Ķ�����Ŀ
		WRITE_VALUE(vertexCount, m_OriSceneBufferFile);

		// mesh �Ķ�����Ϣ
		void *pVertextBuffer = NULL;
		pModel->LockVertexBuffer(&pVertextBuffer);	
		WRITE_PTR(pVertextBuffer, vertexCount * pModel->m_FVFSize, m_OriSceneBufferFile);
		pModel->UnlockVertexBuffer();

		// д�������
		UINT faceCount = pModel->m_faceCount;  // mesh���õ�������Ŀ
		WRITE_VALUE(faceCount, m_OriSceneBufferFile);

		// index buffer�ĳ���
		//int indexSize = pModel->m_faceCount * 3 * sizeof(unsigned short);
		//if (pModel->m_vertexCount > 65535)
		//{
		//	indexSize = pModel->m_faceCount * 3 * sizeof(UINT);
		//} 
		UINT indexSize = pModel->m_faceCount * 3 * sizeof(UINT); 

		// д��index buffer����
		void *pIndexBuffer = NULL; 
		pModel->LockIndexBuffer(&pIndexBuffer);
		WRITE_PTR(pIndexBuffer, indexSize, m_OriSceneBufferFile);
		pModel->UnlockIndexBuffer();

		// submesh ��Ŀ
		unsigned short SubMeshCount = pModel->getSubModelCount(); // ptrMesh->getNumSubMeshes();
		fwrite(&SubMeshCount, sizeof(SubMeshCount), 1, m_OriSceneBufferFile); // 
		
		// ��¼ÿ��submesh����Ϣ
		for (UINT iSubMesh = 0; iSubMesh < SubMeshCount; ++iSubMesh)
		{
			 // submeshʹ�õĲ�������
			int index = FindMaterial(DWORD(pModel->getSubModelMaterial(iSubMesh)));
			WRITE_VALUE(index, m_OriSceneBufferFile);

			// ÿ����ģ�͵Ķ������С���������
			CSubModel* pSubModel = pModel->getSubModel(iSubMesh);
			WRITE_VALUE(pSubModel->m_vertexStart, m_OriSceneBufferFile);
			WRITE_VALUE(pSubModel->m_vertexCount, m_OriSceneBufferFile);
			WRITE_VALUE(pSubModel->m_faceStart, m_OriSceneBufferFile);
			WRITE_VALUE(pSubModel->m_faceCount, m_OriSceneBufferFile);

		} // if

		// ģ�͵�������Ϣ
		bool bBillBoard = (pModel->isBillboard == TRUE);
        fwrite(&bBillBoard, sizeof(bBillBoard), 1, m_OriSceneBufferFile);
		if (bBillBoard)
		{
			BYTE axis ;//= (BYTE)pModel->billboardDirectionAxis;
			fwrite(&axis, sizeof(axis), 1, m_OriSceneBufferFile);  // ���������������
		}
		bool bCollision = (pModel->isCollision == TRUE);
        fwrite(&bCollision, sizeof(bCollision), 1, m_OriSceneBufferFile); 
		bool bGround = (pModel->isGround == TRUE);
        fwrite(&bGround, sizeof(bGround), 1, m_OriSceneBufferFile);
		bool bVisible = (pModel->visible == TRUE);
		fwrite(&bVisible, sizeof(bVisible), 1, m_OriSceneBufferFile);
		bool bClickable = (pModel->m_isClickable == TRUE);
		fwrite(&bClickable, sizeof(bClickable), 1, m_OriSceneBufferFile);
		BYTE upAxis = 0; //(BYTE)pModel->billboradUpAxis;
		fwrite(&upAxis, sizeof(upAxis), 1, m_OriSceneBufferFile);
		bool bIsAutoNormalise = (pModel->isAutoNormalise == TRUE);
		fwrite(&bIsAutoNormalise, sizeof(bIsAutoNormalise), 1, m_OriSceneBufferFile);
		bool bSkybox = (pModel->isSkyBox == TRUE);
		fwrite(&bSkybox, sizeof(bSkybox), 1, m_OriSceneBufferFile); 

		// ������Ϣ
		int bindingSkeletonIndex  = -1;
		fwrite(&bindingSkeletonIndex, sizeof(bindingSkeletonIndex), 1, m_OriSceneBufferFile); 


	} // for
}

//UINT CSaveN3D::GetVGSMtrlIndex(string materialName)
//{
//  UINT mtrlIndex =0;
//  MAP_Name2Name::iterator it = m_pVGSSceneMgr->m_MtrNameList.begin();
//	for (it; it != m_pVGSSceneMgr->m_MtrNameList.end(); ++it)
//	{
//
//		string mtrName = it->first;   // ���ʵ�VGS����
//		if(materialName == mtrName)
//			return mtrlIndex;
//		mtrlIndex++;
//	
//	}
//  return 0;
//}
//VOID CSaveN3D::WriteImageInfo(const UINT& imgIndex, /*const TextureUnitState* pTextUnit, const UINT& frame,*/ const char* texName)
//{
	//if (texName && !std::string(texName).empty())
	//{
	//	FILE* pImageFile = fopen(texName, "rb");

	//	fseek(pImageFile, 0, SEEK_END);
	//	DWORD fileSize = ftell(pImageFile);    // �õ��ļ���С

	//	BYTE* data = new BYTE[fileSize];
	//	fseek(pImageFile, 0, SEEK_SET);
	//	fread(date, fileSize, pIamgeFile);
	//	fclose(pImageFile);

	//	fwrite(&fileSize, sizeof(fileSize), 1, m_ImgBufm_ImagesBufferFileferFile);
	//	fwrite(&data, sizeof(fileSize), 1, m_ImagesBufferFile);

	//	delete[] data;
	//	data = NULL;

	//	// imgIndex++;
	//	m_imageCount = imgIndex + 1;
	//}
//}

// ��ȡtexture������
N3D_TextureType CSaveN3D::GetTextureLayerType(CTextureLayer *pState)
{
	VGS_TextureType type = (VGS_TextureType)pState->type;
	return (N3D_TextureType)type;
	//if (pState)
	//{
	//	if (pState->isBlank())
	//	{
	//		return TLT_INVALID;
	//	}
	//	int count = pState->getNumFrames(); // ͼƬ��Ŀ
	//	if (pState->isCubic())
	//	{
	//		return TLT_CUBEMAP;  // ������ͼ
	//	}
	//	if (VRT_INVALID != _GetTextureAsReflect(pState)) // �Ƿ�����������ͼ
	//	{
	//		return TLT_REFLECT;
	//	}
	//	if (GetTextureAsOpacity((DWORD)m_pVGSSceneMgr, (DWORD)pState))  // �Ƿ�����͸����ͼ
	//	{
	//		return TLT_OPACITY;
	//	}
	//	if (_IsBumpTextureLayer(pState))
	//	{
	//		return TLT_BUMP;  // bump��ͼ
	//	}
	//	if (count <= 1 && pState->getAnimationDuration() == 0)
	//	{
	//		return TLT_BASAL;  // ������ͼ
	//	}	

	//	return TLT_DYNAMIC; // ��̬��ͼ	
	//}
	//
	//return TLT_INVALID;	
}

VOID CSaveN3D::WriteMaterialData()
{
	if (!m_OriSceneBufferFile)
	{
		return;
	}

	// ��ȡ������ʹ�õ�������ͼ�б�, ������·��
	std::vector<std::string> vImageList;
	m_pVGSSceneMgr->getImageNameList(false, vImageList);

    //��ͼ�������������
	m_Table.mtrlDataPtr = ftell(m_OriSceneBufferFile); // ��¼material�������ļ��Ŀ�ʼλ��

	GetAllMaterials();  // ��ȡ�����õ��Ĳ�����Ŀ��������ά�������ά����

	UINT mtrCount    = m_vMaterialList.size();
	UINT mtrCountPos = ftell(m_OriSceneBufferFile);                  // ����material��Ŀ���ļ�λ�� 
	WRITE_VALUE(mtrCount, m_OriSceneBufferFile);// material����Ŀ

	UINT InvalidUnitCount = 0;   // ��Ч��Unit

	vector<DWORD>::iterator it = m_vMaterialList.begin();
	for (; it != m_vMaterialList.end(); ++it)
	{
		CMaterial *pMaterial = (CMaterial*)(*it);  
		if (!pMaterial)
		{
			--mtrCount;
			continue;
		}

		N3D_MtrlInfo MtrInfo;
		ZeroMemory(&MtrInfo, sizeof(MtrInfo));

		// ���ʵ�VGS����
		std::string sMatName = pMaterial->getName(); 
		WRITE_STR(sMatName, m_OriSceneBufferFile);

		MtrInfo.BasalInfo.bRecShadow = false;

		////// ���ʵĻ������� //////
		//ambient
		MtrInfo.BasalInfo.ambient = D3DCOLOR_ARGB(BYTE(pMaterial->d3dmtrl.Ambient.a * 255), BYTE(pMaterial->d3dmtrl.Ambient.r * 255), 
			                                      BYTE(pMaterial->d3dmtrl.Ambient.g * 255), BYTE(pMaterial->d3dmtrl.Ambient.b * 255));
		//diffuse
		MtrInfo.BasalInfo.diffuse = D3DCOLOR_ARGB(BYTE(pMaterial->d3dmtrl.Diffuse.a * 255), BYTE(pMaterial->d3dmtrl.Diffuse.r * 255), 
			                                      BYTE(pMaterial->d3dmtrl.Diffuse.g * 255), BYTE(pMaterial->d3dmtrl.Diffuse.b * 255));

		//opacity
		MtrInfo.BasalInfo.opacity = (float)(pMaterial->d3dmtrl.Diffuse.a * 100);

		MtrInfo.BasalInfo.specular = D3DCOLOR_ARGB(BYTE(pMaterial->d3dmtrl.Specular.a * 255), BYTE(pMaterial->d3dmtrl.Specular.r * 255), 
			                                       BYTE(pMaterial->d3dmtrl.Specular.g * 255), BYTE(pMaterial->d3dmtrl.Specular.b * 255));

		//specularLevel
		MtrInfo.BasalInfo.specularLevel = pMaterial->specularLevel;

		//shininess
		MtrInfo.BasalInfo.shininess  = (float)(pMaterial->d3dmtrl.Power);
		
		//selfIllumination		
		MtrInfo.BasalInfo.selfIllumination = (float)pMaterial->selfIllumination;
		MtrInfo.BasalInfo.shaderType = (int)D3DSHADE_GOURAUD;  // 0 : ShadeOptions::SO_FLAT, 1: ShadeOptions::SO_GOURAUD, 2: ShadeOptions::SO_PHONG

		MtrInfo.BasalInfo.bTwoSide   = (pMaterial->twoSide == TRUE); // CULL_NONEʱ��˫����Ⱦ

		MtrInfo.BasalInfo.alphaTest = bool(pMaterial->bAlphaTest == TRUE);
		MtrInfo.BasalInfo.alphaTestValue = pMaterial->AlpahaTestValue;
		MtrInfo.BasalInfo.fillMode = (int)(pMaterial->fillMode);		
		
		fwrite(&MtrInfo.BasalInfo, sizeof(MtrInfo.BasalInfo), 1, m_OriSceneBufferFile);  // д����ʵĻ�������

		// �����Ƿ�ʹ�ò��ʵ�diffuse��ɫ��
		bool bUseDiffuse = (pMaterial->m_bUseDiffuse == TRUE);
		fwrite(&bUseDiffuse, sizeof(bUseDiffuse), 1, m_OriSceneBufferFile);  // ���false����Ч�ڲ��ʵ�diffuseΪ��ɫ

		////// Ŀǰ���ʲ�֧�ֶ�����˲��ʵĹؼ�֡����Ϊ0 //////
		UINT kfrCnt = 0;
		fwrite(&kfrCnt, sizeof(kfrCnt), 1, m_OriSceneBufferFile);  // д����ʶ���

		////// textureList //////

		UINT UnitStateCount = pMaterial->getTexureLayerCount();
		UINT UnitStateCountPos = ftell(m_OriSceneBufferFile);                       // ����UnitStateCount���ļ�λ��
		fwrite(&UnitStateCount, sizeof(UnitStateCount), 1, m_OriSceneBufferFile);   // д��������ͼ����Ŀ

		// int actualUintCount = UnitStateCount; // ����д�뵽�ļ���texture��Ŀ
		for (UINT iUnit = 0; iUnit < UnitStateCount; ++iUnit)
		{
			CTextureLayer *pState = pMaterial->getTextureLayerByIndex(iUnit);

			N3D_TextureLayer TextureLayer;
			ZeroMemory(&TextureLayer, sizeof(TextureLayer));

			TextureLayer.type  = pState->type;            // �õ�texture������
			if (TextureLayer.type == TLT_INVALID)         // ������Ч��texture
			{
				++InvalidUnitCount;    // ����һ��UnitState
				continue;
			}			

			string sStateName = _GetPureFileNameStr(pState->m_sName);
			int len = sStateName.length();
			WRITE_VALUE(len, m_OriSceneBufferFile);
			WRITE_PTR(sStateName.c_str(), len, m_OriSceneBufferFile);
			// <==
                                  
			fwrite(&TextureLayer.type, sizeof(TextureLayer.type), 1, m_OriSceneBufferFile);			// д��texture������

			TextureLayer.UVIndex = pState->UVIndex;  // UV�������
			fwrite(&TextureLayer.UVIndex, sizeof(TextureLayer.UVIndex), 1, m_OriSceneBufferFile);   // д��texture��UV��������

			BYTE mipmap = 0;                                          // fuxb 2009-02-11 ��ҳ��������Ϊ�ܱ�֤���κ�������бȽϺõ�Ч��
			fwrite(&mipmap, sizeof(mipmap), 1, m_OriSceneBufferFile); // fuxb 2008-12-12 ����mipmap

			// д��texturelayer����
			WriteTexturealayerParams(pState, TextureLayer.type, m_OriSceneBufferFile);

			// д���ɫģʽ
			TextureLayer.Blenderinfo.AlphaBlendParam.BlenderOP_alpha = (pState->Blenderinfo.AlphaBlendParam >> 24) / 255.0f; // to range [0, 1]
			TextureLayer.Blenderinfo.AlphaBlendOption =GetN3DTexOP(pState->Blenderinfo.AlphaBlendOption);
			TextureLayer.Blenderinfo.ColorBlendParam.BlenderOP_alpha = (pState->Blenderinfo.ColorBlendParam >> 24) / 255.0f;
			TextureLayer.Blenderinfo.ColorBlendOption = GetN3DTexOP(pState->Blenderinfo.ColorBlendOption);
			fwrite(&TextureLayer.Blenderinfo, sizeof(TextureLayer.Blenderinfo), 1, m_OriSceneBufferFile);   // д��texture�Ļ�ɫ��Ϣ

			TextureLayer.ImageCount = pState->getFrameCount();
			if (TextureLayer.type == TLT_CUBEMAP && TextureLayer.ImageCount > 0)
			{
				TextureLayer.ImageCount = 6;
			}
			fwrite(&TextureLayer.ImageCount, sizeof(TextureLayer.ImageCount), 1, m_OriSceneBufferFile);		// д��texture����ͼ��Ŀ

			// ����ʹ�õ�ͼ���ļ�����
			if (TextureLayer.ImageCount > 0)
			{
				if (TextureLayer.ImageCount > 0)
				{
					TextureLayer.ImageIdxList = new int[TextureLayer.ImageCount];
					for (UINT iImage = 0; iImage < TextureLayer.ImageCount; ++iImage)
					{
						std::string sImage = pState->getFrameFileName(iImage);   // �õ�textureʹ�õĵ�iImage��ͼƬ
						sImage = _GetPureFileNameStr(sImage);
						TextureLayer.ImageIdxList[iImage] = _GetIndexInVector(vImageList, sImage);   // �ҵ�ͼƬ��Image�б��е�����
						fwrite(&(TextureLayer.ImageIdxList[iImage]), sizeof(int), 1, m_OriSceneBufferFile);  // д��ͼƬ������
					}
					delete [] TextureLayer.ImageIdxList;
				}
			}

		} // for iUnit

		// ��дUnitStateCount
		if (InvalidUnitCount > 0)
		{
			UnitStateCount -= InvalidUnitCount;                                // ʵ��д���ļ���UnitState����Ŀ
			UINT CurrentPos = ftell(m_OriSceneBufferFile);
			fseek(m_OriSceneBufferFile, UnitStateCountPos, SEEK_SET);           // ���ļ�дUnitStateCount��λ��
			fwrite(&UnitStateCount, sizeof(UnitStateCount), 1, m_OriSceneBufferFile);
			fseek(m_OriSceneBufferFile, CurrentPos, SEEK_SET);					// �ص��ļ�ĩβ

			InvalidUnitCount = 0;  // ����Ϊ0
		}

	} // for ���в���

	// ��д������Ŀ
	if (m_vMaterialList.size() > 0)
	{
		UINT currentPos = ftell(m_OriSceneBufferFile);						// ��ǰ���ļ�λ��
		fseek(m_OriSceneBufferFile, mtrCountPos, SEEK_SET);					// �ص�дmtrCount���ļ�λ��
		fwrite(&mtrCount, sizeof(mtrCount), 1, m_OriSceneBufferFile);		// material����Ŀ
		fseek(m_OriSceneBufferFile, currentPos, SEEK_SET);					// �ص��ļ�ĩβ
	}	
}

//  ��ȡ�����õ��Ĳ�����Ŀ��������ά�������ά����
void CSaveN3D::GetAllMaterials()
{
	m_vMaterialList.clear();   // ��ղ����б�

	UINT count = m_pRoot->GetMaterialManager().getResourceCount();
	for (UINT i = 0; i < count; ++i)
	{
		CMaterial *pMaterial = (CMaterial*)m_pRoot->GetMaterialManager().getResource(i);
		if (pMaterial)
		{
			m_vMaterialList.push_back((DWORD)pMaterial);
		}
	}


// 	m_vMaterialList.clear();   // ��ղ����б�
// 	
// 	// return;
// 
// 	set<std::string> vMaterialNameSet;  // �������Ƽ���
// 
// 	// ��ά����
// 	MAP_Name2Name::iterator it = m_pVGSSceneMgr->m_MtrNameList.begin();
// 	for (it; it != m_pVGSSceneMgr->m_MtrNameList.end(); ++it)
// 	{
// 		vMaterialNameSet.insert(it->second); // ������ʵ�Ogre����
// 	}
// 
// 	// Overlay
// 	UINT overlayCount = GUI_GetObjectsCount(VGSGUITYPE::VGT_OVERLAY);
// 	if (overlayCount > 0)
// 	{
// 		DWORD *pOverlayHandle = new DWORD[overlayCount];
// 		GUI_GetObjectsList(VGSGUITYPE::VGT_OVERLAY, pOverlayHandle);
// 		for (UINT i = 0; i < overlayCount; ++i)
// 		{
// 			char sMatName[MAX_GUI_NAMESTR_LENGTH];
// 			if (true == GUI_GetOverlayMaterial(pOverlayHandle[i], sMatName))
// 			{
// 				if (!std::string(sMatName).empty())
// 				{
// 					vMaterialNameSet.insert(std::string(sMatName));
// 				}
// 			}
// 		} // for
// 		delete [] pOverlayHandle;
// 		pOverlayHandle = NULL;
// 	}
// 
// 	// button
// 	UINT btnCount = GUI_GetObjectsCount(VGSGUITYPE::VGT_BUTTON);
// 	if (btnCount > 0)
// 	{
// 		DWORD *pBtnHandle = new DWORD[btnCount];
// 		GUI_GetObjectsList(VGSGUITYPE::VGT_BUTTON, pBtnHandle);
// 		for (UINT i = 0; i < btnCount; ++i)
// 		{
// 			char sMatUpName[MAX_GUI_NAMESTR_LENGTH];
// 			char sMatDownName[MAX_GUI_NAMESTR_LENGTH];
// 			char sMatOverName[MAX_GUI_NAMESTR_LENGTH];
// 			if (true == GUI_GetButtonMaterial(pBtnHandle[i], sMatUpName, sMatDownName, sMatOverName))
// 			{
// 				if (!std::string(sMatUpName).empty())
// 				{
// 					vMaterialNameSet.insert(std::string(sMatUpName));
// 				}
// 				if (!std::string(sMatDownName).empty())
// 				{
// 					vMaterialNameSet.insert(std::string(sMatDownName));
// 				}
// 				if (!std::string(sMatOverName).empty())
// 				{
// 					vMaterialNameSet.insert(std::string(sMatOverName));
// 				} 
// 			}
// 		} // for
// 		delete [] pBtnHandle;
// 		pBtnHandle = NULL;
// 	}
// 
// 	// navigator
// 	UINT naviCount = GUI_GetObjectsCount(VGSGUITYPE::VGT_NAVIGATION);
// 	if (naviCount)
// 	{
// 		DWORD *pNaviHandle = new DWORD[naviCount];
// 		GUI_GetObjectsList(VGSGUITYPE::VGT_NAVIGATION, pNaviHandle);
// 		for (UINT i = 0; i < naviCount; ++i)
// 		{
// 			char sMatName[MAX_GUI_NAMESTR_LENGTH];
// 			if (true == GUI_GetNavigaterMaterial(pNaviHandle[i], sMatName))
// 			{
// 				if (!std::string(sMatName).empty())
// 				{
// 					vMaterialNameSet.insert(std::string(sMatName));
// 				}
// 			}
// 
// 			// ������
// 			UINT pointCount = GUI_GetNavPointerCount(pNaviHandle[i]);
// 			for (UINT iPoint = 0; iPoint < pointCount; ++iPoint)
// 			{
// 				char sName[MAX_GUI_NAMESTR_LENGTH];
// 				char sMatName[MAX_GUI_NAMESTR_LENGTH];
// 				GUI_GetNavPointerNameByIndex(pNaviHandle[i], iPoint, sName);
// 				if (true == GUI_GetPointerMaterail(pNaviHandle[i], sName, sMatName))
// 				{
// 					if (!std::string(sMatName).empty())
// 					{
// 						vMaterialNameSet.insert(std::string(sMatName));
// 					}
// 				}
// 			}
// 
// 		} // for
// 		delete [] pNaviHandle;
// 		pNaviHandle = NULL;
// 	}
// 
// 	// �������в��ʵ�ָ��
// 	std::set<std::string>::iterator iter = vMaterialNameSet.begin();
// 	for ( ; iter != vMaterialNameSet.end(); ++iter)
// 	{
// 		MaterialPtr mtrptr = MaterialManager::getSingleton().getByName(*iter);
// 
// 		if (mtrptr.get())
// 		{
// 			m_vMaterialList.push_back((DWORD)(mtrptr.get()));  // �������ָ��
// 		}
// 	}
		
}

// д��texturelayer�Ĳ���
void CSaveN3D::WriteTexturealayerParams(CTextureLayer *pState, N3D_TextureType type, FILE *pFile)
{
 	if (!pState || !pFile)
 	{
 		return;
 	}
 	switch (type)
 	{
 	case TLT_BASAL:					// ������ͼ
 		{
			INT width  = 256;
			INT height = 256;
 			if (pState->pTexture)
 			{
 				width  = pState->pTexture->width;
 				height = pState->pTexture->height;
 			}
			float uSpeed = pState->USpeed;
			float vSpeed = pState->VSpeed;
			WRITE_VALUE(width, pFile);
			WRITE_VALUE(height, pFile);
			WRITE_VALUE(uSpeed, pFile);
			WRITE_VALUE(vSpeed, pFile);
 			break;
 		}
 	case TLT_BUMPMAP:
 		{		
 			// ���ݲ�ͬ����д�벻ͬ�Ĳ���

 			// ����bump������
			CBumpMap* pBump = (CBumpMap*)pState->pTexture;
 			INT Type = pBump->m_type;
 			fwrite(&Type, sizeof(Type), 1, pFile);
 
 			INT width  = 0;
			INT height = 0;
			if (pState->pTexture)
			{
				width  = pBump->width;
				height = pBump->height;
			}
 			fwrite(&width, sizeof(width), 1, pFile);
 			fwrite(&height, sizeof(height), 1, pFile);

 			if (Type == VBT_BUMPWATER)
 			{
				WRITE_VALUE(pBump->m_xSize, pFile);
				WRITE_VALUE(pBump->m_ySize, pFile);
				WRITE_VALUE(pBump->m_radius, pFile);
				WRITE_VALUE(pBump->m_speed, pFile);
				WRITE_VALUE(pBump->m_CenterX, pFile);
				WRITE_VALUE(pBump->m_CenterY, pFile);
 			}
 			else if (Type == VBT_BUMPENVIRONMENT)
			{
				WRITE_VALUE(pBump->m_Noise1, pFile);
				WRITE_VALUE(pBump->m_Noise2, pFile);
 			}
 			else if (Type == VBT_BUMPNORMARLMAP)
 			{
 				INT index = (INT)_GetIndexInVector(m_vpLight, pBump->m_Light);
 				fwrite(&index, sizeof(index), 1, pFile);
 			}

			float USpeed = pState->USpeed;
			float VSpeed = pState->VSpeed;
 			fwrite(&USpeed, sizeof(USpeed), 1, pFile);
 			fwrite(&VSpeed, sizeof(VSpeed), 1, pFile);
 			
 			break;
 		}
 	case TLT_OPACITY:		            // ͸����ͼ
 		{
			INT width  = 256;
			INT height = 256;
			if (pState->pTexture)
			{
				width  = pState->pTexture->width;
				height = pState->pTexture->height;
			}
			float uSpeed = pState->USpeed;
			float vSpeed = pState->VSpeed;
			WRITE_VALUE(width, pFile);
			WRITE_VALUE(height, pFile);
			WRITE_VALUE(uSpeed, pFile);
			WRITE_VALUE(vSpeed, pFile);
 			break;
 		}
 	case TLT_REFLECT:                 // ������ͼ
 		{
			INT width  = 256;
			INT height = 256;
			if (pState->pTexture)
			{
				width  = pState->pTexture->width;
				height = pState->pTexture->height;
			}
			WRITE_VALUE(width, pFile);
			WRITE_VALUE(height, pFile);			

 			char reflectionType = (char)pState->m_refletionType;  //  д�뷴������
			WRITE_VALUE(reflectionType, pFile);

			float uSpeed = pState->USpeed;
			float vSpeed = pState->VSpeed;
			WRITE_VALUE(uSpeed, pFile);
			WRITE_VALUE(vSpeed, pFile);
 			break;
 		}
 	case TLT_RTFMAP: // ʵʱ����
	{
		CRTFMap *pRTPMap = (CRTFMap*)pState->pTexture;

		INT width  = 256;
		INT height = 256;
		if (pState->pTexture)
		{
			width  = pState->pTexture->width;
			height = pState->pTexture->height;
		}
		WRITE_VALUE(width, pFile);
		WRITE_VALUE(height, pFile);
 
 		// ����ģ�͵�����
 		int modelIndex = (INT)_GetIndexInVector(m_vEntityList, (DWORD)pRTPMap->m_pRefPlaneModel);
 		fwrite(&modelIndex, sizeof(modelIndex), 1, m_pFile);
 
 		// ������ģ�͵���Ŀ�������б�
 		DWORD refModelCount = pRTPMap->m_refModelList.size();
 		fwrite(&refModelCount, sizeof(refModelCount), 1, m_pFile);
 		for (UINT i = 0; i < refModelCount; ++i)
 		{
 			int Index = (INT)_GetIndexInVector(m_vEntityList, (DWORD)pRTPMap->m_refModelList[i]);
 			fwrite(&Index, sizeof(Index), 1, m_pFile);
 		}
 		break;
 	}
 	case TLT_RTTMAP:  // ʵʱ����
 	{
		INT width  = 256;
		INT height = 256;
		if (pState->pTexture)
		{
			width  = pState->pTexture->width;
			height = pState->pTexture->height;
		}
		WRITE_VALUE(width, pFile);
		WRITE_VALUE(height, pFile);
 
 		// ����ģ�͵�����
		int modelIndex = -1;
		WRITE_VALUE(modelIndex, m_pFile);
 
 		// ������ģ�͵���Ŀ�������б�
 		DWORD refModelCount = 0;
		WRITE_VALUE(refModelCount, m_pFile);
 		for (UINT i = 0; i < refModelCount; ++i)
 		{
			int Index = -1;
			WRITE_VALUE(Index, m_pFile);
 		}
 		break;
 	}
 	case TLT_CUBEMAP:                 // ����ͼ
		{
			INT width  = 256;
			INT height = 256;
			if (pState->pTexture)
			{
				width  = pState->pTexture->width;
				height = pState->pTexture->height;
			}
			WRITE_VALUE(width, pFile);
			WRITE_VALUE(height, pFile);

			float uSpeed = pState->USpeed;
			float vSpeed = pState->VSpeed;
			WRITE_VALUE(uSpeed, pFile);
			WRITE_VALUE(vSpeed, pFile);
			
			CCubeMap *pCubeMap = (CCubeMap*)pState->pTexture;
 			BYTE mode = (BYTE)pCubeMap->m_Mode;  // 0��̬ 1ʵʱ
			WRITE_VALUE(mode, pFile);

 			break;
 		}
 	case TLT_DYNAMIC:               // ��̬��ͼ
 		{
			INT width  = 256;
			INT height = 256;
			if (pState->pTexture)
			{
				width  = pState->pTexture->width;
				height = pState->pTexture->height;
			}
			WRITE_VALUE(width, pFile);
			WRITE_VALUE(height, pFile);

			CDynamicTex *pTex = (CDynamicTex*)pState->pTexture;
			float duration = pTex->GetDuration();
			WRITE_VALUE(duration, pFile);

			float uSpeed = pState->USpeed;
			float vSpeed = pState->VSpeed;
			WRITE_VALUE(uSpeed, pFile);
			WRITE_VALUE(vSpeed, pFile);
 			break;
 		}
 	case TLT_MOVIE:                    // ��Ӱ��ͼ
 		{
			INT width  = 256;
			INT height = 256;
			if (pState->pTexture)
			{
				width  = pState->pTexture->width;
				height = pState->pTexture->height;
			}
			WRITE_VALUE(width, pFile);
			WRITE_VALUE(height, pFile);

			float uSpeed = pState->USpeed;
			float vSpeed = pState->VSpeed;
			WRITE_VALUE(uSpeed, pFile);
			WRITE_VALUE(vSpeed, pFile);
 			break;
 		}
 	case TLT_FLASH:					// Flash��ͼ
 		{
			INT width  = 256;
			INT height = 256;
			if (pState->pTexture)
			{
				width  = pState->pTexture->width;
				height = pState->pTexture->height;
			}
			WRITE_VALUE(width, pFile);
			WRITE_VALUE(height, pFile);

			float uSpeed = pState->USpeed;
			float vSpeed = pState->VSpeed;
			WRITE_VALUE(uSpeed, pFile);
			WRITE_VALUE(vSpeed, pFile);
 			break;
 		}
 	default:
 		{
 			break;
 		}
 	} // switch
}

// ��ȡtexture������
//TextureLayerType CSaveN3D::GetTextureLayerType(int mapchanel)
//{
//	switch (mapchanel)
//	{
//	case TEXTURECOORD_DIFFUSE:       /*= 0*/
//	case TEXTURECOORD_LIGHTMAP:      /*= 1 */
//	case TEXTURECOORD_REFLECT:        /*= 2 */  
//	case TEXTURECOORD_OPACITY:        /*= 3 */ 
//	case TEXTURECOORD_SHADOW:         /*= 4*/
//	case TEXTURECOORD_BUMP:           /*= 5*/
//	case TEXTURECOORD_WATER:          /*= 6*/   
//		{
//			return TLT_Basal;
//		}
//	case TEXTURECOORD_CUBE:           /*= 7 */ 
//		{			
//			return TLT_CUBEMAP;
//		}
//	case TEXTURECOORD_DYNAMIC:        /*= 8*/
//		{			
//			return TLT_DYNAMIC;
//		}
//	case TEXTURECOORD_MOVIE:          /*= 9*/
//		{			
//			return TLT_MOVIE;
//		}
//	case TEXTURECOORD_FLASH:          /*= 10*/
//		{			
//			return TLT_FLASH;
//		}
//	}
//}

// ��ȡtexture���͵�����
//std::string CSaveN3D::GetTextureLayerTypeDes(N3D_TextureType type)
//{
//	switch (type)
//	{
//	case TLT_BASAL:
//		{
//			return std::string("_basal");
//		}
//	case TLT_OPACITY:
//		{
//			return std::string("_opacity");
//		}
//	case TLT_REFLECT:
//		{
//			return std::string("_reflect");
//		}
//	case TLT_CUBEMAP:
//		{
//			return std::string("_cubemap");
//		}
//	case TLT_DYNAMIC:
//		{
//			return std::string("_dynamic");
//		}
//	case TLT_MOVIE:
//		{
//			return std::string("_movie");
//		}
//	case TLT_FLASH:
//		{
//			return std::string("_flash");
//		}
//	case TLT_BUMPMAP:
//		{
//			return std::string("_bump");
//		}
//	default:
//		{
//			return "";
//		}
//	}
//}

// ������ͼ
//VOID CSaveN3D::WriteTextureData()
//{	
//	if (!m_pVGSSceneMgr)
//	{
//		throw Ogre::Exception::exception("��ȡ��������");
//		//return;
//	}
//	if (!m_OriSceneBufferFile)
//	{
//		throw Ogre::Exception::exception("��ȡ�ļ�����");
//		//return;
//	}
//
//	m_Table.texturePtr = ftell(m_OriSceneBufferFile); // ��¼texture�������ļ��Ŀ�ʼλ��
//
//	UINT count = m_pVGSSceneMgr->m_vTextureMtrlDev.size();
//	fwrite(&count, sizeof(count), 1, m_OriSceneBufferFile); // д��texture�ĸ���
//
//	//// д������texture�����Ƴ��Ⱥ�����(������·��)
//	for (UINT i = 0; i < count; ++i)
//	{
//		N3D_TextureLayer TextureInfo;
//		ZeroMemory(&TextureInfo, sizeof(TextureInfo));
//
//		std::set<TextureLayerType> LayerSet;
//		std::vector<VGS_MtrlUseTex> vMtrlTex = m_pVGSSceneMgr->m_vTextureMtrlDev[i].usedMtrlList;
//		std::vector<VGS_MtrlUseTex>::iterator it = vMtrlTex.begin();
//		for ( ; it != MtrlTex.end(); ++it)
//		{
//			LayerSet.insert(GetTextureLayerType((*it).mapChannel));
//		}
//
//		std::set<TextureLayerType>::iterator itLayerSet = LayerSet.begin();
//		for ( ; itLayerSet != LayerSet.end(); ++it)
//		{
//			std::string sName = m_pVGSSceneMgr->m_vTextureMtrlDev[i].fileName + GetTextureLayerTypeDes(*itLayerSet);
//			strcpy(&TextureInfo.Name, sName.c_str());
//
//			TextureInfo.type = *itLayerSet;
//			// TextureInfo.UVIndex = 
//			// TextureInfo.Blenderinfo
//			//TextureInfo.ImageCount = GetTextureImageCount(*itLayerSet);
//		}
//	}
//
//}

// ����
VOID CSaveN3D::WriteTextData()
{
// 	if (!m_pVGSSceneMgr)
// 	{
// 		throw Ogre::Exception::exception("��ȡ��������");
// 		//return;
// 	}
// 	if (!m_OriSceneBufferFile)
// 	{
// 		throw Ogre::Exception::exception("��ȡ�ļ�����");
// 		//return;
// 	}
// 
 	m_Table.TextDataPtr = ftell(m_OriSceneBufferFile); // ��¼camera�������ļ��Ŀ�ʼλ��
 
 	UINT i = 0;
 	fwrite(&i, sizeof(UINT), 1, m_OriSceneBufferFile); // û�����ݣ�д��0

}
/*
VOID CSaveV3D::WriteCameraData()
{
	if (!m_pVGSSceneMgr)
	{
		throw Ogre::Exception::exception("��ȡ��������");
		// return;
	}
	if (!m_pFile)
	{
		throw Ogre::Exception::exception("��ȡ�ļ�����");
		// return;
	}

	m_Table.cameraDataPtr = ftell(m_pFile); // ��¼camera�������ļ��Ŀ�ʼλ��


	UINT CamCount = m_pVGSSceneMgr->m_CamNameList.size();
	fwrite(&CamCount, sizeof(CamCount), 1, m_pFile);  //�������Ŀ

	MAP_Name2Name::iterator it = m_pVGSSceneMgr->m_CamNameList.begin();
	for (it; it != m_pVGSSceneMgr->m_CamNameList.end(); ++it)
	{
		Camera *pCamera = m_pVGSSceneMgr->mSceneMgr->getCamera(it->second);
		string sCamName   = m_pVGSSceneMgr->GetVGSCamName(pCamera->getName());
		UINT  NameLen     = strlen(sCamName.c_str());

		V3D_CameraDetail CamDetail;
		CamDetail.PrjType   = pCamera->getProjectionType();


		CamDetail.FromX     = pCamera->getPosition().x;  //����λ��
		CamDetail.FromY     = pCamera->getPosition().y;			
		CamDetail.FromZ     = pCamera->getPosition().z;
		CamDetail.OrientW   = pCamera->getOrientation().w;
		CamDetail.OrientX   = pCamera->getOrientation().x;
		CamDetail.OrientY   = pCamera->getOrientation().y;
		CamDetail.OrientZ   = pCamera->getOrientation().z;
		CamDetail.DirectionX = pCamera->getDirection().x;
		CamDetail.DirectionY = pCamera->getDirection().y;
		CamDetail.DirectionZ = pCamera->getDirection().z;

		CamDetail.PolygonMode  = pCamera->getPolygonMode();
		CamDetail.fFOVY       = (float)(pCamera->getFOVy().valueRadians());
		CamDetail.nearClip    = (float)(pCamera->getNearClipDistance());
		CamDetail.farClip     = (float)(pCamera->getFarClipDistance()); 

		fwrite(&NameLen, sizeof(NameLen), 1, m_pFile);      // д��cam���ֳ��� 
		fwrite(sCamName.c_str(), NameLen, 1, m_pFile);      // д��cam����
		fwrite(&CamDetail, sizeof(CamDetail), 1, m_pFile);  // д��cam����
	}
}*/

// ���˵�Ԥ�����������
void CSaveN3D::FilterCamera()
{
 	m_vpCamera.clear();
 
 	if (m_pVGSSceneMgr)
 	{
 		UINT count = m_pVGSSceneMgr->getCameraCount();
 		for (UINT i = 0; i < count; ++i)
 		{
 			CCamera *pCamera = m_pVGSSceneMgr->GetCameraByIndex(i);
 			if (pCamera && !IsPredefinedOrthCamera(pCamera))
 			{
 				m_vpCamera.push_back(pCamera);
 			}
 		}
 	}
}

// ��ȡ�������
int CSaveN3D::GetCameraIndex(CCamera *pCamera)
{
 	int count = m_vpCamera.size();
 	for (int i = 0; i < count; ++i)
 	{
 		if (m_vpCamera[i] == pCamera)
 		{
 			return i;
 		}
 	}

	return -1;
}

// д���������
VOID CSaveN3D::WriteCameraData()
{
 	if (!m_pVGSSceneMgr)
 	{
 		throw exception("��ȡ��������");
		return;
 	}
 	if (!m_OriSceneBufferFile)
 	{
 		throw exception("��ȡ�ļ�����");
 	}
 
 	// ��¼camera�������ļ��Ŀ�ʼλ��
 	m_Table.cameraDataPtr = ftell(m_OriSceneBufferFile); 
 
 	FilterCamera();  // ���˵�Ԥ�����������
 
 	//д���������
 	DWORD CamCount = m_vpCamera.size();
 	fwrite(&CamCount, sizeof(CamCount), 1, m_OriSceneBufferFile);
 
 	//������д������
 	for (DWORD i = 0; i < CamCount; ++i)
 	{
 		CCamera *pCamera = m_vpCamera[i];
 
 		N3D_Camera N3DCam;
		std::string sName = pCamera->getName();
		WRITE_STR(sName, m_OriSceneBufferFile);

 		// д�����������Ϣ 		
 		N3DCam.isOrtho              = (pCamera->m_camType == VCT_ORTHO);  //�����ͶӰ���� 		
 		N3DCam.BaseInfo.fov         = pCamera->GetFov();
		N3DCam.BaseInfo.nearClipDis = pCamera->getNearClip();
		N3DCam.BaseInfo.farClipDis  = pCamera->getFarClip(); 
		WRITE_VALUE(N3DCam.isOrtho, m_OriSceneBufferFile); 	
		WRITE_VALUE(N3DCam.BaseInfo.fov, m_OriSceneBufferFile); 	
		WRITE_VALUE(N3DCam.BaseInfo.nearClipDis, m_OriSceneBufferFile); 	
		WRITE_VALUE(N3DCam.BaseInfo.farClipDis, m_OriSceneBufferFile);

		D3DXVECTOR3 v3Pos = pCamera->GetPosition(VTS_PARENT);
		D3DXVECTOR3 v3At  = pCamera->GetLookat(VTS_PARENT);
		D3DXVECTOR3 v3Up  = pCamera->GetUp(VTS_PARENT);
		// float  fRoll = pCamera->GetRoll();
		WRITE_VECTOR3(v3Pos, m_OriSceneBufferFile);
		WRITE_VECTOR3(v3At, m_OriSceneBufferFile);
		WRITE_VECTOR3(v3Up, m_OriSceneBufferFile); 
		// WRITE_VALUE(fRoll, m_OriSceneBufferFile);
 	}
}


//ͨ������������������б��л�������������
//CameraParamAnimation* CSaveV3D::GetCameraParamAnimation(std::vector<CameraParamAnimation*> list, CCamera* pCam)
//{
//	std::vector<CameraParamAnimation*>::iterator it = list.begin();
//	int listCnt = list.size();
//	for (int i = 0; i< listCnt; i++)
//	{
//		CameraParamAnimation* pAnim = *(it + i);
//		if (pAnim->mCamera == pCam)
//			return pAnim;
//	}
//	return NULL;
//}

//д��ƹ�����
/*VOID CSaveV3D::WriteLightData()
{
	if (!m_pVGSSceneMgr)
	{
		throw Ogre::Exception::exception("��ȡ��������");
		// return;
	}
	if (!m_pFile)
	{
		throw Ogre::Exception::exception("��ȡ�ļ�����");
		// return;
	}

	m_Table.lightDataPtr = ftell(m_pFile); // ��¼light�������ļ��Ŀ�ʼλ��


	UINT LightCount = m_pVGSSceneMgr->m_LightNameList.size();
	fwrite(&LightCount, sizeof(LightCount), 1, m_pFile); // �ƹ���Ŀ
	
	MAP_Name2Name::iterator it = m_pVGSSceneMgr->m_LightNameList.begin();
	for (it; it != m_pVGSSceneMgr->m_LightNameList.end(); ++it)
	{
		string sName   = it->first;   // �ƹ��VGS����
		UINT NameLen = strlen(sName.c_str());

		Light *pLight  = m_pVGSSceneMgr->mSceneMgr->getLight(it->second);

	//LightIter = m_pVGSSceneMgr->mSceneMgr->getMovableObjectIterator(LightFactory::FACTORY_TYPE_NAME);
	//while (LightIter.hasMoreElements())
	//{
	//	string sName   = m_pVGSSceneMgr->GetVGSLightName(LightIter.peekNextKey());
	//	UINT NameLen = strlen(sName.c_str());

	//	Light *pLight  = dynamic_cast<Light*>(LightIter.getNext());

		V3D_LightDetail LightDetail;
		LightDetail.type      = pLight->getType();
		LightDetail.Dif_RGBA = (DWORD)(pLight->getDiffuseColour().getAsRGBA());
		LightDetail.Spe_RGBA = (DWORD)(pLight->getSpecularColour().getAsRGBA());
		LightDetail.fAttenuationRange   = pLight->getAttenuationRange();
		LightDetail.fAttenuationConst   = pLight->getAttenuationConstant();
		LightDetail.fAttenuationLinear  = pLight->getAttenuationLinear();
		LightDetail.fAttenuationQuadric = pLight->getAttenuationQuadric();
		LightDetail.PositionX  = pLight->getPosition().x;
		LightDetail.PositionY  = pLight->getPosition().y;
		LightDetail.PositionZ  = pLight->getPosition().z;
		LightDetail.DirectionX = pLight->getDirection().x;
		LightDetail.DirectionY = pLight->getDirection().y;
		LightDetail.DirectionZ = pLight->getDirection().z;
		
		fwrite(&NameLen, sizeof(NameLen), 1, m_pFile); 
		fwrite(sName.c_str(), NameLen, 1, m_pFile);            // д��ƹ���
		fwrite(&LightDetail, sizeof(LightDetail), 1, m_pFile); // д��ƹ������Ϣ

		// ����Ǿ۹�ȣ���Ҫ����۹�Ƶ��ڡ���׶�εĽǶ�,falloff
		if (pLight->getType() == CLight::LT_SPOTLIGHT)  
		{
			float innerAngle = (float)(pLight->getSpotlightInnerAngle().valueRadians());
			float outerAngle = (float)(pLight->getSpotlightOuterAngle().valueRadians());
			float falloff = (float)(pLight->getSpotlightFalloff());
			fwrite(&innerAngle, sizeof(innerAngle), 1, m_pFile);
			fwrite(&outerAngle, sizeof(outerAngle), 1, m_pFile);
			fwrite(&falloff, sizeof(falloff), 1, m_pFile);
		}
		else if (pLight->getType() == CLight::LT_DIRECTIONAL)  // ����Ƿ����
		{
			// NULL
		}

	}

}*/
VOID CSaveN3D::WriteLightData()
{
 	if (!m_pVGSSceneMgr)
 	{
 		throw exception("��ȡ��������");
 	}
 	if (!m_OriSceneBufferFile)
 	{
 		throw exception("��ȡ�ļ�����");
 	}
 
 	m_Table.lightDataPtr = ftell(m_OriSceneBufferFile); // ��¼light�������ļ��Ŀ�ʼλ��
 
 	//д��ƹ�����
 	DWORD LightCount = m_pVGSSceneMgr->getLightCount();
 	fwrite(&LightCount, sizeof(LightCount), 1, m_OriSceneBufferFile);
 
 	m_vpLight.clear();  // д��ĵƹ��б�
 	
 	//����ƹ�д������
 	for (UINT i = 0; i < LightCount; ++i)
 	{ 
 		CLight *pLight = m_pVGSSceneMgr->GetLightByIndex(i);
 
 		//д��ƹ��VGS����	
		std::string strLightName = pLight->getName();
		WRITE_STR(strLightName, m_OriSceneBufferFile);
 
 		//д��ƹ�����
		VGS_LIGHT_TYPE type = pLight->getType();
		WRITE_VALUE(type, m_OriSceneBufferFile); 
 
 		//д��ƹ�Ļ�����Ϣ
 		//���ݲ�ͬ�ĵƹ����ͣ�д�벻ͬ�Ĳ���
 		switch(type)
 		{
 		case VLT_OMNI:
 			{
 				N3D_OMNI omni;
 				omni.color = (DWORD)(D3DCOLOR_COLORVALUE(pLight->diffuse.r, pLight->diffuse.g, pLight->diffuse.b, pLight->diffuse.a)); 
 				omni.attenuationRange = pLight->param.Range;
 				omni.attenuation0 = pLight->param.Attenuation0;
 				omni.attenuation1 = pLight->param.Attenuation1;
 				omni.attenuation2 = pLight->param.Attenuation2;
				omni.multiply = pLight->multiply;
 
 				// д����Դ��Ϣ
 				fwrite(&omni, sizeof(omni), 1, m_OriSceneBufferFile);
 			}
 			break;
 
		case VLT_DIR:
 			{
 				N3D_DIRECTIONAL directionLight;

				directionLight.color = (DWORD)(D3DCOLOR_COLORVALUE(pLight->diffuse.r, pLight->diffuse.g, pLight->diffuse.b, pLight->diffuse.a)); 
 
 				// ��ȡ�ƹ�ڵ�����ڸ��ڵ�ķ���
 				directionLight.direction = vector3(pLight->direction.x, pLight->direction.y, pLight->direction.z);
				directionLight.attenuationRange = pLight->param.Range;
				directionLight.attenuation0 = pLight->param.Attenuation0;
				directionLight.attenuation1 = pLight->param.Attenuation1;
				directionLight.attenuation2 = pLight->param.Attenuation2;
				directionLight.multiply = pLight->multiply;
 
 				//д�뷽�����Ϣ
 				fwrite(&directionLight, sizeof(directionLight), 1, m_OriSceneBufferFile);
 			}
 			break;
 
		case VLT_SPOT:
 			{
 				N3D_SPOTLIGHT spotLight;

				spotLight.color = (DWORD)(D3DCOLOR_COLORVALUE(pLight->diffuse.r, pLight->diffuse.g, pLight->diffuse.b, pLight->diffuse.a)); 
 
 				// ��ȡ�ƹ�ڵ�����ڸ��ڵ�ķ���
				spotLight.direction = vector3(pLight->direction.x, pLight->direction.y, pLight->direction.z);
				spotLight.attenuationRange = pLight->param.Range;
				spotLight.attenuation0 = pLight->param.Attenuation0;
				spotLight.attenuation1 = pLight->param.Attenuation1;
				spotLight.attenuation2 = pLight->param.Attenuation2;
				spotLight.multiply = pLight->multiply;

 				spotLight.innerAngle = pLight->param.Theta;
				spotLight.outerAngle = pLight->param.Phi;
				spotLight.fallOff    = pLight->param.Falloff;
 
 				//д��۹����Ϣ
 				fwrite(&spotLight, sizeof(spotLight), 1, m_OriSceneBufferFile);
 			}
 			break;
 		}

 		m_vpLight.push_back(pLight);
 
 	} // for LightCount
}

//ͨ���ƹ����ӵƹ⶯���б��л�õƹ������������
//LightParamAnimation* CSaveV3D::GetLightParamAnimation(std::vector<LightParamAnimation*> list, CLight* pLight)
//{
//	std::vector<LightParamAnimation*>::iterator it = list.begin();
//	int listCnt = list.size();
//	for (int i = 0; i< listCnt; i++)
//	{
//		LightParamAnimation* pAnim = *(it + i);
//		if (pAnim->mLight == pLight)
//			return pAnim;
//	}
//	return NULL;
//}

// ����overlay,��������ͼ
VOID CSaveN3D::WriteOverlayData()
{
 	if (!m_pVGSSceneMgr)
 	{
 		throw exception("��ȡ��������");
 		// return;
 	}
 	if (!m_OriSceneBufferFile)
 	{
 		throw exception("��ȡ�ļ�����");
 		// return;
 	}
 
	// ��¼overlay�������ļ��Ŀ�ʼλ��
 	m_Table.OverlayPtr = ftell(m_OriSceneBufferFile); 
 
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

	// Overlay��Ŀ
	UINT uOverlayCount = GUIMngr.GetObjectsCount(VGS_GUIOBJ_TYPE_OVERLAY); 
	WRITE_VALUE(uOverlayCount, m_OriSceneBufferFile);
 
 	// ����Overlay
 	for (UINT idx = 0; idx < uOverlayCount; ++idx)
	{
		WriteOverlayInfo((COverlay*)GUIMngr.GetObjectHandlebyIndex(VGS_GUIOBJ_TYPE_OVERLAY, idx)); 		
 	} // for


	// ������ڱ���ͼ�����汳��ͼ
	UINT uBackGroundCnt = GUIMngr.GetObjectsCount(VGS_GUIOBJ_TYPE_BACKDROP); 
	WRITE_VALUE(uBackGroundCnt, m_OriSceneBufferFile);
 	for (UINT idx = 0; idx < uBackGroundCnt; ++idx)
	{
		WriteOverlayInfo((COverlay*)GUIMngr.GetObjectHandlebyIndex(VGS_GUIOBJ_TYPE_BACKDROP, idx), TRUE);	
	}
}

// д��һ��overlay��Ϣ
void CSaveN3D::WriteOverlayInfo(COverlay *pOverlay, BOOL bIsBackGround)
{
	if (!pOverlay)
	{
		return;
	}

	N3D_OverlayBasalInfoV3 OverlayInfo;
	ZeroMemory(&OverlayInfo, sizeof(OverlayInfo));
	//
	strcpy(OverlayInfo.Name, pOverlay->GetName());
	OverlayInfo.UseRelativePos  = pOverlay->GetUsedRelativePos();
	OverlayInfo.UseRelativeSize = pOverlay->GetUsedRelativeSize();
	pOverlay->GetRect(OverlayInfo.Rect);
	OverlayInfo.Angle = pOverlay->getRotation();
	OverlayInfo.Opacity = pOverlay->GetOpacity();
	OverlayInfo.IsBackdrop = bIsBackGround;
	OverlayInfo.MaterialIndex = _GetIndexInVector(m_vMaterialList, (DWORD)pOverlay->GetMaterial());
	OverlayInfo.IsVisible = pOverlay->GetVisibility();
	OverlayInfo.zOrder = pOverlay->getZOrder();	
	//
	std::string strFuncName;
	for (int iFunc = 0; iFunc < VGS_MOUSEEVENT_CNT; iFunc++)
	{
		strFuncName = pOverlay->GetEventFuncName(VGS_MOUSEEVENT_TYPE(iFunc));
		strcpy(OverlayInfo.sMouseEventFunc[iFunc], strFuncName.c_str()); 
	}
	WRITE_VALUE(OverlayInfo, m_OriSceneBufferFile);

	// д��ؼ�֡��Ϣ
	UINT keyframeCount = 0;
	WRITE_VALUE(keyframeCount, m_OriSceneBufferFile);
	// Add key frame animation data there!

/*
	// overlay����
	std::string strName = pOverlay->GetName();
	UINT len = strName.length();
	WRITE_VALUE(len, m_OriSceneBufferFile);
	WRITE_PTR(strName.c_str(), len, m_OriSceneBufferFile);

	OverlayInfo.Angle = pOverlay->getRotation();
	WRITE_VALUE(OverlayInfo.Angle, m_OriSceneBufferFile);

	OverlayInfo.Opacity = pOverlay->GetOpacity();
	WRITE_VALUE(OverlayInfo.Opacity, m_OriSceneBufferFile);

	OverlayInfo.OffsetOrigin = (int)pOverlay->GetOffsetOrigin();
	WRITE_VALUE(OverlayInfo.OffsetOrigin, m_OriSceneBufferFile);

	pOverlay->GetRect(OverlayInfo.Rect);
	WRITE_VALUE(OverlayInfo.Rect.left, m_OriSceneBufferFile);
	WRITE_VALUE(OverlayInfo.Rect.top, m_OriSceneBufferFile);
	WRITE_VALUE(OverlayInfo.Rect.right, m_OriSceneBufferFile);
	WRITE_VALUE(OverlayInfo.Rect.bottom, m_OriSceneBufferFile);

	OverlayInfo.UseRelativePos  = pOverlay->GetUsedRelativePos();
	OverlayInfo.UseRelativeSize = pOverlay->GetUsedRelativeSize();
	OverlayInfo.IsVisible       = pOverlay->GetVisibility();
	int  zOrder                 = pOverlay->getZOrder();	
	WRITE_VALUE(OverlayInfo.UseRelativePos, m_OriSceneBufferFile);
	WRITE_VALUE(OverlayInfo.UseRelativeSize, m_OriSceneBufferFile);
	WRITE_VALUE(OverlayInfo.IsVisible, m_OriSceneBufferFile);
	WRITE_VALUE(zOrder, m_OriSceneBufferFile);  // overlay��ǰ���ϵ��ֵԽ��Խ����

	// ʹ�õĲ��ʱ��
	int matIndex = _GetIndexInVector(m_vMaterialList, (DWORD)pOverlay->GetMaterial());
	WRITE_VALUE(matIndex, m_OriSceneBufferFile);

	// д���¼�����
	for (int iFunc = (int)VGS_MOUSEEVENT_TYPE_OVER; iFunc <= (int)VGS_MOUSEEVENT_TYPE_LEAVE; ++iFunc)
	{
		std::string strFunc = pOverlay->GetEventFuncName((VGS_MOUSEEVENT_TYPE)iFunc);
		UINT len = strFunc.length();
		WRITE_VALUE(len, m_OriSceneBufferFile);       // д�뺯�����Ƴ���
		if (len > 0)
		{
			WRITE_PTR(strFunc.c_str(), len, m_OriSceneBufferFile);  // д�뺯������
		}
	}

	// д��ؼ�֡��Ϣ
	UINT keyframeCount = 0;
	WRITE_VALUE(keyframeCount, m_OriSceneBufferFile);
	for (int iFrm = 0; iFrm < OverlayInfo.KfrCnt; ++iFrm)
	{			
	fwrite(&(OverlayInfo.KfrInfo[iFrm]), sizeof(OverlayInfo.KfrInfo[iFrm]), 1, m_OriSceneBufferFile);
	}
*/
}

//VOID CSaveN3D::WriteBackGround()
//{
//};

// ����N3D�б���ĵ���ͼ��ʹ�õ�overlay��Ϣ
void CSaveN3D::GetNavigatorInfo()
{
// 	m_vNaviInfo.clear();
// 
// 	UINT count = GUI_GetObjectsCount(VGSGUITYPE::VGT_NAVIGATION);   // ����ͼ
// 
// 	DWORD *pHandle = new DWORD[count];
// 	GUI_GetObjectsList(VGSGUITYPE::VGT_NAVIGATION, pHandle);
// 
// 	// ���浼��ͼ
// 	for (UINT idx = 0; idx < count; ++idx)
// 	{
// 		GUI_CommonProperty info;
// 		GUI_GetObjectCommonPropety(pHandle[idx], info);
// 
// 		N3D_NaviOverlay NaviOverlay;
// 
// 		strcpy(NaviOverlay.Name, info.sName);
// 		std::string sName = std::string(info.sName) + std::string("_NavOly");
// 		strcpy(NaviOverlay.OverlayInfo.BasalInfo.Name, sName.c_str());
// 
// 		// ����ͼ��λ�úͳߴ�
// 		float width, height, left, top;
// 		bool bRelativePos, bRelativeSize;		
// 		GUI_GetObjectSize(pHandle[idx], width, height, bRelativeSize);
// 		GUI_GetObjectPosition(pHandle[idx], left, top, bRelativePos);
// 		NaviOverlay.OverlayInfo.BasalInfo.Rect.left   = (INT)left;
// 		NaviOverlay.OverlayInfo.BasalInfo.Rect.right  = (INT)left + (INT)width;
// 		NaviOverlay.OverlayInfo.BasalInfo.Rect.top    = (INT)top;
// 		NaviOverlay.OverlayInfo.BasalInfo.Rect.bottom = (INT)top + (INT)height;
// 		NaviOverlay.OverlayInfo.BasalInfo.UseRelativePos = bRelativePos;
// 		NaviOverlay.OverlayInfo.BasalInfo.UseRelativeSize = bRelativeSize;
// 
// 		NaviOverlay.OverlayInfo.BasalInfo.OffsetOrigin = info.OffsetOrigin;
// 		NaviOverlay.OverlayInfo.BasalInfo.Angle = 0;
// 		NaviOverlay.OverlayInfo.BasalInfo.Opacity = info.Opacity;
// 		NaviOverlay.OverlayInfo.BasalInfo.IsBackdrop = false;
// 		NaviOverlay.OverlayInfo.BasalInfo.IsVisible = info.bVisible;
// 
// 		// NaviOverlay.OverlayInfo.BasalInfo.ZOrder = ; // ����ͼ��ZOrder�����
// 
// 		// ����ͼ�ĵ�����Χ
// 		float minX, maxX, minZ, maxZ;
// 		GUI_GetNavigaterSceneRect(pHandle[idx], minX, maxX, minZ, maxZ);
// 		NaviOverlay.sceneLeft   = minX;
// 		NaviOverlay.sceneRight  = maxX;
// 		NaviOverlay.sceneTop    = minZ;
// 		NaviOverlay.sceneBottom = maxZ;
// 
// 		// ��ͼʹ�õĲ���
// 		char sMatName[256];
// 		GUI_GetNavigaterMaterial(pHandle[idx], sMatName);
// 		MaterialPtr matPtr = MaterialManager::getSingleton().getByName(std::string(sMatName));
// 		NaviOverlay.OverlayInfo.BasalInfo.MaterialIndex = _GetIndexInVector(m_vMaterialList, (DWORD)matPtr.get());
// 
// 		NaviOverlay.OverlayInfo.KfrCnt = 0;
// 
// 		// ��������Ŀ
// 		UINT pointCount = GUI_GetNavPointerCount(pHandle[idx]);
// 		for (UINT iPoint = 0; iPoint < pointCount; ++iPoint)
// 		{
// 			N3D_NaviPointerOverlay PointOverlay;
// 					
// 			// �����������
// 			GUI_GetNavPointerNameByIndex(pHandle[idx], iPoint, PointOverlay.Name);
// 			std::string sName = std::string(PointOverlay.Name) + std::string("_PtrOly");
// 			strcpy(PointOverlay.OverlayInfo.BasalInfo.Name, sName.c_str());
// 
// 			// ����������Ϣ
// 			int type;
// 			DWORD pointedHandle;
// 			PointOverlay.ObjIndex = -1;
// 			if (true == GUI_GetNavPointerObj(pHandle[idx], PointOverlay.Name, type, pointedHandle))
// 			{
// 				PointOverlay.Type     = type;				
// 
// 				if (type == 0) // camera
// 				{				
// 					PointOverlay.ObjIndex	= _GetIndexInVector(m_vpCamera, (CCamera*)pointedHandle);
// 				}
// 				else if (type == 1)  // model
// 				{
// 					PointOverlay.ObjIndex	= _GetIndexInVector(m_vEntityList, pointedHandle);
// 				}
// 			}
// 
// 			// ������ʹ�õĲ���
// 			char sMatName[256];
// 			GUI_GetPointerMaterail(pHandle[idx], PointOverlay.Name, sMatName);
// 			MaterialPtr matPtr = MaterialManager::getSingleton().getByName(std::string(sMatName));
// 			PointOverlay.OverlayInfo.BasalInfo.MaterialIndex = _GetIndexInVector(m_vMaterialList, (DWORD)matPtr.get());
// 
// 			PointOverlay.OverlayInfo.BasalInfo.Angle = 0;
// 			PointOverlay.OverlayInfo.BasalInfo.IsBackdrop = false;
// 			PointOverlay.OverlayInfo.BasalInfo.OffsetOrigin = 0;
// 			PointOverlay.OverlayInfo.BasalInfo.Opacity = GUI_GetNavPointerOpacity(pHandle[idx], PointOverlay.Name);
// 
// 			PointOverlay.OverlayInfo.BasalInfo.IsVisible = GUI_GetNavPointerVisiblilty(pHandle[idx], PointOverlay.Name);
// 
// 			PointOverlay.OverlayInfo.BasalInfo.ZOrder = iPoint;  // fuxb 2008-11-21 �������ڵ�������ͼ�ڵ�ZOrder
// 
// 			int width, height, left, top;
// 			GUI_GetNavPointerSize(pHandle[idx], PointOverlay.Name, width, height);
// 			GUI_GetPointerPosition(pHandle[idx], PointOverlay.Name, left, top);
// 			PointOverlay.OverlayInfo.BasalInfo.Rect.left   = left;
// 			PointOverlay.OverlayInfo.BasalInfo.Rect.right  = left + width;
// 			PointOverlay.OverlayInfo.BasalInfo.Rect.top    = top;
// 			PointOverlay.OverlayInfo.BasalInfo.Rect.bottom = top + height;
// 
// 			PointOverlay.OverlayInfo.BasalInfo.UseRelativePos  = false;
// 			PointOverlay.OverlayInfo.BasalInfo.UseRelativeSize = false;
// 
// 			PointOverlay.OverlayInfo.KfrCnt = 0;
// 
// 			NaviOverlay.vPointer.push_back(PointOverlay);  // ���浼����
// 
// 		} // for ������
// 
// 		m_vNaviInfo.push_back(NaviOverlay); // ���浼��ͼ
// 
// 	}  // for ����ͼ
// 
// 	delete [] pHandle;
// 	pHandle = NULL;
}

VOID CSaveN3D::WriteNavigaterData()
{
 	if (!m_pVGSSceneMgr)
 	{
 		throw exception("��ȡ��������");
 		// return;
 	}
 	if (!m_OriSceneBufferFile)
 	{
 		throw exception("��ȡ�ļ�����");
 		// return;
 	}
 
	// ��¼Navigator�������ļ��Ŀ�ʼλ��
 	m_Table.NavigatotPtr = ftell(m_OriSceneBufferFile); 
  
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

	// ����ͼ��Ŀ
	UINT uNaviCount = GUIMngr.GetObjectsCount(VGS_GUIOBJ_TYPE_NAVIGATION);
	fwrite(&(uNaviCount), sizeof(UINT), 1, m_OriSceneBufferFile); 

 	// ���浼��ͼ
 	for (UINT idx = 0; idx < uNaviCount; ++idx)
 	{
 		CNavigator *pNavigator = (CNavigator*)GUIMngr.GetObjectHandlebyIndex(VGS_GUIOBJ_TYPE_NAVIGATION, idx);
		WriteOverlayInfo(pNavigator);
 
 		// ����ͼ�ĵ�����Χ
		FRect rect;
		pNavigator->GetNavRange(rect);
		WRITE_VALUE(rect.Left, m_OriSceneBufferFile);
		WRITE_VALUE(rect.Right, m_OriSceneBufferFile);
		WRITE_VALUE(rect.Top, m_OriSceneBufferFile);
		WRITE_VALUE(rect.Bottom, m_OriSceneBufferFile);
  
 		// ��������Ŀ
		UINT pointerCnt = pNavigator->GetPointerCount();
		WRITE_VALUE(pointerCnt, m_OriSceneBufferFile);
 
 		for (UINT iPoint = 0; iPoint < pointerCnt; ++iPoint)
 		{
			CNavPointer *pPointer = pNavigator->GetPointerByIndex(iPoint);
			WriteOverlayInfo(pPointer); 
 		} // for ������
 	}
}

//VOID CSaveN3D::WriteNavigaterData()
//{
//	if (!m_pVGSSceneMgr)
//	{
//		throw Ogre::Exception::exception("��ȡ��������");
//		// return;
//	}
//	if (!m_OriSceneBufferFile)
//	{
//		throw Ogre::Exception::exception("��ȡ�ļ�����");
//		// return;
//	}
//
//	m_Table.NavigatotPtr = ftell(m_OriSceneBufferFile); // ��¼camera�������ļ��Ŀ�ʼλ��
//
//	UINT count = GUI_GetObjectsCount(VGSGUITYPE::VGT_NAVIGATION);   // ����ͼ
//
//	fwrite(&count, sizeof(count), 1, m_OriSceneBufferFile); // ����ͼ����Ŀ
//
//	DWORD *pHandle = new DWORD[count];
//	GUI_GetObjectsList(VGSGUITYPE::VGT_NAVIGATION, pHandle);
//
//	// ���浼��ͼ
//	for (UINT idx = 0; idx < count; ++idx)
//	{
//		GUI_CommonProperty info;
//		GUI_GetObjectCommonPropety(pHandle[idx], info);
//
//		N3D_Navigater NavInfo;
//		ZeroMemory(&NavInfo, sizeof(NavInfo));
//
//		strcpy(NavInfo.BasalInfo.Name, info.sName);
//
//		// ����ͼ��λ�úͳߴ�
//		float width, height, left, top;
//		bool bRelativePos, bRelativeSize;		
//		GUI_GetObjectSize(pHandle[idx], width, height, bRelativeSize);
//		GUI_GetObjectPosition(pHandle[idx], left, top, bRelativePos);
//		NavInfo.BasalInfo.Rect.left   = (INT)left;
//		NavInfo.BasalInfo.Rect.right  = (INT)left + (INT)width;
//		NavInfo.BasalInfo.Rect.top    = (INT)top;
//		NavInfo.BasalInfo.Rect.bottom = (INT)top + (INT)height;
//		NavInfo.BasalInfo.UseRelativePos = bRelativePos;
//		NavInfo.BasalInfo.UseRelativeSize = bRelativeSize;
//
//		// ����ͼ�ĵ�����Χ
//		float minX, maxX, minZ, maxZ;
//		GUI_GetNavigaterSceneRect(pHandle[idx], minX, maxX, minZ, maxZ);
//		NavInfo.BasalInfo.sceneLeft   = minX;
//		NavInfo.BasalInfo.sceneRight  = maxX;
//		NavInfo.BasalInfo.sceneTop    = minZ;
//		NavInfo.BasalInfo.sceneBottom = maxZ;
//
//		// ��ͼʹ�õĲ���
//		char sMatName[256];
//		GUI_GetNavigaterMaterial(pHandle[idx], sMatName);
//		MaterialPtr matPtr = MaterialManager::getSingleton().getByName(std::string(sMatName));
//		NavInfo.BasalInfo.MapMaterialIndex = _GetIndexInVector(m_vMaterialList, (DWORD)matPtr.get());
//
//		// д�������Ϣ
//		fwrite(&(NavInfo.BasalInfo), sizeof(NavInfo.BasalInfo), 1, m_OriSceneBufferFile);		
//
//		// ��������Ŀ
//		NavInfo.PointerCnt = GUI_GetNavPointerCount(pHandle[idx]);
//		fwrite(&(NavInfo.PointerCnt), sizeof(NavInfo.PointerCnt), 1, m_OriSceneBufferFile);
//
//		for (UINT iPoint = 0; iPoint < NavInfo.PointerCnt; ++iPoint)
//		{
//			// ��������Ϣ
//			N3D_NavigaterPointer PointInfo;
//			ZeroMemory(&PointInfo, sizeof(PointInfo));
//			
//			// �����������
//			GUI_GetNavPointerNameByIndex(pHandle[idx], iPoint, PointInfo.Name);
//
//			// ����������Ϣ
//			int type;
//			DWORD pointedHandle;
//			PointInfo.ObjIndex = -1;
//			if (true == GUI_GetNavPointerObj(pHandle[idx], PointInfo.Name, type, pointedHandle))
//			{
//				PointInfo.Type     = type;				
//
//				if (type == 0) // camera
//				{				
//					PointInfo.ObjIndex	= _GetIndexInVector(m_vpCamera, (CCamera*)pointedHandle);
//				}
//				else if (type == 1)  // model
//				{
//					PointInfo.ObjIndex	= _GetIndexInVector(m_vEntityList, pointedHandle);
//				}
//			}
//
//			// ������ʹ�õĲ���
//			char sMatName[256];
//			GUI_GetPointerMaterail(pHandle[idx], PointInfo.Name, sMatName);
//			MaterialPtr matPtr = MaterialManager::getSingleton().getByName(std::string(sMatName));
//			PointInfo.materialIndex = _GetIndexInVector(m_vMaterialList, (DWORD)matPtr.get());
//
//			// д�뵼������Ϣ
//			fwrite(&PointInfo, sizeof(PointInfo), 1, m_OriSceneBufferFile);
//
//		} // for ������
//
//	}  // for ����ͼ
//
//	delete [] pHandle;
//	pHandle = NULL;
//}

// ����N3D�б����Button��Ϣ
void CSaveN3D::GetButtonInfo()
{
// 	m_vButtonInfo.clear();
// 
// 	UINT count = GUI_GetObjectsCount(VGSGUITYPE::VGT_BUTTON);   // button
// 	DWORD *pHandle = new DWORD[count];
// 	GUI_GetObjectsList(VGSGUITYPE::VGT_BUTTON, pHandle);
// 
// 	// ����button
// 	for (UINT idx = 0; idx < count; ++idx)
// 	{
// 		N3D_ButtonOverlay BtnOverlay;
// 
// 		// BtnOverlay.BtnHandle = pHandle[idx];
// 		GUI_CommonProperty info;
// 		GUI_GetObjectCommonPropety(pHandle[idx], info);
// 
// 		// ���ɰ�ťʹ�õ�Overlay����
// 		std::string sName = std::string(info.sName) + "_btnOly";
// 		strcpy(BtnOverlay.OverlayInfo.BasalInfo.Name, sName.c_str());
// 		strcpy(BtnOverlay.Name, info.sName);
// 		
// 		BtnOverlay.OverlayInfo.KfrCnt = 0;
// 		BtnOverlay.OverlayInfo.KfrInfo = NULL;
// 
// 		char sCallback[256];
// 		GUI_GetButtonCallBack(pHandle[idx], sCallback);
// 		BtnOverlay.sCallBackFunc = std::string(sCallback);
// 		BtnOverlay.OverlayInfo.BasalInfo.Opacity = info.Opacity;
// 
// 		float width, height, left, top;
// 		bool bRelativePos, bRelativeSize;		
// 		GUI_GetObjectSize(pHandle[idx], width, height, bRelativeSize);
// 		GUI_GetObjectPosition(pHandle[idx], left, top, bRelativePos);
// 		BtnOverlay.OverlayInfo.BasalInfo.Rect.left   = (INT)left;
// 		BtnOverlay.OverlayInfo.BasalInfo.Rect.right  = (INT)left + (INT)width;
// 		BtnOverlay.OverlayInfo.BasalInfo.Rect.top    = (INT)top;
// 		BtnOverlay.OverlayInfo.BasalInfo.Rect.bottom = (INT)top + (INT)height;
// 		BtnOverlay.OverlayInfo.BasalInfo.UseRelativePos = bRelativePos;
// 		BtnOverlay.OverlayInfo.BasalInfo.UseRelativeSize = bRelativeSize;
// 
// 		BtnOverlay.OverlayInfo.BasalInfo.Angle        = 0;
// 		BtnOverlay.OverlayInfo.BasalInfo.IsBackdrop   = false;
// 		BtnOverlay.OverlayInfo.BasalInfo.IsVisible    = info.bVisible;
// 		BtnOverlay.OverlayInfo.BasalInfo.OffsetOrigin = info.OffsetOrigin;
// 
// 		BtnOverlay.OverlayInfo.BasalInfo.ZOrder = info.ZOrder;  // fuxb 2008-11-21
// 
// 		// ʹ�õĲ��ʱ��
// 		char sUpMaterialName[256], sDownMaterialName[256], sOverMaterialName[256];		
// 		if (true == GUI_GetButtonMaterial(pHandle[idx], sUpMaterialName, sDownMaterialName, sOverMaterialName))
// 		{
// 			BtnOverlay.OverlayInfo.BasalInfo.MaterialIndex = -1;
// 			MaterialPtr matPtr = MaterialManager::getSingleton().getByName(std::string(sUpMaterialName));
// 			if (matPtr.get())
// 			{
// 				BtnOverlay.OverlayInfo.BasalInfo.MaterialIndex = _GetIndexInVector(m_vMaterialList, (DWORD)matPtr.get());
// 			}	
// 
// 			BtnOverlay.DownMaterialIndex = -1;
// 			matPtr = MaterialManager::getSingleton().getByName(std::string(sDownMaterialName));
// 			if (matPtr.get())
// 			{
// 				BtnOverlay.DownMaterialIndex = _GetIndexInVector(m_vMaterialList, (DWORD)matPtr.get());
// 			}
// 
// 			BtnOverlay.OverMaterialIndex = -1;
// 			matPtr = MaterialManager::getSingleton().getByName(std::string(sOverMaterialName));
// 			if (matPtr.get())
// 			{
// 				BtnOverlay.OverMaterialIndex = _GetIndexInVector(m_vMaterialList, (DWORD)matPtr.get());
// 			}
// 		}
// 		else
// 		{
// 			BtnOverlay.OverlayInfo.BasalInfo.MaterialIndex = -1;
// 		}
// 
// 		m_vButtonInfo.push_back(BtnOverlay);  // ���浽�б�
// 
// 	}  // for
// 	
// 	// �ͷ��ڴ�
// 	delete [] pHandle;
// 	pHandle = NULL;
}

VOID CSaveN3D::WriteButtonData()
{
 	if (!m_pVGSSceneMgr)
 	{
 		throw exception("��ȡ��������");
 		// return;
 	}
 	if (!m_OriSceneBufferFile)
 	{
 		throw exception("��ȡ�ļ�����");
 		// return;
 	}
 
	// ��¼button�������ļ��Ŀ�ʼλ��
 	m_Table.ButtonPtr = ftell(m_OriSceneBufferFile);
   
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

	// д��button����Ŀ
	UINT uButtonCount = GUIMngr.GetObjectsCount(VGS_GUIOBJ_TYPE_BUTTON); 
	WRITE_VALUE(uButtonCount, m_OriSceneBufferFile);		
 
 	// ����button
 	for (UINT idx = 0; idx < uButtonCount; ++idx)
 	{		
		WriteOverlayInfo((COverlay*)GUIMngr.GetObjectHandlebyIndex(VGS_GUIOBJ_TYPE_BUTTON, idx));
	}// for
}

//VOID CSaveN3D::WriteButtonData()
//{
//	if (!m_pVGSSceneMgr)
//	{
//		throw Ogre::Exception::exception("��ȡ��������");
//		// return;
//	}
//	if (!m_OriSceneBufferFile)
//	{
//		throw Ogre::Exception::exception("��ȡ�ļ�����");
//		// return;
//	}
//
//	m_Table.ButtonPtr = ftell(m_OriSceneBufferFile); // ��¼camera�������ļ��Ŀ�ʼλ��
//
//	UINT count = GUI_GetObjectsCount(VGSGUITYPE::VGT_BUTTON);   // button
//	fwrite(&count, sizeof(count), 1, m_OriSceneBufferFile);		// д��button����Ŀ
//
//	DWORD *pHandle = new DWORD[count];
//	GUI_GetObjectsList(VGSGUITYPE::VGT_BUTTON, pHandle);
//
//	// ����button
//	for (UINT idx = 0; idx < count; ++idx)
//	{
//		GUI_CommonProperty info;
//		GUI_GetObjectCommonPropety(pHandle[idx], info);
//
//		N3D_Button BtnInfo;
//		ZeroMemory(&BtnInfo, sizeof(BtnInfo));
//
//		BtnInfo.KfrCnt = 0;
//		BtnInfo.KfrInfo = NULL;
//
//		// BtnInfo.BasalInfo.BtnOverlayIndex
//		GUI_GetButtonCallBack(pHandle[idx], BtnInfo.BasalInfo.CallBackFunc);
//		strcpy(BtnInfo.BasalInfo.Name, info.sName);
//		BtnInfo.BasalInfo.Opacity = info.Opacity;
//
//		float width, height, left, top;
//		bool bRelativePos, bRelativeSize;		
//		GUI_GetObjectSize(pHandle[idx], width, height, bRelativeSize);
//		GUI_GetObjectPosition(pHandle[idx], left, top, bRelativePos);
//		BtnInfo.BasalInfo.Rect.left   = (INT)left;
//		BtnInfo.BasalInfo.Rect.right  = (INT)left + (INT)width;
//		BtnInfo.BasalInfo.Rect.top    = (INT)top;
//		BtnInfo.BasalInfo.Rect.bottom = (INT)top + (INT)height;
//		BtnInfo.BasalInfo.UseRelativePos = bRelativePos;
//		BtnInfo.BasalInfo.UseRelativeSize = bRelativeSize;
//
//		// ʹ�õĲ��ʱ��
//		char sUpMaterialName[256], sDownMaterialName[256], sOverMaterialName[256];		
//		if (true == GUI_GetButtonMaterial(pHandle[idx], sUpMaterialName, sDownMaterialName, sOverMaterialName))
//		{
//			MaterialPtr matPtr = MaterialManager::getSingleton().getByName(std::string(sUpMaterialName));
//			if (matPtr.get())
//			{
//				BtnInfo.BasalInfo.MouseUpMtrlIndex = _GetIndexInVector(m_vMaterialList, (DWORD)matPtr.get());
//			}
//
//			matPtr = MaterialManager::getSingleton().getByName(std::string(sDownMaterialName));
//			if (matPtr.get())
//			{
//				BtnInfo.BasalInfo.MouseDownMtrlIndex = _GetIndexInVector(m_vMaterialList, (DWORD)matPtr.get());
//			}
//
//			matPtr = MaterialManager::getSingleton().getByName(std::string(sOverMaterialName));
//			if (matPtr.get())
//			{
//				BtnInfo.BasalInfo.MouseOverMtrlIndex = _GetIndexInVector(m_vMaterialList, (DWORD)matPtr.get());
//			}
//		}
//		else
//		{
//			BtnInfo.BasalInfo.MouseUpMtrlIndex = -1;
//			BtnInfo.BasalInfo.MouseDownMtrlIndex = -1;
//			BtnInfo.BasalInfo.MouseOverMtrlIndex = -1;
//		}
//
//		// д��button�Ļ�����Ϣ
//		fwrite(&(BtnInfo.BasalInfo), sizeof(BtnInfo.BasalInfo), 1, m_OriSceneBufferFile); 
//
//		// д��ؼ�֡��Ϣ
//		fwrite(&(BtnInfo.KfrCnt), sizeof(BtnInfo.KfrCnt), 1, m_OriSceneBufferFile);
//		for (int iFrm = 0; iFrm < BtnInfo.KfrCnt; ++iFrm)
//		{			
//			fwrite(&(BtnInfo.KfrInfo[iFrm]), sizeof(BtnInfo.KfrInfo[iFrm]), 1, m_OriSceneBufferFile);
//		}
//
//	}// for
//
//	delete [] pHandle;
//	pHandle = NULL;
//}

VOID CSaveN3D::WriteAudioData()
{
// 	if (!m_pVGSSceneMgr)
// 	{
// 		throw Ogre::Exception::exception("��ȡ��������");
// 		// return;
// 	}
// 	if (!m_OriSceneBufferFile)
// 	{
// 		throw Ogre::Exception::exception("��ȡ�ļ�����");
// 		// return;
// 	}
// 
 	m_Table.AudioPtr = ftell(m_OriSceneBufferFile); // ��¼audio�������ļ��Ŀ�ʼλ��
 
 	UINT i = 0;
 	fwrite(&i, sizeof(UINT), 1, m_OriSceneBufferFile); // û�����ݣ�д��0
}

VOID CSaveN3D::WriteControlData()
{
 	if (!m_pVGSSceneMgr)
 	{
 		throw exception("��ȡ��������");
 	}
 	if (!m_OriSceneBufferFile)
 	{
 		throw exception("��ȡ�ļ�����");
 	}
 
 	m_Table.ControlPtr = ftell(m_OriSceneBufferFile); // ��¼control�������ļ��Ŀ�ʼλ��
 
 	// ����������Ϳ�������Ŀ�Լ��ܵ���Ŀ
 	DWORD WalkerCount = m_pVGSSceneMgr->GetWalkerCount();
 	DWORD FlyerCount  = m_pVGSSceneMgr->GetFlyerCount();
 	DWORD EditorCount = m_pVGSSceneMgr->GetEditorCount();
 	DWORD TotalCount = WalkerCount + FlyerCount + EditorCount;
 	
 	DWORD CurrentController = (DWORD)(m_pVGSSceneMgr->GetCurrentController()); // ��ǰ������ָ��
 	UINT CurrentIdx = 0;  // ��ǰ�������ڶ����е�����
 
 	WRITE_VALUE(WalkerCount, m_OriSceneBufferFile);								// �ܵĿ���������
 
 	if (TotalCount == 0)  // ���һ��controlҲû�У��Ͳ���Ҫ��������������
 	{
 		return;
 	}
 
 	// Walker����Ŀ���б�
	WRITE_VALUE(WalkerCount, m_OriSceneBufferFile);
 	for (UINT i = 0; i < WalkerCount; ++i)
 	{
		WalkerInfo Info;
 		CWalker *pController = (CWalker*)(m_pVGSSceneMgr->GetWalker(i));
 		pController->GetWalkerInfo(Info);

		// д������
		std::string strName = pController->GetName();
		WRITE_STR(strName, m_OriSceneBufferFile);

		// ʹ�õ��������
		string sCamName = pController->GetCamera()->getName();

		N3D_Walker walker;
		ZeroMemory(&walker, sizeof(walker));
		walker.CameraIndex = GetCameraIndex(m_pVGSSceneMgr->GetCameraByName(sCamName));  // ����walker�ı��		
 		walker.bCollision = Info.bCollision;
 		walker.bWeight    = Info.bWeight;
 		walker.colDis     = Info.colDis;
 		walker.height     = Info.height;
 		walker.liftAcc    = Info.liftAcc;
 		walker.liftSpeed  = Info.liftSpeed;
 		walker.rosAcc     = Info.rosAcc;
 		walker.rosSpeed   = Info.rosSpeed;
 		walker.upperAngle = Info.upperAngle;
 		walker.walkAcc    = Info.walkAcc;
 		walker.walkSpeed  = Info.walkSpeed;

		// д��walker��Ϣ
		WRITE_VALUE(walker.CameraIndex, m_OriSceneBufferFile);
		WRITE_VALUE(walker.bCollision, m_OriSceneBufferFile);
		WRITE_VALUE(walker.bWeight, m_OriSceneBufferFile);
		WRITE_VALUE(walker.colDis, m_OriSceneBufferFile);
		WRITE_VALUE(walker.height, m_OriSceneBufferFile);
		WRITE_VALUE(walker.liftAcc, m_OriSceneBufferFile);
		WRITE_VALUE(walker.liftSpeed, m_OriSceneBufferFile);
		WRITE_VALUE(walker.rosAcc, m_OriSceneBufferFile);
		WRITE_VALUE(walker.rosSpeed, m_OriSceneBufferFile);
		WRITE_VALUE(walker.upperAngle, m_OriSceneBufferFile);
		WRITE_VALUE(walker.walkAcc, m_OriSceneBufferFile);
		WRITE_VALUE(walker.walkSpeed, m_OriSceneBufferFile);
 	
		// ��¼��ǰ���ƶ�������
 		if (CurrentController == (DWORD)(pController))
 		{
 			CurrentIdx = i;
 		}
 	}
 	
 	// editor����Ŀ���б�
 	fwrite(&EditorCount, sizeof(EditorCount), 1, m_OriSceneBufferFile);
 	for (UINT i = 0; i < EditorCount; ++i)
 	{
 		EditorInfo Info;
 		CEditor *pController = (CEditor*)(m_pVGSSceneMgr->GetEditor(i));
		pController->GetEditorInfo(Info);

		// д������
		std::string strName = pController->GetName();
		WRITE_STR(strName, m_OriSceneBufferFile);

		// ʹ�õ��������
		string sCamName = pController->GetCamera()->getName();
 
 		N3D_Editor Editor;
 		ZeroMemory(&Editor, sizeof(Editor));
 		Editor.CameraIndex  = GetCameraIndex(m_pVGSSceneMgr->GetCameraByName(sCamName));  // ����Editor�ı��
 		
 		Editor.autoSpeedX        = Info.autoSpeedX;
 		Editor.autoSpeedY        = Info.autoSpeedY;
 		Editor.bIsAutoRoundRoll  = Info.bIsAutoRoundRoll;
 		Editor.dollyAcc          = Info.dollyAcc;
 		Editor.dollyFarDis       = Info.dollyFarDis;
 		Editor.dollyNearDis      = Info.dollyNearDis;
 		Editor.dollySpeed        = Info.dollySpeed;
 		Editor.LookAtPos.x        = Info.LookAtPos.x;
 		Editor.LookAtPos.y        = Info.LookAtPos.y;
 		Editor.LookAtPos.z        = Info.LookAtPos.z;
 		Editor.manualPanXSpeed   = Info.manualPanXSpeed;
 		Editor.manualPanYSpeed   = Info.manualPanYSpeed;
 		Editor.manualSpeedX      = Info.manualSpeedX;
 		Editor.manualSpeedY      = Info.manualSpeedY;
 		Editor.rosAxis           = Info.rosAxis;

		// д��walker��Ϣ
		WRITE_VALUE(Editor.CameraIndex, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.autoSpeedX, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.autoSpeedY, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.dollyAcc, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.dollyFarDis, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.dollyNearDis, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.dollySpeed, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.LookAtPos.x, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.LookAtPos.y, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.LookAtPos.z, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.manualPanXSpeed, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.manualPanYSpeed, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.manualSpeedX, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.manualSpeedY, m_OriSceneBufferFile);
		WRITE_VALUE(Editor.rosAxis, m_OriSceneBufferFile);
 
 		// �������ģ�ͣ�����ģ�͵�λ��
 		CModel *pModel = pController->GetTargetModel();
 		if (pModel)
 		{
			D3DXVECTOR3 v3Pos = pModel->m_pNode->getPosition(VTS_WORLD);
 			Editor.LookAtPos = vector3(v3Pos.x, v3Pos.y, v3Pos.z);
 		}
 		fwrite(&Editor, sizeof(Editor), 1, m_OriSceneBufferFile);
 
 		if (CurrentController == (DWORD)(pController))
 		{
 			CurrentIdx = i;
 		}
 	}
 
 	// flyer����Ŀ���б�
 	fwrite(&FlyerCount, sizeof(FlyerCount), 1, m_OriSceneBufferFile);
 	for (UINT i = 0; i < FlyerCount; ++i)
 	{
 		FlyerInfo Info;
 		CFlyer *pController = (CFlyer*)(m_pVGSSceneMgr->GetFlyer(i));
		pController->GetFlyerInfo(Info);

		// д������
		std::string strName = pController->GetName();
		WRITE_STR(strName, m_OriSceneBufferFile);

		// ʹ�õ��������
		string sCamName = pController->GetCamera()->getName();
 
 		N3D_Flyer Flyer;
		ZeroMemory(&Flyer, sizeof(Flyer));
 		Flyer.CameraIndex  = GetCameraIndex(m_pVGSSceneMgr->GetCameraByName(sCamName));  // ����walker�ı��
 		Flyer.bCollision   = Info.bCollision;
 		Flyer.colDis       = Info.colDis;
 		Flyer.dollyAcc     = Info.dollyAcc;
 		Flyer.dollyFarDis  = Info.dollyFarDis;
 		Flyer.dollyNearDis = Info.dollyNearDis;
 		Flyer.dollySpeed   = Info.dollySpeed;
 		Flyer.liftAcc      = Info.liftAcc;
 		Flyer.liftSpeed    = Info.liftSpeed;
 		Flyer.lowerAngle   = Info.lowerAngle;
 		Flyer.panAcc       = Info.panAcc;
 		Flyer.panSpeed     = Info.panSpeed;
 		Flyer.rotateAcc    = Info.rotateAcc;
 		Flyer.rotateSpeed  = Info.rotateSpeed;
 		Flyer.upperAngle   = Info.upperAngle;

		// д��flyer��Ϣ
		WRITE_VALUE(Flyer.CameraIndex, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.bCollision, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.colDis, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.dollyAcc, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.dollyFarDis, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.dollyNearDis, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.dollySpeed, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.liftAcc, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.lowerAngle, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.panAcc, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.panSpeed, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.rotateAcc, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.rotateSpeed, m_OriSceneBufferFile);
		WRITE_VALUE(Flyer.upperAngle, m_OriSceneBufferFile);
 
 		fwrite(&Flyer, sizeof(Flyer), 1, m_OriSceneBufferFile);
 		if (CurrentController == (DWORD)(pController))
 		{
 			CurrentIdx = i;
 		}
 	}
 
 	// ��ǰ�����������ͺ�����
 	N3D_ControlData ControlData;
 	ControlData.ControlType    = (int)(m_pVGSSceneMgr->GetCurrentController()->GetControlType());
 	ControlData.ControlerIndex = CurrentIdx;
 	fwrite(&(ControlData.ControlType), sizeof(ControlData.ControlType), 1, m_OriSceneBufferFile);
 	fwrite(&(ControlData.ControlerIndex), sizeof(ControlData.ControlerIndex), 1, m_OriSceneBufferFile);
}

VOID CSaveN3D::WriteFlashData()
{
// 	if (!m_pVGSSceneMgr)
// 	{
// 		throw Ogre::Exception::exception("��ȡ��������");
// 	}
// 	if (!m_OriSceneBufferFile)
// 	{
// 		throw Ogre::Exception::exception("��ȡ�ļ�����");
// 	}
// 
 	m_Table.FlashPtr = ftell(m_OriSceneBufferFile); // ��¼flash�������ļ��Ŀ�ʼλ��
 
 	UINT i = 0;
 	fwrite(&i, sizeof(UINT), 1, m_OriSceneBufferFile); // û�����ݣ�д��0
}

VOID CSaveN3D::WritePrivacyData()
{
 	if (!m_pVGSSceneMgr)
 	{
 		throw exception("��ȡ��������");
 	}
 	if (!m_OriSceneBufferFile)
 	{
 		throw exception("��ȡ�ļ�����");
 	}
 
 	m_Table.PrivacyDataPtr = ftell(m_OriSceneBufferFile); // ��¼��Ȩ�������ļ��Ŀ�ʼλ��
 
 	UINT i = 0;
 	fwrite(&i, sizeof(UINT), 1, m_OriSceneBufferFile); // û�����ݣ�д��0
}

//VOID CSaveN3D::WriteAnimation()
//{
//	if (!m_pVGSSceneMgr)
//	{
//		throw Ogre::Exception::exception("��ȡ��������");
//	}
//	if (!m_OriSceneBufferFile)
//	{
//		throw Ogre::Exception::exception("��ȡ�ļ�����");
//	}
//
//	m_Table.AnimationPtr = ftell(m_OriSceneBufferFile); // ��¼�����������ļ��Ŀ�ʼλ��
//
//	UINT i = 0;
//	fwrite(&i, sizeof(UINT), 1, m_OriSceneBufferFile); // û�����ݣ�д��0
//}


//----------------------------------------------------------------------------------------
//add by NickLi, start here, 2008-05-22
//----------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
//д�볡���ṹͼ�� V3D�ļ�
//------------------------------------------------------------------------------------------
VOID CSaveN3D::WriteSceneGraph()
{	
 	if (!m_pVGSSceneMgr)
 	{
 		throw exception("��ȡ��������");
 	}
 	if (!m_OriSceneBufferFile)
 	{
 		throw exception("��ȡ�ļ�����");
 	}
 
 	//����ͼ���ݵ�ƫ����
 	m_Table.sceneGraphDataPtr = ftell(m_OriSceneBufferFile); // ��¼�����������ļ��Ŀ�ʼλ��
 
 	//�ݹ�д�볡��ͼ�ṹ
 	CSceneNode* pRootNode = m_pVGSSceneMgr->GetRootSceneNode();
 	ConstructSceneGraph(pRootNode);

}

//------------------------------------------------------------------------------------------
//�ݹ��ó���ͼ���ݲ�д��V3D�ļ�
//------------------------------------------------------------------------------------------
VOID CSaveN3D::ConstructSceneGraph(CSceneNode* pNode)
{
	//���Node������
 	N3D_Node N3DNode;
 	GetNodeTypeAndIndex(pNode, &N3DNode);
 
 	// Ԥ��������ڵ㲻��Ҫ����
 	if (N3DNode.index == -1 && N3DNode.type != NNT_SCENEROOT)
 	{
 		return;
 	}
 
 	//д��node������
 	fwrite(&(N3DNode.type), sizeof(N3DNode.type), 1, m_OriSceneBufferFile);
 	
 	//д��nodem_pFile
 	fwrite(&(N3DNode.index), sizeof(N3DNode.index), 1, m_OriSceneBufferFile);
 
 	// ����ڵ������,node������ڵ����������
	WRITE_STR(pNode->name, m_OriSceneBufferFile);
 
 	//Add by baojiahui ------------------------------------------------------------------------------------------------------------
 	//д�����ݿ���չID
 	DWORD ID1 = 0;
    fwrite(&ID1, sizeof(ID1), 1, m_OriSceneBufferFile);
 	fwrite(&ID1, sizeof(ID1), 1, m_OriSceneBufferFile);
     //------------------------------------------------------------------------------------------------------------------------------
 
 	//д��node�ı任
 
 	//λ��
	WRITE_VECTOR3(pNode->getPosition(VTS_PARENT), m_OriSceneBufferFile); 

	// ��ת
	D3DXQUATERNION quat = pNode->getRotition(VTS_PARENT);
	D3DXQuaternionNormalize(&quat, &quat);
	fwrite(&quat.x, sizeof(quat.x), 1, m_OriSceneBufferFile);
	fwrite(&quat.y, sizeof(quat.y), 1, m_OriSceneBufferFile);
	fwrite(&quat.z, sizeof(quat.z), 1, m_OriSceneBufferFile);
	fwrite(&quat.w, sizeof(quat.w), 1, m_OriSceneBufferFile);
	// WRITE_VECTOR3(pNode->curTransform.rotation, m_OriSceneBufferFile);

	// ����
	D3DXVECTOR3 v3Scale = pNode->getScale(VTS_PARENT);
	WRITE_VECTOR3(v3Scale, m_OriSceneBufferFile);

	// node����
	CNodeAnimation *pNodeAnim = m_pRoot->GetAnimManager().getKeyFrameByName(m_pRoot->GetAnimManager().GetNodeAnimationName(pNode));
	if (!pNodeAnim)
	{
		UINT frameCount = 0;
		WRITE_VALUE(frameCount, m_OriSceneBufferFile); // д��ؼ�֡��Ŀ
	}
	else
	{
		UINT frameCount = pNodeAnim->getFrameCount();
		WRITE_VALUE(frameCount, m_OriSceneBufferFile); // д��ؼ�֡��Ŀ

		for (UINT iframe = 0; iframe < frameCount; ++iframe)
		{
			//д��Node�Ĺؼ�֡��������
			N3D_NodeTransform n3dNodeTransform;
			CNodeKeyFrame *pNodeKeyFrame = pNodeAnim->getFrameData(iframe);
			if (pNodeKeyFrame)
			{
				n3dNodeTransform.position = vector3(pNodeKeyFrame->m_pos.x, pNodeKeyFrame->m_pos.y, pNodeKeyFrame->m_pos.z);
				n3dNodeTransform.quat     = quaternion(pNodeKeyFrame->m_quat.x, pNodeKeyFrame->m_quat.y, pNodeKeyFrame->m_quat.z, pNodeKeyFrame->m_quat.w);
				n3dNodeTransform.scale    = vector3(pNodeKeyFrame->m_scale.x, pNodeKeyFrame->m_scale.y, pNodeKeyFrame->m_scale.z);
				n3dNodeTransform.time     = 0.0f;
			}

			WRITE_VALUE(n3dNodeTransform, m_OriSceneBufferFile); // д��ؼ�֡����

			// д�������ƹ��������
			if (pNodeKeyFrame->getType() == VAT_ANIMATIONTYPE_CAMERA)
			{
				CCameraKeyFrame *pCamAnim = (CCameraKeyFrame*)pNodeKeyFrame;
				bool bHasParamAnimation = pCamAnim->m_bHasParamAnimation;
				WRITE_VALUE(bHasParamAnimation, m_OriSceneBufferFile); // д��ؼ�֡����
				if (pCamAnim && pCamAnim->m_bHasParamAnimation)
				{
					float fov      = pCamAnim->getFov();
					float nearClip = pCamAnim->getNearClip();
					float farClip  = pCamAnim->getFarClip();
					float roll     = pCamAnim->getRoll();
					WRITE_VALUE(fov, m_OriSceneBufferFile);
					WRITE_VALUE(nearClip, m_OriSceneBufferFile);
					WRITE_VALUE(farClip, m_OriSceneBufferFile);
					WRITE_VALUE(roll, m_OriSceneBufferFile);
				}
			}
			else if (pNodeKeyFrame->getType() == VAT_ANIMATIONTYPE_LIGHT)
			{
				CLightKeyFrame *pLightAnim = (CLightKeyFrame*)pNodeKeyFrame;
				bool bHasParamAnimation = pLightAnim->m_bHasParamAnimation;
				WRITE_VALUE(bHasParamAnimation, m_OriSceneBufferFile); // д��ؼ�֡����
				if (pLightAnim && pLightAnim->m_bHasParamAnimation && pLightAnim->m_pLight)
				{
					D3DCOLORVALUE colorvalue = pLightAnim->getColor();
					DWORD color = D3DCOLOR_COLORVALUE(colorvalue.r, colorvalue.g, colorvalue.b, colorvalue.a);
					float range = pLightAnim->getRange();
					WRITE_VALUE(color, m_OriSceneBufferFile);
					WRITE_VALUE(range, m_OriSceneBufferFile);

					if (pLightAnim->m_pLight->getType() == VLT_DIR)
					{
						VECTOR3 v3Dir = pLightAnim->getDirection();
						WRITE_VECTOR3(v3Dir, m_OriSceneBufferFile); 
					}
					else if (pLightAnim->m_pLight->getType() == VLT_SPOT)
					{
						VECTOR3 v3Dir = pLightAnim->getDirection();
						WRITE_VECTOR3(v3Dir, m_OriSceneBufferFile);

						float theta, phi, fallof;
						pLightAnim->getAngle(theta, phi, fallof);
						WRITE_VALUE(theta, m_OriSceneBufferFile);
						WRITE_VALUE(phi, m_OriSceneBufferFile);
						WRITE_VALUE(fallof, m_OriSceneBufferFile);
					}
				}
			}
		}
	}

 	//д������������
 	UINT childCnt = GetChildNodeCount(pNode);
 	fwrite(&childCnt, sizeof(childCnt), 1, m_OriSceneBufferFile);
 
 	//���������
 	for (UINT iChild = 0; iChild < pNode->m_child.size(); ++iChild)
 	{
 		CSceneNode* pChildNode = pNode->m_child[iChild];
 		ConstructSceneGraph(pChildNode);
 	}
}

//���һ���ڵ���ӽڵ�����
UINT CSaveN3D::GetChildNodeCount(CSceneNode* pNode)
{
	if (!pNode || pNode->m_child.size() == 0)
	{
		return 0;
	}

	UINT count = 0;
	UINT childCount = pNode->m_child.size();
 	for (UINT iChild = 0; iChild < childCount; ++iChild)
 	{
 		CSceneNode *pChildNode = pNode->m_child[iChild];
 
 		if (!pChildNode)
 		{
 			continue;
 		}
 		if (pChildNode->getMovableObject() == 0 && pChildNode->m_child.size() == 0)
 		{
 			continue;
 		}
 		
 		// ����һЩ����Ҫ����Ľڵ�
 		if (pChildNode && pChildNode->getMovableObject() != 0)
 		{
 			CMovableObject *pAttachObj = pChildNode->getMovableObject();
 			if (pAttachObj)
 			{
 				if ( (pAttachObj->getType() == VGS_MOVABLETYPE_CAMERA) && _GetIndexInVector(m_vpCamera, (CCamera*)pAttachObj) == -1)
 				{
 					continue;
 				}
 			}
 		}
 		++count;
 	}
	return count;
}

// ���ݶ���ָ�뷵��Node����
void CSaveN3D::GetNodeTypeAndIndex(CSceneNode* pSceneNode, N3D_Node* pN3DNode)
{	
	// �������͵�����
 	if (pSceneNode->m_parent == NULL)
 	{
 		pN3DNode->type  = NNT_SCENEROOT;
 		pN3DNode->index = -1;
 		return;
 	}
 
 	if (pSceneNode->getMovableObject() == NULL)
 	{
 		pN3DNode->type  = NNT_MODEL;  // ֻҪ����SCENEROOT����
 		pN3DNode->index = -1;
 		return;
 	}
 	
 	CMovableObject* pAttachObj = pSceneNode->getMovableObject(); 
 	if (VGS_MOVABLETYPE_CAMERA == pAttachObj->getType())
 	{
 		pN3DNode->type  = NNT_CAMERANODE;
 		pN3DNode->index = _GetIndexInVector(m_vpCamera, (CCamera*)pAttachObj);
 	}
 	else if (VGS_MOVABLETYPE_LIGHT == pAttachObj->getType())
 	{
 		pN3DNode->type = NNT_LIGHTNODE;
 		pN3DNode->index = m_pVGSSceneMgr->GetLightIndex((CLight*)pAttachObj);
 	}
 	else if (VGS_MOVABLETYPE_MODEL == pAttachObj->getType())
 	{
 		pN3DNode->type = NNT_MODEL;		
 		pN3DNode->index = m_pVGSSceneMgr->GetModelIndex((CModel*)pAttachObj);
 	}
	else
	{
 		pN3DNode->type  = NNT_MODEL;  // ֻҪ����SCENEROOT����
 		pN3DNode->index = -1;
 	}
}

// ��ʼ��Image�б�
void CSaveN3D::InitImageList()
{
 	// ��ȡ������ʹ�õ�������ͼ�б�
	m_pVGSSceneMgr->getImageNameList(true, m_vImageList);
}

// д��ͼ������
VOID CSaveN3D::WriteImageData()
{
 	if (!m_pVGSSceneMgr)
 	{
 		throw exception("��ȡ��������");
 	}
 	if (!m_pFile)
 	{
 		throw exception("��ȡ�ļ�����");
 	}

 	// Image���ݵ�ƫ����
 	m_Header.baseHeader.ImageDataPtr = ftell(m_pFile); // ��¼Image�������ļ��Ŀ�ʼλ��
 
 	// ��ȡ������ʹ�õ�������ͼ�б�
 	DWORD count = m_vImageList.size(); 
	WRITE_VALUE(count, m_pFile);

 	for (DWORD idx = 0; idx < count; ++idx)
 	{		
		//�����ͼƬ���ļ��е���ʼλ��
		m_Header.pImageIndexData[idx] = ftell(m_pFile);

		// ��ͼƬ�ļ�
 		N3D_ImageInfo ImageInfo;
 		ZeroMemory(&ImageInfo, sizeof(ImageInfo));
 		ImageInfo.Data = NULL;
		FILE *pFile = fopen(m_vImageList[idx].c_str(), "rb");
		if (pFile)
		{
			fseek(pFile, 0, SEEK_END);
			UINT size = ftell(pFile);
			ImageInfo.DataLen = size;
			ImageInfo.Data = new char[size];
			fseek(pFile, 0, SEEK_SET);
			fread(ImageInfo.Data, size, 1, pFile);
			fclose(pFile);
		} 
 
 		// д��һ��Image����Ϣ
		std::string sFileName = _GetPureFileNameStr(m_vImageList[idx]);
 		WRITE_STR(sFileName, m_pFile);
		WRITE_VALUE(ImageInfo.DataLen, m_pFile);
 		if (ImageInfo.DataLen > 0 && ImageInfo.Data)
 		{
			WRITE_PTR(ImageInfo.Data, ImageInfo.DataLen, m_pFile);
 			delete [] ImageInfo.Data;
 		}
 		else
 		{
 			std::string str = "��ͼ�ļ� " + m_vImageList[idx] + " û���ҵ���";
 			MessageBox(NULL, (str.c_str()), TEXT("����"), 0);
 			continue;
 		}
 	} // for
 
 	m_Header.baseHeader.ImageDataLen = ftell(m_pFile) - m_Header.baseHeader.ImageDataPtr;  // ͼ�����ݵĳ���
}

// �Ӳ��ʶ����в��Ҳ��ʵı�ţ��Ҳ�������-1
int CSaveN3D::FindMaterial(DWORD pMaterial)
{
	return _GetIndexInVector(m_vMaterialList, pMaterial);
 	/*for (int idx = 0; idx < m_vMaterialList.size(); ++idx)
 	{
 		if (pMaterial == m_vMaterialList[idx])
 		{
 			return idx;
 		}
 	}
	return -1;*/
}


//-------------------------------------------------------------------
//����: GetN3DTexOP()
//����: 
//��;: ��D3D�Ļ�ɫģʽ����ת����N3D�Ļ�ɫģʽ����
//-------------------------------------------------------------------
N3D_BlenderOP CSaveN3D::GetN3DTexOP(const D3DTEXTUREOP& d3d_BlenderOP)
{	
	switch(d3d_BlenderOP)
	{
	case D3DTOP_DISABLE:
		return NBM_DISABLE;
		break;
	case D3DTOP_SELECTARG1:
		return NBM_REPLACE;
		break;
	case D3DTOP_MODULATE:
		return NBM_MODULATE;
		break;
	case D3DTOP_MODULATE2X:
		return NBM_MODULATE2;
		break;
	case D3DTOP_MODULATE4X:
		return NBM_MODULATE4;
		break;
	case D3DTOP_ADD:
		return NBM_ADD;
		break;
	case D3DTOP_ADDSIGNED:
		return NBM_ADD_SIGNED;
		break;
	case D3DTOP_ADDSMOOTH:
		return NBM_ADD_SMOOTH;
		break;
	case D3DTOP_SUBTRACT:
		return NBM_SUBTRACT;
		break;
	case D3DTOP_BLENDDIFFUSEALPHA:
		return NBM_BLEND_DIFFUSE_ALPHA;
		break;
	case D3DTOP_BLENDTEXTUREALPHA:
		return NBM_BLEND_TEXTURE_ALPHA;
		break;
	case D3DTOP_BLENDCURRENTALPHA:
		return  NBM_BLEND_CURRENT_ALPHA;
		break;
	case D3DTOP_BLENDFACTORALPHA:
		return NBM_BLEND_MANUAL;
		break;
	case D3DTOP_DOTPRODUCT3:
		return NBM_DOTPRODUCT;
		break;
	/*case D3DTOP_SELECTARG1:
		return NBM_BLEND_DIFFUSE_COLOUR;
		break;*/
	}

	return NBM_DISABLE;
}