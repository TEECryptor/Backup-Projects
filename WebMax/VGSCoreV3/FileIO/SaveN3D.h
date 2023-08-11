/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // SaveN3D.h
* 内容摘要： // 输出N3D的类定义
* 其它说明： // 
* 当前版本： // V2.0
* 作    者： // 傅新波
* 完成日期： // 
* 
* 修改日期      版本号     修改人	      修改内容
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

	N3D_HeaderV3			m_Header;					//N3D文件头

	N3D_5_1_Table           m_Table;                    //表结构

	N3D_SceneProp			m_SceneProp;				//场景参数

	N3D_AudioList			m_AudioData;				//声音数据的起始位置

	V3D_FlashList			m_FlashData;				//Flash数据的起始位置

	VGS_CameraList			m_CameraData;				

	VGS_MtrlList    		m_MaterialData;			

	VGS_LightList			m_LightData;				//灯光数据

	VGS_MeshList			m_MeshData;

	V3D_TextList			m_TextData;				//文字数据

	V3D_OverlayList			m_OverlayData;			//Overlay数据

	V3D_NavigatorList		m_NavigaterData;			//Navigater数据

	V3D_ButtonList			m_ButtonData;				//按钮列表


	V3D_Privacy				m_PrivacyData;			//授权数据

	V3D_ControlData			m_ControlData;			//用户控制对象数据
	
	UINT                    m_imageCount;             //场景中使用的贴图的个数
private:

	CScene*	            m_pVGSSceneMgr;			    //指向VGS场景管理器的指针

	FILE*				m_pFile;					//指向N3D文件的指针

	// 存储场景构造信息的临时文件，不压缩
    FILE*               m_OriSceneBufferFile;

	// 存储图像数据，包括所有贴图以及未使用的图像
	// FILE*               m_ImagesBufferFile;

	// 压缩后场景构造信息的临时文件
    FILE*               m_ZipSceneBufferFile;

	DWORD				m_nFileSize;				//N3D文件的大小

	CHAR				m_FileName[256];			//N3D文件名

public:

	//写入文件头
	VOID		WriteHeader();

    //Add by baojiahui 创建临时的场景构造文件，用于player构建场景，保留了V3D中的基础数据
	bool CreateSceneDataBuffer();
    
	void        WriteFileTable();

	//写入窗体属性
	VOID		WriteWindowProp();
	
	//写入声音数据
	VOID		WriteAudioData();
	//写入Flash数据
	VOID		WriteFlashData();

	//写入摄像机信息
	VOID		WriteCameraData();
	//写入材质信息
	VOID		WriteMaterialData();
	
	//Add by Baojiahui 将贴图信息写入相应的buffer文件，并在场景文件中保存贴图的索引号
	// VOID        WriteImageInfo(const UINT& imgIndex, /*const TextureUnitState *pTextUnit,const UINT& frame,*/ const char* texName);
	//VOID        WriteTextureData();
	//写入灯光信息
	VOID		WriteLightData();

	//写入Mesh信息
	// VOID		WriteMeshData();
	VOID		WriteModelData();
	VOID        WriteAModel(CModel *pModel);


	//写入文字信息
	VOID		WriteTextData();
	//写入Overlay信息
	VOID		WriteOverlayData();

	// 写入一个overlay信息
	void WriteOverlayInfo(COverlay *pOverlay, BOOL bIsBackGround = FALSE);

	// 写入背景图
	// VOID        WriteBackGround();

	//写入Navigater信息
	VOID		WriteNavigaterData();
	//写入Button信息
	VOID		WriteButtonData();
	
	// VOID        WriteAnimation();

	// 写入场景图
	VOID        WriteSceneGraph();

	//构造场景图数据
	VOID		ConstructSceneGraph(CSceneNode* pNode);


	//写入文件版本类型和授权标示
	VOID		WritePrivacyData();

	//写入控制对象数据
	VOID		WriteControlData();

	VOID		WriteImageData();  // 写入图像数据
   
	HRESULT     ZipAndWriteSceneData();

	//释放资源
	VOID		Release();

	//保存对应的Scene场景为E3D文件
	HRESULT		SaveSceneAsN3D(const CScene* pVGSSceneMgr, const CHAR* strFileName, BOOL zipStatus = true, DWORD fVersion = N3D_CURRENT_V3DVERSION);	

	//在保存之前，为对象重新设置编号，防止因为删除对象后，导致保存对象编号错误

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

	//给控制对象重新编号
	VOID		ResetWalkerIndex();

	VOID		ResetEditorIndex();

	VOID		ResetFlyerIndex();

	VOID		ResetButtonIndex();

	VOID		ResetImageIndex();

	
	//获得一个节点的子节点数量
	UINT			GetChildNodeCount(CSceneNode* pNode);
	//通过相机对象从相机动画列表中获得相机参数动画对象
	//CameraParamAnimation* GetCameraParamAnimation(std::vector<CameraParamAnimation*> list, Ogre::Camera* pCam);
	//通过灯光对象从灯光动画列表中获得灯光参数动画对象
	//LightParamAnimation* GetLightParamAnimation(std::vector<LightParamAnimation*> list, Ogre::Light* pLight);


