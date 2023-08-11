#ifndef _VGSDEF_H
#define _VGSDEF_H

#include <windows.h>
//#include <windef.h>
// namespace VGS {

#pragma pack(push, 4) // ���ݽṹ��4�ֽڶ���


#if _VGS_BUILDING_DLL
# define VGS_EXPORT __declspec (dllexport)
#else 
# define VGS_EXPORT// __declspec (dllimport)
#endif

/////////// ���峣�� ////////////////////////////////////
/// �����256���ֽڣ�����'\0'
const INT MAX_NAMESTR_LENGTH  = 64;    // ����������������64
const INT MAX_ERRORSTR_LENGTH = 128;
const INT MAX_FILENAME_LENGTH = 256;   // �ļ�����·�������ƣ�������256�ֽ�

/////////////////////////////////////////////////////////

/////////////////////////////// vgs ��Ϣ����  ///////////////////////

#define WM_WEBMAX_PROGRESS (WM_USER + 300)          // ������Ϣ

////////////////////////////////////////////////////////////////////

/***����ö�����Ͷ���
*/
// ���ƶ��������
enum CONTROLTYPE
{
	CT_WALKER = 0,  // ��������ģʽ
	CT_EDITOR,      // ���չʾģʽ
	CT_FLYER,       // ��������ģʽ
	CT_NONE         // �ǽ���ģʽ 
};

/// 
enum MODELCONTROLSTATUS
{
	IDLE,			//����״̬
	MOVE_WORLD,		//����������ϵ�ƶ�
	MOVE_SELF,      //����������ϵ�ƶ�
	ROTATE_WORLD,   //����������ϵ��ת
	ROTATE_SELF,    //����������ϵ��ת
	SCALE_WORLD          //���ο�����ϵ����
};
///������Ϣ�ṹ��
typedef struct MOUSEINFO
{
	SHORT		X;
	SHORT		Y;				//����
	BOOL		LButton;		//����Ϊ1���ɿ�Ϊ0
	BOOL		MButton;		//�м�����Ϊ1�ɿ�Ϊ0
	BOOL		RButton;
	FLOAT		RollDis;		//����
}MOUSEINFO;

// ��갴������
enum VGS_MOUSEKEY_TYPE
{
	VMT_NONE = -1,  // ��Ч
	VMT_LCLICK,     // �������
	VMT_RCLICK,     // �Ҽ�����
	VMT_MCLICK,     // �м�����
	VMT_LDBCLICK    // ���˫��
};

///���̵���Ϣ�ṹ��
typedef struct KEYBOARDINFO
{
	BYTE		Key[256];
}KEYBOARDINFO;

///D3D FORMAT
enum TEXFORMAT
{
	RGBA5650,
	RGBA5551,
	RGBA4444,
	RGBA8880,
	RGBA8888
};

///����ģʽ
enum ALIGNMODE
{
	ALIGN_NONE = 0,
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_TOP,
	ALIGN_BOTTOM,
	ALIGN_LEFT_TOP,
	ALIGN_MIDDLE_TOP,
	ALIGN_RIGHT_TOP,
	ALIGN_LEFT_BOTTOM,
	ALIGN_MIDDLE_BOTTOM,
	ALIGN_RIGHT_BOTTOM,
	ALIGN_CENTER
};

/// ����ʱ�Ĳο�ԭ��
enum VGSOFFSETORIGIN
{
	VOO_TOP_LEFT = 0,  // ���Ͻ�
	VOO_TOP_MIDDLE,    // ������е�
	VOO_TOP_RIGHT,     // ���Ͻ�
	VOO_MIDDLE_LEFT,   // ����ߵ��е�
	VOO_CENTER,        // ����
	VOO_MIDDLE_RIGHT,  // ���ұߵ��е�
	VOO_BOTTOM_LEFT,   // ���½�
	VOO_BOTTOM_MIDDLE, // ��������е�
	VOO_BOTTOM_RIGHT   // ���½�
};


