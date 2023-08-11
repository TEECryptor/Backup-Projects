/////////////////////////////////////////////////////////////////////////
// 名称: CGetV3DFileInfo_3_2 -- V3D 3.2版本文件读取类
// 描述: 读取V3D文件，并规整场景数据
// 作者: lixin	
// 日期: 2007-05-09
////////////////////////////////////////////////////////////////////////

#include "CGetV3DFileInfo_3_2.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74A014A
CGetV3DFileInfo_3_2::CGetV3DFileInfo_3_2(CRITICAL_SECTION*	pRenderCS)
{
	m_pRenderCS = pRenderCS;
	pFile = NULL;		//E3D文件数据流

	CameraData.Camera = NULL;
	LightData.Lights = NULL;
	TextData.Texts = NULL;
	ModelData.Model = NULL;
	MaterialData.Material = NULL;
	OverlayData.Overlays = NULL;
	NavigaterData.Navigaters = NULL;
	ButtonData.Buttons = NULL;
	CubeMapData.CubeMap = NULL;
	MovieTexData.MovieTex = NULL;
	BumpWaterData.BumpWater = NULL;
}

//##ModelId=47EBC74A0186
CGetV3DFileInfo_3_2::~CGetV3DFileInfo_3_2()
{

}


//-------------------------------------------------------------------
//方法: ReadHeader()
//参数: 
//用途: 读取文件头
//-------------------------------------------------------------------
//##ModelId=47EBC7490271
VOID CGetV3DFileInfo_3_2::ReadHeader()
{
	//write the Header to pFile
	if (pFile == NULL) return;
	//init _E3DHeader
	fseek(pFile, 0, SEEK_SET);
	fread(&Header, sizeof(Header), 1, pFile);

	pScene->m_fileVersion = Header.V3DVer;
}
	
//读取窗体信息
//##ModelId=47EBC749029F
VOID CGetV3DFileInfo_3_2::ReadWindowProp()
{
	fread(&WindowProp, sizeof(WindowProp), 1, pFile);

	pScene->m_oriWidth = WindowProp.Width;
	pScene->m_oriHeight = WindowProp.Height;

	pScene->rect.left = 0;
	pScene->rect.top = 0;
	pScene->rect.right = pScene->m_oriWidth;
	pScene->rect.bottom = pScene->m_oriHeight;

	pScene->background = WindowProp.BackColor;
	pScene->m_kfrPlayspeed = WindowProp.FPS;
	pScene->antiLevel = WindowProp.AntiAliasLevel;
	
	pScene->FSBlurLevel = WindowProp.FSBlurLevel;
	pScene->FSGlowLevel = WindowProp.FSGlowLevel;

	pScene->m_KfrCnt = WindowProp.KeyframeCnt;
	pScene->m_KfrRepeatCnt = WindowProp.RepeatCnt;
}

//读入授权信息
//##ModelId=47EBC74A00CB
VOID CGetV3DFileInfo_3_2::ReadPrivacyData()
{
	//找到授权信息的位置
	fseek(pFile, Header.FileTable.PrivacyDataPtr, SEEK_SET);
	//读入文件版本和授权表示的数据
	fread(&PrivacyData, sizeof(PrivacyData), 1, pFile);
}

//读取控制信息
//##ModelId=47EBC74A00FA
VOID  CGetV3DFileInfo_3_2::ReadContorlData()
{
	//找到控制信息的位置
	fseek(pFile, Header.FileTable.ControlDataPtr, SEEK_SET);

	//读入当前的控制模式
	fread(&(ControlData.ControlType), sizeof(ControlData.ControlType), 1, pFile);
	pScene->controlType = ControlData.ControlType;  //0 walk, 1 Editor, 2 flyer

	//读入控制对象的编号，设置当前控制对象
	fread(&(ControlData.ControlerIndex), sizeof(ControlData.ControlerIndex), 1, pFile);
	switch (ControlData.ControlType)
	{
	case 0:
		pScene->currentWalker = *(pScene->walker.begin() + ControlData.ControlerIndex);
		break;
	case 1:
		pScene->currentEditor = *(pScene->editor.begin() + ControlData.ControlerIndex);
		break;
	case 2:
		pScene->currentFlyer = *(pScene->flyer.begin() + ControlData.ControlerIndex);
		break;
	}

	//读入Walker对象数量
	fread(&(ControlData.WalkerList.WalkerCnt), sizeof(ControlData.WalkerList.WalkerCnt), 1, pFile);

	//写入Walker对象列表
	if (ControlData.WalkerList.WalkerCnt > 0)
	{
		INT walkerCnt = ControlData.WalkerList.WalkerCnt;
		ControlData.WalkerList.Walkers = new V3D_3_2_Walker[walkerCnt];
		for (INT i = 0; i < walkerCnt; i++)
		{
			fread(&(ControlData.WalkerList.Walkers[i]), sizeof(ControlData.WalkerList.Walkers[i]), 1, pFile);
		
			if (ControlData.WalkerList.Walkers[i].CameraIndex >= 0)
			{
				CWalker* pWalker = new CWalker;
				
				strcpy(pWalker->name, ControlData.WalkerList.Walkers[i].Name);

				pWalker->m_pCamera = *(pScene->camera.begin() + ControlData.WalkerList.Walkers[i].CameraIndex);

				pWalker->walkSpeed = ControlData.WalkerList.Walkers[i].walkSpeed;
				pWalker->walkAcc = ControlData.WalkerList.Walkers[i].walkAcc;
				pWalker->rosSpeed = ControlData.WalkerList.Walkers[i].rosSpeed;
				pWalker->rosAcc = ControlData.WalkerList.Walkers[i].rosAcc;

				pWalker->height = ControlData.WalkerList.Walkers[i].height;

				pWalker->bCollision = ControlData.WalkerList.Walkers[i].bCollision;
				pWalker->colDis = ControlData.WalkerList.Walkers[i].colDis;

				pWalker->bWeight = ControlData.WalkerList.Walkers[i].bWeight;
				pWalker->liftSpeed = ControlData.WalkerList.Walkers[i].liftSpeed;
				pWalker->liftAcc = ControlData.WalkerList.Walkers[i].liftAcc;

				pWalker->upperAngle = ControlData.WalkerList.Walkers[i].upperAngle;
				pWalker->lowerAngle = ControlData.WalkerList.Walkers[i].lowerAngle;

				pScene->walker.push_back(pWalker);
			}
		}
		SAFE_DELETE_ARRAY(ControlData.WalkerList.Walkers);
	}

	//读入Editor对象的数量
	fread(&(ControlData.EditorList.EditorCnt), sizeof(ControlData.EditorList.EditorCnt), 1, pFile);

	//读入Editor对象列表
	if (ControlData.EditorList.EditorCnt > 0)
	{
		INT EditorCnt = ControlData.EditorList.EditorCnt;
		ControlData.EditorList.Editors = new V3D_3_2_Editor[EditorCnt];
		for (INT i = 0; i < EditorCnt; i++)
		{
			fread(&(ControlData.EditorList.Editors[i]), sizeof(ControlData.EditorList.Editors[i]), 1, pFile);

			if (ControlData.EditorList.Editors[i].CameraIndex >= 0)
			{
				CEditor* pEditor = new CEditor;

				strcpy(pEditor->name, ControlData.EditorList.Editors[i].Name);

				pEditor->m_pCamera = *(pScene->camera.begin() + ControlData.EditorList.Editors[i].CameraIndex);

				pEditor->bIsAutoRoundRoll = ControlData.EditorList.Editors[i].bIsAutoRoundRoll;

				pEditor->autoPitchSpeed = ControlData.EditorList.Editors[i].autoPitchSpeed;
				pEditor->autoRoundSpeed = ControlData.EditorList.Editors[i].autoRoundSpeed;
				pEditor->autoRollSpeed = ControlData.EditorList.Editors[i].autoRollSpeed;

				pEditor->manualPitchSpeed = ControlData.EditorList.Editors[i].manualPitchSpeed;
				pEditor->manualRoundSpeed = ControlData.EditorList.Editors[i].manualRoundSpeed;

				pEditor->manualPanXSpeed = ControlData.EditorList.Editors[i].manualPanXSpeed;
				pEditor->manualPanYSpeed = ControlData.EditorList.Editors[i].manualPanYSpeed;

				pEditor->dollySpeed = ControlData.EditorList.Editors[i].dollySpeed;
				pEditor->dollyAcc = ControlData.EditorList.Editors[i].dollyAcc;

				pEditor->dollyNearDis = ControlData.EditorList.Editors[i].dollyNearDis;
				pEditor->dollyFarDis = ControlData.EditorList.Editors[i].dollyFarDis;
				
				pScene->editor.push_back(pEditor);
			}
		}
		SAFE_DELETE_ARRAY(ControlData.EditorList.Editors);
	}

	//读入Flyer对象的数量
	fread(&(ControlData.FlyerList.FlyerCnt), sizeof(ControlData.FlyerList.FlyerCnt), 1, pFile);

	//读入Flyer对象列表
	if (ControlData.FlyerList.FlyerCnt > 0)
	{
		INT FlyerCnt = ControlData.FlyerList.FlyerCnt;
		ControlData.FlyerList.Flyers = new V3D_3_2_Flyer[FlyerCnt];
		for (INT i = 0; i < FlyerCnt; i++)
		{
			fread(&(ControlData.FlyerList.Flyers[i]), sizeof(ControlData.FlyerList.Flyers[i]), 1, pFile);

			if (ControlData.FlyerList.Flyers[i].CameraIndex >= 0)
			{
				CFlyer* pFlyer = new CFlyer;			

				strcpy(pFlyer->name, ControlData.FlyerList.Flyers[i].Name);
				pFlyer->m_pCamera = *(pScene->camera.begin() + ControlData.FlyerList.Flyers[i].CameraIndex);

				pFlyer->panSpeed = ControlData.FlyerList.Flyers[i].panSpeed;
				pFlyer->panAcc = ControlData.FlyerList.Flyers[i].panAcc;

				pFlyer->dollySpeed = ControlData.FlyerList.Flyers[i].dollySpeed;
				pFlyer->dollyAcc = ControlData.FlyerList.Flyers[i].dollyAcc;

				pFlyer->dollyNearDis = ControlData.FlyerList.Flyers[i].dollyNearDis;
				pFlyer->dollyFarDis = ControlData.FlyerList.Flyers[i].dollyFarDis;

				pFlyer->dollySpeed = ControlData.FlyerList.Flyers[i].dollySpeed;
				pFlyer->dollyAcc = ControlData.FlyerList.Flyers[i].dollyAcc;

				pFlyer->rotateSpeed = ControlData.FlyerList.Flyers[i].rotateSpeed;
				pFlyer->rotateAcc = ControlData.FlyerList.Flyers[i].rotateAcc;

				pFlyer->liftSpeed = ControlData.FlyerList.Flyers[i].liftSpeed;
				pFlyer->liftAcc = ControlData.FlyerList.Flyers[i].liftAcc;

				pFlyer->upperAngle = ControlData.FlyerList.Flyers[i].upperAngle;
				pFlyer->lowerAngle = ControlData.FlyerList.Flyers[i].lowerAngle;

				pFlyer->bCollision = ControlData.FlyerList.Flyers[i].bCollision;
				pFlyer->colDis = ControlData.FlyerList.Flyers[i].colDis;

				pScene->flyer.push_back(pFlyer);
			}
		}
		SAFE_DELETE_ARRAY(ControlData.FlyerList.Flyers);
	}
}

