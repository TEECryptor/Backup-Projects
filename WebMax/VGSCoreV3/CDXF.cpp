//------------------------------------------------------------------------
//CDXF.cpp DXF文件导入头文件
//描述：导入DXF文件到场景中
//作者: lixin
//日期: 2006 -05 -07
//------------------------------------------------------------------------

#include "CDXF.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDXF::CDXF()
{
	m_pPolyLines = NULL;
}

CDXF::~CDXF()
{
	
}

//-------------------------------------------------------------------
//方法: LoadDXFFileToMesh()
//参数: char* strDXFFileName, CScene* pScene
//用途: 导入DXF文件，并规整数据，转化成Mesh
//-------------------------------------------------------------------
HRESULT CDXF::LoadDXFFileToMesh(char* strDXFFileName, CScene* theScene)
{
	pScene = theScene;
	//OK! Now Render Scene
	pScene->renderScene = FALSE;
	if (SUCCEEDED(LoadDXFFile(strDXFFileName)))
	{
		//把二维线段Extrude = 0
		ConvertPolyLineToMesh();	
		//把相同材质的模型规整到材质的modelIndices中
		pScene->PushSameMtrlModels();
		//OK! Now Render Scene
		pScene->renderScene = TRUE;
		return S_OK;
	}
	//OK! Now Render Scene
	pScene->renderScene = TRUE;
	return E_FAIL;
}

//-------------------------------------------------------------------
//方法: Release()
//参数: 
//用途: 释放资源
//-------------------------------------------------------------------
VOID CDXF::Release()
{
	
}

//-------------------------------------------------------------------
//方法: LoadDXFFile()
//参数: 
//用途: 导入DXF文件，并规整数据，只读取POLYLINE数据
//-------------------------------------------------------------------
HRESULT	CDXF::LoadDXFFile(char* strFileName)
{
	DXFFile = fopen(strFileName, "rt");
	if (DXFFile == NULL) return E_FAIL;

	m_PolyLineCnt = GetPolyLineCount();
	//new PolyLine Obj
	m_pPolyLines = new POLYLINE[m_PolyLineCnt];
	
	char	tLine[256];
	int		index = 0;
	DWORD	Color = 0;
	bool    bPolyLine= false;
	bool	bVertex = false;
	int		PolyLineIndex = -1;
	int		VertexIndex;
	while (!feof(DXFFile) && !ferror(DXFFile))
	{
		fscanf(DXFFile, "%s\\n", tLine);
		//找到PolyLine的起点 
		if (strcmp(tLine, "POLYLINE") == 0)
		{
			bPolyLine = true;
			PolyLineIndex++;
			//得到当前多义线的点的数量
			int tPos = ftell(DXFFile);
			m_pPolyLines[PolyLineIndex].PointNum = GetVertexCount(PolyLineIndex);
			fseek(DXFFile, tPos, SEEK_SET);
			m_pPolyLines[PolyLineIndex].Points = new VERTEX2D[m_pPolyLines[PolyLineIndex].PointNum ];
			VertexIndex = -1;
		}
		
		//当前PolyLine的颜色
		if (bPolyLine)
		{
			if (strcmp(tLine, "62") == 0)
			{
				fscanf(DXFFile, "%s\\n", tLine);
				Color = atoi(tLine);
				m_pPolyLines[PolyLineIndex].Color = D3DCOLOR_RGBA(Color, Color, Color, 0 );
			}
			if (strcmp(tLine, "VERTEX") == 0)
			{
				bVertex = true;
				VertexIndex++;
			}
			if (bVertex)
			{
				if (strcmp(tLine, "10") == 0)
				{
					fscanf(DXFFile, "%s\\n", tLine);
					m_pPolyLines[PolyLineIndex].Points[VertexIndex].x = (float)atof(tLine);
				}
				if (strcmp(tLine, "20") == 0)
				{
					fscanf(DXFFile, "%s\\n", tLine);
					m_pPolyLines[PolyLineIndex].Points[VertexIndex].y = (float)atof(tLine);
				}
			}
		}
	}
	fclose(DXFFile);
	return S_OK;
}

//-------------------------------------------------------------------
//方法: GetPolyLineCount()
//参数: 
//用途: 先得到多义线的数量
//-------------------------------------------------------------------
UINT CDXF::GetPolyLineCount()
{
	fseek(DXFFile, 0, SEEK_SET);
	char tLine[256];
	int	 tLineCnt= 0;
	//先得到多义线的数量
	while ( !feof(DXFFile) && !ferror(DXFFile))
	{
		//逐行读数据
		fscanf(DXFFile, "%s\\n", tLine);
		//判断当前行是否是POLYLINE
		if (strcmp(tLine, "POLYLINE") == 0)
			tLineCnt ++;
	}
	fseek(DXFFile, 0, SEEK_SET);
	return tLineCnt;
}


