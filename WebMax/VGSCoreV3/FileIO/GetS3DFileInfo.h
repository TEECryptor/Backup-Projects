/////////////////////////////////////////////////////////////////////////
// 名称: CGetS3DFileInfo--S3D[Version 3.0]文件读取类， OGRE版本
// 描述: 读取3.0版本的S3D文件，并规整场景数据到相应的数据结构，但不加载到场景
// 作者: lixin	
// 日期: 2007-11-26
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
	//S3D文件数据流
	FILE*					m_pS3DStream;

	HWND                    m_hWnd;      //  接收消息的窗体句柄

public:
	std::string				mStrFileName;
	std::string				mStrPurePath;

	//场景的数据结构---------------------------------------------
public:
	//文件头
	VGS_Header				header;
	//材质
	VGS_MtrlList			mtrlManager;
	//相机
	VGS_CameraList			cameraManager;
	//灯光
	VGS_LightList			lightManager;

	//骨骼
	VGS_SkeletonAnimationList		skeletonAnimationManager;
	//模型
	// VGS_MeshList			meshManager;
	std::vector<CVGSMesh>        m_vMeshData;

	//2D图形
	VGS_ShapeList			shapeManager;

	//场景结构图
	VGS_SceneNodeGraph		sceneNodeGraph;	

	
	DWORD                   m_totalSize; // 文件的总大小

	//贴图
	//std::vector<VGS_TextureDev>	textureManager;

public:
	//读入S3D文件到场景数据结构
	int						ReadS3DFile(std::string strFileName, HWND hWnd);
	//读入文件头
	int						ReadFileHeader();
	//读入材质数据
	int						ReadMtrlData();
	//读入相机数据
	int						ReadCameraData();
	//读入灯光数据
	int						ReadLightData();
	//读入骨骼数据
	int						ReadSkeletonAnimationData();
	//读入Mesh模型数据
	int						ReadMeshData();
	//递归读入蒙皮数据
	void					ReadSkeletonAndSkinData(VGS_Bone* pBone);
	//读入骨骼动画数据
	void					ProcessSkeletonAnimationStruct(VGS_BoneAnimation* pBoneAnimation, int frameCnt);
	//读入Shape数据
	int						ReadShapeData();
	//读入场景图
	int						ReadSceneGraphData();
	//构造场景图数据
	void					ConstructSceneGraph(VGS_Node* pNode);

	//释放资源
	void					Release();
	//释放骨骼动画数据管理对象
	void					ReleaseSkeletonAnimationManager();
	//递归删除骨骼动画数据
	void					ReleaseBoneAnimationRecursion(VGS_BoneAnimation* bone);
	//删除模型和蒙皮数据
	void					ReleaseMeshAndSkinManager();
	//递归释放模型和蒙皮数据
	void					ReleaseMeshAndSkinRecursion(VGS_Bone* pBone);
	//删除场景图
	void					ReleaseNodeRecursion(VGS_Node& node);
	//删除贴图
	void					ReleaseTexture(std::vector<VGS_TextureDev>& textureManager);

public:
	CGetS3DFileInfo(void);
	~CGetS3DFileInfo(void);
};

#endif