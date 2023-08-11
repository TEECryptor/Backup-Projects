/////////////////////////////////////////////////////////////////////////
// 名称: CGetV3DFileInfo_3_1 -- V3D 1.0版本文件读取类
// 描述: 读取V3D文件，并规整场景数据
// 作者: lixin	
// 日期: 2007-05-09
////////////////////////////////////////////////////////////////////////

#ifndef _CGetV3DFileInfo_3_1_H
#define _CGetV3DFileInfo_3_1_H

#include "CScene.h"
#include "V3D_3_1.h"

//-----------------------------------------------------------------------------
//类  名: CGetV3DFileInfo
//用  途: 用于打开V3D文件，并提取场景数据
//-----------------------------------------------------------------------------
//##ModelId=47EBC74A0232
class CGetV3DFileInfo_3_1  
{
	//V3D文件包含的数据解析为对应的数据结构
public:
	//##ModelId=47EBC74A0234
	V3D_3_1_Header				Header;				//E3D文件头
	//##ModelId=47EBC74A0243
	V3D_3_1_WindowProp			WindowProp;			//窗体参数
	
	//##ModelId=47EBC74A0248
	V3D_3_1_AudioList			AudioData;			//声音数据的起始位置
	//##ModelId=47EBC74A024D
	V3D_3_1_FlashList			FlashData;			//Flash数据的起始位置
	
	//##ModelId=47EBC74A0252
	V3D_3_1_CameraList			CameraData;			//摄像机列表
	//##ModelId=47EBC74A0257
	V3D_3_1_MaterialList		MaterialData;		//材质列表
	//##ModelId=47EBC74A025C
	V3D_3_1_LightList			LightData;			//灯光列表

	//##ModelId=47EBC74A0261
	V3D_3_1_VertexList			VertexData;			//顶点列表
	//##ModelId=47EBC74A0266
	V3D_3_1_VertexPosList		VertexPosData;		//压缩后的顶点位置列表
	//##ModelId=47EBC74A026B
	V3D_3_1_VertexNormalList	VertexNormalData;   //压缩后的定点法线列表
	//##ModelId=47EBC74A0270
	V3D_3_1_UVList				UVData;				//顶点UV数据
	//##ModelId=47EBC74A0275
	V3D_3_1_ModelList			ModelData;			//Model列表，模型列表
	//##ModelId=47EBC74A027A
	V3D_3_1_ModelGroup			ModelGroupData;		//模型组列表

	//##ModelId=47EBC74A027F
	V3D_3_1_TextList			TextData;			//文字列表
	//##ModelId=47EBC74A0284
	V3D_3_1_OverlayList			OverlayData;		//Overlay列表
	//##ModelId=47EBC74A0289
	V3D_3_1_NavigaterList		NavigaterData;		//Navigater列表
	//##ModelId=47EBC74A028E
	V3D_3_1_ButtonList			ButtonData;			//按钮列表

	//##ModelId=47EBC74A0293
	V3D_3_1_CubeMapList			CubeMapData;		//CubeMap列表，2.5Version Up
	//##ModelId=47EBC74A0298
	V3D_3_1_MovieTexList		MovieTexData;		//电影贴图列表
	//##ModelId=47EBC74A029D
	V3D_3_1_BumpWaterList		BumpWaterData;		//水面铁图列表

	//##ModelId=47EBC74A02A2
	V3D_3_1_DynamicTexList		DynamicTexData;		//动态贴图数据起始位置

	//##ModelId=47EBC74A02A7
	V3D_3_1_Privacy				PrivacyData;		//授权数据

	//##ModelId=47EBC74A02AC
	V3D_3_1_ControlData			ControlData;		//用户控制对象数据

	//##ModelId=47EBC74A02B1
	V3D_3_1_TextureList			TexData;			//贴图列表
	//##ModelId=47EBC74A02B6
	V3D_3_1_ImageList			ImageData;			//图像列表

public:
	//##ModelId=47EBC74A02BA
	CHAR						m_strFileName[256];	//文件名
	//##ModelId=47EBC74A02BC
	CScene*						pScene;				//场景
	
