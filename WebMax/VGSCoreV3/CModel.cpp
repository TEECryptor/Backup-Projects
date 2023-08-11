/////////////////////////////////////////////////////////////////////////
// 文件名: CModel.h: interface for the CModel class.
// 类  名: CModel
// 描  述: 基本Model类
// 作  者: lixin
// 日  期: 2006-04-30
/////////////////////////////////////////////////////////////////////////

#include "CModel.h"
#include <algorithm>
#include "CSysFunc.h"

#include "Axis.h"
#include "AxisMove.h"
#include "AxisRotate.h"
#include "AxisScale.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModel::CModel(LPDIRECT3DDEVICE9 pd3dDevice)
	:m_pAxis(NULL)
	,m_pAxisMove(NULL)
	,m_pAxisRotate(NULL)
	,m_pAxisScale(NULL)
	,m_bShowAxis(false)
	,m_hHasRenderAxis(false)
	,m_nAxisIndex(-1)
{
	m_type = VGS_MOVABLETYPE_MODEL;

	m_pd3dDevice = pd3dDevice;
	pCloneModel = NULL;

	m_VBuffer = NULL;
//	m_IBuffer = NULL;

	m_pVertices =NULL;
//	m_pIndices = NULL;

//	oriShader = NULL;
//	shader = NULL;
//	mouseOverShader = NULL;
//	mouseClickedShader = NULL;
//	mouseUpShader = NULL;

//	vertices_kfr = NULL;
//	matrix_kfr = NULL;
		
	vertex = NULL;
//	face = NULL;
	
	opacity = 100;
	oriOpacity = 100;
	kfrCnt = 0;			
	kfrCurFrame = 0;
	currentTimer = 0;
	kfrFrameTime = 40;
	kfrPlay = TRUE;		
	kfrRepeatCnt = 1;
	kfrRepeatIndex = 0;	//从0开始计数，第1次播放
	
//	vkfrCnt = 0;
//	vkfrPlay = TRUE;
//	vkfrRepeatCnt = 1;	
//	vkfrRepeatIndex = 0;
	startFrame = 0;	
	endFrame = 0;	

	user_moveKFR = NULL;			//用户自定义模型帧变换的逐帧变换矢量
	user_moveFrameCnt = 0;		//用户自定义模型帧变换矩阵帧数
	user_moveCurFrame = 0;
	bMatrixChanged = FALSE;

//	DifPosVertexCnt = 0;
//	DifNormalVertexCnt = 0;
//	DifUVVertexCnt = 0;
//	DifPosVertexCnt = 0;
//	DifUVVertexCnt = 0;	

//	worldPosition = D3DXVECTOR3(0,0,0);
//	worldRotation = D3DXVECTOR3(0,0,0);
//	worldScale = D3DXVECTOR3(1,1,1);
	//初始化模型颜色为白色
    oriColor =    D3DCOLOR_RGBA(255, 255, 255, 255);
	modelColor = D3DCOLOR_RGBA(255, 255, 255, 255);
	bModelColor = FALSE;
	isBillboard = FALSE;
	isCollision = FALSE;
	clickable = FALSE;
	mouseInOutable = FALSE;
	isGround = FALSE;
	bSelected = FALSE;
	colDetectDistance = 50.0f;
	culled = false;

	bDynamicUV = FALSE;
	//UStep = 0.0f;
	//VStep = 0.0f;

	isSkyBox = FALSE;
	bUseFBL = TRUE;
	bMouseShader = FALSE;
	bMouseColor = FALSE;

	index = -1;

	isZOne = FALSE;

	ID1 = 0;
	ID2 = 0;

	modelGroup = 0;
	bmouseHintByGroup = FALSE;

	pRBBox = NULL;             //包围盒模型对象
	showRBBox = FALSE;          //是否显示包围盒模型

//	IBufferFormat = D3DFMT_INDEX16;

	D3DXMatrixIdentity(&m_matIdentity);

	//是否显示反射标志
    bReflect = FALSE;
    //该物体要反射的信息索引，如果没有反射 则为-1
	sceneRefInfoListIdx = -1;
    
	refedModelIdxList.clear();  

	m_pNode = 0; //new CSceneNode();

	m_SkeletonIndex = -1;

	//--------------------------------------------------------------------------
	//new
	//--------------------------------------------------------------------------
	m_pVGSMesh = NULL;
	m_pMesh = NULL;
	m_pSkinMesh = NULL;
	m_pSkinInfo = NULL;
	m_pSkeleton = NULL;

	oriBBox.minCorner = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	oriBBox.maxCorner = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	bDynamicUV = false;
	bReflect = false;
	bSelected = false;
	isCollision = false;
	isGround = false;
	visible = true;
}

CModel::~CModel()
{
	DestroyAxis();
}
//初始化
HRESULT CModel::Init()
{
	//创建D3DXMESH的选项
	m_optionXMesh = D3DXMESH_32BIT | D3DXMESH_MANAGED;	

	SAFE_RELEASE_D3D(m_pMesh);

	//创建D3DXMESH对象
	HRESULT hr = D3DXCreateMeshFVF(m_faceCount,			//Mesh包含的面数
		m_vertexCount,		//顶点数
		m_optionXMesh,				//创建D3DXMESH的一些选项，譬如顶点缓冲是在硬件还是Manage
		m_FVF,				//顶点声明，前面已经说过了
		m_pd3dDevice, 				//D3D设备
		&m_pMesh);					//返回LPD3DXMESH的指针

	return hr;
}

//写入顶点缓冲数据
HRESULT CModel::FillVertexBuffer(void* pVertexSrcData)
{
	if (!pVertexSrcData)
	{
		return S_FALSE;
	}

	// 顶点数据buffer
	VOID* pVertexData = NULL;
	HRESULT hr = LockVertexBuffer(&pVertexData, D3DLOCK_DISCARD);
	if (hr != D3D_OK)
	{
		return hr;
	}

	memcpy(pVertexData, pVertexSrcData, m_FVFSize * m_vertexCount);  // 设置顶点缓冲

	//计算BoundBox
	D3DXComputeBoundingBox((D3DXVECTOR3*)pVertexData, m_vertexCount, m_FVFSize, 
		&(oriBBox.minCorner), &(oriBBox.maxCorner));

	//计算包围球
	D3DXComputeBoundingSphere((D3DXVECTOR3*)pVertexData, m_vertexCount, m_FVFSize, 
		&(oriBSphere.Center), &(oriBSphere.Radius));

	UnlockVertexBuffer();

	return D3D_OK;
}

