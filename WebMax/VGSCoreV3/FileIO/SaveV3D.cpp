
#include "SaveV3D.h"
#include "..\VGSSupport.h"
#include "..\VGSErrCode.h"
#include "..\CSysFunc.h"
#include "..\Root.h"

// 写一个值到文件
#define WRITE_VALUE(value) {fwrite(&(value), sizeof(value), 1, m_pFile);}
#define WRITE_VALUE_PTR(ptr, len) {fwrite(ptr, len, 1, m_pFile);}

// 往文件中写入一个vector3
#define WRITE_VECTOR3(V3, pFile) {fwrite(&(V3.x), sizeof(V3.x), 1, m_pFile); fwrite(&(V3.y), sizeof(V3.y), 1, m_pFile); fwrite(&(V3.z), sizeof(V3.z), 1, m_pFile); }

#define RETURN(RE)                       {if (RE != ERR_COMMON_OK)return RE;} 

CSaveV3D::CSaveV3D()
{
	m_pVGSSceneMgr = NULL;
}
CSaveV3D::~CSaveV3D()
{	
	m_pVGSSceneMgr = NULL;
}
VOID CSaveV3D::Release()
{
	m_pVGSSceneMgr = NULL;
}

DWORD CSaveV3D::SaveSceneAsV3D(const CScene* pVGSSceneMgr, const std::string &strFileName, HWND hWnd, BOOL zipStatus /*= false*/)
{
	try
	{
		m_hWnd = hWnd;

		if (pVGSSceneMgr == NULL)
		{
			//::MessageBox(NULL, LPCSTR("找不到场景"), LPCSTR("错误"), MB_OK | MB_ICONERROR);
			//return E_FAIL;
			return ERR_FILE_SAVEV3D_NODATA;
		}
		m_pVGSSceneMgr = (CScene*)pVGSSceneMgr;

		SetRoot(m_pVGSSceneMgr->GetRoot());

		// 创建文件
		if (NULL == (m_pFile = fopen(strFileName.c_str(), "wb")) )
		{
			/*::MessageBox(NULL, LPCSTR("访问文件失败"), LPCSTR("错误"), MB_OK | MB_ICONERROR);
			return E_FAIL;*/
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		//-------------------------------------------------------
		//  **** 注意：
		//  1. 如果某种类型的数据没有，就写入一个四字节的0
		//  2. 写入文件的数据顺序不能变
		//-------------------------------------------------------
		UINT totalSize = 19;
		UINT curSize = 0;
		//write the header data
		DWORD result = WriteHeader();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		result = WriteFileTable();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		//write the window prop
		result = WriteWindowProp();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度
		
		//write privacy data
		result = WritePrivacyData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		//write the  camera data
		result = WriteCameraData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		//write the light data
		result = WriteLightData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		// write texture
		result = WriteTextureData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		//write the Material data
		result = WriteMaterialData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		result = WriteModelData(); // 模型信息数据
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度
		
		//result = WriteMeshData();	// 模型资源数据
		//RETURN(result);

		//write the Text data
		result = WriteTextData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		//write the overlay data
		result = WriteOverlayData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		//write the navigater data
		result = WriteNavigaterData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		//write the button data
		result = WriteButtonData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		// 写背景图
		result = WriteBackdropData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		//write AudioData
		result = WriteAudioData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		//write FlashData
		result = WriteFlashData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		// 动画
		result = WriteAnimation();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		// 控制
		result = WriteControlData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		// 场景图, 包括关键帧动画
		result = WriteSceneGraph();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		fseek(m_pFile, sizeof(m_Header), SEEK_SET);
		result = WriteFileTable();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curSize++, totalSize);  // 开始进度

		//close the pFile
		if (0 != fclose(m_pFile))
		{
			return ERR_FILE_SAVEV3D_CLOSEFILE;	
		}

		return ERR_COMMON_OK;
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_FAIL;
	}
}

DWORD CSaveV3D::WriteHeader()
{
	try
	{
		//init _E3DHeader
		strcpy(m_Header.corp, SUNTOWARD_NAME);
		m_Header.v3dVersion = CURRENT_V3DVERSION;  

		//write the Header to pFile
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("写入文件头发生错误，无法写入文件");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		fwrite(&m_Header, sizeof(m_Header), 1, m_pFile); // 写文件头

		ZeroMemory(&m_Table, sizeof(m_Table)); // 把table清空

	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_HEAD;
	}

	return ERR_COMMON_OK;
}

DWORD CSaveV3D::WriteFileTable()
{	
	try
	{
		if (m_pFile)
		{
			fwrite(&m_Table, sizeof(V3D_Table), 1, m_pFile);
		}
		else
		{
			// throw Ogre::Exception::exception("写入数据索引表发生错误，无法写入文件");
			return ERR_FILE_SAVEV3D_FAIL;
		}
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_TABLE;
	}

	return ERR_COMMON_OK;
}

// 
DWORD CSaveV3D::WriteWindowProp()
{	
	try
	{
		if (m_pFile)
		{
			m_Table.SceneproperyPtr = ftell(m_pFile); // 记录WindowProp数据在文件的开始位置
			fwrite(&m_SceneProp, sizeof(m_SceneProp), 1, m_pFile);
		}
		else
		{
			// throw Ogre::Exception::exception("写入场景信息发生错误，无法写入文件");
			return ERR_FILE_SAVEV3D_FAIL;
		}
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_SCENEPROPERTY;
	}

	return ERR_COMMON_OK;
}

// 保存Entity信息
DWORD CSaveV3D::WriteModelData()
{
	if (!m_pFile)
	{
		return ERR_FILE_SAVEV3D_OPENFILE;
	}

	m_vpModel.clear();

	m_Table.meshDataPtr = ftell(m_pFile); // 记录mesh数据在文件的开始位置

	DWORD ModelCount = m_pVGSSceneMgr->GetModelCount();
	fwrite(&ModelCount, sizeof(ModelCount), 1, m_pFile); // Model的数目
	m_vpModel.resize(ModelCount);

	for (DWORD i = 0; i < ModelCount; ++i)
	{
		CModel *pModel = m_pVGSSceneMgr->GetModelByIndex(i);

		m_vpModel[i] = pModel;  // 保存写入的entity列表

		WriteAModel(pModel);
	}

	return ERR_COMMON_OK;

}

DWORD CSaveV3D::WriteAModel(CModel *pModel)
{
	if (pModel && m_pFile)
	{
		// // Mesh跟entity在ogre中的名字相同，entity另外还有一个vgs名字，用户可以修改VGS名字。
		string sVGSName = pModel->getName();
		UINT len = sVGSName.length(); 
		WRITE_VALUE(len, m_pFile);          // mesh name string length
		WRITE_PTR(sVGSName.c_str(), len, m_pFile);            // mesh name

		DWORD fvf = pModel->m_FVF;
		WRITE_VALUE(fvf, m_pFile);  // 模型的顶点格式

		//写入顶点个数
		UINT vertexCount = pModel->m_vertexCount;  // mesh所用到的顶点数目
		WRITE_VALUE(vertexCount, m_pFile);

		// mesh 的顶点信息
		void *pVertextBuffer = NULL;
		pModel->LockVertexBuffer(&pVertextBuffer);	
		WRITE_PTR(pVertextBuffer, vertexCount * pModel->m_FVFSize, m_pFile);
		pModel->UnlockVertexBuffer();

		// 写入面个数
		UINT faceCount = pModel->m_faceCount;  // mesh所用到的面数目
		WRITE_VALUE(faceCount, m_pFile);

		// index buffer的长度
		//int indexSize = pModel->m_faceCount * 3 * sizeof(unsigned short);
		//if (pModel->m_vertexCount > 65535)
		//{
		//	indexSize = pModel->m_faceCount * 3 * sizeof(UINT);
		//} 
		UINT indexSize = pModel->m_faceCount * 3 * sizeof(UINT); 

		// 写入index buffer数据
		void *pIndexBuffer = NULL; 
		pModel->LockIndexBuffer(&pIndexBuffer);
		WRITE_PTR(pIndexBuffer, indexSize, m_pFile);
		pModel->UnlockIndexBuffer();

		// submesh 数目
		unsigned short SubMeshCount = pModel->getSubModelCount(); // ptrMesh->getNumSubMeshes();
		fwrite(&SubMeshCount, sizeof(SubMeshCount), 1, m_pFile); // 

		// 记录每个submesh的信息
		for (UINT iSubMesh = 0; iSubMesh < SubMeshCount; ++iSubMesh)
		{
			// submesh使用的材质索引
			int index = _GetIndexInVector(m_vMaterialList, pModel->getSubModelMaterial(iSubMesh));
			WRITE_VALUE(index, m_pFile);

			// 每个字模型的顶点序列、索引序列
			CSubModel* pSubModel = pModel->getSubModel(iSubMesh);
			WRITE_VALUE(pSubModel->m_vertexStart, m_pFile);
			WRITE_VALUE(pSubModel->m_vertexCount, m_pFile);
			WRITE_VALUE(pSubModel->m_faceStart, m_pFile);
			WRITE_VALUE(pSubModel->m_faceCount, m_pFile);

		} // if

		// 模型的属性信息
		bool bBillBoard = (pModel->isBillboard == TRUE);
		fwrite(&bBillBoard, sizeof(bBillBoard), 1, m_pFile);
		if (bBillBoard)
		{
			BYTE axis ;//= (BYTE)pModel->billboardDirectionAxis;
			fwrite(&axis, sizeof(axis), 1, m_pFile);  // 正对相机的坐标轴
		}
		bool bCollision = (pModel->isCollision == TRUE);
		fwrite(&bCollision, sizeof(bCollision), 1, m_pFile); 
		bool bGround = (pModel->isGround == TRUE);
		fwrite(&bGround, sizeof(bGround), 1, m_pFile);
		bool bVisible = (pModel->visible == TRUE);
		fwrite(&bVisible, sizeof(bVisible), 1, m_pFile);
		bool bClickable = (pModel->m_isClickable == TRUE);
		fwrite(&bClickable, sizeof(bClickable), 1, m_pFile);
		BYTE upAxis = 0; //(BYTE)pModel->billboradUpAxis;
		fwrite(&upAxis, sizeof(upAxis), 1, m_pFile);
		bool bIsAutoNormalise = (pModel->isAutoNormalise == TRUE);
		fwrite(&bIsAutoNormalise, sizeof(bIsAutoNormalise), 1, m_pFile);
		bool bSkybox = (pModel->isSkyBox == TRUE);
		fwrite(&bSkybox, sizeof(bSkybox), 1, m_pFile); 

		// 骨骼信息
		int bindingSkeletonIndex  = -1;
		fwrite(&bindingSkeletonIndex, sizeof(bindingSkeletonIndex), 1, m_pFile);

	} // for

	return ERR_COMMON_OK;
}