///��Ƶ�ĸ�ʽ
enum MOVIETYPE
{
	MOVIETYPE_AVI = 0,
	MOVIETYPE_MPG,
	MOVIETYPE_WMV,
	MOVIETYPE_MP4,
	MOVIETYPE_RMVB
};

///���弸��Ĭ�ϵ����
enum DEFAULTCAMERA_TYPE
{
	CAME_NONE = 0,  // ��Ч�����
	USER_DEFINE,		//�û��Զ������
	FRONT_CAMERA,
	BACK_CAMERA,
	LEFT_CAMERA,
	RIGHT_CAMERA,
	TOP_CAMERA,
	BOTTOM_CAMERA,
	USER_CAMERA,         //Ĭ�����
	PERSPECTIVE_CAMERA   //Ĭ��͸��
};

// �����ͶӰ��ʽ
enum VGS_CAMERA_TYPE
{
	VCT_ORTHO = 0,  // 0: ������
	VCT_PROJECT     // 1��͸�����
};

//-------------------------------------------------------------------------------------------
/***ʸ������
*/
//-------------------------------------------------------------------------------------------
typedef struct VECTOR2
{
	FLOAT x;
	FLOAT y;
}VECTOR2;

typedef struct VECTOR3
{
	FLOAT x;
	FLOAT y;
	FLOAT z;
	VECTOR3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	VECTOR3(FLOAT X, FLOAT Y, FLOAT Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	VECTOR3 operator +(VECTOR3 v)
	{
		return VECTOR3(x + v.x,	y + v.y, z + v.z);
	}
	VECTOR3 operator +=(VECTOR3 v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	VECTOR3 operator -()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}
	VECTOR3 operator -(VECTOR3 v)
	{
		return VECTOR3(x - v.x,	y - v.y, z - v.z);
	}
	VECTOR3 operator -=(VECTOR3 v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	VECTOR3 operator *(float v)
	{
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}
	VECTOR3 operator *=(float v)
	{
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}
}VECTOR3;

typedef VECTOR3 vector3;

// vgs��Ԫ�������ڼ�¼ת��
typedef struct VGS_QUATERNION
{
	FLOAT x;
	FLOAT y;
	FLOAT z;
	FLOAT w;
	VGS_QUATERNION()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}
	VGS_QUATERNION(FLOAT X, FLOAT Y, FLOAT Z, FLOAT W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
}VGS_QUATERNION;

typedef VGS_QUATERNION quaternion;

//-------------------------------------------------------------------------------------------
///��ɫ����
//-------------------------------------------------------------------------------------------
typedef struct VGSRGB
{
	BYTE red;
	BYTE green;
	BYTE blue;
	VGSRGB()
	{
		red = green = blue = 255;
	}
	VGSRGB(BYTE r, BYTE g, BYTE b)
	{
		red = r; green = g; blue = b;
	}
}VGSRGB;

/// RGBA���ݸ�ʽ
typedef struct VGSRGBA
{
	BYTE red;
	BYTE green;
	BYTE blue;
	BYTE alpha;
	VGSRGBA()
	{
		red = green = blue = alpha = 255;
	}
	VGSRGBA(BYTE r, BYTE g, BYTE b, BYTE a)
	{
		red = r; green = g; blue = b; alpha = a;
	}
}VGSRGBA;

///�����Χ����Ϣ�ṹ
typedef struct BoundSphere
{
	VECTOR3 vCenter; //���ĵ�
	FLOAT   fRadius; //�뾶
}BoundSphere;

///�����Χ�����ݽṹ
typedef struct BoundBox
{
	VECTOR3 vMin; //���½�
	VECTOR3 vMax; //���Ͻ�
}BoundBox;

///����walker�����ݽṹ
typedef struct WalkerInfo
{
	CHAR	Name[MAX_NAMESTR_LENGTH];		        //Walker������
	// INT			cameraIndex;	//ʹ�õ�������
	CHAR        Camera[MAX_NAMESTR_LENGTH];			//ʹ�õ��������

	FLOAT	walkSpeed;		//Walker������������ٶ�
	FLOAT	walkAcc;		//Walker����������߼��ٶ�
	FLOAT	rosSpeed;		//Walker���������ת�ٶ�
	FLOAT	rosAcc;			//Walker���������ת���ٶ�
	FLOAT   height;			//Walker�ĸ߶�
	
	BOOL	bCollision;		//�Ƿ�����ײ
	FLOAT	colDis;			//��ײ����

	BOOL	bWeight;		//�Ƿ�������
	FLOAT	liftSpeed;		//�����½����ٶȣ�ʧȥ����ʱ��Ч
	FLOAT	liftAcc;		//�����½��ļ��ٶȣ�ʧȥ����ʱ��Ч

	FLOAT	upperAngle;		//̧ͷ���������
	FLOAT	lowerAngle;		//��ͷ������ӽ�
}WalkerInfo;

///����Editor�����ݽṹ
typedef struct EditorInfo
{
	CHAR		Name[MAX_NAMESTR_LENGTH];							//Editor������
	// INT			cameraIndex;					//ʹ�õ�������
	CHAR        Camera[MAX_NAMESTR_LENGTH];						    //ʹ�õ��������

	BOOL		bIsAutoRoundRoll;					//Editģʽ���Ƿ��Զ�����

	FLOAT		autoSpeedX;						    // Editģʽ���Զ��� X ����ת���ٶȣ���λ����/����
	FLOAT		autoSpeedY;						    // Editģʽ���Զ��� Y ����ת���ٶȣ���λ����/����

	//FLOAT		autoPitchSpeed;						//Editģʽ���Զ��� X ����ת���ٶ�
	//FLOAT		autoRoundSpeed;						//Editģʽ���Զ��� Y ����ת���ٶ�
	//FLOAT		autoRollSpeed;						//Editģʽ���Զ��� X ����ת���ٶ�

	FLOAT		manualSpeedX;					    //Editģʽ���ֶ�����������ٶ��� X ��
	FLOAT		manualSpeedY;					    //Editģʽ���ֶ���ת������ٶ��� Y ��
	//FLOAT		manualPitchSpeed;					//Editģʽ���ֶ���ת������ٶ� Z
	//FLOAT		manualRoundSpeed;					//Editģʽ���ֶ�����������ٶ� Y

	FLOAT		manualPanXSpeed;					//ƽ�Ƶ�X�����ٶ� 
	FLOAT		manualPanYSpeed;					//ƽ�Ƶ�Y�����ٶ� 

	FLOAT		dollySpeed;							//��������ʱ�ƶ�������ٶ� [1, 100]
	FLOAT		dollyAcc;							//����������ٶ�

	FLOAT		dollyNearDis;						//�����Dolly
	FLOAT		dollyFarDis;						//��Զ��Dolly

	INT			rosAxis;							// ��ת���־λ, 0: xy, 1 : x, 2 : y
	VECTOR3     LookAtPos;                          // Editor��Ŀ��� 

	DWORD       LookAtModelIndex;                   // �������ĳ��ģ�ͣ�����ģ�͵����������û��ģ�ͣ���ֵΪ0xffffffff
	INT         LookAtType;                         // 0���������� 1�������е�һ��  2��ģ��3�������Ŀ���

}EditorInfo;


///����Flyer�����ݽṹ
typedef struct FlyerInfo
{
	CHAR		Name[MAX_NAMESTR_LENGTH];							//Editor������
	// INT			cameraIndex;						//ʹ�õ�������
	CHAR        Camera[MAX_NAMESTR_LENGTH];						    //ʹ�õ��������

	FLOAT		panSpeed;							//ƽ���ٶ�
	FLOAT		panAcc;								//ƽ�Ƽ��ٶ�

	FLOAT		dollySpeed;							//��������ٶ�
	FLOAT		dollyAcc;							//����������ٶ�

	FLOAT		dollyNearDis;						//�����Dolly
	FLOAT		dollyFarDis;						//��Զ��Dolly

	FLOAT		rotateSpeed;						//�����ת���ٶ�
	FLOAT		rotateAcc;							//�����ת�ļ��ٶ�

	FLOAT		liftSpeed;							//�����½����ٶȣ�ʧȥ����ʱ��Ч
	FLOAT		liftAcc;							//�����½��ļ��ٶȣ�ʧȥ����ʱ��Ч

	FLOAT		upperAngle;							//̧ͷ���������
	FLOAT		lowerAngle;							//��ͷ������ӽ�

	BOOL		bCollision;							//�Ƿ�����ײ
	FLOAT		colDis;								//��ײ����
}FlyerInfo;

///������������Ե����ݽṹ
// typedef struct CameraInfo
// {
// 	VECTOR3                 from;			// ����������
// 	VGS_QUATERNION          quat;           // ת��
// 
// 	FLOAT	                nearClip;		// �������nearClip
// 	FLOAT					farClip;		// �������farClip
// 	FLOAT					fov;			// �������fov
// 	BYTE					ProjectType;    // �����ͶӰ��ʽ�� 0: ��������1��͸�����
// 
// }CameraInfo;

///������������Ե����ݽṹ
typedef struct CameraInfoEx
{
	VECTOR3					from;			// ����������
	VECTOR3                 at;             // �����looat�ķ������ at - from
	VECTOR3					Up;             // Up����

	FLOAT                   roll;           // ��look at��������ת�Ƕ�

	FLOAT	                nearClip;		// �������nearClip
	FLOAT					farClip;		// �������farClip
	FLOAT					fov;			// �������fov
	VGS_CAMERA_TYPE			ProjectType;    // �����ͶӰ��ʽ�� 0: ��������1��͸�����

}CameraInfoEx;

typedef CameraInfoEx CameraInfo;

// �ƹ�����
enum VGS_LIGHT_TYPE
{
	VLT_OMNI = 0,
	VLT_DIR,
	VLT_SPOT
};

///����ƹ����Ե����ݽṹ
typedef struct LightInfo
{
	VGS_LIGHT_TYPE	Type;  // 0: LT_POINT, 1: LT_DIRECTIONAL, 2:LT_SPOTLIGHT
    VGSRGB			Diffuse;
	VGSRGB			Ambient;
	FLOAT			Multiply;
    VECTOR3		    Position;
    VECTOR3		    Direction;
    FLOAT           Range;
    FLOAT           Falloff;
    FLOAT           Attenuation0;
    FLOAT           Attenuation1;
    FLOAT           Attenuation2;
    FLOAT           Theta;
    FLOAT           Phi;
}LightInfo;

///����������Ե����ݽṹ
typedef struct MtrlInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];		//������
	VGSRGB				Ambient;		//����ɫ
    VGSRGB				Diffuse;		//��ɫ
	VGSRGB				Specular;		//�߹���ɫ
	FLOAT			SpecularLevel;	   //�߹�ǿ��
	FLOAT			shininess;	       //������Χ
	// VGSRGB				Emissive;		//�Է�����ɫ
	FLOAT           Emissive;       // �Է���ֵ[0, 100]���������Է�����ɫ����diffuse * Emissive
	INT				Opacity;		//͸���� [0, 100]
	BOOL			bTwoSide;		//�Ƿ�˫��
	BOOL			bAlphaTest;		//�Ƿ����Alpha����
	INT				FillMode;		//���ģʽ  1: ��ģʽ�� 2���߿�ģʽ 3��ʵ��ģʽ��ȱʡ��3

	INT             AlphaTestValue;  // AlphaTest������ֵ��С�ڸ�����ֵ�ĵ�ᱻ�޳���

	//float           USpeed;          // ��̬UV���ٶ�
	//float           VSpeed;

	BOOL            bUseDiffuse;    // �Ƿ�ʹ�ò��ʵ�diffuse��ɫ�������ʹ�ã���Ч��diffuse��ɫΪ��ɫ��

}MtrlInfo;

