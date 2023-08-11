/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // S3D_V3.h
* ����ժҪ�� // ���ļ���Max�����S3D�ļ�[3.0]�汾����ӦVGS3DLL
* ����˵���� // S3D[V3]�汾���ļ��ṹ����
* ��ǰ�汾�� // V3.0
* ��    �ߣ� // ����
* ������ڣ� // 
* 
* �޸�����      �汾��     �޸���	      �޸�����
* ---------------------------------------------------------------
* 2007/11/22	V3.0		����		  ��д��� 
*
*********************************************************************/

#ifndef _S3D_H
#define _S3D_H

#include "..\VGSHeader.h"
#include <windows.h>
#include <iostream>   
#include <vector>
#include "..\VGSDef.h"

#pragma pack(push, 1) // ���ݽṹ�Ե��ֽڶ���

//��ά����
// typedef struct vector3
// {
// 	float					x;
// 	float					y;
// 	float					z;
// 	vector3()
// 	{
// 		x = y = z = 0.0f;
// 	}
// 	vector3(float X, float Y, float Z)
// 	{
// 		x = X;
// 		y = Y;
// 		z = Z;
// 	}
// }vector3;
// 
// //��Ԫ��
// typedef struct quaternion
// {
// 	float					x;
// 	float					y;
// 	float					z;
// 	float					w;
// }quaternion;

//typedef VECTOR3 vector3;
//typedef VGS_QUATERNION quaternion;

typedef struct F_Rect
{
	float minX;
	float maxX;
	float minY;
	float maxY;
}F_Rect;

//Node�ı任
typedef struct NodeTransform
{
	//֡���
	DWORD					timeElapsed;
	//Node�ı任-λ��
	vector3					position;
	//Node�ı任-��ת
	quaternion				quat;
	//Node�ı任-����
	vector3					scale;
}NodeTransform;

//Node������
enum NodeType
{
	SCENEROOT = 0,
	CAMERANODE,
	LIGHTNODE,
	MESHNODE,
	BACKDROP,  // ����ͼ�ڵ� fuxb 2008-07-17
	INVALID    // ��Ч�Ľڵ�
};

//�ڵ�
typedef struct VGS_Node
{
	VGS_Node()
	{
		bHasSkeleton = false;
	}
	//Node����
	NodeType				type;
	//Node���ڶ�Ӧ�����б��еı�ţ���������ǵƹ⣬�����ڵƹ��б��еı�ţ���������������������б��еı��
	int						index;

	//����			
	int						strLen;
	std::string				name;

	//�任����
	//Node�Ĺؼ�֡����-�ؼ�֡����
	//float					keyframeCnt;
	//ÿ֡������
	//NodeTransform*		keyframeTransform;
	std::vector<NodeTransform>  keyframeTransform; //V3D�ļ�ֻ�����Ż���Ĺؼ�֡�����������е�֡
	
	//Node��ChildNode����
	//int					childNodeCnt;
	//Node��ChildNode�ı���б�
	//VGS_Node*				childNode;
	std::vector<VGS_Node>	childNode;

	bool bHasSkeleton;

}VGS_Node;

//�����ṹͼ
typedef struct VGS_SceneNodeGraph
{
	//�����е�Node����
	int					nodeCnt;
	//Node�б�
	VGS_Node			node;
}VGS_SceneNodeGraph;

//�����UV
typedef struct VGS_UV
{
	//uv����
	float					u;
	float					v;
}VGS_UV;

//������
typedef struct VGS_Vertex
{
	//�����λ��
	vector3					position;
	//����ķ���
	vector3					normal;
	//�������ɫ
	DWORD					color;

	//UV����Ĳ���, 8λ��ʾ8�㣬1-8��Ŀǰֻ�����Ĳ�
	BYTE					uvFlag; 
	//�����map Channel(1-8) UV�����ݲ���ͼ�����־λ�������п���û��
	VGS_UV					uv1;
	VGS_UV					uv2;
	VGS_UV					uv3;
	VGS_UV					uv4;

	//FLOAT					weight[4];

}VGS_Vertex;