//写入顶点缓冲数据
HRESULT CModel::FillVertexBuffer()
{
	if (!m_pVGSMesh || !m_pMesh)
	{
		return S_FALSE;
	}

	// 顶点数据buffer
	VOID* pVertexData = NULL;
	// HRESULT hr = m_pMesh->LockVertexBuffer(D3DLOCK_DISCARD, &pVertexData);
	HRESULT hr = LockVertexBuffer(&pVertexData, D3DLOCK_DISCARD);
	if (hr != D3D_OK)
	{
		return hr;
	}

	switch (m_FVF)
	{
	case FVF_POSNORMALCOLORCOORD1:
	{
		CVertex_POSNORMALCOLORCOORD1 *pVGSVertexData = (CVertex_POSNORMALCOLORCOORD1*)(m_pVGSMesh->pVertex);
		for (DWORD i = 0; i < m_vertexCount; i++)
		{
			((VERTEXDECL_POSNORMALCOLORCOORD1*)pVertexData)[i].position.x = pVGSVertexData[i].position.x;
			((VERTEXDECL_POSNORMALCOLORCOORD1*)pVertexData)[i].position.y = pVGSVertexData[i].position.y;
			((VERTEXDECL_POSNORMALCOLORCOORD1*)pVertexData)[i].position.z = pVGSVertexData[i].position.z;

			((VERTEXDECL_POSNORMALCOLORCOORD1*)pVertexData)[i].normal.x = pVGSVertexData[i].normal.x;
			((VERTEXDECL_POSNORMALCOLORCOORD1*)pVertexData)[i].normal.y = pVGSVertexData[i].normal.y;
			((VERTEXDECL_POSNORMALCOLORCOORD1*)pVertexData)[i].normal.z = pVGSVertexData[i].normal.z;

			((VERTEXDECL_POSNORMALCOLORCOORD1*)pVertexData)[i].su = pVGSVertexData[i].uv1.u;
			((VERTEXDECL_POSNORMALCOLORCOORD1*)pVertexData)[i].sv = pVGSVertexData[i].uv1.v;
		}
		break;
	}
	case FVF_POSNORMALCOLORCOORD2:
	{
		CVertex_POSNORMALCOLORCOORD2 *pVGSVertexData = (CVertex_POSNORMALCOLORCOORD2*)(m_pVGSMesh->pVertex);
		for (DWORD i = 0; i < m_vertexCount; i++)
		{
			((VERTEXDECL_POSNORMALCOLORCOORD2*)pVertexData)[i].position.x = pVGSVertexData[i].position.x;
			((VERTEXDECL_POSNORMALCOLORCOORD2*)pVertexData)[i].position.y = pVGSVertexData[i].position.y;
			((VERTEXDECL_POSNORMALCOLORCOORD2*)pVertexData)[i].position.z = pVGSVertexData[i].position.z;

			((VERTEXDECL_POSNORMALCOLORCOORD2*)pVertexData)[i].normal.x = pVGSVertexData[i].normal.x;
			((VERTEXDECL_POSNORMALCOLORCOORD2*)pVertexData)[i].normal.y = pVGSVertexData[i].normal.y;
			((VERTEXDECL_POSNORMALCOLORCOORD2*)pVertexData)[i].normal.z = pVGSVertexData[i].normal.z;

			((VERTEXDECL_POSNORMALCOLORCOORD2*)pVertexData)[i].su1 = pVGSVertexData[i].uv1.u;
			((VERTEXDECL_POSNORMALCOLORCOORD2*)pVertexData)[i].sv1 = pVGSVertexData[i].uv1.v;

			((VERTEXDECL_POSNORMALCOLORCOORD2*)pVertexData)[i].su2 = pVGSVertexData[i].uv2.u;
			((VERTEXDECL_POSNORMALCOLORCOORD2*)pVertexData)[i].sv2 = pVGSVertexData[i].uv2.v;
		}
		break;
	}
	case FVF_POSNORMALCOLORCOORD3:
	{
		CVertex_POSNORMALCOLORCOORD3 *pVGSVertexData = (CVertex_POSNORMALCOLORCOORD3*)(m_pVGSMesh->pVertex);
		for (DWORD i = 0; i < m_vertexCount; i++)
		{
			((VERTEXDECL_POSNORMALCOLORCOORD3*)pVertexData)[i].position.x = pVGSVertexData[i].position.x;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)pVertexData)[i].position.y = pVGSVertexData[i].position.y;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)pVertexData)[i].position.z = pVGSVertexData[i].position.z;

			((VERTEXDECL_POSNORMALCOLORCOORD3*)pVertexData)[i].normal.x = pVGSVertexData[i].normal.x;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)pVertexData)[i].normal.y = pVGSVertexData[i].normal.y;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)pVertexData)[i].normal.z = pVGSVertexData[i].normal.z;

			((VERTEXDECL_POSNORMALCOLORCOORD3*)pVertexData)[i].su1 = pVGSVertexData[i].uv1.u;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)pVertexData)[i].sv1 = pVGSVertexData[i].uv1.v;

			((VERTEXDECL_POSNORMALCOLORCOORD3*)pVertexData)[i].su2 = pVGSVertexData[i].uv2.u;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)pVertexData)[i].sv2 = pVGSVertexData[i].uv2.v;

			((VERTEXDECL_POSNORMALCOLORCOORD3*)pVertexData)[i].su3 = pVGSVertexData[i].uv3.u;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)pVertexData)[i].sv3 = pVGSVertexData[i].uv3.v;
		}
		break;
	}
	case FVF_POSNORMALCOLORCOORD4:
	{
		CVertex_POSNORMALCOLORCOORD4 *pVGSVertexData = (CVertex_POSNORMALCOLORCOORD4*)(m_pVGSMesh->pVertex);
		for (DWORD i = 0; i < m_vertexCount; i++)
		{
			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].position.x = pVGSVertexData[i].position.x;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].position.y = pVGSVertexData[i].position.y;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].position.z = pVGSVertexData[i].position.z;

			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].normal.x = pVGSVertexData[i].normal.x;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].normal.y = pVGSVertexData[i].normal.y;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].normal.z = pVGSVertexData[i].normal.z;

			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].su1 = pVGSVertexData[i].uv1.u;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].sv1 = pVGSVertexData[i].uv1.v;

			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].su2 = pVGSVertexData[i].uv2.u;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].sv2 = pVGSVertexData[i].uv2.v;

			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].su3 = pVGSVertexData[i].uv3.u;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].sv3 = pVGSVertexData[i].uv3.v;

			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].su4 = pVGSVertexData[i].uv4.u;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)pVertexData)[i].sv4 = pVGSVertexData[i].uv4.v;
		}
		break;
	}
	default:
		break;
	}
	
	//计算BoundBox
	D3DXComputeBoundingBox((D3DXVECTOR3*)pVertexData, m_vertexCount, m_FVFSize, 
							&(oriBBox.minCorner), &(oriBBox.maxCorner));
	boundingBox.minCorner = oriBBox.minCorner;
	boundingBox.maxCorner = oriBBox.maxCorner;

	//计算包围球
	D3DXComputeBoundingSphere((D3DXVECTOR3*)pVertexData, m_vertexCount, m_FVFSize, 
							&(oriBSphere.Center), &(oriBSphere.Radius));
	boundingSphere.Center = oriBSphere.Center;
	boundingSphere.Radius = oriBSphere.Radius;

	// m_pMesh->UnlockVertexBuffer();
	UnlockVertexBuffer();

	return S_OK;
}

//锁定并填充模型的索引缓冲
HRESULT CModel::FillIndexBuffer(void* pIndexBuf)
{
	DWORD* pIndexData = NULL;
	LockIndexBuffer((void**)&pIndexData, D3DLOCK_DISCARD);

	// index buffer的长度
	DWORD indexSize = m_faceCount * 3 * sizeof(DWORD);
	memcpy((void*)pIndexData, pIndexBuf, indexSize);  // 设置index索引数据

	UnlockIndexBuffer();
	return S_OK;
}

//锁定并填充模型的索引缓冲
HRESULT CModel::FillIndexBuffer(vector<CMaterial*> vpShader)
{
	if (!m_pVGSMesh || !m_pMesh)
	{
		return S_FALSE;
	}

	// m_vpMtrl.clear();
	m_vSubModel.clear();
	m_vSubModel.resize(m_pVGSMesh->subMeshCount);

	UINT faceIndex = 0;
	DWORD* pIndexData = NULL;
	LockIndexBuffer((VOID**)&pIndexData, D3DLOCK_DISCARD);
	// m_pMesh->LockIndexBuffer(D3DLOCK_DISCARD, (VOID**)&pIndexData);
	for (DWORD subMeshIndex = 0; subMeshIndex < m_pVGSMesh->subMeshCount; subMeshIndex++)
	{
		CVGSSubMesh &curVGS_SubMesh = ((CVGSSubMesh*)(m_pVGSMesh->pSubMesh))[subMeshIndex];

		//逐个面填入索引缓冲数据
		for (DWORD i = 0; i < curVGS_SubMesh.faceCount; i++)
		{
			pIndexData[3*(faceIndex+i)]     = curVGS_SubMesh.pFace[i].a;
			pIndexData[3*(faceIndex+i) + 1] = curVGS_SubMesh.pFace[i].b;
			pIndexData[3*(faceIndex+i) + 2] = curVGS_SubMesh.pFace[i].c;
		}

		faceIndex += curVGS_SubMesh.faceCount;

//#if _DEBUG
//		DWORD tmp[20000];
//		memcpy((void*)tmp, (void*)pIndexData, min(20000 * sizeof(DWORD), sizeof(DWORD) * 3 * curVGS_SubMesh.faceCount));
//#endif

		//材质队列
		// m_vpMtrl.push_back(vpShader[curVGS_SubMesh.mtrIndex]);
		if (curVGS_SubMesh.mtrIndex >= 0 && curVGS_SubMesh.mtrIndex < (int)vpShader.size())
		{
			m_vSubModel[subMeshIndex].m_pMaterial = vpShader[curVGS_SubMesh.mtrIndex];
		}
		else
		{
			m_vSubModel[subMeshIndex].m_pMaterial = 0;
		}
	}
	
	UnlockIndexBuffer();
	// m_pMesh->UnlockIndexBuffer();

	return S_OK;
}

//写入属性表
HRESULT CModel::FillAttributeTable(vector<CSubModel> &vSubmodel)
{
	DWORD* pAttributeBuffer = NULL;
	m_pMesh->LockAttributeBuffer(D3DLOCK_DISCARD, &pAttributeBuffer);

	for (int i = 0; i < (int)vSubmodel.size(); ++i)
	{
		CSubModel &submodel = vSubmodel[i];
		UINT iface = 0; //submodel.m_faceStart;
		while (iface < submodel.m_faceCount)
		{
			pAttributeBuffer[(submodel.m_faceStart + iface)] = (DWORD)submodel.m_pMaterial;   // 填充每个面的属性ID,也就是材质指针
			++iface;
		}

		// 将sunmesn记录到材质
		VGS_RenderSubmesh rsm;
		rsm.modelPointer = (DWORD)this;
		rsm.subMeshID = i;
		submodel.m_pMaterial->AddSubmeshToList(rsm);
	}

	m_pMesh->UnlockAttributeBuffer();

	//优化模型，不优化属性表无法通过GetAttributeTable获得列表
	std::vector<DWORD> adjacencyBuffer(m_pMesh->GetNumFaces() * 3);
	HRESULT hr = m_pMesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE | D3DXMESHOPT_DONOTSPLIT,
		&adjacencyBuffer[0], 0, 0, 0);

	// 更新模型的顶点和面数
	m_faceCount = m_pMesh->GetNumFaces();
	m_vertexCount = m_pMesh->GetNumVertices();

	// 更新每个submodel的信息
	generateSubmeshIndexData();	

	return D3D_OK;
}

