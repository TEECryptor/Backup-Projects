#include "Gets3dfileinfo.h"
#include "../CVertex.h"

#include <stdio.h>
#include <wtypes.h>

// 从文件中读一个数据结构
#define READ_FROM_FILE(value, pFile) {fread(&(value), sizeof(value), 1, pFile); }


void D3DXMatrixFromMAX( D3DXMATRIX& mtx, float* dat )
{


	for( int i = 0;i<4;i++ )
	{
		mtx.m[i][0] = dat[i*3+0] ;
		mtx.m[i][1] = dat[i*3+2] ;
		mtx.m[i][2] = dat[i*3+1] ;
	}

	mtx.m[0][3] = 0;
	mtx.m[1][3] = 0;
	mtx.m[2][3] = 0;
	mtx.m[3][3] = 1;
}

CGetS3DFileInfo::CGetS3DFileInfo(void)
{
	m_totalSize = 0;
}

CGetS3DFileInfo::~CGetS3DFileInfo(void)
{	
	m_totalSize = 0;
	Release();  
}


//------------------------------------------------------------------------------------------
//读入S3D文件到场景数据结构
//------------------------------------------------------------------------------------------
int CGetS3DFileInfo::ReadS3DFile(std::string strFileName, HWND hWnd)
{
	//打开文件数据流
	fopen_s(&m_pS3DStream, strFileName.c_str(), "rb");
	if (m_pS3DStream == NULL)
	{
		::MessageBox(GetActiveWindow(), (strFileName.c_str()), TEXT("打开文件失败"), MB_ICONERROR|MB_OK);
		return -1;
	}

	m_hWnd = hWnd;

	fseek(m_pS3DStream, 0, SEEK_END);
	m_totalSize = ftell(m_pS3DStream);
	UINT totalSize = 2 * m_totalSize;  // 因为加载s3d还需要在ImportS3D中加载到场景，所以将进度条总进度设为2倍
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, 0, totalSize);  // 开始进度
	fseek(m_pS3DStream, 0, SEEK_SET);
	
	//找到S3D文件的纯路径
	mStrFileName = strFileName;
	char filePath[512];
	GetLocalFilePath(filePath, strFileName.c_str());
	mStrPurePath = std::string(filePath);

	try
	{
		//读入文件头
		ReadFileHeader();
		DWORD curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // 开始进度

		//读入材质数据
		ReadMtrlData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // 开始进度

		//读入相机数据
		ReadCameraData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // 开始进度

		//读入灯光数据
		ReadLightData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // 开始进度

		//读入骨骼数据
		ReadSkeletonAnimationData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // 开始进度

		//读入Mesh模型数据
		ReadMeshData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // 开始进度

		//读入Shape数据
		ReadShapeData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // 开始进度

		//读入场景图
		ReadSceneGraphData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // 开始进度
	}
	catch (...)
	{
		::MessageBox(GetActiveWindow(), TEXT("读取文件失败"), TEXT("错误"), MB_ICONERROR|MB_OK);
		return -1;
	}

	fclose(m_pS3DStream);

	return 1;
}

//读入文件头
int	CGetS3DFileInfo::ReadFileHeader()
{
	//读入文件头
	fread(&header, sizeof(header), 1, m_pS3DStream);

	return 1;
}

