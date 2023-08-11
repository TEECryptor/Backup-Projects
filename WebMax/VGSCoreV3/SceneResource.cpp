// SceneResource.cpp: implementation of the CSubScene class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SceneResource.h"
//播放电影文件的类
//#include "CMovieTexture.h"
//#include "CAudioPlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSceneResource::CSceneResource(std::string strName, CScene* pScene)
{
	m_pScene = pScene;
	m_fileVersion = 2.0f;					//当前N3D文件的版本号
	name = strName;
	m_pCloneScene = NULL;					//场景的类型，是新建的还是克隆的
	m_pRootNode = NULL;
	D3DXMatrixIdentity(&m_transform);     // 初始化根节点的变换矩阵
}

CSceneResource::~CSceneResource()
{

}

//获得资源在主场景资源列表中的编号
int CSceneResource::GetModelIndexInMainScene(CModel* pModel)
{
	int modelCnt = m_pScene->model.size();
	std::vector<CModel*>::iterator ppModel = m_pScene->model.begin();
	for (int i = 0; i< modelCnt; i++)
	{
		CModel* _pModel = *(ppModel + i);
		if (pModel == _pModel)
			return i;
	}
	return -1;
}

int CSceneResource::GetModelGroupIndexInMainScene(CModelGroup* pModelGroup)
{
	int modelGroupCnt = m_pScene->modelGroup.size();
	std::vector<CModelGroup*>::iterator ppModelGroup = m_pScene->modelGroup.begin();
	for (int i = 0; i< modelGroupCnt; i++)
	{
		CModelGroup* _pModelGroup = *(ppModelGroup + i);
		if (_pModelGroup = pModelGroup)
			return i;
	}
	return -1;
}

int CSceneResource::GetSectionMapIndexInMainScene(CSectionMap* pSectionMap)
{
	int sectionMapCnt = m_pScene->sectionMap.size();
	std::vector<CSectionMap*>::iterator ppSectionMap = m_pScene->sectionMap.begin();
	for (int i = 0; i< sectionMapCnt; i++)
	{
		CSectionMap* _pSectionMap = *(ppSectionMap + i);
		if (_pSectionMap = pSectionMap)
			return i;
	}
	return -1;
}

int CSceneResource::GetMaterialIndexInMainScene(CMaterial* pMaterial)
{
	int mtrlCnt = m_pScene->m_vShader.size();
	std::vector<CMaterial*>::iterator ppMtrl = m_pScene->m_vShader.begin();
	for (int i = 0; i< mtrlCnt; i++)
	{
		CMaterial* _pMtrl = *(ppMtrl + i);
		if (_pMtrl == pMaterial)
			return i;
	}
	return -1;
}

int CSceneResource::GetTextureIndexInMainScene(CTexture* pTexture)
{
	int texCnt = m_pScene->m_vTexture.size();
	std::vector<CTexture*>::iterator ppTex = m_pScene->m_vTexture.begin();
	for (int i = 0; i< texCnt; i++)
	{
		CTexture* _pTex = *(ppTex + i);
		if (_pTex == pTexture)
			return i;
	}
	return -1;
}


int CSceneResource::GetCubeMapIndexInMainScene(CCubeMap* pCubeMap)
{
	int cubeMapCnt = m_pScene->cubeMap.size();
	std::vector<CCubeMap*>::iterator ppCubeMap = m_pScene->cubeMap.begin();
	for (int i = 0; i< cubeMapCnt; i++)
	{
		CCubeMap* _pCubeMap = *(ppCubeMap + i);
		if (_pCubeMap == pCubeMap)
			return i;
	}
	return -1;
}

int CSceneResource::GetMovieTextureIndexInMainScene(DWORD pMovieTexture)
{
	/*
	int movieTexCnt = m_pScene->movieTexture.size();
	CMovieTexture** ppMovieTex = (CMovieTexture**)m_pScene->movieTexture.begin();
	for (int i = 0; i< movieTexCnt; i++)
	{
		CMovieTexture* _pMovieTex = *(ppMovieTex + i);
		if ((DWORD)_pMovieTex == pMovieTexture)
			return i;
	}
	*/
	return -1;
}

