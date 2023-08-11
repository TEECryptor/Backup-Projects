// SceneResource.cpp: implementation of the CSubScene class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SceneResource.h"
//���ŵ�Ӱ�ļ�����
//#include "CMovieTexture.h"
//#include "CAudioPlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSceneResource::CSceneResource(std::string strName, CScene* pScene)
{
	m_pScene = pScene;
	m_fileVersion = 2.0f;					//��ǰN3D�ļ��İ汾��
	name = strName;
	m_pCloneScene = NULL;					//���������ͣ����½��Ļ��ǿ�¡��
	m_pRootNode = NULL;
	D3DXMatrixIdentity(&m_transform);     // ��ʼ�����ڵ�ı任����
}

CSceneResource::~CSceneResource()
{

}

//�����Դ����������Դ�б��еı��
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

//�ݹ��¡�����еĶ���
void CSceneResource::CloneNodeObject(CSceneResource* pNewSceneResource, CSceneNode* pNewNode, CSceneNode* pOldNode)
{
// 	int childNodeCnt = pOldNode->m_child.size();
// 	CSceneNode** ppNode = pOldNode->m_child.begin();
// 	if (childNodeCnt == 0) return;
// 	
// 	for (int i = 0; i< childNodeCnt; i++)
// 	{
// 		//�����Ƶĳ�����Node
// 		CSceneNode* iNode = *(ppNode + i);
// 		
// 		//�½�һ��Node���µĳ�����Դ����
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
// 		//�������ӹ�ϵ
// 		iNewNode->m_parent = pNewNode;
// 		pNewNode->m_child.push_back(iNewNode);
// 
// 		//��¡�ýڵ�����Ķ���
// 		switch(iNode->type)
// 		{
// 			//ģ�ͽڵ�
// 		case NNT_MODEL:
// 			{
// 				CModel* pOldModel = (CModel*)iNode->m_object;
// 				std::string strNewName = pOldModel->name;
// 				strNewName.append("_clone");
// 				CModel* pNewModel = pOldModel->clone(strNewName);
// 				pNewModel->pNode = iNewNode;
// 				//���볡����Դ�����б�
// 				pNewModel->index = pNewSceneResource->m_pScene->model.size();
// 				pNewSceneResource->model.push_back(pNewModel);
// 				pNewSceneResource->m_pScene->model.push_back(pNewModel);
// 				//�½ڵ������
// 				iNewNode->name = strNewName;
// 			}
// 			break;
// 
// 			//����ڵ�
// 		case NNT_CAMERANODE:
// 			{
// 				CCamera* pOldCamera = (CCamera*)iNode->m_object;
// 				std::string strNewName = pOldCamera->name;
// 				strNewName.append("_clone");
// 				CCamera* pNewCamera = pOldCamera->clone(strNewName);
// 				pNewCamera->pNode = iNewNode;
// 				//���볡����Դ�����б�
// 				pNewCamera->index = pNewSceneResource->m_pScene->m_vCamera.size();
// 				pNewSceneResource->camera.push_back(pNewCamera);
// 				pNewSceneResource->m_pScene->m_vCamera.push_back(pNewCamera);
// 				//�½ڵ������
// 				iNewNode->name = strNewName;
// 			}
// 			break;
// 
// 			//�ƹ�ڵ�
// 		case NNT_LIGHTNODE:
// 			{
// 				CLight* pOldLight = (CLight*)iNode->m_object;
// 				std::string strNewName = pOldLight->name;
// 				strNewName.append("_clone");
// 				CLight* pNewLight = pOldLight->clone(strNewName);
// 				pNewLight->pNode = iNewNode;
// 				//���볡����Դ�����б�
// 				pNewLight->index = pNewSceneResource->m_pScene->m_vLight.size();
// 				pNewSceneResource->light.push_back(pNewLight);
// 				pNewSceneResource->m_pScene->m_vLight.push_back(pNewLight);
// 				//���õƹ��״̬
// 				pNewLight->SetLightEnable(true);
// 				//�½ڵ������
// 				iNewNode->name = strNewName;
// 			}
// 			break;
// 		}
// 
// 		//�ݹ�
// 		CloneNodeObject(pNewSceneResource, iNewNode, iNode);
// 	}
}

