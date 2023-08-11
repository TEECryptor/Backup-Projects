/////////////////////////////////////////////////////////////////////////
// ����: CGetS3DFileInfo--S3D[Version 3.0]�ļ���ȡ�࣬ OGRE�汾
// ����: ��ȡ3.0�汾��S3D�ļ����������������ݵ���Ӧ�����ݽṹ���������ص�����
// ����: lixin	
// ����: 2007-11-26
////////////////////////////////////////////////////////////////////////

#ifndef _CGETS3DFILEINFO_H
#define _CGETS3DFILEINFO_H

#include "..\VGSHeader.h"
#include "S3D_Def.h"
#include "..\CSysFunc.h"

#include "..\VGSDataStruct.h"

class CGetS3DFileInfo
{
private:
	//S3D�ļ�������
	FILE*					m_pS3DStream;

	HWND                    m_hWnd;      //  ������Ϣ�Ĵ�����

public:
	std::string				mStrFileName;
	std::string				mStrPurePath;

	//���������ݽṹ---------------------------------------------
public:
	//�ļ�ͷ
	VGS_Header				header;
	//����
	VGS_MtrlList			mtrlManager;
	//���
	VGS_CameraList			cameraManager;
	//�ƹ�
	VGS_LightList			lightManager;

	//����
	VGS_SkeletonAnimationList		skeletonAnimationManager;
	//ģ��
	// VGS_MeshList			meshManager;
	std::vector<CVGSMesh>        m_vMeshData;

	//2Dͼ��
	VGS_ShapeList			shapeManager;

	//�����ṹͼ
	VGS_SceneNodeGraph		sceneNodeGraph;	

	
	DWORD                   m_totalSize; // �ļ����ܴ�С

	//��ͼ
	//std::vector<VGS_TextureDev>	textureManager;

public:
	//����S3D�ļ����������ݽṹ
	int						ReadS3DFile(std::string strFileName, HWND hWnd);
	//�����ļ�ͷ
	int						ReadFileHeader();
	//�����������
	int						ReadMtrlData();
	//�����������
	int						ReadCameraData();
	//����ƹ�����
	int						ReadLightData();
	//�����������
	int						ReadSkeletonAnimationData();
	//����Meshģ������
	int						ReadMeshData();
	//�ݹ������Ƥ����
	void					ReadSkeletonAndSkinData(VGS_Bone* pBone);
	//���������������
	void					ProcessSkeletonAnimationStruct(VGS_BoneAnimation* pBoneAnimation, int frameCnt);
	//����Shape����
	int						ReadShapeData();
	//���볡��ͼ
	int						ReadSceneGraphData();
	//���쳡��ͼ����
	void					ConstructSceneGraph(VGS_Node* pNode);

	//�ͷ���Դ
	void					Release();
	//�ͷŹ����������ݹ������
	void					ReleaseSkeletonAnimationManager();
	//�ݹ�ɾ��������������
	void					ReleaseBoneAnimationRecursion(VGS_BoneAnimation* bone);
	//ɾ��ģ�ͺ���Ƥ����
	void					ReleaseMeshAndSkinManager();
	//�ݹ��ͷ�ģ�ͺ���Ƥ����
	void					ReleaseMeshAndSkinRecursion(VGS_Bone* pBone);
	//ɾ������ͼ
	void					ReleaseNodeRecursion(VGS_Node& node);
	//ɾ����ͼ
	void					ReleaseTexture(std::vector<VGS_TextureDev>& textureManager);

public:
	CGetS3DFileInfo(void);
	~CGetS3DFileInfo(void);
};

#endif