//��
typedef struct VGS_Face
{
	//����������������ģ���еĶ�����
	int						a;
	int						b;
	int						c;
}VGS_Face;

//��ģ��SubMesh
typedef struct VGS_SubMesh
{
	//���ʱ��
	int						mtrlIndex;

	//��Ƭ����
	//int						faceCnt;
	//��Ƭ����[��������]
	//VGS_Face*				face;

	std::vector<VGS_Face>	face;
	
	INT                     boneCount;          // Ӱ���submesh�Ĺ�ͷ��Ŀ
	INT                     boneIndex[64];  // Ӱ���submesh�Ĺ�ͷ�ڹǼ��е�����
	
}VGS_SubMesh;

//��Χ��
typedef struct VGS_BoundBox
{
	D3DXVECTOR3					minCorner;
	D3DXVECTOR3					maxCorner;
}VGS_BoundBox;            

//��Χ��
typedef struct VGS_BoundSphere
{
	D3DXVECTOR3 Center;
	FLOAT		Radius;
}VGS_BoundSphere;

//BillBoard
typedef struct VGS_PointAtVectors
{
	D3DXVECTOR3 PointAtPos;
	D3DXVECTOR3 PointAtUp;
}VGS_PointAtVectors;

//��������������
typedef struct VGS_Bone
{
	//����
	int							strLen;
	std::string					name;

	//��ʼ��̬
	NodeTransform				startPose;

	//��ͷ�󶨵Ķ����б�
	int							bindingVertexCnt;  //�󶨵Ķ�������
	std::vector<DWORD>			bindingVertexIndex;//�󶨵Ķ������б�

	//�Ȩ���б���󶨵Ķ����б�һһ��Ӧ
	std::vector<FLOAT>			bindingWeight;     //�󶨵Ķ���Ȩ��

	//�ӹ�ͷ����
	int							childBoneCnt;
	//�ӹ�ͷ�б�
	std::vector<VGS_Bone>		childBone;

}VGS_Bone;

//ģ��
typedef struct VGS_Mesh
{
	//Node������
	int						strLen;
	std::string 			name;

	//��������
	int						vertexCnt;	
	//��������
	std::vector<VGS_Vertex>	vertex;

	//��Χ��
	BoundBox				boundBox;

	//SubMesh����
	int						subMeshCnt;
	//SubMesh����
	std::vector<VGS_SubMesh> subMesh;

	//v3.02-�󶨵Ĺ������; v3.03���������ı��
	int						SkeletonAnimationIndex; //bindingSkeletonIndex-v3.02

	//3.03�汾��������-start
	//������
	int						faceCnt;

	//��ͷ����
	int						boneCnt;
	//��Ƥ��Ϣ
	VGS_Bone				bone;

	//��������ͷ��Mesh����ϵ�еı任
	NodeTransform			skeletontransform;

	//3.03�汾��������-end

}VGS_Mesh;

//ģ���б�
typedef struct VGS_MeshList
{
	//�����е�Mesh����
	int						meshCnt;
	//Mesh�б�
	std::vector<VGS_Mesh>	mesh;
}VGS_MeshList;

//��ά��
typedef struct VGS_Point
{
	//λ��
	vector3					position;
}VGS_Point;

//��άͼ��
typedef struct VGS_Shape
{
	//Node������
	int						strLen;
	std::string 			name;

	//������
	int						pointCnt;
	//���б�
	std::vector<VGS_Point>	point;
}VGS_Shape;

//��άͼ���б�
typedef struct VGS_ShapeList
{
	//�����е�Shape����
	int						shapeCnt;
	//Shape�б�
	std::vector<VGS_Shape>				shape;
}VGS_ShapeList;