//读取声音数据
//##ModelId=47EBC74902CE
VOID  CGetV3DFileInfo_3_2::ReadAudioData()
{
	//找到声音信息的位置
	fseek(pFile, Header.FileTable.AudioDataPtr, SEEK_SET);

	//得到音频数量
	fread(&(AudioData.AudioCnt), sizeof(AudioData.AudioCnt), 1, pFile);
	if (AudioData.AudioCnt == 0)
		return;

	AudioData.Audio = new V3D_3_2_Audio[AudioData.AudioCnt];
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
//##ModelId=47EBC74902FD
VOID  CGetV3DFileInfo_3_2::ReadFlashData()
{
	//找到Flash信息的位置
	fseek(pFile, Header.FileTable.FlashDataPtr, SEEK_SET);

	//得到Flash数量
	fread(&(FlashData.FlashCnt), sizeof(FlashData.FlashCnt), 1, pFile);
	if (FlashData.FlashCnt == 0)
		return;
}

//读取模型组列表信息
//##ModelId=47EBC74A003E
VOID  CGetV3DFileInfo_3_2::ReadModelGroupData1(CModelGroup* pModelGroup)
{
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
	}

	//3.11以后的版本才有
	if (pScene->m_fileVersion >= 3.11)
	{
		//读入当前组对象的父级组对象
		INT parentModelGroupIndex;
		fread(&parentModelGroupIndex, sizeof(parentModelGroupIndex), 1, pFile);
		pModelGroup->parentModelGroupIndex = parentModelGroupIndex;
	}
}

//读取模型组列表信息
//##ModelId=47EBC74A001F
VOID  CGetV3DFileInfo_3_2::ReadModelGroupData()
{
	//获得模型组数据记录的偏移量
	fseek(pFile, Header.FileTable.ModelGroupDataPtr, SEEK_SET);

	//读入模型组数量
	fread(&(ModelGroupData.ModelGroupCnt), sizeof(ModelGroupData.ModelGroupCnt), 1, pFile);
	if (ModelGroupData.ModelGroupCnt == 0)
		return;

	ModelGroupData.ModelGroup = new V3D_3_2_ModelGroup[ModelGroupData.ModelGroupCnt];
	
	//逐个读取模型组信息
	for (UINT i =0; i< ModelGroupData.ModelGroupCnt; i++)
	{
		CModelGroup* pModelGroup = new CModelGroup;
		pModelGroup->index = pScene->modelGroup.size();

		//递归读入ModelGroup的数据
		ReadModelGroupData1(pModelGroup);
		pScene->modelGroup.push_back(pModelGroup);
	}

	//3.11以后的版本才有
	if (pScene->m_fileVersion >= 3.11)
	{
		//整理每个ModelGroup的parentGroup
		for (INT j = 0; j< pScene->modelGroup.size(); j++)
		{
			CModelGroup* pModelGroup = *(pScene->modelGroup.begin() + j);
			if (pModelGroup->parentModelGroupIndex != -1)
				pModelGroup->parent = *(pScene->modelGroup.begin() + pModelGroup->parentModelGroupIndex);
		}
	}

	//release memory
	SAFE_DELETE_ARRAY(ModelGroupData.ModelGroup);
}

//读取动态贴图信息
//##ModelId=47EBC74A00AC
VOID  CGetV3DFileInfo_3_2::ReadDynamicTexData()
{
	//读取动态贴图数据记录的偏移量
	fseek(pFile, Header.FileTable.DynamicTexDataPtr, SEEK_SET);

	//读入动态贴图的数量
	fread(&(DynamicTexData.DynamicTexCnt), sizeof(DynamicTexData.DynamicTexCnt), 1, pFile);
	if (DynamicTexData.DynamicTexCnt == 0) return;

	DynamicTexData.DynamicTex = new V3D_3_2_DynamicTex[DynamicTexData.DynamicTexCnt];
	for (INT i = 0; i< DynamicTexData.DynamicTexCnt; i++)
	{
		CDynamicTex* pDyTex = new CDynamicTex(pScene->m_pd3dDevice);
		pDyTex->index = pScene->dynamicTexture.size();

		//读入动态贴图的名称
		fread(&(DynamicTexData.DynamicTex[i].Name), 
			sizeof(DynamicTexData.DynamicTex[i].Name), 1, pFile);
		strcpy(pDyTex->name, DynamicTexData.DynamicTex[i].Name);

		//动态贴图的宽度
		fread(&(DynamicTexData.DynamicTex[i].Width), 
			sizeof(DynamicTexData.DynamicTex[i].Width), 1, pFile);
		pDyTex->width = DynamicTexData.DynamicTex[i].Width;

		//动态贴图的高度
		fread(&(DynamicTexData.DynamicTex[i].Height), 
			sizeof(DynamicTexData.DynamicTex[i].Height), 1, pFile);
		pDyTex->height = DynamicTexData.DynamicTex[i].Height;

		//贴图格式
		fread(&(DynamicTexData.DynamicTex[i].Format), 
			sizeof(DynamicTexData.DynamicTex[i].Format), 1, pFile);
		pDyTex->format = DynamicTexData.DynamicTex[i].Format;

		if (pDyTex->format == D3DFMT_A8R8G8B8 ||
			pDyTex->format == D3DFMT_A1R5G5B5 ||
			pDyTex->format == D3DFMT_A4R4G4B4)
			pDyTex->bAlphaBlend = TRUE;

		//得到动态贴图的帧数量
		fread(&(DynamicTexData.DynamicTex[i].FrameCnt), sizeof(DynamicTexData.DynamicTex[i].FrameCnt), 1, pFile);
		pDyTex->frameCnt = DynamicTexData.DynamicTex[i].FrameCnt;
		if (DynamicTexData.DynamicTex[i].FrameCnt == 0)
			continue;

		DynamicTexData.DynamicTex[i].FileNames = new V3D_3_2_FileName[DynamicTexData.DynamicTex[i].FrameCnt];
		//逐个读取动态贴图信息
		for (INT j =0; j< DynamicTexData.DynamicTex[i].FrameCnt; j++)
		{
			_FileName* pFileName = new _FileName;
			//读入帧图像文件名
			fread(&(DynamicTexData.DynamicTex[i].FileNames[j]), 
				sizeof(DynamicTexData.DynamicTex[i].FileNames[j]), 1, pFile);
			strcpy(pFileName->FileName, DynamicTexData.DynamicTex[i].FileNames[j].FileName);
			pDyTex->fileName.push_back(pFileName);

			_ImageData* pImageData = new _ImageData;
			//读入帧图像的数据
			FILE* pFile = fopen(DynamicTexData.DynamicTex[i].FileNames[j].FileName, "rb");
			fseek(pFile, 0, SEEK_END);
			DWORD fileLen = ftell(pFile);
			fseek(pFile, 0, SEEK_SET);
			pImageData->Data = new BYTE[fileLen];
			fread(pImageData->Data, fileLen, 1, pFile);
			pImageData->DataSize = fileLen;

			pDyTex->imageData.push_back(pImageData);

			fclose(pFile);
		}	
		SAFE_DELETE_ARRAY(DynamicTexData.DynamicTex[i].FileNames);

		//默认循环播放
		pDyTex->Play();
		pDyTex->bLoop = TRUE;

		pScene->dynamicTexture.push_back(pDyTex);
	}

	//release memory
	SAFE_DELETE_ARRAY(DynamicTexData.DynamicTex);
}

