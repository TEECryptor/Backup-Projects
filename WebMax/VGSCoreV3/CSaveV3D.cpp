///////////////////////////////////////////////////////////////////////////
// SaveV3D.cpp: interface for the CSaveE3D class.
// 类  名: CSaveV3D
// 描  述: 场景数据保存类，保存场景中的数据为*.V3D文件, 贴图数据做外部路径保存
// 作  者: Lixin
// 日  期: 2007-04-09
// 版  权: 上海创图电脑网络信息技术有限公司
///////////////////////////////////////////////////////////////////////////

#include "CSaveV3D.h"
#include "CMovieTexture.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74402DE
CSaveV3D::CSaveV3D()
{
	pScene = NULL;				//指向场景数据Scene的指针
	pFile = NULL;				//指向E3D文件的指针
	nFileSize = 0;				//E3D文件的大小
}

//##ModelId=47EBC74402EE
CSaveV3D::~CSaveV3D()
{

}

//-------------------------------------------------------------------
//方法: WriteHeader()
//参数: 
//用途: 写入文件头
//-------------------------------------------------------------------
//##ModelId=47EBC7440376
VOID CSaveV3D::WriteHeader()
{
}

//-------------------------------------------------------------------
//方法: WriteCameraData()
//参数: 
//用途: 写入摄像机信息
//-------------------------------------------------------------------
//##ModelId=47EBC744038A
VOID CSaveV3D::WriteCameraData()
{
	
}

//-------------------------------------------------------------------
//方法: WriteCameraData()
//参数: 
//用途: 写入材质信息
//-------------------------------------------------------------------
//##ModelId=47EBC7440399
VOID CSaveV3D::WriteMaterialData()
{

}

//-------------------------------------------------------------------
//方法: WriteLightData()
//参数: 
//用途: 写入灯光信息
//-------------------------------------------------------------------
//##ModelId=47EBC74403B9
VOID CSaveV3D::WriteLightData()
{}

//-------------------------------------------------------------------
//方法: WriteModelData()
//参数: 
//用途: 写入模型信息
//-------------------------------------------------------------------
//##ModelId=47EBC74403C8
VOID CSaveV3D::WriteModelData()
{
}

//-------------------------------------------------------------------
//方法: WriteVertexPosData()
//参数: 
//用途: 写入顶点位置信息
//-------------------------------------------------------------------
//##ModelId=47EBC74403D8
VOID CSaveV3D::WriteVertexPosData(BOOL zipStatus)
{}


//-------------------------------------------------------------------
//方法: WriteVertexNormalData()
//参数: 
//用途: 写入顶点法线信息
//-------------------------------------------------------------------
//##ModelId=47EBC7450000
VOID CSaveV3D::WriteVertexNormalData(BOOL zipStatus)
{
}

//-------------------------------------------------------------------
//方法: WriteVetexData()
//参数: 
//用途: 写入顶点信息
//-------------------------------------------------------------------
//##ModelId=47EBC745001F
VOID CSaveV3D::WriteVertexData(BOOL zipStatus)
{
}

//-------------------------------------------------------------------
//方法: WriteUVData()
//参数: 
//用途: 写入贴图坐标信息
//-------------------------------------------------------------------
//##ModelId=47EBC745000F
VOID CSaveV3D::WriteUVData(BOOL zipStatus)
{
}

//-------------------------------------------------------------------
//方法: WriteTexData()
//参数: 
//用途: 写入贴图信息
//-------------------------------------------------------------------
//##ModelId=47EBC745005E
VOID CSaveV3D::WriteTexData()
{

}

//-------------------------------------------------------------------
//方法: WriteTextData()
//参数: 
//用途: 写入文字信息
//-------------------------------------------------------------------
//##ModelId=47EBC745002E
VOID CSaveV3D::WriteTextData()
{
	
}

//-------------------------------------------------------------------
//方法: WriteOverlayData()
//参数: 
//用途: 写入Overlay信息
//-------------------------------------------------------------------
//##ModelId=47EBC745003E
VOID CSaveV3D::WriteOverlayData()
{
}

//-------------------------------------------------------------------
//方法: WriteNavigaterData
//参数: 
//用途: 写入Navigater信息
//-------------------------------------------------------------------
//##ModelId=47EBC745003F
VOID CSaveV3D::WriteNavigaterData()
{
}

//-------------------------------------------------------------------
//方法: WriteButtonData()
//参数: 
//用途: write the button data
//-------------------------------------------------------------------
//##ModelId=47EBC745004E
VOID CSaveV3D::WriteButtonData()
{
}

//-------------------------------------------------------------------
//方法: WriteImageData
//参数: 
//用途: 写入外部图像信息
//-------------------------------------------------------------------
//##ModelId=47EBC745006D
VOID CSaveV3D::WriteImageData()
{
}


//-------------------------------------------------------------------
//方法: WriteCubeMapData()
//参数: 
//用途: 写入CubeMap 数据
//-------------------------------------------------------------------
//##ModelId=47EBC745004F
VOID CSaveV3D::WriteCubeMapData()
{
}

//写入MovieTex信息
//##ModelId=47EBC7450050
VOID CSaveV3D::WriteMovieTexData()
{	
}

