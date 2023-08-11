/////////////////////////////////////////////////////////////////////////
// 名称: CGetE3DFileInfo_3_1--E3D文件读取类
// 描述: 读取E3D文件，并规整场景数据
// 作者: lixin	
// 日期: 2006-05-06
////////////////////////////////////////////////////////////////////////

#include "CGetE3DFileInfo_3_1.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74D0213
CGetE3DFileInfo_3_1::CGetE3DFileInfo_3_1(CRITICAL_SECTION*	pRenderCS)
{
	m_pRenderCS = pRenderCS;
	pFile = NULL;		//E3D文件数据流
}

//##ModelId=47EBC74D0242
CGetE3DFileInfo_3_1::~CGetE3DFileInfo_3_1()
{

}


//-------------------------------------------------------------------
//方法: ReadHeader()
//参数: 
//用途: 读取文件头
//-------------------------------------------------------------------
//##ModelId=47EBC74C03CE
VOID CGetE3DFileInfo_3_1::ReadHeader()
{
	//write the Header to pFile
	if (pFile == NULL) return;
	//init _E3DHeader
	fseek(pFile, 0, SEEK_SET);
	fread(&Header, sizeof(Header), 1, pFile);
}

//读取窗体信息
//##ModelId=47EBC74D000F
VOID CGetE3DFileInfo_3_1::ReadWindowProp()
{
	fread(&WindowProp, sizeof(WindowProp), 1, pFile);

	pScene->m_oriWidth = WindowProp.Width;
	pScene->m_oriHeight = WindowProp.Height;

	pScene->rect.left = 0;
	pScene->rect.top = 0;
	pScene->rect.right = pScene->m_oriWidth;
	pScene->rect.bottom = pScene->m_oriHeight;

	pScene->background = WindowProp.BackColor;
	pScene->fps = WindowProp.FPS;
	pScene->antiLevel = WindowProp.AntiAliasLevel;
	
	pScene->FSBlurLevel = WindowProp.FSBlurLevel;
	pScene->FSGlowLevel = WindowProp.FSGlowLevel;

	pScene->m_KfrCnt = WindowProp.KeyframeCnt;
	pScene->m_KfrRepeatCnt = WindowProp.RepeatCnt;
}

//读取声音数据
//##ModelId=47EBC74D003E
VOID CGetE3DFileInfo_3_1::ReadAudioData()
{	
	//找到声音信息的位置
	fseek(pFile, Header.FileTable.AudioDataPtr, SEEK_SET);

	//得到音频数量
	fread(&(AudioData.AudioCnt), sizeof(AudioData.AudioCnt), 1, pFile);
	if (AudioData.AudioCnt == 0)
		return;

	AudioData.Audio = new E3D_3_1_Audio[AudioData.AudioCnt];
	//逐个读取音频信息
	for (UINT i =0; i< AudioData.AudioCnt; i++)
	{
		//保存当前音频的基本信息
		fread(&(AudioData.Audio[i]), sizeof(AudioData.Audio[i]), 1, pFile);
		CAudioPlayer* pAudio = new CAudioPlayer;

		//当前音频的基本信息
		strcpy(pAudio->name, AudioData.Audio[i].Name);
		pAudio->index = pScene->audio.size();
		pAudio->m_repeatCnt = AudioData.Audio[i].RepeatCnt;
		pAudio->type = AudioData.Audio[i].Type;

		if (pAudio->Create(AudioData.Audio[i].FileName, AudioData.Audio[i].RepeatCnt) == false)
		{
			SAFE_RELEASE_D3D(pAudio);
			SAFE_DELETE(pAudio);
		}
		pScene->audio.push_back(pAudio);
	}

	//release memory
	SAFE_DELETE_ARRAY(AudioData.Audio);
}

//读取Flash数据
//##ModelId=47EBC74D005D
VOID CGetE3DFileInfo_3_1::ReadFlashData()
{	
	//找到Flash信息的位置
	fseek(pFile, Header.FileTable.FlashDataPtr, SEEK_SET);

	//得到Flash数量
	fread(&(FlashData.FlashCnt), sizeof(FlashData.FlashCnt), 1, pFile);
	if (FlashData.FlashCnt == 0)
		return;
}

//-------------------------------------------------------------------
//方法: ReadVertexPosData()
//参数: 
//用途: 读取顶点位置信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D00BC
VOID CGetE3DFileInfo_3_1::ReadVertexPosData()
{
	fseek(pFile, Header.FileTable.VertexPosDataPtr, SEEK_SET);
	
	//zipped vertex cnt;
	fread(&VertexPosData.VertexPosCnt, sizeof(VertexPosData.VertexPosCnt), 1, pFile);

	VertexPosData.PosList = new D3DXVECTOR3[VertexPosData.VertexPosCnt];
	
	for (UINT i = 0; i< VertexPosData.VertexPosCnt; i++)
	{
		fread(&(VertexPosData.PosList[i]), sizeof(VertexPosData.PosList[i]), 1, pFile);
	}
}

