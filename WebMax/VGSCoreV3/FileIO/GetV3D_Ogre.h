/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // GetV3D.h
* 内容摘要： // 读取V3D类定义
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
	void Release(); // 释放内存

	// 打开V3D文件并加载进场景
	DWORD OpenV3DFile(CVGS_SceneManager* pVGSSceneMgr, const CHAR* strFileName);

	// 获取V3D文件的Table目录
	bool GetV3DTable(const CHAR* strFileName, V3D_Table &Table);

	// 得到texture数据在文件中的范围, start: 开始位置, len:数据长度
	void GetTextureDataRange(const V3D_Table &table, DWORD &start, DWORD &len);

private:

	CVGS_SceneManager* m_pVGSSceneMgr;  // VGVS 场景管理器

	FILE *m_pFile;  // 要读的文件指针

	String m_sV3DPath;  // 当前V3D文件所在的目录

	float m_V3DFileVersion; // 要打开的V3D文件版本号

	V3D_Table m_V3DTable; // V3D文件中各数据块的开始位置目录

	V3D_SceneProp m_SceneProperty;

	// vector <V3D_CameraDetail> m_vCameraDetail; 
	vector <Camera*> m_vpCamera;  // 加载的相机
	vector <Light*>  m_vpLight;   // 加载的灯光
	vector <Mesh*>   m_vMesh;     // 加载的Mesh对象列表
	vector <DWORD>   m_vEntityList;  // 加载的Entity对象列表

	Ogre::String sDefaultTexturePath;    // 缺省的贴图路径

	void SetCurrentDirctory(const char *sFileName);  // 根据v3d文件名，得到当前的V3D目录

	char *m_pTextureBuf;
	char *m_pMaterailBuf;
	char *m_pEntityBuf;  // 模型信息数据
	char *m_pMeshBuf;    // 模型资源数据
	char *m_pCameraBuf;
	char *m_pLightBuf;
	char *m_pOverlayBuf;
	char *m_pButtonBuf;
	char *m_pNaviBuf;
	char *m_pBackdropBuf; // 背景图 2008-07-17
	char *m_pTextBuf;
	char *m_pPrivacyBuf;
	char *m_pFlashBuf;
	char *m_pAudioBuf;
	char *m_pControlBuf;
	char *m_pAnimationBuf;
	char *m_pSceneGraphBuf;
	DWORD	m_Offset;     //记录偏移量的变量

	void ReadPass(Pass *pPass, char** OffsetPtr);
	void ReadPassTexture(Pass *pPass, char** OffsetPtr);
	void ReadPassTexture_old(Pass *pPass, char** OffsetPtr);  // 读50004及之前版本用


	// 读取所有数据
	DWORD ReadAllData();

	DWORD ReadHeader();
	DWORD ReadFileTable();
	DWORD ReadWindowProp();
	DWORD ReadTextureData();
	DWORD ReadMaterialData();
	
	DWORD ReadEnityData();    // 读取模型数据
	DWORD ReadMeshData();     // 读取模型资源数据，多个模型可能使用相同的模型资源（例如顶点数据）
	DWORD ReadCameraData();
	DWORD ReadLightData();
	DWORD ReadTextData();

	//Read the overlay data
	DWORD ReadOverlayData();
	//Read the navigater data
	DWORD ReadNavigaterData();
	//Read the button data
	DWORD ReadButtonData();

	// 背景图
	DWORD ReadBackdropData();

	//Read AudioData
	DWORD ReadAudioData();

	//Read FlashData
	DWORD ReadFlashData();

	// 动画
	DWORD ReadAnimationData();

	//Read privacy data
	DWORD ReadPrivacyData();

	// 控制
	DWORD ReadControlData();

	// 场景图
	DWORD ReadSceneGraph();

	//递归创建场景
	void ConstructSceneGraph(SceneNode* pNode, char* offset);
	
	//创建节点的动画
	void ReadAndCreateNodeAnimation(SceneNode* pNode, float animationLength, char** offset);
	
	//创建相机的参数动画
	void CreateCameraParamAnimation(Ogre::Camera* pCamera, VGS_V3D_Camera* pVGSCamera, int frameCnt);
	//创建灯光参数动画
	void CreateLightParamAnimation(Light* pLight, VGS_Light* pVGSLight, int frameCnt, char** Offset);

	// 给指定的纯文件名添加路径
	Ogre::String AddPathtoTextureFile(const char * sPureFileName);

	// 更新法线贴图中的灯光句柄
	VOID UpdateBumpNormalmapLight();

	// 处理打开完成后的一些工作
	VOID PostOpen();

};