//写入属性表
HRESULT CModel::FillAttributeTable()
{
	if (!m_pVGSMesh || !m_pMesh)
	{
		return S_FALSE;
	}

	DWORD faceIndex = 0;

	DWORD* pAttributeBuffer = NULL;
	m_pMesh->LockAttributeBuffer(D3DLOCK_DISCARD, &pAttributeBuffer);

	for (DWORD subMeshIndex = 0; subMeshIndex < m_pVGSMesh->subMeshCount; subMeshIndex++)
	{
		CVGSSubMesh* pVGS_SubMesh = &(((CVGSSubMesh*)(m_pVGSMesh->pSubMesh))[subMeshIndex]);

		//逐个面填入属性表缓冲数据
		for (DWORD i = 0; i < pVGS_SubMesh->faceCount; i++)
		{
			// 用material的index将face分组,执行Optimize后，将会按照材质划分submesh
			pAttributeBuffer[faceIndex + i] = (DWORD)m_vSubModel[subMeshIndex].m_pMaterial;// pVGS_SubMesh->mtrIndex;   
		}
		faceIndex += pVGS_SubMesh->faceCount;


		// 将sunmesn记录到材质
		VGS_RenderSubmesh rsm;
		rsm.modelPointer = (DWORD)this;
		rsm.subMeshID = subMeshIndex;
		m_vSubModel[subMeshIndex].m_pMaterial->AddSubmeshToList(rsm);

	}
	m_pMesh->UnlockAttributeBuffer();

	//优化模型，不优化属性表无法通过GetAttributeTable获得列表
	std::vector<DWORD> adjacencyBuffer(m_pMesh->GetNumFaces() * 3);
	m_pMesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]);
	HRESULT hr = m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE | D3DXMESHOPT_DONOTSPLIT,
										  &adjacencyBuffer[0], 0, 0, 0);

	// 更新模型的顶点和面数
	m_faceCount = m_pMesh->GetNumFaces();
	m_vertexCount = m_pMesh->GetNumVertices();

	// 更新每个submodel的信息
	generateSubmeshIndexData();	

	//for test
	/*DWORD numEntries = 0;
	m_pMesh->GetAttributeTable(0, &numEntries);
	vector<D3DXATTRIBUTERANGE> table(numEntries);
	m_pMesh->GetAttributeTable(&table[0], &numEntries);*/


	return S_OK;
}

//填充SkinInfo蒙皮对象并绑定骨骼数据
HRESULT CModel::CreateSkeletonAndSkin()
{
	//没有骨骼蒙皮绑定
	if (m_pVGSMesh->SkeletonAnimationIndex  == -1) return S_OK;

	//创建骨架对象
	m_pSkeleton = new CSkeleton();

	//创建骨骼蒙皮对象
	HRESULT hr = D3DXCreateSkinInfoFVF(m_pVGSMesh->vertexCnt, m_FVF, m_pVGSMesh->boneCnt, &m_pSkinInfo);

	//填充骨骼层次结构
	DWORD boneIndex = 0;
	CBone* pRootBone = CreateSkeletonHierarchy(&m_pVGSMesh->bone, boneIndex);

	//设置根骨骼
	m_pSkeleton->setRootBone(pRootBone);

	return hr;
}

//填充骨骼层次结构
CBone* CModel::CreateSkeletonHierarchy(VGS_Bone* pVGSBone, DWORD boneIndex)
{
	//创建一个新的骨骼
	CBone* pBone = new CBone();

	//骨骼名字
	pBone->name = pVGSBone->name;

	//骨骼初始姿态
	D3DXVECTOR3 scale = D3DXVECTOR3(pVGSBone->startPose.scale.x, 
									pVGSBone->startPose.scale.y, 
									pVGSBone->startPose.scale.z);
	D3DXQUATERNION quat = D3DXQUATERNION(pVGSBone->startPose.quat.x, 
										pVGSBone->startPose.quat.y, 
										pVGSBone->startPose.quat.z, 
										pVGSBone->startPose.quat.w);
	D3DXVECTOR3 position = D3DXVECTOR3(pVGSBone->startPose.position.x, 
										pVGSBone->startPose.position.y, 
										pVGSBone->startPose.position.z);
	D3DXMatrixTransformation(&pBone->startPoseMat, 
							&D3DXVECTOR3(0,0,0), 
							&D3DXQUATERNION(0, 0, 0, 0), 
							&scale, 
							&D3DXVECTOR3(0,0,0), 
							&quat, 
							&position);

	//设置骨骼影响的顶点及权重
	if (pVGSBone->bindingVertexCnt > 0)
	{
		m_pSkinInfo->SetBoneInfluence(boneIndex, pVGSBone->bindingVertexCnt, 
										&pVGSBone->bindingVertexIndex[0], 
										&pVGSBone->bindingWeight[0]);
	}

	boneIndex++;

	//骨骼的孩子
	for (DWORD i = 0; i< pVGSBone->childBone.size(); i++)
	{
		CBone* pChildBone = CreateSkeletonHierarchy(&(pVGSBone->childBone[i]), boneIndex);
		pBone->child.push_back(pChildBone);
	}

	return pBone;
}

//创建模型
HRESULT	CModel::Create(CVGSMesh* pVGSMesh, vector<CMaterial*> vpShader)
{
	if (!pVGSMesh)
	{
		return S_FALSE;
	}

	m_pVGSMesh = pVGSMesh;

	//每个顶点所占的空间大小
	m_FVF = m_pVGSMesh->fvf;
	m_FVFSize = D3DXGetFVFVertexSize(m_FVF);
	m_vertexCount = m_pVGSMesh->vertexCnt;
	m_faceCount   = m_pVGSMesh->faceCnt;

	//顶点声明
	/*int bUV1 = m_pVGSMesh [0].uvFlag & 0x01;
	int bUV2 = m_pVGSMesh->vertex[0].uvFlag & 0x02;
	int bUV3 = m_pVGSMesh->vertex[0].uvFlag & 0x04;
	int bUV4 = m_pVGSMesh->vertex[0].uvFlag & 0x08;

	if (bUV1 && !bUV2) 	m_FVF = FVF_POSNORMALCOLORCOORD1;
	if (bUV1 && bUV2 && !bUV3) m_FVF = FVF_POSNORMALCOLORCOORD2;
	if (bUV1 && bUV2 && bUV3 && !bUV4) m_FVF = FVF_POSNORMALCOLORCOORD3;
	if (bUV1 && bUV2 && bUV3 && bUV4) m_FVF = FVF_POSNORMALCOLORCOORD4; */

	//初始化
	Init();

	//向Mesh的顶点缓冲写入数据
	FillVertexBuffer();

	//向Mesh的索引缓冲写入数据
	FillIndexBuffer(vpShader);

	//定义Mesh的属性表
	FillAttributeTable();

	//创建骨骼和蒙皮对象
	CreateSkeletonAndSkin();

	m_pVGSMesh = NULL;

	return S_OK;
}

// 创建模型
HRESULT CModel::Create(DWORD fvf, DWORD vertexCount, DWORD faceCount, void* pVertextBuf, void *pIndexBuf, vector<CSubModel> &vSubModel)
{
	m_FVF = fvf;
	m_FVFSize = D3DXGetFVFVertexSize(m_FVF);
	m_vertexCount = vertexCount;
	m_faceCount   = faceCount;

	Init();
	FillVertexBuffer(pVertextBuf);
	FillIndexBuffer(pIndexBuf);
	FillAttributeTable(vSubModel);

//============>
	// 顶点数据buffer
	//VOID* pVertexData = NULL;
	//HRESULT hr = LockVertexBuffer(&pVertexData, D3DLOCK_READONLY);
	//if (hr != D3D_OK)
	//{
	//	return hr;
	//}

	////计算BoundBox
	//D3DXComputeBoundingBox((D3DXVECTOR3*)pVertexData, m_vertexCount, m_FVFSize, 
	//	&(oriBBox.minCorner), &(oriBBox.maxCorner));

	//////计算包围球
	//D3DXComputeBoundingSphere((D3DXVECTOR3*)pVertexData, m_vertexCount, m_FVFSize, 
	//	&(oriBSphere.Center), &(oriBSphere.Radius));

	//UnlockVertexBuffer();
// <=============

	return D3D_OK;
}

// 创建submesh的index数据
void CModel::generateSubmeshIndexData()
{
	if (!m_pMesh)
	{
		return;
	}

	DWORD subModelCount = 0;
	m_pMesh->GetAttributeTable(0, &subModelCount);
	vector<D3DXATTRIBUTERANGE> table(subModelCount);
	m_pMesh->GetAttributeTable(&table[0], &subModelCount);

	m_vSubModel.resize(subModelCount);
	for (UINT i = 0; i < subModelCount; ++i)
	{
		CSubModel &subModel = m_vSubModel[i];
		subModel.m_pMaterial = (CMaterial*)(table[i].AttribId);
		subModel.m_vertexStart = table[i].VertexStart;
		subModel.m_vertexCount = table[i].VertexCount;
		subModel.m_faceStart  = table[i].FaceStart;
		subModel.m_faceCount  = table[i].FaceCount;
	}

//	void *pIndexBuffer = NULL;
//	LockIndexBuffer(pIndexBuffer, D3DLOCK_READONLY);
//	UINT indexPos = 0;  // index buffer的当前位置
//
//	DWORD* pAttributeBuffer = NULL;
//	m_pMesh->LockAttributeBuffer(D3DLOCK_READONLY, &pAttributeBuffer);
//	UINT attriPos = 0;  // attributer buffer的当前位置
//
//	m_vSubModel.resize(subModelCount);
//	UINT faceCount = m_pMesh->GetNumFaces();
//	bool b32bit = faceCount * 3 > 65535 ? true : false;
//
//	// 初始化子模型的indexdata
//	for (UINT iSubModel = 0; iSubModel < subModelCount; ++iSubModel)
//	{
//		CSubModel &subModel = m_vSubModel[iSubModel];
//		subModel.m_pMaterial = (CMaterial*)table[iSubModel].AttribId;  // 材质指针
//		subModel.m_indexStart = attriPos * 3;
//
//		while (pAttributeBuffer[attriPos] == table[iSubModel] && attriPos < m_faceCount)  // 当前面的材质相同
//		{
//			++attriPos;
//		}
//		subModel.m_indexCount = attriPos * 3 - subModel.m_indexStart;
//
//		subModel.m_vertexStart = pIndexBuffer[subModel.m_indexStart];
//		UINT lastVertexPos = subModel.m_indexStart + subModel.m_indexCount - 1;
//		subModel.m_vertexCount = pIndexBuffer[lastVertexPos] - subModel.m_vertexStart + 1;
//	}
//
//	m_pMesh->UnlockAttributeBuffer();
//	UnlockIndexBuffer();
}

