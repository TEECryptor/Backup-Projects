/////////////////////////////////////////////////////////////////////////
// 名称: GetS3DFileInfo--S3D文件读取类
// 描述: 读取S3D文件，并规整场景数据
// 作者: lixin	
// 日期: 2005-12-23
////////////////////////////////////////////////////////////////////////

#ifndef _CGETS3DFILEINFO_H
#define _CGETS3DFILEINFO_H

#include "S3D.h"
#include "CScene.h"

//-----------------------------------------------------------------------------
//类名：GetS3DFileInfo
//用途：用于打开S3D文件，并提取场景数据
//-----------------------------------------------------------------------------
//##ModelId=47EBC74C007D
class CGetS3DFileInfo
{
//------------------------------------------------------------------------------
//用于保存场景数据的变量
//------------------------------------------------------------------------------
private:
	//##ModelId=47EBC74C008D
	S3DHEADER Header; //文件头
	//##ModelId=47EBC74C0092
	SCENETABLE SceneTable; //场景信息列表

	//##ModelId=47EBC74C0097
	GCS SceneGCS; //场景全局坐标系

	//##ModelId=47EBC74C009C
	NAME* TexNameList; //贴图文件名称列表 (from 1 to Texture Count), use 32 byte per TexName
	//##ModelId=47EBC74C00A0
	UINT TextureCnt; //贴图总数量
	//##ModelId=47EBC74C00A1
	UINT TexNameListOffset; //贴图文件名列表的起始位置，相对文件开头的偏移量

	//##ModelId=47EBC74C00A3
	MATERIAL *MaterialInfoList;//材质数据列表
	//##ModelId=47EBC74C00A7
	UINT MaterialCnt; //材质数量
	//##ModelId=47EBC74C00A8
	UINT MaterialInfoListOffset; //材质数据列表的起始位置，相对文件的开头的偏移量

	//##ModelId=47EBC74C00AC
	ORIFACE *FaceInfoList; //面的数据列表
	//##ModelId=47EBC74C00B1
	FACE *FaceInfoList1; //FaceInfoList解开后的详细数据
	//##ModelId=47EBC74C00B5
	UINT FaceCnt; //面数量
	//##ModelId=47EBC74C00B6
	UINT FaceInfoListOffset; //面数据列表的起始位置，相对文件的开头的偏移量

	//##ModelId=47EBC74C00BC
	VERTEX * VertexInfoList; //顶点的数据列表
	//##ModelId=47EBC74C00C0
	UINT VertexCnt; //顶点数量
	//##ModelId=47EBC74C00C1
	UINT VertexInfoListOffset; //顶点数据列表的起始位置，相对文件的开头的偏移量

	//##ModelId=47EBC74C00C3
	DCAMINFO * CameraInfoList;//摄像机的详细数据列表,如果当前摄像机没有动画，则没有这项
	//##ModelId=47EBC74C00CC
	CAMERA *CameraList;//摄影机的概要数据列表，包含指向对应详细数据的指针
	//##ModelId=47EBC74C00D0
	UINT CameraCnt;//摄像机数量
	//##ModelId=47EBC74C00D1
	UINT CameraListOffset;//摄像机概要数据列表的起始位置，相对文件开头的偏移量

	//##ModelId=47EBC74C00D3
	UNIT * UnitInfoList; //模型数据列表
	//##ModelId=47EBC74C00DA
	UINT UnitCnt; //模型数量，没有关键帧动画时
	//##ModelId=47EBC74C00DB
	UINT UnitInfoListOffset; //模型信息偏移量，没有关键帧动画时

	//##ModelId=47EBC74C00DD
	DUNITINFO *DUnitInfoList;//模型变换矩阵，有关键帧动画时
	//##ModelId=47EBC74C00E2
	DVTXUNITINFO *VTXDUnitInfoList;//动态顶点数据列表，以模型为单位，当有顶点动画时
	//##ModelId=47EBC74C00EB
	DUNIT * DUnitList; //关键帧动画模型信息，有关键帧动画时

	//##ModelId=47EBC74C00EF
	UINT KeyFrameCnt;//场景输出的关键帧数量，这里是关键帧数据读取的入口
	//##ModelId=47EBC74C00F0
	UINT DUnitCnt; //场景中输出的模型数量,当有关键帧动画时
	//##ModelId=47EBC74C00FA
	UINT DUnitListOffset;//关键帧动画模型数据的起始位置，相对文件开头的偏移量，即DMODEL * DModelInfo处;