// ������ͼ������
const INT TEXTURECOORD_DIFFUSE       = 0;   // diffuse
const INT TEXTURECOORD_LIGHTMAP      = 1;   // 
const INT TEXTURECOORD_REFLECT       = 2;   // 
const INT TEXTURECOORD_OPACITY       = 3;   // diffuse
const INT TEXTURECOORD_SHADOW        = 4;   // 
const INT TEXTURECOORD_BUMP          = 5;   // 
const INT TEXTURECOORD_WATER         = 6;   // diffuse
const INT TEXTURECOORD_CUBE          = 7;   // 
const INT TEXTURECOORD_DYNAMIC       = 8;   // 
const INT TEXTURECOORD_MOVIE         = 9;   // 
const INT TEXTURECOORD_FLASH         = 10;   // 

// ������ͼ��ͼƬ����
enum VGS_CUBIC_FACE
{
	VCF_FRONT = 0,
	VCF_BACK,
	VCF_LEFT,
	VCF_RIGHT,
	VCF_TOP,
	VCF_BOTTOM
};

///������ͼ���Ե����ݽṹ
typedef struct TexInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];		//��ͼ��
	INT				Width;			//��ͼ�߶�
	INT				Height;			//��ͼ���
	TEXFORMAT		Format;			//��ʽ
	INT				MipLevel;		//Mip�㼶			
}TexInfo;