//绕自身坐标系或世界坐标系平移
void CModel::translate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum) //coordEnum = 0 = worldcoord, 1 = selfcoor;
{
	m_pNode->translate(pVec,coordEnum);
	UpdateFromNode();
}
//通过设置模型的坐标来设置模型的位置，世界坐标系, Pivot位置为模型的位置
void CModel::setPosition(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum)
{
	m_pNode->setPosition(pVec, coordEnum);
	UpdateFromNode();
}
//绕自身坐标系或世界坐标系旋转
void	CModel::rotate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum)
{
	m_pNode->rotate(pVec,coordEnum);
	UpdateFromNode();
}
//绕穿过固定点的指定轴旋转一定的角度
void	CModel::rotate(D3DXVECTOR3* pPos, D3DXVECTOR3* pVec, float fAngle, VGS_TRANSFORMSPACE coordEnum)
{
	m_pNode->rotate(pPos,pVec,fAngle,coordEnum);
	UpdateFromNode();
}
//直接设置旋转的角度，x,y,z三轴
void CModel::setRotationByAngle(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum)
{
	m_pNode->setRotationByAngle(pVec, coordEnum);
	UpdateFromNode();
}
//以自身坐标中心或世界坐标中心放大
void CModel::scale(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum)
{
  m_pNode->scale(pVec,coordEnum);
  UpdateFromNode();
}
//以固定点为中心放大
void CModel::scaleByPoint(D3DXVECTOR3* pVec, D3DXVECTOR3* pPos, VGS_TRANSFORMSPACE coordEnum)
{
  m_pNode->scaleByPoint(pVec, pPos, coordEnum);
  UpdateFromNode();
}
//直接设置缩放
void	CModel::setScale(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum)
{
  m_pNode->setScale(pVec,coordEnum);
  UpdateFromNode();
}

//-----------------------------------------------------------------------------
// 方法名: UpdateOther()
// 参  数: 
// 用  途: 更新模型的包围盒和包围球
//-----------------------------------------------------------------------------
//update other
// void CModel::update()
void CModel::UpdateFromNode()
{
	//update boundingbox and boundsphere by transform
//	if (kfrCnt > 0)
//	{
		D3DXVec3TransformCoord(&boundingBox.minCorner, &(oriBBox.minCorner), &(m_pNode->getMatrix(VTS_WORLD)) );
		D3DXVec3TransformCoord(&boundingBox.maxCorner, &(oriBBox.maxCorner), &(m_pNode->getMatrix(VTS_WORLD)) );
		D3DXVec3TransformCoord(&boundingSphere.Center, &(oriBSphere.Center), &(m_pNode->getMatrix(VTS_WORLD)) );
//	}
//	else
//	{
//		D3DXVec3TransformCoord(&boundingBox.minCorner, &(oriBBox.minCorner), &(transform.ori_matrix ) );
//		D3DXVec3TransformCoord(&boundingBox.maxCorner, &(oriBBox.maxCorner), &(transform.ori_matrix) );
//		D3DXVec3TransformCoord(&boundingSphere.Center, &(oriBSphere.Center), &(transform.ori_matrix) );
//	}
	
	//更新包围球半径，包围球半径不能在原有半径基础上乘，要根据包围盒计算
	//Bounding Sphere
	D3DXVECTOR3 Vec3Len = boundingBox.maxCorner - boundingBox.minCorner;
	boundingSphere.Radius = D3DXVec3Length(&Vec3Len)/2.0f;
         
	//轴的变换矩阵
	UpdateAxis();

	if (pRBBox)
	{
		//if (kfrCnt > 0)
			pRBBox->SetTransformMatrix(&m_pNode->getMatrix(VTS_WORLD));
	    //	else
		//	pRBBox->SetTransformMatrix(&transform.ori_matrix);
	}
}