//��¡��ǰ�ĳ�����Դ�����ص�����
CSceneResource*	CSceneResource::Clone(std::string newSceneName)
{
	//���ճ���ͼ�����п�¡
	if (m_pRootNode == NULL) return NULL;
	
	//ע�⣬���ʺ���ͼ����Ƶ��Image����¡�����Թ���
	CSceneResource* pNewSceneResource = new CSceneResource(newSceneName, m_pScene);
	pNewSceneResource->m_pCloneScene = this;

	//�³�����RootNode
	pNewSceneResource->m_pRootNode = new CSceneNode();
	// pNewSceneResource->m_pRootNode->m_Type = NNT_SCENEROOT;
	pNewSceneResource->m_pRootNode->name = newSceneName;

	//�ݹ���г�������Ŀ�¡
	CloneNodeObject(pNewSceneResource, pNewSceneResource->m_pRootNode, m_pRootNode);

	//��¡���ʺ���ͼ�б�
	CloneShaderList(pNewSceneResource);
	CloneTextureList(pNewSceneResource);

	//�����¡�б�
	this->clonedScene.push_back(pNewSceneResource);

	//���¹�����ʵ�SubMeshList�б�
	ConstructShaderSubMeshList();	

	return pNewSceneResource;
}

//��¡�����б�
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

//��¡��ͼ�б�
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

