#include "Gets3dfileinfo.h"
#include "../CVertex.h"

#include <stdio.h>
#include <wtypes.h>

// ���ļ��ж�һ�����ݽṹ
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
//����S3D�ļ����������ݽṹ
//------------------------------------------------------------------------------------------
int CGetS3DFileInfo::ReadS3DFile(std::string strFileName, HWND hWnd)
{
	//���ļ�������
	fopen_s(&m_pS3DStream, strFileName.c_str(), "rb");
	if (m_pS3DStream == NULL)
	{
		::MessageBox(GetActiveWindow(), (strFileName.c_str()), TEXT("���ļ�ʧ��"), MB_ICONERROR|MB_OK);
		return -1;
	}

	m_hWnd = hWnd;

	fseek(m_pS3DStream, 0, SEEK_END);
	m_totalSize = ftell(m_pS3DStream);
	UINT totalSize = 2 * m_totalSize;  // ��Ϊ����s3d����Ҫ��ImportS3D�м��ص����������Խ��������ܽ�����Ϊ2��
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, 0, totalSize);  // ��ʼ����
	fseek(m_pS3DStream, 0, SEEK_SET);
	
	//�ҵ�S3D�ļ��Ĵ�·��
	mStrFileName = strFileName;
	char filePath[512];
	GetLocalFilePath(filePath, strFileName.c_str());
	mStrPurePath = std::string(filePath);

	try
	{
		//�����ļ�ͷ
		ReadFileHeader();
		DWORD curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // ��ʼ����

		//�����������
		ReadMtrlData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // ��ʼ����

		//�����������
		ReadCameraData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // ��ʼ����

		//����ƹ�����
		ReadLightData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // ��ʼ����

		//�����������
		ReadSkeletonAnimationData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // ��ʼ����

		//����Meshģ������
		ReadMeshData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // ��ʼ����

		//����Shape����
		ReadShapeData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // ��ʼ����

		//���볡��ͼ
		ReadSceneGraphData();
		curProgress = ftell(m_pS3DStream);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, curProgress, totalSize);  // ��ʼ����
	}
	catch (...)
	{
		::MessageBox(GetActiveWindow(), TEXT("��ȡ�ļ�ʧ��"), TEXT("����"), MB_ICONERROR|MB_OK);
		return -1;
	}

	fclose(m_pS3DStream);

	return 1;
}

//�����ļ�ͷ
int	CGetS3DFileInfo::ReadFileHeader()
{
	//�����ļ�ͷ
	fread(&header, sizeof(header), 1, m_pS3DStream);

	return 1;
}