	//##ModelId=47EBC74C00FC
	LIGHTFRAME* LightInfoList;//场景中所有灯光详细信息数据列表
	//##ModelId=47EBC74C0101
	LIGHT*	LightList;		//场景中的灯光概要信息数据列表
	//##ModelId=47EBC74C0109
	UINT LightCnt; //灯光数量
	//##ModelId=47EBC74C010A
	UINT LightListOffset; //灯光概要信息列表的起始位置，相对文件开头的偏移量

//------------------------------------------------------------------------------	
//用于数据传递的变量
//------------------------------------------------------------------------------
private:
	//文件有关的成员变量---------------------------------------------------------------------------
	//##ModelId=47EBC74C010B
	CHAR					m_StrS3DFilePath[256];			//当前S3D文件的路径
	//##ModelId=47EBC74C0119
	CHAR					m_FileName[256];				//文件名
	//##ModelId=47EBC74C011A
	FILE*					S3DFile;						//S3D文件数据流
	//##ModelId=47EBC74C011B
	FILE*					DebugFile;						//用于调试的数据流
	//##ModelId=47EBC74C0129
	CScene*					pScene;							//场景
	
	//读文件头
	//##ModelId=47EBC74C012D
	VOID ReadFileHeader();
	//读场景信息表
	//##ModelId=47EBC74C0148
	VOID ReadSceneTable();
	//读场景坐标系
	//##ModelId=47EBC74C0167
	VOID ReadSceneGCS();

	//读贴图信息
	//##ModelId=47EBC74C0186
	VOID ReadTextureInfo();
	//读材质信息
	//##ModelId=47EBC74C0196
	VOID ReadMaterialInfo();

	//读模型信息
	//##ModelId=47EBC74C01B5
	VOID ReadUnitInfo();
	//读面信息
	//##ModelId=47EBC74C01C5
	VOID ReadFaceInfo();
	//读顶点信息
	//##ModelId=47EBC74C01D4
	VOID ReadVerticesInfo();
	//读模型关键帧动画信息
	//##ModelId=47EBC74C01F4
	VOID ReadUnitKeyFrameInfo();

	//读取摄像机信息
	//##ModelId=47EBC74C0203
	VOID ReadCameraInfo();
	//读取灯光信息
	//##ModelId=47EBC74C0213
	VOID ReadLightInfo();

	//得到面和顶点的法线，顶点的法线为公用该顶点的面的法线平均值
	//##ModelId=47EBC74C0222
	VOID CalculateNormal(CModel* pModel, CFace* pFace);

	//从2个DWORD类型数据中提取三个INT类型数据,之前这样保存的原因是为了节省空间
	//##ModelId=47EBC74C0232
	inline VOID DetachData_1(DWORD ab, DWORD bc, DWORD *a, DWORD *b, DWORD *c)
	{
		//具体算法参考公司位运算文档
		*a = (ab >> 10) & 0x003fffff;
		*b = ((ab << 11) & 0x001ff800) + ((bc >> 21) & 0x000007ff); //是+号，不是&
		*c = bc & 0x001fffff;
	}

	//--------------------------------------------------------------------------
	//从S3DFile对象读取数据，并把数据规整到Scene中
	//--------------------------------------------------------------------------
	//读入贴图数据
	//##ModelId=47EBC74C0243
	VOID ReadTextureData();
	//读入材质数据
	//##ModelId=47EBC74C0244
	VOID ReadMtrlData();
	//读入摄像机数据
	//##ModelId=47EBC74C0251
	VOID ReadCameraData();
	//读入模型数据
	//##ModelId=47EBC74C0252
	VOID ReadModelData();
	//得到灯光数据
	//##ModelId=47EBC74C0253
	VOID ReadLightData();
	//得到模型列表中的模型编号对应的动态模型列表中的编号
	//##ModelId=47EBC74C0261
	INT  GetDUnitIndex(UINT unitIndex);


//------------------------------------------------------------------------------
//用于调用私有数据的方法
//------------------------------------------------------------------------------
public:
	//打开S3D文件到场景
	//##ModelId=47EBC74C0263
	HRESULT LoadS3DFile(CScene* pScene, CHAR* strFileName, BOOL bImportModel, BOOL bImportLight, BOOL bImportCamera);
	//打开S3D文件
	//##ModelId=47EBC74C0280
	HRESULT	LoadS3DFile(CHAR* strFileName);
	//用于贴图到库的拷贝
	//##ModelId=47EBC74C0290
	UINT	GetTextureCount();
	//##ModelId=47EBC74C0291
	CHAR*	GetTextureName(UINT index);
	//释放S3D对象，回收内存
	//##ModelId=47EBC74C0293
	VOID Release();


public:
	//##ModelId=47EBC74C0294
	CGetS3DFileInfo();
	//##ModelId=47EBC74C02AF
	~CGetS3DFileInfo();

};
#endif
