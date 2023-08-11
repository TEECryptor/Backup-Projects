/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // V3D_Def.h
* ����ժҪ�� // V3D���ݽṹ����
* ����˵���� // 
* ��ǰ�汾�� // V2.0
* ��    �ߣ� // ���²�
* ������ڣ� // 
* 
* �޸�����      �汾��     �޸���	      �޸�����
* ---------------------------------------------------------------
* 
*
*********************************************************************/

#pragma once

#include "..\VGSHeader.h"
//#include "..\VGSDef.h"
#include "s3d_def.h"

#include <vector>
using namespace std;

#pragma pack(push, 1) // ���ݽṹ�Ե��ֽڶ���

//��������ṹV3D��λ�ú�ת�ǽ���Node
typedef struct VGS_V3D_CAMINFO_OLD
{
	//�Ƿ���������
	bool					isOrtho;

	//��ͷ
	float					fov;

	//����������Զ������
	float					nearClipDis;
	float					farClipDis;

	VECTOR3                 position;      // λ��  
	VGS_QUATERNION          quat;          // ת��

}VGS_V3D_CAMINFO_OLD;

typedef struct VGS_V3D_CAMINFO
{
	//�Ƿ���������
	bool					isOrtho;
	
	//��ͷ
	float					fov;
	
	//����������Զ������
	float					nearClipDis;
	float					farClipDis;
	
	VECTOR3                 position;      // λ��

	VECTOR3                 at;
	quaternion              quat;          // ת��
	
}VGS_V3D_CAMINFO;


typedef struct VGS_V3D_CAMINFO_V3
{
	//�Ƿ���������
	bool					isOrtho;

	//��ͷ
	float					fov;

	//����������Զ������
	float					nearClipDis;
	float					farClipDis;

	VECTOR3                 position;      // λ��

	VECTOR3                 at;
	VECTOR3                 up;

}VGS_V3D_CAMINFO_V3;

// V3D���
typedef struct VGS_V3D_Camera_old
{
	//���������
	int                    nameLen;
	std::string            name;

	//�����ʼ��Ϣ
	VGS_V3D_CAMINFO_OLD				v3dBaseInfo;			// ������V3D, ���������Ϣ
	std::vector<VGS_V3D_CAMINFO>	v3dAnimationInfo;		// ������V3D���������

}VGS_V3D_Camera_old;

typedef struct VGS_V3D_Camera
{
	//���������
	int                             nameLen;
	std::string                     name;
	
	//�����ʼ��Ϣ
	VGS_V3D_CAMINFO_V3				v3dBaseInfo;			// ������V3D, ���������Ϣ
	std::vector<VGS_V3D_CAMINFO>	v3dAnimationInfo;		// ������V3D���������
	
}VGS_V3D_Camera;

//�ƹ�Ŀɱ����V3D
typedef struct VGS_V3D_OMNI
{
	//��ɫ
	DWORD					color;
	//ǿ��
	float					multiply;
	
	//���ߵ�λ��
	VECTOR3                 position;

	//����˥����Χ
	float					attenuationRange;
	float					attenuation0;
	float					attenuation1;
	float					attenuation2;
}VGS_V3D_OMNI;

//�ƹ�Ŀɱ����V3D
typedef struct VGS_V3D_SPOTLIGHT
{
	//��ɫ
	DWORD					color;
	//ǿ��
	float					multiply;
	
	//���ߵ�λ�úͷ���
	VECTOR3                 position;
	VECTOR3					direction;

	//����˥����Χ
	float					attenuationRange;
	float					attenuation0;
	float					attenuation1;
	float					attenuation2;

	//��Ȧ����ķ�Χ��
	float					innerAngle;
	float					outerAngle;
	float					fallOff;
}VGS_V3D_SPOTLIGHT;


//�ƹ�Ŀɱ����V3D
typedef struct VGS_V3D_DIRECTIONAL
{
	//��ɫ
	DWORD					color;
	//ǿ��
	float					multiply;
	
	//���ߵ�λ�úͷ���
	VECTOR3					direction;

	//����˥����Χ
	float					attenuationRange;
	float					attenuation0;
	float					attenuation1;
	float					attenuation2;

}VGS_V3D_DIRECTIONAL;