//�����������
int	CGetS3DFileInfo::ReadMtrlData()
{
	//�������ݵ�ƫ����
	fseek(m_pS3DStream, header.mtrlDataPtr, SEEK_SET);

	//���볡���еĲ�������
	fread(&(mtrlManager.mtrlCnt), sizeof(mtrlManager.mtrlCnt), 1, m_pS3DStream);
	if (mtrlManager.mtrlCnt == 0) return 1;

	//д�볡���еĲ�������
	for (int i = 0; i < mtrlManager.mtrlCnt; i++)
	{
		//���浽mtrlMananger
		VGS_Mtrl mtrl;

		//����
		fread(&(mtrl.strLen), sizeof(mtrl.strLen), 1, m_pS3DStream);
		mtrl.name.resize(mtrl.strLen);
		fread((CHAR*)(mtrl.name.c_str()), mtrl.strLen, 1, m_pS3DStream);
		
		//���ʵ�Shadeģʽ
		fread(&(mtrl.shaderType), sizeof(mtrl.shaderType), 1, m_pS3DStream);

		//���ʵ�˫����Ⱦ����
		fread(&(mtrl.bTwoSide), sizeof(mtrl.bTwoSide), 1, m_pS3DStream);

		//������ɫ
		fread(&(mtrl.ambient), sizeof(mtrl.ambient), 1, m_pS3DStream);

		fread(&(mtrl.diffuse), sizeof(mtrl.diffuse), 1, m_pS3DStream);

		fread(&(mtrl.specular), sizeof(mtrl.specular), 1, m_pS3DStream);
		
		//���ʵĸ߹�ǿ��
		fread(&(mtrl.specularLevel), sizeof(mtrl.specularLevel), 1, m_pS3DStream);

		//�߹�����
		fread(&(mtrl.shininess), sizeof(mtrl.shininess), 1, m_pS3DStream);

		//�Է���
		fread(&(mtrl.selfIllumination), sizeof(mtrl.selfIllumination), 1, m_pS3DStream);

		//���ʵ�͸����
		fread(&(mtrl.opacity), sizeof(mtrl.opacity), 1, m_pS3DStream);
		
		//�����õ�����ͼ
		//diffuse��ͼ
		fread(&(mtrl.texture.diffuseTexture_strLen), sizeof(mtrl.texture.diffuseTexture_strLen), 1, m_pS3DStream);
		if (mtrl.texture.diffuseTexture_strLen > 0)
		{
			mtrl.texture.diffuseTexture.resize(mtrl.texture.diffuseTexture_strLen);
			fread((CHAR*)(mtrl.texture.diffuseTexture.c_str()), mtrl.texture.diffuseTexture_strLen, 1, m_pS3DStream);
			mtrl.texture.diffuseTexture_uvIndex = 0;
			if (header.s3dVersion >= (float)3.01)
			{
				// ��ͼʹ�õ�UV����
				fread(&mtrl.texture.diffuseTexture_uvIndex, sizeof(mtrl.texture.diffuseTexture_uvIndex), 1, m_pS3DStream);
				mtrl.texture.diffuseTexture_uvIndex -= 1;
			}
		}

		//selfIlluminationͨ��//����ͼ�����Է�����ͼͨ����
		fread(&(mtrl.texture.selfIlluminationTexture_strLen), sizeof(mtrl.texture.selfIlluminationTexture_strLen), 1, m_pS3DStream);
		if (mtrl.texture.selfIlluminationTexture_strLen > 0)
		{
			mtrl.texture.selfIlluminationTexture.resize(mtrl.texture.selfIlluminationTexture_strLen);
			fread((CHAR*)(mtrl.texture.selfIlluminationTexture.c_str()), mtrl.texture.selfIlluminationTexture_strLen, 1, m_pS3DStream);
			mtrl.texture.selfIlluminationTexture_uvIndex = 2;
			if (header.s3dVersion >= (float)3.01)
			{
				// ��ͼʹ�õ�UV����
				fread(&mtrl.texture.selfIlluminationTexture_uvIndex, sizeof(mtrl.texture.selfIlluminationTexture_uvIndex), 1, m_pS3DStream);
				mtrl.texture.selfIlluminationTexture_uvIndex -= 1;
			}
		}
		
		//opacityͨ��
		fread(&(mtrl.texture.opacityTexture_strLen), sizeof(mtrl.texture.opacityTexture_strLen), 1, m_pS3DStream);
		if (mtrl.texture.opacityTexture_strLen > 0)
		{
			mtrl.texture.opacityTexture.resize(mtrl.texture.opacityTexture_strLen);
			fread((CHAR*)(mtrl.texture.opacityTexture.c_str()), mtrl.texture.opacityTexture_strLen, 1, m_pS3DStream);
			mtrl.texture.opacityTexture_uvIndex = 0;
			if (header.s3dVersion >= (float)3.01)
			{
				// ��ͼʹ�õ�UV����
				fread(&mtrl.texture.opacityTexture_uvIndex, sizeof(mtrl.texture.opacityTexture_uvIndex), 1, m_pS3DStream);
				mtrl.texture.opacityTexture_uvIndex -= 1;
			}
		}
		
		//reflectionͨ��
		fread(&(mtrl.texture.reflectionTexture_strLen), sizeof(mtrl.texture.reflectionTexture_strLen), 1, m_pS3DStream);
		if (mtrl.texture.reflectionTexture_strLen > 0)
		{
			mtrl.texture.reflectionTexture.resize(mtrl.texture.reflectionTexture_strLen);
			fread((CHAR*)(mtrl.texture.reflectionTexture.c_str()), mtrl.texture.reflectionTexture_strLen, 1, m_pS3DStream);
			mtrl.texture.reflectionTexture_uvIndex = 0;
			if (header.s3dVersion >= (float)3.01)
			{
				// ��ͼʹ�õ�UV����
				fread(&mtrl.texture.reflectionTexture_uvIndex, sizeof(mtrl.texture.reflectionTexture_uvIndex), 1, m_pS3DStream);
				mtrl.texture.reflectionTexture_uvIndex -= 1;
			}
		}
			
		//bumpͨ��
		fread(&(mtrl.texture.bumpTexture_strLen), sizeof(mtrl.texture.bumpTexture_strLen), 1, m_pS3DStream);
		if (mtrl.texture.bumpTexture_strLen > 0)
		{
			mtrl.texture.bumpTexture.resize(mtrl.texture.bumpTexture_strLen);
			fread((CHAR*)(mtrl.texture.bumpTexture.c_str()), mtrl.texture.bumpTexture_strLen, 1, m_pS3DStream);
			mtrl.texture.bumpTexture_uvIndex = 0;
			if (header.s3dVersion >= (float)3.01)
			{
				// ��ͼʹ�õ�UV����
				fread(&mtrl.texture.bumpTexture_uvIndex, sizeof(mtrl.texture.bumpTexture_uvIndex), 1, m_pS3DStream);
				mtrl.texture.bumpTexture_uvIndex -= 1;
			}				 
		}
			
		//displacementͨ��//����ͼ�����Է�����ͼͨ����
		fread(&(mtrl.texture.displacementTexture_strLen), sizeof(mtrl.texture.displacementTexture_strLen), 1, m_pS3DStream);
		if (mtrl.texture.displacementTexture_strLen > 0)
		{
			mtrl.texture.displacementTexture.resize(mtrl.texture.displacementTexture_strLen);
			fread((CHAR*)(mtrl.texture.displacementTexture.c_str()), mtrl.texture.displacementTexture_strLen, 1, m_pS3DStream);
			mtrl.texture.displacementTexture_uvIndex = 0;
			if (header.s3dVersion >= (float)3.01)
			{
				// ��ͼʹ�õ�UV����
				fread(&mtrl.texture.displacementTexture_uvIndex, sizeof(mtrl.texture.displacementTexture_uvIndex), 1, m_pS3DStream);
				mtrl.texture.displacementTexture_uvIndex -= 1;
			}
		}

		//����material����
		mtrlManager.mtrl.push_back(mtrl);
	}
	return 1;
}