///����CubeMap��ͼ���Ե����ݽṹ
typedef struct CubeMapInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];
	INT				Size;				//��ͼ�ߴ�
	TEXFORMAT		Format;				//��ʽ
}CubeMapInfo;

///����BumpWater��ͼ���Ե����ݽṹ
typedef struct BumpWaterInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];			
	INT				Width;					//��ͼ�Ŀ�Ⱥ͸߶�
	INT				Height;
	FLOAT			XScale;					//ˮ���Ƶ�X�����ܶ�
	FLOAT			YScale;					//ˮ���Ƶ�Y�����ܶ�
	FLOAT			Radius;					//����
	FLOAT			Speed;					//����
}BumpWaterInfo;

///����MovieTex��ͼ���Ե����ݽṹ
typedef struct MovieTexInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];				//��������
	MOVIETYPE		Type;					//�ļ�����
	BOOL			WithAudio;				//�Ƿ񲥷���Ƶ
	INT				RepeatCnt;				//ѭ�����ŵĴ����� -1Ϊ���޴�ѭ��
}MovieTexInfo;

///����Image����Ϣ
typedef struct ImageInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];
	INT				FileSize;
	INT				Width;
	INT				Height;
}ImageInfo;

// ǰ��ͼ������ͼ����
typedef struct OverlayInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];  // ����
	CHAR            BaseImg[256];  // ����ͼƬ����
	CHAR            OpacImg[256];  // ͸��ͼƬ����

	BOOL            bRelativeSize; // �Ƿ�ʹ����Գߴ�
	BOOL            bRelativePos;  // �Ƿ�ʹ�����λ��
	BOOL            bVisible;      // �Ƿ�ɼ�

	FLOAT           Opacity;       // ͸����
	FLOAT			Rotation;      // ��ת�Ƕ�

	FLOAT           ActualWidth;   // ͼ����ʵ�Ŀ��
	FLOAT           ActualHeight;  // ͼ����ʵ�ĸ߶�

	VGSOFFSETORIGIN             Origin;  // ����ʱ�Ĳο�ԭ�㣬VGSOFFSETORIGINֵ 
	INT				Width;   // Ŀǰ�Ŀ��
	INT				Height;  // Ŀǰ�ĸ߶�
	INT             Left;    // �����꣬������ӿ����Ͻ� 
	INT             Top;     // �����꣬������ӿ����Ͻ� 

}OverlayInfo;

