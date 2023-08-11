/////////////////////////////////////////////////////////////////////////
// 名称: CGetE3DFileInfo--E3D文件读取类
// 描述: 读取E3D文件，并规整场景数据
// 作者: lixin	
// 日期: 2006-05-06
////////////////////////////////////////////////////////////////////////

#include "CGetE3DFileInfo.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74E0001
CGetE3DFileInfo::CGetE3DFileInfo(CRITICAL_SECTION*	pRenderCS)
{
	//m_pRenderCS = pRenderCS;
	//pFile = NULL;		//E3D文件数据流
}

//##ModelId=47EBC74E001F
CGetE3DFileInfo::~CGetE3DFileInfo()
{

}


//-------------------------------------------------------------------
//方法: ReadHeader()
//参数: 
//用途: 读取文件头
//-------------------------------------------------------------------
//##ModelId=47EBC74D0316
VOID CGetE3DFileInfo::ReadHeader()
{/*
	//write the Header to pFile
	if (pFile == NULL) return;
	//init _E3DHeader
	fseek(pFile, 0, SEEK_SET);
	fread(&Header, sizeof(Header), 1, pFile);
	*/

}

//-------------------------------------------------------------------
//方法: ReadVertexPosData()
//参数: 
//用途: 读取顶点位置信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D035B
VOID CGetE3DFileInfo::ReadVertexPosData()
{/*
	fseek(pFile, Header.FileTable.VertexPosDataPtr, SEEK_SET);
	
	//zipped vertex cnt;
	fread(&VertexPosData.VertexPosCnt, sizeof(VertexPosData.VertexPosCnt), 1, pFile);

	VertexPosData.PosList = new D3DXVECTOR3[VertexPosData.VertexPosCnt];
	
	for (UINT i = 0; i< VertexPosData.VertexPosCnt; i++)
	{
		fread(&(VertexPosData.PosList[i]), sizeof(VertexPosData.PosList[i]), 1, pFile);
	}*/
}

//-------------------------------------------------------------------
//方法: ReadVertexNormalData()
//参数: 
//用途: 读取顶点的法线信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D036B
VOID CGetE3DFileInfo::ReadVertexNormalData()
{/*
	fseek(pFile, Header.FileTable.VertexNormalDataPtr, SEEK_SET);
	
	//zipped vertex cnt;
	fread(&VertexNormalData.VertexNormalCnt, sizeof(VertexNormalData.VertexNormalCnt), 1, pFile);

	VertexNormalData.NormalList = new D3DXVECTOR3[VertexNormalData.VertexNormalCnt];
	
	for (UINT i = 0; i< VertexNormalData.VertexNormalCnt; i++)
	{
		fread(&(VertexNormalData.NormalList[i]), sizeof(VertexNormalData.NormalList[i]), 1, pFile);
	}*/
}

//-------------------------------------------------------------------
//方法: ReadVertexUVData()
//参数: 
//用途: 读取顶点的UV信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D038A
VOID CGetE3DFileInfo::ReadVertexUVData()
{/*
	fseek(pFile, Header.FileTable.UVDataPtr, SEEK_SET);	

	//zipped uv count
	fread(&(UVData.UVCount), sizeof(UVData.UVCount), 1, pFile);

	UVData.UVList = new _UV[UVData.UVCount];
	
	for (UINT i = 0; i< UVData.UVCount; i++)
	{
		fread(&(UVData.UVList[i]), sizeof(UVData.UVList[i]), 1, pFile);
	}*/
}

//-------------------------------------------------------------------
//方法: ReadVetexData()
//参数: 
//用途: 读取顶点信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D0399
VOID CGetE3DFileInfo::ReadVertexData()
{/*
	fseek(pFile, Header.FileTable.VertexDataPtr, SEEK_SET);

	//vertex count
	fread(&(VertexData.VertexCnt), sizeof(VertexData.VertexCnt), 1, pFile);

	//vertex data
	VertexData.Vertex = new _Vertex[VertexData.VertexCnt];
	for (UINT i = 0; i< VertexData.VertexCnt; i++)
	{
		//vertex pos data offset in VertexPosData array
		fread(&(VertexData.Vertex[i].indexInVPList), sizeof(VertexData.Vertex[i].indexInVPList), 1, pFile);

		//vertex normal data offset in VertexNormalData array
		fread(&(VertexData.Vertex[i].indexInVNList), sizeof(VertexData.Vertex[i].indexInVNList), 1, pFile);
	
		//vertex uv data offset in UVData array
		fread(&(VertexData.Vertex[i].indexInUVList), sizeof(VertexData.Vertex[i].indexInUVList), 1, pFile);
		
		if (VertexData.Vertex[i].indexInUVList > UVData.UVCount -1)
		{
			MessageBox(GetActiveWindow(), "顶点UV数据解压发生未知错误，无法打开文件。", "顶点UV解压...错误", MB_OK | MB_ICONERROR);
		}

	}*/
}