//读入材质数据
int	CGetS3DFileInfo::ReadMtrlData()
{
	//材质数据的偏移量
	fseek(m_pS3DStream, header.mtrlDataPtr, SEEK_SET);

	//读入场景中的材质数量
	fread(&(mtrlManager.mtrlCnt), sizeof(mtrlManager.mtrlCnt), 1, m_pS3DStream);
	if (mtrlManager.mtrlCnt == 0) return 1;

	//写入场景中的材质数据
	for (int i = 0; i < mtrlManager.mtrlCnt; i++)
	{
		//保存到mtrlMananger
		VGS_Mtrl mtrl;

		//名称
		fread(&(mtrl.strLen), sizeof(mtrl.strLen), 1, m_pS3DStream);
		mtrl.name.resize(mtrl.strLen);
		fread((CHAR*)(mtrl.name.c_str()), mtrl.strLen, 1, m_pS3DStream);
		
		//材质的Shade模式
		fread(&(mtrl.shaderType), sizeof(mtrl.shaderType), 1, m_pS3DStream);

		//材质的双面渲染属性
		fread(&(mtrl.bTwoSide), sizeof(mtrl.bTwoSide), 1, m_pS3DStream);

		//材质颜色
		fread(&(mtrl.ambient), sizeof(mtrl.ambient), 1, m_pS3DStream);

		fread(&(mtrl.diffuse), sizeof(mtrl.diffuse), 1, m_pS3DStream);

		fread(&(mtrl.specular), sizeof(mtrl.specular), 1, m_pS3DStream);
		
		//材质的高光强度
		fread(&(mtrl.specularLevel), sizeof(mtrl.specularLevel), 1, m_pS3DStream);

		//高光的锐度
		fread(&(mtrl.shininess), sizeof(mtrl.shininess), 1, m_pS3DStream);

		//自发光
		fread(&(mtrl.selfIllumination), sizeof(mtrl.selfIllumination), 1, m_pS3DStream);

		//材质的透明度
		fread(&(mtrl.opacity), sizeof(mtrl.opacity), 1, m_pS3DStream);
		
		//材质用到的贴图
		//diffuse贴图
		fread(&(mtrl.texture.diffuseTexture_strLen), sizeof(mtrl.texture.diffuseTexture_strLen), 1, m_pS3DStream);
		if (mtrl.texture.diffuseTexture_strLen > 0)
		{
			mtrl.texture.diffuseTexture.resize(mtrl.texture.diffuseTexture_strLen);
			fread((CHAR*)(mtrl.texture.diffuseTexture.c_str()), mtrl.texture.diffuseTexture_strLen, 1, m_pS3DStream);
			mtrl.texture.diffuseTexture_uvIndex = 0;
			if (header.s3dVersion >= (float)3.01)
			{
				// 贴图使用的UV索引
				fread(&mtrl.texture.diffuseTexture_uvIndex, sizeof(mtrl.texture.diffuseTexture_uvIndex), 1, m_pS3DStream);
				mtrl.texture.diffuseTexture_uvIndex -= 1;
			}
		}

		//selfIllumination通道//光贴图放在自发光贴图通道里
		fread(&(mtrl.texture.selfIlluminationTexture_strLen), sizeof(mtrl.texture.selfIlluminationTexture_strLen), 1, m_pS3DStream);
		if (mtrl.texture.selfIlluminationTexture_strLen > 0)
		{
			mtrl.texture.selfIlluminationTexture.resize(mtrl.texture.selfIlluminationTexture_strLen);
			fread((CHAR*)(mtrl.texture.selfIlluminationTexture.c_str()), mtrl.texture.selfIlluminationTexture_strLen, 1, m_pS3DStream);
			mtrl.texture.selfIlluminationTexture_uvIndex = 2;
			if (header.s3dVersion >= (float)3.01)
			{
				// 贴图使用的UV索引
				fread(&mtrl.texture.selfIlluminationTexture_uvIndex, sizeof(mtrl.texture.selfIlluminationTexture_uvIndex), 1, m_pS3DStream);
				mtrl.texture.selfIlluminationTexture_uvIndex -= 1;
			}
		}
		
		//opacity通道
		fread(&(mtrl.texture.opacityTexture_strLen), sizeof(mtrl.texture.opacityTexture_strLen), 1, m_pS3DStream);
		if (mtrl.texture.opacityTexture_strLen > 0)
		{
			mtrl.texture.opacityTexture.resize(mtrl.texture.opacityTexture_strLen);
			fread((CHAR*)(mtrl.texture.opacityTexture.c_str()), mtrl.texture.opacityTexture_strLen, 1, m_pS3DStream);
			mtrl.texture.opacityTexture_uvIndex = 0;
			if (header.s3dVersion >= (float)3.01)
			{
				// 贴图使用的UV索引
				fread(&mtrl.texture.opacityTexture_uvIndex, sizeof(mtrl.texture.opacityTexture_uvIndex), 1, m_pS3DStream);
				mtrl.texture.opacityTexture_uvIndex -= 1;
			}
		}
		
		//reflection通道
		fread(&(mtrl.texture.reflectionTexture_strLen), sizeof(mtrl.texture.reflectionTexture_strLen), 1, m_pS3DStream);
		if (mtrl.texture.reflectionTexture_strLen > 0)
		{
			mtrl.texture.reflectionTexture.resize(mtrl.texture.reflectionTexture_strLen);
			fread((CHAR*)(mtrl.texture.reflectionTexture.c_str()), mtrl.texture.reflectionTexture_strLen, 1, m_pS3DStream);
			mtrl.texture.reflectionTexture_uvIndex = 0;
			if (header.s3dVersion >= (float)3.01)
			{
				// 贴图使用的UV索引
				fread(&mtrl.texture.reflectionTexture_uvIndex, sizeof(mtrl.texture.reflectionTexture_uvIndex), 1, m_pS3DStream);
				mtrl.texture.reflectionTexture_uvIndex -= 1;
			}
		}
			
		//bump通道
		fread(&(mtrl.texture.bumpTexture_strLen), sizeof(mtrl.texture.bumpTexture_strLen), 1, m_pS3DStream);
		if (mtrl.texture.bumpTexture_strLen > 0)
		{
			mtrl.texture.bumpTexture.resize(mtrl.texture.bumpTexture_strLen);
			fread((CHAR*)(mtrl.texture.bumpTexture.c_str()), mtrl.texture.bumpTexture_strLen, 1, m_pS3DStream);
			mtrl.texture.bumpTexture_uvIndex = 0;
			if (header.s3dVersion >= (float)3.01)
			{
				// 贴图使用的UV索引
				fread(&mtrl.texture.bumpTexture_uvIndex, sizeof(mtrl.texture.bumpTexture_uvIndex), 1, m_pS3DStream);
				mtrl.texture.bumpTexture_uvIndex -= 1;
			}				 
		}
			
		//displacement通道//光贴图放在自发光贴图通道里
		fread(&(mtrl.texture.displacementTexture_strLen), sizeof(mtrl.texture.displacementTexture_strLen), 1, m_pS3DStream);
		if (mtrl.texture.displacementTexture_strLen > 0)
		{
			mtrl.texture.displacementTexture.resize(mtrl.texture.displacementTexture_strLen);
			fread((CHAR*)(mtrl.texture.displacementTexture.c_str()), mtrl.texture.displacementTexture_strLen, 1, m_pS3DStream);
			mtrl.texture.displacementTexture_uvIndex = 0;
			if (header.s3dVersion >= (float)3.01)
			{
				// 贴图使用的UV索引
				fread(&mtrl.texture.displacementTexture_uvIndex, sizeof(mtrl.texture.displacementTexture_uvIndex), 1, m_pS3DStream);
				mtrl.texture.displacementTexture_uvIndex -= 1;
			}
		}

		//加入material队列
		mtrlManager.mtrl.push_back(mtrl);
	}
	return 1;
}