///����FRect
typedef struct FRect
{
	FLOAT			Left;
	FLOAT			Top;
	FLOAT			Right;
	FLOAT			Bottom;
	FRect(){Left = Top = Right = Bottom = 0.0f;}
	FRect(FLOAT l, FLOAT t, FLOAT r, FLOAT b){Left = l; Top = t; Right = r; Bottom = b;}
}FRect;

// operation type of open v3d
enum V3D_OPENTYPE
{
	VOP_REPLACE = 0,
	VOP_MERGE
};

// ��Ӱ�Χ�еķ�ʽ
enum VGS_ADDTYPE
{
	VAT_ADDTYPE_REPLACE = 0,  // ����һ����Χ�У�ͬʱȡ�������İ�Χ��
	VAT_ADDTYPE_REVERSE,      // ������ʵ��Χ�У�����ʾ�������а�Χ��
	VAT_ADDTYPE_ADD           // ����һ����Χ��
};
// ��������
enum VGS_ANIMATIONTYPE
{
	VAT_ANIMATIONTYPE_MODEL = 0,        // ģ�͹ؼ�֡����
	VAT_ANIMATIONTYPE_CAMERA,           // ����ؼ�֡����
	VAT_ANIMATIONTYPE_LIGHT,            // �ƹ⶯��
	VAT_ANIMATIONTYPE_SKELETON,          // ��������
	VAT_ANIMATIONTYPE_NONE
};