//-------------------------------------------------------------------
//方法: ReadModelData()
//参数: 
//用途: 读取模型信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D034B
VOID CGetE3DFileInfo::ReadModelData()
{/*
	fseek(pFile, Header.FileTable.ModelDataPtr, SEEK_SET);

	//读入模型的数量
	fread(&(ModelData.ModelCnt), sizeof(ModelData.ModelCnt), 1, pFile);

	ModelData.Model = new _Model[ModelData.ModelCnt];
	UINT tVertexOffset = 0;
	//得到模型数据
	for(UINT modelIndex= 0; modelIndex< ModelData.ModelCnt; modelIndex++)
	{
		CModel* pModel = new CModel();
		pModel->index = modelIndex;

		//name
		fread(&(ModelData.Model[modelIndex].Name), sizeof(ModelData.Model[modelIndex].Name), 1, pFile);
		strcpy(pModel->name, ModelData.Model[modelIndex].Name);

		//matrix
		fread(&(ModelData.Model[modelIndex].Matrix), sizeof(ModelData.Model[modelIndex].Matrix), 1, pFile);
		//计算模型的位置，转角，缩放值
		D3DXMATRIX tMat = ModelData.Model[modelIndex].Matrix;
		
		D3DXVECTOR3 tVecX = D3DXVECTOR3(tMat._11, tMat._12, tMat._13);
		FLOAT tLenX= D3DXVec3Length(&tVecX);
		D3DXVECTOR3 tVecY = D3DXVECTOR3(tMat._21, tMat._22, tMat._23);
		FLOAT tLenY= D3DXVec3Length(&tVecY);
		D3DXVECTOR3 tVecZ = D3DXVECTOR3(tMat._31, tMat._32, tMat._33);
		FLOAT tLenZ= D3DXVec3Length(&tVecZ);

		//so model rotation is:
		GetRotationVecByRT(&tMat, &(pModel->transform.rotation));

		//so model scale is:
		pModel->transform.scale = D3DXVECTOR3(tLenX, tLenY, tLenZ);
		//model translate is:
		pModel->worldPosition = D3DXVECTOR3(tMat._41, tMat._42, tMat._43);
		pModel->transform.position = pModel->worldPosition;

		//pivot
		fread(&(ModelData.Model[modelIndex].Pivot), sizeof(ModelData.Model[modelIndex].Pivot), 1, pFile);
		pModel->pivot = ModelData.Model[modelIndex].Pivot;
			
		//读入模型的ID1
		fread(&(ModelData.Model[modelIndex].ID1), sizeof(ModelData.Model[modelIndex].ID1), 1, pFile);
		pModel->ID1 = ModelData.Model[modelIndex].ID1;

		//读入模型的ID2
		fread(&(ModelData.Model[modelIndex].ID2), sizeof(ModelData.Model[modelIndex].ID2), 1, pFile);
		pModel->ID2 = ModelData.Model[modelIndex].ID2;

		//reference
		fread(&(ModelData.Model[modelIndex].RefModelIndex), sizeof(ModelData.Model[modelIndex].RefModelIndex), 1, pFile);
		pModel->refModelIndex = ModelData.Model[modelIndex].RefModelIndex;

		//如果是索引模型，则后面的数据不读了
		if (pModel->refModelIndex == -1)
		{
			//bounding box
			fread(&(ModelData.Model[modelIndex].OriBBox), sizeof(ModelData.Model[modelIndex].OriBBox), 1, pFile);
			pModel->oriBBox.MinCorner = ModelData.Model[modelIndex].OriBBox.vMin;
			pModel->oriBBox.MaxCorner = ModelData.Model[modelIndex].OriBBox.vMax;
					
			//Bounding Sphere
			pModel->oriBSphere.Center = (pModel->oriBBox.MaxCorner + pModel->oriBBox.MinCorner)/2.0f;
			D3DXVECTOR3 Vec3Len = pModel->oriBBox.MaxCorner - pModel->oriBBox.MinCorner;
			pModel->oriBSphere.Radius = D3DXVec3Length(&Vec3Len)/2.0f;


			//update matrix
			pModel->UpdateScaleMatrix();
			pModel->UpdateRotateMatrix();
			pModel->UpdateTranslateMatrix();
			pModel->MultiplyMatrix();
			pModel->transform.matrix = tMat;
			pModel->transform.ori_matrix = tMat;
			//update other
			pModel->UpdateOther();
			pModel->visible = TRUE;

			//model axis
			pModel->pAxis = new CAxis(pScene->m_pd3dDevice);
			//把模型的位置变换给Axis，缩放矩阵不放进去,否则把轴也缩小了GCS	
			pModel->pAxis->SetRotationMatrix(&pModel->transform.rotation_matrix);
			pModel->pAxis->SetTranslateMatrix(&pModel->transform.translate_matrix);
			pModel->pAxis->MultiplyMatrix();
			//pModel->showAxis = false;

			//model RenderBoundBox
			pModel->pRBBox= new CRenderBoundBox(pScene->m_pd3dDevice);
			pModel->pRBBox->SetRBBoxVertex(pModel->oriBBox.MinCorner,pModel->oriBBox.MaxCorner);
			//把模型的位置变换给RBBox，
			pModel->pRBBox->SetRotationMatrix(&pModel->transform.rotation_matrix);
			pModel->pRBBox->SetTranslateMatrix(&pModel->transform.translate_matrix);
			pModel->pRBBox->SetScaleMatrix(&pModel->transform.scale_matrix);
			pModel->pRBBox->MultiplyMatrix();
			//pModel->showRBBox = false;

			//material ID
			fread(&(ModelData.Model[modelIndex].MaterialID), sizeof(ModelData.Model[modelIndex].MaterialID), 1, pFile);
			pModel->shader = pScene->shader[ModelData.Model[modelIndex].MaterialID];

			//vertex offset
			fread(&(ModelData.Model[modelIndex].VertexOffset), sizeof(ModelData.Model[modelIndex].VertexOffset), 1, pFile);

			//vertex count
			fread(&(ModelData.Model[modelIndex].VertexCnt), sizeof(ModelData.Model[modelIndex].VertexCnt), 1, pFile);
			pModel->vertexCnt = ModelData.Model[modelIndex].VertexCnt;

			//如果不是索引模型，则读入顶点，如果是索引模型，就不读入顶点了，这样节省了内存空间
			if (pModel->refModelIndex == -1)
			{
				pModel->vertex = new CVertex[pModel->vertexCnt];
				//注意: 对于顶点的颜色，为了尽量和max中的渲染效果接近，所以调和了灯光和材质的颜色。
				//当有贴图时，调和顶点的颜色值变暗，然后再与贴图混合出高光的效果
				//当没有贴图时，顶点的颜色为原始颜色
				BYTE alpha = (BYTE)(pModel->shader->opacity/100.0f * 255);
				if (pModel->shader->opacity == 100)
					alpha = 255;

				for (UINT m = 0; m< pModel->vertexCnt; m++)
				{
					pModel->vertex[m].index = m;
					//positon
					UINT VertexPosDataIndex = VertexData.Vertex[ModelData.Model[modelIndex].VertexOffset + m].indexInVPList;
					pModel->vertex[m].position = VertexPosData.PosList[VertexPosDataIndex];

					//world position
					//D3DXVec3TransformCoord(&pModel->vertex[m].worldPosition, &pModel->vertex[m].position, &(pModel->transform.matrix));
					
					//normal
					UINT VertexNormalDataIndex = VertexData.Vertex[ModelData.Model[modelIndex].VertexOffset + m].indexInVNList;
					pModel->vertex[m].normal = VertexNormalData.NormalList[VertexNormalDataIndex];

					//uv
					UINT VertexUVDataIndex = VertexData.Vertex[ModelData.Model[modelIndex].VertexOffset + m].indexInUVList;
					pModel->vertex[m].su = UVData.UVList[VertexUVDataIndex].su;
					pModel->vertex[m].sv = UVData.UVList[VertexUVDataIndex].sv;
					pModel->vertex[m].sw = UVData.UVList[VertexUVDataIndex].sw;
					pModel->vertex[m].sz = UVData.UVList[VertexUVDataIndex].sz;

					pModel->vertex[m].diffuse =  D3DCOLOR_RGBA((UINT)(pModel->shader->d3dmtrl.Diffuse.r*255),
						(UINT)(pModel->shader->d3dmtrl.Diffuse.g*255),
						(UINT)(pModel->shader->d3dmtrl.Diffuse.b*255),
						alpha);
				}
			}

			//face count
			fread(&(ModelData.Model[modelIndex].FaceCnt), sizeof(ModelData.Model[modelIndex].FaceCnt), 1, pFile);

			pModel->faceCnt = ModelData.Model[modelIndex].FaceCnt;	
			if (pModel->refModelIndex == -1)
				pModel->face = new CFace[pModel->faceCnt];

			//face list
			ModelData.Model[modelIndex].Face = new _Face[ModelData.Model[modelIndex].FaceCnt];

			for (UINT n=0; n< ModelData.Model[modelIndex].FaceCnt; n++)
			{
				//a,b,c
				fread(&(ModelData.Model[modelIndex].Face[n].a), sizeof(ModelData.Model[modelIndex].Face[n].a), 1, pFile);
				fread(&(ModelData.Model[modelIndex].Face[n].b), sizeof(ModelData.Model[modelIndex].Face[n].b), 1, pFile);
				fread(&(ModelData.Model[modelIndex].Face[n].c), sizeof(ModelData.Model[modelIndex].Face[n].c), 1, pFile);
				
				if (pModel->refModelIndex == -1)
				{
					pModel->face[n].index = n;
					pModel->face[n].visible = TRUE;
					UINT vertex_a = ModelData.Model[modelIndex].Face[n].a;
					pModel->face[n].vertices0 = &(pModel->vertex[vertex_a]);
					UINT vertex_b = ModelData.Model[modelIndex].Face[n].b;
					pModel->face[n].vertices1 = &(pModel->vertex[vertex_b]);
					UINT vertex_c = ModelData.Model[modelIndex].Face[n].c;
					pModel->face[n].vertices2 = &(pModel->vertex[vertex_c]);
						
					D3DXVECTOR3 ba = pModel->face[n].vertices0->position - pModel->face[n].vertices1->position;
					D3DXVECTOR3 ca = pModel->face[n].vertices0->position - pModel->face[n].vertices2->position;
					D3DXVec3Cross(&(pModel->face[n].normal), &ca, &ba);
					D3DXVec3Normalize(&(pModel->face[n].normal), &(pModel->face[n].normal));
				}

			}
			//release memory
			SAFE_DELETE_ARRAY(ModelData.Model[modelIndex].Face);

			//keyframe
			fread(&(ModelData.Model[modelIndex].KeyFrameCnt), sizeof(ModelData.Model[modelIndex].KeyFrameCnt), 1, pFile);

			//变换矩阵转化为相对上一帧的变换矩阵//--------------------------
			D3DXMATRIX mat;
			D3DXMatrixInverse(&mat, NULL, &(pModel->transform.matrix));

			pModel->kfrCnt = ModelData.Model[modelIndex].KeyFrameCnt;
			if (pModel->kfrCnt > 0) 
			{
				ModelData.Model[modelIndex].KfrInfo = new D3DXMATRIX[pModel->kfrCnt];
				pModel->matrix_kfr = new D3DXMATRIX[pModel->kfrCnt];
				//keyframe Data
				for (UINT k = 0; k< ModelData.Model[modelIndex].KeyFrameCnt; k++)
				{
					//frame data
					fread(&(ModelData.Model[modelIndex].KfrInfo[k]), sizeof(ModelData.Model[modelIndex].KfrInfo[k]), 1, pFile);
					pModel->matrix_kfr[k] = ModelData.Model[modelIndex].KfrInfo[k];
					
					//计算差值变换矩阵
					D3DXMatrixMultiply(&(pModel->matrix_CurFrame),	
										&mat,
										&pModel->matrix_CurFrame);

				}
				//release memory
				SAFE_DELETE_ARRAY(ModelData.Model[modelIndex].KfrInfo);
			}

			//vkeyframe
			fread(&(ModelData.Model[modelIndex].VKeyFrameCnt), sizeof(ModelData.Model[modelIndex].VKeyFrameCnt), 1, pFile);
			pModel ->vkfrCnt = ModelData.Model[modelIndex].VKeyFrameCnt;
			if (pModel->vkfrCnt > 0)
			{
				ModelData.Model[modelIndex].VertexPosList = new _VertexPosList[ModelData.Model[modelIndex].KeyFrameCnt];
			
				pModel->matrix_kfr = new D3DXMATRIX[pModel->kfrCnt];
				//vkeyframeData
				for (UINT k = 0; k< ModelData.Model[modelIndex].KeyFrameCnt; k++)
				{
					ModelData.Model[modelIndex].VertexPosList[k].PosList = new D3DXVECTOR3[ModelData.Model[modelIndex].VertexCnt];
					
					pModel->vertices_kfr[k] = new CVertex[pModel->vertexCnt];
					for (UINT z=0; z< ModelData.Model[modelIndex].VertexCnt; z++)
					{
						fread(&(ModelData.Model[modelIndex].VertexPosList[k].PosList[z]), sizeof(ModelData.Model[modelIndex].VertexPosList[k].PosList[z]), 1, pFile);
						pModel->vertices_kfr[k][z].position = ModelData.Model[modelIndex].VertexPosList[k].PosList[z];	
					}
				}	
				//release memory
				SAFE_DELETE_ARRAY(ModelData.Model[modelIndex].VertexPosList);
			}

			//确定当前模型所使用的材质，确定模型的FVF
			pModel->m_FVF = pModel->shader->m_FVF;
			pModel->m_FVFSize = pModel->shader->m_FVFSize;

			//确定模型是否是Billboard模型
			fread(&(ModelData.Model[modelIndex].IsBillboard), sizeof(ModelData.Model[modelIndex].IsBillboard), 1, pFile);
			pModel->isBillboard = ModelData.Model[modelIndex].IsBillboard;

			//确定模型是否是Collision模型
			fread(&(ModelData.Model[modelIndex].IsCollision), sizeof(ModelData.Model[modelIndex].IsCollision), 1, pFile);
			pModel->isCollision = ModelData.Model[modelIndex].IsCollision;

			//确定模型是否是Ground模型
			fread(&(ModelData.Model[modelIndex].IsGround), sizeof(ModelData.Model[modelIndex].IsGround), 1, pFile);
			pModel->isGround = ModelData.Model[modelIndex].IsGround;
		
			//确定模型是否是SkyBox
			fread(&(ModelData.Model[modelIndex].IsSkyBox), sizeof(ModelData.Model[modelIndex].IsSkyBox), 1, pFile);
			pModel->isSkyBox = ModelData.Model[modelIndex].IsSkyBox;

			//确定模型是否可见
			fread(&(ModelData.Model[modelIndex].IsVisible), sizeof(ModelData.Model[modelIndex].IsVisible), 1, pFile);
			pModel->visible = ModelData.Model[modelIndex].IsVisible;
	
			//确定模型是否是Clickable
			fread(&(ModelData.Model[modelIndex].IsClickable), sizeof(ModelData.Model[modelIndex].IsClickable), 1, pFile);
			pModel->clickable = ModelData.Model[modelIndex].IsClickable;
			
			//create vBuffer
			pScene->CreateModelVBuffer(pModel);
			//create IBuffer
			pScene->CreateModelIBuffer(pModel);	
		}	
		
		//将模型放到对应的材质modelList
		pModel->shader->modelList.push_back((DWORD)pModel);

		//push model
		pScene->model.push_back(pModel);

		//push model - transparent model半透明模型
		// if (pModel->shader->bAlphaBlend)
		if (pModel->shader->bAlphaBlend || pModel->opacity < 100) // fuxb 2007-11-15
			pScene->transModel.push_back(pModel);
	}

	//release memory
	SAFE_DELETE_ARRAY(ModelData.Model);
	SAFE_DELETE_ARRAY(VertexData.Vertex);
	SAFE_DELETE_ARRAY(VertexPosData.PosList);
	SAFE_DELETE_ARRAY(UVData.UVList);
	*/
}