int CSceneResource::GetDynamicTextureIndexInMainScene(CDynamicTex* pDynamicTexture)
{
	int dynamicTexCnt = m_pScene->dynamicTexture.size();
	std::vector<CDynamicTex*>::iterator ppDyTex = m_pScene->dynamicTexture.begin();
	for (int i = 0; i< dynamicTexCnt; i++)
	{
		CDynamicTex* _pDyTex = *(ppDyTex + i);
		if (_pDyTex == pDynamicTexture)
			return i;
	}
	return -1;
}

int CSceneResource::GetCameraIndexInMainScene(CCamera* pCamera)
{
	return m_pScene->GetCameraIndex(pCamera);
// 	int camCnt = m_pScene->m_pCurCamera.size();
// 	CCamera** ppCam = m_pScene->m_pCurCamera.begin();
// 	for (int i = 0; i< camCnt; i++)
// 	{
// 		CCamera* _pCam = *(ppCam + i);
// 		if (_pCam == pCamera)
// 			return i;
// 	}
// 	return -1;
}

int CSceneResource::GetLightIndexInMainScene(CLight* pLight)
{
	return m_pScene->GetLightIndex(pLight);
// 	int lightCnt = m_pScene->m_vLight.size();
// 	CLight** ppLight = m_pScene->m_vLight.begin();
// 	for (int i = 0; i< lightCnt; i++)
// 	{
// 		CLight* _pLight = *(ppLight + i);
// 		if (_pLight == pLight)
// 			return i;
// 	}
// 	return -1;
}

int CSceneResource::GetOverlayIndexInMainScene(COverlay* pOverlay)
{
	int overlayCnt = m_pScene->m_vOverlay.size();
	std::vector<COverlay*>::iterator ppOverlay = m_pScene->m_vOverlay.begin();
	for (int i = 0; i< overlayCnt; i++)
	{
		COverlay* _pOverlay = *(ppOverlay + i);
		if (_pOverlay == pOverlay)
			return i;
	}
	return -1;
}

int CSceneResource::GetImageIndexInMainScene(CImage* pImage)
{
	int imageCnt = m_pScene->m_vImage.size();
	std::vector<CImage*>::iterator ppImage = m_pScene->m_vImage.begin();
	for (int i = 0; i< imageCnt; i++)
	{
		CImage* _pImage = *(ppImage + i);
		if (_pImage == pImage)
			return i;
	}
	return -1;
}

int CSceneResource::GetNavigaterIndexInMainScene(CNavigator* pNavigater)
{
	int navCnt = m_pScene->m_vNavigater.size();
	std::vector<CNavigator*>::iterator ppNav = m_pScene->m_vNavigater.begin();
	for (int i = 0; i< navCnt; i++)
	{
		CNavigator* _pNav = *(ppNav + i);
		if (_pNav == pNavigater)
			return i;
	}
	return -1;
}

int CSceneResource::GetButtonIndexInMainScene(CButton* pButton)
{
	int btnCnt = m_pScene->m_vButton.size();
	std::vector<CButton*>::iterator ppBtn = m_pScene->m_vButton.begin();
	for (int i = 0; i< btnCnt; i++)
	{
		CButton* _pBtn = *(ppBtn + i);
		if (_pBtn == pButton)
			return i;
	}
	return -1;
}

int CSceneResource::GetFontIndexInMainScene(CFont* pFont)
{
	int fontCnt = m_pScene->m_vFont.size();
	std::vector<CFont*>::iterator ppFont = m_pScene->m_vFont.begin();
	for (int i = 0; i< fontCnt; i++)
	{
		CFont* _pFont = *(ppFont + i);
		if (_pFont == pFont)
			return i;
	}
	return -1;
}

int CSceneResource::GetTextIndexInMainScene(CText* pText)
{
	/*int texCnt = m_pScene->m_vText.size();
	std::vector<CText*>::iterator ppText = m_pScene->m_vText.begin();
	for (int i = 0; i< texCnt; i++)
	{
		CText* _pText = *(ppText + i);
		if (_pText == pText)
			return i;
	}*/
	return -1;
}

