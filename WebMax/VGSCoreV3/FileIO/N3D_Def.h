//----------------------------------------------------------------------
// N3D文件相关结构
//----------------------------------------------------------------------

#ifndef _N3D_DEF_H
#define _N3D_DEF_H

#include "..\VGSHeader.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include "S3D_Def.h"

using namespace std;

#pragma pack(push, 1) // 数据结构以单字节对齐

namespace N3D
{
	const char N3D_SUNTOWARD_NAME[] = "SUNTOWARD\0";

	/** 版本更新记录
	   时间              版本号       说明

	1. 2008-10-29之前    51000
	2. 2008-10-29        51001        材质中增加bUseDiffuse，用于标识是否使用当前材质的diffuse颜色
	3. 2008-10-10        51002        N3D_ReflectTextureParam中增加反射贴图的类型，平面还是曲面
	4. 2008-11-21        51003        1. 二维对象增加ZOrder，标识前后关系
	                                  2. 材质保存动态UV
	5. 2008-12-02        51004        1. 保存节点名称
	2. 贴图层增加mipmap
	6. 2008-12-17        51005        1. texture层重新定义，bump和cube贴图增加参数
	7. 2008-01-06        51006        1. 材质中增加实时反射和实时折射层及其参数
	8. 2009-02-23        51007        1. 模型、材质、texturelayer、灯光、相机的名称有原先的32字节定长改为不定长。

	8. 2009-06-16        5200          新引擎格式修改
	                                   1. 增加模型的fvf
	
	*/
	const DWORD N3D_CURRENT_V3DVERSION = 52000;  // 实际的版本号等于 N3D_CURRENT_V3DVERSION / 10000.0，如果51203 -> 5.12.03
	#define _N3D_CUR_FLOATVERSION FLOAT(N3D_CURRENT_V3DVERSION / 10000.0) // 实际的N3D版本号，浮点型
	#define _N3D_FLOATVERSION(ver) FLOAT(ver / 10000.0) // 实际的N3D版本号，浮点型

	const int N3D_MAX_CORPNAME_LENGTH = 64;

	const int N3D_MAX_NAME_LENGTH = 32;


	//三维向量
// 	typedef struct vector3
// 	{
// 		float					x;
// 		float					y;
// 		float					z;
// 	}vector3;

	//四元数
// 	typedef struct quaternion
// 	{
// 		float					x;
// 		float					y;
// 		float					z;
// 		float					w;
// 	}quaternion;

	//Node的变换
	typedef struct N3D_NodeTransform
	{
		//帧编号
		float					time;   //时刻
		//Node的变换-位置
		vector3					position;	
		//Node的变换-旋转
		quaternion				quat;
		//Node的变换-缩放
		vector3					scale;
	}N3D_NodeTransform;

	//Node的类型
	enum N3D_NODETYPE
	{
		NNT_SCENEROOT = 0,
		NNT_CAMERANODE,
		NNT_LIGHTNODE,
		NNT_MODEL,
		NNT_BACKDROP,  // 背景图节点
		NNT_NONE
	};

	//节点
	typedef struct N3D_Node
	{
		//Node类型
		N3D_NODETYPE			type;

		//Node的在对应类型列表中的编号，如果类型是灯光，则是在灯光列表中的编号；如果是相机，则是在相机列表中的编号
		int					index;
		//Add by baojiahui
		DWORD                   ID1;                            //模型的ID;            
		DWORD                   ID2;                            //模型的扩展ID;

		N3D_NodeTransform       Transform;

		//名称			
		/*UINT					strLen;
		std::string				name;*/

		//变换队列
		std::vector<N3D_NodeTransform>  keyframeTransform; //V3D文件只保存优化后的关键帧，不保存所有的帧
		
		std::vector<N3D_Node>	childNode;

	}N3D_Node;

	//场景结构图
	typedef struct N3D_SceneNodeGraph
	{
		//场景中的Node个数
		UINT				nodeCnt;
		//Node列表
		N3D_Node			node;
	}N3D_SceneNodeGraph;

	//顶点的UV
	typedef struct N3D_UV
	{
		//uv坐标
		float					u;
		float					v;
	}N3D_UV;

	//顶点数
	typedef struct N3D_Vertex
	{
		//顶点的位置
		vector3					position;
		//顶点的法线
		vector3					normal;
		//顶点的颜色
		DWORD					color;

		//UV坐标的层数, 8位表示8层，1-8，目前只保存四层
		// BYTE					uvFlag; 

		//顶点的map Channel(1-8) UV，根据层贴图坐标标志位，部分有可能没有
		N3D_UV					uv1;
		N3D_UV					uv2;
		N3D_UV					uv3;
		N3D_UV					uv4;

	}N3D_Vertex;

	//面
	typedef struct N3D_Face_32
	{
		//三个顶点在所属的模型中的顶点编号
		UINT					a;
		UINT					b;
		UINT					c;
	}N3D_Face_32;
   
	typedef struct N3D_Face_16
	{
	    unsigned short                a;
		unsigned short                b;
		unsigned short                c;
	}N3D_Face_16;
	//子模型SubMesh
	typedef struct N3D_SubMesh
	{
		//材质编号
		int					mtrlIndex;   // -1表示没有材质
		UINT                faceCnt;
		//自节点面列表
		VOID*               face;
		//std::vector<N3D_Face>	face;
		
	}N3D_SubMesh;

	//包围盒
	typedef struct N3D_BoundBox
	{
		vector3					minCorner;
		vector3					maxCorner;
	}N3D_BoundBox;            