//读入相机数据
int	CGetS3DFileInfo::ReadCameraData()
{
	//相机数据的偏移量
	fseek(m_pS3DStream, header.cameraDataPtr, SEEK_SET);

	//读入相机的数量
	fread(&(cameraManager.cameraCnt), sizeof(cameraManager.cameraCnt), 1, m_pS3DStream);
	if (cameraManager.cameraCnt == 0)
		return 1;

	//读入相机数据
	for (int i = 0; i< cameraManager.cameraCnt; i++)
	{
		VGS_Camera camera;
		
		//名称
		fread(&(camera.strLen), sizeof(camera.strLen), 1, m_pS3DStream);
		camera.name.resize(camera.strLen);
		fread((CHAR*)(camera.name.c_str()), camera.strLen, 1, m_pS3DStream);
		
		//是否是轴侧投影
		fread(&(camera.isOrtho),sizeof(camera.isOrtho), 1, m_pS3DStream);

		//读入帧数
		int frameCnt;
		fread(&frameCnt, sizeof(frameCnt), 1, m_pS3DStream);
		
		//读入帧数据
		for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
		{
			VGS_CAMERA_INFO camInfo;

			//读入相机的from
			fread(&camInfo, sizeof(camInfo), 1, m_pS3DStream);
			
			//放入队列
			camera.camInfo.push_back(camInfo);
		}

		//加入Camera队列
		cameraManager.camera.push_back(camera);
	}
	return 1;
}

