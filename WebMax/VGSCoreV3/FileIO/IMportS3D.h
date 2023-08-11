/////////////////////////////////////////////////////////////////////////
// ����: CImportS3D--S3D[Version 3.0]S3D���ݵ����࣬ OGRE�汾
// ����: ����3.0�汾��S3D�ļ����ݣ���CGetS3DFileInfo_V3��ã�������
// CGetS3DFileInfo_V3�����S3D�ļ���ȡ���ݣ���CImportS3D�������ȡ�����ݼ��ص�����
// ����: lixin	
// ����: 2007-11-27
////////////////////////////////////////////////////////////////////////

#ifndef _CIMPORTS3D_H
#define _CIMPORTS3D_H

#include "..\VGSHeader.h"
#include "S3D_Def.h"
#include "gets3dfileinfo.h"
#include "..\VGSDataStruct.h"

#include "..\CScene.h"
#include "..\Node.h"
#include "..\CMaterial.h"

#include "..\Animation\NodeKeyFrame.h"
#include "..\Animation\LightKeyFrame.h"
#include "..\Animation\CameraKeyFrame.h"
#include "..\Animation\\ModelKeyFrame.h"

#include "..\\VGSDef.h"
#include "..\VGSGetRoot.h"

#include <set>

using namespace std;


typedef struct CCameraS3DTransform
{
	std::string sName;
	D3DXVECTOR3 v3Pos, v3At;
	float  roll;

	CCameraS3DTransform()
	{
		v3Pos = v3At = D3DXVECTOR3(0, 0, 0);
		roll = 0.0f;
	}
}CCameraS3DTransform;

class CImportS3D : public CVGSGetRoot
{
private:
	//CGetS3DFileInfo_V3�����ָ��
	CGetS3DFileInfo*		mS3DFileInfo;

	//��Ⱦͬ������
	CRITICAL_SECTION*		m_render_cs;
	
	//���������еĲ�������
	int						mPreMtrlCnt;
	//���������е���ͼ����
	int						mPreTexCnt;
	//���������е��������
	int						mPreCameraCnt;
	//���������еĵƹ�����
	int						mPreLightCnt;
	//���������еĹ�������
	int						mPreSkeletonCnt;
	//�������е�Mesh����
	// int						mPreMeshCnt;
	//���������е�Entity����
	int						mPreEntityCnt;

	int                     m_preModelCnt;
	
//-----------------------------------------------------------------------------
//OGRE��ر���
//-----------------------------------------------------------------------------
public:
	CScene*					m_pScene;					//VGS�ֳ���������		
	double					mKeyFrameStartPlayTime;		//�ؼ�֡������ʼ���ŵ�ʱ��

	//��ͼ���ʹ����б�
	// std::vector<VGS_TextureDev>			m_textureMtrlDev;
	std::set<std::string>  m_textureset;               // ʱ�õ�����ͼ�ļ���������·��

	//�����S3D�ļ�������ļ��������б���Ϊ��ҹ�����ģ�͹����б�
	//std::vector<VGS_SkeletonModelDev>	m_skeletonModelDev;

public:
	//���㳡�������еĲ�������
	//int						GetSceneMtrlCount();
	////���㳡�������е��������
	//int						GetSceneCameraCount();
	////���㳡�������еĵƹ�����
	//int						GetSceneLightCount();
	////���㳡�����Ѿ��еĹ�������
	//int						GetSceneSkeletonCount();
	////���㳡�����Ѿ��е�Mesh����
	//int						GetSceneMeshCount();
	////���㳡�������е�Entity����
	//int						GetSceneEntityCount();


	//�����ƹ�Ĺؼ�֡����
	void					CreateLightParamAnimation(CLight* pLight, VGS_Light* pVGSLight);
	//��������Ĳ�������
	void					CreateCameraParamAnimation(CCamera* pCamera, VGS_Camera* pVGSCamera);
	//����Node�Ķ���
	void					CreateNodeAnimation(CSceneNode* pNode, VGS_Node* pVGSNode, NodeType type);
	//����Entity�Ĺ���
	// bool					ShareEntitySkeleton(CModel* pModel);