//-------------------------------------------------------------------
//方法: ReadCameraData()
//参数: 
//用途: 读取摄像机信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D032C
VOID CGetE3DFileInfo::ReadCameraData()
{
	/*
	fseek(pFile, Header.FileTable.CameraDataPtr, SEEK_SET);

	//得到摄像机数量
	fread(&(CameraData.CameraCnt), sizeof(CameraData.CameraCnt), 1, pFile);
	
	CameraData.Camera = new _Camera[CameraData.CameraCnt];
	//逐个读取摄像机信息
	for (UINT i =0; i< CameraData.CameraCnt; i++)
	{
		//打开当前摄像机的基本信息
		fread(&(CameraData.Camera[i].Name), sizeof(CameraData.Camera[i].Name), 1, pFile);
		fread(&(CameraData.Camera[i].Info.From), sizeof(CameraData.Camera[i].Info.From), 1, pFile);
		fread(&(CameraData.Camera[i].Info.At), sizeof(CameraData.Camera[i].Info.At), 1, pFile);
		fread(&(CameraData.Camera[i].Info.Roll), sizeof(CameraData.Camera[i].Info.Roll), 1, pFile);
		fread(&(CameraData.Camera[i].Info.Fov), sizeof(CameraData.Camera[i].Info.Fov), 1, pFile);
		fread(&(CameraData.Camera[i].Info.NearClip), sizeof(CameraData.Camera[i].Info.NearClip), 1, pFile);
		fread(&(CameraData.Camera[i].Info.FarClip), sizeof(CameraData.Camera[i].Info.FarClip), 1, pFile);

		CCamera* pCamera = new CCamera();
		
		pCamera->index = i;

		//当前摄像机的基本信息
		strcpy(pCamera->name, CameraData.Camera[i].Name);
		pCamera->SetFrom(&CameraData.Camera[i].Info.From);
		pCamera->SetAt(&CameraData.Camera[i].Info.At);
		pCamera->SetFov(CameraData.Camera[i].Info.Fov * STOA);
		pCamera->SetRoll(CameraData.Camera[i].Info.Roll);
		pCamera->nearClip = CameraData.Camera[i].Info.NearClip;
		pCamera->farClip = CameraData.Camera[i].Info.FarClip;

		//set for transform
		pCamera->InitMatrix();
		
		//当前摄像机的关键帧信息
		fread(&(CameraData.Camera[i].KeyFrameCnt), sizeof(CameraData.Camera[i].KeyFrameCnt), 1, pFile);

		pCamera->kfrCnt = CameraData.Camera[i].KeyFrameCnt;
		
		if (CameraData.Camera[i].KeyFrameCnt > 1)
		{
			CameraData.Camera[i].KFRInfo = new _CameraKFRInfo[CameraData.Camera[i].KeyFrameCnt];
			
			pCamera->kfrInfo = new CAMINFO[CameraData.Camera[i].KeyFrameCnt];
			//逐帧保存摄像机的信息
			for (UINT j =0; j< CameraData.Camera[i].KeyFrameCnt; j++)
			{
				fread(&(CameraData.Camera[i].KFRInfo[j].From), sizeof(CameraData.Camera[i].KFRInfo[j].From), 1, pFile);
				fread(&(CameraData.Camera[i].KFRInfo[j].At), sizeof(CameraData.Camera[i].KFRInfo[j].At), 1, pFile);
				fread(&(CameraData.Camera[i].KFRInfo[j].Roll), sizeof(CameraData.Camera[i].KFRInfo[j].Roll), 1, pFile);
				
				pCamera->kfrInfo[j].From = CameraData.Camera[i].KFRInfo[j].From;
				pCamera->kfrInfo[j].At = CameraData.Camera[i].KFRInfo[j].At;
				pCamera->kfrInfo[j].Rol = CameraData.Camera[i].KFRInfo[j].Roll;
			}
			
			//release memory
			SAFE_DELETE_ARRAY(CameraData.Camera[i].KFRInfo);
		}
		pScene->camera.push_back(pCamera);
	}
	
	//release memory
	SAFE_DELETE_ARRAY(CameraData.Camera);
	*/
}

//-------------------------------------------------------------------
//方法: ReadTexData()
//参数: 
//用途: 读取贴图信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D03C8
VOID CGetE3DFileInfo::ReadTexData()
{/*
	fseek(pFile, Header.FileTable.TexDataPtr, SEEK_SET);

	//texture count
	fread(&(TexData.TexCnt), sizeof(TexData.TexCnt), 1, pFile);

	CHAR errInfo[256];
	//texture data
	TexData.Textures= new _Texture[TexData.TexCnt];
	for (UINT i= 0; i< TexData.TexCnt; i++)
	{
		CTexture* pTex = new CTexture();
		pTex->index = i;

		//读入贴图文件
		fread(&(TexData.Textures[i].Name), sizeof(TexData.Textures[i].Name), 1, pFile);
		fread(&(TexData.Textures[i].DataLength), sizeof(TexData.Textures[i].DataLength), 1, pFile);
		fread(&(TexData.Textures[i].Width), sizeof(TexData.Textures[i].Width), 1, pFile);
		fread(&(TexData.Textures[i].Height), sizeof(TexData.Textures[i].Height), 1, pFile);
		fread(&(TexData.Textures[i].MipCount), sizeof(TexData.Textures[i].MipCount), 1, pFile);
		fread(&(TexData.Textures[i].Format), sizeof(TexData.Textures[i].Format), 1, pFile);

		strcpy(pTex->name, TexData.Textures[i].Name);

		pTex->dataSize = TexData.Textures[i].DataLength;
		if (pTex->dataSize > 0)
		{
			pTex->data = new BYTE[pTex->dataSize];
			fread(pTex->data, pTex->dataSize, 1, pFile);
			if (pTex->data == NULL)
			{
				strcpy(errInfo, "读取贴图文件:");
				strcat(errInfo, pTex->pathAndName);
				strcat(errInfo, "不明原因失败");
				MessageBox(NULL, errInfo, "读取贴图文件...错误", MB_OK | MB_ICONERROR);
				ZeroMemory(errInfo, sizeof(errInfo) );
			}
			pTex->width = TexData.Textures[i].Width;
			pTex->height = TexData.Textures[i].Height;
			pTex->BP = (D3DFORMAT)TexData.Textures[i].Format;
			
			if (pTex->BP == D3DFMT_A4R4G4B4 || 
				pTex->BP == D3DFMT_A8R8G8B8 ||
				pTex->BP == D3DFMT_A1R5G5B5)
				pTex->bAlphaBlend = TRUE;

			pTex->MipCount = TexData.Textures[i].MipCount;
		}

		//贴图不在这里创建了，因为有可能有两张贴图组合成的带Alpha通道的贴图，
		//所以在读材质信息的时候创建
		pScene->texture.push_back(pTex);
	}
	//release memory
	SAFE_DELETE_ARRAY(TexData.Textures);
	*/
}

