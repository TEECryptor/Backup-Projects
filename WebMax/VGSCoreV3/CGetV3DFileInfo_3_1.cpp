/////////////////////////////////////////////////////////////////////////
// 名称: CGetV3DFileInfo_3_1 -- V3D 1.0版本文件读取类
// 描述: 读取V3D文件，并规整场景数据
// 作者: lixin	
// 日期: 2007-05-09
////////////////////////////////////////////////////////////////////////

#include "CGetV3DFileInfo_3_1.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74B01B7
CGetV3DFileInfo_3_1::CGetV3DFileInfo_3_1(CRITICAL_SECTION*	pRenderCS)
{
}

//##ModelId=47EBC74B01F4
CGetV3DFileInfo_3_1::~CGetV3DFileInfo_3_1()
{

}


//-------------------------------------------------------------------
//方法: ReadHeader()
//参数: 
//用途: 读取文件头
//-------------------------------------------------------------------
//##ModelId=47EBC74A02C1
VOID CGetV3DFileInfo_3_1::ReadHeader()
{
}
	
//读取窗体信息
//##ModelId=47EBC74A02FD
VOID CGetV3DFileInfo_3_1::ReadWindowProp()
{
}

//读入授权信息
//##ModelId=47EBC74B0138
VOID CGetV3DFileInfo_3_1::ReadPrivacyData()
{
}

//读取控制信息
//##ModelId=47EBC74B0177
VOID  CGetV3DFileInfo_3_1::ReadContorlData()
{
}

//读取声音数据
//##ModelId=47EBC74A032C
VOID  CGetV3DFileInfo_3_1::ReadAudioData()
{
}

//读取Flash数据
//##ModelId=47EBC74A035B
VOID  CGetV3DFileInfo_3_1::ReadFlashData()
{
}

//读取模型组列表信息
//##ModelId=47EBC74B00AB
VOID  CGetV3DFileInfo_3_1::ReadModelGroupData1(CModelGroup* pModelGroup)
{

}

//读取模型组列表信息
//##ModelId=47EBC74B007D
VOID  CGetV3DFileInfo_3_1::ReadModelGroupData()
{
}

//读取动态贴图信息
//##ModelId=47EBC74B0119
VOID  CGetV3DFileInfo_3_1::ReadDynamicTexData()
{
}

//-------------------------------------------------------------------
//方法: ReadVertexPosData()
//参数: 
//用途: 读取顶点位置信息
//-------------------------------------------------------------------
//##ModelId=47EBC74A03B9
VOID CGetV3DFileInfo_3_1::ReadVertexPosData()
{}

//-------------------------------------------------------------------
//方法: ReadVertexNormalData()
//参数: 
//用途: 读取顶点的法线信息
//-------------------------------------------------------------------
//##ModelId=47EBC74A03D8
VOID CGetV3DFileInfo_3_1::ReadVertexNormalData()
{}

//-------------------------------------------------------------------
//方法: ReadVertexUVData()
//参数: 
//用途: 读取顶点的UV信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B001F
VOID CGetV3DFileInfo_3_1::ReadVertexUVData()
{
}

//-------------------------------------------------------------------
//方法: ReadVetexData()
//参数: 
//用途: 读取顶点信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B003E
VOID CGetV3DFileInfo_3_1::ReadVertexData()
{
}

//-------------------------------------------------------------------
//方法: ReadModelData()
//参数: 
//用途: 读取模型信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B005D
VOID CGetV3DFileInfo_3_1::ReadModelData()
{
	
}

//-------------------------------------------------------------------
//方法: ReadCameraData()
//参数: 
//用途: 读取摄像机信息
//-------------------------------------------------------------------
//##ModelId=47EBC74A038A
VOID CGetV3DFileInfo_3_1::ReadCameraData()
{
	
}

//-------------------------------------------------------------------
//方法: ReadTexData()
//参数: 
//用途: 读取贴图信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B00FA
VOID CGetV3DFileInfo_3_1::ReadTexData()
{
}

//-------------------------------------------------------------------
//方法: ReadMaterialData()
//参数: 
//用途: 读取材质信息
//-------------------------------------------------------------------
//##ModelId=47EBC74A0399
VOID CGetV3DFileInfo_3_1::ReadMaterialData()
{
}

//-------------------------------------------------------------------
//方法: ReadLightData()
//参数: 
//用途: 读入灯光信息
//-------------------------------------------------------------------
//##ModelId=47EBC74A03A9
VOID CGetV3DFileInfo_3_1::ReadLightData()
{
}

//-------------------------------------------------------------------
//方法: ReadTextData()
//参数: 
//用途: 读取文字信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B00DA
VOID CGetV3DFileInfo_3_1::ReadTextData()
{
}

//-------------------------------------------------------------------
//方法: ReadOverlayData()
//参数: 
//用途: 读取Overlay信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B00EA
VOID CGetV3DFileInfo_3_1::ReadOverlayData()
{
}

//-------------------------------------------------------------------
//方法: ReadNavigaterData()
//参数: 
//用途: 读取Navigate信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B00EB
VOID CGetV3DFileInfo_3_1::ReadNavigaterData()
{
}

//-------------------------------------------------------------------
//方法: ReadButtonData()
//参数: 
//用途: 读取Button信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B00EC
VOID CGetV3DFileInfo_3_1::ReadButtonData()
{

}

//-------------------------------------------------------------------
//方法: ReadImageData()
//参数: 
//用途: 读取Image信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B0109
VOID CGetV3DFileInfo_3_1::ReadImageData()
{
}

//-------------------------------------------------------------------
//方法: ReadCubeMapData()
//参数: 
//用途: 读取CubeMap信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B010A
VOID CGetV3DFileInfo_3_1::ReadCubeMapData()
{

}

//读取MovieTex信息，2.5版本Up
//##ModelId=47EBC74B010B
VOID CGetV3DFileInfo_3_1::ReadMovieTexData()
{
}

//读取BumpWater信息，2.5版本Up
//##ModelId=47EBC74B010C
VOID CGetV3DFileInfo_3_1::ReadBumpWaterData()
{
}

//-------------------------------------------------------------------
//方法: FindReferenceModel()
//参数: 
//用途: 查找每个模型的引用模型并得到引用模型指针
//-------------------------------------------------------------------
//##ModelId=47EBC74B01A5
VOID CGetV3DFileInfo_3_1::FindReferenceModel()
{
}

//-------------------------------------------------------------------
//方法: LoadE3DFile()
//参数: CScene* pScene, CHAR*  strFileName
//用途: 读取对应的Scene场景为E3D文件
//-------------------------------------------------------------------
//##ModelId=47EBC74B01A6
HRESULT CGetV3DFileInfo_3_1::LoadV3DFile(CScene* theScene, CHAR*  strFileName)
{
	return S_OK;
}		

//##ModelId=47EBC74B01B5
VOID CGetV3DFileInfo_3_1::Release()
{
}