//-------------------------------------------------------------------
//方法: GetVertexCount()
//参数: 
//用途: 得到顶点数量
//-------------------------------------------------------------------
UINT CDXF::GetVertexCount(UINT PolyLineIndex)
{
	fseek(DXFFile, 0, SEEK_SET);
	CHAR tLine[256];
	INT	 tLineIndex= -1;
	BOOL bPolyLine = FALSE;
	UINT vertexCnt = 0;
	//先得到多义线的数量
	while ( !feof(DXFFile) && !ferror(DXFFile))
	{
		//逐行读数据
		fscanf(DXFFile, "%s\\n", tLine);
		//判断当前行是否是POLYLINE
		if (strcmp(tLine, "POLYLINE") == 0)
		{
			bPolyLine = TRUE;
			tLineIndex ++;
		}
		
		if (tLineIndex == (INT)PolyLineIndex) 
		{
			//查找当前PolyLine中的Vertex的数量
			if (strcmp(tLine, "VERTEX") == 0) 
				vertexCnt++;
		}
		
	}
	fseek(DXFFile, 0, SEEK_SET);
	return vertexCnt;
}

//-------------------------------------------------------------------
//方法: CreateMtrl()
//参数: 
//用途: 创建材质,一个模型一个材质
//-------------------------------------------------------------------
VOID CDXF::CreateMtrl()
{
	UINT preCnt = pScene->m_vShader.GetCount();
	for (UINT i = 0; i<m_PolyLineCnt; i++)
	{
		CMaterial *pMtrl = new CMaterial();
		
		pMtrl->index = preCnt + i;

		CHAR _dxfMtrl[32];
		strcpy(_dxfMtrl, "dxfMtrl_");
		CHAR _dxfMtrlNum[8];
		itoa(i, _dxfMtrlNum, 10);
		strcat(_dxfMtrl, _dxfMtrlNum);
		strcpy(pMtrl->name, _dxfMtrl);
		pMtrl->d3dmtrl.Diffuse.r = ((m_pPolyLines[i].Color>>16)&0xff)/255.0f;
		pMtrl->d3dmtrl.Diffuse.g = ((m_pPolyLines[i].Color>>8)&0xff)/255.0f;
		pMtrl->d3dmtrl.Diffuse.b = ((m_pPolyLines[i].Color)&0xff)/255.0f;
		pMtrl->d3dmtrl.Diffuse.a = 0.0f;
		pMtrl->d3dmtrl.Ambient = pMtrl->d3dmtrl.Diffuse;
		pMtrl->d3dmtrl.Specular = pMtrl->d3dmtrl.Diffuse;

		pMtrl->selfIllumination = TRUE;

		pScene->m_vShader.PushBack(pMtrl);
	}
}

//-------------------------------------------------------------------
//方法: GetVertexData()
//参数: 
//用途: 填充顶点
//-------------------------------------------------------------------
VOID CDXF::GetVertexData(CModel* pModel, UINT i)
{
	UINT nVertexIndex = 0;
	//上表面顶点,逐个顶点填充数据
	for (UINT VertexOffset = 0; VertexOffset <  m_pPolyLines[i].PointNum; VertexOffset++ )
	{	
		D3DXVECTOR3 tVec;
		tVec.x = m_pPolyLines[i].Points[VertexOffset].x - pModel->worldPosition.x;
		tVec.y = 0.0f - pModel->worldPosition.y;
		tVec.z = m_pPolyLines[i].Points[VertexOffset].y - pModel->worldPosition.z;
		
		pModel->vertex[nVertexIndex].index = nVertexIndex;
		pModel->vertex[nVertexIndex].position = tVec;
		
		//设置法线
		pModel->vertex[nVertexIndex].normal = D3DXVECTOR3(0,1,0);
		pModel->vertex[nVertexIndex].su = 0.0f;
		pModel->vertex[nVertexIndex].sv = 0.0f;
		pModel->vertex[nVertexIndex].sw = 0.0f;
		pModel->vertex[nVertexIndex].sz = 0.0f;

		//设置顶点颜色
		D3DMATERIAL9 mtrl = ((CMaterial*)(pScene->m_vShader.GetAt(i)))->d3dmtrl;
		pModel->vertex[nVertexIndex].diffuse = D3DCOLOR_RGBA((INT)(mtrl.Diffuse.r * 255),
															 (INT)(mtrl.Diffuse.g * 255),
															 (INT)(mtrl.Diffuse.b * 255),
															 (INT)(mtrl.Diffuse.a * 255));
		nVertexIndex ++;
	}

	//下表面顶点
	for (VertexOffset = 0; VertexOffset <  m_pPolyLines[i].PointNum; VertexOffset++ )
	{
		D3DXVECTOR3 tVec;
		tVec.x = m_pPolyLines[i].Points[VertexOffset].x - pModel->worldPosition.x;
		tVec.y = 0.0f - pModel->worldPosition.y;
		tVec.z = m_pPolyLines[i].Points[VertexOffset].y - pModel->worldPosition.z;

		pModel->vertex[nVertexIndex].index = nVertexIndex;
		pModel->vertex[nVertexIndex].position = tVec;
		
		//设置法线
		pModel->vertex[nVertexIndex].normal = D3DXVECTOR3(0,1,0);
		pModel->vertex[nVertexIndex].su = 0.0f;
		pModel->vertex[nVertexIndex].sv = 0.0f;
		pModel->vertex[nVertexIndex].sw = 0.0f;
		pModel->vertex[nVertexIndex].sz = 0.0f;

		//设置顶点颜色
		D3DMATERIAL9 mtrl = ((CMaterial*)(pScene->m_vShader.GetAt(i)))->d3dmtrl;
		pModel->vertex[nVertexIndex].diffuse = D3DCOLOR_RGBA((INT)(mtrl.Diffuse.r * 255),
															 (INT)(mtrl.Diffuse.g * 255),
															 (INT)(mtrl.Diffuse.b * 255),
															 (INT)(mtrl.Diffuse.a * 255));
		nVertexIndex ++;
	}
}

