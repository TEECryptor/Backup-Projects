///////////////////////////////////////////////////////////////////////////
// CSaveV3D_3_1.cpp: interface for the CSaveE3D class.
// ��  ��: CSaveV3D_3_1
// ��  ��: �������ݱ����࣬���泡���е�����Ϊ1.0�汾��*.V3D�ļ�, ��ͼ�������ⲿ·������
// ��  ��: Lixin
// ��  ��: 2007-05-06
// ��  Ȩ: �Ϻ���ͼ����������Ϣ�������޹�˾
///////////////////////////////////////////////////////////////////////////

#include "CSaveV3D_3_1.h"
#include "CMovieTexture.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC7440213
CSaveV3D_3_1::CSaveV3D_3_1()
{
	pScene = NULL;				//ָ�򳡾�����Scene��ָ��
	pFile = NULL;				//ָ��E3D�ļ���ָ��
	nFileSize = 0;				//E3D�ļ��Ĵ�С
}

//##ModelId=47EBC7440242
CSaveV3D_3_1::~CSaveV3D_3_1()
{

}

/*-------------------------------------------------------------------
// ��    ��: WriteHeader()
// ��    ��: 
// ��    ;: д���ļ�ͷ
// �޸ļ�¼:  
2007-07-20 lixin V3D�����ļ��汾������Ϊ 3.11 
2007-10-16 fuxb V3D�����ļ��汾������Ϊ  3.12
//-------------------------------------------------------------------*/
//##ModelId=47EBC743035B
VOID CSaveV3D_3_1::WriteHeader()
{
	//init _E3DHeader
	strcpy(Header.CorpID, "SUNTOWARD");

	Header.V3DVer = 3.12f;  
	Header.VGSVer = 3.11f;

	V3D_3_1_Table ptrTable;
	ptrTable.AudioDataPtr = 0x00;
	ptrTable.FlashDataPtr = 0x00;
	ptrTable.CameraDataPtr = 0x00;
	ptrTable.MaterialDataPtr = 0x00;
	ptrTable.ModelDataPtr = 0x00;
	ptrTable.ModelGroupDataPtr = 0x00;
	ptrTable.LightDataPtr = 0x00;
	ptrTable.VertexPosDataPtr = 0x00;
	ptrTable.VertexNormalDataPtr = 0x00;
	ptrTable.UVDataPtr = 0x00;
	ptrTable.VertexDataPtr = 0x00;
	ptrTable.TextDataPtr = 0x00;
	ptrTable.OverlayDataPtr = 0x00;	
	ptrTable.NavigaterDataPtr = 0x00;		
	ptrTable.ButtonDataPtr = 0x00;	
	ptrTable.DynamicTexDataPtr = 0x00;
	ptrTable.PrivacyDataPtr = 0x00;
	
	ptrTable.ControlDataPtr = 0x00;

	ptrTable.TexDataPtr = 0x00;
	ptrTable.ImageDataPtr = 0x00;
	ptrTable.CubeMapDataPtr = 0x00;
	ptrTable.MovieTexDataPtr = 0x00;
	ptrTable.BumpWaterDataPtr = 0x00;

	Header.FileTable = ptrTable;
	
	//write the Header to pFile
	if (pFile == NULL) return;
	fwrite(&Header, sizeof(Header), 1, pFile);
}

//д�봰������
//##ModelId=47EBC743038A
VOID CSaveV3D_3_1::WriteWindowProp()
{
	WindowProp.Width = pScene->m_oriWidth;
	WindowProp.Height = pScene->m_oriHeight;

	WindowProp.BackColor = pScene->background;
	WindowProp.FPS = pScene->fps;
	WindowProp.AntiAliasLevel = pScene->antiLevel;
	WindowProp.FSBlurLevel = pScene->FSBlurLevel;
	WindowProp.FSGlowLevel = pScene->FSGlowLevel;

	WindowProp.KeyframeCnt = pScene->m_KfrCnt;
	WindowProp.RepeatCnt = pScene->m_KfrRepeatCnt;

	fwrite(&WindowProp, sizeof(WindowProp), 1, pFile);
}

//-------------------------------------------------------------------
//����: WriteCameraData()
//����: 
//��;: д���������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74303BB
VOID CSaveV3D_3_1::WriteCameraData()
{
	//�õ����������
	CameraData.CameraCnt = pScene->camera.size();
	//��¼��������ݼ�¼��ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.CameraDataPtr = tPos;

	fwrite(&(CameraData.CameraCnt), sizeof(CameraData.CameraCnt), 1, pFile);

	if (CameraData.CameraCnt == 0) 
		return;

	CameraData.Camera = new V3D_3_1_Camera[CameraData.CameraCnt];
	//�����ȡ�������Ϣ
	for (UINT i =0; i< CameraData.CameraCnt; i++)
	{
		CCamera* pCamera = pScene->camera[i];
		//��ǰ������Ļ�����Ϣ
		strcpy(CameraData.Camera[i].Name, pCamera->name);
		CameraData.Camera[i].Info.From = *(pCamera->GetFrom());
		CameraData.Camera[i].Info.At = *(pCamera->GetAt());
		CameraData.Camera[i].Info.Fov = pCamera->GetFov();
		CameraData.Camera[i].Info.Roll = pCamera->GetRoll();
		CameraData.Camera[i].Info.NearClip = pCamera->nearClip;
		CameraData.Camera[i].Info.FarClip = pCamera->farClip;

		//���浱ǰ������Ļ�����Ϣ
		fwrite(&(CameraData.Camera[i].Name), sizeof(CameraData.Camera[i].Name), 1, pFile);
		fwrite(&(CameraData.Camera[i].Info.From), sizeof(CameraData.Camera[i].Info.From), 1, pFile);
		fwrite(&(CameraData.Camera[i].Info.At), sizeof(CameraData.Camera[i].Info.At), 1, pFile);
		fwrite(&(CameraData.Camera[i].Info.Roll), sizeof(CameraData.Camera[i].Info.Roll), 1, pFile);
		fwrite(&(CameraData.Camera[i].Info.Fov), sizeof(CameraData.Camera[i].Info.Fov), 1, pFile);
		fwrite(&(CameraData.Camera[i].Info.NearClip), sizeof(CameraData.Camera[i].Info.NearClip), 1, pFile);
		fwrite(&(CameraData.Camera[i].Info.FarClip), sizeof(CameraData.Camera[i].Info.FarClip), 1, pFile);
	
		
		//��ǰ������Ĺؼ�֡��Ϣ
		CameraData.Camera[i].KeyFrameCnt =pCamera->kfrCnt;
		
		fwrite(&(CameraData.Camera[i].KeyFrameCnt), sizeof(CameraData.Camera[i].KeyFrameCnt), 1, pFile);

		if (CameraData.Camera[i].KeyFrameCnt > 1)
		{
			CameraData.Camera[i].KFRInfo = new V3D_3_1_CameraKFRInfo[CameraData.Camera[i].KeyFrameCnt];
			
			//��֡�������������Ϣ
			for (UINT j =0; j< CameraData.Camera[i].KeyFrameCnt; j++)
			{
				CameraData.Camera[i].KFRInfo[j].From = pCamera->kfrInfo[j].From;
				CameraData.Camera[i].KFRInfo[j].At = pCamera->kfrInfo[j].At;
				CameraData.Camera[i].KFRInfo[j].Roll = pCamera->kfrInfo[j].Rol;

				fwrite(&(CameraData.Camera[i].KFRInfo[j].From), sizeof(CameraData.Camera[i].KFRInfo[j].From), 1, pFile);
				fwrite(&(CameraData.Camera[i].KFRInfo[j].At), sizeof(CameraData.Camera[i].KFRInfo[j].At), 1, pFile);
				fwrite(&(CameraData.Camera[i].KFRInfo[j].Roll), sizeof(CameraData.Camera[i].KFRInfo[j].Roll), 1, pFile);
			}
			
			//release memory
			SAFE_DELETE_ARRAY(CameraData.Camera[i].KFRInfo);
		}
	}

	//release memory
	SAFE_DELETE_ARRAY(CameraData.Camera);
	
}

//-------------------------------------------------------------------
//����: WriteCameraData()
//����: 
//��;: д�������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC7440000
VOID CSaveV3D_3_1::WriteMaterialData()
{
	V3D_3_1_1_MaterialList MaterialList;
	
	//�õ���������
	MaterialList.MaterialCnt = pScene->m_vShader.size();

	//��¼�������ݵ�ƫ���� 
	UINT tPos = ftell(pFile);
	Header.FileTable.MaterialDataPtr = tPos;

	//д���������
	fwrite(&(MaterialList.MaterialCnt), sizeof(MaterialList.MaterialCnt), 1, pFile);

	// MaterialList.Material = new V3D_3_1_Material[MaterialList.MaterialCnt];
	MaterialList.Material = new V3D_3_1_1_Material[MaterialList.MaterialCnt];

	UINT MtrlIndex = 0;
	UINT TexIndex =0;
	//�õ������б�
	for (UINT m =0; m< MaterialList.MaterialCnt; m++)
	{
		CMaterial* pMaterial = pScene->m_vShader[m];

		V3D_3_1_1_Material &CurMaterial = MaterialList.Material[m];
		
		strcpy(CurMaterial.Name, pMaterial->name);
		CurMaterial.Dif_Col = D3DCOLOR_RGBA((INT)(pMaterial->d3dmtrl.Diffuse.r * 255),
														 (INT)(pMaterial->d3dmtrl.Diffuse.g * 255),
														 (INT)(pMaterial->d3dmtrl.Diffuse.b * 255),
														 (INT)(pMaterial->d3dmtrl.Diffuse.a * 255));
		CurMaterial.Amb_Col = D3DCOLOR_RGBA((INT)(pMaterial->d3dmtrl.Ambient.r * 255),
														 (INT)(pMaterial->d3dmtrl.Ambient.g * 255),
														 (INT)(pMaterial->d3dmtrl.Ambient.b * 255),
														 (INT)(pMaterial->d3dmtrl.Ambient.a * 255));
		CurMaterial.Spe_Col = D3DCOLOR_RGBA((INT)(pMaterial->specular.r * 255),
														 (INT)(pMaterial->specular.g * 255),
														 (INT)(pMaterial->specular.b * 255),
														 (INT)(pMaterial->specular.a * 255));
		CurMaterial.SpeLevel = pMaterial->specularLevel;

		CurMaterial.Power = pMaterial->d3dmtrl.Power;

		CurMaterial.Illumination = pMaterial->selfIllumination;
		
		CurMaterial.Opacity = pMaterial->opacity;				//͸����

		CurMaterial.TwoSide = pMaterial->twoSide;

		CurMaterial.AlphaTest = pMaterial->bAlphaTest;

		CurMaterial.FillMode = pMaterial->fillMode;

		CurMaterial.AlphaTestValue = pMaterial->AlphaTestValue; // fuxb 2007-10-14 ����AlphaTestValue

		//����ͼ������������
		//diffuse
		if (pMaterial->diffuseTex != NULL)
			CurMaterial.DifTexID = pMaterial->diffuseTex->index;
		else
			CurMaterial.DifTexID = -1;
		//opacity
		if (pMaterial->opacityTex != NULL)
			CurMaterial.OpaTexID = pMaterial->opacityTex->index;
		else
			CurMaterial.OpaTexID = -1;
		//lightmap
		if (pMaterial->lightTex != NULL)
			CurMaterial.LightTexID = pMaterial->lightTex->index;
		else
			CurMaterial.LightTexID = -1;

		//reflection
		if (pMaterial->reflectionTex != NULL)
			CurMaterial.ReflectionTexID = pMaterial->reflectionTex->index;
		else
			CurMaterial.ReflectionTexID = -1;

		//specular
		if (pMaterial->specularTex != NULL)
			CurMaterial.SpecularTexID = pMaterial->specularTex->index;
		else
			CurMaterial.SpecularTexID = -1;
		//bumpmap
		if (pMaterial->bumpTex != NULL)
			CurMaterial.BumpTexID = pMaterial->bumpTex->index;
		else
			CurMaterial.BumpTexID = -1;

		//---------------------���ʵ�FVF---------------------------
		CurMaterial.FVF = pMaterial->m_FVF;
		CurMaterial.FVF_SIZE = pMaterial->m_FVFSize;

		//BumpWaterID
		if (pMaterial->pBumpWater != NULL)
			CurMaterial.BumpWaterID = pMaterial->pBumpWater->index;
		else
			CurMaterial.BumpWaterID = -1;

		//CubeMapID
		if (pMaterial->pCubeMap != NULL)
			CurMaterial.CubeMapID = pMaterial->pCubeMap->index;
		else
			CurMaterial.CubeMapID = -1;
		//cubeMap��ǿ��
		CurMaterial.CubeMapPower = pMaterial->cubeMapPower;

		//SpecularTexID
		if (pMaterial->pSpecularCubeMap != NULL)
			CurMaterial.SpecularCubeMapID = pMaterial->pSpecularCubeMap->index;
		else
			CurMaterial.SpecularCubeMapID = -1;

		//MovieTexID
		if (pMaterial->pMovieTexture != NULL)
			CurMaterial.MovieTexID = ((CMovieTexture*)(pMaterial->pMovieTexture))->index;
		else
			CurMaterial.MovieTexID = -1;

		//DyTexID
		if (pMaterial->dynamicTex != NULL)
			CurMaterial.DyTexID = pMaterial->dynamicTex->index;
		else
			CurMaterial.DyTexID = -1;


		//д���������
		// fwrite(&(CurMaterial), sizeof(CurMaterial), 1, pFile);
		fwrite(&(CurMaterial), sizeof(V3D_3_1_1_Material), 1, pFile);		
	}
	
	//release memory
	SAFE_DELETE_ARRAY(MaterialList.Material);

}