//读入灯光数据
int	CGetS3DFileInfo::ReadLightData()
{	
	//灯光数据的偏移量
	fseek(m_pS3DStream, header.lightDataPtr, SEEK_SET);

	//读入灯光数量
	fread(&(lightManager.lightCnt), sizeof(lightManager.lightCnt), 1, m_pS3DStream);
	if (lightManager.lightCnt == 0) return 1;

	//读入灯光数据
	for (int i = 0; i< lightManager.lightCnt; i++)
	{
		VGS_Light light;
		
		fread(&(light.strLen), sizeof(light.strLen), 1, m_pS3DStream);
		light.name.resize(light.strLen + 1);
		fread((CHAR*)(light.name.c_str()), light.strLen, 1, m_pS3DStream);

		//灯光类型
		fread(&(light.type), sizeof(light.type), 1, m_pS3DStream);
		
		//读入帧数
		int frameCnt;
		fread(&frameCnt, sizeof(frameCnt), 1, m_pS3DStream);

		//写入帧数据
		for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
		{
			VGS_LIGHT_INFO lightInfo;
			ZeroMemory(&lightInfo, sizeof(lightInfo));

			//灯光颜色
			fread(&(lightInfo.color), sizeof(lightInfo.color), 1, m_pS3DStream);
			
			//光线的衰减范围
			fread(&(lightInfo.range1), sizeof(lightInfo.range1), 1, m_pS3DStream);
			fread(&(lightInfo.range2), sizeof(lightInfo.range2), 1, m_pS3DStream);

			//光照强度
			fread(&(lightInfo.multiply), sizeof(lightInfo.multiply), 1, m_pS3DStream);

			//内角外角，只有在SpotLight时才有
			if(light.type == SPOTLIGHT)
			{
				fread(&(lightInfo.theta), sizeof(lightInfo.theta), 1, m_pS3DStream);
				fread(&(lightInfo.phi), sizeof(lightInfo.phi), 1, m_pS3DStream);
			}

			// 3.02读入灯光的相对位置和方向
			if (header.s3dVersion >= 3.02f)
			{
				if (light.type != DIRECTIONLIGHT)
				{
					fread(&(lightInfo.pos), sizeof(lightInfo.pos), 1, m_pS3DStream);				
				}
				if (light.type != OMINI)
				{
					fread(&(lightInfo.dir), sizeof(lightInfo.dir), 1, m_pS3DStream);
				}
			}

			light.lightInfo.push_back(lightInfo);
		}

		//加入灯光队列
		lightManager.light.push_back(light);
	}

	return 1;
}

//读入骨骼数据
int	CGetS3DFileInfo::ReadSkeletonAnimationData()
{
	//骨骼动画数据的偏移量
	fseek(m_pS3DStream, header.skeletonAnimationDataPtr, SEEK_SET);

	//读入骨骼动画的数量
	fread(&(skeletonAnimationManager.skeletonAnimationCnt), sizeof(skeletonAnimationManager.skeletonAnimationCnt), 1, m_pS3DStream);
	if (skeletonAnimationManager.skeletonAnimationCnt == 0) return 1;

	//逐个骨骼动画读入
	for (int i = 0; i< skeletonAnimationManager.skeletonAnimationCnt; i++)
	{
		VGS_SkeletonAnimation skeletonAnimation;

		//读入骨骼动画的名称
		fread(&(skeletonAnimation.strLen), sizeof(skeletonAnimation.strLen), 1, m_pS3DStream);
		skeletonAnimation.name.resize(skeletonAnimation.strLen);
		fread((CHAR*)(skeletonAnimation.name.c_str()), skeletonAnimation.strLen, 1, m_pS3DStream);

		//读入骨骼动画的帧长度
		fread(&(skeletonAnimation.frameCnt), sizeof(skeletonAnimation.frameCnt), 1, m_pS3DStream);
		//读入骨头的数量
		fread(&(skeletonAnimation.boneCnt), sizeof(skeletonAnimation.boneCnt), 1, m_pS3DStream);

		//读入骨骼动画帧数据
		ProcessSkeletonAnimationStruct(&(skeletonAnimation.boneAnimation), skeletonAnimation.frameCnt);

		//放入骨骼动画队列
		skeletonAnimationManager.skeletonAnimation.push_back(skeletonAnimation);
	}

	return 1;
}