//-----------------------------------------------------------------------------
// 方法名: MoveTextureUV()
// 参  数: FLOAT x, FLOAT y
// 用  途: 移动贴图坐标UV
//-----------------------------------------------------------------------------
VOID CModel::MoveVertexUV(UINT uvIdx, FLOAT x, FLOAT y)
{
	if (!m_VBuffer /*|| !m_IBuffer*/) return;

	//锁定内存
	m_VBuffer->Lock(0, vertexCnt * m_FVFSize, (VOID**)&(m_pVertices), D3DLOCK_DISCARD);
	
	//判断顶点的FVF,不同的FVF填充不同的数据
	UINT i;
	switch(m_FVF)
	{
	case FVF_POSCOLOR:
		break;
		
	case FVF_POSNORMAL:
		break;
		
	case FVF_POSCOLORCOORD1:
		for (i=0; i< vertexCnt; i++)
		{
			((VERTEXDECL_POSCOLORCOORD1*)m_pVertices)[i].su += x;
			((VERTEXDECL_POSCOLORCOORD1*)m_pVertices)[i].sv += y;
		}
		break;
		
	case FVF_POSNORMALCOLORCOORD1:
		//逐个顶点填充数据
		for (i=0; i< vertexCnt; i++)
		{
			((VERTEXDECL_POSNORMALCOLORCOORD1*)m_pVertices)[i].su += x;
			((VERTEXDECL_POSNORMALCOLORCOORD1*)m_pVertices)[i].sv += y;
		}
		break;
		
	case FVF_POSNORMALCOLORCOORD2:
		//逐个顶点填充数据
		for (i=0; i< vertexCnt; i++)
		{
			((VERTEXDECL_POSNORMALCOLORCOORD2*)m_pVertices)[i].su1 += x;
			((VERTEXDECL_POSNORMALCOLORCOORD2*)m_pVertices)[i].sv1 += y;
		}
		break;
		
	case FVF_POSNORMALCOLORCOORD3:
		//逐个顶点填充数据
		for (i=0; i< vertexCnt; i++)
		{
			((VERTEXDECL_POSNORMALCOLORCOORD3*)m_pVertices)[i].su1 += x;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)m_pVertices)[i].sv1 += y;
		}
		break;
	case FVF_POSNORMALCOLORCOORD4:
		{
			switch(uvIdx)
			{
			case 0:
				//逐个顶点填充数据
				for (i=0; i< vertexCnt; i++)
				{
					((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].su1 += x;
					((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].sv1 += y;
				}
				break;
			case 1:
				//逐个顶点填充数据
				for (i=0; i< vertexCnt; i++)
				{
					((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].su2 += x;
					((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].sv2 += y;
				}
				break;
			case 2:
				//逐个顶点填充数据
				for (i=0; i< vertexCnt; i++)
				{
					((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].su3 += x;
					((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].sv3 += y;
				}
				break;
			case 3:
				//逐个顶点填充数据
				for (i=0; i< vertexCnt; i++)
				{
					((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].su4 += x;
					((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].sv4 += y;
				}
				break;
			}
		break;
	}
	}

	//Unlock顶点缓冲
	m_VBuffer->Unlock();

}

//-----------------------------------------------------------------------------
// 方法名: SetOpacity()
// 参  数: UINT _opacity
// 用  途: 设置透明度
//-----------------------------------------------------------------------------
VOID CModel::SetOpacity(UINT _opacity)
{
	if (!m_VBuffer /*|| !m_IBuffer*/) return;

	BYTE alpha = (BYTE)(255 * _opacity/100.0f);;

	//锁定内存
	m_VBuffer->Lock(0, vertexCnt * m_FVFSize, (VOID**)&(m_pVertices), D3DLOCK_DISCARD);
	
// 	INT red = (mouseUpColor >> 16) & 0xff;
// 	INT green = (mouseUpColor >> 8) & 0xff;
// 	INT blue = mouseUpColor & 0xff;
// 	mouseUpColor = D3DCOLOR_RGBA(red, green, blue, alpha);

	// 修改颜色的alpah值
	#define _CHANGEALPHA(colARGB, a) ( D3DCOLOR((colARGB & 0x00ffffff) | (a << 24)) )

	//判断顶点的FVF,不同的FVF填充不同的数据
	switch(m_FVF)
	{
	case FVF_POSCOLOR:
		break;
		
	case FVF_POSNORMAL:	
		break;
		
	case FVF_POSCOLORCOORD1:
		break;
		
	case FVF_POSNORMALCOLORCOORD1:
		break;
	case FVF_POSNORMALCOLORCOORD2:
		break;
	case FVF_POSNORMALCOLORCOORD3:
		break;
	case FVF_POSNORMALCOLORCOORD4:
		break;
	}

	opacity = _opacity;

	//Unlock顶点缓冲
	m_VBuffer->Unlock();
}

CMaterial* CModel::GetMaterial(int subMeshIdx)
{
	INT count = m_vSubModel.size();
	if (subMeshIdx >= 0 && subMeshIdx < count)
	{
		return m_vSubModel[subMeshIdx].m_pMaterial;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: SetMtrl()
// 参  数: CMaterial* pMtrl
// 用  途: 设置材质球
//-----------------------------------------------------------------------------
VOID CModel::SetMtrl(CMaterial* pMtrl, int subMeshIdx)
{
	/*
	BYTE alpha; 
	if (opacity == 100)
		alpha = (BYTE)(255 * pMtrl->opacity/100.0f);
	else
		alpha = (BYTE)(255 * opacity/100.0f);
	*/

	if (!m_VBuffer/* || !m_IBuffer*/) return;

	FLOAT modelAlpha = opacity/100.0f;
	FLOAT mtrlAlpha = pMtrl->getOpacity()/100.0f;
	BYTE alpha = BYTE(modelAlpha * mtrlAlpha * 255 + 0.5f);
	//set the model Vertex diffuse color
	D3DCOLOR difColor = D3DCOLOR_RGBA(255,0,0,255);
	if(pMtrl->m_bUseDiffuse &&!bModelColor)
	{
		difColor = D3DCOLOR_RGBA((UINT)(pMtrl->d3dmtrl.Diffuse.r*255),
			(UINT)(pMtrl->d3dmtrl.Diffuse.g*255),
			(UINT)(pMtrl->d3dmtrl.Diffuse.b*255),
			alpha);
	}else if(bModelColor)
	{
	   difColor = modelColor;
	}
	else
	{
		difColor = oriColor;
	}

	//锁定内存
	m_VBuffer->Lock(0, vertexCnt * m_FVFSize, (VOID**)&(m_pVertices), D3DLOCK_DISCARD);

	//判断顶点的FVF,不同的FVF填充不同的数据
	UINT i;
	switch(m_FVF)
	{
	case FVF_POSCOLOR:
		for (i=0; i< vertexCnt; i++)
		{		
			((VERTEXDECL_POSCOLOR*)m_pVertices)[i].diffuse =  difColor;
		}
		break;
		
	case FVF_POSNORMAL:	
		break;
		
	case FVF_POSCOLORCOORD1:
		break;
		
	case FVF_POSNORMALCOLORCOORD1:
		break;
	case FVF_POSNORMALCOLORCOORD2:
		break;
	case FVF_POSNORMALCOLORCOORD3:
		break;
	case FVF_POSNORMALCOLORCOORD4:
		break;
	}

	//Unlock顶点缓冲
	m_VBuffer->Unlock();	
    // subMesh[subMeshIdx].shader =pMtrl;
	// m_vpMtrl[subMeshIdx] =pMtrl;
	if (subMeshIdx >= 0 && subMeshIdx < (int)m_vSubModel.size())
	{
		m_vSubModel[subMeshIdx].m_pMaterial = pMtrl;
	}

	//shader = pMtrl;	
	//opacity = shader->opacity;
}

//设置颜色
VOID CModel::SetVertexColor(INT red, INT green, INT blue, INT alpha)
{
	if (!m_VBuffer/* || !m_IBuffer*/) return;

	if (alpha > 255 || alpha < 0)
		return;

	opacity = INT(alpha / 255.0f * 100 + 0.5f);

	oriColor = D3DCOLOR_RGBA(red, green, blue, alpha);

	//锁定内存
	m_VBuffer->Lock(0, vertexCnt * m_FVFSize, (VOID**)&(m_pVertices), D3DLOCK_DISCARD);
	
	//判断顶点的FVF,不同的FVF填充不同的数据
	UINT i;
	switch(m_FVF)
	{
	case FVF_POSCOLOR:
		for (i=0; i< vertexCnt; i++)
		{		
			((VERTEXDECL_POSCOLOR*)m_pVertices)[i].diffuse = D3DCOLOR_RGBA(red, green, blue, alpha);
		}
		break;
		
	case FVF_POSNORMAL:
		break;
		
	case FVF_POSCOLORCOORD1:
		break;
		
	case FVF_POSNORMALCOLORCOORD1:
		break;
	case FVF_POSNORMALCOLORCOORD2:
		break;
	case FVF_POSNORMALCOLORCOORD3:
		break;
	case FVF_POSNORMALCOLORCOORD4:
		break;
	case FVF_SKINMESHCOORD1:
		break;
	case FVF_SKINMESHCOORD2:
		break;
	}

	//Unlock顶点缓冲
	m_VBuffer->Unlock();	
}
//设置颜色
VOID CModel::SetColor(INT red, INT green, INT blue, INT alpha)
{
     bModelColor = TRUE;
	 modelColor  = D3DCOLOR_RGBA(red, green, blue, alpha);
	 SetVertexColor(red,green,blue,alpha);
}

//创建模型的顶点缓冲
HRESULT CModel::createVBuffer()
{
	if (vertex == NULL)
		return E_FAIL;

	if( FAILED( m_pd3dDevice->CreateVertexBuffer( vertexCnt * m_FVFSize,
		0,
		m_FVF,
		D3DPOOL_MANAGED,
		&m_VBuffer, 
		NULL ) ) )
	{

		return E_FAIL;
	}
	
	//锁定内存
	if( FAILED(m_VBuffer->Lock(0, vertexCnt * m_FVFSize,
		(VOID**)&(m_pVertices), D3DLOCK_DISCARD) ))
	{
		return E_FAIL;
	}
	
	//判断顶点的FVF,不同的FVF填充不同的数据
	UINT i;
	switch(m_FVF)
	{
	case FVF_POSCOLOR:
		//逐个顶点填充数据
		for (i=0; i< vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((VERTEXDECL_POSCOLOR*)m_pVertices)[i].position = vertex[i].position;
			((VERTEXDECL_POSCOLOR*)m_pVertices)[i].diffuse = vertex[i].diffuse;
		}
		break;
		
	case FVF_POSNORMAL:
		//逐个顶点填充数据
		for (i=0; i< vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((VERTEXDECL_POSNORMAL*)m_pVertices)[i].position = vertex[i].position;	
			((VERTEXDECL_POSNORMAL*)m_pVertices)[i].normal = vertex[i].normal;
		}
		break;
		
	case FVF_POSCOLORCOORD1:
		//逐个顶点填充数据
		for (i=0; i< vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((VERTEXDECL_POSCOLORCOORD1*)m_pVertices)[i].position = vertex[i].position;
			((VERTEXDECL_POSCOLORCOORD1*)m_pVertices)[i].su = vertex[i].uv1.u;
			((VERTEXDECL_POSCOLORCOORD1*)m_pVertices)[i].sv = vertex[i].uv1.v;
		}
		break;
		
	case FVF_POSNORMALCOLORCOORD1:
		//逐个顶点填充数据
		for (i=0; i< vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((VERTEXDECL_POSNORMALCOLORCOORD1*)m_pVertices)[i].position = vertex[i].position;	
			((VERTEXDECL_POSNORMALCOLORCOORD1*)m_pVertices)[i].normal = vertex[i].normal;
			((VERTEXDECL_POSNORMALCOLORCOORD1*)m_pVertices)[i].su = vertex[i].uv1.u;
			((VERTEXDECL_POSNORMALCOLORCOORD1*)m_pVertices)[i].sv = vertex[i].uv1.v;
		}
		break;
		
	case FVF_POSNORMALCOLORCOORD2:
		//逐个顶点填充数据
		for (i=0; i< vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((VERTEXDECL_POSNORMALCOLORCOORD2*)m_pVertices)[i].position = vertex[i].position;	
			((VERTEXDECL_POSNORMALCOLORCOORD2*)m_pVertices)[i].su1 = vertex[i].uv1.u;
			((VERTEXDECL_POSNORMALCOLORCOORD2*)m_pVertices)[i].sv1 = vertex[i].uv1.v;
			((VERTEXDECL_POSNORMALCOLORCOORD2*)m_pVertices)[i].su2 = vertex[i].uv2.u;
			((VERTEXDECL_POSNORMALCOLORCOORD2*)m_pVertices)[i].sv2 = vertex[i].uv2.v;
		}
		break;
		
	case FVF_POSNORMALCOLORCOORD3:
		//逐个顶点填充数据
		for (i=0; i< vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((VERTEXDECL_POSNORMALCOLORCOORD3*)m_pVertices)[i].position = vertex[i].position;	
			((VERTEXDECL_POSNORMALCOLORCOORD3*)m_pVertices)[i].normal = vertex[i].normal;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)m_pVertices)[i].su1 = vertex[i].uv1.u;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)m_pVertices)[i].sv1 = vertex[i].uv1.v;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)m_pVertices)[i].su2 = vertex[i].uv2.u;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)m_pVertices)[i].sv2 = vertex[i].uv2.v;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)m_pVertices)[i].su3 = vertex[i].uv3.u;
			((VERTEXDECL_POSNORMALCOLORCOORD3*)m_pVertices)[i].sv3 = vertex[i].uv3.v;
		}
		break;
	case FVF_POSNORMALCOLORCOORD4:
		//逐个顶点填充数据
		for (i=0; i< vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].position = vertex[i].position;	
			((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].normal = vertex[i].normal;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].su1 = vertex[i].uv1.u;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].sv1 = vertex[i].uv1.v;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].su2 = vertex[i].uv2.u;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].sv2 = vertex[i].uv2.v;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].su3 = vertex[i].uv3.u;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].sv3 = vertex[i].uv3.v;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].su4 = vertex[i].uv4.u;
			((VERTEXDECL_POSNORMALCOLORCOORD4*)m_pVertices)[i].sv4 = vertex[i].uv4.v;
		}
		break;
	case FVF_SKINMESHCOORD1:
		for (i=0; i< vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((VERTEXDECL_SKINMESHCOORD1*)m_pVertices)[i].position = vertex[i].position;	
			((VERTEXDECL_SKINMESHCOORD1*)m_pVertices)[i].weight[0] = vertex[i].weight[0];
			((VERTEXDECL_SKINMESHCOORD1*)m_pVertices)[i].weight[1] = vertex[i].weight[1];
			((VERTEXDECL_SKINMESHCOORD1*)m_pVertices)[i].weight[2] = vertex[i].weight[2];
			((VERTEXDECL_SKINMESHCOORD1*)m_pVertices)[i].weight[3] = vertex[i].weight[3];
			((VERTEXDECL_SKINMESHCOORD1*)m_pVertices)[i].normal = vertex[i].normal;
			((VERTEXDECL_SKINMESHCOORD1*)m_pVertices)[i].su1 = vertex[i].uv1.u;
			((VERTEXDECL_SKINMESHCOORD1*)m_pVertices)[i].sv1 = vertex[i].uv1.v;
		}
		break;
	case FVF_SKINMESHCOORD2:
		for (i=0; i< vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((VERTEXDECL_SKINMESHCOORD2*)m_pVertices)[i].position = vertex[i].position;	
			((VERTEXDECL_SKINMESHCOORD2*)m_pVertices)[i].weight[0] = vertex[i].weight[0];
			((VERTEXDECL_SKINMESHCOORD2*)m_pVertices)[i].weight[1] = vertex[i].weight[1];
			((VERTEXDECL_SKINMESHCOORD2*)m_pVertices)[i].weight[2] = vertex[i].weight[2];
			((VERTEXDECL_SKINMESHCOORD2*)m_pVertices)[i].weight[3] = vertex[i].weight[3];
			((VERTEXDECL_SKINMESHCOORD2*)m_pVertices)[i].normal = vertex[i].normal;
			((VERTEXDECL_SKINMESHCOORD2*)m_pVertices)[i].su1 = vertex[i].uv1.u;
			((VERTEXDECL_SKINMESHCOORD2*)m_pVertices)[i].sv1 = vertex[i].uv1.v;
			((VERTEXDECL_SKINMESHCOORD2*)m_pVertices)[i].su2 = vertex[i].uv2.u;
			((VERTEXDECL_SKINMESHCOORD2*)m_pVertices)[i].sv2 = vertex[i].uv2.v;
		}
		break;

	}
	//Unlock顶点缓冲
	m_VBuffer->Unlock();

	return S_OK;
}