	//##ModelId=47EBC74A02C0
	CRITICAL_SECTION*			m_pRenderCS;		//渲染线程管理对象

public:
	//读取文件头
	//##ModelId=47EBC74A02C1
	VOID		ReadHeader();
	//读取窗体信息
	//##ModelId=47EBC74A02FD
	VOID		ReadWindowProp();

	//读取声音数据
	//##ModelId=47EBC74A032C
	VOID		ReadAudioData();
	//读取Flash数据
	//##ModelId=47EBC74A035B
	VOID		ReadFlashData();

	//读取摄像机信息
	//##ModelId=47EBC74A038A
	VOID		ReadCameraData();
	//读取材质信息
	//##ModelId=47EBC74A0399
	VOID		ReadMaterialData();
	//读取灯光信息
	//##ModelId=47EBC74A03A9
	VOID		ReadLightData();

	//读取顶点位置信息
	//##ModelId=47EBC74A03B9
	VOID		ReadVertexPosData();
	//读取顶点的法线信息
	//##ModelId=47EBC74A03D8
	VOID		ReadVertexNormalData();
	//读取顶点的UV信息
	//##ModelId=47EBC74B001F
	VOID		ReadVertexUVData();
	//读取顶点信息
	//##ModelId=47EBC74B003E
	VOID		ReadVertexData();
	//读取模型信息
	//##ModelId=47EBC74B005D
	VOID		ReadModelData();
	//读取模型组列表信息
	//##ModelId=47EBC74B007D
	VOID		ReadModelGroupData();
	//读取模型组列表信息
	//##ModelId=47EBC74B00AB
	VOID		ReadModelGroupData1(CModelGroup* pModelGroup);

	//读取文字信息
	//##ModelId=47EBC74B00DA
	VOID		ReadTextData();
	//读取Overlay信息
	//##ModelId=47EBC74B00EA
	VOID		ReadOverlayData();
	//读取Navigate信息
	//##ModelId=47EBC74B00EB
	VOID		ReadNavigaterData();
	//读取Button信息
	//##ModelId=47EBC74B00EC
	VOID		ReadButtonData();
	//读取贴图信息
	//##ModelId=47EBC74B00FA
	VOID		ReadTexData();
	//读取Image信息
	//##ModelId=47EBC74B0109
	VOID		ReadImageData();
	//读取CubeMap信息，2.5版本Up
	//##ModelId=47EBC74B010A
	VOID		ReadCubeMapData();
	//读取MovieTex信息，2.5版本Up
	//##ModelId=47EBC74B010B
	VOID		ReadMovieTexData();
	//读取BumpWater信息，2.5版本Up
	//##ModelId=47EBC74B010C
	VOID		ReadBumpWaterData();

	//读取动态贴图信息
	//##ModelId=47EBC74B0119
	VOID		ReadDynamicTexData();
	//读取授权信息
	//##ModelId=47EBC74B0138
	VOID		ReadPrivacyData();

	//读取控制信息
	//##ModelId=47EBC74B0177
	VOID		ReadContorlData();

	//查找每个模型的引用模型并得到引用模型指针
	//##ModelId=47EBC74B01A5
	VOID		FindReferenceModel();
	//打开对应的Scene场景为E3D文件
	//##ModelId=47EBC74B01A6
	HRESULT		LoadV3DFile(CScene* theScene, CHAR*  strFileName);	
	
	//##ModelId=47EBC74B01B5
	VOID		Release();

//------------------------------------------------------------------------------	
//用于数据传递的变量
//------------------------------------------------------------------------------
private:
	//##ModelId=47EBC74B01B6
	FILE		*pFile;						//E3D文件数据流

public:
	//##ModelId=47EBC74B01B7
	CGetV3DFileInfo_3_1(CRITICAL_SECTION* pRenderCS);				//渲染线程管理对象
	//##ModelId=47EBC74B01F4
	~CGetV3DFileInfo_3_1();

};

#endif
