// SceneResource.h: interface for the CSubScene class.
//
//////////////////////////////////////////////////////////////////////


//Ϊ�˿��Ե�����N3D�ļ���һ��N3D�ļ�����һ��SubScene��������ʵ�ֶԵ���SubScene�Ĺ������Դ�����SubScene����
//SubScene��Ҫʵ�ָ�N3D�ļ��õ�����Դ�Ĺ�������ģ�ͣ����ʣ���ͼ��
//SubScene�õ���ģ�ͣ����ʣ���ͼ���������ĳ�������أ��ͷ�ʱ�Ѹ�SubScene�е��õ���������Դȫ���ͷ�

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
	FLOAT					m_fileVersion;					//��ǰN3D�ļ��İ汾��

	std::string				name;							//����

	CSceneResource*			m_pCloneScene;					//����¡��ԭ����
	vector<CSceneResource*> clonedScene;					//�ó�����¡���³����б�

	D3DXMATRIX				m_transform;					//�����ı任����

	CScene*					m_pScene;						//����������
	//ģ���йصĳ�Ա����----------------------------------------------
	vector<CModel*>			model;							//����ģ����Ⱦ����


	//ģ�����йصı���------------------------------------------------
	vector<CModelGroup*>	modelGroup;						//ģ����

	//����ͼ----------------------------------------------------------
	vector<CSectionMap*>    sectionMap;						//����ͼ

	//�����йصĳ�Ա����----------------------------------------------
	vector<CMaterial*>		shader;							//������Դ����

	//��ͼ�йصĳ�Ա����----------------------------------------------
	vector<CTexture*>		texture;						//��ͼ��Դ����
	vector<CCubeMap*>		cubeMap;						//CubeMap����
	vector<DWORD>			movieTexture;					//movieTex����
	vector<CDynamicTex*>	dynamicTexture;					//��̬��ͼ

	//������йصĳ�Ա����--------------------------------------------
	vector<CCamera*>		camera;							//������б�

	//�ƹ��йصĳ�Ա����----------------------------------------------
	vector<CLight*>			light;							//�ƹ��б�

	//Overlay�йصĳ�Ա����-------------------------------------------
	vector<COverlay*>		overlay;						//Overlay�б�

	//Image�йصĳ�Ա����----------------------------------------------
	vector<CImage*>			image;							//image�б�

	//Navigater�йصĳ�Ա����------------------------------------------
	vector<CNavigator*>		navigater;						//navigaters�б�

	//Button�йصĳ�Ա����---------------------------------------------
	vector<CButton*>		button;							//button�б�

	//������صĳ�Ա����-----------------------------------------------
	vector<CFont*>			font;							//�������
	vector<CText*>			text;							//�����б�

	//��Ƶ��صĳ�Ա����-----------------------------------------------
	vector<DWORD>			audio;							//��Ƶ����

	//���������-----------------------------------------------------
	vector<CWalker*>		walker;							//�������ζ���
	vector<CEditor*>		editor;							//���չʾ���ƶ���
	vector<CFlyer*>			flyer;							//�������ζ���

	//�ڵ������
	CSceneNode*					m_pRootNode;					//���ڵ�


public:
	//�����Դ����������Դ�б��еı��
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

	//�ݹ��¡�����еĶ���
	void CloneNodeObject(CSceneResource* pNewSceneResource, CSceneNode* pNewNode, CSceneNode* pOldNode);

	//��¡���ʺ���ͼ�б�
	void CloneShaderList(CSceneResource* pNewSceneResource);
	void CloneTextureList(CSceneResource* pNewSceneResource);

	//��¡��ǰ�ĳ�����Դ�����ص�����
	CSceneResource*	Clone(std::string newSceneName);

	//�������ÿ�¡���ӳ���������
	void ResetClonedSceneProp();
	//��ʹ��pMtrl���ʵ�SubMesh������pMtrl->subMeshList��
	void PushSubMeshToShader(CMaterial* pMtrl);

	//���쳡�������в��ʵ�SubMesh�б�
	void ConstructShaderSubMeshList();
	//�ͷ���Դ����
	void Release();

	void setMatrix(D3DXMATRIX mat){m_transform = mat;}  // ���ó������ڵ�ı任����

public:
	CSceneResource(std::string strName, CScene* pScene);
	virtual ~CSceneResource();

};

#endif // !defined(AFX_SUBSCENE_H__78609289_B68D_4D4B_8825_D3CE3862F0F8__INCLUDED_)
