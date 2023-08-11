/////////////////////////////////////////////////////////////////////////
// ����: CGetE3DFileInfo_3_1--E3D�ļ���ȡ��
// ����: ��ȡE3D�ļ�����������������
// ����: lixin	
// ����: 2006-05-06
////////////////////////////////////////////////////////////////////////

#include "CGetE3DFileInfo_3_1.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74D0213
CGetE3DFileInfo_3_1::CGetE3DFileInfo_3_1(CRITICAL_SECTION*	pRenderCS)
{
	m_pRenderCS = pRenderCS;
	pFile = NULL;		//E3D�ļ�������
}

//##ModelId=47EBC74D0242
CGetE3DFileInfo_3_1::~CGetE3DFileInfo_3_1()
{

}


//-------------------------------------------------------------------
//����: ReadHeader()
//����: 
//��;: ��ȡ�ļ�ͷ
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

//��ȡ������Ϣ
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

//��ȡ��������
//##ModelId=47EBC74D003E
VOID CGetE3DFileInfo_3_1::ReadAudioData()
{	
	//�ҵ�������Ϣ��λ��
	fseek(pFile, Header.FileTable.AudioDataPtr, SEEK_SET);

	//�õ���Ƶ����
	fread(&(AudioData.AudioCnt), sizeof(AudioData.AudioCnt), 1, pFile);
	if (AudioData.AudioCnt == 0)
		return;

	AudioData.Audio = new E3D_3_1_Audio[AudioData.AudioCnt];
	//�����ȡ��Ƶ��Ϣ
	for (UINT i =0; i< AudioData.AudioCnt; i++)
	{
		//���浱ǰ��Ƶ�Ļ�����Ϣ
		fread(&(AudioData.Audio[i]), sizeof(AudioData.Audio[i]), 1, pFile);
		CAudioPlayer* pAudio = new CAudioPlayer;

		//��ǰ��Ƶ�Ļ�����Ϣ
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

//��ȡFlash����
//##ModelId=47EBC74D005D
VOID CGetE3DFileInfo_3_1::ReadFlashData()
{	
	//�ҵ�Flash��Ϣ��λ��
	fseek(pFile, Header.FileTable.FlashDataPtr, SEEK_SET);

	//�õ�Flash����
	fread(&(FlashData.FlashCnt), sizeof(FlashData.FlashCnt), 1, pFile);
	if (FlashData.FlashCnt == 0)
		return;
}

//-------------------------------------------------------------------
//����: ReadVertexPosData()
//����: 
//��;: ��ȡ����λ����Ϣ
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
//����: ReadVertexNormalData()
//����: 
//��;: ��ȡ����ķ�����Ϣ
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
//����: ReadVertexUVData()
//����: 
//��;: ��ȡ�����UV��Ϣ
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
//����: ReadVetexData()
//����: 
//��;: ��ȡ������Ϣ
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
			MessageBox(GetActiveWindow(), "����UV���ݽ�ѹ����δ֪�����޷����ļ���", "����UV��ѹ...����", MB_OK | MB_ICONERROR);
		}

	}
}