//�ƹ�
typedef struct VGS_V3D_Light
{
	//Node������
	int						strLen;
	std::string 			name;

	//�ƹ�����
	VGS_LIGHT_TYPE   		type;

	//V3D����ĵƹ����֡����
	std::vector<VGS_V3D_OMNI>				omniInfo;
	std::vector<VGS_V3D_DIRECTIONAL>		directionLightInfo;
	std::vector<VGS_V3D_SPOTLIGHT>			spotLightInfo;

}VGS_V3D_Light;

//////////////////////////////////// V3D //////////////////////////////////////////

const char SUNTOWARD_NAME[] = "SUNTOWARD\0";

/** V3D�汾
1. 2008-10-28֮ǰ 5.0000
2. 2008-10-29     50001  // ���汾�Ÿ�Ϊ������50001 = 5.00.01
3. 2008-12-08     50002  // V3D��cube map������ͼ�ļ�����
4. 2008-12-13     50003  // ģ����Ϣ�ṹ�޸�
5. 2008-12-15     50004  // editor��Ϣ����lookatmodelindex�ֶ�
*/
const float CURRENT_V3DVERSION = 60000; 

const int MAX_CORPNAME_LENGTH = 64;

//V3D�ļ�ͷ
typedef struct V3D_Header
{
	//��ͼ��˾������
	char					corp[MAX_CORPNAME_LENGTH];
	//v3d�ļ��İ汾
	float					v3dVersion;
	CHAR                    Reserverd[60]; // ������չ
}V3D_Header;

// V3D����Ŀ¼��
typedef struct V3D_Table  // 96 bytes
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

	// ģ����Դ���ݵ�ƫ����
	DWORD					meshDataPtr;

	// ģ����Ϣ����ƫ����
	DWORD                   entityDataPtr;  // fuxb fuxb 2008-07-23

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

	DWORD                   BackdropPtr;  // ����ͼ  fuxb 2008--7-17

	// ����
	DWORD                   AudioPtr;
	// ������Ϣ
	DWORD                   ControlPtr;
	// flash
	DWORD                   FlashPtr;

	// ��Ȩ��Ϣ
	DWORD                   PrivacyDataPtr;


	CHAR                    Reserved[28];  // ������չ
}V3D_Table;

//�ṹ�壺����������
typedef struct V3D_SceneProp
{
	INT				Width;				//������
	INT				Height;				//����߶�
	DWORD			BackColor;			//������ɫ
	INT				FPS;				//��Ⱦ֡��
	INT				AntiAliasLevel;		//���ܳݼ���
	//INT				FSBlurLevel;		//ȫ����̬ģ���ļ���
	//INT				FSGlowLevel;		//ȫ�����εļ���

	//INT				KeyframeCnt;		//�ؼ�֡������
	//INT				RepeatCnt;			//ѭ�����ŵĴ����� -1Ϊ���޴�ѭ��
}V3D_SceneProp;


typedef struct V3D_Animation
{
}V3D_Animation;

//�ṹ��: �������ݽṹ
typedef struct V3D_Audio
{
	CHAR			Name[32];				//��Ƶ������
	INT             FileNameLen;            //��Ƶ�ļ�·������
	CHAR*			pFileName;		    	//��Ƶ�ļ�ȫ·��
	BYTE			Type;					//�ļ�����
	INT				RepeatCnt;				//�ز�����
}V3D_Audio;

//��Ƶ�б�
typedef struct V3D_AudioList
{
	INT				AudioCnt;			//��Ƶ������
	V3D_Audio*	    Audio;				//��Ƶ�б�
}V3D_AudioList;

//�ṹ��: Flash���ݽṹ
typedef struct V3D_Flash
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
}V3D_Flash;

//�ṹ��: Flash�б�
typedef struct V3D_FlashList
{		
	INT				FlashCnt;			//Flash����
	V3D_Flash*	    Flash;				//Flash�б�
}V3D_FlashList;

//�ṹ�壺������Ϣ
typedef struct V3D_TextInfo
{
	DWORD	Color;
	INT			X;
	INT			Y;
}V3D_TextInfo;

