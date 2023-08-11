// GetN3DFileInfo_5.h: interface for the CGetN3DFileInfo_5 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETN3DFILEINFO_5_H__6D920467_70D4_479B_B084_0237A5D88D9A__INCLUDED_)
#define AFX_GETN3DFILEINFO_5_H__6D920467_70D4_479B_B084_0237A5D88D9A__INCLUDED_

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
#include "..\\VGSATLHeader.h"
#include "N3D_Def.h"
// #include "..\\vgszip\\vgszip.h"
// #include "SceneResource.h"

#include "..\\VGSGetCore.h"

using namespace N3D;

class CGetN3DFileInfo_5  : public CVGSGetCore
{
public:
	N3D_SceneProp               sceneProp;                  //场景参数

    N3D_5_1_Table               TableData;                  //文件指针

	DWORD                       ImageDataPtr;
	//E3D_3_1_WindowProp			WindowProp;				//窗体参数
    N3D_AudioList               AudioData;
	//E3D_3_1_AudioList			AudioData;			//声音数据的起始位置
	N3D_FlashList               FlashData;
	//E3D_3_1_FlashList			FlashData;			//Flash数据的起始位置
	N3D_CameraList			    CameraData;
	//E3D_3_1_CameraList			CameraData;			//摄像机列表
    N3D_MtrlList                MaterialData;
	//E3D_3_1_MaterialList		MaterialData;		//材质列表
	N3D_LightList               LightData;
	//E3D_3_1_LightList			LightData;			//灯光列表
    N3D_ModelList               ModelData;         //模型列表
/*
	E3D_3_1_VertexList			VertexData;			//顶点列表
	E3D_3_1_VertexPosList		VertexPosData;		//压缩后的顶点位置列表
	E3D_3_1_VertexNormalList	VertexNormalData;   //压缩后的定点法线列表
	E3D_3_1_UVList				UVData;				//顶点UV数据
	E3D_3_1_ModelList			ModelData;			//Model列表，模型列表*/
    N3D_ModelGroup              ModelGroupData;
	//E3D_3_1_ModelGroup			ModelGroupData;		//模型组列表

    N3D_TextList			    TextData;			//文字列表
	N3D_OverlayList			    OverlayData;		//Overlay列表
	N3D_NavigaterList		    NavigaterData;		//Navigater列表
	N3D_ButtonList			    ButtonData;			//按钮列表

	
	/*
	E3D_3_1_TextList			TextData;			//文字列表
	E3D_3_1_OverlayList			OverlayData;		//Overlay列表
	E3D_3_1_NavigaterList		NavigaterData;		//Navigater列表
	E3D_3_1_ButtonList			ButtonData;			//按钮列表

	E3D_3_1_CubeMapList			CubeMapData;		//CubeMap列表，2.5Version Up
	E3D_3_1_MovieTexList		MovieTexData;		//电影贴图列表
	E3D_3_1_BumpWaterList		BumpWaterData;		//水面铁图列表*/

	N3D_ControlData			    ControlData;		//用户控制对象数据

	N3D_TextureList             texData;
	//E3D_3_1_TextureList			TexData;			//贴图列表
	N3D_ImageList               ImageData;			    //图像列表



//	N3D_NormalTexList           NormalTexData;
	
	//N3D_OpaTexList              OpaTexData;
    
	//N3D_DynamicTexList          DynamicTexData;
//------------------------------------------------------------------------------	
//用于数据传递的变量
//------------------------------------------------------------------------------
public:
	DWORD				N3DBufSize;							//N3D数据尺寸
	BYTE*				N3DBuffer;							//N3D数据缓冲

	DWORD				V3DBufSize;							//E3D文件的尺寸

	BYTE*				m_pSceneData;						//解压后的场景数据

	INT					m_preModelCnt;						//之前的Model数量
	INT					m_preModelGroupCnt;

	INT					m_preMtrlCnt;						//在加载本N3D文件之前场景中的材质数量
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
	FILE				*pDebugFile;						//用于调试的数据流


public:
	//DWORD						m_pScene;					//总场景