	//顶点邦定骨骼的数据
	typedef struct N3D_BoneAss
	{
		//顶点编号
		UINT					vertexIndex;
		//邦定的骨头编号
		UINT					bindingBoneIndex;
		//邦定权重
		float					bindingWeight;
	}N3D_BoneAss;

	//Mesh邦定数据
	typedef struct N3D_BindingData
	{
		//邦定的顶点数量
		UINT					bindingVertexCnt;
		//邦定的顶点数据
		N3D_BoneAss*				boneAss;
	}N3D_BindingData;

	//模型
	typedef struct N3D_Model
	{
		//Node的名称
		UINT					strLen;
		std::string 			name;
		//顶点数量
		UINT					vertexCnt;	
		//顶点数据
		N3D_Vertex*				vertex;
		//submesh 
		std::vector<N3D_SubMesh> subMesh;
		//包围盒
		N3D_BoundBox			boundBox;
	    
		float                   boundSphereRadius;
		
		//Add by Baojiahui-----------------------------------------------------------------------------
		bool                    isBillboard;
		bool                    isCollision;
		bool                    isGround;
		bool                    isSkyBox; 
		bool                    isVisible;		
		bool                    isClickable;
		bool                    isAutoNormalise;            // 模型缩放时，是否自动更新法线 fuxb 2008-09-09

		//绑定的骨骼编号
		int					bindingSkeletonIndex;
		//绑定数据
		std::vector<N3D_BoneAss>	bindingData;
		//骨骼根骨头在Mesh坐标系中的变换
		N3D_NodeTransform			skeletontransform;
	}N3D_Model;

	//结构体: 模型组
	typedef struct N3D_ModelGroup
	{
		//Node的名称
		UINT					strLen;
		std::string 			name;

		INT						ModelCnt;			//组中的模型数量
 		INT*					ModelIndex;			//组中的模型编号序列,对应meshlist中的索引号

		std::vector<N3D_ModelGroup*> ModelGroup;
	}N3D_ModelGroup;

	//结构体: 模型组列表 --模型组列表将作为一个scene下的全局vector使用，不再单独定义结构体、

	//模型列表
	typedef struct N3D_ModelList
	{
		//场景中的model数量
		UINT					modelCnt;
		// model列表
		N3D_Model*				model;
	}N3D_ModelList;

	//二维点
	typedef struct N3D_Point
	{
		//位置
		vector3					position;
	}N3D_Point;

	//二维图形
	typedef struct N3D_Shape
	{
		//Node的名称
		UINT					strLen;
		std::string 			name;

		//点数量
		UINT					pointCnt;
		//点列表
		N3D_Point*				point;
	}N3D_Shape;

	//二维图形列表
	typedef struct N3D_ShapeList
	{
		//场景中的Shape数量
		UINT					shapeCnt;
		//Shape列表
		N3D_Shape*				shape;
	}N3D_ShapeList;
	
	// texture的类型
// 	enum N3D_TextureType
// 	{
// 		TLT_INVALID = 0,
// 		TLT_BASAL,						// 基础贴图
// 		TLT_OPACITY,		            // 透明贴图
// 		TLT_REFLECT,                    // 反射贴图
// 		TLT_SPECULAR,                   // 高光贴图
// 		TLT_CUBEMAP,                    // 立方图
// 		TLT_DYNAMIC,                    // 动态贴图
// 		TLT_BUMPMAP,                  // 水特效
// 		TLT_MOVIE,                      // 电影贴图
// 		TLT_FLASH,						// Flash贴图
// 		TLT_REALTIMEREF                 // 实时反射
// 	};
	// texture的类型
	typedef VGS_TextureType N3D_TextureType;

	// 基础贴图参数
	typedef struct N3D_BasalTextureParam
	{
		INT width;
		INT height;
	}N3D_BasalTextureParam;

	// 透明贴图参数
	typedef struct N3D_OpacityTextureParam
	{
		INT width;
		INT height;
	}N3D_OpacityTextureParam;

	// 反射贴图参数
	typedef struct N3D_ReflectTextureParam
	{
		INT width;
		INT height;
	}N3D_ReflectTextureParam;
	typedef struct N3D_ReflectTextureParam51002
	{
		INT width;
		INT height;
		char reflectionType;  // fuxb 2008-11-10 反射类型, -1:无效，0：平面，1：曲面
	}N3D_ReflectTextureParam51002;

	// 立方贴图参数
	typedef struct N3D_CubicTextureParam
	{
		INT width;
		INT height;
	}N3D_CubicTextureParam;

	// 动态贴图参数
	typedef struct N3D_DynamicTextureParam
	{
		INT width;
		INT height;
		INT duration;
	}N3D_DynamicTextureParam;

	// bumpwater参数
	typedef struct N3D_BumpwaterTextureParam
	{
		INT width;
		INT height;
		FLOAT			XScale;					//水波纹的X方向密度
		FLOAT			YScale;					//水波纹的Y方向密度
		FLOAT			Radius;					//波长
		FLOAT			Speed;					//波速
		FLOAT           CenterX;	// 波纹的中心点坐标
		FLOAT           CenterY;
	}N3D_BumpwaterTextureParam;

	// bump环境反射
	typedef struct N3D_BumpEnvTextureParam
	{
		INT width;
		INT height;
		float   noise1;
		float   noise2;
	}N3D_BumpEnvTextureParam;
	
	// 法线贴图
	typedef struct N3D_BumpNormalTextureParam
	{
		INT   LightIndex;
	}N3D_BumpNormalTextureParam;

	typedef struct N3D_BumpTextureParam
	{
		BYTE bumpType;  // 0 : water, 1: environment 2: normal map
		union
		{
			N3D_BumpwaterTextureParam   water;
			N3D_BumpEnvTextureParam     evn;
			N3D_BumpNormalTextureParam  normal;
			char reserved[31];
		};
	}N3D_BumpTextureParam;

