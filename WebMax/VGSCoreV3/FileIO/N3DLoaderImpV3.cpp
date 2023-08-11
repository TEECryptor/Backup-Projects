#include "N3DLoaderImpV3.h"
#include "..\\CModel.h"
#include "..\\VGSSupport.h"
#include "..\\Root.h"

CN3DLoaderImpV3::CN3DLoaderImpV3()
{
	m_minVersion = 52000;
	m_maxVersion = 52999;
}
CN3DLoaderImpV3::~CN3DLoaderImpV3()
{

}

// 读文件头
void CN3DLoaderImpV3::ReadHeader()
{
	BYTE* pOffset = m_pSceneData; 
	if (NULL == pOffset)
		return;

	ZeroMemory(&m_Header, sizeof(m_Header));

	// 读基本文件头信息
	CPY_FROM_STREAM(m_Header.baseHeader, pOffset);

	// 读3.0扩展信息
	CPY_FROM_STREAM(m_Header.imageCnt, pOffset);
	CPY_FROM_STREAM_EX(m_Header.pImageIndexData, m_Header.imageCnt * sizeof(DWORD), pOffset);
}

//读取相机信息
VOID CN3DLoaderImpV3::ReadCameraData()
{
	BYTE* pOffset = m_pSceneData + TableData.cameraDataPtr;

	m_preCameraCnt = m_pScene->getCameraCount();

	m_vCamN3DTransform.clear();

	//得到摄像机数量
	DWORD cameraCnt = 0;
	CPY_FROM_STREAM(cameraCnt, pOffset);

	//逐个读取摄像机信息
	for (UINT i = 0; i < cameraCnt; i++)
	{	
		N3D_Camera curN3DCamera;
		// N3D_Camera &curN3DCamera = vN3DCam[i];  
		UINT len;
		CPY_FROM_STREAM(len, pOffset);
		char readName[MAX_FILENAME_LENGTH];
		ZeroMemory(readName, sizeof(readName));
		CPY_FROM_STREAM_EX(readName, len, pOffset);
		std::string sName = std::string(readName);			
		sName = m_pScene->renameCameraIfExist(sName);  // 检查重命名
		CCamera* pCamera = m_pScene->CreateCamera(sName);

		pCamera->index = m_preCameraCnt + i;

		//当前摄像机的基本信息
		CPY_FROM_STREAM(curN3DCamera.isOrtho, pOffset);
		CPY_FROM_STREAM(curN3DCamera.BaseInfo.fov, pOffset);
		CPY_FROM_STREAM(curN3DCamera.BaseInfo.nearClipDis, pOffset);
		CPY_FROM_STREAM(curN3DCamera.BaseInfo.farClipDis, pOffset);

		pCamera->SetFov(curN3DCamera.BaseInfo.fov);
		pCamera->setNearClip(curN3DCamera.BaseInfo.nearClipDis);
		pCamera->setFarClip(curN3DCamera.BaseInfo.farClipDis);

		CCameraN3DTransform camTrans;
		camTrans.sName = pCamera->getName();
		CPY_FROM_STREAM(camTrans.v3Pos, pOffset);
		CPY_FROM_STREAM(camTrans.v3At, pOffset);
		CPY_FROM_STREAM(camTrans.v3Up, pOffset);
		m_vCamN3DTransform.push_back(camTrans);

		// 保存到资源组
		m_pSceneResource->camera.push_back(pCamera);
	}
}

VOID CN3DLoaderImpV3::ReadLightData()
{
	BYTE* pOffset = m_pSceneData +  TableData.lightDataPtr;

	//读入灯光数量
	DWORD lightCount = 0;
	CPY_FROM_STREAM(lightCount, pOffset);

	m_preLightCnt = m_pScene->getLightCount();

	//读入灯光详细数据
	//std::vector<N3D_Light> vN3DLight(lightCount);
	for (UINT i = 0; i < lightCount; i++)
	{
		int Len;
		CPY_FROM_STREAM(Len, pOffset);
		char readName[MAX_FILENAME_LENGTH];
		ZeroMemory(readName, sizeof(readName));
		CPY_FROM_STREAM_EX(readName, Len, pOffset);
		std::string sName = std::string(readName);

		sName = m_pScene->RenameLightIfExist(sName);
		CLight *pLight = m_pScene->CreateLight(sName);

		VGS_LIGHT_TYPE type;
		CPY_FROM_STREAM(type, pOffset);

		//灯光类型,不同的灯光类型必须设定不同的参数--------------------------	
		switch (type)
		{
		case VLT_OMNI:
			{	
				pLight->param.Type = D3DLIGHT_POINT;

				N3D_OMNI onmiLight;
				CPY_FROM_STREAM(onmiLight, pOffset);

				//强度
				pLight->multiply = onmiLight.multiply;

				// 颜色a r g b
				pLight->diffuse.a = 1.0f; //((LightData.light[i].pOmniLight->color >> 24) &0xff)/255.0f;
				pLight->diffuse.r = ((onmiLight.color>>16) &0xff)/255.0f;
				pLight->diffuse.g = ((onmiLight.color>>8) &0xff)/255.0f;
				pLight->diffuse.b = ((onmiLight.color) &0xff)/255.0f;

				pLight->param.Diffuse.r = pLight->diffuse.r * pLight->multiply;
				pLight->param.Diffuse.g = pLight->diffuse.g * pLight->multiply;
				pLight->param.Diffuse.b = pLight->diffuse.b * pLight->multiply;
				pLight->param.Diffuse.a = 1.0f;

				//环境光，用它来近似模拟全局光
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
				pLight->param.Ambient.a = 1.0f;

				//高光
				// pLight->specular = 0.0 * pLight->diffuse;
				pLight->param.Specular   = pLight->multiply * pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式
				pLight->param.Specular.a = 1.0f;

				//光线到达的最远距离
				pLight->param.Range        = onmiLight.attenuationRange;
				pLight->param.Attenuation0 = onmiLight.attenuation0;
				pLight->param.Attenuation1 = onmiLight.attenuation1;
				pLight->param.Attenuation2 = onmiLight.attenuation2;			
	  
				break;
			}
			case VLT_SPOT:
			{	
				pLight->param.Type = D3DLIGHT_SPOT;

				N3D_SPOTLIGHT spotLight;
				CPY_FROM_STREAM(spotLight, pOffset);

				//强度
				pLight->multiply = spotLight.multiply;

				// 颜色
				pLight->diffuse.a = 1.0f; //((spotLight.color >> 24) & 0xff)/255.0f;
				pLight->diffuse.r = ((spotLight.color >> 16) & 0xff)/255.0f;
				pLight->diffuse.g = ((spotLight.color >> 8) & 0xff)/255.0f;
				pLight->diffuse.b = ((spotLight.color) & 0xff)/255.0f;

				pLight->ambient = 0.0f * pLight->diffuse;

				pLight->param.Diffuse.r = pLight->diffuse.r * pLight->multiply;
				pLight->param.Diffuse.g = pLight->diffuse.g * pLight->multiply;
				pLight->param.Diffuse.b = pLight->diffuse.b * pLight->multiply;
				pLight->param.Diffuse.a = 1.0f;

				//环境光，用它来近似模拟全局光
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
				pLight->param.Ambient.a = 1.0f;

				//高光
				pLight->param.Specular = pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式

				//光线到达的最远距离
				pLight->param.Range        = spotLight.attenuationRange;
				pLight->param.Attenuation0 = spotLight.attenuation0;
				pLight->param.Attenuation1 = spotLight.attenuation1;
				pLight->param.Attenuation2 = spotLight.attenuation2;

				// 在世界坐标系中的方向
				pLight->param.Direction.x = spotLight.direction.x;
				pLight->param.Direction.y = spotLight.direction.y;
				pLight->param.Direction.z = spotLight.direction.z;

				//光线区域的范围角
				pLight->param.Theta   = spotLight.innerAngle;
				pLight->param.Phi     = spotLight.outerAngle;
				pLight->param.Falloff = spotLight.fallOff;

				break;
			}
			case VLT_DIR:
			{
				pLight->param.Type = D3DLIGHT_DIRECTIONAL; 
				N3D_DIRECTIONAL directionLight;
				CPY_FROM_STREAM(directionLight, pOffset);

				//强度
				pLight->multiply = directionLight.multiply;

				// 颜色
				pLight->diffuse.a = 1.0f; //((directionLight.color >> 24) &0xff)/255.0f;
				pLight->diffuse.r = ((directionLight.color >> 16) &0xff)/255.0f;
				pLight->diffuse.g = ((directionLight.color >> 8) &0xff)/255.0f;
				pLight->diffuse.b = ((directionLight.color) &0xff)/255.0f;

				pLight->ambient   = 0.0f * pLight->diffuse;
				pLight->param.Diffuse.r = pLight->diffuse.r * pLight->multiply;
				pLight->param.Diffuse.g = pLight->diffuse.g * pLight->multiply;
				pLight->param.Diffuse.b = pLight->diffuse.b * pLight->multiply;
				pLight->param.Diffuse.a = 1.0f;
				//环境光，用它来近似模拟全局光
				pLight->param.Ambient = 0.0f * pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
				pLight->param.Ambient.a = 1.0f;
				//高光
				pLight->param.Specular = pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式
				//光线到达的最远距离
				pLight->param.Range        = directionLight.attenuationRange;
				pLight->param.Attenuation0 = directionLight.attenuation0;
				pLight->param.Attenuation1 = directionLight.attenuation1;
				pLight->param.Attenuation2 = directionLight.attenuation2;

				//方向
				pLight->param.Direction.x = directionLight.direction.x;
				pLight->param.Direction.y = directionLight.direction.y;
				pLight->param.Direction.z = directionLight.direction.z;

				break;
			}
			default:
				break;
		}

		m_pScene->m_pd3dDevice->SetLight(pLight->index, &pLight->param);
		m_pScene->m_pd3dDevice->LightEnable(pLight->index, TRUE);

		// 保存到资源组
		m_pSceneResource->light.push_back(pLight);
	}
}