//创建模型的索引缓冲
HRESULT CModel::createIBuffer()
{
	//int bufUnitSize = 0;
	//
	//int subMeshCnt = subMesh.size();
	//for(int i = 0; i< subMeshCnt; i++)
	//{
	//	_subMesh* pSubMesh = &subMesh[i];
	//	if (pSubMesh->face == NULL)
	//		return E_FAIL;
	//	
	//	//判断单个模型的顶点数量是否超过了16位索引缓冲的最大顶点容量
	//	if (vertexCnt < 65536)
	//	{
	//		pSubMesh->IBufferFormat = D3DFMT_INDEX16;
	//		bufUnitSize = sizeof(unsigned short);
	//	}
	//	else
	//	{
	//		pSubMesh->IBufferFormat = D3DFMT_INDEX32;
	//		bufUnitSize = sizeof(unsigned int);
	//	}
	//	
	//	//建立当前模型的索引缓冲
	//	if( FAILED( m_pd3dDevice ->CreateIndexBuffer(pSubMesh->faceCnt * 3 * bufUnitSize,
	//		0,
	//		pSubMesh->IBufferFormat,
	//		D3DPOOL_MANAGED,
	//		&pSubMesh->m_IBuffer, NULL) ) )
	//		return E_FAIL;

	//	//锁定内存
	//	if( FAILED(pSubMesh->m_IBuffer->Lock(0, pSubMesh->faceCnt * 3 * bufUnitSize, (VOID**)&pSubMesh->m_pIndices, 0) ))
	//		return E_FAIL;
	//	
	//	//16位顶点索引缓冲
	//	if (pSubMesh->IBufferFormat == D3DFMT_INDEX16)
	//	{
	//		//逐个面填充索引数据
	//		for (UINT j =0; j< pSubMesh->faceCnt; j++)
	//		{
	//			((unsigned short*)pSubMesh->m_pIndices)[3*j] = pSubMesh->face[j].vertices0->index;
	//			((unsigned short*)pSubMesh->m_pIndices)[3*j+1] = pSubMesh->face[j].vertices1->index;
	//			((unsigned short*)pSubMesh->m_pIndices)[3*j+2] = pSubMesh->face[j].vertices2->index;
	//		}
	//	}
	//	
	//	//32位顶点索引缓冲
	//	if (pSubMesh->IBufferFormat == D3DFMT_INDEX32)
	//	{
	//		//逐个面填充索引数据
	//		for (UINT j =0; j< pSubMesh->faceCnt; j++)
	//		{
	//			((unsigned int*)pSubMesh->m_pIndices)[3*j] = pSubMesh->face[j].vertices0->index;
	//			((unsigned int*)pSubMesh->m_pIndices)[3*j+1] = pSubMesh->face[j].vertices1->index;
	//			((unsigned int*)pSubMesh->m_pIndices)[3*j+2] = pSubMesh->face[j].vertices2->index;
 //               
	//		}
	//	}		
	//	//Unlock索引缓冲
	//	pSubMesh->m_IBuffer->Unlock();
	//	
	//	//删除模型对象中的面数据，节省内存
	//	SAFE_DELETE_ARRAY(pSubMesh->face);		   
	//	SetMtrl(subMesh[i].shader,i);
	//}

	////删除模型对象中的顶点数据，节省内存
	////SAFE_DELETE_ARRAY(vertex);

	return S_OK;
}

//克隆模型到场景
CModel*	CModel::clone(std::string newModelName)
{
	/*
	pNewModel->modelGroup;			//属于哪个modelGroup
	pNewModel->bmouseHintByGroup;	//是否使用[模型组]来处理鼠标移上物体和选中物体
	*/

	//创建模型
//	CModel* pNewModel = new CModel(m_pd3dDevice);
//	pNewModel->pCloneModel = this;
//
//	pNewModel->setName(newModelName);
//	
//	pNewModel->m_VBuffer = m_VBuffer;		
//    pNewModel->m_pVertices = m_pVertices;	
//	pNewModel->m_FVF = m_FVF;				
//	pNewModel->m_FVFSize = m_FVFSize;		
//
//	pNewModel->oriBBox = oriBBox;			
//	pNewModel->boundingBox = boundingBox;		
//	pNewModel->oriBSphere = oriBSphere;		
//	pNewModel->boundingSphere = boundingSphere;	
//
//	pNewModel->vertexCnt = vertexCnt;		
//	pNewModel->vertex = vertex;			
//        
//	pNewModel->subMesh = subMesh;         
//	
//	pNewModel->opacity = opacity;			
//	pNewModel->oriOpacity = oriOpacity;		
//
//	pNewModel->mouseInOutable = mouseInOutable;	
//
//	pNewModel->bMouseShader = bMouseShader;	
//
//	pNewModel->bMouseColor = bMouseColor;		
//	pNewModel->oriColor = oriColor;		
//	pNewModel->mouseOverColor = mouseOverColor;	
//	pNewModel->mouseClickedColor = mouseClickedColor;
//	pNewModel->mouseUpColor = mouseUpColor;	
//	
//	pNewModel->visible = visible;			
//	pNewModel->culled = culled;			
//	pNewModel->clickable = clickable;		
//	pNewModel->rayHitable = rayHitable;		
//	pNewModel->colStatus = colStatus;		
//	pNewModel->isSkyBox = isSkyBox;		
//	pNewModel->bSelected = bSelected;		
//	
//	pNewModel->currentTimer = currentTimer;	
//	pNewModel->kfrCnt = kfrCnt;			
//	pNewModel->kfrFrameTime = kfrFrameTime;	
//	pNewModel->kfrCurFrame = kfrCurFrame;		
//	pNewModel->startFrame = startFrame;		
//	pNewModel->endFrame = endFrame;		
//
//	//用于MoveModelTo, RosModelTo, ScaleModelTo帧变换方法
//	pNewModel->user_moveKFR = user_moveKFR;		
//	pNewModel->user_moveFrameCnt = user_moveFrameCnt;	
//	pNewModel->user_moveCurFrame = user_moveCurFrame;	
//	pNewModel->bMatrixChanged = bMatrixChanged;		
//
//    //模型的关键帧动画（此部分要写到node里）
//	pNewModel->kfrPlay = kfrPlay;			
//	pNewModel->kfrRepeatCnt = kfrRepeatCnt;	
//	pNewModel->kfrRepeatIndex = kfrRepeatIndex;	
//
//	pNewModel->isBillboard = isBillboard;		
//	pNewModel->pointAtOrient = pointAtOrient;	
//	pNewModel->billboardDirectionAxis = billboardDirectionAxis;     
//
//	// fuxb 2008-09-09 增加两个参数 ==>
//	pNewModel->billboradUpAxis = billboradUpAxis;   
//	pNewModel->isAutoNormalise = isAutoNormalise;   
//	// <==
//
//	pNewModel->isCollision = isCollision;		//
//	pNewModel->colDetectDistance = colDetectDistance;
//
//	pNewModel->isGround = isGround;		
//
//	pNewModel->isZOne = isZOne;			
//
//	pNewModel->bDynamicUV = bDynamicUV;		
////	pNewModel->UStep = UStep;			
////	pNewModel->VStep = VStep;			
//
//	pNewModel->pAxis = pAxis;			
//	pNewModel->showAxis = showAxis;		
//
//	pNewModel->pRBBox = pRBBox;            
//	pNewModel->showRBBox = showRBBox; 
//    
//	pNewModel->ID1 = ID1;				//
//	pNewModel->ID2 = ID2;				//
//
//	pNewModel->bUseFBL = bUseFBL;			//
//
//	//是否显示反射标志
//    pNewModel->bReflect = bReflect;
//    //该物体要反射的信息索引，如果没有反射 则为-1
//	pNewModel->sceneRefInfoListIdx = sceneRefInfoListIdx;    
//	pNewModel->refedModelIdxList = refedModelIdxList;    
//   
//	SAFE_RELEASE_D3D(m_pMesh);
//
//	return pNewModel;
return 0;
}