// ��������״̬
enum VGS_ANIMATIONSTATUS
{
	VAS_PLAYING = 0,  // ����
	VAS_PAUSED,       // ��ͣ
	VAS_STOP,          // ֹͣ
	VAS_NONE
};

// gui����
enum VGS_GUIOBJ_TYPE
{
	VGS_GUIOBJ_TYPE_NONE = 0,  
	VGS_GUIOBJ_TYPE_OVERLAY,      // ǰ��ͼ
	VGS_GUIOBJ_TYPE_BACKDROP,     // ����ͼ
	VGS_GUIOBJ_TYPE_BUTTON,       // ��ť 
	VGS_GUIOBJ_TYPE_NAVIGATION,    // ����ͼ
	VGS_GUIOBJ_TYPE_NAVIPOINTOR,    // ����ͼָ��
	VGS_GUIOBJ_TYPE_FLASH			//Flash�ؼ�GUI.
};

// ����������
enum VGS_AXIS_TYPE
{
	VGS_AXIS_TYPE_NONE = 0,  // ���������Ե���

	// ��������ϵ����
	VGS_LAXIS_X,
	VGS_LAXIS_Y,
	VGS_LAXIS_Z,
	VGS_LAXIS_NEGATIVEX,  // -x
	VGS_LAXIS_NEGATIVEY,  // -y
	VGS_LAXIS_NEGATIVEZ,  // -z

	// ��������ϵ����
	VGS_WAXIS_X,
	VGS_WAXIS_Y,
	VGS_WAXIS_Z,
	VGS_WAXIS_NEGATIVEX,
	VGS_WAXIS_NEGATIVEY,
	VGS_WAXIS_NEGATIVEZ,

	// ���ڵ�����ϵ����
	VGS_PAXIS_X,
	VGS_PAXIS_Y,
	VGS_PAXIS_Z,
	VGS_PAXIS_NEGATIVEX,
	VGS_PAXIS_NEGATIVEY,
	VGS_PAXIS_NEGATIVEZ
};

// transform(ƽ�ơ���ת������)�����Ĳο�����ϵ
enum VGS_TRANSFORMSPACE
{
	VTS_WORLD = 0,
	VTS_LOCAL,
	VTS_PARENT
};

// ��ת�Ĳο�����
enum VGS_ROTATIONSPACE
{
	VRS_WORLD = 0,
	VRS_WORLD2,
	VRS_LOCAL
};

// ��������
enum VGS_REFLECT_TYPE
{
	VRT_INVALID = -1,
	VRT_PLANAR,
	VRT_CURVED,
	VRT_REFLECTION,
	VRT_NORMAL
};

// ���������ٶ���Чֵ
#define VGS_ANIMSPEED_NONE (-10000)


///////////// texture�Ļ�ɫ��ʽ /////////////////////