	// 电影贴图参数
	typedef struct N3D_MovieTextureParam
	{
		INT duration;
	}N3D_MovieTextureParam;

	// flash贴图参数
	typedef struct N3D_FlashTextureParam
	{
		INT duration;
	}N3D_FlashTextureParam;

	typedef struct N3D_RTReflectionParam  // 实时反射
	{
		INT width;
		INT height;
// 		INT modelIndex;
// 		INT refModleCount;
// 		DWORD* vRefModels;
	}N3D_RTReflectionParam;

	typedef N3D_RTReflectionParam N3D_RTRefractionParam;  // 实时折射
	// 保存所有类型参数
	typedef union N3D_TextureParam
	{
		N3D_BasalTextureParam			BasalTextureParam;
		N3D_OpacityTextureParam			OpacityTextureParam;
		N3D_ReflectTextureParam			ReflectTextureParam;
		N3D_ReflectTextureParam51002	ReflectTextureParam51002;
		N3D_CubicTextureParam			CubicTextureParam;
		N3D_DynamicTextureParam			DynamicTextureParam;
		N3D_BumpwaterTextureParam		BumpwaterTextureParam;
		N3D_MovieTextureParam			MovieTextureParam;
		N3D_FlashTextureParam			FlashTextureParam;
		N3D_RTReflectionParam			RTReflectionParam;
		N3D_RTRefractionParam			RTRefractionParam;
		char data[32];
	}N3D_TextureParam;

	//图片对象
	typedef struct N3D_ImageInfo
	{
		//SHORT                   Width;                   // 贴图宽度
		//SHORT                   Height;                  // 贴图高度
		//UINT                    MipCount;                // 贴图的mipmap数量

		UINT                    nameLen;
		char*                   name;
		UINT                    DataLen;
		char*                   Data;                    // 贴图数据
	}N3D_ImageInfo;

	//typedef struct N3D_Image
	//{
	//	UINT                    image_strLen;          // 贴图名称长度
	//	std::string             imageName;             // 贴图名称
	//}N3D_Image;

	//图片对象名称
	typedef struct N3D_ImageList
	{
	   UINT					imageCnt;          // Image的数量
	   N3D_ImageInfo*       imageInfoList;         // Image对象火数组指针
	}N3D_ImageList;

//压缩后的Image单元参数
typedef struct N3D_5_1_ImageInfo
{
	CHAR			Name[32];		//名称
	DWORD			Size;			//压缩后的Image数据大小
}N3D_5_1_ImageInfo;

//下载http上的N3D文件中的Image，然后保存到内存中的Image单元数据结构
typedef struct N3D_5_1_ZipImageData
{
	
    N3D_5_1_ImageInfo   imageInfo;      //image参数
	BYTE*			    Data;			//压缩后的Image数据
}N3D_5_1_ZipImageData;

	// 混色类型
	//enum N3D_BlenderType
	//{
	//	NBT_COLOR = 0,
	//	NBT_ALPHA
	//};

	// 混色方式
	enum N3D_BlenderOP
	{
		NBM_REPLACE = 0,  // 使用source1
		NBM_DISABLE,      // 用source2替换source1
		NBM_MODULATE,     // source2 * source1
		NBM_MODULATE2,    // NBM_MODULATE * 2
		NBM_MODULATE4,    // NBM_MODULATE * 4	
		NBM_ADD,          // source2 + source1        
        NBM_ADD_SIGNED,	/// as LBX_ADD, but subtract 0.5 from the result        
        NBM_ADD_SMOOTH,	/// as LBX_ADD, but subtract product from the sum
        NBM_SUBTRACT,	  // source1 - source2
		NBM_BLEND_DIFFUSE_ALPHA,		/// use interpolated alpha value from vertices to scale source1, then add source2 scaled by (1-alpha)
        NBM_BLEND_TEXTURE_ALPHA,		/// as LBX_BLEND_DIFFUSE_ALPHA, but use alpha from texture        
        NBM_BLEND_CURRENT_ALPHA,		/// as LBX_BLEND_DIFFUSE_ALPHA, but use current alpha from previous stages       
        NBM_BLEND_MANUAL,			/// as LBX_BLEND_DIFFUSE_ALPHA but use a constant manual blend value (0.0-1.0)        
        NBM_DOTPRODUCT,					/// dotproduct of color1 and color2         
        NBM_BLEND_DIFFUSE_COLOUR	/// use interpolated color values from vertices to scale source1, then add source2 scaled by (1-color)        
        
	};

	/* enum N3D_LayerBlendSource
    {
        /// the colour as built up from previous stages
        NBS_CURRENT,
        /// the colour derived from the texture assigned to this layer
        NBS_TEXTURE,
        /// the interpolated diffuse colour from the vertices
        NBS_DIFFUSE,
        /// the interpolated specular colour from the vertices
        NBS_SPECULAR,
        /// a colour supplied manually as a separate argument
        NBS_MANUAL
    };*/

	// 混色参数
	typedef struct N3D_BlenderParam
	{
		/*N3D_LayerBlendSource source1;
		N3D_LayerBlendSource source2;*/
		float BlenderOP_alpha;  // 当N3D_BlenderOP == NBM_BLEND_MANUAL时有效，混色模式中用到的参数alpha [0, 1],

	}N3D_BlenderParam;