//-------------------------------------------------------------------
//����: WriteLightData()
//����: 
//��;: д��ƹ���Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC744001F
VOID CSaveV3D_3_1::WriteLightData()
{
	//�õ��ƹ�����
	LightData.LightCnt = pScene->light.size();
	//��¼�ƹ����ݵ�ƫ���� 
	UINT tPos = ftell(pFile);
	Header.FileTable.LightDataPtr = tPos;

	//д��ƹ�����
	fwrite(&(LightData.LightCnt), sizeof(LightData.LightCnt), 1, pFile);

	//д��ƹ���ϸ����  // fuxb 2007-10-16 û��д��Ƶĸ߹����
	LightData.Lights = new V3D_3_1_Light[LightData.LightCnt];
	for (UINT i = 0; i< LightData.LightCnt; i++)
	{
		CLight* pLight = pScene->light[i];
		//��ǰ�ƹ�Ļ�����Ϣ
		strcpy(LightData.Lights[i].Name, pLight->name);
		LightData.Lights[i].Type = (UINT)pLight->param.Type;
		LightData.Lights[i].Pos = pLight->param.Position;
		LightData.Lights[i].Dir = pLight->param.Direction;
		LightData.Lights[i].Diffuse = pLight->diffuse;
		LightData.Lights[i].Ambient = pLight->ambient; //�ƹ⻷��ɫ�� /*<2007-2-24����������>*/
		LightData.Lights[i].Multiply = pLight->multiply; //�ƹ�ǿ�ȱ����� /*<2007-2-24����������>*/
		LightData.Lights[i].Range = pLight->param.Range;
		LightData.Lights[i].Theta = pLight->param.Theta;
		LightData.Lights[i].Phi = pLight->param.Phi;
		LightData.Lights[i].Attenuation0 = pLight->param.Attenuation0;/*<2007-2-24����������>*/
		LightData.Lights[i].Attenuation1 = pLight->param.Attenuation1;/*<2007-2-24����������>*/
		LightData.Lights[i].Attenuation2 = pLight->param.Attenuation2;/*<2007-2-24����������>*/
		LightData.Lights[i].NumFrame = pLight->kfrCnt;

		//д�뵽E3D�ļ�
		fwrite(&(LightData.Lights[i].Name), sizeof(LightData.Lights[i].Name), 1, pFile);
		fwrite(&(LightData.Lights[i].Type), sizeof(LightData.Lights[i].Type), 1, pFile);
		fwrite(&(LightData.Lights[i].Pos), sizeof(LightData.Lights[i].Pos), 1, pFile);
		fwrite(&(LightData.Lights[i].Dir), sizeof(LightData.Lights[i].Dir), 1, pFile);
		fwrite(&(LightData.Lights[i].Diffuse), sizeof(LightData.Lights[i].Diffuse), 1, pFile);
		fwrite(&(LightData.Lights[i].Ambient), sizeof(LightData.Lights[i].Ambient), 1, pFile);
		fwrite(&(LightData.Lights[i].Multiply), sizeof(LightData.Lights[i].Multiply), 1, pFile);
		fwrite(&(LightData.Lights[i].Range), sizeof(LightData.Lights[i].Range), 1, pFile);
		fwrite(&(LightData.Lights[i].Theta), sizeof(LightData.Lights[i].Theta), 1, pFile);
		fwrite(&(LightData.Lights[i].Phi), sizeof(LightData.Lights[i].Phi), 1, pFile);
		fwrite(&(LightData.Lights[i].Attenuation0), sizeof(LightData.Lights[i].Attenuation0), 1, pFile);
		fwrite(&(LightData.Lights[i].Attenuation1), sizeof(LightData.Lights[i].Attenuation1), 1, pFile);
		fwrite(&(LightData.Lights[i].Attenuation2), sizeof(LightData.Lights[i].Attenuation2), 1, pFile);
		fwrite(&(LightData.Lights[i].NumFrame), sizeof(LightData.Lights[i].NumFrame), 1, pFile);

		//�ؼ�֡������Ϣ
		LightData.Lights[i].NumFrame = pLight->kfrCnt;
		if (pLight->kfrCnt > 1)
		{
			LightData.Lights[i].kfrInfo = new V3D_3_1_LightKfrInfo[pLight->kfrCnt];
			for (UINT j = 0; j< pLight->kfrCnt; j++)
			{
				LightData.Lights[i].kfrInfo[j].Pos = pLight->kfrInfo[j].Pos;
				LightData.Lights[i].kfrInfo[j].Dir = pLight->kfrInfo[j].Dir;
				LightData.Lights[i].kfrInfo[j].Diffuse = pLight->kfrInfo[j].Diffuse;
				fwrite(&(LightData.Lights[i].kfrInfo[j]), sizeof(LightData.Lights[i].kfrInfo[j]), 1, pFile);
			}	
			SAFE_DELETE_ARRAY(LightData.Lights[i].kfrInfo);
		}
	}
	SAFE_DELETE_ARRAY(LightData.Lights);
}

//-------------------------------------------------------------------
//����: WriteModelData()
//����: 
//��;: д��ģ����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74400BB
VOID CSaveV3D_3_1::WriteModelData()
{
	//�õ�ģ�͵�����
	ModelData.ModelCnt = pScene->model.size();

	//����ģ�����ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.ModelDataPtr = tPos;

	//д��ģ�͵�����
	fwrite(&(ModelData.ModelCnt), sizeof(ModelData.ModelCnt), 1, pFile);

	ModelData.Model = new V3D_3_1_Model[ModelData.ModelCnt];
	UINT tVertexOffset = 0;
	//�õ�ģ������
	for(UINT modelIndex= 0; modelIndex< ModelData.ModelCnt; modelIndex++)
	{
		/*
		//show complete percent-------------------------------------------
		char string[64];
		strcpy(string, "WriteModelData..., ");
		INT nPercent = (1.0f * modelIndex/ModelData.ModelCnt) * 100;
		char percent[4];
		itoa(nPercent, percent, 10);
		strcat(string, percent);
		strcat(string, "% completed.");
		SetWindowText(GetActiveWindow(), string);
		//show complete percent-------------------------------------------
		*/

		CModel* pModel = *(pScene->model.begin() + modelIndex);

		//name
		strcpy(ModelData.Model[modelIndex].Name, pModel->name);
		fwrite(&(ModelData.Model[modelIndex].Name), sizeof(ModelData.Model[modelIndex].Name), 1, pFile);

		//matrix
		if (pModel->kfrCnt > 0)
			ModelData.Model[modelIndex].Matrix = pModel->matrix_kfr[0];
		else
			ModelData.Model[modelIndex].Matrix = pModel->transform.matrix;
		fwrite(&(ModelData.Model[modelIndex].Matrix), sizeof(ModelData.Model[modelIndex].Matrix), 1, pFile);

		//pivot
		ModelData.Model[modelIndex].Pivot = pModel->pivot;
		fwrite(&(ModelData.Model[modelIndex].Pivot), sizeof(ModelData.Model[modelIndex].Pivot), 1, pFile);
	
		//save model ID1
		ModelData.Model[modelIndex].ID1 = pModel->ID1;
		fwrite(&(ModelData.Model[modelIndex].ID1), sizeof(ModelData.Model[modelIndex].ID1), 1, pFile);
	
		//save model ID2
		ModelData.Model[modelIndex].ID2 = pModel->ID2;
		fwrite(&(ModelData.Model[modelIndex].ID2), sizeof(ModelData.Model[modelIndex].ID2), 1, pFile);
	
		//reference
		if (pModel->refModel == NULL)
			ModelData.Model[modelIndex].RefModelIndex = -1;
		else
			ModelData.Model[modelIndex].RefModelIndex = pModel->refModel->index;
		fwrite(&(ModelData.Model[modelIndex].RefModelIndex), sizeof(ModelData.Model[modelIndex].RefModelIndex), 1, pFile);
	

		//���������ģ�ͣ����������ݲ�д��
		if (ModelData.Model[modelIndex].RefModelIndex == -1)
		{
			//bounding box
			ModelData.Model[modelIndex].OriBBox.vMin = pModel->oriBBox.MinCorner;
			ModelData.Model[modelIndex].OriBBox.vMax = pModel->oriBBox.MaxCorner;
			fwrite(&(ModelData.Model[modelIndex].OriBBox), sizeof(ModelData.Model[modelIndex].OriBBox), 1, pFile);

			//material ID
			ModelData.Model[modelIndex].MaterialID = pModel->shader->index;
			fwrite(&(ModelData.Model[modelIndex].MaterialID), sizeof(ModelData.Model[modelIndex].MaterialID), 1, pFile);
		
			//vertex offset
			ModelData.Model[modelIndex].VertexOffset= tVertexOffset;
			fwrite(&(ModelData.Model[modelIndex].VertexOffset), sizeof(ModelData.Model[modelIndex].VertexOffset), 1, pFile);
		
			//vertex count
			ModelData.Model[modelIndex].VertexCnt= pModel->vertexCnt;
			tVertexOffset += pModel->vertexCnt;
			fwrite(&(ModelData.Model[modelIndex].VertexCnt), sizeof(ModelData.Model[modelIndex].VertexCnt), 1, pFile);
		
			//face count
			ModelData.Model[modelIndex].FaceCnt = pModel->faceCnt;
			fwrite(&(ModelData.Model[modelIndex].FaceCnt), sizeof(ModelData.Model[modelIndex].FaceCnt), 1, pFile);
		
			//face list
			ModelData.Model[modelIndex].Face = new V3D_3_1_Face[ModelData.Model[modelIndex].FaceCnt];
			for (UINT n=0; n< ModelData.Model[modelIndex].FaceCnt; n++)
			{
				//a
				ModelData.Model[modelIndex].Face[n].a = pModel->face[n].vertices0->index;
				fwrite(&(ModelData.Model[modelIndex].Face[n].a), sizeof(ModelData.Model[modelIndex].Face[n].a), 1, pFile);
			
				//b
				ModelData.Model[modelIndex].Face[n].b = pModel->face[n].vertices1->index;
				fwrite(&(ModelData.Model[modelIndex].Face[n].b), sizeof(ModelData.Model[modelIndex].Face[n].b), 1, pFile);
			
				//c
				ModelData.Model[modelIndex].Face[n].c = pModel->face[n].vertices2->index;
				fwrite(&(ModelData.Model[modelIndex].Face[n].c), sizeof(ModelData.Model[modelIndex].Face[n].c), 1, pFile);
			
			}
			//release memory
			SAFE_DELETE_ARRAY(ModelData.Model[modelIndex].Face);

			//keyframe
			ModelData.Model[modelIndex].KeyFrameCnt = pModel->kfrCnt;
			ModelData.Model[modelIndex].KfrInfo = new D3DXMATRIX[pModel->kfrCnt];
			fwrite(&(ModelData.Model[modelIndex].KeyFrameCnt), sizeof(ModelData.Model[modelIndex].KeyFrameCnt), 1, pFile);
		
			//keyframe Data
			for (UINT k = 0; k< ModelData.Model[modelIndex].KeyFrameCnt; k++)
			{
				//frame data
				ModelData.Model[modelIndex].KfrInfo[k] = pModel->matrix_kfr[k];
				fwrite(&(ModelData.Model[modelIndex].KfrInfo[k]), sizeof(ModelData.Model[modelIndex].KfrInfo[k]), 1, pFile);
			}
			//release memory
			SAFE_DELETE_ARRAY(ModelData.Model[modelIndex].KfrInfo);

			fwrite(&(pModel->vkfrCnt), sizeof(pModel->vkfrCnt), 1, pFile);
			if (pModel->vkfrCnt > 0)
			{
				//vkeyframe
				ModelData.Model[modelIndex].VertexPosList = new V3D_3_1_VertexPosList[pModel->vkfrCnt];
			
				//vkeyframeData
				for (k = 0; k< pModel->vkfrCnt; k++)
				{
					ModelData.Model[modelIndex].VertexPosList[k].PosList = new D3DXVECTOR3[ModelData.Model[modelIndex].VertexCnt];
					for (UINT z=0; z< ModelData.Model[modelIndex].VertexCnt; z++)
					{
						ModelData.Model[modelIndex].VertexPosList[k].PosList[z] = pModel->vertices_kfr[k][z].position;
						fwrite(&(ModelData.Model[modelIndex].VertexPosList[k].PosList[z]), sizeof(ModelData.Model[modelIndex].VertexPosList[k].PosList[z]), 1, pFile);
					}
				}	
				//release memory
				SAFE_DELETE_ARRAY(ModelData.Model[modelIndex].VertexPosList);
			}

			//current model is Billboard model?
			ModelData.Model[modelIndex].IsBillboard = pModel->isBillboard;
			fwrite(&(ModelData.Model[modelIndex].IsBillboard), sizeof(ModelData.Model[modelIndex].IsBillboard), 1, pFile);
		
			//current model is Collision model?
			ModelData.Model[modelIndex].IsCollision = pModel->isCollision;
			fwrite(&(ModelData.Model[modelIndex].IsCollision), sizeof(ModelData.Model[modelIndex].IsCollision), 1, pFile);
		
			//current model is Ground model?
			ModelData.Model[modelIndex].IsGround = pModel->isGround;
			fwrite(&(ModelData.Model[modelIndex].IsGround), sizeof(ModelData.Model[modelIndex].IsGround), 1, pFile);

			//current model is skyBox?
			ModelData.Model[modelIndex].IsSkyBox = pModel->isSkyBox;
			fwrite(&(ModelData.Model[modelIndex].IsSkyBox), sizeof(ModelData.Model[modelIndex].IsSkyBox), 1, pFile);

			//current model is Visible?
			ModelData.Model[modelIndex].IsVisible = pModel->visible;
			fwrite(&(ModelData.Model[modelIndex].IsVisible), sizeof(ModelData.Model[modelIndex].IsVisible), 1, pFile);

			//current model is Clickable?
			ModelData.Model[modelIndex].IsClickable = pModel->clickable;
			fwrite(&(ModelData.Model[modelIndex].IsClickable), sizeof(ModelData.Model[modelIndex].IsClickable), 1, pFile);

			//�Ƿ�ʹ�ö�̬UV
			ModelData.Model[modelIndex].UseDynamicUV = pModel->bDynamicUV;
			fwrite(&(ModelData.Model[modelIndex].UseDynamicUV), sizeof(ModelData.Model[modelIndex].UseDynamicUV), 1, pFile);

			//U Step
			ModelData.Model[modelIndex].UStep = pModel->UStep;
			fwrite(&(ModelData.Model[modelIndex].UStep), sizeof(ModelData.Model[modelIndex].UStep), 1, pFile);

			//V Step
			ModelData.Model[modelIndex].VStep = pModel->VStep;
			fwrite(&(ModelData.Model[modelIndex].VStep), sizeof(ModelData.Model[modelIndex].UStep), 1, pFile);

		}
	}

	//release memory
	SAFE_DELETE_ARRAY(ModelData.Model);
}

