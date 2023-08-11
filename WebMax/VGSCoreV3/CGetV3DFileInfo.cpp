/////////////////////////////////////////////////////////////////////////
// 名称: CGetV3DFileInfo--V3D文件读取类
// 描述: 读取V3D文件，并规整场景数据
// 作者: lixin	
// 日期: 2006-05-06
////////////////////////////////////////////////////////////////////////

#include "CGetV3DFileInfo.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74B03D8
CGetV3DFileInfo::CGetV3DFileInfo(CRITICAL_SECTION*	pRenderCS)
{
}

//##ModelId=47EBC74C000F
CGetV3DFileInfo::~CGetV3DFileInfo()
{

}


//-------------------------------------------------------------------
//方法: ReadHeader()
//参数: 
//用途: 读取文件头
//-------------------------------------------------------------------
//##ModelId=47EBC74B02E2
VOID CGetV3DFileInfo::ReadHeader()
{
}

//-------------------------------------------------------------------
//方法: ReadVertexPosData()
//参数: 
//用途: 读取顶点位置信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B032C
VOID CGetV3DFileInfo::ReadVertexPosData()
{
}

//-------------------------------------------------------------------
//方法: ReadVertexNormalData()
//参数: 
//用途: 读取顶点的法线信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B034B
VOID CGetV3DFileInfo::ReadVertexNormalData()
{
}

//-------------------------------------------------------------------
//方法: ReadVertexUVData()
//参数: 
//用途: 读取顶点的UV信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B036B
VOID CGetV3DFileInfo::ReadVertexUVData()
{
}

//-------------------------------------------------------------------
//方法: ReadVetexData()
//参数: 
//用途: 读取顶点信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B037A
VOID CGetV3DFileInfo::ReadVertexData()
{
}

//-------------------------------------------------------------------
//方法: ReadModelData()
//参数: 
//用途: 读取模型信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B031C
VOID CGetV3DFileInfo::ReadModelData()
{
}

//-------------------------------------------------------------------
//方法: ReadCameraData()
//参数: 
//用途: 读取摄像机信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B02FD
VOID CGetV3DFileInfo::ReadCameraData()
{
}

//-------------------------------------------------------------------
//方法: ReadTexData()
//参数: 
//用途: 读取贴图信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B03AB
VOID CGetV3DFileInfo::ReadTexData()
{
}

//-------------------------------------------------------------------
//方法: ReadMaterialData()
//参数: 
//用途: 读取材质信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B030D
VOID CGetV3DFileInfo::ReadMaterialData()
{
}

//-------------------------------------------------------------------
//方法: ReadLightData()
//参数: 
//用途: 读入灯光信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B030E
VOID CGetV3DFileInfo::ReadLightData()
{
}

//-------------------------------------------------------------------
//方法: ReadTextData()
//参数: 
//用途: 读取文字信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B0399
VOID CGetV3DFileInfo::ReadTextData()
{
	/*
	fseek(pFile, Header.FileTable.TextDataPtr, SEEK_SET);

	//读入文字数量
	fread(&(TextData.TextCnt), sizeof(TextData.TextCnt), 1, pFile);

	//如果文字数量为0，则忽略保存文字内容列表字段
	if (TextData.TextCnt == 0) return;

	//text data
	TextData.Texts = new V3D_Text[TextData.TextCnt];
	for (UINT i= 0; i< TextData.TextCnt; i++)
	{
		CText* pText = new CText();

		pText->index = i;
		
		//读入文字数据
		fread(&(TextData.Texts[i].Length), sizeof(TextData.Texts[i].Length), 1, pFile);

		//如果字符串为空，则忽略保存字符串的StrText字段
		if (TextData.Texts[i].Length > 0) 
		{
			TextData.Texts[i].StrText = new CHAR[TextData.Texts[i].Length];
			fread(TextData.Texts[i].StrText, TextData.Texts[i].Length, 1, pFile);
			pText->strText = new CHAR[TextData.Texts[i].Length];
			strcpy(pText->strText, TextData.Texts[i].StrText);
			pText->strText[TextData.Texts[i].Length] = '\0';
			//release memory
			SAFE_DELETE_ARRAY(TextData.Texts[i].StrText);
		}

		fread(&(TextData.Texts[i].Color), sizeof(TextData.Texts[i].Color), 1, pFile);
		pText->color = TextData.Texts[i].Color;

		fread(&(TextData.Texts[i].X), sizeof(TextData.Texts[i].X), 1, pFile);
		pText->rect.left = TextData.Texts[i].X ;

		fread(&(TextData.Texts[i].Y), sizeof(TextData.Texts[i].Y), 1, pFile);
		pText->rect.top = TextData.Texts[i].Y;

		pScene->text.push_back(pText);

	}

	//release memory
	SAFE_DELETE_ARRAY(TextData.Texts);
	*/
}

//-------------------------------------------------------------------
//方法: ReadOverlayData()
//参数: 
//用途: 读取Overlay信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B039A
VOID CGetV3DFileInfo::ReadOverlayData()
{
}

//-------------------------------------------------------------------
//方法: ReadNavigaterData()
//参数: 
//用途: 读取Navigate信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B03A9
VOID CGetV3DFileInfo::ReadNavigaterData()
{
}

//-------------------------------------------------------------------
//方法: ReadButtonData()
//参数: 
//用途: 读取Button信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B03AA
VOID CGetV3DFileInfo::ReadButtonData()
{
}

//-------------------------------------------------------------------
//方法: ReadImageData()
//参数: 
//用途: 读取Image信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B03B9
VOID CGetV3DFileInfo::ReadImageData()
{}

//-------------------------------------------------------------------
//方法: ReadCubeMapData()
//参数: 
//用途: 读取CubeMap信息
//-------------------------------------------------------------------
//##ModelId=47EBC74B03BA
VOID CGetV3DFileInfo::ReadCubeMapData()
{
}

//读取MovieTex信息，2.5版本Up
//##ModelId=47EBC74B03BB
VOID CGetV3DFileInfo::ReadMovieTexData()
{
}

//读取BumpWater信息，2.5版本Up
//##ModelId=47EBC74B03C8
VOID CGetV3DFileInfo::ReadBumpWaterData()
{
}

//-------------------------------------------------------------------
//方法: FindReferenceModel()
//参数: 
//用途: 查找每个模型的引用模型并得到引用模型指针
//-------------------------------------------------------------------
//##ModelId=47EBC74B03C9
VOID CGetV3DFileInfo::FindReferenceModel()
{
	
}

//-------------------------------------------------------------------
//方法: LoadE3DFile()
//参数: CScene* pScene, CHAR*  strFileName
//用途: 读取对应的Scene场景为E3D文件
//-------------------------------------------------------------------
//##ModelId=47EBC74B03CA
HRESULT CGetV3DFileInfo::LoadV3DFile(CScene* theScene, CHAR*  strFileName)
{
	return S_OK;
}		

//##ModelId=47EBC74B03CD
VOID CGetV3DFileInfo::Release()
{
}
