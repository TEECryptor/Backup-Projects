/////////////////////////////////////////////////////////////////////////
// 名称: CGetV3DFileInfo--V3D文件老版本读取类
// 描述: 读取V3D文件，并规整场景数据
// 作者: lixin	
// 日期: 2007-04-09
////////////////////////////////////////////////////////////////////////

#ifndef _CGETV3DFILEINFO_H
#define _CGETV3DFILEINFO_H

#include "V3D.h"
#include "CScene.h"

//-----------------------------------------------------------------------------
//类  名: CGetE3DFileInfo
//用  途: 用于打开E3D文件，并提取场景数据
//-----------------------------------------------------------------------------
//##ModelId=47EBC74B0280
class CGetV3DFileInfo  
{

public:
	//##ModelId=47EBC74B0282
	V3D_V3DHeader			Header;				//E3D文件头
	//##ModelId=47EBC74B0287
	V3D_CameraList			CameraData;			//摄像机列表
	//##ModelId=47EBC74B028C
	V3D_MaterialList		MaterialData;		//材质列表
	//##ModelId=47EBC74B0291
	V3D_LightList			LightData;			//灯光列表
	//##ModelId=47EBC74B0296
	V3D_ModelList			ModelData;			//Model列表，模型列表

	//##ModelId=47EBC74B029B
	V3D_VertexList			VertexData;			//顶点列表
	//##ModelId=47EBC74B02A0
	V3D_VertexPosList		VertexPosData;		//压缩后的顶点位置列表
	//##ModelId=47EBC74B02A5
	V3D_VertexNormalList	VertexNormalData;   //压缩后的定点法线列表
	//##ModelId=47EBC74B02AA
	V3D_UVList				UVData;				//顶点UV数据


	//##ModelId=47EBC74B02AF
	V3D_TextList			TextData;			//文字列表
	//##ModelId=47EBC74B02B4
	V3D_OverlayList			OverlayData;		//Overlay列表
	//##ModelId=47EBC74B02B9
	V3D_NavigaterList		NavigaterData;		//Navigater列表
	//##ModelId=47EBC74B02BE
	V3D_ButtonList			ButtonData;			//按钮列表

	//##ModelId=47EBC74B02C3
	V3D_CubeMapList			CubeMapData;		//CubeMap列表，2.5Version Up
	//##ModelId=47EBC74B02C8
	V3D_MovieTexList		MovieTexData;		//电影贴图列表
	//##ModelId=47EBC74B02CD
	V3D_BumpWaterList		BumpWaterData;		//水面铁图列表

	//##ModelId=47EBC74B02D2
	V3D_TextureList			TexData;			//贴图列表
	//##ModelId=47EBC74B02D7
	V3D_ImageList			ImageData;			//图像列表

	//##ModelId=47EBC74B02DB
	CHAR					m_strFileName[256];	//文件名
	//##ModelId=47EBC74B02DD
	CScene*					pScene;				//场景
	
	//##ModelId=47EBC74B02E1
	CRITICAL_SECTION*		m_pRenderCS;		//渲染线程管理对象

	//读取文件头
	//##ModelId=47EBC74B02E2
	VOID		ReadHeader();
	//读取摄像机信息
	//##ModelId=47EBC74B02FD
	VOID		ReadCameraData();
	//读取材质信息
	//##ModelId=47EBC74B030D
	VOID		ReadMaterialData();
	//读取灯光信息
	//##ModelId=47EBC74B030E
	VOID		ReadLightData();
	//读取模型信息
	//##ModelId=47EBC74B031C
	VOID		ReadModelData();
	//读取顶点位置信息
	//##ModelId=47EBC74B032C
	VOID		ReadVertexPosData();
	//读取顶点的法线信息
	//##ModelId=47EBC74B034B
	VOID		ReadVertexNormalData();
	//读取顶点的UV信息
	//##ModelId=47EBC74B036B
	VOID		ReadVertexUVData();
	//读取顶点信息
	//##ModelId=47EBC74B037A
	VOID		ReadVertexData();
	//读取文字信息
	//##ModelId=47EBC74B0399
	VOID		ReadTextData();
	//读取Overlay信息
	//##ModelId=47EBC74B039A
	VOID		ReadOverlayData();
	//读取Navigate信息
	//##ModelId=47EBC74B03A9
	VOID		ReadNavigaterData();
	//读取Button信息
	//##ModelId=47EBC74B03AA
	VOID		ReadButtonData();
	//读取贴图信息
	//##ModelId=47EBC74B03AB
	VOID		ReadTexData();
	//读取Image信息
	//##ModelId=47EBC74B03B9
	VOID		ReadImageData();
	//读取CubeMap信息，2.5版本Up
	//##ModelId=47EBC74B03BA
	VOID		ReadCubeMapData();
	//读取MovieTex信息，2.5版本Up
	//##ModelId=47EBC74B03BB
	VOID		ReadMovieTexData();
	//读取BumpWater信息，2.5版本Up
	//##ModelId=47EBC74B03C8
	VOID		ReadBumpWaterData();

	//查找每个模型的引用模型并得到引用模型指针
	//##ModelId=47EBC74B03C9
	VOID		FindReferenceModel();
	//打开对应的Scene场景为E3D文件
	//##ModelId=47EBC74B03CA
	HRESULT		LoadV3DFile(CScene* theScene, CHAR*  strFileName);	
	
	//##ModelId=47EBC74B03CD
	VOID		Release();

//------------------------------------------------------------------------------	
//用于数据传递的变量
//------------------------------------------------------------------------------
private:
	//##ModelId=47EBC74B03CE
	FILE		*pFile;						//E3D文件数据流

public:
	//##ModelId=47EBC74B03D8
	CGetV3DFileInfo(CRITICAL_SECTION* pRenderCS);				//渲染线程管理对象
	//##ModelId=47EBC74C000F
	~CGetV3DFileInfo();

};

#endif