	// texturelayer的混色信息
	typedef struct N3D_BlenderInfo
	{
		// N3D_BlenderType		type;         // 混色模式 0 : color, 1: alpha
		N3D_BlenderOP			ColorBlendOption;       // 颜色混色方式
		N3D_BlenderParam		ColorBlendParam;        // 颜色混色参数

		N3D_BlenderOP			AlphaBlendOption;       // alpha混色方式
		N3D_BlenderParam		AlphaBlendParam;        // alpha混色参数

	}N3D_BlenderInfo;
    //Edit by Baojiahui 2008/8/6
	//贴图
	typedef struct N3D_TextureLayer
	{
		// CHAR					Name[N3D_MAX_NAME_LENGTH];     // 贴图名称 Edit by Baojiahui for nonfinite of the texture name String length /2009/2/24
		
		N3D_TextureType         type;                          // 贴图类型, 如果type == TLT_INVALID, 后面不填写内容
		int						UVIndex;                       // 使用的UV索引

		BYTE                     Mipmap;                       // fuxb 2008-12-12 增加mipmap

		N3D_TextureParam        TextureParam;                 // 保存各种类型texture的参数

		N3D_BlenderInfo         Blenderinfo;                   // 混色参数
		UINT					ImageCount;                    // 使用的贴图数目
		int*                    ImageIdxList;                  // 贴图列表
	}N3D_TextureLayer;

	//贴图列表
	typedef struct N3D_TextureList
	{
	   UINT							textureCnt;
	   N3D_TextureLayer*			texture;
	}N3D_TextureList;

	// 材质基本信息
	typedef struct N3D_MtrlBasalInfo
	{ 				
		//CHAR					Name[N3D_MAX_NAME_LENGTH];Edit by Baojiahui for nonfinite of the texture name String length 2009/2/24
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

		//材质的Shade类型，用于光照选择
		int						shaderType;	// 0 : SO_FLAT, 1: SO_GOURAUD, 2: SO_PHONG 
		//Add by baojiahui 2008/8/8
		//材质的填充模式
		int                     fillMode;
		//是否双面渲染
		bool					bTwoSide;	
		//是否透明测试
		bool                    alphaTest;         
		//透明测试门限参数[0，255]
		BYTE                    alphaTestValue;
		//是否接受阴影
		bool                    bRecShadow;

	}N3D_MtrlBasalInfo;

	//关键帧材质动画信息
	typedef struct N3D_MtrlAminInfo
	{
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

	}N3D_MtrlAminInfo;

	//材质
	typedef struct N3D_MtrlInfo
	{		
		N3D_MtrlBasalInfo       BasalInfo;  // 材质的基本参数
		bool                    bUseDiffuse;  // fuxb 2008-10-29 是否使用材质的diffuse颜色，如果false，等效于材质的diffuse为白色

		//关键帧信息，如果不存在关键帧，KfrCnt为0
		UINT                    kfrCnt;
		N3D_MtrlAminInfo*       kfrMtrlInfo;

		// texture列表
		UINT					TextureLayerCount;
		N3D_TextureLayer*		TextureLayerList;  // 在texture列表中的索引

	}N3D_MtrlInfo;

	//材质列表
	typedef struct N3D_MtrlList
	{
		//场景中的材质数量
		int							mtrlCnt;

		//材质列表
		N3D_MtrlInfo*				mtrl;

	}N3D_MtrlList;

	//贴图关联信息，用于动态创建贴图
	typedef struct N3D_MtrlUseTex
	{
		std::string					mtrlName;		//材质名
		int							mapChannel;		//贴图通道
		friend bool operator == (const N3D_MtrlUseTex &LV, const N3D_MtrlUseTex &RV)
		{
			bool b1 = strcmp(LV.mtrlName.c_str(), RV.mtrlName.c_str()) == 0;
			bool b2 = LV.mapChannel == RV.mapChannel;
			return (b1 && b2);
		}
	}N3D_MtrlUseTex;

	typedef struct N3D_TextureDev
	{
		std::string					fileName;
		std::vector<N3D_MtrlUseTex>	usedMtrlList;  //使用该贴图的材质列表
	}N3D_TextureDev;

	//玩家模型骨骼的关联信息，用于创建玩家
	typedef struct N3D_SkeletonModelDev
	{
		std::string					modelName;						//模型名字
		std::string					skeletonName;					//骨骼名字
	}N3D_SkeletonModelDev;

	////相机参数结构S3D
	//typedef struct N3D_CAMERA_INFO
	//{
	//	//位置
	//	vector3					from;
	//	//目标点
	//	vector3					at;
	//	//翻滚值
	//	float					roll;
	//	//镜头
	//	float					fov;
	//}N3D_CAMERA_INFO;

	//相机参数结构V3D，位置和转角交给Node
	typedef struct N3D_CAMINFO
	{
		//绝对的From 和At
		//vector3					from;
		//vector3					at;
		

		//镜头
		float					fov;
		//相机的最近最远剪切面
		float					nearClipDis;
		float					farClipDis;
	}N3D_CAMINFO;

	// 相机参数结构, N3D的相机动画中保存的相机参数，暂时没有用
	typedef struct N3D_CAMPARAM
	{
		//是否是轴侧相机
		bool					isOrtho;
		
		//镜头
		float					fov;
		
		//相机的最近最远剪切面
		float					nearClipDis;
		float					farClipDis;
		
		vector3                 position;      // 位置  
		quaternion              quat;          // 转角
		
	}N3D_CAMPARAM;

	//typedef struct N3D_Camera
	//{
 //  		//相机的名称
	//	int						strLen;
	//	std::string 			name;

	//	//是否是轴侧相机
	//	bool					isOrtho;
	//	//镜头
	//	float					fov;
	//	//相机的最近最远剪切面
	//	float					nearClipDis;
	//	float					farClipDis;
	//}N3D_Camera;