//��ͼ
typedef struct VGS_Texture
{
	//��������ͼ
	int						diffuseTexture_strLen;
	std::string				diffuseTexture;
	//�Է�����ͼ
	int						selfIlluminationTexture_strLen;
	std::string				selfIlluminationTexture;
	//͸����ͼ
	int						opacityTexture_strLen;
	std::string				opacityTexture;
	//������ͼ
	int						reflectionTexture_strLen;
	std::string				reflectionTexture;
	//��͹��ͼ				
	int						bumpTexture_strLen;
	std::string				bumpTexture;
	//�߶���ͼ
	int						displacementTexture_strLen;
	std::string				displacementTexture;

	BYTE					diffuseTexture_uvIndex; //������ͼʹ�õ� UV ͨ����� 0-7
	BYTE					opacityTexture_uvIndex;
	BYTE					reflectionTexture_uvIndex;
	BYTE					bumpTexture_uvIndex;
	BYTE					selfIlluminationTexture_uvIndex;
	BYTE					displacementTexture_uvIndex;

}VGS_Texture;

//����
typedef struct VGS_Mtrl
{
	//��������
	int						strLen;
	std::string				name;

	//���ʵ�Shade����
	int						shaderType;
	
	//�Ƿ�˫����Ⱦ
	bool					bTwoSide;
	
	//���ʵ���ɫ����
	DWORD					ambient;
	DWORD					diffuse;
	DWORD					specular;

	//���ʵĸ߹�ǿ��
	float					specularLevel;
	//���ʵĸ߹����
	float					shininess;

	//�Է���
	float					selfIllumination;

	//͸����
	float					opacity;

	//��ͼ�б�
	VGS_Texture				texture;
}VGS_Mtrl;

//�����б�
typedef struct VGS_MtrlList
{
	//�����еĲ�������
	int						mtrlCnt;
	//�����б�
	std::vector<VGS_Mtrl>				mtrl;
}VGS_MtrlList;

//��ͼ������Ϣ�����ڶ�̬������ͼ
typedef struct VGS_MtrlUseTex
{
	std::string					mtrlName;		//������
	// int							mapChannel;		//��ͼͨ��
	int                         textureLayerIndex;  // texture���ڵĲ�����
	int                         frameIndex;         // ͼ����texture�е�֡���������textureʹ���˶���ͼƬʱ����
	/*VGS_MtrlUseTex(){};
	VGS_MtrlUseTex(const std::string &sMtrName, int iMapChannel)
	{
		mtrlName   = sMtrName;
		mapChannel = iMapChannel;
	}*/
	friend bool operator == (const VGS_MtrlUseTex &LV, const VGS_MtrlUseTex &RV)
	{
		bool b1 = strcmp(LV.mtrlName.c_str(), RV.mtrlName.c_str()) == 0;
		bool b2 = LV.textureLayerIndex == RV.textureLayerIndex;
		bool b3 = LV.frameIndex == RV.frameIndex;
		return (b1 && b2);
	}
}VGS_MtrlUseTex;

typedef struct VGS_TextureDev
{
	std::string					fileName;
	std::vector<VGS_MtrlUseTex>	usedMtrlList;  //ʹ�ø���ͼ�Ĳ����б�
}VGS_TextureDev;


//��������ṹS3D
typedef struct VGS_CAMERA_INFO
{
	//λ��
	vector3					from;
	//Ŀ���
	vector3					at;
	//����ֵ
	float					roll;
	//��ͷ
	float					fov;
}VGS_CAMERA_INFO;

// S3D���
typedef struct VGS_Camera
{
	//���������
	int						strLen;
	std::string 			name;

	//�Ƿ���������
	bool					isOrtho;
	
	//����ؼ�֡������Ϣ
	std::vector<VGS_CAMERA_INFO> camInfo;  //������S3D

}VGS_Camera;


//���
typedef struct VGS_CameraList
{
	//�����е��������
	int						cameraCnt;
	//����б�
	std::vector<VGS_Camera>	camera;
}VGS_CameraList;