//-------------------------------------------------------------------
//方法: ReadVertexNormalData()
//参数: 
//用途: 读取顶点的法线信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D00EA
VOID CGetE3DFileInfo_3_1::ReadVertexNormalData()
{
	fseek(pFile, Header.FileTable.VertexNormalDataPtr, SEEK_SET);
	
	//zipped vertex cnt;
	fread(&VertexNormalData.VertexNormalCnt, sizeof(VertexNormalData.VertexNormalCnt), 1, pFile);

	VertexNormalData.NormalList = new D3DXVECTOR3[VertexNormalData.VertexNormalCnt];
	
	for (UINT i = 0; i< VertexNormalData.VertexNormalCnt; i++)
	{
		fread(&(VertexNormalData.NormalList[i]), sizeof(VertexNormalData.NormalList[i]), 1, pFile);
	}
}

//-------------------------------------------------------------------
//方法: ReadVertexUVData()
//参数: 
//用途: 读取顶点的UV信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D0109
VOID CGetE3DFileInfo_3_1::ReadVertexUVData()
{
	fseek(pFile, Header.FileTable.UVDataPtr, SEEK_SET);	

	//zipped uv count
	fread(&(UVData.UVCount), sizeof(UVData.UVCount), 1, pFile);

	UVData.UVList = new E3D_3_1_UV[UVData.UVCount];
	
	for (UINT i = 0; i< UVData.UVCount; i++)
	{
		fread(&(UVData.UVList[i]), sizeof(UVData.UVList[i]), 1, pFile);
	}
}

//-------------------------------------------------------------------
//方法: ReadVetexData()
//参数: 
//用途: 读取顶点信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D0138
VOID CGetE3DFileInfo_3_1::ReadVertexData()
{
	fseek(pFile, Header.FileTable.VertexDataPtr, SEEK_SET);

	//vertex count
	fread(&(VertexData.VertexCnt), sizeof(VertexData.VertexCnt), 1, pFile);

	//vertex data
	VertexData.Vertex = new E3D_3_1_Vertex[VertexData.VertexCnt];
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

	}
}

//-------------------------------------------------------------------
//方法: ReadModelData()
//参数: 
//用途: 读取模型信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D0157
VOID CGetE3DFileInfo_3_1::ReadModelData()
{/*
	fseek(pFile, Header.FileTable.ModelDataPtr, SEEK_SET);

	//读入模型的数量
	fread(&(ModelData.ModelCnt), sizeof(ModelData.ModelCnt), 1, pFile);

	ModelData.Model = new E3D_3_1_Model[ModelData.ModelCnt];
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
			ModelData.Model[modelIndex].Face = new E3D_3_1_Face[ModelData.Model[modelIndex].FaceCnt];

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
				ModelData.Model[modelIndex].VertexPosList = new E3D_3_1_VertexPosList[ModelData.Model[modelIndex].KeyFrameCnt];
			
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
				
			//确定模型是否使用动态UV
			fread(&(ModelData.Model[modelIndex].UseDynamicUV), sizeof(ModelData.Model[modelIndex].UseDynamicUV), 1, pFile);
			pModel->bDynamicUV = ModelData.Model[modelIndex].UseDynamicUV;
			
			//U Step
			fread(&(ModelData.Model[modelIndex].UStep), sizeof(ModelData.Model[modelIndex].UStep), 1, pFile);
			pModel->UStep = ModelData.Model[modelIndex].UStep;
			
			//确定模型是否使用动态UV
			fread(&(ModelData.Model[modelIndex].VStep), sizeof(ModelData.Model[modelIndex].VStep), 1, pFile);
			pModel->VStep = ModelData.Model[modelIndex].VStep;

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
		if (pModel->shader->bAlphaBlend)
			pScene->transModel.push_back(pModel);

	}

	//release memory
	SAFE_DELETE_ARRAY(ModelData.Model);
	SAFE_DELETE_ARRAY(VertexData.Vertex);
	SAFE_DELETE_ARRAY(VertexPosData.PosList);
	SAFE_DELETE_ARRAY(UVData.UVList);*/
}

//读取模型组列表信息
//##ModelId=47EBC74D0177
VOID CGetE3DFileInfo_3_1::ReadModelGroupData()
{/*
	//获得模型组数据记录的偏移量
	fseek(pFile, Header.FileTable.ModelGroupDataPtr, SEEK_SET);

	//读入模型组数量
	fread(&(ModelGroupData.ModelGroupCnt), sizeof(ModelGroupData.ModelGroupCnt), 1, pFile);
	if (ModelGroupData.ModelGroupCnt == 0)
		return;

	ModelGroupData.ModelGroup = new E3D_3_1_ModelGroup[ModelGroupData.ModelGroupCnt];
	
	//逐个读取模型组信息
	for (UINT i =0; i< ModelGroupData.ModelGroupCnt; i++)
	{
		CModelGroup* pModelGroup = new CModelGroup;
		//递归读入ModelGroup的数据
		ReadModelGroupData1(pModelGroup);
		pScene->modelGroup.push_back(pModelGroup);
	}
	//release memory
	SAFE_DELETE_ARRAY(ModelGroupData.ModelGroup);
	*/
}

