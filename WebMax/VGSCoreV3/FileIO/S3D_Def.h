/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // S3D_V3.h
* 内容摘要： // 该文件是Max输出的S3D文件[3.0]版本，对应VGS3DLL
* 其它说明： // S3D[V3]版本的文件结构定义
* 当前版本： // V3.0
* 作    者： // 李欣
* 完成日期： // 
* 
* 修改日期      版本号     修改人	      修改内容
* ---------------------------------------------------------------
* 2007/11/22	V3.0		李欣		  编写框架 
*
*********************************************************************/

#ifndef _S3D_H
#define _S3D_H

#include "..\VGSHeader.h"
#include <windows.h>
#include <iostream>   
#include <vector>
#include "..\VGSDef.h"

#pragma pack(push, 1) // 数据结构以单字节对齐

//三维向量
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
// //四元数
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

//Node的变换
typedef struct NodeTransform
{
	//帧编号
	DWORD					timeElapsed;
	//Node的变换-位置
	vector3					position;
	//Node的变换-旋转
	quaternion				quat;
	//Node的变换-缩放
	vector3					scale;
}NodeTransform;

//Node的类型
enum NodeType
{
	SCENEROOT = 0,
	CAMERANODE,
	LIGHTNODE,
	MESHNODE,
	BACKDROP,  // 背景图节点 fuxb 2008-07-17
	INVALID    // 无效的节点
};

//节点
typedef struct VGS_Node
{
	VGS_Node()
	{
		bHasSkeleton = false;
	}
	//Node类型
	NodeType				type;
	//Node的在对应类型列表中的编号，如果类型是灯光，则是在灯光列表中的编号；如果是相机，则是在相机列表中的编号
	int						index;

	//名称			
	int						strLen;
	std::string				name;

	//变换队列
	//Node的关键帧动画-关键帧数量
	//float					keyframeCnt;
	//每帧的数据
	//NodeTransform*		keyframeTransform;
	std::vector<NodeTransform>  keyframeTransform; //V3D文件只保存优化后的关键帧，不保存所有的帧
	
	//Node的ChildNode数量
	//int					childNodeCnt;
	//Node的ChildNode的编号列表
	//VGS_Node*				childNode;
	std::vector<VGS_Node>	childNode;

	bool bHasSkeleton;

}VGS_Node;

//场景结构图
typedef struct VGS_SceneNodeGraph
{
	//场景中的Node个数
	int					nodeCnt;
	//Node列表
	VGS_Node			node;
}VGS_SceneNodeGraph;

//顶点的UV
typedef struct VGS_UV
{
	//uv坐标
	float					u;
	float					v;
}VGS_UV;

//顶点数
typedef struct VGS_Vertex
{
	//顶点的位置
	vector3					position;
	//顶点的法线
	vector3					normal;
	//顶点的颜色
	DWORD					color;

	//UV坐标的层数, 8位表示8层，1-8，目前只保存四层
	BYTE					uvFlag; 
	//顶点的map Channel(1-8) UV，根据层贴图坐标标志位，部分有可能没有
	VGS_UV					uv1;
	VGS_UV					uv2;
	VGS_UV					uv3;
	VGS_UV					uv4;

	//FLOAT					weight[4];

}VGS_Vertex;

//面
typedef struct VGS_Face
{
	//三个顶点在所属的模型中的顶点编号
	int						a;
	int						b;
	int						c;
}VGS_Face;

//子模型SubMesh
typedef struct VGS_SubMesh
{
	//材质编号
	int						mtrlIndex;

	//面片数量
	//int						faceCnt;
	//面片数据[顶点索引]
	//VGS_Face*				face;

	std::vector<VGS_Face>	face;
	
	INT                     boneCount;          // 影响该submesh的骨头数目
	INT                     boneIndex[64];  // 影响该submesh的骨头在骨架中的索引
	
}VGS_SubMesh;

//包围盒
typedef struct VGS_BoundBox
{
	D3DXVECTOR3					minCorner;
	D3DXVECTOR3					maxCorner;
}VGS_BoundBox;            

//包围球
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