//-------------------------------------------------------------------
//方法: ReadMaterialData()
//参数: 
//用途: 读取材质信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D033C
VOID CGetE3DFileInfo::ReadMaterialData()
{
	/*
	fseek(pFile, Header.FileTable.MaterialDataPtr, SEEK_SET);

	fread(&(MaterialData.MaterialCnt), sizeof(MaterialData.MaterialCnt), 1, pFile);

	MaterialData.Material = new _Material[MaterialData.MaterialCnt];
	//得到材质列表
	for (UINT m =0; m< MaterialData.MaterialCnt; m++)
	{
		CMaterial* pMtrl = new CMaterial();
		pMtrl->index = m;
		
		//读入材质数据
		fread(&(MaterialData.Material[m]), sizeof(MaterialData.Material[m]), 1, pFile); 

		strcpy( pMtrl->name, MaterialData.Material[m].Name);
		//mtrl->dif 
		pMtrl->d3dmtrl.Diffuse.a = 1.0f;
		pMtrl->d3dmtrl.Diffuse.r = ((MaterialData.Material[m].Dif_Col>>16)&0xff)/255.0f;
		pMtrl->d3dmtrl.Diffuse.g = ((MaterialData.Material[m].Dif_Col>>8)&0xff)/255.0f;
		pMtrl->d3dmtrl.Diffuse.b = ((MaterialData.Material[m].Dif_Col)&0xff)/255.0f;
		//mtrl->amb
		pMtrl->d3dmtrl.Ambient.a = 1.0f;
		pMtrl->d3dmtrl.Ambient.r = ((MaterialData.Material[m].Amb_Col>>16)&0xff)/255.0f;
		pMtrl->d3dmtrl.Ambient.g = ((MaterialData.Material[m].Amb_Col>>8)&0xff)/255.0f;
		pMtrl->d3dmtrl.Ambient.b = ((MaterialData.Material[m].Amb_Col)&0xff)/255.0f;
		//mtrl->spe
		pMtrl->specular.r = ((MaterialData.Material[m].Spe_Col>>16)&0xff)/255.0f;
		pMtrl->specular.g = ((MaterialData.Material[m].Spe_Col>>8)&0xff)/255.0f;
		pMtrl->specular.b = ((MaterialData.Material[m].Spe_Col)&0xff)/255.0f;		
		pMtrl->specular.a = 1.0f;
		pMtrl->d3dmtrl.Specular = MaterialData.Material[m].SpeLevel * pMtrl->specular;
		pMtrl->d3dmtrl.Specular.a = 1.0f;	
		pMtrl->specularLevel = MaterialData.Material[m].SpeLevel;

		//mtrl->emi
		pMtrl->d3dmtrl.Emissive.a = 1.0f;
		pMtrl->d3dmtrl.Emissive.r = (MaterialData.Material[m].Illumination)/100.0f;
		pMtrl->d3dmtrl.Emissive.g = (MaterialData.Material[m].Illumination)/100.0f;
		pMtrl->d3dmtrl.Emissive.b = (MaterialData.Material[m].Illumination)/100.0f;
		//power
		pMtrl->d3dmtrl.Power = MaterialData.Material[m].Power;	//高光锐度

		pMtrl->type = D3DSHADE_GOURAUD;		
		
		pMtrl->opacity = MaterialData.Material[m].Opacity;				//透明度
		if (pMtrl->opacity < 100)
			pMtrl->bAlphaBlend = TRUE;

		pMtrl->twoSide = MaterialData.Material[m].TwoSide;

		pMtrl->bAlphaTest = MaterialData.Material[m].AlphaTest;

		pMtrl->AlphaTestValue = MaterialData.Material[m].AlphaTestValue;  // fuxb 2007-10-14

		//diffuse tex
		if (MaterialData.Material[m].DifTexID != -1)
		{
			pMtrl->diffuseTexIndex = MaterialData.Material[m].DifTexID;
			pMtrl->diffuseTex = pScene->texture[MaterialData.Material[m].DifTexID];
		}

		//opacity tex
		if (MaterialData.Material[m].OpaTexID != -1)
		{
			pMtrl->opacityTexIndex = MaterialData.Material[m].OpaTexID;
			pMtrl->opacityTex = pScene->texture[MaterialData.Material[m].OpaTexID];
		}

		//lightmap tex
		if (MaterialData.Material[m].LightTexID != -1)
		{
			pMtrl->lightTexIndex = MaterialData.Material[m].LightTexID;
			pMtrl->lightTex = pScene->texture[MaterialData.Material[m].LightTexID];
		}

		//specular tex
		if (MaterialData.Material[m].SpecularTexID != -1)
		{
			pMtrl->specularTexIndex = MaterialData.Material[m].SpecularTexID;
			pMtrl->specularTex = pScene->texture[MaterialData.Material[m].SpecularTexID];
		}

		//bumpmap tex
		if (MaterialData.Material[m].BumpTexID != -1)
		{
			pMtrl->bumpTexIndex = MaterialData.Material[m].BumpTexID;
			pMtrl->bumpTex = pScene->texture[MaterialData.Material[m].BumpTexID];
		}

		//CubeMapID tex
		if (MaterialData.Material[m].CubeMapID != -1)
		{
			pMtrl->cubeMapIndex = MaterialData.Material[m].CubeMapID;
			pMtrl->pCubeMap = pScene->cubeMap[pMtrl->cubeMapIndex];	
			
			//cubeMap的强度
			pMtrl->cubeMapPower = MaterialData.Material[m].CubeMapPower;
		}

		//SpecularCubeMap tex
		if (MaterialData.Material[m].SpecularCubeMapID != -1)
		{
			pMtrl->specularCubeMapIndex = MaterialData.Material[m].SpecularCubeMapID;
			pMtrl->pSpecularCubeMap = pScene->cubeMap[pMtrl->specularCubeMapIndex];	
		}

		//MovieTexID tex
		if (MaterialData.Material[m].MovieTexID != -1)
		{
			pMtrl->movieTexIndex = MaterialData.Material[m].MovieTexID;
			pMtrl->pMovieTexture = (DWORD)(pScene->movieTexture[pMtrl->movieTexIndex]);
			
			CMovieTexture* pMovieTex = (CMovieTexture*)pMtrl->pMovieTexture;

			//如果还没有创建
			if (pMovieTex->m_pMtrl == NULL)
			{
				//初始化电影贴图对象
				CHAR strFilePath[256];
				GetFilePath(strFilePath, m_strFileName);
				strcat(strFilePath, pMovieTex->fileName);
				pMovieTex->Create(strFilePath, 
										pMovieTex->m_mediaType, 
										pMovieTex->m_withAudio, 
										pMovieTex->m_repeatCnt,
										(DWORD)pMtrl);
			}

		}

		//bumpWater tex
		if (MaterialData.Material[m].BumpWaterID != -1)
		{
			pMtrl->bumpWaterIndex = MaterialData.Material[m].BumpWaterID;
			pMtrl->pBumpWater = pScene->bumpWater[pMtrl->bumpWaterIndex];
		}

		//DyTexID tex
		if (MaterialData.Material[m].DyTexID != -1)
		{
			pMtrl->dyTexIndex = MaterialData.Material[m].DyTexID;
		}


		//---------------------材质的FVF---------------------------
		pMtrl->m_FVF = MaterialData.Material[m].FVF;
		pMtrl->m_FVFSize = MaterialData.Material[m].FVF_SIZE;

		//self illumination 自发光值
		pMtrl->selfIllumination = MaterialData.Material[m].Illumination;
	
		//如果相应编号的贴图还没有创建，则为材质创建该编号的贴图
		if(pMtrl->diffuseTex != NULL )
		{
			//看看该贴图是否已经创建
			if (pMtrl->diffuseTex->textureBuf == NULL)
			{
				if ( FAILED(pScene->CreateTexFromBuffer(pMtrl->diffuseTex, pMtrl->diffuseTex->MipCount)) )
				{
					CHAR strErr[32];
					itoa(pMtrl->diffuseTex->index, strErr, 10);
					MessageBox(NULL, strErr, "创建贴图错误", MB_OK | MB_ICONERROR);
				}
				//SAFE_DELETE_ARRAY(pMtrl->diffuseTex->data);
			}
		}

		//如果有透明贴图，则创建贴图
		if(pMtrl->opacityTex != NULL )
		{
			//先看看透明通道贴图是否已经创建
			if (pMtrl->opacityTex->textureBuf == NULL)
			{
				pMtrl->opacityTex->width = pMtrl->diffuseTex->width;
				pMtrl->opacityTex->height = pMtrl->diffuseTex->height;
				if ( FAILED(pScene->CreateTexFromBuffer(pMtrl->opacityTex, pMtrl->opacityTex->MipCount)) )
				{
					CHAR strErr[32];
					itoa(pMtrl->diffuseTex->index, strErr, 10);
					MessageBox(NULL, strErr, "创建贴图错误", MB_OK | MB_ICONERROR);
				}
				//SAFE_DELETE_ARRAY(pMtrl->opacityTex->data);
			}

			//新建comp合成贴图
			pMtrl->compTex = new CTexture();
			//设置合成后的贴图的格式
			if (pMtrl->diffuseTex->BP == D3DFMT_R8G8B8 || pMtrl->opacityTex->BP == D3DFMT_R8G8B8)
				pMtrl->compTex->BP = D3DFMT_A8R8G8B8;
			else
				pMtrl->compTex->BP = D3DFMT_A4R4G4B4;

			pMtrl->compTex->width = pMtrl->diffuseTex->width;
			pMtrl->compTex->height = pMtrl->diffuseTex->height;
			pMtrl->compTex->bAlphaBlend = TRUE;

			UINT bufSize;
			if (pMtrl->compTex->BP == D3DFMT_A8R8G8B8)
				bufSize = pMtrl->compTex->width * pMtrl->compTex->height * 4;
			else
				bufSize = pMtrl->compTex->width * pMtrl->compTex->height * 2;
			pMtrl->compTex->buffer = new BYTE[bufSize];

			//合成dif通道和opa通道的贴图
			if(FAILED(pScene->CompositeAlphaMtrl(pMtrl)))
				MessageBox(NULL, pMtrl->name, "透明贴图合成失败", MB_OK|MB_ICONERROR);	

			pMtrl->bAlphaBlend = TRUE;
		}

		//创建光贴图
		if(pMtrl->lightTex != NULL )
		{
			if (pMtrl->lightTex->textureBuf == NULL)
			{
				if ( FAILED(pScene->CreateTexFromBuffer(pMtrl->lightTex, 
					pMtrl->lightTex->MipCount)) )
				{
					CHAR strErr[32];
					itoa(pMtrl->lightTex->index, strErr, 10);
					MessageBox(NULL, strErr, "创建贴图错误", MB_OK | MB_ICONERROR);
				}
				//SAFE_DELETE_ARRAY(pMtrl->reflectionTex->data);
			}
		}

		//push material
		pScene->shader.push_back(pMtrl);
	}
	
	//release memory
	SAFE_DELETE_ARRAY(MaterialData.Material);

	//release memory
	SAFE_DELETE_ARRAY(CubeMapData.CubeMap);
	*/
}

