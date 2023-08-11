/////////////////////////////////////////////////////////////////////////
// 名称: CGetV3DFileInfo_3_2-- V3D 3.2版本文件读取类
// 描述: 读取V3D文件，并规整场景数据
// 作者: lixin	
// 日期: 2007-05-09
////////////////////////////////////////////////////////////////////////

#ifndef _CGetV3DFileInfo_3_2_H
#define _CGetV3DFileInfo_3_2_H

#include "CScene.h"
#include "V3D_3_2.h"

//-----------------------------------------------------------------------------
//类  名: CGetV3DFileInfo
//用  途: 用于打开V3D文件，并提取场景数据
//-----------------------------------------------------------------------------
//##ModelId=47EBC74901E4
class CGetV3DFileInfo_3_2 
{
	//V3D文件包含的数据解析为对应的数据结构
public:
	//##ModelId=47EBC74901E6
	V3D_3_2_Header				Header;				//E3D文件头
	//##ModelId=47EBC74901EB
	V3D_3_2_WindowProp			WindowProp;			//窗体参数
	
	//##ModelId=47EBC74901F0
	V3D_3_2_AudioList			AudioData;			//声音数据的起始位置
	//##ModelId=47EBC74901F5
	V3D_3_2_FlashList			FlashData;			//Flash数据的起始位置
	
	//##ModelId=47EBC74901FA
	V3D_3_2_CameraList			CameraData;			//摄像机列表
	//##ModelId=47EBC74901FF
	V3D_3_2_MaterialList		MaterialData;		//材质列表
	//##ModelId=47EBC7490204
	V3D_3_2_LightList			LightData;			//灯光列表

	//##ModelId=47EBC7490209
	V3D_3_2_VertexList			VertexData;			//顶点列表
	//##ModelId=47EBC749020E
	V3D_3_2_VertexPosList		VertexPosData;		//压缩后的顶点位置列表
	//##ModelId=47EBC7490213
	V3D_3_2_VertexNormalList	VertexNormalData;   //压缩后的定点法线列表
	//##ModelId=47EBC7490218
	V3D_3_2_UVList				UVData;				//顶点UV数据
	//##ModelId=47EBC749021D
	V3D_3_2_ModelList			ModelData;			//Model列表，模型列表
	//##ModelId=47EBC7490222
	V3D_3_2_ModelGroup			ModelGroupData;		//模型组列表

	//##ModelId=47EBC7490227
	V3D_3_2_TextList			TextData;			//文字列表
	//##ModelId=47EBC749022C
	V3D_3_2_OverlayList			OverlayData;		//Overlay列表
	//##ModelId=47EBC7490231
	V3D_3_2_NavigaterList		NavigaterData;		//Navigater列表
	//##ModelId=47EBC7490236
	V3D_3_2_ButtonList			ButtonData;			//按钮列表

	//##ModelId=47EBC749023B
	V3D_3_2_CubeMapList			CubeMapData;		//CubeMap列表，2.5Version Up
	//##ModelId=47EBC7490240
	V3D_3_2_MovieTexList		MovieTexData;		//电影贴图列表
	//##ModelId=47EBC7490245
	V3D_3_2_BumpWaterList		BumpWaterData;		//水面铁图列表

	//##ModelId=47EBC749024A
	V3D_3_2_DynamicTexList		DynamicTexData;		//动态贴图数据起始位置

	//##ModelId=47EBC749024F
	V3D_3_2_Privacy				PrivacyData;		//授权数据

	//##ModelId=47EBC7490254
	V3D_3_2_ControlData			ControlData;		//用户控制对象数据

	//##ModelId=47EBC7490259
	V3D_3_2_TextureList			TexData;			//贴图列表
	//##ModelId=47EBC749025E
	V3D_3_2_ImageList			ImageData;			//图像列表

public:
	//##ModelId=47EBC7490262
	CHAR						m_strFileName[256];	//文件名
	//##ModelId=47EBC7490264
	CScene*						pScene;				//场景
	
