/////////////////////////////////////////////////////////////////////////
// 名称: CGetE3DFileInfo_3_1--E3D (3.1版本)文件读取类
// 描述: 读取E3D文件，并规整场景数据
// 作者: lixin	
// 日期: 2007-05-10
////////////////////////////////////////////////////////////////////////

#ifndef _CGETE3DFILEINFO_3_1_H
#define _CGETE3DFILEINFO_3_1_H

#include "CScene.h"
#include "E3D_3_1.h"

//-----------------------------------------------------------------------------
//类  名: CGetE3DFileInfo_3_1
//用  途: 用于打开E3D文件，并提取场景数据
//-----------------------------------------------------------------------------
//##ModelId=47EBC74C033C
class CGetE3DFileInfo_3_1  
{
public:
	//##ModelId=47EBC74C033E
	E3D_3_1_Header				Header;				//E3D文件头
	//##ModelId=47EBC74C034E
	E3D_3_1_WindowProp			WindowProp;				//窗体参数

	//##ModelId=47EBC74C0353
	E3D_3_1_AudioList			AudioData;			//声音数据的起始位置
	//##ModelId=47EBC74C0358
	E3D_3_1_FlashList			FlashData;			//Flash数据的起始位置
	
	//##ModelId=47EBC74C035D
	E3D_3_1_CameraList			CameraData;			//摄像机列表
	//##ModelId=47EBC74C0362
	E3D_3_1_MaterialList		MaterialData;		//材质列表
	//##ModelId=47EBC74C0367
	E3D_3_1_LightList			LightData;			//灯光列表

	//##ModelId=47EBC74C036C
	E3D_3_1_VertexList			VertexData;			//顶点列表
	//##ModelId=47EBC74C0371
	E3D_3_1_VertexPosList		VertexPosData;		//压缩后的顶点位置列表
	//##ModelId=47EBC74C0376
	E3D_3_1_VertexNormalList	VertexNormalData;   //压缩后的定点法线列表
	//##ModelId=47EBC74C037B
	E3D_3_1_UVList				UVData;				//顶点UV数据
	//##ModelId=47EBC74C0380
	E3D_3_1_ModelList			ModelData;			//Model列表，模型列表
	//##ModelId=47EBC74C0385
	E3D_3_1_ModelGroup			ModelGroupData;		//模型组列表

	//##ModelId=47EBC74C038A
	E3D_3_1_TextList			TextData;			//文字列表
	//##ModelId=47EBC74C038F
	E3D_3_1_OverlayList			OverlayData;		//Overlay列表
	//##ModelId=47EBC74C0394
	E3D_3_1_NavigaterList		NavigaterData;		//Navigater列表
	//##ModelId=47EBC74C0399
	E3D_3_1_ButtonList			ButtonData;			//按钮列表

	//##ModelId=47EBC74C039E
	E3D_3_1_CubeMapList			CubeMapData;		//CubeMap列表，2.5Version Up
	//##ModelId=47EBC74C03A3
	E3D_3_1_MovieTexList		MovieTexData;		//电影贴图列表
	//##ModelId=47EBC74C03A8
	E3D_3_1_BumpWaterList		BumpWaterData;		//水面铁图列表

	//##ModelId=47EBC74C03AD
	E3D_3_1_Privacy				PrivacyData;		//授权数据

	//##ModelId=47EBC74C03B2
	E3D_3_1_ControlData			ControlData;		//用户控制对象数据

	//##ModelId=47EBC74C03B7
	E3D_3_1_TextureList			TexData;			//贴图列表
	//##ModelId=47EBC74C03BC
	E3D_3_1_ImageList			ImageData;			//图像列表
	//##ModelId=47EBC74C03C1
	E3D_3_1_DynamicTexList		DynamicTexData;		//动态贴图数据

public:
	//##ModelId=47EBC74C03C5
	CHAR						m_strFileName[256];	//文件名
	//##ModelId=47EBC74C03C9
	CScene*						pScene;				//场景
	