public:

	CSaveN3D();

	~CSaveN3D();

private:
	
	//std::vector<SceneGraphNode> m_VSceneGraph;  // 场景图数据结构
	
	// void WriteAMesh(const MeshPtr &ptrMesh);                                // 写入一个Mesh的数据
	// NodeObject GetNodeObject(const MovableObject *pObject);          
	void CreateSceneGraphNode(int ParentIdx, CSceneNode *pNode);             // 生成一个节点及其子节点的场景图数据
	void GetNodeTypeAndIndex(CSceneNode* pSceneNode, N3D_Node* pN3DNode); 

	// UINT GetVGSMtrlIndex(String materialName);

	// 过滤掉预定义的轴侧相机
	void FilterCamera();

	// 获取相机索引
	int GetCameraIndex(CCamera *pCamera);

	vector<CCamera*>       m_vpCamera;		// 剔除掉预定义轴侧相机后的相机列表

	vector<std::string>  m_vImageList;		// 写入N3D的Image文件名称列表,包含文件路径

	vector<DWORD>         m_vMaterialList;  // 写入N3D的材质列表

	vector<DWORD>         m_vEntityList;    // 写入的模型列表

	vector<CLight*>		  m_vpLight;  // 写入的灯光列表

	// 获取texture的类型
	// TextureLayerType GetTextureLayerType(int mapchanel);
	N3D_TextureType GetTextureLayerType(CTextureLayer *pState);

	// 获取texture类型的描述
	// std::string GetTextureLayerTypeDes(N3D_TextureType type);

	// 初始化Image列表
	void InitImageList();

	// 写入texturelayer的参数
	void WriteTexturealayerParams(CTextureLayer *pState, N3D_TextureType type, FILE *pFile);

	UINT m_PureOverlayCount;        // 真正的Overlay数目，不包括其他二维对象使用到的overlay
	UINT m_BackdropCount;           // 背景图数目，0 or 1

	// 构造N3D中保存的Button及使用的overlay信息
	void GetButtonInfo();

	typedef struct N3D_ButtonOverlay
	{
		CHAR		   Name[32];			// 按钮的的名字
		N3D_OverlayV3  OverlayInfo;

		int            DownMaterialIndex;
		int            OverMaterialIndex;
		std::string    sCallBackFunc;
	}N3D_ButtonOverlay;

	vector <N3D_ButtonOverlay> m_vButtonInfo;  // 要保存到N3D中的按钮基本信息

	// 构造N3D中保存的导航图及使用的overlay信息
	void GetNavigatorInfo();

	typedef struct N3D_NaviPointerOverlay
	{
		N3D_OverlayV3    OverlayInfo;

		CHAR		Name[32];			//Pointer的名字
		int			Type;				//Pointer的类型，Camera-0, Model-1
		int			ObjIndex;			//相机或模型的编号

	}N3D_NaviPointerOverlay;
	typedef struct N3D_NaviOverlay
	{
		CHAR		                Name[32];			// 导航图的的名字
		FLOAT						sceneLeft;			// 场景TopView的Rect
		FLOAT						sceneTop;
		FLOAT						sceneRight;
		FLOAT						sceneBottom;

		N3D_OverlayV3    OverlayInfo;
		vector <N3D_NaviPointerOverlay> vPointer;

	}N3D_NaviOverlay;

	vector <N3D_NaviOverlay> m_vNaviInfo;  // 要保存到N3D中的导航图基本信息

	//  获取所有用到的材质数目，包括二维对象和三维对象
	void GetAllMaterials();

	// 从材质队列中查找材质的编号，找不到返回-1
	int FindMaterial(DWORD pMaterial);

	//-------------------------------------------------------------------
	//方法: GetN3DTexOP()
	//参数: 
	//用途: 将D3D的混色模式参数转换成N3D的混色模式参数
	//-------------------------------------------------------------------
	N3D_BlenderOP GetN3DTexOP(const D3DTEXTUREOP& d3d_BlenderOP);

};



//------------------------------------------------------------------------
#endif
