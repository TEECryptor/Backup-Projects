#pragma once

#include "VGSHeader.h"
#include "VGSDef.h"
#include "fileio\\S3D_Def.h"
#include "CSysFunc.h"

// 清空buffer
#define _ZeroMemory(buffer) ZeroMemory(buffer, sizeof(buffer));

#pragma pack(push, 1) // 数据结构以单字节对齐

// 标准向量
const D3DXVECTOR3 D3DXVECTOR3_ZERO = D3DXVECTOR3(0, 0, 0);
const D3DXVECTOR3 D3DXVECTOR3_X = D3DXVECTOR3(1, 0, 0);
const D3DXVECTOR3 D3DXVECTOR3_Y = D3DXVECTOR3(0, 1, 0);
const D3DXVECTOR3 D3DXVECTOR3_Z = D3DXVECTOR3(0, 0, 1);


typedef struct CVertex_UV
{
	float u;
	float v;
}CVertex_UV;

// 顶点格式
typedef struct CVertex_POSNORMALCOLORCOORD1
{
	//顶点的位置
	VECTOR3					position;
	//顶点的法线
	VECTOR3					normal;
	//顶点的颜色
	DWORD					color;

	//uv坐标
	CVertex_UV         uv1;
}CVertex_POSNORMALCOLORCOORD1;

// 顶点格式
typedef struct CVertex_POSNORMALCOLORCOORD2
{
	//顶点的位置
	VECTOR3					position;
	//顶点的法线
	VECTOR3					normal;
	//顶点的颜色
	DWORD					color;

	//uv坐标
	CVertex_UV         uv1;
	CVertex_UV         uv2;
}CVertex_POSNORMALCOLORCOORD2;

// 顶点格式
typedef struct CVertex_POSNORMALCOLORCOORD3
{
	//顶点的位置
	VECTOR3					position;
	//顶点的法线
	VECTOR3					normal;
	//顶点的颜色
	DWORD					color;

	//uv坐标
	CVertex_UV         uv1;
	CVertex_UV         uv2;
	CVertex_UV         uv3;
}CVertex_POSNORMALCOLORCOORD3;

// 顶点格式
typedef struct CVertex_POSNORMALCOLORCOORD4
{
	//顶点的位置
	VECTOR3					position;
	//顶点的法线
	VECTOR3					normal;
	//顶点的颜色
	DWORD					color;

	//uv坐标
	CVertex_UV         uv1;
	CVertex_UV         uv2;
	CVertex_UV         uv3;
	CVertex_UV         uv4;
}CVertex_POSNORMALCOLORCOORD4;

typedef struct CVGSFace
{
	UINT a;  // 定点索引
	UINT b;
	UINT c;
}CVGSFace;

// 包围盒
typedef struct CVGSBoundBox
{
	VECTOR3					minCorner;
	VECTOR3					maxCorner;
}CVGSBoundBox;            

// 包围球
typedef struct CVGSBoundSphere
{
	VECTOR3     Center;
	FLOAT		Radius;
}CVGSBoundSphere;

// submesh
typedef struct CVGSSubMesh
{
	INT mtrIndex;
	UINT faceCount;
	CVGSFace *pFace;

	CVGSSubMesh()
	{
		mtrIndex = -1;
		faceCount = 0;
		pFace = 0;
	}
	~CVGSSubMesh()
	{
		SAFE_DELETE_ARRAY(pFace);
	}

	// 骨骼

}CVGSSubMesh;

// 模型
class VGS_EXPORT CVGSMesh
{
public:
	std::string		   sName;

	//顶点数量
	DWORD               fvf;        // 顶点格式
	UINT			    vertexCnt;	// 定点数目 	
	void*		        pVertex;     // 顶点数据

	//面数量
	UINT				faceCnt;    // 面数目

	UINT				subMeshCount;  // submesh数目
	void*               pSubMesh;      // submesh数据

	//v3.02-绑定的骨骼编号; v3.03骨骼动画的编号
	int						SkeletonAnimationIndex; //bindingSkeletonIndex-v3.02

	//骨头数量
	int						boneCnt;
	//蒙皮信息
	VGS_Bone				bone;

	//骨骼根骨头在Mesh坐标系中的变换
	NodeTransform			skeletontransform;

	CVGSMesh()
	{
		sName = "";
		vertexCnt = 0;
		pVertex = 0;
		faceCnt = 0;
		subMeshCount = 0;
		pSubMesh = 0;
		SkeletonAnimationIndex = -1;
		boneCnt = 0;
	}

	void Release()
	{
		if (pVertex)
		{
			switch (fvf)
			{
			case FVF_POSNORMALCOLORCOORD1:
				{
					CVertex_POSNORMALCOLORCOORD1 *pVert = (CVertex_POSNORMALCOLORCOORD1 *)pVertex;
					SAFE_DELETE_ARRAY(pVert);
					break;
				}
			case FVF_POSNORMALCOLORCOORD2:
				{
					CVertex_POSNORMALCOLORCOORD2 *pVert = (CVertex_POSNORMALCOLORCOORD2*)pVertex;
					SAFE_DELETE_ARRAY(pVert);
					break;
				}
			case FVF_POSNORMALCOLORCOORD3:
				{
					CVertex_POSNORMALCOLORCOORD3 *pVert = (CVertex_POSNORMALCOLORCOORD3 *)pVertex;
					SAFE_DELETE_ARRAY(pVert);
					break;
				}
			case FVF_POSNORMALCOLORCOORD4:
				{
					CVertex_POSNORMALCOLORCOORD4 *pVert = (CVertex_POSNORMALCOLORCOORD4 *)pVertex;
					SAFE_DELETE_ARRAY(pVert);
					break;
				}
			default:
				break;
			}
			pVertex = 0;

			if (pSubMesh)
			{
				CVGSSubMesh *pMesh = (CVGSSubMesh*)pSubMesh;
				SAFE_DELETE_ARRAY(pMesh);
				pSubMesh = 0;
			}

		}
	}

	~CVGSMesh()
	{
		
	}

};

// 模型
typedef struct CVGSModelData
{
	CVGSMesh                mesh;
	bool                    isBillboard;
	bool                    isCollision;
	bool                    isGround;
	bool                    isSkyBox; 
	bool                    isVisible;		
	bool                    isClickable;
	bool                    isAutoNormalise;            // 模型缩放时，是否自动更新法线 fuxb 2008-09-09
}CVGSModelData;

// 相机参数动画结构S3D
typedef struct CVGS_CAMERA_ANIMINFO
{	
	float                  nearClip;
	float                  farClip;
	//镜头
	float					fov;
}CVGS_CAMERA_ANIMINFO;

//Node的变换
typedef struct CNodeTransform
{
	//帧编号
	float					time;   //时刻
	//Node的变换-位置
	VECTOR3					position;
	//Node的变换-旋转
	VGS_QUATERNION				quat;
	//Node的变换-缩放
	VECTOR3					scale;
}CNodeTransform;

//灯光的可变参数S3D
typedef struct CVGS_LIGHT_INFO
{
	//颜色
	DWORD					color;

	//光线衰减范围
	float					range1;
	float					range2;

	//强度
	float					multiply;

	//内角外角
	float					theta;
	float					phi;

	VECTOR3                 pos;   // 灯光的位置，相对于父节点
	VECTOR3                 dir;   // 灯光的方向，相对于父节点

}CVGS_LIGHT_INFO;

#pragma pack(pop)