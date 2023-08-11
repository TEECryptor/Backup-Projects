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
#include "..\VGSHeader.h"
#include "..\CCamera.h"
#include "..\CLight.h"
#include "..\CModel.h"
#include "GetV3DBase.h"
#include "..\\VGSDataStruct.h"
#include "..\VGSGetRoot.h"
//----------------------------------------------------------
#define RETURN(RE)                       {if (RE != ERR_COMMON_OK)return RE;} 

// ɾ������
#define DELETE_ARRAYPTR(Arrayptr)        {if (Arrayptr) {delete [] Arrayptr; Arrayptr = NULL;}}                                

// ��Offsetλ�ÿ��������ֽ�
#define CPY_FROM_STREAM(value, Offset)   {memcpy(&value, Offset, sizeof(value)); Offset += sizeof(value);  m_Offset += sizeof(value);} 

// ��offsetλ�ÿ���һ��vector3���ݽṹ
#define CPY_VECTOR3_FROM_STRRAM(V3, Offset)\
{\
	CPY_FROM_STREAM((V3.x), Offset)\
	CPY_FROM_STREAM((V3.y), Offset)\
	CPY_FROM_STREAM((V3.z), Offset)\
}

// ��offsetλ�ÿ���һ�����ݵ�һ���ڴ�
#define CPY_PTR_FROM_STREAM(ptr, n, Offset)   {memcpy(ptr, Offset, n); Offset += n;} 

// ���ļ��ж�һ��Vector3
#define READ_VECTOR3(V3, pFile) {fread(&(V3.x), sizeof(V3.x), 1, pFile); fread(&(V3.y), sizeof(V3.y), 1, pFile); fread(&(V3.z), sizeof(V3.z), 1, pFile);}

//----------------------------------------------------------
class CGetV3DImpV2 : public CGetV3DBase , public CVGSGetRoot
{

public:
	CGetV3DImpV2(/*LPDIRECT3DDEVICE9 pd3dDevice*/);
	virtual ~CGetV3DImpV2();

public: // �̳��Խӿ�

	void Release(); // �ͷ��ڴ�

	// ��V3D�ļ������ؽ�����
	virtual DWORD OpenV3DFile(CScene* pVGSSceneMgr, const std::string &strFileName, HWND hWnd);

	// ��ȡV3D�ļ���TableĿ¼
	virtual bool GetV3DTable(const std::string &strFileName, V3D_Table &Table);

	// �õ�texture�������ļ��еķ�Χ, start: ��ʼλ��, len:���ݳ���
	virtual void GetTextureDataRange(const V3D_Table &table, DWORD &start, DWORD &len);

	virtual BOOL isMyVersion(const std::string &sFileName);  // �Ƿ�����֧�ֵİ汾

protected:

	HWND  m_hWnd;  // ���ս�����Ϣ�Ĵ�����

	//LPDIRECT3DDEVICE9  m_pD3dDevice;

	CScene* m_pVGSSceneMgr;  // VGVS ����������

	FILE *m_pFile;  // Ҫ�����ļ�ָ��

	std::string m_sV3DPath;  // ��ǰV3D�ļ����ڵ�Ŀ¼

	float m_V3DFileVersion; // Ҫ�򿪵�V3D�ļ��汾��

	V3D_Table m_V3DTable; // V3D�ļ��и����ݿ�Ŀ�ʼλ��Ŀ¼

	V3D_SceneProp m_SceneProperty;

	// vector <V3D_CameraDetail> m_vCameraDetail; 
	vector <CCamera*> m_vpCamera;  // ���ص����
	vector <CLight*>  m_vpLight;   // ���صĵƹ�
	vector <CModel*>  m_vModel;  // ���ص�model�����б�
	vector <CVGSMesh> m_vMesh;
	vector <CMaterial*>	m_vMaterial;

	std::string m_sDefaultTexturePath;    // ȱʡ����ͼ·��

	void SetCurrentDirctory(const std::string &sFileName);  // ����v3d�ļ������õ���ǰ��V3DĿ¼

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

	// ��ȡ��������
	virtual DWORD ReadAllData();

	virtual DWORD ReadHeader();
	virtual DWORD ReadFileTable();
	virtual DWORD ReadWindowProp();
	virtual DWORD ReadTextureData();
	virtual DWORD ReadMaterialData();

	virtual DWORD ReadMeshData();     // ��ȡģ������
	virtual DWORD ReadModelData();    // ��ȡģ������
	virtual DWORD ReadCameraData();
	virtual DWORD ReadLightData();
	virtual DWORD ReadTextData();

	//Read the overlay data
	virtual DWORD ReadOverlayData();
	//Read the navigater data
	virtual DWORD ReadNavigaterData();
	//Read the button data
	virtual DWORD ReadButtonData();

	// ����ͼ
	virtual DWORD ReadBackdropData();

	//Read AudioData
	virtual DWORD ReadAudioData();

	//Read FlashData
	virtual DWORD ReadFlashData();

	// ����
	virtual DWORD ReadAnimationData();

	//Read privacy data
	virtual DWORD ReadPrivacyData();

	// ����
	virtual DWORD ReadControlData();

	// ����ͼ
	virtual DWORD ReadSceneGraph();

	//�ݹ鴴������
	virtual void ConstructSceneGraph(CSceneNode* pNode, char* offset);
	
	//�����ڵ�Ķ���
	virtual void ReadAndCreateNodeAnimation(CSceneNode* pNode, float animationLength, char** offset);
	
	//��������Ĳ�������
	virtual void CreateCameraParamAnimation(CCamera* pCamera, VGS_V3D_Camera* pVGSCamera, int frameCnt);
	virtual BOOL CreateCameraNodeAnimation(CSceneNode* pSceneNode, CCamera* pCamera, const std::vector<CNodeTransform> &vTransform);
	
	//�����ƹ��������
	virtual void CreateLightParamAnimation(CLight* pLight, VGS_Light* pVGSLight, int frameCnt, char** Offset);
	virtual BOOL CreateLightNodeAnimation(CSceneNode* pSceneNode, CLight* pLight, const std::vector<CNodeTransform> &vTransform);

	// ��ָ���Ĵ��ļ������·��
	virtual std::string AddPathtoTextureFile(const std::string &sPureFileName);

	// ���·�����ͼ�еĵƹ���
	virtual VOID UpdateBumpNormalmapLight();

	// �������ɺ��һЩ����
	virtual VOID PostOpen();

	UINT m_ProcessTotalSize;  // ���������ܽ���
	UINT m_ProcessCurSize;    // �������ĵ�ǰ����

	std::vector<VGS_V3D_Camera>	m_arCameraFrameInfo;
	std::vector<VGS_V3D_Light>	m_arLightFrameInfo;
};