//-------------------------------------------------------------------
//方法: ReadLightData()
//参数: 
//用途: 读入灯光信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D033D
VOID CGetE3DFileInfo::ReadLightData()
{
	/*
	fseek(pFile, Header.FileTable.LightDataPtr, SEEK_SET);

	//读入灯光数量
	fread(&(LightData.LightCnt), sizeof(LightData.LightCnt), 1, pFile);

	//读入灯光详细数据
	LightData.Lights = new _Light[LightData.LightCnt];
	for (UINT i = 0; i< LightData.LightCnt; i++)
	{
		//读入E3D文件
		fread(&(LightData.Lights[i].Name), sizeof(LightData.Lights[i].Name), 1, pFile);
		fread(&(LightData.Lights[i].Type), sizeof(LightData.Lights[i].Type), 1, pFile);
		fread(&(LightData.Lights[i].Pos), sizeof(LightData.Lights[i].Pos), 1, pFile);
		fread(&(LightData.Lights[i].Dir), sizeof(LightData.Lights[i].Dir), 1, pFile);
		fread(&(LightData.Lights[i].Diffuse), sizeof(LightData.Lights[i].Diffuse), 1, pFile);
		fread(&(LightData.Lights[i].Ambient), sizeof(LightData.Lights[i].Ambient), 1, pFile);
		fread(&(LightData.Lights[i].Multiply), sizeof(LightData.Lights[i].Multiply), 1, pFile);
		fread(&(LightData.Lights[i].Range), sizeof(LightData.Lights[i].Range), 1, pFile);
		fread(&(LightData.Lights[i].Theta), sizeof(LightData.Lights[i].Theta), 1, pFile);
		fread(&(LightData.Lights[i].Phi), sizeof(LightData.Lights[i].Phi), 1, pFile);
		fread(&(LightData.Lights[i].Attenuation0), sizeof(LightData.Lights[i].Attenuation0), 1, pFile);
		fread(&(LightData.Lights[i].Attenuation1), sizeof(LightData.Lights[i].Attenuation1), 1, pFile);
		fread(&(LightData.Lights[i].Attenuation2), sizeof(LightData.Lights[i].Attenuation2), 1, pFile);
		fread(&(LightData.Lights[i].NumFrame), sizeof(LightData.Lights[i].NumFrame), 1, pFile);

		CLight* pLight = new CLight();
		//灯光类型,不同的灯光类型必须设定不同的参数--------------------------
		strcpy(pLight->name, LightData.Lights[i].Name);
		pLight->param.Type = (_D3DLIGHTTYPE)LightData.Lights[i].Type;
		//强度
		pLight->multiply = LightData.Lights[i].Multiply;
		//颜色
		pLight->diffuse = LightData.Lights[i].Diffuse;
		pLight->ambient = LightData.Lights[i].Ambient;

		pLight->param.Diffuse.r = pLight->diffuse.r * pLight->multiply;
		pLight->param.Diffuse.g = pLight->diffuse.g * pLight->multiply;
		pLight->param.Diffuse.b = pLight->diffuse.b * pLight->multiply;
		pLight->param.Diffuse.a = 1.0f;
		//环境光，用它来近似模拟全局光
		pLight->param.Ambient.r = pLight->ambient.r * pLight->multiply;
		pLight->param.Ambient.g = pLight->ambient.g * pLight->multiply;
		pLight->param.Ambient.b = pLight->ambient.b * pLight->multiply;
		pLight->param.Ambient.a = 1.0f;

		switch (pLight->param.Type)
		{
		case D3DLIGHT_POINT:
			//灯光的位置
			pLight->param.Position.x = LightData.Lights[i].Pos.x;
			pLight->param.Position.y = LightData.Lights[i].Pos.y;
			pLight->param.Position.z = LightData.Lights[i].Pos.z;
			//光线到达的最远距离
			//光线到达的最远距离
			pLight->param.Range = LightData.Lights[i].Range;
			pLight->param.Attenuation0 = LightData.Lights[i].Attenuation0;
			pLight->param.Attenuation1 = LightData.Lights[i].Attenuation1;
			pLight->param.Attenuation2 = LightData.Lights[i].Attenuation2;
			break;
		case D3DLIGHT_DIRECTIONAL:
			//方向
			pLight->param.Direction.x = LightData.Lights[i].Dir.x;
			pLight->param.Direction.y = LightData.Lights[i].Dir.y;
			pLight->param.Direction.z = LightData.Lights[i].Dir.z;
			break;
		case D3DLIGHT_SPOT:
			//灯光的位置
			pLight->param.Position.x = LightData.Lights[i].Pos.x;
			pLight->param.Position.y = LightData.Lights[i].Pos.y;
			pLight->param.Position.z = LightData.Lights[i].Pos.z;
			//方向
			pLight->param.Direction.x = LightData.Lights[i].Dir.x;
			pLight->param.Direction.y = LightData.Lights[i].Dir.y;
			pLight->param.Direction.z = LightData.Lights[i].Dir.z;
			//光线到达的最远距离
			pLight->param.Range = LightData.Lights[i].Range;
			//如果是spot，
			pLight->param.Theta = LightData.Lights[i].Theta;
			pLight->param.Phi = LightData.Lights[i].Phi;
			break;

		}

		//关键帧动画信息
		pLight->playKfr = TRUE;
		pLight->isPlaying = TRUE;
		pLight->kfrCnt = LightData.Lights[i].NumFrame;
		if (pLight->kfrCnt > 1)
		{
			LightData.Lights[i].kfrInfo = new _LightKfrInfo[pLight->kfrCnt];
			pLight->kfrInfo = new _DLightInfo[pLight->kfrCnt];
			for (UINT j = 0; j< pLight->kfrCnt; j++)
			{
				fread(&(LightData.Lights[i].kfrInfo[j]), sizeof(LightData.Lights[i].kfrInfo[j]), 1, pFile);
				pLight->kfrInfo[j].Pos = LightData.Lights[i].kfrInfo[j].Pos;
				pLight->kfrInfo[j].Dir = LightData.Lights[i].kfrInfo[j].Dir;
				pLight->kfrInfo[j].Diffuse = LightData.Lights[i].kfrInfo[j].Diffuse;
			}	
			SAFE_DELETE_ARRAY(LightData.Lights[i].kfrInfo);
		}
		
		pLight->index = i;
		pScene->light.push_back(pLight);	
		pScene->m_pd3dDevice->SetLight(i, &pLight->param);
		pScene->m_pd3dDevice->LightEnable(i, TRUE);
	}
	SAFE_DELETE_ARRAY(LightData.Lights);
	*/
}

