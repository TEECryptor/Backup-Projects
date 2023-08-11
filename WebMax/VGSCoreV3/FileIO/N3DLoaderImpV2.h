// GetN3DFileInfo_5.h: interface for the CGetN3DFileInfo_5 class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////
// 名称: CGetN3DFileInfo_5_1-- 场景文件读取类
// 描述: 读取N3D文件或N3D缓冲，并规整场景数据到预渲染队列中
// 作者: Baojiahui	
// 日期: 2007-10-19
////////////////////////////////////////////////////////////////////////

//由于N3D是E3D的压缩文件，所以文件解压后的结构和V3D是一样的，用V3D文件结构即可
#include "..\\VGSHeader.h"
#include "N3D_Def.h"
#include "..\\CScene.h"
#include "..\\SceneResource.h"
#include "..\\gui\CGUIManager.h"
#include "N3DLoaderImpBase.h"
#include "..\\SceneResource.h"
#include "..\\CLight.h"
#include <vector>

#include "..\\VGSGetRoot.h"

using namespace std;
using namespace N3D;

typedef struct N3DOverlayData
{
	BOOL        bCreated;
	N3D_Overlay overlayInfo;
	N3DOverlayData(BOOL b, N3D_Overlay info)
	{
		bCreated = b;
		overlayInfo = info;
	}
}N3DOverlayData;

// 灯光参数动画数据
typedef struct N3DLightParamData
{
	CLight *pLight;
	vector<N3D_OMNI> vOmni;
	vector<N3D_DIRECTIONAL>vDir;
	vector<N3D_SPOTLIGHT> vSpot;
	N3DLightParamData()
	{
		pLight = 0;
	}
}N3DLightParamData;

class VGS_EXPORT CN3DLoaderImpV2 : public CN3DLoaderImpBase , public CVGSGetRoot
{
// 继承自基类的接口
public:
	// 加载一个N3D文件
	virtual DWORD LoadN3DFile(CScene *pScene, const std::string &sFileName, HWND hWnd);
	virtual DWORD LoadSceneFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd);
	virtual DWORD LoadImageDataFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd);
protected:  
	// 读文件头
	virtual void        ReadHeader();
	//读取场景对象指针
	virtual VOID        ReadFileTable();
	//读取窗体信息
	virtual VOID		ReadSceneProp();
	// 读取私有信息
	virtual VOID        ReadPrivacyData();
	//读取摄像机信息
	virtual VOID		ReadCameraData();
	//读取声音数据
	virtual VOID		ReadAudioData();
	//读取Flash数据
	virtual VOID		ReadFlashData();
	//读取灯光信息
	virtual VOID		ReadLightData();
	//读取材质信息
	virtual VOID		ReadMaterialData();
	//读取图像信息
	virtual VOID        ReadImageData();
	//读取模型信息
	virtual VOID        ReadModelData();
	//读取模型组列表信息
	virtual VOID		ReadModelGroupData();
	//读取模型组列表信息
	// virtual VOID		ReadModelGroupData1(CModelGroup* pModelGroup, BYTE* offset);
	//读取文字信息
	virtual VOID		ReadTextData();
	//读取Overlay信息
	virtual VOID		ReadOverlayData();
	//读取Navigate信息
	virtual VOID		ReadNavigaterData();
	//读取Button信息
	virtual VOID		ReadButtonData();
	//读取控制信息
	virtual VOID		ReadControlData();
	//读取节点场景图信息
	virtual VOID        ReadSceneGraph();


protected:
	N3D_SceneProp               sceneProp;                  //场景参数

	N3D_5_1_Table               TableData;                  //文件指针

	DWORD                       ImageDataPtr;
    N3D_AudioList               AudioData;
	N3D_FlashList               FlashData;
	N3D_CameraList			    CameraData;
    N3D_MtrlList                MaterialData;
	N3D_LightList               LightData;

    N3D_TextList			    TextData;			//文字列表
	N3D_OverlayList			    OverlayData;		//Overlay列表
	N3D_NavigaterList		    NavigaterData;		//Navigater列表
	N3D_ButtonList			    ButtonData;			//按钮列表

	N3D_ControlData			    ControlData;		//用户控制对象数据

	N3D_TextureList             texData;
	N3D_ImageList               ImageData;			    //图像列表

	vector <CMaterial*>			m_vMaterial;