//读取模型组列表信息
//##ModelId=47EBC74D0196
VOID  CGetE3DFileInfo_3_1::ReadModelGroupData1(CModelGroup* pModelGroup)
{/*
	//读入当前模型组的名称
	fread(&(pModelGroup->name), sizeof(pModelGroup->name), 1, pFile);

	//读入当前模型组中的模型数量
	INT modelCnt = 0;
	fread(&modelCnt, sizeof(modelCnt), 1, pFile);
	
	//读入当前模型组中的模型列表
	if (modelCnt > 0)
	{
		CModel** ppModel = pScene->model.begin();
		for (INT i = 0; i < modelCnt; i++)
		{
			INT index = 0;
			fread(&index, sizeof(index), 1, pFile);
			CModel* pModel = *(ppModel + index);
			pModelGroup->model.push_back(pModel);
			pModel->modelGroup = (DWORD)pModelGroup;
		}
	}

	//读入当前模型组中的模型组数量
	INT modelGroupCnt = 0;
	fread(&modelGroupCnt, sizeof(modelGroupCnt), 1, pFile);
	
	//当前模型组中的模型组列表, 递归调用
	for (INT j = 0; j < modelGroupCnt; j++)
	{
		CModelGroup* sModelGroup = new CModelGroup;
		ReadModelGroupData1(sModelGroup);
		pModelGroup->modelGroup.push_back(sModelGroup);
	}*/
}

//-------------------------------------------------------------------
//方法: ReadCameraData()
//参数: 
//用途: 读取摄像机信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D008C
VOID CGetE3DFileInfo_3_1::ReadCameraData()
{
	
}

//-------------------------------------------------------------------
//方法: ReadTexData()
//参数: 
//用途: 读取贴图信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D01C7
VOID CGetE3DFileInfo_3_1::ReadTexData()
{
}

//-------------------------------------------------------------------
//方法: ReadMaterialData()
//参数: 
//用途: 读取材质信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D009C
VOID CGetE3DFileInfo_3_1::ReadMaterialData()
{
}

//-------------------------------------------------------------------
//方法: ReadLightData()
//参数: 
//用途: 读入灯光信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D00BB
VOID CGetE3DFileInfo_3_1::ReadLightData()
{
}

//-------------------------------------------------------------------
//方法: ReadTextData()
//参数: 
//用途: 读取文字信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D01B5
VOID CGetE3DFileInfo_3_1::ReadTextData()
{
}

//-------------------------------------------------------------------
//方法: ReadOverlayData()
//参数: 
//用途: 读取Overlay信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D01B6
VOID CGetE3DFileInfo_3_1::ReadOverlayData()
{
}

//-------------------------------------------------------------------
//方法: ReadNavigaterData()
//参数: 
//用途: 读取Navigate信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D01C5
VOID CGetE3DFileInfo_3_1::ReadNavigaterData()
{
}

//-------------------------------------------------------------------
//方法: ReadButtonData()
//参数: 
//用途: 读取Button信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D01C6
VOID CGetE3DFileInfo_3_1::ReadButtonData()
{

}

//-------------------------------------------------------------------
//方法: ReadImageData()
//参数: 
//用途: 读取Image信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D01E4
VOID CGetE3DFileInfo_3_1::ReadImageData()
{
}

//-------------------------------------------------------------------
//方法: ReadCubeMapData()
//参数: 
//用途: 读取CubeMap信息
//-------------------------------------------------------------------
//##ModelId=47EBC74D01E5
VOID CGetE3DFileInfo_3_1::ReadCubeMapData()
{

}

//读取MovieTex信息，2.5版本Up
//##ModelId=47EBC74D01E6
VOID CGetE3DFileInfo_3_1::ReadMovieTexData()
{
}

//读取BumpWater信息，2.5版本Up
//##ModelId=47EBC74D01F4
VOID CGetE3DFileInfo_3_1::ReadBumpWaterData()
{
}

//-------------------------------------------------------------------
//方法: FindReferenceModel()
//参数: 
//用途: 查找每个模型的引用模型并得到引用模型指针
//-------------------------------------------------------------------
//##ModelId=47EBC74D01F8
VOID CGetE3DFileInfo_3_1::FindReferenceModel()
{
}

//读取动态贴图信息
//##ModelId=47EBC74D01F5
VOID CGetE3DFileInfo_3_1::ReadDynamicTexData()
{
}

//读取授权信息
//##ModelId=47EBC74D01F6
VOID CGetE3DFileInfo_3_1::ReadPrivacyData()
{	
}

//读取控制信息
//##ModelId=47EBC74D01F7
VOID CGetE3DFileInfo_3_1::ReadContorlData()
{
}

//-------------------------------------------------------------------
//方法: LoadE3DFile()
//参数: CScene* pScene, CHAR*  strFileName
//用途: 读取对应的Scene场景为E3D文件
//-------------------------------------------------------------------
//##ModelId=47EBC74D01F9
HRESULT CGetE3DFileInfo_3_1::LoadE3DFile(CScene* theScene, CHAR*  strFileName)
{

	return S_OK;

}		

//##ModelId=47EBC74D0204
VOID CGetE3DFileInfo_3_1::Release()
{

}