//-------------------------------------------------------------------
//����: WriteVertexPosData()
//����: 
//��;: д�붥��λ����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC744003E
VOID CSaveV3D_3_1::WriteVertexPosData(BOOL zipStatus)
{
	//���涥��λ�����ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.VertexPosDataPtr = tPos;

	//ѹ������λ������
	ZipVertexPosData(zipStatus);
	fwrite(&VertexPosData.VertexPosCnt, sizeof(VertexPosData.VertexPosCnt), 1, pFile);
	//write vertex pos data
	fwrite(VertexPosData.PosList, VertexPosData.VertexPosCnt * sizeof(D3DXVECTOR3), 1, pFile);
}


//-------------------------------------------------------------------
//����: WriteVertexNormalData()
//����: 
//��;: д�붥�㷨����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC744005D
VOID CSaveV3D_3_1::WriteVertexNormalData(BOOL zipStatus)
{
	//���涥�㷨�����ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.VertexNormalDataPtr = tPos;

	//ѹ�����㷨������
	ZipVertexNormalData(zipStatus);
	fwrite(&VertexNormalData.VertexNormalCnt, sizeof(VertexNormalData.VertexNormalCnt), 1, pFile);
	//write vertex pos data
	fwrite(VertexNormalData.NormalList, VertexNormalData.VertexNormalCnt * sizeof(D3DXVECTOR3), 1, pFile);
}

//-------------------------------------------------------------------
//����: WriteVetexData()
//����: 
//��;: д�붥����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC744009C
VOID CSaveV3D_3_1::WriteVertexData(BOOL zipStatus)
{
	//���涥�����ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.VertexDataPtr = tPos;
	
	//vertex count
	fwrite(&(VertexData.VertexCnt), sizeof(VertexData.VertexCnt), 1, pFile);

	//vertex data
	VertexData.Vertex = new V3D_3_1_Vertex[VertexData.VertexCnt];
	UINT index = 0;
	INT modelCnt = pScene->model.size();
	for (UINT i = 0; i< modelCnt; i++)
	{
		CModel* pModel = *(pScene->model.begin() + i);
		for (UINT j = 0; j< pModel->vertexCnt; j++)
		{
			//�ҵ��ö���λ����VertexPosData�е�ƫ����
			UINT posOffset = 0;
			if(zipStatus)
				posOffset = FindOffsetInVertexPosData(i, j);
			else
				posOffset = index;

			if (posOffset > VertexPosData.VertexPosCnt -1)
				MessageBox(GetActiveWindow(), "����λ������ѹ������δ֪�����޷������ļ���", "����λ��ѹ��...����", MB_OK | MB_ICONERROR);
	
			//vertex data pos offset
			VertexData.Vertex[index].indexInVPList = posOffset;
			fwrite(&(VertexData.Vertex[index].indexInVPList ), sizeof(VertexData.Vertex[index].indexInVPList ), 1, pFile);	
				
			//�ҵ��ö��㷨����VertexNormalData�е�ƫ����
			UINT normalOffset = 0;
			if (zipStatus)
				normalOffset = FindOffsetInVertexNormalData(i, j);
			else
				normalOffset = index;

			if (normalOffset >  VertexNormalData.VertexNormalCnt -1)
				MessageBox(GetActiveWindow(), "���㷨������ѹ������δ֪�����޷������ļ���", "���㷨��ѹ��...����", MB_OK | MB_ICONERROR);
	
			//vertex data normal offset
			VertexData.Vertex[index].indexInVNList = normalOffset;
			fwrite(&(VertexData.Vertex[index].indexInVNList ), sizeof(VertexData.Vertex[index].indexInVNList ), 1, pFile);

			//�ҵ��ö���UV��UVData�е�ƫ����
			UINT uvOffset = 0;
			if (zipStatus)
				uvOffset = FindOffsetInUVData(i, j);
			else
				uvOffset = index;

			if (uvOffset > UVData.UVCount -1)
				MessageBox(GetActiveWindow(), "����UV����ѹ������δ֪�����޷������ļ���", "����UVѹ��...����", MB_OK | MB_ICONERROR);

			VertexData.Vertex[index].indexInUVList = uvOffset;
			fwrite(&(VertexData.Vertex[index].indexInUVList ), sizeof(VertexData.Vertex[index].indexInUVList ), 1, pFile);
			index++;
		}

		if (zipStatus)
		{
			//show complete percent-------------------------------------------
			if (i % 10 == 0)
			{
				char string[64];
				strcpy(string, "Write Vertex Data..., ");
				INT nPercent = (1.0f * i/modelCnt) * 100;
				char percent[4];
				itoa(nPercent, percent, 10);
				strcat(string, percent);
				strcat(string, "% completed.");
				SetWindowText(GetActiveWindow(), string);
			}
			//show complete percent-------------------------------------------
		}
	}

	//release memory
	SAFE_DELETE_ARRAY(VertexData.Vertex);
	SAFE_DELETE_ARRAY(VertexPosData.PosList);
	SAFE_DELETE_ARRAY(VertexNormalData.NormalList);
	SAFE_DELETE_ARRAY(UVData.UVList);
}

//-------------------------------------------------------------------
//����: WriteUVData()
//����: 
//��;: д����ͼ������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC744007D
VOID CSaveV3D_3_1::WriteUVData(BOOL zipStatus)
{
	//���涥����ͼ�������ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.UVDataPtr = tPos;

	//ѹ������UV����
	ZipUVData(zipStatus);
	fwrite(&(UVData.UVCount), sizeof(UVData.UVCount), 1, pFile);
	//write vertex uv data
	fwrite(UVData.UVList, UVData.UVCount * sizeof(V3D_3_1_UV), 1, pFile);
}

//-------------------------------------------------------------------
//����: WriteTexData()
//����: 
//��;: д����ͼ��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC744015A
VOID CSaveV3D_3_1::WriteTexData()
{
	//������ͼ���ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.TexDataPtr = tPos;

	TexData.TexCnt = pScene->texture.size();	
	//texture count
	fwrite(&(TexData.TexCnt), sizeof(TexData.TexCnt), 1, pFile);

	//texture data
	TexData.Textures= new V3D_3_1_Texture[TexData.TexCnt];
	for (UINT i= 0; i< TexData.TexCnt; i++)
	{

		CTexture* pTex = pScene->texture[i];
		if (pTex == NULL)
		{
			::MessageBox(GetActiveWindow(), "Some texture file lost, you can't save the E3D file correctly.", "Save E3D File Error", MB_OK|MB_ICONERROR);
			return;
		}

		//������ͼ����----------------------------------
		//��ͼ��
		strcpy(TexData.Textures[i].Name, pTex->name);
		strcpy(TexData.Textures[i].FileName, pTex->pathAndName);

		//��ͼ����
		TexData.Textures[i].Width = pTex->width;       //�����͸����ͼʵ����д�����Diffuse�ĸ߶ȺͿ�ȣ����ڱ��浽N3D�д����óߴ�͸����ͼ
		TexData.Textures[i].Height = pTex->height;
		TexData.Textures[i].MipCount = pTex->MipCount;
		TexData.Textures[i].Format = pTex->BP;
		TexData.Textures[i].Brightness = pTex->brightness;
		TexData.Textures[i].Contrast = pTex->contrast;

		//write tex data
		fwrite(&(TexData.Textures[i].Name), sizeof(TexData.Textures[i].Name), 1, pFile);
		fwrite(&(TexData.Textures[i].FileName), sizeof(TexData.Textures[i].FileName), 1, pFile);
		fwrite(&(TexData.Textures[i].Width), sizeof(TexData.Textures[i].Width), 1, pFile);
		fwrite(&(TexData.Textures[i].Height), sizeof(TexData.Textures[i].Height), 1, pFile);
		fwrite(&(TexData.Textures[i].MipCount), sizeof(TexData.Textures[i].MipCount), 1, pFile);
		fwrite(&(TexData.Textures[i].Format), sizeof(TexData.Textures[i].Format), 1, pFile);
	
		fwrite(&(TexData.Textures[i].Brightness), sizeof(TexData.Textures[i].Brightness), 1, pFile);	
		fwrite(&(TexData.Textures[i].Contrast), sizeof(TexData.Textures[i].Contrast), 1, pFile);
	}

	//release memory
	SAFE_DELETE_ARRAY(TexData.Textures);

}

//-------------------------------------------------------------------
//����: WriteTextData()
//����: 
//��;: д��������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74400EB
VOID CSaveV3D_3_1::WriteTextData()
{
	//�����������ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.TextDataPtr = tPos;

	TextData.TextCnt = pScene->text.size();	
	//text count
	fwrite(&(TextData.TextCnt), sizeof(TextData.TextCnt), 1, pFile);


	//�����������Ϊ0������Ա������������б��ֶ�
	if (TextData.TextCnt == 0) return;

	//text data
	TextData.Texts = new V3D_3_1_Text[TextData.TextCnt];
	for (UINT i= 0; i< TextData.TextCnt; i++)
	{
		CText* pText = pScene->text[i];
		
		//д����������
		TextData.Texts[i].Length = GetStrLength(pText->strText);
		fwrite(&(TextData.Texts[i].Length), sizeof(TextData.Texts[i].Length), 1, pFile);
	
		//����ַ���Ϊ�գ�����Ա����ַ�����StrText�ֶ�
		if (TextData.Texts[i].Length > 0) 
			fwrite(pText->strText, TextData.Texts[i].Length, 1, pFile);

		TextData.Texts[i].X = pText->rect.left;
		fwrite(&(TextData.Texts[i].X), sizeof(TextData.Texts[i].X), 1, pFile);

		TextData.Texts[i].Y = pText->rect.top;
		fwrite(&(TextData.Texts[i].Y), sizeof(TextData.Texts[i].Y), 1, pFile);

		TextData.Texts[i].Color = pText->color;
		fwrite(&(TextData.Texts[i].Color), sizeof(TextData.Texts[i].Color), 1, pFile);

		//������
		pText->m_pFont->GetFont(TextData.Texts[i].FontName);
		fwrite(&(TextData.Texts[i].FontName), sizeof(TextData.Texts[i].FontName), 1, pFile);
	
		//�ؼ�֡֡��
		TextData.Texts[i].KfrCnt = pText->m_kfrCnt;
		fwrite(&(TextData.Texts[i].KfrCnt), sizeof(TextData.Texts[i].KfrCnt), 1, pFile);
		//�ؼ�֡����
		if (pText->m_kfrCnt > 0)
		{
			TextData.Texts[i].KfrInfo = new V3D_3_1_TextInfo[pText->m_kfrCnt];
			for (INT j = 0; j< pText->m_kfrCnt; j++)
			{
				TextData.Texts[i].KfrInfo[j].X = pText->m_kfrInfo[j].x;
				TextData.Texts[i].KfrInfo[j].Y = pText->m_kfrInfo[j].y;
				TextData.Texts[i].KfrInfo[j].Color = pText->m_kfrInfo[j].color;
				fwrite(&(TextData.Texts[i].KfrInfo[j]), sizeof(TextData.Texts[i].KfrInfo[j]), 1, pFile);
			}
			SAFE_DELETE_ARRAY(TextData.Texts[i].KfrInfo);
		}
		
	}

	//release memory
	SAFE_DELETE_ARRAY(TextData.Texts);
	
}