VOID CN3DLoaderImpV3::ReadModelData()
{
	BYTE* pOffset = m_pSceneData + TableData.meshDataPtr;

	//之前的模型数量
	m_preModelCnt = m_pScene->model.size();	

	// 得到场景中模型的数量
	UINT modelCount = 0;
	CPY_FROM_STREAM(modelCount, pOffset);

	std::vector<CVGSModelData> vModelData(modelCount);

	//模型的材质是否有透明信息
	BOOL bTransModel = FALSE;

	//将Mesh信息转换为模型信息
	for (UINT i = 0; i < modelCount; i++)
	{
		// CVGSModelData &curModelData = vModelData[i];

		//模型名称
		UINT nameLen = 0;
		CPY_FROM_STREAM(nameLen, pOffset);

		char name[MAX_FILENAME_LENGTH];
		ZeroMemory(name, sizeof(name));
		CPY_FROM_STREAM_EX(name, nameLen, pOffset);

		CModel *pModel = m_pScene->CreateModel(std::string(name));
		if (!pModel)
		{
			continue;
		}

		// 读fvf
		DWORD fvf;
		CPY_FROM_STREAM(fvf, pOffset);

		// 模型的顶点数量
		UINT vertexCount = 0;
		CPY_FROM_STREAM(vertexCount, pOffset);
		char *pVertexBuffer = 0;
		UINT vertexBufferLen = 0;
		switch (fvf)
		{
		case FVF_POSNORMALCOLORCOORD1:
			{
				_VERTEXDECL_POSNORMALCOLORCOORD1 *pVertexData = new _VERTEXDECL_POSNORMALCOLORCOORD1[vertexCount];
				vertexBufferLen = vertexCount * sizeof(_VERTEXDECL_POSNORMALCOLORCOORD1);
				CPY_FROM_STREAM_EX(pVertexData, vertexBufferLen, pOffset);
				// memcpy(pVertexData, pOffset, vertexBufferLen);
				pVertexBuffer = (char*)pVertexData;
				break;
			}
		case FVF_POSNORMALCOLORCOORD2:
			{
				_VERTEXDECL_POSNORMALCOLORCOORD2 *pVertexData = new _VERTEXDECL_POSNORMALCOLORCOORD2[vertexCount];
				vertexBufferLen = vertexCount * sizeof(_VERTEXDECL_POSNORMALCOLORCOORD2);
				CPY_FROM_STREAM_EX(pVertexData, vertexBufferLen, pOffset);
				// memcpy(pVertexData, pOffset, vertexBufferLen);
				pVertexBuffer = (char*)pVertexData;
				break;
			}
		case FVF_POSNORMALCOLORCOORD3:
			{
				_VERTEXDECL_POSNORMALCOLORCOORD3 *pVertexData = new _VERTEXDECL_POSNORMALCOLORCOORD3[vertexCount];
				vertexBufferLen = vertexCount * sizeof(_VERTEXDECL_POSNORMALCOLORCOORD3);
				CPY_FROM_STREAM_EX(pVertexData, vertexBufferLen, pOffset);
				// memcpy(pVertexData, pOffset, vertexBufferLen);
				pVertexBuffer = (char*)pVertexData;
				break;
			}
		case FVF_POSNORMALCOLORCOORD4:
			{
				_VERTEXDECL_POSNORMALCOLORCOORD4 *pVertexData = new _VERTEXDECL_POSNORMALCOLORCOORD4[vertexCount];
				vertexBufferLen = vertexCount * sizeof(_VERTEXDECL_POSNORMALCOLORCOORD4);
				CPY_FROM_STREAM_EX(pVertexData, vertexBufferLen, pOffset);
				// memcpy(pVertexData, pOffset, vertexBufferLen);
				pVertexBuffer = (char*)pVertexData;
				break;
			}
		default:
			break;
		} // switch

		UINT faceCount = 0;
		CPY_FROM_STREAM(faceCount, pOffset);  // 总面数

		// 计算index buffer的数据长度
		/*int indexSize = faceCount * 3 * sizeof(unsigned short);
		if (vertexCount > 65535)
		{
			indexSize = faceCount * 3 * sizeof(UINT);
		}*/
		UINT indexSize = faceCount * 3 * sizeof(UINT);

		// 读入index buffer
		DWORD *pIndexBuffer = new DWORD[faceCount * 3];
		CPY_FROM_STREAM_EX((void*)pIndexBuffer, indexSize, pOffset);
		// memcpy(pIndexBuffer, pOffset, indexSize);

		//SubMesh数量
		unsigned short subMeshCnt = 0;
		CPY_FROM_STREAM(subMeshCnt, pOffset);

		//SubMesh列表
		bTransModel = false;
		std::vector<CSubModel> vSubModel(subMeshCnt);
		for (UINT j = 0; j < subMeshCnt; j++)
		{
			CSubModel &subModel = vSubModel[j];

			int matIndex = -1;
			CPY_FROM_STREAM(matIndex, pOffset);
			if (matIndex >= 0)
			{
				subModel.m_pMaterial = _GetHandleByIndex(m_vMaterial, matIndex);
				if (subModel.m_pMaterial && (/*subMesh.shader->bAlphaBlend||*/subModel.m_pMaterial->getOpacity() < 100) )  // fuxb 2009-03-06, alphaBlend不参与排序
				{
					bTransModel = TRUE;
				}
				m_pScene->AddMaterial(VGS_MATERIAL_TYPE_3D, subModel.m_pMaterial);
			}
			// 读取submesh的其他信息
			CPY_FROM_STREAM(subModel.m_vertexStart, pOffset);
			CPY_FROM_STREAM(subModel.m_vertexCount, pOffset);
			CPY_FROM_STREAM(subModel.m_faceStart, pOffset);
			CPY_FROM_STREAM(subModel.m_faceCount, pOffset);
			
		} // for submesh

		// 读入模型的其他属性
		bool bBillBoard;
		BYTE axis;
		CPY_FROM_STREAM(bBillBoard, pOffset);
		if (bBillBoard)
		{
			CPY_FROM_STREAM(axis, pOffset);
		}
		bool bCollision;
		CPY_FROM_STREAM(bCollision, pOffset);
		bool bGround;
		CPY_FROM_STREAM(bGround, pOffset);
		bool bVisible;
		CPY_FROM_STREAM(bVisible, pOffset);
		bool bClickable;
		CPY_FROM_STREAM(bClickable, pOffset);
		BYTE upAxis;
		CPY_FROM_STREAM(upAxis, pOffset);
		bool bIsAutoNormalise;
		CPY_FROM_STREAM(bIsAutoNormalise, pOffset);
		bool bSkybox;
		CPY_FROM_STREAM(bSkybox, pOffset);

		// 绑定的骨骼索引
		int bindingSkeletonIndex = -1;
		CPY_FROM_STREAM(bindingSkeletonIndex, pOffset);

		// 创建模型
		pModel->Create(fvf, vertexCount, faceCount, (void*)pVertexBuffer, (void*)pIndexBuffer, vSubModel);
		SAFE_DELETE_ARRAY(pVertexBuffer);
		SAFE_DELETE_ARRAY(pIndexBuffer);

		//isBillboard
		pModel->isBillboard = bBillBoard;
		if (pModel->isBillboard)
		{ 
			VECTOR3 vDir = _GetAxisDirection((VGS_AXIS_TYPE)axis);
			pModel->billboardDirectionAxis = D3DXVECTOR3(vDir.x, vDir.y, vDir.z);  // 朝向相机的方向轴
		}

		// Collision
		pModel->isCollision = bCollision;

		//isGround
		pModel->isGround = bGround;
		if (pModel->isGround)
		{
			pModel->isCollision = TRUE;
		}

		//isVisible
		pModel->visible = bVisible;

		//isClickable
		pModel->clickable = bClickable;

		// upAxis  billboradUpAxis
		VECTOR3 vDir = _GetAxisDirection((VGS_AXIS_TYPE)upAxis);
		pModel->billboradUpAxis = D3DXVECTOR3(vDir.x, vDir.y, vDir.z);  // 模型正对相机时的Up方向轴

		// isAutoNormalise
		pModel->isAutoNormalise = bIsAutoNormalise;

		//isSkyBox
		pModel->isSkyBox = bSkybox;   

		//push model
		m_pSceneResource->model.push_back(pModel);

		//push the transparent model
		if (bTransModel)
		{
			m_pScene->AddModelToTransparentList(pModel);
			/*if (find(m_pScene->m_vTransModel.begin(), m_pScene->m_vTransModel.end(), pModel) == m_pScene->m_vTransModel.end())
			{
				m_pScene->m_vTransModel.push_back(pModel);
			}*/
		}

		//如果模型需要辅助渲染表面，则绑定模型到辅助渲染表面——凹凸合成贴图
		//BandModelToAddtionalSurface_Bump(pModel);
	}

	//重新构造材质的SubMeshList列表
	//m_pSceneResource->ConstructShaderSubMeshList();

}

// 读取overlay信息
VOID CN3DLoaderImpV3::ReadOverlayData()
{
	BYTE* pOffset = m_pSceneData +  TableData.OverlayPtr;
	m_preOverlayCnt = m_pScene->m_vOverlay.size();

	// 读入Overlay对象数量
	UINT overlayCount = 0;
	CPY_FROM_STREAM(overlayCount, pOffset);
	if (overlayCount <= 0)
	{
		return;
	}

	// 读入所有overlay信息
	for (UINT iOverlay = 0; iOverlay < overlayCount; ++iOverlay)
	{
		ReadandCreateAOverlay(VGS_GUIOBJ_TYPE_OVERLAY, &pOffset);

	} // for each overlay

	// 读入Overlay对象数量
	UINT backGroundCount = 0;
	CPY_FROM_STREAM(backGroundCount, pOffset);
	for (UINT iBack = 0; iBack < backGroundCount; ++iBack)
	{
		ReadandCreateAOverlay(VGS_GUIOBJ_TYPE_BACKDROP, &pOffset);
	}
}