//顶点邦定骨骼的数据
typedef struct VGS_Bone
{
	//名称
	int							strLen;
	std::string					name;

	//初始姿态
	NodeTransform				startPose;

	//骨头绑定的顶点列表
	int							bindingVertexCnt;  //绑定的顶点数量
	std::vector<DWORD>			bindingVertexIndex;//绑定的顶点编号列表

	//邦定权重列表，与绑定的顶点列表一一对应
	std::vector<FLOAT>			bindingWeight;     //绑定的顶点权重

	//子骨头数量
	int							childBoneCnt;
	//子骨头列表
	std::vector<VGS_Bone>		childBone;

}VGS_Bone;

//模型
typedef struct VGS_Mesh
{
	//Node的名称
	int						strLen;
	std::string 			name;

	//顶点数量
	int						vertexCnt;	
	//顶点数据
	std::vector<VGS_Vertex>	vertex;

	//包围盒
	BoundBox				boundBox;

	//SubMesh数量
	int						subMeshCnt;
	//SubMesh数据
	std::vector<VGS_SubMesh> subMesh;

	//v3.02-绑定的骨骼编号; v3.03骨骼动画的编号
	int						SkeletonAnimationIndex; //bindingSkeletonIndex-v3.02

	//3.03版本新增数据-start
	//面数量
	int						faceCnt;

	//骨头数量
	int						boneCnt;
	//蒙皮信息
	VGS_Bone				bone;

	//骨骼根骨头在Mesh坐标系中的变换
	NodeTransform			skeletontransform;

	//3.03版本新增数据-end

}VGS_Mesh;

//模型列表
typedef struct VGS_MeshList
{
	//场景中的Mesh数量
	int						meshCnt;
	//Mesh列表
	std::vector<VGS_Mesh>	mesh;
}VGS_MeshList;

//二维点
typedef struct VGS_Point
{
	//位置
	vector3					position;
}VGS_Point;

//二维图形
typedef struct VGS_Shape
{
	//Node的名称
	int						strLen;
	std::string 			name;

	//点数量
	int						pointCnt;
	//点列表
	std::vector<VGS_Point>	point;
}VGS_Shape;

//二维图形列表
typedef struct VGS_ShapeList
{
	//场景中的Shape数量
	int						shapeCnt;
	//Shape列表
	std::vector<VGS_Shape>				shape;
}VGS_ShapeList;

//贴图
typedef struct VGS_Texture
{
	//漫反射贴图
	int						diffuseTexture_strLen;
	std::string				diffuseTexture;
	//自发光贴图
	int						selfIlluminationTexture_strLen;
	std::string				selfIlluminationTexture;
	//透明贴图
	int						opacityTexture_strLen;
	std::string				opacityTexture;
	//反射贴图
	int						reflectionTexture_strLen;
	std::string				reflectionTexture;
	//凹凸贴图				
	int						bumpTexture_strLen;
	std::string				bumpTexture;
	//高度贴图
	int						displacementTexture_strLen;
	std::string				displacementTexture;

	BYTE					diffuseTexture_uvIndex; //本层贴图使用的 UV 通道编号 0-7
	BYTE					opacityTexture_uvIndex;
	BYTE					reflectionTexture_uvIndex;
	BYTE					bumpTexture_uvIndex;
	BYTE					selfIlluminationTexture_uvIndex;
	BYTE					displacementTexture_uvIndex;

}VGS_Texture;

//材质
typedef struct VGS_Mtrl
{
	//材质名称
	int						strLen;
	std::string				name;

	//材质的Shade类型
	int						shaderType;
	
	//是否双面渲染
	bool					bTwoSide;
	
	//材质的颜色属性
	DWORD					ambient;
	DWORD					diffuse;
	DWORD					specular;

	//材质的高光强度
	float					specularLevel;
	//材质的高光锐度
	float					shininess;

	//自发光
	float					selfIllumination;

	//透明度
	float					opacity;

	//贴图列表
	VGS_Texture				texture;
}VGS_Mtrl;

//材质列表
typedef struct VGS_MtrlList
{
	//场景中的材质数量
	int						mtrlCnt;
	//材质列表
	std::vector<VGS_Mtrl>				mtrl;
}VGS_MtrlList;

//贴图关联信息，用于动态创建贴图
typedef struct VGS_MtrlUseTex
{
	std::string					mtrlName;		//材质名
	// int							mapChannel;		//贴图通道
	int                         textureLayerIndex;  // texture所在的层索引
	int                         frameIndex;         // 图像在texture中的帧索引，如果texture使用了多张图片时有用
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
	std::vector<VGS_MtrlUseTex>	usedMtrlList;  //使用该贴图的材质列表
}VGS_TextureDev;