//写入BumpWater信息
//##ModelId=47EBC745005D
VOID CSaveV3D::WriteBumpWaterData()
{
}

//-------------------------------------------------------------------
//方法: ZipVertexPosData()
//参数: 
//用途: 规整顶点位置信息，去掉重复数据
//-------------------------------------------------------------------
//##ModelId=47EBC745006E
VOID CSaveV3D::ZipVertexPosData(BOOL zipStatus)
{
}

//-------------------------------------------------------------------
//方法: FindOffsetInVertexPosData()
//参数: UINT modelIndex, UINT vertexIndex
//用途: 找到pModel的第index个点在VertexPosData数组中的位置
//-------------------------------------------------------------------
//##ModelId=47EBC7450070
UINT CSaveV3D::FindOffsetInVertexPosData(UINT modelIndex, UINT vertexIndex)
{
	return 0;

}

//-------------------------------------------------------------------
//方法: ZipVertexNormalData()
//参数: 
//用途: 压缩顶点法线数据，去掉重复数据
//-------------------------------------------------------------------
//##ModelId=47EBC745007F
VOID CSaveV3D::ZipVertexNormalData(BOOL zipStatus)
{
}

//-------------------------------------------------------------------
//方法: FindOffsetInVertexNormalData()
//参数: 
//用途: 找到pModel的第index个点在VertexNormalData数组中的位置
//-------------------------------------------------------------------
//##ModelId=47EBC745008C
UINT CSaveV3D::FindOffsetInVertexNormalData(UINT modelIndex, UINT vertexIndex)
{
	return 0;
}

//-------------------------------------------------------------------
//方法: ZipUVData()
//参数: 
//用途: 压缩UV数据，去掉重复数据
//-------------------------------------------------------------------
//##ModelId=47EBC745008F
VOID CSaveV3D::ZipUVData(BOOL zipStatus)
{
}

//-------------------------------------------------------------------
//方法: FindOffsetInVertexPosData()
//参数: UINT modelIndex, UINT vertexIndex
//用途: 找到pModel的第index个点的UV在UVData数组中的位置
//-------------------------------------------------------------------
//##ModelId=47EBC745009C
UINT CSaveV3D::FindOffsetInUVData(UINT modelIndex, UINT vertexIndex)
{
	return 0;
}

//-------------------------------------------------------------------
//方法: ZipModelData()
//参数: 
//用途: 压缩模型数据，贴图数据不压缩
//-------------------------------------------------------------------
//##ModelId=47EBC745009F
HRESULT CSaveV3D::ZipModelData()
{
	return S_OK;
}

//-------------------------------------------------------------------
//方法: Release()
//参数: 
//用途: 释放资源
//-------------------------------------------------------------------
//##ModelId=47EBC74500A0
VOID CSaveV3D::Release()
{
	
}

//-------------------------------------------------------------------
//方法: SaveSceneAsV3D()
//参数: CScene* pScene, CHAR*  strFileName
//用途: 保存对应的Scene场景为V3D文件
//-------------------------------------------------------------------
//##ModelId=47EBC74500A1
HRESULT CSaveV3D::SaveSceneAsV3D(CScene* theScene, CHAR*  strFileName, BOOL zipStatus)
{

	return S_OK;

}

//给模型队列重新编号
//##ModelId=47EBC74500AE
VOID CSaveV3D::ResetModelIndex()
{
}

//重新给贴图队列编号
//##ModelId=47EBC74500AF
VOID CSaveV3D::ResetTextureIndex()
{
}


//给BumpWater重新编号
//##ModelId=47EBC74500BB
VOID CSaveV3D::ResetBumpWaterIndex()
{	
}


//给CubeMap序列中的元素重新编号
//##ModelId=47EBC74500BC
VOID CSaveV3D::ResetCubeMapIndex()
{}

//给MovieTex重新编号
//##ModelId=47EBC74500BD
VOID CSaveV3D::ResetMovieTexIndex()
{
}

//给材质队列重新编号
//##ModelId=47EBC74500BE
VOID CSaveV3D::ResetMaterialIndex()
{
	CMaterial** ppMtrl = (CMaterial**)pScene->m_vShader.begin();
	INT mtrlCnt = pScene->m_vShader.size();
	for (INT i = 0; i< mtrlCnt; i++)
	{
		CMaterial* pMtrl = *(ppMtrl + i);
		pMtrl->index = i;
	}
}

//为Overlay重新编号
//##ModelId=47EBC74500BF
VOID CSaveV3D::ResetOverlayIndex()
{
}

//为相机队列编号
//##ModelId=47EBC74500C0
VOID CSaveV3D::ResetCameraIndex()
{
}

//为灯光队列编号
//##ModelId=47EBC74500C1
VOID CSaveV3D::ResetLightIndex()
{
}

//为导航图队列重新编号
//##ModelId=47EBC74500C2
VOID CSaveV3D::ResetNavigaterIndex()
{

}

//为文字队列重新编号
//##ModelId=47EBC74500C3
VOID CSaveV3D::ResetTextIndex()
{
}

//给Button队列重新编号
//##ModelId=47EBC74500C4
VOID CSaveV3D::ResetButtonIndex()
{
}

//给Image队列重新编号
//##ModelId=47EBC74500C5
VOID CSaveV3D::ResetImageIndex()
{
}