	//相机
	typedef struct N3D_Camera
	{
		//相机的名称
		/*int						strLen;
		std::string 			name;*/
		// char					name[N3D_MAX_NAME_LENGTH];//Edit by Baojiahui for nonfinite of the camera name String length 2009/2/24

		//是否是轴侧相机
		bool					isOrtho;
		
		//相机关键帧动画信息
		//std::vector<N3D_CAMERA_INFO> camInfo;  //仅用于S3D

		//相机初始信息
		N3D_CAMINFO				BaseInfo;			//仅用于V3D
		//std::vector<N3D_N3D_CAMINFO>	v3dAnimationInfo;		//仅用于V3D

	}N3D_Camera;

	//相机
	typedef struct N3D_CameraList
	{
		//场景中的相机数量
		int						cameraCnt;
		//相机列表
		N3D_Camera*				camera;
	}N3D_CameraList;

	//灯光类型
	enum N3D_LIGHTTYPE
	{
		NLT_OMINI = 0,
		NLT_SPOTLIGHT,
		NLT_DIRECTIONLIGHT,
		NLT_FREESPOTLIGHT,
		NLT_TARGETDIRECTIONLIGHT
	};

	//灯光的可变参数S3D
	typedef struct N3D_LIGHT_INFO
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

	}N3D_LIGHT_INFO;


	//灯光的可变参数V3D
	typedef struct N3D_OMNI
	{
		//颜色
		DWORD					color;
		//强度
		float					multiply;
		
		//光线衰减范围
		float					attenuationRange;
		float					attenuation0;
		float					attenuation1;
		float					attenuation2;
	}N3D_OMNI;

	//灯光的可变参数V3D
	typedef struct N3D_SPOTLIGHT
	{
		//颜色
		DWORD					color;
		//强度
		float					multiply;
		
		//光线的方向, 世界坐标系
		vector3					direction;

		//光线衰减范围
		float					attenuationRange;
		float					attenuation0;
		float					attenuation1;
		float					attenuation2;

		//光圈区域的范围角
		float					innerAngle; // theta
		float					outerAngle; // phi
		float					fallOff;
	}N3D_SPOTLIGHT;


	//灯光的可变参数V3D
	typedef struct N3D_DIRECTIONAL
	{
		//颜色
		DWORD					color;
		//强度
		float					multiply;
		
		//光线的方向, 世界坐标系
		vector3					direction;

		//光线衰减范围
		float					attenuationRange;
		float					attenuation0;
		float					attenuation1;
		float					attenuation2;

	}N3D_DIRECTIONAL;


	//灯光
	typedef struct N3D_Light
	{
		//Node的名称
		/*int						strLen;
		std::string 			name;*/
		// char                    name[N3D_MAX_NAME_LENGTH];//Edit by Baojiahui for nonfinite of the Light name String length 2009/2/24

		//灯光类型
		N3D_LIGHTTYPE   		 type;
		N3D_OMNI *               pOmniLight;           // 如果type!=NLT_OMINI, pOmniLight = NULL
		N3D_DIRECTIONAL*         pDirectionalLight;    // 如果type!=NLT_DIRECTIONLIGHT, pDirectionalLight = NULL
		N3D_SPOTLIGHT*           pSpotLightInfo;       // 如果type!=NLT_SPOTLIGHT, pSpotLightInfo = NULL

		int                      FrameCount;               // 动画帧数
		N3D_OMNI *               pAnimOmniLight;           // 如果type!=NLT_OMINI, pOmniLight = NULL
		N3D_DIRECTIONAL*         pAnimDirectionalLight;    // 如果type!=NLT_DIRECTIONLIGHT, pDirectionalLight = NULL
		N3D_SPOTLIGHT*           pAnimSpotLightInfo;       // 如果type!=NLT_SPOTLIGHT, pSpotLightInfo = NULL

		////灯光帧数据
		//std::vector<N3D_LIGHT_INFO>        vLightInfo;  //仅用于S3D

		////V3D保存的灯光参数帧数据
		//std::vector<N3D_OMNI>				vOmniInfo;
		//std::vector<N3D_DIRECTIONAL>		vDirectionLightInfo;
		//std::vector<N3D_SPOTLIGHT>			vSpotLightInfo;

	}N3D_Light;

	//灯光列表
	typedef struct N3D_LightList
	{
		//场景中的灯光数量
		int						lightCnt;
		//灯光列表
		N3D_Light*				light;
	}N3D_LightList;

	//骨头
	typedef struct N3D_Bone
	{
		//名称
		int						strLen;
		std::string				name;

		//初始姿态
		N3D_NodeTransform			pose;

		//动画，在父骨头坐标系中当前帧相对初始姿态的变换
		std::vector<N3D_NodeTransform>	transform;

		//子骨头数量
		//int					childBoneCnt;
		//子骨头列表
		//N3D_Bone*				childBone;
		
		std::vector<N3D_Bone>	childBone;
	}N3D_Bone;

	//骨骼动画
	typedef struct N3D_SkeletonAnimation
	{	
		//名称
		int						strLen;
		std::string				name;

		//关键帧长度
		int						frameCnt;
		//骨头数量
		int						boneCnt;
		//骨头列表
		N3D_Bone				bone;
	}N3D_SkeletonAnimation;

	//骨骼
	typedef struct N3D_Skeleton
	{
		//名称
		int						strLen;
		std::string				name;
		
		//骨骼包含的动画列表
		int						animationCnt;
		std::vector<N3D_SkeletonAnimation*>   animationList;
	}N3D_Skeleton;

	//场景中的骨骼列表
	typedef struct N3D_SkeletonList
	{
		//骨骼数量
		int						skeletonCnt;
		N3D_Skeleton*			skeleton;
	}N3D_SkeletonList;



	//结构体: 声音数据结构
	typedef struct N3D_Audio
	{
		int						strLen;
		std::string				name;
		INT						fileNameLen;            //音频文件路径长度
		std::string				fileName;		    	//音频文件全路径
		BYTE					type;					//文件类型
		INT						repeatCnt;				//重播次数
	}N3D_Audio;

	//音频列表
	typedef struct N3D_AudioList
	{
		INT				AudioCnt;			//音频的数量
		N3D_Audio*	    Audio;				//音频列表
	}N3D_AudioList;

	//结构体: Flash数据结构
	typedef struct N3D_Flash
	{
		CHAR			Name[32];			//Flash对象名称
		INT             FileNameLen;        //Flash文件路径长度
		CHAR*			pFileName;		    //Flash文件全路径
		
		INT				OffsetOrigin;		//偏移位置的坐标原点标示
		BOOL			UseRelativePos;		//是否使用相对位置

		RECT			Rect;				//Flash 尺寸
		FLOAT			Angle;				//转角
		INT				Opacity;			//透明度
		BOOL			Transparency;		//是否抠背景

		BOOL			UseFlashSize;		//使用Flash的尺寸
	}N3D_Flash;

	//结构体: Flash列表
	typedef struct N3D_FlashList
	{		
		INT				FlashCnt;			//Flash数量
		N3D_Flash*	    Flash;				//Flash列表
	}N3D_FlashList;

	//结构体：文字信息
	typedef struct N3D_TextInfo
	{
		DWORD	Color;
		INT			X;
		INT			Y;
	}N3D_TextInfo;

	//结构体: 文字基本结构
	typedef struct N3D_Text
	{
		INT						Length;		//字符数量
		CHAR*					StrText;	//字符串

		INT						X;
		INT						Y;			//文字的位置
		DWORD				Color;		//文字的颜色

		CHAR					FontName[32];	//字体名
		
		UINT					KfrCnt;		//关键帧数量
		N3D_TextInfo*		KfrInfo;	//关键帧数据
	}N3D_Text;

	//结构体: 文字列表
	typedef struct N3D_TextList
	{
		INT			TextCnt;	//文字数量
		N3D_Text*	Texts;		//文字列表
	}N3D_TextList;

	//结构体：Overlay信息
	typedef struct N3D_OverlayFrameInfo
	{
		RECT		Rect;			//Overlay设定的相对尺寸
		FLOAT		Angle;			//转角
		INT			Opacity;		//透明度
	}N3D_OverlayFrameInfo;

	typedef struct N3D_OverlayBasalInfo
	{
		CHAR		Name[N3D_MAX_NAME_LENGTH];			// Overlay的名字

		INT			OffsetOrigin;		// 偏移位置的坐标原点
		BOOL		UseRelativePos;		// 使用相对位置
		BOOL		UseRelativeSize;	// 是否使用相对的尺寸

		RECT		Rect;				// Overlay尺寸及位置
		FLOAT		Angle;				// 转角
		INT			Opacity;			// 透明度
		BOOL		IsBackdrop;			// 是否是背景图

		INT			MaterialIndex;		// Overlay的材质Index

		BOOL        IsVisible;          // 是否可见
	}N3D_OverlayBasalInfo;

	//结构体: Overlay基本结构
	typedef struct N3D_Overlay
	{
		N3D_OverlayBasalInfo	BasalInfo;  // 基本信息

	
		UINT					 KfrCnt;		// 关键帧数量
		N3D_OverlayFrameInfo*	 KfrInfo;	// 关键帧信息

		INT                      zOrder;    // overlay的z顺序

	}N3D_Overlay;

	//结构体: Overlay列表
	typedef struct N3D_OverlayList
	{
		INT				OverlayCnt;	//Overlay的数量
		N3D_Overlay*	Overlays;	//Overlay列表
	}N3D_OverlayList;

	//导航图指针信息
	typedef struct N3D_NavigaterPointer
	{
		CHAR		Name[32];			//Pointer的名字
		UINT		OverlayIndex;		//Pointer使用的Overlay的编号
		UINT		Type;				//Pointer的类型，Camera-0, Model-1
		INT			ObjIndex;			//相机或模型的编号
	}N3D_NavigaterPointer;

	typedef struct N3D_NavigaterBasalInfo
	{
		CHAR						Name[32];
		RECT						Rect;						// 导航Map平面区域

		int						    MapOverlayIndex;			// 代表Map(Overlay)的编号

		FLOAT						sceneLeft;					// 场景TopView的Rect
		FLOAT						sceneTop;
		FLOAT						sceneRight;
		FLOAT						sceneBottom;	
 
		//bool						UseRelativePos;    // 是否使用相对位置
		//bool						UseRelativeSize;   // 是否使用相对大小
	}N3D_NavigaterBasalInfo;

	//结构体: Navigater基本结构
	typedef struct N3D_Navigater
	{
		N3D_NavigaterBasalInfo	    BasalInfo;               // 基本信息

		UINT						PointerCnt;					// Pointer的数量
		N3D_NavigaterPointer*		NavPointer;					// Pointer信息

	}N3D_Navigater;

	//结构体: Navigater列表
	typedef struct N3D_NavigaterList
	{
		INT						    NavigaterCnt;	//Navigater数量
		N3D_Navigater*			    Navigaters;		//Navigater列表
	}N3D_NavigaterList;

	//结构体: Button的关键帧数据
	typedef struct N3D_ButtonFrameInfo
	{
		RECT		Rect;				//按钮的Rect
		INT			Opacity;			//按钮的透明度
	}N3D_ButtonFrameInfo;

	// 按钮基本参数
	typedef struct N3D_ButtonBasalInfo
	{
		CHAR		Name[32];			//按钮的名称
		RECT		Rect;				//按钮的Rect
		INT			Opacity;			//按钮的透明度
		CHAR		CallBackFunc[32];	//回调函数名称
		UINT		BtnOverlayIndex;	//Button Up的ID号
		UINT		MouseOverMtrlIndex;	//overImage的材质ID号
		UINT		MouseDownMtrlIndex;	//downImage的材质ID号

	}N3D_ButtonBasalInfo;

	//结构体: Button单元结构
	typedef struct N3D_Button
	{
		N3D_ButtonBasalInfo     BasalInfo;  // 按钮基本参数

		UINT		            KfrCnt;		// 关键帧数量
		N3D_ButtonFrameInfo*	KfrInfo;	// 关键帧数据
	}N3D_Button;

	//结构体: Button列表
	typedef struct N3D_ButtonList
	{
		INT					ButtonCnt;			//按钮个数
		N3D_Button*		Buttons;			//按钮列表
	}N3D_ButtonList;

	//结构体: N3D_1_0授权结构体
	typedef struct N3D_Privacy
	{
		INT				VersionType;			//版本标示(个人版本0，标准版本1，企业版本2)
		DWORD			ClientIndex;			//授权客户编号
		CHAR			SerialNum[128];			//被授权的软件序列号，已经过128位加密
		CHAR			Authorize[128];			//是否已经被授权去广告的序列号
		DWORD			ExtendInfo1;			//扩展加密信息1
		DWORD			ExtendInfo2;			//扩展加密信息2
	}N3D_Privacy;

	//结构体: Walker对象的结构体
	typedef struct N3D_Walker
	{
		CHAR			Name[32];		//名称
		INT				CameraIndex;	//使用的相机编号

		FLOAT			walkSpeed;		//Walker摄像机的行走速度
		FLOAT			walkAcc;		//Walker摄像机的行走加速度

		FLOAT			rosSpeed;		//Walker摄像机的旋转速度
		FLOAT			rosAcc;			//Walker摄像机的旋转加速度

		FLOAT			height;			//Walker的高度

		BOOL			bCollision;		//是否有碰撞
		FLOAT			colDis;			//Walker的碰撞距离

		BOOL			bWeight;		//是否有重力
		FLOAT			liftSpeed;		//提升下降的速度，失去重力时有效
		FLOAT			liftAcc;		//提升下降的加速度，失去重力时有效

		FLOAT			upperAngle;		//抬头的最大仰角
		FLOAT			lowerAngle;		//低头的最大俯视角
	}N3D_Walker;

	//结构体: walker对象列表
	typedef struct N3D_WalkerList
	{
		INT				WalkerCnt;			//Walker的数量
		N3D_Walker* Walkers;			//Walkder对象列表
	}N3D_WalkerList;

	//结构体: Editor对象
	typedef struct N3D_Editor
	{	
		CHAR				Name[32];		//名称

		INT					CameraIndex;	//使用的相机编号

		BOOL		bIsAutoRoundRoll;					//Edit模式下是否自动翻滚

		FLOAT		autoSpeedX;						    // Edit模式下自动绕 X 轴旋转的速度
		FLOAT		autoSpeedY;						    // Edit模式下自动绕 Y 轴旋转的速度

		FLOAT		manualSpeedX;					//Edit模式下手动旋转相机的速度绕 X 轴
		FLOAT		manualSpeedY;					//Edit模式下手动翻滚相机的速度绕 Y 轴

		FLOAT		manualPanXSpeed;					//平移的X方向速度
		FLOAT		manualPanYSpeed;					//平移的Y方向速度

		FLOAT		dollySpeed;							//滚鼠标滚轮时移动相机的速度
		FLOAT		dollyAcc;							//相机推拉加速度

		FLOAT		dollyNearDis;						//最近的Dolly
		FLOAT		dollyFarDis;						//最远的Dolly

		INT			rosAxis;							// 旋转轴标志位, 0: xy, 1 : x, 2 : y
		vector3     LookAtPos;                          // Editor的目标点 

	}N3D_Editor;

	//结构体：Editor对象列表
	typedef struct N3D_EditorList
	{
		INT					EditorCnt;							//Editor的数量
		N3D_Editor*		Editors;							//Editor列表
	}N3D_EditorList;

	//结构体: Flyer对象
	typedef struct N3D_Flyer
	{
		CHAR				Name[32];		//名称

		INT					CameraIndex;					//使用的相机编号

		FLOAT				panSpeed;						//平移速度
		FLOAT				panAcc;							//平移加速度

		FLOAT				dollySpeed;						//相机推拉速度
		FLOAT				dollyAcc;						//相机推拉加速度

		FLOAT				dollyNearDis;					//最近的Dolly
		FLOAT				dollyFarDis;					//最远的Dolly

		FLOAT				rotateSpeed;					//相机旋转的速度
		FLOAT				rotateAcc;						//相机旋转的加速度
		
		FLOAT				liftSpeed;						//上升下降的速度，失去重力时有效
		FLOAT				liftAcc;						//上升下降的加速度，失去重力时有效

		FLOAT				upperAngle;						//抬头的最大仰角
		FLOAT				lowerAngle;						//低头的最大俯视角

		BOOL				bCollision;						//是否有碰撞
		FLOAT				colDis;							//碰撞距离
	}N3D_Flyer;

	//结构体: Flyer对象列表
	typedef struct N3D_FlyerList
	{
		INT					FlyerCnt;						//飞翔对象数量
		N3D_Flyer*		Flyers;							//飞翔对象列表
	}N3D_FlyerList;

	//结构体：控制对象
	typedef struct N3D_ControlData
	{
		INT					ControlType;					//控制类型	CT_WALKER = 0, CT_EDITOR = 1, CT_FLYER = 2,	CT_NONE 非交互模式 
		INT					ControlerIndex;					//控制对象编号

		N3D_WalkerList	WalkerList;						//漫游对象列表
		N3D_EditorList	EditorList;						//编辑对象列表
		N3D_FlyerList	FlyerList;						//飞行对象列表
	}N3D_ControlData;

	//结构体：场景的属性
	typedef struct N3D_SceneProp
	{
		WORD				Width;				//窗体宽度
		WORD				Height;				//窗体高度
		DWORD				BackColor;			//背景颜色
		WORD				FPS;				//渲染帧率
		BYTE				AntiAliasLevel;		//抗拒齿级别
		//INT				FSBlurLevel;		//全屏动态模糊的级别
		//INT				FSGlowLevel;		//全屏光晕的级别

		//INT				KeyframeCnt;		//关键帧的数量
		//INT				RepeatCnt;			//循环播放的次数， -1为无限次循环

		char				Reserved[19];       // 预留字段

	}N3D_SceneProp;

	
	//Add for fast write the V3D file

	// 用于实现 块 写入v3d文件
	typedef struct N3D_CameraDetail 
	{
		BYTE       PrjType;
		float      FromX;
		float      FromY;
		float      FromZ;
		float      OrientW;
		float      OrientX;
		float      OrientY;
		float      OrientZ;
		float      DirectionX;
		float      DirectionY;
		float      DirectionZ;
		BYTE       PolygonMode;
		float      fFOVY;
		float      nearClip; 
		float      farClip; 
	}N3D_CameraDetail;

	// 用于实现 块 写入v3d文件
	typedef struct N3D_LightDetail
	{
		BYTE  type;
		DWORD Dif_RGBA;
		DWORD Spe_RGBA;
		float fAttenuationRange;
		float fAttenuationConst;
		float fAttenuationLinear;
		float fAttenuationQuadric;
		float PositionX;
		float PositionY;
		float PositionZ;
		float DirectionX;
		float DirectionY;
		float DirectionZ;

	}N3D_LightDetail;


	// 授权结构体 32 个字节
	typedef struct N3D_RegInfo
	{
		DWORD			CorpID;				// 被授权的公司标识
		DWORD			Date1;				// 授权的日期参数
		DWORD			Date2;				// 授权的日期参数
		DWORD           Reserved[5];        // 预留位置
	}N3D_RegInfo;

	// 结构体: N3D文件头结构 96个字节
	typedef struct N3D_Header
	{	
		DWORD           Version;               // 版本号，真正的版本号等于 Version / 10000.0
		N3D_RegInfo	    RegInfo;			   // 注册信息

		DWORD			SceneDataPtr;		    // Zip 后 Scene数据在文件中的起始位置
		DWORD			SceneZipDataLen;	    // Zip 后 Scene数据长度，不包括image
		DWORD           OriginSceneDataLen;     // Zip 前 Scene数据长度    

		DWORD			ImageDataPtr;			// 图像数据在文件中的起始位置
		DWORD			ImageDataLen;			// 图像数据的总长度

		DWORD           Reserved[10];           // 预留位置


	}N3D_Header;

	//--------------------------------------------------------------------------------
	//结构体: N3D_5_1文件索引表
	// V3D数据目录表
	typedef struct N3D_5_1_Table  // 108 bytes
	{
		// 场景属性
		DWORD                   SceneproperyPtr;
		//材质数据的偏移量
		DWORD					mtrlDataPtr;
		// texture ptr
		DWORD                   texturePtr;
		//相机数据的偏移量
		DWORD					cameraDataPtr;
		//灯光数据的偏移量
		DWORD					lightDataPtr;
		//骨骼数据的偏移量
		DWORD					skeletonDataPtr;
		//模型数据的偏移量
		DWORD                   meshDataPtr;
		//Add By Baojiahui-----------------------------------------------------------------------
		//模型组数据的偏移量
		DWORD                   modelGroupPtr;
		//---------------------------------------------------------------------------------------
		//图形数据的偏移量
		DWORD					shapeDataPtr;
		//场景图数据的偏移量
		DWORD					sceneGraphDataPtr;

		// 文字数据的偏移量
		DWORD                   TextDataPtr;
		// 动画数据
		DWORD                   AnimationPtr;
		// overlay数据
		DWORD                   OverlayPtr;
		// button
		DWORD                   ButtonPtr;
		// 导航图
		DWORD                   NavigatotPtr;
		// 声音
		DWORD                   AudioPtr;
		// 控制信息
		DWORD                   ControlPtr;
		// flash
		DWORD                   FlashPtr;

		// 授权信息
		DWORD                   PrivacyDataPtr;

		// DWORD					ImageDataPtr;  // 图像数据

		CHAR                    Reserved[32];  // 保留扩展
	}N3D_5_1_Table;

	///// 下面的数据结构保存在V3D内部，记录文件中所有的纹理信息
	//// 纹理信息，包含一个名字长度和名字（不包含路径）
	//typedef struct TextureInfo
	//{
	//	BYTE   NameLen;  // 不超过255个字符
	//	char   *pName;   // 文件名（不包含路径）
	//		
	//}TextureInfo;

	//typedef struct TextureListInfo
	//{
	//	DWORD        Count;          // 纹理个数
	//	TextureInfo* TextureList;    // 纹理列表
	//		
	//}TextureListInfo;

} // namespace N3D



#pragma pack(pop)

#endif