//-------------------------------------------------------------------
//����: WriteOverlayData()
//����: 
//��;: д��Overlay��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74400FA
VOID CSaveV3D_3_1::WriteOverlayData()
{
	//����Overlay���ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.OverlayDataPtr = tPos;

	OverlayData.OverlayCnt = pScene->overlay.size();	
	//overlay count
	fwrite(&(OverlayData.OverlayCnt), sizeof(OverlayData.OverlayCnt), 1, pFile);

	//���Overlay������Ϊ0,����Ա���Ovelay�����б��ֶ�
	if (OverlayData.OverlayCnt == 0) return;

	//overlay data
	OverlayData.Overlays = new V3D_3_1_Overlay[OverlayData.OverlayCnt];
	for (UINT i= 0; i< OverlayData.OverlayCnt; i++)
	{
		COverlay* pOverlay = pScene->overlay[i];
		
		//д��Overlay������
		strcpy(OverlayData.Overlays[i].Name, pOverlay->name);
		fwrite(&(OverlayData.Overlays[i].Name), sizeof(OverlayData.Overlays[i].Name), 1, pFile);

		//д��Overlay��ƫ������ԭ���ʾ
		OverlayData.Overlays[i].OffsetOrigin = pOverlay->offsetOrigin;
		fwrite(&(OverlayData.Overlays[i].OffsetOrigin), sizeof(OverlayData.Overlays[i].OffsetOrigin), 1, pFile);
		
		//д��Overlay��λ�ñ�ʾ(������껹�Ǿ�������)
		OverlayData.Overlays[i].UseRelativePos = pOverlay->useRelativePos;
		fwrite(&(OverlayData.Overlays[i].UseRelativePos), sizeof(OverlayData.Overlays[i].UseRelativePos), 1, pFile);
		
		//д��Overlay�ߴ�, ��¼����ƫ�Ƴߴ磬���Ǿ��Գߴ�
		OverlayData.Overlays[i].Rect = pOverlay->offsetRect;
		fwrite(&(OverlayData.Overlays[i].Rect), sizeof(OverlayData.Overlays[i].Rect), 1, pFile);

		//д��Overlayת��
		FLOAT angle = pOverlay->GetRotation();
		OverlayData.Overlays[i].Angle = angle;
		fwrite(&(OverlayData.Overlays[i].Angle), sizeof(OverlayData.Overlays[i].Angle), 1, pFile);

		//д��Overlay��͸����
		INT opacity = pOverlay->GetOpacity();
		OverlayData.Overlays[i].Opacity = opacity;
		fwrite(&(OverlayData.Overlays[i].Opacity), sizeof(OverlayData.Overlays[i].Opacity), 1, pFile);

		//д��Overlay�Ƿ�Ϊbackdrop
		OverlayData.Overlays[i].IsBackdrop = pOverlay->isBackdrop;
		fwrite(&(OverlayData.Overlays[i].IsBackdrop), sizeof(OverlayData.Overlays[i].IsBackdrop), 1, pFile);

		//д��Overlay�Ĳ���ID
		OverlayData.Overlays[i].MaterialID = (pOverlay->GetMaterial())->index;
		fwrite(&(OverlayData.Overlays[i].MaterialID), sizeof(OverlayData.Overlays[i].MaterialID), 1, pFile);

		//д��Overlay�Ƿ�ʹ��ͼ��ߴ�
		OverlayData.Overlays[i].UseRelativeSize = pOverlay->useRelativeSize;
		fwrite(&(OverlayData.Overlays[i].UseRelativeSize), sizeof(OverlayData.Overlays[i].UseRelativeSize), 1, pFile);
		
		//д��Overlay�Ĺؼ�֡����
		OverlayData.Overlays[i].KfrCnt = pOverlay->m_kfrCnt;
		fwrite(&(OverlayData.Overlays[i].KfrCnt), sizeof(OverlayData.Overlays[i].KfrCnt), 1, pFile);
		
		//д��ؼ�֡����
		if (pOverlay->m_kfrCnt > 0)
		{
			OverlayData.Overlays[i].KfrInfo = new V3D_3_1_OverlayInfo[pOverlay->m_kfrCnt];
			for (INT j = 0; j< pOverlay->m_kfrCnt; j++)
			{
				OverlayData.Overlays[i].KfrInfo[j].Rect = pOverlay->m_kfrInfo[j].Rect;
				OverlayData.Overlays[i].KfrInfo[j].Angle = pOverlay->m_kfrInfo[j].Angle;
				OverlayData.Overlays[i].KfrInfo[j].Opacity = pOverlay->m_kfrInfo[j].Opacity;
				fwrite(&(OverlayData.Overlays[i].KfrInfo[j]), sizeof(OverlayData.Overlays[i].KfrInfo[j]), 1, pFile);
			}
			SAFE_DELETE_ARRAY(OverlayData.Overlays[i].KfrInfo);
		}
	}

	//release memory
	SAFE_DELETE_ARRAY(OverlayData.Overlays);
}

//-------------------------------------------------------------------
//����: WriteNavigaterData
//����: 
//��;: д��Navigater��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC7440119
VOID CSaveV3D_3_1::WriteNavigaterData()
{
	//����Navigater���ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.NavigaterDataPtr = tPos;

	NavigaterData.NavigaterCnt = pScene->navigater.size();	
	//overlay count
	fwrite(&(NavigaterData.NavigaterCnt), sizeof(NavigaterData.NavigaterCnt), 1, pFile);

	//���Navigater������Ϊ0,����Ա���Navigate�����б��ֶ�
	if (NavigaterData.NavigaterCnt == 0) return;

	//navigate data
	NavigaterData.Navigaters = new V3D_3_1_Navigater[NavigaterData.NavigaterCnt];
	for (UINT i= 0; i< NavigaterData.NavigaterCnt; i++)
	{
		CNavigater* pNavigater = pScene->navigater[i];
		
		//д��navigate������
		strcpy(NavigaterData.Navigaters[i].Name, pNavigater->name);
		fwrite(&(NavigaterData.Navigaters[i].Name), sizeof(NavigaterData.Navigaters[i].Name), 1, pFile);

		//д��navigate��navRect
		NavigaterData.Navigaters[i].Rect = pNavigater->navRect;
		fwrite(&(NavigaterData.Navigaters[i].Rect), sizeof(NavigaterData.Navigaters[i].Rect), 1, pFile);
		
		//д��navigate��mapOverlay��Index
		NavigaterData.Navigaters[i].MapOverlayIndex = pNavigater->pNavigateMap->index;
		fwrite(&(NavigaterData.Navigaters[i].MapOverlayIndex), sizeof(NavigaterData.Navigaters[i].MapOverlayIndex), 1, pFile);
		
		//д��Pointer������
		INT pointerCnt = pNavigater->m_NavPointer.size();
		NavigaterData.Navigaters[i].PointerCnt = pointerCnt;
		fwrite(&(NavigaterData.Navigaters[i].PointerCnt), sizeof(NavigaterData.Navigaters[i].PointerCnt), 1, pFile);
		
		//д��Pointer����
		if (pointerCnt > 0)
		{
			NavigaterData.Navigaters[i].NavPointer = new V3D_3_1_NavigaterPointer[pointerCnt];
			for (INT j = 0; j<pointerCnt; j++)
			{
				//Pointer����
				strcpy(NavigaterData.Navigaters[i].NavPointer[j].Name, pNavigater->m_NavPointer[j]->Name);

				//Pointer Overlay���
				NavigaterData.Navigaters[i].NavPointer[j].OverlayIndex = pNavigater->m_NavPointer[j]->pPointerOverlay->index;
				
				//Pointer����Ķ�������
				NavigaterData.Navigaters[i].NavPointer[j].Type = pNavigater->m_NavPointer[j]->NavPointerType;
			
				//Pointer����Ķ���ı��
				CModel* pModel = NULL;
				CCamera* pCamera = NULL;
				if (pNavigater->m_NavPointer[j]->NavPointerType == 0)
				{
					pCamera = (CCamera*)pNavigater->m_NavPointer[j]->NavPointerHandle;
					if (pCamera)
					NavigaterData.Navigaters[i].NavPointer[j].ObjIndex = pCamera->index;
				}
				if (pNavigater->m_NavPointer[j]->NavPointerType == 1)
				{
					pModel = (CModel*)pNavigater->m_NavPointer[j]->NavPointerHandle;
					if (pModel)
					NavigaterData.Navigaters[i].NavPointer[j].ObjIndex = pModel->index;
				}	

				//д��Pointer����
				fwrite(&(NavigaterData.Navigaters[i].NavPointer[j]), sizeof(NavigaterData.Navigaters[i].NavPointer[j]), 1, pFile);
		
			}
			SAFE_DELETE_ARRAY(NavigaterData.Navigaters[i].NavPointer);
		}

		//д��navigate��sceneLeft
		NavigaterData.Navigaters[i].sceneLeft = pNavigater->sceneLeft;
		fwrite(&(NavigaterData.Navigaters[i].sceneLeft), sizeof(NavigaterData.Navigaters[i].sceneLeft), 1, pFile);

		//д��navigate��sceneTop
		NavigaterData.Navigaters[i].sceneTop = pNavigater->sceneTop;
		fwrite(&(NavigaterData.Navigaters[i].sceneTop), sizeof(NavigaterData.Navigaters[i].sceneTop), 1, pFile);
								
		//д��navigate��sceneRight
		NavigaterData.Navigaters[i].sceneRight = pNavigater->sceneRight;
		fwrite(&(NavigaterData.Navigaters[i].sceneRight), sizeof(NavigaterData.Navigaters[i].sceneRight), 1, pFile);
													
		//д��navigate��sceneBottom
		NavigaterData.Navigaters[i].sceneBottom = pNavigater->sceneBottom;
		fwrite(&(NavigaterData.Navigaters[i].sceneBottom), sizeof(NavigaterData.Navigaters[i].sceneBottom), 1, pFile);
				
	}
	//release memory
	SAFE_DELETE_ARRAY(NavigaterData.Navigaters);
}

//-------------------------------------------------------------------
//����: WriteButtonData()
//����: 
//��;: write the button data
//-------------------------------------------------------------------
//##ModelId=47EBC7440128
VOID CSaveV3D_3_1::WriteButtonData()
{
	//����Button���ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.ButtonDataPtr = tPos;

	ButtonData.ButtonCnt = pScene->button.size();	
	//overlay count
	fwrite(&(ButtonData.ButtonCnt), sizeof(ButtonData.ButtonCnt), 1, pFile);

	//���Button������Ϊ0,����Ա���Button�����б��ֶ�
	if (ButtonData.ButtonCnt == 0) return;

	//button data
	ButtonData.Buttons = new V3D_3_1_Button[ButtonData.ButtonCnt];
	for (UINT i= 0; i< ButtonData.ButtonCnt; i++)
	{
		CButton* pButton = pScene->button[i];
		
		//д��button������
		strcpy(ButtonData.Buttons[i].Name, pButton->name);
		fwrite(&(ButtonData.Buttons[i].Name), sizeof(ButtonData.Buttons[i].Name), 1, pFile);

		//д��button��Rect
		RECT btnRect;
		pButton->btnOverlay->GetRect(&btnRect);
		ButtonData.Buttons[i].Rect = btnRect;
		fwrite(&(ButtonData.Buttons[i].Rect), sizeof(ButtonData.Buttons[i].Rect), 1, pFile);
		
		//д��Button��͸����
		ButtonData.Buttons[i].Opacity = pButton->btnOverlay->GetOpacity();
		fwrite(&(ButtonData.Buttons[i].Opacity), sizeof(ButtonData.Buttons[i].Opacity), 1, pFile);

		//д��button�Ļص���������
		strcpy(ButtonData.Buttons[i].CallBackFunc, pButton->callBackFunc);
		fwrite(&(ButtonData.Buttons[i].CallBackFunc), sizeof(ButtonData.Buttons[i].CallBackFunc), 1, pFile);				
	
		//д��button��Overlay��Index
		ButtonData.Buttons[i].BtnOverlayIndex = pButton->btnOverlay->index;
		fwrite(&(ButtonData.Buttons[i].BtnOverlayIndex), sizeof(ButtonData.Buttons[i].BtnOverlayIndex), 1, pFile);
		
		//д��button��mouseOver Mtrl��Index
		if (pButton->pMtrl_MouseOver)
			ButtonData.Buttons[i].MouseOverMtrlIndex = pButton->pMtrl_MouseOver->index;
		else
			ButtonData.Buttons[i].MouseOverMtrlIndex = -1;
		fwrite(&(ButtonData.Buttons[i].MouseOverMtrlIndex), sizeof(ButtonData.Buttons[i].MouseOverMtrlIndex), 1, pFile);

		//д��button��mouseDown Mtrl��Index
		if (pButton->pMtrl_MouseDown)
			ButtonData.Buttons[i].MouseDownMtrlIndex = pButton->pMtrl_MouseDown->index;
		else
			ButtonData.Buttons[i].MouseDownMtrlIndex = -1;
		fwrite(&(ButtonData.Buttons[i].MouseDownMtrlIndex), sizeof(ButtonData.Buttons[i].MouseDownMtrlIndex), 1, pFile);

		//д��Button�Ĺؼ�֡����
		ButtonData.Buttons[i].KfrCnt = pButton->m_kfrCnt;
		fwrite(&(ButtonData.Buttons[i].KfrCnt), sizeof(ButtonData.Buttons[i].KfrCnt), 1, pFile);

		//д��Button�Ĺؼ�֡����
		if (pButton->m_kfrCnt > 0)
		{
			ButtonData.Buttons[i].KfrInfo = new V3D_3_1_ButtonInfo[pButton->m_kfrCnt];
			for (INT j = 0; j< pButton->m_kfrCnt; j++)
			{
				ButtonData.Buttons[i].KfrInfo[j].Rect = pButton->m_kfrInfo[j].Rect;
				ButtonData.Buttons[i].KfrInfo[j].Opacity = pButton->m_kfrInfo[j].Opacity;
				fwrite(&(ButtonData.Buttons[i].KfrInfo[j]), sizeof(ButtonData.Buttons[i].KfrInfo[j]), 1, pFile);		
			}
			SAFE_DELETE_ARRAY(ButtonData.Buttons[i].KfrInfo);
		}
	}

	//release memory
	SAFE_DELETE_ARRAY(ButtonData.Buttons);	
}

