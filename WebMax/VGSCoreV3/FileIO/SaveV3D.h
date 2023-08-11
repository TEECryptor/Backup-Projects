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

	V3D_Header				m_Header;					//V3D文件头

	V3D_Table               m_Table;                    //表结构

	V3D_SceneProp			m_SceneProp;				//场景参数
	

	V3D_AudioList			m_AudioData;				//声音数据的起始位置

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


private:

	CScene*	           m_pVGSSceneMgr;			    //指向VGS场景管理器的指针

	FILE*				m_pFile;					//指向V3D文件的指针

	DWORD				m_nFileSize;				//V3D文件的大小

	CHAR				m_FileName[256];			//V3D文件名

public:

	//写入文件头
	DWORD		WriteHeader();

	DWORD        WriteFileTable();

	//写入窗体属性
	DWORD		WriteWindowProp();
	
	//写入声音数据
	DWORD		WriteAudioData();
	//写入Flash数据
	DWORD		WriteFlashData();

	//写入摄像机信息
	DWORD		WriteCameraData();
	//写入材质信息
	DWORD		WriteMaterialData();
	DWORD        WriteTextureData();
	//写入灯光信息
	DWORD		WriteLightData();

	// 保存Entity信息
	DWORD       WriteModelData();

	//写入Mesh信息,mesh是Entity引用的资源，多个entity可能会引用同一个mesh资源
	DWORD		WriteAModel(CModel *pModel);

	//写入文字信息
	DWORD		WriteTextData();
	//写入Overlay信息
	DWORD		WriteOverlayData();
	//写入Navigater信息
	DWORD		WriteNavigaterData();
	//写入Button信息
	DWORD		WriteButtonData();

	// 写背景图
	DWORD		WriteBackdropData();
	
	DWORD        WriteAnimation();

	// 写入场景图
	DWORD        WriteSceneGraph();

	//构造场景图数据
	void		ConstructSceneGraph(CSceneNode* pNode);


	//写入文件版本类型和授权标示
	DWORD		WritePrivacyData();

	//写入控制对象数据
	DWORD		WriteControlData();

	//释放资源
	VOID		Release();

	//保存对应的Scene场景为E3D文件
	// fVersion = 0 ，表示写入最高版本 zipStatus = true，表示压缩场景
	DWORD		SaveSceneAsV3D(const CScene* pVGSSceneMgr, const std::string &strFileName, HWND hWnd, BOOL zipStatus = false);	

	
	//获得一个节点的子节点数量
	int			GetChildNodeCount(CSceneNode* pNode);

	// 根据索引返回子节点指针
	CSceneNode*	GetChildNodePtr(CSceneNode* pParentNode, int iIndex);

public:

	CSaveV3D();

	~CSaveV3D();

private:
		
	// void WriteAMesh(const Ogre::Mesh* pMesh);                                // 写入一个Mesh的数据
	// NodeObject GetNodeObject(const MovableObject *pObject);          
	void CreateSceneGraphNode(int ParentIdx, CSceneNode *pNode);             // 生成一个节点及其子节点的场景图数据
	void GetNodeTypeAndIndex(CSceneNode* pSceneNode, VGS_Node* pVGSNode);    // 获取node节点的类型和附属对象在各自队列中的索引 

	// 保存一个材质对象
	void WriteAMaterial(CMaterial* pMaterial);

	// 遍历所有相机，滤掉没有被控制对象使用的预定义相机
	void FilterCamera();

	// 保存一个Node的动画数据
	void WriteANodeAnimation(CSceneNode *pNode);

	vector<CCamera*>       m_vpCamera;     // 剔除掉预定义相机后的相机列表
	//vector<DWORD>		  m_vMeshList;     // 要保存的Ogre::Mesh对象列表
	vector<CMaterial*>		  m_vMaterialList;  // 要保存的material对象列表
	//vector<DWORD>		  m_vEntityList;  // 要保存的Ogre::Entity对象列表
	vector<CModel*>       m_vpModel;       
	vector<CLight*>		  m_vpLight;
	typedef struct TEntityMesh
	{
		std::string sEntityName;
		int           meshIndex;   // Entity使用的Mesh在m_MeshList中的索引
	}TEntityMesh;

	HWND m_hWnd;  // 接收进度消息的窗体句柄
};