//�ṹ��: ���ֻ����ṹ
typedef struct V3D_Text
{
	INT						Length;		//�ַ�����
	CHAR*					StrText;	//�ַ���

	INT						X;
	INT						Y;			//���ֵ�λ��
	DWORD				Color;		//���ֵ���ɫ

	CHAR					FontName[32];	//������
	
	UINT					KfrCnt;		//�ؼ�֡����
	V3D_TextInfo*		KfrInfo;	//�ؼ�֡����
}V3D_Text;

//�ṹ��: �����б�
typedef struct V3D_TextList
{
	INT			TextCnt;	//��������
	V3D_Text*	Texts;		//�����б�
}V3D_TextList;

////�ṹ�壺Overlay��Ϣ
//typedef struct V3D_OverlayInfo
//{
//	RECT		Rect;			//Overlay�趨����Գߴ�
//	FLOAT		Angle;			//ת��
//	INT			Opacity;		//͸����
//}V3D_OverlayInfo;
//
////�ṹ��: Overlay�����ṹ
//typedef struct V3D_Overlay
//{
//	CHAR		Name[32];			//Overlay������
//
//	INT			OffsetOrigin;		//ƫ��λ�õ�����ԭ��
//	BOOL		UseRelativePos;		//ʹ�����λ��
//
//	RECT		Rect;				//Overlay�ߴ�
//	FLOAT		Angle;				//ת��
//	INT			Opacity;			//͸����
//	BOOL		IsBackdrop;			//�Ƿ��Ǳ���ͼ
//
//	UINT		MaterialID;			//Overlay�Ĳ���ID��
//	BOOL		UseRelativeSize;	//�Ƿ�ʹ����Եĳߴ�
//	UINT		KfrCnt;				//�ؼ�֡����
//	V3D_OverlayInfo*	KfrInfo;	//�ؼ�֡��Ϣ
//}V3D_Overlay;
//
////�ṹ��: Overlay�б�
//typedef struct V3D_OverlayList
//{
//	INT				OverlayCnt;	//Overlay������
//	V3D_Overlay*	Overlays;	//Overlay�б�
//}V3D_OverlayList;
//
////����ͼָ����Ϣ
//typedef struct V3D_NavigaterPointer
//{
//	CHAR		Name[32];			//Pointer������
//	UINT		OverlayIndex;		//Pointerʹ�õ�Overlay�ı��
//	UINT		Type;				//Pointer�����ͣ�Camera-0, Model-1
//	UINT		ObjIndex;			//�����ģ�͵ı��
//}V3D_NavigaterPointer;
//
////�ṹ��: Navigater�����ṹ
//typedef struct V3D_Navigater
//{
//	CHAR						Name[32];
//	RECT						Rect;						//����Mapƽ������
//
//	UINT						MapOverlayIndex;			//����Map(Overlay)�ı��
//
//	UINT						PointerCnt;					//Pointer������
//	V3D_NavigaterPointer*		NavPointer;					//Pointer��Ϣ
//
//	FLOAT						sceneLeft;					//����TopView��Rect
//	FLOAT						sceneTop;
//	FLOAT						sceneRight;
//	FLOAT						sceneBottom;
//}V3D_Navigater;
//
////�ṹ��: Navigater�б�
//typedef struct V3D_NavigaterList
//{
//	INT							NavigaterCnt;	//Navigater����
//	V3D_Navigater*			Navigaters;		//Navigater�б�
//}V3D_NavigaterList;

////�ṹ��: Button������
//typedef struct V3D_ButtonInfo
//{
//	RECT		Rect;				//��ť��Rect
//	INT			Opacity;			//��ť��͸����
//}V3D_ButtonInfo;
//
////�ṹ��: Button��Ԫ�ṹ
//typedef struct V3D_Button
//{
//	CHAR		Name[32];			//��ť������
//	RECT		Rect;				//��ť��Rect
//
//	INT			Opacity;			//��ť��͸����
//	CHAR		CallBackFunc[32];	//�ص���������
//	int		MouseUpMtrlIndex;	//Button Up��ID��
//	int		MouseOverMtrlIndex;	//overImage�Ĳ���ID��
//	int		MouseDownMtrlIndex;	//downImage�Ĳ���ID��
//	
//	UINT		KfrCnt;				//�ؼ�֡����
//	V3D_ButtonInfo*	KfrInfo;		//�ؼ�֡����
//
//}V3D_Button;
//
////�ṹ��: Button�б�
//typedef struct V3D_ButtonList
//{
//	INT					ButtonCnt;			//��ť����
//	V3D_Button*		Buttons;			//��ť�б�
//}V3D_ButtonList;

