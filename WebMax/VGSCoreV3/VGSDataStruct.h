#pragma once

#include "VGSHeader.h"
#include "VGSDef.h"
#include "fileio\\S3D_Def.h"
#include "CSysFunc.h"

// ���buffer
#define _ZeroMemory(buffer) ZeroMemory(buffer, sizeof(buffer));

#pragma pack(push, 1) // ���ݽṹ�Ե��ֽڶ���

// ��׼����
const D3DXVECTOR3 D3DXVECTOR3_ZERO = D3DXVECTOR3(0, 0, 0);
const D3DXVECTOR3 D3DXVECTOR3_X = D3DXVECTOR3(1, 0, 0);
const D3DXVECTOR3 D3DXVECTOR3_Y = D3DXVECTOR3(0, 1, 0);
const D3DXVECTOR3 D3DXVECTOR3_Z = D3DXVECTOR3(0, 0, 1);


typedef struct CVertex_UV
{
	float u;
	float v;
}CVertex_UV;

// �����ʽ
typedef struct CVertex_POSNORMALCOLORCOORD1
{
	//�����λ��
	VECTOR3					position;
	//����ķ���
	VECTOR3					normal;
	//�������ɫ
	DWORD					color;

	//uv����
	CVertex_UV         uv1;
}CVertex_POSNORMALCOLORCOORD1;

// �����ʽ
typedef struct CVertex_POSNORMALCOLORCOORD2
{
	//�����λ��
	VECTOR3					position;
	//����ķ���
	VECTOR3					normal;
	//�������ɫ
	DWORD					color;

	//uv����
	CVertex_UV         uv1;
	CVertex_UV         uv2;
}CVertex_POSNORMALCOLORCOORD2;

// �����ʽ
typedef struct CVertex_POSNORMALCOLORCOORD3
{
	//�����λ��
	VECTOR3					position;
	//����ķ���
	VECTOR3					normal;
	//�������ɫ
	DWORD					color;

	//uv����
	CVertex_UV         uv1;
	CVertex_UV         uv2;
	CVertex_UV         uv3;
}CVertex_POSNORMALCOLORCOORD3;

// �����ʽ
typedef struct CVertex_POSNORMALCOLORCOORD4
{
	//�����λ��
	VECTOR3					position;
	//����ķ���
	VECTOR3					normal;
	//�������ɫ
	DWORD					color;

	//uv����
	CVertex_UV         uv1;
	CVertex_UV         uv2;
	CVertex_UV         uv3;
	CVertex_UV         uv4;
}CVertex_POSNORMALCOLORCOORD4;

typedef struct CVGSFace
{
	UINT a;  // ��������
	UINT b;
	UINT c;
}CVGSFace;

// ��Χ��
typedef struct CVGSBoundBox
{
	VECTOR3					minCorner;
	VECTOR3					maxCorner;
}CVGSBoundBox;            

// ��Χ��
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

	// ����

}CVGSSubMesh;

// ģ��
class VGS_EXPORT CVGSMesh
{
public:
	std::string		   sName;

	//��������
	DWORD               fvf;        // �����ʽ
	UINT			    vertexCnt;	// ������Ŀ 	
	void*		        pVertex;     // ��������

	//������
	UINT				faceCnt;    // ����Ŀ

	UINT				subMeshCount;  // submesh��Ŀ
	void*               pSubMesh;      // submesh����

	//v3.02-�󶨵Ĺ������; v3.03���������ı��
	int						SkeletonAnimationIndex; //bindingSkeletonIndex-v3.02

	//��ͷ����
	int						boneCnt;
	//��Ƥ��Ϣ
	VGS_Bone				bone;

	//��������ͷ��Mesh����ϵ�еı任
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

// ģ��
typedef struct CVGSModelData
{
	CVGSMesh                mesh;
	bool                    isBillboard;
	bool                    isCollision;
	bool                    isGround;
	bool                    isSkyBox; 
	bool                    isVisible;		
	bool                    isClickable;
	bool                    isAutoNormalise;            // ģ������ʱ���Ƿ��Զ����·��� fuxb 2008-09-09
}CVGSModelData;

// ������������ṹS3D
typedef struct CVGS_CAMERA_ANIMINFO
{	
	float                  nearClip;
	float                  farClip;
	//��ͷ
	float					fov;
}CVGS_CAMERA_ANIMINFO;

//Node�ı任
typedef struct CNodeTransform
{
	//֡���
	float					time;   //ʱ��
	//Node�ı任-λ��
	VECTOR3					position;
	//Node�ı任-��ת
	VGS_QUATERNION				quat;
	//Node�ı任-����
	VECTOR3					scale;
}CNodeTransform;

//�ƹ�Ŀɱ����S3D
typedef struct CVGS_LIGHT_INFO
{
	//��ɫ
	DWORD					color;

	//����˥����Χ
	float					range1;
	float					range2;

	//ǿ��
	float					multiply;

	//�ڽ����
	float					theta;
	float					phi;

	VECTOR3                 pos;   // �ƹ��λ�ã�����ڸ��ڵ�
	VECTOR3                 dir;   // �ƹ�ķ�������ڸ��ڵ�

}CVGS_LIGHT_INFO;

#pragma pack(pop)