//处理并读入骨骼数据
//------------------------------------------------------------------------------------------
void CGetS3DFileInfo::ProcessSkeletonAnimationStruct(VGS_BoneAnimation* pBoneAnimation, int frameCnt)
{
	//骨头的名称
	fread(&(pBoneAnimation->strLen), sizeof(pBoneAnimation->strLen), 1, m_pS3DStream);
	pBoneAnimation->name.resize(pBoneAnimation->strLen);
	fread((CHAR*)(pBoneAnimation->name.c_str()), pBoneAnimation->strLen, 1, m_pS3DStream);

	//读入关键帧帧数据
	for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
	{
		NodeTransform transform;
		//读入骨骼的关键帧动画
		fread(&transform, sizeof(transform), 1, m_pS3DStream);
		pBoneAnimation->transform.push_back(transform);
	}

	//当前骨头的子骨头数量
	fread(&pBoneAnimation->childBoneCnt, sizeof(pBoneAnimation->childBoneCnt), 1, m_pS3DStream);

	//骨头的子骨头
	for(int i = 0; i < pBoneAnimation->childBoneCnt; i++)
	{
		VGS_BoneAnimation childBoneAnimation;
		ProcessSkeletonAnimationStruct(&childBoneAnimation, frameCnt);
		pBoneAnimation->childBoneAnimation.push_back(childBoneAnimation);
	}
}


