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
#include "..\VGSHeader.h"
#include "..\CCamera.h"
#include "..\CLight.h"
#include "..\CModel.h"
#include "GetV3DBase.h"
#include "..\\VGSDataStruct.h"
#include "..\VGSGetRoot.h"
//----------------------------------------------------------
#define RETURN(RE)                       {if (RE != ERR_COMMON_OK)return RE;} 

// 删除数组
#define DELETE_ARRAYPTR(Arrayptr)        {if (Arrayptr) {delete [] Arrayptr; Arrayptr = NULL;}}                                

// 从Offset位置拷贝几个字节
#define CPY_FROM_STREAM(value, Offset)   {memcpy(&value, Offset, sizeof(value)); Offset += sizeof(value);  m_Offset += sizeof(value);} 

// 从offset位置拷贝一个vector3数据结构
#define CPY_VECTOR3_FROM_STRRAM(V3, Offset)\
{\
	CPY_FROM_STREAM((V3.x), Offset)\
	CPY_FROM_STREAM((V3.y), Offset)\
	CPY_FROM_STREAM((V3.z), Offset)\
}

// 从offset位置拷贝一段数据到一段内存
#define CPY_PTR_FROM_STREAM(ptr, n, Offset)   {memcpy(ptr, Offset, n); Offset += n;} 

// 从文件中读一个Vector3
#define READ_VECTOR3(V3, pFile) {fread(&(V3.x), sizeof(V3.x), 1, pFile); fread(&(V3.y), sizeof(V3.y), 1, pFile); fread(&(V3.z), sizeof(V3.z), 1, pFile);}

//----------------------------------------------------------
class CGetV3DImpV2 : public CGetV3DBase , public CVGSGetRoot
{

public:
	CGetV3DImpV2(/*LPDIRECT3DDEVICE9 pd3dDevice*/);
	virtual ~CGetV3DImpV2();

public: // 继承自接口

	void Release(); // 释放内存

	// 打开V3D文件并加载进场景
	virtual DWORD OpenV3DFile(CScene* pVGSSceneMgr, const std::string &strFileName, HWND hWnd);

	// 获取V3D文件的Table目录
	virtual bool GetV3DTable(const std::string &strFileName, V3D_Table &Table);

	// 得到texture数据在文件中的范围, start: 开始位置, len:数据长度
	virtual void GetTextureDataRange(const V3D_Table &table, DWORD &start, DWORD &len);

	virtual BOOL isMyVersion(const std::string &sFileName);  // 是否是我支持的版本

protected:

	HWND  m_hWnd;  // 接收进度消息的窗体句柄

	//LPDIRECT3DDEVICE9  m_pD3dDevice;

	CScene* m_pVGSSceneMgr;  // VGVS 场景管理器

	FILE *m_pFile;  // 要读的文件指针

	std::string m_sV3DPath;  // 当前V3D文件所在的目录

	float m_V3DFileVersion; // 要打开的V3D文件版本号

	V3D_Table m_V3DTable; // V3D文件中各数据块的开始位置目录

	V3D_SceneProp m_SceneProperty;

	// vector <V3D_CameraDetail> m_vCameraDetail; 
	vector <CCamera*> m_vpCamera;  // 加载的相机
	vector <CLight*>  m_vpLight;   // 加载的灯光
	vector <CModel*>  m_vModel;  // 加载的model对象列表
	vector <CVGSMesh> m_vMesh;
	vector <CMaterial*>	m_vMaterial;

	std::string m_sDefaultTexturePath;    // 缺省的贴图路径

	void SetCurrentDirctory(const std::string &sFileName);  // 根据v3d文件名，得到当前的V3D目录

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

	// 读取所有数据
	virtual DWORD ReadAllData();

	virtual DWORD ReadHeader();
	virtual DWORD ReadFileTable();
	virtual DWORD ReadWindowProp();
	virtual DWORD ReadTextureData();
	virtual DWORD ReadMaterialData();

	virtual DWORD ReadMeshData();     // 读取模型数据
	virtual DWORD ReadModelData();    // 读取模型数据
	virtual DWORD ReadCameraData();
	virtual DWORD ReadLightData();
	virtual DWORD ReadTextData();

	//Read the overlay data
	virtual DWORD ReadOverlayData();
	//Read the navigater data
	virtual DWORD ReadNavigaterData();
	//Read the button data
	virtual DWORD ReadButtonData();

	// 背景图
	virtual DWORD ReadBackdropData();

	//Read AudioData
	virtual DWORD ReadAudioData();

	//Read FlashData
	virtual DWORD ReadFlashData();

	// 动画
	virtual DWORD ReadAnimationData();

	//Read privacy data
	virtual DWORD ReadPrivacyData();

	// 控制
	virtual DWORD ReadControlData();

	// 场景图
	virtual DWORD ReadSceneGraph();

	//递归创建场景
	virtual void ConstructSceneGraph(CSceneNode* pNode, char* offset);
	
	//创建节点的动画
	virtual void ReadAndCreateNodeAnimation(CSceneNode* pNode, float animationLength, char** offset);
	
	//创建相机的参数动画
	virtual void CreateCameraParamAnimation(CCamera* pCamera, VGS_V3D_Camera* pVGSCamera, int frameCnt);
	virtual BOOL CreateCameraNodeAnimation(CSceneNode* pSceneNode, CCamera* pCamera, const std::vector<CNodeTransform> &vTransform);
	
	//创建灯光参数动画
	virtual void CreateLightParamAnimation(CLight* pLight, VGS_Light* pVGSLight, int frameCnt, char** Offset);
	virtual BOOL CreateLightNodeAnimation(CSceneNode* pSceneNode, CLight* pLight, const std::vector<CNodeTransform> &vTransform);

	// 给指定的纯文件名添加路径
	virtual std::string AddPathtoTextureFile(const std::string &sPureFileName);

	// 更新法线贴图中的灯光句柄
	virtual VOID UpdateBumpNormalmapLight();

	// 处理打开完成后的一些工作
	virtual VOID PostOpen();

	UINT m_ProcessTotalSize;  // 进度条的总进度
	UINT m_ProcessCurSize;    // 进度条的当前进度

	std::vector<VGS_V3D_Camera>	m_arCameraFrameInfo;
	std::vector<VGS_V3D_Light>	m_arLightFrameInfo;
};