//-------------------------------------------------------------------
//方法: ReadVertexPosData()
//参数: 
//用途: 读取顶点位置信息
//-------------------------------------------------------------------
//##ModelId=47EBC749035B
VOID CGetV3DFileInfo_3_2::ReadVertexPosData()
{
	fseek(pFile, Header.FileTable.VertexPosDataPtr, SEEK_SET);
	
	//zipped vertex cnt;
	fread(&VertexPosData.VertexPosCnt, sizeof(VertexPosData.VertexPosCnt), 1, pFile);
	if (VertexPosData.VertexPosCnt == 0) return;

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
//##ModelId=47EBC749037A
VOID CGetV3DFileInfo_3_2::ReadVertexNormalData()
{
	fseek(pFile, Header.FileTable.VertexNormalDataPtr, SEEK_SET);
	
	//zipped vertex cnt;
	fread(&VertexNormalData.VertexNormalCnt, sizeof(VertexNormalData.VertexNormalCnt), 1, pFile);
	if (VertexNormalData.VertexNormalCnt == 0) return;

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
//##ModelId=47EBC7490399
VOID CGetV3DFileInfo_3_2::ReadVertexUVData()
{
	fseek(pFile, Header.FileTable.UVDataPtr, SEEK_SET);	

	//zipped uv count
	fread(&(UVData.UVCount), sizeof(UVData.UVCount), 1, pFile);
	if (UVData.UVCount == 0) return;

	UVData.UVList = new V3D_3_2_UV[UVData.UVCount];
	
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
//##ModelId=47EBC74903C8
VOID CGetV3DFileInfo_3_2::ReadVertexData()
{
	fseek(pFile, Header.FileTable.VertexDataPtr, SEEK_SET);

	//vertex count
	fread(&(VertexData.VertexCnt), sizeof(VertexData.VertexCnt), 1, pFile);
	if (VertexData.VertexCnt == 0) return;

	//vertex data
	VertexData.Vertex = new V3D_3_2_Vertex[VertexData.VertexCnt];
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
//##ModelId=47EBC74A0000
VOID CGetV3DFileInfo_3_2::ReadModelData()
{
	fseek(pFile, Header.FileTable.ModelDataPtr, SEEK_SET);

	//读入模型的数量
	fread(&(ModelData.ModelCnt), sizeof(ModelData.ModelCnt), 1, pFile);
	if (ModelData.ModelCnt == 0) return;

	ModelData.Model = new V3D_3_2_Model[ModelData.ModelCnt];
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
			pModel->shader = pScene->m_vShader[ModelData.Model[modelIndex].MaterialID];

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
			ModelData.Model[modelIndex].Face = new V3D_3_2_Face[ModelData.Model[modelIndex].FaceCnt];

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
				ModelData.Model[modelIndex].VertexPosList = new V3D_3_2_VertexPosList[ModelData.Model[modelIndex].KeyFrameCnt];
				
				pModel->vertices_kfr = new CVertex*[pModel->kfrCnt];
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
					SAFE_DELETE_ARRAY(ModelData.Model[modelIndex].VertexPosList[k].PosList);
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
		if (pModel->shader->bAlphaBlend || pModel->opacity < 100)
			pScene->transModel.push_back(pModel);

	}

	/*
	//等待所有的模型都加载到渲染队列后，再确定模型的索引模型
	INT modelCnt = pScene->model.GetCount();
	for (INT m = 0; m < modelCnt; m++)
	{
		CModel* pModel = (CModel*)(pScene->model.GetAt(m));
		if (pModel->refModelIndex != -1)
		{
			CModel* pRefModel = (CModel*)(pScene->model.GetAt(pModel->refModelIndex));
			pModel->refModel = pRefModel;
		}
	}
	*/

	//release memory
	SAFE_DELETE_ARRAY(VertexPosData.PosList);
	SAFE_DELETE_ARRAY(VertexNormalData.NormalList);
	SAFE_DELETE_ARRAY(UVData.UVList);
	SAFE_DELETE_ARRAY(VertexData.Vertex);

	SAFE_DELETE_ARRAY(ModelData.Model);
}

//-------------------------------------------------------------------
//方法: ReadCameraData()
//参数: 
//用途: 读取摄像机信息
//-------------------------------------------------------------------
//##ModelId=47EBC749032C
VOID CGetV3DFileInfo_3_2::ReadCameraData()
{
	fseek(pFile, Header.FileTable.CameraDataPtr, SEEK_SET);

	//得到摄像机数量
	fread(&(CameraData.CameraCnt), sizeof(CameraData.CameraCnt), 1, pFile);
	if (CameraData.CameraCnt == 0) return;

	CameraData.Camera = new V3D_3_2_Camera[CameraData.CameraCnt];
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
			CameraData.Camera[i].KFRInfo = new V3D_3_2_CameraKFRInfo[CameraData.Camera[i].KeyFrameCnt];
			
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
	
}

//-------------------------------------------------------------------
//方法: ReadTexData()
//参数: 
//用途: 读取贴图信息
//-------------------------------------------------------------------
//##ModelId=47EBC74A008C
VOID CGetV3DFileInfo_3_2::ReadTexData()
{
	fseek(pFile, Header.FileTable.TexDataPtr, SEEK_SET);

	//texture count
	fread(&(TexData.TexCnt), sizeof(TexData.TexCnt), 1, pFile);
	if (TexData.TexCnt == 0) return;

	CHAR errInfo[256];
	//texture data
	TexData.Textures= new V3D_3_2_Texture[TexData.TexCnt];
	for (UINT i= 0; i< TexData.TexCnt; i++)
	{
		CTexture* pTex = new CTexture();
		pTex->index = i;

		//读入贴图文件
		fread(&(TexData.Textures[i].Name), sizeof(TexData.Textures[i].Name), 1, pFile);
		fread(&(TexData.Textures[i].FileName), sizeof(TexData.Textures[i].FileName), 1, pFile);
		fread(&(TexData.Textures[i].Width), sizeof(TexData.Textures[i].Width), 1, pFile);
		fread(&(TexData.Textures[i].Height), sizeof(TexData.Textures[i].Height), 1, pFile);
		fread(&(TexData.Textures[i].MipCount), sizeof(TexData.Textures[i].MipCount), 1, pFile);
		fread(&(TexData.Textures[i].Format), sizeof(TexData.Textures[i].Format), 1, pFile);
		
		fread(&(TexData.Textures[i].Brightness), sizeof(TexData.Textures[i].Brightness), 1, pFile);
		fread(&(TexData.Textures[i].Contrast), sizeof(TexData.Textures[i].Contrast), 1, pFile);

		strcpy(pTex->name, TexData.Textures[i].Name);
		strcpy(pTex->pathAndName, TexData.Textures[i].FileName);
		pTex->width = TexData.Textures[i].Width;
		pTex->height = TexData.Textures[i].Height;
		pTex->BP = (D3DFORMAT)TexData.Textures[i].Format;
		if (pTex->BP == D3DFMT_A4R4G4B4 || 
			pTex->BP == D3DFMT_A8R8G8B8 ||
			pTex->BP == D3DFMT_A1R5G5B5)
			pTex->bAlphaBlend = TRUE;
		pTex->MipCount = TexData.Textures[i].MipCount;
		pTex->brightness = TexData.Textures[i].Brightness;
		pTex->contrast = TexData.Textures[i].Contrast;

		//创建D3D贴图对象
		//创建图像的Buffer,并从Buffer创建贴图
		BOOL bFindFile = TRUE;
		FILE* pTexFile = fopen(pTex->pathAndName, "rb");
		//如果找不到文件，则尝试相对路径
		if (!pTexFile)
		{
			CHAR path[256];
			GetFilePath(path, m_strFileName);

			CHAR name[32];
			_GetPureFileName(pTex->pathAndName, name);
			
			strcpy(pTex->pathAndName, path);
			strcat(pTex->pathAndName, name);
			
			pTexFile = fopen(pTex->pathAndName, "rb");

			if (!pTexFile)
			{
				strcpy(pTex->pathAndName, path);
				strcat(pTex->pathAndName, "texture\\");
				strcat(pTex->pathAndName, name);
				pTexFile = fopen(pTex->pathAndName, "rb");
			}
		}
		
		if (pTexFile)
		{
			D3DXGetImageInfoFromFile(pTex->pathAndName, &(pTex->imageInfo));
			//创建图像的Buffer,并从Buffer创建贴图
			fseek(pTexFile, 0, SEEK_END);
			pTex->dataSize = ftell(pTexFile);
			fseek(pTexFile, 0, SEEK_SET);
			pTex->data = new BYTE[pTex->dataSize];
			fread(pTex->data, pTex->dataSize, 1, pTexFile);
			fclose(pTexFile);
			if (pTex->data == NULL)
			{
				strcpy(errInfo, "读取贴图文件:");
				strcat(errInfo, pTex->pathAndName);
				strcat(errInfo, "不明原因失败");
				MessageBox(NULL, errInfo, "读取贴图文件...错误", MB_OK | MB_ICONEXCLAMATION);
				ZeroMemory(errInfo, sizeof(errInfo) );
			}
		}

		//贴图不在这里创建了，因为有可能有两张贴图组合成的带Alpha通道的贴图，
		//所以在读材质信息的时候创建
		pScene->texture.push_back(pTex);
	}
	//release memory
	SAFE_DELETE_ARRAY(TexData.Textures);
}

//-------------------------------------------------------------------
//方法: ReadMaterialData()
//参数: 
//用途: 读取材质信息
//-------------------------------------------------------------------
//##ModelId=47EBC749033C
VOID CGetV3DFileInfo_3_2::ReadMaterialData()
{
	fseek(pFile, Header.FileTable.MaterialDataPtr, SEEK_SET);

	fread(&(MaterialData.MaterialCnt), sizeof(MaterialData.MaterialCnt), 1, pFile);
	
	if (MaterialData.MaterialCnt == 0) return;

	MaterialData.Material = new V3D_3_2_Material[MaterialData.MaterialCnt];
	//得到材质列表
	for (UINT m =0; m< MaterialData.MaterialCnt; m++)
	{
		CMaterial* pMtrl = new CMaterial();
		pMtrl->index = m;
		
		//读入材质数据
		fread(&(MaterialData.Material[m]), sizeof(MaterialData.Material[m]), 1, pFile); 

		strcpy(pMtrl->name, MaterialData.Material[m].Name);
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

		pMtrl->fillMode = (_D3DFILLMODE)MaterialData.Material[m].FillMode;

		//diffuse tex
		if (MaterialData.Material[m].DifTexID != -1)
		{
			pMtrl->diffuseTexIndex = MaterialData.Material[m].DifTexID;
			pMtrl->diffuseTex = pScene->texture[MaterialData.Material[m].DifTexID];

			if (!pMtrl->bAlphaBlend)
				pMtrl->bAlphaBlend = pMtrl->diffuseTex->bAlphaBlend;
		}

		//opacity tex
		if (MaterialData.Material[m].OpaTexID != -1)
		{
			pMtrl->opacityTexIndex = MaterialData.Material[m].OpaTexID;
			pMtrl->opacityTex = pScene->texture[MaterialData.Material[m].OpaTexID];
			
			pMtrl->bAlphaBlend = TRUE;
		}

		//lightmap tex
		if (MaterialData.Material[m].LightTexID != -1)
		{
			pMtrl->lightTexIndex = MaterialData.Material[m].LightTexID;
			pMtrl->lightTex = pScene->texture[MaterialData.Material[m].LightTexID];
		}

		//reflection
		if (MaterialData.Material[m].ReflectionTexID != -1)
		{
			pMtrl->reflectionTexIndex = MaterialData.Material[m].ReflectionTexID;
			pMtrl->reflectionTex  = pScene->texture[MaterialData.Material[m].ReflectionTexID];
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
				//GetFilePath(strFilePath, m_strFileName);
				//strcat(strFilePath, pMovieTex->fileName);
				strcpy(strFilePath, pMovieTex->fileName);
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
			pMtrl->dynamicTex = pScene->dynamicTexture[pMtrl->dyTexIndex];

			if (!pMtrl->bAlphaBlend)
				pMtrl->bAlphaBlend = pMtrl->dynamicTex->bAlphaBlend;
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

		//如果Diffuse贴图不带Alpha通道，则看看Opacity通道是否有贴图，如果有则合成
		if (pMtrl->diffuseTex != NULL && !pMtrl->diffuseTex->bAlphaBlend)
		{
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
				if (pMtrl->diffuseTex->BP == D3DFMT_X8R8G8B8 || pMtrl->opacityTex->BP == D3DFMT_X8R8G8B8 ||
					pMtrl->diffuseTex->BP == D3DFMT_R8G8B8 || pMtrl->opacityTex->BP == D3DFMT_R8G8B8)
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

		//创建反射贴图
		if(pMtrl->reflectionTex != NULL )
		{
			if (pMtrl->reflectionTex->textureBuf == NULL)
			{
				if ( FAILED(pScene->CreateTexFromBuffer(pMtrl->reflectionTex, 
					pMtrl->reflectionTex->MipCount)) )
				{
					CHAR strErr[32];
					itoa(pMtrl->reflectionTex->index, strErr, 10);
					MessageBox(NULL, strErr, "创建贴图错误", MB_OK | MB_ICONERROR);
				}
				//SAFE_DELETE_ARRAY(pMtrl->reflectionTex->data);
			}
		}

		//创建高光贴图
		if(pMtrl->specularTex != NULL )
		{
			if (pMtrl->specularTex->textureBuf == NULL)
			{
				if ( FAILED(pScene->CreateTexFromBuffer(pMtrl->specularTex, 
					pMtrl->specularTex->MipCount)) )
				{
					CHAR strErr[32];
					itoa(pMtrl->specularTex->index, strErr, 10);
					MessageBox(NULL, strErr, "创建贴图错误", MB_OK | MB_ICONERROR);
				}
				//SAFE_DELETE_ARRAY(pMtrl->reflectionTex->data);
			}
		}

		//创建动态贴图
		if (pMtrl->dynamicTex != NULL)
		{
			if (pMtrl->dynamicTex->texture == NULL)
			{
				pMtrl->dynamicTex->texture = new CTexture();

				pMtrl->dynamicTex->texture->width = pMtrl->dynamicTex->width;
				pMtrl->dynamicTex->texture->height = pMtrl->dynamicTex->height;
				pMtrl->dynamicTex->texture->BP = pMtrl->dynamicTex->format;
				if (pMtrl->dynamicTex->texture->BP == D3DFMT_A8R8G8B8 ||
					pMtrl->dynamicTex->texture->BP == D3DFMT_A1R5G5B5 ||
					pMtrl->dynamicTex->texture->BP == D3DFMT_A4R4G4B4 )
				{
					pMtrl->bAlphaBlend = TRUE;
					pMtrl->dynamicTex->bAlphaBlend = TRUE;
				}

				_FileName** ppImageFile = pMtrl->dynamicTex->fileName.begin();
				_FileName* pImageFile = *(ppImageFile);

				pScene->CreateDyTexFromFile(pImageFile->FileName, pMtrl->dynamicTex->texture);
			}
		}

		//push material
		pScene->shader.push_back(pMtrl);
	}
	
	//release memory
	SAFE_DELETE_ARRAY(MaterialData.Material);

	//release memory
	SAFE_DELETE_ARRAY(CubeMapData.CubeMap);
}

//-------------------------------------------------------------------
//方法: ReadLightData()
//参数: 
//用途: 读入灯光信息
//-------------------------------------------------------------------
//##ModelId=47EBC749034B
VOID CGetV3DFileInfo_3_2::ReadLightData()
{
	fseek(pFile, Header.FileTable.LightDataPtr, SEEK_SET);

	//读入灯光数量
	fread(&(LightData.LightCnt), sizeof(LightData.LightCnt), 1, pFile);

	if (LightData.LightCnt == 0) return;

	//读入灯光详细数据
	LightData.Lights = new V3D_3_2_Light[LightData.LightCnt];
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
		pLight->param.Ambient = 0.05f * pLight->param.Diffuse;
		pLight->param.Ambient.a = 1.0f;

		//高光
		D3DCOLORVALUE color;
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 1.0f;
		color.a = 1.0f;
		pLight->specular = color;
		pLight->param.Specular = pLight->specular;

		switch (pLight->param.Type)
		{
		case D3DLIGHT_POINT:
			//灯光的位置
			pLight->param.Position.x = LightData.Lights[i].Pos.x;
			pLight->param.Position.y = LightData.Lights[i].Pos.y;
			pLight->param.Position.z = LightData.Lights[i].Pos.z;
			//光线到达的最远距离
			pLight->param.Range = 1500000000;//LightInfoList[i].LightFrame[0].Range;
			pLight->param.Attenuation0 = 1.0f;
			pLight->param.Attenuation1 = 0.0f;
			pLight->param.Attenuation2 = 0.0f;
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
			LightData.Lights[i].kfrInfo = new V3D_3_2_LightKfrInfo[pLight->kfrCnt];
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
}

//-------------------------------------------------------------------
//方法: ReadTextData()
//参数: 
//用途: 读取文字信息
//-------------------------------------------------------------------
//##ModelId=47EBC74A006D
VOID CGetV3DFileInfo_3_2::ReadTextData()
{
	fseek(pFile, Header.FileTable.TextDataPtr, SEEK_SET);

	//读入文字数量
	fread(&(TextData.TextCnt), sizeof(TextData.TextCnt), 1, pFile);

	//如果文字数量为0，则忽略保存文字内容列表字段
	if (TextData.TextCnt == 0) return;

	//text data
	TextData.Texts = new V3D_3_2_Text[TextData.TextCnt];
	for (UINT i= 0; i< TextData.TextCnt; i++)
	{
		//读入文字数据
		fread(&(TextData.Texts[i].Length), sizeof(TextData.Texts[i].Length), 1, pFile);

		//如果字符串为空，则忽略保存字符串的StrText字段
		if (TextData.Texts[i].Length > 0) 
		{
			TextData.Texts[i].StrText = new CHAR[TextData.Texts[i].Length + 1];
			fread(TextData.Texts[i].StrText, TextData.Texts[i].Length, 1, pFile);
		}

		fread(&(TextData.Texts[i].X), sizeof(TextData.Texts[i].X), 1, pFile);
		fread(&(TextData.Texts[i].Y), sizeof(TextData.Texts[i].Y), 1, pFile);
		fread(&(TextData.Texts[i].Color), sizeof(TextData.Texts[i].Color), 1, pFile);

		//字体名
		fread(&(TextData.Texts[i].FontName), sizeof(TextData.Texts[i].FontName), 1, pFile);
		//判断场景是否已经创建了该字体，否则创建新的字体
		CFont* pFont = pScene->IsFontCreated(TextData.Texts[i].FontName);
		if (pFont == NULL)
		{
			//创建字体
			pFont = new CFont(pScene->m_pd3dDevice);
			pFont->Init(TextData.Texts[i].FontName);
			pScene->font.push_back(pFont);
		}

		CText* pText = new CText(pFont);
		pText->index = i;

		pText->strText = new CHAR[TextData.Texts[i].Length];
		strcpy(pText->strText, TextData.Texts[i].StrText);
		pText->strText[TextData.Texts[i].Length] = '\0';
		//release memory
		SAFE_DELETE_ARRAY(TextData.Texts[i].StrText);

		pText->rect.left = TextData.Texts[i].X ;
		pText->rect.top = TextData.Texts[i].Y;
		pText->rect.right = pText->rect.left + TextData.Texts[i].Length * pFont->GetHeight();
		pText->rect.bottom = pText->rect.top + pFont->GetHeight();
		pText->color = TextData.Texts[i].Color;

		//关键帧帧数
		fread(&(TextData.Texts[i].KfrCnt), sizeof(TextData.Texts[i].KfrCnt), 1, pFile);
		pText->m_kfrCnt = TextData.Texts[i].KfrCnt;

		//关键帧数据
		if (pText->m_kfrCnt > 0)
		{
			TextData.Texts[i].KfrInfo = new V3D_3_2_TextInfo[pText->m_kfrCnt];
			pText->m_kfrInfo = new _TextKfrInfo[pText->m_kfrCnt];
			for (INT j = 0; j< pText->m_kfrCnt; j++)
			{
				fread(&(TextData.Texts[i].KfrInfo[j]), sizeof(TextData.Texts[i].KfrInfo[j]), 1, pFile);
				pText->m_kfrInfo[j].x = TextData.Texts[i].KfrInfo[j].X;
				pText->m_kfrInfo[j].y = TextData.Texts[i].KfrInfo[j].Y;
				pText->m_kfrInfo[j].color = TextData.Texts[i].KfrInfo[j].Color;
			}
			SAFE_DELETE_ARRAY(TextData.Texts[i].KfrInfo);
		}

		pScene->text.push_back(pText);

	}

	//release memory
	SAFE_DELETE_ARRAY(TextData.Texts);
}

//-------------------------------------------------------------------
//方法: ReadOverlayData()
//参数: 
//用途: 读取Overlay信息
//-------------------------------------------------------------------
//##ModelId=47EBC74A007D
VOID CGetV3DFileInfo_3_2::ReadOverlayData()
{
	fseek(pFile, Header.FileTable.OverlayDataPtr, SEEK_SET);

	//读入Overlay数量
	fread(&(OverlayData.OverlayCnt), sizeof(OverlayData.OverlayCnt), 1, pFile);

	//如果Overlay的数量为0,则忽略保存Ovelay内容列表字段
	if (OverlayData.OverlayCnt == 0) return;

	//overlay data
	OverlayData.Overlays = new V3D_3_2_Overlay[OverlayData.OverlayCnt];
	for (UINT i= 0; i< OverlayData.OverlayCnt; i++)
	{
		COverlay* pOverlay = new COverlay(pScene->m_pd3dDevice);
		
		pOverlay->index = i;

		//读入Overlay的名字
		fread(&(OverlayData.Overlays[i].Name), sizeof(OverlayData.Overlays[i].Name), 1, pFile);
		strcpy(pOverlay->name, OverlayData.Overlays[i].Name);

		//写入Overlay的偏移坐标原点标示
		fread(&(OverlayData.Overlays[i].OffsetOrigin), sizeof(OverlayData.Overlays[i].OffsetOrigin), 1, pFile);
		pOverlay->offsetOrigin = OverlayData.Overlays[i].OffsetOrigin;
		
		//读入Overlay的位置标示(相对坐标还是绝对坐标)
		fread(&(OverlayData.Overlays[i].UseRelativePos), sizeof(OverlayData.Overlays[i].UseRelativePos), 1, pFile);
		pOverlay->useRelativePos = OverlayData.Overlays[i].UseRelativePos;

		//读入Overlay尺寸
		fread(&(OverlayData.Overlays[i].Rect), sizeof(OverlayData.Overlays[i].Rect), 1, pFile);
		RECT Rect = OverlayData.Overlays[i].Rect;
		pOverlay->SetRect(&Rect);
		pOverlay->offsetRect = Rect;
		//计算相对尺寸
		pOverlay->m_x1Percent = (FLOAT)Rect.left / pScene->m_oriWidth;
		pOverlay->m_y1Percent = (FLOAT)Rect.top / pScene->m_oriHeight;
		pOverlay->m_x2Percent = (FLOAT)Rect.right / pScene->m_oriWidth;
		pOverlay->m_y2Percent = (FLOAT)Rect.bottom / pScene->m_oriHeight;

		//读入Overlay转角
		fread(&(OverlayData.Overlays[i].Angle), sizeof(OverlayData.Overlays[i].Angle), 1, pFile);
		FLOAT angle = OverlayData.Overlays[i].Angle;
		pOverlay->SetRotation(angle);

		//读入Overlay的透明度
		fread(&(OverlayData.Overlays[i].Opacity), sizeof(OverlayData.Overlays[i].Opacity), 1, pFile);
		INT opacity = OverlayData.Overlays[i].Opacity;
		pOverlay->SetOpacity(opacity);

		//是否是背景图
		fread(&(OverlayData.Overlays[i].IsBackdrop), sizeof(OverlayData.Overlays[i].IsBackdrop), 1, pFile);
		pOverlay->isBackdrop = OverlayData.Overlays[i].IsBackdrop;

		//读入Overlay的材质ID
		fread(&(OverlayData.Overlays[i].MaterialID), sizeof(OverlayData.Overlays[i].MaterialID), 1, pFile);
		if (OverlayData.Overlays[i].MaterialID != -1)
		{
			UINT index = OverlayData.Overlays[i].MaterialID;
			pOverlay->m_pMtrl = *(pScene->shader.begin() + index);

			if (pOverlay->m_pMtrl->diffuseTex)
			{
				pOverlay->m_imageWidth = pOverlay->m_pMtrl->diffuseTex->imageInfo.Width;
				pOverlay->m_imageHeight = pOverlay->m_pMtrl->diffuseTex->imageInfo.Height;
			}
			else if (pOverlay->m_pMtrl->compTex)
			{
				pOverlay->m_imageWidth = pOverlay->m_pMtrl->compTex->imageInfo.Width;
				pOverlay->m_imageHeight = pOverlay->m_pMtrl->compTex->imageInfo.Height;
			}
		}

		//读入Overlay是否使用图像尺寸
		fread(&(OverlayData.Overlays[i].UseRelativeSize), sizeof(OverlayData.Overlays[i].UseRelativeSize), 1, pFile);
		pOverlay->useRelativeSize = OverlayData.Overlays[i].UseRelativeSize;
		
		if (!pOverlay->useRelativeSize)
		{
			pOverlay->SetRect(&Rect);
			pOverlay->offsetRect = Rect;
		}
		else
		{
			INT width = pScene->rect.right - pScene->rect.left;
			INT height = pScene->rect.bottom - pScene->rect.top;
			if (pOverlay->useRelativeSize)
			{
				Rect.left = pOverlay->m_x1Percent * width;
				Rect.top = pOverlay->m_y1Percent * height;
			}
			Rect.right = pOverlay->m_x2Percent * width;
			Rect.bottom = pOverlay->m_y2Percent * height;
			pOverlay->SetRect(&Rect);
			pOverlay->offsetRect = Rect;
		}

		//读入Overlay的关键帧数量
		fread(&(OverlayData.Overlays[i].KfrCnt), sizeof(OverlayData.Overlays[i].KfrCnt), 1, pFile);
		pOverlay->m_kfrCnt = OverlayData.Overlays[i].KfrCnt;

		//写入关键帧数据
		if (pOverlay->m_kfrCnt > 0)
		{
			OverlayData.Overlays[i].KfrInfo = new V3D_3_2_OverlayInfo[pOverlay->m_kfrCnt];
			for (INT j = 0; j< pOverlay->m_kfrCnt; j++)
			{
				fread(&(OverlayData.Overlays[i].KfrInfo[j]), sizeof(OverlayData.Overlays[i].KfrInfo[j]), 1, pFile);
				pOverlay->m_kfrInfo[j].Rect = OverlayData.Overlays[i].KfrInfo[j].Rect;
				pOverlay->m_kfrInfo[j].Angle = OverlayData.Overlays[i].KfrInfo[j].Angle;
				pOverlay->m_kfrInfo[j].Opacity = OverlayData.Overlays[i].KfrInfo[j].Opacity;
			}
			SAFE_DELETE_ARRAY(OverlayData.Overlays[i].KfrInfo);
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
//##ModelId=47EBC74A007E
VOID CGetV3DFileInfo_3_2::ReadNavigaterData()
{
	fseek(pFile, Header.FileTable.NavigaterDataPtr, SEEK_SET);

	//navigate count
	fread(&(NavigaterData.NavigaterCnt), sizeof(NavigaterData.NavigaterCnt), 1, pFile);
	
	//如果navigater的数量为0,则忽略保存navigater内容列表字段
	if (NavigaterData.NavigaterCnt == 0) return;

	//navigate data
	NavigaterData.Navigaters = new V3D_3_2_Navigater[NavigaterData.NavigaterCnt];
	for (UINT i= 0; i< NavigaterData.NavigaterCnt; i++)
	{
		CNavigater* pNavigater = new CNavigater();
		if (pNavigater == NULL) return;
		
		//读入pNavigater的名字
		fread(&(NavigaterData.Navigaters[i].Name), sizeof(NavigaterData.Navigaters[i].Name), 1, pFile);
		strcpy(pNavigater->name, NavigaterData.Navigaters[i].Name);
	
		//读入navigate的navRect
		fread(&(NavigaterData.Navigaters[i].Rect), sizeof(NavigaterData.Navigaters[i].Rect), 1, pFile);
		pNavigater->navRect = NavigaterData.Navigaters[i].Rect;
		
		//读入navigate的mapOverlay的Index
		fread(&(NavigaterData.Navigaters[i].MapOverlayIndex), sizeof(NavigaterData.Navigaters[i].MapOverlayIndex), 1, pFile);
		INT overlayIndex = NavigaterData.Navigaters[i].MapOverlayIndex;
		pNavigater->pNavigateMap = *(pScene->overlay.begin() + overlayIndex);
		
		//读入Pointer的数量
		fread(&(NavigaterData.Navigaters[i].PointerCnt), sizeof(NavigaterData.Navigaters[i].PointerCnt), 1, pFile);
		INT pointerCnt = NavigaterData.Navigaters[i].PointerCnt;
		
		//读入Pointer数据
		if (pointerCnt > 0)
		{
			NavigaterData.Navigaters[i].NavPointer = new V3D_3_2_NavigaterPointer[pointerCnt];
			for (INT j = 0; j<pointerCnt; j++)
			{
				fread(&(NavigaterData.Navigaters[i].NavPointer[j]), sizeof(NavigaterData.Navigaters[i].NavPointer[j]), 1, pFile);
		
				NavPointer* pNavPointer = new NavPointer;
				
				//Pointer名称
				strcpy(pNavPointer->Name, NavigaterData.Navigaters[i].NavPointer[j].Name);

				//Pointer Overlay编号
				INT OLIndex = NavigaterData.Navigaters[i].NavPointer[j].OverlayIndex;
				pNavPointer->pPointerOverlay = *(pScene->overlay.begin() + OLIndex);
				pNavPointer->pPointerOverlay->useRelativeSize = FALSE;
				
				//Pointer代表的对象类型
				pNavPointer->NavPointerType = NavigaterData.Navigaters[i].NavPointer[j].Type;
			
				//Pointer代表的对象的编号
				CModel* pModel = NULL;
				CCamera* pCamera = NULL;
				if (pNavPointer->NavPointerType == 0)
				{
					INT index = NavigaterData.Navigaters[i].NavPointer[j].ObjIndex;
					CCamera* pCamera = *(pScene->camera.begin() + index);
					pNavPointer->NavPointerHandle = (DWORD)pCamera;
				}
				if (pNavPointer->NavPointerType == 1)
				{
					INT index = NavigaterData.Navigaters[i].NavPointer[j].ObjIndex;
					CModel* pModel = *(pScene->model.begin() + index);
					pNavPointer->NavPointerHandle = (DWORD)pModel;
				}

				//将导航指针加入导航图对象
				pNavigater->AddNavPointer(pNavPointer);	

			}
			SAFE_DELETE_ARRAY(NavigaterData.Navigaters[i].NavPointer);
		}

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
						
		//将 navigater Push到 navigater 列表
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
//##ModelId=47EBC74A007F
VOID CGetV3DFileInfo_3_2::ReadButtonData()
{
	fseek(pFile, Header.FileTable.ButtonDataPtr, SEEK_SET);

	//button count
	fread(&(ButtonData.ButtonCnt), sizeof(ButtonData.ButtonCnt), 1, pFile);

	//如果button的数量为0,则忽略保存button内容列表字段
	if (ButtonData.ButtonCnt == 0) return;

	//button data
	ButtonData.Buttons = new V3D_3_2_Button[ButtonData.ButtonCnt];
	for (UINT i= 0; i< ButtonData.ButtonCnt; i++)
	{
		CButton* pButton = new CButton();
		if (pButton == NULL) return;
		
		//读入pbutton的名字
		fread(&(ButtonData.Buttons[i].Name), sizeof(ButtonData.Buttons[i].Name), 1, pFile);
		strcpy(pButton->name, ButtonData.Buttons[i].Name);
		
		//读入button的Rect
		fread(&(ButtonData.Buttons[i].Rect), sizeof(ButtonData.Buttons[i].Rect), 1, pFile);
		RECT Rect = ButtonData.Buttons[i].Rect;
		
		//读入Button的透明度
		fread(&(ButtonData.Buttons[i].Opacity), sizeof(ButtonData.Buttons[i].Opacity), 1, pFile);
		INT opacity = ButtonData.Buttons[i].Opacity;

		//读入button的回调函数名称
		fread(&(ButtonData.Buttons[i].CallBackFunc), sizeof(ButtonData.Buttons[i].CallBackFunc), 1, pFile);
		strcpy(pButton->callBackFunc, ButtonData.Buttons[i].CallBackFunc);			
	
		//读入button的Overlay的Index
		fread(&(ButtonData.Buttons[i].BtnOverlayIndex), sizeof(ButtonData.Buttons[i].BtnOverlayIndex), 1, pFile);
		INT index = ButtonData.Buttons[i].BtnOverlayIndex;
		pButton->btnOverlay = *(pScene->overlay.begin() + index);
		
		//改变透明度
		pButton->btnOverlay->SetOpacity(opacity);
		
		//改变ButtonOverlay 的Rect
		pButton->btnOverlay->SetRect(&Rect);
		pButton->btnOverlay->offsetRect = Rect;
		//计算相对尺寸
		pButton->btnOverlay->m_x1Percent = (FLOAT)Rect.left / pScene->m_oriWidth;
		pButton->btnOverlay->m_y1Percent = (FLOAT)Rect.top / pScene->m_oriHeight;
		pButton->btnOverlay->m_x2Percent = (FLOAT)Rect.right / pScene->m_oriWidth;
		pButton->btnOverlay->m_y2Percent = (FLOAT)Rect.bottom / pScene->m_oriHeight;
	
		if (!pButton->btnOverlay->useRelativeSize)
		{
			pButton->btnOverlay->SetRect(&Rect);
			pButton->btnOverlay->offsetRect = Rect;
		}
		else
		{
			INT width = pScene->rect.right - pScene->rect.left;
			INT height = pScene->rect.bottom - pScene->rect.top;
			Rect.left = pButton->btnOverlay->m_x1Percent * width;
			Rect.top = pButton->btnOverlay->m_y1Percent * height;
			Rect.right = pButton->btnOverlay->m_x2Percent * width;
			Rect.bottom = pButton->btnOverlay->m_y2Percent * height;
			pButton->btnOverlay->SetRect(&Rect);
			pButton->btnOverlay->offsetRect = Rect;
		}

		//读入button的mouseOver Mtrl的Index
		fread(&(ButtonData.Buttons[i].MouseOverMtrlIndex), sizeof(ButtonData.Buttons[i].MouseOverMtrlIndex), 1, pFile);
		INT MouseOverMtrlIndex = ButtonData.Buttons[i].MouseOverMtrlIndex; 
		if (MouseOverMtrlIndex >= 0)
			pButton->pMtrl_MouseOver = *(pScene->shader.begin() + MouseOverMtrlIndex);
		else
			pButton->pMtrl_MouseOver = pButton->btnOverlay->m_pMtrl;

		//读入button的mouseDown Mtrl的Index
		fread(&(ButtonData.Buttons[i].MouseDownMtrlIndex), sizeof(ButtonData.Buttons[i].MouseDownMtrlIndex), 1, pFile);
		INT MouseDownMtrlIndex = ButtonData.Buttons[i].MouseDownMtrlIndex;
		if (MouseDownMtrlIndex >= 0)
			pButton->pMtrl_MouseDown = *(pScene->shader.begin() + MouseDownMtrlIndex);
		else
			pButton->pMtrl_MouseDown = pButton->btnOverlay->m_pMtrl;

		//读入Button的关键帧数量
		fread(&(ButtonData.Buttons[i].KfrCnt), sizeof(ButtonData.Buttons[i].KfrCnt), 1, pFile);
		pButton->m_kfrCnt = ButtonData.Buttons[i].KfrCnt;

		//读入Button的关键帧数据
		if (pButton->m_kfrCnt > 0)
		{
			ButtonData.Buttons[i].KfrInfo = new V3D_3_2_ButtonInfo[pButton->m_kfrCnt];
			for (INT j = 0; j< pButton->m_kfrCnt; j++)
			{
				fread(&(ButtonData.Buttons[i].KfrInfo[j]), sizeof(ButtonData.Buttons[i].KfrInfo[j]), 1, pFile);		
				pButton->m_kfrInfo[j].Rect = ButtonData.Buttons[i].KfrInfo[j].Rect;
				pButton->m_kfrInfo[j].Opacity = ButtonData.Buttons[i].KfrInfo[j].Opacity;
			}
			SAFE_DELETE_ARRAY(ButtonData.Buttons[i].KfrInfo);
		}
				
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
//##ModelId=47EBC74A009C
VOID CGetV3DFileInfo_3_2::ReadImageData()
{
	fseek(pFile, Header.FileTable.ImageDataPtr, SEEK_SET);

	//image count
	fread(&(ImageData.ImageCount), sizeof(ImageData.ImageCount), 1, pFile);
	
	//如果Image的数量为0,则忽略保存Image内容列表字段
	if (ImageData.ImageCount == 0) return;

	//image data
	ImageData.Images = new V3D_3_2_Image[ImageData.ImageCount];
	for (UINT i= 0; i< ImageData.ImageCount; i++)
	{
		CImage* pImage = new CImage();
		if (pImage == NULL) return;
		
		//读入Image的名字
		fread(&(ImageData.Images[i].Name), sizeof(ImageData.Images[i].Name), 1, pFile);
		strcpy(pImage->name, ImageData.Images[i].Name);

		//Image的Index
		pImage->index = i;

		fread(&(ImageData.Images[i].FileName), sizeof(ImageData.Images[i].FileName), 1, pFile);
		strcpy(pImage->pathAndName, ImageData.Images[i].FileName);

		//读入Image的宽度
		fread(&(ImageData.Images[i].Width), sizeof(ImageData.Images[i].Width), 1, pFile);
		pImage->width = ImageData.Images[i].Width;
		//读入Image的高度
		fread(&(ImageData.Images[i].Height), sizeof(ImageData.Images[i].Height), 1, pFile);
		pImage->height = ImageData.Images[i].Height;
		//读入Image的格式
		fread(&(ImageData.Images[i].Format), sizeof(ImageData.Images[i].Format), 1, pFile);
		pImage->format = ImageData.Images[i].Format;

		//创建图像的Buffer
		FILE* pImageFile = fopen(pImage->pathAndName, "rb");
		if (pImageFile)
		{
			//创建图像的Buffer,并从Buffer创建贴图
			fseek(pImageFile, 0, SEEK_END);
			pImage->dataSize = ftell(pImageFile);
			fseek(pImageFile, 0, SEEK_SET);
			pImage->data = new BYTE[pImage->dataSize];
			fread(pImage->data, pImage->dataSize, 1, pImageFile);
			fclose(pImageFile);
			if (pImage->data == NULL)
			{
				CHAR errInfo[256];
				strcpy(errInfo, "读取图像文件:");
				strcat(errInfo, pImage->pathAndName);
				strcat(errInfo, "不明原因失败");
				MessageBox(NULL, errInfo, "读取图像文件...错误", MB_OK | MB_ICONEXCLAMATION);
				ZeroMemory(errInfo, sizeof(errInfo) );
			}
		}

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
//##ModelId=47EBC74A009D
VOID CGetV3DFileInfo_3_2::ReadCubeMapData()
{
	fseek(pFile, Header.FileTable.CubeMapDataPtr, SEEK_SET);

	CubeMapData.CubeMap = NULL;
	//cubeMap count
	fread(&(CubeMapData.CubeMapCnt), sizeof(CubeMapData.CubeMapCnt), 1, pFile);
	
	//如果cubeMap的数量为0
	if (CubeMapData.CubeMapCnt == 0) return;

	//判断是否支持
	//if (!vgsRender->m_DeviceInfo->IsSupport_D3D_CUBEMAP)
	//	return;

	//cubeMap data
	CubeMapData.CubeMap = new V3D_3_2_CubeMap[CubeMapData.CubeMapCnt];
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
		CTexture** ppTex = pScene->texture.begin();
		if (CubeMapData.CubeMap[i].Front_Z != -1)
		{
			pCubeMap->m_pFrontTex = *(ppTex + CubeMapData.CubeMap[i].Front_Z);
			if (pCubeMap->m_pFrontTex->textureBuf == NULL)
				pScene->CreateTexFromBuffer(pCubeMap->m_pFrontTex, 1);
		}
		else
			pCubeMap->m_pFrontTex = NULL;

		if (CubeMapData.CubeMap[i].Back_NZ != -1)
		{
			pCubeMap->m_pBackTex = *(ppTex + CubeMapData.CubeMap[i].Back_NZ);
			if (pCubeMap->m_pBackTex->textureBuf == NULL)
				pScene->CreateTexFromBuffer(pCubeMap->m_pBackTex, 1);
		}
		else
			pCubeMap->m_pBackTex = NULL;

		if (CubeMapData.CubeMap[i].Left_NX != -1)
		{
			pCubeMap->m_pLeftTex = *(ppTex + CubeMapData.CubeMap[i].Left_NX);
			if (pCubeMap->m_pLeftTex->textureBuf == NULL)
				pScene->CreateTexFromBuffer(pCubeMap->m_pLeftTex, 1);
		}
		else
			pCubeMap->m_pLeftTex = NULL;

		if (CubeMapData.CubeMap[i].Right_X != -1)
		{
			pCubeMap->m_pRightTex = *(ppTex + CubeMapData.CubeMap[i].Right_X);
			if (pCubeMap->m_pRightTex->textureBuf == NULL)
				pScene->CreateTexFromBuffer(pCubeMap->m_pRightTex, 1);
		}
		else
			pCubeMap->m_pRightTex = NULL;

		if (CubeMapData.CubeMap[i].Top_Y != -1)
		{
			pCubeMap->m_pTopTex = *(ppTex + CubeMapData.CubeMap[i].Top_Y);
			if (pCubeMap->m_pTopTex->textureBuf == NULL)
				pScene->CreateTexFromBuffer(pCubeMap->m_pTopTex, 1);
		}
		else
			pCubeMap->m_pTopTex = NULL;

		if (CubeMapData.CubeMap[i].Bottom_NY != -1)
		{
			pCubeMap->m_pBottomTex = *(ppTex + CubeMapData.CubeMap[i].Bottom_NY);
			if (pCubeMap->m_pBottomTex->textureBuf == NULL)
				pScene->CreateTexFromBuffer(pCubeMap->m_pBottomTex, 1);
		}
		else
			pCubeMap->m_pBottomTex = NULL;

		//初始化CubeMap
		pCubeMap->InitCubeMap(0, pScene->m_d3dpp.BackBufferFormat, 
								CubeMapData.CubeMap[i].Length);

		pScene->cubeMap.push_back(pCubeMap);
	}

}

//读取MovieTex信息，2.5版本Up
//##ModelId=47EBC74A009E
VOID CGetV3DFileInfo_3_2::ReadMovieTexData()
{
	fseek(pFile, Header.FileTable.MovieTexDataPtr, SEEK_SET);

	//MovieTexture count
	fread(&(MovieTexData.MovieTexCnt), sizeof(MovieTexData.MovieTexCnt), 1, pFile);
	
	//如果MovieTex的数量为0,则忽略保存MovieTexData内容列表字段
	if (MovieTexData.MovieTexCnt == 0) return;

	MovieTexData.MovieTex = new V3D_3_2_MovieTex[MovieTexData.MovieTexCnt];
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
}

//读取BumpWater信息，2.5版本Up
//##ModelId=47EBC74A00AB
VOID CGetV3DFileInfo_3_2::ReadBumpWaterData()
{
	fseek(pFile, Header.FileTable.BumpWaterDataPtr, SEEK_SET);

	//获得BumpWater的数量
	fread(&(BumpWaterData.BumpWaterCnt), sizeof(BumpWaterData.BumpWaterCnt), 1, pFile);
	
	//如果BumpWaterData的数量为0,则忽略BumpWaterData内容列表字段
	if (BumpWaterData.BumpWaterCnt == 0) return;

	BumpWaterData.BumpWater = new V3D_3_2_BumpWater[BumpWaterData.BumpWaterCnt];
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
	SAFE_DELETE_ARRAY(BumpWaterData.BumpWater);	
}

//-------------------------------------------------------------------
//方法: FindReferenceModel()
//参数: 
//用途: 查找每个模型的引用模型并得到引用模型指针
//-------------------------------------------------------------------
//##ModelId=47EBC74A0138
VOID CGetV3DFileInfo_3_2::FindReferenceModel()
{
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
	
}

//-------------------------------------------------------------------
//方法: LoadE3DFile()
//参数: CScene* pScene, CHAR*  strFileName
//用途: 读取对应的Scene场景为E3D文件
//-------------------------------------------------------------------
//##ModelId=47EBC74A0139
HRESULT CGetV3DFileInfo_3_2::LoadV3DFile(CScene* theScene, CHAR*  strFileName)
{
	//指定场景
	if (theScene == NULL)
	{
		MessageBox(NULL, "场景构造失败", "打开E3D文件过程错误", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	pScene = theScene;
	strcpy(m_strFileName, strFileName);

	if (strcmp(strFileName,"") == 0) 
		return S_OK;

	//打开文件IO
	if (NULL == (pFile = fopen(strFileName, "rb")) )
	{
		MessageBox(NULL, "没有找到文件，指定的V3D文件无法打开", "打开V3D文件错误", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//read the header data
	ReadHeader();
	//read WindowProp
	ReadWindowProp();
	//读入授权信息
	ReadPrivacyData();

	//read the camera data
	ReadCameraData();

	//read Audio data
	ReadAudioData();
	//read flash data
	ReadFlashData();

	//read the texture data
	ReadTexData();
	//读取CubeMap数据
	ReadCubeMapData();
	//读取BumpWater数据
	ReadBumpWaterData();
	//读取电影贴图数据
	ReadMovieTexData();
	//read dynamic texture data
	ReadDynamicTexData();
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
	//read the modelGroup data
	ReadModelGroupData();
	//找到每个模型对应的索引模型
	FindReferenceModel();


	//读取文字信息
	ReadTextData();
	//读取Overlay信息
	ReadOverlayData();
	//读取Navigater信息
	ReadNavigaterData();
	//读取Button数据
	ReadButtonData();
	//读取图像数据
	ReadImageData();

	//读入控制信息
	ReadContorlData();

	//close the pFile
	fclose(pFile);
	return S_OK;
}		

//##ModelId=47EBC74A0148
VOID CGetV3DFileInfo_3_2::Release()
{
	//release camera data
	if (CameraData.Camera)
	{
		for (INT i =0; i< CameraData.CameraCnt; i++)
		{
			if (CameraData.Camera[i].KeyFrameCnt > 1)
				SAFE_DELETE_ARRAY(CameraData.Camera[i].KFRInfo);
		}
		SAFE_DELETE_ARRAY(CameraData.Camera);
	}

	//release light data
	if (LightData.Lights)
	{
		for (INT i = 0; i< LightData.LightCnt; i++)	
		{
			if (LightData.Lights[i].NumFrame > 1)
				SAFE_DELETE_ARRAY(LightData.Lights[i].kfrInfo);
		}
		SAFE_DELETE_ARRAY(LightData.Lights);
	}

	//release model data
	if (ModelData.Model)
	{
		for(INT i= 0; i< ModelData.ModelCnt; i++)
		{
			if (ModelData.Model[i].FaceCnt > 0)
				SAFE_DELETE_ARRAY(ModelData.Model[i].Face);
			if (ModelData.Model[i].KeyFrameCnt > 1)
				SAFE_DELETE_ARRAY(ModelData.Model[i].KfrInfo);
			if (ModelData.Model[i].VKeyFrameCnt > 1)
				SAFE_DELETE_ARRAY(ModelData.Model[i].VertexPosList);
		}
		//release memory
		if (ModelData.ModelCnt > 0)
			SAFE_DELETE_ARRAY(ModelData.Model);
		if (VertexData.VertexCnt > 0)
			SAFE_DELETE_ARRAY(VertexData.Vertex);
		if (VertexPosData.VertexPosCnt > 0)
			SAFE_DELETE_ARRAY(VertexPosData.PosList);
		if (UVData.UVCount > 0)
			SAFE_DELETE_ARRAY(UVData.UVList);
	}

	//release text
	if (TextData.Texts)
	{
		for (INT i= 0; i< TextData.TextCnt; i++)
			SAFE_DELETE_ARRAY(TextData.Texts[i].StrText);
		SAFE_DELETE_ARRAY(TextData.Texts);
	}

	//release material
	if (MaterialData.Material)
		SAFE_DELETE_ARRAY(MaterialData.Material);

	//release Overlays
	if (OverlayData.Overlays)
		SAFE_DELETE_ARRAY(OverlayData.Overlays);

	//release Navigaters
	if (NavigaterData.Navigaters)
		SAFE_DELETE_ARRAY(NavigaterData.Navigaters);

	//release Buttons
	if (ButtonData.Buttons)
		SAFE_DELETE_ARRAY(ButtonData.Buttons);
	
	//CubeMap列表，
	if (CubeMapData.CubeMap)
		SAFE_DELETE_ARRAY(CubeMapData.CubeMap);
	
	//电影贴图列表
	if (MovieTexData.MovieTex)
		SAFE_DELETE_ARRAY(MovieTexData.MovieTex);
	
	//水面贴图列表
	if (BumpWaterData.BumpWater)
		SAFE_DELETE_ARRAY(BumpWaterData.BumpWater);
}