//-------------------------------------------------------------------
//方法: ReadTextData()
//参数: 
//用途: 读取文字信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D03A9
VOID CGetE3DFileInfo::ReadTextData()
{
	/*
	fseek(pFile, Header.FileTable.TextDataPtr, SEEK_SET);

	//读入文字数量
	fread(&(TextData.TextCnt), sizeof(TextData.TextCnt), 1, pFile);

	//如果文字数量为0，则忽略保存文字内容列表字段
	if (TextData.TextCnt == 0) return;

	//text data
	TextData.Texts = new _Text[TextData.TextCnt];
	for (UINT i= 0; i< TextData.TextCnt; i++)
	{
		CText* pText = new CText("System");

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
//##ModelId=47EBC74D03B9
VOID CGetE3DFileInfo::ReadOverlayData()
{
	fseek(pFile, Header.FileTable.OverlayDataPtr, SEEK_SET);

	//读入Overlay数量
	fread(&(OverlayData.OverlayCnt), sizeof(OverlayData.OverlayCnt), 1, pFile);

	//如果Overlay的数量为0,则忽略保存Ovelay内容列表字段
	if (OverlayData.OverlayCnt == 0) return;

	//overlay data
	OverlayData.Overlays = new _Overlay[OverlayData.OverlayCnt];
	for (UINT i= 0; i< OverlayData.OverlayCnt; i++)
	{
		COverlay* pOverlay = new COverlay(pScene->m_pd3dDevice);
		
		pOverlay->index = i;

		//读入Overlay的名字
		fread(&(OverlayData.Overlays[i].Name), sizeof(OverlayData.Overlays[i].Name), 1, pFile);
		strcpy(pOverlay->name, OverlayData.Overlays[i].Name);

		//读入Overlay的对齐模式
		fread(&(OverlayData.Overlays[i].AlignMode), sizeof(OverlayData.Overlays[i].AlignMode), 1, pFile);
		pOverlay->alignMode = OverlayData.Overlays[i].AlignMode;

		//读入Overlay Rect
		if (Header.E3DVer == 2.5f)		//采用绝对坐标
		{
			RECT rect1;
			fread(&rect1, sizeof(rect1), 1, pFile);
			OverlayData.Overlays[i].Rect.left_xPosPercent = rect1.left;
			OverlayData.Overlays[i].Rect.top_yPosPercent = rect1.top;
			OverlayData.Overlays[i].Rect.right_widthPercent = rect1.right;
			OverlayData.Overlays[i].Rect.bottom_heightPercent = rect1.bottom;
			pOverlay->SetRect(&rect1);
			
			pOverlay->useRelativeSize = FALSE;
		}

		if (Header.E3DVer == 3.0f)		//采用相对坐标
		{
			fread(&(OverlayData.Overlays[i].Rect), sizeof(OverlayData.Overlays[i].Rect), 1, pFile);
			
			//计算相对尺寸
			pOverlay->m_x1Percent = OverlayData.Overlays[i].Rect.left_xPosPercent;
			pOverlay->m_y1Percent = OverlayData.Overlays[i].Rect.top_yPosPercent;
			pOverlay->m_x2Percent = pOverlay->m_x1Percent + OverlayData.Overlays[i].Rect.right_widthPercent;
			pOverlay->m_y2Percent = pOverlay->m_y1Percent + OverlayData.Overlays[i].Rect.bottom_heightPercent;

			pOverlay->useRelativeSize = TRUE;
			pOverlay->UpdateRect(&pScene->rect);
		}

		//读入Overlay MaterialID
		fread(&(OverlayData.Overlays[i].MaterialID), sizeof(OverlayData.Overlays[i].MaterialID), 1, pFile);
		if (OverlayData.Overlays[i].MaterialID != -1)
		{
			UINT index = OverlayData.Overlays[i].MaterialID;
			pOverlay->m_pMtrl = pScene->m_vShader[index];
		}

		pScene->overlay.push_back(pOverlay);
	}

	//release memory
	SAFE_DELETE_ARRAY(OverlayData.Overlays);
}

//-------------------------------------------------------------------
//方法: ReadNavigaterData()
//参数: 
//用途: 读取Navigate信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D03BA
VOID CGetE3DFileInfo::ReadNavigaterData()
{
	fseek(pFile, Header.FileTable.NavigaterDataPtr, SEEK_SET);

	//navigate count
	fread(&(NavigaterData.NavigaterCnt), sizeof(NavigaterData.NavigaterCnt), 1, pFile);
	
	//如果navigater的数量为0,则忽略保存navigater内容列表字段
	if (NavigaterData.NavigaterCnt == 0) return;

	//navigate data
	NavigaterData.Navigaters = new _Navigater[NavigaterData.NavigaterCnt];
	for (UINT i= 0; i< NavigaterData.NavigaterCnt; i++)
	{
		CNavigater* pNavigater = new CNavigater();
		if (pNavigater == NULL) return;
		
		//读入pNavigater的名字
		fread(&(NavigaterData.Navigaters[i].Name), sizeof(NavigaterData.Navigaters[i].Name), 1, pFile);
		strcpy(pNavigater->name, NavigaterData.Navigaters[i].Name);
	
		//读入navigate的mapOverlay的Index
		fread(&(NavigaterData.Navigaters[i].MapOverlayIndex), sizeof(NavigaterData.Navigaters[i].MapOverlayIndex), 1, pFile);
		INT mapOverlayIndex = NavigaterData.Navigaters[i].MapOverlayIndex;
		pNavigater->pNavigateMap = pScene->overlay[mapOverlayIndex];
		
		//读入navigate的pointerOverlay的Index
		fread(&(NavigaterData.Navigaters[i].PointerOverlayIndex), sizeof(NavigaterData.Navigaters[i].PointerOverlayIndex), 1, pFile);
		INT pointerOverlayIndex = NavigaterData.Navigaters[i].PointerOverlayIndex;
		COverlay* pPointerOverlay = pScene->overlay[pointerOverlayIndex];
	
		//读入navigate的walker camera Index
		fread(&(NavigaterData.Navigaters[i].CameraIndex), sizeof(NavigaterData.Navigaters[i].CameraIndex), 1, pFile);
		INT CameraIndex = NavigaterData.Navigaters[i].CameraIndex;

		//创建导航指针对象
		NavPointer* pNavPointer = new NavPointer;
		strcpy(pNavPointer->Name, "NavPointer1");
		pNavPointer->NavPointerType = 0;
		pNavPointer->NavPointerHandle = (DWORD)(pScene->camera[CameraIndex]);
		pNavPointer->pPointerOverlay = pPointerOverlay;
		
		//将导航指针加入导航图对象
		pNavigater->AddNavPointer(pNavPointer);

		//读入navigate的AlignMode
		fread(&(NavigaterData.Navigaters[i].AlignMode), sizeof(NavigaterData.Navigaters[i].AlignMode), 1, pFile);
		pNavigater->alignMode = NavigaterData.Navigaters[i].AlignMode;
		
		//读入navigate的navRect
		fread(&(NavigaterData.Navigaters[i].navRect), sizeof(NavigaterData.Navigaters[i].navRect), 1, pFile);
		pNavigater->navRect = NavigaterData.Navigaters[i].navRect;
		
		//读入navigate的sceneLeft
		fread(&(NavigaterData.Navigaters[i].sceneLeft), sizeof(NavigaterData.Navigaters[i].sceneLeft), 1, pFile);
		pNavigater->sceneLeft = NavigaterData.Navigaters[i].sceneLeft;

		//读入navigate的sceneTop
		fread(&(NavigaterData.Navigaters[i].sceneTop), sizeof(NavigaterData.Navigaters[i].sceneTop), 1, pFile);
		pNavigater->sceneTop = NavigaterData.Navigaters[i].sceneTop;
								
		//读入navigate的sceneRight
		fread(&(NavigaterData.Navigaters[i].sceneRight), sizeof(NavigaterData.Navigaters[i].sceneRight), 1, pFile);
		pNavigater->sceneRight = NavigaterData.Navigaters[i].sceneRight;
													
		//读入navigate的sceneBottom
		fread(&(NavigaterData.Navigaters[i].sceneBottom), sizeof(NavigaterData.Navigaters[i].sceneBottom), 1, pFile);
		pNavigater->sceneBottom = NavigaterData.Navigaters[i].sceneBottom;
				
		//将pImage Push到image列表
		pScene->navigater.push_back(pNavigater);
	}

	//release memory
	SAFE_DELETE_ARRAY(NavigaterData.Navigaters);
}

//-------------------------------------------------------------------
//方法: ReadButtonData()
//参数: 
//用途: 读取Button信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D03BB
VOID CGetE3DFileInfo::ReadButtonData()
{
	fseek(pFile, Header.FileTable.ButtonDataPtr, SEEK_SET);

	//button count
	fread(&(ButtonData.ButtonCnt), sizeof(ButtonData.ButtonCnt), 1, pFile);

	//如果button的数量为0,则忽略保存button内容列表字段
	if (ButtonData.ButtonCnt == 0) return;

	//button data
	ButtonData.Buttons = new _Button[ButtonData.ButtonCnt];
	for (UINT i= 0; i< ButtonData.ButtonCnt; i++)
	{
		CButton* pButton = new CButton();
		if (pButton == NULL) return;
		
		//读入pbutton的名字
		fread(&(ButtonData.Buttons[i].Name), sizeof(ButtonData.Buttons[i].Name), 1, pFile);
		strcpy(pButton->name, ButtonData.Buttons[i].Name);
	
		//读入button的Overlay的Index
		fread(&(ButtonData.Buttons[i].BtnOverlayIndex), sizeof(ButtonData.Buttons[i].BtnOverlayIndex), 1, pFile);
		INT BtnOverlayIndex = ButtonData.Buttons[i].BtnOverlayIndex;
		pButton->btnOverlay = pScene->overlay[BtnOverlayIndex];
		pButton->pMtrl_MouseUp = pButton->btnOverlay->m_pMtrl;
		
		//读入button的MouseOverMtrl的Index
		fread(&(ButtonData.Buttons[i].MouseOverMtrlIndex), sizeof(ButtonData.Buttons[i].MouseOverMtrlIndex), 1, pFile);
		INT MouseOverMtrlIndex = ButtonData.Buttons[i].MouseOverMtrlIndex;
		pButton->pMtrl_MouseOver = pScene->m_vShader[MouseOverMtrlIndex];

		//读入button的MouseDownMtrl的Index
		fread(&(ButtonData.Buttons[i].MouseDownMtrlIndex), sizeof(ButtonData.Buttons[i].MouseDownMtrlIndex), 1, pFile);
		INT MouseDownMtrlIndex = ButtonData.Buttons[i].MouseDownMtrlIndex;
		pButton->pMtrl_MouseDown = pScene->m_vShader[MouseDownMtrlIndex];

		//读入button的AlignMode
		fread(&(ButtonData.Buttons[i].AlignMode), sizeof(ButtonData.Buttons[i].AlignMode), 1, pFile);
		pButton->alignMode = ButtonData.Buttons[i].AlignMode;
		pButton->btnOverlay->alignMode = ButtonData.Buttons[i].AlignMode;
			
		//读入Button Rect
		if (Header.E3DVer == 2.5f)		//采用绝对坐标
		{
			RECT rect1;
			fread(&rect1, sizeof(rect1), 1, pFile);
			pButton->btnOverlay->SetRect(&rect1);
			pButton->m_bUseImageSize = TRUE;
		}

		if (Header.E3DVer == 3.0f)		//采用相对坐标
		{
			fread(&(ButtonData.Buttons[i].ButtonRect), sizeof(ButtonData.Buttons[i].ButtonRect), 1, pFile);
			
			pButton->m_xPosPercent = ButtonData.Buttons[i].ButtonRect.left_xPosPercent;
			pButton->m_yPosPercent = ButtonData.Buttons[i].ButtonRect.top_yPosPercent;
			pButton->m_widthPercent = ButtonData.Buttons[i].ButtonRect.right_widthPercent;
			pButton->m_heightPercent = ButtonData.Buttons[i].ButtonRect.bottom_heightPercent;
			
			pButton->m_bUseImageSize = FALSE;
			//pButton->UpdateRect(&pScene->rect);
		}

		//读入button的CallBackFunc
		fread(&(ButtonData.Buttons[i].CallBackFunc), sizeof(ButtonData.Buttons[i].CallBackFunc), 1, pFile);
		strcpy(pButton->callBackFunc, ButtonData.Buttons[i].CallBackFunc);
				
		//将pButton Push到image列表
		pScene->button.push_back(pButton);
	}

	//release memory
	SAFE_DELETE_ARRAY(ButtonData.Buttons);

}

//-------------------------------------------------------------------
//方法: ReadImageData()
//参数: 
//用途: 读取Image信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D03C9
VOID CGetE3DFileInfo::ReadImageData()
{
	fseek(pFile, Header.FileTable.ImageDataPtr, SEEK_SET);

	//image count
	fread(&(ImageData.ImageCount), sizeof(ImageData.ImageCount), 1, pFile);
	
	//如果Image的数量为0,则忽略保存Image内容列表字段
	if (ImageData.ImageCount == 0) return;

	//image data
	ImageData.Images = new _Image[ImageData.ImageCount];
	for (UINT i= 0; i< ImageData.ImageCount; i++)
	{
		CImage* pImage = new CImage();
		if (pImage == NULL) return;
		
		//读入Image的名字
		fread(&(ImageData.Images[i].Name), sizeof(ImageData.Images[i].Name), 1, pFile);
		strcpy(pImage->name, ImageData.Images[i].Name);

		//Image的Index
		pImage->index = i;

		//读入Image的数据长度
		fread(&(ImageData.Images[i].DataLength), sizeof(ImageData.Images[i].DataLength), 1, pFile);
		pImage->dataSize = ImageData.Images[i].DataLength;

		//读入Image的宽度
		fread(&(ImageData.Images[i].Width), sizeof(ImageData.Images[i].Width), 1, pFile);
		pImage->width = ImageData.Images[i].Width;
		//读入Image的高度
		fread(&(ImageData.Images[i].Height), sizeof(ImageData.Images[i].Height), 1, pFile);
		pImage->height = ImageData.Images[i].Height;
		//读入Image的格式
		fread(&(ImageData.Images[i].Format), sizeof(ImageData.Images[i].Format), 1, pFile);
		pImage->format = ImageData.Images[i].Format;
		//读入Image的数据
		pImage->data = new BYTE[ImageData.Images[i].DataLength];
		if (pImage->data == NULL) return;
		fread(pImage->data, ImageData.Images[i].DataLength, 1, pFile);

		//将pImage Push到image列表
		pScene->image.push_back(pImage);
	}

	//release memory
	SAFE_DELETE_ARRAY(ImageData.Images);
}

//-------------------------------------------------------------------
//方法: ReadCubeMapData()
//参数: 
//用途: 读取CubeMap信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D03CA
VOID CGetE3DFileInfo::ReadCubeMapData()
{
	/*
	fseek(pFile, Header.FileTable.CubeMapDataPtr, SEEK_SET);

	//cubeMap count
	fread(&(CubeMapData.CubeMapCnt), sizeof(CubeMapData.CubeMapCnt), 1, pFile);
	
	//如果cubeMap的数量为0
	if (CubeMapData.CubeMapCnt == 0) return;

	//判断是否支持
	//if (!vgsRender->m_DeviceInfo->IsSupport_D3D_CUBEMAP)
	//	return;

	//cubeMap data
	CubeMapData.CubeMap = new _CubeMap[CubeMapData.CubeMapCnt];
	for (UINT i= 0; i< CubeMapData.CubeMapCnt; i++)
	{		
		//读入CubeMap的名字
		fread(&(CubeMapData.CubeMap[i].Name), sizeof(CubeMapData.CubeMap[i].Name), 1, pFile);
		
		//读入CubeMap的尺寸
		fread(&(CubeMapData.CubeMap[i].Length), sizeof(CubeMapData.CubeMap[i].Length), 1, pFile);

		//6个面的Texture
		fread(&(CubeMapData.CubeMap[i].Front_Z), sizeof(CubeMapData.CubeMap[i].Front_Z), 1, pFile);
		fread(&(CubeMapData.CubeMap[i].Back_NZ), sizeof(CubeMapData.CubeMap[i].Back_NZ), 1, pFile);
		fread(&(CubeMapData.CubeMap[i].Left_NX), sizeof(CubeMapData.CubeMap[i].Left_NX), 1, pFile);
		fread(&(CubeMapData.CubeMap[i].Right_X), sizeof(CubeMapData.CubeMap[i].Right_X), 1, pFile);
		fread(&(CubeMapData.CubeMap[i].Top_Y), sizeof(CubeMapData.CubeMap[i].Top_Y), 1, pFile);
		fread(&(CubeMapData.CubeMap[i].Bottom_NY), sizeof(CubeMapData.CubeMap[i].Bottom_NY), 1, pFile);

		CCubeMap* pCubeMap = new CCubeMap(pScene->m_pd3dDevice);
		strcpy(pCubeMap->name, CubeMapData.CubeMap[i].Name);
		pCubeMap->index = pScene->cubeMap.size();
		
		//6副贴图
		pCubeMap->m_pFrontTex = 
			pScene->texture[CubeMapData.CubeMap[i].Front_Z];
		if (pCubeMap->m_pFrontTex->textureBuf == NULL)
			pScene->CreateTexFromBuffer(pCubeMap->m_pFrontTex, 1);

		pCubeMap->m_pBackTex = 
			pScene->texture[CubeMapData.CubeMap[i].Back_NZ];
		if (pCubeMap->m_pBackTex->textureBuf == NULL)
		pScene->CreateTexFromBuffer(pCubeMap->m_pBackTex, 1);

		pCubeMap->m_pLeftTex = 
			pScene->texture[CubeMapData.CubeMap[i].Left_NX];
		if (pCubeMap->m_pLeftTex->textureBuf == NULL)
			pScene->CreateTexFromBuffer(pCubeMap->m_pLeftTex, 1);


		pCubeMap->m_pRightTex = 
			pScene->texture[CubeMapData.CubeMap[i].Right_X];
		if (pCubeMap->m_pRightTex->textureBuf == NULL)
			pScene->CreateTexFromBuffer(pCubeMap->m_pRightTex, 1);


		pCubeMap->m_pTopTex = 
			pScene->texture[CubeMapData.CubeMap[i].Top_Y];
		if (pCubeMap->m_pTopTex->textureBuf == NULL)
			pScene->CreateTexFromBuffer(pCubeMap->m_pTopTex, 1);

		pCubeMap->m_pBottomTex = 
			pScene->texture[CubeMapData.CubeMap[i].Bottom_NY];
		if (pCubeMap->m_pBottomTex->textureBuf == NULL)
			pScene->CreateTexFromBuffer(pCubeMap->m_pBottomTex, 1);

		//初始化CubeMap
		pCubeMap->InitCubeMap(0, pScene->m_d3dpp.BackBufferFormat, 
								CubeMapData.CubeMap[i].Length);

		pScene->cubeMap.push_back(pCubeMap);
	}
*/
}