// texture������
// enum VGS_TextureType
// {
// 	VTT_INVALID = 0,  // ��Ч��ͼ
// 	VTT_BASAL,		  // ������ͼ
// 	VTT_CUBIC,
// 	VTT_DYNAMIC,
// 	VTT_MOVIE,
// 	VTT_FLASH,
// };
// texture������
enum VGS_TextureType
{
	//VTT_INVALID = 0,  // ��Ч��ͼ
	//VTT_BASAL,		  // ������ͼ
	//VTT_CUBIC,
	//VTT_DYNAMIC,
	//VTT_MOVIE,
	//VTT_FLASH,
	//VTT_BUMP
	TLT_INVALID = 0,
	TLT_BASAL,						// ������ͼ
	TLT_OPACITY,		            // ͸����ͼ
	TLT_REFLECT,                    // ������ͼ
	TLT_REALTIMESHADOW,             // ʵʱ��Ӱ,��ʱ��֧��
	TLT_CUBEMAP,                    // ����ͼ��������̬������ͼ��ʵʱ����������ͼ
	TLT_DYNAMIC,                    // ��̬��ͼ
	TLT_RTFMAP,						// ʵʱ����,��ʱ��֧��
	TLT_MOVIE,                      // ��Ӱ��ͼ,��ʱ��֧��
	TLT_FLASH,						// Flash��ͼ,��ʱ��֧��
	TLT_BUMPMAP,					// ��͹��ͼ��������͹���ƣ���͹�������䣬������ͼ��
	TLT_HEIGHTMAP,					// �߶���ͼ,��ʱ��֧��
	TLT_RTTMAP						// ʵʱ����,��ʱ��֧��
};

// bump����
enum VGS_BumpType
{
	VBT_BUMPINVALID = 0,
	VBT_BUMPWATER,           // bump ˮ��Ч
	VBT_BUMPENVIRONMENT,     // bump ��������
	VBT_BUMPNORMARLMAP       // ������ͼ
};

// cube map����
enum VGS_CubemapType
{
	VCT_INVALID = -1,
	VCT_STATIC,      // ��̬��ͼ
	VCT_REALTIME     // ʵʱ���� 
};

// bumpwater����
typedef struct VGS_BumpwaterTextureParam
{
	INT             width;
	INT			    height;
	FLOAT			XScale;					//ˮ���Ƶ�X�����ܶ�
	FLOAT			YScale;					//ˮ���Ƶ�Y�����ܶ�
	FLOAT			Radius;					//����
	FLOAT			Speed;					//����
	
	FLOAT           CenterX;	// ���Ƶ����ĵ�����
	FLOAT			CenterY;   
}VGS_BumpwaterTextureParam;

// bump��������
typedef struct VGS_BumpEnvTextureParam
{
	INT width;
	INT height;
	float   noise1;
	float   noise2;
}VGS_BumpEnvTextureParam;

// ������ͼ
typedef struct VGS_BumpNormalTextureParam
{
	DWORD   LightHandle;
}VGS_BumpNormalTextureParam;

typedef union VGS_BumpParam
{
	VGS_BumpwaterTextureParam    waterParam;
	VGS_BumpEnvTextureParam      envParam;
	VGS_BumpNormalTextureParam   normalParam;
	
}VGS_BumpParam;

// ��ɫ����
enum VGS_BlenderType
{
	VBT_COLOR = 0,
	VBT_ALPHA
};