//�ṹ��: V3D_1_0��Ȩ�ṹ��
typedef struct V3D_Privacy
{
	INT				VersionType;			//�汾��ʾ(���˰汾0����׼�汾1����ҵ�汾2)
	DWORD			ClientIndex;			//��Ȩ�ͻ����
	CHAR			SerialNum[128];			//����Ȩ��������кţ��Ѿ���128λ����
	CHAR			Authorize[128];			//�Ƿ��Ѿ�����Ȩȥ�������к�
	DWORD			ExtendInfo1;			//��չ������Ϣ1
	DWORD			ExtendInfo2;			//��չ������Ϣ2
}V3D_Privacy;

//�ṹ��: Walker����Ľṹ��
typedef struct V3D_Walker
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
}V3D_Walker;

//�ṹ��: walker�����б�
typedef struct V3D_WalkerList
{
	INT				WalkerCnt;			//Walker������
	V3D_Walker* Walkers;			//Walkder�����б�
}V3D_WalkerList;

//�ṹ��: Editor����
typedef struct V3D_Editor
{	
	CHAR				Name[32];		//����

	INT					CameraIndex;	//ʹ�õ�������

	BOOL				bIsAutoRoundRoll;					//Editģʽ���Ƿ��Զ�����

	FLOAT				autoPitchSpeed;						//Editģʽ���Զ���X����ת���ٶ�
	FLOAT				autoRoundSpeed;						//Editģʽ���Զ���ת������ٶ�
	FLOAT				autoRollSpeed;						//Editģʽ���Զ�������̵��ٶ�

	FLOAT				manualPitchSpeed;					//Editģʽ���ֶ���ת������ٶ�
	FLOAT				manualRoundSpeed;					//Editģʽ���ֶ�����������ٶ�

	FLOAT				manualPanXSpeed;					//Editģʽ���ֶ�ƽ��������ٶ�
	FLOAT				manualPanYSpeed;					//Editģʽ���ֶ�ƽ��������ٶ�

	FLOAT				dollySpeed;							//��������ʱ�ƶ�������ٶ�
	FLOAT				dollyAcc;							//����������ٶ�

	FLOAT				dollyNearDis;						//�����Dolly
	FLOAT				dollyFarDis;						//��Զ��Dolly
}V3D_Editor;

//�ṹ�壺Editor�����б�
typedef struct V3D_EditorList
{
	INT					EditorCnt;							//Editor������
	V3D_Editor*		Editors;							//Editor�б�
}V3D_EditorList;

//�ṹ��: Flyer����
typedef struct V3D_Flyer
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
}V3D_Flyer;

//�ṹ��: Flyer�����б�
typedef struct V3D_FlyerList
{
	INT					FlyerCnt;						//�����������
	V3D_Flyer*		Flyers;							//��������б�
}V3D_FlyerList;

//�ṹ�壺���ƶ���
typedef struct V3D_ControlData
{
	INT					ControlType;					//��������
	INT					ControlerIndex;					//���ƶ�����

	V3D_WalkerList	WalkerList;						//���ζ����б�
	V3D_EditorList	EditorList;						//�༭�����б�
	V3D_FlyerList	FlyerList;						//���ж����б�
}V3D_ControlData;

//V3D�ļ��ṹ
typedef struct V3D_FileStruct
{
	//�ļ�ͷ
	V3D_Header				header;

}V3D_FileStruct;

//////////////////// �������ݽṹ��Ϊ�����д��V3Dʱ��Ч�ʶ������ ///////////////////////////////////

// v3d submesh����
typedef struct V3D_SubMeshDetail
{
	std::string sName;
	UINT indexCount;
	UINT indexSize;
	char* pIndexBuf;
	V3D_SubMeshDetail()
	{
		sName = "";
		indexSize = 0;
		indexCount = 0;
		pIndexBuf = 0;
	}
}V3D_SubMeshDetail;