//-------------------------------------------------------------------
//����: WriteImageData
//����: 
//��;: д���ⲿͼ����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC7440177
VOID CSaveV3D_3_1::WriteImageData()
{
	//����ͼ����Դ���ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.ImageDataPtr = tPos;

	ImageData.ImageCount = pScene->image.size();	
	//image count
	fwrite(&(ImageData.ImageCount), sizeof(ImageData.ImageCount), 1, pFile);

	
	//���Image������Ϊ0,����Ա���Image�����б��ֶ�
	if (ImageData.ImageCount == 0) return;

	//image data
	ImageData.Images = new V3D_3_1_Image[ImageData.ImageCount];
	for (UINT i= 0; i< ImageData.ImageCount; i++)
	{
		CImage* pImage = pScene->image[i];
		
		//д��Image������
		strcpy(ImageData.Images[i].Name, pImage->name);
		fwrite(&(ImageData.Images[i].Name), sizeof(ImageData.Images[i].Name), 1, pFile);

		//д��Image�����ݳ���
		strcpy(ImageData.Images[i].FileName, pImage->pathAndName);
		fwrite(&(ImageData.Images[i].FileName), sizeof(ImageData.Images[i].FileName), 1, pFile);
		//д��Image�Ŀ��
		ImageData.Images[i].Width = pImage->width;
		fwrite(&(ImageData.Images[i].Width), sizeof(ImageData.Images[i].Width), 1, pFile);
		//д��Image�ĸ߶�
		ImageData.Images[i].Height = pImage->height;
		fwrite(&(ImageData.Images[i].Height), sizeof(ImageData.Images[i].Height), 1, pFile);
		//д��Image�ĸ�ʽ
		ImageData.Images[i].Format = pImage->format;
		fwrite(&(ImageData.Images[i].Format), sizeof(ImageData.Images[i].Format), 1, pFile);
	}

	//release memory
	SAFE_DELETE_ARRAY(ImageData.Images);
}


//-------------------------------------------------------------------
//����: WriteCubeMapData()
//����: 
//��;: д��CubeMap ����
//-------------------------------------------------------------------
//##ModelId=47EBC7440138
VOID CSaveV3D_3_1::WriteCubeMapData()
{
	INT cubeMapCnt = pScene->cubeMap.size();

	//����ģ�����ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.CubeMapDataPtr = tPos;

	//cubeMap count
	CubeMapData.CubeMapCnt = cubeMapCnt;
	fwrite(&(CubeMapData.CubeMapCnt), sizeof(CubeMapData.CubeMapCnt), 1, pFile);
	
	//���Image������Ϊ0,����Ա���CubeMap�����б��ֶ�
	if (CubeMapData.CubeMapCnt == 0) return;

	CubeMapData.CubeMap = new V3D_3_1_CubeMap[CubeMapData.CubeMapCnt];

	CCubeMap** ppCubeMap = pScene->cubeMap.begin();
	for (INT i = 0; i< cubeMapCnt; i++)
	{
		CCubeMap* pCubeMap = *(ppCubeMap + i);
	
		//д��CubeMap������
		strcpy(CubeMapData.CubeMap[i].Name, pCubeMap->name);
		fwrite(&(CubeMapData.CubeMap[i].Name), sizeof(CubeMapData.CubeMap[i].Name), 1, pFile);

		//д�� CubeMap �ĳߴ�
		CubeMapData.CubeMap[i].Length = pCubeMap->m_Size;
		fwrite(&(CubeMapData.CubeMap[i].Length), sizeof(CubeMapData.CubeMap[i].Length), 1, pFile);

		//д�� CubeMap ��6��Texture�ı��
		if (pCubeMap->m_pFrontTex)
			CubeMapData.CubeMap[i].Front_Z = pCubeMap->m_pFrontTex->index;
		else
			CubeMapData.CubeMap[i].Front_Z = -1;
		fwrite(&(CubeMapData.CubeMap[i].Front_Z), sizeof(CubeMapData.CubeMap[i].Front_Z), 1, pFile);
		
		if (pCubeMap->m_pBackTex)
			CubeMapData.CubeMap[i].Back_NZ = pCubeMap->m_pBackTex->index;
		else
			CubeMapData.CubeMap[i].Back_NZ = -1;
		fwrite(&(CubeMapData.CubeMap[i].Back_NZ), sizeof(CubeMapData.CubeMap[i].Back_NZ), 1, pFile);

		if (pCubeMap->m_pLeftTex)
			CubeMapData.CubeMap[i].Left_NX = pCubeMap->m_pLeftTex->index;
		else
			CubeMapData.CubeMap[i].Left_NX = -1;
		fwrite(&(CubeMapData.CubeMap[i].Left_NX), sizeof(CubeMapData.CubeMap[i].Left_NX), 1, pFile);
		
		if (pCubeMap->m_pRightTex)
			CubeMapData.CubeMap[i].Right_X = pCubeMap->m_pRightTex->index;
		else
			CubeMapData.CubeMap[i].Right_X = -1;
		fwrite(&(CubeMapData.CubeMap[i].Right_X), sizeof(CubeMapData.CubeMap[i].Right_X), 1, pFile);
		
		if (pCubeMap->m_pTopTex)
			CubeMapData.CubeMap[i].Top_Y = pCubeMap->m_pTopTex->index;
		else
			CubeMapData.CubeMap[i].Top_Y = -1;
		fwrite(&(CubeMapData.CubeMap[i].Top_Y), sizeof(CubeMapData.CubeMap[i].Top_Y), 1, pFile);

		if (pCubeMap->m_pBottomTex)
			CubeMapData.CubeMap[i].Bottom_NY = pCubeMap->m_pBottomTex->index;
		else
			CubeMapData.CubeMap[i].Bottom_NY = -1;
		fwrite(&(CubeMapData.CubeMap[i].Bottom_NY), sizeof(CubeMapData.CubeMap[i].Bottom_NY), 1, pFile);
		
	}
	//release memory
	SAFE_DELETE_ARRAY(CubeMapData.CubeMap);

}

//д��MovieTex��Ϣ
//##ModelId=47EBC7440139
VOID CSaveV3D_3_1::WriteMovieTexData()
{	
	INT MovieTexCnt = pScene->movieTexture.size();

	//�����Ӱ��ͼ���ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.MovieTexDataPtr = tPos;

	//MovieTexture count
	MovieTexData.MovieTexCnt = MovieTexCnt;
	fwrite(&(MovieTexData.MovieTexCnt), sizeof(MovieTexData.MovieTexCnt), 1, pFile);
	
	//���MovieTex������Ϊ0,����Ա���MovieTexData�����б��ֶ�
	if (MovieTexData.MovieTexCnt == 0) return;

	MovieTexData.MovieTex = new V3D_3_1_MovieTex[MovieTexData.MovieTexCnt];

	CMovieTexture** ppMovieTex = pScene->movieTexture.begin();
	for (INT i = 0; i< MovieTexCnt; i++)
	{
		CMovieTexture* pMovieTex = *(ppMovieTex + i);

		//д��MovieTex������
		strcpy(MovieTexData.MovieTex[i].Name, pMovieTex->name);
		fwrite(&(MovieTexData.MovieTex[i].Name), sizeof(MovieTexData.MovieTex[i].Name), 1, pFile);

		//д��MovieTex��·��
		CHAR filePath[256];
		pMovieTex->GetMovieFilePath(filePath);
		strcpy(MovieTexData.MovieTex[i].FileName, filePath);
		fwrite(&(MovieTexData.MovieTex[i].FileName), sizeof(MovieTexData.MovieTex[i].FileName), 1, pFile);

		//д��MovieTex������
		MovieTexData.MovieTex[i].Type = pMovieTex->m_mediaType;
		fwrite(&(MovieTexData.MovieTex[i].Type), sizeof(MovieTexData.MovieTex[i].Type), 1, pFile);

		//д��MovieTex���ز�����
		MovieTexData.MovieTex[i].RepeatCnt = pMovieTex->m_repeatCnt;
		fwrite(&(MovieTexData.MovieTex[i].RepeatCnt), sizeof(MovieTexData.MovieTex[i].RepeatCnt), 1, pFile);
		
		//д��MovieTex�Ƿ��������
		MovieTexData.MovieTex[i].WithAudio = pMovieTex->m_withAudio;
		fwrite(&(MovieTexData.MovieTex[i].WithAudio), sizeof(MovieTexData.MovieTex[i].WithAudio), 1, pFile);
		
	}
	//release memory
	SAFE_DELETE_ARRAY(MovieTexData.MovieTex);
}

//д��BumpWater��Ϣ
//##ModelId=47EBC7440148
VOID CSaveV3D_3_1::WriteBumpWaterData()
{
	INT BumpWaterCnt = pScene->bumpWater.size();

	//����ˮ����ͼ���ݵ�ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.BumpWaterDataPtr = tPos;

	//BumpWater count
	BumpWaterData.BumpWaterCnt = BumpWaterCnt;
	fwrite(&(BumpWaterData.BumpWaterCnt), sizeof(BumpWaterData.BumpWaterCnt), 1, pFile);
	
	//���MovieTex������Ϊ0,����Ա���MovieTexData�����б��ֶ�
	if (BumpWaterData.BumpWaterCnt == 0) return;

	BumpWaterData.BumpWater = new V3D_3_1_BumpWater[BumpWaterData.BumpWaterCnt];

	CBumpWater** ppBumpWater = pScene->bumpWater.begin();
	for (INT i = 0; i< BumpWaterCnt; i++)
	{
		CBumpWater* pBumpWater = *(ppBumpWater + i);

		//д��BumpWater������
		strcpy(BumpWaterData.BumpWater[i].Name, pBumpWater->name);
		fwrite(&(BumpWaterData.BumpWater[i].Name), sizeof(BumpWaterData.BumpWater[i].Name), 1, pFile);

		//д��BumpWater��ͼ�Ŀ��
		BumpWaterData.BumpWater[i].Width = pBumpWater->width;
		fwrite(&(BumpWaterData.BumpWater[i].Width), sizeof(BumpWaterData.BumpWater[i].Width), 1, pFile);

		//д��BumpWater��ͼ�ĸ߶�
		BumpWaterData.BumpWater[i].Height = pBumpWater->height;
		fwrite(&(BumpWaterData.BumpWater[i].Height), sizeof(BumpWaterData.BumpWater[i].Height), 1, pFile);

		//д��BumpWater��ͼ��XScale
		BumpWaterData.BumpWater[i].XScale = pBumpWater->m_xSize;
		fwrite(&(BumpWaterData.BumpWater[i].XScale), sizeof(BumpWaterData.BumpWater[i].XScale), 1, pFile);

		//д��BumpWater��ͼ��YScale
		BumpWaterData.BumpWater[i].YScale = pBumpWater->m_ySize;
		fwrite(&(BumpWaterData.BumpWater[i].YScale), sizeof(BumpWaterData.BumpWater[i].YScale), 1, pFile);

		//д��BumpWater��ͼ��Radius
		BumpWaterData.BumpWater[i].Radius = pBumpWater->m_radius;
		fwrite(&(BumpWaterData.BumpWater[i].Radius), sizeof(BumpWaterData.BumpWater[i].Radius), 1, pFile);

		//д��BumpWater��ͼ��Speed
		BumpWaterData.BumpWater[i].Speed = pBumpWater->m_speed;
		fwrite(&(BumpWaterData.BumpWater[i].Speed), sizeof(BumpWaterData.BumpWater[i].Speed), 1, pFile);
		
	}
	//release memory
	SAFE_DELETE_ARRAY(BumpWaterData.BumpWater);	
}