	//##ModelId=47EBC74C03CD
	CRITICAL_SECTION*			m_pRenderCS;		//渲染线程管理对象

public:
	//读取文件头
	//##ModelId=47EBC74C03CE
	VOID		ReadHeader();
	//读取窗体信息
	//##ModelId=47EBC74D000F
	VOID		ReadWindowProp();

	//读取声音数据
	//##ModelId=47EBC74D003E
	VOID		ReadAudioData();
	//读取Flash数据
	//##ModelId=47EBC74D005D
	VOID		ReadFlashData();

	//读取摄像机信息
	//##ModelId=47EBC74D008C
	VOID		ReadCameraData();
	//读取材质信息
	//##ModelId=47EBC74D009C
	VOID		ReadMaterialData();
	//读取灯光信息
	//##ModelId=47EBC74D00BB
	VOID		ReadLightData();

	//读取顶点位置信息
	//##ModelId=47EBC74D00BC
	VOID		ReadVertexPosData();
	//读取顶点的法线信息
	//##ModelId=47EBC74D00EA
	VOID		ReadVertexNormalData();
	//读取顶点的UV信息
	//##ModelId=47EBC74D0109
	VOID		ReadVertexUVData();
	//读取顶点信息
	//##ModelId=47EBC74D0138
	VOID		ReadVertexData();
	//读取模型信息
	//##ModelId=47EBC74D0157
	VOID		ReadModelData();
	//读取模型组列表信息
	//##ModelId=47EBC74D0177
	VOID		ReadModelGroupData();
	//读取模型组列表信息
	//##ModelId=47EBC74D0196
	VOID		ReadModelGroupData1(CModelGroup* pModelGroup);

	//读取文字信息
	//##ModelId=47EBC74D01B5
	VOID		ReadTextData();
	//读取Overlay信息
	//##ModelId=47EBC74D01B6
	VOID		ReadOverlayData();
	//读取Navigate信息
	//##ModelId=47EBC74D01C5
	VOID		ReadNavigaterData();
	//读取Button信息
	//##ModelId=47EBC74D01C6
	VOID		ReadButtonData();
	//读取贴图信息
	//##ModelId=47EBC74D01C7
	VOID		ReadTexData();
	//读取Image信息
	//##ModelId=47EBC74D01E4
	VOID		ReadImageData();
	//读取CubeMap信息，2.5版本Up
	//##ModelId=47EBC74D01E5
	VOID		ReadCubeMapData();
	//读取MovieTex信息，2.5版本Up
	//##ModelId=47EBC74D01E6
	VOID		ReadMovieTexData();
	//读取BumpWater信息，2.5版本Up
	//##ModelId=47EBC74D01F4
	VOID		ReadBumpWaterData();

	//读取动态贴图信息
	//##ModelId=47EBC74D01F5
	VOID		ReadDynamicTexData();
	//读取授权信息
	//##ModelId=47EBC74D01F6
	VOID		ReadPrivacyData();
	//读取控制信息
	//##ModelId=47EBC74D01F7
	VOID		ReadContorlData();

	//查找每个模型的引用模型并得到引用模型指针
	//##ModelId=47EBC74D01F8
	VOID		FindReferenceModel();
	//打开对应的Scene场景为E3D文件
	//##ModelId=47EBC74D01F9
	HRESULT		LoadE3DFile(CScene* theScene, CHAR*  strFileName);	
	
	//##ModelId=47EBC74D0204
	VOID		Release();

//------------------------------------------------------------------------------	
//用于数据传递的变量
//------------------------------------------------------------------------------
private:
	//##ModelId=47EBC74D0205
	FILE *pFile;						//E3D文件数据流

public:
	//##ModelId=47EBC74D0213
	CGetE3DFileInfo_3_1(CRITICAL_SECTION* pRenderCS);				//渲染线程管理对象
	//##ModelId=47EBC74D0242
	~CGetE3DFileInfo_3_1();

};

#endif