//�ƹ�����
enum LIGHTTYPE
{
	OMINI = 0,
	SPOTLIGHT,
	DIRECTIONLIGHT,
	FREESPOTLIGHT,
	TARGETDIRECTIONLIGHT
};

//�ƹ�Ŀɱ����S3D
typedef struct VGS_LIGHT_INFO
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

	vector3                 pos;   // �ƹ��λ�ã�����ڸ��ڵ�
	vector3                 dir;   // �ƹ�ķ�������ڸ��ڵ�

}VGS_LIGHT_INFO;


//�ƹ�
typedef struct VGS_Light
{
	//Node������
	int						strLen;
	std::string 			name;

	//�ƹ�����
	LIGHTTYPE   			type;

	//�ƹ�֡����
	std::vector<VGS_LIGHT_INFO> lightInfo;  //������S3D

}VGS_Light;


//�ƹ��б�
typedef struct VGS_LightList
{
	//�����еĵƹ�����
	int						lightCnt;
	//�ƹ��б�
	std::vector<VGS_Light>				light;
}VGS_LightList;


//��ͷ
typedef struct VGS_BoneAnimation
{
	//����
	int						strLen;
	std::string				name;

	//�������ڸ���ͷ����ϵ�е�ǰ֡��Գ�ʼ��̬�ı任
	std::vector<NodeTransform>	transform;

	//�ӹ�ͷ����
	int						childBoneCnt;
	//�ӹ�ͷ�����б�
	std::vector<VGS_BoneAnimation>	childBoneAnimation;

	~VGS_BoneAnimation()
	{
		transform.clear();
		childBoneAnimation.clear();
	}

}VGS_BoneAnimation;

//��������
typedef struct VGS_SkeletonAnimation
{	
	//����
	int						strLen;
	std::string				name;

	//�ؼ�֡����
	int						frameCnt;
	//��ͷ����
	int						boneCnt;
	//��ͷ�б�
	VGS_BoneAnimation		boneAnimation;

}VGS_SkeletonAnimation;


//�����еĹ��������б�
typedef struct VGS_SkeletonAnimationList
{
	//������������
	int											skeletonAnimationCnt;
	std::vector<VGS_SkeletonAnimation>			skeletonAnimation;

	~VGS_SkeletonAnimationList()
	{
		skeletonAnimation.clear();
	}

}VGS_SkeletonAnimationList;

//S3D�ļ�ͷ
typedef struct VGS_Header
{
	//��ͼ��˾������
	char					corp[32];
	//s3d�ļ��İ汾
	float					s3dVersion;
	
	//�������ݵ�ƫ����
	DWORD					mtrlDataPtr;
	//������ݵ�ƫ����
	DWORD					cameraDataPtr;
	//�ƹ����ݵ�ƫ����
	DWORD					lightDataPtr;
	//�������ݵ�ƫ����
	DWORD					skeletonAnimationDataPtr;
	//ģ�����ݵ�ƫ����
	DWORD					meshDataPtr;
	//ͼ�����ݵ�ƫ����
	DWORD					shapeDataPtr;
	//����ͼ���ݵ�ƫ����
	DWORD					sceneGraphDataPtr;
}VGS_Header;


//S3D�ļ��ṹ
typedef struct Scene_FileStruct
{
	//�ļ�ͷ
	VGS_Header				header;
	//����
	VGS_MtrlList			mtrlManager;
	//���
	VGS_CameraList			cameraManager;
	//�ƹ�
	VGS_LightList			lightManager;

	//����
	VGS_SkeletonAnimationList		skeletonAnimationManager;
	//ģ��
	VGS_MeshList			meshManager;

	//2Dͼ��
	VGS_ShapeList			shapeManager;

	//�����ṹͼ
	VGS_SceneNodeGraph		sceneNodeGraph;	
}Scene_FileStruct;

#pragma pack(pop)

#endif