void CModel::RenderSubMesh(int submeshIdx)
{
	if (submeshIdx < 0 || submeshIdx >= (int)m_vSubModel.size())
	{
		return;
	}

	//模型的可见性
	if( !visible || isCollision || isGround)
		return;

	//渲染当前模型
	DWORD subMeshCnt = 0;
	m_pMesh->GetAttributeTable(0, &subMeshCnt);
	if (submeshIdx >= (int)subMeshCnt)
	{
		return;
	}

	vector<D3DXATTRIBUTERANGE> table(subMeshCnt);
	m_pMesh->GetAttributeTable(&table[0], &subMeshCnt);

	//设置是否重新计算模型法线，保证缩放模型能正常显示高光
	m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, isAutoNormalise);

	//设置该模型当前帧的矩阵
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &(m_pNode->getMatrix(VTS_WORLD)));

	// 设置材质纹理到设备
	CMaterial* pMtrl = (CMaterial*)(table[submeshIdx].AttribId);//m_vSubModel[submeshIdx].m_pMaterial;
	if (pMtrl)
	{
		pMtrl->SetToDevice(m_pd3dDevice, 0);
	}

	//绘制
	m_pMesh->DrawSubset(table[submeshIdx].AttribId);
}

//单独渲染这个模型的方法
void CModel::render()
{
	//模型的可见性
	if ( !visible || isCollision || isGround || !m_pNode)
		return;
	
	//设置是否重新计算模型法线，保证缩放模型能正常显示高光
	m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, isAutoNormalise);

	//设置该模型当前帧的矩阵
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &(m_pNode->getMatrix(VTS_WORLD)));

	//渲染当前模型
	DWORD subMeshCnt = 0;
	m_pMesh->GetAttributeTable(0, &subMeshCnt); 
	vector<D3DXATTRIBUTERANGE> table(subMeshCnt);
	m_pMesh->GetAttributeTable(&table[0], &subMeshCnt);
	for (DWORD i = 0; i < subMeshCnt; i++)
	{
		// 设置材质纹理到设备
		// CMaterial* pMtrl = m_vSubModel[i].m_pMaterial;
		CMaterial* pMtrl = (CMaterial*)(table[i].AttribId);
		if (pMtrl)
		{
			pMtrl->SetToDevice(m_pd3dDevice, 0, this);
		}

		//绘制
		m_pMesh->DrawSubset(table[i].AttribId);
	}

	/*
	//使用该材质的模型
	vector<_subMesh>::iterator ppSubMesh = subMesh.begin();
	int subMeshCnt = subMesh.size();
	for (int i = 0; i< subMeshCnt; i++)
	{
		_subMesh subMesh = *(ppSubMesh + i);
		
		//设置是否重新计算模型法线，保证缩放模型能正常显示高光
		m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, isAutoNormalise);
				
		//设置该模型当前帧的矩阵
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &(m_pNode->curTransform.worldMatrix));
		
		//渲染模型
		m_pd3dDevice->SetStreamSource( 0, m_VBuffer, 0, m_FVFSize );
		//设置FVF
		m_pd3dDevice->SetFVF(m_FVF); //DX9为SetFVF,DX8用这个函数

		//渲染SubMesh
		//设置IB
		m_pd3dDevice->SetIndices(subMesh.m_IBuffer);
		
		//改变索引缓冲区的面的数量将会较大幅度的提升运算速度**************
		m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
			vertexCnt, //当前模型的顶点数
			0, //当前模型的索引缓冲区起点
			subMesh.faceCnt); //当前模型的面数
	}		
	*/
}

//渲染模型的SubMesh
//VOID CModel::RenderSubMesh(INT index)
//{
//	m_pd3dDevice->SetStreamSource( 0, m_VBuffer, 0, m_FVFSize );
//	//设置FVF
//	m_pd3dDevice->SetFVF(m_FVF); //DX9为SetFVF,DX8用这个函数
//
//	
//	//渲染SubMesh
//	//设置IB
//	m_pd3dDevice->SetIndices(subMesh[index].m_IBuffer);
//	
//	//改变索引缓冲区的面的数量将会较大幅度的提升运算速度**************
//	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
//										vertexCnt, //当前模型的顶点数
//										0, //当前模型的索引缓冲区起点
//										subMesh[index].faceCnt); //当前模型的面数
//}

//释放模型的资源
VOID CModel::Release()
{
//	if (pCloneModel != NULL) return;
//
//	//delete vertex and face
//	SAFE_DELETE_ARRAY(vertex);
//   
//	/*
//	//delete vertices for kfr
//	SAFE_DELETE_ARRAY(matrix_kfr);	
//	if (vkfrCnt > 0)
//	{
//		for (UINT j =0; j< kfrCnt; j++)
//			SAFE_DELETE_ARRAY(vertices_kfr[j]);
//	}
//	SAFE_DELETE_ARRAY(vertices_kfr);
//	*/
//
//	SAFE_DELETE_ARRAY(user_moveKFR);
//
//	//release VB & IB
//	SAFE_RELEASE_D3D(m_VBuffer);
//
//	//删除SubMesh
//	int subMeshCnt = subMesh.size();
//	vector<_subMesh>::iterator pSubMesh = subMesh.begin();
//	for (int i =0; i< subMeshCnt; i++)
//	{
//		_subMesh* _pSubMesh = &(*(pSubMesh + i));
//
//       SAFE_DELETE_ARRAY(_pSubMesh->face);
//
//	   SAFE_RELEASE_D3D(_pSubMesh->m_IBuffer);
//	   
//		_pSubMesh->m_pIndices = NULL;
//       _pSubMesh->face = NULL;
//	   _pSubMesh->shader =NULL;
//	   _pSubMesh->oriShader =NULL;
//	}
//	subMesh.clear();
//
//	//释放并删除轴对象
//	SAFE_RELEASE(pAxis);
//	SAFE_DELETE(pAxis);
//
//	//释放并删除包围盒对象
//	SAFE_RELEASE(pRBBox);
//	SAFE_DELETE(pRBBox);
//
//	// SAFE_DELETE(m_pNode);
//
//	m_pVertices =NULL;
//	
//	bReflect = FALSE;
//
//	refedModelIdxList.clear();
//	useMeCubeMap.clear();
//	//shader = NULL;
////	mouseOverShader = NULL;
////	mouseClickedShader = NULL;
////	mouseUpShader = NULL;
//
////	vertices_kfr = NULL;
////	matrix_kfr = NULL;
//	vertex = NULL;
//	//清除模型渲染所需的信息
//	m_additionalRenderInfo.clear();
	
}
// 模型是否透明，包括模型自身颜色是否透明，或者使用的材质是否透明
BOOL CModel::IsTransparent()
{
	if (opacity < 100)
	{
		return TRUE;
	}

	return IsUseTransparentMaterial();
}

// 模型是否使用透明材质
BOOL CModel::IsUseTransparentMaterial()
{
	for (vector<CSubModel>::iterator it = m_vSubModel.begin(); it != m_vSubModel.end(); ++it)
	{
		if (it->m_pMaterial && it->m_pMaterial->IsTransparent())
		{
			return TRUE;
		}
	}
	return FALSE;
}


// std::string CModel::GetName()
// {
// 	return name;
// }
// VOID CModel::SetName(const std::string &sName)
// {
// 	name = sName;
// }

// 加锁顶点缓冲
HRESULT CModel::LockVertexBuffer(void **pVertexBuffer, DWORD op/* = D3DLOCK_READONLY*/)
{
	if (m_pMesh)
	{
		HRESULT hr = m_pMesh->LockVertexBuffer(op, (void**)pVertexBuffer);
		return hr;
	}
	return E_FAIL;
}

// 解锁顶点缓冲
HRESULT CModel::UnlockVertexBuffer()
{
	if (m_pMesh)
	{
		return m_pMesh->UnlockVertexBuffer();	
	}
	return S_OK;
}

HRESULT CModel::LockIndexBuffer(void **pIndexBuffer, DWORD op/* = D3DLOCK_READONLY*/)
{
	if (m_pMesh)
	{
		m_pMesh->LockIndexBuffer(op, (void**)pIndexBuffer);
	}
	return E_FAIL;
}

HRESULT CModel::UnlockIndexBuffer()
{
	if (m_pMesh)
	{
		return m_pMesh->UnlockIndexBuffer();	
	}
	return S_OK;

}
// 加锁某个子模型的索引缓冲
//HRESULT CModel::LockIndexBuffer(INT iSubMeshIdx)
//{
//	UINT bufUnitSize = 0;
//	if (iSubMeshIdx >= 0 && iSubMeshIdx < (int)m_vpMtrl.size())
//	{
//		_subMesh* pSubMesh = &m_vpMtrl[iSubMeshIdx];
//
//		//判断单个模型的顶点数量是否超过了16位索引缓冲的最大顶点容量
//		if (vertexCnt < 65536)
//		{
//			pSubMesh->IBufferFormat = D3DFMT_INDEX16;
//			bufUnitSize = sizeof(unsigned short);
//		}
//		else
//		{
//			pSubMesh->IBufferFormat = D3DFMT_INDEX32;
//			bufUnitSize = sizeof(unsigned int);
//		}
//		//锁定内存
//		if (FAILED(pSubMesh->m_IBuffer->Lock(0, pSubMesh->faceCnt * 3 * bufUnitSize, (VOID**)&pSubMesh->m_pIndices, 0)))
//		{
//			return E_FAIL;
//		}
//	}
//	return S_OK;
//}
//
//// 解锁子模型的索引缓冲
//HRESULT CModel::UnlockIndexBuffer(INT iSubMeshIdx)
//{
//	if (iSubMeshIdx >= 0 && iSubMeshIdx < (INT)subMesh.size())
//	{
//		// Unlock索引缓冲
//		_subMesh* pSubMesh = &subMesh[iSubMeshIdx];
//		if (pSubMesh)
//		{
//			pSubMesh->m_IBuffer->Unlock();
//		}
//	}
//	return S_OK;	
//}