	//##ModelId=47EBC7490268
	CRITICAL_SECTION*			m_pRenderCS;		//渲染线程管理对象

public:
	//读取文件头
	//##ModelId=47EBC7490271
	VOID		ReadHeader();
	//读取窗体信息
	//##ModelId=47EBC749029F
	VOID		ReadWindowProp();

	//读取声音数据
	//##ModelId=47EBC74902CE
	VOID		ReadAudioData();
	//读取Flash数据
	//##ModelId=47EBC74902FD
	VOID		ReadFlashData();

	//读取摄像机信息
	//##ModelId=47EBC749032C
	VOID		ReadCameraData();
	//读取材质信息
	//##ModelId=47EBC749033C
	VOID		ReadMaterialData();
	//读取灯光信息
	//##ModelId=47EBC749034B
	VOID		ReadLightData();

	//读取顶点位置信息
	//##ModelId=47EBC749035B
	VOID		ReadVertexPosData();
	//读取顶点的法线信息
	//##ModelId=47EBC749037A
	VOID		ReadVertexNormalData();
	//读取顶点的UV信息
	//##ModelId=47EBC7490399
	VOID		ReadVertexUVData();
	//读取顶点信息
	//##ModelId=47EBC74903C8
	VOID		ReadVertexData();
	//读取模型信息
	//##ModelId=47EBC74A0000
	VOID		ReadModelData();
	//读取模型组列表信息
	//##ModelId=47EBC74A001F
	VOID		ReadModelGroupData();
	//读取模型组列表信息
	//##ModelId=47EBC74A003E
	VOID		ReadModelGroupData1(CModelGroup* pModelGroup);

	//读取文字信息
	//##ModelId=47EBC74A006D
	VOID		ReadTextData();
	//读取Overlay信息
	//##ModelId=47EBC74A007D
	VOID		ReadOverlayData();
	//读取Navigate信息
	//##ModelId=47EBC74A007E
	VOID		ReadNavigaterData();
	//读取Button信息
	//##ModelId=47EBC74A007F
	VOID		ReadButtonData();
	//读取贴图信息
	//##ModelId=47EBC74A008C
	VOID		ReadTexData();
	//读取Image信息
	//##ModelId=47EBC74A009C
	VOID		ReadImageData();
	//读取CubeMap信息，2.5版本Up
	//##ModelId=47EBC74A009D
	VOID		ReadCubeMapData();
	//读取MovieTex信息，2.5版本Up
	//##ModelId=47EBC74A009E
	VOID		ReadMovieTexData();
	//读取BumpWater信息，2.5版本Up
	//##ModelId=47EBC74A00AB
	VOID		ReadBumpWaterData();

	//读取动态贴图信息
	//##ModelId=47EBC74A00AC
	VOID		ReadDynamicTexData();
	//读取授权信息
	//##ModelId=47EBC74A00CB
	VOID		ReadPrivacyData();

	//读取控制信息
	//##ModelId=47EBC74A00FA
	VOID		ReadContorlData();

	//查找每个模型的引用模型并得到引用模型指针
	//##ModelId=47EBC74A0138
	VOID		FindReferenceModel();
	//打开对应的Scene场景为E3D文件
	//##ModelId=47EBC74A0139
	HRESULT		LoadV3DFile(CScene* theScene, CHAR*  strFileName);	
	
	//##ModelId=47EBC74A0148
	VOID		Release();

//------------------------------------------------------------------------------	
//用于数据传递的变量
//------------------------------------------------------------------------------
private:
	//##ModelId=47EBC74A0149
	FILE		*pFile;						//E3D文件数据流

public:
	//##ModelId=47EBC74A014A
	CGetV3DFileInfo_3_2(CRITICAL_SECTION* pRenderCS);				//渲染线程管理对象
	//##ModelId=47EBC74A0186
	~CGetV3DFileInfo_3_2();

};

#endif