//读入Mesh模型数据
int	CGetS3DFileInfo::ReadMeshData()
{
	//Mesh数据的偏移量
	fseek(m_pS3DStream, header.meshDataPtr, SEEK_SET);

	// 读入Mesh的数量
	UINT meshCount = 0;
	READ_FROM_FILE(meshCount, m_pS3DStream);
	if (meshCount == 0) return 1;

	//读入Mesh列表
	m_vMeshData.resize(meshCount);
	for (int meshIndex = 0; meshIndex < meshCount; meshIndex++)
	{
		CVGSMesh &curMesh = m_vMeshData[meshIndex];
		curMesh.faceCnt = 0;

		// 名称
		int nameLen = 0;
		READ_FROM_FILE(nameLen, m_pS3DStream);
		char sName[MAX_FILENAME_LENGTH + 1];
		_ZeroMemory(sName);
		fread(sName, nameLen, 1, m_pS3DStream);
		curMesh.sName = std::string(sName);

		// 读入顶点数量
		fread(&(curMesh.vertexCnt), sizeof(curMesh.vertexCnt), 1, m_pS3DStream);

		//读入顶点列表
		curMesh.fvf = FVF_POSNORMALCOLORCOORD4;
		curMesh.pVertex = new CVertex_POSNORMALCOLORCOORD4[curMesh.vertexCnt];  // 顶点buffer

		BYTE uvFlag;
		for (int j = 0; j < curMesh.vertexCnt; j++)
		{
			CVertex_POSNORMALCOLORCOORD4 *pVertex = &(((CVertex_POSNORMALCOLORCOORD4*)(curMesh.pVertex))[j]);
			
			// 位置, max中是右手坐标系，而且y轴向上
			fread(&(pVertex->position), sizeof(pVertex->position), 1, m_pS3DStream);
			float y = pVertex->position.y;
			pVertex->position.y = pVertex->position.z;
			pVertex->position.z = y;

			// 法线, max中是右手坐标系，而且y轴向上
			fread(&(pVertex->normal), sizeof(pVertex->normal), 1, m_pS3DStream);
			y = pVertex->normal.y;
			pVertex->normal.y = pVertex->normal.z;
			pVertex->normal.z = y;

			//颜色
			fread(&(pVertex->color), sizeof(pVertex->color), 1, m_pS3DStream);

			//UV Flag
			fread(&uvFlag, sizeof(uvFlag), 1, m_pS3DStream);
			
			int bUV1 = uvFlag & 0x01;
			int bUV2 = uvFlag & 0x02;
			int bUV3 = uvFlag & 0x04;
			int bUV4 = uvFlag & 0x08;
			//UV1坐标
			if (bUV1)
			{
				fread(&(pVertex->uv1), sizeof(pVertex->uv1), 1, m_pS3DStream);
				pVertex->uv1.v = 1.0f - pVertex->uv1.v;  // max中uv坐标系在左下角,我们在左上角
			}

			//UV2坐标
			if (bUV2)
			{
				fread(&(pVertex->uv2), sizeof(pVertex->uv2), 1, m_pS3DStream);
				pVertex->uv2.v = 1.0f - pVertex->uv2.v;  // max中uv坐标系在左下角,我们在左上角
			}

			//UV3坐标
			if (bUV3)
			{
				fread(&(pVertex->uv3), sizeof(pVertex->uv3), 1, m_pS3DStream);
				pVertex->uv3.v = 1.0f - pVertex->uv3.v;  // max中uv坐标系在左下角,我们在左上角
			}

			//UV4坐标
			if (bUV4)
			{
				fread(&(pVertex->uv4), sizeof(pVertex->uv4), 1, m_pS3DStream);
				pVertex->uv4.v = 1.0f - pVertex->uv4.v;  // max中uv坐标系在左下角,我们在左上角
			}

		} // for j


		//包围盒
		VGS_BoundBox boundBox;
		fread(&(boundBox), sizeof(boundBox), 1, m_pS3DStream);

		//读入SubMesh数量
		READ_FROM_FILE(curMesh.subMeshCount, m_pS3DStream);

		//读入SubMesh数据
		curMesh.pSubMesh = new CVGSSubMesh[curMesh.subMeshCount];
		for (int subMeshIndex = 0; subMeshIndex < curMesh.subMeshCount; subMeshIndex++)
		{
			CVGSSubMesh &subMesh = ((CVGSSubMesh*)(curMesh.pSubMesh))[subMeshIndex];
			
			//读入SubMesh的材质编号
			READ_FROM_FILE(subMesh.mtrIndex, m_pS3DStream);

			//读入SubMesh的面表
			READ_FROM_FILE(subMesh.faceCount, m_pS3DStream);

			//pMesh的总面数
			curMesh.faceCnt += subMesh.faceCount;

			subMesh.pFace = new CVGSFace[subMesh.faceCount];  // face
			for (int faceIndex = 0; faceIndex < subMesh.faceCount; faceIndex++)
			{
				CVGSFace &face = subMesh.pFace[faceIndex];
				READ_FROM_FILE(face.a, m_pS3DStream);
				READ_FROM_FILE(face.b, m_pS3DStream);
				READ_FROM_FILE(face.c, m_pS3DStream);
			}
		} // for subMeshIndex

		// 绑定的骨骼编
		READ_FROM_FILE(curMesh.SkeletonAnimationIndex, m_pS3DStream);

		//版本判断
		if (header.s3dVersion == 3.03f)
		{
			if (curMesh.SkeletonAnimationIndex != -1)
			{
				//读入面数量
				fread(&curMesh.faceCnt, sizeof(curMesh.faceCnt), 1, m_pS3DStream);

				//读入绑定的骨骼数量
				fread(&(curMesh.boneCnt), sizeof(curMesh.boneCnt), 1, m_pS3DStream);

				//递归读入蒙皮数据
				ReadSkeletonAndSkinData(&curMesh.bone);

				//读入骨骼根骨头在Mesh坐标系中的变换
				fread(&(curMesh.skeletontransform), sizeof(curMesh.skeletontransform), 1, m_pS3DStream);
			}
		}

	} // for meshIndex

	return 1;
}