// v3d mesh����
typedef struct V3D_MeshDetail
{
	std::string sName;
	UINT vertextCount;
	char* pVertexBuf;
	UINT subMeshCout;
	vector3 boxNearConner;
	vector3 boxFarConner;
	float   sphereRadius;
	// float   sphereCenter;
	vector<V3D_SubMeshDetail> vSubMesh;

	V3D_MeshDetail()
	{
		sName = "";
		vertextCount = 0;
		pVertexBuf   = 0;
		subMeshCout  = 0;
		boxFarConner = boxNearConner = vector3(0, 0, 0);
		sphereRadius = 0;
	}

}V3D_MeshDetail;

typedef struct V3D_MaterialDetail
{
}V3D_MaterialDetail;

typedef struct V3D_PassDetail
{
	BYTE ShadingMode;
	UINT Amb_RGBA;
	UINT Dif_RGBA;
	UINT Spe_RGBA;
	float shininess;
	UINT sel_RGBA;
	BYTE sBlendFactor;
	BYTE dBlendFactor;
	BYTE CullMode;
	bool bDepthCheck;
	bool bLighting;
	BYTE PolygonMode;
	bool bDepthWrite;
	BYTE RejFunc;
	BYTE AlphaValue;
	float Spe_Level;  // [0, 100] add by fuxb 2008-8-31
	float selfIllumination; // [0, 100]
	float USpeed;  // fuxb 2008-12-17 ����������
	float VSpeed;  // fuxb 2008-12-17 ����������

	bool  bUseDiffuse;  // fuxb 2008-10-28, �Ƿ�ʹ�ò��ʵ���ɫ

}V3D_PassDetail;


// fuxb 2008-10-28, ���ӱ�ʶ�Ƿ�ʹ�ò��ʵ���ɫ
typedef struct V3D_PassDetail_Ex
{	
	BYTE ShadingMode;
	UINT Amb_ARGB;
	UINT Dif_ARGB;
	UINT Spe_ARGB;
	float Spe_Level;  // [0, 100] add by fuxb 2008-8-31
	float shininess;
	float selfIllumination; // [0, 100]
	UINT sel_ARGB;
	BYTE bTwoside;
	BYTE PolygonMode;
	bool bAlphaBlend;
	bool bAlphaTest;
	BYTE AlphaTestValue;
	INT Opacity;		//͸���� [0, 100]
	bool bLighting;
	bool bDepthWrite;
	BYTE fillMode;

	bool  bUseDiffuse;  // fuxb 2008-10-28, �Ƿ�ʹ�ò��ʵ���ɫ
}V3D_PassDetail_Ex;

// ����ʵ�� �� д��v3d�ļ�
typedef struct V3D_CameraDetail 
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
}V3D_CameraDetail;

// ����ʵ�� �� д��v3d�ļ�
typedef struct V3D_LightDetail
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

}V3D_LightDetail;

// V3D�б����ǰ��ͼ����
typedef struct V3D_OverlayInfo   
{
	CHAR			Name[MAX_NAMESTR_LENGTH];  // ����
	CHAR            BaseImg[MAX_NAMESTR_LENGTH];  // ����ͼƬ����
	CHAR            OpacImg[MAX_NAMESTR_LENGTH];  // ͸��ͼƬ����
	// INT				materialIndex;  // ʹ�õĲ�������
	//CHAR            materialName[MAX_NAMESTR_LENGTH];   // ��ͼ�õ��Ĳ�������

	BOOL            bRelativeSize; // �Ƿ�ʹ����Գߴ�
	BOOL            bRelativePos;  // �Ƿ�ʹ�����λ��
	BOOL            bVisible;      // �Ƿ�ɼ�

	FLOAT           Opacity;       // ͸����

	INT             Origin;  // ����ʱ�Ĳο�ԭ�㣬VGSOFFSETORIGINֵ 

	FLOAT			Width;   // Ŀǰ�Ŀ��, ���ʹ����Դ�С�����ﱣ��ٷֱ� 
	FLOAT			Height;  // Ŀǰ�ĸ߶�
	FLOAT           Left;    // �����꣬������ӿ����Ͻ�, ���ʹ�����λ�ã����ﱣ��ٷֱ�
	FLOAT           Top;     // �����꣬������ӿ����Ͻ� 

	int             ZOrder;  // ���µĲ��˳��Խ��Խ����

}V3D_OverlayInfo;