//-------------------------------------------------------------------
//����: ReadModelData()
//����: 
//��;: ��ȡģ����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74D0157
VOID CGetE3DFileInfo_3_1::ReadModelData()
{/*
	fseek(pFile, Header.FileTable.ModelDataPtr, SEEK_SET);

	//����ģ�͵�����
	fread(&(ModelData.ModelCnt), sizeof(ModelData.ModelCnt), 1, pFile);

	ModelData.Model = new E3D_3_1_Model[ModelData.ModelCnt];
	UINT tVertexOffset = 0;
	//�õ�ģ������
	for(UINT modelIndex= 0; modelIndex< ModelData.ModelCnt; modelIndex++)
	{
		CModel* pModel = new CModel();
		pModel->index = modelIndex;

		//name
		fread(&(ModelData.Model[modelIndex].Name), sizeof(ModelData.Model[modelIndex].Name), 1, pFile);
		strcpy(pModel->name, ModelData.Model[modelIndex].Name);

		//matrix
		fread(&(ModelData.Model[modelIndex].Matrix), sizeof(ModelData.Model[modelIndex].Matrix), 1, pFile);
		//����ģ�͵�λ�ã�ת�ǣ�����ֵ
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
			
		//����ģ�͵�ID1
		fread(&(ModelData.Model[modelIndex].ID1), sizeof(ModelData.Model[modelIndex].ID1), 1, pFile);
		pModel->ID1 = ModelData.Model[modelIndex].ID1;

		//����ģ�͵�ID2
		fread(&(ModelData.Model[modelIndex].ID2), sizeof(ModelData.Model[modelIndex].ID2), 1, pFile);
		pModel->ID2 = ModelData.Model[modelIndex].ID2;

		//reference
		fread(&(ModelData.Model[modelIndex].RefModelIndex), sizeof(ModelData.Model[modelIndex].RefModelIndex), 1, pFile);
		pModel->refModelIndex = ModelData.Model[modelIndex].RefModelIndex;

		//���������ģ�ͣ����������ݲ�����
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
			//��ģ�͵�λ�ñ任��Axis�����ž��󲻷Ž�ȥ,�������Ҳ��С��GCS	
			pModel->pAxis->SetRotationMatrix(&pModel->transform.rotation_matrix);
			pModel->pAxis->SetTranslateMatrix(&pModel->transform.translate_matrix);
			pModel->pAxis->MultiplyMatrix();
			//pModel->showAxis = false;

			//model RenderBoundBox
			pModel->pRBBox= new CRenderBoundBox(pScene->m_pd3dDevice);
			pModel->pRBBox->SetRBBoxVertex(pModel->oriBBox.MinCorner,pModel->oriBBox.MaxCorner);
			//��ģ�͵�λ�ñ任��RBBox��
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

			//�����������ģ�ͣ�����붥�㣬���������ģ�ͣ��Ͳ����붥���ˣ�������ʡ���ڴ�ռ�
			if (pModel->refModelIndex == -1)
			{
				pModel->vertex = new CVertex[pModel->vertexCnt];
				//ע��: ���ڶ������ɫ��Ϊ�˾�����max�е���ȾЧ���ӽ������Ե����˵ƹ�Ͳ��ʵ���ɫ��
				//������ͼʱ�����Ͷ������ɫֵ�䰵��Ȼ��������ͼ��ϳ��߹��Ч��
				//��û����ͼʱ���������ɫΪԭʼ��ɫ
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

			//�任����ת��Ϊ�����һ֡�ı任����//--------------------------
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
					
					//�����ֵ�任����
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

			//ȷ����ǰģ����ʹ�õĲ��ʣ�ȷ��ģ�͵�FVF
			pModel->m_FVF = pModel->shader->m_FVF;
			pModel->m_FVFSize = pModel->shader->m_FVFSize;

			//ȷ��ģ���Ƿ���Billboardģ��
			fread(&(ModelData.Model[modelIndex].IsBillboard), sizeof(ModelData.Model[modelIndex].IsBillboard), 1, pFile);
			pModel->isBillboard = ModelData.Model[modelIndex].IsBillboard;

			//ȷ��ģ���Ƿ���Collisionģ��
			fread(&(ModelData.Model[modelIndex].IsCollision), sizeof(ModelData.Model[modelIndex].IsCollision), 1, pFile);
			pModel->isCollision = ModelData.Model[modelIndex].IsCollision;

			//ȷ��ģ���Ƿ���Groundģ��
			fread(&(ModelData.Model[modelIndex].IsGround), sizeof(ModelData.Model[modelIndex].IsGround), 1, pFile);
			pModel->isGround = ModelData.Model[modelIndex].IsGround;
		
			//ȷ��ģ���Ƿ���SkyBox
			fread(&(ModelData.Model[modelIndex].IsSkyBox), sizeof(ModelData.Model[modelIndex].IsSkyBox), 1, pFile);
			pModel->isSkyBox = ModelData.Model[modelIndex].IsSkyBox;

			//ȷ��ģ���Ƿ�ɼ�
			fread(&(ModelData.Model[modelIndex].IsVisible), sizeof(ModelData.Model[modelIndex].IsVisible), 1, pFile);
			pModel->visible = ModelData.Model[modelIndex].IsVisible;
	
			//ȷ��ģ���Ƿ���Clickable
			fread(&(ModelData.Model[modelIndex].IsClickable), sizeof(ModelData.Model[modelIndex].IsClickable), 1, pFile);
			pModel->clickable = ModelData.Model[modelIndex].IsClickable;
				
			//ȷ��ģ���Ƿ�ʹ�ö�̬UV
			fread(&(ModelData.Model[modelIndex].UseDynamicUV), sizeof(ModelData.Model[modelIndex].UseDynamicUV), 1, pFile);
			pModel->bDynamicUV = ModelData.Model[modelIndex].UseDynamicUV;
			
			//U Step
			fread(&(ModelData.Model[modelIndex].UStep), sizeof(ModelData.Model[modelIndex].UStep), 1, pFile);
			pModel->UStep = ModelData.Model[modelIndex].UStep;
			
			//ȷ��ģ���Ƿ�ʹ�ö�̬UV
			fread(&(ModelData.Model[modelIndex].VStep), sizeof(ModelData.Model[modelIndex].VStep), 1, pFile);
			pModel->VStep = ModelData.Model[modelIndex].VStep;

			//create vBuffer
			pScene->CreateModelVBuffer(pModel);
			//create IBuffer
			pScene->CreateModelIBuffer(pModel);	
		}	
		
		//��ģ�ͷŵ���Ӧ�Ĳ���modelList
		pModel->shader->modelList.push_back((DWORD)pModel);

		//push model
		pScene->model.push_back(pModel);

		//push model - transparent model��͸��ģ��
		if (pModel->shader->bAlphaBlend)
			pScene->transModel.push_back(pModel);

	}

	//release memory
	SAFE_DELETE_ARRAY(ModelData.Model);
	SAFE_DELETE_ARRAY(VertexData.Vertex);
	SAFE_DELETE_ARRAY(VertexPosData.PosList);
	SAFE_DELETE_ARRAY(UVData.UVList);*/
}