CMaterial* CModel::getSubModelMaterial(INT subMeshIndex)
{
	if (subMeshIndex >= 0 && subMeshIndex < (INT)m_vSubModel.size())
	{
		return m_vSubModel[subMeshIndex].m_pMaterial;
	}
	return 0;
}

VOID CModel::setSubModelMaterial(INT subMeshIndex, CMaterial *pMaterial)
{
	if (subMeshIndex >= 0 && subMeshIndex < (INT)m_vSubModel.size())
	{
		m_vSubModel[subMeshIndex].m_pMaterial = pMaterial;
	}
}

// 添加cubemap，如果添加了cubemap，那么cubemap就会反射该模型
VOID CModel::addCubemap(CCubeMap *pCubemap)
{
	if (_GetIndexByHandle(useMeCubeMap, pCubemap) == -1)
	{
		useMeCubeMap.push_back(pCubemap);

		ADDITIONALRENDERINFO info;
		info.type = ADS_CUBEMAP;
		info.pRenderToTexture = (DWORD)pCubemap;
	    m_additionalRenderInfo.push_back(info);
	}
}

// 移除cubemap
VOID CModel::removeCubemap(CCubeMap *pCubemap)
{
	int index = _GetIndexByHandle(useMeCubeMap, pCubemap);
	if (index != -1)
	{
		useMeCubeMap.erase(useMeCubeMap.begin() + index);

		for (int i = 0; i < (INT)m_additionalRenderInfo.size(); ++i)
		{
			if (m_additionalRenderInfo[i].type == ADS_CUBEMAP && m_additionalRenderInfo[i].pRenderToTexture == (DWORD)pCubemap)
			{
				m_additionalRenderInfo.erase(m_additionalRenderInfo.begin() + i);
				break;
			}
		}
	}
}

// 添加bump map
VOID CModel::addBumpmap(CBumpMap *pBumpmap)
{
	if (_GetIndexByHandle(m_useMeBumpmap, pBumpmap) == -1)
	{
		m_useMeBumpmap.push_back(pBumpmap);
		
		ADDITIONALRENDERINFO info;
		info.type = ADS_BUMPWAVEMAP;
		info.pRenderToTexture = (DWORD)pBumpmap;
		m_additionalRenderInfo.push_back(info);
	}
}

VOID CModel::removeBumpmap(CBumpMap* pBumpmap)
{
	int index = _GetIndexByHandle(m_useMeBumpmap, pBumpmap);
	if (index != -1)
	{
		m_useMeBumpmap.erase(m_useMeBumpmap.begin() + index);
		
		for (int i = 0; i < (INT)m_additionalRenderInfo.size(); ++i)
		{
			if (m_additionalRenderInfo[i].type == ADS_BUMPWAVEMAP && m_additionalRenderInfo[i].pRenderToTexture == (DWORD)pBumpmap)
			{
				m_additionalRenderInfo.erase(m_additionalRenderInfo.begin() + i);
				break;
			}
		}
	}
}


// 点是否在模型包围球内部
BOOL CModel::IsInsideSphere(D3DXVECTOR3* pPos)  
{
	D3DXVECTOR3 v3Temp;
	D3DXVec3Subtract(&v3Temp, &(boundingSphere.Center), pPos);
	FLOAT distSq = D3DXVec3LengthSq(&v3Temp);                // 点与模型中心的距离平方
	return distSq < boundingSphere.Radius * boundingSphere.Radius;
}

// 点是否在模型包围盒内部
BOOL CModel::IsInsideBox(D3DXVECTOR3* pPos)  
{
	return (   pPos->x >= boundingBox.minCorner.x && pPos->x <= boundingBox.maxCorner.x
		&& pPos->y >= boundingBox.minCorner.y && pPos->y <= boundingBox.maxCorner.y
		&& pPos->z >= boundingBox.minCorner.z && pPos->z <= boundingBox.maxCorner.z
		);
}

// 点到模型包围盒所有顶点的最近距离平方，不区分点在模型内部还是外部的情况
FLOAT CModel::getNearDistanceSq(D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 v0(boundingBox.minCorner.x, boundingBox.minCorner.y, boundingBox.minCorner.z);
	D3DXVECTOR3 v1(boundingBox.maxCorner.x, boundingBox.minCorner.y, boundingBox.minCorner.z);
	D3DXVECTOR3 v2(boundingBox.maxCorner.x, boundingBox.minCorner.y, boundingBox.maxCorner.z);
	D3DXVECTOR3 v3(boundingBox.minCorner.x, boundingBox.minCorner.y, boundingBox.maxCorner.z);
	
	D3DXVECTOR3 v4(boundingBox.minCorner.x, boundingBox.maxCorner.y, boundingBox.minCorner.z);
	D3DXVECTOR3 v5(boundingBox.maxCorner.x, boundingBox.maxCorner.y, boundingBox.minCorner.z);
	D3DXVECTOR3 v6(boundingBox.maxCorner.x, boundingBox.maxCorner.y, boundingBox.maxCorner.z);
	D3DXVECTOR3 v7(boundingBox.minCorner.x, boundingBox.maxCorner.y, boundingBox.maxCorner.z);
	
	vector<FLOAT> vDist; 
	D3DXVECTOR3 v3Temp;
	vDist.push_back(D3DXVec3LengthSq(D3DXVec3Subtract(&v3Temp, &v0, pPos)));
	vDist.push_back(D3DXVec3LengthSq(D3DXVec3Subtract(&v3Temp, &v1, pPos)));
	vDist.push_back(D3DXVec3LengthSq(D3DXVec3Subtract(&v3Temp, &v2, pPos)));
	vDist.push_back(D3DXVec3LengthSq(D3DXVec3Subtract(&v3Temp, &v3, pPos)));
	
	vDist.push_back(D3DXVec3LengthSq(D3DXVec3Subtract(&v3Temp, &v4, pPos)));
	vDist.push_back(D3DXVec3LengthSq(D3DXVec3Subtract(&v3Temp, &v5, pPos)));
	vDist.push_back(D3DXVec3LengthSq(D3DXVec3Subtract(&v3Temp, &v6, pPos)));
	vDist.push_back(D3DXVec3LengthSq(D3DXVec3Subtract(&v3Temp, &v7, pPos)));
	
	stable_sort(vDist.begin(), vDist.end());
	
	return vDist[0];
}

CSubModel* CModel::getSubModel(int submeshIndex)
{
	if (submeshIndex >= 0 && submeshIndex < (int)m_vSubModel.size())
	{
		return &m_vSubModel[submeshIndex];
	}
	return 0;
}

void CModel::CreateAxis()
{
	DestroyAxis();
	D3DXMATRIX mat;
	m_pNode->getMatrix(VTS_WORLD);
	m_pAxis = new Axis(m_pd3dDevice);
	if (NULL != m_pAxis)
	{
 		m_pAxis->Create();
		m_pAxis->SetTransformMatrix(&mat);
	}
	m_pAxisMove = new AxisMove(m_pd3dDevice);
	if (NULL != m_pAxisMove)
	{
		m_pAxisMove->SetTransformMatrix(&mat);
	}
	m_pAxisRotate = new AxisRotate(m_pd3dDevice);
	if (NULL != m_pAxisRotate)
	{
		m_pAxisRotate->SetTransformMatrix(&mat);
	}
	m_pAxisScale = new AxisScale(m_pd3dDevice);
	if (NULL != m_pAxisScale)
	{
		m_pAxisScale->SetTransformMatrix(&mat);
	}
	UpdateAxis();
}
void CModel::DestroyAxis()
{
	SAFE_DELETE(m_pAxis);
	SAFE_DELETE(m_pAxisMove);
	SAFE_DELETE(m_pAxisRotate);
	SAFE_DELETE(m_pAxisScale);
}
void CModel::SetShowAxis(bool bShow)
{
	m_bShowAxis = bShow;
	if (m_bShowAxis)
	{
		CreateAxis();
	}
	else
	{
		DestroyAxis();
	}
}
void CModel::UpdateAxis()
{
	if (NULL != m_pAxis)
	{
		m_pAxis->SetTransformMatrix(&m_pNode->getMatrix(VTS_WORLD));
	}
	if (NULL != m_pAxisMove)
	{
		m_pAxisMove->SetTransformMatrix(&m_pNode->getMatrix(VTS_WORLD));
	}
	if (NULL != m_pAxisRotate)
	{
		m_pAxisRotate->SetTransformMatrix(&m_pNode->getMatrix(VTS_WORLD));
	}
	if (NULL != m_pAxisScale)
	{
		m_pAxisScale->SetTransformMatrix(&m_pNode->getMatrix(VTS_WORLD));
	}
}

void CModel::RenderAxis(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection)
{
	if (!m_bShowAxis)
		return;
	switch (m_nAxisIndex)
	{
	case 0:
		if (NULL != m_pAxis)
		{
			m_pAxis->Draw(pViewport, pView, pProjection);
		}
		break;
	case 1:
		if (NULL != m_pAxisMove)
		{
			m_pAxisMove->Draw(pViewport, pView, pProjection);
		}
		break;
	case 2:
		if (NULL != m_pAxisRotate)
		{
			m_pAxisRotate->Draw(pViewport, pView, pProjection);
		}
		break;
	case 3:
		if (NULL != m_pAxisScale)
		{
			m_pAxisScale->Draw(pViewport, pView, pProjection);
		}
		break;
	default:
		break;
	}
}
Axis* CModel::GetCurAxis()
{
	switch (m_nAxisIndex)
	{
	case 0:
		return m_pAxis;
	case 1:
		return m_pAxisMove;
	case 2:
		return m_pAxisRotate;
	case 3:
		return m_pAxisScale;
	default:
		return 0;
	}
}