// ��ɫ��ʽ
enum VGS_BlenderOP
{
	VBO_DISABLE = 0,  // ʹ��source2
	VBO_REPLACE,      // ��source1�滻source2
	VBO_MODULATE,     // source2 * source1
	VBO_MODULATE2,    // VBO_MODULATE * 2
	VBO_MODULATE4,    // VBO_MODULATE * 4	
	VBO_ADD,          // source2 + source1        
    VBO_ADD_SIGNED,	/// as LBX_ADD, but subtract 0.5 from the result        
    VBO_ADD_SMOOTH,	/// as LBX_ADD, but subtract product from the sum
    VBO_SUBTRACT,	  // source1 - source2
	VBO_BLEND_DIFFUSE_ALPHA,		/// use interpolated alpha value from vertices to scale source1, then add source2 scaled by (1-alpha)
    VBO_BLEND_TEXTURE_ALPHA,		/// as LBX_BLEND_DIFFUSE_ALPHA, but use alpha from texture        
    VBO_BLEND_CURRENT_ALPHA,		/// as LBX_BLEND_DIFFUSE_ALPHA, but use current alpha from previous stages       
    VBO_BLEND_MANUAL,			/// as LBX_BLEND_DIFFUSE_ALPHA but use a constant manual blend value (0.0-1.0)        
    VBO_DOTPRODUCT,					/// dotproduct of color1 and color2         
    VBO_BLEND_DIFFUSE_COLOUR	/// use interpolated color values from vertices to scale source1, then add source2 scaled by (1-color)        
    
};

 enum VGS_LayerBlendSource
{
    /// the colour as built up from previous stages
    VBS_CURRENT,
    /// the colour derived from the texture assigned to this layer
    VBS_TEXTURE,
    /// the interpolated diffuse colour from the vertices
    VBS_DIFFUSE,
    /// the interpolated specular colour from the vertices
    VBS_SPECULAR,
    /// a colour supplied manually as a separate argument
    VBS_MANUAL
};

// ��ɫ����
typedef struct VGS_BlenderParam
{
	//VGS_LayerBlendSource source1;
	//VGS_LayerBlendSource source2;
	float BlenderOP_alpha;  // ��VGS_BlenderOP == VBO_BLEND_MANUALʱ��Ч����ɫģʽ���õ��Ĳ���alpha,

}VGS_BlenderParam;

// texturelayer�Ļ�ɫ��Ϣ
typedef struct VGS_BlenderInfo
{
	VGS_BlenderOP       option;       // 
	VGS_BlenderParam    param;        // ��ɫ����
}VGS_BlenderInfo;

// ģ��������Ϣ
typedef struct VGS_ModelInfo
{
	// CHAR  sName[MAX_NAMESTR_LENGTH];
	BOOL  isBillboard;
	BOOL  isCollision;
	BOOL  isGround;
	BOOL  isVisible;
	BOOL  isClickable;                // ����״̬���Ƿ�ɵ��
	VGS_AXIS_TYPE   dirctionAxis;     // ģ����������ķ�����
}VGS_ModelInfo;

// 3d��������
enum VGS_MOVABLETYPE
{
	VGS_MOVABLETYPE_NONE = 0,
	VGS_MOVABLETYPE_MODEL,
	VGS_MOVABLETYPE_CAMERA,
	VGS_MOVABLETYPE_LIGHT
};

// ����¼�����
enum VGS_MOUSEEVENT_TYPE
{
	VGS_MOUSEEVENT_TYPE_NONE = 0,
	VGS_MOUSEEVENT_TYPE_OVER,         // 
	VGS_MOUSEEVENT_TYPE_DBCLICK,      
	VGS_MOUSEEVENT_TYPE_LEFTDOWN,
	VGS_MOUSEEVENT_TYPE_LEFTUP,
	VGS_MOUSEEVENT_TYPE_RIGHTDOWN,
	VGS_MOUSEEVENT_TYPE_RIGHTUP,
	VGS_MOUSEEVENT_TYPE_MOVE,         // �ƶ�
	VGS_MOUSEEVENT_TYPE_LEAVE,        // �뿪 
	VGS_MOUSEEVENT_CNT
};


// ���������
typedef enum WEBBROWSERTYPE
{
	WBT_IE = 0,
	WBT_FF,
	WBT_GOOGLE,
	WBT_OPERA,
	WBT_SAFARI
}WEBBROWSERTYPE;

// ���������
typedef enum WEBBROWSER_SUBTYPE
{
	WBST_IE      = 0,
	WBST_MAXTHON,
	WBST_TENCENT
}WEBBROWSER_SUBTYPE;

extern WEBBROWSERTYPE  g_BrowserType/* = WBT_IE*/;        // ��������ͣ�0 ��ie 1��ff 2��google
extern WEBBROWSER_SUBTYPE g_BrowserSubType/* = WBST_IE*/;


#pragma pack(pop)

//}
//
//using namespace VGS;

#endif