//��ȡģ�����б���Ϣ
//##ModelId=47EBC74D0177
VOID CGetE3DFileInfo_3_1::ReadModelGroupData()
{/*
	//���ģ�������ݼ�¼��ƫ����
	fseek(pFile, Header.FileTable.ModelGroupDataPtr, SEEK_SET);

	//����ģ��������
	fread(&(ModelGroupData.ModelGroupCnt), sizeof(ModelGroupData.ModelGroupCnt), 1, pFile);
	if (ModelGroupData.ModelGroupCnt == 0)
		return;

	ModelGroupData.ModelGroup = new E3D_3_1_ModelGroup[ModelGroupData.ModelGroupCnt];
	
	//�����ȡģ������Ϣ
	for (UINT i =0; i< ModelGroupData.ModelGroupCnt; i++)
	{
		CModelGroup* pModelGroup = new CModelGroup;
		//�ݹ����ModelGroup������
		ReadModelGroupData1(pModelGroup);
		pScene->modelGroup.push_back(pModelGroup);
	}
	//release memory
	SAFE_DELETE_ARRAY(ModelGroupData.ModelGroup);
	*/
}

//��ȡģ�����б���Ϣ
//##ModelId=47EBC74D0196
VOID  CGetE3DFileInfo_3_1::ReadModelGroupData1(CModelGroup* pModelGroup)
{/*
	//���뵱ǰģ���������
	fread(&(pModelGroup->name), sizeof(pModelGroup->name), 1, pFile);

	//���뵱ǰģ�����е�ģ������
	INT modelCnt = 0;
	fread(&modelCnt, sizeof(modelCnt), 1, pFile);
	
	//���뵱ǰģ�����е�ģ���б�
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

	//���뵱ǰģ�����е�ģ��������
	INT modelGroupCnt = 0;
	fread(&modelGroupCnt, sizeof(modelGroupCnt), 1, pFile);
	
	//��ǰģ�����е�ģ�����б�, �ݹ����
	for (INT j = 0; j < modelGroupCnt; j++)
	{
		CModelGroup* sModelGroup = new CModelGroup;
		ReadModelGroupData1(sModelGroup);
		pModelGroup->modelGroup.push_back(sModelGroup);
	}*/
}