//读取MovieTex信息，2.5版本Up
//##ModelId=47EBC74D03D8
VOID CGetE3DFileInfo::ReadMovieTexData()
{/*
	fseek(pFile, Header.FileTable.MovieTexDataPtr, SEEK_SET);

	//MovieTexture count
	fread(&(MovieTexData.MovieTexCnt), sizeof(MovieTexData.MovieTexCnt), 1, pFile);
	
	//如果MovieTex的数量为0,则忽略保存MovieTexData内容列表字段
	if (MovieTexData.MovieTexCnt == 0) return;

	MovieTexData.MovieTex = new _MovieTex[MovieTexData.MovieTexCnt];
	CMovieTexture** ppMovieTex = pScene->movieTexture.begin();
	for (INT i = 0; i< MovieTexData.MovieTexCnt; i++)
	{	
		CMovieTexture* pMovieTex = new CMovieTexture(pScene->m_pd3dDevice, m_pRenderCS);
			
		//读入MovieTex的名字
		fread(&(MovieTexData.MovieTex[i].Name), sizeof(MovieTexData.MovieTex[i].Name), 1, pFile);
		strcpy(pMovieTex->name, MovieTexData.MovieTex[i].Name);
		pMovieTex->index =  pScene->movieTexture.size();

		//读入MovieTex的路径
		fread(&(MovieTexData.MovieTex[i].FileName), sizeof(MovieTexData.MovieTex[i].FileName), 1, pFile);
		strcpy(pMovieTex->fileName, MovieTexData.MovieTex[i].FileName);

		//读入MovieTex的类型
		fread(&(MovieTexData.MovieTex[i].Type), sizeof(MovieTexData.MovieTex[i].Type), 1, pFile);
		pMovieTex->m_mediaType = MovieTexData.MovieTex[i].Type;

		//读入播放次数
		fread(&(MovieTexData.MovieTex[i].RepeatCnt), sizeof(MovieTexData.MovieTex[i].RepeatCnt), 1, pFile);
		pMovieTex->m_repeatCnt = MovieTexData.MovieTex[i].RepeatCnt;

		//读入MovieTex是否包含声音
		fread(&(MovieTexData.MovieTex[i].WithAudio), sizeof(MovieTexData.MovieTex[i].WithAudio), 1, pFile);
		pMovieTex->m_withAudio = MovieTexData.MovieTex[i].WithAudio;

		//放入movieTexture队列
		pScene->movieTexture.push_back(pMovieTex);
	
	}
	//release memory
	SAFE_DELETE_ARRAY(MovieTexData.MovieTex);
	*/
}