// 读取一个overlay信息
COverlay * CN3DLoaderImpV3::ReadandCreateAOverlay(VGS_GUIOBJ_TYPE guiType, BYTE **pBuffer)
{
	BYTE *pOffset = *pBuffer;

	N3D_OverlayBasalInfoV3 Info;
	ZeroMemory(&Info, sizeof(Info));

	// 读取overlay基本信息
	CPY_FROM_STREAM(Info, pOffset);

	COverlay *pOverlay = NULL;
	switch (guiType)
	{
	case VGS_GUIOBJ_TYPE_OVERLAY:
		pOverlay = (COverlay *)m_pScene->GetGuiManagerRef().CreateOverLayer(Info.Name);
		break;
	case VGS_GUIOBJ_TYPE_BUTTON:
		pOverlay = (COverlay *)m_pScene->GetGuiManagerRef().CreateButton(Info.Name, Info.Rect);
		break;
	case VGS_GUIOBJ_TYPE_BACKDROP:
		pOverlay = (COverlay *)m_pScene->GetGuiManagerRef().CreateBackGroudLayer();
		break;
	case VGS_GUIOBJ_TYPE_NAVIGATION:
		pOverlay = (COverlay *)m_pScene->GetGuiManagerRef().CreateNavigator(Info.Name);
		break;
	case VGS_GUIOBJ_TYPE_NAVIPOINTOR:
		pOverlay = (COverlay *)m_pScene->GetGuiManagerRef().CreateNaviPointer(Info.Name);
		break;
	}
	if (NULL == pOverlay)
		return NULL;

	//写入Overlay的偏移坐标原点标示
	pOverlay->SetOffsetOrigin((VGSOFFSETORIGIN)Info.OffsetOrigin);

	//读入Overlay的位置标示(相对坐标还是绝对坐标)
	pOverlay->SetUsedRelativePos((bool)Info.UseRelativePos);

	//读入Overlay是否使用设定的绝对尺寸
	pOverlay->SetUsedRelativeSize((bool)Info.UseRelativeSize);

	//读入Overlay尺寸		
	pOverlay->SetRect(Info.Rect);   // 计算相对尺寸

	//读入Overlay转角
	FLOAT angle = Info.Angle;
	pOverlay->SetRotation(angle);

	//是否可见
	pOverlay->SetVisibility((bool)Info.IsVisible);

	//读入Overlay的透明度
	pOverlay->SetOpacity(Info.Opacity);	

	//读入Overlay的材质ID
	if (Info.MaterialIndex != -1)
	{
		UINT index = Info.MaterialIndex;
		CMaterial* pMtrl = _GetHandleByIndex(m_vMaterial, index);
		pOverlay->SetMaterial(pMtrl);
		m_pScene->AddMaterial(VGS_MATERIAL_TYPE_2D, pMtrl);

		//设置Overlay用到的贴图的MipMap为1
		int texLayerCnt = pOverlay->GetMaterial()->getTexureLayerCount();
		//材质有贴图，则遍历贴图层列表逐层按类型贴图并混色
		if (texLayerCnt > 0)
		{
			for(int i = 0; i< texLayerCnt; i++)
			{
				CTextureLayer* pTexLayer = pOverlay->GetMaterial()->getTextureLayerByIndex(i);
				CTexture* pTex = (CTexture*)(pTexLayer->pTexture);
				pTex->MipCount = 1;
			}
		}

		pOverlay->GetMaterial()->selfIllumination = 100;
	}

	// 事件函数
	for (int iFunc = 0; iFunc < VGS_MOUSEEVENT_CNT; iFunc++)
	{
		pOverlay->SetEventFuncName((VGS_MOUSEEVENT_TYPE)iFunc, std::string(Info.sMouseEventFunc[iFunc]));
	}

	// 关键帧
	UINT keyframeCount = 0;
	CPY_FROM_STREAM(keyframeCount, pOffset);
	// Read key frame animation

	// 保存到场景列表
	m_pScene->m_vOverlay.push_back(pOverlay);
	m_pSceneResource->overlay.push_back(pOverlay);

	*pBuffer = pOffset;

	return pOverlay;
}

// 读取导航图
VOID CN3DLoaderImpV3::ReadNavigaterData()
{
	BYTE* pOffset = m_pSceneData +  TableData.NavigatotPtr;
	m_preNavigaterCnt = m_pScene->m_vNavigater.size();

	// 读入navigator对象数量
	UINT naviCount = 0;
	CPY_FROM_STREAM(naviCount, pOffset);
	if (naviCount <= 0)
	{
		return;
	}

	// 读入所有navigator信息
	for (UINT iNaviCnt = 0; iNaviCnt < naviCount; ++iNaviCnt)
	{
		CNavigator *pNavigator = (CNavigator*)ReadandCreateAOverlay(VGS_GUIOBJ_TYPE_NAVIGATION, &pOffset);

		// 导航范围
		FRect rect;
		CPY_FROM_STREAM(rect.Left, pOffset);
		CPY_FROM_STREAM(rect.Right, pOffset);
		CPY_FROM_STREAM(rect.Top, pOffset);
		CPY_FROM_STREAM(rect.Bottom, pOffset);
		pNavigator->SetNavRange(rect);  

		// 每个导航点
		UINT pointerCnt;
		CPY_FROM_STREAM(pointerCnt, pOffset);
		for (UINT iPoint = 0; iPoint < pointerCnt; ++iPoint)
		{
			CNavPointer *pPointer = (CNavPointer*)ReadandCreateAOverlay(VGS_GUIOBJ_TYPE_NAVIPOINTOR, &pOffset);  // 创建导航点

			pNavigator->AddNavPointer(pPointer);  // 添加到导航图
		}
	} // for each overlay 
}

// 读取按钮
VOID CN3DLoaderImpV3::ReadButtonData()
{
	BYTE* pOffset = m_pSceneData +  TableData.ButtonPtr;
	m_preButtonCnt = m_pScene->m_vNavigater.size();

	// 读入button对象数量
	UINT btnCount = 0;
	CPY_FROM_STREAM(btnCount, pOffset);
	if (btnCount <= 0)
	{
		return;
	}

	// 读入所有button信息
	for (UINT iIndex = 0; iIndex < btnCount; ++iIndex)
	{
		ReadandCreateAOverlay(VGS_GUIOBJ_TYPE_BUTTON, &pOffset);
	}
}

