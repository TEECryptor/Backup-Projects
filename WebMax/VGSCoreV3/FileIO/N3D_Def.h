//----------------------------------------------------------------------
// N3D�ļ���ؽṹ
//----------------------------------------------------------------------

#ifndef _N3D_DEF_H
#define _N3D_DEF_H

#include "..\VGSHeader.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include "S3D_Def.h"

using namespace std;

#pragma pack(push, 1) // ���ݽṹ�Ե��ֽڶ���

namespace N3D
{
	const char N3D_SUNTOWARD_NAME[] = "SUNTOWARD\0";

	/** �汾���¼�¼
	   ʱ��              �汾��       ˵��

	1. 2008-10-29֮ǰ    51000
	2. 2008-10-29        51001        ����������bUseDiffuse�����ڱ�ʶ�Ƿ�ʹ�õ�ǰ���ʵ�diffuse��ɫ
	3. 2008-10-10        51002        N3D_ReflectTextureParam�����ӷ�����ͼ�����ͣ�ƽ�滹������
	4. 2008-11-21        51003        1. ��ά��������ZOrder����ʶǰ���ϵ
	                                  2. ���ʱ��涯̬UV
	5. 2008-12-02        51004        1. ����ڵ�����
	2. ��ͼ������mipmap
	6. 2008-12-17        51005        1. texture�����¶��壬bump��cube��ͼ���Ӳ���
	7. 2008-01-06        51006        1. ����������ʵʱ�����ʵʱ����㼰�����
	8. 2009-02-23        51007        1. ģ�͡����ʡ�texturelayer���ƹ⡢�����������ԭ�ȵ�32�ֽڶ�����Ϊ��������

	8. 2009-06-16        5200          �������ʽ�޸�
	                                   1. ����ģ�͵�fvf
	
	*/
	const DWORD N3D_CURRENT_V3DVERSION = 52000;  // ʵ�ʵİ汾�ŵ��� N3D_CURRENT_V3DVERSION / 10000.0�����51203 -> 5.12.03
	#define _N3D_CUR_FLOATVERSION FLOAT(N3D_CURRENT_V3DVERSION / 10000.0) // ʵ�ʵ�N3D�汾�ţ�������
	#define _N3D_FLOATVERSION(ver) FLOAT(ver / 10000.0) // ʵ�ʵ�N3D�汾�ţ�������

	const int N3D_MAX_CORPNAME_LENGTH = 64;

	const int N3D_MAX_NAME_LENGTH = 32;


	//��ά����
// 	typedef struct vector3
// 	{
// 		float					x;
// 		float					y;
// 		float					z;
// 	}vector3;

	//��Ԫ��
// 	typedef struct quaternion
// 	{
// 		float					x;
// 		float					y;
// 		float					z;
// 		float					w;
// 	}quaternion;

	//Node�ı任
	typedef struct N3D_NodeTransform
	{
		//֡���
		float					time;   //ʱ��
		//Node�ı任-λ��
		vector3					position;	
		//Node�ı任-��ת
		quaternion				quat;
		//Node�ı任-����
		vector3					scale;
	}N3D_NodeTransform;

	//Node������
	enum N3D_NODETYPE
	{
		NNT_SCENEROOT = 0,
		NNT_CAMERANODE,
		NNT_LIGHTNODE,
		NNT_MODEL,
		NNT_BACKDROP,  // ����ͼ�ڵ�
		NNT_NONE
	};

	//�ڵ�
	typedef struct N3D_Node
	{
		//Node����
		N3D_NODETYPE			type;

		//Node���ڶ�Ӧ�����б��еı�ţ���������ǵƹ⣬�����ڵƹ��б��еı�ţ���������������������б��еı��
		int					index;
		//Add by baojiahui
		DWORD                   ID1;                            //ģ�͵�ID;            
		DWORD                   ID2;                            //ģ�͵���չID;

		N3D_NodeTransform       Transform;

		//����			
		/*UINT					strLen;
		std::string				name;*/

		//�任����
		std::vector<N3D_NodeTransform>  keyframeTransform; //V3D�ļ�ֻ�����Ż���Ĺؼ�֡�����������е�֡
		
		std::vector<N3D_Node>	childNode;

	}N3D_Node;

	//�����ṹͼ
	typedef struct N3D_SceneNodeGraph
	{
		//�����е�Node����
		UINT				nodeCnt;
		//Node�б�
		N3D_Node			node;
	}N3D_SceneNodeGraph;

	//�����UV
	typedef struct N3D_UV
	{
		//uv����
		float					u;
		float					v;
	}N3D_UV;

	//������
	typedef struct N3D_Vertex
	{
		//�����λ��
		vector3					position;
		//����ķ���
		vector3					normal;
		//�������ɫ
		DWORD					color;

		//UV����Ĳ���, 8λ��ʾ8�㣬1-8��Ŀǰֻ�����Ĳ�
		// BYTE					uvFlag; 

		//�����map Channel(1-8) UV�����ݲ���ͼ�����־λ�������п���û��
		N3D_UV					uv1;
		N3D_UV					uv2;
		N3D_UV					uv3;
		N3D_UV					uv4;

	}N3D_Vertex;