//-------------------------------------------------------------------
//方法: GetFaceData()
//参数: UINT i
//用途: 填充面数据
//-------------------------------------------------------------------
VOID CDXF::GetFaceData(CModel* pModel, UINT i)
{
	//逐个面填充索引数据
	//上表面
	UINT index = 0;
	for (UINT m = 0; m < m_pPolyLines[i].PointNum -2; m++)
	{	
		pModel->face[index].index = index;
		pModel->face[index].visible = TRUE;
		pModel->face[index].vertices0 = &(pModel->vertex[0]);
		pModel->face[index].vertices1 = &(pModel->vertex[m+2]);
		pModel->face[index].vertices2 = &(pModel->vertex[m+1]);
		pModel->face[index].normal = D3DXVECTOR3(0, 1, 0);
		index++;
	}
	//下表面
	for (UINT n = 0; n < m_pPolyLines[i].PointNum -2; n++)
	{
		pModel->face[index].index = index;
		pModel->face[index].visible = TRUE;
		pModel->face[index].vertices0 = &(pModel->vertex[m_pPolyLines[i].PointNum + 0]);
		pModel->face[index].vertices1 = &(pModel->vertex[m_pPolyLines[i].PointNum + n+2]);
		pModel->face[index].vertices2 = &(pModel->vertex[m_pPolyLines[i].PointNum + n+1]);
		pModel->face[index].normal = D3DXVECTOR3(0, -1, 0);
		index++;
	}
	//侧表面
	UINT TopVIndex = 0;
	for (UINT k = 0; k< m_pPolyLines[i].PointNum; k++)
	{		
		pModel->face[index].index = index;
		pModel->face[index].visible = TRUE;
		//顶点指针
		pModel->face[index].vertices0 = &(pModel->vertex[TopVIndex]);
		pModel->face[index].vertices1 = &(pModel->vertex[TopVIndex + m_pPolyLines[i].PointNum]);
		pModel->face[index].vertices2 = &(pModel->vertex[TopVIndex +1]);
		//计算法线
		D3DXVECTOR3 ab, ac, normal;
		ab = pModel->face[index].vertices0->position - pModel->face[index].vertices1->position;
		ac = pModel->face[index].vertices0->position - pModel->face[index].vertices2->position;
		D3DXVec3Cross(&normal, &ab, &ac);
		pModel->face[index].normal = normal;
		index++;

		pModel->face[index].index = index;
		pModel->face[index].visible = TRUE;
		//顶点指针
		pModel->face[index].vertices0 = &(pModel->vertex[TopVIndex +1]);
		pModel->face[index].vertices1 = &(pModel->vertex[TopVIndex + m_pPolyLines[i].PointNum]);
		pModel->face[index].vertices2 = &(pModel->vertex[(TopVIndex + m_pPolyLines[i].PointNum + 1)%(pModel->vertexCnt)]);
		//计算法线
		ab = pModel->face[index].vertices0->position - pModel->face[index].vertices1->position;
		ac = pModel->face[index].vertices0->position - pModel->face[index].vertices2->position;
		D3DXVec3Cross(&normal, &ab, &ac);
		pModel->face[index].normal = normal;
		index++;

		TopVIndex++;
	}
}