//-------------------------------------------------------------------
//����: ZipVertexPosData()
//����: 
//��;: ��������λ����Ϣ��ȥ���ظ�����
//-------------------------------------------------------------------
//##ModelId=47EBC7440178
VOID CSaveV3D_3_1::ZipVertexPosData(BOOL zipStatus)
{
	//Ϊ�˲��������ȳ�ʼ��һ����ԭ������һ����С��Buf,��ʱ�����ݱ�����pVertexArray��
	VertexData.VertexCnt = 0;
	for (UINT k=0; k< pScene->model.size(); k++)
			VertexData.VertexCnt += (*(pScene->model.begin() + k))->vertexCnt;

	D3DXVECTOR3* pVertexPosArray = new D3DXVECTOR3[VertexData.VertexCnt];
	//�����еĶ������ݶ��Ž�ȥ
	DWORD index = 0;
	for (DWORD i = 0; i< pScene->model.size(); i++)
	{
		CModel* pModel = *(pScene->model.begin() + i);
		for (DWORD j = 0; j< pModel->vertexCnt; j++)
		{
			pVertexPosArray[index] = pModel->vertex[j].position;
			index++;
		}
	}

	DWORD MinVertexCnt = 1;
	D3DXVECTOR3* pMinVertexPosArray = new D3DXVECTOR3[VertexData.VertexCnt];
	//ѹ�����ļ�С���ٶ���
	if (zipStatus)
	{
		//����ͬλ�õĶ����޳���
		pMinVertexPosArray[0] = pVertexPosArray[0];
		for (i = 1; i< VertexData.VertexCnt; i++)
		{
			//����ǰ������pMinVertexPosArray�Ƿ��Ѿ�����
			bool findSame = false;
			for (DWORD j = 0; j< MinVertexCnt; j++)
			{
				if (pVertexPosArray[i] == pMinVertexPosArray[j])
				{
					findSame = true;
					break;
				}
			}
			//��������ڣ������pMinVertexPosArray
			if (!findSame)
			{
				pMinVertexPosArray[MinVertexCnt] = pVertexPosArray[i];
				MinVertexCnt ++;
			}
			
			//show complete percent-------------------------------------------
			if (i%100 == 0)
			{
				char string[64];
				strcpy(string, "Zip Vertex Position Data..., ");
				INT nPercent = (1.0f * i/VertexData.VertexCnt) * 100;
				char percent[4];
				itoa(nPercent, percent, 10);
				strcat(string, percent);
				strcat(string, "% completed.");
				SetWindowText(GetActiveWindow(), string);
			}
			//show complete percent-------------------------------------------
		}
	}
	//��ѹ�����ļ��󣬵��ٶȿ�
	else
	{
		MinVertexCnt = VertexData.VertexCnt;
		memcpy(pMinVertexPosArray, pVertexPosArray, MinVertexCnt*sizeof(D3DXVECTOR3));
	}

	//MinVertexPosCntΪ��С����λ������
	VertexPosData.VertexPosCnt = MinVertexCnt;

	//�����ݸ��Ƶ�VertexPosData�����У�������ʱ���� pVertexPosArray ���
	VertexPosData.PosList = new D3DXVECTOR3[VertexPosData.VertexPosCnt];
	memcpy(VertexPosData.PosList, pMinVertexPosArray, VertexPosData.VertexPosCnt*sizeof(D3DXVECTOR3));
	SAFE_DELETE_ARRAY(pVertexPosArray);
	SAFE_DELETE_ARRAY(pMinVertexPosArray);
}

//-------------------------------------------------------------------
//����: FindOffsetInVertexPosData()
//����: UINT modelIndex, UINT vertexIndex
//��;: �ҵ�pModel�ĵ�index������VertexPosData�����е�λ��
//-------------------------------------------------------------------
//##ModelId=47EBC7440187
UINT CSaveV3D_3_1::FindOffsetInVertexPosData(UINT modelIndex, UINT vertexIndex)
{
	CModel* pModel = *(pScene->model.begin() + modelIndex);
	D3DXVECTOR3 vertexPos = pModel->vertex[vertexIndex].position;
	
	//�ҵ��ö���λ�ö�Ӧ�ڶ������еı��
	for(DWORD i = 0; i< VertexPosData.VertexPosCnt; i++)
	{
		if (VertexPosData.PosList[i] == vertexPos)
			return i;
	}
	::MessageBox(GetActiveWindow(), "Can not find the correct vertex position data", "error", MB_ICONERROR|MB_OK);
	return 0;

}

//-------------------------------------------------------------------
//����: ZipVertexNormalData()
//����: 
//��;: ѹ�����㷨�����ݣ�ȥ���ظ�����
//-------------------------------------------------------------------
//##ModelId=47EBC74401A5
VOID CSaveV3D_3_1::ZipVertexNormalData(BOOL zipStatus)
{
	//Ϊ�˲��������ȳ�ʼ��һ����ԭ������һ����С��Buf,��ʱ�����ݱ����� pVertexNormalArray ��
	//VertexData.VertexCnt�Ѿ���ZipVertexPosData()�м���
	D3DXVECTOR3* pVertexNormalArray = new D3DXVECTOR3[VertexData.VertexCnt];
	
	//�����еĶ��㷨�����ݶ��Ž�ȥ
	DWORD index = 0;
	for (DWORD i = 0; i< pScene->model.size(); i++)
	{
		CModel* pModel = *(pScene->model.begin() + i);
		for (DWORD j = 0; j< pModel->vertexCnt; j++)
		{
			pVertexNormalArray[index] = pModel->vertex[j].normal;
			index++;
		}
	}

	DWORD MinVertexCnt = 1;
	D3DXVECTOR3* pMinVertexNormalArray = new D3DXVECTOR3[VertexData.VertexCnt];
	//ѹ�����ļ�С���ٶ���
	if (zipStatus)
	{
		//����ͬ���ߵĶ����޳���
		pMinVertexNormalArray[0] = pVertexNormalArray[0];
		for (i = 1; i< VertexData.VertexCnt; i++)
		{
			//����ǰ������pMinVertexPosArray�Ƿ��Ѿ�����
			bool findSame = false;
			for (DWORD j = 0; j< MinVertexCnt; j++)
			{
				if (pVertexNormalArray[i] == pMinVertexNormalArray[j])
				{
					findSame = true;
					break;
				}
			}
			//��������ڣ������pMinVertexPosArray
			if (!findSame)
			{
				pMinVertexNormalArray[MinVertexCnt] = pVertexNormalArray[i];
				MinVertexCnt ++;
			}
			
			//show complete percent-------------------------------------------
			if (i % 100 == 0)
			{
				char string[64];
				strcpy(string, "Zip Vertex Normal Data..., ");
				INT nPercent = (1.0f * i/VertexData.VertexCnt) * 100;
				char percent[4];
				itoa(nPercent, percent, 10);
				strcat(string, percent);
				strcat(string, "% completed.");
				SetWindowText(GetActiveWindow(), string);
			}
			//show complete percent-------------------------------------------
		}
	}
	//��ѹ�����ļ����ٶȿ�
	else
	{
		MinVertexCnt = VertexData.VertexCnt;
		memcpy(pMinVertexNormalArray, pVertexNormalArray, MinVertexCnt * sizeof(D3DXVECTOR3));
	}

	//MinVertexNormalCntΪ��С����λ������
	VertexNormalData.VertexNormalCnt = MinVertexCnt;

	//�����ݸ��Ƶ�VertexNormalData�����У�������ʱ���� pVertexPosArray ���
	VertexNormalData.NormalList = new D3DXVECTOR3[VertexNormalData.VertexNormalCnt];
	memcpy(VertexNormalData.NormalList, pMinVertexNormalArray, VertexNormalData.VertexNormalCnt*sizeof(D3DXVECTOR3));
	SAFE_DELETE_ARRAY(pVertexNormalArray);
	SAFE_DELETE_ARRAY(pMinVertexNormalArray);
}

//-------------------------------------------------------------------
//����: FindOffsetInVertexNormalData()
//����: 
//��;: �ҵ�pModel�ĵ�index������VertexNormalData�����е�λ��
//-------------------------------------------------------------------
//##ModelId=47EBC74401A7
UINT CSaveV3D_3_1::FindOffsetInVertexNormalData(UINT modelIndex, UINT vertexIndex)
{
	CModel* pModel = *(pScene->model.begin() + modelIndex);
	D3DXVECTOR3 vertexNormal = pModel->vertex[vertexIndex].normal;
	
	//�ҵ��ö���λ�ö�Ӧ�ڶ������еı��
	for(DWORD i = 0; i< VertexNormalData.VertexNormalCnt; i++)
	{
		if (VertexNormalData.NormalList[i] == vertexNormal)
			return i;
	}
	return 0;
}

//-------------------------------------------------------------------
//����: ZipUVData()
//����: 
//��;: ѹ��UV���ݣ�ȥ���ظ�����
//-------------------------------------------------------------------
//##ModelId=47EBC74401B6
VOID CSaveV3D_3_1::ZipUVData(BOOL zipStatus)
{
	//Ϊ�˲��������ȳ�ʼ��һ����ԭ������һ����С��Buf,��ʱ�����ݱ����� pUVArray ��
	V3D_3_1_UV* pUVArray = new V3D_3_1_UV[VertexData.VertexCnt];
	
	//�����еĶ���UV���ݶ��Ž�ȥ
	DWORD index = 0;
	for (DWORD i = 0; i< pScene->model.size(); i++)
	{
		CModel* pModel = *(pScene->model.begin() + i);
		for (DWORD j = 0; j< pModel->vertexCnt; j++)
		{
			pUVArray[index].su = pModel->vertex[j].su;
			pUVArray[index].sv = pModel->vertex[j].sv;
			pUVArray[index].sw = pModel->vertex[j].sw;
			pUVArray[index].sz = pModel->vertex[j].sz;
			index++;
		}
	}

	DWORD MinVertexCnt = 1;
	V3D_3_1_UV* pMinUVArray = new V3D_3_1_UV[VertexData.VertexCnt];
	//ѹ�����ļ�С���ٶ���
	if(zipStatus)
	{
		//����ͬUV�Ķ����޳���
		pMinUVArray[0] = pUVArray[0];
		for (i = 1; i< VertexData.VertexCnt; i++)
		{
			//����ǰ������pMinVertexPosArray�Ƿ��Ѿ�����
			bool findSame = false;
			for (DWORD j = 0; j< MinVertexCnt; j++)
			{
				if (pUVArray[i].su == pMinUVArray[j].su &&
					pUVArray[i].sv == pMinUVArray[j].sv &&
					pUVArray[i].sw == pMinUVArray[j].sw &&
					pUVArray[i].sz == pMinUVArray[j].sz)
				{
					findSame = true;
					break;
				}
			}
			//��������ڣ������pMinVertexPosArray
			if (!findSame)
			{
				pMinUVArray[MinVertexCnt] = pUVArray[i];
				MinVertexCnt ++;
			}

			//show complete percent-------------------------------------------
			if (i % 100 == 0)
			{
				char string[64];
				strcpy(string, "Zip Vertex UV Data..., ");
				INT nPercent = (1.0f * i/VertexData.VertexCnt) * 100;
				char percent[4];
				itoa(nPercent, percent, 10);
				strcat(string, percent);
				strcat(string, "% completed.");
				SetWindowText(GetActiveWindow(), string);
			}
			//show complete percent-------------------------------------------

		}
	}
	//��ѹ�����ļ����ٶȿ�
	else
	{
		MinVertexCnt = VertexData.VertexCnt;
		memcpy(pMinUVArray, pUVArray, MinVertexCnt*sizeof(V3D_3_1_UV));
	}

	//MinVertexNormalCntΪ��С����λ������
	UVData.UVCount = MinVertexCnt;

	//�����ݸ��Ƶ�VertexPosData�����У�������ʱ���� pVertexPosArray ���
	UVData.UVList = new V3D_3_1_UV[UVData.UVCount];
	memcpy(UVData.UVList, pMinUVArray, UVData.UVCount*sizeof(V3D_3_1_UV));
	SAFE_DELETE_ARRAY(pUVArray);
	SAFE_DELETE_ARRAY(pMinUVArray);
}

//-------------------------------------------------------------------
//����: FindOffsetInVertexPosData()
//����: UINT modelIndex, UINT vertexIndex
//��;: �ҵ�pModel�ĵ�index�����UV��UVData�����е�λ��
//-------------------------------------------------------------------
//##ModelId=47EBC74401C5
UINT CSaveV3D_3_1::FindOffsetInUVData(UINT modelIndex, UINT vertexIndex)
{
	CModel* pModel = *(pScene->model.begin() + modelIndex);
	FLOAT vertex_su = pModel->vertex[vertexIndex].su;
	FLOAT vertex_sv = pModel->vertex[vertexIndex].sv;
	FLOAT vertex_sw = pModel->vertex[vertexIndex].sw;
	FLOAT vertex_sz = pModel->vertex[vertexIndex].sz;
	
	//�ҵ��ö���UV��Ӧ�ڶ������еı��
	for(DWORD i = 0; i< UVData.UVCount; i++)
	{
		if (UVData.UVList[i].su == vertex_su &&
			UVData.UVList[i].sv == vertex_sv &&
			UVData.UVList[i].sw == vertex_sw &&
			UVData.UVList[i].sz == vertex_sz )
			return i;
	}
	return 0;
}

//-------------------------------------------------------------------
//����: ZipModelData()
//����: 
//��;: ѹ��ģ�����ݣ���ͼ���ݲ�ѹ��
//-------------------------------------------------------------------
//##ModelId=47EBC74401D5
HRESULT CSaveV3D_3_1::ZipModelData()
{
	return S_OK;
}

//-------------------------------------------------------------------
//����: Release()
//����: 
//��;: �ͷ���Դ
//-------------------------------------------------------------------
//##ModelId=47EBC74401D6
VOID CSaveV3D_3_1::Release()
{
	
}