VOID CN3DLoaderImpV3::ReadControlData()
{
	BYTE* pOffset = m_pSceneData +  TableData.ControlPtr;

	// 读入控制对象数量
	UINT controlCount = 0;
	CPY_FROM_STREAM(controlCount, pOffset);
	if (controlCount <= 0)
	{
		return;
	}

	INT preWalkerCnt = m_pScene->GetWalkerCount();
	INT preEditorCnt = m_pScene->GetEditorCount();
	INT preFlyerCnt  = m_pScene->GetFlyerCount();

	// Walker的数目及列表
	UINT WalkerCount = 0;
	CPY_FROM_STREAM(WalkerCount, pOffset);
	for (UINT iWalker = 0; iWalker < WalkerCount; ++iWalker)
	{
		UINT len;
		CPY_FROM_STREAM(len, pOffset);
		char name[MAX_FILENAME_LENGTH];
		ZeroMemory(name, sizeof(name));
		CPY_FROM_STREAM_EX(name, len, pOffset);

		N3D_Walker walker;
		ZeroMemory(&walker, sizeof(walker));
		CPY_FROM_STREAM(walker.CameraIndex, pOffset);
		CPY_FROM_STREAM(walker.bCollision, pOffset);
		CPY_FROM_STREAM(walker.bWeight, pOffset);
		CPY_FROM_STREAM(walker.colDis, pOffset);
		CPY_FROM_STREAM(walker.height, pOffset);
		CPY_FROM_STREAM(walker.liftAcc, pOffset);
		CPY_FROM_STREAM(walker.liftSpeed, pOffset);
		CPY_FROM_STREAM(walker.rosAcc, pOffset);
		CPY_FROM_STREAM(walker.rosSpeed, pOffset);
		CPY_FROM_STREAM(walker.upperAngle, pOffset);
		CPY_FROM_STREAM(walker.walkAcc, pOffset);
		CPY_FROM_STREAM(walker.walkSpeed, pOffset);

		WalkerInfo walkerInfo;
		ZeroMemory(&walkerInfo, sizeof(walkerInfo));
		strncpy_s(walkerInfo.Name, sizeof(walkerInfo.Name), name, sizeof(name));

		std::string sCamName = m_pScene->GetCameraNameByIndex(walker.CameraIndex + m_preCameraCnt);

		walkerInfo.bCollision = walker.bCollision;
		walkerInfo.bWeight    = walker.bWeight;
		walkerInfo.colDis     = walker.colDis;
		strncpy_s(walkerInfo.Camera, sizeof(walkerInfo.Camera), sCamName.c_str(), sCamName.length());
		walkerInfo.height     = walker.height;
		walkerInfo.liftAcc    = walker.liftAcc;
		walkerInfo.liftSpeed  = walker.liftSpeed;
		walkerInfo.rosAcc     = walker.rosAcc;
		walkerInfo.rosSpeed   = walker.rosSpeed;
		walkerInfo.upperAngle = walker.upperAngle;
		walkerInfo.walkAcc    = walker.walkAcc;
		walkerInfo.walkSpeed  = walker.walkSpeed;

		// pWalker->SetWalkerInfo(walkerInfo);  // 设置walker属性
		CWalker* pWalker = new CWalker(walkerInfo);
		pWalker->SetCamera(m_pScene->GetCameraByName(sCamName));

		m_pScene->AddWalker(pWalker); // m_vWalker.push_back(pWalker);
		m_pSceneResource->walker.push_back(pWalker);
	}

	// Editor的数目及列表
	UINT EditorCount = 0;
	CPY_FROM_STREAM(EditorCount, pOffset);
	for (UINT iEditor = 0; iEditor < EditorCount; ++iEditor)
	{
		UINT len;
		CPY_FROM_STREAM(len, pOffset);
		char name[MAX_FILENAME_LENGTH];
		ZeroMemory(name, sizeof(name));
		CPY_FROM_STREAM_EX(name, len, pOffset);

		// 读入保存的Editor信息
		N3D_Editor editor;
		ZeroMemory(&editor, sizeof(editor));
		CPY_FROM_STREAM(editor.CameraIndex, pOffset);
		CPY_FROM_STREAM(editor.autoSpeedX, pOffset);
		CPY_FROM_STREAM(editor.autoSpeedY, pOffset);
		CPY_FROM_STREAM(editor.bIsAutoRoundRoll, pOffset);
		CPY_FROM_STREAM(editor.dollyAcc, pOffset);
		CPY_FROM_STREAM(editor.dollyFarDis, pOffset);
		CPY_FROM_STREAM(editor.dollyNearDis, pOffset);
		CPY_FROM_STREAM(editor.dollySpeed, pOffset);
		CPY_FROM_STREAM(editor.LookAtPos.x, pOffset);
		CPY_FROM_STREAM(editor.LookAtPos.y, pOffset);
		CPY_FROM_STREAM(editor.LookAtPos.z, pOffset);
		CPY_FROM_STREAM(editor.manualPanXSpeed, pOffset);
		CPY_FROM_STREAM(editor.manualPanYSpeed, pOffset);
		CPY_FROM_STREAM(editor.manualSpeedX, pOffset);
		CPY_FROM_STREAM(editor.manualSpeedY, pOffset);
		CPY_FROM_STREAM(editor.rosAxis, pOffset);

		// 检查cameraIndex的合理性
		INT camIndx = editor.CameraIndex + m_preCameraCnt;
		std::string sCamName = m_pScene->GetCameraNameByIndex(camIndx);

		EditorInfo editorInfo;
		ZeroMemory(&editorInfo, sizeof(editorInfo));
		editorInfo.autoSpeedY       = editor.autoSpeedX;
		editorInfo.autoSpeedY       = editor.autoSpeedY;
		editorInfo.bIsAutoRoundRoll = editor.bIsAutoRoundRoll;
		strncpy_s(editorInfo.Camera, sizeof(editorInfo.Camera), sCamName.c_str(), sCamName.length());
		editorInfo.dollyAcc = editor.dollyAcc;
		editorInfo.dollyFarDis      = editor.dollyFarDis;
		editorInfo.dollyNearDis     = editor.dollyNearDis;
		editorInfo.dollySpeed       = editor.dollySpeed;
		editorInfo.LookAtModelIndex = -1;
		editorInfo.LookAtPos        = VECTOR3(editor.LookAtPos.x, editor.LookAtPos.y, editor.LookAtPos.z);
		editorInfo.LookAtType       = 1;
		editorInfo.manualPanXSpeed  = editor.manualPanXSpeed;
		editorInfo.manualPanYSpeed  = editor.manualPanYSpeed;
		editorInfo.manualSpeedX     = editor.manualSpeedX;
		editorInfo.manualSpeedY     = editor.manualSpeedY;
		editorInfo.rosAxis          = editor.rosAxis;

		// pEditor->SetEditorInfo(editorInfo);
		CEditor* pEditor = new CEditor(m_pScene, editorInfo);
		pEditor->SetCamera(m_pScene->GetCameraByName(sCamName));

		m_pScene->AddEditor(pEditor);//m_vEditor.push_back(pEditor);
		m_pSceneResource->editor.push_back(pEditor);
	}

	// Flyer的数目及列表
	UINT FlyerCount = 0;
	CPY_FROM_STREAM(FlyerCount, pOffset);
	for (UINT iFlyer = 0; iFlyer < FlyerCount; ++iFlyer)
	{
		UINT len;
		CPY_FROM_STREAM(len, pOffset);
		char name[MAX_FILENAME_LENGTH];
		ZeroMemory(name, sizeof(name));
		CPY_FROM_STREAM_EX(name, len, pOffset);

		// 读入保存的Editor信息
		N3D_Flyer flyer;
		ZeroMemory(&flyer, sizeof(flyer));
		CPY_FROM_STREAM(flyer.CameraIndex, pOffset);
		CPY_FROM_STREAM(flyer.bCollision, pOffset);
		CPY_FROM_STREAM(flyer.colDis, pOffset);
		CPY_FROM_STREAM(flyer.dollyAcc, pOffset);
		CPY_FROM_STREAM(flyer.dollyFarDis, pOffset);
		CPY_FROM_STREAM(flyer.dollyNearDis, pOffset);
		CPY_FROM_STREAM(flyer.dollySpeed, pOffset);
		CPY_FROM_STREAM(flyer.liftAcc, pOffset);
		CPY_FROM_STREAM(flyer.liftSpeed, pOffset);
		CPY_FROM_STREAM(flyer.lowerAngle, pOffset);
		CPY_FROM_STREAM(flyer.panAcc, pOffset);
		CPY_FROM_STREAM(flyer.panSpeed, pOffset);
		CPY_FROM_STREAM(flyer.rotateAcc, pOffset);
		CPY_FROM_STREAM(flyer.rotateSpeed, pOffset);
		CPY_FROM_STREAM(flyer.upperAngle, pOffset);

		// 检查cameraIndex的合理性
		INT camIndx = flyer.CameraIndex + m_preCameraCnt;
		std::string sCamName = m_pScene->GetCameraNameByIndex(camIndx);

		FlyerInfo flyerInfo;
		ZeroMemory(&flyerInfo, sizeof(flyerInfo));
		flyerInfo.bCollision   = flyer.bCollision;
		flyerInfo.colDis       = flyer.colDis;
		strncpy_s(flyerInfo.Camera, sizeof(flyerInfo.Camera), sCamName.c_str(), sCamName.length());
		flyerInfo.dollySpeed   = flyer.dollySpeed;
		flyerInfo.dollyAcc     = flyer.dollyAcc;
		flyerInfo.dollyNearDis = flyer.dollyNearDis;
		flyerInfo.dollyFarDis  = flyer.dollyFarDis;
		flyerInfo.dollySpeed   = flyer.dollySpeed;
		flyerInfo.dollyAcc     = flyer.dollyAcc;
		flyerInfo.liftSpeed    = flyer.liftSpeed;
		flyerInfo.liftAcc      = flyer.liftAcc;
		flyerInfo.lowerAngle   = flyer.lowerAngle;
		flyerInfo.panSpeed     = flyer.panSpeed;
		flyerInfo.panAcc       = flyer.panAcc;
		flyerInfo.rotateSpeed  = flyer.rotateSpeed;
		flyerInfo.rotateAcc    = flyer.rotateAcc;
		flyerInfo.upperAngle   = flyer.upperAngle;
		// pFlyer->SetFlyerInfo(flyerInfo);

		CFlyer* pFlyer = new CFlyer(flyerInfo);
		pFlyer->SetCamera(m_pScene->GetCameraByName(sCamName));

		m_pScene->AddFlyer(pFlyer);//m_vFlyer.push_back(pFlyer);
		m_pSceneResource->flyer.push_back(pFlyer);
	}

	// 当前控制对象的类型
	N3D_ControlData ControlData;
	CPY_FROM_STREAM(ControlData.ControlType, pOffset); 
	CPY_FROM_STREAM(ControlData.ControlerIndex, pOffset);

	IController *pCurController = m_pScene->GetCurrentController();
	if (!pCurController)  // 没有当前控制对象
	{
		switch (ControlData.ControlType)
		{
		case 0:
			m_pScene->SetCurrentController(m_pScene->GetWalker(ControlData.ControlerIndex + preWalkerCnt));
			break;
		case 1:
			m_pScene->SetCurrentController(m_pScene->GetEditor(ControlData.ControlerIndex + preEditorCnt));
			break;
		case 2:
			m_pScene->SetCurrentController(m_pScene->GetFlyer(ControlData.ControlerIndex + preFlyerCnt));
			break;
		} // switch
	} // end if
}

// 读取场景图
VOID CN3DLoaderImpV3::ReadSceneGraph()
{
	BYTE* pOffset = m_pSceneData + TableData.sceneGraphDataPtr;
	N3D_Node* pRootNode = new N3D_Node;
	CSceneNode *pNode = m_pScene->GetRootSceneNode();
	ConstructSceneGraph(pNode, pRootNode, &pOffset);
	SAFE_DELETE(pRootNode);
}