int CSceneResource::GetAudioIndexInMainScene(DWORD pAudio)
{
	/*
	int audioCnt = m_pScene->audio.size();
	CAudioPlayer** ppAudio = (CAudioPlayer**)m_pScene->audio.begin();
	for (int i = 0; i< audioCnt; i++)
	{
		CAudioPlayer* _pAudio = *(ppAudio + i);
		if ((DWORD)_pAudio == pAudio)
			return i;
	}
	*/
	return -1;
}

int CSceneResource::GetWalkerIndexInMainScene(CWalker* pWalker)
{
	return m_pScene->GetWalkerIndex(pWalker);
	/*int walkerCnt = m_pScene->m_vWalker.size();
	std::vector<CWalker*>::iterator ppWalker = m_pScene->m_vWalker.begin();
	for (int i = 0; i< walkerCnt; i++)
	{
		CWalker* _pWalker = *(ppWalker + i);
		if (_pWalker == pWalker)
			return i;
	}
	return -1;*/
}

int CSceneResource::GetEditorIndexInMainScene(CEditor* pEditor)
{
	return m_pScene->GetEditorIndex(pEditor);
	/*int editorCnt = m_pScene->GetEditorCount();
	std::vector<CEditor*>::iterator ppEditor = m_pScene->m_vEditor.begin();
	for (int i = 0; i< editorCnt; i++)
	{
		CEditor* _pEditor = *(ppEditor + i);
		if (_pEditor == pEditor)
			return i;
	}
	return -1;*/
}

int CSceneResource::GetFlyerIndexInMainScene(CFlyer* pFlyer)
{
	return m_pScene->GetFlyerIndex(pFlyer);
	/*int flyerCnt = m_pScene->m_vFlyer.size();
	std::vector<CFlyer*>::iterator ppFlyer = m_pScene->m_vFlyer.begin();
	for (int i = 0; i< flyerCnt; i++)
	{
		CFlyer* _pFlyer = *(ppFlyer + i);
		if (_pFlyer == pFlyer)
			return i;
	}
	return -1;*/
}

//递归克隆场景中的对象
void CSceneResource::CloneNodeObject(CSceneResource* pNewSceneResource, CSceneNode* pNewNode, CSceneNode* pOldNode)
{
// 	int childNodeCnt = pOldNode->m_child.size();
// 	CSceneNode** ppNode = pOldNode->m_child.begin();
// 	if (childNodeCnt == 0) return;
// 	
// 	for (int i = 0; i< childNodeCnt; i++)
// 	{
// 		//被复制的场景的Node
// 		CSceneNode* iNode = *(ppNode + i);
// 		
// 		//新建一个Node到新的场景资源对象
// 		CSceneNode* iNewNode = new CSceneNode();
// 		iNewNode->curTransform = iNode->curTransform;
// 		iNewNode->currentFrame = iNode->currentFrame;
// 		iNewNode->keyFrameCnt = iNode->keyFrameCnt;
// 		if (iNode->keyFrameCnt > 0)
// 		{
// 			iNewNode->kfrtransform = new D3DXMATRIX[iNode->keyFrameCnt];
// 			memcpy(iNewNode->kfrtransform, iNode->kfrtransform, sizeof(D3DXMATRIX) * iNode->keyFrameCnt);
// 		}
// 
// 		//保留父子关系
// 		iNewNode->m_parent = pNewNode;
// 		pNewNode->m_child.push_back(iNewNode);
// 
// 		//克隆该节点下面的对象
// 		switch(iNode->type)
// 		{
// 			//模型节点
// 		case NNT_MODEL:
// 			{
// 				CModel* pOldModel = (CModel*)iNode->m_object;
// 				std::string strNewName = pOldModel->name;
// 				strNewName.append("_clone");
// 				CModel* pNewModel = pOldModel->clone(strNewName);
// 				pNewModel->pNode = iNewNode;
// 				//加入场景资源对象列表
// 				pNewModel->index = pNewSceneResource->m_pScene->model.size();
// 				pNewSceneResource->model.push_back(pNewModel);
// 				pNewSceneResource->m_pScene->model.push_back(pNewModel);
// 				//新节点的名字
// 				iNewNode->name = strNewName;
// 			}
// 			break;
// 
// 			//相机节点
// 		case NNT_CAMERANODE:
// 			{
// 				CCamera* pOldCamera = (CCamera*)iNode->m_object;
// 				std::string strNewName = pOldCamera->name;
// 				strNewName.append("_clone");
// 				CCamera* pNewCamera = pOldCamera->clone(strNewName);
// 				pNewCamera->pNode = iNewNode;
// 				//加入场景资源对象列表
// 				pNewCamera->index = pNewSceneResource->m_pScene->m_vCamera.size();
// 				pNewSceneResource->camera.push_back(pNewCamera);
// 				pNewSceneResource->m_pScene->m_vCamera.push_back(pNewCamera);
// 				//新节点的名字
// 				iNewNode->name = strNewName;
// 			}
// 			break;
// 
// 			//灯光节点
// 		case NNT_LIGHTNODE:
// 			{
// 				CLight* pOldLight = (CLight*)iNode->m_object;
// 				std::string strNewName = pOldLight->name;
// 				strNewName.append("_clone");
// 				CLight* pNewLight = pOldLight->clone(strNewName);
// 				pNewLight->pNode = iNewNode;
// 				//加入场景资源对象列表
// 				pNewLight->index = pNewSceneResource->m_pScene->m_vLight.size();
// 				pNewSceneResource->light.push_back(pNewLight);
// 				pNewSceneResource->m_pScene->m_vLight.push_back(pNewLight);
// 				//设置灯光的状态
// 				pNewLight->SetLightEnable(true);
// 				//新节点的名字
// 				iNewNode->name = strNewName;
// 			}
// 			break;
// 		}
// 
// 		//递归
// 		CloneNodeObject(pNewSceneResource, iNewNode, iNode);
// 	}
}