//-------------------------------------------------------------------
//方法: ConvertPolyLineToMesh()
//参数: 
//用途: 把多义线转化成 Mesh， 默认Extrude 为0
//-------------------------------------------------------------------
VOID CDXF::ConvertPolyLineToMesh()
{
	UINT preCnt = pScene->model.size();
	//创建材质
	UINT preMtrlCnt = pScene->m_vShader.GetCount();
	CreateMtrl();
	
	for (UINT i = 0; i<m_PolyLineCnt; i++)
	{
		CModel* pModel = new CModel();
		//index
		pModel->index= preCnt + i;
		//name
		CHAR _dxfModelName[32];
		strcpy(_dxfModelName, "dxfModel_");
		CHAR _dxfModelNum[8];
		itoa(i, _dxfModelNum, 10);
		strcat(_dxfModelName, _dxfModelNum);
		strcpy(pModel->name, _dxfModelName);
		//shader
		pModel->shader = (CMaterial*)(pScene->m_vShader.GetAt(preMtrlCnt + i));	
		//计算包围盒和包围球
		FLOAT minx = 1000000.0f, miny = 1000000.0f;
		FLOAT maxx = -1000000.0f, maxy = -1000000.0f;
		for (UINT v2 =0; v2 < m_pPolyLines[i].PointNum; v2 ++)
		{
			if (minx > m_pPolyLines[i].Points[v2].x) minx = m_pPolyLines[i].Points[v2].x;
			if (miny > m_pPolyLines[i].Points[v2].y) miny = m_pPolyLines[i].Points[v2].y;

			if (maxx < m_pPolyLines[i].Points[v2].x) maxx = m_pPolyLines[i].Points[v2].x;
			if (maxy < m_pPolyLines[i].Points[v2].y) maxy = m_pPolyLines[i].Points[v2].y;
		}
		pModel->oriBBox.MinCorner.x = minx;
		pModel->oriBBox.MinCorner.y = 0;
		pModel->oriBBox.MinCorner.z = miny;
		pModel->oriBBox.MaxCorner.x = maxx;
		pModel->oriBBox.MaxCorner.y = 0;
		pModel->oriBBox.MaxCorner.z = maxy;
		//bounding sphere
		pModel->oriBSphere.Center = (pModel->oriBBox.MinCorner + pModel->oriBBox.MaxCorner) /2.0f;
		D3DXVECTOR3 tVec = pModel->oriBBox.MaxCorner - pModel->oriBBox.MinCorner;
		pModel->oriBSphere.Radius = D3DXVec3Length(&tVec) /2.0f;

		//worldposition
		pModel->worldPosition = pModel->transform.position = pModel->oriBSphere.Center;
		pModel->oriBBox.MinCorner -= pModel->worldPosition; 
		pModel->oriBBox.MaxCorner -= pModel->worldPosition; 
		pModel->oriBSphere.Center -= pModel->worldPosition; 

		//transform
		pModel->transform.translate_matrix._41 = pModel->worldPosition.x;
		pModel->transform.translate_matrix._42 = pModel->worldPosition.y;
		pModel->transform.translate_matrix._43 = pModel->worldPosition.z;
		
		//update model matrix
		pModel->UpdateScaleMatrix();
		pModel->UpdateRotateMatrix();
		pModel->UpdateTranslateMatrix();
		pModel->MultiplyMatrix();
		//update other like bound box
		pModel->UpdateOther();
	
		//model axis
		//pModel->pAxis = new CAxis(pScene->m_pd3dDevice);
		//把模型的旋转和位置变换给Axis，缩放矩阵不放进去,否则把轴也缩小了
		//pModel->pAxis->SetRotationMatrix(&pModel->transform.rotation_matrix);
		//pModel->pAxis->SetTranslateMatrix(&pModel->transform.translate_matrix);
		//pModel->pAxis->MultiplyMatrix();
		//pModel->showAxis = 0;

		//计算面数量
		pModel->faceCnt = (m_pPolyLines[i].PointNum -2) * 2 + (m_pPolyLines[i].PointNum ) * 2;
		pModel->face = new CFace[pModel->faceCnt];
		//计算当前模型顶点数量
		pModel->vertexCnt = m_pPolyLines[i].PointNum *2;
		pModel->vertex = new CVertex[pModel->vertexCnt];
		//填充顶点
		GetVertexData(pModel, i);

		//填充面数据
		GetFaceData(pModel, i);
			
		//创建顶点缓冲和索引缓冲
		pScene->CreateModelVBuffer(pModel);
		pScene->CreateModelIBuffer(pModel);

		//放进模型队列
		pScene->model.push_back(pModel);
	}
	//release mem
	SAFE_DELETE_ARRAY(m_pPolyLines);
}