// 写入三维对象使用的材质，不包括二维对象使用的材质
DWORD CSaveV3D::WriteMaterialData()
{
	try
	{
		m_vMaterialList.clear();

		if (!m_pFile)
		{
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		m_Table.mtrlDataPtr = ftell(m_pFile); // 记录material数据在文件的开始位置
		
		UINT mtrCount = m_pRoot->GetMaterialManager().getResourceCount();
		fwrite(&mtrCount, sizeof(mtrCount), 1, m_pFile); // material的数目

		for (UINT i = 0; i < mtrCount; ++i)
		{
			CMaterial *pMaterial = (CMaterial*)m_pRoot->GetMaterialManager().getResource(i);
			WriteAMaterial(pMaterial);
			m_vMaterialList.push_back(pMaterial);
		}
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_HEAD;
	}

	return ERR_COMMON_OK;
	
}

// 保存一个材质对象
void CSaveV3D::WriteAMaterial(CMaterial* pMaterial)
 {
	if (!pMaterial)
	{
		return;
	}
	std::string mtrName = pMaterial->getName();
	UINT len = mtrName.length();
	bool bRecShadow = false;

	fwrite(&len, sizeof(len), 1, m_pFile);   
	fwrite(mtrName.c_str(), len, 1, m_pFile);               // material的名字

	V3D_PassDetail_Ex PassDetail;
	PassDetail.ShadingMode = (BYTE)(pMaterial->type);		// _D3DSHADEMODE 1 : D3DSHADE_FLAT, 2: D3DSHADE_GOURAUD, 3: D3DSHADE_PHONG 
  	
	D3DCOLORVALUE color = pMaterial->d3dmtrl.Diffuse;
	PassDetail.Dif_ARGB = D3DCOLOR_ARGB(DWORD(color.a*255.f), DWORD(color.r*255.f), DWORD(color.g*255.f), DWORD(color.b*255.f));
	color = pMaterial->d3dmtrl.Ambient;
	PassDetail.Amb_ARGB = D3DCOLOR_ARGB(DWORD(color.a*255.f), DWORD(color.r*255.f), DWORD(color.g*255.f), DWORD(color.b*255.f));   	

	color = pMaterial->specular;            // 高光颜色，等于真正的颜色值 / level
	PassDetail.Spe_ARGB = D3DCOLOR_ARGB(DWORD(color.a*255.f), DWORD(color.r*255.f), DWORD(color.g*255.f), DWORD(color.b*255.f));      // 保存设置的specular，而不是最终值
	PassDetail.Spe_Level = pMaterial->specularLevel;

	PassDetail.shininess = (float)(pMaterial->d3dmtrl.Power);
	color = pMaterial->d3dmtrl.Emissive;
	PassDetail.sel_ARGB = D3DCOLOR_ARGB(DWORD(color.a*255.f), DWORD(color.r*255.f), DWORD(color.g*255.f), DWORD(color.b*255.f));

	PassDetail.selfIllumination = pMaterial->selfIllumination;	//INT(pMaterial->d3dmtrl.Emissive.r / pMaterial->d3dmtrl.Diffuse.r * 100);

	PassDetail.bAlphaBlend = (bool)pMaterial->bAlphaBlend;
	PassDetail.bAlphaTest = (bool)pMaterial->bAlphaTest;
	PassDetail.AlphaTestValue = pMaterial->AlpahaTestValue;
	PassDetail.Opacity = pMaterial->getOpacity();
	PassDetail.bLighting   = 0;
	PassDetail.bTwoside    = pMaterial->twoSide;				// 是否双面渲染
	PassDetail.fillMode    = pMaterial->fillMode;				// 0:point 1:wireframe, 2:slod
	PassDetail.bUseDiffuse = (bool)pMaterial->m_bUseDiffuse;	// fuxb 2008-10-28, 是否使用材质的颜色

	fwrite(&PassDetail, sizeof(V3D_PassDetail_Ex), 1, m_pFile); // pass详细参数

	UINT texLayerCount = pMaterial->getTexureLayerCount();		// 纹理贴图的数目
	fwrite(&texLayerCount, sizeof(texLayerCount), 1, m_pFile);
	for (UINT iLayer = 0; iLayer < texLayerCount; ++iLayer)
	{
		CTextureLayer* pLayer = pMaterial->getTextureLayerByIndex(iLayer);
		std::string sLayerName = std::string(pLayer->m_sName);
		UINT iLayerNameLen = strlen(sLayerName.c_str());
		UINT UVIndex = pLayer->UVIndex;		
		BYTE mipMap  = pLayer->mipMap;	
		FLOAT USpeed = pLayer->USpeed;
		FLOAT VSpeed = pLayer->VSpeed;

		fwrite(&iLayerNameLen, sizeof(iLayerNameLen), 1, m_pFile);       // length of unit mame
		fwrite(sLayerName.c_str(), iLayerNameLen, 1, m_pFile);           // unit mame
		fwrite(&UVIndex, sizeof(UVIndex), 1, m_pFile);                   // 坐标序号
		fwrite(&mipMap, sizeof(mipMap), 1, m_pFile);					 // mipMap	
		fwrite(&USpeed, sizeof(USpeed), 1, m_pFile);
		fwrite(&VSpeed, sizeof(VSpeed), 1, m_pFile);

		// 层类型
		VGS_TextureType texType = pLayer->type;
		fwrite(&texType, sizeof(texType), 1, m_pFile);					// 写入层类型
		
		// 保存图像的帧数，以及每帧的图像名称，不包括路径
		UINT FrameCount = pLayer->getFrameCount();
		WRITE_VALUE(FrameCount)
		for (UINT iFrame = 0; iFrame < FrameCount; ++iFrame)
		{
			std::string sFile = pLayer->getFrameFileName(iFrame);
			std::string sFileName = /*_GetPureFileNameStr*/(sFile);
			UINT len = sFileName.length();
			WRITE_VALUE(len)
			fwrite(sFileName.c_str(), len, 1, m_pFile);
		}

		// 如果是动态贴图，记录播放周期
		if (texType == TLT_DYNAMIC)
		{
			int duration = pLayer->param.DynamicTextureParam.duration;
			fwrite(&duration, sizeof(duration), 1, m_pFile);            
		}
		else if (texType == TLT_REFLECT)
		{
			char type = (char)pLayer->param.ReflectTextureParam51002.reflectionType;
			fwrite(&type, sizeof(type), 1, m_pFile); // 由原来的bool bReflect改成了char type
		}
		else if (texType == TLT_BUMPMAP)
		{
			fwrite(&pLayer->param.BumpwaterTextureParam, sizeof(pLayer->param.BumpwaterTextureParam), 1, m_pFile);
		} // if texType
		
		// 写入颜色的混色模式
		VGS_BlenderInfo blendInfo;
		blendInfo.option = _getVGSBlenderOperation(pLayer->Blenderinfo.ColorBlendOption);
		blendInfo.param.BlenderOP_alpha = (FLOAT)(pLayer->Blenderinfo.ColorBlendOption & 0xff);
		fwrite(&blendInfo, sizeof(blendInfo), 1, m_pFile);

		// 写入alpah通道的混色模式
		blendInfo.option = _getVGSBlenderOperation(pLayer->Blenderinfo.AlphaBlendOption);
		blendInfo.param.BlenderOP_alpha = (FLOAT)(pLayer->Blenderinfo.AlphaBlendParam && 0xff);
		fwrite(&blendInfo, sizeof(blendInfo), 1, m_pFile);	

	} // for iUnit
}

// 纹理贴图
DWORD CSaveV3D::WriteTextureData()
 {
	try
	{
		if (!m_pVGSSceneMgr)
		{
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		m_Table.texturePtr = ftell(m_pFile); // 记录texture数据在文件的开始位置

		UINT count = 0; // m_pVGSSceneMgr->getTextureCount();
		fwrite(&count, sizeof(count), 1, m_pFile); // 写入texture的个数
// 		for (UINT i = 0; i < count; ++i)
// 		{
// 			CTexture *pTexture = m_pVGSSceneMgr->getTextureByIndex(i);
// 			if (pTexture)
// 			{
// 
// 			}
// 		}

	}catch (...)
	{
		return ERR_FILE_SAVEV3D_TEXTURE;
	}

	return ERR_COMMON_OK;

}

// 文字
DWORD CSaveV3D::WriteTextData()
{
	try
	{
		if (!m_pVGSSceneMgr)
		{
			//throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			//throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		m_Table.TextDataPtr = ftell(m_pFile); // 记录camera数据在文件的开始位置

		UINT i = 0;
		fwrite(&i, sizeof(UINT), 1, m_pFile); // 没有数据，写入0

	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_TEXT;
	}

	return ERR_COMMON_OK;

}

// 遍历所有相机，滤掉没有被控制对象使用的预定义相机
void CSaveV3D::FilterCamera()
{
	m_vpCamera.clear();

	if (m_pVGSSceneMgr)
	{
		// 遍历所有相机，滤掉没有被控制对象使用的预定义相机
		UINT count = m_pVGSSceneMgr->getCameraCount();
		for (UINT i = 0; i < count; ++i)
		{
			CCamera *pCamera = m_pVGSSceneMgr->GetCameraByIndex(i);

			// 如果相机不是预定义相机 	
			if (pCamera && !IsPredefinedCamera(pCamera))	
			{
				m_vpCamera.push_back(pCamera);
			}
		}
	}
}

//写入相机数据
DWORD CSaveV3D::WriteCameraData()
{
	try
	{
		if (!m_pVGSSceneMgr)
		{
			// throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		// 记录camera数据在文件的开始位置
		m_Table.cameraDataPtr = ftell(m_pFile); 

		FilterCamera();  // 过滤掉预定义的相机

		//写入相机数量
		UINT CamCount = m_vpCamera.size()/*m_pVGSSceneMgr->m_CamNameList.size()*/;
		fwrite(&CamCount, sizeof(CamCount), 1, m_pFile);

		//逐个相机写入数据
		for (int i = 0; i < CamCount; ++i)
		{
			CCamera *pCamera = m_vpCamera[i];

			VGS_V3D_Camera VGSCam;

			//相机名称
			VGSCam.name = pCamera->getName();
			VGSCam.nameLen = VGSCam.name.length()/* + 1*/;
			fwrite(&VGSCam.nameLen, sizeof(VGSCam.nameLen), 1, m_pFile);
			fwrite((void*)(VGSCam.name.c_str()), VGSCam.nameLen, 1, m_pFile);

			//相机的投影类型
			VGSCam.v3dBaseInfo.isOrtho = (pCamera->m_camType == VCT_ORTHO);

			//写入相机基本信息
			VGSCam.v3dBaseInfo.fov         = pCamera->GetFov();
			VGSCam.v3dBaseInfo.nearClipDis = pCamera->getNearClip();
			VGSCam.v3dBaseInfo.farClipDis  = pCamera->getFarClip();
			D3DXVECTOR3 vPos = pCamera->GetPosition(VTS_PARENT);
			VGSCam.v3dBaseInfo.position = VECTOR3(vPos.x, vPos.y, vPos.z);
			D3DXVECTOR3 vAt = pCamera->GetLookat(VTS_PARENT);
			VGSCam.v3dBaseInfo.at = VECTOR3(vAt.x, vAt.y, vAt.z);
			D3DXVECTOR3 v3Up = pCamera->GetUp(VTS_PARENT);
			VGSCam.v3dBaseInfo.up = VECTOR3(v3Up.x, v3Up.y, v3Up.z);
			//VGSCam.v3dBaseInfo.roll = pCamera->GetRoll();

			fwrite(&VGSCam.v3dBaseInfo, sizeof(VGSCam.v3dBaseInfo), 1, m_pFile);
		}
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_CAMERA;
	}

	return ERR_COMMON_OK;
}


//通过相机对象从相机动画列表中获得相机动画对象
//CameraParamAnimation* CSaveV3D::GetCameraParamAnimation(std::vector<CameraParamAnimation*> list, Ogre::Camera* pCam)
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

//写入灯光数据
/*VOID CSaveV3D::WriteLightData()
{
	if (!m_pVGSSceneMgr)
	{
		throw Ogre::Exception::exception("获取场景错误");
		// return;
	}
	if (!m_pFile)
	{
		throw Ogre::Exception::exception("获取文件错误");
		// return;
	}

	m_Table.lightDataPtr = ftell(m_pFile); // 记录light数据在文件的开始位置


	UINT LightCount = m_pVGSSceneMgr->m_LightNameList.size();
	fwrite(&LightCount, sizeof(LightCount), 1, m_pFile); // 灯光数目
	
	MAP_Name2Name::iterator it = m_pVGSSceneMgr->m_LightNameList.begin();
	for (it; it != m_pVGSSceneMgr->m_LightNameList.end(); ++it)
	{
		String sName   = it->first;   // 灯光的VGS名字
		UINT NameLen = strlen(sName.c_str());

		Light *pLight  = m_pVGSSceneMgr->mSceneMgr->getLight(it->second);

	//LightIter = m_pVGSSceneMgr->mSceneMgr->getMovableObjectIterator(LightFactory::FACTORY_TYPE_NAME);
	//while (LightIter.hasMoreElements())
	//{
	//	String sName   = m_pVGSSceneMgr->GetVGSLightName(LightIter.peekNextKey());
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
		fwrite(sName.c_str(), NameLen, 1, m_pFile);            // 写入灯光名
		fwrite(&LightDetail, sizeof(LightDetail), 1, m_pFile); // 写入灯光相关信息

		// 如果是聚光等，需要保存聚光灯的内、外锥形的角度,falloff
		if (pLight->getType() == Ogre::Light::LT_SPOTLIGHT)  
		{
			float innerAngle = (float)(pLight->getSpotlightInnerAngle().valueRadians());
			float outerAngle = (float)(pLight->getSpotlightOuterAngle().valueRadians());
			float falloff = (float)(pLight->getSpotlightFalloff());
			fwrite(&innerAngle, sizeof(innerAngle), 1, m_pFile);
			fwrite(&outerAngle, sizeof(outerAngle), 1, m_pFile);
			fwrite(&falloff, sizeof(falloff), 1, m_pFile);
		}
		else if (pLight->getType() == Ogre::Light::LT_DIRECTIONAL)  // 如果是方向灯
		{
			// NULL
		}

	}

}*/
DWORD CSaveV3D::WriteLightData()
{
	try
	{
		if (!m_pVGSSceneMgr)
		{
			// throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		m_Table.lightDataPtr = ftell(m_pFile); // 记录light数据在文件的开始位置

		//写入灯光数量
		int LightCount = m_pVGSSceneMgr->getLightCount();
		fwrite(&LightCount, sizeof(LightCount), 1, m_pFile);

		m_vpLight.clear();  // 写入的灯光列表
		
		//逐个灯光写入数据
		for (INT i = 0; i < LightCount; ++i)
		{
			CLight *pLight = m_pVGSSceneMgr->GetLightByIndex(i);
			//写入灯光的VGS名字
			std::string sName   = pLight->getName();   
			int NameLen = sName.length() + 1;
			fwrite(&NameLen, sizeof(NameLen), 1, m_pFile); 
			fwrite(sName.c_str(), NameLen, 1, m_pFile);        
			
			VGS_V3D_Light vgsLight;
			
			//写入灯光类型
			VGS_LIGHT_TYPE type = pLight->getType();
			fwrite(&type, sizeof(type), 1, m_pFile);

			// 获取灯光的m_Multiply
			float lightMultiply = pLight->multiply;

			//根据不同的灯光类型，写入不同的参数
			switch(type)
			{
			case VLT_OMNI:
				{
					VGS_V3D_OMNI omni;
					
					omni.multiply = lightMultiply;
					if (lightMultiply > 0)
					{
						omni.color = D3DCOLOR_COLORVALUE(pLight->diffuse.r / lightMultiply, pLight->diffuse.g / lightMultiply, pLight->diffuse.b / lightMultiply, pLight->diffuse.a / lightMultiply);
					}
					else
					{
						omni.color = D3DCOLOR_COLORVALUE(pLight->diffuse.r, pLight->diffuse.g, pLight->diffuse.b, pLight->diffuse.a);
					}
					omni.attenuationRange = pLight->param.Range;
					omni.attenuation0 = pLight->param.Attenuation0;
					omni.attenuation1 = pLight->param.Attenuation1;
					omni.attenuation2 = pLight->param.Attenuation2;

					D3DXVECTOR3 *vPos = pLight->getPosition();  // 相对位置
					omni.position = VECTOR3(vPos->x, vPos->y, vPos->z);

					//写入点光源信息
					fwrite(&omni, sizeof(omni), 1, m_pFile);
				}
				break;

			case VLT_DIR:
				{
					VGS_V3D_DIRECTIONAL directionLight;

					directionLight.multiply = lightMultiply;

					if (lightMultiply > 0)
					{
						directionLight.color = D3DCOLOR_COLORVALUE(pLight->diffuse.r / lightMultiply, pLight->diffuse.g / lightMultiply, pLight->diffuse.b / lightMultiply, pLight->diffuse.a / lightMultiply);
					}
					else
					{
						directionLight.color = D3DCOLOR_COLORVALUE(pLight->diffuse.r, pLight->diffuse.g, pLight->diffuse.b, pLight->diffuse.a);
					}
					directionLight.attenuationRange = pLight->param.Range;
					directionLight.attenuation0 = pLight->param.Attenuation0;
					directionLight.attenuation1 = pLight->param.Attenuation1;
					directionLight.attenuation2 = pLight->param.Attenuation2;
					
					directionLight.direction.x = pLight->getDirection()->x;
					directionLight.direction.y = pLight->getDirection()->y;
					directionLight.direction.z = pLight->getDirection()->z;

					//写入方向光信息
					fwrite(&directionLight, sizeof(directionLight), 1, m_pFile);
				}
				break;

			case VLT_SPOT:
				{
					VGS_V3D_SPOTLIGHT spotLight;

					spotLight.multiply = lightMultiply;

					if (lightMultiply > 0)
					{
						spotLight.color = D3DCOLOR_COLORVALUE(pLight->diffuse.r / lightMultiply, pLight->diffuse.g / lightMultiply, pLight->diffuse.b / lightMultiply, pLight->diffuse.a / lightMultiply);
					}
					else
					{
						spotLight.color = D3DCOLOR_COLORVALUE(pLight->diffuse.r, pLight->diffuse.g, pLight->diffuse.b, pLight->diffuse.a);
					}
					spotLight.attenuationRange = pLight->param.Range;
					spotLight.attenuation0 = pLight->param.Attenuation0;
					spotLight.attenuation1 = pLight->param.Attenuation1;
					spotLight.attenuation2 = pLight->param.Attenuation2;
					
					spotLight.direction.x = pLight->getDirection()->x;
					spotLight.direction.y = pLight->getDirection()->y;
					spotLight.direction.z = pLight->getDirection()->z;

					spotLight.innerAngle = pLight->param.Theta;
					spotLight.outerAngle = pLight->param.Phi;
					spotLight.fallOff = pLight->param.Falloff;

					D3DXVECTOR3 *vPos = pLight->getPosition();  // 相对位置
					spotLight.position = VECTOR3(vPos->x, vPos->y, vPos->z);

					//写入聚光灯信息
					fwrite(&spotLight, sizeof(spotLight), 1, m_pFile);
				}
				break;
			}

			m_vpLight.push_back(pLight); // 保存到列表
		} // for
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_LIGHT;
	}

	return ERR_COMMON_OK;

}

//通过灯光对象从灯光动画列表中获得灯光参数动画对象
//LightParamAnimation* CSaveV3D::GetLightParamAnimation(std::vector<LightParamAnimation*> list, Ogre::Light* pLight)
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

// 保存overlay
DWORD CSaveV3D::WriteOverlayData()
{
	try
	{
		if (!m_pVGSSceneMgr)
		{
			// throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		// 记录overlay数据在文件的开始位置
		m_Table.OverlayPtr = ftell(m_pFile); 

		CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

		// Overlay数目
		UINT uOverlayCount = GUIMngr.GetObjectsCount(VGS_GUIOBJ_TYPE_OVERLAY); 
		fwrite(&(uOverlayCount), sizeof(UINT), 1, m_pFile); 

		// Overlay数据
		for (UINT uIndex = 0; uIndex < uOverlayCount; uIndex++)
		{
			V3D_OverlayInfoV3 overlayInfo;
 			ZeroMemory(&overlayInfo, sizeof(V3D_OverlayInfoV3));

			// 获取Overlay对象
			COverlay* pOverlay = (COverlay*)GUIMngr.GetObjectHandlebyIndex(VGS_GUIOBJ_TYPE_OVERLAY, uIndex);			
			if (NULL != pOverlay)			
			{
				// 获取Overlay对象属性
				GUIInfo guiInfo;
 				ZeroMemory(&guiInfo, sizeof(GUIInfo));
				GUIMngr.GetObjectCommonPropety(pOverlay, guiInfo);

				// 将属性转存到结构体
 				strcpy(overlayInfo.Name, guiInfo.sName); 			
				overlayInfo.bRelativePos  = guiInfo.bRelativePos;
				overlayInfo.bRelativeSize = guiInfo.bRelativeSize;
 				overlayInfo.bVisible      = guiInfo.bVisible;
				overlayInfo.Height        = guiInfo.Height;
 				overlayInfo.Left          = guiInfo.Left;
 				overlayInfo.Opacity       = guiInfo.Opacity;
				overlayInfo.Origin        = guiInfo.Origin;
 				overlayInfo.Top           = guiInfo.Top;
 				overlayInfo.Width         = guiInfo.Width;
 				overlayInfo.ZOrder        = guiInfo.ZOrder; 
				//
				CMaterial* pMtrl = pOverlay->GetMaterial();
				overlayInfo.materialIndex =  _GetIndexByHandle(m_vMaterialList, pMtrl);//m_pVGSSceneMgr->getMaterialIndex(pMtrl);
				//
				std::string strFuncName;
				for (int iFunc = 0; iFunc < VGS_MOUSEEVENT_CNT; iFunc++)
				{
					strFuncName = pOverlay->GetEventFuncName(VGS_MOUSEEVENT_TYPE(iFunc));
					strcpy(overlayInfo.sMouseEventFunc[iFunc], strFuncName.c_str()); 
				}
			}
 
 			fwrite(&overlayInfo, sizeof(V3D_OverlayInfoV3), 1, m_pFile);
		}
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_OVERLAY;
	}

	return ERR_COMMON_OK;
}

DWORD CSaveV3D::WriteNavigaterData()
{
	try
	{
		if (!m_pVGSSceneMgr)
		{
			// throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		// 记录导航数据在文件的开始位置
		m_Table.NavigatotPtr = ftell(m_pFile); 

		CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

		// 导航图数目
		UINT uNaviCount = GUIMngr.GetObjectsCount(VGS_GUIOBJ_TYPE_NAVIGATION);
		fwrite(&(uNaviCount), sizeof(UINT), 1, m_pFile); 

		// 每个导航图的数据
		for (UINT uIndex = 0; uIndex < uNaviCount; uIndex++)
		{
			FRect mapRect;
			V3D_NavigatorInfoV3 naviInfo; 
 			ZeroMemory(&mapRect, sizeof(FRect));
 			ZeroMemory(&naviInfo, sizeof(V3D_NavigatorInfoV3));

			// 获取导航图对象
			CNavigator* pNavigator = (CNavigator*)GUIMngr.GetObjectHandlebyIndex(VGS_GUIOBJ_TYPE_NAVIGATION, uIndex);			
			if (NULL != pNavigator)			
			{
				// 获取导航图对象属性
				GUIInfo guiInfo;
 				ZeroMemory(&guiInfo, sizeof(GUIInfo));
				GUIMngr.GetObjectCommonPropety(pNavigator, guiInfo);

				// 将属性转存到结构体
 				strcpy(naviInfo.NaviBaseInfo.Name, guiInfo.sName);
 				naviInfo.NaviBaseInfo.bRelativePos  = guiInfo.bRelativePos;
 				naviInfo.NaviBaseInfo.bRelativeSize = guiInfo.bRelativeSize;
 				naviInfo.NaviBaseInfo.bVisible	    = guiInfo.bVisible;
 				naviInfo.NaviBaseInfo.Height        = guiInfo.Height;
 				naviInfo.NaviBaseInfo.Left          = guiInfo.Left;	 
 				naviInfo.NaviBaseInfo.Opacity       = guiInfo.Opacity;
 				naviInfo.NaviBaseInfo.Origin        = guiInfo.Origin;
 				naviInfo.NaviBaseInfo.Top           = guiInfo.Top;
 				naviInfo.NaviBaseInfo.Width         = guiInfo.Width;
 				naviInfo.NaviBaseInfo.ZOrder        = guiInfo.ZOrder; 
				//
				CMaterial* pMtrl = pNavigator->GetMaterial();
				naviInfo.NaviBaseInfo.MapMaterialIndex = _GetIndexByHandle(m_vMaterialList, pMtrl);//m_pVGSSceneMgr->getMaterialIndex(pMtrl);
				//
				std::string strFuncName;
				for (int iFunc = 0; iFunc < VGS_MOUSEEVENT_CNT; iFunc++)
				{
					strFuncName = pNavigator->GetEventFuncName(VGS_MOUSEEVENT_TYPE(iFunc));
					strcpy(naviInfo.sMouseEventFunc[iFunc], strFuncName.c_str()); 
				}

				// 导航图范围
				pNavigator->GetNavRange(mapRect);

				// 导航点的个数
				naviInfo.PointerCount = pNavigator->GetPointerCount();
				if (naviInfo.PointerCount > 0)
				{
					// 导航点信息
					naviInfo.PointerList = new V3D_NaviPointerInfoV3[naviInfo.PointerCount];
 					ZeroMemory(naviInfo.PointerList, naviInfo.PointerCount*sizeof(V3D_NaviPointerInfoV3));
					for (UINT uPointer = 0; uPointer < naviInfo.PointerCount; uPointer++)
					{					
						CNavPointer* pPointer = pNavigator->GetPointerByIndex(uPointer);
						strcpy(naviInfo.PointerList[uPointer].sName, pPointer->GetName());
						naviInfo.PointerList[uPointer].NavType = pPointer->GetNavType();
						if (0 == naviInfo.PointerList[uPointer].NavType)		// 获取相机的索引
							naviInfo.PointerList[uPointer].PointedObjIndex = _GetIndexInVector(m_vpCamera, (CCamera*)pPointer);
						else if (1 == naviInfo.PointerList[uPointer].NavType)	// 获取模型的索引
							naviInfo.PointerList[uPointer].PointedObjIndex = _GetIndexInVector(m_vpModel, (CModel*)pPointer);
						naviInfo.PointerList[uPointer].bVisible = pPointer->GetVisibility();
						naviInfo.PointerList[uPointer].Opacity = pPointer->GetOpacity();
						pPointer->GetPosition(naviInfo.PointerList[uPointer].Left, naviInfo.PointerList[uPointer].Top);
						pPointer->GetSize(naviInfo.PointerList[uPointer].Width, naviInfo.PointerList[uPointer].Height);
						//
						CMaterial* pMtrl = pPointer->GetMaterial();
						naviInfo.PointerList[uPointer].materialIndex = _GetIndexByHandle(m_vMaterialList, pMtrl);//m_pVGSSceneMgr->getMaterialIndex(pMtrl);	
						//
						std::string strFuncName;
						for (int iFunc = 0; iFunc < VGS_MOUSEEVENT_CNT; iFunc++)
						{
							strFuncName = pPointer->GetEventFuncName(VGS_MOUSEEVENT_TYPE(iFunc));
							strcpy(naviInfo.PointerList[uPointer].sMouseEventFunc[iFunc], strFuncName.c_str());
						}		
					}
				}
			}

			// 写入导航图基本参数
			fwrite(&(naviInfo.NaviBaseInfo), sizeof(naviInfo.NaviBaseInfo), 1, m_pFile);

			// 写入导航图范围
 			fwrite(&mapRect, sizeof(FRect), 1, m_pFile);   

			// 写入导航点的个数
 			fwrite(&naviInfo.PointerCount, sizeof(INT), 1, m_pFile);   

			// 写入导航点信息
			for (UINT uPointer = 0; uPointer < naviInfo.PointerCount; uPointer++)
			{
 				fwrite(&naviInfo.PointerList[uPointer], sizeof(V3D_NaviPointerInfo), 1, m_pFile);  
			}

			// 释放内存
			if (NULL != naviInfo.PointerList)
			{
				delete []naviInfo.PointerList;
				naviInfo.PointerList = NULL;
			}
		}
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_NAVIGATOR;
	}

	return ERR_COMMON_OK;
}

DWORD CSaveV3D::WriteButtonData()
{
	try
	{
		if (!m_pVGSSceneMgr)
		{
			// throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		// 记录Button数据在文件的开始位置
		m_Table.ButtonPtr = ftell(m_pFile); 

		CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

		// Button数目
		UINT uBtnCount = GUIMngr.GetObjectsCount(VGS_GUIOBJ_TYPE_BUTTON);
		fwrite(&(uBtnCount), sizeof(UINT), 1, m_pFile); 

		// 每个导航图的数据
		for (UINT uIndex = 0; uIndex < uBtnCount; uIndex++)
		{
 			V3D_ButtonInfoV3 buttonInfo;
			ZeroMemory(&buttonInfo, sizeof(V3D_ButtonInfoV3));

			CButton* pButton = (CButton*)GUIMngr.GetObjectHandlebyIndex(VGS_GUIOBJ_TYPE_BUTTON, uIndex);
			if (NULL != pButton)
			{
				// 获取导航图对象属性
				GUIInfo guiInfo;
 				ZeroMemory(&guiInfo, sizeof(GUIInfo));
				GUIMngr.GetObjectCommonPropety(pButton, guiInfo);

				// 将属性转存到结构体
				strcpy(buttonInfo.Name, guiInfo.sName);
 				buttonInfo.bRelativeSize = guiInfo.bRelativeSize;
 				buttonInfo.bVisible	    = guiInfo.bVisible;
 				buttonInfo.Height        = guiInfo.Height;
 				buttonInfo.Left          = guiInfo.Left;	 
 				buttonInfo.Opacity       = guiInfo.Opacity;
 				buttonInfo.Origin        = guiInfo.Origin;
 				buttonInfo.Top           = guiInfo.Top;
 				buttonInfo.Width         = guiInfo.Width;
				buttonInfo.ZOrder	     = guiInfo.ZOrder;
				//
				strcpy(buttonInfo.CallbackFunName, pButton->GetJsCallbackFunc());
				//
				CMaterial* pUpMtrl = pButton->GetMaterial(0);
				CMaterial* pOverMtrl = pButton->GetMaterial(1);
				CMaterial* pDownMtrl = pButton->GetMaterial(2);
				buttonInfo.UpMaterialIndex = _GetIndexByHandle(m_vMaterialList, pUpMtrl);//m_pVGSSceneMgr->getMaterialIndex(pUpMtrl);
				buttonInfo.OverMaterialIndex = _GetIndexByHandle(m_vMaterialList, pOverMtrl);//m_pVGSSceneMgr->getMaterialIndex(pOverMtrl);
				buttonInfo.DownMaterialIndex = _GetIndexByHandle(m_vMaterialList, pDownMtrl);//m_pVGSSceneMgr->getMaterialIndex(pDownMtrl);
				//
				std::string strFuncName;
				for (int iFunc = 0; iFunc < VGS_MOUSEEVENT_CNT; iFunc++)
				{
					strFuncName = pButton->GetEventFuncName(VGS_MOUSEEVENT_TYPE(iFunc));
					strcpy(buttonInfo.sMouseEventFunc[iFunc], strFuncName.c_str());
				}		
			}

			// 写入button数据
			fwrite(&buttonInfo, sizeof(V3D_ButtonInfoV3), 1, m_pFile);  
		}
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_BUTTON;
	}

	return ERR_COMMON_OK;
}

// 背景图
DWORD CSaveV3D::WriteBackdropData()
{
	try
	{
		if (!m_pVGSSceneMgr)
		{
			// throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		// 记录背景图数据在文件的开始位置
		m_Table.BackdropPtr = ftell(m_pFile);

		CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

		// 背景图片个数
		UINT uBkgCount = GUIMngr.GetObjectsCount(VGS_GUIOBJ_TYPE_BACKDROP);
		fwrite(&(uBkgCount), sizeof(UINT), 1, m_pFile); 

		// 每个背景图片的数据
		for (UINT uIndex = 0; uIndex < uBkgCount; uIndex++)
		{
 			V3D_BackdropInfoV3 bgkInfo;
 			ZeroMemory(&bgkInfo, sizeof(V3D_BackdropInfoV3));

			BackGround* pBkgnd = (BackGround*)GUIMngr.GetObjectHandlebyIndex(VGS_GUIOBJ_TYPE_BACKDROP, uIndex);
			if (NULL != pBkgnd)
			{
				// 获取背景图片对象属性
				GUIInfo guiInfo;
 				ZeroMemory(&guiInfo, sizeof(GUIInfo));
				GUIMngr.GetObjectCommonPropety(pBkgnd, guiInfo);

				// 将属性转存到结构体
				strcpy(bgkInfo.Name, guiInfo.sName);
 				bgkInfo.bVisible	  = guiInfo.bVisible;
 				bgkInfo.Height        = guiInfo.Height;
 				bgkInfo.Left          = guiInfo.Left;	 
 				bgkInfo.Top           = guiInfo.Top;
 				bgkInfo.Width         = guiInfo.Width;
				//
				CMaterial* pMtrl = pBkgnd->GetMaterial();
				bgkInfo.materialIndex = m_pVGSSceneMgr->getMaterialIndex(pMtrl);
				//
				std::string strFuncName;
				for (int iFunc = 0; iFunc < VGS_MOUSEEVENT_CNT; iFunc++)
				{
					strFuncName = pBkgnd->GetEventFuncName(VGS_MOUSEEVENT_TYPE(iFunc));
					strcpy(bgkInfo.sMouseEventFunc[iFunc], strFuncName.c_str());
				}		
			}
 			fwrite(&bgkInfo, sizeof(V3D_BackdropInfo), 1, m_pFile);
		}
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_BACKDROP;
	}
	return ERR_COMMON_OK;
}

DWORD CSaveV3D::WriteAudioData()
{
	try
	{
		if (!m_pVGSSceneMgr)
		{
			// throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		m_Table.AudioPtr = ftell(m_pFile); // 记录audio数据在文件的开始位置

		UINT i = 0;
		fwrite(&i, sizeof(UINT), 1, m_pFile); // 没有数据，写入0
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_AUDIO;
	}
	return ERR_COMMON_OK;
}

DWORD CSaveV3D::WriteControlData()
{
	try
	{
		if (!m_pVGSSceneMgr)
		{
			// throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		// 记录control数据在文件的开始位置
		m_Table.ControlPtr = ftell(m_pFile); 

		// 计算各种类型控制器数目以及总的数目
 		DWORD WalkerCount = m_pVGSSceneMgr->GetWalkerCount();
 		DWORD FlyerCount  = m_pVGSSceneMgr->GetFlyerCount();
 		DWORD EditorCount = m_pVGSSceneMgr->GetEditorCount();
 		DWORD TotalCount = WalkerCount + FlyerCount + EditorCount;
 		 
		// 总的控制器个数
 		fwrite(&TotalCount, sizeof(TotalCount), 1, m_pFile);
 		if (TotalCount == 0) 
 		{
 			return ERR_COMMON_OK;
 		}

		// 当前控制器指针
		IController* pCurController = m_pVGSSceneMgr->GetCurrentController();

		V3D_ControlDataV3 controlInfo;
 		ZeroMemory(&controlInfo, sizeof(V3D_ControlDataV3));
		controlInfo.ControlType = (BYTE)(pCurController->GetControlType());
		//
		controlInfo.WalkerList.WalkerCnt = WalkerCount;
		if (WalkerCount > 0)
		{
			controlInfo.WalkerList.Walkers = new V3D_WalkerV3[WalkerCount];
			for (UINT uWalker = 0; uWalker < WalkerCount; uWalker++)
			{
 				WalkerInfo Info;
				CWalker *pController = (CWalker*)(m_pVGSSceneMgr->GetWalker(uWalker));
				pController->GetWalkerInfo(Info);
				//
				strcpy(controlInfo.WalkerList.Walkers[uWalker].Name, Info.Name);
				controlInfo.WalkerList.Walkers[uWalker].CameraIndex = _GetIndexInVector(m_vpCamera, m_pVGSSceneMgr->GetCameraByName(Info.Camera));
				controlInfo.WalkerList.Walkers[uWalker].walkSpeed = Info.walkSpeed;
				controlInfo.WalkerList.Walkers[uWalker].walkAcc = Info.walkAcc;
				controlInfo.WalkerList.Walkers[uWalker].rosSpeed = Info.rosSpeed;
				controlInfo.WalkerList.Walkers[uWalker].rosAcc = Info.rosAcc;
				controlInfo.WalkerList.Walkers[uWalker].height = Info.height;
				controlInfo.WalkerList.Walkers[uWalker].bCollision = Info.bCollision;
				controlInfo.WalkerList.Walkers[uWalker].colDis = Info.colDis;
				controlInfo.WalkerList.Walkers[uWalker].bWeight = Info.bWeight;
				controlInfo.WalkerList.Walkers[uWalker].liftSpeed = Info.liftSpeed;
				controlInfo.WalkerList.Walkers[uWalker].liftAcc = Info.liftAcc;
				controlInfo.WalkerList.Walkers[uWalker].upperAngle = Info.upperAngle;
				controlInfo.WalkerList.Walkers[uWalker].lowerAngle = Info.lowerAngle;
				//				
 				if (pCurController == pController)
 				{
 					controlInfo.ControlerIndex = uWalker;
 				}
			}
		}
		//
		controlInfo.FlyerList.FlyerCnt = FlyerCount;
		if (FlyerCount > 0)
		{
			controlInfo.FlyerList.Flyers = new V3D_FlyerV3[FlyerCount];
 			for (UINT uFlyer = 0; uFlyer < FlyerCount; uFlyer++)
 			{
 				FlyerInfo Info;
 				CFlyer *pController = (CFlyer*)(m_pVGSSceneMgr->GetFlyer(uFlyer));
 				pController->GetFlyerInfo(Info);
				//
				strcpy(controlInfo.FlyerList.Flyers[uFlyer].Name, Info.Name);
				controlInfo.FlyerList.Flyers[uFlyer].CameraIndex = _GetIndexInVector(m_vpCamera, m_pVGSSceneMgr->GetCameraByName(Info.Camera));
				controlInfo.FlyerList.Flyers[uFlyer].panSpeed = Info.panSpeed;
				controlInfo.FlyerList.Flyers[uFlyer].panAcc = Info.panAcc;
				controlInfo.FlyerList.Flyers[uFlyer].dollySpeed = Info.dollySpeed;
				controlInfo.FlyerList.Flyers[uFlyer].dollyAcc = Info.dollyAcc;
				controlInfo.FlyerList.Flyers[uFlyer].dollyFarDis = Info.dollyFarDis;
				controlInfo.FlyerList.Flyers[uFlyer].dollyNearDis = Info.dollyNearDis;
				controlInfo.FlyerList.Flyers[uFlyer].rotateSpeed = Info.rotateSpeed;
				controlInfo.FlyerList.Flyers[uFlyer].rotateAcc = Info.rotateAcc;
				controlInfo.FlyerList.Flyers[uFlyer].liftSpeed = Info.liftSpeed;
				controlInfo.FlyerList.Flyers[uFlyer].liftAcc = Info.liftAcc;
				controlInfo.FlyerList.Flyers[uFlyer].upperAngle = Info.upperAngle;
				controlInfo.FlyerList.Flyers[uFlyer].lowerAngle = Info.lowerAngle;
				controlInfo.FlyerList.Flyers[uFlyer].bCollision = Info.bCollision;
				controlInfo.FlyerList.Flyers[uFlyer].colDis = Info.colDis;
				//				
 				if (pCurController == pController)
 				{
 					controlInfo.ControlerIndex = uFlyer;
 				}
			}
		}
		//
		controlInfo.EditorList.EditorCnt = EditorCount;
		if (EditorCount > 0)
		{
			controlInfo.EditorList.Editors = new V3D_EditorV3[EditorCount];
 			for (UINT uEditor = 0; uEditor < EditorCount; uEditor++)
 			{
 				EditorInfo Info;
 				CEditor *pController = (CEditor*)(m_pVGSSceneMgr->GetEditor(uEditor));
				pController->GetEditorInfo(Info);
				CModel* pModel = pController->GetTargetModel();				
 				if (pModel)
 				{
					D3DXVECTOR3 v3Pos = pModel->m_pNode->getPosition(VTS_WORLD);
					Info.LookAtPos = vector3(v3Pos.x, v3Pos.y, v3Pos.z);
					Info.LookAtModelIndex = _GetIndexInVector(m_vpModel, pModel);
 				}
				//
				strcpy(controlInfo.EditorList.Editors[uEditor].Name, Info.Name);
				controlInfo.EditorList.Editors[uEditor].CameraIndex = _GetIndexInVector(m_vpCamera, m_pVGSSceneMgr->GetCameraByName(Info.Camera));
				controlInfo.EditorList.Editors[uEditor].bIsAutoRoundRoll = Info.bIsAutoRoundRoll;
				controlInfo.EditorList.Editors[uEditor].autoPitchSpeed = 0;
				controlInfo.EditorList.Editors[uEditor].autoRollSpeed = 0;
				controlInfo.EditorList.Editors[uEditor].autoRoundSpeed = 0;
				controlInfo.EditorList.Editors[uEditor].manualPitchSpeed = Info.manualSpeedX;
				controlInfo.EditorList.Editors[uEditor].manualRoundSpeed = Info.manualSpeedY;
				controlInfo.EditorList.Editors[uEditor].manualPanXSpeed = Info.manualPanXSpeed;
				controlInfo.EditorList.Editors[uEditor].manualPanYSpeed = Info.manualPanYSpeed;
				controlInfo.EditorList.Editors[uEditor].dollySpeed = Info.dollySpeed;
				controlInfo.EditorList.Editors[uEditor].dollyAcc = Info.dollyAcc;
				controlInfo.EditorList.Editors[uEditor].dollyFarDis = Info.dollyFarDis;
				controlInfo.EditorList.Editors[uEditor].dollyNearDis = Info.dollyNearDis;
				//				
 				if (pCurController == pController)
 				{
 					controlInfo.ControlerIndex = uEditor;
 				}
			}
		}

 		// 当前控制器信息
		fwrite(&controlInfo.ControlType, sizeof(INT), 1, m_pFile);
		fwrite(&controlInfo.ControlerIndex, sizeof(INT), 1, m_pFile);
		
		// Walker控制器信息
		fwrite(&controlInfo.WalkerList.WalkerCnt, sizeof(INT), 1, m_pFile);	
		if (controlInfo.WalkerList.WalkerCnt> 0 && controlInfo.WalkerList.Walkers != NULL)
		{
			fwrite(controlInfo.WalkerList.Walkers, controlInfo.WalkerList.WalkerCnt*sizeof(V3D_WalkerV3), 1, m_pFile);
			delete []controlInfo.WalkerList.Walkers;
			controlInfo.WalkerList.Walkers = NULL;
		}
		
		// Flayer控制器信息
		fwrite(&controlInfo.FlyerList.FlyerCnt, sizeof(INT), 1, m_pFile);		
		if (controlInfo.FlyerList.FlyerCnt> 0 && controlInfo.FlyerList.Flyers != NULL)
		{
			fwrite(controlInfo.FlyerList.Flyers, controlInfo.FlyerList.FlyerCnt*sizeof(V3D_FlyerV3), 1, m_pFile);
			delete []controlInfo.FlyerList.Flyers;
			controlInfo.FlyerList.Flyers = NULL;
		}
		
		// Editor控制器信息
		fwrite(&controlInfo.EditorList.EditorCnt, sizeof(INT), 1, m_pFile);		
		if (controlInfo.EditorList.EditorCnt> 0 && controlInfo.EditorList.Editors != NULL)
		{
			fwrite(controlInfo.EditorList.Editors, controlInfo.EditorList.EditorCnt*sizeof(V3D_EditorV3), 1, m_pFile);
			delete []controlInfo.EditorList.Editors;
			controlInfo.EditorList.Editors = NULL;
		}
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_CONTORLLER;
	}
	return ERR_COMMON_OK;
}

DWORD CSaveV3D::WriteFlashData()
{
	try
	{
		if (!m_pVGSSceneMgr)
		{
			// throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		m_Table.FlashPtr = ftell(m_pFile); // 记录flash数据在文件的开始位置

		UINT i = 0;
		fwrite(&i, sizeof(UINT), 1, m_pFile); // 没有数据，写入0
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_FALSH;
	}
	return ERR_COMMON_OK;
}

DWORD CSaveV3D::WritePrivacyData()
{
	try
	{
		if (!m_pVGSSceneMgr)
		{
			// throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		m_Table.PrivacyDataPtr = ftell(m_pFile); // 记录授权数据在文件的开始位置

		UINT i = 0;
		fwrite(&i, sizeof(UINT), 1, m_pFile); // 没有数据，写入0
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_FAIL;
	}
	return ERR_COMMON_OK;
}

DWORD CSaveV3D::WriteAnimation()
{
	try
	{
		if (!m_pVGSSceneMgr)
		{
			// throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		m_Table.AnimationPtr = ftell(m_pFile); // 记录动画数据在文件的开始位置

		UINT i = 0;
		fwrite(&i, sizeof(UINT), 1, m_pFile); // 没有数据，写入0
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_ANNIMATION;
	}

	return ERR_COMMON_OK;
}

//----------------------------------------------------------------------------------------
//add by NickLi, start here, 2008-05-22
//----------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
//写入场景结构图到 V3D文件
//------------------------------------------------------------------------------------------
DWORD CSaveV3D::WriteSceneGraph()
{	
	try
	{
		if (!m_pVGSSceneMgr)
		{
			// throw Ogre::Exception::exception("获取场景错误");
			return ERR_FILE_SAVEV3D_NODATA;
		}
		if (!m_pFile)
		{
			// throw Ogre::Exception::exception("获取文件错误");
			return ERR_FILE_SAVEV3D_OPENFILE;
		}

		//场景图数据的偏移量
		m_Table.sceneGraphDataPtr = ftell(m_pFile); // 记录动画数据在文件的开始位置

		//递归写入场景图结构
		CSceneNode* pRootNode = m_pVGSSceneMgr->GetRootSceneNode();
		ConstructSceneGraph(pRootNode);
	}
	catch (...)
	{
		return ERR_FILE_SAVEV3D_SCENEGRAPH;
	}

	return ERR_COMMON_OK;
}

//------------------------------------------------------------------------------------------
//递归获得场景图数据并写入V3D文件
//------------------------------------------------------------------------------------------
void CSaveV3D::ConstructSceneGraph(CSceneNode* pNode)
{
	//获得Node的数据
	VGS_Node vgsNode;
	GetNodeTypeAndIndex(pNode, &vgsNode);

	// 预定义相机节点不需要导出
	if (vgsNode.index == -1 && vgsNode.type != SCENEROOT)
	{
		return;
	}

	//写入node的类型
	fwrite(&(vgsNode.type), sizeof(vgsNode.type), 1, m_pFile);
	
	//写入nodem_pFile
	fwrite(&(vgsNode.index), sizeof(vgsNode.index), 1, m_pFile);

	//写入node的名称
	int strLen = pNode->name.length() + 1;
	fwrite(&strLen, sizeof(strLen), 1, m_pFile);
	fwrite((void*)(pNode->name.c_str()), strLen, 1, m_pFile);

	// 缩放
	D3DXVECTOR3 v3Scale = pNode->getScale(VTS_PARENT);
	WRITE_VECTOR3(v3Scale, m_pFile);
	//VECTOR3 vScale = VECTOR3(pNode->worldScale.x, pNode->worldScale.y, pNode->worldScale.z);
	//WRITE_VALUE(vScale)

	// 转角
	// VECTOR3 vRotation = VECTOR3(pNode->worldRotation.x, pNode->worldRotation.y, pNode->worldRotation.z);
	// WRITE_VALUE(vRotation)
	D3DXQUATERNION quat = pNode->getRotition(VTS_PARENT);
	fwrite(&quat.x, sizeof(quat.x), 1, m_pFile);
	fwrite(&quat.y, sizeof(quat.y), 1, m_pFile);
	fwrite(&quat.z, sizeof(quat.z), 1, m_pFile);
	fwrite(&quat.w, sizeof(quat.w), 1, m_pFile);

	// 写入node 位置
	D3DXVECTOR3 v3Pos = pNode->getPosition(VTS_PARENT);
	WRITE_VECTOR3(v3Pos, m_pFile);
	//VECTOR3 vPos = VECTOR3(pNode->worldPosition.x, pNode->worldPosition.y, pNode->worldPosition.z);
	//WRITE_VALUE(vPos)

	// 写入该node的动画数据
	WriteANodeAnimation(pNode);

	//写入子物体数量
	int childCnt = GetChildNodeCount(pNode);
	WRITE_VALUE(childCnt)

	//子物体队列
	for (INT i = 0; i < childCnt; ++i)
	{
		CSceneNode* pChildNode = GetChildNodePtr(pNode, i);
		if (NULL != pChildNode)
		{
			ConstructSceneGraph(pChildNode);
		}
	}
}

// 保存一个Node的动画数据
void CSaveV3D::WriteANodeAnimation(CSceneNode *pNode)
{
	UINT uFrameCount = 0;
	CAnimationManager& AnimationMngr = m_pRoot->GetAnimManager();
	std::string strAnimationName = AnimationMngr.GetNodeAnimationName(pNode);
	CNodeAnimation *pNodeAnim = AnimationMngr.getKeyFrameByName(strAnimationName);

	// 写入关键帧数目
	uFrameCount = (NULL != pNodeAnim) ? pNodeAnim->getFrameCount() : 0;
	WRITE_VALUE(uFrameCount, m_pFile); 

	for (UINT uFrame = 0; uFrame < uFrameCount; ++uFrame)
	{
		//写入Node的关键帧动画数据
		N3D_NodeTransform n3dNodeTransform;
		CNodeKeyFrame *pNodeKeyFrame = pNodeAnim->getFrameData(uFrame);
		if (pNodeKeyFrame)
		{
			n3dNodeTransform.position = vector3(pNodeKeyFrame->m_pos.x, pNodeKeyFrame->m_pos.y, pNodeKeyFrame->m_pos.z);
			n3dNodeTransform.quat     = quaternion(pNodeKeyFrame->m_quat.x, pNodeKeyFrame->m_quat.y, pNodeKeyFrame->m_quat.z, pNodeKeyFrame->m_quat.w);
			n3dNodeTransform.scale    = vector3(pNodeKeyFrame->m_scale.x, pNodeKeyFrame->m_scale.y, pNodeKeyFrame->m_scale.z);
			n3dNodeTransform.time     = 0.0f;
		}
		WRITE_VALUE(n3dNodeTransform, m_pFile);

		// 写入相机或灯光参数动画
		if (pNodeKeyFrame->getType() == VAT_ANIMATIONTYPE_CAMERA)
		{
			CCameraKeyFrame *pCamAnim = (CCameraKeyFrame*)pNodeKeyFrame;
			bool bHasParamAnimation = pCamAnim->m_bHasParamAnimation;
			WRITE_VALUE(bHasParamAnimation, m_pFile);
			if (pCamAnim && pCamAnim->m_bHasParamAnimation)
			{
				float fov      = pCamAnim->getFov();
				float nearClip = pCamAnim->getNearClip();
				float farClip  = pCamAnim->getFarClip();
				float roll     = pCamAnim->getRoll();
				WRITE_VALUE(fov, m_pFile);
				WRITE_VALUE(nearClip, m_pFile);
				WRITE_VALUE(farClip, m_pFile);
				WRITE_VALUE(roll, m_pFile);
			}
		}
		else if (pNodeKeyFrame->getType() == VAT_ANIMATIONTYPE_LIGHT)
		{
			CLightKeyFrame *pLightAnim = (CLightKeyFrame*)pNodeKeyFrame;
			bool bHasParamAnimation = pLightAnim->m_bHasParamAnimation;
			WRITE_VALUE(bHasParamAnimation, m_pFile);
			if (pLightAnim && pLightAnim->m_bHasParamAnimation && pLightAnim->m_pLight)
			{
				D3DCOLORVALUE colorvalue = pLightAnim->getColor();
				DWORD color = D3DCOLOR_COLORVALUE(colorvalue.r, colorvalue.g, colorvalue.b, colorvalue.a);
				float range = pLightAnim->getRange();
				WRITE_VALUE(color, m_pFile);
				WRITE_VALUE(range, m_pFile);

				if (pLightAnim->m_pLight->getType() == VLT_DIR)
				{
					VECTOR3 v3Dir = pLightAnim->getDirection();
					WRITE_VECTOR3(v3Dir, m_pFile); 
				}
				else if (pLightAnim->m_pLight->getType() == VLT_SPOT)
				{
					VECTOR3 v3Dir = pLightAnim->getDirection();
					WRITE_VECTOR3(v3Dir, m_pFile);

					float theta, phi, fallof;
					pLightAnim->getAngle(theta, phi, fallof);
					WRITE_VALUE(theta, m_pFile);
					WRITE_VALUE(phi, m_pFile);
					WRITE_VALUE(fallof, m_pFile);
				}
			}
		}
	}
}

//获得一个节点的子节点数量
INT CSaveV3D::GetChildNodeCount(CSceneNode* pNode)
{
	if (!pNode)
	{
		return 0;
	}

	INT count = 0;
	INT childCount = pNode->m_child.size();
	for (INT i = 0; i < childCount; ++i)
	{
		CSceneNode *pChildNode = pNode->m_child[i];
		if (pChildNode && pChildNode->getMovableObject() != 0)
		{
			if (pChildNode->getMovableType() == VGS_MOVABLETYPE_MODEL)
			{
				if (_GetIndexInVector(m_vpCamera, (CCamera*)pChildNode->getMovableObject()) == -1)  // 过滤掉预定义的且没有被控制对象使用的相机
				{
					continue;
				}
			}
			++count;
		}
	}
	return count;
}

// 根据索引返回子节点指针
CSceneNode*	CSaveV3D::GetChildNodePtr(CSceneNode* pParentNode, int iIndex)
{
	if (!pParentNode || pParentNode->m_child.size() == 0)
	{
		return NULL;
	}

	CSceneNode* pResultNode = NULL;

	int iCurRealIndex = 0;
	UINT childCount = pParentNode->m_child.size();
 	for (UINT iChild = 0; iChild < childCount; ++iChild)
 	{
 		CSceneNode *pChildNode = pParentNode->m_child[iChild]; 
		if (pChildNode && pChildNode->getMovableObject() != 0)
		{
			if (pChildNode->getMovableType() == VGS_MOVABLETYPE_MODEL)
			{
				if (_GetIndexInVector(m_vpCamera, (CCamera*)pChildNode->getMovableObject()) == -1)  // 过滤掉预定义的且没有被控制对象使用的相机
				{
					continue;
				}
			}
			if (iCurRealIndex == iIndex)
			{
				pResultNode = pChildNode;
				break;
			}
			iCurRealIndex++;
		}
		/*
 		if (!pChildNode)
 		{
 			continue;
 		}
 		if (pChildNode->getMovableObject() == 0 && pChildNode->m_child.size() == 0)
 		{
 			continue;
 		}
 		
 		// 过滤一些不需要保存的节点
		CMovableObject *pAttachObj = pChildNode->getMovableObject();
		if (pAttachObj)
		{
			if ( (pAttachObj->getType() == VGS_MOVABLETYPE_CAMERA) && _GetIndexInVector(m_vpCamera, (CCamera*)pAttachObj) == -1)
			{
				continue;
			}
		}

		if (iIndex == iChild)
		{
			pResultNode = pChildNode;
			break;
		}*/
 	}
	return pResultNode;
}

// 获取node节点的类型和附属对象在各自队列中的索引
void CSaveV3D::GetNodeTypeAndIndex(CSceneNode* pSceneNode, VGS_Node* pVGSNode)
{	
	if (NULL == pSceneNode || NULL == pVGSNode)
		return;
	pVGSNode->index = -1;
	pVGSNode->type = INVALID;

	CMovableObject* pAttachObj = pSceneNode->getMovableObject();

	// 对象类型的描述
	if (pSceneNode->m_parent == NULL)
	{
		pVGSNode->type  = SCENEROOT;
		pVGSNode->index = -1;
	}
	else if (NULL == pAttachObj)
	{
		pVGSNode->type  = INVALID;  // 只要不是SCENEROOT就行
		pVGSNode->index = -1;
	}
	else if (VGS_MOVABLETYPE_CAMERA == pAttachObj->getType())
	{
		pVGSNode->index = _GetIndexInVector(m_vpCamera, (CCamera*)pAttachObj);
		pVGSNode->type  = CAMERANODE;
	}
	else if (VGS_MOVABLETYPE_LIGHT == pAttachObj->getType())
	{
		pVGSNode->index = m_pVGSSceneMgr->GetLightIndex((CLight*)pAttachObj);
		pVGSNode->type  = LIGHTNODE;
	}
	else if (VGS_MOVABLETYPE_MODEL == pAttachObj->getType())
	{
		pVGSNode->type = MESHNODE;		
		pVGSNode->index = _GetIndexInVector(m_vpModel, (CModel*)pAttachObj);
	}
	else
	{
		pVGSNode->type  = INVALID; // 
		pVGSNode->index = -1;
	}
}

/*
// 写入场景图
VOID CSaveV3D::WriteSceneGraph()
{
	if (!m_pVGSSceneMgr)
	{
		throw Ogre::Exception::exception("获取场景错误");
	}
	if (!m_pFile)
	{
		throw Ogre::Exception::exception("获取文件错误");
	}

	m_Table.sceneGraphDataPtr = ftell(m_pFile); // 记录动画数据在文件的开始位置

	CreateSceneGraphNode(-1, m_pVGSSceneMgr->mSceneMgr->getRootSceneNode());  // 得到根节点的场景图

	// 写入场景图数据
	UINT NodeCount = m_VSceneGraph.size();
	fwrite(&NodeCount, sizeof(NodeCount), 1, m_pFile); // 写入节点数目
	for (UINT iNode = 0; iNode < NodeCount; ++iNode)
	{
		SceneGraphNode &SGNode = m_VSceneGraph[iNode];
		UINT ObjCount = SGNode.vObject.size();
		fwrite(&SGNode, sizeof(NodeSaveParam), 1, m_pFile);    // 直接把SGNode前面的信息（跟NodeSaveParam数据结构相同）写到文件
		//fwrite(&(SGNode.ParentIdx), sizeof(SGNode.ParentIdx), 1, m_pFile);  // 写入节点的父节点索引
		//NodeSaveParam Param;
		//Param.ParentIdx = SGNode.ParentIdx;
		//Param.PosX = SGNode.PosX;
		//Param.PosY = SGNode.PosY;
		//Param.PosZ = SGNode.PosZ;
		//Param.ScaleX = SGNode.ScaleX;
		//Param.ScaleY = SGNode.ScaleY;
		//Param.ScaleZ = SGNode.ScaleZ;
		//Param.OrientW = SGNode.OrientW;
		//Param.OrientX = SGNode.OrientX;
		//Param.OrientY = SGNode.OrientY;
		//Param.OrientY = SGNode.OrientZ;
		//fwrite(&Param, sizeof(Param), 1, m_pFile);                          // batch write the Node parameters
		fwrite(&(ObjCount), sizeof(ObjCount), 1, m_pFile);                  // 写入节点下附属对象的个数
		for (UINT iObj = 0; iObj < ObjCount; ++iObj)
		{
			NodeObject &obj = SGNode.vObject[iObj];
			fwrite(&obj.type, sizeof(obj.type), 1, m_pFile);               // 写入节点对象的类型和索引
			fwrite(&obj.Index, sizeof(obj.Index), 1, m_pFile);
		}
	}
} 

// 生成一个节点及其子节点的场景图数据
void CSaveV3D::CreateSceneGraphNode(int ParentIdx, SceneNode *pNode)
{
	SceneGraphNode SGNode;
	SGNode.ParentIdx   = ParentIdx;
	Ogre::Vector3 Position  = pNode->getPosition();
	Ogre::Vector3 Scale     = pNode->getScale();
	Ogre::Quaternion Orient = pNode->getOrientation();
	SGNode.PosX        = Position.x;
	SGNode.PosY        = Position.y;
	SGNode.PosZ        = Position.z;
	SGNode.ScaleX      = Scale.x;
	SGNode.ScaleY      = Scale.y;
	SGNode.ScaleZ      = Scale.z;
	SGNode.OrientW     = Orient.w;
	SGNode.OrientX     = Orient.x;
	SGNode.OrientY     = Orient.y;
	SGNode.OrientZ     = Orient.z;

	unsigned short ObjCount = pNode->numAttachedObjects();
	for (unsigned short i = 0; i < ObjCount; ++i)
	{
		MovableObject *pObject = pNode->getAttachedObject(i);
		SGNode.vObject.push_back(GetNodeObject(pObject));
	}

	m_VSceneGraph.push_back(SGNode);  // 生成自身的场景图节点

	int thisParentIdx = m_VSceneGraph.size() - 1;

	unsigned short childrenCount = pNode->numChildren();
	for (unsigned short i = 0; i < childrenCount; ++i)
	{
		SceneNode *pChild = dynamic_cast<SceneNode *>(pNode->getChild(i));
		CreateSceneGraphNode(thisParentIdx, pChild);
	}

}

// 根据对象指针返回Node对象
NodeObject CSaveV3D::GetNodeObject(const MovableObject *pObject)
{
	NodeObject obj;
	
	String sType = pObject->getMovableType(); // 对象类型的描述
	if (0 == strcmp(sType.c_str(), "Camera"))
	{
		obj.type  = NBT_CAMERA;
		obj.Index = m_pVGSSceneMgr->GetCamNameIndex(pObject->getName());
	}
	else if (sType == Ogre::LightFactory::FACTORY_TYPE_NAME)
	{
		 obj.type = NBT_LIGHT;
		 obj.Index = m_pVGSSceneMgr->GetLightNameIndex(pObject->getName());
	}
	else if (sType == Ogre::EntityFactory::FACTORY_TYPE_NAME)
	{
		obj.type = NBT_ENTITY;		
		obj.Index = m_pVGSSceneMgr->GetEntityNameIndex(pObject->getName());
	}
	else
	{
		obj.type  = NBT_NONE;
		obj.Index = -1;
	}

	return obj;
}*/

