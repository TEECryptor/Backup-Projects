///////////////////////////////////////////////////////////////////////////
// SaveV3D.h: interface for the CSaveE3D class.
// 类  名: CSaveV3D_3_1
// 描  述: 场景数据保存类，保存场景中的数据为1.0版本的*.V3D文件, 贴图数据做外部路径保存
// 作  者: Lixin
// 日  期: 2007-05-06
// 版  权: 上海创图电脑网络信息技术有限公司
///////////////////////////////////////////////////////////////////////////

#ifndef _SAVEV3D_3_1_H
#define _SAVEV3D_3_1_H

//------------------------------------------------------------------------
#include "CScene.h"
#include "V3D_3_1.h"

//------------------------------------------------------------------------
//##ModelId=47EBC74302BF
class CSaveV3D_3_1  
{
public:
	//##ModelId=47EBC74302C1
	V3D_3_1_Header				Header;					//V3D文件头
	//##ModelId=47EBC74302CF
	V3D_3_1_WindowProp			WindowProp;				//窗体参数
	
	//##ModelId=47EBC74302D4
	V3D_3_1_AudioList			AudioData;				//声音数据的起始位置
	//##ModelId=47EBC74302D9
	V3D_3_1_FlashList			FlashData;				//Flash数据的起始位置

	//##ModelId=47EBC74302DE
	V3D_3_1_CameraList			CameraData;				
	//##ModelId=47EBC74302E3
	V3D_3_1_MaterialList		MaterialData;			
	//##ModelId=47EBC74302E8
	V3D_3_1_LightList			LightData;				//灯光数据

	//##ModelId=47EBC74302ED
	V3D_3_1_VertexList			VertexData;				//顶点数据
	//##ModelId=47EBC74302F2
	V3D_3_1_VertexPosList		VertexPosData;			//只包含顶点的位置
	//##ModelId=47EBC74302F7
	V3D_3_1_VertexNormalList	VertexNormalData;		//只包含顶点的法线
	//##ModelId=47EBC74302FC
	V3D_3_1_UVList				UVData;					//贴图的UV数据，把位置、法线、UV分开保存是因为有很多顶点的位置、法线、UV是相同的，所以只保存一次
	//##ModelId=47EBC7430301
	V3D_3_1_ModelList			ModelData;
	//##ModelId=47EBC7430306
	V3D_3_1_ModelGroup			ModelGroupData;			//模型组列表

	//##ModelId=47EBC743030B
	V3D_3_1_TextList			TextData;				//文字数据
	//##ModelId=47EBC7430310
	V3D_3_1_OverlayList			OverlayData;			//Overlay数据
	//##ModelId=47EBC7430315
	V3D_3_1_NavigaterList		NavigaterData;			//Navigater数据
	//##ModelId=47EBC743031A
	V3D_3_1_ButtonList			ButtonData;				//按钮列表
	//##ModelId=47EBC743031F
	V3D_3_1_CubeMapList			CubeMapData;			//CubeMap列表2007年2月25日新增，WebMax2.5版本
	//##ModelId=47EBC7430324
	V3D_3_1_MovieTexList		MovieTexData;			//电影贴图列表2007年3月17日新增，WebMax2.5版本
	//##ModelId=47EBC7430329
	V3D_3_1_BumpWaterList		BumpWaterData;			//BumpWater贴图列表2007年3月17日新增

	//##ModelId=47EBC743032E
	V3D_3_1_DynamicTexList		DynamicTexData;			//动态贴图数据起始位置

	//##ModelId=47EBC7430333
	V3D_3_1_Privacy				PrivacyData;			//授权数据

	//##ModelId=47EBC7430338
	V3D_3_1_ControlData			ControlData;			//用户控制对象数据

	//##ModelId=47EBC743033D
	V3D_3_1_TextureList			TexData;				//贴图数据
	//##ModelId=47EBC7430342
	V3D_3_1_ImageList			ImageData;				//Image数据

private:
	//##ModelId=47EBC7430347
	CScene*				pScene;					//指向场景数据Scene的指针
	//##ModelId=47EBC743034B
	FILE*				pFile;					//指向V3D文件的指针
	//##ModelId=47EBC743034C
	DWORD				nFileSize;				//V3D文件的大小
	//##ModelId=47EBC743034D
	CHAR				FileName[256];			//V3D文件名

public:

	//写入文件头
	//##ModelId=47EBC743035B
	VOID		WriteHeader();
	//写入窗体属性
	//##ModelId=47EBC743038A
	VOID		WriteWindowProp();
	
	//写入声音数据
	//##ModelId=47EBC74303B9
	VOID		WriteAudioData();
	//写入Flash数据
	//##ModelId=47EBC74303BA
	VOID		WriteFlashData();

	//写入摄像机信息
	//##ModelId=47EBC74303BB
	VOID		WriteCameraData();
	//写入材质信息
	//##ModelId=47EBC7440000
	VOID		WriteMaterialData();
	//写入灯光信息
	//##ModelId=47EBC744001F
	VOID		WriteLightData();