//------------------------------------------------------------------------------	
//用于数据传递的变量
//------------------------------------------------------------------------------
protected:
	DWORD				N3DBufSize;							//N3D数据尺寸
	BYTE*				N3DBuffer;							//N3D数据缓冲

	DWORD				V3DBufSize;							//E3D文件的尺寸

	BYTE*				m_pSceneData;						//解压后的场景数据
	BYTE*               m_pImageData;                       // image数据

	INT					m_preModelCnt;						//之前的Model数量
	INT					m_preModelGroupCnt;

	//INT					m_preMtrlCnt;						//在加载本N3D文件之前场景中的材质数量
	INT					m_preTexCnt;		                //在导入本N3D文件之前，场景中已经有的贴图个数
	INT					m_preCubeMapCnt;					//在加载本N3D文件之前场景中的CubeMap数量
	INT					m_preBumpWaterCnt;
	INT					m_preDyTexCnt;
	INT					m_preMovieTexCnt;

	INT					m_preOverlayCnt;					//在加载本N3D文件之前场景中的Overlay数量
	INT					m_preButtonCnt;						//在加载本N3D文件之前场景中的Button数量
	INT					m_preNavigaterCnt;

	INT					m_preCameraCnt;						//在加载本N3D文件之前场景中的Camera数量
	INT                 m_preLightCnt;

	INT					m_ImageIndex;						//图像的编号
	//FILE				*pDebugFile;						//用于调试的数据流


protected:
	CScene*						m_pScene;					//总场景

	CSceneResource*				m_pSceneResource;			//当前场景管理对象
	CRITICAL_SECTION*			m_pRenderCS;		//渲染线程管理对象
	CRITICAL_SECTION*			m_pCtrlCS;			//控制线程管理对象

	CHAR						m_strFileName[256]; //N3D的文件名

	FLOAT        m_fN3DFileVersion;  // 正在操作的N3D文件版本号

protected:
	// 构造场景
	DWORD        ConstructScene(CScene *pScene, CSceneResource* pSceneResource);
	//读取动态贴图信息
	VOID		ReadDynamicTexData();
	// 创建还没有创建的Overlay
	VOID CreateOverlay();

	//如果模型需要辅助渲染表面，则绑定模型到辅助渲染表面——凹凸合成表面
	VOID		BandModelToAddtionalSurface_Bump(CModel* pModel);

	VOID        ConstructSceneGraph(CSceneNode* parentNode, N3D_Node* pNode, BYTE** offset);

	//查找每个模型的引用模型并得到引用模型指针
	VOID		FindReferenceModel();

	//释放过渡数据
	VOID		Release();

	//检查贴图对象是否已经存在
	int			IsTextureExist(std::string texName, INT width, INT height, D3DFORMAT format);

	//创建用到该图像的所有基本贴图对象
	VOID		CreateBasicTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData);

	//创建用到该图像的所有基本贴图对象
	VOID		CreateDynamicTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData);

	//创建该N3D中用到该贴图的CubeMap贴图
	VOID		CreateCubeTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData);

	//判断使用该贴图的材质是否有AlphaBlend--------------------------------------------------------
	VOID		UpdateAllMtrlAlphaBlendStatus(DWORD pTex);

	//从图像构建贴图
	BOOL        ConstructTextureFromImage(N3D_5_1_ZipImageData* pImageData);
	
	D3DTEXTUREOP GetD3DTexOP(const N3D_BlenderOP& n3D_BlenderOP); 


public:
	CN3DLoaderImpV2();
	virtual ~CN3DLoaderImpV2();

protected:
	INT m_SceneWidth;
	INT m_SceneHeight;

	vector <N3DOverlayData> m_vN3DOverlayData;  // n3d中读到的overlay数据

	// 从N3D_Overlay中获取Overlay属性
	VOID CopyOverlayInfo(COverlay *pOverlay, const N3D_Overlay &info);

	vector <N3DLightParamData> m_vLightParam;
	N3DLightParamData getLightParamFrameData(CLight *pLight)
	{
		UINT count = m_vLightParam.size();
		for (UINT i = 0; i < count; ++i)
		{
			if (m_vLightParam[i].pLight == pLight)
			{
				return m_vLightParam[i];
			}
		}

		N3DLightParamData param;
		return param;
	}
};
