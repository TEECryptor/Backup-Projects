/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // SaveN3D.h
* ����ժҪ�� // ���N3D���ඨ��
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


#ifndef _SAVEN3D_H
#define _SAVEN3D_H
// #pragma once

#include "..\\VGSHeader.h"
#include "N3D_DefV3.h"
#include "V3D_DefV3.h"

#include "..\VGSGetRoot.h"

#include "..\\Animation\\LightKeyFrame.h"
#include "..\\Animation\\CameraKeyFrame.h"

#include "..\\gui\\CGUIManager.h"
#include "..\\gui\\CGUIObject.h"


using namespace N3D;
using namespace N3DV3;

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class CSaveN3D  : public CVGSGetRoot
{
public:

	N3D_HeaderV3			m_Header;					//N3D�ļ�ͷ

	N3D_5_1_Table           m_Table;                    //��ṹ

	N3D_SceneProp			m_SceneProp;				//��������

	N3D_AudioList			m_AudioData;				//�������ݵ���ʼλ��

	V3D_FlashList			m_FlashData;				//Flash���ݵ���ʼλ��

	VGS_CameraList			m_CameraData;				

	VGS_MtrlList    		m_MaterialData;			

	VGS_LightList			m_LightData;				//�ƹ�����

	VGS_MeshList			m_MeshData;

	V3D_TextList			m_TextData;				//��������

	V3D_OverlayList			m_OverlayData;			//Overlay����

	V3D_NavigatorList		m_NavigaterData;			//Navigater����

	V3D_ButtonList			m_ButtonData;				//��ť�б�


	V3D_Privacy				m_PrivacyData;			//��Ȩ����

	V3D_ControlData			m_ControlData;			//�û����ƶ�������
	
	UINT                    m_imageCount;             //������ʹ�õ���ͼ�ĸ���
private:

	CScene*	            m_pVGSSceneMgr;			    //ָ��VGS������������ָ��

	FILE*				m_pFile;					//ָ��N3D�ļ���ָ��

	// �洢����������Ϣ����ʱ�ļ�����ѹ��
    FILE*               m_OriSceneBufferFile;

	// �洢ͼ�����ݣ�����������ͼ�Լ�δʹ�õ�ͼ��
	// FILE*               m_ImagesBufferFile;

	// ѹ���󳡾�������Ϣ����ʱ�ļ�
    FILE*               m_ZipSceneBufferFile;

	DWORD				m_nFileSize;				//N3D�ļ��Ĵ�С

	CHAR				m_FileName[256];			//N3D�ļ���

public:

	//д���ļ�ͷ
	VOID		WriteHeader();

    //Add by baojiahui ������ʱ�ĳ��������ļ�������player����������������V3D�еĻ�������
	bool CreateSceneDataBuffer();
    
	void        WriteFileTable();

	//д�봰������
	VOID		WriteWindowProp();
	
	//д����������
	VOID		WriteAudioData();
	//д��Flash����
	VOID		WriteFlashData();

	//д���������Ϣ
	VOID		WriteCameraData();
	//д�������Ϣ
	VOID		WriteMaterialData();
	
	//Add by Baojiahui ����ͼ��Ϣд����Ӧ��buffer�ļ������ڳ����ļ��б�����ͼ��������
	// VOID        WriteImageInfo(const UINT& imgIndex, /*const TextureUnitState *pTextUnit,const UINT& frame,*/ const char* texName);
	//VOID        WriteTextureData();
	//д��ƹ���Ϣ
	VOID		WriteLightData();

	//д��Mesh��Ϣ
	// VOID		WriteMeshData();
	VOID		WriteModelData();
	VOID        WriteAModel(CModel *pModel);


	//д��������Ϣ
	VOID		WriteTextData();
	//д��Overlay��Ϣ
	VOID		WriteOverlayData();

	// д��һ��overlay��Ϣ
	void WriteOverlayInfo(COverlay *pOverlay, BOOL bIsBackGround = FALSE);

	// д�뱳��ͼ
	// VOID        WriteBackGround();

	//д��Navigater��Ϣ
	VOID		WriteNavigaterData();
	//д��Button��Ϣ
	VOID		WriteButtonData();
	
	// VOID        WriteAnimation();

	// д�볡��ͼ
	VOID        WriteSceneGraph();

	//���쳡��ͼ����
	VOID		ConstructSceneGraph(CSceneNode* pNode);


	//д���ļ��汾���ͺ���Ȩ��ʾ
	VOID		WritePrivacyData();

	//д����ƶ�������
	VOID		WriteControlData();

	VOID		WriteImageData();  // д��ͼ������
   
	HRESULT     ZipAndWriteSceneData();

	//�ͷ���Դ
	VOID		Release();

	//�����Ӧ��Scene����ΪE3D�ļ�
	HRESULT		SaveSceneAsN3D(const CScene* pVGSSceneMgr, const CHAR* strFileName, BOOL zipStatus = true, DWORD fVersion = N3D_CURRENT_V3DVERSION);	

	//�ڱ���֮ǰ��Ϊ�����������ñ�ţ���ֹ��Ϊɾ������󣬵��±�������Ŵ���

	VOID		ResetModelIndex();

	VOID		ResetTextureIndex();

	VOID		ResetBumpWaterIndex();

	VOID		ResetCubeMapIndex();

	VOID		ResetMovieTexIndex();

	VOID		ResetMaterialIndex();

	VOID		ResetOverlayIndex();

	VOID		ResetDyTexIndex();

	VOID		ResetCameraIndex();

	VOID		ResetLightIndex();

	VOID		ResetNavigaterIndex();

	VOID		ResetTextIndex();

	//�����ƶ������±��
	VOID		ResetWalkerIndex();

	VOID		ResetEditorIndex();

	VOID		ResetFlyerIndex();

	VOID		ResetButtonIndex();

	VOID		ResetImageIndex();

	
	//���һ���ڵ���ӽڵ�����
	UINT			GetChildNodeCount(CSceneNode* pNode);
	//ͨ������������������б��л�����������������
	//CameraParamAnimation* GetCameraParamAnimation(std::vector<CameraParamAnimation*> list, Ogre::Camera* pCam);
	//ͨ���ƹ����ӵƹ⶯���б��л�õƹ������������
	//LightParamAnimation* GetLightParamAnimation(std::vector<LightParamAnimation*> list, Ogre::Light* pLight);


public:

	CSaveN3D();

	~CSaveN3D();

private:
	
	//std::vector<SceneGraphNode> m_VSceneGraph;  // ����ͼ���ݽṹ
	
	// void WriteAMesh(const MeshPtr &ptrMesh);                                // д��һ��Mesh������
	// NodeObject GetNodeObject(const MovableObject *pObject);          
	void CreateSceneGraphNode(int ParentIdx, CSceneNode *pNode);             // ����һ���ڵ㼰���ӽڵ�ĳ���ͼ����
	void GetNodeTypeAndIndex(CSceneNode* pSceneNode, N3D_Node* pN3DNode); 

	// UINT GetVGSMtrlIndex(String materialName);

	// ���˵�Ԥ�����������
	void FilterCamera();

	// ��ȡ�������
	int GetCameraIndex(CCamera *pCamera);

	vector<CCamera*>       m_vpCamera;		// �޳���Ԥ�����������������б�

	vector<std::string>  m_vImageList;		// д��N3D��Image�ļ������б�,�����ļ�·��

	vector<DWORD>         m_vMaterialList;  // д��N3D�Ĳ����б�

	vector<DWORD>         m_vEntityList;    // д���ģ���б�

	vector<CLight*>		  m_vpLight;  // д��ĵƹ��б�

	// ��ȡtexture������
	// TextureLayerType GetTextureLayerType(int mapchanel);
	N3D_TextureType GetTextureLayerType(CTextureLayer *pState);

	// ��ȡtexture���͵�����
	// std::string GetTextureLayerTypeDes(N3D_TextureType type);

	// ��ʼ��Image�б�
	void InitImageList();

	// д��texturelayer�Ĳ���
	void WriteTexturealayerParams(CTextureLayer *pState, N3D_TextureType type, FILE *pFile);

	UINT m_PureOverlayCount;        // ������Overlay��Ŀ��������������ά����ʹ�õ���overlay
	UINT m_BackdropCount;           // ����ͼ��Ŀ��0 or 1

	// ����N3D�б����Button��ʹ�õ�overlay��Ϣ
	void GetButtonInfo();

	typedef struct N3D_ButtonOverlay
	{
		CHAR		   Name[32];			// ��ť�ĵ�����
		N3D_OverlayV3  OverlayInfo;

		int            DownMaterialIndex;
		int            OverMaterialIndex;
		std::string    sCallBackFunc;
	}N3D_ButtonOverlay;

	vector <N3D_ButtonOverlay> m_vButtonInfo;  // Ҫ���浽N3D�еİ�ť������Ϣ

	// ����N3D�б���ĵ���ͼ��ʹ�õ�overlay��Ϣ
	void GetNavigatorInfo();

	typedef struct N3D_NaviPointerOverlay
	{
		N3D_OverlayV3    OverlayInfo;

		CHAR		Name[32];			//Pointer������
		int			Type;				//Pointer�����ͣ�Camera-0, Model-1
		int			ObjIndex;			//�����ģ�͵ı��

	}N3D_NaviPointerOverlay;
	typedef struct N3D_NaviOverlay
	{
		CHAR		                Name[32];			// ����ͼ�ĵ�����
		FLOAT						sceneLeft;			// ����TopView��Rect
		FLOAT						sceneTop;
		FLOAT						sceneRight;
		FLOAT						sceneBottom;

		N3D_OverlayV3    OverlayInfo;
		vector <N3D_NaviPointerOverlay> vPointer;

	}N3D_NaviOverlay;

	vector <N3D_NaviOverlay> m_vNaviInfo;  // Ҫ���浽N3D�еĵ���ͼ������Ϣ

	//  ��ȡ�����õ��Ĳ�����Ŀ��������ά�������ά����
	void GetAllMaterials();

	// �Ӳ��ʶ����в��Ҳ��ʵı�ţ��Ҳ�������-1
	int FindMaterial(DWORD pMaterial);

	//-------------------------------------------------------------------
	//����: GetN3DTexOP()
	//����: 
	//��;: ��D3D�Ļ�ɫģʽ����ת����N3D�Ļ�ɫģʽ����
	//-------------------------------------------------------------------
	N3D_BlenderOP GetN3DTexOP(const D3DTEXTUREOP& d3d_BlenderOP);

};



//------------------------------------------------------------------------
#endif