//�����������
int	CGetS3DFileInfo::ReadCameraData()
{
	//������ݵ�ƫ����
	fseek(m_pS3DStream, header.cameraDataPtr, SEEK_SET);

	//�������������
	fread(&(cameraManager.cameraCnt), sizeof(cameraManager.cameraCnt), 1, m_pS3DStream);
	if (cameraManager.cameraCnt == 0)
		return 1;

	//�����������
	for (int i = 0; i< cameraManager.cameraCnt; i++)
	{
		VGS_Camera camera;
		
		//����
		fread(&(camera.strLen), sizeof(camera.strLen), 1, m_pS3DStream);
		camera.name.resize(camera.strLen);
		fread((CHAR*)(camera.name.c_str()), camera.strLen, 1, m_pS3DStream);
		
		//�Ƿ������ͶӰ
		fread(&(camera.isOrtho),sizeof(camera.isOrtho), 1, m_pS3DStream);

		//����֡��
		int frameCnt;
		fread(&frameCnt, sizeof(frameCnt), 1, m_pS3DStream);
		
		//����֡����
		for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
		{
			VGS_CAMERA_INFO camInfo;

			//���������from
			fread(&camInfo, sizeof(camInfo), 1, m_pS3DStream);
			
			//�������
			camera.camInfo.push_back(camInfo);
		}

		//����Camera����
		cameraManager.camera.push_back(camera);
	}
	return 1;
}