	//  �����ؼ�֡����
	CNodeKeyFrame* CreateCameraKeyFrame(CSceneNode *pNode, NodeTransform* pNodeTransform, VGS_CAMERA_INFO* pCamParam);
	CNodeKeyFrame* CreateLightKeyFrame(CSceneNode *pNode, NodeTransform* pNodeTransform, VGS_LIGHT_INFO* pLightParam);
	CNodeKeyFrame* CreateModelKeyFrame(CSceneNode *pNode, NodeTransform* pNodeTransform);

public:
	//����������
	bool					RenameMaterialIfExist(std::string& RealName, std::string& RenameLog);
	//���������
	bool					RenameCameraIfExist(std::string& RealName, std::string& RenameLog);
	//�������ƹ�
	bool					RenameLightIfExist(std::string& RealName, std::string& RenameLog);
	//����������
	bool					RenameSkeletonIfExist(std::string& RealName, std::string& RenameLog);
	//������Mesh
	bool					RenameMeshIfExist(std::string& RealName, std::string& RenameLog);
	//������SceneNode
	bool					RenameSceneNodeIfExist(std::string& RealName);

public:
	//����CGetS3DFileInfo_V3���󵽳���
	BOOL        			LoadS3DFileInfo(CGetS3DFileInfo* pS3DFileInfo, CScene* pVGS_SceneMgr, HWND hWnd);
	
	//���ز�������
	int						LoadMtrlData();
	//�����������
	int						LoadCameraData();
	//���صƹ�����
	int						LoadLightData();
	//���ع�������
	int						LoadSkeletonAnimationData();

	void					ReadNodeAnimation( VGS_Node* pVGSNode,FILE* pStream );
	//����Meshģ������
	int						LoadMeshData();
	//����Shape����
	int						LoadShapeData();
	//���س���ͼ
	int						LoadSceneGraphData();
	//��ó������ӽڵ�
	void					GetSceneNodePtrList( vector<CSceneNode*>& vNodeList, CSceneNode* pRoot );

	//���س���ͼ����
	void					CreateSceneNode(CSceneNode* pNode, VGS_Node* pVGSNode);

	//�жϵ�ǰ��ͼ�Ƿ��Ѿ�����
	// bool					IsTextureExist(std::string texName, std::string mtrlName, int mapChannel);
	// bool CImportS3D::IsTextureExist(std::string texName, std::string mtrlName, int layerindx, int frameidx);
	bool IsTextureImageInused(const std::string &ImageFileName);
	UINT GetTextureImageCnt();

	//�ͷ���Դ
	void					Release();
	//ɾ����ͼ���ʹ����б�
	void					ReleaseTextureMtrlDev();
	//ɾ��ģ�͹��������б�
	void					ReleaseSkeletonModelDev();

	//��S3D�������ת��ΪV3D�������
	void					ConvCamTransformToNodeTransform(NodeTransform* pNodeTransform, VGS_CAMERA_INFO* pS3DCamInfo);

public:
	CImportS3D(void);
	~CImportS3D(void);

private:
	//�ж�����Ƿ��Ѿ�����
	BOOL IsCameraInScene(std::string strName);
	//�жϵƹ��Ƿ��Ѿ�����
	BOOL IsLightInScene(std::string strName);

	vector<CMaterial*>		m_vLoadedShader;							// ���μ��صĲ�����Դ����

	HWND m_hWnd;  // ���ս�����Ϣ�Ĵ�����


	vector <CCameraS3DTransform> m_vCamS3DTransform;
	CCameraS3DTransform getCameraParamByName(const std::string &sName);

	// ��ͼ���ļ�����һ��texturelayer
	CTextureLayer* CreateTextureLayerFromFile(CScene *pScene, const std::string &sLayerName, const std::string &sTextureFileName, BOOL bAlphaOnly = FALSE, INT UVIndex = 0, INT mipMap = DEFAULT_MIPMAP);

};

////ȫ�ֱ�������VGS3DLL.CPP
//extern int ResourceGroupIndex;
//
////�����ǹ���������Entity�б�ShareEntity����
//extern std::vector<Ogre::Entity*> skeletonEntityList;
////���嶯̬�ƹ���У������Զ�����LightParamAnimationList��
//extern std::vector<LightParamAnimation*> lightParamAnimationList;
////���嶯̬������У������Զ�����CameraParamAnimation��
//extern std::vector<CameraParamAnimation*> cameraParamAnimationList;
//
////�ؼ�֡�������ŵ�֡��
//extern int	kfrPlaySpeed;	
//
////��ֹ�����ı���
//extern UINT gRenamedCameraCnt;
//extern UINT gRenamedLightCnt;
//extern UINT gRenamedSkeletonCnt;
//extern UINT gRenamedMaterialCnt;
//extern UINT gRenamedMeshCnt;
//extern UINT gRenamedSceneNodeCnt;


#endif