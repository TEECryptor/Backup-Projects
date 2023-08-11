///////////////////////////////////////////////////////////////////////////
// SaveV3D.h: interface for the CSaveE3D class.
// 类  名: CSaveV3D
// 描  述: 场景数据保存类，保存场景中的数据为老版本*.V3D文件, 贴图数据做外部路径保存
// 作  者: Lixin
// 日  期: 2007-04-09
// 版  权: 上海创图电脑网络信息技术有限公司
///////////////////////////////////////////////////////////////////////////

#ifndef _SAVEV3D_H
#define _SAVEV3D_H

//------------------------------------------------------------------------
#include "CScene.h"
#include "V3D_def.h"

//------------------------------------------------------------------------
//##ModelId=47EBC74402CE
class CSaveV3D  
{
public:
	//##ModelId=47EBC74402DE
	CSaveV3D();
	//##ModelId=47EBC74402EE
	~CSaveV3D();

private:
	//##ModelId=47EBC744030E
	CScene*				pScene;					//指向场景数据Scene的指针
	//##ModelId=47EBC7440312
	FILE*				pFile;					//指向V3D文件的指针
	//##ModelId=47EBC7440313
	DWORD				nFileSize;				//V3D文件的大小
	//##ModelId=47EBC7440314
	CHAR				FileName[256];			//V3D文件名

	//##ModelId=47EBC744031D
	V3D_V3DHeader			Header;					//V3D文件头
	//##ModelId=47EBC7440322
	V3D_CameraList			CameraData;				
	//##ModelId=47EBC7440327
	V3D_MaterialList		MaterialData;			
	//##ModelId=47EBC744032C
	V3D_LightList			LightData;				//灯光数据
	//##ModelId=47EBC7440331
	V3D_ModelList			ModelData;
	//##ModelId=47EBC7440336
	V3D_VertexList			VertexData;				//顶点数据
	//##ModelId=47EBC744033B
	V3D_VertexPosList		VertexPosData;			//只包含顶点的位置
	//##ModelId=47EBC7440340
	V3D_VertexNormalList	VertexNormalData;		//只包含顶点的法线
	//##ModelId=47EBC7440345
	V3D_UVList				UVData;					//贴图的UV数据，把位置、法线、UV分开保存是因为有很多顶点的位置、法线、UV是相同的，所以只保存一次
	//##ModelId=47EBC744034A
	V3D_TextList			TextData;				//文字数据
	//##ModelId=47EBC744034F
	V3D_OverlayList			OverlayData;			//Overlay数据
	//##ModelId=47EBC7440354
	V3D_NavigaterList		NavigaterData;			//Navigater数据
	//##ModelId=47EBC7440359
	V3D_ButtonList			ButtonData;				//按钮列表
	//##ModelId=47EBC744035E
	V3D_CubeMapList			CubeMapData;			//CubeMap列表2007年2月25日新增，WebMax2.5版本
	//##ModelId=47EBC7440363
	V3D_MovieTexList		MovieTexData;			//电影贴图列表2007年3月17日新增，WebMax2.5版本
	//##ModelId=47EBC7440368
	V3D_BumpWaterList		BumpWaterData;			//BumpWater贴图列表2007年3月17日新增

	//##ModelId=47EBC744036D
	V3D_TextureList			TexData;				//贴图数据
	//##ModelId=47EBC7440372
	V3D_ImageList			ImageData;				//Image数据


public:

	//写入文件头
	//##ModelId=47EBC7440376
	VOID		WriteHeader();
	//写入摄像机信息
	//##ModelId=47EBC744038A
	VOID		WriteCameraData();
	//写入材质信息
	//##ModelId=47EBC7440399
	VOID		WriteMaterialData();
	//写入灯光信息
	//##ModelId=47EBC74403B9
	VOID		WriteLightData();
	//写入模型信息
	//##ModelId=47EBC74403C8
	VOID		WriteModelData();
	//写入顶点位置信息
	//##ModelId=47EBC74403D8
	VOID		WriteVertexPosData(BOOL zipStatus);
	//写入顶点法线信息
	//##ModelId=47EBC7450000
	VOID		WriteVertexNormalData(BOOL zipStatus);
	//写入贴图坐标信息
	//##ModelId=47EBC745000F
	VOID		WriteUVData(BOOL zipStatus);
	//写入顶点信息
	//##ModelId=47EBC745001F
	VOID		WriteVertexData(BOOL zipStatus);
	//写入文字信息
	//##ModelId=47EBC745002E
	VOID		WriteTextData();
	//写入Overlay信息
	//##ModelId=47EBC745003E
	VOID		WriteOverlayData();
	//写入Navigater信息
	//##ModelId=47EBC745003F
	VOID		WriteNavigaterData();
	//写入Button信息
	//##ModelId=47EBC745004E
	VOID		WriteButtonData();
	
	//写入CubeMap信息
	//##ModelId=47EBC745004F
	VOID		WriteCubeMapData();
	//写入MovieTex信息
	//##ModelId=47EBC7450050
	VOID		WriteMovieTexData();
	//写入BumpWater信息
	//##ModelId=47EBC745005D
	VOID		WriteBumpWaterData();

	//写入贴图信息
	//##ModelId=47EBC745005E
	VOID		WriteTexData();
	//写入外部图像信息
	//##ModelId=47EBC745006D
	VOID		WriteImageData();

	//压缩顶点位置数据，去掉重复数据
	//##ModelId=47EBC745006E
	VOID		ZipVertexPosData(BOOL zipStatus);
	//找到pModel的第index个点在VertexPosData数组中的位置
	//##ModelId=47EBC7450070
	UINT		FindOffsetInVertexPosData(UINT modelIndex, UINT vertexIndex);

	//压缩顶点法线数据，去掉重复数据
	//##ModelId=47EBC745007F
	VOID		ZipVertexNormalData(BOOL zipStatus);
	//找到pModel的第index个点在VertexNormalData数组中的位置
	//##ModelId=47EBC745008C
	UINT		FindOffsetInVertexNormalData(UINT modelIndex, UINT vertexIndex);

	//压缩顶点UV数据，去掉重复数据
	//##ModelId=47EBC745008F
	VOID		ZipUVData(BOOL zipStatus);
	//找到pModel的第index个点的UV在UVData数组中的位置
	//##ModelId=47EBC745009C
	UINT		FindOffsetInUVData(UINT modelIndex, UINT vertexIndex);

	//压缩模型数据，贴图数据不压缩
	//##ModelId=47EBC745009F
	HRESULT		ZipModelData();

	//释放资源
	//##ModelId=47EBC74500A0
	VOID		Release();

	//保存对应的Scene场景为E3D文件
	//##ModelId=47EBC74500A1
	HRESULT		SaveSceneAsV3D(CScene* pScene, CHAR* strFileName, BOOL zipStatus);	

	//在保存之前，为对象重新设置编号，防止因为删除对象后，导致保存对象编号错误
	//##ModelId=47EBC74500AE
	VOID		ResetModelIndex();
	//##ModelId=47EBC74500AF
	VOID		ResetTextureIndex();
	//##ModelId=47EBC74500BB
	VOID		ResetBumpWaterIndex();
	//##ModelId=47EBC74500BC
	VOID		ResetCubeMapIndex();
	//##ModelId=47EBC74500BD
	VOID		ResetMovieTexIndex();
	//##ModelId=47EBC74500BE
	VOID		ResetMaterialIndex();
	//##ModelId=47EBC74500BF
	VOID		ResetOverlayIndex();

	//##ModelId=47EBC74500C0
	VOID		ResetCameraIndex();
	//##ModelId=47EBC74500C1
	VOID		ResetLightIndex();
	//##ModelId=47EBC74500C2
	VOID		ResetNavigaterIndex();
	//##ModelId=47EBC74500C3
	VOID		ResetTextIndex();

	//##ModelId=47EBC74500C4
	VOID		ResetButtonIndex();
	//##ModelId=47EBC74500C5
	VOID		ResetImageIndex();


};


//------------------------------------------------------------------------
#endif