//读取BumpWater信息，2.5版本Up
//##ModelId=47EBC74D03D9
VOID CGetE3DFileInfo::ReadBumpWaterData()
{/*
	fseek(pFile, Header.FileTable.BumpWaterDataPtr, SEEK_SET);

	//获得BumpWater的数量
	fread(&(BumpWaterData.BumpWaterCnt), sizeof(BumpWaterData.BumpWaterCnt), 1, pFile);
	
	//如果MovieTex的数量为0,则忽略保存MovieTexData内容列表字段
	if (BumpWaterData.BumpWaterCnt == 0) return;

	BumpWaterData.BumpWater = new _BumpWater[BumpWaterData.BumpWaterCnt];
	for (INT i = 0; i< BumpWaterData.BumpWaterCnt; i++)
	{
		//写入BumpWater的名字
		fread(&(BumpWaterData.BumpWater[i].Name), sizeof(BumpWaterData.BumpWater[i].Name), 1, pFile);

		//写入BumpWater贴图的宽度
		fread(&(BumpWaterData.BumpWater[i].Width), sizeof(BumpWaterData.BumpWater[i].Width), 1, pFile);

		//写入BumpWater贴图的高度
		fread(&(BumpWaterData.BumpWater[i].Height), sizeof(BumpWaterData.BumpWater[i].Height), 1, pFile);
		
		//写入BumpWater贴图的XScale
		fread(&(BumpWaterData.BumpWater[i].XScale), sizeof(BumpWaterData.BumpWater[i].XScale), 1, pFile);
		
		//写入BumpWater贴图的YScale
		fread(&(BumpWaterData.BumpWater[i].YScale), sizeof(BumpWaterData.BumpWater[i].YScale), 1, pFile);
		
		//写入BumpWater贴图的Radius
		fread(&(BumpWaterData.BumpWater[i].Radius), sizeof(BumpWaterData.BumpWater[i].Radius), 1, pFile);
		
		//写入BumpWater贴图的Speed
		fread(&(BumpWaterData.BumpWater[i].Speed), sizeof(BumpWaterData.BumpWater[i].Speed), 1, pFile);
		
		
		CBumpWater* pBumpWater = new CBumpWater(pScene->m_pd3dDevice);		
		
		strcpy(pBumpWater->name, BumpWaterData.BumpWater[i].Name);
		pBumpWater->width = BumpWaterData.BumpWater[i].Width;
		pBumpWater->height = BumpWaterData.BumpWater[i].Height;
		pBumpWater->m_xSize = BumpWaterData.BumpWater[i].XScale;
		pBumpWater->m_ySize = BumpWaterData.BumpWater[i].YScale;
		pBumpWater->m_radius = BumpWaterData.BumpWater[i].Radius;
		pBumpWater->m_speed = BumpWaterData.BumpWater[i].Speed;

		pBumpWater->CreateBumpMap(pBumpWater->width, pBumpWater->height,
								  pBumpWater->m_xSize, pBumpWater->m_ySize,
								  pBumpWater->m_radius, pBumpWater->m_speed);
		//插入队列
		pScene->bumpWater.push_back(pBumpWater);
	}
	//release memory
	SAFE_DELETE_ARRAY(BumpWaterData.BumpWater);	*/
}

//-------------------------------------------------------------------
//方法: FindReferenceModel()
//参数: 
//用途: 查找每个模型的引用模型并得到引用模型指针
//-------------------------------------------------------------------
//##ModelId=47EBC74D03DA
VOID CGetE3DFileInfo::FindReferenceModel()
{/*
	UINT modelCnt = pScene->model.size();
	for (UINT i = 0; i< modelCnt; i++)
	{
		CModel* pModel = *(pScene->model.begin() + i);
		if (pModel->refModelIndex >= 0)
		{
			pModel->refModel = *(pScene->model.begin() + pModel->refModelIndex);
			//bounding box
			pModel->oriBBox.MinCorner = pModel->refModel->oriBBox.MinCorner;
			pModel->oriBBox.MaxCorner = pModel->refModel->oriBBox.MaxCorner;
					
			//Bounding Sphere
			pModel->oriBSphere.Center = pModel->refModel->oriBSphere.Center;
			pModel->oriBSphere.Radius = pModel->refModel->oriBSphere.Radius;

			//update matrix
			pModel->UpdateScaleMatrix();
			pModel->UpdateRotateMatrix();
			pModel->UpdateTranslateMatrix();
			pModel->MultiplyMatrix();
			//update other
			pModel->UpdateOther();
			
			pModel->visible = TRUE;
		}
	}
	*/
}

//-------------------------------------------------------------------
//方法: LoadE3DFile()
//参数: CScene* pScene, CHAR*  strFileName
//用途: 读取对应的Scene场景为E3D文件
//-------------------------------------------------------------------
//##ModelId=47EBC74D03DB
HRESULT CGetE3DFileInfo::LoadE3DFile(CScene* theScene, CHAR*  strFileName)
{/*
	//指定场景
	if (theScene == NULL)
	{
		MessageBox(NULL, "场景构造失败", "打开E3D文件过程错误", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//打开一个E3D文件时，要把当前场景清空，并停止Render
	pScene = theScene;
	pScene->renderScene = FALSE;
	pScene->Reset();
	strcpy(m_strFileName, strFileName);

	if (strcmp(strFileName,"") == 0) 
	{
		//设置场景可以渲染
		pScene->renderScene = TRUE;
		return S_OK;
	}

	//打开文件IO
	if (NULL == (pFile = fopen(strFileName, "rb")) )
	{
		MessageBox(NULL, "没有找到文件，指定的E3D文件无法打开", "打开E3D文件错误", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//read the header data
	ReadHeader();
	//read the texture data
	ReadTexData();
	//读取CubeMap数据
	CubeMapData.CubeMap = NULL;
	ReadCubeMapData();
	//读取BumpWater数据
	ReadBumpWaterData();
	//读取电影贴图数据
	ReadMovieTexData();
	//read the Material data
	ReadMaterialData();	
	//read the Light Data
	ReadLightData();
	//read the vertices pos in vertesPosData
	ReadVertexPosData();
	//read the vertices normal in vertesPosData
	ReadVertexNormalData();
	//read the vertices uv in UVData
	ReadVertexUVData();
	//read the vertices data
	ReadVertexData();
	//read the Model data
	ReadModelData();
	//读取文字信息
	ReadTextData();
	//读取Overlay信息
	ReadOverlayData();
	//read the camera data
	ReadCameraData();
	//读取Navigater信息
	ReadNavigaterData();
	//找到每个模型对应的索引模型
	FindReferenceModel();
	//读取Button数据
	ReadButtonData();
	//读取图像数据
	ReadImageData();

	//设置场景可以渲染
	pScene->renderScene = TRUE;

	//close the pFile
	fclose(pFile);

*/
	return S_OK;

}		

//##ModelId=47EBC74D03DE
VOID CGetE3DFileInfo::Release()
{

}