//克隆当前的场景资源并加载到场景
CSceneResource*	CSceneResource::Clone(std::string newSceneName)
{
	//按照场景图来进行克隆
	if (m_pRootNode == NULL) return NULL;
	
	//注意，材质和贴图、音频、Image不克隆，可以公用
	CSceneResource* pNewSceneResource = new CSceneResource(newSceneName, m_pScene);
	pNewSceneResource->m_pCloneScene = this;

	//新场景的RootNode
	pNewSceneResource->m_pRootNode = new CSceneNode();
	// pNewSceneResource->m_pRootNode->m_Type = NNT_SCENEROOT;
	pNewSceneResource->m_pRootNode->name = newSceneName;

	//递归进行场景对象的克隆
	CloneNodeObject(pNewSceneResource, pNewSceneResource->m_pRootNode, m_pRootNode);

	//克隆材质和贴图列表
	CloneShaderList(pNewSceneResource);
	CloneTextureList(pNewSceneResource);

	//加入克隆列表
	this->clonedScene.push_back(pNewSceneResource);

	//重新构造材质的SubMeshList列表
	ConstructShaderSubMeshList();	

	return pNewSceneResource;
}

//克隆材质列表
void CSceneResource::CloneShaderList(CSceneResource* pNewSceneResource)
{
	int shaderCnt = shader.size();
	std::vector<CMaterial*>::iterator ppMtrl = shader.begin();
	for (int i = 0; i< shaderCnt; i++)
	{
		CMaterial* pMtrl = *(ppMtrl + i);
		pNewSceneResource->shader.push_back(pMtrl);
	}
}

//克隆贴图列表
void CSceneResource::CloneTextureList(CSceneResource* pNewSceneResource)
{
	int texCnt = texture.size();
	std::vector<CTexture*>::iterator ppTex = texture.begin();
	for (int i = 0; i< texCnt; i++)
	{
		CTexture* pTex = *(ppTex + i);
		pNewSceneResource->texture.push_back(pTex);
	}	

	int cubeMapCnt = cubeMap.size();
	std::vector<CCubeMap*>::iterator ppCubeMap = cubeMap.begin();
	for (i = 0; i< cubeMapCnt; i++)
	{
		CCubeMap* pCubeMap = *(ppCubeMap + i);
		pNewSceneResource->cubeMap.push_back(pCubeMap);
	}
	
	/*
	int movieTexCnt = movieTexture.size();
	CMovieTexture** ppMovieTexture = (CMovieTexture**)movieTexture.begin();
	for (i = 0; i< movieTexCnt; i++)
	{
		CMovieTexture* pMovieTexture = *(ppMovieTexture + i);
		pNewSceneResource->movieTexture.push_back((DWORD)pMovieTexture);
	}
	*/

	int dyTexCnt = dynamicTexture.size();
	std::vector<CDynamicTex*>::iterator ppDynamicTex = dynamicTexture.begin();
	for (i = 0; i< dyTexCnt; i++)
	{
		CDynamicTex* pDynamicTex = *(ppDynamicTex + i);
		pNewSceneResource->dynamicTexture.push_back(pDynamicTex);
	}
}