	//��
	typedef struct N3D_Face_32
	{
		//����������������ģ���еĶ�����
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
	//��ģ��SubMesh
	typedef struct N3D_SubMesh
	{
		//���ʱ��
		int					mtrlIndex;   // -1��ʾû�в���
		UINT                faceCnt;
		//�Խڵ����б�
		VOID*               face;
		//std::vector<N3D_Face>	face;
		
	}N3D_SubMesh;

	//��Χ��
	typedef struct N3D_BoundBox
	{
		vector3					minCorner;
		vector3					maxCorner;
	}N3D_BoundBox;            

	//��������������
	typedef struct N3D_BoneAss
	{
		//������
		UINT					vertexIndex;
		//��Ĺ�ͷ���
		UINT					bindingBoneIndex;
		//�Ȩ��
		float					bindingWeight;
	}N3D_BoneAss;

	//Mesh�����
	typedef struct N3D_BindingData
	{
		//��Ķ�������
		UINT					bindingVertexCnt;
		//��Ķ�������
		N3D_BoneAss*				boneAss;
	}N3D_BindingData;

	//ģ��
	typedef struct N3D_Model
	{
		//Node������
		UINT					strLen;
		std::string 			name;
		//��������
		UINT					vertexCnt;	
		//��������
		N3D_Vertex*				vertex;
		//submesh 
		std::vector<N3D_SubMesh> subMesh;
		//��Χ��
		N3D_BoundBox			boundBox;
	    
		float                   boundSphereRadius;
		
		//Add by Baojiahui-----------------------------------------------------------------------------
		bool                    isBillboard;
		bool                    isCollision;
		bool                    isGround;
		bool                    isSkyBox; 
		bool                    isVisible;		
		bool                    isClickable;
		bool                    isAutoNormalise;            // ģ������ʱ���Ƿ��Զ����·��� fuxb 2008-09-09

		//�󶨵Ĺ������
		int					bindingSkeletonIndex;
		//������
		std::vector<N3D_BoneAss>	bindingData;
		//��������ͷ��Mesh����ϵ�еı任
		N3D_NodeTransform			skeletontransform;
	}N3D_Model;

	//�ṹ��: ģ����
	typedef struct N3D_ModelGroup
	{
		//Node������
		UINT					strLen;
		std::string 			name;

		INT						ModelCnt;			//���е�ģ������
 		INT*					ModelIndex;			//���е�ģ�ͱ������,��Ӧmeshlist�е�������

		std::vector<N3D_ModelGroup*> ModelGroup;
	}N3D_ModelGroup;

	//�ṹ��: ģ�����б� --ģ�����б���Ϊһ��scene�µ�ȫ��vectorʹ�ã����ٵ�������ṹ�塢

	//ģ���б�
	typedef struct N3D_ModelList
	{
		//�����е�model����
		UINT					modelCnt;
		// model�б�
		N3D_Model*				model;
	}N3D_ModelList;

	//��ά��
	typedef struct N3D_Point
	{
		//λ��
		vector3					position;
	}N3D_Point;

	//��άͼ��
	typedef struct N3D_Shape
	{
		//Node������
		UINT					strLen;
		std::string 			name;

		//������
		UINT					pointCnt;
		//���б�
		N3D_Point*				point;
	}N3D_Shape;

	//��άͼ���б�
	typedef struct N3D_ShapeList
	{
		//�����е�Shape����
		UINT					shapeCnt;
		//Shape�б�
		N3D_Shape*				shape;
	}N3D_ShapeList;
	
	// texture������
// 	enum N3D_TextureType
// 	{
// 		TLT_INVALID = 0,
// 		TLT_BASAL,						// ������ͼ
// 		TLT_OPACITY,		            // ͸����ͼ
// 		TLT_REFLECT,                    // ������ͼ
// 		TLT_SPECULAR,                   // �߹���ͼ
// 		TLT_CUBEMAP,                    // ����ͼ
// 		TLT_DYNAMIC,                    // ��̬��ͼ
// 		TLT_BUMPMAP,                  // ˮ��Ч
// 		TLT_MOVIE,                      // ��Ӱ��ͼ
// 		TLT_FLASH,						// Flash��ͼ
// 		TLT_REALTIMEREF                 // ʵʱ����
// 	};
	// texture������
	typedef VGS_TextureType N3D_TextureType;

	// ������ͼ����
	typedef struct N3D_BasalTextureParam
	{
		INT width;
		INT height;
	}N3D_BasalTextureParam;

	// ͸����ͼ����
	typedef struct N3D_OpacityTextureParam
	{
		INT width;
		INT height;
	}N3D_OpacityTextureParam;

	// ������ͼ����
	typedef struct N3D_ReflectTextureParam
	{
		INT width;
		INT height;
	}N3D_ReflectTextureParam;
	typedef struct N3D_ReflectTextureParam51002
	{
		INT width;
		INT height;
		char reflectionType;  // fuxb 2008-11-10 ��������, -1:��Ч��0��ƽ�棬1������
	}N3D_ReflectTextureParam51002;

	// ������ͼ����
	typedef struct N3D_CubicTextureParam
	{
		INT width;
		INT height;
	}N3D_CubicTextureParam;

	// ��̬��ͼ����
	typedef struct N3D_DynamicTextureParam
	{
		INT width;
		INT height;
		INT duration;
	}N3D_DynamicTextureParam;

	// bumpwater����
	typedef struct N3D_BumpwaterTextureParam
	{
		INT width;
		INT height;
		FLOAT			XScale;					//ˮ���Ƶ�X�����ܶ�
		FLOAT			YScale;					//ˮ���Ƶ�Y�����ܶ�
		FLOAT			Radius;					//����
		FLOAT			Speed;					//����
		FLOAT           CenterX;	// ���Ƶ����ĵ�����
		FLOAT           CenterY;
	}N3D_BumpwaterTextureParam;

	// bump��������
	typedef struct N3D_BumpEnvTextureParam
	{
		INT width;
		INT height;
		float   noise1;
		float   noise2;
	}N3D_BumpEnvTextureParam;
	
	// ������ͼ
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

	// ��Ӱ��ͼ����
	typedef struct N3D_MovieTextureParam
	{
		INT duration;
	}N3D_MovieTextureParam;

	// flash��ͼ����
	typedef struct N3D_FlashTextureParam
	{
		INT duration;
	}N3D_FlashTextureParam;

	typedef struct N3D_RTReflectionParam  // ʵʱ����
	{
		INT width;
		INT height;
// 		INT modelIndex;
// 		INT refModleCount;
// 		DWORD* vRefModels;
	}N3D_RTReflectionParam;

	typedef N3D_RTReflectionParam N3D_RTRefractionParam;  // ʵʱ����
	// �����������Ͳ���
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

	//ͼƬ����
	typedef struct N3D_ImageInfo
	{
		//SHORT                   Width;                   // ��ͼ���
		//SHORT                   Height;                  // ��ͼ�߶�
		//UINT                    MipCount;                // ��ͼ��mipmap����

		UINT                    nameLen;
		char*                   name;
		UINT                    DataLen;
		char*                   Data;                    // ��ͼ����
	}N3D_ImageInfo;

	//typedef struct N3D_Image
	//{
	//	UINT                    image_strLen;          // ��ͼ���Ƴ���
	//	std::string             imageName;             // ��ͼ����
	//}N3D_Image;

	//ͼƬ��������
	typedef struct N3D_ImageList
	{
	   UINT					imageCnt;          // Image������
	   N3D_ImageInfo*       imageInfoList;         // Image���������ָ��
	}N3D_ImageList;

//ѹ�����Image��Ԫ����
typedef struct N3D_5_1_ImageInfo
{
	CHAR			Name[32];		//����
	DWORD			Size;			//ѹ�����Image���ݴ�С
}N3D_5_1_ImageInfo;

//����http�ϵ�N3D�ļ��е�Image��Ȼ�󱣴浽�ڴ��е�Image��Ԫ���ݽṹ
typedef struct N3D_5_1_ZipImageData
{
	
    N3D_5_1_ImageInfo   imageInfo;      //image����
	BYTE*			    Data;			//ѹ�����Image����
}N3D_5_1_ZipImageData;

	// ��ɫ����
	//enum N3D_BlenderType
	//{
	//	NBT_COLOR = 0,
	//	NBT_ALPHA
	//};

	// ��ɫ��ʽ
	enum N3D_BlenderOP
	{
		NBM_REPLACE = 0,  // ʹ��source1
		NBM_DISABLE,      // ��source2�滻source1
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

	// ��ɫ����
	typedef struct N3D_BlenderParam
	{
		/*N3D_LayerBlendSource source1;
		N3D_LayerBlendSource source2;*/
		float BlenderOP_alpha;  // ��N3D_BlenderOP == NBM_BLEND_MANUALʱ��Ч����ɫģʽ���õ��Ĳ���alpha [0, 1],

	}N3D_BlenderParam;

	// texturelayer�Ļ�ɫ��Ϣ
	typedef struct N3D_BlenderInfo
	{
		// N3D_BlenderType		type;         // ��ɫģʽ 0 : color, 1: alpha
		N3D_BlenderOP			ColorBlendOption;       // ��ɫ��ɫ��ʽ
		N3D_BlenderParam		ColorBlendParam;        // ��ɫ��ɫ����

		N3D_BlenderOP			AlphaBlendOption;       // alpha��ɫ��ʽ
		N3D_BlenderParam		AlphaBlendParam;        // alpha��ɫ����

	}N3D_BlenderInfo;
    //Edit by Baojiahui 2008/8/6
	//��ͼ
	typedef struct N3D_TextureLayer
	{
		// CHAR					Name[N3D_MAX_NAME_LENGTH];     // ��ͼ���� Edit by Baojiahui for nonfinite of the texture name String length /2009/2/24
		
		N3D_TextureType         type;                          // ��ͼ����, ���type == TLT_INVALID, ���治��д����
		int						UVIndex;                       // ʹ�õ�UV����

		BYTE                     Mipmap;                       // fuxb 2008-12-12 ����mipmap

		N3D_TextureParam        TextureParam;                 // �����������texture�Ĳ���

		N3D_BlenderInfo         Blenderinfo;                   // ��ɫ����
		UINT					ImageCount;                    // ʹ�õ���ͼ��Ŀ
		int*                    ImageIdxList;                  // ��ͼ�б�
	}N3D_TextureLayer;

	//��ͼ�б�
	typedef struct N3D_TextureList
	{
	   UINT							textureCnt;
	   N3D_TextureLayer*			texture;
	}N3D_TextureList;

	// ���ʻ�����Ϣ
	typedef struct N3D_MtrlBasalInfo
	{ 				
		//CHAR					Name[N3D_MAX_NAME_LENGTH];Edit by Baojiahui for nonfinite of the texture name String length 2009/2/24
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

		//���ʵ�Shade���ͣ����ڹ���ѡ��
		int						shaderType;	// 0 : SO_FLAT, 1: SO_GOURAUD, 2: SO_PHONG 
		//Add by baojiahui 2008/8/8
		//���ʵ����ģʽ
		int                     fillMode;
		//�Ƿ�˫����Ⱦ
		bool					bTwoSide;	
		//�Ƿ�͸������
		bool                    alphaTest;         
		//͸���������޲���[0��255]
		BYTE                    alphaTestValue;
		//�Ƿ������Ӱ
		bool                    bRecShadow;

	}N3D_MtrlBasalInfo;

	//�ؼ�֡���ʶ�����Ϣ
	typedef struct N3D_MtrlAminInfo
	{
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

	}N3D_MtrlAminInfo;

	//����
	typedef struct N3D_MtrlInfo
	{		
		N3D_MtrlBasalInfo       BasalInfo;  // ���ʵĻ�������
		bool                    bUseDiffuse;  // fuxb 2008-10-29 �Ƿ�ʹ�ò��ʵ�diffuse��ɫ�����false����Ч�ڲ��ʵ�diffuseΪ��ɫ

		//�ؼ�֡��Ϣ����������ڹؼ�֡��KfrCntΪ0
		UINT                    kfrCnt;
		N3D_MtrlAminInfo*       kfrMtrlInfo;

		// texture�б�
		UINT					TextureLayerCount;
		N3D_TextureLayer*		TextureLayerList;  // ��texture�б��е�����

	}N3D_MtrlInfo;

	//�����б�
	typedef struct N3D_MtrlList
	{
		//�����еĲ�������
		int							mtrlCnt;

		//�����б�
		N3D_MtrlInfo*				mtrl;

	}N3D_MtrlList;

	//��ͼ������Ϣ�����ڶ�̬������ͼ
	typedef struct N3D_MtrlUseTex
	{
		std::string					mtrlName;		//������
		int							mapChannel;		//��ͼͨ��
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
		std::vector<N3D_MtrlUseTex>	usedMtrlList;  //ʹ�ø���ͼ�Ĳ����б�
	}N3D_TextureDev;

	//���ģ�͹����Ĺ�����Ϣ�����ڴ������
	typedef struct N3D_SkeletonModelDev
	{
		std::string					modelName;						//ģ������
		std::string					skeletonName;					//��������
	}N3D_SkeletonModelDev;

	////��������ṹS3D
	//typedef struct N3D_CAMERA_INFO
	//{
	//	//λ��
	//	vector3					from;
	//	//Ŀ���
	//	vector3					at;
	//	//����ֵ
	//	float					roll;
	//	//��ͷ
	//	float					fov;
	//}N3D_CAMERA_INFO;

	//��������ṹV3D��λ�ú�ת�ǽ���Node
	typedef struct N3D_CAMINFO
	{
		//���Ե�From ��At
		//vector3					from;
		//vector3					at;
		

		//��ͷ
		float					fov;
		//����������Զ������
		float					nearClipDis;
		float					farClipDis;
	}N3D_CAMINFO;

	// ��������ṹ, N3D����������б���������������ʱû����
	typedef struct N3D_CAMPARAM
	{
		//�Ƿ���������
		bool					isOrtho;
		
		//��ͷ
		float					fov;
		
		//����������Զ������
		float					nearClipDis;
		float					farClipDis;
		
		vector3                 position;      // λ��  
		quaternion              quat;          // ת��
		
	}N3D_CAMPARAM;

	//typedef struct N3D_Camera
	//{
 //  		//���������
	//	int						strLen;
	//	std::string 			name;

	//	//�Ƿ���������
	//	bool					isOrtho;
	//	//��ͷ
	//	float					fov;
	//	//����������Զ������
	//	float					nearClipDis;
	//	float					farClipDis;
	//}N3D_Camera;

	//���
	typedef struct N3D_Camera
	{
		//���������
		/*int						strLen;
		std::string 			name;*/
		// char					name[N3D_MAX_NAME_LENGTH];//Edit by Baojiahui for nonfinite of the camera name String length 2009/2/24

		//�Ƿ���������
		bool					isOrtho;
		
		//����ؼ�֡������Ϣ
		//std::vector<N3D_CAMERA_INFO> camInfo;  //������S3D

		//�����ʼ��Ϣ
		N3D_CAMINFO				BaseInfo;			//������V3D
		//std::vector<N3D_N3D_CAMINFO>	v3dAnimationInfo;		//������V3D

	}N3D_Camera;

	//���
	typedef struct N3D_CameraList
	{
		//�����е��������
		int						cameraCnt;
		//����б�
		N3D_Camera*				camera;
	}N3D_CameraList;

	//�ƹ�����
	enum N3D_LIGHTTYPE
	{
		NLT_OMINI = 0,
		NLT_SPOTLIGHT,
		NLT_DIRECTIONLIGHT,
		NLT_FREESPOTLIGHT,
		NLT_TARGETDIRECTIONLIGHT
	};

	//�ƹ�Ŀɱ����S3D
	typedef struct N3D_LIGHT_INFO
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

	}N3D_LIGHT_INFO;


	//�ƹ�Ŀɱ����V3D
	typedef struct N3D_OMNI
	{
		//��ɫ
		DWORD					color;
		//ǿ��
		float					multiply;
		
		//����˥����Χ
		float					attenuationRange;
		float					attenuation0;
		float					attenuation1;
		float					attenuation2;
	}N3D_OMNI;

	//�ƹ�Ŀɱ����V3D
	typedef struct N3D_SPOTLIGHT
	{
		//��ɫ
		DWORD					color;
		//ǿ��
		float					multiply;
		
		//���ߵķ���, ��������ϵ
		vector3					direction;

		//����˥����Χ
		float					attenuationRange;
		float					attenuation0;
		float					attenuation1;
		float					attenuation2;

		//��Ȧ����ķ�Χ��
		float					innerAngle; // theta
		float					outerAngle; // phi
		float					fallOff;
	}N3D_SPOTLIGHT;


	//�ƹ�Ŀɱ����V3D
	typedef struct N3D_DIRECTIONAL
	{
		//��ɫ
		DWORD					color;
		//ǿ��
		float					multiply;
		
		//���ߵķ���, ��������ϵ
		vector3					direction;

		//����˥����Χ
		float					attenuationRange;
		float					attenuation0;
		float					attenuation1;
		float					attenuation2;

	}N3D_DIRECTIONAL;


	//�ƹ�
	typedef struct N3D_Light
	{
		//Node������
		/*int						strLen;
		std::string 			name;*/
		// char                    name[N3D_MAX_NAME_LENGTH];//Edit by Baojiahui for nonfinite of the Light name String length 2009/2/24

		//�ƹ�����
		N3D_LIGHTTYPE   		 type;
		N3D_OMNI *               pOmniLight;           // ���type!=NLT_OMINI, pOmniLight = NULL
		N3D_DIRECTIONAL*         pDirectionalLight;    // ���type!=NLT_DIRECTIONLIGHT, pDirectionalLight = NULL
		N3D_SPOTLIGHT*           pSpotLightInfo;       // ���type!=NLT_SPOTLIGHT, pSpotLightInfo = NULL

		int                      FrameCount;               // ����֡��
		N3D_OMNI *               pAnimOmniLight;           // ���type!=NLT_OMINI, pOmniLight = NULL
		N3D_DIRECTIONAL*         pAnimDirectionalLight;    // ���type!=NLT_DIRECTIONLIGHT, pDirectionalLight = NULL
		N3D_SPOTLIGHT*           pAnimSpotLightInfo;       // ���type!=NLT_SPOTLIGHT, pSpotLightInfo = NULL

		////�ƹ�֡����
		//std::vector<N3D_LIGHT_INFO>        vLightInfo;  //������S3D

		////V3D����ĵƹ����֡����
		//std::vector<N3D_OMNI>				vOmniInfo;
		//std::vector<N3D_DIRECTIONAL>		vDirectionLightInfo;
		//std::vector<N3D_SPOTLIGHT>			vSpotLightInfo;

	}N3D_Light;

	//�ƹ��б�
	typedef struct N3D_LightList
	{
		//�����еĵƹ�����
		int						lightCnt;
		//�ƹ��б�
		N3D_Light*				light;
	}N3D_LightList;

	//��ͷ
	typedef struct N3D_Bone
	{
		//����
		int						strLen;
		std::string				name;

		//��ʼ��̬
		N3D_NodeTransform			pose;

		//�������ڸ���ͷ����ϵ�е�ǰ֡��Գ�ʼ��̬�ı任
		std::vector<N3D_NodeTransform>	transform;

		//�ӹ�ͷ����
		//int					childBoneCnt;
		//�ӹ�ͷ�б�
		//N3D_Bone*				childBone;
		
		std::vector<N3D_Bone>	childBone;
	}N3D_Bone;

	//��������
	typedef struct N3D_SkeletonAnimation
	{	
		//����
		int						strLen;
		std::string				name;

		//�ؼ�֡����
		int						frameCnt;
		//��ͷ����
		int						boneCnt;
		//��ͷ�б�
		N3D_Bone				bone;
	}N3D_SkeletonAnimation;

	//����
	typedef struct N3D_Skeleton
	{
		//����
		int						strLen;
		std::string				name;
		
		//���������Ķ����б�
		int						animationCnt;
		std::vector<N3D_SkeletonAnimation*>   animationList;
	}N3D_Skeleton;

	//�����еĹ����б�
	typedef struct N3D_SkeletonList
	{
		//��������
		int						skeletonCnt;
		N3D_Skeleton*			skeleton;
	}N3D_SkeletonList;



	//�ṹ��: �������ݽṹ
	typedef struct N3D_Audio
	{
		int						strLen;
		std::string				name;
		INT						fileNameLen;            //��Ƶ�ļ�·������
		std::string				fileName;		    	//��Ƶ�ļ�ȫ·��
		BYTE					type;					//�ļ�����
		INT						repeatCnt;				//�ز�����
	}N3D_Audio;

	//��Ƶ�б�
	typedef struct N3D_AudioList
	{
		INT				AudioCnt;			//��Ƶ������
		N3D_Audio*	    Audio;				//��Ƶ�б�
	}N3D_AudioList;

	//�ṹ��: Flash���ݽṹ
	typedef struct N3D_Flash
	{
		CHAR			Name[32];			//Flash��������
		INT             FileNameLen;        //Flash�ļ�·������
		CHAR*			pFileName;		    //Flash�ļ�ȫ·��
		
		INT				OffsetOrigin;		//ƫ��λ�õ�����ԭ���ʾ
		BOOL			UseRelativePos;		//�Ƿ�ʹ�����λ��

		RECT			Rect;				//Flash �ߴ�
		FLOAT			Angle;				//ת��
		INT				Opacity;			//͸����
		BOOL			Transparency;		//�Ƿ�ٱ���

		BOOL			UseFlashSize;		//ʹ��Flash�ĳߴ�
	}N3D_Flash;

	//�ṹ��: Flash�б�
	typedef struct N3D_FlashList
	{		
		INT				FlashCnt;			//Flash����
		N3D_Flash*	    Flash;				//Flash�б�
	}N3D_FlashList;

	//�ṹ�壺������Ϣ
	typedef struct N3D_TextInfo
	{
		DWORD	Color;
		INT			X;
		INT			Y;
	}N3D_TextInfo;

	//�ṹ��: ���ֻ����ṹ
	typedef struct N3D_Text
	{
		INT						Length;		//�ַ�����
		CHAR*					StrText;	//�ַ���

		INT						X;
		INT						Y;			//���ֵ�λ��
		DWORD				Color;		//���ֵ���ɫ

		CHAR					FontName[32];	//������
		
		UINT					KfrCnt;		//�ؼ�֡����
		N3D_TextInfo*		KfrInfo;	//�ؼ�֡����
	}N3D_Text;

	//�ṹ��: �����б�
	typedef struct N3D_TextList
	{
		INT			TextCnt;	//��������
		N3D_Text*	Texts;		//�����б�
	}N3D_TextList;

	//�ṹ�壺Overlay��Ϣ
	typedef struct N3D_OverlayFrameInfo
	{
		RECT		Rect;			//Overlay�趨����Գߴ�
		FLOAT		Angle;			//ת��
		INT			Opacity;		//͸����
	}N3D_OverlayFrameInfo;

	typedef struct N3D_OverlayBasalInfo
	{
		CHAR		Name[N3D_MAX_NAME_LENGTH];			// Overlay������

		INT			OffsetOrigin;		// ƫ��λ�õ�����ԭ��
		BOOL		UseRelativePos;		// ʹ�����λ��
		BOOL		UseRelativeSize;	// �Ƿ�ʹ����Եĳߴ�

		RECT		Rect;				// Overlay�ߴ缰λ��
		FLOAT		Angle;				// ת��
		INT			Opacity;			// ͸����
		BOOL		IsBackdrop;			// �Ƿ��Ǳ���ͼ

		INT			MaterialIndex;		// Overlay�Ĳ���Index

		BOOL        IsVisible;          // �Ƿ�ɼ�
	}N3D_OverlayBasalInfo;

	//�ṹ��: Overlay�����ṹ
	typedef struct N3D_Overlay
	{
		N3D_OverlayBasalInfo	BasalInfo;  // ������Ϣ

	
		UINT					 KfrCnt;		// �ؼ�֡����
		N3D_OverlayFrameInfo*	 KfrInfo;	// �ؼ�֡��Ϣ

		INT                      zOrder;    // overlay��z˳��

	}N3D_Overlay;

	//�ṹ��: Overlay�б�
	typedef struct N3D_OverlayList
	{
		INT				OverlayCnt;	//Overlay������
		N3D_Overlay*	Overlays;	//Overlay�б�
	}N3D_OverlayList;

	//����ͼָ����Ϣ
	typedef struct N3D_NavigaterPointer
	{
		CHAR		Name[32];			//Pointer������
		UINT		OverlayIndex;		//Pointerʹ�õ�Overlay�ı��
		UINT		Type;				//Pointer�����ͣ�Camera-0, Model-1
		INT			ObjIndex;			//�����ģ�͵ı��
	}N3D_NavigaterPointer;

	typedef struct N3D_NavigaterBasalInfo
	{
		CHAR						Name[32];
		RECT						Rect;						// ����Mapƽ������

		int						    MapOverlayIndex;			// ����Map(Overlay)�ı��

		FLOAT						sceneLeft;					// ����TopView��Rect
		FLOAT						sceneTop;
		FLOAT						sceneRight;
		FLOAT						sceneBottom;	
 
		//bool						UseRelativePos;    // �Ƿ�ʹ�����λ��
		//bool						UseRelativeSize;   // �Ƿ�ʹ����Դ�С
	}N3D_NavigaterBasalInfo;

	//�ṹ��: Navigater�����ṹ
	typedef struct N3D_Navigater
	{
		N3D_NavigaterBasalInfo	    BasalInfo;               // ������Ϣ

		UINT						PointerCnt;					// Pointer������
		N3D_NavigaterPointer*		NavPointer;					// Pointer��Ϣ

	}N3D_Navigater;

	//�ṹ��: Navigater�б�
	typedef struct N3D_NavigaterList
	{
		INT						    NavigaterCnt;	//Navigater����
		N3D_Navigater*			    Navigaters;		//Navigater�б�
	}N3D_NavigaterList;

	//�ṹ��: Button�Ĺؼ�֡����
	typedef struct N3D_ButtonFrameInfo
	{
		RECT		Rect;				//��ť��Rect
		INT			Opacity;			//��ť��͸����
	}N3D_ButtonFrameInfo;

	// ��ť��������
	typedef struct N3D_ButtonBasalInfo
	{
		CHAR		Name[32];			//��ť������
		RECT		Rect;				//��ť��Rect
		INT			Opacity;			//��ť��͸����
		CHAR		CallBackFunc[32];	//�ص���������
		UINT		BtnOverlayIndex;	//Button Up��ID��
		UINT		MouseOverMtrlIndex;	//overImage�Ĳ���ID��
		UINT		MouseDownMtrlIndex;	//downImage�Ĳ���ID��

	}N3D_ButtonBasalInfo;

	//�ṹ��: Button��Ԫ�ṹ
	typedef struct N3D_Button
	{
		N3D_ButtonBasalInfo     BasalInfo;  // ��ť��������

		UINT		            KfrCnt;		// �ؼ�֡����
		N3D_ButtonFrameInfo*	KfrInfo;	// �ؼ�֡����
	}N3D_Button;

	//�ṹ��: Button�б�
	typedef struct N3D_ButtonList
	{
		INT					ButtonCnt;			//��ť����
		N3D_Button*		Buttons;			//��ť�б�
	}N3D_ButtonList;

	//�ṹ��: N3D_1_0��Ȩ�ṹ��
	typedef struct N3D_Privacy
	{
		INT				VersionType;			//�汾��ʾ(���˰汾0����׼�汾1����ҵ�汾2)
		DWORD			ClientIndex;			//��Ȩ�ͻ����
		CHAR			SerialNum[128];			//����Ȩ��������кţ��Ѿ���128λ����
		CHAR			Authorize[128];			//�Ƿ��Ѿ�����Ȩȥ�������к�
		DWORD			ExtendInfo1;			//��չ������Ϣ1
		DWORD			ExtendInfo2;			//��չ������Ϣ2
	}N3D_Privacy;

	//�ṹ��: Walker����Ľṹ��
	typedef struct N3D_Walker
	{
		CHAR			Name[32];		//����
		INT				CameraIndex;	//ʹ�õ�������

		FLOAT			walkSpeed;		//Walker������������ٶ�
		FLOAT			walkAcc;		//Walker����������߼��ٶ�

		FLOAT			rosSpeed;		//Walker���������ת�ٶ�
		FLOAT			rosAcc;			//Walker���������ת���ٶ�

		FLOAT			height;			//Walker�ĸ߶�

		BOOL			bCollision;		//�Ƿ�����ײ
		FLOAT			colDis;			//Walker����ײ����

		BOOL			bWeight;		//�Ƿ�������
		FLOAT			liftSpeed;		//�����½����ٶȣ�ʧȥ����ʱ��Ч
		FLOAT			liftAcc;		//�����½��ļ��ٶȣ�ʧȥ����ʱ��Ч

		FLOAT			upperAngle;		//̧ͷ���������
		FLOAT			lowerAngle;		//��ͷ������ӽ�
	}N3D_Walker;

	//�ṹ��: walker�����б�
	typedef struct N3D_WalkerList
	{
		INT				WalkerCnt;			//Walker������
		N3D_Walker* Walkers;			//Walkder�����б�
	}N3D_WalkerList;

	//�ṹ��: Editor����
	typedef struct N3D_Editor
	{	
		CHAR				Name[32];		//����

		INT					CameraIndex;	//ʹ�õ�������

		BOOL		bIsAutoRoundRoll;					//Editģʽ���Ƿ��Զ�����

		FLOAT		autoSpeedX;						    // Editģʽ���Զ��� X ����ת���ٶ�
		FLOAT		autoSpeedY;						    // Editģʽ���Զ��� Y ����ת���ٶ�

		FLOAT		manualSpeedX;					//Editģʽ���ֶ���ת������ٶ��� X ��
		FLOAT		manualSpeedY;					//Editģʽ���ֶ�����������ٶ��� Y ��

		FLOAT		manualPanXSpeed;					//ƽ�Ƶ�X�����ٶ�
		FLOAT		manualPanYSpeed;					//ƽ�Ƶ�Y�����ٶ�

		FLOAT		dollySpeed;							//��������ʱ�ƶ�������ٶ�
		FLOAT		dollyAcc;							//����������ٶ�

		FLOAT		dollyNearDis;						//�����Dolly
		FLOAT		dollyFarDis;						//��Զ��Dolly

		INT			rosAxis;							// ��ת���־λ, 0: xy, 1 : x, 2 : y
		vector3     LookAtPos;                          // Editor��Ŀ��� 

	}N3D_Editor;

	//�ṹ�壺Editor�����б�
	typedef struct N3D_EditorList
	{
		INT					EditorCnt;							//Editor������
		N3D_Editor*		Editors;							//Editor�б�
	}N3D_EditorList;

	//�ṹ��: Flyer����
	typedef struct N3D_Flyer
	{
		CHAR				Name[32];		//����

		INT					CameraIndex;					//ʹ�õ�������

		FLOAT				panSpeed;						//ƽ���ٶ�
		FLOAT				panAcc;							//ƽ�Ƽ��ٶ�

		FLOAT				dollySpeed;						//��������ٶ�
		FLOAT				dollyAcc;						//����������ٶ�

		FLOAT				dollyNearDis;					//�����Dolly
		FLOAT				dollyFarDis;					//��Զ��Dolly

		FLOAT				rotateSpeed;					//�����ת���ٶ�
		FLOAT				rotateAcc;						//�����ת�ļ��ٶ�
		
		FLOAT				liftSpeed;						//�����½����ٶȣ�ʧȥ����ʱ��Ч
		FLOAT				liftAcc;						//�����½��ļ��ٶȣ�ʧȥ����ʱ��Ч

		FLOAT				upperAngle;						//̧ͷ���������
		FLOAT				lowerAngle;						//��ͷ������ӽ�

		BOOL				bCollision;						//�Ƿ�����ײ
		FLOAT				colDis;							//��ײ����
	}N3D_Flyer;

	//�ṹ��: Flyer�����б�
	typedef struct N3D_FlyerList
	{
		INT					FlyerCnt;						//�����������
		N3D_Flyer*		Flyers;							//��������б�
	}N3D_FlyerList;

	//�ṹ�壺���ƶ���
	typedef struct N3D_ControlData
	{
		INT					ControlType;					//��������	CT_WALKER = 0, CT_EDITOR = 1, CT_FLYER = 2,	CT_NONE �ǽ���ģʽ 
		INT					ControlerIndex;					//���ƶ�����

		N3D_WalkerList	WalkerList;						//���ζ����б�
		N3D_EditorList	EditorList;						//�༭�����б�
		N3D_FlyerList	FlyerList;						//���ж����б�
	}N3D_ControlData;

	//�ṹ�壺����������
	typedef struct N3D_SceneProp
	{
		WORD				Width;				//������
		WORD				Height;				//����߶�
		DWORD				BackColor;			//������ɫ
		WORD				FPS;				//��Ⱦ֡��
		BYTE				AntiAliasLevel;		//���ܳݼ���
		//INT				FSBlurLevel;		//ȫ����̬ģ���ļ���
		//INT				FSGlowLevel;		//ȫ�����εļ���

		//INT				KeyframeCnt;		//�ؼ�֡������
		//INT				RepeatCnt;			//ѭ�����ŵĴ����� -1Ϊ���޴�ѭ��

		char				Reserved[19];       // Ԥ���ֶ�

	}N3D_SceneProp;

	
	//Add for fast write the V3D file

	// ����ʵ�� �� д��v3d�ļ�
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

	// ����ʵ�� �� д��v3d�ļ�
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


	// ��Ȩ�ṹ�� 32 ���ֽ�
	typedef struct N3D_RegInfo
	{
		DWORD			CorpID;				// ����Ȩ�Ĺ�˾��ʶ
		DWORD			Date1;				// ��Ȩ�����ڲ���
		DWORD			Date2;				// ��Ȩ�����ڲ���
		DWORD           Reserved[5];        // Ԥ��λ��
	}N3D_RegInfo;

	// �ṹ��: N3D�ļ�ͷ�ṹ 96���ֽ�
	typedef struct N3D_Header
	{	
		DWORD           Version;               // �汾�ţ������İ汾�ŵ��� Version / 10000.0
		N3D_RegInfo	    RegInfo;			   // ע����Ϣ

		DWORD			SceneDataPtr;		    // Zip �� Scene�������ļ��е���ʼλ��
		DWORD			SceneZipDataLen;	    // Zip �� Scene���ݳ��ȣ�������image
		DWORD           OriginSceneDataLen;     // Zip ǰ Scene���ݳ���    

		DWORD			ImageDataPtr;			// ͼ���������ļ��е���ʼλ��
		DWORD			ImageDataLen;			// ͼ�����ݵ��ܳ���

		DWORD           Reserved[10];           // Ԥ��λ��


	}N3D_Header;

	//--------------------------------------------------------------------------------
	//�ṹ��: N3D_5_1�ļ�������
	// V3D����Ŀ¼��
	typedef struct N3D_5_1_Table  // 108 bytes
	{
		// ��������
		DWORD                   SceneproperyPtr;
		//�������ݵ�ƫ����
		DWORD					mtrlDataPtr;
		// texture ptr
		DWORD                   texturePtr;
		//������ݵ�ƫ����
		DWORD					cameraDataPtr;
		//�ƹ����ݵ�ƫ����
		DWORD					lightDataPtr;
		//�������ݵ�ƫ����
		DWORD					skeletonDataPtr;
		//ģ�����ݵ�ƫ����
		DWORD                   meshDataPtr;
		//Add By Baojiahui-----------------------------------------------------------------------
		//ģ�������ݵ�ƫ����
		DWORD                   modelGroupPtr;
		//---------------------------------------------------------------------------------------
		//ͼ�����ݵ�ƫ����
		DWORD					shapeDataPtr;
		//����ͼ���ݵ�ƫ����
		DWORD					sceneGraphDataPtr;

		// �������ݵ�ƫ����
		DWORD                   TextDataPtr;
		// ��������
		DWORD                   AnimationPtr;
		// overlay����
		DWORD                   OverlayPtr;
		// button
		DWORD                   ButtonPtr;
		// ����ͼ
		DWORD                   NavigatotPtr;
		// ����
		DWORD                   AudioPtr;
		// ������Ϣ
		DWORD                   ControlPtr;
		// flash
		DWORD                   FlashPtr;

		// ��Ȩ��Ϣ
		DWORD                   PrivacyDataPtr;

		// DWORD					ImageDataPtr;  // ͼ������

		CHAR                    Reserved[32];  // ������չ
	}N3D_5_1_Table;

	///// ��������ݽṹ������V3D�ڲ�����¼�ļ������е�������Ϣ
	//// ������Ϣ������һ�����ֳ��Ⱥ����֣�������·����
	//typedef struct TextureInfo
	//{
	//	BYTE   NameLen;  // ������255���ַ�
	//	char   *pName;   // �ļ�����������·����
	//		
	//}TextureInfo;

	//typedef struct TextureListInfo
	//{
	//	DWORD        Count;          // �������
	//	TextureInfo* TextureList;    // �����б�
	//		
	//}TextureListInfo;

} // namespace N3D



#pragma pack(pop)

#endif