	DWORD/*CSceneResource**/				m_pSceneResource;			//当前场景管理对象
	//CRITICAL_SECTION*			m_pRenderCS;		//渲染线程管理对象
	//CRITICAL_SECTION*			m_pCtrlCS;			//控制线程管理对象

	CHAR						m_strFileName[256]; //N3D的文件名
	//E3D_3_1_DynamicTexList		DynamicTexData;		//动态贴图数据
//----------------------------------------------------------------
//读取方法
//----------------------------------------------------------------
public:
	//解压场景数据，写入解压后的数据到destBuf
	BOOL		UnzipSceneData(BYTE* srcBuf, DWORD srcBufSize, DWORD destBufSize);    
	//构造场景
	BOOL        ConstructScene(const std::string &sResourceGoupName/*CHAR* sceneName, DWORD pSceneResource*/);
	//读取场景对象指针
 	// VOID        ReadFileTable();

	// 设置文件头信息
	VOID SetHeaderInfo(N3D_Header N3DHeader);

// 	//VOID		ReadHeader();
// 	//读取窗体信息
// 	VOID		ReadSceneProp();
// 
// 	//读取声音数据
// 	VOID		ReadAudioData();
// 	//读取Flash数据
// 	VOID		ReadFlashData();
// 
// 	//读取摄像机信息
// 	VOID		ReadCameraData();
// 	//读取材质信息
// 	VOID		ReadMaterialData();
// 	//读取灯光信息
// 	VOID		ReadLightData();
    //读取图像信息
	VOID        ReadImageData(BYTE *pImageDataBuffer);
// 	//读取模型信息
// 	VOID        ReadModelData();
// 
// 	//读取模型组列表信息
// 	VOID		ReadModelGroupData();
// 	//读取模型组列表信息
// 	// VOID		ReadModelGroupData1(CModelGroup* pModelGroup, BYTE* offset);
// 
// 	//读取文字信息
// 	VOID		ReadTextData();
// 	//读取Overlay信息
// 	VOID		ReadOverlayData();
// 	//读取Navigate信息
// 	VOID		ReadNavigaterData();
// 	//读取Button信息
// 	VOID		ReadButtonData();
// 
// 	//读取动态贴图信息
// 	VOID		ReadDynamicTexData();
// 	//读取控制信息
// 	VOID		ReadControlData();
    //读取节点场景图信息
	//VOID        ReadSceneGraph();

	//如果模型需要辅助渲染表面，则绑定模型到辅助渲染表面——凹凸合成表面
	//VOID		BandModelToAddtionalSurface_Bump(CModel* pModel);

	//VOID        ConstructSceneGraph(CSceneNode** parentNode, N3D_Node* pNode, BYTE** offset);

	//查找每个模型的引用模型并得到引用模型指针
// 	VOID		FindReferenceModel();
// 
 	//释放过渡数据
 	VOID		Release();
// 
// 	//检查贴图对象是否已经存在
// 	int			IsTextureExist(std::string texName, INT width, INT height, D3DFORMAT format);
// 
// 	//创建用到该图像的所有基本贴图对象
// 	VOID		CreateBasicTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData);
// 
// 	//创建用到该图像的所有基本贴图对象
// 	VOID		CreateDynamicTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData);
// 
// 	//创建该N3D中用到该贴图的CubeMap贴图
// 	VOID		CreateCubeTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData);
// 
// 	//判断使用该贴图的材质是否有AlphaBlend--------------------------------------------------------
// 	VOID		UpdateAllMtrlAlphaBlendStatus(DWORD pTex);
// 
 	//从图像构建贴图
 	BOOL        ConstructTextureFromImage(N3D_5_1_ZipImageData* pImageData, INT imgIndex);
// 	
// 	D3DTEXTUREOP GetTexOP(const N3D_BlenderOP& n3D_BlenderOP); 

public:
	CGetN3DFileInfo_5(/*DWORD pScene*/);
	virtual ~CGetN3DFileInfo_5();

private:
	N3D_Header m_N3DHeader;
};

#endif // !defined(AFX_GETN3DFILEINFO_5_H__6D920467_70D4_479B_B084_0237A5D88D9A__INCLUDED_)