	//写入顶点位置信息
	//##ModelId=47EBC744003E
	VOID		WriteVertexPosData(BOOL zipStatus);
	//写入顶点法线信息
	//##ModelId=47EBC744005D
	VOID		WriteVertexNormalData(BOOL zipStatus);
	//写入贴图坐标信息
	//##ModelId=47EBC744007D
	VOID		WriteUVData(BOOL zipStatus);
	//写入顶点信息
	//##ModelId=47EBC744009C
	VOID		WriteVertexData(BOOL zipStatus);
	//写入模型信息
	//##ModelId=47EBC74400BB
	VOID		WriteModelData();
	//写入模型组信息
	//##ModelId=47EBC74400DA
	VOID		WriteModelGroupData();
	//##ModelId=47EBC74400DB
	VOID		WriteModelGroupData1(CModelGroup* pModelGroup);

	//写入文字信息
	//##ModelId=47EBC74400EB
	VOID		WriteTextData();
	//写入Overlay信息
	//##ModelId=47EBC74400FA
	VOID		WriteOverlayData();
	//写入Navigater信息
	//##ModelId=47EBC7440119
	VOID		WriteNavigaterData();
	//写入Button信息
	//##ModelId=47EBC7440128
	VOID		WriteButtonData();
	
	//写入CubeMap信息
	//##ModelId=47EBC7440138
	VOID		WriteCubeMapData();
	//写入MovieTex信息
	//##ModelId=47EBC7440139
	VOID		WriteMovieTexData();
	//写入BumpWater信息
	//##ModelId=47EBC7440148
	VOID		WriteBumpWaterData();

	//写入动态贴图信息
	//##ModelId=47EBC7440157
	VOID		WriteDynamicTexData();
	//写入文件版本类型和授权标示
	//##ModelId=47EBC7440158
	VOID		WritePrivacyData();

	//写入控制对象数据
	//##ModelId=47EBC7440159
	VOID		WriteControlData();

	//写入贴图信息
	//##ModelId=47EBC744015A
	VOID		WriteTexData();
	//写入外部图像信息
	//##ModelId=47EBC7440177
	VOID		WriteImageData();

	//压缩顶点位置数据，去掉重复数据
	//##ModelId=47EBC7440178
	VOID		ZipVertexPosData(BOOL zipStatus);
	//找到pModel的第index个点在VertexPosData数组中的位置
	//##ModelId=47EBC7440187
	UINT		FindOffsetInVertexPosData(UINT modelIndex, UINT vertexIndex);

	//压缩顶点法线数据，去掉重复数据
	//##ModelId=47EBC74401A5
	VOID		ZipVertexNormalData(BOOL zipStatus);
	//找到pModel的第index个点在VertexNormalData数组中的位置
	//##ModelId=47EBC74401A7
	UINT		FindOffsetInVertexNormalData(UINT modelIndex, UINT vertexIndex);

	//压缩顶点UV数据，去掉重复数据
	//##ModelId=47EBC74401B6
	VOID		ZipUVData(BOOL zipStatus);
	//找到pModel的第index个点的UV在UVData数组中的位置
	//##ModelId=47EBC74401C5
	UINT		FindOffsetInUVData(UINT modelIndex, UINT vertexIndex);

	//压缩模型数据，贴图数据不压缩
	//##ModelId=47EBC74401D5
	HRESULT		ZipModelData();

	//释放资源
	//##ModelId=47EBC74401D6
	VOID		Release();

	//保存对应的Scene场景为E3D文件
	//##ModelId=47EBC74401E4
	HRESULT		SaveSceneAsV3D(CScene* pScene, CHAR* strFileName, BOOL zipStatus);	

	//在保存之前，为对象重新设置编号，防止因为删除对象后，导致保存对象编号错误
	//##ModelId=47EBC74401F5
	VOID		ResetModelIndex();
	//##ModelId=47EBC74401F6
	VOID		ResetTextureIndex();
	//##ModelId=47EBC74401F7
	VOID		ResetBumpWaterIndex();
	//##ModelId=47EBC74401F8
	VOID		ResetCubeMapIndex();
	//##ModelId=47EBC74401F9
	VOID		ResetMovieTexIndex();
	//##ModelId=47EBC7440203
	VOID		ResetMaterialIndex();
	//##ModelId=47EBC7440204
	VOID		ResetOverlayIndex();
	//##ModelId=47EBC7440205
	VOID		ResetDyTexIndex();

	//##ModelId=47EBC7440206
	VOID		ResetCameraIndex();
	//##ModelId=47EBC7440207
	VOID		ResetLightIndex();
	//##ModelId=47EBC7440208
	VOID		ResetNavigaterIndex();
	//##ModelId=47EBC7440209
	VOID		ResetTextIndex();

	//给控制对象重新编号
	//##ModelId=47EBC744020A
	VOID		ResetWalkerIndex();
	//##ModelId=47EBC744020B
	VOID		ResetEditorIndex();
	//##ModelId=47EBC744020C
	VOID		ResetFlyerIndex();

	//##ModelId=47EBC744020D
	VOID		ResetButtonIndex();
	//##ModelId=47EBC744020E
	VOID		ResetImageIndex();

public:
	//##ModelId=47EBC7440213
	CSaveV3D_3_1();
	//##ModelId=47EBC7440242
	~CSaveV3D_3_1();

};


//------------------------------------------------------------------------
#endif
