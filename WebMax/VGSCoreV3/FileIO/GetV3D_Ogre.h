/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // GetV3D.h
* ����ժҪ�� // ��ȡV3D�ඨ��
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


#pragma once
// #include "N3D_Def.h"
#include "VGS_SceneManager.h"

#include "LightParamAnimation.h"
#include "CameraParamAnimation.h"
#include "V3D_Def.h"
class CGetV3D
{

public:
	CGetV3D();
	~CGetV3D();
	void Release(); // �ͷ��ڴ�

	// ��V3D�ļ������ؽ�����
	DWORD OpenV3DFile(CVGS_SceneManager* pVGSSceneMgr, const CHAR* strFileName);

	// ��ȡV3D�ļ���TableĿ¼
	bool GetV3DTable(const CHAR* strFileName, V3D_Table &Table);

	// �õ�texture�������ļ��еķ�Χ, start: ��ʼλ��, len:���ݳ���
	void GetTextureDataRange(const V3D_Table &table, DWORD &start, DWORD &len);

private:

	CVGS_SceneManager* m_pVGSSceneMgr;  // VGVS ����������

	FILE *m_pFile;  // Ҫ�����ļ�ָ��

	String m_sV3DPath;  // ��ǰV3D�ļ����ڵ�Ŀ¼

	float m_V3DFileVersion; // Ҫ�򿪵�V3D�ļ��汾��

	V3D_Table m_V3DTable; // V3D�ļ��и����ݿ�Ŀ�ʼλ��Ŀ¼

	V3D_SceneProp m_SceneProperty;

	// vector <V3D_CameraDetail> m_vCameraDetail; 
	vector <Camera*> m_vpCamera;  // ���ص����
	vector <Light*>  m_vpLight;   // ���صĵƹ�
	vector <Mesh*>   m_vMesh;     // ���ص�Mesh�����б�
	vector <DWORD>   m_vEntityList;  // ���ص�Entity�����б�

	Ogre::String sDefaultTexturePath;    // ȱʡ����ͼ·��

	void SetCurrentDirctory(const char *sFileName);  // ����v3d�ļ������õ���ǰ��V3DĿ¼

	char *m_pTextureBuf;
	char *m_pMaterailBuf;
	char *m_pEntityBuf;  // ģ����Ϣ����
	char *m_pMeshBuf;    // ģ����Դ����
	char *m_pCameraBuf;
	char *m_pLightBuf;
	char *m_pOverlayBuf;
	char *m_pButtonBuf;
	char *m_pNaviBuf;
	char *m_pBackdropBuf; // ����ͼ 2008-07-17
	char *m_pTextBuf;
	char *m_pPrivacyBuf;
	char *m_pFlashBuf;
	char *m_pAudioBuf;
	char *m_pControlBuf;
	char *m_pAnimationBuf;
	char *m_pSceneGraphBuf;
	DWORD	m_Offset;     //��¼ƫ�����ı���

	void ReadPass(Pass *pPass, char** OffsetPtr);
	void ReadPassTexture(Pass *pPass, char** OffsetPtr);
	void ReadPassTexture_old(Pass *pPass, char** OffsetPtr);  // ��50004��֮ǰ�汾��


	// ��ȡ��������
	DWORD ReadAllData();

	DWORD ReadHeader();
	DWORD ReadFileTable();
	DWORD ReadWindowProp();
	DWORD ReadTextureData();
	DWORD ReadMaterialData();
	
	DWORD ReadEnityData();    // ��ȡģ������
	DWORD ReadMeshData();     // ��ȡģ����Դ���ݣ����ģ�Ϳ���ʹ����ͬ��ģ����Դ�����綥�����ݣ�
	DWORD ReadCameraData();
	DWORD ReadLightData();
	DWORD ReadTextData();

	//Read the overlay data
	DWORD ReadOverlayData();
	//Read the navigater data
	DWORD ReadNavigaterData();
	//Read the button data
	DWORD ReadButtonData();

	// ����ͼ
	DWORD ReadBackdropData();

	//Read AudioData
	DWORD ReadAudioData();

	//Read FlashData
	DWORD ReadFlashData();

	// ����
	DWORD ReadAnimationData();

	//Read privacy data
	DWORD ReadPrivacyData();

	// ����
	DWORD ReadControlData();

	// ����ͼ
	DWORD ReadSceneGraph();

	//�ݹ鴴������
	void ConstructSceneGraph(SceneNode* pNode, char* offset);
	
	//�����ڵ�Ķ���
	void ReadAndCreateNodeAnimation(SceneNode* pNode, float animationLength, char** offset);
	
	//��������Ĳ�������
	void CreateCameraParamAnimation(Ogre::Camera* pCamera, VGS_V3D_Camera* pVGSCamera, int frameCnt);
	//�����ƹ��������
	void CreateLightParamAnimation(Light* pLight, VGS_Light* pVGSLight, int frameCnt, char** Offset);

	// ��ָ���Ĵ��ļ������·��
	Ogre::String AddPathtoTextureFile(const char * sPureFileName);

	// ���·�����ͼ�еĵƹ���
	VOID UpdateBumpNormalmapLight();

	// �������ɺ��һЩ����
	VOID PostOpen();

};