//����ͷŵ��ǿ�¡Դ��������ʣ�µĺ��ӳ����ĵ�һ����Ϊ��¡Դ����
void CSceneResource::ResetClonedSceneProp()
{
	//������ǿ�¡�����ĳ���
	if (m_pCloneScene == NULL)
	{
		int clonedSceneCnt = clonedScene.size();
		if (clonedSceneCnt > 0)
		{
			std::vector<CSceneResource*>::iterator ppSceneResource = clonedScene.begin();
			//����һ����������Ϊ��¡Դ
			CSceneResource* firstClonedSceneResource = *ppSceneResource;
			firstClonedSceneResource->m_pCloneScene = NULL;
			//���뺢�ӽڵ�
			firstClonedSceneResource->clonedScene.clear();
			for (int i = 1; i< clonedSceneCnt; i++)
			{
				CSceneResource* pCurSceneResource = *(ppSceneResource + i);
				pCurSceneResource->m_pCloneScene = firstClonedSceneResource;
				firstClonedSceneResource->clonedScene.push_back(pCurSceneResource);
			}
		}
	}
	else //�ǿ�¡�����ĳ���
	{
		//��ǰ������ԭ����
		CSceneResource* pSrcSceneResource = this->m_pCloneScene;
		//����ǰ������ԭ�����Ŀ�¡����ɾ��
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
// ������: ConstructShaderSubMeshList()
// ��  ��: 
// ��  ��: ���쳡�������в��ʵ�SubMesh�б�
//-----------------------------------------------------------------------------
void CSceneResource::ConstructShaderSubMeshList()
{
	int shaderCnt = shader.size();
	std::vector<CMaterial*>::iterator ppMtrl = shader.begin();
	for (int i = 0; i< shaderCnt; i++)
	{
		CMaterial* pMtrl = *(ppMtrl + i);
		
		//��ʹ��pMtrl���ʵ�SubMesh������pMtrl->subMeshList��
		pMtrl->subMeshList.clear();
		PushSubMeshToShader(pMtrl);
	}
}

//-----------------------------------------------------------------------------
// ������: PushSubMeshToShader()
// ��  ��: 
// ��  ��: ��ʹ��pMtrl���ʵ�SubMesh������pMtrl->subMeshList��
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

//�ͷ���Դ����
void CSceneResource::Release()
{
	//����ͷŵ��ǿ�¡Դ��������ʣ�µĺ��ӳ����ĵ�һ����Ϊ��¡Դ����
	ResetClonedSceneProp();

	//ģ���йصĳ�Ա����----------------------------------------------
	int modelCnt = model.size();
	std::vector<CModel*>::iterator ppModel = model.begin();
	for (int i = 0; i< modelCnt; i++)
	{
		CModel* pModel = *(ppModel + i);

		//��������ģ�Ͷ�����ɾ��
		int index = GetModelIndexInMainScene(pModel);
		if (index != -1)
			m_pScene->model.erase(m_pScene->model.begin() + index);

		//ɾ���ڵ�
		SAFE_RELEASE(pModel->m_pNode);
		SAFE_DELETE(pModel->m_pNode);

		//�ͷ���Դ//�����ԭ����������û�п�¡����
		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pModel);

		SAFE_DELETE(pModel);
	}	

	//����ͼ----------------------------------------------------------
	int sectionMapCnt = sectionMap.size();
	std::vector<CSectionMap*>::iterator ppSectionMap = sectionMap.begin();
	for (i = 0; i< sectionMapCnt; i++)
	{
		CSectionMap* pSectionMap = *(ppSectionMap + i);

		//����������ɾ��
		int index = GetSectionMapIndexInMainScene(pSectionMap);
		if (index != -1)
			m_pScene->sectionMap.erase(m_pScene->sectionMap.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0) 
			SAFE_RELEASE(pSectionMap);
		SAFE_DELETE(pSectionMap);
	}

	//�����йصĳ�Ա����----------------------------------------------
	int shaderCnt = shader.size();
	std::vector<CMaterial*>::iterator ppMaterial = shader.begin();
	for (i = 0; i< shaderCnt; i++)
	{
		CMaterial* pMaterial = *(ppMaterial + i);

		//����������ɾ��
		int index = GetMaterialIndexInMainScene(pMaterial);
		if (index == -1) continue;
		
		//�����ԭ����������û�п�¡����
		if (m_pCloneScene == NULL && clonedScene.size() == 0)  
		{
			m_pScene->m_vShader.erase(m_pScene->m_vShader.begin() + index);
			SAFE_RELEASE(pMaterial);
			SAFE_DELETE(pMaterial);
		}	

	}

	//��ͼ�йصĳ�Ա����----------------------------------------------
	int texCnt = texture.size();
	std::vector<CTexture*>::iterator ppTexture = texture.begin();
	for (i = 0; i< texCnt; i++)
	{
		CTexture* pTexture = *(ppTexture + i);

		//����������ɾ��
		int index = GetTextureIndexInMainScene(pTexture);
		if (index == -1) continue;

		//�����ԭ����������û�п�¡����
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

		//�ӳ���������ɾ��
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

		//��������������ɾ��
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

		//��������������ɾ��
		int index = GetDynamicTextureIndexInMainScene(pDynamicTex);
		if (index != -1)
			m_pScene->dynamicTexture.erase(m_pScene->dynamicTexture.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pDynamicTex);
		SAFE_DELETE(pDynamicTex);
	}

	//������йصĳ�Ա����--------------------------------------------
	int camCnt = camera.size();
	std::vector<CCamera*>::iterator ppCamera = camera.begin();
	for (i = 0; i< camCnt; i++)
	{
		CCamera* pCamera = *(ppCamera + i);

		//���ɾ��������ǵ�ǰ���,�ͽ�Ĭ���������Ϊ��ǰ���
// 		if (m_pScene->m_pCurCamera == pCamera)
// 		{
// 			//��ȡ��ǰCamera��״̬
// 			m_pScene->defaultCam.SetFrom(&m_pScene->currentCam->GetFrom());
// 			m_pScene->defaultCam.SetAt(&m_pScene->currentCam->GetAt());
// 			m_pScene->defaultCam.m_nearClip = m_pScene->currentCam->m_nearClip;
// 			m_pScene->defaultCam.m_farClip = m_pScene->currentCam->m_farClip;
// 			m_pScene->defaultCam.m_fov = m_pScene->currentCam->m_fov;
// 			m_pScene->defaultCam.m_roll = m_pScene->currentCam->m_roll;
// 			m_pScene->defaultCam.m_up = m_pScene->currentCam->m_up;
// 			m_pScene->currentCam = &(m_pScene->defaultCam);
// 		}

		//��������������ɾ��
		int index = GetCameraIndexInMainScene(pCamera);
		if (index != -1)
			m_pScene->m_vCamera.erase(m_pScene->m_vCamera.begin() + index);

		//ɾ���ڵ�
		SAFE_RELEASE(pCamera->m_pNode);
		SAFE_DELETE(pCamera->m_pNode);

		//ɾ������
		SAFE_RELEASE(pCamera);
		SAFE_DELETE(pCamera);
	}

	//�ƹ��йصĳ�Ա����----------------------------------------------
	int lightCnt = light.size();
	std::vector<CLight*>::iterator ppLight = light.begin();
	for (i = 0; i< lightCnt; i++)
	{
		CLight* pLight = *(ppLight + i);
		//�ѵƹ�ص�
		pLight->SetLightEnable(false);
	}
	//ɾ���ƹ����
	for (i = 0; i< lightCnt; i++)
	{
		CLight* pLight = *(ppLight + i);

		//��������������ɾ��
		int index = GetLightIndexInMainScene(pLight);
		if (index != -1)
			m_pScene->m_vLight.erase(m_pScene->m_vLight.begin() + index);

		//ɾ���ڵ�
		SAFE_RELEASE(pLight->m_pNode);
		SAFE_DELETE(pLight->m_pNode);

		//ɾ������
		SAFE_RELEASE(pLight);
		SAFE_DELETE(pLight);
	}

	/*
	//Overlay�йصĳ�Ա����-------------------------------------------
	int overlayCnt = overlay.size();
	COverlay** ppOverlay = overlay.begin();
	for (i = 0; i< overlayCnt; i++)
	{
		COverlay* pOverlay = *(ppOverlay + i);

		//��������������ɾ��
		int index = GetOverlayIndexInMainScene(pOverlay);
		if (index != -1)
			m_pScene->overlay.erase(m_pScene->overlay.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pOverlay);
		SAFE_DELETE(pOverlay);
	}

	//Image�йصĳ�Ա����----------------------------------------------
	int imageCnt = image.size();
	CImage** ppImage = image.begin();
	for (i = 0; i< imageCnt; i++)
	{
		CImage* pImage = *(ppImage + i);

		//��������������ɾ��
		int index = GetImageIndexInMainScene(pImage);
		if (index != -1)
			m_pScene->image.erase(m_pScene->image.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pImage);
		SAFE_DELETE(pImage);
	}

	//Navigater�йصĳ�Ա����------------------------------------------
	int navCnt = navigater.size();
	CNavigator** ppNavigater = navigater.begin();
	for (i = 0; i< navCnt; i++)
	{
		CNavigater* pNavigater = *(ppNavigater + i);

		//��������������ɾ��
		int index = GetNavigaterIndexInMainScene(pNavigater);
		if (index != -1)
			m_pScene->navigater.erase(m_pScene->navigater.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pNavigater);
		SAFE_DELETE(pNavigater);
	}

	//Button�йصĳ�Ա����---------------------------------------------
	int buttonCnt = button.size();
	CButton** ppButton = button.begin();
	for (i = 0; i< buttonCnt; i++)
	{
		CButton* pButton = *(ppButton + i);

		//��������������ɾ��
		int index = GetButtonIndexInMainScene(pButton);
		if (index != -1)
			m_pScene->button.erase(m_pScene->button.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pButton);
		SAFE_DELETE(pButton);
	}

	//������ر���
	int textCnt = text.size();
	CText** ppText = text.begin();
	for (i = 0; i< textCnt; i++)
	{
		CText* pText = *(ppText + i);

		//��������������ɾ��
		int index = GetTextIndexInMainScene(pText);
		if (index != -1)
			m_pScene->text.erase(m_pScene->text.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pText);
		SAFE_DELETE(pText);
	}

	//��Ƶ��صĳ�Ա����-----------------------------------------------
	int audioPlayerCnt = audio.size();
	CAudioPlayer** ppAudioPlayer = (CAudioPlayer**)audio.begin();
	for (i = 0; i< audioPlayerCnt; i++)
	{
		CAudioPlayer* pAudioPlayer = *(ppAudioPlayer + i);

		//��������������ɾ��
		int index = GetAudioIndexInMainScene((DWORD)pAudioPlayer);
		if (index != -1)
			m_pScene->audio.erase(m_pScene->audio.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pAudioPlayer);
		SAFE_DELETE(pAudioPlayer);
	}

	//���������-----------------------------------------------------
	//�������ζ���
	int walkerCnt = walker.size();
	CWalker** ppWalker = walker.begin();
	for (i = 0; i< walkerCnt; i++)
	{
		CWalker* pWalker = *(ppWalker + i);

		//�����ǰ��Walker��ɾ��
		if (pWalker == m_pScene->currentWalker)
			m_pScene->currentWalker = NULL;

		//��������������ɾ��
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

		//�����ǰ��Editor��ɾ��
		if (pEditor == m_pScene->currentEditor)
			m_pScene->currentEditor = NULL;

		//��������������ɾ��
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

		//�����ǰ��Flyer��ɾ��
		if (pFlyer == m_pScene->currentFlyer)
			m_pScene->currentFlyer = NULL;

		//��������������ɾ��
		int index =  GetFlyerIndexInMainScene(pFlyer);
		if (index != -1)
			m_pScene->flyer.erase(m_pScene->flyer.begin() + index);

		if (m_pCloneScene == NULL && clonedScene.size() == 0)
			SAFE_RELEASE(pFlyer);
		SAFE_DELETE(pFlyer);
	}
	*/

	//�����¡�ĳ����б�
	clonedScene.clear();
}
