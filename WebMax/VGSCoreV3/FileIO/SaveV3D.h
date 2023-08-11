#pragma once

#include "V3D_DefV3.h"
#include "..\VGSHeader.h"
#include "..\VGSGetRoot.h"
#include "..\CCamera.h"
#include "..\CLight.h"
#include "..\CModel.h"
#include "..\CScene.h"
#include "..\Node.h"
#include "..\CMaterial.h"
#include "..\\Animation\\LightKeyFrame.h"
#include "..\\Animation\\CameraKeyFrame.h"

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class CSaveV3D : public CVGSGetRoot
{
public:

	V3D_Header				m_Header;					//V3D�ļ�ͷ

	V3D_Table               m_Table;                    //��ṹ

	V3D_SceneProp			m_SceneProp;				//��������
	

	V3D_AudioList			m_AudioData;				//�������ݵ���ʼλ��

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


private:

	CScene*	           m_pVGSSceneMgr;			    //ָ��VGS������������ָ��

	FILE*				m_pFile;					//ָ��V3D�ļ���ָ��

	DWORD				m_nFileSize;				//V3D�ļ��Ĵ�С

	CHAR				m_FileName[256];			//V3D�ļ���

public:

	//д���ļ�ͷ
	DWORD		WriteHeader();

	DWORD        WriteFileTable();

	//д�봰������
	DWORD		WriteWindowProp();
	
	//д����������
	DWORD		WriteAudioData();
	//д��Flash����
	DWORD		WriteFlashData();

	//д���������Ϣ
	DWORD		WriteCameraData();
	//д�������Ϣ
	DWORD		WriteMaterialData();
	DWORD        WriteTextureData();
	//д��ƹ���Ϣ
	DWORD		WriteLightData();

	// ����Entity��Ϣ
	DWORD       WriteModelData();

	//д��Mesh��Ϣ,mesh��Entity���õ���Դ�����entity���ܻ�����ͬһ��mesh��Դ
	DWORD		WriteAModel(CModel *pModel);

	//д��������Ϣ
	DWORD		WriteTextData();
	//д��Overlay��Ϣ
	DWORD		WriteOverlayData();
	//д��Navigater��Ϣ
	DWORD		WriteNavigaterData();
	//д��Button��Ϣ
	DWORD		WriteButtonData();

	// д����ͼ
	DWORD		WriteBackdropData();
	
	DWORD        WriteAnimation();

	// д�볡��ͼ
	DWORD        WriteSceneGraph();

	//���쳡��ͼ����
	void		ConstructSceneGraph(CSceneNode* pNode);


	//д���ļ��汾���ͺ���Ȩ��ʾ
	DWORD		WritePrivacyData();

	//д����ƶ�������
	DWORD		WriteControlData();

	//�ͷ���Դ
	VOID		Release();

	//�����Ӧ��Scene����ΪE3D�ļ�
	// fVersion = 0 ����ʾд����߰汾 zipStatus = true����ʾѹ������
	DWORD		SaveSceneAsV3D(const CScene* pVGSSceneMgr, const std::string &strFileName, HWND hWnd, BOOL zipStatus = false);	

	
	//���һ���ڵ���ӽڵ�����
	int			GetChildNodeCount(CSceneNode* pNode);

	// �������������ӽڵ�ָ��
	CSceneNode*	GetChildNodePtr(CSceneNode* pParentNode, int iIndex);

public:

	CSaveV3D();

	~CSaveV3D();

private:
		
	// void WriteAMesh(const Ogre::Mesh* pMesh);                                // д��һ��Mesh������
	// NodeObject GetNodeObject(const MovableObject *pObject);          
	void CreateSceneGraphNode(int ParentIdx, CSceneNode *pNode);             // ����һ���ڵ㼰���ӽڵ�ĳ���ͼ����
	void GetNodeTypeAndIndex(CSceneNode* pSceneNode, VGS_Node* pVGSNode);    // ��ȡnode�ڵ�����ͺ͸��������ڸ��Զ����е����� 

	// ����һ�����ʶ���
	void WriteAMaterial(CMaterial* pMaterial);

	// ��������������˵�û�б����ƶ���ʹ�õ�Ԥ�������
	void FilterCamera();

	// ����һ��Node�Ķ�������
	void WriteANodeAnimation(CSceneNode *pNode);

	vector<CCamera*>       m_vpCamera;     // �޳���Ԥ��������������б�
	//vector<DWORD>		  m_vMeshList;     // Ҫ�����Ogre::Mesh�����б�
	vector<CMaterial*>		  m_vMaterialList;  // Ҫ�����material�����б�
	//vector<DWORD>		  m_vEntityList;  // Ҫ�����Ogre::Entity�����б�
	vector<CModel*>       m_vpModel;       
	vector<CLight*>		  m_vpLight;
	typedef struct TEntityMesh
	{
		std::string sEntityName;
		int           meshIndex;   // Entityʹ�õ�Mesh��m_MeshList�е�����
	}TEntityMesh;

	HWND m_hWnd;  // ���ս�����Ϣ�Ĵ�����
};
