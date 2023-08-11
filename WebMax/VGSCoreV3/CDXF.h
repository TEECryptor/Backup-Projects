//------------------------------------------------------------------------
//CDXF.h DXF文件导入头文件
//描述：导入DXF文件到场景中
//作者: lixin
//日期: 2006 -05 -07
//------------------------------------------------------------------------

#ifndef _CDXF_H
#define _CDXF_H

#include <stdio.h>
#include "CManyFVF.h"
#include "CScene.h"

class CDXF  
{
public:
	CDXF();
	~CDXF();

//var
private:
	FILE*					DXFFile;
	CScene*					pScene;

	//模型对象的数学变量--------------------------------------------------------
	UINT					m_PolyLineCnt;		//多义线数量
	POLYLINE*				m_pPolyLines;		//多义线序列

	CHAR					Name[32];			//静态DXF模型文件的名称
	CHAR					Path[224];			//静态DXF模型文件的路径
	CHAR					PathAndName[256];	//静态DXF模型文件的路径和名称

	UINT			GetPolyLineCount();
	UINT			GetVertexCount(UINT PolyLineIndex);
	VOID			ConvertPolyLineToMesh();
	HRESULT			LoadDXFFile(char* strFileName);
	//创建材质
	VOID CreateMtrl();
	//填充顶点
	VOID GetVertexData(CModel* pModel, UINT index);  //index是多义线编号
	//填充面数据
	VOID GetFaceData(CModel* pModel, UINT index);    //index是多义线编号

public:
	//转化成三维数据
	UINT				faceCnt;
	UINT				vertexCnt;

public:
	VOID	Release();
	HRESULT LoadDXFFileToMesh(char* strDXFFileName, CScene* theScene);
};

#endif 