//------------------------------------------------------------------------------------------
//递归读入蒙皮数据
//------------------------------------------------------------------------------------------
void CGetS3DFileInfo::ReadSkeletonAndSkinData(VGS_Bone* pVGSBone)
{
	//递归读入VGS_Bone数据

	//读入入骨头的名称
	fread(&pVGSBone->strLen, sizeof(pVGSBone->strLen), 1, m_pS3DStream);
	pVGSBone->name.resize(pVGSBone->strLen);
	fread((CHAR*)(pVGSBone->name.c_str()), pVGSBone->strLen, 1, m_pS3DStream);

	//读入骨头的初始姿态
	fread(&pVGSBone->startPose, sizeof(pVGSBone->startPose), 1, m_pS3DStream);

	//读入骨头绑定的顶点数量
	fread(&pVGSBone->bindingVertexCnt, sizeof(pVGSBone->bindingVertexCnt), 1, m_pS3DStream);

	//读入绑定的顶点编号列表
	//读入绑定的顶点权重列表
	if (pVGSBone->bindingVertexCnt > 0)
	{
		pVGSBone->bindingVertexIndex.resize(pVGSBone->bindingVertexCnt);
		fread(&(pVGSBone->bindingVertexIndex[0]), sizeof(DWORD) * pVGSBone->bindingVertexCnt, 1, m_pS3DStream);
		pVGSBone->bindingWeight.resize(pVGSBone->bindingVertexCnt);
		fread(&(pVGSBone->bindingWeight[0]), sizeof(FLOAT) * pVGSBone->bindingVertexCnt, 1, m_pS3DStream);
	}

	//读入子骨头的数量、
	fread(&pVGSBone->childBoneCnt, sizeof(pVGSBone->childBoneCnt), 1, m_pS3DStream);

	//递归骨头的子骨头
	for (int i = 0; i<pVGSBone->childBoneCnt; i++)
	{
		VGS_Bone childVGSBone;
		ReadSkeletonAndSkinData(&childVGSBone);
		pVGSBone->childBone.push_back(childVGSBone);
	}
}

//------------------------------------------------------------------------------------------
//读入Shape数据
int	CGetS3DFileInfo::ReadShapeData()
{
	return 1;
}

//读入场景图
int	CGetS3DFileInfo::ReadSceneGraphData()
{
	//场景图数据的偏移量
	fseek(m_pS3DStream, header.sceneGraphDataPtr, SEEK_SET);

	//读入场景节点数量
	fread(&(sceneNodeGraph.nodeCnt), sizeof(sceneNodeGraph.nodeCnt), 1, m_pS3DStream);

	//构造并读入场景图结构
	ConstructSceneGraph(&(sceneNodeGraph.node));

	return 1;
}

//------------------------------------------------------------------------------------------
//构造场景图数据
//------------------------------------------------------------------------------------------
void CGetS3DFileInfo::ConstructSceneGraph(VGS_Node* pNode)
{
	//node的类型
	fread(&(pNode->type), sizeof(pNode->type), 1, m_pS3DStream);
	
	//node的编号
	fread(&(pNode->index), sizeof(pNode->index), 1, m_pS3DStream);

	//node的名称
	fread(&(pNode->strLen), sizeof(pNode->strLen), 1, m_pS3DStream);
	pNode->name.reserve(pNode->strLen);
	fread((void*)(pNode->name.c_str()), pNode->strLen, 1, m_pS3DStream);

	//读入动画帧数
	int frameCnt = 1;
	fread(&frameCnt, sizeof(frameCnt), 1, m_pS3DStream);

	//写入帧数据
	for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
	{
		NodeTransform transform;
		fread(&transform, sizeof(transform), 1, m_pS3DStream);
		//关键帧动画队列
		pNode->keyframeTransform.push_back(transform);
	}

	//子物体队列
	int childNodeCnt = 0;
	fread(&childNodeCnt, sizeof(childNodeCnt), 1, m_pS3DStream);
	pNode->childNode.reserve(childNodeCnt);
	for (int i = 0; i< childNodeCnt; i++)
	{
		VGS_Node node;
		pNode->childNode.push_back(node);
		ConstructSceneGraph(&(pNode->childNode[i]));
	}

	//m_totalSize = 0;
}