// V3D�б����button����
typedef struct V3D_OverlayList
{
	UINT					count;
	V3D_OverlayInfo*		OverlayList;	
}V3D_OverlayList;


// V3D�б���ĵ���������
typedef struct V3D_NaviPointerInfo  
{
	CHAR        sName[MAX_NAMESTR_LENGTH];  // �����������
	int			NavType;		// 0��ʾ�����������1��ʾ��ģ�͵���
	// DWORD		PointedHandle;	// �����ģ�͵ľ��
	DWORD		PointedIndex;		// �����ģ�Ͷ��������

	CHAR        PointerImg[MAX_NAMESTR_LENGTH];  // ʹ�õ�ͼƬ����
	// int 		materialIndex;	// �������õ��Ĳ�������
	// CHAR            materialName[MAX_NAMESTR_LENGTH];   // ��ͼ�õ��Ĳ�������

	int         width;
	int         height;
	int         left;
	int         top;

	bool        bVisible;
	int         Opacity; // [0, 100]

}V3D_NaviPointerInfo;

typedef struct V3D_NavigatorBaseInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];    // ����
	CHAR            MapImg[MAX_NAMESTR_LENGTH];  // ��ͼͼƬ����
	// int             MapMaterialIndex;   // ��ͼ�õ��Ĳ�������
	// CHAR            materialName[MAX_NAMESTR_LENGTH];   // ��ͼ�õ��Ĳ�������

	BOOL            bRelativeSize; // �Ƿ�ʹ����Գߴ�
	BOOL            bRelativePos;  // �Ƿ�ʹ�����λ��
	BOOL            bVisible;      // �Ƿ�ɼ�

	FLOAT           Opacity;       // ͸����

	INT             Origin;  // ����ʱ�Ĳο�ԭ�㣬VGSOFFSETORIGINֵ 
	float			Width;   // Ŀǰ�Ŀ��, ���ʹ����Դ�С�����ﱣ��ٷֱ�
	float			Height;  // Ŀǰ�ĸ߶�
	float			Left;    // �����꣬������ӿ����Ͻ� , ���ʹ�����λ�ã����ﱣ��ٷֱ�
	float           Top;     // �����꣬������ӿ����Ͻ�

	int             ZOrder;  // ���µĲ��˳��Խ��Խ����, ���ڵ���ͼ������ZOrder��Ч

}V3D_NavigatorBaseInfo;


// V3D�б���ĵ���ͼ����
typedef struct V3D_NavigatorInfo   
{
	V3D_NavigatorBaseInfo		NaviBaseInfo;     // ����ͼ�Ļ�������

	float						mapRectMinX;	  // ����ͼӳ���3D�ռ䷶Χ�������Ϸ��ĸ������
	float                       mapRectMaxX;
	float                       mapRectMinZ;
	float                       mapRectMaxZ;

	INT							PointerCount;     // ���������Ŀ
	V3D_NaviPointerInfo*		PointerList;      // �������б�

}V3D_NavigatorInfo;

// V3D�б���ĵ���ͼ����
typedef struct V3D_NavigatorList   
{
	UINT				count;
	V3D_NavigatorInfo*  NaviList;	
}V3D_NavigatorList;