//构造场景图
VOID CN3DLoaderImpV3::ConstructSceneGraph(CSceneNode* parentNode, N3D_Node* pNode, BYTE** offset)
{
	BYTE *pOffset = *offset;

	//创建场景中的一个子节点信息----------------------------------------
	//节点的类型
	CPY_FROM_STREAM(pNode->type, pOffset);
	CPY_FROM_STREAM(pNode->index, pOffset);

	UINT len;
	char tmp[MAX_FILENAME_LENGTH];
	ZeroMemory(tmp, MAX_FILENAME_LENGTH);
	CPY_FROM_STREAM(len, pOffset);
	if (len > 0)
	{
		CPY_FROM_STREAM_EX(tmp, len, pOffset);
	}

	//数据库标记
	DWORD ID1,ID2;
	CPY_FROM_STREAM(ID1, pOffset);
	CPY_FROM_STREAM(ID2, pOffset);

	// CTransform transform;
	D3DXVECTOR3 v3Pos;
	CPY_FROM_STREAM(v3Pos.x, pOffset);
	CPY_FROM_STREAM(v3Pos.y, pOffset);
	CPY_FROM_STREAM(v3Pos.z, pOffset);

	D3DXQUATERNION quat(0, 0, 0, 1);
	CPY_FROM_STREAM(quat.x, pOffset);
	CPY_FROM_STREAM(quat.y, pOffset);
	CPY_FROM_STREAM(quat.z, pOffset);
	CPY_FROM_STREAM(quat.w, pOffset);

	D3DXVECTOR3 v3Scale;
	CPY_FROM_STREAM(v3Scale.x, pOffset);
	CPY_FROM_STREAM(v3Scale.y, pOffset);
	CPY_FROM_STREAM(v3Scale.z, pOffset);

	//创建节点对象
	CSceneNode* pSceneNode = NULL;


	//如果有父节点
	if (parentNode)
	{
		//(*parentNode)->m_child.push_back(pSceneNode);
		//pSceneNode->m_parent =(*parentNode);
		pSceneNode = parentNode->CreateChild();
	}

	//如果是根节点
	if (pNode->type == NNT_SCENEROOT)
	{
		m_pSceneResource->m_pRootNode = new CSceneNode();
		pSceneNode = m_pSceneResource->m_pRootNode;
	}

	//计算节点的变换矩阵-------------------------------------------------------
	
	//缩放
	D3DXVECTOR3 scale = D3DXVECTOR3(1, 1, 1);
	if (pNode->type == NNT_MODEL)
	{
		scale = D3DXVECTOR3(v3Scale.x, v3Scale.y, v3Scale.z);
	}

	// 设置节点的变换
	D3DXMATRIX transMat;
	D3DXMatrixTransformation(&transMat, 
							 &D3DXVECTOR3(0,0,0), 
							 &D3DXQUATERNION(0, 0, 0, 0), 
							 &scale, 
							 &D3DXVECTOR3(0,0,0), 
							 &quat, 
							 &v3Pos);
	pSceneNode->setMatrix(transMat, VTS_PARENT);
	//pSceneNode->setScale(&v3Scale, VTS_PARENT);
	//pSceneNode->rotate(&quat, VTS_PARENT);
	//// pSceneNode->setRotationByAngle(&transform.rotation);
	//pSceneNode->setPosition(&v3Pos, VTS_PARENT);	

	CCamera *pCamera = 0;
	CLight  *pLight  = 0;
	if (pNode->type == NNT_CAMERANODE)
	{
		if (pNode->index >=0 && (pNode->index + m_preCameraCnt) < m_pScene->getCameraCount())
		{
			pCamera = m_pScene->GetCameraByIndex(pNode->index + m_preCameraCnt);
		}
	}
	else if (pNode->type == NNT_LIGHTNODE)
	{
		if (pNode->index >= 0 && (pNode->index + m_preLightCnt) < m_pScene->getLightCount())
		{
			pLight = m_pScene->GetLightByIndex(pNode->index + m_preLightCnt);
		}
	}

	// 读入节点动画数据
	std::vector<CNodeTransform> vTransformKeyframe;
	UINT keyFrameCnt = 0;
	CPY_FROM_STREAM(keyFrameCnt, pOffset); // 关键帧数目

	std::vector<CVGS_CAMERA_ANIMINFO> vCamParamInfo;   // 相机参数动画
	std::vector<CVGS_LIGHT_INFO> vLightParamInfo;  // 灯光参数动画
	if (keyFrameCnt > 1)
	{
		vTransformKeyframe.resize(keyFrameCnt);

		if (pNode->type == NNT_CAMERANODE)
		{
			vCamParamInfo.resize(keyFrameCnt);
		}
		else if (pNode->type == NNT_LIGHTNODE)
		{
			vLightParamInfo.resize(keyFrameCnt);
		}

		// 读取每一帧数据
		for (UINT iFrame = 0; iFrame < keyFrameCnt; ++iFrame)
		{
			CNodeTransform &nodeTrans = vTransformKeyframe[iFrame];
			N3D_NodeTransform n3dTrans;
			CPY_FROM_STREAM(n3dTrans, pOffset);
			nodeTrans.time = n3dTrans.time;
			nodeTrans.position = n3dTrans.position;
			nodeTrans.quat     = n3dTrans.quat;
			nodeTrans.scale    = n3dTrans.scale;

			// 如果是相机或灯光节点，可能还有参数动画
			if (pNode->type == NNT_CAMERANODE)
			{
				bool bHasParamAnimation;
				CPY_FROM_STREAM(bHasParamAnimation, pOffset); // 是否包含参数动画
				if (bHasParamAnimation)
				{
					float fov, nearClip, farClip, roll;
					CPY_FROM_STREAM(fov, pOffset);
					CPY_FROM_STREAM(nearClip, pOffset);
					CPY_FROM_STREAM(farClip, pOffset);
					CPY_FROM_STREAM(roll, pOffset);
					vCamParamInfo[iFrame].fov = fov;
					vCamParamInfo[iFrame].nearClip = nearClip;
					vCamParamInfo[iFrame].farClip = farClip;
				}
			}
			else if (pNode->type == NNT_LIGHTNODE)
			{
				bool bHasParamAnimation;
				CPY_FROM_STREAM(bHasParamAnimation, pOffset); // 是否包含参数动画

				DWORD color;
				float range;
				CPY_FROM_STREAM(color, pOffset); 
				CPY_FROM_STREAM(range, pOffset); 
				vLightParamInfo[iFrame].color = color;
				vLightParamInfo[iFrame].range2 = range;
				vLightParamInfo[iFrame].multiply = 1.0f;
				if (pLight && pLight->getType() == VLT_DIR)
				{
					VECTOR3 v3Dir;
					CPY_FROM_STREAM(v3Dir.x, pOffset); 
					CPY_FROM_STREAM(v3Dir.y, pOffset); 
					CPY_FROM_STREAM(v3Dir.z, pOffset); 
					vLightParamInfo[iFrame].dir = v3Dir;
				}
				else if (pLight && pLight->getType() == VLT_DIR)
				{
					VECTOR3 v3Dir;
					float theta, phi, fallof;
					CPY_FROM_STREAM(v3Dir.x, pOffset); 
					CPY_FROM_STREAM(v3Dir.y, pOffset); 
					CPY_FROM_STREAM(v3Dir.z, pOffset); 
					CPY_FROM_STREAM(theta, pOffset); 
					CPY_FROM_STREAM(phi, pOffset); 
					CPY_FROM_STREAM(fallof, pOffset); 
					vLightParamInfo[iFrame].dir = v3Dir;
					vLightParamInfo[iFrame].theta = theta;
					vLightParamInfo[iFrame].phi   = phi;
				}
			}
		}
	}

	switch (pNode->type)
	{
	case NNT_SCENEROOT:
		break;
	case NNT_CAMERANODE:
		{
			if (pCamera)
			{
				pSceneNode->attachObject(pCamera);
				
				CCameraN3DTransform camTrans = getCameraParamByName(pCamera->getName());
				pCamera->SetPosition(&(camTrans.v3Pos), VTS_PARENT);
				pCamera->Lookat(&(camTrans.v3At), VTS_PARENT);
				pCamera->SetUp(camTrans.v3Up, VTS_PARENT);
				// pCamera->SetRoll(pCamera->GetRoll());

				// 创建相机关键帧动画
				m_pRoot->GetAnimManager().CreateCameraAnimation(pSceneNode, vTransformKeyframe, vCamParamInfo);

			} // if pNode->index	
		} 
		break;
	case NNT_LIGHTNODE:
		{
			if (pLight)
			{
				pSceneNode->attachObject(pLight);
				pSceneNode->name = std::string(pLight->getName());

				//更新灯光数据，设置灯光的位置
				pLight->param.Position = pLight->m_pNode->getPosition(VTS_WORLD);
				m_pScene->m_pd3dDevice->SetLight(pNode->index, &(pLight->param));		
				m_pScene->m_pd3dDevice->LightEnable(pNode->index, TRUE);

				// 创建灯光动画
				m_pRoot->GetAnimManager().CreateLightAnimation(pSceneNode, vTransformKeyframe, pLight->getType(), vLightParamInfo);
			}
		}

		break;
	case NNT_MODEL:
		{
			if ((pNode->index + m_preModelCnt) < m_pScene->GetModelCount())
			{
				CModel* pModel = m_pScene->GetModelByIndex(pNode->index + m_preModelCnt);
				pSceneNode->attachObject(pModel);

				// 创建模型关键帧动画
				m_pRoot->GetAnimManager().CreateModelAnimation(pSceneNode, vTransformKeyframe);
			}
		}
		break;
	}

	// 读取子节点
	UINT childCnt = 0;
	CPY_FROM_STREAM(childCnt, pOffset);
	*offset = pOffset;
	for (UINT i =0 ;i < childCnt; i++)
	{
		ConstructSceneGraph(pSceneNode, pNode, offset);	      
	}
}

CCameraN3DTransform CN3DLoaderImpV3::getCameraParamByName(const std::string &sName)
{
	CCameraN3DTransform trans;
	UINT count = m_vCamN3DTransform.size();
	for (UINT i = 0; i < count; ++i)
	{
		if (m_vCamN3DTransform[i].sName == sName)
		{
			return m_vCamN3DTransform[i];
		}
	}
	return trans;
}

