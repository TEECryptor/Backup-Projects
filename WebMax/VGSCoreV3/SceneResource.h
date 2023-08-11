// SceneResource.h: interface for the CSubScene class.
//
//////////////////////////////////////////////////////////////////////


//为了可以导入多个N3D文件（一个N3D文件代表一个SubScene），并且实现对单个SubScene的管理，所以创建了SubScene类型
//SubScene主要实现该N3D文件用到的资源的管理，例如模型，材质，贴图等
//SubScene用到的模型，材质，贴图都和其它的场景不相关，释放时把该SubScene中的用到的所有资源全部释放

#ifndef _CSceneResource_H
#define _CSceneResource_H


#include <stdio.h>
#include "CScene.h"

#include <vector>
using std::vector;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSectionMap.h"

class VGS_EXPORT CSceneResource  
{
public:
	FLOAT					m_fileVersion;					//当前N3D文件的版本号

	std::string				name;							//名称

	CSceneResource*			m_pCloneScene;					//被克隆的原场景
	vector<CSceneResource*> clonedScene;					//该场景克隆的新场景列表

	D3DXMATRIX				m_transform;					//场景的变换矩阵

	CScene*					m_pScene;						//主场景对象
	//模型有关的成员变量----------------------------------------------
	vector<CModel*>			model;							//所有模型渲染队列


	//模型组有关的变量------------------------------------------------
	vector<CModelGroup*>	modelGroup;						//模型组

	//剖面图----------------------------------------------------------
	vector<CSectionMap*>    sectionMap;						//剖面图

	//材质有关的成员变量----------------------------------------------
	vector<CMaterial*>		shader;							//材质资源队列

	//贴图有关的成员变量----------------------------------------------
	vector<CTexture*>		texture;						//贴图资源队列
	vector<CCubeMap*>		cubeMap;						//CubeMap对列
	vector<DWORD>			movieTexture;					//movieTex队列
	vector<CDynamicTex*>	dynamicTexture;					//动态贴图

	//摄像机有关的成员变量--------------------------------------------
	vector<CCamera*>		camera;							//摄像机列表

	//灯光有关的成员变量----------------------------------------------
	vector<CLight*>			light;							//灯光列表

	//Overlay有关的成员变量-------------------------------------------
	vector<COverlay*>		overlay;						//Overlay列表

	//Image有关的成员变量----------------------------------------------
	vector<CImage*>			image;							//image列表

	//Navigater有关的成员变量------------------------------------------
	vector<CNavigator*>		navigater;						//navigaters列表

	//Button有关的成员变量---------------------------------------------
	vector<CButton*>		button;							//button列表

	//文字相关的成员变量-----------------------------------------------
	vector<CFont*>			font;							//字体对象
	vector<CText*>			text;							//文字列表

	//音频相关的成员变量-----------------------------------------------
	vector<DWORD>			audio;							//音频队列

	//控制类对象-----------------------------------------------------
	vector<CWalker*>		walker;							//行走漫游对象
	vector<CEditor*>		editor;							//物件展示控制对象
	vector<CFlyer*>			flyer;							//飞翔漫游对象

	//节点类对象
	CSceneNode*					m_pRootNode;					//根节点


public:
	//获得资源在主场景资源列表中的编号
	int GetModelIndexInMainScene(CModel* pModel);

	int GetModelGroupIndexInMainScene(CModelGroup* pModelGroup);

	int GetSectionMapIndexInMainScene(CSectionMap* pSectionMap);

	int GetMaterialIndexInMainScene(CMaterial* pMaterial);

	int GetTextureIndexInMainScene(CTexture* pTexture);
	
	int GetCubeMapIndexInMainScene(CCubeMap* pModel);

	int GetMovieTextureIndexInMainScene(DWORD pMovieTexture);

	int GetDynamicTextureIndexInMainScene(CDynamicTex* pDynamicTexture);

	int GetCameraIndexInMainScene(CCamera* pCamera);

	int GetLightIndexInMainScene(CLight* pLight);

	int GetOverlayIndexInMainScene(COverlay* pOverlay);
	
	int GetImageIndexInMainScene(CImage* pImage);

	int GetNavigaterIndexInMainScene(CNavigator* pNavigater);
	
	int GetButtonIndexInMainScene(CButton* pButton);

	int GetFontIndexInMainScene(CFont* pFont);

	int GetTextIndexInMainScene(CText* pText);

	int GetAudioIndexInMainScene(DWORD pAudio);

	int GetWalkerIndexInMainScene(CWalker* pWalker);

	int GetEditorIndexInMainScene(CEditor* pEditor);

	int GetFlyerIndexInMainScene(CFlyer* pFlyer);

	//递归克隆场景中的对象
	void CloneNodeObject(CSceneResource* pNewSceneResource, CSceneNode* pNewNode, CSceneNode* pOldNode);

	//克隆材质和贴图列表
	void CloneShaderList(CSceneResource* pNewSceneResource);
	void CloneTextureList(CSceneResource* pNewSceneResource);

	//克隆当前的场景资源并加载到场景
	CSceneResource*	Clone(std::string newSceneName);

	//重新设置克隆孩子场景的属性
	void ResetClonedSceneProp();
	//将使用pMtrl材质的SubMesh规整到pMtrl->subMeshList中
	void PushSubMeshToShader(CMaterial* pMtrl);

	//构造场景中所有材质的SubMesh列表
	void ConstructShaderSubMeshList();
	//释放资源方法
	void Release();

	void setMatrix(D3DXMATRIX mat){m_transform = mat;}  // 设置场景根节点的变换矩阵

public:
	CSceneResource(std::string strName, CScene* pScene);
	virtual ~CSceneResource();

};

#endif // !defined(AFX_SUBSCENE_H__78609289_B68D_4D4B_8825_D3CE3862F0F8__INCLUDED_)