//相机参数结构S3D
typedef struct VGS_CAMERA_INFO
{
	//位置
	vector3					from;
	//目标点
	vector3					at;
	//翻滚值
	float					roll;
	//镜头
	float					fov;
}VGS_CAMERA_INFO;

// S3D相机
typedef struct VGS_Camera
{
	//相机的名称
	int						strLen;
	std::string 			name;

	//是否是轴侧相机
	bool					isOrtho;
	
	//相机关键帧动画信息
	std::vector<VGS_CAMERA_INFO> camInfo;  //仅用于S3D

}VGS_Camera;


//相机
typedef struct VGS_CameraList
{
	//场景中的相机数量
	int						cameraCnt;
	//相机列表
	std::vector<VGS_Camera>	camera;
}VGS_CameraList;

//灯光类型
enum LIGHTTYPE
{
	OMINI = 0,
	SPOTLIGHT,
	DIRECTIONLIGHT,
	FREESPOTLIGHT,
	TARGETDIRECTIONLIGHT
};

//灯光的可变参数S3D
typedef struct VGS_LIGHT_INFO
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

	vector3                 pos;   // 灯光的位置，相对于父节点
	vector3                 dir;   // 灯光的方向，相对于父节点

}VGS_LIGHT_INFO;


//灯光
typedef struct VGS_Light
{
	//Node的名称
	int						strLen;
	std::string 			name;

	//灯光类型
	LIGHTTYPE   			type;

	//灯光帧数据
	std::vector<VGS_LIGHT_INFO> lightInfo;  //仅用于S3D

}VGS_Light;


//灯光列表
typedef struct VGS_LightList
{
	//场景中的灯光数量
	int						lightCnt;
	//灯光列表
	std::vector<VGS_Light>				light;
}VGS_LightList;


//骨头
typedef struct VGS_BoneAnimation
{
	//名称
	int						strLen;
	std::string				name;

	//动画，在父骨头坐标系中当前帧相对初始姿态的变换
	std::vector<NodeTransform>	transform;

	//子骨头数量
	int						childBoneCnt;
	//子骨头动画列表
	std::vector<VGS_BoneAnimation>	childBoneAnimation;

	~VGS_BoneAnimation()
	{
		transform.clear();
		childBoneAnimation.clear();
	}

}VGS_BoneAnimation;

//骨骼动画
typedef struct VGS_SkeletonAnimation
{	
	//名称
	int						strLen;
	std::string				name;

	//关键帧长度
	int						frameCnt;
	//骨头数量
	int						boneCnt;
	//骨头列表
	VGS_BoneAnimation		boneAnimation;

}VGS_SkeletonAnimation;


//场景中的骨骼动画列表
typedef struct VGS_SkeletonAnimationList
{
	//骨骼动画数量
	int											skeletonAnimationCnt;
	std::vector<VGS_SkeletonAnimation>			skeletonAnimation;

	~VGS_SkeletonAnimationList()
	{
		skeletonAnimation.clear();
	}

}VGS_SkeletonAnimationList;

//S3D文件头
typedef struct VGS_Header
{
	//创图公司的名称
	char					corp[32];
	//s3d文件的版本
	float					s3dVersion;
	
	//材质数据的偏移量
	DWORD					mtrlDataPtr;
	//相机数据的偏移量
	DWORD					cameraDataPtr;
	//灯光数据的偏移量
	DWORD					lightDataPtr;
	//骨骼数据的偏移量
	DWORD					skeletonAnimationDataPtr;
	//模型数据的偏移量
	DWORD					meshDataPtr;
	//图形数据的偏移量
	DWORD					shapeDataPtr;
	//场景图数据的偏移量
	DWORD					sceneGraphDataPtr;
}VGS_Header;


//S3D文件结构
typedef struct Scene_FileStruct
{
	//文件头
	VGS_Header				header;
	//材质
	VGS_MtrlList			mtrlManager;
	//相机
	VGS_CameraList			cameraManager;
	//灯光
	VGS_LightList			lightManager;

	//骨骼
	VGS_SkeletonAnimationList		skeletonAnimationManager;
	//模型
	VGS_MeshList			meshManager;

	//2D图形
	VGS_ShapeList			shapeManager;

	//场景结构图
	VGS_SceneNodeGraph		sceneNodeGraph;	
}Scene_FileStruct;

#pragma pack(pop)

#endif
