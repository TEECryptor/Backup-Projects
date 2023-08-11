//------------------------------------------------------------------------
//CDXF.cpp DXF�ļ�����ͷ�ļ�
//����������DXF�ļ���������
//����: lixin
//����: 2006 -05 -07
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
//����: LoadDXFFileToMesh()
//����: char* strDXFFileName, CScene* pScene
//��;: ����DXF�ļ������������ݣ�ת����Mesh
//-------------------------------------------------------------------
HRESULT CDXF::LoadDXFFileToMesh(char* strDXFFileName, CScene* theScene)
{
	pScene = theScene;
	//OK! Now Render Scene
	pScene->renderScene = FALSE;
	if (SUCCEEDED(LoadDXFFile(strDXFFileName)))
	{
		//�Ѷ�ά�߶�Extrude = 0
		ConvertPolyLineToMesh();	
		//����ͬ���ʵ�ģ�͹��������ʵ�modelIndices��
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
//����: Release()
//����: 
//��;: �ͷ���Դ
//-------------------------------------------------------------------
VOID CDXF::Release()
{
	
}

//-------------------------------------------------------------------
//����: LoadDXFFile()
//����: 
//��;: ����DXF�ļ������������ݣ�ֻ��ȡPOLYLINE����
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
		//�ҵ�PolyLine����� 
		if (strcmp(tLine, "POLYLINE") == 0)
		{
			bPolyLine = true;
			PolyLineIndex++;
			//�õ���ǰ�����ߵĵ������
			int tPos = ftell(DXFFile);
			m_pPolyLines[PolyLineIndex].PointNum = GetVertexCount(PolyLineIndex);
			fseek(DXFFile, tPos, SEEK_SET);
			m_pPolyLines[PolyLineIndex].Points = new VERTEX2D[m_pPolyLines[PolyLineIndex].PointNum ];
			VertexIndex = -1;
		}
		
		//��ǰPolyLine����ɫ
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
//����: GetPolyLineCount()
//����: 
//��;: �ȵõ������ߵ�����
//-------------------------------------------------------------------
UINT CDXF::GetPolyLineCount()
{
	fseek(DXFFile, 0, SEEK_SET);
	char tLine[256];
	int	 tLineCnt= 0;
	//�ȵõ������ߵ�����
	while ( !feof(DXFFile) && !ferror(DXFFile))
	{
		//���ж�����
		fscanf(DXFFile, "%s\\n", tLine);
		//�жϵ�ǰ���Ƿ���POLYLINE
		if (strcmp(tLine, "POLYLINE") == 0)
			tLineCnt ++;
	}
	fseek(DXFFile, 0, SEEK_SET);
	return tLineCnt;
}


//-------------------------------------------------------------------
//����: GetVertexCount()
//����: 
//��;: �õ���������
//-------------------------------------------------------------------
UINT CDXF::GetVertexCount(UINT PolyLineIndex)
{
	fseek(DXFFile, 0, SEEK_SET);
	CHAR tLine[256];
	INT	 tLineIndex= -1;
	BOOL bPolyLine = FALSE;
	UINT vertexCnt = 0;
	//�ȵõ������ߵ�����
	while ( !feof(DXFFile) && !ferror(DXFFile))
	{
		//���ж�����
		fscanf(DXFFile, "%s\\n", tLine);
		//�жϵ�ǰ���Ƿ���POLYLINE
		if (strcmp(tLine, "POLYLINE") == 0)
		{
			bPolyLine = TRUE;
			tLineIndex ++;
		}
		
		if (tLineIndex == (INT)PolyLineIndex) 
		{
			//���ҵ�ǰPolyLine�е�Vertex������
			if (strcmp(tLine, "VERTEX") == 0) 
				vertexCnt++;
		}
		
	}
	fseek(DXFFile, 0, SEEK_SET);
	return vertexCnt;
}

//-------------------------------------------------------------------
//����: CreateMtrl()
//����: 
//��;: ��������,һ��ģ��һ������
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
//����: GetVertexData()
//����: 
//��;: ��䶥��
//-------------------------------------------------------------------
VOID CDXF::GetVertexData(CModel* pModel, UINT i)
{
	UINT nVertexIndex = 0;
	//�ϱ��涥��,��������������
	for (UINT VertexOffset = 0; VertexOffset <  m_pPolyLines[i].PointNum; VertexOffset++ )
	{	
		D3DXVECTOR3 tVec;
		tVec.x = m_pPolyLines[i].Points[VertexOffset].x - pModel->worldPosition.x;
		tVec.y = 0.0f - pModel->worldPosition.y;
		tVec.z = m_pPolyLines[i].Points[VertexOffset].y - pModel->worldPosition.z;
		
		pModel->vertex[nVertexIndex].index = nVertexIndex;
		pModel->vertex[nVertexIndex].position = tVec;
		
		//���÷���
		pModel->vertex[nVertexIndex].normal = D3DXVECTOR3(0,1,0);
		pModel->vertex[nVertexIndex].su = 0.0f;
		pModel->vertex[nVertexIndex].sv = 0.0f;
		pModel->vertex[nVertexIndex].sw = 0.0f;
		pModel->vertex[nVertexIndex].sz = 0.0f;

		//���ö�����ɫ
		D3DMATERIAL9 mtrl = ((CMaterial*)(pScene->m_vShader.GetAt(i)))->d3dmtrl;
		pModel->vertex[nVertexIndex].diffuse = D3DCOLOR_RGBA((INT)(mtrl.Diffuse.r * 255),
															 (INT)(mtrl.Diffuse.g * 255),
															 (INT)(mtrl.Diffuse.b * 255),
															 (INT)(mtrl.Diffuse.a * 255));
		nVertexIndex ++;
	}

	//�±��涥��
	for (VertexOffset = 0; VertexOffset <  m_pPolyLines[i].PointNum; VertexOffset++ )
	{
		D3DXVECTOR3 tVec;
		tVec.x = m_pPolyLines[i].Points[VertexOffset].x - pModel->worldPosition.x;
		tVec.y = 0.0f - pModel->worldPosition.y;
		tVec.z = m_pPolyLines[i].Points[VertexOffset].y - pModel->worldPosition.z;

		pModel->vertex[nVertexIndex].index = nVertexIndex;
		pModel->vertex[nVertexIndex].position = tVec;
		
		//���÷���
		pModel->vertex[nVertexIndex].normal = D3DXVECTOR3(0,1,0);
		pModel->vertex[nVertexIndex].su = 0.0f;
		pModel->vertex[nVertexIndex].sv = 0.0f;
		pModel->vertex[nVertexIndex].sw = 0.0f;
		pModel->vertex[nVertexIndex].sz = 0.0f;

		//���ö�����ɫ
		D3DMATERIAL9 mtrl = ((CMaterial*)(pScene->m_vShader.GetAt(i)))->d3dmtrl;
		pModel->vertex[nVertexIndex].diffuse = D3DCOLOR_RGBA((INT)(mtrl.Diffuse.r * 255),
															 (INT)(mtrl.Diffuse.g * 255),
															 (INT)(mtrl.Diffuse.b * 255),
															 (INT)(mtrl.Diffuse.a * 255));
		nVertexIndex ++;
	}
}

//-------------------------------------------------------------------
//����: GetFaceData()
//����: UINT i
//��;: ���������
//-------------------------------------------------------------------
VOID CDXF::GetFaceData(CModel* pModel, UINT i)
{
	//����������������
	//�ϱ���
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
	//�±���
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
	//�����
	UINT TopVIndex = 0;
	for (UINT k = 0; k< m_pPolyLines[i].PointNum; k++)
	{		
		pModel->face[index].index = index;
		pModel->face[index].visible = TRUE;
		//����ָ��
		pModel->face[index].vertices0 = &(pModel->vertex[TopVIndex]);
		pModel->face[index].vertices1 = &(pModel->vertex[TopVIndex + m_pPolyLines[i].PointNum]);
		pModel->face[index].vertices2 = &(pModel->vertex[TopVIndex +1]);
		//���㷨��
		D3DXVECTOR3 ab, ac, normal;
		ab = pModel->face[index].vertices0->position - pModel->face[index].vertices1->position;
		ac = pModel->face[index].vertices0->position - pModel->face[index].vertices2->position;
		D3DXVec3Cross(&normal, &ab, &ac);
		pModel->face[index].normal = normal;
		index++;

		pModel->face[index].index = index;
		pModel->face[index].visible = TRUE;
		//����ָ��
		pModel->face[index].vertices0 = &(pModel->vertex[TopVIndex +1]);
		pModel->face[index].vertices1 = &(pModel->vertex[TopVIndex + m_pPolyLines[i].PointNum]);
		pModel->face[index].vertices2 = &(pModel->vertex[(TopVIndex + m_pPolyLines[i].PointNum + 1)%(pModel->vertexCnt)]);
		//���㷨��
		ab = pModel->face[index].vertices0->position - pModel->face[index].vertices1->position;
		ac = pModel->face[index].vertices0->position - pModel->face[index].vertices2->position;
		D3DXVec3Cross(&normal, &ab, &ac);
		pModel->face[index].normal = normal;
		index++;

		TopVIndex++;
	}
}

//-------------------------------------------------------------------
//����: ConvertPolyLineToMesh()
//����: 
//��;: �Ѷ�����ת���� Mesh�� Ĭ��Extrude Ϊ0
//-------------------------------------------------------------------
VOID CDXF::ConvertPolyLineToMesh()
{
	UINT preCnt = pScene->model.size();
	//��������
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
		//�����Χ�кͰ�Χ��
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
		//��ģ�͵���ת��λ�ñ任��Axis�����ž��󲻷Ž�ȥ,�������Ҳ��С��
		//pModel->pAxis->SetRotationMatrix(&pModel->transform.rotation_matrix);
		//pModel->pAxis->SetTranslateMatrix(&pModel->transform.translate_matrix);
		//pModel->pAxis->MultiplyMatrix();
		//pModel->showAxis = 0;

		//����������
		pModel->faceCnt = (m_pPolyLines[i].PointNum -2) * 2 + (m_pPolyLines[i].PointNum ) * 2;
		pModel->face = new CFace[pModel->faceCnt];
		//���㵱ǰģ�Ͷ�������
		pModel->vertexCnt = m_pPolyLines[i].PointNum *2;
		pModel->vertex = new CVertex[pModel->vertexCnt];
		//��䶥��
		GetVertexData(pModel, i);

		//���������
		GetFaceData(pModel, i);
			
		//�������㻺�����������
		pScene->CreateModelVBuffer(pModel);
		pScene->CreateModelIBuffer(pModel);

		//�Ž�ģ�Ͷ���
		pScene->model.push_back(pModel);
	}
	//release mem
	SAFE_DELETE_ARRAY(m_pPolyLines);
}