//** 新版本N3D文件颜色值是argb格式，旧版本是rgba
// **/
VOID CN3DLoaderImpV3::ReadMaterialData()
{
	/*#ifdef _DEBUG
	FILE *pFile;
	std::string tmpfile = std::string("D:\\StartTeam\\editmodel\\material_atl.txt");
	pFile = fopen(tmpfile.c_str(), "wt");
	#endif*/

	m_vMaterial.clear();
	BYTE* pOffset = m_pSceneData + TableData.mtrlDataPtr;

	//mtrl count
	memcpy(&(MaterialData.mtrlCnt), pOffset, sizeof(MaterialData.mtrlCnt));
	pOffset += sizeof(MaterialData.mtrlCnt);
	//mtrl data
	MaterialData.mtrl = new N3D_MtrlInfo[MaterialData.mtrlCnt];	

	for (int i= 0; i< MaterialData.mtrlCnt; i++)
	{		
		std::string sName;

		//51007版本，材质名称没有字符限制
		if(m_fN3DFileVersion < 5.1007f)
		{
			CHAR* readName =new CHAR[N3D_MAX_NAME_LENGTH];
			ZeroMemory(readName,sizeof(readName));
			memcpy(readName , pOffset, N3D_MAX_NAME_LENGTH);
			readName[N3D_MAX_NAME_LENGTH - 1] = '\0';
			pOffset += N3D_MAX_NAME_LENGTH;
			sName = std::string(readName);
			SAFE_DELETE_ARRAY(readName);	
		}
		else
		{
			INT strLen;
			memcpy(&(strLen), pOffset, sizeof(strLen));	
			pOffset += sizeof(strLen);	
			char* readName =new char[strLen+1];
			ZeroMemory(readName, strLen + 1);
			memcpy(readName , pOffset, strLen);
			readName[strLen] = '\0';
			pOffset += strLen;
			sName = std::string(readName);
			SAFE_DELETE_ARRAY(readName);		
		}

		CMaterial* pMtrl = m_pScene->CreateMaterial(sName);
		if (!pMtrl)
		{
			continue;
		}
		pMtrl->index = i;
		memcpy(&(MaterialData.mtrl[i].BasalInfo), pOffset, sizeof(MaterialData.mtrl[i].BasalInfo));	
		pOffset += sizeof(MaterialData.mtrl[i].BasalInfo); 

		//mtrl->dif 
		pMtrl->d3dmtrl.Diffuse.a = ((MaterialData.mtrl[i].BasalInfo.diffuse>>24)&0xff)/255.0f;
		pMtrl->d3dmtrl.Diffuse.r = ((MaterialData.mtrl[i].BasalInfo.diffuse>>16)&0xff)/255.0f;
		pMtrl->d3dmtrl.Diffuse.g = ((MaterialData.mtrl[i].BasalInfo.diffuse>>8)&0xff)/255.0f;
		pMtrl->d3dmtrl.Diffuse.b = ((MaterialData.mtrl[i].BasalInfo.diffuse)&0xff)/255.0f;

		//mtrl->amb
		pMtrl->d3dmtrl.Ambient.a = ((MaterialData.mtrl[i].BasalInfo.ambient>>24)&0xff)/255.0f;
		pMtrl->d3dmtrl.Ambient.r = ((MaterialData.mtrl[i].BasalInfo.ambient>>16)&0xff)/255.0f;
		pMtrl->d3dmtrl.Ambient.g = ((MaterialData.mtrl[i].BasalInfo.ambient>>8)&0xff)/255.0f;
		pMtrl->d3dmtrl.Ambient.b = ((MaterialData.mtrl[i].BasalInfo.ambient)&0xff)/255.0f;
		
		//mtrl->spe
		pMtrl->specular.a = ((MaterialData.mtrl[i].BasalInfo.specular>>24)&0xff)/(255.0f/**MaterialData.mtrl[i].BasalInfo.specularLevel*/);
		pMtrl->specular.r = ((MaterialData.mtrl[i].BasalInfo.specular>>16)&0xff)/(255.0f/**MaterialData.mtrl[i].BasalInfo.specularLevel*/);
		pMtrl->specular.g = ((MaterialData.mtrl[i].BasalInfo.specular>>8)&0xff)/(255.0f/**MaterialData.mtrl[i].BasalInfo.specularLevel*/);		
		pMtrl->specular.b = ((MaterialData.mtrl[i].BasalInfo.specular)&0xff)/(255.0f/**MaterialData.mtrl[i].BasalInfo.specularLevel*/);		

		pMtrl->specularLevel      = MaterialData.mtrl[i].BasalInfo.specularLevel; 
		pMtrl->d3dmtrl.Specular   = pMtrl->specularLevel / 100.0f * pMtrl->specular;
		pMtrl->d3dmtrl.Specular.a = 1.0f;	

		//power
		pMtrl->d3dmtrl.Power = MaterialData.mtrl[i].BasalInfo.shininess;//高光锐度

		//mtrl->emi
		pMtrl->selfIllumination = (UINT)MaterialData.mtrl[i].BasalInfo.selfIllumination;

		//pMtrl->selfIllumination = 100.0f;
		pMtrl->d3dmtrl.Emissive.a = 1.0f;
		pMtrl->d3dmtrl.Emissive.r = pMtrl->selfIllumination / 100.0f * pMtrl->d3dmtrl.Diffuse.r;
		pMtrl->d3dmtrl.Emissive.g = pMtrl->selfIllumination / 100.0f * pMtrl->d3dmtrl.Diffuse.g;
		pMtrl->d3dmtrl.Emissive.b = pMtrl->selfIllumination / 100.0f * pMtrl->d3dmtrl.Diffuse.b;
		// pMtrl->type = MaterialData.mtrl[i].BasalInfo.shaderType;
		pMtrl->type = D3DSHADE_GOURAUD;

		pMtrl->setOpacity((INT)MaterialData.mtrl[i].BasalInfo.opacity); //透明度

		if (pMtrl->getOpacity() < 100)
		{
			pMtrl->bAlphaBlend = TRUE;
		}

		pMtrl->twoSide  = MaterialData.mtrl[i].BasalInfo.bTwoSide;

		pMtrl->bAlphaTest = MaterialData.mtrl[i].BasalInfo.alphaTest;		
		pMtrl->AlpahaTestValue =MaterialData.mtrl[i].BasalInfo.alphaTestValue;
		pMtrl->fillMode = (_D3DFILLMODE)MaterialData.mtrl[i].BasalInfo.fillMode;

		//51001版本，记录是否材质使用模型diffuse值的参数
		if(m_fN3DFileVersion >= 5.1001f)
		{
			memcpy(&(MaterialData.mtrl[i].bUseDiffuse), pOffset, sizeof(MaterialData.mtrl[i].bUseDiffuse));
			pOffset +=sizeof(MaterialData.mtrl[i].bUseDiffuse);
		}
		else
		{
			MaterialData.mtrl[i].bUseDiffuse =true;
		}
		pMtrl->m_bUseDiffuse = (BOOL) MaterialData.mtrl[i].bUseDiffuse;

		//目前材质动画为空 kfrCnt为0
		memcpy(&(MaterialData.mtrl[i].kfrCnt), pOffset, sizeof(MaterialData.mtrl[i].kfrCnt));
		pOffset +=sizeof(MaterialData.mtrl[i].kfrCnt);

		// memcpy(&(MaterialData.mtrl->kfrMtrlInfo), pOffset, sizeof(MaterialData.mtrl->kfrCnt));
		// pOffset +=sizeof(MaterialData.mtrl->kfrCnt);

		//读取	 
		memcpy(&(MaterialData.mtrl[i].TextureLayerCount), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerCount));
		pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerCount);
		MaterialData.mtrl[i].TextureLayerList =new N3D_TextureLayer[MaterialData.mtrl[i].TextureLayerCount];
		for(UINT m = 0; m < MaterialData.mtrl[i].TextureLayerCount; m++)
		{
			CTextureLayer* pTexLayer = new CTextureLayer();
			//51007版本，材质贴图层名称没有字符限制
			if(m_fN3DFileVersion < 5.1007f)
			{
				CHAR* readName =new CHAR[N3D_MAX_NAME_LENGTH];
				ZeroMemory(readName,sizeof(readName));
				memcpy(readName , pOffset, N3D_MAX_NAME_LENGTH);
				readName[N3D_MAX_NAME_LENGTH - 1] = '\0';
				pOffset += N3D_MAX_NAME_LENGTH;
				pTexLayer->m_sName = std::string(readName);
				SAFE_DELETE_ARRAY(readName);			
			}else
			{
				INT strLen;
				memcpy(&(strLen), pOffset, sizeof(strLen));	
				pOffset += sizeof(strLen);	
				char* readName =new char[strLen+1];
				ZeroMemory(readName, strLen + 1);
				memcpy(readName , pOffset, strLen);
				readName[strLen] = '\0';
				pOffset += strLen;
				pTexLayer->m_sName = std::string(readName);
				SAFE_DELETE_ARRAY(readName); 		
			}

			/*memcpy(MaterialData.mtrl[i].TextureLayerList[m].Name, pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].Name));
			pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].Name);
			MaterialData.mtrl[i].TextureLayerList[m].Name[31] = '\0';*/

			memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].type), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].type));
			pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].type);

			memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].UVIndex), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].UVIndex));
			pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].UVIndex);

			//MipMap //51004版本，添加了贴图层的MipMap信息
			if (m_fN3DFileVersion >= 5.1004f)
			{
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].Mipmap), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].Mipmap));
				pOffset += sizeof(MaterialData.mtrl[i].TextureLayerList[m].Mipmap);
			}

			switch(MaterialData.mtrl[i].TextureLayerList[m].type)
			{
			case TLT_BASAL:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam);
				break;

			case TLT_BUMPMAP:
				{
					INT type;
					memcpy(&(type), pOffset, sizeof(type));
					pOffset += sizeof(type);
					VGS_BumpType bumpType = (VGS_BumpType)type;
					//创建凹凸贴图
					CBumpMap* pBumpMap = new CBumpMap(m_pScene->m_pd3dDevice, &m_pScene->m_d3dpp, m_pScene->d3dParam.SupportBestBumpFormat);
					m_pScene->m_vBumpTexture.push_back(pBumpMap);

					switch(bumpType)
					{
					case VBT_BUMPWATER:
						{//Bump波纹贴图
							N3D_BumpwaterTextureParam bumpwaterTexParam;
							memcpy(&(bumpwaterTexParam), pOffset, sizeof(bumpwaterTexParam));
							pOffset += sizeof(bumpwaterTexParam);
							if (pBumpMap->m_MapWidth == 0)
							{
								bumpwaterTexParam.width = 256;
								bumpwaterTexParam.height = 256;
							}
							pBumpMap->SetBumpAsWavemap(bumpwaterTexParam.width, bumpwaterTexParam.height, bumpwaterTexParam.XScale, bumpwaterTexParam.YScale,
								bumpwaterTexParam.Radius, bumpwaterTexParam.Speed, bumpwaterTexParam.CenterX, bumpwaterTexParam.CenterY);

						}
						break;
					case VBT_BUMPNORMARLMAP:     //Bump法线贴图
						{
							N3D_BasalTextureParam basalTexParam;
							memcpy(&(basalTexParam), pOffset, sizeof(basalTexParam));
							pOffset += sizeof(basalTexParam);
							pBumpMap->m_MapWidth = basalTexParam.width;
							pBumpMap->m_MapHeight = basalTexParam.height;
							if (pBumpMap->m_MapWidth == 0)
							{
								basalTexParam.width  = 256;
								basalTexParam.height = 256;
							}

							N3D_BumpNormalTextureParam  bumpnormalTexParam;
							memcpy(&(bumpnormalTexParam), pOffset, sizeof(bumpnormalTexParam));
							pOffset += sizeof(bumpnormalTexParam);
							CLight* pLight = m_pScene->GetLightByIndex(bumpnormalTexParam.LightIndex + m_preLightCnt);
							pBumpMap->SetBumpMapLight(pLight);
						}
						break;
					case VBT_BUMPENVIRONMENT:       //Bump环境贴图
						{
							N3D_BumpEnvTextureParam bumpenvTexParam;
							memcpy(&(bumpenvTexParam), pOffset, sizeof(bumpenvTexParam));
							pOffset += sizeof(bumpenvTexParam);
							pBumpMap->m_MapWidth = bumpenvTexParam.width;
							pBumpMap->m_MapHeight = bumpenvTexParam.height;
							pBumpMap->m_Noise1 = bumpenvTexParam.noise1;
							pBumpMap->m_Noise2 = bumpenvTexParam.noise2;
						}
						break;
					}					
				}
				break;

			case TLT_OPACITY:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.OpacityTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.OpacityTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.OpacityTextureParam);				
				pMtrl->bAlphaBlend = TRUE;
				break;

			case TLT_RTFMAP:  //实时反射贴图的读取
				if (m_fN3DFileVersion >= 5.1006f)
				{
					memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTReflectionParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTReflectionParam));
					pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTReflectionParam);	
					//读取反射主体的模型索引
					int modelIdx;
					memcpy(&(modelIdx), pOffset, sizeof(modelIdx));
					pOffset +=sizeof(modelIdx);

					//创建RTF材质
					//获得模型句柄
					std::vector<CModel*>::iterator ppModel =m_pScene->model.begin();
					int modelCnt = m_pScene->model.size();
					BOOL rtfTexCreated = FALSE;
					if (modelIdx < modelCnt)
					{
						CModel* pModel = *(ppModel + modelIdx);
						//创建一个RTFMap
						CRTFMap* pRTFMap = new CRTFMap(pTexLayer->m_sName, m_pScene->m_pd3dDevice, &m_pScene->m_d3dpp, MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTReflectionParam.width, MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTReflectionParam.height, D3DFMT_R8G8B8);
						pRTFMap->SetRefPlaneByModel((DWORD)pModel);
						pRTFMap->m_texProjMatrix = m_pScene->GetTextureProjMatrix();	
						//放入RTFMap贴图队列
						m_pScene->m_vRtfTexture.push_back(pRTFMap);
						rtfTexCreated = TRUE;
						pTexLayer->pTexture = pRTFMap;
					}
					//读取被反射物体列表
					DWORD refModelCount;
					memcpy(&refModelCount, pOffset, sizeof(refModelCount));
					pOffset +=sizeof(refModelCount);

					for(UINT i =0; i<(UINT)refModelCount;i++)
					{
						int refModelIdx;
						memcpy(&refModelIdx, pOffset, sizeof(refModelIdx));
						pOffset +=sizeof(refModelIdx);
						//创建
						if (rtfTexCreated && refModelIdx < modelCnt)
						{

							CRTFMap* pRTFMap = *(m_pScene->m_vRtfTexture.end());

							//获得模型句柄
							std::vector<CModel*>::iterator ppModel =m_pScene->model.begin();
							int modelCnt = m_pScene->model.size();
							if (modelIdx < modelCnt)
							{
								CModel* pModel = *(ppModel + refModelIdx);

								//将模型加入到反射贴图的反射模型队列
								pRTFMap->AddRefModel((DWORD)pModel);

								//该模型需要渲染到反射贴图表面，所以增加附加渲染信息
								ADDITIONALRENDERINFO renderInfo;
								renderInfo.type = ADS_RTFMAP;
								renderInfo.pRenderToTexture =  (DWORD) pRTFMap;

								//放入模型的附加渲染信息队列
								pModel->m_additionalRenderInfo.push_back(renderInfo);
							}
						}
					}	
				}
				break;
				//实时折射贴图的读取
			case TLT_RTTMAP:
				if (m_fN3DFileVersion >= 5.1006f)
				{
					memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTRefractionParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTRefractionParam));
					pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.RTRefractionParam);

					//读取折射主体的模型索引
					int modelIdx;
					memcpy(&(modelIdx), pOffset, sizeof(modelIdx));
					pOffset +=sizeof(modelIdx);
					//读取被折射物体列表
					DWORD refModelCount;
					memcpy(&refModelCount, pOffset, sizeof(refModelCount));
					pOffset +=sizeof(refModelCount);

					for(UINT i =0; i<(UINT)refModelCount;i++)
					{
						int refModelIdx;
						memcpy(&refModelIdx, pOffset, sizeof(refModelIdx));
						pOffset +=sizeof(refModelIdx);
					}	        
				}
				break;
			case TLT_REFLECT:
				//51002版本，反射添加反射种类函数
				if (m_fN3DFileVersion >= 5.1002f)
				{
					memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam51002), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam51002));
					pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam51002);
					//反射贴图的反射方式
					//平面反射
					if (MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam51002.reflectionType == 0)
					{
						if (m_pScene->d3dParam.IsSupportTexGenSphereMap)
							pTexLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
						else
							pTexLayer->UVIndex = D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR;	
						pTexLayer->transformFlag = _D3DTEXTURETRANSFORMFLAGS(D3DTTFF_COUNT3 | D3DTTFF_PROJECTED);
					}
					//曲面反射
					if (MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam51002.reflectionType == 1)
					{
						if (m_pScene->d3dParam.IsSupportTexGenSphereMap)
							pTexLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
						else
							pTexLayer->UVIndex = D3DTSS_TCI_CAMERASPACENORMAL;
						pTexLayer->transformFlag = D3DTTFF_COUNT3;
					}
					//无效
					if (MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam51002.reflectionType == -1)
					{
						pTexLayer->transformFlag = D3DTTFF_DISABLE;
						pTexLayer->UVIndex = D3DTSS_TCI_PASSTHRU;
					}
				}
				else
				{
					memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam));
					pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.ReflectTextureParam);

					//曲面反射
					if (m_pScene->d3dParam.IsSupportTexGenSphereMap)
						pTexLayer->UVIndex = D3DTSS_TCI_SPHEREMAP;
					else
						pTexLayer->UVIndex = D3DTSS_TCI_CAMERASPACENORMAL;
					pTexLayer->transformFlag = D3DTTFF_COUNT3;
				}
				break;

			case TLT_CUBEMAP:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.CubicTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.CubicTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.CubicTextureParam);
				break;

			case TLT_DYNAMIC:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.DynamicTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.DynamicTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.DynamicTextureParam);
				break;
				/*
				case TLT_BUMPWATER:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BumpwaterTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BumpwaterTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BumpwaterTextureParam);
				break;
				*/

			case TLT_MOVIE:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.MovieTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.MovieTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.MovieTextureParam);
				break;

			case TLT_FLASH:
				memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.FlashTextureParam), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.FlashTextureParam));
				pOffset +=sizeof(MaterialData.mtrl[i].TextureLayerList[m].TextureParam.FlashTextureParam);
				break;
			} // switch

			//51003版本，添加了模型的UV信息  
			if (m_fN3DFileVersion >= 5.1003f && MaterialData.mtrl[i].TextureLayerList[m].type != TLT_RTFMAP && MaterialData.mtrl[i].TextureLayerList[m].type != TLT_RTTMAP)
			{
				memcpy(&(pTexLayer->USpeed), pOffset, sizeof(pTexLayer->USpeed));
				pOffset += sizeof(pTexLayer->USpeed);
				memcpy(&(pTexLayer->VSpeed), pOffset, sizeof(pTexLayer->VSpeed));
				pOffset += sizeof(pTexLayer->VSpeed);

				if (fabs(pTexLayer->USpeed) < 0.0001f) pTexLayer->USpeed = 0;
				if (fabs(pTexLayer->VSpeed) < 0.0001f) pTexLayer->VSpeed = 0;
			}
			pTexLayer->USpeed = - pTexLayer->USpeed/m_pScene->fps;
			pTexLayer->VSpeed = - pTexLayer->VSpeed/m_pScene->fps;

			if (m_fN3DFileVersion >= 5.1005f)
			{
				if(MaterialData.mtrl[i].TextureLayerList[m].type == TLT_CUBEMAP)
				{
					BYTE cubicType;
					memcpy(&(cubicType), pOffset, sizeof(cubicType));
					pOffset += sizeof(cubicType);				   
				}			
			}

			// 混色方式
			memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo));
			pOffset += sizeof(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo);

			memcpy(&(MaterialData.mtrl[i].TextureLayerList[m].ImageCount), pOffset, sizeof(MaterialData.mtrl[i].TextureLayerList[m].ImageCount));
			pOffset += sizeof(MaterialData.mtrl[i].TextureLayerList[m].ImageCount);

			UINT imageCnt =MaterialData.mtrl[i].TextureLayerList[m].ImageCount;
			MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList = new int[imageCnt];
			int *pTemint = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList;
			memcpy(pTemint, pOffset, sizeof(int) * imageCnt);
			pOffset += sizeof(int) * imageCnt;

			pTexLayer->type =(N3D_TextureType)(MaterialData.mtrl[i].TextureLayerList[m].type);
			if (pTexLayer->type != TLT_REFLECT) //反射贴图自动生成UV
			{
				pTexLayer->UVIndex = MaterialData.mtrl[i].TextureLayerList[m].UVIndex;
			}
			//设置混色模式
			pTexLayer->Blenderinfo.AlphaBlendOption=GetD3DTexOP(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo.AlphaBlendOption);
			pTexLayer->Blenderinfo.ColorBlendOption=GetD3DTexOP(MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo.ColorBlendOption);

			//设置混色参数
			D3DCOLOR blendFactor = D3DCOLOR_COLORVALUE(0,0,0, MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo.AlphaBlendParam.BlenderOP_alpha);
			pTexLayer->Blenderinfo.AlphaBlendParam = blendFactor;
			blendFactor = D3DCOLOR_COLORVALUE(0,0,0, MaterialData.mtrl[i].TextureLayerList[m].Blenderinfo.ColorBlendParam.BlenderOP_alpha);
			pTexLayer->Blenderinfo.ColorBlendParam = blendFactor;

			//加入材质的贴图列表
			pMtrl->m_vpTexLayers.push_back(pTexLayer);
		}

		//创建贴图---------------------------------------------------------------------------------------------------------
		for (m = 0; m < MaterialData.mtrl[i].TextureLayerCount; m++)
		{
			CCubeMap* pCubeMap =NULL; 
			CTexture* pTex =NULL;
			CDynamicTex* pDyTex =NULL;
			CTextureLayer* pTexLayer = pMtrl->m_vpTexLayers[m];
			std::string texName = pTexLayer->m_sName;
			switch(pTexLayer->type)
			{
			case TLT_BASAL:				
			case TLT_REFLECT:
				{
					//判断该材质是否已经被创建				
					CTexture *pTex = m_pScene->getTextureByName(texName);
					//创建新的贴图对象
					if (pTex == NULL)
					{
						pTex = m_pScene->CreateEmptyTex(texName, D3DFMT_A8R8G8B8, MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam.width,
							MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam.height, MaterialData.mtrl[i].TextureLayerList[m].Mipmap);

						//混色模式选择
						if (MaterialData.mtrl[i].BasalInfo.alphaTest)
						{
							pTex->bAlphaTest= TRUE;
						}
						//图像数据索引
						pTex->imageIdx = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[0];		  
						m_pSceneResource->texture.push_back(pTex);
						pTexLayer->pTexture = pTex;
						pTexLayer->type = TLT_BASAL;
						pTexLayer->UVIndex = MaterialData.mtrl[i].TextureLayerList[m].UVIndex;
					}
					else
					{
						pTexLayer->pTexture = pTex;
					}
					break;
				}
				//透明贴图D3DFMT_A8----------------------------------------------------------------------------------------
			case TLT_OPACITY:	
				{
					//判断该材质是否已经被创建				
					CTexture *pTex = m_pScene->getTextureByName(texName);
					//创建新的贴图对象
					if (pTex == NULL)
					{
						pTex = m_pScene->CreateEmptyTex(texName, D3DFMT_A8, MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam.width,
							MaterialData.mtrl[i].TextureLayerList[m].TextureParam.BasalTextureParam.height, MaterialData.mtrl[i].TextureLayerList[m].Mipmap);

						//混色模式选择
						if (MaterialData.mtrl[i].BasalInfo.alphaTest)
						{
							pTex->bAlphaTest= TRUE;
						}

						//图像数据索引
						pTex->imageIdx = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[0];	
						pTex->isOpacity = true;
						m_pSceneResource->texture.push_back(pTex);
						pTexLayer->pTexture = pTex;
						pTexLayer->type = TLT_OPACITY;
						pTexLayer->UVIndex = MaterialData.mtrl[i].TextureLayerList[m].UVIndex;
					}
					else
					{
						pTexLayer->pTexture = pTex;
					}
					break;
				}
				//实时反射--------------------------------------------------------------------------------------------
			case TLT_RTFMAP:
				break;
			case TLT_RTTMAP:
				break;			
			case TLT_REALTIMESHADOW: 
				break;
			case TLT_BUMPMAP: //TLT_BUMPMAP
				{
					//                 CBumpMap* pBumpMap = m_pScene->bumpTexture[m_pScene->bumpTexture.size() - 1];
					// 				if (pBumpMap->m_type != BUMPWAVEMAP)
					// 				{
					// 					//判断该材质是否已经被创建
					// 					pTexIdx = IsTextureExist(texName, 
					// 											pBumpMap->m_MapWidth, 
					// 											pBumpMap->m_MapHeight,
					// 											pBumpMap->m_BumpMapFormat);
					// 					// if(!pTexIdx)
					// 					if (pTexIdx == -1)
					// 					{
					// 						CTexture* pTex = new CTexture();
					// 						pTex->index = m_pScene->texture.size();
					// 						//Texture的名字用贴图层的名字
					// 						//strcpy(pTex->pathAndName, pTexLayer->Name);
					//                         pTex->pathAndName = pTexLayer->Name;
					// 						pTex->width = pBumpMap->m_MapWidth;//MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.width;
					// 						pTex->height = pBumpMap->m_MapHeight;//MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_BasalTextureParam.height;
					// 						pTex->mipCount = 0;// 必须等于0；MaterialData.mtrl[i].TextureLayerList[m].Mipmap;
					// 						pTex->BP = D3DFMT_A8R8G8B8;  //以R8G8B8创建，然后在InitBumpMap中转化为U8V8的贴图
					// 						pTex->type = BUMP_TYPE;
					// 
					// 						//混色模式选择
					// 						if(MaterialData.mtrl[i].BasalInfo.alphaTest)
					// 							pTex->bAlphaTest= TRUE;
					// 						
					// 						//图像数据索引
					// 						pTex->imageIdx = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[0];		  
					// 						m_pScene->texture.push_back(pTex);
					// 						m_pSceneResource->texture.push_back(pTex);
					// 						pBumpMap->m_pBumpTexture = pTex;
					// 					}
					// 					else
					// 					{
					// 						pBumpMap->m_pBumpTexture = m_pScene->texture[pTexIdx];
					// 					}	
					// 					pBumpMap->m_pBumpTexture->bumpIdx = m_pScene->bumpTexture.size() - 1;
					// 				}
					// 				pTexLayer->pTexture = (DWORD)pBumpMap;

					break;
				}

				//CubeMap--------------------------------------------------------------------------------------
			case TLT_CUBEMAP:
				{
					//判断是否支持CubeMap
					// 				if (!m_pScene->d3dParam.IsSupportCUBEMAP) break;
					// 
					// 				INT width = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_CubicTextureParam.width;
					// 				INT height = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.N3D_CubicTextureParam.height;
					// 				INT mipCount = MaterialData.mtrl[i].TextureLayerList[m].Mipmap;
					// 				
					// 				pCubeMap = new CCubeMap(m_pScene->m_pd3dDevice, 0, D3DFMT_R8G8B8, width, mipCount);
					// 
					// 				//CubeMap的名字和编号
					// 				pCubeMap->index = m_pScene->cubeMap.size(); 
					// 				pCubeMap->name = std::string(pTexLayer->Name);
					// 
					// 				for(n = 0 ; n< 6; n++)
					// 				{
					// 					INT imageIdx = MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[n];
					// 					switch(n)
					// 					{
					// 					case 0: //前 = +z
					// 						pCubeMap->imagesIdx[4] = imageIdx;
					// 						break;
					// 					case 1: //后 = -z
					// 						pCubeMap->imagesIdx[5] = imageIdx;
					// 						break;
					// 					case 2: //左 = -x
					// 						pCubeMap->imagesIdx[1] = imageIdx;
					// 						break;
					// 					case 3: //右 = +x
					// 						pCubeMap->imagesIdx[0] = imageIdx;
					// 						break;
					// 					case 4:  //上 = +y
					// 						pCubeMap->imagesIdx[2] = imageIdx;
					// 						break;
					// 					case 5: // 下= -y
					// 						pCubeMap->imagesIdx[3] = imageIdx;
					// 						break;
					// 					}
					// 				}
					// 
					// 				//放入场景资源队列
					// 				m_pScene->cubeMap.push_back(pCubeMap);
					// 				m_pSceneResource->cubeMap.push_back(pCubeMap);
					// 				pTexLayer->pTexture = DWORD(pCubeMap);

					break;
				}

				//动态贴图--------------------------------------------------------------------------------------
			case TLT_DYNAMIC:
				{
					//判断是否支持动态贴图  verify by tim
	 				if (!m_pScene->d3dParam.IsSupportDynamicTexture) break;
	 
	 				//创建动态贴图
	 				INT width = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.DynamicTextureParam.width;
	 				INT height = MaterialData.mtrl[i].TextureLayerList[m].TextureParam.DynamicTextureParam.height;
	 				INT frameCnt = MaterialData.mtrl[i].TextureLayerList[m].ImageCount;
	 				INT fps = (INT)(frameCnt/MaterialData.mtrl[i].TextureLayerList[m].TextureParam.DynamicTextureParam.duration);
	 				pDyTex = new CDynamicTex(m_pScene->m_pd3dDevice, width, height, D3DFMT_A8R8G8B8, frameCnt );

	 
	 				//动态贴图的编号，名称，格式
	 				pDyTex->index = m_pScene->dynamicTexture.size();
					pDyTex->name = std::string(pTexLayer->m_sName);
	 
	 				//混色模式选择
	 				if(MaterialData.mtrl[i].BasalInfo.alphaTest) pDyTex->bAlphaTest= TRUE;
	 
	 				//动态贴图每帧图片的编号
	 				for(int n = 0; n< frameCnt; n++)
					{			
						pDyTex->imagesIdx.push_back(MaterialData.mtrl[i].TextureLayerList[m].ImageIdxList[n]);							
					}
	 
	 				//放入场景资源队列
	 				m_pScene->dynamicTexture.push_back(pDyTex);
	 				m_pSceneResource->dynamicTexture.push_back(pDyTex);
	 				pTexLayer->pTexture =  	( CTexture* )pDyTex;
					break;
				}
			}
		} // for material texture

		SAFE_DELETE_ARRAY(MaterialData.mtrl[i].TextureLayerList);
		m_vMaterial.push_back(pMtrl);
		m_pSceneResource->shader.push_back(pMtrl);
	} // for each material

	SAFE_DELETE_ARRAY(MaterialData.mtrl);	
}