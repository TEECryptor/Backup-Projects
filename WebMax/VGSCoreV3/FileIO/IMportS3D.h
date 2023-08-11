/////////////////////////////////////////////////////////////////////////
// 名称: CImportS3D--S3D[Version 3.0]S3D数据导入类， OGRE版本
// 描述: 加载3.0版本的S3D文件数据（从CGetS3DFileInfo_V3获得）到场景
// CGetS3DFileInfo_V3负责从S3D文件读取数据，而CImportS3D负责将其读取的数据加载到场景
// 作者: lixin	
// 日期: 2007-11-27
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
	//CGetS3DFileInfo_V3对象的指针
	CGetS3DFileInfo*		mS3DFileInfo;

	//渲染同步对象
	CRITICAL_SECTION*		m_render_cs;
	
	//场景中已有的材质数量
	int						mPreMtrlCnt;
	//场景中已有的贴图数量
	int						mPreTexCnt;
	//场景中已有的相机数量
	int						mPreCameraCnt;
	//场景中已有的灯光数量
	int						mPreLightCnt;
	//场景中已有的骨骼数量
	int						mPreSkeletonCnt;
	//场景已有的Mesh数量
	// int						mPreMeshCnt;
	//场景中已有的Entity数量
	int						mPreEntityCnt;

	int                     m_preModelCnt;
	
//-----------------------------------------------------------------------------
//OGRE相关变量
//-----------------------------------------------------------------------------
public:
	CScene*					m_pScene;					//VGS分场景管理器		
	double					mKeyFrameStartPlayTime;		//关键帧动画开始播放的时间

	//贴图材质关联列表
	// std::vector<VGS_TextureDev>			m_textureMtrlDev;
	std::set<std::string>  m_textureset;               // 时用到的贴图文件，不包括路径

	//如果该S3D文件是玩家文件，则下列变量为玩家骨骼及模型关联列表
	//std::vector<VGS_SkeletonModelDev>	m_skeletonModelDev;

public:
	//计算场景中已有的材质数量
	//int						GetSceneMtrlCount();
	////计算场景中已有的相机数量
	//int						GetSceneCameraCount();
	////计算场景中已有的灯光数量
	//int						GetSceneLightCount();
	////计算场景中已经有的骨骼数量
	//int						GetSceneSkeletonCount();
	////计算场景中已经有的Mesh数量
	//int						GetSceneMeshCount();
	////计算场景中已有的Entity数量
	//int						GetSceneEntityCount();


	//创建灯光的关键帧动画
	void					CreateLightParamAnimation(CLight* pLight, VGS_Light* pVGSLight);
	//创建相机的参数动画
	void					CreateCameraParamAnimation(CCamera* pCamera, VGS_Camera* pVGSCamera);
	//创建Node的动画
	void					CreateNodeAnimation(CSceneNode* pNode, VGS_Node* pVGSNode, NodeType type);
	//共享Entity的骨骼
	// bool					ShareEntitySkeleton(CModel* pModel);

	//  创建关键帧动画
	CNodeKeyFrame* CreateCameraKeyFrame(CSceneNode *pNode, NodeTransform* pNodeTransform, VGS_CAMERA_INFO* pCamParam);
	CNodeKeyFrame* CreateLightKeyFrame(CSceneNode *pNode, NodeTransform* pNodeTransform, VGS_LIGHT_INFO* pLightParam);
	CNodeKeyFrame* CreateModelKeyFrame(CSceneNode *pNode, NodeTransform* pNodeTransform);

public:
	//重命名材质
	bool					RenameMaterialIfExist(std::string& RealName, std::string& RenameLog);
	//重命名相机
	bool					RenameCameraIfExist(std::string& RealName, std::string& RenameLog);
	//重命名灯光
	bool					RenameLightIfExist(std::string& RealName, std::string& RenameLog);
	//重命名骨骼
	bool					RenameSkeletonIfExist(std::string& RealName, std::string& RenameLog);
	//重命名Mesh
	bool					RenameMeshIfExist(std::string& RealName, std::string& RenameLog);
	//重命名SceneNode
	bool					RenameSceneNodeIfExist(std::string& RealName);

public:
	//加载CGetS3DFileInfo_V3对象到场景
	BOOL        			LoadS3DFileInfo(CGetS3DFileInfo* pS3DFileInfo, CScene* pVGS_SceneMgr, HWND hWnd);
	
	//加载材质数据
	int						LoadMtrlData();
	//加载相机数据
	int						LoadCameraData();
	//加载灯光数据
	int						LoadLightData();
	//加载骨骼数据
	int						LoadSkeletonAnimationData();

	void					ReadNodeAnimation( VGS_Node* pVGSNode,FILE* pStream );
	//加载Mesh模型数据
	int						LoadMeshData();
	//加载Shape数据
	int						LoadShapeData();
	//加载场景图
	int						LoadSceneGraphData();
	//获得场景的子节点
	void					GetSceneNodePtrList( vector<CSceneNode*>& vNodeList, CSceneNode* pRoot );

	//加载场景图数据
	void					CreateSceneNode(CSceneNode* pNode, VGS_Node* pVGSNode);

	//判断当前贴图是否已经存在
	// bool					IsTextureExist(std::string texName, std::string mtrlName, int mapChannel);
	// bool CImportS3D::IsTextureExist(std::string texName, std::string mtrlName, int layerindx, int frameidx);
	bool IsTextureImageInused(const std::string &ImageFileName);
	UINT GetTextureImageCnt();

	//释放资源
	void					Release();
	//删除贴图材质关联列表
	void					ReleaseTextureMtrlDev();
	//删除模型骨骼关联列表
	void					ReleaseSkeletonModelDev();

	//将S3D相机参数转化为V3D相机参数
	void					ConvCamTransformToNodeTransform(NodeTransform* pNodeTransform, VGS_CAMERA_INFO* pS3DCamInfo);

public:
	CImportS3D(void);
	~CImportS3D(void);

private:
	//判断相机是否已经存在
	BOOL IsCameraInScene(std::string strName);
	//判断灯光是否已经存在
	BOOL IsLightInScene(std::string strName);

	vector<CMaterial*>		m_vLoadedShader;							// 本次加载的材质资源队列

	HWND m_hWnd;  // 接收进度消息的窗体句柄


	vector <CCameraS3DTransform> m_vCamS3DTransform;
	CCameraS3DTransform getCameraParamByName(const std::string &sName);

	// 从图像文件创建一个texturelayer
	CTextureLayer* CreateTextureLayerFromFile(CScene *pScene, const std::string &sLayerName, const std::string &sTextureFileName, BOOL bAlphaOnly = FALSE, INT UVIndex = 0, INT mipMap = DEFAULT_MIPMAP);

};

////全局变量，见VGS3DLL.CPP
//extern int ResourceGroupIndex;
//
////包含骨骨骼动画的Entity列表，ShareEntity除外
//extern std::vector<Ogre::Entity*> skeletonEntityList;
////定义动态灯光队列，这里自定义了LightParamAnimationList类
//extern std::vector<LightParamAnimation*> lightParamAnimationList;
////定义动态相机队列，这里自定义了CameraParamAnimation类
//extern std::vector<CameraParamAnimation*> cameraParamAnimationList;
//
////关键帧动画播放的帧率
//extern int	kfrPlaySpeed;	
//
////防止重名的变量
//extern UINT gRenamedCameraCnt;
//extern UINT gRenamedLightCnt;
//extern UINT gRenamedSkeletonCnt;
//extern UINT gRenamedMaterialCnt;
//extern UINT gRenamedMeshCnt;
//extern UINT gRenamedSceneNodeCnt;


#endif