//释放资源
void CGetS3DFileInfo::Release()
{
	//删除材质数组
	mtrlManager.mtrl.clear();
	//删除相机
	cameraManager.camera.clear();
	//删除灯光
	lightManager.light.clear();
	//删除2D图形
	shapeManager.shape.clear();

	//删除模型和蒙皮数据
	ReleaseMeshAndSkinManager();

	//释放骨骼动画数据
	ReleaseSkeletonAnimationManager();

	//删除场景结构图
	ReleaseNodeRecursion(sceneNodeGraph.node);
	//删除贴图
}

//释放骨骼动画数据管理对象 SkeletonAnimationManager
void CGetS3DFileInfo::ReleaseSkeletonAnimationManager()
{
	for (int i = 0; i< skeletonAnimationManager.skeletonAnimationCnt; i++)
	{
		VGS_SkeletonAnimation* pSkeletonAnimation = &(skeletonAnimationManager.skeletonAnimation[i]);
		//递归释放骨骼动画数据
		ReleaseBoneAnimationRecursion(&pSkeletonAnimation->boneAnimation);
	}
	skeletonAnimationManager.skeletonAnimation.clear();
}

//递归释放骨骼动画数据
void CGetS3DFileInfo::ReleaseBoneAnimationRecursion(VGS_BoneAnimation* pBoneAnimation)
{
	pBoneAnimation->name.clear();
	pBoneAnimation->transform.clear();
	for (int i = 0; i< pBoneAnimation->childBoneCnt; i++)
		ReleaseBoneAnimationRecursion(&(pBoneAnimation->childBoneAnimation[i]));
	pBoneAnimation->childBoneAnimation.clear();
}

//删除模型和蒙皮数据
void CGetS3DFileInfo::ReleaseMeshAndSkinManager()
{
	//for (int i = 0; i< meshManager.meshCnt; i++)
	//{
	//	VGS_Mesh* pMesh = &(meshManager.mesh[i]);
	//	//释放Mesh的的蒙皮数据
	//	if (pMesh->SkeletonAnimationIndex != -1)
	//		ReleaseMeshAndSkinRecursion(&pMesh->bone);
	//}
}

//递归释放模型和蒙皮数据
void CGetS3DFileInfo::ReleaseMeshAndSkinRecursion(VGS_Bone* pBone)
{
	pBone->name.clear();
	pBone->bindingVertexIndex.clear();
	pBone->bindingWeight.clear();
	for (int i = 0; i< pBone->childBoneCnt; i++)
		ReleaseMeshAndSkinRecursion(&(pBone->childBone[i]));
}


//删除场景图
void CGetS3DFileInfo::ReleaseNodeRecursion(VGS_Node& node)
{
	node.name = "";
	node.keyframeTransform.clear();
	int childNodeCnt = node.childNode.size();
	for (int i = 0; i< childNodeCnt; i++)
		ReleaseNodeRecursion(node.childNode[i]);
	node.childNode.clear();
}

//删除贴图
void CGetS3DFileInfo::ReleaseTexture(std::vector<VGS_TextureDev>& textureManager)
{
	int texCnt = textureManager.size();
	for (int i = 0; i< texCnt; i++)
	{
		VGS_TextureDev* pVGSTex = &(textureManager[i]);
		pVGSTex->fileName = "";
		int texDevCnt = pVGSTex->usedMtrlList.size();
		for (int j = 0; j< texDevCnt; j++)
		{
			VGS_MtrlUseTex* pTexDev = &(pVGSTex->usedMtrlList[j]);
			pTexDev->mtrlName = "";
		}
		pVGSTex->usedMtrlList.clear();
	}
	textureManager.clear();
}