//-------------------------------------------------------------------
//����: ReadCameraData()
//����: 
//��;: ��ȡ�������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74D008C
VOID CGetE3DFileInfo_3_1::ReadCameraData()
{
	
}

//-------------------------------------------------------------------
//����: ReadTexData()
//����: 
//��;: ��ȡ��ͼ��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74D01C7
VOID CGetE3DFileInfo_3_1::ReadTexData()
{
}

//-------------------------------------------------------------------
//����: ReadMaterialData()
//����: 
//��;: ��ȡ������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74D009C
VOID CGetE3DFileInfo_3_1::ReadMaterialData()
{
}

//-------------------------------------------------------------------
//����: ReadLightData()
//����: 
//��;: ����ƹ���Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74D00BB
VOID CGetE3DFileInfo_3_1::ReadLightData()
{
}

//-------------------------------------------------------------------
//����: ReadTextData()
//����: 
//��;: ��ȡ������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74D01B5
VOID CGetE3DFileInfo_3_1::ReadTextData()
{
}

//-------------------------------------------------------------------
//����: ReadOverlayData()
//����: 
//��;: ��ȡOverlay��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74D01B6
VOID CGetE3DFileInfo_3_1::ReadOverlayData()
{
}

//-------------------------------------------------------------------
//����: ReadNavigaterData()
//����: 
//��;: ��ȡNavigate��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74D01C5
VOID CGetE3DFileInfo_3_1::ReadNavigaterData()
{
}

//-------------------------------------------------------------------
//����: ReadButtonData()
//����: 
//��;: ��ȡButton��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74D01C6
VOID CGetE3DFileInfo_3_1::ReadButtonData()
{

}

//-------------------------------------------------------------------
//����: ReadImageData()
//����: 
//��;: ��ȡImage��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74D01E4
VOID CGetE3DFileInfo_3_1::ReadImageData()
{
}

//-------------------------------------------------------------------
//����: ReadCubeMapData()
//����: 
//��;: ��ȡCubeMap��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74D01E5
VOID CGetE3DFileInfo_3_1::ReadCubeMapData()
{

}

//��ȡMovieTex��Ϣ��2.5�汾Up
//##ModelId=47EBC74D01E6
VOID CGetE3DFileInfo_3_1::ReadMovieTexData()
{
}

//��ȡBumpWater��Ϣ��2.5�汾Up
//##ModelId=47EBC74D01F4
VOID CGetE3DFileInfo_3_1::ReadBumpWaterData()
{
}

//-------------------------------------------------------------------
//����: FindReferenceModel()
//����: 
//��;: ����ÿ��ģ�͵�����ģ�Ͳ��õ�����ģ��ָ��
//-------------------------------------------------------------------
//##ModelId=47EBC74D01F8
VOID CGetE3DFileInfo_3_1::FindReferenceModel()
{
}

//��ȡ��̬��ͼ��Ϣ
//##ModelId=47EBC74D01F5
VOID CGetE3DFileInfo_3_1::ReadDynamicTexData()
{
}

//��ȡ��Ȩ��Ϣ
//##ModelId=47EBC74D01F6
VOID CGetE3DFileInfo_3_1::ReadPrivacyData()
{	
}

//��ȡ������Ϣ
//##ModelId=47EBC74D01F7
VOID CGetE3DFileInfo_3_1::ReadContorlData()
{
}

//-------------------------------------------------------------------
//����: LoadE3DFile()
//����: CScene* pScene, CHAR*  strFileName
//��;: ��ȡ��Ӧ��Scene����ΪE3D�ļ�
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