//����ƹ�����
int	CGetS3DFileInfo::ReadLightData()
{	
	//�ƹ����ݵ�ƫ����
	fseek(m_pS3DStream, header.lightDataPtr, SEEK_SET);

	//����ƹ�����
	fread(&(lightManager.lightCnt), sizeof(lightManager.lightCnt), 1, m_pS3DStream);
	if (lightManager.lightCnt == 0) return 1;

	//����ƹ�����
	for (int i = 0; i< lightManager.lightCnt; i++)
	{
		VGS_Light light;
		
		fread(&(light.strLen), sizeof(light.strLen), 1, m_pS3DStream);
		light.name.resize(light.strLen + 1);
		fread((CHAR*)(light.name.c_str()), light.strLen, 1, m_pS3DStream);

		//�ƹ�����
		fread(&(light.type), sizeof(light.type), 1, m_pS3DStream);
		
		//����֡��
		int frameCnt;
		fread(&frameCnt, sizeof(frameCnt), 1, m_pS3DStream);

		//д��֡����
		for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
		{
			VGS_LIGHT_INFO lightInfo;
			ZeroMemory(&lightInfo, sizeof(lightInfo));

			//�ƹ���ɫ
			fread(&(lightInfo.color), sizeof(lightInfo.color), 1, m_pS3DStream);
			
			//���ߵ�˥����Χ
			fread(&(lightInfo.range1), sizeof(lightInfo.range1), 1, m_pS3DStream);
			fread(&(lightInfo.range2), sizeof(lightInfo.range2), 1, m_pS3DStream);

			//����ǿ��
			fread(&(lightInfo.multiply), sizeof(lightInfo.multiply), 1, m_pS3DStream);

			//�ڽ���ǣ�ֻ����SpotLightʱ����
			if(light.type == SPOTLIGHT)
			{
				fread(&(lightInfo.theta), sizeof(lightInfo.theta), 1, m_pS3DStream);
				fread(&(lightInfo.phi), sizeof(lightInfo.phi), 1, m_pS3DStream);
			}

			// 3.02����ƹ�����λ�úͷ���
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

		//����ƹ����
		lightManager.light.push_back(light);
	}

	return 1;
}

//�����������
int	CGetS3DFileInfo::ReadSkeletonAnimationData()
{
	//�����������ݵ�ƫ����
	fseek(m_pS3DStream, header.skeletonAnimationDataPtr, SEEK_SET);

	//�����������������
	fread(&(skeletonAnimationManager.skeletonAnimationCnt), sizeof(skeletonAnimationManager.skeletonAnimationCnt), 1, m_pS3DStream);
	if (skeletonAnimationManager.skeletonAnimationCnt == 0) return 1;

	//���������������
	for (int i = 0; i< skeletonAnimationManager.skeletonAnimationCnt; i++)
	{
		VGS_SkeletonAnimation skeletonAnimation;

		//�����������������
		fread(&(skeletonAnimation.strLen), sizeof(skeletonAnimation.strLen), 1, m_pS3DStream);
		skeletonAnimation.name.resize(skeletonAnimation.strLen);
		fread((CHAR*)(skeletonAnimation.name.c_str()), skeletonAnimation.strLen, 1, m_pS3DStream);

		//�������������֡����
		fread(&(skeletonAnimation.frameCnt), sizeof(skeletonAnimation.frameCnt), 1, m_pS3DStream);
		//�����ͷ������
		fread(&(skeletonAnimation.boneCnt), sizeof(skeletonAnimation.boneCnt), 1, m_pS3DStream);

		//�����������֡����
		ProcessSkeletonAnimationStruct(&(skeletonAnimation.boneAnimation), skeletonAnimation.frameCnt);

		//���������������
		skeletonAnimationManager.skeletonAnimation.push_back(skeletonAnimation);
	}

	return 1;
}

//���������������
//------------------------------------------------------------------------------------------
void CGetS3DFileInfo::ProcessSkeletonAnimationStruct(VGS_BoneAnimation* pBoneAnimation, int frameCnt)
{
	//��ͷ������
	fread(&(pBoneAnimation->strLen), sizeof(pBoneAnimation->strLen), 1, m_pS3DStream);
	pBoneAnimation->name.resize(pBoneAnimation->strLen);
	fread((CHAR*)(pBoneAnimation->name.c_str()), pBoneAnimation->strLen, 1, m_pS3DStream);

	//����ؼ�֡֡����
	for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
	{
		NodeTransform transform;
		//��������Ĺؼ�֡����
		fread(&transform, sizeof(transform), 1, m_pS3DStream);
		pBoneAnimation->transform.push_back(transform);
	}

	//��ǰ��ͷ���ӹ�ͷ����
	fread(&pBoneAnimation->childBoneCnt, sizeof(pBoneAnimation->childBoneCnt), 1, m_pS3DStream);

	//��ͷ���ӹ�ͷ
	for(int i = 0; i < pBoneAnimation->childBoneCnt; i++)
	{
		VGS_BoneAnimation childBoneAnimation;
		ProcessSkeletonAnimationStruct(&childBoneAnimation, frameCnt);
		pBoneAnimation->childBoneAnimation.push_back(childBoneAnimation);
	}
}


//����Meshģ������
int	CGetS3DFileInfo::ReadMeshData()
{
	//Mesh���ݵ�ƫ����
	fseek(m_pS3DStream, header.meshDataPtr, SEEK_SET);

	// ����Mesh������
	UINT meshCount = 0;
	READ_FROM_FILE(meshCount, m_pS3DStream);
	if (meshCount == 0) return 1;

	//����Mesh�б�
	m_vMeshData.resize(meshCount);
	for (int meshIndex = 0; meshIndex < meshCount; meshIndex++)
	{
		CVGSMesh &curMesh = m_vMeshData[meshIndex];
		curMesh.faceCnt = 0;

		// ����
		int nameLen = 0;
		READ_FROM_FILE(nameLen, m_pS3DStream);
		char sName[MAX_FILENAME_LENGTH + 1];
		_ZeroMemory(sName);
		fread(sName, nameLen, 1, m_pS3DStream);
		curMesh.sName = std::string(sName);

		// ���붥������
		fread(&(curMesh.vertexCnt), sizeof(curMesh.vertexCnt), 1, m_pS3DStream);

		//���붥���б�
		curMesh.fvf = FVF_POSNORMALCOLORCOORD4;
		curMesh.pVertex = new CVertex_POSNORMALCOLORCOORD4[curMesh.vertexCnt];  // ����buffer

		BYTE uvFlag;
		for (int j = 0; j < curMesh.vertexCnt; j++)
		{
			CVertex_POSNORMALCOLORCOORD4 *pVertex = &(((CVertex_POSNORMALCOLORCOORD4*)(curMesh.pVertex))[j]);
			
			// λ��, max������������ϵ������y������
			fread(&(pVertex->position), sizeof(pVertex->position), 1, m_pS3DStream);
			float y = pVertex->position.y;
			pVertex->position.y = pVertex->position.z;
			pVertex->position.z = y;

			// ����, max������������ϵ������y������
			fread(&(pVertex->normal), sizeof(pVertex->normal), 1, m_pS3DStream);
			y = pVertex->normal.y;
			pVertex->normal.y = pVertex->normal.z;
			pVertex->normal.z = y;

			//��ɫ
			fread(&(pVertex->color), sizeof(pVertex->color), 1, m_pS3DStream);

			//UV Flag
			fread(&uvFlag, sizeof(uvFlag), 1, m_pS3DStream);
			
			int bUV1 = uvFlag & 0x01;
			int bUV2 = uvFlag & 0x02;
			int bUV3 = uvFlag & 0x04;
			int bUV4 = uvFlag & 0x08;
			//UV1����
			if (bUV1)
			{
				fread(&(pVertex->uv1), sizeof(pVertex->uv1), 1, m_pS3DStream);
				pVertex->uv1.v = 1.0f - pVertex->uv1.v;  // max��uv����ϵ�����½�,���������Ͻ�
			}

			//UV2����
			if (bUV2)
			{
				fread(&(pVertex->uv2), sizeof(pVertex->uv2), 1, m_pS3DStream);
				pVertex->uv2.v = 1.0f - pVertex->uv2.v;  // max��uv����ϵ�����½�,���������Ͻ�
			}

			//UV3����
			if (bUV3)
			{
				fread(&(pVertex->uv3), sizeof(pVertex->uv3), 1, m_pS3DStream);
				pVertex->uv3.v = 1.0f - pVertex->uv3.v;  // max��uv����ϵ�����½�,���������Ͻ�
			}

			//UV4����
			if (bUV4)
			{
				fread(&(pVertex->uv4), sizeof(pVertex->uv4), 1, m_pS3DStream);
				pVertex->uv4.v = 1.0f - pVertex->uv4.v;  // max��uv����ϵ�����½�,���������Ͻ�
			}

		} // for j


		//��Χ��
		VGS_BoundBox boundBox;
		fread(&(boundBox), sizeof(boundBox), 1, m_pS3DStream);

		//����SubMesh����
		READ_FROM_FILE(curMesh.subMeshCount, m_pS3DStream);

		//����SubMesh����
		curMesh.pSubMesh = new CVGSSubMesh[curMesh.subMeshCount];
		for (int subMeshIndex = 0; subMeshIndex < curMesh.subMeshCount; subMeshIndex++)
		{
			CVGSSubMesh &subMesh = ((CVGSSubMesh*)(curMesh.pSubMesh))[subMeshIndex];
			
			//����SubMesh�Ĳ��ʱ��
			READ_FROM_FILE(subMesh.mtrIndex, m_pS3DStream);

			//����SubMesh�����
			READ_FROM_FILE(subMesh.faceCount, m_pS3DStream);

			//pMesh��������
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

		// �󶨵Ĺ�����
		READ_FROM_FILE(curMesh.SkeletonAnimationIndex, m_pS3DStream);

		//�汾�ж�
		if (header.s3dVersion == 3.03f)
		{
			if (curMesh.SkeletonAnimationIndex != -1)
			{
				//����������
				fread(&curMesh.faceCnt, sizeof(curMesh.faceCnt), 1, m_pS3DStream);

				//����󶨵Ĺ�������
				fread(&(curMesh.boneCnt), sizeof(curMesh.boneCnt), 1, m_pS3DStream);

				//�ݹ������Ƥ����
				ReadSkeletonAndSkinData(&curMesh.bone);

				//�����������ͷ��Mesh����ϵ�еı任
				fread(&(curMesh.skeletontransform), sizeof(curMesh.skeletontransform), 1, m_pS3DStream);
			}
		}

	} // for meshIndex

	return 1;
}

//------------------------------------------------------------------------------------------
//�ݹ������Ƥ����
//------------------------------------------------------------------------------------------
void CGetS3DFileInfo::ReadSkeletonAndSkinData(VGS_Bone* pVGSBone)
{
	//�ݹ����VGS_Bone����

	//�������ͷ������
	fread(&pVGSBone->strLen, sizeof(pVGSBone->strLen), 1, m_pS3DStream);
	pVGSBone->name.resize(pVGSBone->strLen);
	fread((CHAR*)(pVGSBone->name.c_str()), pVGSBone->strLen, 1, m_pS3DStream);

	//�����ͷ�ĳ�ʼ��̬
	fread(&pVGSBone->startPose, sizeof(pVGSBone->startPose), 1, m_pS3DStream);

	//�����ͷ�󶨵Ķ�������
	fread(&pVGSBone->bindingVertexCnt, sizeof(pVGSBone->bindingVertexCnt), 1, m_pS3DStream);

	//����󶨵Ķ������б�
	//����󶨵Ķ���Ȩ���б�
	if (pVGSBone->bindingVertexCnt > 0)
	{
		pVGSBone->bindingVertexIndex.resize(pVGSBone->bindingVertexCnt);
		fread(&(pVGSBone->bindingVertexIndex[0]), sizeof(DWORD) * pVGSBone->bindingVertexCnt, 1, m_pS3DStream);
		pVGSBone->bindingWeight.resize(pVGSBone->bindingVertexCnt);
		fread(&(pVGSBone->bindingWeight[0]), sizeof(FLOAT) * pVGSBone->bindingVertexCnt, 1, m_pS3DStream);
	}

	//�����ӹ�ͷ��������
	fread(&pVGSBone->childBoneCnt, sizeof(pVGSBone->childBoneCnt), 1, m_pS3DStream);

	//�ݹ��ͷ���ӹ�ͷ
	for (int i = 0; i<pVGSBone->childBoneCnt; i++)
	{
		VGS_Bone childVGSBone;
		ReadSkeletonAndSkinData(&childVGSBone);
		pVGSBone->childBone.push_back(childVGSBone);
	}
}

//------------------------------------------------------------------------------------------
//����Shape����
int	CGetS3DFileInfo::ReadShapeData()
{
	return 1;
}

//���볡��ͼ
int	CGetS3DFileInfo::ReadSceneGraphData()
{
	//����ͼ���ݵ�ƫ����
	fseek(m_pS3DStream, header.sceneGraphDataPtr, SEEK_SET);

	//���볡���ڵ�����
	fread(&(sceneNodeGraph.nodeCnt), sizeof(sceneNodeGraph.nodeCnt), 1, m_pS3DStream);

	//���첢���볡��ͼ�ṹ
	ConstructSceneGraph(&(sceneNodeGraph.node));

	return 1;
}

//------------------------------------------------------------------------------------------
//���쳡��ͼ����
//------------------------------------------------------------------------------------------
void CGetS3DFileInfo::ConstructSceneGraph(VGS_Node* pNode)
{
	//node������
	fread(&(pNode->type), sizeof(pNode->type), 1, m_pS3DStream);
	
	//node�ı��
	fread(&(pNode->index), sizeof(pNode->index), 1, m_pS3DStream);

	//node������
	fread(&(pNode->strLen), sizeof(pNode->strLen), 1, m_pS3DStream);
	pNode->name.reserve(pNode->strLen);
	fread((void*)(pNode->name.c_str()), pNode->strLen, 1, m_pS3DStream);

	//���붯��֡��
	int frameCnt = 1;
	fread(&frameCnt, sizeof(frameCnt), 1, m_pS3DStream);

	//д��֡����
	for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
	{
		NodeTransform transform;
		fread(&transform, sizeof(transform), 1, m_pS3DStream);
		//�ؼ�֡��������
		pNode->keyframeTransform.push_back(transform);
	}

	//���������
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


//�ͷ���Դ
void CGetS3DFileInfo::Release()
{
	//ɾ����������
	mtrlManager.mtrl.clear();
	//ɾ�����
	cameraManager.camera.clear();
	//ɾ���ƹ�
	lightManager.light.clear();
	//ɾ��2Dͼ��
	shapeManager.shape.clear();

	//ɾ��ģ�ͺ���Ƥ����
	ReleaseMeshAndSkinManager();

	//�ͷŹ�����������
	ReleaseSkeletonAnimationManager();

	//ɾ�������ṹͼ
	ReleaseNodeRecursion(sceneNodeGraph.node);
	//ɾ����ͼ
}

//�ͷŹ����������ݹ������ SkeletonAnimationManager
void CGetS3DFileInfo::ReleaseSkeletonAnimationManager()
{
	for (int i = 0; i< skeletonAnimationManager.skeletonAnimationCnt; i++)
	{
		VGS_SkeletonAnimation* pSkeletonAnimation = &(skeletonAnimationManager.skeletonAnimation[i]);
		//�ݹ��ͷŹ�����������
		ReleaseBoneAnimationRecursion(&pSkeletonAnimation->boneAnimation);
	}
	skeletonAnimationManager.skeletonAnimation.clear();
}

//�ݹ��ͷŹ�����������
void CGetS3DFileInfo::ReleaseBoneAnimationRecursion(VGS_BoneAnimation* pBoneAnimation)
{
	pBoneAnimation->name.clear();
	pBoneAnimation->transform.clear();
	for (int i = 0; i< pBoneAnimation->childBoneCnt; i++)
		ReleaseBoneAnimationRecursion(&(pBoneAnimation->childBoneAnimation[i]));
	pBoneAnimation->childBoneAnimation.clear();
}

//ɾ��ģ�ͺ���Ƥ����
void CGetS3DFileInfo::ReleaseMeshAndSkinManager()
{
	//for (int i = 0; i< meshManager.meshCnt; i++)
	//{
	//	VGS_Mesh* pMesh = &(meshManager.mesh[i]);
	//	//�ͷ�Mesh�ĵ���Ƥ����
	//	if (pMesh->SkeletonAnimationIndex != -1)
	//		ReleaseMeshAndSkinRecursion(&pMesh->bone);
	//}
}

//�ݹ��ͷ�ģ�ͺ���Ƥ����
void CGetS3DFileInfo::ReleaseMeshAndSkinRecursion(VGS_Bone* pBone)
{
	pBone->name.clear();
	pBone->bindingVertexIndex.clear();
	pBone->bindingWeight.clear();
	for (int i = 0; i< pBone->childBoneCnt; i++)
		ReleaseMeshAndSkinRecursion(&(pBone->childBone[i]));
}


//ɾ������ͼ
void CGetS3DFileInfo::ReleaseNodeRecursion(VGS_Node& node)
{
	node.name = "";
	node.keyframeTransform.clear();
	int childNodeCnt = node.childNode.size();
	for (int i = 0; i< childNodeCnt; i++)
		ReleaseNodeRecursion(node.childNode[i]);
	node.childNode.clear();
}

//ɾ����ͼ
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