//-------------------------------------------------------------------
//����: SaveSceneAsV3D()
//����: CScene* pScene, CHAR*  strFileName
//��;: �����Ӧ��Scene����ΪV3D�ļ�
//-------------------------------------------------------------------
//##ModelId=47EBC74401E4
HRESULT CSaveV3D_3_1::SaveSceneAsV3D(CScene* theScene, CHAR*  strFileName, BOOL zipStatus)
{
	//ָ������
	if (theScene == NULL)
	{
		MessageBox(NULL, "��������ʧ��,�޷�����V3D�ļ���", "����V3D�ļ�...����", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	pScene = theScene;

	//���ļ�IO
	if (NULL == (pFile = fopen(strFileName, "wb")) )
	{
		MessageBox(NULL, "�ļ���ռ�ã�ָ����E3D�ļ��޷����档", "����E3D�ļ�...����", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//���������
	ResetModelIndex();
	ResetCameraIndex();
	ResetLightIndex();

	ResetTextureIndex();
	ResetBumpWaterIndex();
	ResetCubeMapIndex();
	ResetMovieTexIndex();
	ResetMaterialIndex();

	ResetOverlayIndex();
	ResetButtonIndex();
	ResetNavigaterIndex();

	//�����ƶ������±��
	ResetWalkerIndex();
	ResetEditorIndex();
	ResetFlyerIndex();

	ResetTextIndex();
	ResetImageIndex();
	ResetDyTexIndex();
	
	//-------------------------------------------------------
	//Write V3D Data
	//-------------------------------------------------------
	//write the header data
	WriteHeader();
	//write the window prop
	WriteWindowProp();

	//write AudioData
	WriteAudioData();
	//write FlashData
	WriteFlashData();

	//write the  camera data
	WriteCameraData();
	//write the Material data
	WriteMaterialData();
	//write the light data
	WriteLightData();

	//write the vertices pos data
	WriteVertexPosData(zipStatus);
	//write the vertices pos data
	WriteVertexNormalData(zipStatus);
	//write vertex uv data
	WriteUVData(zipStatus);
	//write the vertices data
	WriteVertexData(zipStatus);
	//write the Model data
	WriteModelData();
	//write Modelgroup Data
	WriteModelGroupData();

	//write the Text data
	WriteTextData();
	//write the overlay data
	WriteOverlayData();
	//write the navigater data
	WriteNavigaterData();
	//write the button data
	WriteButtonData();

	//д��CubeMap����
	WriteCubeMapData();
	//д��MovieTex����
	WriteMovieTexData();
	//д��BumpWater����
	WriteBumpWaterData();

	//write privacy data
	WritePrivacyData();
	//д����ƶ�������
	WriteControlData();

	//write the texture data
	WriteTexData();
	//write the image data
	WriteImageData();
	//write dynamicTex Data
	WriteDynamicTexData();
	
	//rewrite the the PTR table
	fseek(pFile, 0, SEEK_SET);
	fwrite(&Header, sizeof(Header), 1, pFile);
	fseek(pFile, 0, SEEK_END);
	UINT fileLen = ftell(pFile);

	//close the pFile
	fclose(pFile);

	return S_OK;
}

//д����������
//##ModelId=47EBC74303B9
VOID CSaveV3D_3_1::WriteAudioData()
{
	//��¼��Ƶ���ݼ�¼��ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.AudioDataPtr = tPos;

	//�õ���Ƶ����
	AudioData.AudioCnt = pScene->audio.size();

	fwrite(&(AudioData.AudioCnt), sizeof(AudioData.AudioCnt), 1, pFile);
	if (AudioData.AudioCnt == 0)
		return;

	AudioData.Audio = new V3D_3_1_Audio[AudioData.AudioCnt];
	//�����ȡ��Ƶ��Ϣ
	for (UINT i =0; i< AudioData.AudioCnt; i++)
	{
		CAudioPlayer* pAudio = *(pScene->audio.begin() + i);

		//��ǰ��Ƶ�Ļ�����Ϣ
		strcpy(AudioData.Audio[i].Name, pAudio->name);
		pAudio->GetAudioFilePath(AudioData.Audio[i].FileName);
		AudioData.Audio[i].Type = 0;
		AudioData.Audio[i].RepeatCnt = pAudio->m_repeatCnt;

		//���浱ǰ��Ƶ�Ļ�����Ϣ
		fwrite(&(AudioData.Audio[i].Name), sizeof(AudioData.Audio[i].Name), 1, pFile);
		fwrite(&(AudioData.Audio[i].FileName), sizeof(AudioData.Audio[i].FileName), 1, pFile);
		fwrite(&(AudioData.Audio[i].Type), sizeof(AudioData.Audio[i].Type), 1, pFile);
		fwrite(&(AudioData.Audio[i].RepeatCnt), sizeof(AudioData.Audio[i].RepeatCnt), 1, pFile);
	}

	//release memory
	SAFE_DELETE_ARRAY(AudioData.Audio);
}

//--------------------------------------------------------
//д��Flash����, ��ʱ��������Flash���ݣ�ֻ����Flash���ݽṹ
//--------------------------------------------------------
//##ModelId=47EBC74303BA
VOID CSaveV3D_3_1::WriteFlashData()
{
	//��¼Flash���ݼ�¼��ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.FlashDataPtr = tPos;

	//�õ�Flash����
	FlashData.FlashCnt = 0;

	fwrite(&(FlashData.FlashCnt), sizeof(FlashData.FlashCnt), 1, pFile);
	if (FlashData.FlashCnt == 0)
		return;
}

//����д��ModelGroupData����
//##ModelId=47EBC74400DB
VOID CSaveV3D_3_1::WriteModelGroupData1(CModelGroup* pModelGroup)
{
	//д�뵱ǰģ���������
	fwrite(&(pModelGroup->name), sizeof(pModelGroup->name), 1, pFile);

	//д�뵱ǰģ�����е�ģ������
	INT ModelCnt = pModelGroup->model.size();
	fwrite(&ModelCnt, sizeof(ModelCnt), 1, pFile);

	//д�뵱ǰģ�����е�ģ���б�
	if (ModelCnt > 0)
	{
		CModel** ppModel = pModelGroup->model.begin();
		for (INT i = 0; i < ModelCnt; i++)
		{
			CModel* pModel = *(ppModel + i);
			fwrite(&(pModel->index), sizeof(pModel->index), 1, pFile);
		}
	}

	//��ǰģ�����е�ģ��������
	INT modelGroupCnt = pModelGroup->modelGroup.size();
	fwrite(&modelGroupCnt, sizeof(modelGroupCnt), 1, pFile);
	
	CModelGroup** ppModelGroup = pModelGroup->modelGroup.begin();
	//��ǰģ�����е�ģ�����б�, �ݹ����
	for (INT j = 0; j < modelGroupCnt; j++)
	{
		CModelGroup* sModelGroup = *(ppModelGroup + j);
		WriteModelGroupData1(sModelGroup);
	}

	//д�뵱ǰ�����ĸ�������
	INT parentModelGroupIndex = -1;
	if (pModelGroup->parent != NULL)
		parentModelGroupIndex = pModelGroup->index;
	fwrite(&parentModelGroupIndex, sizeof(parentModelGroupIndex), 1, pFile);
}

//д��ģ������Ϣ
//##ModelId=47EBC74400DA
VOID CSaveV3D_3_1::WriteModelGroupData()
{
	//��¼ģ�������ݼ�¼��ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.ModelGroupDataPtr = tPos;

	//�õ�ģ���������
	ModelGroupData.ModelGroupCnt = pScene->modelGroup.size();
	//д��ģ��������
	fwrite(&(ModelGroupData.ModelGroupCnt), sizeof(ModelGroupData.ModelGroupCnt), 1, pFile);
	if (ModelGroupData.ModelGroupCnt == 0)
		return;

	ModelGroupData.ModelGroup = new V3D_3_1_ModelGroup[ModelGroupData.ModelGroupCnt];
	
	//�����ȡģ������Ϣ
	for (UINT i =0; i< ModelGroupData.ModelGroupCnt; i++)
	{
		CModelGroup* pModelGroup = *(pScene->modelGroup.begin() + i);
		//�ݹ�д��ModelGroup������
		WriteModelGroupData1(pModelGroup);
	}
	//release memory
	SAFE_DELETE_ARRAY(ModelGroupData.ModelGroup);
}

//д�붯̬��ͼ��Ϣ
//##ModelId=47EBC7440157
VOID CSaveV3D_3_1::WriteDynamicTexData()
{
	//��¼��̬��ͼ���ݼ�¼��ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.DynamicTexDataPtr = tPos;

	//д�붯̬��ͼ������
	DynamicTexData.DynamicTexCnt = pScene->dynamicTexture.size();
	fwrite(&(DynamicTexData.DynamicTexCnt), sizeof(DynamicTexData.DynamicTexCnt), 1, pFile);
	if (DynamicTexData.DynamicTexCnt == 0) return;

	DynamicTexData.DynamicTex = new V3D_3_1_DynamicTex[DynamicTexData.DynamicTexCnt];
	CDynamicTex** ppDyTex = pScene->dynamicTexture.begin();
	for (INT i = 0; i< DynamicTexData.DynamicTexCnt; i++)
	{
		CDynamicTex* pDyTex = *(ppDyTex + i);

		//д�붯̬��ͼ������
		strcpy(DynamicTexData.DynamicTex[i].Name, pDyTex->name);
		fwrite(&(DynamicTexData.DynamicTex[i].Name), 
			sizeof(DynamicTexData.DynamicTex[i].Name), 1, pFile);
		
		//��̬��ͼ�Ŀ��
		DynamicTexData.DynamicTex[i].Width = pDyTex->width;
		fwrite(&(DynamicTexData.DynamicTex[i].Width), 
			sizeof(DynamicTexData.DynamicTex[i].Width), 1, pFile);

		//��̬��ͼ�ĸ߶�
		DynamicTexData.DynamicTex[i].Height = pDyTex->height;
		fwrite(&(DynamicTexData.DynamicTex[i].Height), 
			sizeof(DynamicTexData.DynamicTex[i].Height), 1, pFile);

		//��ͼ��ʽ
		DynamicTexData.DynamicTex[i].Format = pDyTex->format;
		fwrite(&(DynamicTexData.DynamicTex[i].Format), 
			sizeof(DynamicTexData.DynamicTex[i].Format), 1, pFile);

		//�õ���̬��ͼ��֡����
		DynamicTexData.DynamicTex[i].FrameCnt = pDyTex->frameCnt;
		//д�붯̬��ͼ��֡����
		fwrite(&(DynamicTexData.DynamicTex[i].FrameCnt), sizeof(DynamicTexData.DynamicTex[i].FrameCnt), 1, pFile);
		if (DynamicTexData.DynamicTex[i].FrameCnt == 0)
			continue;
		
		DynamicTexData.DynamicTex[i].FileNames = new V3D_3_1_FileName[DynamicTexData.DynamicTex[i].FrameCnt];
		//�����ȡ��̬��ͼ��Ϣ
		for (INT j =0; j< DynamicTexData.DynamicTex[i].FrameCnt; j++)
		{
			//��ǰ��̬��ͼ���ļ���
			strcpy(DynamicTexData.DynamicTex[i].FileNames[j].FileName, 
					pDyTex->fileName[j]->FileName);
			//����
			fwrite(&(DynamicTexData.DynamicTex[i].FileNames[j]), 
				sizeof(DynamicTexData.DynamicTex[i].FileNames[j]), 1, pFile);
		}	
		SAFE_DELETE_ARRAY(DynamicTexData.DynamicTex[i].FileNames);
	}

	//release memory
	SAFE_DELETE_ARRAY(DynamicTexData.DynamicTex);
}

//д���ļ��汾���ͺ���Ȩ��ʾ
//##ModelId=47EBC7440158
VOID CSaveV3D_3_1::WritePrivacyData()
{
	//��¼�ļ��汾����Ȩ���ݼ�¼��ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.PrivacyDataPtr = tPos;

	V3D_3_1_Privacy privacy;

	privacy.VersionType = 1;
	privacy.ClientIndex = 0;
	ZeroMemory(privacy.SerialNum, sizeof(privacy.SerialNum));
	ZeroMemory(privacy.Authorize, sizeof(privacy.Authorize));
	privacy.ExtendInfo1 = 0;
	privacy.ExtendInfo2 = 0;

	//д���ļ��汾����Ȩ��ʾ������
	fwrite(&privacy, sizeof(privacy), 1, pFile);
}

//д����ƶ�������
//##ModelId=47EBC7440159
VOID CSaveV3D_3_1::WriteControlData()
{
	//��¼Flash���ݼ�¼��ƫ����
	UINT tPos = ftell(pFile);
	Header.FileTable.ControlDataPtr = tPos;

	//д�뵱ǰ�Ŀ���ģʽ
	ControlData.ControlType = pScene->controlType;  //0 walk, 1 Editor, 2 flyer
	fwrite(&(ControlData.ControlType), sizeof(ControlData.ControlType), 1, pFile);

	//д����ƶ���ı��
	switch (ControlData.ControlType)
	{
	case 0:
		if (pScene->currentWalker)
			ControlData.ControlerIndex = pScene->currentWalker->index;
		else
			ControlData.ControlerIndex = -1;
		break;
	case 1:
		if (pScene->currentEditor)
			ControlData.ControlerIndex = pScene->currentEditor->index;
		else
			ControlData.ControlerIndex = -1;
		break;
	case 2:
		if (pScene->currentFlyer)
			ControlData.ControlerIndex = pScene->currentFlyer->index;
		else
			ControlData.ControlerIndex = -1;
		break;
	default:
			ControlData.ControlerIndex = -1;
		break;
	}
	fwrite(&(ControlData.ControlerIndex), sizeof(ControlData.ControlerIndex), 1, pFile);

	//д��Walker��������
	ControlData.WalkerList.WalkerCnt = pScene->walker.size();
	fwrite(&(ControlData.WalkerList.WalkerCnt), sizeof(ControlData.WalkerList.WalkerCnt), 1, pFile);

	//д��Walker�����б�
	if (ControlData.WalkerList.WalkerCnt > 0)
	{
		INT walkerCnt = ControlData.WalkerList.WalkerCnt;
		ControlData.WalkerList.Walkers = new V3D_3_1_Walker[walkerCnt];
		
		CWalker** ppWalker = pScene->walker.begin();
		for (INT i = 0; i < walkerCnt; i++)
		{
			CWalker* pWalker = *(ppWalker + i);
			
			strcpy(ControlData.WalkerList.Walkers[i].Name, pWalker->name);
			ControlData.WalkerList.Walkers[i].CameraIndex = pWalker->m_pCamera->index;

			ControlData.WalkerList.Walkers[i].walkSpeed = pWalker->walkSpeed;
			ControlData.WalkerList.Walkers[i].walkAcc = pWalker->walkAcc;
			ControlData.WalkerList.Walkers[i].rosSpeed = pWalker->rosSpeed;
			ControlData.WalkerList.Walkers[i].rosAcc = pWalker->rosAcc;

			ControlData.WalkerList.Walkers[i].height = pWalker->height;

			ControlData.WalkerList.Walkers[i].bCollision = pWalker->bCollision;
			ControlData.WalkerList.Walkers[i].colDis = pWalker->colDis;
			
			ControlData.WalkerList.Walkers[i].bWeight = pWalker->bWeight;
			ControlData.WalkerList.Walkers[i].liftSpeed = pWalker->liftSpeed;
			ControlData.WalkerList.Walkers[i].liftAcc = pWalker->liftAcc;

			ControlData.WalkerList.Walkers[i].upperAngle = pWalker->upperAngle;
			ControlData.WalkerList.Walkers[i].lowerAngle = pWalker->lowerAngle;

			fwrite(&(ControlData.WalkerList.Walkers[i]), sizeof(ControlData.WalkerList.Walkers[i]), 1, pFile);
		}
		SAFE_DELETE_ARRAY(ControlData.WalkerList.Walkers);
	}

	//д��Editor���������
	ControlData.EditorList.EditorCnt = pScene->editor.size();
	fwrite(&(ControlData.EditorList.EditorCnt), sizeof(ControlData.EditorList.EditorCnt), 1, pFile);

	//д��Editor�����б�
	if (ControlData.EditorList.EditorCnt > 0)
	{
		INT EditorCnt = ControlData.EditorList.EditorCnt;

		ControlData.EditorList.Editors = new V3D_3_1_Editor[EditorCnt];

		CEditor** ppEditor = pScene->editor.begin();
		for (INT i = 0; i < EditorCnt; i++)
		{
			CEditor* pEditor = *(ppEditor + i);
			
			strcpy(ControlData.EditorList.Editors[i].Name, pEditor->name);
			ControlData.EditorList.Editors[i].CameraIndex = pEditor->m_pCamera->index;

			ControlData.EditorList.Editors[i].bIsAutoRoundRoll = pEditor->bIsAutoRoundRoll;

			ControlData.EditorList.Editors[i].autoPitchSpeed = pEditor->autoPitchSpeed;
			ControlData.EditorList.Editors[i].autoRoundSpeed = pEditor->autoRoundSpeed;
			ControlData.EditorList.Editors[i].autoRollSpeed = pEditor->autoRollSpeed;

			ControlData.EditorList.Editors[i].manualPitchSpeed = pEditor->manualPitchSpeed;
			ControlData.EditorList.Editors[i].manualRoundSpeed = pEditor->manualRoundSpeed;

			ControlData.EditorList.Editors[i].manualPanXSpeed = pEditor->manualPanXSpeed;
			ControlData.EditorList.Editors[i].manualPanYSpeed = pEditor->manualPanYSpeed;

			ControlData.EditorList.Editors[i].dollySpeed = pEditor->dollySpeed;
			ControlData.EditorList.Editors[i].dollyAcc = pEditor->dollyAcc;

			ControlData.EditorList.Editors[i].dollyNearDis = pEditor->dollyNearDis;
			ControlData.EditorList.Editors[i].dollyFarDis = pEditor->dollyFarDis;

			fwrite(&(ControlData.EditorList.Editors[i]), sizeof(ControlData.EditorList.Editors[i]), 1, pFile);
		}
		SAFE_DELETE_ARRAY(ControlData.EditorList.Editors);
	}

	//д��Flyer���������
	ControlData.FlyerList.FlyerCnt = pScene->flyer.size();
	fwrite(&(ControlData.FlyerList.FlyerCnt), sizeof(ControlData.FlyerList.FlyerCnt), 1, pFile);

	//д��Flyer�����б�
	if (ControlData.FlyerList.FlyerCnt > 0)
	{
		INT FlyerCnt = ControlData.FlyerList.FlyerCnt;

		ControlData.FlyerList.Flyers = new V3D_3_1_Flyer[FlyerCnt];

		CFlyer** ppFlyer = pScene->flyer.begin();
		for (INT i = 0; i < FlyerCnt; i++)
		{
			CFlyer* pFlyer = *(ppFlyer + i);
			
			strcpy(ControlData.FlyerList.Flyers[i].Name, pFlyer->name);
			ControlData.FlyerList.Flyers[i].CameraIndex = pFlyer->m_pCamera->index;

			ControlData.FlyerList.Flyers[i].panSpeed = pFlyer->panSpeed;
			ControlData.FlyerList.Flyers[i].panAcc = pFlyer->panAcc;

			ControlData.FlyerList.Flyers[i].dollySpeed = pFlyer->dollySpeed;
			ControlData.FlyerList.Flyers[i].dollyAcc = pFlyer->dollyAcc;

			ControlData.FlyerList.Flyers[i].dollyNearDis = pFlyer->dollyNearDis;
			ControlData.FlyerList.Flyers[i].dollyFarDis = pFlyer->dollyFarDis;

			ControlData.FlyerList.Flyers[i].dollySpeed = pFlyer->dollySpeed;
			ControlData.FlyerList.Flyers[i].dollyAcc = pFlyer->dollyAcc;

			ControlData.FlyerList.Flyers[i].rotateSpeed = pFlyer->rotateSpeed;
			ControlData.FlyerList.Flyers[i].rotateAcc = pFlyer->rotateAcc;
			
			ControlData.FlyerList.Flyers[i].liftSpeed = pFlyer->liftSpeed;
			ControlData.FlyerList.Flyers[i].liftAcc = pFlyer->liftAcc;

			ControlData.FlyerList.Flyers[i].upperAngle = pFlyer->upperAngle;
			ControlData.FlyerList.Flyers[i].lowerAngle = pFlyer->lowerAngle;

			ControlData.FlyerList.Flyers[i].bCollision = pFlyer->bCollision;
			ControlData.FlyerList.Flyers[i].colDis = pFlyer->colDis;

			fwrite(&(ControlData.FlyerList.Flyers[i]), sizeof(ControlData.FlyerList.Flyers[i]), 1, pFile);
		}
		SAFE_DELETE_ARRAY(ControlData.FlyerList.Flyers);
	}
}

//��ģ�Ͷ������±��
//##ModelId=47EBC74401F5
VOID CSaveV3D_3_1::ResetModelIndex()
{
	CModel** ppModel = pScene->model.begin();
	INT modelCnt = pScene->model.size();
	for (INT i = 0; i< modelCnt; i++)
	{
		CModel* pModel = *(ppModel + i);
		pModel->index = i;
	}
}

//���¸���ͼ���б��
//##ModelId=47EBC74401F6
VOID CSaveV3D_3_1::ResetTextureIndex()
{
	CTexture** ppTex = pScene->texture.begin();
	INT texCnt = pScene->texture.size();
	for (INT i = 0; i< texCnt; i++)
	{
		CTexture* pTex = *(ppTex + i);
		pTex->index = i;
	}
}


//��BumpWater���±��
//##ModelId=47EBC74401F7
VOID CSaveV3D_3_1::ResetBumpWaterIndex()
{	
	INT bumpWaterCnt = pScene->bumpWater.size();
	CBumpWater** ppWater = pScene->bumpWater.begin();
	for (INT i = 0; i< bumpWaterCnt; i++)
	{
		CBumpWater* pBumpWater = *(ppWater + i);
		pBumpWater->index = i;
	}
}


//��CubeMap�����е�Ԫ�����±��
//##ModelId=47EBC74401F8
VOID CSaveV3D_3_1::ResetCubeMapIndex()
{
	INT cubeMapCnt = pScene->cubeMap.size();
	CCubeMap** ppCubeMap = pScene->cubeMap.begin();
	for (INT i = 0; i< cubeMapCnt; i++)
	{
		CCubeMap* pCubeMap = *(ppCubeMap + i);
		pCubeMap->index = i;
	}
}

//��MovieTex���±��
//##ModelId=47EBC74401F9
VOID CSaveV3D_3_1::ResetMovieTexIndex()
{
	INT movieTexCnt = pScene->movieTexture.size();
	CMovieTexture** ppMovieTex = pScene->movieTexture.begin();
	for (INT i = 0; i< movieTexCnt; i++)
	{
		CMovieTexture* pMovieTex = *(ppMovieTex + i);
		pMovieTex->index = i;
	}
}

//�����ʶ������±��
//##ModelId=47EBC7440203
VOID CSaveV3D_3_1::ResetMaterialIndex()
{
	CMaterial** ppMtrl = (CMaterial**)pScene->shader.begin();
	INT mtrlCnt = pScene->shader.size();
	for (INT i = 0; i< mtrlCnt; i++)
	{
		CMaterial* pMtrl = *(ppMtrl + i);
		pMtrl->index = i;
	}
}

//ΪOverlay���±��
//##ModelId=47EBC7440204
VOID CSaveV3D_3_1::ResetOverlayIndex()
{
	INT overlayCnt = pScene->overlay.size();
	COverlay** ppOverlay = pScene->overlay.begin();
	for (INT i  =0; i< overlayCnt; i++)
	{
		COverlay* pOverlay = *(ppOverlay + i);
		pOverlay->index = i;
	}
}

//Ϊ��̬��ͼ���
//##ModelId=47EBC7440205
VOID CSaveV3D_3_1::ResetDyTexIndex()
{
	INT DyTexCnt = pScene->dynamicTexture.size();
	CDynamicTex** ppDyTex = pScene->dynamicTexture.begin();
	for (INT i = 0; i< DyTexCnt; i++)
	{
		CDynamicTex* pDyTex = *(ppDyTex + i);
		pDyTex->index = i;
	}
}

//Ϊ������б��
//##ModelId=47EBC7440206
VOID CSaveV3D_3_1::ResetCameraIndex()
{
	INT cameraCnt = pScene->camera.size();
	CCamera** ppCamera = pScene->camera.begin();
	for (INT i = 0; i< cameraCnt; i++)
	{
		CCamera* pCam = *(ppCamera + i);
		pCam->index = i;
	}
}

//Ϊ�ƹ���б��
//##ModelId=47EBC7440207
VOID CSaveV3D_3_1::ResetLightIndex()
{
	INT lightCnt = pScene->light.size();
	CLight** ppLight = pScene->light.begin();
	for (INT i = 0; i< lightCnt; i++)
	{
		CLight* pLight = *(ppLight + i);
		pLight->index = i;
	}
}

//Ϊ����ͼ�������±��
//##ModelId=47EBC7440208
VOID CSaveV3D_3_1::ResetNavigaterIndex()
{
	INT NavCnt = pScene->navigater.size();
	CNavigater** ppNav = pScene->navigater.begin();
	for (INT i = 0; i< NavCnt; i++)
	{
		CNavigater* pNav = *(ppNav + i);
		pNav->index = i;
	}

}

//Ϊ���ֶ������±��
//##ModelId=47EBC7440209
VOID CSaveV3D_3_1::ResetTextIndex()
{
	INT textCnt = pScene->text.size();
	CText** ppText = pScene->text.begin();
	for (INT i =0; i< textCnt; i++)
	{
		CText* pText = *(ppText + i);
		pText->index = i;
	}
}

//��Button�������±��
//##ModelId=47EBC744020D
VOID CSaveV3D_3_1::ResetButtonIndex()
{
	INT buttonCnt = pScene->button.size();
	CButton** ppButton = pScene->button.begin();
	for (INT i = 0; i< buttonCnt; i++)
	{
		CButton* pButton = *(ppButton + i);
		pButton->index = i;
	}
}

//��Image�������±��
//##ModelId=47EBC744020E
VOID CSaveV3D_3_1::ResetImageIndex()
{
	INT imageCnt = pScene->image.size();
	CImage** ppImage = pScene->image.begin();
	for (INT i = 0; i< imageCnt; i++)
	{
		CImage* pImage = *(ppImage + i);
		pImage->index = i;
	}
}

//�����ƶ������±��
//##ModelId=47EBC744020A
VOID CSaveV3D_3_1::ResetWalkerIndex()
{
	INT walkerCnt = pScene->walker.size();
	CWalker** ppWalker = pScene->walker.begin();
	
	for (INT i = 0; i< walkerCnt; i++)
	{
		CWalker* pWalker = *(ppWalker + i);
		pWalker->index = i;
	}
}

//##ModelId=47EBC744020B
VOID CSaveV3D_3_1::ResetEditorIndex()
{
	INT editorCnt = pScene->editor.size();
	CEditor** ppEditor = pScene->editor.begin();
	
	for (INT i = 0; i< editorCnt; i++)
	{
		CEditor* pEditor = *(ppEditor + i);
		pEditor->index = i;
	}
}

//##ModelId=47EBC744020C
VOID CSaveV3D_3_1::ResetFlyerIndex()
{
	INT flyerCnt = pScene->flyer.size();
	CFlyer** ppFlyer = pScene->flyer.begin();
	
	for (INT i = 0; i< flyerCnt; i++)
	{
		CFlyer* pFlyer = *(ppFlyer + i);
		pFlyer->index = i;
	}
}