//如果释放的是克隆源场景，则将剩下的孩子场景的第一个变为克隆源场景
void CSceneResource::ResetClonedSceneProp()
{
	//如果不是克隆出来的场景
	if (m_pCloneScene == NULL)
	{
		int clonedSceneCnt = clonedScene.size();
		if (clonedSceneCnt > 0)
		{
			std::vector<CSceneResource*>::iterator ppSceneResource = clonedScene.begin();
			//将第一个孩子设置为克隆源
			CSceneResource* firstClonedSceneResource = *ppSceneResource;
			firstClonedSceneResource->m_pCloneScene = NULL;
			//加入孩子节点
			firstClonedSceneResource->clonedScene.clear();
			for (int i = 1; i< clonedSceneCnt; i++)
			{
				CSceneResource* pCurSceneResource = *(ppSceneResource + i);
				pCurSceneResource->m_pCloneScene = firstClonedSceneResource;
				firstClonedSceneResource->clonedScene.push_back(pCurSceneResource);
			}
		}
	}
	else //是克隆出来的场景
	{
		//当前场景的原场景
		CSceneResource* pSrcSceneResource = this->m_pCloneScene;
		//将当前场景从原场景的克隆队列删除
		int clonedSceneCnt = pSrcSceneResource->clonedScene.size();
		std::vector<CSceneResource*>::iterator ppSceneResource = pSrcSceneResource->clonedScene.begin();
		for (int i = 0; i< clonedSceneCnt; i++)
		{
			CSceneResource* pSceneResource = *(ppSceneResource + i);
			if (pSceneResource == this)
			{
				pSrcSceneResource->clonedScene.erase(ppSceneResource + i);
				return;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 方法名: ConstructShaderSubMeshList()
// 参  数: 
// 描  述: 构造场景中所有材质的SubMesh列表
//-----------------------------------------------------------------------------
void CSceneResource::ConstructShaderSubMeshList()
{
	int shaderCnt = shader.size();
	std::vector<CMaterial*>::iterator ppMtrl = shader.begin();
	for (int i = 0; i< shaderCnt; i++)
	{
		CMaterial* pMtrl = *(ppMtrl + i);
		
		//将使用pMtrl材质的SubMesh规整到pMtrl->subMeshList中
		pMtrl->subMeshList.clear();
		PushSubMeshToShader(pMtrl);
	}
}

//-----------------------------------------------------------------------------
// 方法名: PushSubMeshToShader()
// 参  数: 
// 描  述: 将使用pMtrl材质的SubMesh规整到pMtrl->subMeshList中
//-----------------------------------------------------------------------------
void CSceneResource::PushSubMeshToShader(CMaterial* pMtrl)
{
	int modelCnt = model.size();
	std::vector<CModel*>::iterator ppModel = model.begin();
	for (int i = 0; i< modelCnt; i++)
	{
		CModel* pModel = *(ppModel + i);

		int subMeshCnt = pModel->getSubModelCount();
		for (int j = 0; j < subMeshCnt; ++j)
		{
			CMaterial *pMaterial = pModel->getSubModelMaterial(j);
			if (pMaterial == pMtrl)
			{
				VGS_RenderSubmesh subMesh;
				subMesh.modelPointer = (DWORD)pModel;
				subMesh.subMeshID = j;
				pMtrl->subMeshList.push_back(subMesh);
			}
		}
		
		/*int subMeshCnt = pModel->subMesh.size();
		std::vector<_subMesh>::iterator pSubMesh = pModel->subMesh.begin();
		for (int j = 0; j< subMeshCnt; j++)
		{
			_subMesh subMesh = *(pSubMesh + j);
			if (subMesh.shader == pMtrl)
			{
				VGS_RenderSubmesh subMesh;
				subMesh.modelPointer = (DWORD)pModel;
				subMesh.subMeshID = j;
				pMtrl->subMeshList.push_back(subMesh);
			}
		}*/
	}
}

//释放资源方法
void CSceneResource::Release()
{
	//如果释放的是克隆源场景，则将剩下的孩子场景的第一个变为克隆源场景
	ResetClonedSceneProp();

	//模型有关的成员变量----------------------------------------------
	int modelCnt = model.size();
	std::vector<CModel*>::iterator ppModel = model.begin();
	for (int i = 0; i< modelCnt; i++)
	{
		CModel* pModel = *(ppModel + i);

		//从主场景模型队列中删除
		int index = GetModelIndexInMainScene(pModel);
		if (index != -1)
			m_pScene->model.erase(m_pScene->model.begin() + index);

		//删除节点
		SAFE_RELEASE(pModel->m_pNode);
		SAFE_DELETE(pModel->m_pNode);

		//释放资源//如果是原场景，而且没有克隆孩子
		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pModel);

		SAFE_DELETE(pModel);
	}	

	//剖面图----------------------------------------------------------
	int sectionMapCnt = sectionMap.size();
	std::vector<CSectionMap*>::iterator ppSectionMap = sectionMap.begin();
	for (i = 0; i< sectionMapCnt; i++)
	{
		CSectionMap* pSectionMap = *(ppSectionMap + i);

		//从主场景中删除
		int index = GetSectionMapIndexInMainScene(pSectionMap);
		if (index != -1)
			m_pScene->sectionMap.erase(m_pScene->sectionMap.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0) 
			SAFE_RELEASE(pSectionMap);
		SAFE_DELETE(pSectionMap);
	}

	//材质有关的成员变量----------------------------------------------
	int shaderCnt = shader.size();
	std::vector<CMaterial*>::iterator ppMaterial = shader.begin();
	for (i = 0; i< shaderCnt; i++)
	{
		CMaterial* pMaterial = *(ppMaterial + i);

		//从主场景中删除
		int index = GetMaterialIndexInMainScene(pMaterial);
		if (index == -1) continue;
		
		//如果是原场景，而且没有克隆孩子
		if (m_pCloneScene == NULL && clonedScene.size() == 0)  
		{
			m_pScene->m_vShader.erase(m_pScene->m_vShader.begin() + index);
			SAFE_RELEASE(pMaterial);
			SAFE_DELETE(pMaterial);
		}	

	}

	//贴图有关的成员变量----------------------------------------------
	int texCnt = texture.size();
	std::vector<CTexture*>::iterator ppTexture = texture.begin();
	for (i = 0; i< texCnt; i++)
	{
		CTexture* pTexture = *(ppTexture + i);

		//从主场景中删除
		int index = GetTextureIndexInMainScene(pTexture);
		if (index == -1) continue;

		//如果是原场景，而且没有克隆孩子
		if (m_pCloneScene == NULL && clonedScene.size() == 0)  
		{
			m_pScene->m_vTexture.erase(m_pScene->m_vTexture.begin() + index);
			SAFE_RELEASE(pTexture);
			SAFE_DELETE(pTexture);
		}
	}

	int cubeMapCnt = cubeMap.size();
	std::vector<CCubeMap*>::iterator ppCubeMap = cubeMap.begin();
	for (i = 0; i< cubeMapCnt; i++)
	{
		CCubeMap* pCubeMap = *(ppCubeMap + i);

		//从场景对列中删除
		int index = GetCubeMapIndexInMainScene(pCubeMap);
		if (index != -1)
			m_pScene->cubeMap.erase(m_pScene->cubeMap.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pCubeMap);
		SAFE_DELETE(pCubeMap);
	}
	
	/*
	int movieTexCnt = movieTexture.size();
	CMovieTexture** ppMovieTexture = (CMovieTexture**)movieTexture.begin();
	for (i = 0; i< movieTexCnt; i++)
	{
		CMovieTexture* pMovieTexture = *(ppMovieTexture + i);

		//从主场景队列中删除
		int index = GetMovieTextureIndexInMainScene((DWORD)pMovieTexture);
		if (index != -1)
			m_pScene->movieTexture.erase(m_pScene->movieTexture.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pMovieTexture);
		SAFE_DELETE(pMovieTexture);
	}
	*/

	int dyTexCnt = dynamicTexture.size();
	std::vector<CDynamicTex*>::iterator ppDynamicTex = dynamicTexture.begin();
	for (i = 0; i< dyTexCnt; i++)
	{
		CDynamicTex* pDynamicTex = *(ppDynamicTex + i);

		//从主场景队列中删除
		int index = GetDynamicTextureIndexInMainScene(pDynamicTex);
		if (index != -1)
			m_pScene->dynamicTexture.erase(m_pScene->dynamicTexture.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pDynamicTex);
		SAFE_DELETE(pDynamicTex);
	}

	//摄像机有关的成员变量--------------------------------------------
	int camCnt = camera.size();
	std::vector<CCamera*>::iterator ppCamera = camera.begin();
	for (i = 0; i< camCnt; i++)
	{
		CCamera* pCamera = *(ppCamera + i);

		//如果删除的相机是当前相机,就将默认相机设置为当前相机
// 		if (m_pScene->m_pCurCamera == pCamera)
// 		{
// 			//获取当前Camera的状态
// 			m_pScene->defaultCam.SetFrom(&m_pScene->currentCam->GetFrom());
// 			m_pScene->defaultCam.SetAt(&m_pScene->currentCam->GetAt());
// 			m_pScene->defaultCam.m_nearClip = m_pScene->currentCam->m_nearClip;
// 			m_pScene->defaultCam.m_farClip = m_pScene->currentCam->m_farClip;
// 			m_pScene->defaultCam.m_fov = m_pScene->currentCam->m_fov;
// 			m_pScene->defaultCam.m_roll = m_pScene->currentCam->m_roll;
// 			m_pScene->defaultCam.m_up = m_pScene->currentCam->m_up;
// 			m_pScene->currentCam = &(m_pScene->defaultCam);
// 		}

		//从主场景队列中删除
		int index = GetCameraIndexInMainScene(pCamera);
		if (index != -1)
			m_pScene->m_vCamera.erase(m_pScene->m_vCamera.begin() + index);

		//删除节点
		SAFE_RELEASE(pCamera->m_pNode);
		SAFE_DELETE(pCamera->m_pNode);

		//删除对象
		SAFE_RELEASE(pCamera);
		SAFE_DELETE(pCamera);
	}

	//灯光有关的成员变量----------------------------------------------
	int lightCnt = light.size();
	std::vector<CLight*>::iterator ppLight = light.begin();
	for (i = 0; i< lightCnt; i++)
	{
		CLight* pLight = *(ppLight + i);
		//把灯光关掉
		pLight->SetLightEnable(false);
	}
	//删除灯光对象
	for (i = 0; i< lightCnt; i++)
	{
		CLight* pLight = *(ppLight + i);

		//从主场景队列中删除
		int index = GetLightIndexInMainScene(pLight);
		if (index != -1)
			m_pScene->m_vLight.erase(m_pScene->m_vLight.begin() + index);

		//删除节点
		SAFE_RELEASE(pLight->m_pNode);
		SAFE_DELETE(pLight->m_pNode);

		//删除对象
		SAFE_RELEASE(pLight);
		SAFE_DELETE(pLight);
	}

	/*
	//Overlay有关的成员变量-------------------------------------------
	int overlayCnt = overlay.size();
	COverlay** ppOverlay = overlay.begin();
	for (i = 0; i< overlayCnt; i++)
	{
		COverlay* pOverlay = *(ppOverlay + i);

		//从主场景队列中删除
		int index = GetOverlayIndexInMainScene(pOverlay);
		if (index != -1)
			m_pScene->overlay.erase(m_pScene->overlay.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pOverlay);
		SAFE_DELETE(pOverlay);
	}

	//Image有关的成员变量----------------------------------------------
	int imageCnt = image.size();
	CImage** ppImage = image.begin();
	for (i = 0; i< imageCnt; i++)
	{
		CImage* pImage = *(ppImage + i);

		//从主场景队列中删除
		int index = GetImageIndexInMainScene(pImage);
		if (index != -1)
			m_pScene->image.erase(m_pScene->image.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pImage);
		SAFE_DELETE(pImage);
	}

	//Navigater有关的成员变量------------------------------------------
	int navCnt = navigater.size();
	CNavigator** ppNavigater = navigater.begin();
	for (i = 0; i< navCnt; i++)
	{
		CNavigater* pNavigater = *(ppNavigater + i);

		//从主场景队列中删除
		int index = GetNavigaterIndexInMainScene(pNavigater);
		if (index != -1)
			m_pScene->navigater.erase(m_pScene->navigater.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pNavigater);
		SAFE_DELETE(pNavigater);
	}

	//Button有关的成员变量---------------------------------------------
	int buttonCnt = button.size();
	CButton** ppButton = button.begin();
	for (i = 0; i< buttonCnt; i++)
	{
		CButton* pButton = *(ppButton + i);

		//从主场景队列中删除
		int index = GetButtonIndexInMainScene(pButton);
		if (index != -1)
			m_pScene->button.erase(m_pScene->button.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pButton);
		SAFE_DELETE(pButton);
	}

	//文字相关变量
	int textCnt = text.size();
	CText** ppText = text.begin();
	for (i = 0; i< textCnt; i++)
	{
		CText* pText = *(ppText + i);

		//从主场景队列中删除
		int index = GetTextIndexInMainScene(pText);
		if (index != -1)
			m_pScene->text.erase(m_pScene->text.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pText);
		SAFE_DELETE(pText);
	}

	//音频相关的成员变量-----------------------------------------------
	int audioPlayerCnt = audio.size();
	CAudioPlayer** ppAudioPlayer = (CAudioPlayer**)audio.begin();
	for (i = 0; i< audioPlayerCnt; i++)
	{
		CAudioPlayer* pAudioPlayer = *(ppAudioPlayer + i);

		//从主场景队列中删除
		int index = GetAudioIndexInMainScene((DWORD)pAudioPlayer);
		if (index != -1)
			m_pScene->audio.erase(m_pScene->audio.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pAudioPlayer);
		SAFE_DELETE(pAudioPlayer);
	}

	//控制类对象-----------------------------------------------------
	//行走漫游对象
	int walkerCnt = walker.size();
	CWalker** ppWalker = walker.begin();
	for (i = 0; i< walkerCnt; i++)
	{
		CWalker* pWalker = *(ppWalker + i);

		//如果当前的Walker被删除
		if (pWalker == m_pScene->currentWalker)
			m_pScene->currentWalker = NULL;

		//从主场景队列中删除
		int index = GetWalkerIndexInMainScene(pWalker);
		if (index != -1)
			m_pScene->walker.erase(m_pScene->walker.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pWalker);
		SAFE_DELETE(pWalker);
	}
	
	int editorCnt = editor.size();
	CEditor** ppEditor = editor.begin();
	for (i = 0; i< editorCnt; i++)
	{
		CEditor* pEditor = *(ppEditor + i);

		//如果当前的Editor被删除
		if (pEditor == m_pScene->currentEditor)
			m_pScene->currentEditor = NULL;

		//从主场景队列中删除
		int index = GetEditorIndexInMainScene(pEditor);
		if (index != -1)
			m_pScene->editor.erase(m_pScene->editor.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pEditor);
		SAFE_DELETE(pEditor);
	}
	
	int flyerCnt = flyer.size();
	CFlyer** ppFlyer = flyer.begin();
	for (i = 0; i< flyerCnt; i++)
	{
		CFlyer* pFlyer = *(ppFlyer + i);

		//如果当前的Flyer被删除
		if (pFlyer == m_pScene->currentFlyer)
			m_pScene->currentFlyer = NULL;

		//从主场景队列中删除
		int index =  GetFlyerIndexInMainScene(pFlyer);
		if (index != -1)
			m_pScene->flyer.erase(m_pScene->flyer.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pFlyer);
		SAFE_DELETE(pFlyer);
	}
	*/

	//清除克隆的场景列表
	clonedScene.clear();
}