typedef struct V3D_ButtonInfo
{
	CHAR			Name[MAX_NAMESTR_LENGTH];    // ����

	//int             UpMaterialIndex;     // ��굯��ʱ��������״̬ʱ�õ��Ĳ�������
	//int             DownMaterialIndex;   // ��갴��ʱʹ�õĲ�������
	//int             OverMaterialIndex;   // ����ڰ�ť��ʱʹ�õĲ�������
	CHAR             UpImageName[MAX_NAMESTR_LENGTH];     // ��굯��ʱ��������״̬ʱ�õ���ͼƬ����
	CHAR             DownImageName[MAX_NAMESTR_LENGTH];   // ��갴��ʱʹ�õ�ͼƬ����
	CHAR             OverImageName[MAX_NAMESTR_LENGTH];   // ����ڰ�ť��ʱʹ�õ�ͼƬ����

	CHAR            CallbackFunName[MAX_NAMESTR_LENGTH];  // ��ť�Ļص���������

	BOOL            bRelativeSize; // �Ƿ�ʹ����Գߴ�
	BOOL            bRelativePos;  // �Ƿ�ʹ�����λ��
	BOOL            bVisible;      // �Ƿ�ɼ�

	FLOAT           Opacity;       // ͸����

	INT             Origin;  // ����ʱ�Ĳο�ԭ�㣬VGSOFFSETORIGINֵ 
	FLOAT			Width;   // Ŀǰ�Ŀ��, ���ʹ����Դ�С�����ﱣ��ٷֱ�
	FLOAT			Height;  // Ŀǰ�ĸ߶�, ���ʹ����Դ�С�����ﱣ��ٷֱ�
	FLOAT           Left;    // �����꣬������ӿ����Ͻ� , ���ʹ�����λ�ã����ﱣ��ٷֱ�
	FLOAT           Top;     // �����꣬������ӿ����Ͻ� , ���ʹ�����λ�ã����ﱣ��ٷֱ� 

	int             ZOrder;  // ���µĲ��˳��Խ��Խ����

}V3D_ButtonInfo;

// V3D�б����button����
typedef struct V3D_ButtonList
{
	UINT				count;
	V3D_ButtonInfo*		ButtonList;	
}V3D_ButtonList;

// V3D�б����ǰ��ͼ����
typedef struct V3D_BackdropInfo   
{
	CHAR			Name[MAX_NAMESTR_LENGTH];  // ����

	CHAR            ImgFile[MAX_NAMESTR_LENGTH];  // ����ͼƬ����


	BOOL            bVisible;      // �Ƿ�ɼ�

	FLOAT			Width;   // �ٷֱ� 
	FLOAT			Height;  // 
	FLOAT           Left;    // �ٷֱ�
	FLOAT           Top;     // 

}V3D_BackdropInfo;

// V3D�б���ı���ͼ����
typedef struct V3D_BackdropList
{
	UINT					count;
	V3D_BackdropInfo*		BackdropList;	
}V3D_BackdropList;



/*// node�����Ŷ��������
enum NODEOBJECT_TYPE
{
	NBT_NONE = 0,
	NBT_CAMERA,
	NBT_LIGHT,
	NBT_ENTITY
};

// node�����ŵĶ���
typedef struct NodeObject
{
	NODEOBJECT_TYPE	type;   // ��������
	UINT            Index;  // �����ڸ��Դ洢�����е�����
}NodeObject;

typedef struct SceneGraphNode
{
	int            ParentIdx;         // ���ڵ��ڽڵ�����е����� 
	float          PosX;              // positon
	float          PosY;
	float          PosZ;
	float          ScaleX;            // scale
	float          ScaleY;
	float          ScaleZ;
	float          OrientW;           // rotation
	float          OrientX;
	float          OrientY;
	float          OrientZ;
	std::vector<NodeObject>   vObject;     // �ڵ��¸��ŵĶ���
}SceneGraphNode;

// used to fast write and read v3d file
typedef struct NodeSaveParam
{
	int            ParentIdx;
	float          PosX;
	float          PosY;
	float          PosZ;
	float          ScaleX;
	float          ScaleY;
	float          ScaleZ;
	float          OrientW;
	float          OrientX;
	float          OrientY;
	float          OrientZ;
}NodeSaveParam;
*/


// ʵʱ�������
typedef struct VGS_RTF_PARAM
{
	int width, height;
	DWORD refectionModel;          // ����ģ��ָ��
	std::vector<DWORD> vrefectedModels; // ������ģ��ָ��
}VGS_RTF_PARAM;


//////////////////// end of �������ݽṹ��Ϊ�����д��V3Dʱ��Ч�ʶ������ ///////////////////////////////////


//////////////////////////////////// End of V3D //////////////////////////////////////////

#pragma pack(pop)
