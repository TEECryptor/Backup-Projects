/////////////////////////////////////////////////////////////////////////
// 名称: CGetE3DFileInfo--E3D文件读取类
// 描述: 读取E3D文件，并规整场景数据
// 作者: lixin	
// 日期: 2006-05-06
////////////////////////////////////////////////////////////////////////

#ifndef _CGETE3DFILEINFO_H
#define _CGETE3DFILEINFO_H

#include "E3D.h"
#include "CScene.h"

//-----------------------------------------------------------------------------
//类  名: CGetE3DFileInfo
//用  途: 用于打开E3D文件，并提取场景数据
//-----------------------------------------------------------------------------
//##ModelId=47EBC74D02AF
class CGetE3DFileInfo  
{

public:
	//##ModelId=47EBC74D02B1
	_E3DHeader			Header;				//E3D文件头
	//##ModelId=47EBC74D02B6
	_CameraList			CameraData;			//摄像机列表
	//##ModelId=47EBC74D02C0
	_MaterialList		MaterialData;		//材质列表
	//##ModelId=47EBC74D02C5
	_LightList			LightData;			//灯光列表
	//##ModelId=47EBC74D02CA
	_ModelList			ModelData;			//Model列表，模型列表

	//##ModelId=47EBC74D02CF
	_VertexList			VertexData;			//顶点列表
	//##ModelId=47EBC74D02D4
	_VertexPosList		VertexPosData;		//压缩后的顶点位置列表
	//##ModelId=47EBC74D02D9
	_VertexNormalList	VertexNormalData;   //压缩后的定点法线列表
	//##ModelId=47EBC74D02DE
	_UVList				UVData;				//顶点UV数据


	//##ModelId=47EBC74D02E3
	_TextList			TextData;			//文字列表
	//##ModelId=47EBC74D02E8
	_OverlayList		OverlayData;		//Overlay列表
	//##ModelId=47EBC74D02ED
	_NavigaterList		NavigaterData;		//Navigater列表
	//##ModelId=47EBC74D02F2
	_ButtonList			ButtonData;			//按钮列表

	//##ModelId=47EBC74D02F7
	_CubeMapList		CubeMapData;		//CubeMap列表，2.5Version Up
	//##ModelId=47EBC74D02FC
	_MovieTexList		MovieTexData;		//电影贴图列表
	//##ModelId=47EBC74D0301
	_BumpWaterList		BumpWaterData;		//水面铁图列表

	//##ModelId=47EBC74D0306
	_TextureList		TexData;			//贴图列表
	//##ModelId=47EBC74D030B
	_ImageList			ImageData;			//图像列表

	//##ModelId=47EBC74D030F
	CHAR				m_strFileName[256];	//文件名
	//##ModelId=47EBC74D0311
	CScene*				pScene;				//场景
	
	//##ModelId=47EBC74D0315
	CRITICAL_SECTION*	m_pRenderCS;		//渲染线程管理对象

	//读取文件头
	//##ModelId=47EBC74D0316
	VOID		ReadHeader();
	//读取摄像机信息
	//##ModelId=47EBC74D032C
	VOID		ReadCameraData();
	//读取材质信息
	//##ModelId=47EBC74D033C
	VOID		ReadMaterialData();
	//读取灯光信息
	//##ModelId=47EBC74D033D
	VOID		ReadLightData();
	//读取模型信息
	//##ModelId=47EBC74D034B
	VOID		ReadModelData();
	//读取顶点位置信息
	//##ModelId=47EBC74D035B
	VOID		ReadVertexPosData();
	//读取顶点的法线信息
	//##ModelId=47EBC74D036B
	VOID		ReadVertexNormalData();
	//读取顶点的UV信息
	//##ModelId=47EBC74D038A
	VOID		ReadVertexUVData();
	//读取顶点信息
	//##ModelId=47EBC74D0399
	VOID		ReadVertexData();
	//读取文字信息
	//##ModelId=47EBC74D03A9
	VOID		ReadTextData();
	//读取Overlay信息
	//##ModelId=47EBC74D03B9
	VOID		ReadOverlayData();
	//读取Navigate信息
	//##ModelId=47EBC74D03BA
	VOID		ReadNavigaterData();
	//读取Button信息
	//##ModelId=47EBC74D03BB
	VOID		ReadButtonData();
	//读取贴图信息
	//##ModelId=47EBC74D03C8
	VOID		ReadTexData();
	//读取Image信息
	//##ModelId=47EBC74D03C9
	VOID		ReadImageData();
	//读取CubeMap信息，2.5版本Up
	//##ModelId=47EBC74D03CA
	VOID		ReadCubeMapData();
	//读取MovieTex信息，2.5版本Up
	//##ModelId=47EBC74D03D8
	VOID		ReadMovieTexData();
	//读取BumpWater信息，2.5版本Up
	//##ModelId=47EBC74D03D9
	VOID		ReadBumpWaterData();

	//查找每个模型的引用模型并得到引用模型指针
	//##ModelId=47EBC74D03DA
	VOID		FindReferenceModel();
	//打开对应的Scene场景为E3D文件
	//##ModelId=47EBC74D03DB
	HRESULT		LoadE3DFile(CScene* theScene, CHAR*  strFileName);	
	
	//##ModelId=47EBC74D03DE
	VOID		Release();

//------------------------------------------------------------------------------	
//用于数据传递的变量
//------------------------------------------------------------------------------
private:
	//##ModelId=47EBC74E0000
	FILE *pFile;						//E3D文件数据流

public:
	//##ModelId=47EBC74E0001
	CGetE3DFileInfo(CRITICAL_SECTION* pRenderCS);				//渲染线程管理对象
	//##ModelId=47EBC74E001F
	~CGetE3DFileInfo();

};

#endif
